//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwMath.h"
#include "lwExpObj.h"
#include "lwITypes.h"
#include "lwInterfaceExt.h"

//////////////
//
LW_BEGIN

class lwModelNode
{
protected:
    // base
    DWORD _type;
    DWORD _id;
    char _descriptor[64];
    lwMatrix44 _mat_local;
    lwMatrix44 _mat_world;
    lwStateCtrl _state_ctrl;

    // link state
    lwIModelNode* _parent;
    DWORD _link_parent_id;
    DWORD _link_id;

protected:
    //LW_RESULT _UpdateMatrix();
public:
    lwModelNode();
    //~lwModelNode();
};

class lwPrimitiveObj : public lwModelNode, public lwIPrimitiveObj
{
    LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwIMeshAgent* _mesh_agent;
    lwIMtlTexAgent** _mtltex_agent_seq;
    lwIAnimCtrlAgent* _anim_agent;
    lwIRenderCtrlAgent* _render_agent;
    lwIHelperObject* _helper_object;
    //lwIVertexBlendCtrl* _vertex_blend_ctrl;
    lwIAnimCtrlObjBone* _ref_ctrl_obj_bone;

    DWORD _mtltex_agent_seqsize;

private:
    LW_RESULT _DestroyMtlTexAgent();
    LW_RESULT _UpdateBoundingObject(lwIAnimCtrlObjBone* ctrl_obj);

public:
    lwPrimitiveObj(lwIResourceMgr* res_mgr);
    virtual ~lwPrimitiveObj();

    // base method
    void SetID(DWORD id) { _id = id; }
    LW_RESULT SetParent(lwIModelNode* parent);
    void SetLocalMatrix(const lwMatrix44* mat) { _mat_local = *mat; }
    void SetDescriptor(const char* str64) { _tcscpy(_descriptor, str64); }
    DWORD GetType() const { return _type; }
    DWORD GetID() const { return _id; }
    lwMatrix44* GetLocalMatrix() { return &_mat_local; }
    lwMatrix44* GetWorldMatrix() { return &_mat_world; }
    char* GetDescriptor() { return _descriptor; }
    lwIModelNode* GetParent() const { return _parent; }
    DWORD GetLinkID() const { return _link_id; }
    DWORD GetLinkParentID() const { return _link_parent_id; }
    void SetStateCtrl(const lwStateCtrl* ctrl) { _state_ctrl = *ctrl; }
    lwStateCtrl* GetStateCtrl() { return &_state_ctrl; }
    LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id);
    
    // primitive method
    void SetMeshAgent(lwIMeshAgent* agent) { _mesh_agent = agent; }
    void SetMtlTexAgent(DWORD subset, lwIMtlTexAgent* agent) { _mtltex_agent_seq[subset] = agent; }
    void SetAnimCtrlAgent(lwIAnimCtrlAgent* agent) { _anim_agent = agent; }
    void SetRenderCtrl(lwIRenderCtrlAgent* obj) { _render_agent = obj; }
    void SetHelperObject(lwIHelperObject* obj) { _helper_object = obj; }
    void SetMaterial(const lwMaterial* mtl);
    void SetOpacity(float opacity);
    //LW_RESULT SetVertexBlendCtrl(lwIVertexBlendCtrl* ctrl);

    lwIResourceMgr* GetResourceMgr() { return _res_mgr; }
    lwIMtlTexAgent* GetMtlTexAgent(DWORD id) { return _mtltex_agent_seq[id]; }
    lwIMeshAgent* GetMeshAgent() { return _mesh_agent; }
    lwIAnimCtrlAgent* GetAnimCtrlAgent() { return _anim_agent; }
    lwIRenderCtrlAgent* GetRenderCtrlAgent() { return _render_agent; }
    lwIHelperObject* GetHelperObject() { return _helper_object; }
    //lwIVertexBlendCtrl* GetVertexBlendCtrl() { return _vertex_blend_ctrl; }
    LW_RESULT AllocateMtlTexAgentSeq(DWORD num);
    DWORD GetMtlTexAgentSeqSize() const { return _mtltex_agent_seqsize; }


    LW_RESULT Destroy();
    LW_RESULT Load(lwIGeomObjInfo* info, const char* tex_path, const lwResFile* res);
    LW_RESULT LoadMesh(lwMeshInfo* info);
    LW_RESULT LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, const char* tex_path);
    LW_RESULT LoadAnimData(lwIAnimDataInfo* info, const char* tex_path, const lwResFile* res);
    LW_RESULT LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci);
    LW_RESULT Copy(lwIPrimitiveObj* src_obj);

    LW_RESULT Update();
    LW_RESULT Render();
    LW_RESULT RenderSubset(DWORD subset);
    LW_RESULT RenderHelperObject();


    LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);

};

class lwVertexBlendCtrl : public lwModelNode, public lwIVertexBlendCtrl
{
    LW_STD_DECLARATION()
private:
    lwIResourceMgr* _res_mgr;
    lwIAnimCtrlObjBone* _ctrl_obj;

public:
    lwVertexBlendCtrl(lwIResourceMgr* res_mgr);
    virtual ~lwVertexBlendCtrl();

    // base method
    void SetID(DWORD id) { _id = id; }
    LW_RESULT SetParent(lwIModelNode* parent) { _parent = parent; return LW_RET_OK; }
    void SetLocalMatrix(const lwMatrix44* mat) { _mat_local = *mat; }
    void SetDescriptor(const char* str64) { _tcscpy(_descriptor, str64); }
    DWORD GetType() const { return _type; }
    DWORD GetID() const { return _id; }
    lwMatrix44* GetLocalMatrix() { return &_mat_local; }
    lwMatrix44* GetWorldMatrix() { return &_mat_world; }
    char* GetDescriptor() { return _descriptor; }
    lwIModelNode* GetParent() const { return _parent; }
    DWORD GetLinkID() const { return _link_id; }
    DWORD GetLinkParentID() const { return _link_parent_id; }
    void SetStateCtrl(const lwStateCtrl* ctrl) { _state_ctrl = *ctrl; }
    lwStateCtrl* GetStateCtrl() { return &_state_ctrl; }
    LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id);
    LW_RESULT Update();

    LW_RESULT Load(lwIAnimDataBone* data);
    void SetAnimCtrlObjBone(lwIAnimCtrlObjBone* ctrl_obj) { _ctrl_obj = ctrl_obj; }
    lwIAnimCtrlObjBone* GetAnimCtrlObj() { return _ctrl_obj; }
    
    LW_RESULT PlayPose(const lwPlayPoseInfo* info);
    lwIPoseCtrl* GetPoseCtrl();
    lwPlayPoseInfo* GetPlayPoseInfo();

};

class lwNodeDummy : public lwModelNode, public lwINodeDummy
{
    LW_STD_DECLARATION()
private:
    lwIResourceMgr* _res_mgr;

public:
    lwNodeDummy(lwIResourceMgr* res_mgr);

    // base method
    void SetID(DWORD id) { _id = id; }
    LW_RESULT SetParent(lwIModelNode* parent) { _parent = parent; return LW_RET_OK; }
    void SetLocalMatrix(const lwMatrix44* mat) { _mat_local = *mat; }
    void SetDescriptor(const char* str64) { _tcscpy(_descriptor, str64); }
    DWORD GetType() const { return _type; }
    DWORD GetID() const { return _id; }
    lwMatrix44* GetLocalMatrix() { return &_mat_local; }
    lwMatrix44* GetWorldMatrix() { return &_mat_world; }
    char* GetDescriptor() { return _descriptor; }
    lwIModelNode* GetParent() const { return _parent; }
    DWORD GetLinkID() const { return _link_id; }
    DWORD GetLinkParentID() const { return _link_parent_id; }
    void SetStateCtrl(const lwStateCtrl* ctrl) { _state_ctrl = *ctrl; }
    lwStateCtrl* GetStateCtrl() { return &_state_ctrl; }
    LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id);
    LW_RESULT Update();


};

class lwHelperObj : public lwModelNode, public lwIHelperObj
{
LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;

    lwIHelperDummy* _obj_dummy;
    lwIHelperBox* _obj_box;
    lwIHelperMesh* _obj_mesh;

public:
    lwHelperObj(lwIResourceMgr* mgr);
    ~lwHelperObj();

    // base method
    void SetID(DWORD id) { _id = id; }
    LW_RESULT SetParent(lwIModelNode* parent) { _parent = parent; return LW_RET_OK; }
    void SetLocalMatrix(const lwMatrix44* mat) { _mat_local = *mat; }
    void SetDescriptor(const char* str64) { _tcscpy(_descriptor, str64); }
    DWORD GetType() const { return _type; }
    DWORD GetID() const { return _id; }
    lwMatrix44* GetLocalMatrix() { return &_mat_local; }
    lwMatrix44* GetWorldMatrix() { return &_mat_world; }
    char* GetDescriptor() { return _descriptor; }
    lwIModelNode* GetParent() const { return _parent; }
    DWORD GetLinkID() const { return _link_id; }
    DWORD GetLinkParentID() const { return _link_parent_id; }
    void SetStateCtrl(const lwStateCtrl* ctrl) { _state_ctrl = *ctrl; }
    lwStateCtrl* GetStateCtrl() { return &_state_ctrl; }
    LW_RESULT GetLinkMatrix(lwMatrix44* mat, DWORD link_id);
    LW_RESULT Update();

    // helper object method
    LW_RESULT LoadHelperInfo(const lwHelperInfo* info, int create_instance_flag);
    LW_RESULT Copy(const lwIHelperObj* src);
    LW_RESULT Destroy();

    LW_RESULT Render();

    lwIHelperDummy* GetHelperDummy() { return _obj_dummy; }
    lwIHelperBox* GetHelperBox() { return _obj_box; }
    lwIHelperMesh* GetHelperMesh() { return _obj_mesh; }

};


class lwModelObject : public lwIModelObject
{
    LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwITreeNode* _obj_root;

    char _name[LW_MAX_NAME];

public:
    lwModelObject(lwIResourceMgr* res_mgr);
    virtual ~lwModelObject();


    LW_RESULT Update();
    LW_RESULT Render();
    LW_RESULT Destroy();
    void SetName(const char* name) { _tcscpy(_name, name); }
    void SetMatrix(const lwMatrix44* mat);
    void SetStateCtrl(lwStateCtrl* ctrl);
    char* GetName() { return _name; }
    lwMatrix44* GetMatrix();
    lwStateCtrl* GetStateCtrl();
    lwITreeNode* GetTreeNodeRoot() { return _obj_root; }
    LW_RESULT QueryTreeNode(lwModelNodeQueryInfo* info);
    LW_RESULT InsertTreeNode(lwITreeNode* parent_node, lwITreeNode* prev_node, lwITreeNode* node);
    LW_RESULT RemoveTreeNode(lwITreeNode* node);
    LW_RESULT Load(const char* file, DWORD flag);

};

// assistant method
LW_RESULT lwPrimitiveObj_PlayPose(lwIPrimitiveObj* obj, const lwPlayPoseInfo* info, DWORD ctrl_type, DWORD subset, DWORD stage);
LW_RESULT lwVertexBlendCtrl_PlayPose(lwIVertexBlendCtrl* obj, const lwPlayPoseInfo* info);
LW_RESULT lwPrimitiveObj_PlayPoseAll(lwIPrimitiveObj* obj, const lwPlayPoseInfo* info);
LW_RESULT lwModelObject_PlayDefaultPose(lwIModelObject* obj);

LW_END
