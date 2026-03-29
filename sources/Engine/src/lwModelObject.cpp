//
#if 1

#include "lwModelObject.h"
#include "lwMisc.h"
#include "lwIUtil.h"
#include "lwPrimitiveHelper.h"

LW_BEGIN

DWORD __tree_proc_play_pose(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwPlayPoseInfo* info = (lwPlayPoseInfo*)param;
    lwIModelNode* base_node = (lwIModelNode*)node->GetData();
    if(base_node == 0)
        goto __ret;

    switch(base_node->GetType())
    {
    case MODELNODE_PRIMITIVE:
        if(LW_FAILED(lwPrimitiveObj_PlayPoseAll((lwIPrimitiveObj*)base_node, info)))
            goto __ret;
        break;
    case MODELNODE_VERTEXBLENDCTRL:
        if(LW_FAILED(lwVertexBlendCtrl_PlayPose((lwIVertexBlendCtrl*)base_node, info)))
            goto __ret;
        break;
    case MODELNODE_DUMMY:
        break;
    case MODELNODE_HELPER:
        break;
    default:
        __asm int 3;
    }

    ret = TREENODE_PROC_RET_CONTINUE;
__ret:
    return ret;
}

LW_RESULT lwModelObject_PlayDefaultPose(lwIModelObject* obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwPlayPoseInfo ppi;
    memset( &ppi, 0, sizeof( ppi ) );
    ppi.bit_mask = PPI_MASK_DEFAULT;
    ppi.pose = 0;
    ppi.frame = 0.0f;
    ppi.type = PLAY_LOOP;
    ppi.velocity = 1.0f;

    if(obj->GetTreeNodeRoot()->EnumTree(__tree_proc_play_pose,(void*)&ppi, TREENODE_PROC_PREORDER)
        != TREENODE_PROC_RET_CONTINUE)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwPrimitiveObj_PlayPose(lwIPrimitiveObj* obj, const lwPlayPoseInfo* info, DWORD ctrl_type, DWORD subset, DWORD stage)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrlAgent* anim_agent = obj->GetAnimCtrlAgent();
    if(anim_agent == 0)
        goto __ret;

    lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(subset, stage, ctrl_type);
    if(ctrl_obj == 0)
        goto __ret;

    if(LW_FAILED(ctrl_obj->PlayPose(info)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwPrimitiveObj_PlayPoseAll(lwIPrimitiveObj* obj, const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrlAgent* anim_agent = obj->GetAnimCtrlAgent();
    if(anim_agent == 0)
        goto __addr_ret_ok;

    lwIAnimCtrlObj* ctrl_obj;
    DWORD n = anim_agent->GetAnimCtrlObjNum();

    for(DWORD i = 0; i < n; i++)
    {
        ctrl_obj = anim_agent->GetAnimCtrlObj(i);

        // lwAnimCtrlObjBonelwPrimitive
        if(ctrl_obj->GetType() == ANIM_CTRL_TYPE_BONE)
            continue;

        if(LW_FAILED(ctrl_obj->PlayPose(info)))
            goto __ret;
    }
__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwVertexBlendCtrl_PlayPose(lwIVertexBlendCtrl* obj, const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrlObjBone* ctrl_obj_bone = obj->GetAnimCtrlObj();
    if(ctrl_obj_bone == 0)
        goto __ret;

    if(LW_FAILED(ctrl_obj_bone->PlayPose(info)))
        goto __ret;


    ret = LW_RET_OK;
__ret:
    return ret;

}

// lwModelNode
lwModelNode::lwModelNode()
{
    // base
    _type = MODELNODE_INVALID;
    _id = LW_INVALID_INDEX;
    _descriptor[0] = '\0';
    lwMatrix44Identity(&_mat_local);
    lwMatrix44Identity(&_mat_world);
    lwStateCtrl _state_ctrl;

    // link state
    _parent = 0;
    _link_parent_id = LW_INVALID_INDEX;
    _link_id = LW_INVALID_INDEX;
}

//LW_RESULT lwModelNode::_UpdateMatrix()
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

    lwIModelNode* model_node = 0;
    if(LW_FAILED(res_mgr->CreateModelNode(&model_node, node_info->_type)))
        goto __ret;

    switch(node_info->_type)
    {
    case MODELNODE_PRIMITIVE:
        if(LW_FAILED(((lwIPrimitiveObj*)model_node)->Load((lwIGeomObjInfo*)node_info->_data, tex_path, 0)))
            goto __ret;
        break;
    case MODELNODE_VERTEXBLENDCTRL:
        if(LW_FAILED(((lwIVertexBlendCtrl*)model_node)->Load((lwIAnimDataBone*)node_info->_data)))
            goto __ret;
        break;
    case MODELNODE_DUMMY:
        model_node->SetLocalMatrix((lwMatrix44*)node_info->_data);
        break;
    case MODELNODE_HELPER:
        assert(0);
        break;
    default:
        assert(0);
    }

    lwITreeNode* model_tree;
    lwGUIDCreateObject((LW_VOID**)&model_tree, LW_GUID_TREENODE);
    model_tree->SetData(model_node);

    // set param
    node_info->_param = model_tree;

    // link tree parent
    lwITreeNode* node_parent = node->GetParent();
    if(node_parent)
    {
        lwModelNodeInfo* parent_info = (lwModelNodeInfo*)node_parent->GetData();
        lwITreeNode* parent_obj_node = (lwITreeNode*)parent_info->_param;

        parent_obj_node->InsertChild(0, model_tree);

        lwIModelNode* parent_obj_data = (lwIModelNode*)parent_obj_node->GetData();
        assert(parent_obj_data && "invalid parent_obj_data when called __tree_proc_loadmodel");

        model_node->SetParent(parent_obj_data);
        
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

    lwModelNodeInfo* root_info = (lwModelNodeInfo*)info->_obj_tree->GetData();

    *ret_obj_tree = (lwITreeNode*)root_info->_param;

    ret = LW_RET_OK;
__ret:
    return ret;
}
// lwPrimitiveObj
LW_STD_IMPLEMENTATION(lwPrimitiveObj)

lwPrimitiveObj::lwPrimitiveObj(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr)
{
    _type = MODELNODE_PRIMITIVE;

    _mesh_agent = 0;
    _mtltex_agent_seq = 0;
    _anim_agent = 0;
    _render_agent = 0;
    _helper_object = 0;
    //_vertex_blend_ctrl = 0;
    _ref_ctrl_obj_bone = 0;

    _mtltex_agent_seqsize = 0;
}

lwPrimitiveObj::~lwPrimitiveObj()
{
    Destroy();
}

LW_RESULT lwPrimitiveObj::_DestroyMtlTexAgent()
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

LW_RESULT lwPrimitiveObj::_UpdateBoundingObject(lwIAnimCtrlObjBone* ctrl_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_helper_object == 0)
        goto __addr_ret_ok;

    lwIBoundingSphere* b;
    lwBoundingSphereInfo* s;

    if((b = _helper_object->GetBoundingSphere()) == 0)
        goto __addr_ret_ok;

    DWORD bs_num = b->GetObjNum();

    for(DWORD j = 0; j < bs_num; j++)
    {
        s = b->GetObjInfo(j);

        assert(s->id < LW_MAX_BONEDUMMY_NUM);

        lwMatrix44* mat = ctrl_obj->GetDummyRTM(s->id);
        if(mat)
        {
            s->mat = *mat;
        }
    }

__addr_ret_ok:
    ret = LW_RET_OK;

    return ret;
}

LW_RESULT lwPrimitiveObj::Destroy()
{
    LW_SAFE_RELEASE(_mesh_agent);
    LW_SAFE_RELEASE(_anim_agent);
    LW_SAFE_RELEASE(_render_agent);
    LW_SAFE_RELEASE(_helper_object);

    _DestroyMtlTexAgent();

    return LW_RET_OK;
}

LW_RESULT lwPrimitiveObj::AllocateMtlTexAgentSeq(DWORD num)
{
    _DestroyMtlTexAgent();

    _mtltex_agent_seq = LW_NEW(lwIMtlTexAgent*[num]);
    _mtltex_agent_seqsize = num;
    memset(_mtltex_agent_seq, 0, sizeof(lwIMtlTexAgent*) * num);

    return LW_RET_OK;
}

LW_RESULT lwPrimitiveObj::Load(lwIGeomObjInfo* geom_info, const char* tex_path, const lwResFile* res)
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

    _id = info->id;
    _mat_local = info->mat_local;
    _state_ctrl = info->state_ctrl;
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
        _tcscpy( info.file_name, res->file_name );

        _mesh_agent->GetMesh()->SetResFile( &info );
    }

    // helper info
    if(info->helper_data.type != HELPER_TYPE_INVALID)
    {
        LW_IF_RELEASE(_helper_object);
        _res_mgr->CreateHelperObject(&_helper_object);
        if(LW_FAILED(_helper_object->LoadHelperInfo(&info->helper_data, create_helper_primitive)))
        {
            lwMessageBox("LoadHelperInfo");
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

LW_RESULT lwPrimitiveObj::LoadMesh(lwMeshInfo* info)
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
LW_RESULT lwPrimitiveObj::LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, const char* tex_path)
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

LW_RESULT lwPrimitiveObj::LoadAnimData(lwIAnimDataInfo* data_info, const char* tex_path, const lwResFile* res)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimDataInfo* info = (lwAnimDataInfo*)data_info;

    void* data;
    DWORD ctrl_type;
    DWORD subset_type;
    lwIAnimCtrl* ctrl = NULL;
    lwIAnimCtrlObj* ctrl_obj = NULL;

    LW_SAFE_RELEASE(_anim_agent);

    if(LW_FAILED(_res_mgr->CreateAnimCtrlAgent(&_anim_agent)))
        goto __ret;


    if( info->anim_mat.GetDataSize() > 0 )
    {
        ctrl_type = ANIM_CTRL_TYPE_MAT;
        data = &info->anim_mat;
        subset_type = LW_INVALID_INDEX;

        if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, ctrl_type)))
            goto __ret;

        if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, ctrl_type)))
            goto __ret;

        if(LW_FAILED(ctrl->LoadData(data)))
            goto __ret;

        ctrl_obj->SetAnimCtrl(ctrl);
        _anim_agent->AddAnimCtrlObj(ctrl_obj);
        if( res )
        {
            lwResFileAnimData info;
            info.obj_id = res->obj_id;
            info.res_type = res->res_type;
            _tcscpy( info.file_name, res->file_name );
            info.anim_type = ctrl_type;

            ctrl->SetResFile( &info );
        }
        
    }
    if( info->anim_bone.GetDataSize() > 0 )
    {
        ctrl_type = ANIM_CTRL_TYPE_BONE;
        data = &info->anim_bone;


        if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, ctrl_type)))
            goto __ret;

        if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, ctrl_type)))
            goto __ret;

        if(LW_FAILED(ctrl->LoadData(data)))
            goto __ret;

        ctrl_obj->SetAnimCtrl(ctrl);
        _anim_agent->AddAnimCtrlObj(ctrl_obj);
        if( res )
        {
            lwResFileAnimData info;
            info.obj_id = res->obj_id;
            info.res_type = res->res_type;
            _tcscpy( info.file_name, res->file_name );
            info.anim_type = ctrl_type;

            ctrl->SetResFile( &info );
        }

        ctrl = NULL;
        ctrl_obj = NULL;


    }


    for( DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++ )
    {
        if( _mtltex_agent_seq[ i ] == NULL )
            continue;

        for( DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++ )
        {
            if( info->anim_tex[ i ][ j ].GetDataSize() > 0 )
            {
                ctrl_type = ANIM_CTRL_TYPE_TEXUV;
                data = &info->anim_tex[ i ][ j ];

                if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, ctrl_type)))
                    goto __ret;

                if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, ctrl_type)))
                    goto __ret;

                if(LW_FAILED(ctrl->LoadData(data)))
                    goto __ret;

                ctrl_obj->SetAnimCtrl(ctrl);
                ctrl_obj->SetSubset(i);
                ctrl_obj->SetStage(j);
                _anim_agent->AddAnimCtrlObj(ctrl_obj);
                if( res )
                {
                    lwResFileAnimData info;
                    info.obj_id = res->obj_id;
                    info.res_type = res->res_type;
                    _tcscpy( info.file_name, res->file_name );
                    info.anim_type = ctrl_type;

                    ctrl->SetResFile( &info );
                }

                ctrl = NULL;
                ctrl_obj = NULL;
            }

            //continue;
            // image
            if(info->anim_img[i][j].GetDataSize() > 0)
            {
                ctrl_type = ANIM_CTRL_TYPE_TEXIMG;
                data = &info->anim_img[i][j];

                _tcscpy(info->anim_img[i][j]._tex_path, tex_path);

                if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, ctrl_type)))
                    goto __ret;

                if(LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, ctrl_type)))
                    goto __ret;

                if(LW_FAILED(ctrl->LoadData(data)))
                    goto __ret;

                ctrl_obj->SetAnimCtrl(ctrl);
                ctrl_obj->SetSubset(i);
                ctrl_obj->SetStage(j);
                _anim_agent->AddAnimCtrlObj(ctrl_obj);
                if( res )
                {
                    lwResFileAnimData info;
                    info.obj_id = res->obj_id;
                    info.res_type = res->res_type;
                    _tcscpy( info.file_name, res->file_name );
                    info.anim_type = ctrl_type;

                    ctrl->SetResFile( &info );
                }

                ctrl = NULL;
                ctrl_obj = NULL;
            }

        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwPrimitiveObj::LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIRenderCtrl* render_ctrl = 0;
    lwIRenderCtrlAgent* render_agent = 0;

    if(_render_agent)
        goto __ret;


    if(LW_FAILED(_res_mgr->CreateRenderCtrlAgent(&render_agent)))
        goto __ret;

    if(LW_FAILED(_res_mgr->RequestRenderCtrl(&render_ctrl, rcci->ctrl_id)))
        goto __ret;

    render_agent->SetVertexDeclaration(rcci->decl_id);
    render_agent->SetVertexShader(rcci->vs_id);
    render_agent->SetRenderCtrl(render_ctrl);

    _render_agent = render_agent;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwPrimitiveObj::Copy(lwIPrimitiveObj* src_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    _id = src_obj->GetID();
    _mat_local = *src_obj->GetLocalMatrix();
    _mat_world = *src_obj->GetWorldMatrix();

    _parent = src_obj->GetParent();
    _link_id = src_obj->GetLinkID();
    _link_parent_id = src_obj->GetLinkParentID();
    
    lwIMeshAgent* src_mesh_agent = src_obj->GetMeshAgent();
    if(src_mesh_agent)
    {
        if(LW_FAILED(src_mesh_agent->Clone(&_mesh_agent )))
            goto __ret;
    }

    lwIMtlTexAgent* src_mtltex_agent;
    DWORD src_mtltex_agent_seqsize = GetMtlTexAgentSeqSize();

    AllocateMtlTexAgentSeq(src_mtltex_agent_seqsize);

    for(DWORD i = 0; i < src_mtltex_agent_seqsize; i++)
    {
        if((src_mtltex_agent = src_obj->GetMtlTexAgent(i)) == NULL)
            continue;

        if(LW_FAILED(src_mtltex_agent->Clone(&_mtltex_agent_seq[i])))
            goto __ret;
    }

    lwIAnimCtrlAgent* src_anim_agent = src_obj->GetAnimCtrlAgent();
    if(src_anim_agent)
    {
        if(LW_FAILED(src_anim_agent->Clone(&_anim_agent)))
            goto __ret;
    }

    lwIRenderCtrlAgent* src_render_agent = src_obj->GetRenderCtrlAgent();
    if(src_render_agent)
    {
        if(LW_FAILED(_render_agent->Clone(&_render_agent)))
            goto __ret;
    }

    lwIHelperObject* src_helper_object = src_obj->GetHelperObject();
    if(src_helper_object)
    {
        if(LW_FAILED(src_helper_object->Clone(&_helper_object)))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwPrimitiveObj::Update()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_anim_agent)
    {
        if(LW_FAILED(_anim_agent->Update()))
            goto __ret;

        if(LW_FAILED(_anim_agent->UpdateObject2(this)))
            goto __ret;
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
            lwIAnimCtrlObjBone* ctrl_obj = ((lwIVertexBlendCtrl*)_parent)->GetAnimCtrlObj();
            if(ctrl_obj == 0)
                goto __ret;

            ctrl_obj->UpdateObject(_ref_ctrl_obj_bone, this);

            // update bounding object
            // has been done in ctrl_obj->UpdateObject
            //if(LW_FAILED(_UpdateBoundingObject(ctrl_obj)))
            //    goto __ret;
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

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwPrimitiveObj::Render()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_state_ctrl.GetState(STATE_VISIBLE) == 0)
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

        if(LW_FAILED(_render_agent->BeginSetSubset(i)))
            goto __ret;

        if(LW_FAILED(_render_agent->DrawSubset(i)))
            goto __ret;

        if(LW_FAILED(_render_agent->EndSetSubset(i)))
            goto __ret;
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
LW_RESULT lwPrimitiveObj::RenderSubset(DWORD subset)
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
LW_RESULT lwPrimitiveObj::RenderHelperObject()
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

LW_RESULT lwPrimitiveObj::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_helper_object == 0)
        goto __ret;

    lwIBoundingSphere* bs = 0;
    lwIBoundingBox* bb = 0;

    lwMatrix44* mat = _render_agent->GetGlobalMatrix();

    if((bs = _helper_object->GetBoundingSphere()) != 0)
    {
        if(LW_SUCCEEDED(bs->HitTest(info, org, ray, mat)))
            goto __addr_ret_ok;
    }

    if((bb = _helper_object->GetBoundingBox()) != 0)
    {
        if(LW_SUCCEEDED(bb->HitTest(info, org, ray, mat)))
            goto __addr_ret_ok;
    }

    goto __ret;

__addr_ret_ok:

    ret = LW_RET_OK;

__ret:

    return ret;

}

LW_RESULT lwPrimitiveObj::GetLinkMatrix(lwMatrix44* mat, DWORD link_id)
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

void lwPrimitiveObj::SetMaterial(const lwMaterial* mtl)
{
    for(DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++)
    {
        if( _mtltex_agent_seq[i] == NULL )
            continue;

        _mtltex_agent_seq[i]->SetMaterial(mtl);
    }
}
void lwPrimitiveObj::SetOpacity(float opacity)
{
    for(DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++)
    {
        if(_mtltex_agent_seq[i] == NULL)
            continue;

        _mtltex_agent_seq[i]->SetOpacity(opacity);
    }

    _state_ctrl.SetState(STATE_TRANSPARENT, opacity == 1.0f ? 0 : 1);
}

// SetParentparentVertexBlendCtrl
// 1.lwAnimCtrlAgentANIM_CTRL_TYPE_BONE
// 2._ref_ctrl_obj_bone
// 3.lwRenderCtrlAgentvs_typevs_ctrlvs_type
//   fvfvs_typevs_ctrl
//   
//   vs_type,vs_ctrl
LW_RESULT lwPrimitiveObj::SetParent(lwIModelNode* parent)
{
    LW_RESULT ret = LW_RET_FAILED;

    //check parent model type
    if(parent == 0)
    {
        if(_parent == 0)
            goto __addr_ret_ok;

        if(_parent->GetType() == MODELNODE_VERTEXBLENDCTRL)
        {
            if(_anim_agent)
            {
                lwIAnimCtrlObj* ctrl_obj = _anim_agent->RemoveAnimCtrlObj(LW_INVALID_INDEX, LW_INVALID_INDEX, ANIM_CTRL_TYPE_MAT);
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

    if(parent->GetType() == MODELNODE_VERTEXBLENDCTRL)
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

    _parent = parent;


__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;

}

/*
LW_RESULT lwPrimitiveObj::SetVertexBlendCtrl(lwIVertexBlendCtrl* ctrl)
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
// lwVertexBlendCtrl
LW_STD_IMPLEMENTATION(lwVertexBlendCtrl)

lwVertexBlendCtrl::lwVertexBlendCtrl(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _ctrl_obj(0)
{
    _type = MODELNODE_VERTEXBLENDCTRL;
}
lwVertexBlendCtrl::~lwVertexBlendCtrl()
{
}
LW_RESULT lwVertexBlendCtrl::GetLinkMatrix(lwMatrix44* mat, DWORD link_id)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_ctrl_obj == 0)
        goto __ret;

    if(link_id == LW_INVALID_INDEX)
    {
        *mat = _mat_world;
        goto __addr_ret_ok;
    }

    lwMatrix44* rtm = _ctrl_obj->GetDummyRTM(link_id);

    if(rtm == NULL)
        goto __ret;

    lwMatrix44Multiply(mat, rtm, &_mat_world);

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwVertexBlendCtrl::Update()
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

    if(LW_FAILED(_ctrl_obj->Update()))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwVertexBlendCtrl::Load(lwIAnimDataBone* data)
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

    ctrl_obj_bone->SetAnimCtrl(ctrl_bone);

    _ctrl_obj = ctrl_obj_bone;

    ctrl_obj_bone = 0;
    ctrl_bone = 0;

    ret = LW_RET_OK;
__ret:
    LW_IF_RELEASE(ctrl_bone);
    LW_IF_RELEASE(ctrl_obj_bone);

    return ret;

}

LW_RESULT lwVertexBlendCtrl::PlayPose(const lwPlayPoseInfo* info)
{
    return _ctrl_obj->PlayPose(info);
}
lwIPoseCtrl* lwVertexBlendCtrl::GetPoseCtrl()
{
    return lwIAnimCtrlObj_GetPoseCtrl(_ctrl_obj);
}
lwPlayPoseInfo* lwVertexBlendCtrl::GetPlayPoseInfo()
{
    return lwIAnimCtrlObj_GetPlayPoseInfo(_ctrl_obj);
}


//lwNodeDummy
LW_STD_IMPLEMENTATION(lwNodeDummy)
lwNodeDummy::lwNodeDummy(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr)
{
    _type = MODELNODE_DUMMY;
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
LW_RESULT lwNodeDummy::Update()
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

    ret = LW_RET_OK;
__ret:
    return ret;

}

// lwHelperObj
LW_STD_IMPLEMENTATION(lwHelperObj)

lwHelperObj::lwHelperObj(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr)
{
    _type = MODELNODE_HELPER;

    _obj_dummy = 0;
    _obj_box = 0;
    _obj_mesh = 0;
}
lwHelperObj::~lwHelperObj()
{
    Destroy();
}

LW_RESULT lwHelperObj::Destroy()
{
    LW_SAFE_RELEASE(_obj_dummy);
    LW_SAFE_RELEASE(_obj_box);
    LW_SAFE_RELEASE(_obj_mesh);

    return LW_RET_OK;
}

LW_RESULT lwHelperObj::LoadHelperInfo(const lwHelperInfo* info, int create_instance_flag)
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
LW_RESULT lwHelperObj::Copy(const lwIHelperObj* src)
{
    lwHelperObj* s = (lwHelperObj*)src;

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

LW_RESULT lwHelperObj::Render()
{
    LW_RESULT ret = LW_RET_FAILED;

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

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwHelperObj::GetLinkMatrix(lwMatrix44* mat, DWORD link_id)
{
    __asm int 3;
    return 0;
}
LW_RESULT lwHelperObj::Update()
{
    return 0;
}

// lwModelObject
static DWORD __tree_enum_update(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwIModelNode* obj = (lwIModelNode*)node->GetData();
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

    lwIModelNode* obj = (lwIModelNode*)node->GetData();
    if(obj == 0)
    {
        assert(0 && "call __tree_enum_render error");
        goto __ret;
    }

    if(obj->GetType() == MODELNODE_PRIMITIVE)
    {
        if(LW_FAILED(((lwIPrimitiveObj*)obj)->Render()))
            goto __ret;
    }

    ret = TREENODE_PROC_RET_CONTINUE;
__ret:
    return ret;
}

static DWORD __tree_enum_destroy(lwITreeNode* node, void* param)
{
    DWORD ret = TREENODE_PROC_RET_ABORT;

    lwIModelNode* obj = (lwIModelNode*)node->GetData();
    if(obj == 0)
    {
        assert(0 && "call __tree_enum_destroy error");
        goto __ret;
    }

    if(obj->GetType() == MODELNODE_PRIMITIVE)
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

    lwIModelNode* obj = (lwIModelNode*)node->GetData();
    if(obj == 0)
    {
        assert(0 && "call __tree_enum_query error");
        goto __ret;
    }

    lwModelNodeQueryInfo* info = (lwModelNodeQueryInfo*)param;

    if(info->mask & MNQI_ID)
    {
        if(obj->GetID() != info->id)
            goto __ret;
    }
    if(info->mask & MNQI_TYPE)
    {
        if(obj->GetType() != info->type)
            goto __ret;
    }
    if(info->mask & MNQI_DATA)
    {
        if(obj != info->data)
            goto __ret;
    }
    if(info->mask & MNQI_DESCRIPTOR)
    {
        if(_tcscmp(obj->GetDescriptor(), info->descriptor))
            goto __ret;
    }
    if(info->mask & MNQI_USERPROC)
    {
        if((*info->proc)(node, info->proc_param) == TREENODE_PROC_RET_CONTINUE)
            goto __ret;
    }

    info->node = node;

    ret = TREENODE_PROC_RET_ABORT;

__ret:
    return ret;
}

LW_STD_IMPLEMENTATION(lwModelObject)

lwModelObject::lwModelObject(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _obj_root(0)
{
    _name[0] = '\0';

    lwGUIDCreateObject((LW_VOID**)&_obj_root, LW_GUID_TREENODE);
    lwIModelNode* node = 0;
    _res_mgr->CreateModelNode(&node, MODELNODE_DUMMY);
    _obj_root->SetData(node);
}
lwModelObject::~lwModelObject()
{
    Destroy();

    LW_IF_RELEASE(((lwIModelNode*)_obj_root->GetData()));
    LW_RELEASE(_obj_root);
}
LW_RESULT lwModelObject::Update()
{
    LW_RESULT ret = LW_RET_FAILED;
    
    if(LW_FAILED(_obj_root->EnumTree(__tree_enum_update, 0, TREENODE_PROC_PREORDER)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwModelObject::Render()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_obj_root->EnumTree(__tree_enum_render, 0, TREENODE_PROC_PREORDER) == TREENODE_PROC_RET_ABORT)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwModelObject::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwITreeNode* obj_node = _obj_root->GetChild();
    if(obj_node == 0)
        goto __addr_ret_ok;

    if(LW_FAILED(obj_node->EnumTree(__tree_enum_destroy, 0, TREENODE_PROC_INORDER)))
        goto __ret;

    lwReleaseTreeNodeList(obj_node);

    _obj_root->SetChild(0);

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
void lwModelObject::SetMatrix(const lwMatrix44* mat)
{
    lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
    o->SetLocalMatrix(mat);
}
void lwModelObject::SetStateCtrl(lwStateCtrl* ctrl)
{
    lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
    o->SetStateCtrl(ctrl);
}
lwMatrix44* lwModelObject::GetMatrix()
{
    lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
    return o->GetWorldMatrix();
}
lwStateCtrl* lwModelObject::GetStateCtrl()
{
    lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
    return o->GetStateCtrl();
}

LW_RESULT lwModelObject::QueryTreeNode(lwModelNodeQueryInfo* info)
{
    return _obj_root->EnumTree(__tree_enum_query, (void*)info, TREENODE_PROC_PREORDER);
}

LW_RESULT lwModelObject::InsertTreeNode(lwITreeNode* parent_node, lwITreeNode* prev_node, lwITreeNode* node)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(parent_node == 0)
    {
        parent_node = _obj_root;
    }

    if(LW_FAILED(parent_node->InsertChild(prev_node, node)))
        goto __ret;

    lwITreeNode* parent = node->GetParent();
    assert(parent && "invalid parent when called lwModelObject::InsertTreeNode");

    lwIModelNode* model_parent = (lwIModelNode*)parent->GetData();
    assert(model_parent && "invalid model_parent when called lwModelObject::InsertTreeNode");

    lwIModelNode* model = (lwIModelNode*)node->GetData();
    assert(model && "invalid model when called lwModelObject::InsertTreeNode");

    if(LW_FAILED(model->SetParent(model_parent)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwModelObject::RemoveTreeNode(lwITreeNode* node)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwITreeNode* parent = node->GetParent();

    assert(parent && "invalid node when called lwModelObject::RemoveTreeNode");

    if(parent == 0)
        goto __ret;

    if(LW_FAILED(parent->RemoveChild(node)))
        goto __ret;

    lwIModelNode* model = (lwIModelNode*)node->GetData();
    if(model == 0)
        goto __ret;

    if(LW_FAILED(model->SetParent(0)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

#if 0
    // process parent relation of children of node
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_FAILED(_obj_root->RemoveChild(node)))
        goto __ret;

    lwIModelNode* root_dummy = (lwIModelNode*)_obj_root->GetData();

    // set children of node to root node
    lwITreeNode* c = node->GetChild();
    if(c)
    {
        lwIModelNode* n = (lwIModelNode*)node->GetData();

        /* for vertex blend ctrl version
        // check node type with vertex blend ctrl
        if(n->GetType() == MODELNODE_VERTEXBLENDCTRL)
        {
            lwITreeNode* d = c;

            // set vertex blend ctrl
            while(d)
            {
                lwIPrimitiveObj* p = (lwIPrimitiveObj*)d->GetData();
                p->SetVertexBlendCtrl(0);

                d = d->GetSibling();
            }

            // set parent
            d = c;

            while(d)
            {
                lwIModelNode* m = (lwIPrimitiveObj*)d->GetData();
                lwIModelNode* m_p = m->GetParent();
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
                lwIModelNode* p = (lwIModelNode*)d->GetData();
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
LW_RESULT lwModelObject::Load(const char* file, DWORD flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwModelInfo info;

    if(flag != MODELOBJECT_LOAD_RESET
        && flag != MODELOBJECT_LOAD_MERGE
        && flag != MODELOBJECT_LOAD_MERGE2)        
    {
        goto __ret;
    }

    if(LW_FAILED(info.Load(file)))
        goto __ret;

    lwITreeNode* tree_node;

    if(LW_FAILED(lwLoadModelInfo(&tree_node, &info, _res_mgr)))
        goto __ret;

    if(flag == MODELOBJECT_LOAD_RESET || flag == MODELOBJECT_LOAD_MERGE)
    {
        if(flag == MODELOBJECT_LOAD_RESET)
        {
            if(LW_FAILED(Destroy()))
                goto __ret;
        }

        if(LW_FAILED(_obj_root->InsertChild(0, tree_node->GetChild())))
            goto __ret;

        tree_node->SetChild(0);
        // release root dummy node
        LW_DELETE((lwModelNodeInfo*)tree_node->GetChild());
        tree_node->Release();
    }
    else if(flag == MODELOBJECT_LOAD_MERGE2)
    {
        if(LW_FAILED(_obj_root->InsertChild(0, tree_node)))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}


LW_END

#endif
