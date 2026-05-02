//

#include "stdafx.h"
#include "lwExpObj.h"
#include "lwErrorCode.h"
#include "lwTreeNode.h"
#include "lwAnimKeySetPRS.h"
#include "lwIUtil.h"
#include "AssetLoaders.h"

LW_BEGIN
	// ============================================================================
	// Замки размеров структур, участвующих в fread/fwrite целиком.
	// ============================================================================
	// Файлы моделей/анимаций (`.lgo`, `.lmo`, `.lab`, `.lam`) пишутся/читаются
	// через `fread(&x, sizeof(Type), count, fp)`, т.е. размер структуры жёстко
	// зашит в бинарный формат. Если `sizeof(Type)` меняется — например, из-за
	// добавления поля-указателя (его размер 4 на x86 и 8 на x64), либо смены
	// порядка полей — смещения в файле съезжают, в поля типа `index_num`
	// попадает мусор, и клиент падает на `LGO_NEW_ARRAY(DWORD, info->index_num)`.
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



	// lwLoadMtlTexInfo / lwSaveMtlTexInfo / lwGetMtlTexInfoSize перенесены
	// в LgoLoader (sources/Engine/src/AssetLoaders.cpp).

	DWORD lwMtlTexInfo_GetDataSize(lwMtlTexInfo* info) {
		return sizeof(info->opacity) + sizeof(info->transp_type)
			+ sizeof(info->mtl) + sizeof(info->rs_set)
			+ sizeof(info->tex_seq);
	}


	// lwAnimDataTexUV
	LW_STD_IMPLEMENTATION(lwAnimDataTexUV)



	LW_RESULT lwAnimDataTexUV::Copy(const lwAnimDataTexUV* src) {
		_frame_num = src->_frame_num;
		if (_frame_num > 0) {
			_mat_seq = LGO_NEW_ARRAY(lwMatrix44, _frame_num);
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
	// LW_STD_IMPLEMENTATION snято — наследование от lwIAnimDataTexImg удалено.





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
		_data_seq = LGO_NEW_ARRAY(lwTexInfo, _data_num);
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




	LW_RESULT lwAnimDataBone::Copy(const lwAnimDataBone* src) {
		if (src->_key_type == BONE_KEY_TYPE_INVALID)
			return LW_RET_FAILED;

		_header = src->_header;

		_base_seq = LGO_NEW_ARRAY(lwBoneBaseInfo, _bone_num);
		_invmat_seq = LGO_NEW_ARRAY(lwMatrix44, _bone_num);
		_key_seq = LGO_NEW_ARRAY(lwBoneKeyInfo, _bone_num);
		_dummy_seq = LGO_NEW_ARRAY(lwBoneDummyInfo, _dummy_num);

		memcpy(&_base_seq[0], &src->_base_seq[0], sizeof(lwBoneBaseInfo) * _bone_num);
		memcpy(&_invmat_seq[0], &src->_invmat_seq[0], sizeof(lwMatrix44) * _bone_num);
		memcpy(&_dummy_seq[0], &src->_dummy_seq[0], sizeof(lwBoneDummyInfo) * _dummy_num);


		DWORD i;
		lwBoneKeyInfo* key;

		switch (_key_type) {
		case BONE_KEY_TYPE_MAT43:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				key->mat43_seq = LGO_NEW_ARRAY(lwMatrix43, _frame_num);
				memcpy(&key->mat43_seq[0], &src->_key_seq[i].mat43_seq[0], sizeof(lwMatrix43) * _frame_num);
			}
			break;
		case BONE_KEY_TYPE_MAT44:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				key->mat44_seq = LGO_NEW_ARRAY(lwMatrix44, _frame_num);
				memcpy(&key->mat44_seq[0], &src->_key_seq[i].mat44_seq[0], sizeof(lwMatrix44) * _frame_num);
			}
			break;
		case BONE_KEY_TYPE_QUAT:
			for (i = 0; i < _bone_num; i++) {
				key = &_key_seq[i];
				key->pos_seq = LGO_NEW_ARRAY(lwVector3, _frame_num);
				key->quat_seq = LGO_NEW_ARRAY(lwQuaternion, _frame_num);
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

	LW_RESULT lwAnimDataMatrix::Copy(const lwAnimDataMatrix* src) {
		_frame_num = src->_frame_num;

		if (_frame_num > 0) {
			_mat_seq = LGO_NEW_ARRAY(lwMatrix43, _frame_num);
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
			dst->vertex_seq = LGO_NEW_ARRAY(lwVector3, dst->vertex_num);
			memcpy(dst->vertex_seq, src->vertex_seq, sizeof(lwVector3) * dst->vertex_num);
		}

		if (dst->fvf & D3DFVF_NORMAL) {
			dst->normal_seq = LGO_NEW_ARRAY(lwVector3, dst->vertex_num);
			memcpy(dst->normal_seq, src->normal_seq, sizeof(lwVector3) * dst->vertex_num);
		}

		if (dst->fvf & D3DFVF_TEX1) {
			dst->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			memcpy(dst->texcoord0_seq, src->texcoord0_seq, sizeof(lwVector2) * dst->vertex_num);

			//// added by clp
		}
		else if (dst->fvf & D3DFVF_TEX2) {
			dst->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			dst->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			memcpy(dst->texcoord0_seq, src->texcoord0_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord1_seq, src->texcoord1_seq, sizeof(lwVector2) * dst->vertex_num);
		}
		else if (dst->fvf & D3DFVF_TEX3) {
			dst->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			dst->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			dst->texcoord2_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			memcpy(dst->texcoord0_seq, src->texcoord0_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord1_seq, src->texcoord1_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord2_seq, src->texcoord2_seq, sizeof(lwVector2) * dst->vertex_num);
		}
		else if (dst->fvf & D3DFVF_TEX4) {
			dst->texcoord0_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			dst->texcoord1_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			dst->texcoord2_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			dst->texcoord3_seq = LGO_NEW_ARRAY(lwVector2, dst->vertex_num);
			memcpy(dst->texcoord0_seq, src->texcoord0_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord1_seq, src->texcoord1_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord2_seq, src->texcoord2_seq, sizeof(lwVector2) * dst->vertex_num);
			memcpy(dst->texcoord3_seq, src->texcoord3_seq, sizeof(lwVector2) * dst->vertex_num);
		}

		if (dst->fvf & D3DFVF_DIFFUSE) {
			dst->vercol_seq = LGO_NEW_ARRAY(DWORD, dst->vertex_num);
			memcpy(dst->vercol_seq, src->vercol_seq, sizeof(DWORD) * dst->vertex_num);
		}

		if (dst->bone_index_num > 0) {
			dst->blend_seq = LGO_NEW_ARRAY(lwBlendInfo, dst->vertex_num);
			dst->bone_index_seq = LGO_NEW_ARRAY(DWORD, dst->bone_index_num);
			memcpy(dst->blend_seq, src->blend_seq, sizeof(lwBlendInfo) * dst->vertex_num);
			memcpy(dst->bone_index_seq, src->bone_index_seq, sizeof(DWORD) * dst->bone_index_num);
		}

		if (dst->index_num > 0) {
			dst->index_seq = LGO_NEW_ARRAY(DWORD, dst->index_num);
			memcpy(dst->index_seq, src->index_seq, sizeof(DWORD) * dst->index_num);
		}

		if (dst->subset_num > 0) {
			dst->subset_seq = LGO_NEW_ARRAY(lwSubsetInfo, dst->subset_num);
			memcpy(dst->subset_seq, src->subset_seq, sizeof(lwSubsetInfo) * dst->subset_num);
		}

		return LW_RET_OK;
	}

	// lwAnimDataInfo
	// LW_STD_IMPLEMENTATION снято — наследование от lwIAnimDataInfo удалено.

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

	// lwAnimDataInfo::Load / Save / GetDataSize, lwLoadMtlTexInfo /
	// lwSaveMtlTexInfo / lwGetMtlTexInfoSize, lwMeshInfo_Load / lwMeshInfo_Save
	// / lwMeshInfo_GetDataSize — все перенесены в LgoLoader
	// (sources/Engine/src/AssetLoaders.cpp). Сами data-структуры
	// (lwMtlTexInfo, lwMeshInfo, lwAnimDataInfo) теперь без I/O-логики.

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




	LW_RESULT lwSaveAnimDataBone(std::string_view file, const lwAnimDataBone* info) {
		LW_RESULT ret = LW_RET_FAILED;

		FILE* fp = fopen(std::string{file}.c_str(), "wb");
		if (fp == NULL)
			return 0;

		DWORD version = EXP_OBJ_VERSION;
		fwrite(&version, sizeof(DWORD), 1, fp);

		if (LW_RESULT r = Corsairs::Engine::Render::LgoLoader::SaveAnimDataBone(*info, fp);
			LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] LgoLoader::SaveAnimDataBone failed: file='{}', ret={}",
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

		info->vertex_seq = LGO_NEW_ARRAY(lwVector3, info->vertex_num);
		memcpy(&info->vertex_seq[0], &mi->vertex_seq[0], sizeof(lwVector3) * info->vertex_num);

		info->face_num = mi->index_num / 3;
		info->face_seq = LGO_NEW_ARRAY(lwHelperMeshFaceInfo, info->face_num);

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
		int* mark_buf = LGO_NEW_ARRAY(int, info->face_num);
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
			dst->pos_seq = LGO_NEW_ARRAY(lwKeyDataVector3, dst->pos_num);
			memcpy(&dst->pos_seq[0], &src->pos_seq[0], sizeof(lwKeyDataVector3) * dst->pos_num);
		}

		if (dst->rot_num > 0) {
			dst->rot_seq = LGO_NEW_ARRAY(lwKeyDataQuaternion, dst->rot_num);
			memcpy(&dst->rot_seq[0], &src->rot_seq[0], sizeof(lwKeyDataQuaternion) * dst->rot_num);
		}

		if (dst->sca_num > 0) {
			dst->sca_seq = LGO_NEW_ARRAY(lwKeyDataVector3, dst->sca_num);
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

		vertex_seq = LGO_NEW_ARRAY(lwVector3, vertex_num);
		face_seq = LGO_NEW_ARRAY(lwHelperMeshFaceInfo, face_num);

		memcpy(vertex_seq, src->vertex_seq, sizeof(lwVector3) * vertex_num);
		memcpy(face_seq, src->face_seq, sizeof(lwHelperMeshFaceInfo) * face_num);

		return LW_RET_OK;
	}

	// lwHelperInfo
	// LW_STD_IMPLEMENTATION снято — наследование от lwIHelperInfo удалено.


	LW_RESULT lwHelperInfo::Copy(const lwHelperInfo* src) {
		type = src->type;

		if (type & HELPER_TYPE_DUMMY) {
			dummy_num = src->dummy_num;
			dummy_seq = LGO_NEW_ARRAY(lwHelperDummyInfo, dummy_num);
			memcpy(&dummy_seq[0], &src->dummy_seq[0], sizeof(lwHelperDummyInfo) * dummy_num);
		}
		if (type & HELPER_TYPE_BOX) {
			box_num = src->box_num;
			box_seq = LGO_NEW_ARRAY(lwHelperBoxInfo, box_num);
			memcpy(&box_seq[0], &src->box_seq[0], sizeof(lwHelperBoxInfo) * box_num);
		}
		if (type & HELPER_TYPE_MESH) {
			mesh_num = src->mesh_num;
			mesh_seq = LGO_NEW_ARRAY(lwHelperMeshInfo, mesh_num);

			for (DWORD i = 0; i < mesh_num; i++) {
				mesh_seq[i].Copy(&src->mesh_seq[i]);
			}
		}

		if (type & HELPER_TYPE_BOUNDINGBOX) {
			bbox_num = src->bbox_num;
			bbox_seq = LGO_NEW_ARRAY(lwBoundingBoxInfo, bbox_num);
			memcpy(&bbox_seq[0], &src->bbox_seq[0], sizeof(lwBoundingBoxInfo) * bbox_num);
		}
		if (type & HELPER_TYPE_BOUNDINGSPHERE) {
			bsphere_num = src->bsphere_num;
			bsphere_seq = LGO_NEW_ARRAY(lwBoundingSphereInfo, bsphere_num);
			memcpy(&bsphere_seq[0], &src->bsphere_seq[0], sizeof(lwBoundingSphereInfo) * bsphere_num);
		}

		return LW_RET_OK;
	}


	// lwGeomObjInfo
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


	// lwModelInfo
	static DWORD __tree_proc_modlinfo_destroy(lwITreeNode* node, void* param) {
		lwModelNodeInfo* data = (lwModelNodeInfo*)node->GetData();
		LW_IF_DELETE(data);

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
