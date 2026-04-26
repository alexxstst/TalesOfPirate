//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwDirectX.h"
#include "lwMath.h"
#include "lwErrorCode.h"

LW_BEGIN
#define LW_MAX_SUBSET_NUM                   16
#define LW_MAX_BLENDWEIGHT_NUM              4
#define LW_MAX_BONE_NUM                     25
#define LW_MAX_BONEDUMMY_NUM                64
#define LW_MAX_MODEL_GEOM_OBJ_NUM           32
#define LW_MAX_MODEL_OBJ_NUM                LW_MAX_MODEL_GEOM_OBJ_NUM + 1 // geometry_num + helper
#define LW_MAX_GEOM_DUMMY_NUM               8
#define LW_MAX_OBJDUMMY_NUM                 8
#define LW_MAX_HELPER_MESH_NUM              8
#define LW_MAX_HELPER_BOX_NUM               8
#define LW_MAX_LINK_ITEM_NUM                16

#define LW_MAX_RENDERSTATE_NUM          210 // dx9
#define LW_MAX_TEXTURESTAGE_NUM         4   // max 8
#define LW_MAX_SAMPLESTAGE_NUM          4
#define LW_MAX_TEXTURESTAGESTATE_NUM    33
#define LW_MAX_SAMPLESTATE_NUM          14

#define LW_INVALID_RS_VALUE             D3DRS_FORCE_DWORD
#define LW_INVALID_TSS_VALUE            D3DTSS_FORCE_DWORD
#define LW_INVALID_SS_VALUE             D3DSAMP_FORCE_DWORD

#define LW_TEX_TSS_NUM                  8
#define LW_TEX_RS_NUM                   2
#define LW_MTL_RS_NUM                   8
#define LW_MESH_RS_NUM                  8
#define LW_TEX_DTSS_NUM                 4
#define LW_TEX_DRS_NUM                  4
#define LW_MESH_DRS_NUM                 4
#define LW_MTL_RSA_NUM                  4
#define LW_MAX_STREAM_NUM               8
#define LW_MAX_LIGHT_NUM                8
#define LW_MAX_SUBSKIN_NUM              10
#define LW_MAX_MTL_TEX_NUM              4


#define LW_MAX_BOUNDING_OBJ_NUM         4
#define LW_MAX_BOUNDING_BOX_NUM         8
#define LW_MAX_BOUNDING_SPHERE_NUM      8


#define LW_MAX_ITEM_LINK_NUM            8

#define LW_RENDER_CTRL_PROC_NUM         512

	// PrimitiveLW_MAX_OBJ_DUMMY_NUMDummyDummy
	// LW_MAX_OBJ_DUMMY_INV_NUMmaxdummydummyid
	// dummy_0 - dummy_3LW_MAX_OBJ_DUMMY_INV_NUM
	//               dummy_4 - dummy_7Inverse Matrixid
#define LW_MAX_OBJ_DUMMY_NUM            16
#define LW_MAX_OBJ_DUMMY_INV_NUM        4

	// Pose Ctrl Info
#define MAX_KEY_FRAME_NUM           8

	enum {
		HELPER_TYPE_DUMMY = 0x0001,
		HELPER_TYPE_BOX = 0x0002,
		HELPER_TYPE_MESH = 0x0004,
		HELPER_TYPE_BOUNDINGBOX = 0x0010,
		HELPER_TYPE_BOUNDINGSPHERE = 0x0020,
		HELPER_TYPE_INVALID = 0x0000
	};

	enum {
		GEOMOBJ_TYPE_GENERIC = 0,
		GEOMOBJ_TYPE_CHECK_BB = 1, //
		GEOMOBJ_TYPE_CHECK_BB2 = 2, //( 0, 1, 0 )
	};

	enum {
		TSS_BEGIN = 0,
		TSS_END = 1,
		RS_BEGIN = 0,
		RS_END = 1,
	};

	enum {
		ANIM_CTRL_TYPE_MATRIX = 0,
		ANIM_CTRL_TYPE_BONE = 1,
		ANIM_CTRL_TYPE_TEXCOORD = 2,
		ANIM_CTRL_TYPE_TEXIMG = 3,
		ANIM_CTRL_TYPE_MTLOPACITY = 4,

		ANIM_CTRL_TYPE_NUM,

		ANIM_CTRL_TYPE_MAT = ANIM_CTRL_TYPE_MATRIX,
		ANIM_CTRL_TYPE_TEXUV = ANIM_CTRL_TYPE_TEXCOORD,

		ANIM_CTRL_TYPE_INVALID = 0xffffffff
	};

	// resource file type enumulation
	// *.lmo, *.lgo, *.lab, *.lam, *.lat
	enum lwResourceFileType {
		RES_FILE_TYPE_GENERIC = 0,
		RES_FILE_TYPE_MODEL = 1,
		RES_FILE_TYPE_GEOMETRY = 2,
		RES_FILE_TYPE_INVALID = 0xffffffff
	};

	struct lwRenderCtrlCreateInfo {
		DWORD ctrl_id;
		DWORD decl_id;
		DWORD vs_id;
		DWORD ps_id;
	};

	inline void lwRenderCtrlCreateInfo_Construct(lwRenderCtrlCreateInfo* info) {
		info->ctrl_id = LW_INVALID_INDEX;
		info->decl_id = LW_INVALID_INDEX;
		info->vs_id = LW_INVALID_INDEX;
		info->ps_id = LW_INVALID_INDEX;
	}

	class lwIRenderCtrlVS;
	typedef lwIRenderCtrlVS* (*lwRenderCtrlVSCreateProc)();

	enum lwPathInfoType {
		PATH_TYPE_DEFAULT = 0x0000,
		PATH_TYPE_WORKINGDIRECTORY,
		PATH_TYPE_DATA,

		PATH_TYPE_SCRIPT,
		PATH_TYPE_SOUND,
		PATH_TYPE_SHADER,

		PATH_TYPE_ANIMATION,

		PATH_TYPE_MODEL,
		PATH_TYPE_MODEL_CHARACTER,
		PATH_TYPE_MODEL_SCENE,
		PATH_TYPE_MODEL_ITEM,

		PATH_TYPE_TEXTURE,
		PATH_TYPE_TEXTURE_CHARACTER,
		PATH_TYPE_TEXTURE_SCENE,
		PATH_TYPE_TEXTURE_ITEM,

		PATH_TYPE_NUM,
	};

	enum lwColorKeyTypeEnum {
		COLORKEY_TYPE_NONE = 0,
		COLORKEY_TYPE_COLOR = 1,
		COLORKEY_TYPE_PIXEL = 2,
	};


#define LW_COLOR_R( c ) (BYTE)( (c) & 0xff )
#define LW_COLOR_G( c ) (BYTE)( ((c) >> 8) & 0xff )
#define LW_COLOR_B( c ) (BYTE)( ((c) >> 16) & 0xff )
#define LW_COLOR_A( c ) (BYTE)( ((c) >> 24) & 0xff )

	enum lwPlayPoseEnum {
		PLAY_ONCE = 1,
		PLAY_LOOP = 2,
		PLAY_FRAME = 3,
		PLAY_ONCE_SMOOTH = 4,
		PLAY_LOOP_SMOOTH = 5,
		PLAY_PAUSE = 6,
		PLAY_CONTINUE = 7,
		PLAY_INVALID = 0,
	};

	enum {
		KEYFRAME_TYPE_BEGIN = 0,
		KEYFRAME_TYPE_END = 1,
		KEYFRAME_TYPE_KEY = 2,
		KEYFRAME_TYPE_INVALID = 0xffffffff,
	};

	enum {
		OBJ_TYPE_MODEL = 0,
		OBJ_TYPE_CHARACTER = 1,
		OBJ_TYPE_ITEM = 2,
		OBJ_TYPE_INVALID = 0xffffffff,
	};

	struct lwPickInfo {
		DWORD obj_id;
		DWORD face_id;
		float dis;
		lwVector3 pos;
		DWORD data;
	};

	struct lwPoseInfo {
		DWORD charType{};
		DWORD start{};
		DWORD end{};
		DWORD key_frame_seq[MAX_KEY_FRAME_NUM]{};
		DWORD key_frame_num{};
	};


	typedef void (*lwPoseKeyFrameProc)(DWORD type, DWORD pose_id, DWORD key_id, DWORD key_frame, void* param);

	enum lwPlayPoseInfoMask {
		PPI_MASK_POSE = 0x0001,
		PPI_MASK_TYPE = 0x0002,
		PPI_MASK_VELOCITY = 0x0004,
		PPI_MASK_FRAME = 0x0008,
		PPI_MASK_PROC = 0x0010,
		PPI_MASK_DATA = 0x0020,
		PPI_MASK_BLENDINFO = 0x0100,
		PPI_MASK_BLENDINFO_SRCFRAMENUM = 0x0200,
		PPI_MASK_BLENDINFO_DSTFRAMENUM = 0x0400,

		PPI_MASK_INVALID = 0x0000,
		PPI_MASK_DEFAULT = (PPI_MASK_POSE | PPI_MASK_TYPE | PPI_MASK_VELOCITY | PPI_MASK_FRAME | PPI_MASK_DATA),
		PPI_MASK_ABSOLUTE = (PPI_MASK_DEFAULT | PPI_MASK_PROC),
	};


	struct lwPoseBlendInfo {
		DWORD op_state;
		float op_frame_length;
		float weight;
		float factor;
		float src_frame_num;
		float dst_frame_num;
		BYTE* bone_data;
		DWORD blend_pose;
		DWORD blend_type;
		float blend_frame;
		float blend_ret_frame;
	};

	struct lwPlayPoseInfo {
		DWORD bit_mask;

		DWORD pose;
		DWORD type;
		DWORD data;
		float velocity;
		float frame;
		float ret_frame;
		lwPoseKeyFrameProc proc;
		void* proc_param;
		lwPoseBlendInfo blend_info;
	};



	inline void lwPlayPoseInfo_Construct(lwPlayPoseInfo* info) {
		memset(info, 0, sizeof(lwPlayPoseInfo));
		info->bit_mask = PPI_MASK_INVALID;
	}

	struct lwColorValue4f {
		float r;
		float g;
		float b;
		float a;
	};

	inline void lwColorValue4f_Construct(lwColorValue4f* obj, float r, float g, float b, float a) {
		obj->r = r;
		obj->g = g;
		obj->b = b;
		obj->a = a;
	}

	inline lwColorValue4f lwColorValue4f_Construct(float r, float g, float b, float a) {
		lwColorValue4f c;
		c.r = r;
		c.g = g;
		c.b = b;
		c.a = a;
		return c;
	}

	struct lwColorValue4b {
		union {
			struct {
				std::uint8_t b;
				std::uint8_t g;
				std::uint8_t r;
				std::uint8_t a;
			};

			std::uint32_t color;
		};
	};

	struct lwMaterial {
		lwColorValue4f dif;
		lwColorValue4f amb;
		lwColorValue4f spe;
		lwColorValue4f emi;
		float power;
	};

	inline void lwMaterial_Construct(lwMaterial* mtl) {
		memset(mtl, 0, sizeof(lwMaterial));
		mtl->amb.a = mtl->amb.r = mtl->amb.g = mtl->amb.b = 1.0f;
		mtl->dif.a = mtl->dif.r = mtl->dif.g = mtl->dif.b = 1.0f;
	}

	struct lwRenderStateValue {
		DWORD state;
		DWORD value;
	};

	inline lwRenderStateValue RS_VALUE(DWORD state, DWORD value) {
		lwRenderStateValue ret = {state, value};
		return ret;
	}

	class lwIItem;

	struct lwItemLinkInfo {
		lwIItem* obj;
		DWORD id;
		DWORD link_item_id;
		DWORD link_parent_id;
	};

	struct lwResFile {
		char file_name[LW_MAX_PATH];
		lwResourceFileType res_type;
		DWORD obj_id;

		lwResFile()
			: obj_id(0), res_type(RES_FILE_TYPE_INVALID) {
			file_name[0] = '\0';
		}
	};

	struct lwResFileMesh {
		char file_name[LW_MAX_PATH];
		lwResourceFileType res_type;
		DWORD obj_id;

		lwResFileMesh()
			: obj_id(0), res_type(RES_FILE_TYPE_INVALID) {
			file_name[0] = '\0';
		}

		int Compare(const lwResFileMesh* src) {
			return (obj_id == src->obj_id)
				&& (res_type == src->res_type)
				&& (_tcscmp(file_name, src->file_name) == 0);
		}
	};

	struct lwResFileAnimData {
		char file_name[LW_MAX_PATH];
		lwResourceFileType res_type;
		DWORD obj_id;
		DWORD anim_type;

		lwResFileAnimData()
			: obj_id(0), res_type(RES_FILE_TYPE_INVALID), anim_type(ANIM_CTRL_TYPE_INVALID) {
			file_name[0] = '\0';
		}

		int Compare(const lwResFileAnimData* src) {
			return (obj_id == src->obj_id)
				&& (anim_type == src->anim_type)
				&& (res_type == src->res_type)
				&& (_tcscmp(file_name, src->file_name) == 0);
		}
	};


	enum lwAnimCtrlAgentSetType {
		ACA_GLOBAL_SET,
		ACA_SUBSET_SET,
	};

	typedef LW_RESULT (*lwOutputLoseDeviceProc)();
	typedef LW_RESULT (*lwOutputResetDeviceProc)();

	// -------------------
	struct lwKeyFloat {
		typedef float DATA_TYPE;

		DWORD key;
		DWORD slerp_type;
		DATA_TYPE data;
	};

	struct lwKeyVector3 {
		DWORD key;
		DWORD slerp_type;
		lwVector3 data;
	};

	struct lwKeyQuaternion {
		DWORD key;
		DWORD slerp_type;
		lwQuaternion data;
	};

	struct lwKeyMatrix43 {
		DWORD key;
		DWORD slerp_type;
		lwMatrix43 data;
	};

	enum lwAnimKeySetMask {
		AKSM_KEY = 0x0001,
		AKSM_DATA = 0x0002,
		AKSM_SLERPTYPE = 0x0004,
	};

	enum lwAnimKeySetSlerpType {
		AKST_INVALID = 0,

		__akst_begin = 0,

		AKST_LINEAR,
		AKST_SIN1, // 0 - 90
		AKST_SIN2, // 90 - 180
		AKST_SIN3, // 180 - 270
		AKST_SIN4, // 270 - 360
		AKST_COS1, // 0 - 90
		AKST_COS2, // 90 - 180
		AKST_COS3, // 180 - 270
		AKST_COS4, // 270 - 360
		AKST_TAN1, // 0 - 45
		AKST_CTAN1, // 0 - 45

		__akst_end
	};

	// end

	enum lwStreamTypeEnum {
		STREAM_STATIC = 0,
		STREAM_DYNAMIC = 1,
		STREAM_LOCKABLE = 2,
		STREAM_GENERIC = 3,
	};

	struct lwWndInfo {
		HWND hwnd;
		DWORD left;
		DWORD top;
		DWORD width;
		DWORD height;
		DWORD windowed_style;
	};

	struct lwWatchDevVideoMemInfo {
		int alloc_vb_size;
		int alloc_ib_size;
		int alloc_tex_size;
		int alloc_vb_cnt;
		int alloc_ib_cnt;
		int alloc_tex_cnt;
	};

	enum lwModelEntityTypeEnum {
		NODE_PRIMITIVE = 1,
		NODE_BONECTRL = 2,
		NODE_DUMMY = 3,
		NODE_HELPER = 4,
		NODE_LIGHT = 5,
		NODE_CAMERA = 6,

		MODELNODE_INVALID = LW_INVALID_INDEX,
	};

	enum lwTreeNodeProcTypeEnum {
		TREENODE_PROC_PREORDER,
		TREENODE_PROC_INORDER,
		TREENODE_PROC_POSTORDER,
		TREENODE_PROC_PREORDER_IGNORE,
	};

	enum lwTreeNodeProcRetTypeEnum {
		TREENODE_PROC_RET_CONTINUE = 0,
		TREENODE_PROC_RET_ABORT = 1,
		TREENODE_PROC_RET_IGNORECHILDREN = 2,
	};

	class lwITreeNode;
	typedef DWORD (*lwTreeNodeEnumProc)(lwITreeNode* node, void* param);

	enum lwModelObjectLoadType {
		MODELOBJECT_LOAD_RESET,
		MODELOBJECT_LOAD_MERGE, // exclude dummy root
		MODELOBJECT_LOAD_MERGE2, // include merge dummy root
	};

	enum lwModelNodeQueryMask {
		MNQI_ID = 0x0001,
		MNQI_TYPE = 0x0002,
		MNQI_DATA = 0x0004,
		MNQI_DESCRIPTOR = 0x0008,
		MNQI_USERPROC = 0x0010,
	};

	struct lwModelNodeQueryInfo {
		DWORD mask;
		DWORD id;
		DWORD type;
		void* data;
		char descriptor[64];
		lwTreeNodeEnumProc proc;
		void* proc_param;
		lwITreeNode* node;
	};

	enum lwSurfaceStreamType {
		SURFACESTREAM_RENDERTARGET = 1,
		SURFACESTREAM_DEPTHSTENCIL = 2,
		SURFACESTREAM_INVALID = LW_INVALID_INDEX,
	};

	struct lwD3DCreateParam {
		UINT adapter;
		D3DDEVTYPE dev_type;
		HWND hwnd;
		DWORD behavior_flag;
		D3DPRESENT_PARAMETERS present_param;
	};

	struct lwD3DCreateParamAdjustInfo {
		D3DMULTISAMPLE_TYPE multi_sample_type;
	};

	enum lwOptionByteFlag {
		OPTION_FLAG_CREATEHELPERPRIMITIVE = 0,
		OPTION_FLAG_CULLPRIMITIVE_MODEL = 1,
		MAX_OPTION_BYTE_FLAG = 64,
	};

	enum lwRenderStateAtomType {
		RENDERSTATE_TYPE_RS = 0,
		RENDERSTATE_TYPE_TSS = 1,
		RENDERSTATE_TYPE_SS = 2,
		RENDERSTATE_TYPE_INVALID = LW_INVALID_INDEX,
	};

	typedef void (*lwTimerProc)(DWORD time); // 1/1000.0f


	typedef LW_RESULT (*lwDirectoryBrowserProc)(const char* file_path, const WIN32_FIND_DATA* wfd, void* param);

	enum lwDirBrowserType {
		DIR_BROWSE_FILE = 0x0001,
		DIR_BROWSE_DIRECTORY = 0x0002,

		DIR_BROWSE_INVALID = 0,
	};

	typedef unsigned int (__stdcall *lwThreadProc)(void* param);

	enum lwThreadPoolType {
		THREAD_POOL_LOADRES = 0,
		THREAD_POOL_SIZE,
	};

	enum lwCriticalSectionType {
		CRITICALSECTION_LOCK_SYSMEMTEX = 0,
	};

	struct lwDwordByte4 {
		union {
			DWORD d;
			BYTE b[4];
		};
	};

	struct lwVertexBufferInfo {
		DWORD size;
		DWORD usage;
		DWORD fvf;
		DWORD stride;
		D3DPOOL pool;
	};

	struct lwIndexBufferInfo {
		DWORD size;
		DWORD usage;
		DWORD format;
		DWORD stride;
		D3DPOOL pool;
	};

	class lwIPrimitive;
	typedef LW_RESULT (*lwTranspPrimitiveProc)(lwIPrimitive* obj, void* param);


	struct lwSceneFrameInfo {
		DWORD _update_count;
		DWORD _render_count;
	};

	enum {
		OPT_RESMGR_LOADTEXTURE_MT = 1,
		OPT_RESMGR_LOADMESH_MT = 2,
		OPT_CREATE_ASSISTANTOBJECT = 3,
		OPT_RESMGR_TEXENCODE = 5,

		OPT_RESMGR_BYTESET_SIZE
	};

	enum {
		TIMER_HIT_ADDITIVE = 1,
		TIMER_HIT_FILTER = 2,
	};

	enum {
		ASSOBJ_MASK_SIZE = 0x0001,
		ASSOBJ_MASK_COLOR = 0x0002,
	};

	struct lwAssObjInfo {
		lwVector3 size;
		DWORD color;
	};

	struct lwStaticStreamMgrDebugInfo {
		DWORD vbs_size;
		DWORD vbs_used_size;
		DWORD vbs_free_size;
		DWORD vbs_unused_size;
		DWORD vbs_locked_size;
		DWORD ibs_size;
		DWORD ibs_used_size;
		DWORD ibs_free_size;
		DWORD ibs_unused_size;
		DWORD ibs_locked_size;
	};

#define DX_VER_MAJOR(id) (id << 16)
#define DX_VER_MINOR(id) (id << 8)

	enum {
		DX_VERSION_INVALID = 0,

		DX_VERSION_8_0 = DX_VER_MAJOR(8),
		DX_VERSION_8_1 = DX_VER_MAJOR(8) + DX_VER_MINOR(1),
		DX_VERSION_8_1_a = DX_VER_MAJOR(8) + DX_VER_MINOR(1) + 1,
		DX_VERSION_8_1_b = DX_VER_MAJOR(8) + DX_VER_MINOR(1) + 2,
		DX_VERSION_8_2 = DX_VER_MAJOR(8) + DX_VER_MINOR(2),

		DX_VERSION_9_0 = DX_VER_MAJOR(9),
		DX_VERSION_9_0_a = DX_VER_MAJOR(9) + 1,
		DX_VERSION_9_0_b = DX_VER_MAJOR(9) + 2,
		DX_VERSION_9_0_c = DX_VER_MAJOR(9) + 3,

		DX_VERSION_UNKNOWN = 0x0007ffff,
		DX_VERSION_X_X = 0xffffffff,
	};

	struct lwDxVerInfo {
		DWORD version;
		DWORD revision;
	};

	struct lwCullPriInfo {
		DWORD total_cnt;
		DWORD culling_cnt;
	};

	enum lwBackBufferFormatsItemMaxType {
		BBFI_MULTISAMPLE = 0,
		BBFI_DEPTHSTENCIL,
		BBFI_RENDERTARGET,
		BBFI_TEXTURE,
		BBFI_CUBETEXTURE,
		BBFI_VOLUMETEXTURE,

		BBFI_MAX_MULTISAMPLE = 16,
		BBFI_MAX_DEPTHSTENCIL = 8,
		BBFI_MAX_RENDERTARGET = 8,
		BBFI_MAX_TEXTURE = 32,
		BBFI_MAX_CUBETEXTURE = 32,
		BBFI_MAX_VOLUMETEXTURE = 32,
	};

	struct lwBackBufferFormatsItemInfo {
		BOOL windowed;
		D3DFORMAT format;
		D3DFORMAT fmt_multisample[BBFI_MAX_MULTISAMPLE];
		D3DFORMAT fmt_depthstencil[BBFI_MAX_DEPTHSTENCIL];
		D3DFORMAT fmt_rendertarget[BBFI_MAX_RENDERTARGET];
		D3DFORMAT fmt_texture[BBFI_MAX_TEXTURE];
		D3DFORMAT fmt_cubetexture[BBFI_MAX_CUBETEXTURE];
		D3DFORMAT fmt_volumetexture[BBFI_MAX_VOLUMETEXTURE];
	};

	struct lwBackBufferFormatsInfo {
		lwBackBufferFormatsItemInfo fmt_seq[4];
	};

	enum {
		LOADINGRES_TYPE_RUNTIME = 1,
		LOADINGRES_TYPE_RUNTIME_MT = 2,

		LOADINGRES_MASK_RT0 = 0x0001,
		LOADINGRES_MASK_RTMT0 = 0x0001,
		LOADINGRES_MASK_RTMT1 = 0x0002,
		LOADINGRES_MASK_LOADSM_FAILED = 0x0010,
		LOADINGRES_MASK_LOADVM_FAILED = 0x0020,

		LOADINGRES_MASK_RTMTREG_FAILED = 0x0100,
	};

	enum {
		RES_PASS_DEFAULT = 0,
		RES_PASS_SKIPTHISDRAW = 1,
		RES_PASS_ERROR = -1,
	};

	enum lwRenderCtrlVSTypesEnum {
		RENDERCTRL_VS_FIXEDFUNCTION = 1, //RENDERCTRL_GENERIC =       0x0001,
		RENDERCTRL_VS_VERTEXBLEND = 2, //RENDERCTRL_VSBONE =        0x0002,
		RENDERCTRL_VS_VERTEXBLEND_DX9 = 3,

		// user defined render ctrl index based from 256 - 512
		RENDERCTRL_VS_USER = 0x100,

		RENDERCTRL_VS_INVALID = LW_INVALID_INDEX
	};

#define LW_ARGB_A(v) (((v) >> 24) / 255.0f)
#define LW_ARGB_R(v) ((((v) & 0x00ff0000) >> 16) / 255.0f)
#define LW_ARGB_G(v) ((((v) & 0x0000ff00) >> 8) / 255.0f)
#define LW_ARGB_B(v) (((v) & 0x000000ff) / 255.0f)

	struct lwVertexShaderInfo9 {
		BYTE* data;
		DWORD size;
		IDirect3DVertexShaderX* handle;
	};

	typedef lwVertexShaderInfo9 lwVertexShaderInfo;

	struct lwVertDeclInfo9 {
		D3DVERTEXELEMENT9* data;
		IDirect3DVertexDeclarationX* handle;
	};

	typedef lwVertDeclInfo9 lwVertDeclInfo;



	enum {
		VS_FILE_OBJECT = 1,
		VS_FILE_ASM = 2,
		VS_FILE_HLSL = 3,
	};

	class lwAnimCtrlTypeInfo {
	public:
		DWORD type;
		DWORD data[4];

		BOOL Check(const lwAnimCtrlTypeInfo* info) {
			return (type == info->type
				&& data[0] == info->data[0]
				&& data[1] == info->data[1]);
		}
	};

	typedef lwAnimCtrlTypeInfo lwAnimCtrlObjTypeInfo;

	typedef BOOL (*lwHeapCompProc)(const void*& i, const void*& j);
	typedef void (*lwHeapFilterProc)(const void*& i, DWORD j);

	enum lwFileStreamOpenInfoType {
		// adapter flag
		FS_ADAPTER_FILE = 0x0001,
		FS_ADAPTER_PACKET = 0x0002, // now invalid

		// access flag
		FS_ACCESS_READ = ACCESS_READ,
		FS_ACCESS_WRITE = ACCESS_WRITE,

		// create flag
		FS_CREATE_NEW = CREATE_NEW,
		FS_CREATE_ALWAYS = CREATE_ALWAYS,
		FS_OPEN_EXISTING = OPEN_EXISTING,
		FS_OPEN_ALWAYS = OPEN_ALWAYS,
		FS_TRUNCATE_EXISTING = TRUNCATE_EXISTING,

		// attributes flag
		FS_SEQUENTIAL_SCAN = FILE_FLAG_SEQUENTIAL_SCAN,

		// seek
		FS_FILE_BEGIN = FILE_BEGIN,
		FS_FILE_CURRENT = FILE_CURRENT,
		FS_FILE_END = FILE_END,
	};

	struct lwFileStreamOpenInfo {
		DWORD adapter_type;
		DWORD access_flag;
		DWORD create_flag;
		DWORD attributes_flag;
	};


LW_END
