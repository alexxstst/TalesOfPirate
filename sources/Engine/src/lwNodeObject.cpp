//
#include "stdafx.h"

#if 1

#include "lwNodeObject.h"
#include "lwMisc.h"
#include "lwIUtil.h"
#include "lwPrimitiveHelper.h"
#include "lwHelperGeometry.h"

LW_BEGIN

// lwNodeBase
lwNodeBase::lwNodeBase()
{
    // base
    _type = MODELNODE_INVALID;
    _id = LW_INVALID_INDEX;
    _descriptor[0] = '\0';
    lwMatrix44Identity(&_mat_local);
    lwMatrix44Identity(&_mat_world);

    // state set
    _state_set.Alloc(OBJECT_STATE_NUM);
    _state_set.SetValue(STATE_VISIBLE, 1);
    _state_set.SetValue(STATE_ENABLE, 1);

    // link state
    _parent = 0;
    _link_parent_id = LW_INVALID_INDEX;
    _link_id = LW_INVALID_INDEX;

}

//LW_RESULT lwNodeBase::_UpdateMatrix()
//{
//    LW_RESULT ret = LW_RET_FAILED;
//
//    if(_parent)
//    {
//        // update world matrix
//        lwMatrix44 mat_parent;
//
//        if(LW_FAILED(_parent->GetLinkMatrix(&mat_parent, _link_parent_id)))
//            goto __ret;
//
//        if(_link_id != LW_INVALID_INDEX)
//        {
//            lwMatrix44 mat_link;
//
//            if(LW_FAILED(GetLinkMatrix(&mat_link, _link_id)))
//                goto __ret;
//
//            lwMatrix44InverseNoScaleFactor(&mat_link, &mat_link);
//
//            lwMatrix44Multiply(&mat_parent, &mat_link, &mat_parent);
//        }
//
//        lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);
//    }
//    else
//    {
//        _mat_world = _mat_local;
//    }
//
//    ret = LW_RET_OK;
//__ret:
//    return ret;
//}
//

static DWORD __tree_proc_loadmodel(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwIResourceMgr* res_mgr = (lwIResourceMgr*)param;
    lwModelNodeInfo* node_info = (lwModelNodeInfo*)node->GetData();

    char* tex_path = res_mgr->GetTexturePath();

    lwINode* model_node = 0;
    if(LW_FAILED(res_mgr->CreateNode(&model_node, node_info->_type)))
        goto __ret;

    model_node->SetID(node_info->_id);
    // for the reason of compatible,
    // link_id and link_parent_id cannot be asigned here
    // because the initialize value of them is zero 
    // which is valid for link_id and link_parent_id

    // now fixed it @_@
    model_node->SetLinkID(node_info->_link_id);
    model_node->SetParentLinkID(node_info->_link_parent_id);

    switch(node_info->_type)
    {
    case NODE_PRIMITIVE:
        if(LW_FAILED(((lwINodePrimitive*)model_node)->Load((lwIGeomObjInfo*)node_info->_data, tex_path, 0)))
            goto __ret;
        break;
    case NODE_BONECTRL:
        if(LW_FAILED(((lwINodeBoneCtrl*)model_node)->Load((lwIAnimDataBone*)node_info->_data)))
            goto __ret;
        break;
    case NODE_DUMMY:
        if(LW_FAILED(((lwINodeDummy*)model_node)->Load((lwIHelperDummyObjInfo*)node_info->_data)))
            goto __ret;
        break;
    case NODE_HELPER:
        assert(0);
        break;
    default:
        assert(0);
    }
    {
        lwITreeNode* model_tree;
        lwGUIDCreateObject((LW_VOID**)&model_tree, LW_GUID_TREENODE);
        model_tree->SetData(model_node);

        // set param
        node_info->_param = model_tree;

        // link tree parent
        lwITreeNode* node_parent = node->GetParent();
        if (node_parent)
        {
            lwModelNodeInfo* parent_info = (lwModelNodeInfo*)node_parent->GetData();
            lwITreeNode* parent_obj_node = (lwITreeNode*)parent_info->_param;

            parent_obj_node->InsertChild(0, model_tree);

            lwINode* parent_obj_data = (lwINode*)parent_obj_node->GetData();
            assert(parent_obj_data && "invalid parent_obj_data when called __tree_proc_loadmodel");

            model_node->SetParent(parent_obj_data);
        }
    }

    ret = TREENODE_PROC_RET_CONTINUE;
__ret:
    return ret;
}

LW_RESULT lwLoadModelInfo(lwITreeNode** ret_obj_tree, lwModelInfo* info, lwIResourceMgr* res_mgr)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(info->_obj_tree->EnumTree(__tree_proc_loadmodel, (void*)res_mgr, TREENODE_PROC_PREORDER) 
        == TREENODE_PROC_RET_ABORT)
        goto __ret;
    {
        lwModelNodeInfo* root_info = (lwModelNodeInfo*)info->_obj_tree->GetData();

        *ret_obj_tree = (lwITreeNode*)root_info->_param;
    }
    ret = LW_RET_OK;
__ret:
    return ret;
}
// lwNodePrimitive
LW_STD_IMPLEMENTATION(lwNodePrimitive)

lwNodePrimitive::lwNodePrimitive(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr)
{
    _type = NODE_PRIMITIVE;

    _mesh_agent = 0;
    _mtltex_agent_seq = 0;
    _anim_agent = 0;
    _render_agent = 0;
    _helper_object = 0;
    //_vertex_blend_ctrl = 0;
    _ref_ctrl_obj_bone = 0;

    _mtltex_agent_seqsize = 0;
}

lwNodePrimitive::~lwNodePrimitive()
{
    Destroy();
}

LW_RESULT lwNodePrimitive::_DestroyMtlTexAgent()
{
    if(_mtltex_agent_seqsize > 0)
    {
        for(DWORD i = 0; i < _mtltex_agent_seqsize; i++)
        {
            LW_IF_RELEASE(_mtltex_agent_seq[i]);
        }
        LW_DELETE_A(_mtltex_agent_seq);
        _mtltex_agent_seqsize = 0;
    }

    return LW_RET_OK;
}

LW_RESULT lwNodePrimitive::_UpdateBoundingObject(lwIAnimCtrlObjBone* ctrl_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_helper_object == 0)
        goto __addr_ret_ok;
    
    lwIBoundingSphere* b;
    lwBoundingSphereInfo* s;

    if((b = _helper_object->GetBoundingSphere()) == 0)
        goto __addr_ret_ok;
    {
        DWORD bs_num = b->GetObjNum();

        for (DWORD j = 0; j < bs_num; j++)
        {
            s = b->GetDataInfo(j);

            assert(s->id < LW_MAX_BONEDUMMY_NUM);

            lwMatrix44* mat = ctrl_obj->GetDummyRTM(s->id);
            if (mat)
            {
                s->mat = *mat;
            }
        }
    }

__addr_ret_ok:
    ret = LW_RET_OK;

    return ret;
}

LW_RESULT lwNodePrimitive::_UpdateTransparentState()
{
    // i recommend this procedure to be invoked when initialize and something changing
    // of transparent state such as opacity, additive/subtractive blend state.
    // consequently it is not advised to update per-frame
    DWORD i;
    BYTE state;
    lwIMtlTexAgent* mtltex_agent;

    if(_state_set.GetValue(STATE_UPDATETRANSPSTATE) == 0)
        goto __ret;

    state = 1;

    for(i = 0; i < _mtltex_agent_seqsize; i++)
    {
        if((mtltex_agent = _mtltex_agent_seq[i]) == NULL)
            break;

        if(mtltex_agent->GetOpacity() != 1.0f)
            goto __set_state;

        if(mtltex_agent->GetTransparencyType() != MTLTEX_TRANSP_FILTER)
            goto __set_state;
    }

    state = 0;

__set_state:
    _state_set.SetValue(STATE_TRANSPARENT, state);
__ret:
    return LW_RET_OK;
}

LW_RESULT lwNodePrimitive::Destroy()
{
    LW_SAFE_RELEASE(_mesh_agent);
    LW_SAFE_RELEASE(_anim_agent);
    LW_SAFE_RELEASE(_render_agent);
    LW_SAFE_RELEASE(_helper_object);

    _DestroyMtlTexAgent();

    return LW_RET_OK;
}

LW_RESULT lwNodePrimitive::AllocateMtlTexAgentSeq(DWORD num)
{
    _DestroyMtlTexAgent();

    _mtltex_agent_seq = LW_NEW(lwIMtlTexAgent*[num]);
    _mtltex_agent_seqsize = num;
    memset(_mtltex_agent_seq, 0, sizeof(lwIMtlTexAgent*) * num);

    return LW_RET_OK;
}

LW_RESULT lwNodePrimitive::Load(lwIGeomObjInfo* geom_info, const char* tex_path, const lwResFile* res)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD i;

    lwGeomObjInfo* info = (lwGeomObjInfo*)geom_info;

    lwISystem* sys = _res_mgr->GetSysGraphics()->GetSystem();
    lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
    BYTE create_helper_primitive = opt_mgr->GetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE);

    LoadRenderCtrl(&info->rcci);

    // base info
    // warning:_typeMODELNODE_
    // type
    //_type = info->type;

    // _id, _link_id, _link_parent_idlwLoadModelInfo
    //_id = info->id;
    _mat_local = info->mat_local;
    //_state_ctrl = info->state_ctrl;
    _state_set.SetValueSeq(0, info->state_ctrl._state_seq, OBJECT_STATE_NUM);
    //_parent;
    //

    // mtltex info
    if(LW_FAILED(AllocateMtlTexAgentSeq(info->mtl_num)))
        goto __ret;

    for(i = 0; i < info->mtl_num; i++)
    {
        if(LW_FAILED(LoadMtlTex(i, &info->mtl_seq[i], tex_path)))
            goto __ret;
    }

    // mesh info
    if(LW_FAILED(LoadMesh(&info->mesh)))
        goto __ret;

    if(res)
    {
        lwResFileMesh info;
        info.obj_id = res->obj_id;
        info.res_type = res->res_type;
        _tcscpy(info.file_name, res->file_name);

        _mesh_agent->GetMesh()->SetResFile(&info);
    }

    // helper info
    if(info->helper_data.type != HELPER_TYPE_INVALID)
    {
        LW_IF_RELEASE(_helper_object);
        _res_mgr->CreateHelperObject(&_helper_object);
        if(LW_FAILED(_helper_object->LoadHelperInfo(&info->helper_data, create_helper_primitive)))
        {
            LG_MSGBOX("LoadHelperInfo");
        }
    }

    // anim info
    if(info->anim_size > 0)
    {
        if(LW_FAILED(LoadAnimData(&info->anim_data, tex_path, res)))
            goto __ret;
    }


    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwNodePrimitive::LoadMesh(lwMeshInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    LW_IF_RELEASE(_mesh_agent);

    if(LW_FAILED(_res_mgr->CreateMeshAgent(&_mesh_agent)))
        goto __ret;

    if(LW_FAILED(_mesh_agent->LoadMesh(info)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodePrimitive::LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, const char* tex_path)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(mtl_id < 0 || mtl_id >= LW_MAX_SUBSET_NUM)
        goto __ret;

    if(LW_FAILED(_res_mgr->CreateMtlTexAgent(&_mtltex_agent_seq[mtl_id])))
        goto __ret;

    if(LW_FAILED(_mtltex_agent_seq[mtl_id]->LoadMtlTex(info, tex_path)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwNodePrimitive::LoadAnimData(lwIAnimDataInfo* data_info, const char* tex_path, const lwResFile* res)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimDataInfo* info = (lwAnimDataInfo*)data_info;

    void* data;
    LW_SAFE_RELEASE(_anim_agent);

    if(LW_FAILED(_res_mgr->CreateAnimCtrlAgent(&_anim_agent)))
        goto __ret;

    lwAnimCtrlObjTypeInfo type_info;

    if(info->anim_mat)
    {
        lwIAnimCtrlMatrix* ctrl = NULL;
        lwIAnimCtrlObjMat* ctrl_obj = NULL;

        data = info->anim_mat;

        type_info.type = ANIM_CTRL_TYPE_MAT;
        type_info.data[0] = LW_INVALID_INDEX;
        type_info.data[1] = LW_INVALID_INDEX;

        if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type)))
            goto __ret;

        if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type)))
            goto __ret;

        if(LW_FAILED(ctrl->LoadData(data)))
            goto __ret;

        ctrl_obj->AttachAnimCtrl(ctrl);
        ctrl_obj->SetTypeInfo(&type_info);

        _anim_agent->AddAnimCtrlObj(ctrl_obj);
        
    }
    if(info->anim_bone)
    {
        lwIAnimCtrlBone* ctrl = NULL;
        lwIAnimCtrlObjBone* ctrl_obj = NULL;

        data = info->anim_bone;

        type_info.type = ANIM_CTRL_TYPE_BONE;
        type_info.data[0] = LW_INVALID_INDEX;
        type_info.data[1] = LW_INVALID_INDEX;

        if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type)))
            goto __ret;

        if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type)))
            goto __ret;

        if(LW_FAILED(ctrl->LoadData(data)))
            goto __ret;

        ctrl_obj->AttachAnimCtrl(ctrl);
        ctrl_obj->SetTypeInfo(&type_info);

        _anim_agent->AddAnimCtrlObj(ctrl_obj);
    }


    for(DWORD i = 0; i < _mtltex_agent_seqsize; i++)
    {
        if(_mtltex_agent_seq[i] == NULL)
            continue;

        if(info->anim_mtlopac[i])
        {
            data = info->anim_mtlopac[i];

            type_info.type = ANIM_CTRL_TYPE_MTLOPACITY;
            type_info.data[0] = i;
            type_info.data[1] = LW_INVALID_INDEX;


            lwIAnimCtrlMtlOpacity* ctrl = NULL;
            lwIAnimCtrlObjMtlOpacity* ctrl_obj = NULL;

            if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type)))
                goto __ret;

            if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type)))
                goto __ret;

            if(LW_FAILED(ctrl->LoadData(data)))
                goto __ret;

            ctrl_obj->AttachAnimCtrl(ctrl);
            ctrl_obj->SetTypeInfo(&type_info);
            _anim_agent->AddAnimCtrlObj(ctrl_obj); 
        }

        for(DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++)
        {
            if(info->anim_tex[i][j])
            {
                lwIAnimCtrlTexUV* ctrl = NULL;
                lwIAnimCtrlObjTexUV* ctrl_obj = NULL;

                data = info->anim_tex[i][j];

                type_info.type = ANIM_CTRL_TYPE_TEXUV;
                type_info.data[0] = i;
                type_info.data[1] = j;

                if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type)))
                    goto __ret;

                if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type)))
                    goto __ret;

                if(LW_FAILED(ctrl->LoadData(data)))
                    goto __ret;

                ctrl_obj->AttachAnimCtrl(ctrl);
                ctrl_obj->SetTypeInfo(&type_info);

                _anim_agent->AddAnimCtrlObj(ctrl_obj);
            }

            //continue;
            // image
            if(info->anim_img[i][j])
            {
                lwIAnimCtrlTexImg* ctrl = NULL;
                lwIAnimCtrlObjTexImg* ctrl_obj = NULL;

                data = info->anim_img[i][j];

                type_info.type = ANIM_CTRL_TYPE_TEXIMG;
                type_info.data[0] = i;
                type_info.data[1] = j;

                _tcscpy(info->anim_img[i][j]->_tex_path, tex_path);

                if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type)))
                    goto __ret;

                if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type)))
                    goto __ret;

                if(LW_FAILED(ctrl->LoadData(data)))
                    goto __ret;

                ctrl_obj->AttachAnimCtrl(ctrl);
                ctrl_obj->SetTypeInfo(&type_info);

                _anim_agent->AddAnimCtrlObj(ctrl_obj);
            }

        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodePrimitive::LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIRenderCtrlAgent* render_agent = 0;

    if(_render_agent)
        goto __ret;

    if(LW_FAILED(_res_mgr->CreateRenderCtrlAgent(&render_agent)))
        goto __ret;

    if(LW_FAILED(render_agent->SetRenderCtrl(rcci->ctrl_id)))
        goto __ret;

    render_agent->SetVertexDeclaration(rcci->decl_id);
    render_agent->SetVertexShader(rcci->vs_id);
    
    _render_agent = render_agent;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodePrimitive::Copy(lwINodePrimitive* src_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    _id = src_obj->GetID();
    _mat_local = *src_obj->GetLocalMatrix();
    _mat_world = *src_obj->GetWorldMatrix();

    _parent = src_obj->GetParent();
    _link_id = src_obj->GetLinkID();
    _link_parent_id = src_obj->GetParentLinkID();
    
    lwIMeshAgent* src_mesh_agent = src_obj->GetMeshAgent();
    if(src_mesh_agent)
    {
        if(LW_FAILED(src_mesh_agent->Clone(&_mesh_agent)))
            goto __ret;
    }
    {
        lwIMtlTexAgent* src_mtltex_agent;
        DWORD src_mtltex_agent_seqsize = GetMtlTexAgentSeqSize();

        AllocateMtlTexAgentSeq(src_mtltex_agent_seqsize);

        for (DWORD i = 0; i < src_mtltex_agent_seqsize; i++)
        {
            if ((src_mtltex_agent = src_obj->GetMtlTexAgent(i)) == NULL)
                continue;

            if (LW_FAILED(src_mtltex_agent->Clone(&_mtltex_agent_seq[i])))
                goto __ret;
        }

        lwIAnimCtrlAgent* src_anim_agent = src_obj->GetAnimCtrlAgent();
        if (src_anim_agent)
        {
            if (LW_FAILED(src_anim_agent->Clone(&_anim_agent)))
                goto __ret;
        }

        lwIRenderCtrlAgent* src_render_agent = src_obj->GetRenderCtrlAgent();
        if (src_render_agent)
        {
            if (LW_FAILED(_render_agent->Clone(&_render_agent)))
                goto __ret;
        }

        lwIHelperObject* src_helper_object = src_obj->GetHelperObject();
        if (src_helper_object)
        {
            if (LW_FAILED(src_helper_object->Clone(&_helper_object)))
                goto __ret;
        }

        ret = LW_RET_OK;
    }
__ret:
    return ret;

}

LW_RESULT lwNodePrimitive::Update()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_anim_agent)
    {
        if(LW_FAILED(_anim_agent->Update()))
            goto __ret;

        lwAnimCtrlObjTypeInfo type_info;
        lwIAnimCtrlObj* ctrl_obj;
        DWORD anim_ctrl_num = _anim_agent->GetAnimCtrlObjNum();
        for(DWORD i = 0; i < anim_ctrl_num; i++)
        {
            ctrl_obj = _anim_agent->GetAnimCtrlObj(i);

            if(ctrl_obj->GetAnimCtrl() && !ctrl_obj->IsPlaying())
                continue;

            ctrl_obj->GetTypeInfo(&type_info);
            switch(type_info.type)
            {
            case ANIM_CTRL_TYPE_MAT:
                {
                    if(LW_FAILED(((lwIAnimCtrlObjMat*)ctrl_obj)->UpdateObject()))
                        goto __ret;

                    if(LW_FAILED(((lwIAnimCtrlObjMat*)ctrl_obj)->GetRTM(&_mat_local)))
                        goto __ret;
                }
                break;
            case ANIM_CTRL_TYPE_BONE:
                {
                    if(LW_FAILED(((lwIAnimCtrlObjBone*)ctrl_obj)->UpdateObject((lwIAnimCtrlObjBone*)ctrl_obj, _mesh_agent->GetMesh())))
                        goto __ret;
                }
                break;
            case ANIM_CTRL_TYPE_MTLOPACITY:
                {
                    if(LW_FAILED(((lwIAnimCtrlObjMtlOpacity*)ctrl_obj)->UpdateObject()))
                        goto __ret;

                    lwIMtlTexAgent* mtltex_agent;
                    mtltex_agent = GetMtlTexAgent(type_info.data[0]);
                    if(mtltex_agent == 0)
                        goto __ret;

                    float o = 1.0f;
                    ((lwIAnimCtrlObjMtlOpacity*)ctrl_obj)->GetRunTimeOpacity(&o);
                    mtltex_agent->SetOpacity(o);
                }
                break;
            case ANIM_CTRL_TYPE_TEXUV:
                {
                    if(LW_FAILED(((lwIAnimCtrlObjTexUV*)ctrl_obj)->UpdateObject()))
                        goto __ret;
                }
                break;
            case ANIM_CTRL_TYPE_TEXIMG:
                {
                    if(LW_FAILED(((lwIAnimCtrlObjTexImg*)ctrl_obj)->UpdateObject()))
                        goto __ret;

                    lwITex* tex;
                    lwIMtlTexAgent* mtltex_agent;

                    if(LW_FAILED(((lwIAnimCtrlTexImg*)ctrl_obj)->GetRunTimeTex(&tex)))
                        goto __ret;

                    mtltex_agent = GetMtlTexAgent(type_info.data[0]);
                    if(mtltex_agent == 0)
                        goto __ret;

                    mtltex_agent->SetTextureTransformImage(type_info.data[1], tex);
                }
                break;
            default:
                ;
            }
        }
    }

    /*
    if(_vertex_blend_ctrl)
    {
        // update world matrix
        lwMatrix44 mat_parent;

        if(LW_FAILED(_vertex_blend_ctrl->GetLinkMatrix(&mat_parent, LW_INVALID_INDEX)))
            goto __ret;

        lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);

        // update render ctrl blend matrix
        lwIAnimCtrlObjBone* ctrl_obj = _vertex_blend_ctrl->GetAnimCtrlObj();
        if(ctrl_obj == 0)
            goto __ret;

        ctrl_obj->UpdateObject(_ref_ctrl_obj_bone, this);

        // update bounding object
        // has been done in ctrl_obj->UpdateObject
        //if(LW_FAILED(_UpdateBoundingObject(ctrl_obj)))
        //    goto __ret;
    }
    // update world matrix
    else 
    */
    if(_parent)
    {
        if(_ref_ctrl_obj_bone)
        {
            // update world matrix
            lwMatrix44 mat_parent;

            if(LW_FAILED(_parent->GetLinkMatrix(&mat_parent, LW_INVALID_INDEX)))
                goto __ret;

            lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);

            // update render ctrl blend matrix
            lwIAnimCtrlObjBone* ctrl_obj = ((lwINodeBoneCtrl*)_parent)->GetAnimCtrlObj();
            if(ctrl_obj == 0)
                goto __ret;

            if(LW_FAILED(ctrl_obj->UpdateObject(_ref_ctrl_obj_bone, _mesh_agent->GetMesh())))
                goto __ret;

            if(LW_FAILED(ctrl_obj->UpdateHelperObject(_helper_object)))
                goto __ret;
        }
        else
        {
            lwMatrix44 mat_parent;

            if(LW_FAILED(_parent->GetLinkMatrix(&mat_parent, _link_parent_id)))
                goto __ret;

            if(_link_id != LW_INVALID_INDEX)
            {
                lwMatrix44 mat_link;

                if(LW_FAILED(GetLinkMatrix(&mat_link, _link_id)))
                    goto __ret;

                lwMatrix44InverseNoScaleFactor(&mat_link, &mat_link);

                lwMatrix44Multiply(&mat_parent, &mat_link, &mat_parent);
            }

            lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);
        }
    }
    else
    {
        _mat_world = _mat_local;
    }

    if(_helper_object)
    {
        _helper_object->SetParentMatrix(&_mat_world);
    }

    _UpdateTransparentState();

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodePrimitive::Render()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(!_CheckVisibleState())
        goto __addr_ret_ok;

    _render_agent->SetMatrix(&_mat_world);
    _render_agent->BindAnimCtrlAgent(_anim_agent);
    _render_agent->BindMeshAgent(_mesh_agent);

    if(LW_FAILED(_render_agent->BeginSet()))
        goto __ret;

    for(DWORD i = 0; i < _mtltex_agent_seqsize; i++)
    {
        if(_mtltex_agent_seq[i] == NULL)
            continue;

        _render_agent->BindMtlTexAgent(_mtltex_agent_seq[i]);

        if(_mtltex_agent_seq[i]->BeginPass() == RES_PASS_DEFAULT)
        {
            if(LW_FAILED(_render_agent->BeginSetSubset(i)))
                goto __ret;

            if(LW_FAILED(_render_agent->DrawSubset(i)))
                goto __ret;

            if(LW_FAILED(_render_agent->EndSetSubset(i)))
                goto __ret;
        }

        _mtltex_agent_seq[i]->EndPass();
    }

    if(LW_FAILED(_render_agent->EndSet()))
        goto __ret;

    if(_helper_object)
    {
        _helper_object->Render();
    }

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodePrimitive::RenderSubset(DWORD subset)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(subset < 0 || subset >= LW_MAX_SUBSET_NUM)
        goto __ret;

    if(_mtltex_agent_seq[subset] == NULL)
        goto __ret;

    _render_agent->BindAnimCtrlAgent(_anim_agent);
    _render_agent->BindMeshAgent(_mesh_agent);
    _render_agent->BindMtlTexAgent(_mtltex_agent_seq[subset]);

    if(LW_FAILED(_render_agent->BeginSet()))
        goto __ret;

    if(LW_FAILED(_render_agent->BeginSetSubset(subset)))
        goto __ret;

    if(LW_FAILED(_render_agent->DrawSubset(subset)))
        goto __ret;

    if(LW_FAILED(_render_agent->EndSetSubset(subset)))
        goto __ret;

    if(LW_FAILED(_render_agent->EndSet()))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwNodePrimitive::RenderHelperObject()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_helper_object == 0)
        goto __ret;

    if(LW_FAILED(_helper_object->Render()))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwNodePrimitive::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_helper_object == 0)
        goto __ret;
    
    {
        lwIBoundingSphere* bs = 0;
        lwIBoundingBox* bb = 0;

        lwMatrix44* mat = _render_agent->GetGlobalMatrix();

        if ((bs = _helper_object->GetBoundingSphere()) != 0)
        {
            if (LW_SUCCEEDED(bs->HitTest(info, org, ray, mat)))
                goto __addr_ret_ok;
        }

        if ((bb = _helper_object->GetBoundingBox()) != 0)
        {
            if (LW_SUCCEEDED(bb->HitTest(info, org, ray, mat)))
                goto __addr_ret_ok;
        }
    }
        goto __ret;
__addr_ret_ok:

    ret = LW_RET_OK;

__ret:

    return ret;

}

LW_RESULT lwNodePrimitive::GetLinkMatrix(lwMatrix44* mat, DWORD link_id)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(link_id == LW_INVALID_INDEX)
    {
        *mat = _mat_world;
    }
    else
    {
        if(_helper_object == 0)
            goto __ret;

        lwIHelperDummy* hd = _helper_object->GetHelperDummy();
        if(hd == 0)
            goto __ret;

        lwHelperDummyInfo* info = hd->GetDataInfoWithID(link_id);

        if(info == 0)
            goto __ret;

        *mat = info->mat;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

void lwNodePrimitive::SetMaterial(const lwMaterial* mtl)
{
    for(DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++)
    {
        if(_mtltex_agent_seq[i] == NULL)
            continue;

        _mtltex_agent_seq[i]->SetMaterial(mtl);
    }
}
void lwNodePrimitive::SetOpacity(float opacity)
{
    for(DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++)
    {
        if(_mtltex_agent_seq[i] == NULL)
            continue;

        _mtltex_agent_seq[i]->SetOpacity(opacity);
    }

    //_state_ctrl.SetState(STATE_TRANSPARENT, opacity == 1.0f ? 0 : 1);
}

// SetParentparentVertexBlendCtrl
// 1.lwAnimCtrlAgentANIM_CTRL_TYPE_BONE
// 2._ref_ctrl_obj_bone
// 3.lwRenderCtrlAgentvs_typevs_ctrlvs_type
//   fvfvs_typevs_ctrl
//   
//   vs_type,vs_ctrl
LW_RESULT lwNodePrimitive::SetParent(lwINode* parent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_MAT;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    //check parent model type
    if(parent == 0)
    {
        if(_parent == 0)
            goto __addr_ret_ok;

        if(_parent->GetType() == NODE_BONECTRL)
        {
            if(_anim_agent)
            {
                lwIAnimCtrlObj* ctrl_obj = _anim_agent->RemoveAnimCtrlObj(&type_info);

                LW_IF_RELEASE(ctrl_obj);
                _ref_ctrl_obj_bone = 0;
            }
        }

        _parent = 0;
        goto __addr_ret_ok;
    }

    // clear current parent
    if(LW_FAILED(SetParent(0)))
        goto __ret;

    if(parent->GetType() == NODE_BONECTRL)
    {
        lwIAnimCtrlObj* ctrl_obj = 0;

        // create anim ctrl agent
        if(_anim_agent == 0)
        {
            if(LW_FAILED(_res_mgr->CreateAnimCtrlAgent(&_anim_agent)))
                goto __ret;
        }
        else
        {
            ctrl_obj = _anim_agent->GetAnimCtrlObj(&type_info);
        }

        // create ctrl bone 
        if(ctrl_obj == 0)
        {
            if(LW_FAILED(_res_mgr->CreateAnimCtrlObj(&ctrl_obj, ANIM_CTRL_TYPE_BONE)))
                goto __ret;

            _anim_agent->AddAnimCtrlObj(ctrl_obj);

            _ref_ctrl_obj_bone = (lwIAnimCtrlObjBone*)ctrl_obj;
        }                
        
    }

    _parent = parent;


__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwNodePrimitive::GetSubsetNum(DWORD* subset_num)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(subset_num == 0)
        goto __ret;

    // use mesh subset?
    if(_mesh_agent == 0)
        goto __ret;

    {
        lwIMesh* mesh = _mesh_agent->GetMesh();
        if (mesh == 0)
            goto __ret;

        lwMeshInfo* info = mesh->GetMeshInfo();
        if (info == 0)
            goto __ret;

        *subset_num = info->subset_num;

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}

/*
LW_RESULT lwNodePrimitive::SetVertexBlendCtrl(lwINodeBoneCtrl* ctrl)
{
    LW_RESULT ret = LW_RET_FAILED;

    // add
    if(ctrl)
    {
        lwIAnimCtrlObj* ctrl_obj = 0;

        // create anim ctrl agent
        if(_anim_agent == 0)
        {
            if(LW_FAILED(_res_mgr->CreateAnimCtrlAgent(&_anim_agent)))
                goto __ret;
        }
        else
        {
            ctrl_obj = _anim_agent->GetAnimCtrlObj(LW_INVALID_INDEX, LW_INVALID_INDEX, ANIM_CTRL_TYPE_MAT);
        }

        // create ctrl bone 
        if(ctrl_obj == 0)
        {
            if(LW_FAILED(_res_mgr->CreateAnimCtrlObj(&ctrl_obj, ANIM_CTRL_TYPE_BONE)))
                goto __ret;

            _anim_agent->AddAnimCtrlObj(ctrl_obj);

            _ref_ctrl_obj_bone = (lwIAnimCtrlObjBone*)ctrl_obj;
        }                
    }
    // remove
    else
    {
        if(_anim_agent)
        {
            lwIAnimCtrlObj* ctrl_obj = _anim_agent->RemoveAnimCtrlObj(LW_INVALID_INDEX, LW_INVALID_INDEX, ANIM_CTRL_TYPE_MAT);
            LW_IF_RELEASE(ctrl_obj);

            _ref_ctrl_obj_bone = 0;
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
*/
// lwNodeBoneCtrl
LW_STD_IMPLEMENTATION(lwNodeBoneCtrl)

lwNodeBoneCtrl::lwNodeBoneCtrl(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _ctrl_obj(0)
{
    _type = NODE_BONECTRL;
}
lwNodeBoneCtrl::~lwNodeBoneCtrl()
{
    LW_IF_RELEASE(_ctrl_obj);
}
LW_RESULT lwNodeBoneCtrl::GetLinkMatrix(lwMatrix44* mat, DWORD link_id)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_ctrl_obj == 0)
        goto __ret;

    if(link_id == LW_INVALID_INDEX)
    {
        *mat = _mat_world;
        goto __addr_ret_ok;
    }

    {
        lwMatrix44* rtm = _ctrl_obj->GetDummyRTM(link_id);

        if (rtm == NULL)
            goto __ret;

        lwMatrix44Multiply(mat, rtm, &_mat_world);
    }
__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwNodeBoneCtrl::Update()
{
    LW_RESULT ret = LW_RET_FAILED;

    // update world matrix
    if(_parent)
    {
        lwMatrix44 mat_parent;

        if(LW_FAILED(_parent->GetLinkMatrix(&mat_parent, _link_parent_id)))
            goto __ret;

        if(_link_id != LW_INVALID_INDEX)
        {
            lwMatrix44 mat_link;

            if(LW_FAILED(GetLinkMatrix(&mat_link, _link_id)))
                goto __ret;

            lwMatrix44InverseNoScaleFactor(&mat_link, &mat_link);

            lwMatrix44Multiply(&mat_parent, &mat_link, &mat_parent);
        }

        lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);
    }
    else
    {
        _mat_world = _mat_local;
    }

    if(_ctrl_obj == 0)
        goto __addr_ret_ok;

    if(LW_FAILED(_ctrl_obj->UpdateAnimCtrl()))
        goto __ret;

    if(LW_FAILED(_ctrl_obj->UpdateObject(_ctrl_obj, NULL)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwNodeBoneCtrl::Load(lwIAnimDataBone* data)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrlBone* ctrl_bone = 0;
    lwIAnimCtrlObjBone* ctrl_obj_bone = 0;
    
    if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_bone, ANIM_CTRL_TYPE_BONE)))
        goto __ret;

    if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj_bone, ANIM_CTRL_TYPE_BONE)))
        goto __ret;

    if(LW_FAILED(ctrl_bone->LoadData((void*)data)))
        goto __ret;

    ctrl_obj_bone->AttachAnimCtrl(ctrl_bone);

    _ctrl_obj = ctrl_obj_bone;

    ctrl_obj_bone = 0;
    ctrl_bone = 0;

    ret = LW_RET_OK;
__ret:
    LW_IF_RELEASE(ctrl_bone);
    LW_IF_RELEASE(ctrl_obj_bone);

    return ret;

}

LW_RESULT lwNodeBoneCtrl::PlayPose(const lwPlayPoseInfo* info)
{
    return _ctrl_obj->PlayPose(info);
}
lwIPoseCtrl* lwNodeBoneCtrl::GetPoseCtrl()
{
    return lwIAnimCtrlObj_GetPoseCtrl(_ctrl_obj);
}
lwPlayPoseInfo* lwNodeBoneCtrl::GetPlayPoseInfo()
{
    return lwIAnimCtrlObj_GetPlayPoseInfo(_ctrl_obj);
}


//lwNodeDummy
LW_STD_IMPLEMENTATION(lwNodeDummy)
lwNodeDummy::lwNodeDummy(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _ctrl_obj(0), _ass_obj(0)
{
    _type = NODE_DUMMY;
}
lwNodeDummy::~lwNodeDummy()
{
    LW_IF_RELEASE(_ass_obj);
    LW_IF_RELEASE(_ctrl_obj);
}
LW_RESULT lwNodeDummy::GetLinkMatrix(lwMatrix44* mat, DWORD link_id)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(link_id != LW_INVALID_INDEX)
        goto __ret;

    *mat = _mat_world;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwNodeDummy::Load(lwIHelperDummyObjInfo* data)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwHelperDummyObjInfo* info = (lwHelperDummyObjInfo*)data;

    _id = info->GetID();
    _mat_local = *info->GetMatrix();

    LW_SAFE_RELEASE(_ctrl_obj);
    lwIAnimDataMatrix* anim_data = info->GetAnimDataMatrix();
    if(anim_data)
    {
        lwIAnimCtrlMatrix* ctrl;

        if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, ANIM_CTRL_TYPE_MAT)))
            goto __ret;

        if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&_ctrl_obj, ANIM_CTRL_TYPE_MAT)))
            goto __ret;

        if(LW_FAILED(ctrl->LoadData(anim_data)))
            goto __ret;

        _ctrl_obj->AttachAnimCtrl(ctrl);
    }

    if(_res_mgr->GetByteSet()->GetValue(OPT_CREATE_ASSISTANTOBJECT))
    {
        lwAssObjInfo info;
        _res_mgr->GetAssObjInfo(&info);

        if(LW_FAILED(CreateAssistantObject(&info.size, info.color)))
            goto __ret;

        _ass_obj->GetStateSet()->SetValue(STATE_VISIBLE, 0);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwNodeDummy::Update()
{
    LW_RESULT ret = LW_RET_FAILED;

    // first update anim data
    if(_ctrl_obj)
    {
        if(LW_FAILED(_ctrl_obj->UpdateAnimCtrl()))
            goto __ret;

        if(LW_FAILED(_ctrl_obj->UpdateObject()))
            goto __ret;

        if(LW_FAILED(_ctrl_obj->GetRTM(&_mat_local)))
            goto __ret;
    }

    // update world matrix
    if(_parent)
    {
        lwMatrix44 mat_parent;

        if(LW_FAILED(_parent->GetLinkMatrix(&mat_parent, _link_parent_id)))
            goto __ret;

        if(_link_id != LW_INVALID_INDEX)
        {
            lwMatrix44 mat_link;

            if(LW_FAILED(GetLinkMatrix(&mat_link, _link_id)))
                goto __ret;

            lwMatrix44InverseNoScaleFactor(&mat_link, &mat_link);

            lwMatrix44Multiply(&mat_parent, &mat_link, &mat_parent);
        }

        lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);
    }
    else
    {
        _mat_world = _mat_local;
    }

    if(_ass_obj)
    {
        _ass_obj->Update();
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodeDummy::Render()
{
    if(_ass_obj)
    {
        _ass_obj->Render();
    }

    return LW_RET_OK;
}
LW_RESULT lwNodeDummy::CreateAssistantObject(const lwVector3* size, DWORD color)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwVector3 s = *size / 2;

    lwVector3 axis_x(s.x, 0.0f, 0.0f);
    lwVector3 axis_y(0.0f, s.y, 0.0f);
    lwVector3 axis_z(0.0f, 0.0f, s.z);

    //lwMatrix44 coord_mat;
    //lwICoordinateSys* coord_sys = _res_mgr->GetSysGraphics()->GetCoordinateSys();
    //coord_sys->GetCurCoordSysMatrix(&coord_mat);

    //lwVec3Mat44Mul(&axis_x, &coord_mat);
    //lwVec3Mat44Mul(&axis_y, &coord_mat);
    //lwVec3Mat44Mul(&axis_z, &coord_mat);


    if(LW_FAILED(_res_mgr->CreateNode((lwINode**)&_ass_obj, NODE_PRIMITIVE)))
        goto __ret;

    if(LW_FAILED(_ass_obj->SetParent(this)))
        goto __ret;
    {
        const int v_num = 24 + 6;
        lwVector3 vert_buf[v_num] =
        {
            // axis
            lwVector3(0.0f, 0.0f, 0.0f),
            lwVector3(axis_x),
            lwVector3(0.0f, 0.0f, 0.0f),
            lwVector3(axis_y),
            lwVector3(0.0f, 0.0f, 0.0f),
            lwVector3(axis_z),

            // bottom
            lwVector3(-s.x, -s.y, -s.z),
            lwVector3(-s.x, -s.y,   s.z),

            lwVector3(-s.x, -s.y,   s.z),
            lwVector3(s.x, -s.y,   s.z),

            lwVector3(s.x, -s.y,   s.z),
            lwVector3(s.x, -s.y, -s.z),

            lwVector3(s.x, -s.y, -s.z),
            lwVector3(-s.x, -s.y, -s.z),

            // top
            lwVector3(-s.x,   s.y, -s.z),
            lwVector3(-s.x,   s.y,   s.z),

            lwVector3(-s.x,   s.y,   s.z),
            lwVector3(s.x,   s.y,   s.z),

            lwVector3(s.x,   s.y,   s.z),
            lwVector3(s.x,   s.y, -s.z),

            lwVector3(s.x,   s.y, -s.z),
            lwVector3(-s.x,   s.y, -s.z),

            // side
            lwVector3(-s.x, -s.y, -s.z),
            lwVector3(-s.x,   s.y, -s.z),

            lwVector3(-s.x, -s.y,   s.z),
            lwVector3(-s.x,   s.y,   s.z),

            lwVector3(s.x, -s.y,   s.z),
            lwVector3(s.x,   s.y,   s.z),

            lwVector3(s.x, -s.y, -s.z),
            lwVector3(s.x,   s.y, -s.z),
        };


        DWORD cor_buf[v_num];
        for (DWORD i = 0; i < v_num; i++)
        {
            if (i < 6)
            {
                const DWORD c[3] =
                {
                    0xffff0000,
                    0xff00ff00,
                    0xff0000ff,
                };

                cor_buf[i] = c[i / 2];
            }
            else
            {
                cor_buf[i] = color;
            }
        }

        lwSubsetInfo subset;
        lwSubsetInfo_Construct(&subset, v_num / 2, 0, v_num, 0);

        if (LW_FAILED(lwLoadPrimitiveLineList(_ass_obj, "assistant_object", v_num, vert_buf, cor_buf, &subset, 1)))
        {
            _ass_obj->Release();
            goto __ret;
        }

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}

// lwNodeHelper
LW_STD_IMPLEMENTATION(lwNodeHelper)

lwNodeHelper::lwNodeHelper(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr)
{
    _type = NODE_HELPER;

    _obj_dummy = 0;
    _obj_box = 0;
    _obj_mesh = 0;
}
lwNodeHelper::~lwNodeHelper()
{
    Destroy();
}

LW_RESULT lwNodeHelper::Destroy()
{
    LW_SAFE_RELEASE(_obj_dummy);
    LW_SAFE_RELEASE(_obj_box);
    LW_SAFE_RELEASE(_obj_mesh);

    return LW_RET_OK;
}

LW_RESULT lwNodeHelper::LoadHelperInfo(const lwHelperInfo* info, int create_instance_flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(info->type & HELPER_TYPE_DUMMY)
    {
        lwHelperDummy* d = LW_NEW(lwHelperDummy);

        d->SetResourceMgr(_res_mgr);
        d->SetDataInfo(&info->dummy_seq[0], info->dummy_num);

        if(create_instance_flag)
        {
            if(LW_FAILED(d->CreateInstance()))
                goto __ret;
        }

        LW_IF_RELEASE(_obj_dummy);
        _obj_dummy = d;

    }

    if(info->type & HELPER_TYPE_BOX)
    {

        lwHelperBox* b = LW_NEW(lwHelperBox);

        b->SetResourceMgr(_res_mgr);
        b->SetDataInfo(&info->box_seq[0], info->box_num);

        if(create_instance_flag)
        {
            if(LW_FAILED(b->CreateInstance()))
                goto __ret;
        }

        LW_IF_RELEASE(_obj_box);
        _obj_box = b;
    }

    if(info->type & HELPER_TYPE_MESH)
    {

        lwHelperMesh* m = LW_NEW(lwHelperMesh);

        m->SetResourceMgr(_res_mgr);
        m->SetDataInfo(&info->mesh_seq[0], info->mesh_num);

        if(create_instance_flag)
        {
            if(LW_FAILED(m->CreateInstance()))
                goto __ret;
        }

        LW_IF_RELEASE(_obj_mesh);
        _obj_mesh = m;
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodeHelper::Copy(const lwINodeHelper* src)
{
    lwNodeHelper* s = (lwNodeHelper*)src;

    if(s->_obj_dummy)
    {
        lwHelperDummy* d = LW_NEW(lwHelperDummy);
        d->Clone((lwHelperDummy*)s->_obj_dummy);
        _obj_dummy = d;
    }
    if(s->_obj_box)
    {
        lwHelperBox* b =LW_NEW(lwHelperBox);
        b->Clone((lwHelperBox*)s->_obj_box);
        _obj_box = b;
    }
    if(s->_obj_mesh)
    {
        lwHelperMesh* m = LW_NEW(lwHelperMesh);
        m->Clone((lwHelperMesh*)s->_obj_mesh);
        _obj_mesh = m;
    }

    return LW_RET_OK;

}

LW_RESULT lwNodeHelper::Render()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(!_CheckVisibleState())
        goto __ret_ok;

    if(_obj_dummy)
    {
        if(LW_FAILED(_obj_dummy->Render()))
            goto __ret;
    }
    if(_obj_box)
    {
        if(LW_FAILED(_obj_box->Render()))
            goto __ret;
    }
    if(_obj_mesh)
    {
        if(LW_FAILED(_obj_mesh->Render()))
            goto __ret;
    }

__ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwNodeHelper::GetLinkMatrix(lwMatrix44* mat, DWORD link_id)
{
    __asm int 3;
    return 0;
}
LW_RESULT lwNodeHelper::Update()
{
    return 0;
}

// lwNodeObject
static DWORD __tree_enum_update(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwINode* obj = (lwINode*)node->GetData();
    if(obj == 0)
    {
        assert(0 && "call __tree_enum_update error");
        goto __ret;
    }

    if(LW_FAILED(obj->Update()))
        goto __ret;

    ret = TREENODE_PROC_RET_CONTINUE;
__ret:
    return ret;
}

static DWORD __tree_enum_render(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwINode* obj = (lwINode*)node->GetData();
    if(obj == 0)
    {
        assert(0 && "call __tree_enum_render error");
        goto __ret;
    }

    //if(obj->GetType() == NODE_PRIMITIVE)
    //{
        if(LW_FAILED(((lwINodePrimitive*)obj)->Render()))
            goto __ret;
    //}

    ret = TREENODE_PROC_RET_CONTINUE;
__ret:
    return ret;
}

static DWORD __tree_proc_cullprimitive(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwISceneMgr* scn_mgr = (lwISceneMgr*)param;
    lwINode* obj = (lwINode*)node->GetData();

    if(obj == 0)
    {
        assert(0 && "call __tree_proc_cullprimitive error");
        goto __ret;
    }

    if(obj->GetType() == NODE_PRIMITIVE)
    {
        __asm { int 3 }
        //BYTE v = LW_SUCCEEDED(scn_mgr->CullPrimitive(obj)) ? 1 : 0;
        //obj->GetStateCtrl()->SetState(STATE_FRAMECULLING, v);
    }

    ret = TREENODE_PROC_RET_CONTINUE;
__ret:
    return ret;
}


static DWORD __tree_enum_destroy(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwINode* obj = (lwINode*)node->GetData();
    if(obj == 0)
    {
        assert(0 && "call __tree_enum_destroy error");
        goto __ret;
    }

    if(obj->GetType() == NODE_PRIMITIVE)
    {
        if(LW_FAILED(obj->Release()))
            goto __ret;
    }

    ret = TREENODE_PROC_RET_CONTINUE;
__ret:
    return ret;
}
static DWORD __tree_enum_query(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_CONTINUE;

    lwINode* obj = (lwINode*)node->GetData();
    if(obj == 0)
    {
        assert(0 && "call __tree_enum_query error");
        goto __ret;
    }
    {
        lwModelNodeQueryInfo* info = (lwModelNodeQueryInfo*)param;

        if (info->mask & MNQI_ID)
        {
            if (obj->GetID() != info->id)
                goto __ret;
        }
        if (info->mask & MNQI_TYPE)
        {
            if (obj->GetType() != info->type)
                goto __ret;
        }
        if (info->mask & MNQI_DATA)
        {
            if (obj != info->data)
                goto __ret;
        }
        if (info->mask & MNQI_DESCRIPTOR)
        {
            if (_tcscmp(obj->GetDescriptor(), info->descriptor))
                goto __ret;
        }
        if (info->mask & MNQI_USERPROC)
        {
            if ((*info->proc)(node, info->proc_param) == TREENODE_PROC_RET_CONTINUE)
                goto __ret;
        }

        info->node = node;

        ret = TREENODE_PROC_RET_ABORT;
    }
__ret:
    return ret;
}

LW_RESULT lwDestroyNodeObject(lwITreeNode* node)
{
    LW_RESULT ret = LW_RET_FAILED;


    if(node->EnumTree(__tree_enum_destroy, 0, TREENODE_PROC_INORDER) == TREENODE_PROC_RET_ABORT)
        goto __ret;

    lwReleaseTreeNodeList(node);

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_STD_IMPLEMENTATION(lwNodeObject)

lwNodeObject::lwNodeObject(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _obj_root(0)
{
    _name[0] = '\0';

    lwGUIDCreateObject((LW_VOID**)&_obj_root, LW_GUID_TREENODE);
    lwINode* node = 0;
    _res_mgr->CreateNode(&node, NODE_DUMMY);
    _obj_root->SetData(node);
}
lwNodeObject::~lwNodeObject()
{
    Destroy();

    LW_IF_RELEASE(((lwINode*)_obj_root->GetData()));
    LW_RELEASE(_obj_root);
}
LW_RESULT lwNodeObject::Update()
{
    LW_RESULT ret = LW_RET_FAILED;
    
    if(_obj_root->EnumTree(__tree_enum_update, 0, TREENODE_PROC_PREORDER) == TREENODE_PROC_RET_ABORT)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodeObject::Render()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_obj_root->EnumTree(__tree_enum_render, 0, TREENODE_PROC_PREORDER) == TREENODE_PROC_RET_ABORT)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwNodeObject::IgnoreNodesRender( const IgnoreStruct* is )
{
	LW_RESULT ret = LW_RET_FAILED;

	if(_obj_root->EnumTree(__tree_enum_render, ( void* ) is, TREENODE_PROC_PREORDER_IGNORE ) == TREENODE_PROC_RET_ABORT)
		goto __ret;

	ret = LW_RET_OK;
__ret:
	return ret;
}

LW_RESULT lwNodeObject::CullPrimitive()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwISysGraphics* sys_grh = _res_mgr->GetSysGraphics();
    lwISceneMgr* scn_mgr = sys_grh->GetSceneMgr();
    lwIOptionMgr* opt_mgr = sys_grh->GetSystem()->GetOptionMgr();
    BOOL cull_flag = opt_mgr->GetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL);

    if(cull_flag == 0)
        goto __ret_ok;

    if(_obj_root->EnumTree(__tree_proc_cullprimitive, (void*)scn_mgr, TREENODE_PROC_PREORDER) == TREENODE_PROC_RET_ABORT)
        goto __ret;

__ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwNodeObject::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwITreeNode* obj_node = _obj_root->GetChild();
    if(obj_node == 0)
        goto __addr_ret_ok;

    if(LW_FAILED(lwDestroyNodeObject(obj_node)))
        goto __ret;

    _obj_root->SetChild(0);

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

void lwNodeObject::SetMatrix(const lwMatrix44* mat)
{
    lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
    o->SetLocalMatrix(mat);
}

lwMatrix44* lwNodeObject::GetMatrix()
{
    lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
    return o->GetWorldMatrix();
}
lwIByteSet* lwNodeObject::GetStateSet()
{
    lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
    return o->GetStateSet();
}

LW_RESULT lwNodeObject::QueryTreeNode(lwModelNodeQueryInfo* info)
{
    return _obj_root->EnumTree(__tree_enum_query, (void*)info, TREENODE_PROC_PREORDER) == TREENODE_PROC_RET_ABORT ? LW_RET_OK : LW_RET_FAILED;
}

LW_RESULT lwNodeObject::InsertTreeNode(lwITreeNode* parent_node, lwITreeNode* prev_node, lwITreeNode* node)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(parent_node == 0)
    {
        parent_node = _obj_root;
    }

    if(LW_FAILED(parent_node->InsertChild(prev_node, node)))
        goto __ret;
    {
        lwITreeNode* parent = node->GetParent();
        assert(parent && "invalid parent when called lwNodeObject::InsertTreeNode");

        lwINode* model_parent = (lwINode*)parent->GetData();
        assert(model_parent && "invalid model_parent when called lwNodeObject::InsertTreeNode");

        lwINode* model = (lwINode*)node->GetData();
        assert(model && "invalid model when called lwNodeObject::InsertTreeNode");

        if (LW_FAILED(model->SetParent(model_parent)))
            goto __ret;

        ret = LW_RET_OK;
    }
__ret:
    return ret;

}
LW_RESULT lwNodeObject::RemoveTreeNode(lwITreeNode* node)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwITreeNode* parent = node->GetParent();

    assert(parent && "invalid node when called lwNodeObject::RemoveTreeNode");

    if(parent == 0)
        goto __ret;

    if(LW_FAILED(parent->RemoveChild(node)))
        goto __ret;

    {
        lwINode* model = (lwINode*)node->GetData();
        if (model == 0)
            goto __ret;

        if (LW_FAILED(model->SetParent(0)))
            goto __ret;

        ret = LW_RET_OK;
    }
__ret:
    return ret;

#if 0
    // process parent relation of children of node
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_FAILED(_obj_root->RemoveChild(node)))
        goto __ret;

    lwINode* root_dummy = (lwINode*)_obj_root->GetData();

    // set children of node to root node
    lwITreeNode* c = node->GetChild();
    if(c)
    {
        lwINode* n = (lwINode*)node->GetData();

        /* for vertex blend ctrl version
        // check node type with vertex blend ctrl
        if(n->GetType() == NODE_BONECTRL)
        {
            lwITreeNode* d = c;

            // set vertex blend ctrl
            while(d)
            {
                lwINodePrimitive* p = (lwINodePrimitive*)d->GetData();
                p->SetVertexBlendCtrl(0);

                d = d->GetSibling();
            }

            // set parent
            d = c;

            while(d)
            {
                lwINode* m = (lwINodePrimitive*)d->GetData();
                lwINode* m_p = m->GetParent();
                if(m_p)
                {
                    lwITreeNode* n_p = _obj_root->FindNode((void*)m_p);
                    if(n_p)
                    {
                        n_p->InsertChild(0, d);
                    }
                    else
                    {
                        m->SetParent(root_dummy);
                        _obj_root->InsertChild(0, d);
                    }
                }
                else
                {
                    m->SetParent(root_dummy);
                    _obj_root->InsertChild(0, d);
                }

                d = d->GetSibling();
            }
        }
        // reset children's parent node
        else
        */
        {
            lwITreeNode* d = c;        
            while(d)
            {
                lwINode* p = (lwINode*)d->GetData();
                p->SetParent(root_dummy);
                _obj_root->InsertChild(0, d);

                d = d->GetSibling();
            }     
        }

        node->SetChild(0);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
#endif
}
LW_RESULT lwNodeObject::Load(const char* file, DWORD flag, lwITreeNode* parent_node)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwModelInfo info;
    lwITreeNode* tree_node = 0;

    if(flag != MODELOBJECT_LOAD_RESET
        && flag != MODELOBJECT_LOAD_MERGE
        && flag != MODELOBJECT_LOAD_MERGE2)        
    {
        goto __ret;
    }

    if(LW_FAILED(info.Load(file)))
        goto __ret;


    if(LW_FAILED(lwLoadModelInfo(&tree_node, &info, _res_mgr)))
        goto __ret;

    if(flag == MODELOBJECT_LOAD_RESET || flag == MODELOBJECT_LOAD_MERGE)
    {
        if(flag == MODELOBJECT_LOAD_RESET)
        {
            if(LW_FAILED(Destroy()))
                goto __ret;
        }

        lwITreeNode* child_node = tree_node->GetChild();
        lwITreeNode* sibling_node;

        while(child_node)
        {
            sibling_node = child_node->GetSibling();

            if(LW_FAILED(InsertTreeNode(parent_node, 0, child_node)))
                goto __ret;

            child_node = sibling_node;
        }
        
        tree_node->SetChild(0);
        // release root dummy node
        //LW_RELEASE((lwINodeDummy*)tree_node->GetData());
    }
    else if(flag == MODELOBJECT_LOAD_MERGE2)
    {
        if(LW_FAILED(InsertTreeNode(parent_node, 0, tree_node)))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    if(tree_node)
    {
        lwDestroyNodeObject(tree_node);
    }
    //LW_IF_RELEASE(tree_node);
    return ret;
}

// assistant proc
DWORD __tree_proc_play_pose(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwPlayPoseInfo* info = (lwPlayPoseInfo*)param;
    lwINode* base_node = (lwINode*)node->GetData();
    if(base_node == 0)
        goto __ret;

    switch(base_node->GetType())
    {
    case NODE_PRIMITIVE:
        if(LW_FAILED(lwNodePrimitive_PlayPoseAll((lwINodePrimitive*)base_node, info)))
            goto __ret;
        break;
    case NODE_BONECTRL:
        if(LW_FAILED(lwNodeBoneCtrl_PlayPose((lwINodeBoneCtrl*)base_node, info)))
            goto __ret;
        break;
    case NODE_DUMMY:        
        if(LW_FAILED(lwNodeDummy_PlayPose((lwINodeDummy*)base_node, info)))
            goto __ret;
        break;
    case NODE_HELPER:
        break;
    default:
        __asm int 3;
    }

    ret = TREENODE_PROC_RET_CONTINUE;
__ret:
    return ret;
}

LW_RESULT lwNodeObject_PlayDefaultPose(lwINodeObject* obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwPlayPoseInfo ppi;
    memset(&ppi, 0, sizeof(ppi));
    ppi.bit_mask = PPI_MASK_DEFAULT;
    ppi.pose = 0;
    ppi.frame = 0.0f;
    ppi.type = PLAY_LOOP;
    ppi.velocity = 0.5f; // doesnt seem to change anything

    if(obj->GetTreeNodeRoot()->EnumTree(__tree_proc_play_pose,(void*)&ppi, TREENODE_PROC_PREORDER)
        != TREENODE_PROC_RET_CONTINUE)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwNodePrimitive_PlayPose(lwINodePrimitive* obj, const lwPlayPoseInfo* info, DWORD ctrl_type, DWORD subset, DWORD stage)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwIAnimCtrlAgent* anim_agent = obj->GetAnimCtrlAgent();
    if(anim_agent == 0)
    {
        ret = LW_RET_OK_1;
        goto __ret;
    }

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ctrl_type;
    type_info.data[0] = subset;
    type_info.data[1] = stage;
    
    {
        lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
        if (ctrl_obj == 0)
        {
            ret = LW_RET_OK_1;
            goto __ret;
        }

        if (LW_FAILED(ctrl_obj->PlayPose(info)))
            goto __ret;

        ret = LW_RET_OK;
    }
__ret:
    return ret;

}
LW_RESULT lwNodePrimitive_PlayPoseAll(lwINodePrimitive* obj, const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrlAgent* anim_agent = obj->GetAnimCtrlAgent();
    if(anim_agent == 0)
    {
        ret = LW_RET_OK_1;
        goto __ret;
    }

    {
        lwAnimCtrlObjTypeInfo type_info;
        lwIAnimCtrlObj* ctrl_obj;
        DWORD n = anim_agent->GetAnimCtrlObjNum();

        for (DWORD i = 0; i < n; i++)
        {
            ctrl_obj = anim_agent->GetAnimCtrlObj(i);

            // lwAnimCtrlObjBonelwPrimitive
            ctrl_obj->GetTypeInfo(&type_info);
            if (type_info.type == ANIM_CTRL_TYPE_BONE)
                continue;

            if (LW_FAILED(ctrl_obj->PlayPose(info)))
                goto __ret;
        }

        ret = LW_RET_OK;
    }
__ret:
    return ret;

}

LW_RESULT lwNodeBoneCtrl_PlayPose(lwINodeBoneCtrl* obj, const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrlObjBone* ctrl_obj_bone = obj->GetAnimCtrlObj();
    if(ctrl_obj_bone == 0)
    {
        ret = LW_RET_OK_1;
        goto __ret;
    }

    if(LW_FAILED(ctrl_obj_bone->PlayPose(info)))
        goto __ret;


    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwNodeDummy_PlayPose(lwINodeDummy* obj, const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrlObjMat* ctrl_obj = obj->GetAnimCtrlObj();
    if(ctrl_obj == 0)
    {
        ret = LW_RET_OK_1;
        goto __ret;
    }

    if(LW_FAILED(ctrl_obj->PlayPose(info)))
        goto __ret;


    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwNode_ShowBoundingObject(lwINode* obj, DWORD flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIHelperObject* helper_obj;
    lwINodePrimitive* dummy_obj;

    DWORD type = obj->GetType();

    switch(type)
    {
    case NODE_PRIMITIVE:
        helper_obj = ((lwINodePrimitive*)obj)->GetHelperObject();
        if(helper_obj)
        {
            helper_obj->SetVisible(flag);
        }
        ret = LW_RET_OK;
        break;
    case NODE_BONECTRL:
        ret = LW_RET_OK;
        break;
    case NODE_HELPER:
        ret = LW_RET_OK;
        break;
    case NODE_LIGHT:
        ret = LW_RET_OK;
        break;
    case NODE_CAMERA:
        ret = LW_RET_OK;
        break;
    case NODE_DUMMY:
        dummy_obj = ((lwINodeDummy*)obj)->GetAssistantObject();
        if(dummy_obj)
        {
            dummy_obj->GetStateSet()->SetValue(STATE_VISIBLE, (BYTE)flag);
        }
        ret = LW_RET_OK;
        break;
    default:
        ;
    }

    return ret;
}

static DWORD __tree_proc_show_bo(lwITreeNode* node, void* param)
{
    if(LW_FAILED(lwNode_ShowBoundingObject((lwINode*)node->GetData(), *(DWORD*)param)))
    {
        return TREENODE_PROC_RET_ABORT;
    }

    return TREENODE_PROC_RET_CONTINUE;
}


static DWORD __tree_proc_dump_objtree(lwITreeNode* node, void* param)
{
    lwINode* obj_node = (lwINode*)node->GetData();
    FILE* fp = (FILE*)param;

    DWORD l = node->GetDepthLevel();
    DWORD c = 0;
    lwINode* p = obj_node->GetParent();
    while(p)
    {
        c++;
        p = p->GetParent();
    }
    assert(l == c);

    for(DWORD i = 0; i < l; i++)
    {
        fprintf(fp, "\t");
    }

    fprintf(fp, "type: %d, id: %d, parent: %d\n", 
        obj_node->GetType(), 
        obj_node->GetID(),
        l == 0 ? -1 : obj_node->GetParent()->GetID());


    return TREENODE_PROC_RET_CONTINUE;
}
static DWORD __tree_proc_getprinum(lwITreeNode* node, void* param)
{
    lwINode* obj = (lwINode*)node->GetData();
    
    if(obj->GetType() == NODE_PRIMITIVE)
    {
        if(obj->GetStateSet()->GetValue(STATE_FRAMECULLING))
        {
            *((DWORD*)param) += 1;
        }

    }

    return TREENODE_PROC_RET_CONTINUE;
}

LW_RESULT lwNodeObject_ShowBoundingObject(lwINodeObject* obj, DWORD flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(obj->GetTreeNodeRoot()->EnumTree(__tree_proc_show_bo,(void*)&flag, TREENODE_PROC_PREORDER)
        != TREENODE_PROC_RET_CONTINUE)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwNodeObject_DumpObjectTree(lwINodeObject* obj, const char* file)
{
    LW_RESULT ret = LW_RET_FAILED;

    FILE* fp = fopen(file, "wt");
    if(fp == 0)
        goto __ret;

    if(obj->GetTreeNodeRoot()->EnumTree(__tree_proc_dump_objtree,(void*)fp, TREENODE_PROC_PREORDER)
        != TREENODE_PROC_RET_CONTINUE)
        goto __ret;

    ret = LW_RET_OK;


__ret:
    if(fp)
    {
        fclose(fp);
    }
    return ret;
}

LW_RESULT lwNodeObject_GetPrimitiveCullingNum(lwINodeObject* obj, DWORD* num)
{
    LW_RESULT ret = LW_RET_FAILED;

    *num = 0;

    if(obj->GetTreeNodeRoot()->EnumTree(__tree_proc_getprinum,(void*)num, TREENODE_PROC_PREORDER)
        != TREENODE_PROC_RET_CONTINUE)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
    
}


LW_END

#endif
