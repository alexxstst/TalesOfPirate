//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwMath.h"
#include "lwExpObj.h"
#include "lwITypes.h"
#include "lwMisc.h"
#include "lwInterfaceExt.h"

LW_BEGIN
	class lwNodeBase {
	protected:
		// base
		DWORD _type;
		DWORD _id;
		char _descriptor[64];
		lwMatrix44 _mat_local;
		lwMatrix44 _mat_world;
		lwByteSet _state_set;

		// link state
		lwINode* _parent;
		DWORD _link_parent_id;
		DWORD _link_id;

	protected:
		BOOL _CheckVisibleState() {
			return _state_set.GetValue(STATE_VISIBLE);
		}

	public:
		lwNodeBase();
	};

	class lwNodePrimitive : public lwNodeBase, public lwINodePrimitive {
		LW_STD_DECLARATION()

	private:
		lwIResourceMgr* _res_mgr;
		lwIMeshAgent* _mesh_agent;
		lwIMtlTexAgent** _mtltex_agent_seq;
		lwIAnimCtrlAgent* _anim_agent;
		lwIRenderCtrlAgent* _render_agent;
		lwIHelperObject* _helper_object;
		lwIAnimCtrlObjBone* _ref_ctrl_obj_bone;

		DWORD _mtltex_agent_seqsize;

	private:
		LW_RESULT _DestroyMtlTexAgent();
		LW_RESULT _UpdateBoundingObject(lwIAnimCtrlObjBone* ctrl_obj);
		LW_RESULT _UpdateTransparentState();

	public:
		lwNodePrimitive(lwIResourceMgr* res_mgr);
		virtual ~lwNodePrimitive();

		// base method
		void SetID(DWORD id) {
			_id = id;
		}

		void SetLinkID(DWORD id) {
			_link_id = id;
		}

		void SetParentLinkID(DWORD id) {
			_link_parent_id = id;
		}

		LW_RESULT SetParent(lwINode* parent);

		void SetLocalMatrix(const lwMatrix44* mat) {
			_mat_local = *mat;
		}

		void SetDescriptor(const char* str64) {
			_tcscpy(_descriptor, str64);
		}

		DWORD GetType() const {
			return _type;
		}

		DWORD GetID() const {
			return _id;
		}

		lwMatrix44* GetLocalMatrix() {
			return &_mat_local;
		}

		lwMatrix44* GetWorldMatrix() {
			return &_mat_world;
		}

		char* GetDescriptor() {
			return _descriptor;
		}

		lwINode* GetParent() const {
			return _parent;
		}

		DWORD GetLinkID() const {
			return _link_id;
		}

		DWORD GetParentLinkID() const {
			return _link_parent_id;
		}

		lwIByteSet* GetStateSet() {
			return &_state_set;
		}

		LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id);

		// primitive method
		void SetMeshAgent(lwIMeshAgent* agent) {
			_mesh_agent = agent;
		}

		void SetMtlTexAgent(DWORD subset, lwIMtlTexAgent* agent) {
			_mtltex_agent_seq[subset] = agent;
		}

		void SetAnimCtrlAgent(lwIAnimCtrlAgent* agent) {
			_anim_agent = agent;
		}

		void SetRenderCtrl(lwIRenderCtrlAgent* obj) {
			_render_agent = obj;
		}

		void SetHelperObject(lwIHelperObject* obj) {
			_helper_object = obj;
		}

		void SetMaterial(const lwMaterial* mtl);
		void SetOpacity(float opacity);

		lwIResourceMgr* GetResourceMgr() {
			return _res_mgr;
		}

		lwIMtlTexAgent* GetMtlTexAgent(DWORD id) {
			return _mtltex_agent_seq[id];
		}

		lwIMeshAgent* GetMeshAgent() {
			return _mesh_agent;
		}

		lwIAnimCtrlAgent* GetAnimCtrlAgent() {
			return _anim_agent;
		}

		lwIRenderCtrlAgent* GetRenderCtrlAgent() {
			return _render_agent;
		}

		lwIHelperObject* GetHelperObject() {
			return _helper_object;
		}

		LW_RESULT AllocateMtlTexAgentSeq(DWORD num);

		DWORD GetMtlTexAgentSeqSize() const {
			return _mtltex_agent_seqsize;
		}

		LW_RESULT GetSubsetNum(DWORD* subset_num);


		LW_RESULT Destroy();
		LW_RESULT Load(lwIGeomObjInfo* info, const char* tex_path, const lwResFile* res);
		LW_RESULT LoadMesh(lwMeshInfo* info);
		LW_RESULT LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, const char* tex_path);
		LW_RESULT LoadAnimData(lwIAnimDataInfo* info, const char* tex_path, const lwResFile* res);
		LW_RESULT LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci);
		LW_RESULT Copy(lwINodePrimitive* src_obj);

		LW_RESULT Update();
		LW_RESULT Render();
		LW_RESULT RenderSubset(DWORD subset);
		LW_RESULT RenderHelperObject();


		LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);
	};

	class lwNodeBoneCtrl : public lwNodeBase, public lwINodeBoneCtrl {
		LW_STD_DECLARATION()

	private:
		lwIResourceMgr* _res_mgr;
		lwIAnimCtrlObjBone* _ctrl_obj;

	public:
		lwNodeBoneCtrl(lwIResourceMgr* res_mgr);
		~lwNodeBoneCtrl();

		// base method
		void SetID(DWORD id) {
			_id = id;
		}

		void SetLinkID(DWORD id) {
			_link_id = id;
		}

		void SetParentLinkID(DWORD id) {
			_link_parent_id = id;
		}

		LW_RESULT SetParent(lwINode* parent) {
			_parent = parent;
			return LW_RET_OK;
		}

		void SetLocalMatrix(const lwMatrix44* mat) {
			_mat_local = *mat;
		}

		void SetDescriptor(const char* str64) {
			_tcscpy(_descriptor, str64);
		}

		void SetAnimCtrlObj(lwIAnimCtrlObjBone* ctrl_obj) {
			_ctrl_obj = ctrl_obj;
		}

		DWORD GetType() const {
			return _type;
		}

		DWORD GetID() const {
			return _id;
		}

		lwMatrix44* GetLocalMatrix() {
			return &_mat_local;
		}

		lwMatrix44* GetWorldMatrix() {
			return &_mat_world;
		}

		char* GetDescriptor() {
			return _descriptor;
		}

		lwINode* GetParent() const {
			return _parent;
		}

		DWORD GetLinkID() const {
			return _link_id;
		}

		DWORD GetParentLinkID() const {
			return _link_parent_id;
		}

		lwIByteSet* GetStateSet() {
			return &_state_set;
		}

		lwIAnimCtrlObjBone* GetAnimCtrlObj() {
			return _ctrl_obj;
		}

		LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id);
		LW_RESULT Update();

		LW_RESULT Render() {
			return LW_RET_OK;
		}

		LW_RESULT Load(lwIAnimDataBone* data);

		LW_RESULT PlayPose(const lwPlayPoseInfo* info);
		lwIPoseCtrl* GetPoseCtrl();
		lwPlayPoseInfo* GetPlayPoseInfo();
	};

	class lwNodeDummy : public lwNodeBase, public lwINodeDummy {
		LW_STD_DECLARATION()

	private:
		lwIResourceMgr* _res_mgr;
		lwIAnimCtrlObjMat* _ctrl_obj;
		lwINodePrimitive* _ass_obj;

	public:
		lwNodeDummy(lwIResourceMgr* res_mgr);
		~lwNodeDummy();

		// base method
		void SetID(DWORD id) {
			_id = id;
		}

		void SetLinkID(DWORD id) {
			_link_id = id;
		}

		void SetParentLinkID(DWORD id) {
			_link_parent_id = id;
		}

		LW_RESULT SetParent(lwINode* parent) {
			_parent = parent;
			return LW_RET_OK;
		}

		void SetLocalMatrix(const lwMatrix44* mat) {
			_mat_local = *mat;
		}

		void SetDescriptor(const char* str64) {
			_tcscpy(_descriptor, str64);
		}

		void SetAnimCtrlObj(lwIAnimCtrlObjMat* ctrl_obj) {
			_ctrl_obj = ctrl_obj;
		}

		DWORD GetType() const {
			return _type;
		}

		DWORD GetID() const {
			return _id;
		}

		lwMatrix44* GetLocalMatrix() {
			return &_mat_local;
		}

		lwMatrix44* GetWorldMatrix() {
			return &_mat_world;
		}

		char* GetDescriptor() {
			return _descriptor;
		}

		lwINode* GetParent() const {
			return _parent;
		}

		DWORD GetLinkID() const {
			return _link_id;
		}

		DWORD GetParentLinkID() const {
			return _link_parent_id;
		}

		lwIByteSet* GetStateSet() {
			return &_state_set;
		}

		LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id);

		lwIAnimCtrlObjMat* GetAnimCtrlObj() {
			return _ctrl_obj;
		}

		LW_RESULT Update();
		LW_RESULT Render();

		LW_RESULT Load(lwIHelperDummyObjInfo* data);
		LW_RESULT CreateAssistantObject(const lwVector3* size, DWORD color);

		lwINodePrimitive* GetAssistantObject() {
			return _ass_obj;
		}
	};

	class lwNodeHelper : public lwNodeBase, public lwINodeHelper {
		LW_STD_DECLARATION()

	private:
		lwIResourceMgr* _res_mgr;

		lwIHelperDummy* _obj_dummy;
		lwIHelperBox* _obj_box;
		lwIHelperMesh* _obj_mesh;

	public:
		lwNodeHelper(lwIResourceMgr* mgr);
		~lwNodeHelper();

		// base method
		void SetID(DWORD id) {
			_id = id;
		}

		void SetLinkID(DWORD id) {
			_link_id = id;
		}

		void SetParentLinkID(DWORD id) {
			_link_parent_id = id;
		}

		LW_RESULT SetParent(lwINode* parent) {
			_parent = parent;
			return LW_RET_OK;
		}

		void SetLocalMatrix(const lwMatrix44* mat) {
			_mat_local = *mat;
		}

		void SetDescriptor(const char* str64) {
			_tcscpy(_descriptor, str64);
		}

		DWORD GetType() const {
			return _type;
		}

		DWORD GetID() const {
			return _id;
		}

		lwMatrix44* GetLocalMatrix() {
			return &_mat_local;
		}

		lwMatrix44* GetWorldMatrix() {
			return &_mat_world;
		}

		char* GetDescriptor() {
			return _descriptor;
		}

		lwINode* GetParent() const {
			return _parent;
		}

		DWORD GetLinkID() const {
			return _link_id;
		}

		DWORD GetParentLinkID() const {
			return _link_parent_id;
		}

		lwIByteSet* GetStateSet() {
			return &_state_set;
		}

		LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id);
		LW_RESULT Update();
		LW_RESULT Render();

		// helper object method
		LW_RESULT LoadHelperInfo(const lwHelperInfo* info, int create_instance_flag);
		LW_RESULT Copy(const lwINodeHelper* src);
		LW_RESULT Destroy();


		lwIHelperDummy* GetHelperDummy() {
			return _obj_dummy;
		}

		lwIHelperBox* GetHelperBox() {
			return _obj_box;
		}

		lwIHelperMesh* GetHelperMesh() {
			return _obj_mesh;
		}
	};


	class lwNodeObject : public lwINodeObject {
		LW_STD_DECLARATION()

	private:
		lwIResourceMgr* _res_mgr;
		lwITreeNode* _obj_root;

		char _name[LW_MAX_NAME];

	public:
		lwNodeObject(lwIResourceMgr* res_mgr);
		virtual ~lwNodeObject();


		LW_RESULT Update();
		LW_RESULT Render();
		LW_RESULT IgnoreNodesRender(const IgnoreStruct* is);
		LW_RESULT Destroy();
		LW_RESULT CullPrimitive();

		void SetName(const char* name) {
			_tcscpy(_name, name);
		}

		void SetMatrix(const lwMatrix44* mat);

		char* GetName() {
			return _name;
		}

		lwMatrix44* GetMatrix();
		lwIByteSet* GetStateSet();

		lwITreeNode* GetTreeNodeRoot() {
			return _obj_root;
		}

		LW_RESULT QueryTreeNode(lwModelNodeQueryInfo* info);
		LW_RESULT InsertTreeNode(lwITreeNode* parent_node, lwITreeNode* prev_node, lwITreeNode* node);
		LW_RESULT RemoveTreeNode(lwITreeNode* node);
		LW_RESULT Load(const char* file, DWORD flag, lwITreeNode* parent_node);
	};

	// assistant method
	LW_RESULT lwDestroyNodeObject(lwITreeNode* node);

	LW_RESULT lwNodePrimitive_PlayPose(lwINodePrimitive* obj, const lwPlayPoseInfo* info, DWORD ctrl_type, DWORD subset,
									   DWORD stage);
	LW_RESULT lwNodePrimitive_PlayPoseAll(lwINodePrimitive* obj, const lwPlayPoseInfo* info);
	LW_RESULT lwNodeBoneCtrl_PlayPose(lwINodeBoneCtrl* obj, const lwPlayPoseInfo* info);
	LW_RESULT lwNodeDummy_PlayPose(lwINodeDummy* obj, const lwPlayPoseInfo* info);
	LW_RESULT lwNodeObject_PlayDefaultPose(lwINodeObject* obj);
	LW_RESULT lwNode_ShowBoundingObject(lwINode* obj, DWORD flag);
	LW_RESULT lwNodeObject_ShowBoundingObject(lwINodeObject* obj, DWORD flag);
	LW_RESULT lwNodeObject_DumpObjectTree(lwINodeObject* obj, const char* file);
	LW_RESULT lwNodeObject_GetPrimitiveCullingNum(lwINodeObject* obj, DWORD* num);

LW_END
