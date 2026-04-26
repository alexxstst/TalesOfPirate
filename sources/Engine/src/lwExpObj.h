//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwMath.h"
#include "lwDirectX.h"
#include "lwClassDecl.h"
#include "lwITypes.h"
#include "lwITypes2.h"
#include "lwInterfaceExt.h"
#include "lwPoseCtrl.h"

LW_BEGIN
#define USE_ANIM_MAT43


	// begin version define
	// --------------------------------
	const DWORD EXP_OBJ_VERSION_0_0_0_0 = 0x0000;
	const DWORD EXP_OBJ_VERSION_1_0_0_0 = 0x1000;
	const DWORD EXP_OBJ_VERSION_1_0_0_1 = 0x1001;
	const DWORD EXP_OBJ_VERSION_1_0_0_2 = 0x1002;

	// bonequaternionpos
	const DWORD EXP_OBJ_VERSION_1_0_0_3 = 0x1003;
	// lwMeshInfobone_infl_factorvertex_decl_seq
	const DWORD EXP_OBJ_VERSION_1_0_0_4 = 0x1004;
	// lwAnimDataMtlOpacty
	const DWORD EXP_OBJ_VERSION_1_0_0_5 = 0x1005;

	const DWORD EXP_OBJ_VERSION = EXP_OBJ_VERSION_1_0_0_5;

	const DWORD MTLTEX_VERSION0000 = 0x0000;
	const DWORD MTLTEX_VERSION0001 = 0x0001;
	const DWORD MTLTEX_VERSION0002 = 0x0002;
	const DWORD MTLTEX_VERSION = MTLTEX_VERSION0002;

	const DWORD MESH_VERSION0000 = 0x0000;
	const DWORD MESH_VERSION0001 = 0x0001;
	const DWORD MESH_VERSION = MESH_VERSION0001;

	struct lwTexInfo_0000 {
		DWORD stage;
		DWORD colorkey_type;
		lwColorValue4b colorkey;
		D3DFORMAT format;
		char file_name[LW_MAX_NAME];

		lwTextureStageStateSetTex2 tss_set;
	};

	struct lwTexInfo_0001 {
		DWORD stage;
		DWORD level;
		DWORD usage;
		D3DFORMAT format;
		D3DPOOL pool;
		DWORD byte_alignment_flag;
		DWORD type; // file texture or user-defined texture
		// user-defined texture
		DWORD width;
		DWORD height;
		// file texture
		DWORD colorkey_type;
		lwColorValue4b colorkey;
		char file_name[LW_MAX_NAME];
		// Было `void* data;` — размер указателя отличается на x86/x64
		// и ломал разбор бинарных файлов, сохранённых в версии 0001 с x86-раскладкой.
		// Поле никогда не использовалось как реальный указатель.
		DWORD _reserved_data;

		lwTextureStageStateSetTex2 tss_set;
	};

	struct lwMtlTexInfo_0000 {
		lwMaterial mtl;
		lwRenderStateSetMtl2 rs_set;
		lwTexInfo tex_seq[LW_MAX_TEXTURESTAGE_NUM];

		lwMtlTexInfo_0000() {
			lwMaterial_Construct(&mtl);
			lwRenderStateSetTemplate_Construct(&rs_set);

			lwTexInfo_Construct(&tex_seq[0]);
			lwTexInfo_Construct(&tex_seq[1]);
			lwTexInfo_Construct(&tex_seq[2]);
			lwTexInfo_Construct(&tex_seq[3]);
		}
	};

	struct lwMtlTexInfo_0001 {
		float opacity;
		DWORD transp_type;
		lwMaterial mtl;
		lwRenderStateSetMtl2 rs_set;
		lwTexInfo tex_seq[LW_MAX_TEXTURESTAGE_NUM];

		lwMtlTexInfo_0001() {
			opacity = 1.0f;
			transp_type = MTLTEX_TRANSP_FILTER;
			lwMaterial_Construct(&mtl);
			lwRenderStateSetTemplate_Construct(&rs_set);

			lwTexInfo_Construct(&tex_seq[0]);
			lwTexInfo_Construct(&tex_seq[1]);
			lwTexInfo_Construct(&tex_seq[2]);
			lwTexInfo_Construct(&tex_seq[3]);
		}
	};

	struct lwMeshInfo_0000 {
		struct lwMeshInfoHeader {
			DWORD fvf;
			D3DPRIMITIVETYPE pt_type;

			DWORD vertex_num;
			DWORD index_num;
			DWORD subset_num;
			DWORD bone_index_num;

			lwRenderStateSetMesh2 rs_set;
		};

		union {
			lwMeshInfoHeader header;

			struct {
				DWORD fvf;
				D3DPRIMITIVETYPE pt_type;

				DWORD vertex_num;
				DWORD index_num;
				DWORD subset_num;
				DWORD bone_index_num;

				lwRenderStateSetMesh2 rs_set;
			};
		};

		lwVector3* vertex_seq;
		lwVector3* normal_seq;

		union {
			lwVector2* texcoord_seq[LW_MAX_TEXTURESTAGE_NUM];

			struct {
				lwVector2* texcoord0_seq;
				lwVector2* texcoord1_seq;
				lwVector2* texcoord2_seq;
				lwVector2* texcoord3_seq;
			};
		};

		DWORD* vercol_seq;
		DWORD* index_seq;
		lwBlendInfo* blend_seq;


		lwSubsetInfo subset_seq[LW_MAX_SUBSET_NUM];
		BYTE bone_index_seq[LW_MAX_BONE_NUM];


		lwMeshInfo_0000()
			: vertex_seq(0), normal_seq(0), vercol_seq(0), index_seq(0), blend_seq(0),
			  texcoord0_seq(0), texcoord1_seq(0), texcoord2_seq(0), texcoord3_seq(0),
			  vertex_num(0), index_num(0), subset_num(0), bone_index_num(0) {
			memset(subset_seq, 0, sizeof(lwSubsetInfo) * LW_MAX_SUBSET_NUM);
			memset(bone_index_seq, 0, sizeof(BYTE) * LW_MAX_BONE_NUM);
			lwRenderStateSetTemplate_Construct(&rs_set);
		}

		~lwMeshInfo_0000() {
			LW_IF_DELETE_A(vertex_seq);
			LW_IF_DELETE_A(normal_seq);
			LW_IF_DELETE_A(vercol_seq);
			LW_IF_DELETE_A(texcoord0_seq);
			LW_IF_DELETE_A(texcoord1_seq);
			LW_IF_DELETE_A(texcoord2_seq);
			LW_IF_DELETE_A(texcoord3_seq);
			LW_IF_DELETE_A(index_seq);
			LW_IF_DELETE_A(blend_seq);
		}
	};

	struct lwMeshInfo_0003 {
		struct lwMeshInfoHeader {
			DWORD fvf;
			D3DPRIMITIVETYPE pt_type;

			DWORD vertex_num;
			DWORD index_num;
			DWORD subset_num;
			DWORD bone_index_num;

			lwRenderStateAtom rs_set[LW_MESH_RS_NUM];
		};

		union {
			lwMeshInfoHeader header;

			struct {
				DWORD fvf;
				D3DPRIMITIVETYPE pt_type;

				DWORD vertex_num;
				DWORD index_num;
				DWORD subset_num;
				DWORD bone_index_num;

				lwRenderStateAtom rs_set[LW_MESH_RS_NUM];
			};
		};

		lwVector3* vertex_seq;
		lwVector3* normal_seq;

		union {
			lwVector2* texcoord_seq[LW_MAX_TEXTURESTAGE_NUM];

			struct {
				lwVector2* texcoord0_seq;
				lwVector2* texcoord1_seq;
				lwVector2* texcoord2_seq;
				lwVector2* texcoord3_seq;
			};
		};

		DWORD* vercol_seq;
		DWORD* index_seq;
		lwBlendInfo* blend_seq;


		lwSubsetInfo subset_seq[LW_MAX_SUBSET_NUM];
		BYTE bone_index_seq[LW_MAX_BONE_NUM];


		lwMeshInfo_0003()
			: vertex_seq(0), normal_seq(0), vercol_seq(0), index_seq(0), blend_seq(0),
			  texcoord0_seq(0), texcoord1_seq(0), texcoord2_seq(0), texcoord3_seq(0),
			  vertex_num(0), index_num(0), subset_num(0), bone_index_num(0) {
			memset(subset_seq, 0, sizeof(lwSubsetInfo) * LW_MAX_SUBSET_NUM);
			memset(bone_index_seq, 0, sizeof(BYTE) * LW_MAX_BONE_NUM);
			lwRenderStateAtom_Construct_A(rs_set, LW_MESH_RS_NUM);
		}

		~lwMeshInfo_0003() {
			LW_IF_DELETE_A(vertex_seq);
			LW_IF_DELETE_A(normal_seq);
			LW_IF_DELETE_A(vercol_seq);
			LW_IF_DELETE_A(texcoord0_seq);
			LW_IF_DELETE_A(texcoord1_seq);
			LW_IF_DELETE_A(texcoord2_seq);
			LW_IF_DELETE_A(texcoord3_seq);
			LW_IF_DELETE_A(index_seq);
			LW_IF_DELETE_A(blend_seq);
		}
	};

	struct lwHelperDummyInfo_1000 {
		DWORD id;
		lwMatrix44 mat;
	};

	class lwBox_1001 {
	public:
		lwVector3 p;
		lwVector3 s;

	public:
		lwBox_1001() {
		}

		lwBox_1001(float x, float y, float z, float sx, float sy, float sz)
			: p(x, y, z), s(sx, sy, sz) {
		}

		lwBox_1001(const lwVector3& pos, const lwVector3& size)
			: p(pos), s(size) {
		}

		lwBox_1001(const lwBox_1001& box)
			: p(box.p), s(box.s) {
		}

		lwVector3* GetPointWithMask(lwVector3* out_v, DWORD mask) const {
			out_v->x = (mask & 0x100) ? (p.x + s.x) : p.x;
			out_v->y = (mask & 0x010) ? (p.y + s.y) : p.y;
			out_v->z = (mask & 0x001) ? (p.z + s.z) : p.z;

			return out_v;
		}
	};

	// --------------------------------
	// end
	enum {
		MODEL_OBJ_TYPE_GEOMETRY = 1,
		MODEL_OBJ_TYPE_HELPER = 2,
	};

	// material and texture struct definition

	enum lwBoneKeyInfoType {
		BONE_KEY_TYPE_MAT43 = 1,
		BONE_KEY_TYPE_MAT44 = 2,
		BONE_KEY_TYPE_QUAT = 3,
		BONE_KEY_TYPE_INVALID = LW_INVALID_INDEX,
	};

	struct lwPoseInfoSet {
		lwPoseInfoSet()
			: pose_seq(0), pose_num(0) {
		}

		~lwPoseInfoSet() {
			LW_SAFE_DELETE_A(pose_seq);
		}

		lwPoseInfo* pose_seq;
		DWORD pose_num;
	};

	struct lwHelperInfo : public lwIHelperInfo {
		LW_STD_DECLARATION()

		lwHelperInfo()
			: type(HELPER_TYPE_INVALID), mesh_seq(0), box_seq(0), dummy_seq(0), bbox_seq(0), bsphere_seq(0),
			  mesh_num(0), box_num(0), dummy_num(0), bbox_num(0), bsphere_num(0) {
		}

		~lwHelperInfo() {
			LW_SAFE_DELETE_A(dummy_seq);
			LW_SAFE_DELETE_A(box_seq);
			LW_SAFE_DELETE_A(mesh_seq);

			LW_SAFE_DELETE_A(bbox_seq);
			LW_SAFE_DELETE_A(bsphere_seq);
		}

		DWORD type;

		lwHelperDummyInfo* dummy_seq;
		lwHelperBoxInfo* box_seq;
		lwHelperMeshInfo* mesh_seq;
		lwBoundingBoxInfo* bbox_seq;
		lwBoundingSphereInfo* bsphere_seq;

		DWORD dummy_num;
		DWORD box_num;
		DWORD mesh_num;
		DWORD bbox_num;
		DWORD bsphere_num;

		// begin load item
		LW_RESULT _LoadHelperDummyInfo(FILE* fp, DWORD version);
		LW_RESULT _LoadHelperBoxInfo(FILE* fp, DWORD version);
		LW_RESULT _LoadHelperMeshInfo(FILE* fp, DWORD version);
		LW_RESULT _LoadBoundingBoxInfo(FILE* fp, DWORD version);
		LW_RESULT _LoadBoundingSphereInfo(FILE* fp, DWORD version);

		LW_RESULT _SaveHelperDummyInfo(FILE* fp) const;
		LW_RESULT _SaveHelperBoxInfo(FILE* fp) const;
		LW_RESULT _SaveHelperMeshInfo(FILE* fp) const;
		LW_RESULT _SaveBoundingBoxInfo(FILE* fp) const;
		LW_RESULT _SaveBoundingSphereInfo(FILE* fp) const;
		// end

		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp) const;

		LW_RESULT Copy(const lwHelperInfo* src);
		DWORD GetDataSize() const;
	};


	// bone-skin struct definition
	struct lwBoneBaseInfo {
		char name[LW_MAX_NAME];
		DWORD id;
		DWORD parent_id;
	};

	struct lwBoneDummyInfo {
		DWORD id;
		DWORD parent_bone_id;
		lwMatrix44 mat;
	};

	struct lwBoneKeyInfo {
		lwMatrix43* mat43_seq;
		lwMatrix44* mat44_seq;

		struct {
			lwVector3* pos_seq;
			lwQuaternion* quat_seq;
		};

		lwBoneKeyInfo()
			: mat43_seq(0), mat44_seq(0), quat_seq(0), pos_seq(0) {
		}

		~lwBoneKeyInfo() {
			LW_SAFE_DELETE_A(mat43_seq);
			LW_SAFE_DELETE_A(mat44_seq);
			LW_SAFE_DELETE_A(quat_seq);
			LW_SAFE_DELETE_A(pos_seq);
		}
	};


	class lwAnimDataBone : public lwIAnimDataBone {
		LW_STD_DECLARATION()

	protected:
	public:
		struct lwBoneInfoHeader {
			DWORD bone_num;
			DWORD frame_num;
			DWORD dummy_num;
			DWORD key_type;
		};

		union {
			lwBoneInfoHeader _header;

			struct {
				DWORD _bone_num;
				DWORD _frame_num;
				DWORD _dummy_num;
				DWORD _key_type;
			};
		};

		lwBoneBaseInfo* _base_seq;
		lwBoneDummyInfo* _dummy_seq;
		lwBoneKeyInfo* _key_seq;
		lwMatrix44* _invmat_seq;

	public:
		lwAnimDataBone();
		virtual ~lwAnimDataBone();


		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp) const;
		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file) const;
		LW_RESULT Destroy();

		LW_RESULT Copy(const lwAnimDataBone* src);

		DWORD GetDataSize() const;

		// start_frame,end_frame frame
		LW_RESULT GetValue(lwMatrix44* mat, DWORD bone_id, float frame, DWORD start_frame, DWORD end_frame);

		inline void SetBoneNum(DWORD num) {
			_bone_num = num;
		}

		inline void SetFrameNum(DWORD num) {
			_frame_num = num;
		}

		inline void SetKeyType(DWORD type) {
			_key_type = type;
		}

		inline DWORD GetBoneNum() const {
			return _bone_num;
		}

		inline DWORD GetFrameNum() const {
			return _frame_num;
		}

		inline DWORD GetDummyNum() const {
			return _dummy_num;
		}

		inline DWORD GetKeyType() const {
			return _key_type;
		};

		inline lwBoneBaseInfo* GetBaseInfo() {
			return _base_seq;
		}

		inline lwBoneKeyInfo* GetKeyInfo() {
			return _key_seq;
		}

		inline lwMatrix44* GetInitInvMat() {
			return _invmat_seq;
		}

		inline lwBoneDummyInfo* GetDummyInfo() {
			return _dummy_seq;
		}
	};


	class lwAnimDataMatrix : public lwIAnimDataMatrix {
		LW_STD_DECLARATION()

	public:
		lwMatrix43* _mat_seq;
		DWORD _frame_num;

	public:
		lwAnimDataMatrix();
		virtual ~lwAnimDataMatrix();


		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp) const;
		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file) const;
		LW_RESULT Copy(const lwAnimDataMatrix* src);

		DWORD GetDataSize() const;

		DWORD GetFrameNum() {
			return _frame_num;
		}

		LW_RESULT GetValue(lwMatrix44* mat, float frame);
	};

	struct lwKeyDataVector3 {
		DWORD key;
		lwVector3 data;
	};

	struct lwKeyDataQuaternion {
		DWORD key;
		lwQuaternion data;
	};

	struct lwKeyDataMatrix43 {
		DWORD key;
		lwMatrix43 data;
	};

	struct lwAnimKeySetPRS {
		DWORD _interpolate_type;

		lwKeyDataVector3* pos_seq;
		lwKeyDataQuaternion* rot_seq;
		lwKeyDataVector3* sca_seq;
		DWORD pos_num;
		DWORD rot_num;
		DWORD sca_num;
		DWORD frame_num;

		lwAnimKeySetPRS()
			: pos_seq(0), rot_seq(0), sca_seq(0), pos_num(0), rot_num(0), sca_num(0), frame_num(0) {
		}

		virtual ~lwAnimKeySetPRS() {
			LW_SAFE_DELETE_A(pos_seq);
			LW_SAFE_DELETE_A(rot_seq);
			LW_SAFE_DELETE_A(sca_seq);
		}

		LW_RESULT GetValue(lwMatrix44* mat, float frame);
	};

	struct lwKeyDataTexUV {
		float u;
		float v;
		float w_angle;
	};

	struct lwAnimDataTexUV : public lwIAnimDataTexUV {
		LW_STD_DECLARATION()

	public:
		lwMatrix44* _mat_seq;
		DWORD _frame_num;

	public:
		lwAnimDataTexUV() : _mat_seq(0), _frame_num(0)/*, _subset_type(LW_INVALID_INDEX)*/ {
		}

		virtual ~lwAnimDataTexUV() {
			LW_SAFE_DELETE_A(_mat_seq);
		}


		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp) const;
		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file) const;

		LW_RESULT Copy(const lwAnimDataTexUV* src);

		DWORD GetDataSize() const;

		LW_RESULT GetValue(lwMatrix44* mat, float frame);

		LW_RESULT SetData(const lwMatrix44* mat_seq, DWORD mat_num) {
			LW_SAFE_DELETE_A(_mat_seq);
			_frame_num = mat_num;
			_mat_seq = LW_NEW(lwMatrix44[_frame_num]);
			memcpy(_mat_seq, mat_seq, sizeof(lwMatrix44) * _frame_num);
			return LW_RET_OK;
		}
	};

	class lwAnimDataTexImg : public lwIAnimDataTexImg {
		LW_STD_DECLARATION();

	public:
		lwTexInfo* _data_seq;
		DWORD _data_num;
		char _tex_path[LW_MAX_PATH];

	public:
		lwAnimDataTexImg()
			: _data_seq(0), _data_num(0) {
		}

		virtual ~lwAnimDataTexImg() {
			LW_IF_DELETE_A(_data_seq);
		}


		LW_RESULT Copy(const lwAnimDataTexImg* src);

		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp) const;
		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file) const;

		DWORD GetDataSize() const;
	};


	struct lwAnimDataMtlOpacity : public lwIAnimDataMtlOpacity {
		LW_STD_DECLARATION();

	private:
		lwIAnimKeySetFloat* _aks_ctrl;

	public:
		lwAnimDataMtlOpacity() : _aks_ctrl(0) {
		}

		virtual ~lwAnimDataMtlOpacity() {
			LW_SAFE_RELEASE(_aks_ctrl);
		}

		LW_RESULT Clone(lwIAnimDataMtlOpacity** obj);
		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp);
		DWORD GetDataSize();

		lwIAnimKeySetFloat* GetAnimKeySet() {
			return _aks_ctrl;
		}

		void SetAnimKeySet(lwIAnimKeySetFloat* aks_ctrl) {
			_aks_ctrl = aks_ctrl;
		}
	};


	enum {
		ANIM_DATA_BONE = 0,
		ANIM_DATA_MAT = 1,
		ANIM_DATA_SUBMTL0_TEXUV = 2,
		ANIM_DATA_SUBMTL0_TEXIMG = 66,
	};

	class lwAnimDataInfo : public lwIAnimDataInfo {
		LW_STD_DECLARATION()

		enum { ANIM_DATA_NUM = 2 + LW_MAX_SUBSET_NUM + LW_MAX_SUBSET_NUM * LW_MAX_TEXTURESTAGE_NUM * 2 };

	public:
		lwAnimDataBone* anim_bone;

#ifdef USE_ANIMKEY_PRS
		lwAnimKeySetPRS* anim_mat;
#else
		lwAnimDataMatrix* anim_mat;
#endif

		lwAnimDataMtlOpacity* anim_mtlopac[LW_MAX_SUBSET_NUM];
		lwAnimDataTexUV* anim_tex[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM];
		lwAnimDataTexImg* anim_img[LW_MAX_SUBSET_NUM][LW_MAX_TEXTURESTAGE_NUM];

	public:
		lwAnimDataInfo();
		virtual ~lwAnimDataInfo();

		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp);

		LW_RESULT GetDataSize() const;
	};


	LW_RESULT lwMeshInfo_Copy(lwMeshInfo* dst, const lwMeshInfo* src);
	LW_RESULT lwMeshInfo_Load(lwMeshInfo* info, FILE* fp, DWORD version);
	LW_RESULT lwMeshInfo_Save(lwMeshInfo* info, FILE* fp);
	DWORD lwMeshInfo_GetDataSize(lwMeshInfo* info);

	struct lwGeomObjInfo : public lwIGeomObjInfo {
		LW_STD_DECLARATION()

		struct lwGeomObjInfoHeader {
			DWORD id;
			DWORD parent_id;
			DWORD type;
			lwMatrix44 mat_local;
			lwRenderCtrlCreateInfo rcci;
			lwStateCtrl state_ctrl;

			DWORD mtl_size;
			DWORD mesh_size;
			DWORD helper_size;
			DWORD anim_size;
		};

		struct {
			DWORD id;
			DWORD parent_id;
			DWORD type;
			lwMatrix44 mat_local;
			lwRenderCtrlCreateInfo rcci;
			lwStateCtrl state_ctrl;

			DWORD mtl_size;
			DWORD mesh_size;
			DWORD helper_size;
			DWORD anim_size;
		};

		DWORD mtl_num;
		lwMtlTexInfo* mtl_seq; //[LW_MAX_SUBSET_NUM];

		lwMeshInfo mesh;

		// helper data
		lwHelperInfo helper_data;

		// animation data
		lwAnimDataInfo anim_data;


		lwGeomObjInfo()
			: id(LW_INVALID_INDEX), mtl_num(0), type(GEOMOBJ_TYPE_GENERIC),
			  mtl_size(0), mesh_size(0), helper_size(0), anim_size(0),
			  mtl_seq(0) {
			lwRenderCtrlCreateInfo_Construct(&rcci);
		}

		virtual ~lwGeomObjInfo() {
			LW_IF_DELETE_A(mtl_seq);
		}

		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp);
		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file);
		DWORD GetDataSize() const;

		virtual lwMeshInfo* GetMeshInfo() {
			return &mesh;
		}

		lwMtlTexInfo* GetMtlTexInfo() {
			return mtl_seq;
		}
	};

	typedef lwGeomObjInfo::lwGeomObjInfoHeader lwGeomObjInfoHeader;

	struct lwModelObjInfo : public lwIModelObjInfo {
		LW_STD_DECLARATION()

		struct lwModelObjInfoHeader {
			DWORD type; // helper or geometry;
			DWORD addr;
			DWORD size;
		};

		DWORD geom_obj_num;
		lwGeomObjInfo* geom_obj_seq[LW_MAX_MODEL_GEOM_OBJ_NUM];

		// helper data
		lwHelperInfo helper_data;

	public:
		lwModelObjInfo()
			: geom_obj_num(0) {
			memset(geom_obj_seq, 0, sizeof(geom_obj_seq));
		}

		virtual ~lwModelObjInfo() {
			for (DWORD i = 0; i < LW_MAX_MODEL_GEOM_OBJ_NUM; i++) {
				LW_SAFE_DELETE(geom_obj_seq[i]);
			}
		}

		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file);

		LW_RESULT SortGeomObjInfoWithID();

		DWORD GetDataSize();

		static LW_RESULT GetHeader(lwModelObjInfoHeader* header_seq, DWORD* header_num, const char* file);
	};

	typedef lwModelObjInfo::lwModelObjInfoHeader lwModelObjInfoHeader;

	class lwHelperDummyObjInfo : public lwIHelperDummyObjInfo {
		LW_STD_DECLARATION()

	private:
		DWORD _id;
		lwMatrix44 _mat;
		lwIAnimDataMatrix* _anim_data;

	public:
		lwHelperDummyObjInfo();
		virtual ~lwHelperDummyObjInfo();

		void SetID(DWORD id) {
			_id = id;
		}

		void SetMatrix(const lwMatrix44* mat) {
			_mat = *mat;
		}

		void SetAnimDataMatrix(lwIAnimDataMatrix* anim_data) {
			_anim_data = anim_data;
		}

		DWORD GetID() const {
			return _id;
		}

		lwMatrix44* GetMatrix() {
			return &_mat;
		}

		lwIAnimDataMatrix* GetAnimDataMatrix() {
			return _anim_data;
		}

		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp);
	};

	struct lwModelNodeHeadInfo {
		DWORD handle;
		DWORD type;
		DWORD id;
		char descriptor[64];
		DWORD parent_handle;
		DWORD link_parent_id;
		DWORD link_id;
	};

	class lwModelNodeInfo {
	public:
		// base
		union {
			lwModelNodeHeadInfo _head;

			struct {
				DWORD _handle;
				DWORD _type;
				DWORD _id;
				char _descriptor[64];
				DWORD _parent_handle;
				DWORD _link_parent_id;
				DWORD _link_id;
			};
		};

		void* _data;
		void* _param;

	public:
		lwModelNodeInfo()
			: _handle(LW_INVALID_INDEX), _type(MODELNODE_INVALID), _id(LW_INVALID_INDEX),
			  _parent_handle(LW_INVALID_INDEX),
			  _link_parent_id(LW_INVALID_INDEX), _link_id(LW_INVALID_INDEX),
			  _data(0), _param(0) {
			_descriptor[0] = '\0';
		}

		virtual ~lwModelNodeInfo();

		LW_RESULT Load(FILE* fp, DWORD version);
		LW_RESULT Save(FILE* fp);
	};

	struct lwModelHeadInfo {
		DWORD mask;
		DWORD version;
		char decriptor[64];
	};


	const DWORD MODELINFO_VERSION_0001 = 0x0001;
	const DWORD MODELINFO_VERSION_0002 = 0x0002;
	const DWORD MODELINFO_VERSION = EXP_OBJ_VERSION; //MODELINFO_VERSION_0002;

	class lwModelInfo {
	public:
		lwModelHeadInfo _head;
		lwITreeNode* _obj_tree;

	public:
		lwModelInfo()
			: _obj_tree(0) {
			memset(&_head, 0, sizeof(_head));
		}

		virtual ~lwModelInfo();

		LW_RESULT Destroy();
		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file);
		LW_RESULT SortChildWithID();
	};

	// =============================================


	DWORD lwGetMtlTexInfoSize(const lwMtlTexInfo* info_seq, DWORD num);
	DWORD lwGetAnimKeySetPRSSize(const lwAnimKeySetPRS* info);

	LW_RESULT lwCreateHelperMeshInfo(lwHelperMeshInfo* info, const lwMeshInfo* mi);

	LW_RESULT lwCopyAnimKeySetPRS(lwAnimKeySetPRS* dst, const lwAnimKeySetPRS* src);

LW_END
