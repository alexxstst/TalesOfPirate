//

#include "stdafx.h"
#include "lwExpObj.h"
#include "lwErrorCode.h"
#include "lwTreeNode.h"
#include "lwAnimKeySetPRS.h"
#include "lwIUtil.h"

LW_BEGIN
	// ============================================================================
	// Замки размеров структур, участвующих в fread/fwrite целиком.
	// ============================================================================
	// Файлы моделей/анимаций (`.lgo`, `.lmo`, `.lab`, `.lam`) пишутся/читаются
	// через `fread(&x, sizeof(Type), count, fp)`, т.е. размер структуры жёстко
	// зашит в бинарный формат. Если `sizeof(Type)` меняется — например, из-за
	// добавления поля-указателя (его размер 4 на x86 и 8 на x64), либо смены
	// порядка полей — смещения в файле съезжают, в поля типа `index_num`
	// попадает мусор, и клиент падает на `LW_NEW(DWORD[info->index_num])`.
	// Static_assert'ы ниже ловят это на этапе компиляции. Если какая-то из проверок
	// упала — ЗАПРЕЩЕНО "подправлять" ожидаемое число; нужно вернуть размер
	// структуры к прежнему, иначе существующие файлы перестанут читаться.
	// При намеренной смене формата необходимо также повысить version и добавить
	// legacy-путь разбора.
	// ============================================================================

	// Примитивы (float/DWORD-массивы, без указателей) — safety net.
	static_assert(sizeof(lwVector2) == 8, "lwVector2 layout changed — breaks .lgo/.lmo binary format");
	static_assert(sizeof(lwVector3) == 12, "lwVector3 layout changed — breaks .lgo/.lmo binary format");
	static_assert(sizeof(lwQuaternion) == 16, "lwQuaternion layout changed — breaks .lab binary format");
	static_assert(sizeof(lwMatrix43) == 48, "lwMatrix43 layout changed — breaks .lab binary format");
	static_assert(sizeof(lwMatrix44) == 64, "lwMatrix44 layout changed — breaks .lgo/.lab binary format");
	static_assert(sizeof(lwColorValue4b) == 4, "lwColorValue4b layout changed");
	static_assert(sizeof(lwColorValue4f) == 16, "lwColorValue4f layout changed");
	static_assert(sizeof(lwPlane) == 16, "lwPlane layout changed");
	static_assert(sizeof(lwBox) == 24, "lwBox layout changed");
	static_assert(sizeof(lwSphere) == 16, "lwSphere layout changed");
	static_assert(sizeof(lwMaterial) == 68, "lwMaterial layout changed");
	static_assert(sizeof(lwRenderStateAtom) == 12, "lwRenderStateAtom layout changed");
	static_assert(sizeof(lwRenderStateValue) == 8, "lwRenderStateValue layout changed");
	static_assert(sizeof(lwSubsetInfo) == 16, "lwSubsetInfo layout changed — breaks mesh subset block");
	static_assert(sizeof(lwBlendInfo) == 20, "lwBlendInfo layout changed — breaks bone-skin block");

	// Render-state template-наборы (используются в заголовках мешей/материалов).
	// Внутри только массив `rsv_seq[SET_SIZE][SEQUENCE_SIZE]` из `lwRenderStateValue`
	// (8 байт); `SET_SIZE` и `SEQUENCE_SIZE` — enum'ы, места не занимают.
	static_assert(sizeof(lwRenderStateSetMesh2) == 128,
				  "lwRenderStateSetMesh2 layout changed — breaks legacy mesh header");
	static_assert(sizeof(lwRenderStateSetMtl2) == 128,
				  "lwRenderStateSetMtl2 layout changed — breaks legacy mtl header");
	static_assert(sizeof(lwTextureStageStateSetTex2) == 128,
				  "lwTextureStageStateSetTex2 layout changed — breaks legacy tex block");

	// Заголовки мешей — читаются `fread(&info->header, sizeof(...), 1, fp)`.
	static_assert(sizeof(lwMeshInfoHeader) == 128,
				  "lwMeshInfoHeader layout changed — breaks .lgo/.lmo (version >= 1.0.0.4)");
	static_assert(sizeof(lwMeshInfo_0003::lwMeshInfoHeader) == 120,
				  "lwMeshInfo_0003::lwMeshInfoHeader layout changed — breaks legacy (version 1.0.0.3)");
	static_assert(sizeof(lwMeshInfo_0000::lwMeshInfoHeader) == 152,
				  "lwMeshInfo_0000::lwMeshInfoHeader layout changed — breaks legacy (MESH_VERSION0000)");

	// Текстуры/материалы (бывший баг: `void* data` в lwTexInfo давал разные
	// размеры на x86 и x64 → заменено на `DWORD _reserved_data`).
	static_assert(sizeof(lwTexInfo) == 208, "lwTexInfo layout changed — breaks .lgo/.lmo material block");
	static_assert(sizeof(lwTexInfo_0001) == 240,
				  "lwTexInfo_0001 layout changed — breaks legacy material (MTLTEX_VERSION0001)");
	static_assert(sizeof(lwTexInfo_0000) == 208,
				  "lwTexInfo_0000 layout changed — breaks legacy material (MTLTEX_VERSION0000)");
	static_assert(sizeof(lwMtlTexInfo) == 1004, "lwMtlTexInfo layout changed — breaks .lgo/.lmo material block");
	static_assert(sizeof(lwMtlTexInfo_0001) == 1036, "lwMtlTexInfo_0001 layout changed — breaks legacy material");
	static_assert(sizeof(lwMtlTexInfo_0000) == 1028, "lwMtlTexInfo_0000 layout changed — breaks legacy material");

	// Вспомогательные структуры геометрии.
	static_assert(sizeof(lwGeomObjInfoHeader) == 116, "lwGeomObjInfoHeader layout changed — breaks .lgo header");
	static_assert(sizeof(lwHelperDummyInfo) == 140, "lwHelperDummyInfo layout changed — breaks helper block");
	static_assert(sizeof(lwHelperBoxInfo) == 132, "lwHelperBoxInfo layout changed — breaks helper block");
	static_assert(sizeof(lwHelperMeshFaceInfo) == 52, "lwHelperMeshFaceInfo layout changed — breaks helper mesh block");
	static_assert(sizeof(lwBoundingBoxInfo) == 92, "lwBoundingBoxInfo layout changed — breaks helper bbox block");
	static_assert(sizeof(lwBoundingSphereInfo) == 84,
				  "lwBoundingSphereInfo layout changed — breaks helper bsphere block");

	// Кости и анимация.
	static_assert(sizeof(lwAnimDataBone::lwBoneInfoHeader) == 16,
				  "lwBoneInfoHeader layout changed — breaks .lab header");
	static_assert(sizeof(lwBoneBaseInfo) == 72, "lwBoneBaseInfo layout changed — breaks .lab base block");
	static_assert(sizeof(lwBoneDummyInfo) == 72, "lwBoneDummyInfo layout changed — breaks .lab dummy block");

	// Позы.
	static_assert(sizeof(lwPoseInfo) == 48, "lwPoseInfo layout changed — breaks .lpc pose block");

#define VERSION_BONESKIN            0x0001

	// lwMtlTexInfo io method
	LW_RESULT lwMtlTexInfo_Load(lwMtlTexInfo* info, FILE* fp, DWORD version) {
		if (version >= EXP_OBJ_VERSION_1_0_0_0) {
			fread(&info->opacity, sizeof(info->opacity), 1, fp);
			fread(&info->transp_type, sizeof(info->transp_type), 1, fp);
			fread(&info->mtl, sizeof(lwMaterial), 1, fp);
			fread(&info->rs_set[0], sizeof(info->rs_set), 1, fp);
			fread(&info->tex_seq[0], sizeof(info->tex_seq), 1, fp);
		}
		else if (version == MTLTEX_VERSION0002) {
			fread(&info->opacity, sizeof(info->opacity), 1, fp);
			fread(&info->transp_type, sizeof(info->transp_type), 1, fp);
			fread(&info->mtl, sizeof(lwMaterial), 1, fp);
			fread(&info->rs_set[0], sizeof(info->rs_set), 1, fp);
			fread(&info->tex_seq[0], sizeof(info->tex_seq), 1, fp);
		}
		else if (version == MTLTEX_VERSION0001) {
			lwRenderStateSetMtl2 rsm;
			lwTexInfo_0001 tex_info[LW_MAX_TEXTURESTAGE_NUM];

			fread(&info->opacity, sizeof(info->opacity), 1, fp);
			fread(&info->transp_type, sizeof(info->transp_type), 1, fp);
			fread(&info->mtl, sizeof(lwMaterial), 1, fp);
			fread(&rsm, sizeof(rsm), 1, fp);
			fread(&tex_info[0], sizeof(tex_info), 1, fp);

			lwRenderStateValue* rsv;
			for (DWORD i = 0; i < rsm.SEQUENCE_SIZE; i++) {
				rsv = &rsm.rsv_seq[0][i];
				if (rsv->state == LW_INVALID_INDEX)
					break;

				DWORD v;
				switch (rsv->state) {
				case D3DRS_ALPHAFUNC:
					v = D3DCMP_GREATER;
					break;
				case D3DRS_ALPHAREF:
					v = 129;
					break;
				default:
					v = rsv->value;
				}

				info->rs_set[i].state = rsv->state;
				info->rs_set[i].value0 = v;
				info->rs_set[i].value1 = v;
			}

			lwTexInfo* t;
			lwTexInfo_0001* p;
			for (DWORD i = 0; i < LW_MAX_TEXTURESTAGE_NUM; i++) {
				p = &tex_info[i];

				if (p->stage == LW_INVALID_INDEX)
					break;

				t = &info->tex_seq[i];

				t->level = p->level;
				t->usage = p->usage;
				t->pool = p->pool;
				t->type = p->type;

				t->width = p->width;
				t->height = p->height;

				t->stage = p->stage;
				t->format = p->format;
				t->colorkey = p->colorkey;
				t->colorkey_type = p->colorkey_type;
				t->byte_alignment_flag = p->byte_alignment_flag;
				_tcscpy(t->file_name, p->file_name);

				lwRenderStateValue* rsv;
				for (DWORD j = 0; j < p->tss_set.SEQUENCE_SIZE; j++) {
					rsv = &p->tss_set.rsv_seq[0][j];
					if (rsv->state == LW_INVALID_INDEX)
						break;

					t->tss_set[j].state = rsv->state;
					t->tss_set[j].value0 = rsv->value;
					t->tss_set[j].value1 = rsv->value;
				}
			}
		}
		else if (version == MTLTEX_VERSION0000) {
			lwRenderStateSetMtl2 rsm;

			fread(&info->mtl, sizeof(lwMaterial), 1, fp);
			fread(&rsm, sizeof(lwRenderStateSetMtl2), 1, fp);
			lwTexInfo_0000 tex_info[LW_MAX_TEXTURESTAGE_NUM];
			fread(&tex_info[0], sizeof(tex_info), 1, fp);

			lwRenderStateValue* rsv;
			for (DWORD i = 0; i < rsm.SEQUENCE_SIZE; i++) {
				rsv = &rsm.rsv_seq[0][i];
				if (rsv->state == LW_INVALID_INDEX)
					break;

				DWORD v;
				switch (rsv->state) {
				case D3DRS_ALPHAFUNC:
					v = D3DCMP_GREATER;
					break;
				case D3DRS_ALPHAREF:
					v = 129;
					break;
				default:
					v = rsv->value;
				}

				info->rs_set[i].state = rsv->state;
				info->rs_set[i].value0 = v;
				info->rs_set[i].value1 = v;
			}

			lwTexInfo* t;
			lwTexInfo_0000* p;
			for (DWORD i = 0; i < LW_MAX_TEXTURESTAGE_NUM; i++) {
				p = &tex_info[i];

				if (p->stage == LW_INVALID_INDEX)
					break;

				t = &info->tex_seq[i];
				t->level = D3DX_DEFAULT;
				t->usage = 0;
				t->pool = D3DPOOL_DEFAULT;
				t->type = TEX_TYPE_FILE;

				t->stage = p->stage;
				t->format = p->format;
				t->colorkey = p->colorkey;
				t->colorkey_type = p->colorkey_type;
				t->byte_alignment_flag = 0;
				_tcscpy(t->file_name, p->file_name);

				lwRenderStateValue* rsv;
				for (DWORD j = 0; j < p->tss_set.SEQUENCE_SIZE; j++) {
					rsv = &p->tss_set.rsv_seq[0][j];
					if (rsv->state == LW_INVALID_INDEX)
						break;

					t->tss_set[j].state = rsv->state;
					t->tss_set[j].value0 = rsv->value;
					t->tss_set[j].value1 = rsv->value;
				}
			}
			// Legacy данные мог указывать 16-bit форматы. Upgrade до A8R8G8B8
			// (A1R5G5B5 / A4R4G4B4 → 8-bit альфа без потерь, память ×2).
			if (info->tex_seq[0].format == D3DFMT_A4R4G4B4 ||
				info->tex_seq[0].format == D3DFMT_A1R5G5B5) {
				info->tex_seq[0].format = D3DFMT_A8R8G8B8;
			}
		}
		else {
			MessageBox(NULL, "invalid file version", "error", MB_OK);
			return LW_RET_FAILED;
		}

		// mipmap level = 3
		{
			info->tex_seq[0].pool = D3DPOOL_MANAGED;
			info->tex_seq[0].level = D3DX_DEFAULT;
		}

		BOOL transp_flag = 0;
		lwRenderStateAtom* rsa;
		DWORD i = 0;
		for (; i < LW_MTL_RS_NUM; i++) {
			rsa = &info->rs_set[i];

			if (rsa->state == LW_INVALID_INDEX)
				break;

			if (rsa->state == D3DRS_DESTBLEND && (rsa->value0 == D3DBLEND_ONE || rsa->value0 == D3DBLEND_INVSRCCOLOR)) {
				transp_flag = 1;
			}
			if (rsa->state == D3DRS_LIGHTING && rsa->value0 == FALSE) {
				transp_flag += 1;
			}
		}

		if (transp_flag == 1 && i < (LW_MTL_RS_NUM - 1)) {
			RSA_VALUE(&info->rs_set[i], D3DRS_LIGHTING, 0);
		}

		// warning:
		if (info->transp_type == 1)
			info->transp_type = MTLTEX_TRANSP_ADDITIVE;
		else if (info->transp_type == 2)
			info->transp_type = MTLTEX_TRANSP_SUBTRACTIVE;
		// end

		return LW_RET_OK;
	}

	LW_RESULT lwMtlTexInfo_Save(lwMtlTexInfo* info, FILE* fp, DWORD version) {
		fwrite(&info->opacity, sizeof(info->opacity), 1, fp);
		fwrite(&info->transp_type, sizeof(info->transp_type), 1, fp);
		fwrite(&info->mtl, sizeof(lwMaterial), 1, fp);
		fwrite(&info->rs_set[0], sizeof(info->rs_set), 1, fp);
		fwrite(&info->tex_seq[0], sizeof(info->tex_seq), 1, fp);

		return LW_RET_OK;
	}

	LW_RESULT lwLoadMtlTexInfo(lwMtlTexInfo** out_buf, DWORD* out_num, FILE* fp, DWORD version) {
		lwMtlTexInfo* buf = 0;
		DWORD num = 0;

		if (version == EXP_OBJ_VERSION_0_0_0_0) {
			DWORD old_version;
			fread(&old_version, sizeof(old_version), 1, fp);
			version = old_version;
		}

		fread(&num, sizeof(num), 1, fp);
		buf = LW_NEW(lwMtlTexInfo[num]);

		for (DWORD i = 0; i < num; i++) {
			lwMtlTexInfo_Load(&buf[i], fp, version);
		}

		*out_buf = buf;
		*out_num = num;

		return LW_RET_OK;
	}

	LW_RESULT lwSaveMtlTexInfo(FILE* fp, const lwMtlTexInfo* info, DWORD num) {
		fwrite(&num, sizeof(DWORD), 1, fp);

		for (DWORD i = 0; i < num; i++) {
			lwMtlTexInfo_Save((lwMtlTexInfo*)&info[i], fp, MTLTEX_VERSION);
		}

		return LW_RET_OK;
	}

	DWORD lwMtlTexInfo_GetDataSize(lwMtlTexInfo* info) {
		return sizeof(info->opacity) + sizeof(info->transp_type)
			+ sizeof(info->mtl) + sizeof(info->rs_set)
			+ sizeof(info->tex_seq);
	}


	// lwAnimDataTexUV
	LW_STD_IMPLEMENTATION(lwAnimDataTexUV)

	LW_RESULT lwAnimDataTexUV::Load(FILE* fp, DWORD version) {
		fread(&_frame_num, sizeof(DWORD), 1, fp);

		_mat_seq = LW_NEW(lwMatrix44[_frame_num]);
		fread(&_mat_seq[0], sizeof(lwMatrix44), _frame_num, fp);

		// pose_ctrl

		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataTexUV::Save(FILE* fp) const {
		fwrite(&_frame_num, sizeof(DWORD), 1, fp);
		fwrite(&_mat_seq[0], sizeof(lwMatrix44), _frame_num, fp);

		// pose_ctrl

		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataTexUV::Load(std::string_view file) {
		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataTexUV::Save(std::string_view file) const {
		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataTexUV::Copy(const lwAnimDataTexUV* src) {
		_frame_num = src->_frame_num;
		if (_frame_num > 0) {
			_mat_seq = LW_NEW(lwMatrix44[_frame_num]);
			memcpy(&_mat_seq[0], &src->_mat_seq[0], sizeof(lwMatrix44) * _frame_num);
		}


		return LW_RET_OK;
	}

	DWORD lwAnimDataTexUV::GetDataSize() const {
		DWORD size = 0;

		if (_frame_num > 0) {
			size += sizeof(_frame_num);
			size += sizeof(lwMatrix44) * _frame_num;
		}

		return size;
	}

	LW_RESULT lwAnimDataTexUV::GetValue(lwMatrix44* mat, float frame) {
		if (frame < 0 || frame >= _frame_num)
			return ERR_INVALID_PARAM;


		// use linear interpolate
		int min_f = lwFloatRoundDec(frame);
		int max_f = min_f + 1;

		if (max_f == _frame_num) {
			max_f = _frame_num - 1;
		}

		float ep = lwFloatDecimal(frame);

		lwMatrix44* mat_0 = &_mat_seq[min_f];
		lwMatrix44* mat_1 = &_mat_seq[max_f];

		lwMat44Slerp((lwMatrix44*)mat, (lwMatrix44*)mat_0, (lwMatrix44*)mat_1, ep);

		return LW_RET_OK;

		//    return 0;


		//    // DirectXUVUV
		//    //


		//return 1;
	}

	// lwAnimDataTexImg
	LW_STD_IMPLEMENTATION(lwAnimDataTexImg);

	LW_RESULT lwAnimDataTexImg::Load(FILE* fp, DWORD version) {
		if (version == EXP_OBJ_VERSION_0_0_0_0) {
			MessageBox(NULL, "old version file, need re-export it", "warning", MB_OK);
			return LW_RET_FAILED;
		}
		else {
			fread(&_data_num, sizeof(_data_num), 1, fp);
			_data_seq = LW_NEW(lwTexInfo[_data_num]);
			fread(_data_seq, sizeof(lwTexInfo), _data_num, fp);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataTexImg::Save(FILE* fp) const {
		fwrite(&_data_num, sizeof(_data_num), 1, fp);
		fwrite(_data_seq, sizeof(lwTexInfo), _data_num, fp);
		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataTexImg::Load(std::string_view file) {
		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataTexImg::Save(std::string_view file) const {
		return LW_RET_OK;
	}


	DWORD lwAnimDataTexImg::GetDataSize() const {
		DWORD size = 0;

		size += sizeof(lwTexInfo) * _data_num;

		if (size > 0) {
			size += sizeof(_data_num);
		}

		return size;
	}

	LW_RESULT lwAnimDataTexImg::Copy(const lwAnimDataTexImg* src) {
		_data_num = src->_data_num;
		_data_seq = LW_NEW(lwTexInfo[_data_num]);
		memcpy(_data_seq, src->_data_seq, sizeof(lwTexInfo) * _data_num);
		_tcscpy(_tex_path, src->_tex_path);

		return LW_RET_OK;
	}

	// lwAnimDataMtlOpacity
	LW_STD_IMPLEMENTATION(lwAnimDataMtlOpacity)

	LW_RESULT lwAnimDataMtlOpacity::Clone(lwIAnimDataMtlOpacity** obj) {
		LW_RESULT ret = LW_RET_FAILED;

		lwAnimDataMtlOpacity* o = LW_NEW(lwAnimDataMtlOpacity);

		if (_aks_ctrl) {
			lwIAnimKeySetFloat* aksf;

			if (LW_RESULT r = _aks_ctrl->Clone(&aksf); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] AnimKeySetFloat::Clone failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			o->SetAnimKeySet(aksf);
		}

		*obj = o;
		o = 0;

		ret = LW_RET_OK;
	__ret:
		LW_IF_DELETE(o);

		return ret;
	}

	LW_RESULT lwAnimDataMtlOpacity::Load(FILE* fp, DWORD version) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD num;
		lwKeyFloat* seq;

		fread(&num, sizeof(num), 1, fp);
		assert(num > 0);

		seq = LW_NEW(lwKeyFloat[num]);

		fread(seq, sizeof(lwKeyFloat), num, fp);

		_aks_ctrl = LW_NEW(lwAnimKeySetFloat);

		if (LW_RESULT r = _aks_ctrl->SetKeySequence(seq, num); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] SetKeySequence failed: num={}, ret={}",
						 __FUNCTION__, static_cast<long long>(num),
						 static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		LW_IF_DELETE_A(seq);
		return ret;
	}

	LW_RESULT lwAnimDataMtlOpacity::Save(FILE* fp) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_aks_ctrl == 0)
			goto __ret;
		{
			DWORD num = _aks_ctrl->GetKeyNum();
			lwKeyFloat* seq = _aks_ctrl->GetKeySequence();

			if (num == 0 || seq == 0)
				goto __ret;

			fwrite(&num, sizeof(num), 1, fp);
			fwrite(&seq[0], sizeof(lwKeyFloat), num, fp);
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	DWORD lwAnimDataMtlOpacity::GetDataSize() {
		if (_aks_ctrl == 0)
			return 0;

		return (sizeof(lwKeyFloat) * _aks_ctrl->GetKeyNum() + sizeof(DWORD));
	}

	// lwAnimDataBone
	LW_STD_IMPLEMENTATION(lwAnimDataBone)

	lwAnimDataBone::lwAnimDataBone()
		: _key_type(BONE_KEY_TYPE_MAT43), _base_seq(0), _invmat_seq(0), _key_seq(0), _dummy_seq(0),
		  _bone_num(0), _frame_num(0), _dummy_num(0) /*, _subset_type(LW_INVALID_INDEX)*/
	{
	}

	lwAnimDataBone::~lwAnimDataBone() {
		Destroy();
	}

	LW_RESULT lwAnimDataBone::Destroy() {
		LW_SAFE_DELETE_A(_base_seq);
		LW_SAFE_DELETE_A(_invmat_seq);
		LW_SAFE_DELETE_A(_key_seq);
		LW_SAFE_DELETE_A(_dummy_seq);
		_bone_num = 0;
		_frame_num = 0;
		_dummy_num = 0;
		_key_type = BONE_KEY_TYPE_MAT43;

		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataBone::Load(FILE* fp, DWORD version) {
		if (version == EXP_OBJ_VERSION_0_0_0_0) {
			DWORD old_version;
			fread(&old_version, sizeof(old_version), 1, fp);
			int x = 0;
		}

		// need destroy first
		if (_base_seq)
			return LW_RET_FAILED;

		fread(&_header, sizeof(lwBoneInfoHeader), 1, fp);

		_base_seq = LW_NEW(lwBoneBaseInfo[_bone_num]);
		_key_seq = LW_NEW(lwBoneKeyInfo[_bone_num]);
		_invmat_seq = LW_NEW(lwMatrix44[_bone_num]);
		_dummy_seq = LW_NEW(lwBoneDummyInfo[_dummy_num]);

		fread(_base_seq, sizeof(lwBoneBaseInfo), _bone_num, fp);
		fread(_invmat_seq, sizeof(lwMatrix44), _bone_num, fp);
		fread(_dummy_seq, sizeof(lwBoneDummyInfo), _dummy_num, fp);

		DWORD i, j;
		lwBoneKeyInfo* key;

		switch (_key_type) {
		case BONE_KEY_TYPE_MAT43:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				key->mat43_seq = LW_NEW(lwMatrix43[_frame_num]);
				fread(&key->mat43_seq[0], sizeof(lwMatrix43), _frame_num, fp);
			}
			break;
		case BONE_KEY_TYPE_MAT44:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				key->mat44_seq = LW_NEW(lwMatrix44[_frame_num]);
				fread(&key->mat44_seq[0], sizeof(lwMatrix44), _frame_num, fp);
			}
			break;
		case BONE_KEY_TYPE_QUAT:
			if (version >= EXP_OBJ_VERSION_1_0_0_3) {
				for (i = 0; i < _bone_num; i++) {
					key = &_key_seq[i];

					key->pos_seq = LW_NEW(lwVector3[_frame_num]);
					fread(&key->pos_seq[0], sizeof(lwVector3), _frame_num, fp);

					key->quat_seq = LW_NEW(lwQuaternion[_frame_num]);
					fread(&key->quat_seq[0], sizeof(lwQuaternion), _frame_num, fp);
				}
			}
			else // old version < EXP_OBJ_VERSION_1_0_0_3
			{
				for (i = 0; i < _bone_num; i++) {
					key = &_key_seq[i];

					DWORD pos_num = (_base_seq[i].parent_id == LW_INVALID_INDEX) ? _frame_num : 1;

					key->pos_seq = LW_NEW(lwVector3[_frame_num]);
					fread(&key->pos_seq[0], sizeof(lwVector3), pos_num, fp);

					if (pos_num == 1) {
						for (j = 1; j < _frame_num; j++) {
							key->pos_seq[j] = key->pos_seq[0];
						}
					}

					key->quat_seq = LW_NEW(lwQuaternion[_frame_num]);
					fread(&key->quat_seq[0], sizeof(lwQuaternion), _frame_num, fp);
				}
			}
			break;
		default:
			assert(0);
		}

		// pose_ctrl

		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataBone::Save(FILE* fp) const {
		fwrite(&_header, sizeof(lwBoneInfoHeader), 1, fp);

		fwrite(_base_seq, sizeof(lwBoneBaseInfo), _bone_num, fp);
		fwrite(_invmat_seq, sizeof(lwMatrix44), _bone_num, fp);
		fwrite(_dummy_seq, sizeof(lwBoneDummyInfo), _dummy_num, fp);

		DWORD i;
		lwBoneKeyInfo* key;


		switch (_key_type) {
		case BONE_KEY_TYPE_MAT43:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				fwrite(&key->mat43_seq[0], sizeof(lwMatrix43), _frame_num, fp);
			}
			break;
		case BONE_KEY_TYPE_MAT44:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				fwrite(&key->mat44_seq[0], sizeof(lwMatrix44), _frame_num, fp);
			}
			break;
		case BONE_KEY_TYPE_QUAT:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				fwrite(&key->pos_seq[0], sizeof(lwVector3), _frame_num, fp);
				fwrite(&key->quat_seq[0], sizeof(lwQuaternion), _frame_num, fp);
			}
			break;
		default:
			assert(0);
		}


		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataBone::Load(std::string_view file) {
		LW_RESULT ret = LW_RET_FAILED;

		FILE* fp = fopen(std::string{file}.c_str(), "rb");
		if (fp == NULL)
			goto __ret;

		DWORD version;
		fread(&version, sizeof(version), 1, fp);

		if (version < EXP_OBJ_VERSION_1_0_0_0) {
			goto __ret;
			version = EXP_OBJ_VERSION_0_0_0_0;
			const std::string buf = std::format("old animation file: {}, need re-export it", file);
			MessageBox(NULL, buf.c_str(), "warning", MB_OK);
		}

		if (LW_RESULT r = Load(fp, version); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] AnimDataBone::Load(fp) failed: file='{}', version={}, ret={}",
						 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
						 static_cast<long long>(version), static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;

	__ret:
		if (fp) {
			fclose(fp);
		}

		return ret;
	}

	LW_RESULT lwAnimDataBone::Save(std::string_view file) const {
		LW_RESULT ret = LW_RET_FAILED;

		FILE* fp = fopen(std::string{file}.c_str(), "wb");
		if (fp == NULL)
			return 0;

		DWORD version = EXP_OBJ_VERSION;
		fwrite(&version, sizeof(version), 1, fp);

		if (LW_RESULT r = Save(fp); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] AnimDataBone::Save(fp) failed: file='{}', ret={}",
						 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
						 static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;

	__ret:
		if (fp) {
			fclose(fp);
		}

		return ret;
	}

	LW_RESULT lwAnimDataBone::Copy(const lwAnimDataBone* src) {
		if (src->_key_type == BONE_KEY_TYPE_INVALID)
			return LW_RET_FAILED;

		_header = src->_header;

		_base_seq = LW_NEW(lwBoneBaseInfo[_bone_num]);
		_invmat_seq = LW_NEW(lwMatrix44[_bone_num]);
		_key_seq = LW_NEW(lwBoneKeyInfo[_bone_num]);
		_dummy_seq = LW_NEW(lwBoneDummyInfo[_dummy_num]);

		memcpy(&_base_seq[0], &src->_base_seq[0], sizeof(lwBoneBaseInfo) * _bone_num);
		memcpy(&_invmat_seq[0], &src->_invmat_seq[0], sizeof(lwMatrix44) * _bone_num);
		memcpy(&_dummy_seq[0], &src->_dummy_seq[0], sizeof(lwBoneDummyInfo) * _dummy_num);


		DWORD i;
		lwBoneKeyInfo* key;

		switch (_key_type) {
		case BONE_KEY_TYPE_MAT43:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				key->mat43_seq = LW_NEW(lwMatrix43[_frame_num]);
				memcpy(&key->mat43_seq[0], &src->_key_seq[i].mat43_seq[0], sizeof(lwMatrix43) * _frame_num);
			}
			break;
		case BONE_KEY_TYPE_MAT44:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				key->mat44_seq = LW_NEW(lwMatrix44[_frame_num]);
				memcpy(&key->mat44_seq[0], &src->_key_seq[i].mat44_seq[0], sizeof(lwMatrix44) * _frame_num);
			}
			break;
		case BONE_KEY_TYPE_QUAT:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				key->pos_seq = LW_NEW(lwVector3[_frame_num]);
				key->quat_seq = LW_NEW(lwQuaternion[_frame_num]);
				memcpy(&key->pos_seq[0], &src->_key_seq[i].pos_seq[0], sizeof(lwVector3) * _frame_num);
				memcpy(&key->quat_seq[0], &src->_key_seq[i].quat_seq[0], sizeof(lwQuaternion) * _frame_num);
			}
			break;
		default:
			assert(0);
		}


		return LW_RET_OK;
	}

	DWORD lwAnimDataBone::GetDataSize() const {
		DWORD size = 0;

		if (_bone_num > 0 && _frame_num > 0) {
			size += sizeof(lwBoneInfoHeader);
			size += sizeof(lwBoneBaseInfo) * _bone_num;
			size += sizeof(lwMatrix44) * _bone_num;
			size += sizeof(lwBoneDummyInfo) * _dummy_num;

			DWORD d = _bone_num * _frame_num;

			switch (_key_type) {
			case BONE_KEY_TYPE_MAT43:
				size += sizeof(lwMatrix43) * d;
				break;
			case BONE_KEY_TYPE_MAT44:
				size += sizeof(lwMatrix44) * d;
				break;
			case BONE_KEY_TYPE_QUAT:
				size += sizeof(lwQuaternion) * d;
				for (DWORD i = 0; i < _bone_num; i++) {
					if (_base_seq[i].parent_id == LW_INVALID_INDEX)
						size += sizeof(lwVector3) * _frame_num;
					else
						size += sizeof(lwVector3) * 1;
				}
				break;
			default:
				assert(0);
			}
		}

		return size;
	}

	LW_RESULT lwAnimDataBone::GetValue(lwMatrix44* mat, DWORD bone_id, float frame, DWORD start_frame,
									   DWORD end_frame) {
		LW_RESULT ret = LW_RET_FAILED;

		if (bone_id < 0 || bone_id >= _bone_num)
			goto __ret;

		// by lsh
		// frame > _frame_num ">="PLAY_LOOP
		// 0_data._frame_num - 1
		if (frame < 0 || frame > _frame_num)
			goto __ret;

		{
			lwBoneKeyInfo* key = &_key_seq[bone_id];

			DWORD d_frame = (DWORD)frame;

			if ((float)d_frame == frame) {
				if (d_frame >= _frame_num)
					goto __ret;

				switch (_key_type) {
				case BONE_KEY_TYPE_MAT43:
					lwConvertMat43ToMat44(mat, &key->mat43_seq[d_frame]);
					break;
				case BONE_KEY_TYPE_MAT44:
					*mat = key->mat44_seq[d_frame];
					break;
				case BONE_KEY_TYPE_QUAT:
					lwQuaternionToMatrix44(mat, &key->quat_seq[d_frame]);
					*(lwVector3*)&mat->_41 = key->pos_seq[d_frame];
					break;
				default:
					assert(0);
					goto __ret;
				}
			}
			else // use linear interpolate
			{
				DWORD min_f = lwFloatRoundDec(frame);
				DWORD max_f = min_f + 1;

				float t = lwFloatDecimal(frame);

				if (start_frame != LW_INVALID_INDEX && end_frame != LW_INVALID_INDEX) {
					if (max_f > end_frame) {
						max_f = start_frame;
					}
				}

				if (max_f == _frame_num) {
					// 0_data._frame_num - 1
					max_f = 0;
				}

				lwMatrix44 mat_0;
				lwMatrix44 mat_1;

				switch (_key_type) {
				case BONE_KEY_TYPE_MAT43:
					lwConvertMat43ToMat44(&mat_0, &key->mat43_seq[min_f]);
					lwConvertMat43ToMat44(&mat_1, &key->mat43_seq[max_f]);

					lwMatrix44Slerp(&mat_0, &mat_1, t, mat);
					break;
				case BONE_KEY_TYPE_MAT44:
					mat_0 = key->mat44_seq[min_f];
					mat_1 = key->mat44_seq[max_f];

					lwMatrix44Slerp(&mat_0, &mat_1, t, mat);
					break;
				case BONE_KEY_TYPE_QUAT:
					lwMatrix44Slerp(&key->pos_seq[min_f], &key->pos_seq[max_f], NULL, NULL, &key->quat_seq[min_f],
									&key->quat_seq[max_f], t, mat);
					break;
				default:
					assert(0);
					goto __ret;
				}
			}
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	// lwAnimDataMatrix
	LW_STD_IMPLEMENTATION(lwAnimDataMatrix)

	lwAnimDataMatrix::lwAnimDataMatrix()
		: _mat_seq(0), _frame_num(0) /*, _subset_type(LW_INVALID_INDEX)*/
	{
	}

	lwAnimDataMatrix::~lwAnimDataMatrix() {
		LW_SAFE_DELETE_A(_mat_seq);
	}

	LW_RESULT lwAnimDataMatrix::Load(FILE* fp, DWORD version) {
		fread(&_frame_num, sizeof(DWORD), 1, fp);

		_mat_seq = LW_NEW(lwMatrix43[_frame_num]);
		fread(&_mat_seq[0], sizeof(lwMatrix43), _frame_num, fp);

		// pose_ctrl

		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataMatrix::Save(FILE* fp) const {
		fwrite(&_frame_num, sizeof(DWORD), 1, fp);
		fwrite(&_mat_seq[0], sizeof(lwMatrix43), _frame_num, fp);

		// pose_ctrl

		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataMatrix::Load(std::string_view file) {
		assert(0);
		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataMatrix::Save(std::string_view file) const {
		assert(0);
		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataMatrix::Copy(const lwAnimDataMatrix* src) {
		_frame_num = src->_frame_num;

		if (_frame_num > 0) {
			_mat_seq = LW_NEW(lwMatrix43[_frame_num]);
			memcpy(&_mat_seq[0], &src->_mat_seq[0], sizeof(lwMatrix43) * _frame_num);
		}


		return LW_RET_OK;
	}

	DWORD lwAnimDataMatrix::GetDataSize() const {
		DWORD size = 0;

		if (_frame_num > 0) {
			size += sizeof(_frame_num);
			size += sizeof(lwMatrix43) * _frame_num;
		}

		return size;
	}

	LW_RESULT lwAnimDataMatrix::GetValue(lwMatrix44* mat, float frame) {
		if (frame < 0 || frame >= _frame_num)
			return ERR_INVALID_PARAM;


		// use linear interpolate
		int min_f = lwFloatRoundDec(frame);
		int max_f = min_f + 1;

		if (max_f == _frame_num) {
			max_f = _frame_num - 1;
		}

		float ep = lwFloatDecimal(frame);

		lwMatrix44 mat_0;
		lwMatrix44 mat_1;

		lwConvertMat43ToMat44(&mat_0, &_mat_seq[min_f]);
		lwConvertMat43ToMat44(&mat_1, &_mat_seq[max_f]);

		lwMat44Slerp(mat, &mat_0, &mat_1, ep);

		return LW_RET_OK;
	}

	// lwAnimKeySetPRS
	template <class T>
	LW_RESULT lwKeyDataSearch(DWORD* ret_min, DWORD* ret_max, DWORD key, T* data_seq, DWORD data_num) {
		DWORD low = 0;
		DWORD high = data_num - 1;
		DWORD k, l;

		while (high >= low) {
			k = (low + high) / 2;

			l = k + 1;

			if (l == data_num) {
				*ret_min = k;
				*ret_max = k;
				break;
			}
			else {
				if (key >= data_seq[k].key && key < data_seq[l].key) {
					*ret_min = k;
					*ret_max = l;
					break;
				}
			}

			if (key < data_seq[k].key) {
				high = k - 1;
			}
			else {
				low = k + 1;
			}
		}

		return (low <= high) ? LW_RET_OK : LW_INVALID_INDEX;
	}


	LW_RESULT lwAnimKeySetPRS::GetValue(lwMatrix44* mat, float frame) {
		DWORD f = (DWORD)frame;

		if (f < 0 || f >= frame_num)
			return ERR_INVALID_PARAM;


		DWORD key_pos[2];
		DWORD key_rot[2];
		DWORD key_sca[2];

		if (LW_RESULT r = lwKeyDataSearch<lwKeyDataVector3>(&key_pos[0], &key_pos[1], f, pos_seq, pos_num);
			LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwKeyDataSearch<Vector3>(pos) failed: f={}, pos_num={}, ret={}",
						 __FUNCTION__, static_cast<long long>(f),
						 static_cast<long long>(pos_num), static_cast<long long>(r));
			assert(0);
		}
		if (LW_RESULT r = lwKeyDataSearch<lwKeyDataQuaternion>(&key_rot[0], &key_rot[1], f, rot_seq, rot_num);
			LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwKeyDataSearch<Quaternion>(rot) failed: f={}, rot_num={}, ret={}",
						 __FUNCTION__, static_cast<long long>(f),
						 static_cast<long long>(rot_num), static_cast<long long>(r));
			assert(0);
		}
		if (LW_RESULT r = lwKeyDataSearch<lwKeyDataVector3>(&key_sca[0], &key_sca[1], f, sca_seq, sca_num);
			LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwKeyDataSearch<Vector3>(sca) failed: f={}, sca_num={}, ret={}",
						 __FUNCTION__, static_cast<long long>(f),
						 static_cast<long long>(sca_num), static_cast<long long>(r));
			assert(0);
		}

		lwVector3 pos;
		lwQuaternion quat;
		lwVector3 scale;
		float t;

		if (key_pos[0] == key_pos[1]) {
			pos = pos_seq[key_pos[0]].data;
		}
		else {
			lwKeyDataVector3* k0 = &pos_seq[key_pos[0]];
			lwKeyDataVector3* k1 = &pos_seq[key_pos[1]];

			t = (float)(f - k0->key) / (float)(k1->key - k0->key);

			lwVector3Slerp(&pos, &k0->data, &k1->data, t);
		}

		if (key_rot[0] == key_rot[1]) {
			quat = rot_seq[key_rot[0]].data;
		}
		else {
			lwKeyDataQuaternion* k0 = &rot_seq[key_rot[0]];
			lwKeyDataQuaternion* k1 = &rot_seq[key_rot[1]];

			t = (float)(f - k0->key) / (float)(k1->key - k0->key);

			lwQuaternionSlerp(&quat, &k0->data, &k1->data, t);
		}

		if (key_sca[0] == key_sca[1]) {
			scale = sca_seq[key_sca[0]].data;
		}
		else {
			lwKeyDataVector3* k0 = &sca_seq[key_sca[0]];
			lwKeyDataVector3* k1 = &sca_seq[key_sca[1]];

			t = (float)(f - k0->key) / (float)(k1->key - k0->key);

			lwVector3Slerp(&scale, &k0->data, &k1->data, t);
		}

		lwQuaternionToMatrix44(mat, &quat);

		const auto matrix = lwMatrix44Translate(pos);
		lwMatrix44Multiply(mat, mat, &matrix);

		mat->_11 *= scale.x;
		mat->_12 *= scale.x;
		mat->_13 *= scale.x;
		mat->_14 *= scale.x;

		mat->_21 *= scale.y;
		mat->_22 *= scale.y;
		mat->_23 *= scale.y;
		mat->_24 *= scale.y;

		mat->_31 *= scale.z;
		mat->_32 *= scale.z;
		mat->_33 *= scale.z;
		mat->_34 *= scale.z;


		return LW_RET_OK;
	}

	// lwMeshInfo

	LW_RESULT lwMeshInfo_Copy(lwMeshInfo* dst, const lwMeshInfo* src) {
		dst->header = src->header;

		if (dst->vertex_num > 0) {
			dst->vertex_seq = LW_NEW(lwVector3[dst->vertex_num]);
			memcpy(dst->vertex_seq, src->vertex_seq, sizeof(lwVector3) * dst->vertex_num);
		}

		if (dst->fvf & D3DFVF_NORMAL) {
			dst->normal_seq = LW_NEW(lwVector3[dst->vertex_num]);
			memcpy(dst->normal_seq, src->normal_seq, sizeof(lwVector3) * dst->vertex_num);
		}

		if (dst->fvf & D3DFVF_TEX1) {
			dst->texcoord0_seq = LW_NEW(lwVector2[dst->vertex_num]);
			memcpy(dst->texcoord0_seq, src->texcoord0_seq, sizeof(lwVector2) * dst->vertex_num);

			//// added by clp
		}
		else if (dst->fvf & D3DFVF_TEX2) {
			dst->texcoord0_seq = LW_NEW(lwVector2[dst->vertex_num]);
			dst->texcoord1_seq = LW_NEW(lwVector2[dst->vertex_num]);
			memcpy(dst->texcoord0_seq, src->texcoord0_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord1_seq, src->texcoord1_seq, sizeof(lwVector2) * dst->vertex_num);
		}
		else if (dst->fvf & D3DFVF_TEX3) {
			dst->texcoord0_seq = LW_NEW(lwVector2[dst->vertex_num]);
			dst->texcoord1_seq = LW_NEW(lwVector2[dst->vertex_num]);
			dst->texcoord2_seq = LW_NEW(lwVector2[dst->vertex_num]);
			memcpy(dst->texcoord0_seq, src->texcoord0_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord1_seq, src->texcoord1_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord2_seq, src->texcoord2_seq, sizeof(lwVector2) * dst->vertex_num);
		}
		else if (dst->fvf & D3DFVF_TEX4) {
			dst->texcoord0_seq = LW_NEW(lwVector2[dst->vertex_num]);
			dst->texcoord1_seq = LW_NEW(lwVector2[dst->vertex_num]);
			dst->texcoord2_seq = LW_NEW(lwVector2[dst->vertex_num]);
			dst->texcoord3_seq = LW_NEW(lwVector2[dst->vertex_num]);
			memcpy(dst->texcoord0_seq, src->texcoord0_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord1_seq, src->texcoord1_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord2_seq, src->texcoord2_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord3_seq, src->texcoord3_seq, sizeof(lwVector2) * dst->vertex_num);
		}

		if (dst->fvf & D3DFVF_DIFFUSE) {
			dst->vercol_seq = LW_NEW(DWORD[dst->vertex_num]);
			memcpy(dst->vercol_seq, src->vercol_seq, sizeof(DWORD) * dst->vertex_num);
		}

		if (dst->bone_index_num > 0) {
			dst->blend_seq = LW_NEW(lwBlendInfo[dst->vertex_num]);
			dst->bone_index_seq = LW_NEW(DWORD[dst->bone_index_num]);
			memcpy(dst->blend_seq, src->blend_seq, sizeof(lwBlendInfo) * dst->vertex_num);
			memcpy(dst->bone_index_seq, src->bone_index_seq, sizeof(DWORD) * dst->bone_index_num);
		}

		if (dst->index_num > 0) {
			dst->index_seq = LW_NEW(DWORD[dst->index_num]);
			memcpy(dst->index_seq, src->index_seq, sizeof(DWORD) * dst->index_num);
		}

		if (dst->subset_num > 0) {
			dst->subset_seq = LW_NEW(lwSubsetInfo[dst->subset_num]);
			memcpy(dst->subset_seq, src->subset_seq, sizeof(lwSubsetInfo) * dst->subset_num);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwMeshInfo_Load(lwMeshInfo* info, FILE* fp, DWORD version) {
		if (version == EXP_OBJ_VERSION_0_0_0_0) {
			DWORD old_version;
			fread(&old_version, sizeof(old_version), 1, fp);
			version = old_version;
		}

		// header
		if (version >= EXP_OBJ_VERSION_1_0_0_4) {
			fread(&info->header, sizeof(info->header), 1, fp);
		}
		else if (version >= EXP_OBJ_VERSION_1_0_0_3) {
			lwMeshInfo_0003::lwMeshInfoHeader header;
			fread(&header, sizeof(header), 1, fp);
			info->fvf = header.fvf;
			info->pt_type = header.pt_type;
			info->vertex_num = header.vertex_num;
			info->index_num = header.index_num;
			info->subset_num = header.subset_num;
			info->bone_index_num = header.bone_index_num;
			info->bone_infl_factor = info->bone_index_num > 0 ? 2 : 0;
			info->vertex_element_num = 0;
		}
		else if ((version >= EXP_OBJ_VERSION_1_0_0_0) || (version == MESH_VERSION0001)) {
			lwMeshInfo_0003::lwMeshInfoHeader header;
			fread(&header, sizeof(header), 1, fp);
			info->fvf = header.fvf;
			info->pt_type = header.pt_type;
			info->vertex_num = header.vertex_num;
			info->index_num = header.index_num;
			info->subset_num = header.subset_num;
			info->bone_index_num = header.bone_index_num;
			info->bone_infl_factor = info->bone_index_num > 0 ? 2 : 0;
			info->vertex_element_num = 0;
		}
		else if (version == MESH_VERSION0000) {
			lwMeshInfo_0000::lwMeshInfoHeader header;
			fread(&header, sizeof(header), 1, fp);
			info->header.fvf = header.fvf;
			info->header.pt_type = header.pt_type;
			info->header.vertex_num = header.vertex_num;
			info->header.index_num = header.index_num;
			info->header.subset_num = header.subset_num;
			info->header.bone_index_num = header.bone_index_num;

			lwRenderStateValue* rsv;
			for (DWORD j = 0; j < header.rs_set.SEQUENCE_SIZE; j++) {
				rsv = &header.rs_set.rsv_seq[0][j];
				if (rsv->state == LW_INVALID_INDEX)
					break;

				DWORD v;
				switch (rsv->state) {
				case D3DRS_AMBIENTMATERIALSOURCE:
					v = D3DMCS_COLOR2;
					break;
				default:
					v = rsv->value;
				}

				info->header.rs_set[j].state = rsv->state;
				info->header.rs_set[j].value0 = v;
				info->header.rs_set[j].value1 = v;
			}
		}
		else {
			MessageBox(NULL, "invalid version", "error", MB_OK);
		}


		if (version >= EXP_OBJ_VERSION_1_0_0_4) {
			if (info->vertex_element_num > 0) {
				info->vertex_element_seq = LW_NEW(D3DVERTEXELEMENTX[info->vertex_element_num]);
				fread(&info->vertex_element_seq[0], sizeof(D3DVERTEXELEMENTX), info->vertex_element_num, fp);
			}

			if (info->vertex_num > 0) {
				info->vertex_seq = LW_NEW(lwVector3[info->vertex_num]);
				fread(&info->vertex_seq[0], sizeof(lwVector3), info->vertex_num, fp);
			}

			if (info->fvf & D3DFVF_NORMAL) {
				info->normal_seq = LW_NEW(lwVector3[info->vertex_num]);
				fread(&info->normal_seq[0], sizeof(lwVector3), info->vertex_num, fp);
			}

			if (info->fvf & D3DFVF_TEX1) {
				info->texcoord0_seq = LW_NEW(lwVector2[info->vertex_num]);
				fread(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);

				//// added by clp
			}
			else if (info->fvf & D3DFVF_TEX2) {
				info->texcoord0_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord1_seq = LW_NEW(lwVector2[info->vertex_num]);
				fread(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			}
			else if (info->fvf & D3DFVF_TEX3) {
				info->texcoord0_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord1_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord2_seq = LW_NEW(lwVector2[info->vertex_num]);
				fread(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord2_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			}
			else if (info->fvf & D3DFVF_TEX4) {
				info->texcoord0_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord1_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord2_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord3_seq = LW_NEW(lwVector2[info->vertex_num]);
				fread(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord2_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord3_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			}

			if (info->fvf & D3DFVF_DIFFUSE) {
				info->vercol_seq = LW_NEW(DWORD[info->vertex_num]);
				fread(&info->vercol_seq[0], sizeof(DWORD), info->vertex_num, fp);
			}

			if (info->bone_index_num > 0) {
				info->blend_seq = LW_NEW(lwBlendInfo[info->vertex_num]);
				info->bone_index_seq = LW_NEW(DWORD[info->bone_index_num]);
				fread(&info->blend_seq[0], sizeof(lwBlendInfo), info->vertex_num, fp);
				fread(&info->bone_index_seq[0], sizeof(DWORD), info->bone_index_num, fp);
			}

			if (info->index_num > 0) {
				info->index_seq = LW_NEW(DWORD[info->index_num]);
				fread(&info->index_seq[0], sizeof(DWORD), info->index_num, fp);
			}

			if (info->subset_num > 0) {
				info->subset_seq = LW_NEW(lwSubsetInfo[info->subset_num]);
				fread(&info->subset_seq[0], sizeof(lwSubsetInfo), info->subset_num, fp);
			}
		}
		else {
			info->subset_seq = LW_NEW(lwSubsetInfo[info->subset_num]);
			fread(&info->subset_seq[0], sizeof(lwSubsetInfo), info->subset_num, fp);

			info->vertex_seq = LW_NEW(lwVector3[info->vertex_num]);
			fread(&info->vertex_seq[0], sizeof(lwVector3), info->vertex_num, fp);

			if (info->fvf & D3DFVF_NORMAL) {
				info->normal_seq = LW_NEW(lwVector3[info->vertex_num]);
				fread(&info->normal_seq[0], sizeof(lwVector3), info->vertex_num, fp);
			}

			if (info->fvf & D3DFVF_TEX1) {
				info->texcoord0_seq = LW_NEW(lwVector2[info->vertex_num]);
				fread(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);

				//// added by clp
			}
			else if (info->fvf & D3DFVF_TEX2) {
				info->texcoord0_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord1_seq = LW_NEW(lwVector2[info->vertex_num]);
				fread(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			}
			else if (info->fvf & D3DFVF_TEX3) {
				info->texcoord0_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord1_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord2_seq = LW_NEW(lwVector2[info->vertex_num]);
				fread(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord2_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			}
			else if (info->fvf & D3DFVF_TEX4) {
				info->texcoord0_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord1_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord2_seq = LW_NEW(lwVector2[info->vertex_num]);
				info->texcoord3_seq = LW_NEW(lwVector2[info->vertex_num]);
				fread(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord2_seq[0], sizeof(lwVector2), info->vertex_num, fp);
				fread(&info->texcoord3_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			}

			if (info->fvf & D3DFVF_DIFFUSE) {
				info->vercol_seq = LW_NEW(DWORD[info->vertex_num]);
				fread(&info->vercol_seq[0], sizeof(DWORD), info->vertex_num, fp);
			}

			if (info->fvf & D3DFVF_LASTBETA_UBYTE4) {
				info->blend_seq = LW_NEW(lwBlendInfo[info->vertex_num]);
				// old version use BYTE
				BYTE* byte_index_seq = LW_NEW(BYTE[info->bone_index_num]);

				fread(&info->blend_seq[0], sizeof(lwBlendInfo), info->vertex_num, fp);
				fread(&byte_index_seq[0], sizeof(BYTE), info->bone_index_num, fp);

				// convert it
				info->bone_index_seq = LW_NEW(DWORD[info->bone_index_num]);
				for (DWORD i = 0; i < info->bone_index_num; i++) {
					info->bone_index_seq[i] = byte_index_seq[i];
				}

				LW_DELETE_A(byte_index_seq);
			}

			if (info->index_num > 0) {
				info->index_seq = LW_NEW(DWORD[info->index_num]);
				fread(&info->index_seq[0], sizeof(DWORD), info->index_num, fp);
			}
		}


		return LW_RET_OK;
	}

	LW_RESULT lwMeshInfo_Save(lwMeshInfo* info, FILE* fp) {
		fwrite(&info->header, sizeof(info->header), 1, fp);

		if (info->vertex_element_num > 0) {
			fwrite(&info->vertex_element_seq[0], sizeof(D3DVERTEXELEMENTX), info->vertex_element_num, fp);
		}

		fwrite(&info->vertex_seq[0], sizeof(lwVector3), info->vertex_num, fp);

		if (info->fvf & D3DFVF_NORMAL) {
			fwrite(&info->normal_seq[0], sizeof(lwVector3), info->vertex_num, fp);
		}

		if (info->fvf & D3DFVF_TEX1) {
			fwrite(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
		}
		else if (info->fvf & D3DFVF_TEX2) {
			fwrite(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			fwrite(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
		}
		else if (info->fvf & D3DFVF_TEX3) {
			fwrite(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			fwrite(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			fwrite(&info->texcoord2_seq[0], sizeof(lwVector2), info->vertex_num, fp);
		}
		else if (info->fvf & D3DFVF_TEX4) {
			fwrite(&info->texcoord0_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			fwrite(&info->texcoord1_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			fwrite(&info->texcoord2_seq[0], sizeof(lwVector2), info->vertex_num, fp);
			fwrite(&info->texcoord3_seq[0], sizeof(lwVector2), info->vertex_num, fp);
		}

		if (info->fvf & D3DFVF_DIFFUSE) {
			fwrite(&info->vercol_seq[0], sizeof(DWORD), info->vertex_num, fp);
		}

		if (info->bone_index_num > 0) {
			fwrite(&info->blend_seq[0], sizeof(lwBlendInfo), info->vertex_num, fp);
			fwrite(&info->bone_index_seq[0], sizeof(DWORD), info->bone_index_num, fp);
		}

		if (info->index_num > 0) {
			fwrite(&info->index_seq[0], sizeof(DWORD), info->index_num, fp);
		}

		fwrite(&info->subset_seq[0], sizeof(lwSubsetInfo), info->subset_num, fp);

		return LW_RET_OK;
	}


	DWORD lwMeshInfo_GetDataSize(lwMeshInfo* info) {
		DWORD size = 0;

		size += sizeof(lwMeshInfoHeader);
		size += sizeof(D3DVERTEXELEMENTX) * info->vertex_element_num;
		size += sizeof(lwSubsetInfo) * info->subset_num;
		size += sizeof(lwVector3) * info->vertex_num;


		if (info->fvf & D3DFVF_NORMAL) {
			size += sizeof(lwVector3) * info->vertex_num;
		}

		if (info->fvf & D3DFVF_TEX1) {
			size += sizeof(lwVector2) * info->vertex_num;
		}
		else if (info->fvf & D3DFVF_TEX2) {
			size += sizeof(lwVector2) * info->vertex_num * 2;
		}
		else if (info->fvf & D3DFVF_TEX3) {
			size += sizeof(lwVector2) * info->vertex_num * 3;
		}
		else if (info->fvf & D3DFVF_TEX4) {
			size += sizeof(lwVector2) * info->vertex_num * 4;
		}

		if (info->fvf & D3DFVF_DIFFUSE) {
			size += sizeof(DWORD) * info->vertex_num;
		}

		if (info->fvf & D3DFVF_LASTBETA_UBYTE4) {
			size += sizeof(lwBlendInfo) * info->vertex_num;
			size += sizeof(DWORD) * info->bone_index_num;
		}

		if (info->index_num > 0) {
			size += sizeof(DWORD) * info->index_num;
		}

		return size;
	}

	// lwAnimDataInfo
	LW_STD_IMPLEMENTATION(lwAnimDataInfo)

	lwAnimDataInfo::lwAnimDataInfo() {
		anim_bone = 0;
		anim_mat = 0;

		memset(anim_mtlopac, 0, sizeof(anim_mtlopac));
		memset(anim_tex, 0, sizeof(anim_tex));
		memset(anim_img, 0, sizeof(anim_img));
	}

	lwAnimDataInfo::~lwAnimDataInfo() {
		LW_IF_DELETE(anim_bone);
		LW_IF_DELETE(anim_mat);

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			LW_IF_DELETE(anim_mtlopac[i]);

			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				LW_IF_DELETE(anim_tex[i][j]);
				LW_IF_DELETE(anim_img[i][j]);
			}
		}
	}

	LW_RESULT lwAnimDataInfo::Load(FILE* fp, DWORD version) {
		if (version == EXP_OBJ_VERSION_0_0_0_0) {
			DWORD old_version;
			fread(&old_version, sizeof(old_version), 1, fp);
		}

		DWORD data_bone_size, data_mat_size;
		DWORD data_mtlopac_size[LW_MAX_SUBSET_NUM];
		DWORD data_texuv_size[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM];
		DWORD data_teximg_size[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM];

		fread(&data_bone_size, sizeof(DWORD), 1, fp);
		fread(&data_mat_size, sizeof(DWORD), 1, fp);

		if (version >= EXP_OBJ_VERSION_1_0_0_5) {
			fread(&data_mtlopac_size, sizeof(data_mtlopac_size), 1, fp);
		}

		fread(&data_texuv_size, sizeof(data_texuv_size), 1, fp);
		fread(&data_teximg_size, sizeof(data_teximg_size), 1, fp);

		if (data_bone_size > 0) {
			anim_bone = LW_NEW(lwAnimDataBone);
			anim_bone->Load(fp, version);
		}

		if (data_mat_size > 0) {
#ifdef USE_ANIMKEY_PRS
			anim_mat = LW_NEW(lwAnimKeySetPRS);
			lwLoadAnimKeySetPRS(anim_mat, fp);
#else
			anim_mat = LW_NEW(lwAnimDataMatrix);
			anim_mat->Load(fp, version);
#endif
		}

		if (version >= EXP_OBJ_VERSION_1_0_0_5) {
			for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
				if (data_mtlopac_size[i] == 0)
					continue;

				anim_mtlopac[i] = LW_NEW(lwAnimDataMtlOpacity);
				anim_mtlopac[i]->Load(fp, version);
			}
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				if (data_texuv_size[i][j] == 0)
					continue;

				anim_tex[i][j] = LW_NEW(lwAnimDataTexUV);
				anim_tex[i][j]->Load(fp, version);
			}
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				if (data_teximg_size[i][j] == 0)
					continue;

				anim_img[i][j] = LW_NEW(lwAnimDataTexImg);
				anim_img[i][j]->Load(fp, version);
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwAnimDataInfo::Save(FILE* fp) {
		DWORD data_bone_size = 0;
		DWORD data_mat_size = 0;
		DWORD data_mtlopac_size[LW_MAX_SUBSET_NUM];
		DWORD data_texuv_size[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM];
		DWORD data_teximg_size[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM];
		memset(data_mtlopac_size, 0, sizeof(data_mtlopac_size));
		memset(data_texuv_size, 0, sizeof(data_texuv_size));
		memset(data_teximg_size, 0, sizeof(data_teximg_size));

		if (anim_bone) {
			data_bone_size = anim_bone->GetDataSize();
		}

		if (anim_mat) {
#ifdef USE_ANIMKEY_PRS
			data_mat_size = lwGetAnimKeySetPRSSize(anim_mat);
#else
			data_mat_size = anim_mat->GetDataSize();
#endif
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (anim_mtlopac[i]) {
				data_mtlopac_size[i] = anim_mtlopac[i]->GetDataSize();
				assert(data_mtlopac_size[i] > 0);
			}
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				if (anim_tex[i][j]) {
					data_texuv_size[i][j] = anim_tex[i][j]->GetDataSize();
				}
				if (anim_img[i][j]) {
					data_teximg_size[i][j] = anim_img[i][j]->GetDataSize();
				}
			}
		}

		fwrite(&data_bone_size, sizeof(DWORD), 1, fp);
		fwrite(&data_mat_size, sizeof(DWORD), 1, fp);
		fwrite(&data_mtlopac_size, sizeof(data_mtlopac_size), 1, fp);
		fwrite(&data_texuv_size, sizeof(data_texuv_size), 1, fp);
		fwrite(&data_teximg_size, sizeof(data_teximg_size), 1, fp);

		if (data_bone_size > 0) {
			if (LW_RESULT r = anim_bone->Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] anim_bone->Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}

		if (data_mat_size > 0) {
#ifdef USE_ANIMKEY_PRS
			lwSaveAnimKeySetPRS(fp, anim_mat);
#else
			if (LW_RESULT r = anim_mat->Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] anim_mat->Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
#endif
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (data_mtlopac_size[i] == 0)
				continue;

			if (LW_RESULT r = anim_mtlopac[i]->Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] anim_mtlopac->Save failed: i={}, ret={}",
							 __FUNCTION__, i, static_cast<long long>(r));
			}
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				if (data_texuv_size[i][j] == 0)
					continue;

				if (LW_RESULT r = anim_tex[i][j]->Save(fp); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] anim_tex->Save failed: i={}, j={}, ret={}",
								 __FUNCTION__, i, j, static_cast<long long>(r));
				}
			}
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				if (data_teximg_size[i][j] == 0)
					continue;

				if (LW_RESULT r = anim_img[i][j]->Save(fp); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] anim_img->Save failed: i={}, j={}, ret={}",
								 __FUNCTION__, i, j, static_cast<long long>(r));
				}
			}
		}


		return LW_RET_OK;
	}


	LW_RESULT lwAnimDataInfo::GetDataSize() const {
		DWORD size = 0;

		if (anim_bone) {
			size += anim_bone->GetDataSize();
		}

		if (anim_mat) {
#ifdef USE_ANIMKEY_PRS
			size += lwGetAnimKeySetPRSSize(anim_mat);
#else
			size += anim_mat->GetDataSize();
#endif
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (anim_mtlopac[i]) {
				size += anim_mtlopac[i]->GetDataSize();
			}

			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				if (anim_tex[i][j]) {
					size += anim_tex[i][j]->GetDataSize();
				}
				if (anim_img[i][j]) {
					size += anim_img[i][j]->GetDataSize();
				}
			}
		}

		if (size > 0) {
			size += sizeof(DWORD) * ANIM_DATA_NUM;
		}

		return size;
	}

	// get size function
	DWORD lwGetMtlTexInfoSize(const lwMtlTexInfo* info_seq, DWORD num) {
		DWORD size = 0;

		for (DWORD i = 0; i < num; i++) {
			size += lwMtlTexInfo_GetDataSize(const_cast<lwMtlTexInfo*>(&info_seq[i]));
		}

		if (size > 0) {
			size += sizeof(DWORD); // number
		}

		return size;
	}

	DWORD lwGetAnimKeySetPRSSize(const lwAnimKeySetPRS* info) {
		DWORD size = 0;

		size += sizeof(info->frame_num) + sizeof(info->pos_num) + sizeof(info->rot_num) + sizeof(info->sca_num);
		size += sizeof(lwKeyDataVector3) * info->pos_num;
		size += sizeof(lwKeyDataQuaternion) * info->rot_num;
		size += sizeof(lwKeyDataVector3) * info->sca_num;

		return size;
	}

	DWORD lwGetHelperMeshInfoSize(const lwHelperMeshInfo* info) {
		DWORD size = 0;

		size += sizeof(info->id);
		size += sizeof(info->type);
		size += sizeof(info->sub_type);
		size += sizeof(info->name);
		size += sizeof(info->state);
		size += sizeof(info->mat);
		size += sizeof(info->box);
		size += sizeof(info->vertex_num) + sizeof(info->face_num);
		size += sizeof(lwVector3) * info->vertex_num;
		size += sizeof(lwHelperMeshFaceInfo) * info->face_num;

		return size;
	}

	DWORD lwGetHelperBoxInfoSize(const lwHelperBoxInfo* info) {
		DWORD size = 0;

		size += sizeof(info->id);
		size += sizeof(info->name);
		size += sizeof(info->type);
		size += sizeof(info->state);
		size += sizeof(info->mat);
		size += sizeof(info->box);

		return size;
	}

	// load atom function

	LW_RESULT lwLoadAnimKeySetPRS(lwAnimKeySetPRS* info, FILE* fp) {
		fread(&info->frame_num, sizeof(DWORD), 1, fp);

		fread(&info->pos_num, sizeof(DWORD), 1, fp);
		fread(&info->rot_num, sizeof(DWORD), 1, fp);
		fread(&info->sca_num, sizeof(DWORD), 1, fp);

		if (info->pos_num > 0) {
			info->pos_seq = LW_NEW(lwKeyDataVector3[info->pos_num]);
			fread(&info->pos_seq[0], sizeof(lwKeyDataVector3), info->pos_num, fp);
		}

		if (info->rot_num > 0) {
			info->rot_seq = LW_NEW(lwKeyDataQuaternion[info->rot_num]);
			fread(&info->rot_seq[0], sizeof(lwKeyDataQuaternion), info->rot_num, fp);
		}

		if (info->sca_num > 0) {
			info->sca_seq = LW_NEW(lwKeyDataVector3[info->sca_num]);
			fread(&info->sca_seq[0], sizeof(lwKeyDataVector3), info->sca_num, fp);
		}


		return LW_RET_OK;
	}

	// save atom function


	LW_RESULT lwSaveAnimKeySetPRS(FILE* fp, const lwAnimKeySetPRS* info) {
		fwrite(&info->frame_num, sizeof(DWORD), 1, fp);

		fwrite(&info->pos_num, sizeof(DWORD), 1, fp);
		fwrite(&info->rot_num, sizeof(DWORD), 1, fp);
		fwrite(&info->sca_num, sizeof(DWORD), 1, fp);

		if (info->pos_num > 0) {
			fwrite(&info->pos_seq[0], sizeof(lwKeyDataVector3), info->pos_num, fp);
		}

		if (info->rot_num > 0) {
			fwrite(&info->rot_seq[0], sizeof(lwKeyDataQuaternion), info->rot_num, fp);
		}

		if (info->sca_num > 0) {
			fwrite(&info->sca_seq[0], sizeof(lwKeyDataVector3), info->sca_num, fp);
		}


		return LW_RET_OK;
	}


	LW_RESULT lwSaveAnimDataBone(std::string_view file, const lwAnimDataBone* info) {
		LW_RESULT ret = LW_RET_FAILED;

		FILE* fp = fopen(std::string{file}.c_str(), "wb");
		if (fp == NULL)
			return 0;

		DWORD version = EXP_OBJ_VERSION;
		fwrite(&version, sizeof(DWORD), 1, fp);

		if (LW_RESULT r = info->Save(fp); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] AnimDataBone::Save failed: file='{}', ret={}",
						 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
						 static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;

	__ret:
		if (fp) {
			fclose(fp);
		}

		return ret;
	}

	LW_RESULT lwCheckHelperMeshFaceShareSides(int* side1, int* side2, const lwHelperMeshFaceInfo* p1,
											  const lwHelperMeshFaceInfo* p2) {
		int r1 = -1;
		int r2 = -1;
		int tab[3] = {0, 2, 1};

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// check index
				if (p1->vertex[i] == p2->vertex[j]) {
					if (r1 != -1) {
						if (p1->vertex[i] == p1->vertex[r1]) {
							MessageBox(0, "the same vertices in one triangle", "warning", MB_OK);
							continue; // in the case of p1 has 2 same vertices , we ignore it
						}

						*side1 = tab[r1 + i - 1];
						*side2 = tab[r2 + j - 1];

						return LW_RET_OK;
					}
					else {
						r1 = i;
						r2 = j;
						break; // next vertex
					}
				}
			}
		}

		return LW_RET_FAILED;
	}


	LW_RESULT lwCreateHelperMeshInfo(lwHelperMeshInfo* info, const lwMeshInfo* mi) {
		if (mi->vertex_num == 0)
			return LW_RET_FAILED;

		info->vertex_num = mi->vertex_num;

		info->vertex_seq = LW_NEW(lwVector3[info->vertex_num]);
		memcpy(&info->vertex_seq[0], &mi->vertex_seq[0], sizeof(lwVector3) * info->vertex_num);

		info->face_num = mi->index_num / 3;
		info->face_seq = LW_NEW(lwHelperMeshFaceInfo[info->face_num]);

		DWORD i, j;
		lwHelperMeshFaceInfo *x, *x_i, *x_j;

		for (i = 0; i < info->face_num; i++) {
			x = &info->face_seq[i];

			// vertex id
			x->vertex[0] = mi->index_seq[3 * i];
			x->vertex[1] = mi->index_seq[3 * i + 1];
			x->vertex[2] = mi->index_seq[3 * i + 2];

			// plane
			lwPlaneFromPoints(&x->plane, &info->vertex_seq[x->vertex[0]], &info->vertex_seq[x->vertex[1]],
							  &info->vertex_seq[x->vertex[2]]);

			// center
			x->center = info->vertex_seq[x->vertex[0]] + info->vertex_seq[x->vertex[1]] + info->vertex_seq[x->vertex[
				2]];
			lwVector3Scale(&x->center, 1.0f / 3);
		}

		// begin set polygon neighbour sides
		int* mark_buf = LW_NEW(int[info->face_num]);
		memset(mark_buf, 0, sizeof(int) * info->face_num);

		for (i = 0; i < info->face_num; i++) {
			x = &info->face_seq[i];

			x->adj_face[0] = LW_INVALID_INDEX;
			x->adj_face[1] = LW_INVALID_INDEX;
			x->adj_face[2] = LW_INVALID_INDEX;
		}

		for (i = 0; i < info->face_num; i++) {
			if (mark_buf[i] == 3)
				continue; // has been checked before

			x_i = &info->face_seq[i];

			for (j = 0; j < info->face_num; j++) {
				if ((i == j) || (mark_buf[j] == 3))
					continue; // has been checked before

				x_j = &info->face_seq[j];

				if (x_i->adj_face[0] == j || x_i->adj_face[1] == j || x_i->adj_face[2] == j)
					continue; // has been checked before

				int side1, side2;

				if (LW_SUCCEEDED(lwCheckHelperMeshFaceShareSides(&side1, &side2, x_i,x_j))) {
					// get adjacent face index
					x_i->adj_face[side1] = j; // index j
					x_j->adj_face[side2] = i; // index i

					// set mark
					mark_buf[i] += 1;
					mark_buf[j] += 1;

					assert(mark_buf[i]<= 3);
					assert(mark_buf[j]<= 3);

					if (mark_buf[i] == 3) {
						break; // topology_i has been checked completely
					}
				}
			}
		}

		LW_DELETE_A(mark_buf);
		// end

		return LW_RET_OK;
	}


	LW_RESULT lwCopyAnimKeySetPRS(lwAnimKeySetPRS* dst, const lwAnimKeySetPRS* src) {
		dst->frame_num = src->frame_num;
		dst->pos_num = src->pos_num;
		dst->rot_num = src->rot_num;
		dst->sca_num = src->sca_num;

		if (dst->pos_num > 0) {
			dst->pos_seq = LW_NEW(lwKeyDataVector3[dst->pos_num]);
			memcpy(&dst->pos_seq[0], &src->pos_seq[0], sizeof(lwKeyDataVector3) * dst->pos_num);
		}

		if (dst->rot_num > 0) {
			dst->rot_seq = LW_NEW(lwKeyDataQuaternion[dst->rot_num]);
			memcpy(&dst->rot_seq[0], &src->rot_seq[0], sizeof(lwKeyDataQuaternion) * dst->rot_num);
		}

		if (dst->sca_num > 0) {
			dst->sca_seq = LW_NEW(lwKeyDataVector3[dst->sca_num]);
			memcpy(&dst->sca_seq[0], &src->sca_seq[0], sizeof(lwKeyDataVector3) * dst->sca_num);
		}

		return LW_RET_OK;
	}

	// lwHelperMeshInfo
	LW_RESULT lwHelperMeshInfo::Copy(const lwHelperMeshInfo* src) {
		id = src->id;
		type = src->type;
		sub_type = src->sub_type;
		mat = src->mat;
		box = src->box;
		vertex_num = src->vertex_num;
		face_num = src->face_num;

		_tcscpy(&name[0], &src->name[0]);

		vertex_seq = LW_NEW(lwVector3[vertex_num]);
		face_seq = LW_NEW(lwHelperMeshFaceInfo[face_num]);

		memcpy(vertex_seq, src->vertex_seq, sizeof(lwVector3) * vertex_num);
		memcpy(face_seq, src->face_seq, sizeof(lwHelperMeshFaceInfo) * face_num);

		return LW_RET_OK;
	}

	// lwHelperInfo
	LW_STD_IMPLEMENTATION(lwHelperInfo)

	LW_RESULT lwHelperInfo::Load(FILE* fp, DWORD version) {
		if (version == EXP_OBJ_VERSION_0_0_0_0) {
			DWORD old_version;
			fread(&old_version, sizeof(old_version), 1, fp);
		}

		fread(&type, sizeof(type), 1, fp);

		if (type & HELPER_TYPE_DUMMY) {
			_LoadHelperDummyInfo(fp, version);
		}

		if (type & HELPER_TYPE_BOX) {
			_LoadHelperBoxInfo(fp, version);
		}

		if (type & HELPER_TYPE_MESH) {
			_LoadHelperMeshInfo(fp, version);
		}

		if (type & HELPER_TYPE_BOUNDINGBOX) {
			_LoadBoundingBoxInfo(fp, version);
		}

		if (type & HELPER_TYPE_BOUNDINGSPHERE) {
			_LoadBoundingSphereInfo(fp, version);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::Save(FILE* fp) const {
		fwrite(&type, sizeof(type), 1, fp);

		if (type & HELPER_TYPE_DUMMY) {
			_SaveHelperDummyInfo(fp);
		}

		if (type & HELPER_TYPE_BOX) {
			_SaveHelperBoxInfo(fp);
		}

		if (type & HELPER_TYPE_MESH) {
			_SaveHelperMeshInfo(fp);
		}

		if (type & HELPER_TYPE_BOUNDINGBOX) {
			_SaveBoundingBoxInfo(fp);
		}

		if (type & HELPER_TYPE_BOUNDINGSPHERE) {
			_SaveBoundingSphereInfo(fp);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::Copy(const lwHelperInfo* src) {
		type = src->type;

		if (type & HELPER_TYPE_DUMMY) {
			dummy_num = src->dummy_num;
			dummy_seq = LW_NEW(lwHelperDummyInfo[dummy_num]);
			memcpy(&dummy_seq[0], &src->dummy_seq[0], sizeof(lwHelperDummyInfo) * dummy_num);
		}
		if (type & HELPER_TYPE_BOX) {
			box_num = src->box_num;
			box_seq = LW_NEW(lwHelperBoxInfo[box_num]);
			memcpy(&box_seq[0], &src->box_seq[0], sizeof(lwHelperBoxInfo) * box_num);
		}
		if (type & HELPER_TYPE_MESH) {
			mesh_num = src->mesh_num;
			mesh_seq = LW_NEW(lwHelperMeshInfo[mesh_num]);

			for (DWORD i = 0; i < mesh_num; i++) {
				mesh_seq[i].Copy(&src->mesh_seq[i]);
			}
		}

		if (type & HELPER_TYPE_BOUNDINGBOX) {
			bbox_num = src->bbox_num;
			bbox_seq = LW_NEW(lwBoundingBoxInfo[bbox_num]);
			memcpy(&bbox_seq[0], &src->bbox_seq[0], sizeof(lwBoundingBoxInfo) * bbox_num);
		}
		if (type & HELPER_TYPE_BOUNDINGSPHERE) {
			bsphere_num = src->bsphere_num;
			bsphere_seq = LW_NEW(lwBoundingSphereInfo[bsphere_num]);
			memcpy(&bsphere_seq[0], &src->bsphere_seq[0], sizeof(lwBoundingSphereInfo) * bsphere_num);
		}

		return LW_RET_OK;
	}

	DWORD lwHelperInfo::GetDataSize() const {
		DWORD size = 0;

		if (type & HELPER_TYPE_DUMMY) {
			size += sizeof(dummy_num);
			size += sizeof(lwHelperDummyInfo) * dummy_num;
		}

		if (type & HELPER_TYPE_BOX) {
			size += sizeof(box_num);
			for (DWORD i = 0; i < box_num; i++) {
				size += lwGetHelperBoxInfoSize(&box_seq[i]);
			}
		}

		if (type & HELPER_TYPE_MESH) {
			size += sizeof(mesh_num);
			for (DWORD i = 0; i < mesh_num; i++) {
				size += lwGetHelperMeshInfoSize(&mesh_seq[i]);
			}
		}

		if (type & HELPER_TYPE_BOUNDINGBOX) {
			size += sizeof(bbox_num);
			size += sizeof(lwBoundingBoxInfo) * bbox_num;
		}

		if (type & HELPER_TYPE_BOUNDINGSPHERE) {
			size += sizeof(bsphere_num);
			size += sizeof(lwBoundingSphereInfo) * bsphere_num;
		}

		if (size > 0) {
			size += sizeof(type);
		}

		return size;
	}

	// begin load / save item
	LW_RESULT lwHelperInfo::_LoadHelperDummyInfo(FILE* fp, DWORD version) {
		if (version >= EXP_OBJ_VERSION_1_0_0_1) {
			fread(&dummy_num, sizeof(dummy_num), 1, fp);
			dummy_seq = LW_NEW(lwHelperDummyInfo[dummy_num]);
			fread(&dummy_seq[0], sizeof(lwHelperDummyInfo), dummy_num, fp);
		}
		else if (version <= EXP_OBJ_VERSION_1_0_0_0) {
			fread(&dummy_num, sizeof(dummy_num), 1, fp);
			lwHelperDummyInfo_1000* old_s = LW_NEW(lwHelperDummyInfo_1000[dummy_num]);
			fread(&old_s[0], sizeof(lwHelperDummyInfo_1000), dummy_num, fp);

			dummy_seq = LW_NEW(lwHelperDummyInfo[dummy_num]);
			for (DWORD i = 0; i < dummy_num; i++) {
				dummy_seq[i].id = old_s[i].id;
				dummy_seq[i].mat = old_s[i].mat;
				dummy_seq[i].parent_type = 0;
				dummy_seq[i].parent_id = 0;
			}

			LW_DELETE_A(old_s);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_LoadHelperBoxInfo(FILE* fp, DWORD version) {
		fread(&box_num, sizeof(box_num), 1, fp);

		box_seq = LW_NEW(lwHelperBoxInfo[box_num]);
		fread(&box_seq[0], sizeof(lwHelperBoxInfo), box_num, fp);

		if (version <= EXP_OBJ_VERSION_1_0_0_1) {
			lwBox* b;
			lwBox_1001 old_b;
			for (DWORD i = 0; i < bbox_num; i++) {
				b = &box_seq[i].box;
				old_b.p = b->c;
				old_b.s = b->r;

				b->r = old_b.s / 2;
				b->c = old_b.p + b->r;
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_LoadHelperMeshInfo(FILE* fp, DWORD version) {
		fread(&mesh_num, sizeof(mesh_num), 1, fp);

		mesh_seq = LW_NEW(lwHelperMeshInfo[mesh_num]);

		lwHelperMeshInfo* info;

		for (DWORD i = 0; i < mesh_num; i++) {
			info = &mesh_seq[i];

			fread(&info->id, sizeof(info->id), 1, fp);
			fread(&info->type, sizeof(info->type), 1, fp);
			fread(&info->sub_type, sizeof(info->sub_type), 1, fp);
			fread(&info->name[0], sizeof(info->name), 1, fp);
			fread(&info->state, sizeof(info->state), 1, fp);
			fread(&info->mat, sizeof(info->mat), 1, fp);
			fread(&info->box, sizeof(info->box), 1, fp);
			fread(&info->vertex_num, sizeof(info->vertex_num), 1, fp);
			fread(&info->face_num, sizeof(info->face_num), 1, fp);

			info->vertex_seq = LW_NEW(lwVector3[info->vertex_num]);
			info->face_seq = LW_NEW(lwHelperMeshFaceInfo[info->face_num]);

			fread(&info->vertex_seq[0], sizeof(lwVector3), info->vertex_num, fp);
			fread(&info->face_seq[0], sizeof(lwHelperMeshFaceInfo), info->face_num, fp);
		}

		if (version <= EXP_OBJ_VERSION_1_0_0_1) {
			lwBox* b;
			lwBox_1001 old_b;
			for (DWORD i = 0; i < mesh_num; i++) {
				b = &mesh_seq[i].box;
				old_b.p = b->c;
				old_b.s = b->r;

				b->r = old_b.s / 2;
				b->c = old_b.p + b->r;
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_LoadBoundingBoxInfo(FILE* fp, DWORD version) {
		fread(&bbox_num, sizeof(DWORD), 1, fp);

		bbox_seq = LW_NEW(lwBoundingBoxInfo[bbox_num]);

		fread(&bbox_seq[0], sizeof(lwBoundingBoxInfo), bbox_num, fp);

		if (version <= EXP_OBJ_VERSION_1_0_0_1) {
			lwBox* b;
			lwBox_1001 old_b;
			for (DWORD i = 0; i < bbox_num; i++) {
				b = &bbox_seq[i].box;
				old_b.p = b->c;
				old_b.s = b->r;

				b->r = old_b.s / 2;
				b->c = old_b.p + b->r;
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_LoadBoundingSphereInfo(FILE* fp, DWORD version) {
		fread(&bsphere_num, sizeof(DWORD), 1, fp);

		bsphere_seq = LW_NEW(lwBoundingSphereInfo[bsphere_num]);

		fread(&bsphere_seq[0], sizeof(lwBoundingSphereInfo), bsphere_num, fp);

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_SaveHelperDummyInfo(FILE* fp) const {
		fwrite(&dummy_num, sizeof(dummy_num), 1, fp);
		fwrite(&dummy_seq[0], sizeof(lwHelperDummyInfo), dummy_num, fp);

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_SaveHelperBoxInfo(FILE* fp) const {
		fwrite(&box_num, sizeof(box_num), 1, fp);
		fwrite(&box_seq[0], sizeof(lwHelperBoxInfo), box_num, fp);

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_SaveHelperMeshInfo(FILE* fp) const {
		fwrite(&mesh_num, sizeof(mesh_num), 1, fp);

		lwHelperMeshInfo* info;
		for (DWORD i = 0; i < mesh_num; i++) {
			info = &mesh_seq[i];

			fwrite(&info->id, sizeof(info->id), 1, fp);
			fwrite(&info->type, sizeof(info->type), 1, fp);
			fwrite(&info->sub_type, sizeof(info->sub_type), 1, fp);
			fwrite(&info->name[0], sizeof(info->name), 1, fp);
			fwrite(&info->state, sizeof(info->state), 1, fp);
			fwrite(&info->mat, sizeof(info->mat), 1, fp);
			fwrite(&info->box, sizeof(info->box), 1, fp);
			fwrite(&info->vertex_num, sizeof(info->vertex_num), 1, fp);
			fwrite(&info->face_num, sizeof(info->face_num), 1, fp);

			fwrite(&info->vertex_seq[0], sizeof(lwVector3), info->vertex_num, fp);
			fwrite(&info->face_seq[0], sizeof(lwHelperMeshFaceInfo), info->face_num, fp);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_SaveBoundingBoxInfo(FILE* fp) const {
		fwrite(&bbox_num, sizeof(DWORD), 1, fp);
		fwrite(&bbox_seq[0], sizeof(lwBoundingBoxInfo), bbox_num, fp);

		return LW_RET_OK;
	}

	LW_RESULT lwHelperInfo::_SaveBoundingSphereInfo(FILE* fp) const {
		fwrite(&bsphere_num, sizeof(DWORD), 1, fp);
		fwrite(&bsphere_seq[0], sizeof(lwBoundingSphereInfo), bsphere_num, fp);

		return LW_RET_OK;
	}

	// lwGeomObjInfo
	LW_STD_IMPLEMENTATION(lwGeomObjInfo)

	LW_RESULT lwGeomObjInfo::Load(FILE* fp, DWORD version) {
		fread((lwGeomObjInfoHeader*)&id, sizeof(lwGeomObjInfoHeader), 1, fp);

		state_ctrl.SetState(STATE_FRAMECULLING, 0);
		state_ctrl.SetState(STATE_UPDATETRANSPSTATE, 1);

		// debug
		if (mtl_size > 100000) {
			return LW_RET_FAILED;
		}
		// end

		// read mtl data
		if (mtl_size > 0) {
			lwLoadMtlTexInfo(&mtl_seq, &mtl_num, fp, version);
		}

		// read mesh data
		if (mesh_size > 0) {
			lwMeshInfo_Load(&mesh, fp, version);
		}

		// read helper datas
		if (helper_size > 0) {
			helper_data.Load(fp, version);
		}

		// read animation data
		if (anim_size > 0) {
			anim_data.Load(fp, version);
		}


		if ((rcci.vs_id == VST_PU4NT0_LD)
			|| (rcci.vs_id == VST_PB1U4NT0_LD)
			|| (rcci.vs_id == VST_PB2U4NT0_LD)
			|| (rcci.vs_id == VST_PB3U4NT0_LD)) {
		}

		return LW_RET_OK;
	}

	LW_RESULT lwGeomObjInfo::Save(FILE* fp) {
		fwrite((lwGeomObjInfoHeader*)&id, sizeof(lwGeomObjInfoHeader), 1, fp);

		// save mtl data
		if (mtl_size > 0) {
			lwSaveMtlTexInfo(fp, &mtl_seq[0], mtl_num);
		}

		// save mesh data
		if (mesh_size > 0) {
			lwMeshInfo_Save(&mesh, fp);
		}

		if (helper_size > 0) {
			if (LW_RESULT r = helper_data.Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] helper_data.Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}

		if (anim_size > 0) {
			if (LW_RESULT r = anim_data.Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] anim_data.Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}


		return LW_RET_OK;
	}

	LW_RESULT lwGeomObjInfo::Load(std::string_view file) {
		FILE* fp = fopen(std::string{file}.c_str(), "rb");
		if (fp == NULL)
			return LW_RET_FAILED;

		std::string value{file};
		if (value == "model\\effect\\01020018.lgo") {
			const auto sizeX86 = sizeof(lwMeshInfoHeader); //128
			int x = 10;
		}

		DWORD version;
		fread(&version, sizeof(version), 1, fp);

		LW_RESULT ret = Load(fp, version);

		if (fp) {
			fclose(fp);
		}

		return ret;
	}

	LW_RESULT lwGeomObjInfo::Save(std::string_view file) {
		FILE* fp = fopen(std::string{file}.c_str(), "wb");
		if (fp == NULL)
			return LW_RET_FAILED;

		DWORD version = EXP_OBJ_VERSION;
		fwrite(&version, sizeof(version), 1, fp);

		Save(fp);

		if (fp) {
			fclose(fp);
		}

		return LW_RET_OK;
	}

	DWORD lwGeomObjInfo::GetDataSize() const {
		DWORD size = 0;


		size += sizeof(lwGeomObjInfoHeader);
		size += mtl_size;
		size += mesh_size;
		size += helper_size;
		size += anim_size;

		return size;
	}

	// lwModelObjInfo
	LW_STD_IMPLEMENTATION(lwModelObjInfo)

	LW_RESULT lwModelObjInfo::Load(std::string_view file) {
		FILE* fp = fopen(std::string{file}.c_str(), "rb");
		if (fp == NULL)
			return LW_RET_FAILED;


		DWORD version;
		fread(&version, sizeof(version), 1, fp);


		DWORD obj_num;
		fread(&obj_num, sizeof(DWORD), 1, fp);

		lwModelObjInfoHeader header[LW_MAX_MODEL_OBJ_NUM];
		fread(&header[0], sizeof(lwModelObjInfoHeader), obj_num, fp);

		geom_obj_num = 0;

		for (DWORD i = 0; i < obj_num; i++) {
			fseek(fp, header[i].addr, SEEK_SET);

			switch (header[i].type) {
			case MODEL_OBJ_TYPE_GEOMETRY:
				geom_obj_seq[geom_obj_num] = LW_NEW(lwGeomObjInfo());
				if (version == EXP_OBJ_VERSION_0_0_0_0) {
					DWORD old_version;
					fread(&old_version, sizeof(old_version), 1, fp);
				}
				geom_obj_seq[geom_obj_num]->Load(fp, version);
				geom_obj_num += 1;
				break;
			case MODEL_OBJ_TYPE_HELPER:
				helper_data.Load(fp, version);
				break;
			default:
				assert(0);
			}
		}


		if (fp) {
			fclose(fp);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwModelObjInfo::Save(std::string_view file) {
		FILE* fp = fopen(std::string{file}.c_str(), "wb");
		if (fp == NULL)
			return LW_RET_FAILED;


		DWORD version = EXP_OBJ_VERSION;

		fwrite(&version, sizeof(version), 1, fp);

		DWORD obj_num = geom_obj_num;

		if (helper_data.type != HELPER_TYPE_INVALID) {
			obj_num += 1;
		}

		fwrite(&obj_num, sizeof(DWORD), 1, fp);

		DWORD i;
		lwModelObjInfoHeader header;
		const lwGeomObjInfo* goi;

		DWORD base_offset_size = sizeof(version) + sizeof(obj_num) + sizeof(header) * obj_num;

		DWORD total_obj_size = 0;

		// geomobj header
		for (i = 0; i < geom_obj_num; i++) {
			goi = geom_obj_seq[i];

			header.type = MODEL_OBJ_TYPE_GEOMETRY;
			header.addr = base_offset_size + total_obj_size;
			header.size = goi->GetDataSize();

			fwrite(&header, sizeof(header), 1, fp);

			total_obj_size += header.size;
		}


		// helper header
		if (helper_data.type != HELPER_TYPE_INVALID) {
			header.addr = base_offset_size + total_obj_size;
			header.size = helper_data.GetDataSize();
			header.type = MODEL_OBJ_TYPE_HELPER;

			fwrite(&header, sizeof(header), 1, fp);

			total_obj_size += header.size;
		}

		// save geometry data
		for (i = 0; i < geom_obj_num; i++) {
			geom_obj_seq[i]->Save(fp);
		}


		// save helper data
		if (helper_data.type != HELPER_TYPE_INVALID) {
			if (LW_RESULT r = helper_data.Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] helper_data.Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}


		if (fp) {
			fclose(fp);
		}


		return LW_RET_OK;
	}

	LW_RESULT lwModelObjInfo::SortGeomObjInfoWithID() {
		lwGeomObjInfo* buf;
		for (DWORD i = 0; i < geom_obj_num - 1; i++) {
			for (DWORD j = i + 1; j < geom_obj_num; j++) {
				if (geom_obj_seq[i]->id > geom_obj_seq[j]->id) {
					buf = geom_obj_seq[i];
					geom_obj_seq[i] = geom_obj_seq[j];
					geom_obj_seq[j] = buf;
				}
			}
		}

		return 1;
	}

	DWORD lwModelObjInfo::GetDataSize() {
		DWORD size = 0;

		for (DWORD i = 0; i < geom_obj_num; i++) {
			size += geom_obj_seq[i]->GetDataSize();
		}

		size += helper_data.GetDataSize();

		return size;
	}

	LW_RESULT lwModelObjInfo::GetHeader(lwModelObjInfoHeader* header_seq, DWORD* header_num, std::string_view file) {
		LW_RESULT ret = LW_RET_FAILED;

		FILE* fp = fopen(std::string{file}.c_str(), "rb");
		if (fp == NULL)
			goto __ret;

		DWORD version;
		fread(&version, sizeof(DWORD), 1, fp);


		fread(header_num, sizeof(DWORD), 1, fp);

		fread(&header_seq[0], sizeof(lwModelObjInfoHeader), *header_num, fp);

		ret = LW_RET_OK;

	__ret:
		if (fp) {
			fclose(fp);
		}

		return ret;
	}

	// lwHelperDummyObjInfo
	LW_STD_IMPLEMENTATION(lwHelperDummyObjInfo)

	lwHelperDummyObjInfo::lwHelperDummyObjInfo() {
		_id = LW_INVALID_INDEX;
		lwMatrix44Identity(&_mat);
		_anim_data = 0;
	}

	lwHelperDummyObjInfo::~lwHelperDummyObjInfo() {
		LW_IF_RELEASE(_anim_data);
	}

	LW_RESULT lwHelperDummyObjInfo::Load(FILE* fp, DWORD version) {
		LW_RESULT ret = LW_RET_FAILED;

		fread(&_id, sizeof(_id), 1, fp);
		fread(&_mat, sizeof(_mat), 1, fp);

		LW_SAFE_RELEASE(_anim_data);

		DWORD anim_data_flag = 0;
		fread(&anim_data_flag, sizeof(anim_data_flag), 1, fp);
		if (anim_data_flag == 1) {
			lwAnimDataMatrix* anim_data = LW_NEW(lwAnimDataMatrix);
			if (LW_RESULT r = anim_data->Load(fp, 0); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] AnimDataMatrix::Load failed: id={}, ret={}",
							 __FUNCTION__, static_cast<long long>(_id),
							 static_cast<long long>(r));
				LW_DELETE(anim_data);
				goto __ret;
			}
			_anim_data = anim_data;
		}


		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwHelperDummyObjInfo::Save(FILE* fp) {
		LW_RESULT ret = LW_RET_FAILED;

		fwrite(&_id, sizeof(_id), 1, fp);
		fwrite(&_mat, sizeof(_mat), 1, fp);

		DWORD anim_data_flag = _anim_data ? 1 : 0;
		fwrite(&anim_data_flag, sizeof(anim_data_flag), 1, fp);
		if (_anim_data) {
			if (LW_RESULT r = ((lwAnimDataMatrix*)_anim_data)->Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] AnimDataMatrix::Save failed: id={}, ret={}",
							 __FUNCTION__, static_cast<long long>(_id),
							 static_cast<long long>(r));
				goto __ret;
			}
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	lwModelNodeInfo::~lwModelNodeInfo() {
		if (_type == NODE_PRIMITIVE) {
			lwGeomObjInfo* data = (lwGeomObjInfo*)_data;
			LW_IF_DELETE(data);
		}
		else if (_type == NODE_BONECTRL) {
			lwAnimDataBone* data = (lwAnimDataBone*)_data;
			LW_IF_DELETE(data);
		}
		else if (_type == NODE_DUMMY) {
			lwMatrix44* data = (lwMatrix44*)_data;
			LW_IF_DELETE(data);
		}
		else if (_type == NODE_HELPER) {
			lwHelperInfo* data = (lwHelperInfo*)_data;
			LW_IF_DELETE(data);
		}
	}

	LW_RESULT lwModelNodeInfo::Load(FILE* fp, DWORD version) {
		LW_RESULT ret = LW_RET_FAILED;

		fread(&_head, sizeof(_head), 1, fp);

		if (_type == NODE_PRIMITIVE) {
			_data = LW_NEW(lwGeomObjInfo);
			if (LW_RESULT r = ((lwGeomObjInfo*)_data)->Load(fp, version); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwGeomObjInfo::Load failed: version={}, ret={}",
							 __FUNCTION__, static_cast<long long>(version),
							 static_cast<long long>(r));
				goto __ret;
			}
		}
		else if (_type == NODE_BONECTRL) {
			_data = LW_NEW(lwAnimDataBone);
			if (LW_RESULT r = ((lwAnimDataBone*)_data)->Load(fp, version); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwAnimDataBone::Load failed: version={}, ret={}",
							 __FUNCTION__, static_cast<long long>(version),
							 static_cast<long long>(r));
				goto __ret;
			}
		}
		else if (_type == NODE_DUMMY) {
			_data = LW_NEW(lwHelperDummyObjInfo);
			if (LW_RESULT r = ((lwHelperDummyObjInfo*)_data)->Load(fp, version); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwHelperDummyObjInfo::Load failed: version={}, ret={}",
							 __FUNCTION__, static_cast<long long>(version),
							 static_cast<long long>(r));
				goto __ret;
			}
		}
		else if (_type == NODE_HELPER) {
			_data = LW_NEW(lwHelperInfo);
			if (LW_RESULT r = ((lwHelperInfo*)_data)->Load(fp, version); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwHelperInfo::Load failed: version={}, ret={}",
							 __FUNCTION__, static_cast<long long>(version),
							 static_cast<long long>(r));
				goto __ret;
			}
		}
		else {
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwModelNodeInfo::Save(FILE* fp) {
		LW_RESULT ret = LW_RET_FAILED;

		fwrite(&_head, sizeof(_head), 1, fp);

		if (_type == NODE_PRIMITIVE) {
			if (LW_RESULT r = ((lwGeomObjInfo*)_data)->Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwGeomObjInfo::Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}
		}
		else if (_type == NODE_BONECTRL) {
			if (LW_RESULT r = ((lwAnimDataBone*)_data)->Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwAnimDataBone::Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}
		}
		else if (_type == NODE_DUMMY) {
			if (LW_RESULT r = ((lwHelperDummyObjInfo*)_data)->Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwHelperDummyObjInfo::Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}
		}
		else if (_type == NODE_HELPER) {
			if (LW_RESULT r = ((lwHelperInfo*)_data)->Save(fp); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwHelperInfo::Save failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	// lwModelInfo
	static DWORD __tree_proc_modlinfo_destroy(lwITreeNode* node, void* param) {
		lwModelNodeInfo* data = (lwModelNodeInfo*)node->GetData();
		LW_IF_DELETE(data);

		return TREENODE_PROC_RET_CONTINUE;
	}

	struct __load_param {
		FILE* fp;
		DWORD version;
	};

	static DWORD __tree_proc_modlinfo_load(lwITreeNode* node, void* param) {
		__load_param* lp = (__load_param*)param;

		lwModelNodeInfo* data = LW_NEW(lwModelNodeInfo);

		if (LW_RESULT r = data->Load(lp->fp, lp->version); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwModelNodeInfo::Load failed: version={}, ret={}",
						 __FUNCTION__, static_cast<long long>(lp->version),
						 static_cast<long long>(r));
			LW_DELETE(data);
			return TREENODE_PROC_RET_ABORT;
		}

		node->SetData(data);

		return TREENODE_PROC_RET_CONTINUE;
	}

	static DWORD __tree_proc_modlinfo_save(lwITreeNode* node, void* param) {
		FILE* fp = (FILE*)param;

		lwModelNodeInfo* data = (lwModelNodeInfo*)node->GetData();

		if (LW_RESULT r = data->Save(fp); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwModelNodeInfo::Save failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			return TREENODE_PROC_RET_ABORT;
		}

		return TREENODE_PROC_RET_CONTINUE;
	}

	lwModelInfo::~lwModelInfo() {
		Destroy();
	}

	static DWORD __tree_node_release_proc(lwITreeNode* node, void* param) {
		LW_RELEASE(node);
		return TREENODE_PROC_RET_CONTINUE;
	}

	void lwReleaseTreeNodeList_(lwITreeNode* node) {
		node->EnumTree(__tree_node_release_proc, 0, TREENODE_PROC_POSTORDER);
	}

	LW_RESULT lwModelInfo::Destroy() {
		if (_obj_tree) {
			_obj_tree->EnumTree(__tree_proc_modlinfo_destroy, 0, TREENODE_PROC_PREORDER);
			lwReleaseTreeNodeList_(_obj_tree);
		}

		return LW_RET_OK;
	}

	struct __find_info {
		lwITreeNode* node;
		DWORD handle;
	};

	static DWORD __tree_proc_find_node(lwITreeNode* node, void* param) {
		lwModelNodeInfo* data = (lwModelNodeInfo*)node->GetData();
		__find_info* info = (__find_info*)param;

		if (data->_handle == info->handle) {
			info->node = node;
			return TREENODE_PROC_RET_ABORT;
		}

		return TREENODE_PROC_RET_CONTINUE;
	}

	LW_RESULT lwModelInfo::Load(std::string_view file) {
		LW_RESULT ret = LW_RET_FAILED;

		FILE* fp = fopen(std::string{file}.c_str(), "rb");
		if (fp == 0)
			goto __ret;

		fread(&_head, sizeof(_head), 1, fp);

		DWORD obj_num;
		fread(&obj_num, sizeof(obj_num), 1, fp);

		if (obj_num == 0)
			goto __addr_ret_ok;

		{
			lwITreeNode* tree_node = 0;
			lwModelNodeInfo* node_info = 0;

			for (DWORD i = 0; i < obj_num; i++) {
				node_info = LW_NEW(lwModelNodeInfo);
				if (LW_RESULT r = node_info->Load(fp, _head.version); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] lwModelNodeInfo::Load failed: file='{}', i={}, version={}, ret={}",
								 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
								 static_cast<long long>(i),
								 static_cast<long long>(_head.version),
								 static_cast<long long>(r));
					LW_DELETE(node_info);
					goto __ret;
				}

				tree_node = LW_NEW(lwTreeNode);
				tree_node->SetData(node_info);

				// reset tree
				if (_obj_tree == 0) {
					_obj_tree = tree_node;
				}
				else {
					__find_info param;
					param.handle = node_info->_parent_handle;
					param.node = 0;
					_obj_tree->EnumTree(__tree_proc_find_node, (void*)&param, TREENODE_PROC_PREORDER);

					if (param.node == 0)
						goto __ret;

					if (LW_RESULT r = param.node->InsertChild(0, tree_node); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] InsertChild failed: file='{}', i={}, parent_handle={}, ret={}",
									 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file),
									 static_cast<long long>(i),
									 static_cast<long long>(node_info->_parent_handle),
									 static_cast<long long>(r));
						goto __ret;
					}
				}
			}
		}
	__addr_ret_ok:
		ret = LW_RET_OK;
	__ret:
		if (fp) {
			fclose(fp);
		}

		return ret;
	}

	LW_RESULT lwModelInfo::Save(std::string_view file) {
		LW_RESULT ret = LW_RET_FAILED;

		FILE* fp = fopen(std::string{file}.c_str(), "wb");
		if (fp == 0)
			goto __ret;

		_head.version = MODELINFO_VERSION;
		_tcscpy(_head.decriptor, "lwModelInfo");

		fwrite(&_head, sizeof(_head), 1, fp);

		{
			DWORD obj_num = _obj_tree ? _obj_tree->GetNodeNum() : 0;

			fwrite(&obj_num, sizeof(obj_num), 1, fp);

			if (_obj_tree == 0)
				goto __ret;

			LW_RESULT r = _obj_tree->EnumTree(__tree_proc_modlinfo_save, fp, TREENODE_PROC_PREORDER);
			if (r == TREENODE_PROC_RET_ABORT)
				goto __ret;
		}
		ret = LW_RET_OK;
	__ret:
		if (fp) {
			fclose(fp);
		}

		return ret;
	}

	LW_RESULT lwModelNodeSortChild(lwITreeNode* node) {
		DWORD child_num = node->GetChildNum();

		if (child_num < 2) {
			return LW_RET_OK;
		}

		lwITreeNode** child_seq = LW_NEW(lwITreeNode*[child_num]);


		// init buf
		DWORD i, j;
		for (i = 0; i < child_num; i++) {
			child_seq[i] = node->GetChild(i);
		}

		// sort
		lwITreeNode* buf;
		lwModelNodeInfo* node_i;
		lwModelNodeInfo* node_j;

		for (i = 0; i < child_num - 1; i++) {
			for (j = i + 1; j < child_num; j++) {
				node_i = (lwModelNodeInfo*)child_seq[i]->GetData();
				node_j = (lwModelNodeInfo*)child_seq[j]->GetData();

				if (node_i->_id > node_j->_id) {
					buf = child_seq[i];
					child_seq[i] = child_seq[j];
					child_seq[j] = buf;
				}
			}
		}

		// reset sibling
		for (i = 0; i < child_num - 1; i++) {
			child_seq[i]->SetSibling(child_seq[i + 1]);
		}
		child_seq[child_num - 1]->SetSibling(0);

		node->SetChild(child_seq[0]);

		LW_DELETE_A(child_seq);

		return LW_RET_OK;
	};

	DWORD __tree_proc_sort_id(lwITreeNode* node, void* param) {
		if (LW_RESULT r = lwModelNodeSortChild(node); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwModelNodeSortChild failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			return TREENODE_PROC_RET_ABORT;
		}

		return TREENODE_PROC_RET_CONTINUE;
	}

	LW_RESULT lwModelInfo::SortChildWithID() {
		DWORD ret = _obj_tree->EnumTree(__tree_proc_sort_id, 0, TREENODE_PROC_PREORDER);

		return ret == TREENODE_PROC_RET_CONTINUE ? LW_RET_OK : LW_RET_FAILED;
	}

LW_END
