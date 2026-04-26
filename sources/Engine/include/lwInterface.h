//
#pragma once

#include <string>
#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwDirectX.h"
#include "lwGuidObj.h"
#include "lwClassDecl.h"
#include "lwITypes.h"
#include "lwITypes2.h"
#include "lwShaderTypes.h"

LW_BEGIN
	class lwISystem;
	class lwISysGraphics;
	class lwIDeviceObject;
	class lwIHelperObject;
	class lwIShaderMgr;
	class lwIMeshAgent;
	class lwIMtlTexAgent;
	class lwIRenderCtrl;
	class lwIRenderCtrlAgent;
	class lwIResourceMgr;
	class lwIAnimCtrlObj;
	class lwIAnimCtrlObjMat;
	class lwIAnimCtrlObjBone;
	class lwIAnimCtrlAgent;
	class lwIPrimitive;
	class lwIRenderCtrlVSBone;
	class lwISceneMgr;
	class lwIAnimKeySetPRS;
	class lwILockableStreamVB;
	class lwILockableStreamIB;
	class lwINode;
	class lwINodePrimitive;
	class lwINodeBoneCtrl;
	class lwINodeObject;
	class lwIPathInfo;
	class lwIOptionMgr;
	class lwIThreadPool;
	class lwIRenderStateAtomSet;
	class lwIVertexBuffer;
	class lwIIndexBuffer;
	class lwISystemInfo;
	class lwIMesh;
	class lwITex;
	class lwIAnimKeySetFloat;
	class lwICoordinateSys;
	class lwIFileStream;

	// ============= begin base interface method =============
	class lwInterface {
	public:
		virtual LW_RESULT Release() PURE_METHOD;
		virtual LW_RESULT GetInterface(LW_VOID** i, lwGUID guid) PURE_METHOD;
	};

#define LW_STD_DECLARATION() \
    public: \
    virtual LW_RESULT Release(); \
    virtual LW_RESULT GetInterface( LW_VOID** i, lwGUID guid );


#define LW_STD_RELEASE( cls ) \
LW_RESULT cls::Release() \
{ LW_DELETE( this ); return LW_RET_OK; }

#define LW_STD_GETINTERFACE( cls ) \
LW_RESULT cls::GetInterface( LW_VOID** i, lwGUID guid ) \
{ return LW_RET_NULL; };

#define LW_STD_IMPLEMENTATION( cls ) \
LW_STD_RELEASE( cls ) \
LW_STD_GETINTERFACE( cls )

	// ============= end base interface method =============

	class LW_DECLSPEC_NOVTABLE lwIPoseCtrl : public lwInterface {
	public:
		virtual LW_RESULT Load(const char* file) PURE_METHOD;
		virtual LW_RESULT Save(const char* file) const PURE_METHOD;

		virtual LW_RESULT InsertPose(DWORD id, const lwPoseInfo* pi, int num) PURE_METHOD;
		virtual LW_RESULT ReplacePose(DWORD id, const lwPoseInfo* pi) PURE_METHOD;
		virtual LW_RESULT RemovePose(DWORD id) PURE_METHOD;
		virtual LW_RESULT RemoveAll() PURE_METHOD;

		virtual void SetFrameNum(int frame) PURE_METHOD;
		virtual DWORD GetPoseNum() const PURE_METHOD;
		virtual lwPoseInfo* GetPoseInfo(DWORD id) PURE_METHOD;

		virtual DWORD GetPoseFrameNum(DWORD pose_id) const PURE_METHOD;
		virtual int IsPosePlaying(const lwPlayPoseInfo* info) const PURE_METHOD;

		virtual LW_RESULT PlayPose(lwPlayPoseInfo* info) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIMtlTexInfo : public lwInterface {
	public:
	};

	class LW_DECLSPEC_NOVTABLE lwIHelperInfo : public lwInterface {
	public:
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimData : public lwInterface {
	public:
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimDataBone : public lwIAnimData {
	public:
		virtual LW_RESULT Load(const char* file) PURE_METHOD;
		virtual LW_RESULT Save(const char* file) const PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimDataMatrix : public lwIAnimData {
	public:
		virtual DWORD GetFrameNum() PURE_METHOD;
		virtual LW_RESULT GetValue(lwMatrix44* mat, float frame) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimDataTexUV : public lwIAnimData {
	public:
		virtual LW_RESULT SetData(const lwMatrix44* mat_seq, DWORD mat_num) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimDataTexImg : public lwIAnimData {
	public:
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimDataMtlOpacity : public lwIAnimData {
	public:
		virtual LW_RESULT Clone(lwIAnimDataMtlOpacity** obj) PURE_METHOD;
		virtual lwIAnimKeySetFloat* GetAnimKeySet() PURE_METHOD;
		virtual void SetAnimKeySet(lwIAnimKeySetFloat* aks_ctrl) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimDataInfo : public lwInterface {
	public:
	};


	class LW_DECLSPEC_NOVTABLE lwIAnimCtrl : public lwInterface {
	public:
		virtual LW_DWORD GetCtrlType() const PURE_METHOD;
		virtual LW_VOID SetRegisterID(LW_DWORD id) PURE_METHOD;
		virtual LW_DWORD GetRegisterID() const PURE_METHOD;
		virtual void SetResFile(const lwResFileAnimData* info) PURE_METHOD;
		virtual lwResFileAnimData* GetResFileInfo() PURE_METHOD;
		virtual lwIPoseCtrl* GetPoseCtrl() PURE_METHOD;

		virtual LW_RESULT LoadData(const void* data) PURE_METHOD;
		virtual LW_RESULT UpdatePose(lwPlayPoseInfo* info) PURE_METHOD;
		virtual LW_RESULT UpdateAnimData(const lwPlayPoseInfo* info) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlBone : public lwIAnimCtrl {
	public:
		virtual LW_RESULT UpdatePoseKeyFrameProc(lwPlayPoseInfo* info) PURE_METHOD;
		virtual lwMatrix44* GetBoneRTMSeq() PURE_METHOD;
		virtual lwIndexMatrix44* GetDummyRTMSeq() PURE_METHOD;
		virtual lwMatrix44* GetDummyRTM(DWORD id) PURE_METHOD;
		virtual DWORD GetDummyNum() PURE_METHOD;
		virtual DWORD GetBoneNum() PURE_METHOD;
		virtual LW_RESULT CreateRunTimeDataBuffer() PURE_METHOD;
		virtual LW_RESULT EnableRunTimeFrameBuffer(DWORD frame, DWORD flag) PURE_METHOD;
		virtual LW_RESULT ExtractAnimData(lwIAnimDataBone* out_data) PURE_METHOD;
		virtual LW_RESULT DumpRunTimeBoneData(const char* file) PURE_METHOD;
		virtual LW_RESULT DumpInitInvMat(const char* file) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlMatrix : public lwIAnimCtrl {
	public:
		virtual LW_RESULT GetRTM(lwMatrix44* mat) PURE_METHOD;
		virtual LW_RESULT ExtractAnimData(lwIAnimDataMatrix* out_data) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlTexUV : public lwIAnimCtrl {
	public:
		virtual LW_RESULT LoadData(const void* data) PURE_METHOD;
		virtual LW_RESULT LoadData(const lwMatrix44* mat_seq, DWORD mat_num) PURE_METHOD;
		virtual void SetAnimKeySetPRS(lwIAnimKeySetPRS* keyset) PURE_METHOD;
		virtual LW_RESULT ExtractAnimData(lwIAnimDataTexUV* out_data) PURE_METHOD;
		virtual LW_RESULT GetRunTimeMatrix(lwMatrix44* mat) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlTexImg : public lwIAnimCtrl {
	public:
		virtual LW_RESULT ExtractAnimData(lwIAnimDataTexImg* out_data) PURE_METHOD;
		virtual LW_RESULT GetRunTimeTex(lwITex** tex) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlMtlOpacity : public lwIAnimCtrl {
	public:
		virtual LW_RESULT ExtractAnimData(lwIAnimDataMtlOpacity* out_data) PURE_METHOD;
		virtual LW_RESULT GetRunTimeOpacity(float* opacity) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIMeshInfo : public lwInterface {
	public:
	};

	class LW_DECLSPEC_NOVTABLE lwIGeomObjInfo : public lwInterface {
	public:
		virtual LW_RESULT Load(const char* file) PURE_METHOD;
		virtual LW_RESULT Save(const char* file) PURE_METHOD;
		virtual lwMeshInfo* GetMeshInfo() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIModelObjInfo : public lwInterface {
	public:
		virtual LW_RESULT Load(const char* file) PURE_METHOD;
		virtual LW_RESULT Save(const char* file) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIHelperDummyObjInfo : public lwInterface {
	public:
	};

	class LW_DECLSPEC_NOVTABLE lwIPrimitive : public lwInterface {
	public:
		virtual LW_RESULT Load(lwIGeomObjInfo* info, const char* tex_path, const lwResFile* res) PURE_METHOD;
		virtual LW_RESULT LoadMesh(lwMeshInfo* info) PURE_METHOD;
		virtual LW_RESULT LoadMesh(const lwResFileMesh* info) PURE_METHOD;
		virtual LW_RESULT LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, const char* tex_path) PURE_METHOD;
		virtual LW_RESULT LoadAnimData(lwIAnimDataInfo* info, const char* tex_path, const lwResFile* res) PURE_METHOD;
		virtual LW_RESULT LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci) PURE_METHOD;

		virtual LW_RESULT ExtractGeomObjInfo(lwIGeomObjInfo* info) PURE_METHOD;

		virtual LW_RESULT DestroyRenderCtrlAgent() PURE_METHOD;
		virtual LW_RESULT DestroyMeshAgent() PURE_METHOD;
		virtual LW_RESULT DestroyMtlTexAgent(DWORD subset) PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT Clone(lwIPrimitive** ret_obj) PURE_METHOD;

		virtual lwIResourceMgr* GetResourceMgr() PURE_METHOD;
		virtual lwIMtlTexAgent* GetMtlTexAgent(DWORD id) PURE_METHOD;
		virtual lwIMeshAgent* GetMeshAgent() PURE_METHOD;
		virtual lwIAnimCtrlAgent* GetAnimAgent() PURE_METHOD;
		virtual lwIRenderCtrlAgent* GetRenderCtrlAgent() PURE_METHOD;
		virtual lwIHelperObject* GetHelperObject() PURE_METHOD;
		virtual LW_RESULT GetSubsetNum(DWORD* subset_num) PURE_METHOD;

		virtual void SetMeshAgent(lwIMeshAgent* agent) PURE_METHOD;
		virtual void SetMtlTexAgent(DWORD subset, lwIMtlTexAgent* agent) PURE_METHOD;
		virtual void SetAnimCtrlAgent(lwIAnimCtrlAgent* agent) PURE_METHOD;
		virtual void SetRenderCtrl(lwIRenderCtrlAgent* obj) PURE_METHOD;
		virtual void SetHelperObject(lwIHelperObject* obj) PURE_METHOD;
		virtual void SetMaterial(const lwMaterial* mtl) PURE_METHOD;
		virtual void SetOpacity(float opacity) PURE_METHOD;

		virtual void SetMatrixLocal(const lwMatrix44* mat_local) PURE_METHOD;
		virtual void SetMatrixParent(const lwMatrix44* mat_parent) PURE_METHOD;
		virtual lwMatrix44* GetMatrixLocal() PURE_METHOD;
		virtual lwMatrix44* GetMatrixGlobal() PURE_METHOD;
		virtual void SetID(DWORD id) PURE_METHOD;
		virtual void SetParentID(DWORD id) PURE_METHOD;
		virtual DWORD GetID() const PURE_METHOD;
		virtual DWORD GetParentID() const PURE_METHOD;


		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
		virtual LW_RESULT RenderSubset(DWORD subset) PURE_METHOD;

		virtual LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) PURE_METHOD;

		virtual void SetState(const lwStateCtrl* ctrl) PURE_METHOD;
		virtual void SetState(DWORD state, BYTE value) PURE_METHOD;
		virtual BYTE GetState(DWORD state) const PURE_METHOD;
		virtual LW_RESULT SetTextureLOD(DWORD level) PURE_METHOD;
		virtual LW_RESULT PlayDefaultAnimation(bool IsGlitched = false) PURE_METHOD;
		virtual LW_RESULT ResetTexture(DWORD subset, DWORD stage, const char* file, const char* tex_path) PURE_METHOD;

		virtual void setPixelShader(const std::string& filename) PURE_METHOD;
	};


	class LW_DECLSPEC_NOVTABLE lwIPhysique : public lwInterface {
	public:
		virtual lwIResourceMgr* GetResourceMgr() PURE_METHOD;

		virtual LW_RESULT Destroy() PURE_METHOD;

		virtual LW_RESULT LoadBone(const char* file) PURE_METHOD;
		virtual LW_RESULT LoadPrimitive(DWORD part_id, const char* file) PURE_METHOD;
		virtual LW_RESULT LoadPrimitive(DWORD part_id, lwIGeomObjInfo* geom_info) PURE_METHOD;
		virtual LW_RESULT DestroyPrimitive(DWORD part_id) PURE_METHOD;
		virtual LW_RESULT CheckPrimitive(DWORD part_id) PURE_METHOD;

		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;

		virtual lwMatrix44* GetMatrix() PURE_METHOD;
		virtual void SetMatrix(const lwMatrix44* mat) PURE_METHOD;

		virtual void SetOpacity(float opacity) PURE_METHOD;

		virtual LW_RESULT HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) PURE_METHOD;
		virtual LW_RESULT HitTestPhysique(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) PURE_METHOD;

		virtual void ShowHelperObject(int show) PURE_METHOD;
		virtual void ShowBoundingObjectPhysique(int show) PURE_METHOD;

		virtual LW_RESULT SetItemLink(const lwItemLinkInfo* info) PURE_METHOD;
		virtual LW_RESULT ClearItemLink(lwIItem* obj) PURE_METHOD;

		virtual void SetObjState(DWORD state, BYTE value) PURE_METHOD;
		virtual DWORD GetObjState(DWORD state) const PURE_METHOD;

		virtual lwIPrimitive* GetPrimitive(DWORD id) PURE_METHOD;
		virtual lwIAnimCtrlAgent* GetAnimCtrlAgent() PURE_METHOD;

		virtual LW_RESULT RegisterSceneMgr(lwISceneMgr* scene_mgr) PURE_METHOD;
		virtual LW_RESULT SetTextureLOD(DWORD level) PURE_METHOD;
		virtual float GetOpacity() PURE_METHOD;
		virtual void Start() PURE_METHOD;
		virtual void End() PURE_METHOD;
		virtual bool isLoaded() PURE_METHOD;
		virtual void setComponentColour(size_t index, D3DCOLOR colour, const std::string& filterTextureName)
		PURE_METHOD;
		virtual void setTextureOperation(size_t index, D3DTEXTUREOP operation) PURE_METHOD;
		virtual std::string_view getTextureOperationDescription(size_t operation) PURE_METHOD;
		virtual void setPixelShader(size_t index, const std::string& filename) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIModel : public lwInterface {
	public:
		virtual LW_RESULT Load(lwIModelObjInfo* info) PURE_METHOD;
		virtual LW_RESULT Load(const char* file, DWORD model_id = LW_INVALID_INDEX) PURE_METHOD;
		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
		virtual LW_RESULT RenderPrimitive(DWORD id) PURE_METHOD;
		virtual LW_RESULT RenderHelperObject() PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;

		virtual lwMatrix44* GetMatrix() PURE_METHOD;
		virtual void SetMatrix(const lwMatrix44* mat) PURE_METHOD;
		virtual void SetMaterial(const lwMaterial* mtl) PURE_METHOD;
		virtual void SetOpacity(float opacity) PURE_METHOD;

		virtual LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) PURE_METHOD;
		virtual LW_RESULT HitTestHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
										   const char* type_name) PURE_METHOD;
		virtual LW_RESULT HitTestHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
											const char* type_name) PURE_METHOD;
		virtual LW_RESULT HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) PURE_METHOD;
		virtual LW_RESULT HitTestPrimitiveHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
													const char* type_name) PURE_METHOD;
		virtual LW_RESULT HitTestPrimitiveHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
													 const char* type_name) PURE_METHOD;

		virtual LW_RESULT PlayDefaultAnimation(bool IsGlitched = false) PURE_METHOD;

		virtual void ShowHelperObject(int show) PURE_METHOD;
		virtual void ShowHelperMesh(int show);
		virtual void ShowHelperBox(int show);
		virtual void ShowBoundingObject(int show);

		virtual DWORD GetPrimitiveNum() const PURE_METHOD;
		virtual lwIPrimitive* GetPrimitive(DWORD id) PURE_METHOD;
		virtual lwIHelperObject* GetHelperObject() PURE_METHOD;

		virtual void SetObjState(DWORD state, BYTE value) PURE_METHOD;
		virtual DWORD GetObjState(DWORD state) const PURE_METHOD;

		virtual LW_RESULT RegisterSceneMgr(lwISceneMgr* scene_mgr) PURE_METHOD;

		virtual LW_RESULT SetItemLink(const lwItemLinkInfo* info) PURE_METHOD;
		virtual LW_RESULT ClearItemLink(lwIItem* obj) PURE_METHOD;
		virtual LW_RESULT SetTextureLOD(DWORD level) PURE_METHOD;
		virtual float GetOpacity() PURE_METHOD;

		virtual LW_RESULT CullPrimitive() PURE_METHOD;
		virtual DWORD GetCullingPrimitiveNum() PURE_METHOD;
		virtual LW_RESULT ExtractModelInfo(lwIModelObjInfo* out_info) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIItem : public lwInterface {
	public:
		virtual lwMatrix44* GetMatrix() PURE_METHOD;
		virtual void SetMatrix(const lwMatrix44* mat) PURE_METHOD;

		virtual LW_RESULT Load(lwIGeomObjInfo* info) PURE_METHOD;
		virtual LW_RESULT Load(const char* file, int arbitrary_flag = 0) PURE_METHOD;
		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;

		virtual LW_RESULT Copy(lwIItem* src_obj) PURE_METHOD;
		virtual LW_RESULT Clone(lwIItem** ret_obj) PURE_METHOD;

		virtual lwIPrimitive* GetPrimitive() PURE_METHOD;

		virtual LW_RESULT HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) PURE_METHOD;

		virtual void SetMaterial(const lwMaterial* mtl) PURE_METHOD;
		virtual void SetOpacity(float opacity) PURE_METHOD;

		virtual void ShowBoundingObject(int show) PURE_METHOD;

		virtual LW_RESULT PlayDefaultAnimation(bool IsGlitched = false) PURE_METHOD;

		virtual LW_RESULT GetDummyMatrix(lwMatrix44* mat, DWORD id) PURE_METHOD;
		virtual const lwMatrix44* GetObjDummyMatrix(DWORD id) PURE_METHOD;
		virtual LW_RESULT GetObjDummyRunTimeMatrix(lwMatrix44* mat, DWORD id) PURE_METHOD;

		virtual LW_RESULT SetLinkCtrl(lwLinkCtrl* ctrl, DWORD link_parent_id, DWORD link_item_id) PURE_METHOD;
		virtual LW_RESULT ClearLinkCtrl() PURE_METHOD;
		virtual void SetObjState(DWORD state, BYTE value) PURE_METHOD;
		virtual DWORD GetObjState(DWORD state) const PURE_METHOD;

		virtual LW_RESULT RegisterSceneMgr(lwISceneMgr* scene_mgr) PURE_METHOD;
		virtual LW_RESULT SetTextureLOD(DWORD level) PURE_METHOD;
		virtual float GetOpacity() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwISysGraphics : public lwInterface {
	public:
		virtual lwISystem* GetSystem()PURE_METHOD;
		virtual lwIDeviceObject* GetDeviceObject() PURE_METHOD;
		virtual lwIResourceMgr* GetResourceMgr() PURE_METHOD;
		virtual lwISceneMgr* GetSceneMgr() PURE_METHOD;
		virtual lwICoordinateSys* GetCoordinateSys() PURE_METHOD;

		virtual LW_RESULT CreateDeviceObject(lwIDeviceObject** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateResourceManager(lwIResourceMgr** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateSceneManager(lwISceneMgr** ret_obj) PURE_METHOD;

		virtual LW_RESULT ToggleFullScreen(D3DPRESENT_PARAMETERS* d3dpp, lwWndInfo* wnd_info) PURE_METHOD;
		virtual LW_RESULT TestCooperativeLevel() PURE_METHOD;
		virtual void SetOutputLoseDeviceProc(lwOutputLoseDeviceProc proc) PURE_METHOD;
		virtual void SetOutputResetDeviceProc(lwOutputResetDeviceProc proc) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwISystem : public lwInterface {
	public:
		virtual LW_RESULT Initialize() PURE_METHOD;
		virtual LW_RESULT CreateGraphicsSystem(lwISysGraphics** sys) PURE_METHOD;
		virtual lwIPathInfo* GetPathInfo() PURE_METHOD;
		virtual lwIOptionMgr* GetOptionMgr() PURE_METHOD;
		virtual lwISystemInfo* GetSystemInfo() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIPathInfo : public lwInterface {
	public:
		virtual void SetPath(DWORD type, std::string_view path) PURE_METHOD;
		virtual const std::string& GetPath(DWORD type) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIOptionMgr : public lwInterface {
	public:
		virtual void SetIgnoreModelTexFlag(BYTE flag) PURE_METHOD;
		virtual BYTE GetIgnoreModelTexFlag() const PURE_METHOD;
		virtual void SetByteFlag(DWORD type, BYTE value) PURE_METHOD;
		virtual BYTE GetByteFlag(DWORD type) const PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIByteSet : public lwInterface {
	public:
		virtual LW_RESULT Alloc(DWORD size) PURE_METHOD;
		virtual LW_RESULT Free() PURE_METHOD;
		virtual LW_RESULT SetValueSeq(DWORD start, BYTE* buf, DWORD num) PURE_METHOD;
		virtual void SetValue(DWORD type, BYTE value) PURE_METHOD;
		virtual BYTE GetValue(DWORD type) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIStaticStreamMgr : public lwInterface {
	public:
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT CreateStreamEntitySeq(DWORD entity_vb_num, DWORD entity_ib_num) PURE_METHOD;
		virtual LW_RESULT CreateVertexBufferStream(DWORD stream_id, DWORD stream_size) PURE_METHOD;
		virtual LW_RESULT CreateIndexBufferStream(DWORD stream_id, DWORD stream_size) PURE_METHOD;
		virtual LW_RESULT RegisterVertexBuffer(LW_HANDLE* out_handle, void* data, DWORD size, DWORD stride) PURE_METHOD;
		virtual LW_RESULT RegisterIndexBuffer(LW_HANDLE* out_handle, void* data, DWORD size, DWORD stride) PURE_METHOD;
		virtual LW_RESULT UnregisterVertexBuffer(LW_HANDLE handle) PURE_METHOD;
		virtual LW_RESULT UnregisterIndexBuffer(LW_HANDLE handle) PURE_METHOD;
		virtual LW_RESULT BindVertexBuffer(LW_HANDLE handle, UINT channel) PURE_METHOD;
		virtual LW_RESULT BindIndexBuffer(LW_HANDLE handle) PURE_METHOD;
		virtual DWORD GetVertexEntityOffset() PURE_METHOD;
		virtual DWORD GetIndexEntityOffset() PURE_METHOD;
		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;
		virtual LW_RESULT GetDebugInfo(lwStaticStreamMgrDebugInfo* info) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIDynamicStreamMgr : public lwInterface {
	public:
		virtual LW_RESULT Create(DWORD vb_size, DWORD ib_size) PURE_METHOD;
		virtual LW_RESULT DrawPrimitive(D3DPRIMITIVETYPE pt_type, UINT count, const void* vert_data, UINT vert_stride,
										D3DFORMAT fvf) PURE_METHOD;
		virtual LW_RESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE pt_type, UINT min_vert_index, UINT num_vert_indices,
											   UINT count,
											   const void* index_data, D3DFORMAT index_format, const void* vertex_data,
											   UINT vert_stride, D3DFORMAT fvf) PURE_METHOD;

		virtual LW_RESULT BindDataVB(DWORD channel, const void* data, DWORD size, UINT stride) PURE_METHOD;
		virtual LW_RESULT BindDataIB(const void* data, DWORD size, DWORD stride) PURE_METHOD;
		virtual LW_RESULT DrawPrimitive(D3DPRIMITIVETYPE pt_type, UINT start_vert, UINT count) PURE_METHOD;
		virtual LW_RESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE pt_type, INT base_vert_index, UINT min_index,
											   UINT num_vert, UINT start_index, UINT count) PURE_METHOD;
		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwILockableStreamMgr : public lwInterface {
	public:
		virtual LW_RESULT RegisterVertexBuffer(LW_HANDLE* handle, void* data, DWORD size, DWORD usage, DWORD fvf)
		PURE_METHOD;
		virtual LW_RESULT RegisterIndexBuffer(LW_HANDLE* handle, void* data, DWORD size, DWORD usage, DWORD fvf)
		PURE_METHOD;
		virtual LW_RESULT UnregisterVertexBuffer(LW_HANDLE handle) PURE_METHOD;
		virtual LW_RESULT UnregisterIndexBuffer(LW_HANDLE handle) PURE_METHOD;
		virtual LW_RESULT BindVertexBuffer(LW_HANDLE handle, UINT channel, UINT offset_byte, UINT stride) PURE_METHOD;
		virtual LW_RESULT BindIndexBuffer(LW_HANDLE handle, UINT base_vert_index) PURE_METHOD;
		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;
		virtual lwILockableStreamVB* GetStreamVB(LW_HANDLE handle) PURE_METHOD;
		virtual lwILockableStreamIB* GetStreamIB(LW_HANDLE handle) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwILockableStream : public lwInterface {
	public:
		virtual LW_RESULT Lock(UINT offset, UINT size, void** data, DWORD flag) PURE_METHOD;
		virtual LW_RESULT Unlock() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwILockableStreamVB : public lwILockableStream {
	public:
	};

	class LW_DECLSPEC_NOVTABLE lwILockableStreamIB : public lwILockableStream {
	public:
	};

	class LW_DECLSPEC_NOVTABLE lwISurfaceStream : public lwInterface {
	public:
		virtual LW_RESULT CreateRenderTarget(UINT width, UINT height, D3DFORMAT format,
											 D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality,
											 BOOL lockable, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT CreateDepthStencilSurface(UINT width, UINT height, D3DFORMAT format,
													D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality,
													BOOL discard, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;
		virtual void SetRegisterID(DWORD id) PURE_METHOD;
		virtual DWORD GetRegisterID() const PURE_METHOD;
		virtual IDirect3DSurfaceX* GetSurface() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwISurfaceStreamMgr : public lwInterface {
	public:
		virtual LW_RESULT CreateRenderTarget(LW_HANDLE* ret_handle, UINT width, UINT height, D3DFORMAT format,
											 D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality,
											 BOOL lockable, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT CreateDepthStencilSurface(LW_HANDLE* ret_handle, UINT width, UINT height, D3DFORMAT format,
													D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality,
													BOOL discard, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT UnregisterSurface(LW_HANDLE handle) PURE_METHOD;
		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;
		virtual lwIResourceMgr* GetResourceMgr() PURE_METHOD;
		virtual lwISurfaceStream* GetSurfaceStream(LW_HANDLE handle) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIDeviceObject : public lwInterface {
	public:
		virtual LW_RESULT CreateDirect3D() PURE_METHOD;
		virtual LW_RESULT CreateDevice(lwD3DCreateParam* param) PURE_METHOD;
		virtual LW_RESULT ResetDevice(D3DPRESENT_PARAMETERS* d3dpp) PURE_METHOD;
		virtual LW_RESULT ResetDeviceStateCache() PURE_METHOD;
		virtual LW_RESULT ResetDeviceTransformMatrix() PURE_METHOD;

		virtual LW_RESULT SetDirect3D(IDirect3DX* d3d) PURE_METHOD;
		virtual LW_RESULT SetDevice(IDirect3DDeviceX* dev) PURE_METHOD;
		virtual IDirect3DX* GetDirect3D() PURE_METHOD;
		virtual IDirect3DDeviceX* GetDevice() PURE_METHOD;
		virtual lwD3DCreateParam* GetD3DCreateParam() PURE_METHOD;
		virtual D3DDISPLAYMODE* GetAdapterDisplayMode() PURE_METHOD;
		virtual D3DCAPSX* GetDeviceCaps() PURE_METHOD;
		virtual lwBackBufferFormatsInfo* GetBackBufferFormatsCaps() PURE_METHOD;

		virtual LW_RESULT SetStreamSource(UINT stream_num, IDirect3DVertexBufferX* stream_data, UINT offset_byte,
										  UINT stride) PURE_METHOD;
		virtual LW_RESULT SetIndices(IDirect3DIndexBufferX* index_data, UINT base_vert_index) PURE_METHOD;
		virtual LW_RESULT DrawPrimitive(D3DPRIMITIVETYPE pt_type, UINT start_vertex, UINT count) PURE_METHOD;
		virtual LW_RESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE pt_type, INT base_vert_index, UINT min_index,
											   UINT vert_num, UINT start_index, UINT count) PURE_METHOD;
		virtual LW_RESULT DrawPrimitiveUP(D3DPRIMITIVETYPE pt_type, UINT count, const void* data, UINT stride)
		PURE_METHOD;
		virtual LW_RESULT DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE pt_type, UINT min_vert_index, UINT vert_num,
												 UINT count, const void* index_data, D3DFORMAT index_data_fmt,
												 const void* vert_data, UINT vert_stride) PURE_METHOD;

		virtual LW_RESULT SetTransform(D3DTRANSFORMSTATETYPE state, const lwMatrix44* mat) PURE_METHOD;
		virtual LW_RESULT SetTransformView(const lwMatrix44* mat) PURE_METHOD;
		virtual LW_RESULT SetTransformProj(const lwMatrix44* mat) PURE_METHOD;
		virtual LW_RESULT SetTransformWorld(const lwMatrix44* mat) PURE_METHOD;

		virtual LW_RESULT SetMaterial(lwMaterial* mtl) PURE_METHOD;

		virtual LW_RESULT SetTexture(DWORD stage, IDirect3DBaseTextureX* tex) PURE_METHOD;
		virtual LW_RESULT SetRenderState(D3DRENDERSTATETYPE state, DWORD value) PURE_METHOD;
		virtual LW_RESULT SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value) PURE_METHOD;
		virtual LW_RESULT SetTextureForced(DWORD stage, IDirect3DTextureX* tex) PURE_METHOD;
		virtual LW_RESULT SetRenderStateForced(D3DRENDERSTATETYPE state, DWORD value) PURE_METHOD;
		virtual LW_RESULT SetTextureStageStateForced(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value)
		PURE_METHOD;
		virtual LW_RESULT SetSamplerState(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value) PURE_METHOD;
		virtual LW_RESULT SetSamplerStateForced(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value) PURE_METHOD;

		virtual LW_RESULT SetFVF(DWORD fvf) PURE_METHOD;
		virtual LW_RESULT SetVertexShader(IDirect3DVertexShaderX* shader) PURE_METHOD;
		virtual LW_RESULT SetVertexShaderForced(IDirect3DVertexShaderX* shader) PURE_METHOD;
		virtual LW_RESULT SetVertexDeclaration(IDirect3DVertexDeclarationX* decl);
		virtual LW_RESULT SetVertexDeclarationForced(IDirect3DVertexDeclarationX* decl);
		virtual LW_RESULT SetVertexShaderConstantF(UINT reg_id, const float* data, UINT v_num) PURE_METHOD;

		virtual LW_RESULT GetRenderState(DWORD state, DWORD* value) PURE_METHOD;
		virtual LW_RESULT GetTextureStageState(DWORD stage, DWORD state, DWORD* value) PURE_METHOD;
		virtual LW_RESULT GetSamplerState(DWORD sampler, D3DSAMPLERSTATETYPE state, DWORD* value) PURE_METHOD;
		virtual LW_RESULT GetTexture(DWORD stage, IDirect3DBaseTextureX** tex) PURE_METHOD;
		virtual const lwMatrix44* GetMatView() PURE_METHOD;
		virtual const lwMatrix44* GetMatProj() PURE_METHOD;
		virtual const lwMatrix44* GetMatViewProj() PURE_METHOD;

		virtual LW_RESULT SetLight(DWORD id, const D3DLIGHTX* light) PURE_METHOD;
		virtual LW_RESULT GetLight(DWORD id, D3DLIGHTX* light) PURE_METHOD;
		virtual LW_RESULT LightEnable(DWORD id, BOOL flag) PURE_METHOD;
		virtual LW_RESULT LightEnableForced(DWORD id, BOOL flag) PURE_METHOD;
		virtual LW_RESULT GetLightEnable(DWORD id, BOOL* flag) PURE_METHOD;
		virtual LW_RESULT UpdateWindowRect() PURE_METHOD;
		virtual LW_RESULT GetWindowRect(RECT* rc_wnd, RECT* rc_client) PURE_METHOD;


		virtual LW_RESULT InitStateCache() PURE_METHOD;
		virtual LW_RESULT InitCapsInfo() PURE_METHOD;

		virtual LW_RESULT CreateVertexBuffer(UINT length, DWORD usage, DWORD fvf, D3DPOOL pool,
											 IDirect3DVertexBufferX** vb, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT CreateIndexBuffer(UINT length, DWORD usage, D3DFORMAT fmt, D3DPOOL pool,
											IDirect3DIndexBufferX** ib, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT CreateTexture(IDirect3DTextureX** out_tex, const lwTexDataInfo* info, DWORD level,
										DWORD usage, DWORD format, D3DPOOL pool) PURE_METHOD;
		virtual LW_RESULT CreateTexture(IDirect3DTextureX** out_tex, UINT width, UINT height, UINT level, DWORD usage,
										D3DFORMAT format, D3DPOOL pool) PURE_METHOD;
		virtual LW_RESULT CreateTextureFromFileInMemory(IDirect3DTextureX** out_tex, void* data, UINT data_size,
														UINT width, UINT height, UINT mip_level, DWORD usage,
														D3DFORMAT format, D3DPOOL pool, DWORD filter, DWORD mip_filter,
														D3DCOLOR colorkey, D3DXIMAGE_INFO* src_info,
														PALETTEENTRY* palette) PURE_METHOD;
		virtual LW_RESULT CreateVertexBuffer(lwIVertexBuffer** out_obj) PURE_METHOD;
		virtual LW_RESULT CreateIndexBuffer(lwIIndexBuffer** out_obj) PURE_METHOD;
		virtual LW_RESULT CreateRenderTarget(IDirect3DSurfaceX** o_surface, UINT width, UINT height, D3DFORMAT format,
											 D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality,
											 BOOL lockable, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT CreateDepthStencilSurface(IDirect3DSurfaceX** o_surface, UINT width, UINT height,
													D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample,
													DWORD multi_sample_quality, BOOL discard,
													HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT CreateCubeTexture(IDirect3DCubeTextureX** o_tex, UINT edge_length, UINT levels, DWORD usage,
											D3DFORMAT format, D3DPOOL pool, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT CreateOffscreenPlainSurface(IDirect3DSurfaceX** surface, UINT width, UINT height,
													  D3DFORMAT format, DWORD pool, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT ReleaseTex(IDirect3DTextureX* tex) PURE_METHOD;
		virtual LW_RESULT ReleaseVertexBuffer(IDirect3DVertexBufferX* vb) PURE_METHOD;
		virtual LW_RESULT ReleaseIndexBuffer(IDirect3DIndexBufferX* ib) PURE_METHOD;

		virtual void BeginBenchMark() PURE_METHOD;
		virtual void EndBenchMark() PURE_METHOD;
		virtual DWORD GetMarkPolygonNum() const PURE_METHOD;
		virtual lwWatchDevVideoMemInfo* GetWatchVideoMemInfo() PURE_METHOD;

		virtual LW_RESULT CheckCurrentDeviceFormat(DWORD type, D3DFORMAT check_fmt) PURE_METHOD;
		virtual LW_RESULT ScreenToWorld(lwVector3* org, lwVector3* ray, int x, int y) PURE_METHOD;
		virtual LW_RESULT WorldToScreen(int* x, int* y, float* z, const lwVector3* v) PURE_METHOD;

		virtual LW_RESULT DumpRenderState(const char* file) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwITex : public lwInterface {
	public:
		virtual int AddRef(int i) PURE_METHOD;
		virtual int GetRef() PURE_METHOD;

		virtual LW_RESULT LoadTexInfo(const lwTexInfo* info, const char* tex_path) PURE_METHOD;
		// User-defined texture data (TEX_TYPE_DATA). Раньше пробрасывалось через
		// `lwTexInfo::data`, но это поле было `void*` разного размера на x86/x64
		// и ломало бинарный формат `.lgo/.lmo`. Теперь — отдельный API.
		virtual void SetUserData(void* data) PURE_METHOD;
		virtual void* GetUserData() const PURE_METHOD;
		virtual LW_RESULT LoadSystemMemory() PURE_METHOD;
		virtual LW_RESULT LoadVideoMemory() PURE_METHOD;
		virtual LW_RESULT LoadVideoMemoryMT() PURE_METHOD;
		virtual LW_RESULT LoadVideoMemoryEx() PURE_METHOD;
		virtual LW_RESULT LoadVideoMemoryDirect() PURE_METHOD;
		virtual LW_RESULT UnloadSystemMemory() PURE_METHOD;
		virtual LW_RESULT UnloadVideoMemory() PURE_METHOD;
		virtual LW_RESULT Unload() PURE_METHOD;
		virtual LW_RESULT BeginSet() PURE_METHOD;
		virtual LW_RESULT EndSet() PURE_METHOD;
		virtual void SetRegisterID(DWORD id) PURE_METHOD;
		virtual DWORD GetRegisterID() const PURE_METHOD;
		virtual lwTexDataInfo* GetDataInfo() PURE_METHOD;
		virtual IDirect3DTextureX* GetTex() PURE_METHOD;
		virtual lwColorValue4b GetColorKey() const PURE_METHOD;
		virtual const std::string& GetFileName() PURE_METHOD;
		virtual DWORD GetStage() const PURE_METHOD;
		virtual DWORD GetState() const PURE_METHOD;
		virtual void GetTexInfo(lwTexInfo* info) PURE_METHOD;
		virtual BOOL IsLoadingOK() const PURE_METHOD;
		virtual DWORD SetLOD(DWORD level) PURE_METHOD;
		virtual void SetLoadMode(TextureLoadMode mode) PURE_METHOD;
		virtual void SetMTFlag(DWORD flag) PURE_METHOD;
		virtual DWORD GetMTFlag() PURE_METHOD;

		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;

		virtual LW_RESULT BeginPass() PURE_METHOD;
		virtual LW_RESULT EndPass() PURE_METHOD;

		virtual void SetLoadResType(DWORD type) PURE_METHOD;
		virtual void SetLoadResMask(DWORD add_mask, DWORD remove_mask) PURE_METHOD;
		virtual DWORD GetLoadResMask() PURE_METHOD;
		virtual DWORD GetLoadResType() PURE_METHOD;
	};


	class LW_DECLSPEC_NOVTABLE lwIMesh : public lwInterface {
	public:
		virtual LW_RESULT LoadSystemMemory(const lwMeshInfo* info) PURE_METHOD;
		virtual LW_RESULT LoadSystemMemoryMT(const lwMeshInfo* info) PURE_METHOD;
		virtual LW_RESULT LoadSystemMemory() PURE_METHOD;
		virtual LW_RESULT LoadVideoMemory() PURE_METHOD;
		virtual LW_RESULT LoadVideoMemoryMT() PURE_METHOD;
		virtual LW_RESULT LoadVideoMemoryEx() PURE_METHOD;
		virtual LW_RESULT UnloadSystemMemory() PURE_METHOD;
		virtual LW_RESULT UnloadVideoMemory() PURE_METHOD;
		virtual LW_RESULT Unload() PURE_METHOD;
		virtual LW_RESULT ExtractMesh(lwMeshInfo* info) PURE_METHOD;

		virtual lwResFileMesh* GetResFileMesh() PURE_METHOD;
		virtual DWORD GetState() const PURE_METHOD;
		virtual lwMeshInfo* GetMeshInfo() PURE_METHOD;
		virtual LW_HANDLE GetVBHandle() const PURE_METHOD;
		virtual LW_HANDLE GetIBHandle() const PURE_METHOD;
		virtual LW_RESULT SetResFile(const lwResFileMesh* info) PURE_METHOD;
		virtual void SetRegisterID(DWORD id) PURE_METHOD;
		virtual DWORD GetRegisterID() const PURE_METHOD;
		virtual void SetStreamType(DWORD type) PURE_METHOD;
		virtual DWORD GetStreamType() const PURE_METHOD;
		virtual void SetColorkey(DWORD key) PURE_METHOD;


		virtual LW_RESULT BeginSet() PURE_METHOD;
		virtual LW_RESULT EndSet() PURE_METHOD;
		virtual LW_RESULT DrawSubset(DWORD subset) PURE_METHOD;
		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;

		virtual lwILockableStreamVB* GetLockableStreamVB() PURE_METHOD;
		virtual lwILockableStreamIB* GetLockableStreamIB() PURE_METHOD;

		virtual BOOL IsLoadingOK() const PURE_METHOD;
		virtual void SetMTFlag(DWORD flag) PURE_METHOD;
		virtual DWORD GetMTFlag() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIMtlTexAgent : public lwInterface {
	public:
		virtual LW_RESULT BeginPass() PURE_METHOD;
		virtual LW_RESULT EndPass() PURE_METHOD;

		virtual LW_RESULT BeginSet() PURE_METHOD;
		virtual LW_RESULT EndSet() PURE_METHOD;

		virtual LW_RESULT LoadMtlTex(lwMtlTexInfo* info, const char* tex_path) PURE_METHOD;
		virtual LW_RESULT LoadTextureStage(const lwTexInfo* info, const char* tex_path) PURE_METHOD;
		virtual LW_RESULT ExtractMtlTex(lwMtlTexInfo* info) PURE_METHOD;

		virtual LW_RESULT DestroyTextureStage(DWORD stage) PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT Clone(lwIMtlTexAgent** ret_obj) PURE_METHOD;

		virtual void SetOpacity(float opacity) PURE_METHOD;
		virtual void SetTranspType(DWORD type) PURE_METHOD;
		virtual LW_RESULT SetTex(DWORD stage, lwITex* obj, lwITex** ret_obj) PURE_METHOD;
		virtual void SetMaterial(const lwMaterial* mtl) PURE_METHOD;
		virtual void SetRenderFlag(BOOL flag) PURE_METHOD;
		virtual float GetOpacity() const PURE_METHOD;
		virtual DWORD GetTransparencyType() const PURE_METHOD;
		virtual lwMaterial* GetMaterial() PURE_METHOD;
		virtual lwITex* GetTex(DWORD stage) PURE_METHOD;
		virtual BOOL GetRenderFlag() PURE_METHOD;
		virtual LW_RESULT SetTextureTransformMatrix(DWORD stage, const lwMatrix44* mat) PURE_METHOD;
		virtual LW_RESULT SetTextureTransformImage(DWORD stage, lwITex* tex) PURE_METHOD;
		virtual lwIRenderStateAtomSet* GetMtlRenderStateSet() PURE_METHOD;
		virtual LW_RESULT SetTextureLOD(DWORD level) PURE_METHOD;
		virtual BOOL IsTextureLoadingOK() const PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIMeshAgent : public lwInterface {
	public:
		virtual void SetRenderState(DWORD begin_end, DWORD state, DWORD value) PURE_METHOD;
		virtual void SetRenderState(lwRenderStateSetMtl2* rs_set) PURE_METHOD;
		virtual void SetMesh(lwIMesh* mesh) PURE_METHOD;
		virtual lwIMesh* GetMesh() PURE_METHOD;

		virtual LW_RESULT BeginSet() PURE_METHOD;
		virtual LW_RESULT EndSet() PURE_METHOD;
		virtual LW_RESULT DrawSubset(DWORD subset) PURE_METHOD;
		virtual LW_RESULT LoadMesh(const lwMeshInfo* info) PURE_METHOD;
		virtual LW_RESULT LoadMesh(const lwResFileMesh* info) PURE_METHOD;
		virtual LW_RESULT DestroyMesh() PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT Clone(lwIMeshAgent** ret_obj) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlObj : public lwInterface {
	public:
		virtual LW_RESULT UpdateAnimCtrl() PURE_METHOD;
		virtual LW_RESULT PlayPose(const lwPlayPoseInfo* info) PURE_METHOD;
		virtual LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual lwIAnimCtrl* GetAnimCtrl() PURE_METHOD;
		virtual lwPlayPoseInfo* GetPlayPoseInfo() PURE_METHOD;
		virtual BOOL IsPlaying() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlObjMat : public lwIAnimCtrlObj {
	public:
		virtual LW_RESULT Clone(lwIAnimCtrlObjMat** ret_obj) PURE_METHOD;
		virtual lwIAnimCtrlMatrix* AttachAnimCtrl(lwIAnimCtrlMatrix* ctrl) PURE_METHOD;
		virtual lwIAnimCtrlMatrix* DetachAnimCtrl() PURE_METHOD;
		virtual lwIAnimCtrl* GetAnimCtrl() PURE_METHOD;
		virtual lwPlayPoseInfo* GetPlayPoseInfo() PURE_METHOD;
		virtual LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT PlayPose(const lwPlayPoseInfo* info) PURE_METHOD;
		virtual BOOL IsPlaying() PURE_METHOD;

		virtual LW_RESULT UpdateAnimCtrl() PURE_METHOD;
		virtual LW_RESULT UpdateObject() PURE_METHOD;
		virtual LW_RESULT GetRTM(lwMatrix44* mat) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlObjBone : public lwIAnimCtrlObj {
	public:
		virtual LW_RESULT Clone(lwIAnimCtrlObjBone** ret_obj) PURE_METHOD;
		virtual lwIAnimCtrlBone* AttachAnimCtrl(lwIAnimCtrlBone* ctrl) PURE_METHOD;
		virtual lwIAnimCtrlBone* DetachAnimCtrl() PURE_METHOD;
		virtual lwIAnimCtrl* GetAnimCtrl() PURE_METHOD;
		virtual lwPlayPoseInfo* GetPlayPoseInfo() PURE_METHOD;
		virtual void SetPlayPoseInfo(const lwPlayPoseInfo* ppi) PURE_METHOD;
		virtual LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT PlayPose(const lwPlayPoseInfo* info) PURE_METHOD;
		virtual BOOL IsPlaying() PURE_METHOD;

		virtual LW_RESULT UpdateAnimCtrl() PURE_METHOD;
		virtual LW_RESULT UpdateObject(lwIAnimCtrlObjBone* ctrl_obj, lwIMesh* mesh_obj) PURE_METHOD;
		virtual LW_RESULT UpdateHelperObject(lwIHelperObject* helper_obj) PURE_METHOD;

		virtual lwMatrix44* GetBoneRTMSeq() PURE_METHOD;
		virtual lwMatrix44* GetDummyRTM(DWORD id) PURE_METHOD;
		virtual lwIndexMatrix44* GetDummyRTMSeq() PURE_METHOD;
		virtual DWORD GetBoneRTTMNum() PURE_METHOD;
		virtual DWORD GetDummyRTMNum() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlObjTexUV : public lwIAnimCtrlObj {
	public:
		virtual LW_RESULT Clone(lwIAnimCtrlObjTexUV** ret_obj) PURE_METHOD;
		virtual lwIAnimCtrlTexUV* AttachAnimCtrl(lwIAnimCtrlTexUV* ctrl) PURE_METHOD;
		virtual lwIAnimCtrlTexUV* DetachAnimCtrl() PURE_METHOD;
		virtual lwIAnimCtrl* GetAnimCtrl() PURE_METHOD;
		virtual lwPlayPoseInfo* GetPlayPoseInfo() PURE_METHOD;
		virtual LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT PlayPose(const lwPlayPoseInfo* info) PURE_METHOD;
		virtual BOOL IsPlaying() PURE_METHOD;

		virtual LW_RESULT UpdateAnimCtrl() PURE_METHOD;
		virtual LW_RESULT UpdateObject() PURE_METHOD;

		virtual LW_RESULT GetRTM(lwMatrix44* mat) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlObjTexImg : public lwIAnimCtrlObj {
	public:
		virtual LW_RESULT Clone(lwIAnimCtrlObjTexImg** ret_obj) PURE_METHOD;
		virtual lwIAnimCtrlTexImg* AttachAnimCtrl(lwIAnimCtrlTexImg* ctrl) PURE_METHOD;
		virtual lwIAnimCtrlTexImg* DetachAnimCtrl() PURE_METHOD;
		virtual lwIAnimCtrl* GetAnimCtrl() PURE_METHOD;
		virtual lwPlayPoseInfo* GetPlayPoseInfo() PURE_METHOD;
		virtual LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT PlayPose(const lwPlayPoseInfo* info) PURE_METHOD;
		virtual BOOL IsPlaying() PURE_METHOD;

		virtual LW_RESULT UpdateAnimCtrl() PURE_METHOD;
		virtual LW_RESULT UpdateObject() PURE_METHOD;

		virtual LW_RESULT GetRunTimeTex(lwITex** tex) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlObjMtlOpacity : public lwIAnimCtrlObj {
	public:
		virtual LW_RESULT Clone(lwIAnimCtrlObjMtlOpacity** ret_obj) PURE_METHOD;
		virtual lwIAnimCtrlMtlOpacity* AttachAnimCtrl(lwIAnimCtrlMtlOpacity* ctrl) PURE_METHOD;
		virtual lwIAnimCtrlMtlOpacity* DetachAnimCtrl() PURE_METHOD;
		virtual lwIAnimCtrl* GetAnimCtrl() PURE_METHOD;
		virtual lwPlayPoseInfo* GetPlayPoseInfo() PURE_METHOD;
		virtual LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual LW_RESULT PlayPose(const lwPlayPoseInfo* info) PURE_METHOD;
		virtual BOOL IsPlaying() PURE_METHOD;

		virtual LW_RESULT UpdateAnimCtrl() PURE_METHOD;
		virtual LW_RESULT UpdateObject() PURE_METHOD;

		virtual LW_RESULT GetRunTimeOpacity(float* opacity) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimCtrlAgent : public lwInterface {
	public:
		virtual LW_RESULT AddAnimCtrlObj(lwIAnimCtrlObj* obj) PURE_METHOD;
		virtual lwIAnimCtrlObj* RemoveAnimCtrlObj(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual lwIAnimCtrlObj* GetAnimCtrlObj(lwAnimCtrlObjTypeInfo* info) PURE_METHOD;
		virtual lwIAnimCtrlObj* GetAnimCtrlObj(DWORD id) PURE_METHOD;
		virtual DWORD GetAnimCtrlObjNum() PURE_METHOD;
		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Clone(lwIAnimCtrlAgent** ret_obj) PURE_METHOD;
		virtual LW_RESULT ExtractAnimData(lwIAnimDataInfo* data_info) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIShaderDeclMgr : public lwInterface {
	public:
		virtual LW_RESULT CreateShaderDeclSet(DWORD decl_type, DWORD buf_size) PURE_METHOD;
		virtual LW_RESULT SetShaderDeclInfo(lwShaderDeclCreateInfo* info) PURE_METHOD;
		virtual LW_RESULT QueryShaderHandle(DWORD* shader_handle, const lwShaderDeclQueryInfo* info) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIShaderMgr : public lwInterface {
	public:
		virtual lwIShaderDeclMgr* GetShaderDeclMgr() PURE_METHOD;
		virtual LW_RESULT Init(DWORD vs_buf_size, DWORD decl_buf_size, DWORD ps_buf_size) PURE_METHOD;
		virtual LW_RESULT RegisterVertexShader(DWORD type, BYTE* data, DWORD size) PURE_METHOD;
		virtual LW_RESULT RegisterVertexShader(DWORD type, const char* file, DWORD binary_flag,
											   const D3DXMACRO* defines = NULL) PURE_METHOD;
		virtual LW_RESULT RegisterVertexDeclaration(DWORD type, D3DVERTEXELEMENT9* data) PURE_METHOD;
		virtual LW_RESULT QueryVertexShader(IDirect3DVertexShaderX** ret_obj, DWORD type) PURE_METHOD;
		virtual LW_RESULT QueryVertexDeclaration(IDirect3DVertexDeclarationX** ret_obj, DWORD type) PURE_METHOD;
		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;
		virtual lwVertexShaderInfo* GetVertexShaderInfo(DWORD type) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIResBufMgr : public lwInterface {
	public:
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT RegisterSysMemTex(LW_HANDLE* handle, const lwSysMemTexInfo* info) PURE_METHOD;
		virtual LW_RESULT QuerySysMemTex(lwSysMemTexInfo* info) PURE_METHOD;
		virtual LW_RESULT QuerySysMemTex(lwSysMemTexInfo** info, const char* file) PURE_METHOD;
		virtual LW_RESULT GetSysMemTex(lwSysMemTexInfo** info, LW_HANDLE handle) PURE_METHOD;
		virtual LW_RESULT UnregisterSysMemTex(LW_HANDLE handle) PURE_METHOD;
		virtual LW_RESULT RegisterModelObjInfo(LW_HANDLE* handle, const char* file) PURE_METHOD;
		virtual LW_RESULT RegisterModelObjInfo(LW_HANDLE handle, const char* file) PURE_METHOD;
		virtual LW_RESULT QueryModelObjInfo(lwIModelObjInfo** info, const char* file) PURE_METHOD;
		virtual LW_RESULT GetModelObjInfo(lwIModelObjInfo** info, LW_HANDLE handle) PURE_METHOD;
		virtual LW_RESULT UnregisterModelObjInfo(LW_HANDLE handle) PURE_METHOD;
		virtual void SetLimitModelObjInfo(DWORD lmt_size, DWORD lmt_time) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIThreadPoolMgr : public lwInterface {
	public:
		virtual LW_RESULT Create() PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual lwIThreadPool* GetThreadPool(DWORD type) PURE_METHOD;
		virtual void LockCriticalSection(DWORD type) PURE_METHOD;
		virtual void UnlockCriticalSection(DWORD type) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIResourceMgr : public lwInterface {
	public:
		virtual lwISysGraphics* GetSysGraphics() PURE_METHOD;
		virtual lwIDeviceObject* GetDeviceObject() PURE_METHOD;
		virtual lwIShaderMgr* GetShaderMgr() PURE_METHOD;
		virtual lwIStaticStreamMgr* GetStaticStreamMgr() PURE_METHOD;
		virtual lwIDynamicStreamMgr* GetDynamicStreamMgr() PURE_METHOD;
		virtual lwILockableStreamMgr* GetLockableStreamMgr() PURE_METHOD;
		virtual lwISurfaceStreamMgr* GetSurfaceStreamMgr() PURE_METHOD;
		virtual lwIResBufMgr* GetResBufMgr() PURE_METHOD;
		virtual lwIThreadPoolMgr* GetThreadPoolMgr() PURE_METHOD;
		virtual lwIByteSet* GetByteSet() PURE_METHOD;

		virtual LW_RESULT SetAssObjInfo(DWORD mask, const lwAssObjInfo* info) PURE_METHOD;
		virtual LW_RESULT GetAssObjInfo(lwAssObjInfo* info) PURE_METHOD;

		virtual LW_RESULT CreateMesh(lwIMesh** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateTex(lwITex** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateAnimCtrl(lwIAnimCtrl** ret_obj, DWORD type) PURE_METHOD;
		virtual LW_RESULT CreateAnimCtrlObj(lwIAnimCtrlObj** ret_obj, DWORD type) PURE_METHOD;
		virtual LW_RESULT CreateMeshAgent(lwIMeshAgent** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateMtlTexAgent(lwIMtlTexAgent** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateAnimCtrlAgent(lwIAnimCtrlAgent** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateRenderCtrlAgent(lwIRenderCtrlAgent** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateRenderCtrlVS(lwIRenderCtrlVS** ret_obj, DWORD type) PURE_METHOD;
		virtual LW_RESULT CreatePrimitive(lwIPrimitive** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateHelperObject(lwIHelperObject** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreatePhysique(lwIPhysique** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateModel(lwIModel** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateItem(lwIItem** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateNode(lwINode** ret_obj, DWORD type) PURE_METHOD;
		virtual LW_RESULT CreateNodeObject(lwINodeObject** ret_obj) PURE_METHOD;
		virtual LW_RESULT CreateStaticStreamMgr(lwIStaticStreamMgr** mgr) PURE_METHOD;
		virtual LW_RESULT CreateDynamicStreamMgr(lwIDynamicStreamMgr** mgr) PURE_METHOD;

		virtual LW_RESULT RegisterMesh(lwIMesh* obj) PURE_METHOD;
		virtual LW_RESULT RegisterTex(lwITex* obj) PURE_METHOD;
		virtual LW_RESULT RegisterAnimCtrl(lwIAnimCtrl* obj) PURE_METHOD;
		virtual LW_RESULT RegisterRenderCtrlProc(DWORD id, lwRenderCtrlVSCreateProc proc) PURE_METHOD;
		virtual LW_RESULT UnregisterMesh(lwIMesh* obj) PURE_METHOD;
		virtual LW_RESULT UnregisterTex(lwITex* obj) PURE_METHOD;
		virtual LW_RESULT UnregisterAnimCtrl(lwIAnimCtrl* obj) PURE_METHOD;

		virtual LW_RESULT GetMesh(lwIMesh** ret_obj, DWORD id) PURE_METHOD;
		virtual LW_RESULT GetTex(lwITex** ret_obj, DWORD id) PURE_METHOD;
		virtual LW_RESULT GetAnimCtrl(lwIAnimCtrl** ret_obj, DWORD id) PURE_METHOD;

		virtual LW_RESULT AddRefMesh(lwIMesh* obj, DWORD ref_cnt) PURE_METHOD;
		virtual LW_RESULT AddRefTex(lwITex* obj, DWORD ref_cnt) PURE_METHOD;
		virtual LW_RESULT AddRefAnimCtrl(lwIAnimCtrl* ret_obj, DWORD ref_cnt) PURE_METHOD;

		virtual LW_ULONG QueryTexRefCnt(lwITex* obj) PURE_METHOD;

		virtual LW_RESULT QueryMesh(DWORD* ret_id, const lwResFileMesh* info) PURE_METHOD;
		virtual LW_RESULT QueryTex(DWORD* ret_id, const char* file_name) PURE_METHOD;
		virtual LW_RESULT QueryAnimCtrl(DWORD* ret_id, const lwResFileAnimData* info) PURE_METHOD;

		virtual LW_RESULT RegisterObject(DWORD* ret_id, void* obj, DWORD type) PURE_METHOD;
		virtual LW_RESULT UnregisterObject(void** ret_obj, DWORD id, DWORD type) PURE_METHOD;
		virtual LW_RESULT QueryObject(void** ret_obj, DWORD type, const char* file_name) PURE_METHOD;
		virtual LW_RESULT QueryModelObject(void** ret_obj, DWORD model_id) PURE_METHOD;
		virtual LW_RESULT LoseDevice() PURE_METHOD;
		virtual LW_RESULT ResetDevice() PURE_METHOD;

		virtual void SetTexturePath(std::string_view path) PURE_METHOD;
		virtual const std::string& GetTexturePath() PURE_METHOD;
		virtual IDirect3DTextureX* getMonochromaticTexture(D3DCOLOR colour, const std::string& filterTexture)
		PURE_METHOD;
		virtual std::string_view getTextureOperationDescription(size_t operation) PURE_METHOD;
	};


	class LW_DECLSPEC_NOVTABLE lwIRenderCtrlAgent : public lwInterface {
	public:
		virtual void SetMatrix(const lwMatrix44* mat) PURE_METHOD;
		virtual void SetLocalMatrix(const lwMatrix44* mat_local) PURE_METHOD;
		virtual void SetParentMatrix(const lwMatrix44* mat_parent) PURE_METHOD;
		virtual lwMatrix44* GetLocalMatrix() PURE_METHOD;
		virtual lwMatrix44* GetParentMatrix() PURE_METHOD;
		virtual lwMatrix44* GetGlobalMatrix() PURE_METHOD;

		virtual void BindMeshAgent(lwIMeshAgent* agent) PURE_METHOD;
		virtual void BindMtlTexAgent(lwIMtlTexAgent* agent) PURE_METHOD;
		virtual void BindAnimCtrlAgent(lwIAnimCtrlAgent* agent) PURE_METHOD;
		virtual void SetVertexShader(DWORD type) PURE_METHOD;
		virtual void SetVertexDeclaration(DWORD type) PURE_METHOD;
		virtual LW_RESULT SetRenderCtrl(DWORD ctrl_type) PURE_METHOD;
		virtual DWORD GetVertexShader() const PURE_METHOD;
		virtual DWORD GetVertexDeclaration() const PURE_METHOD;

		virtual lwIResourceMgr* GetResourceMgr() PURE_METHOD;
		virtual lwIMeshAgent* GetMeshAgent() PURE_METHOD;
		virtual lwIMtlTexAgent* GetMtlTexAgent() PURE_METHOD;
		virtual lwIAnimCtrlAgent* GetAnimCtrlAgent() PURE_METHOD;
		virtual lwIRenderCtrlVS* GetRenderCtrlVS() PURE_METHOD;

		virtual LW_RESULT Clone(lwIRenderCtrlAgent** ret_obj) PURE_METHOD;
		virtual LW_RESULT BeginSet() PURE_METHOD;
		virtual LW_RESULT EndSet() PURE_METHOD;
		virtual LW_RESULT BeginSetSubset(DWORD subset) PURE_METHOD;
		virtual LW_RESULT EndSetSubset(DWORD subset) PURE_METHOD;

		virtual LW_RESULT DrawSubset(DWORD subset) PURE_METHOD;
		virtual void setPixelShader(const std::string& filename) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIBoundingBox : public lwInterface {
	public:
		virtual int IsValidObject() const PURE_METHOD;
		virtual int IsVisible() const PURE_METHOD;
		virtual void SetVisible(int flag) PURE_METHOD;
		virtual lwBoundingBoxInfo* GetDataInfoWithID(DWORD obj_id) PURE_METHOD;
		virtual lwBoundingBoxInfo* GetDataInfo(DWORD id) PURE_METHOD;
		virtual DWORD GetObjNum() const PURE_METHOD;
		virtual lwMatrix44* GetMatrixParent() PURE_METHOD;

		virtual LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
								  const lwMatrix44* mat_parent) PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
		virtual void SetData(DWORD data) PURE_METHOD;
		virtual DWORD GetData() const PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIBoundingSphere : public lwInterface {
	public:
		virtual int IsValidObject() const PURE_METHOD;
		virtual int IsVisible() const PURE_METHOD;
		virtual void SetVisible(int flag) PURE_METHOD;
		virtual lwBoundingSphereInfo* GetDataInfoWithID(DWORD obj_id) PURE_METHOD;
		virtual lwBoundingSphereInfo* GetDataInfo(DWORD id) PURE_METHOD;
		virtual DWORD GetObjNum() const PURE_METHOD;
		virtual lwMatrix44* GetMatrixParent() PURE_METHOD;

		virtual LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const lwMatrix44* mat)
		PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
		virtual void SetData(DWORD data) PURE_METHOD;
		virtual DWORD GetData() const PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIHelperDummy : public lwInterface {
	public:
		virtual LW_RESULT Render() PURE_METHOD;
		virtual int IsValidObject() const PURE_METHOD;
		virtual int IsVisible() const PURE_METHOD;
		virtual void SetVisible(int flag) PURE_METHOD;
		virtual DWORD GetObjNum() PURE_METHOD;
		virtual lwHelperDummyInfo* GetDataInfo(DWORD id) PURE_METHOD;
		virtual lwHelperDummyInfo* GetDataInfoWithID(DWORD obj_id) PURE_METHOD;
		virtual lwMatrix44* GetMatrixParent() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIHelperMesh : public lwInterface {
	public:
		virtual LW_RESULT Render() PURE_METHOD;
		virtual int IsValidObject() const PURE_METHOD;
		virtual int IsVisible() const PURE_METHOD;
		virtual void SetVisible(int flag) PURE_METHOD;
		virtual lwHelperMeshInfo* GetDataInfoWithID(DWORD obj_id) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIHelperBox : public lwInterface {
	public:
		virtual LW_RESULT Render() PURE_METHOD;
		virtual int IsValidObject() const PURE_METHOD;
		virtual int IsVisible() const PURE_METHOD;
		virtual void SetVisible(int flag) PURE_METHOD;
		virtual lwHelperBoxInfo* GetDataInfoWithID(DWORD obj_id) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIHelperObject : public lwInterface {
	public:
		virtual LW_RESULT LoadHelperInfo(const lwHelperInfo* info, int create_instance_flag) PURE_METHOD;
		virtual LW_RESULT Copy(const lwIHelperObject* src) PURE_METHOD;
		virtual LW_RESULT Clone(lwIHelperObject** ret_obj) PURE_METHOD;
		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;

		virtual void SetParentMatrix(const lwMatrix44* mat) PURE_METHOD;
		virtual void SetVisible(int flag) PURE_METHOD;
		virtual lwIHelperDummy* GetHelperDummy() PURE_METHOD;
		virtual lwIHelperBox* GetHelperBox() PURE_METHOD;
		virtual lwIHelperMesh* GetHelperMesh() PURE_METHOD;
		virtual lwIBoundingBox* GetBoundingBox() PURE_METHOD;
		virtual lwIBoundingSphere* GetBoundingSphere() PURE_METHOD;

		virtual LW_RESULT ExtractHelperInfo(lwIHelperInfo* out_info) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIActionBase : public lwInterface {
	public:
		virtual LW_RESULT Run() PURE_METHOD;
		virtual LW_RESULT Update() PURE_METHOD;
		virtual DWORD GetState() const PURE_METHOD;
		virtual void SetState(DWORD state) PURE_METHOD;
		virtual LW_RESULT Pause() PURE_METHOD;
		virtual LW_RESULT Continue() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIActionGeneric : public lwIActionBase {
	public:
		virtual void SetPose(DWORD pose_id) PURE_METHOD;
		virtual void SetPlayType(DWORD type) PURE_METHOD;

		virtual void SetKeyFrameProc(lwPoseKeyFrameProc proc, void* param) PURE_METHOD;
		virtual LW_RESULT PlayPoseOnce() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIActionMove : public lwIActionBase {
	public:
		virtual void SetPose(DWORD pose_id) PURE_METHOD;
		virtual void SetMoveSpeed(float speed) PURE_METHOD;
		virtual void SetTurnSpeedCofficient(float speed) PURE_METHOD;
		virtual void SetLoaction(const lwVector3* location_seq, DWORD location_num) PURE_METHOD;
		virtual void AddLocation(const lwVector3* location_seq, DWORD location_num) PURE_METHOD;
		virtual float GetMovedDistance() const PURE_METHOD;
		virtual lwVector3* GetActMoveDir() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIActionTurn : public lwIActionBase {
	public:
		virtual void SetPose(DWORD pose_id) PURE_METHOD;

		virtual void SetTurnSpeed(float speed) PURE_METHOD;
		virtual void SetTurnAngle(float angle) PURE_METHOD;
		virtual void SetTargetAngle(float angle) PURE_METHOD;
		virtual void SetTargetDirection(const lwVector3* dst_dir) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIActionMTG : public lwIActionBase {
	public:
		virtual lwIActionBase* GetSubAction(DWORD act_type) PURE_METHOD;
		virtual void SetBlockType(DWORD type) PURE_METHOD;
		virtual DWORD GetCurSubAction() const PURE_METHOD;
	};


	class LW_DECLSPEC_NOVTABLE lwIActionAgent : public lwInterface {
	public:
		virtual lwActionObject* GetActionObject() PURE_METHOD;
		virtual LW_RESULT CreateAction(void** obj, DWORD type) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIActionSmith : public lwInterface {
	public:
		virtual void AllocActionBuffer(DWORD act_num) PURE_METHOD;
		virtual LW_RESULT Init(lwActionObject* act_obj, DWORD act_num) PURE_METHOD;
		virtual void SetNextActionType(DWORD dst_act_type, DWORD next_act_type) PURE_METHOD;

		virtual lwIActionAgent* GetActionAgent() PURE_METHOD;
		virtual lwActionObject* GetActionObject() PURE_METHOD;
		virtual lwActImpBase* GetActionImp(DWORD act_type) PURE_METHOD;
		virtual lwActImpBase* GetCurActionImp() PURE_METHOD;
		virtual DWORD GetCurActionImpType() const PURE_METHOD;
		virtual LW_RESULT LoadActionImp(DWORD type, lwActImpBase* act) PURE_METHOD;
		virtual void SetSubAction(DWORD act_type, void* data, DWORD data_size) PURE_METHOD;
		virtual DWORD GetSubActionType() const PURE_METHOD;
		virtual void* GetSubActionData() PURE_METHOD;

		virtual LW_RESULT Run(DWORD act_type, void* param) PURE_METHOD;
		virtual LW_RESULT Update() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIViewFrustum : public lwInterface {
	public:
		virtual void Update(const lwMatrix44* mat_viewproj) PURE_METHOD;
		virtual lwPlane* GetPlane(DWORD type) PURE_METHOD;
		virtual BOOL IsPointInFrustum(const lwVector3* v) PURE_METHOD;
		virtual BOOL IsSphereInFrustum(const lwSphere* s) PURE_METHOD;
		virtual BOOL IsSphereInFrustum(const lwSphere* s, DWORD* last_check_plane) PURE_METHOD;
		virtual BOOL IsBoxInFrustum(const lwBox* b, const lwMatrix44* b_mat, DWORD* last_check_plane) PURE_METHOD;
		virtual float GetDistanceFromNearPlane(const lwVector3* v) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwISceneMgr : public lwInterface {
	public:
		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
		virtual LW_RESULT BeginRender() PURE_METHOD;
		virtual LW_RESULT EndRender() PURE_METHOD;
		virtual LW_RESULT RenderTransparentPrimitive() PURE_METHOD;
		virtual LW_RESULT AddTransparentPrimitive(lwIPrimitive* obj) PURE_METHOD;
		virtual LW_RESULT SortTransparentPrimitive() PURE_METHOD;
		virtual void SetTranspPrimitiveProc(lwTranspPrimitiveProc proc, void* param) PURE_METHOD;
		virtual LW_RESULT CullPrimitive(lwIPrimitive* obj) PURE_METHOD;
		virtual lwIViewFrustum* GetViewFrustum() PURE_METHOD;
		virtual DWORD GetMaxSortNum() const PURE_METHOD;
		virtual void GetCullPrimitiveInfo(lwCullPriInfo* info) PURE_METHOD;
		virtual lwSceneFrameInfo* GetSceneFrameInfo() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimKeySetPRS : public lwInterface {
	public:
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT GetValue(lwMatrix44* mat, float frame) PURE_METHOD;
		virtual LW_RESULT AddKeyPosition(DWORD id, const lwKeyVector3* data, DWORD num) PURE_METHOD;
		virtual LW_RESULT AddKeyRotation(DWORD id, const lwKeyQuaternion* data, DWORD num) PURE_METHOD;
		virtual LW_RESULT AddKeyScale(DWORD id, const lwKeyVector3* data, DWORD num);
		virtual LW_RESULT DelKeyPosition(DWORD id, DWORD num) PURE_METHOD;
		virtual LW_RESULT DelKeyRotation(DWORD id, DWORD num) PURE_METHOD;
		virtual LW_RESULT DelKeyScale(DWORD id, DWORD num) PURE_METHOD;
		virtual DWORD GetFrameNum() const PURE_METHOD;
		virtual DWORD GetKeyPositionNum() const PURE_METHOD;
		virtual DWORD GetKeyRotationNum() const PURE_METHOD;
		virtual DWORD GetKeyScaleNum() const PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIAnimKeySetFloat : public lwInterface {
	public:
		virtual LW_RESULT Allocate(DWORD key_capacity) PURE_METHOD;
		virtual LW_RESULT Clear() PURE_METHOD;
		virtual LW_RESULT Clone(lwIAnimKeySetFloat** obj) PURE_METHOD;
		virtual LW_RESULT SetKeyData(DWORD key, float data, DWORD slerp_type, DWORD mask) PURE_METHOD;
		virtual LW_RESULT GetKeyData(DWORD key, float* data, DWORD* slerp_type) PURE_METHOD;
		virtual LW_RESULT InsertKey(DWORD key, float data, DWORD slerp_type) PURE_METHOD;
		virtual LW_RESULT RemoveKey(DWORD key) PURE_METHOD;
		virtual LW_RESULT GetValue(float* data, float key) PURE_METHOD;
		virtual LW_RESULT SetKeySequence(const lwKeyFloat* seq, DWORD num) PURE_METHOD;
		virtual lwKeyFloat* GetKeySequence() PURE_METHOD;
		virtual DWORD GetKeyNum() PURE_METHOD;
		virtual DWORD GetKeyCapacity() PURE_METHOD;
		virtual DWORD GetBeginKey() PURE_METHOD;
		virtual DWORD GetEndKey() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwINode : public lwInterface {
	public:
		// base method
		virtual void SetID(DWORD id) PURE_METHOD;
		virtual void SetLinkID(DWORD id) PURE_METHOD;
		virtual void SetParentLinkID(DWORD id) PURE_METHOD;
		virtual void SetLocalMatrix(const lwMatrix44* mat) PURE_METHOD;
		virtual void SetDescriptor(std::string_view str64) PURE_METHOD;
		virtual LW_RESULT SetParent(lwINode* parent) PURE_METHOD;
		virtual DWORD GetType() const PURE_METHOD;
		virtual DWORD GetID() const PURE_METHOD;
		virtual lwMatrix44* GetLocalMatrix() PURE_METHOD;
		virtual lwMatrix44* GetWorldMatrix() PURE_METHOD;
		virtual const std::string& GetDescriptor() PURE_METHOD;
		virtual lwINode* GetParent() const PURE_METHOD;
		virtual DWORD GetLinkID() const PURE_METHOD;
		virtual DWORD GetParentLinkID() const PURE_METHOD;
		virtual lwIByteSet* GetStateSet() PURE_METHOD;
		virtual LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id) PURE_METHOD;
		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwINodePrimitive : public lwINode {
	public:
		// primitive method
		virtual void SetMeshAgent(lwIMeshAgent* agent) PURE_METHOD;
		virtual void SetMtlTexAgent(DWORD subset, lwIMtlTexAgent* agent) PURE_METHOD;
		virtual void SetAnimCtrlAgent(lwIAnimCtrlAgent* agent) PURE_METHOD;
		virtual void SetRenderCtrl(lwIRenderCtrlAgent* obj) PURE_METHOD;
		virtual void SetHelperObject(lwIHelperObject* obj) PURE_METHOD;
		virtual void SetMaterial(const lwMaterial* mtl) PURE_METHOD;
		virtual void SetOpacity(float opacity) PURE_METHOD;

		virtual lwIResourceMgr* GetResourceMgr() PURE_METHOD;
		virtual lwIMtlTexAgent* GetMtlTexAgent(DWORD id) PURE_METHOD;
		virtual lwIMeshAgent* GetMeshAgent() PURE_METHOD;
		virtual lwIAnimCtrlAgent* GetAnimCtrlAgent() PURE_METHOD;
		virtual lwIRenderCtrlAgent* GetRenderCtrlAgent() PURE_METHOD;
		virtual lwIHelperObject* GetHelperObject() PURE_METHOD;
		virtual LW_RESULT AllocateMtlTexAgentSeq(DWORD num) PURE_METHOD;
		virtual DWORD GetMtlTexAgentSeqSize() const PURE_METHOD;
		virtual LW_RESULT GetSubsetNum(DWORD* subset_num) PURE_METHOD;

		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT Load(lwIGeomObjInfo* info, const char* tex_path, const lwResFile* res) PURE_METHOD;
		virtual LW_RESULT LoadMesh(lwMeshInfo* info) PURE_METHOD;
		virtual LW_RESULT LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, const char* tex_path) PURE_METHOD;
		virtual LW_RESULT LoadAnimData(lwIAnimDataInfo* info, const char* tex_path, const lwResFile* res) PURE_METHOD;
		virtual LW_RESULT LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci) PURE_METHOD;
		virtual LW_RESULT Copy(lwINodePrimitive* src_obj) PURE_METHOD;

		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
		virtual LW_RESULT RenderSubset(DWORD subset) PURE_METHOD;
		virtual LW_RESULT RenderHelperObject() PURE_METHOD;

		virtual LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwINodeBoneCtrl : public lwINode {
	public:
		virtual LW_RESULT Load(lwIAnimDataBone* data) PURE_METHOD;
		virtual LW_RESULT PlayPose(const lwPlayPoseInfo* info) PURE_METHOD;
		virtual void SetAnimCtrlObj(lwIAnimCtrlObjBone* ctrl_obj) PURE_METHOD;
		virtual lwIAnimCtrlObjBone* GetAnimCtrlObj() PURE_METHOD;
		virtual lwIPoseCtrl* GetPoseCtrl() PURE_METHOD;
		virtual lwPlayPoseInfo* GetPlayPoseInfo() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwINodeDummy : public lwINode {
	public:
		virtual LW_RESULT Load(lwIHelperDummyObjInfo* data) PURE_METHOD;
		virtual void SetAnimCtrlObj(lwIAnimCtrlObjMat* ctrl_obj) PURE_METHOD;
		virtual lwIAnimCtrlObjMat* GetAnimCtrlObj() PURE_METHOD;
		virtual LW_RESULT CreateAssistantObject(const lwVector3* size, DWORD color) PURE_METHOD;
		virtual lwINodePrimitive* GetAssistantObject() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwINodeHelper : public lwINode {
	public:
	};

	enum {
		MAX_IGNORE_NODES = 32
	};

	struct IgnoreStruct {
		lwITreeNode* nodes[MAX_IGNORE_NODES];
	};

	class LW_DECLSPEC_NOVTABLE lwINodeObject : public lwInterface {
	public:
		virtual LW_RESULT Update() PURE_METHOD;
		virtual LW_RESULT Render() PURE_METHOD;
		virtual LW_RESULT IgnoreNodesRender(const IgnoreStruct* is) PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT CullPrimitive() PURE_METHOD;
		virtual void SetName(std::string_view name) PURE_METHOD;
		virtual void SetMatrix(const lwMatrix44* mat) PURE_METHOD;
		virtual const std::string& GetName() PURE_METHOD;
		virtual lwMatrix44* GetMatrix() PURE_METHOD;
		virtual lwIByteSet* GetStateSet() PURE_METHOD;
		virtual lwITreeNode* GetTreeNodeRoot() PURE_METHOD;
		virtual LW_RESULT QueryTreeNode(lwModelNodeQueryInfo* info) PURE_METHOD;
		virtual LW_RESULT InsertTreeNode(lwITreeNode* parent_node, lwITreeNode* prev_node, lwITreeNode* node)
		PURE_METHOD;
		virtual LW_RESULT RemoveTreeNode(lwITreeNode* node) PURE_METHOD;
		virtual LW_RESULT Load(const char* file, DWORD flag, lwITreeNode* parent_node) PURE_METHOD;
	};


	class LW_DECLSPEC_NOVTABLE lwITreeNode : public lwInterface {
	public:
		virtual LW_RESULT EnumTree(lwTreeNodeEnumProc proc, void* param, DWORD enum_type) PURE_METHOD;
		virtual LW_RESULT InsertChild(lwITreeNode* prev_node, lwITreeNode* node) PURE_METHOD;
		virtual LW_RESULT RemoveChild(lwITreeNode* node) PURE_METHOD;
		virtual lwITreeNode* FindNode(lwITreeNode* node) PURE_METHOD;
		virtual lwITreeNode* FindNode(void* data) PURE_METHOD;
		virtual void SetParent(lwITreeNode* node) PURE_METHOD;
		virtual void SetChild(lwITreeNode* node) PURE_METHOD;
		virtual void SetSibling(lwITreeNode* node) PURE_METHOD;
		virtual void SetData(void* data) PURE_METHOD;
		virtual lwITreeNode* GetParent() PURE_METHOD;
		virtual lwITreeNode* GetChild() PURE_METHOD;
		virtual lwITreeNode* GetChild(DWORD id) PURE_METHOD;
		virtual lwITreeNode* GetSibling() PURE_METHOD;
		virtual void* GetData() PURE_METHOD;
		virtual DWORD GetNodeNum() const PURE_METHOD;
		virtual DWORD GetChildNum() const PURE_METHOD;
		virtual DWORD GetDepthLevel() const PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIRenderStateAtomSet : public lwInterface {
	public:
		virtual LW_RESULT Allocate(DWORD size) PURE_METHOD;
		virtual LW_RESULT Clear() PURE_METHOD;
		virtual LW_RESULT Clone(lwIRenderStateAtomSet** obj) PURE_METHOD;
		virtual LW_RESULT Load(const lwRenderStateAtom* rsa_seq, DWORD rsa_num) PURE_METHOD;
		virtual LW_RESULT FindState(DWORD* id, DWORD state) PURE_METHOD;
		virtual LW_RESULT ResetStateValue(DWORD state, DWORD value, DWORD* old_value) PURE_METHOD;
		virtual LW_RESULT SetStateValue(DWORD id, DWORD state, DWORD value) PURE_METHOD;
		virtual LW_RESULT SetStateValue(DWORD id, lwRenderStateAtom* buf, DWORD num) PURE_METHOD;
		virtual LW_RESULT BeginRenderState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num) PURE_METHOD;
		virtual LW_RESULT BeginTextureStageState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num, DWORD stage)
		PURE_METHOD;
		virtual LW_RESULT BeginSamplerState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num, DWORD stage)
		PURE_METHOD;
		virtual LW_RESULT EndRenderState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num) PURE_METHOD;
		virtual LW_RESULT EndTextureStageState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num, DWORD stage)
		PURE_METHOD;
		virtual LW_RESULT EndSamplerState(lwIDeviceObject* dev_obj, DWORD start_id, DWORD num, DWORD stage) PURE_METHOD;
		virtual DWORD GetStateNum() const PURE_METHOD;
		virtual lwRenderStateAtom* GetStateSeq() PURE_METHOD;
		virtual LW_RESULT GetStateAtom(lwRenderStateAtom** rsa, DWORD id) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwITimer : public lwInterface {
	public:
		virtual LW_RESULT OnTimer() PURE_METHOD;
		virtual LW_RESULT SetTimer(DWORD id, lwTimerProc proc, DWORD interval, DWORD hit_type, DWORD hit_add_cnt)
		PURE_METHOD;
		virtual LW_RESULT SetTimerInterval(DWORD id, DWORD interval) PURE_METHOD;
		virtual DWORD GetLastInvokeTime(DWORD id) PURE_METHOD;
		virtual LW_RESULT Pause(DWORD id, DWORD flag) PURE_METHOD; // id == LW_INVALID_INDEX : all
		virtual LW_RESULT ResetTimer(DWORD id) PURE_METHOD; // id == LW_INVALID_INDEX : all
		virtual LW_RESULT ClearTimer(DWORD id) PURE_METHOD; // id == LW_INVALID_INDEX : all
		virtual LW_RESULT ReallocTimerSeq(DWORD size) PURE_METHOD;
		virtual DWORD GetTimerTickCount() PURE_METHOD;
		virtual DWORD GetTickCount() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwITimerThread : public lwInterface {
	public:
		virtual LW_RESULT AllocateTimerSeq(DWORD size) PURE_METHOD;
		virtual LW_RESULT SetTimer(DWORD id, lwTimerProc proc, DWORD interval) PURE_METHOD;
		virtual LW_RESULT SetTimerInterval(DWORD id, DWORD interval) PURE_METHOD;
		virtual LW_RESULT Pause(DWORD id, DWORD flag) PURE_METHOD; // id == LW_INVALID_INDEX : all
		virtual LW_RESULT ClearTimer(DWORD id, DWORD delay) PURE_METHOD; // id == LW_INVALID_INDEX : all
	};

	class LW_DECLSPEC_NOVTABLE lwITimerPeriod : public lwInterface {
	public:
		virtual LW_RESULT SetEvent(UINT delay, UINT resolution, LPTIMECALLBACK proc, DWORD_PTR param, UINT event)
		PURE_METHOD;
		virtual LW_RESULT KillEvent() PURE_METHOD;
		virtual UINT GetTimerID() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIFile : public lwInterface {
	public:
		virtual LW_RESULT CreateFile(const char* file, DWORD access_flag, DWORD share_mode,
									 LPSECURITY_ATTRIBUTES secu_attr, DWORD creation_flag,
									 DWORD attributes_flag = FILE_FLAG_SEQUENTIAL_SCAN) PURE_METHOD;
		virtual LW_RESULT CreateDirectory(const char* path, LPSECURITY_ATTRIBUTES attr) PURE_METHOD;
		virtual LW_RESULT LoadFileBuffer(const char* file, lwIBuffer* buf) PURE_METHOD;
		virtual LW_RESULT SaveFileBuffer(const char* file, lwIBuffer* buf) PURE_METHOD;
		virtual LW_RESULT Close() PURE_METHOD;
		virtual LW_RESULT Read(void* buf, DWORD in_size, DWORD* out_size) PURE_METHOD;
		virtual LW_RESULT Write(const void* buf, DWORD in_size, DWORD* out_size) PURE_METHOD;

		virtual HANDLE GetHandle() const PURE_METHOD;
		virtual const std::string& GetFileName() PURE_METHOD;
		virtual LW_RESULT GetCreationTime(SYSTEMTIME* st) PURE_METHOD;
		virtual LW_RESULT CheckExisting(const char* path, DWORD check_directory) PURE_METHOD;

		// Seek Description
		// flag[in]: FILE_BEGIN / FILE_CURRENT / FILE_END
		virtual LW_RESULT Seek(long offset, DWORD flag) PURE_METHOD;
		virtual DWORD GetSize() PURE_METHOD;
		virtual LW_RESULT Flush() PURE_METHOD;
		virtual LW_RESULT SetEnd() PURE_METHOD;

		virtual LW_RESULT MoveData(DWORD src_pos, DWORD dst_pos, DWORD size) PURE_METHOD;
		virtual LW_RESULT ReplaceData(DWORD pos, const void* buf, DWORD size) PURE_METHOD;
		virtual LW_RESULT InsertData(DWORD pos, const void* buf, DWORD size) PURE_METHOD;
		virtual LW_RESULT RemoveData(DWORD pos, DWORD size) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIFileDialog : public lwInterface {
	public:
		// int flag = OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY
		virtual LW_RESULT GetOpenFileName(HWND hwnd, char* buf, int num, const char* dir, const char* title = 0,
										  const char* filter = "all files(*.*)\0*.*\0\0",
										  int flag = OFN_PATHMUSTEXIST | OFN_EXPLORER) PURE_METHOD;

		virtual LW_RESULT GetSaveFileName(HWND hwnd, char* buf, int num, const char* dir, const char* title = 0,
										  const char* filter = "all files(*.*)\0*.*\0\0", const char* ext = 0,
										  int flag = OFN_PATHMUSTEXIST | OFN_EXPLORER) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIDDSFile : public lwInterface {
	public:
		virtual void SetDevice(IDirect3DDeviceX* dev) PURE_METHOD;
		virtual LW_RESULT Clear() PURE_METHOD;
		virtual LW_RESULT LoadOriginTexture(const char* file, DWORD mip_level, D3DFORMAT format, DWORD colorkey)
		PURE_METHOD;
		virtual LW_RESULT Convert(const char* file, D3DFORMAT src_fmt, D3DFORMAT dds_fmt, DWORD mip_level,
								  DWORD src_colorkey) PURE_METHOD;
		virtual LW_RESULT Save(const char* file) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIDirectoryBrowser : public lwInterface {
	public:
		virtual void SetBrowseProc(lwDirectoryBrowserProc proc, void* param) PURE_METHOD;
		virtual LW_RESULT Browse(const char* file, DWORD flag) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIConsole : public lwInterface {
	public:
		virtual LW_RESULT Alloc() PURE_METHOD;
		virtual LW_RESULT Create(DWORD desired_access = GENERIC_READ | GENERIC_WRITE, DWORD share_mode = 0,
								 const SECURITY_ATTRIBUTES* security_attr = 0) PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT Write(const char* str, ...) PURE_METHOD;
		virtual LW_RESULT SetBufferSize(DWORD row, DWORD column) PURE_METHOD;
		virtual HANDLE GetHandle() const PURE_METHOD;
		virtual BOOL SetActive() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIThreadPool : public lwInterface {
	public:
		virtual LW_RESULT Create(DWORD thread_seq_size, DWORD task_seq_size, DWORD suspend_flag) PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT RegisterTask(lwThreadProc proc, void* param) PURE_METHOD;
		virtual LW_RESULT RemoveTask(lwThreadProc proc, void* param) PURE_METHOD;
		virtual LW_RESULT FindTask(lwThreadProc proc, void* param) PURE_METHOD;
		virtual LW_RESULT SuspendThread() PURE_METHOD;
		virtual LW_RESULT ResumeThread() PURE_METHOD;
		virtual LW_RESULT SetPriority(int priority) PURE_METHOD;
		virtual LW_RESULT SetPoolEvent(BOOL lock_flag) PURE_METHOD;
		virtual int GetPriority() const PURE_METHOD;
		virtual DWORD GetCurrentWaitingTaskNum() const PURE_METHOD;
		virtual DWORD GetCurrentRunningTaskNum() const PURE_METHOD;
		virtual DWORD GetCurrentIdleThreadNum() const PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIVertexBuffer : public lwInterface {
	public:
		virtual LW_RESULT Create(UINT length, DWORD usage, DWORD fvf, D3DPOOL pool, DWORD stride, HANDLE* handle)
		PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT Lock(UINT offset, UINT size, D3DLOCK_TYPE** out_data, DWORD flag) PURE_METHOD;
		virtual LW_RESULT Unlock() PURE_METHOD;
		virtual LW_RESULT LoadData(const void* data_seq, DWORD data_size, UINT offset, DWORD lock_flag) PURE_METHOD;
		virtual LW_RESULT LoadDataDynamic(const void* data_seq, DWORD data_size) PURE_METHOD;
		virtual LW_RESULT BindDevice(DWORD stream_id, UINT offset_byte) PURE_METHOD;
		virtual LW_RESULT SetDeviceFVF() PURE_METHOD;
		virtual lwVertexBufferInfo* GetBufferInfo() PURE_METHOD;
		virtual IDirect3DVertexBufferX* GetStreamBuffer() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIIndexBuffer : public lwInterface {
	public:
		virtual LW_RESULT Create(UINT length, DWORD usage, D3DFORMAT format, D3DPOOL pool, HANDLE* handle) PURE_METHOD;
		virtual LW_RESULT Destroy() PURE_METHOD;
		virtual LW_RESULT Lock(UINT offset, UINT size, D3DLOCK_TYPE** out_data, DWORD flag) PURE_METHOD;
		virtual LW_RESULT Unlock() PURE_METHOD;
		virtual LW_RESULT LoadData(const void* data_seq, DWORD data_size, UINT offset, DWORD lock_flag) PURE_METHOD;
		virtual LW_RESULT LoadDataDynamic(const void* data_seq, DWORD data_size) PURE_METHOD;
		virtual LW_RESULT BindDevice() PURE_METHOD;
		virtual lwIndexBufferInfo* GetBufferInfo() PURE_METHOD;
		virtual IDirect3DIndexBufferX* GetStreamBuffer() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwITextFileLoader : public lwInterface {
	public:
		virtual LW_RESULT Load(const char* file) PURE_METHOD;
		virtual LW_RESULT Close() PURE_METHOD;
		virtual LW_RESULT Eof() const PURE_METHOD;
		virtual LW_RESULT SeekWord(const char* str, const char* termi = 0) PURE_METHOD;
		virtual int SkipSpace() PURE_METHOD;
		virtual int SkipLine(int num = 1) PURE_METHOD;
		virtual int SkipWord(int num = 1) PURE_METHOD;
		virtual char* GetWord(char* buf, int be_remove = 1) PURE_METHOD;
		virtual char* GetLine(char* buf, int len = 256) PURE_METHOD;
		virtual int GetWordSeq(char* buf, int num) PURE_METHOD;
		virtual int GetWordSeq(short* buf, int num) PURE_METHOD;
		virtual int GetWordSeq(int* buf, int num) PURE_METHOD;
		virtual int GetWordSeq(long* buf, int num) PURE_METHOD;
		virtual int GetWordSeq(float* buf, int num) PURE_METHOD;
		virtual int GetWordSeq(double* buf, int num) PURE_METHOD;
		virtual int GetWordSeq(char** buf, int num) PURE_METHOD;
		virtual int GetWordSeq(unsigned int* buf, int num) PURE_METHOD;
		virtual int GetWordSeq(unsigned long* buf, int num) PURE_METHOD;
		virtual int GetStringChunk(char* buf, int len, const char* termi) PURE_METHOD;
		virtual int GetIntWithStrHex() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwISystemInfo : public lwInterface {
	public:
		virtual LW_RESULT CheckDirectXVersion() PURE_METHOD;
		virtual DWORD GetDirectXVersion() PURE_METHOD;
		virtual LW_RESULT GetDirectXVersion(lwDxVerInfo* o_info) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIRenderCtrlVS : public lwInterface {
	public:
		virtual DWORD GetType() PURE_METHOD;
		virtual LW_RESULT Clone(lwIRenderCtrlVS** obj) PURE_METHOD;
		virtual LW_RESULT Initialize(lwIRenderCtrlAgent* agent) PURE_METHOD;
		virtual LW_RESULT BeginSet(lwIRenderCtrlAgent* agent) PURE_METHOD;
		virtual LW_RESULT EndSet(lwIRenderCtrlAgent* agent) PURE_METHOD;
		virtual LW_RESULT BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent) PURE_METHOD;
		virtual LW_RESULT EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent) PURE_METHOD;

		virtual void setPixelShader(const std::string& filename) {
		}
	};

	class LW_DECLSPEC_NOVTABLE lwIBuffer : public lwInterface {
	public:
		virtual LW_RESULT Alloc(DWORD size) PURE_METHOD;
		virtual LW_RESULT Realloc(DWORD size) PURE_METHOD;
		virtual LW_RESULT Free() PURE_METHOD;
		virtual LW_RESULT SetSizeArbitrary(DWORD size) PURE_METHOD;
		virtual BYTE* GetData() PURE_METHOD;
		virtual DWORD GetSize() PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIMeshInfoEx : public lwInterface {
	public:
		virtual LW_RESULT Clear() PURE_METHOD;
		virtual LW_RESULT Clone(lwIMeshInfoEx** info) PURE_METHOD;
		virtual LW_RESULT SetFVF(DWORD fvf) PURE_METHOD;
		virtual LW_RESULT SetPrimitiveType(D3DPRIMITIVETYPE pt_type) PURE_METHOD;
		virtual LW_RESULT SetVertexElement(const D3DVERTEXELEMENTX* data_seq) PURE_METHOD;
		virtual LW_RESULT SetBoneInfluenceFactor(DWORD factor) PURE_METHOD;
		virtual LW_RESULT SetRenderStateAtom(lwIRenderStateAtomSet* rsa) PURE_METHOD;
		virtual LW_RESULT SetVertexSequence(DWORD stream_id, const lwVector3* data_seq, DWORD data_num) PURE_METHOD;
		virtual LW_RESULT SetNormalSequence(DWORD stream_id, const lwVector3* data_seq, DWORD data_num) PURE_METHOD;
		virtual LW_RESULT SetColorSequence(DWORD stream_id, const lwColorValue4f* data_seq, DWORD data_num) PURE_METHOD;
		virtual LW_RESULT SetTexcoordSequence(DWORD stream_id, const lwVector2* data_seq, DWORD data_num) PURE_METHOD;
		virtual LW_RESULT SetIndexSequence(DWORD stream_id, const DWORD* data_seq, DWORD data_num) PURE_METHOD;
		virtual LW_RESULT SetSubsetSequence(DWORD stream_id, const lwSubsetInfo* data_seq, DWORD data_num) PURE_METHOD;
		virtual LW_RESULT SetVertexBlendSequence(const lwBlendInfo* data_seq, DWORD data_num) PURE_METHOD;
		virtual LW_RESULT SetBoneIndexSequence(const DWORD* data_seq, DWORD data_num) PURE_METHOD;
		virtual LW_RESULT GetDataType(DWORD* type) PURE_METHOD;
		virtual LW_RESULT GetFVF(DWORD* fvf) PURE_METHOD;
		virtual LW_RESULT GetPrimitiveType(D3DPRIMITIVETYPE* type) PURE_METHOD;
		virtual LW_RESULT GetBoneInfluenceFactor(DWORD* factor) PURE_METHOD;
		virtual LW_RESULT GetVertexElement(D3DVERTEXELEMENTX** desc) PURE_METHOD;
		virtual LW_RESULT GetRenderStateAtom(lwIRenderStateAtomSet** rsa) PURE_METHOD;
		virtual LW_RESULT GetVertexSequence(lwVector3** data_seq, DWORD* data_num, DWORD stream_id) PURE_METHOD;
		virtual LW_RESULT GetNormalSequence(lwVector3** data_seq, DWORD* data_num, DWORD stream_id) PURE_METHOD;
		virtual LW_RESULT GetColorSequence(lwColorValue4f** data_seq, DWORD* data_num, DWORD stream_id) PURE_METHOD;
		virtual LW_RESULT GetTexCoordSequence(lwVector2** data_seq, DWORD* data_num, DWORD stream_id) PURE_METHOD;
		virtual LW_RESULT GetIndexSequence(DWORD** data_seq, DWORD* data_num, DWORD stream_id) PURE_METHOD;
		virtual LW_RESULT GetSubsetSequence(lwSubsetInfo** data_seq, DWORD* data_num, DWORD stream_id) PURE_METHOD;
		virtual LW_RESULT GetVertexBlendSequence(lwBlendInfo** data_seq, DWORD* data_num) PURE_METHOD;
		virtual LW_RESULT GetBoneIndexSequence(DWORD** data_seq, DWORD* data_num) PURE_METHOD;
		virtual LW_RESULT LoadFile(lwIFileStream* fs) PURE_METHOD;
		virtual LW_RESULT SaveFile(lwIFileStream* fs) PURE_METHOD;
		virtual LW_RESULT DumpData(const char* file) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwICoordinateSys : public lwInterface {
	public:
		virtual LW_RESULT SetActiveCoordSys(DWORD id) PURE_METHOD;
		virtual LW_RESULT SetCoordSysMatrix(DWORD id, const lwMatrix44* mat) PURE_METHOD;
		virtual LW_RESULT GetCoordSysMatrix(DWORD id, lwMatrix44* mat) PURE_METHOD;
		virtual LW_RESULT GetCurCoordSysMatrix(lwMatrix44* mat) PURE_METHOD;
		virtual LW_RESULT GetActiveCoordSys(DWORD* id) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIHeap : public lwInterface {
	public:
		virtual LW_RESULT Reserve(DWORD size) PURE_METHOD;
		virtual LW_RESULT Clear() PURE_METHOD;
		virtual LW_RESULT Push(const void* t) PURE_METHOD;
		virtual LW_RESULT Push(const void* buf, DWORD size) PURE_METHOD;
		virtual LW_RESULT Pop() PURE_METHOD;
		virtual LW_RESULT Top(void** t) PURE_METHOD;
		virtual LW_RESULT Find(DWORD* id, const void* t) PURE_METHOD;
		virtual LW_RESULT Update(DWORD id) PURE_METHOD;
		virtual LW_RESULT Remove(DWORD id) PURE_METHOD;
		virtual void SetCompareProc(lwHeapCompProc proc) PURE_METHOD;
		virtual void SetFilterProc(lwHeapFilterProc proc) PURE_METHOD;
		virtual lwHeapCompProc GetCompareProc() PURE_METHOD;
		virtual lwHeapFilterProc GetFilterProc() PURE_METHOD;
		virtual DWORD GetHeapNum() PURE_METHOD;
		virtual DWORD GetHeapCapacity() PURE_METHOD;
		virtual void* GetBuffer() PURE_METHOD;
		virtual LW_RESULT Clone(lwIHeap** out_heap) PURE_METHOD;
	};

	class LW_DECLSPEC_NOVTABLE lwIFileStream : public lwInterface {
	public:
		virtual LW_RESULT Open(const char* file, const lwFileStreamOpenInfo* info) PURE_METHOD;
		virtual LW_RESULT Close() PURE_METHOD;
		virtual LW_RESULT Read(void* buf, DWORD in_size, DWORD* out_size) PURE_METHOD;
		virtual LW_RESULT Write(const void* buf, DWORD in_size, DWORD* out_size) PURE_METHOD;
		virtual LW_RESULT Seek(DWORD* pos, long offset, DWORD flag) PURE_METHOD;
		virtual LW_RESULT GetSize(DWORD* size) PURE_METHOD;
		virtual LW_RESULT Flush() PURE_METHOD;
		virtual LW_RESULT SetEnd() PURE_METHOD;
	};

LW_END
