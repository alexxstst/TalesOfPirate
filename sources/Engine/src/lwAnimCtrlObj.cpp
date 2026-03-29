//
#include "stdafx.h"
#include "lwAnimCtrlObj.h"
#include "lwPoseCtrl.h"
#include "lwExpObj.h"

LW_BEGIN

// ===================
// lwAnimCtrlObjMat
LW_STD_IMPLEMENTATION(lwAnimCtrlObjMat)

lwAnimCtrlObjMat::lwAnimCtrlObjMat(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _anim_ctrl(0)
{ 
    memset(&_type_info, 0, sizeof(_type_info));
    _type_info.type = THIS_TYPE;
    _type_info.data[0] = LW_INVALID_INDEX;
    _type_info.data[1] = LW_INVALID_INDEX;
    lwPlayPoseInfo_Construct(&_ppi);
    lwMatrix44Identity(&_rtm);
}
lwAnimCtrlObjMat::~lwAnimCtrlObjMat()
{
    LW_IF_RELEASE(_anim_ctrl);
}
LW_RESULT lwAnimCtrlObjMat::Clone(lwIAnimCtrlObjMat** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjMat* o = NULL;

    if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&o, THIS_TYPE)))
        goto __ret;

    o->AttachAnimCtrl(_anim_ctrl);
    // lwAnimCtrl_Ref
    if(_anim_ctrl->GetRegisterID() != LW_INVALID_INDEX)
    {
        _res_mgr->AddRefAnimCtrl(_anim_ctrl, 1);
    }

    if(LW_FAILED(o->PlayPose(&_ppi)))
        goto __ret;

    o->SetTypeInfo(&_type_info);
    
    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;

}
lwIAnimCtrlMatrix* lwAnimCtrlObjMat::AttachAnimCtrl(lwIAnimCtrlMatrix* ctrl) 
{ 
    lwIAnimCtrlMatrix* ret = _anim_ctrl; 
    _anim_ctrl = ctrl;  
    return ret; 
}
lwIAnimCtrlMatrix* lwAnimCtrlObjMat::DetachAnimCtrl()
{
    lwIAnimCtrlMatrix* ret = _anim_ctrl; 
    _anim_ctrl = 0;  
    return ret; 
}

LW_RESULT lwAnimCtrlObjMat::SetTypeInfo(lwAnimCtrlObjTypeInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(info->type != THIS_TYPE)
        goto __ret;

    _type_info = *info;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjMat::GetTypeInfo(lwAnimCtrlObjTypeInfo* info) 
{ 
    *info = _type_info;
    return LW_RET_OK;
}

LW_RESULT lwAnimCtrlObjMat::PlayPose(const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_anim_ctrl == NULL)
        goto __ret;

    if(LW_FAILED(lwPlayPoseSmooth(&_ppi, info, _anim_ctrl->GetPoseCtrl())))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwAnimCtrlObjMat::UpdateAnimCtrl()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->UpdatePose(&_ppi)))
        goto __ret;

    if(LW_FAILED(_anim_ctrl->UpdateAnimData(&_ppi)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjMat::UpdateObject()
{ 
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->GetRTM(&_rtm)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlObjMat::GetRTM(lwMatrix44* mat) 
{ 
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __ret;

    *mat = _rtm;

    ret = LW_RET_OK;
__ret:
    return ret;
}

// lwAnimCtrlObjBone
LW_STD_IMPLEMENTATION(lwAnimCtrlObjBone)

lwAnimCtrlObjBone::lwAnimCtrlObjBone(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _anim_ctrl(0)
{ 
    memset(&_type_info, 0, sizeof(_type_info));
    _type_info.type = THIS_TYPE;
    _type_info.data[0] = LW_INVALID_INDEX;
    _type_info.data[1] = LW_INVALID_INDEX;

    lwPlayPoseInfo_Construct(&_ppi);

    _dummy_rtm_seq = 0;
    _dummy_rtm_num = 0;
    _bone_rtm_seq = 0;
    _bone_rtm_num = 0;
}
lwAnimCtrlObjBone::~lwAnimCtrlObjBone()
{
    LW_IF_RELEASE(_anim_ctrl);

    LW_SAFE_DELETE_A(_dummy_rtm_seq);
    LW_SAFE_DELETE_A(_bone_rtm_seq);
	//LW_SAFE_DELETE_A(mComputedBoneList);
}
LW_RESULT lwAnimCtrlObjBone::Clone(lwIAnimCtrlObjBone** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjBone* o = NULL;

    if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&o, THIS_TYPE)))
        goto __ret;

    o->AttachAnimCtrl(_anim_ctrl);
    // lwAnimCtrl_Ref
    if(_anim_ctrl->GetRegisterID() != LW_INVALID_INDEX)
    {
        _res_mgr->AddRefAnimCtrl(_anim_ctrl, 1);
    }

    if(LW_FAILED(o->PlayPose(&_ppi)))
        goto __ret;

    o->SetTypeInfo(&_type_info);
    
    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}
lwIAnimCtrlBone* lwAnimCtrlObjBone::AttachAnimCtrl(lwIAnimCtrlBone* ctrl) 
{ 
    lwIAnimCtrlBone* ret = _anim_ctrl; 
    _anim_ctrl = ctrl;  
    return ret; 
}
lwIAnimCtrlBone* lwAnimCtrlObjBone::DetachAnimCtrl()
{
    lwIAnimCtrlBone* ret = _anim_ctrl; 
    _anim_ctrl = 0;  
    return ret; 
}

LW_RESULT lwAnimCtrlObjBone::SetTypeInfo(lwAnimCtrlObjTypeInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(info->type != THIS_TYPE)
        goto __ret;

    _type_info = *info;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjBone::GetTypeInfo(lwAnimCtrlObjTypeInfo* info) 
{ 
    *info = _type_info;
    return LW_RET_OK;
}

LW_RESULT lwAnimCtrlObjBone::PlayPose(const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_anim_ctrl == NULL)
        goto __ret;

    if(LW_FAILED(lwPlayPoseSmooth(&_ppi, info, _anim_ctrl->GetPoseCtrl())))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwAnimCtrlObjBone::UpdateAnimCtrl()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_anim_ctrl == NULL || _ppi.type == PLAY_INVALID)
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->UpdatePose(&_ppi)))
        goto __ret;

    if(LW_FAILED(_anim_ctrl->UpdateAnimData(&_ppi)))
        goto __ret;

    if(LW_FAILED(_anim_ctrl->UpdatePoseKeyFrameProc(&_ppi)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlObjBone::UpdateObject(lwIAnimCtrlObjBone* ctrl_obj, lwIMesh* mesh_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    {
        lwAnimCtrlObjBone* o_ctrl_obj = (lwAnimCtrlObjBone*)ctrl_obj;

        DWORD dummy_num = _anim_ctrl->GetDummyNum();
        DWORD bone_num = _anim_ctrl->GetBoneNum();

        if (mesh_obj)
        {
            lwMeshInfo* mesh_info = mesh_obj->GetMeshInfo();

            if (mesh_info->bone_index_num > 0 && bone_num > 0)
            {
                lwMatrix44* bone_rtm = _anim_ctrl->GetBoneRTMSeq();

                if (o_ctrl_obj->_bone_rtm_num == 0)
                {
                    o_ctrl_obj->_bone_rtm_num = mesh_info->bone_index_num;
                    o_ctrl_obj->_bone_rtm_seq = LW_NEW(lwMatrix44[o_ctrl_obj->_bone_rtm_num]);
                }

                for (DWORD i = 0; i < mesh_info->bone_index_num; i++)
                {
                    o_ctrl_obj->_bone_rtm_seq[i] = bone_rtm[mesh_info->bone_index_seq[i]];
                }
            }
        }

        if (dummy_num > 0)
        {
            if (o_ctrl_obj->_dummy_rtm_num == 0)
            {
                o_ctrl_obj->_dummy_rtm_num = dummy_num;
                o_ctrl_obj->_dummy_rtm_seq = LW_NEW(lwIndexMatrix44[o_ctrl_obj->_dummy_rtm_num]);
            }

            lwIndexMatrix44* dummy_rtm = _anim_ctrl->GetDummyRTMSeq();
            memcpy(o_ctrl_obj->_dummy_rtm_seq, dummy_rtm, sizeof(lwIndexMatrix44) * o_ctrl_obj->_dummy_rtm_num);
        }

    }
__addr_ret_ok:
    ret = LW_RET_OK;

    return ret;
}

LW_RESULT lwAnimCtrlObjBone::UpdateHelperObject(lwIHelperObject* helper_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    // IsPlaying()lwAnimCtrlObjBone
    // PlayingHelperObject
    //if(!IsPlaying() || helper_obj == 0)
    if(_dummy_rtm_num == 0 || helper_obj == 0)
        goto __addr_ret_ok;

    {
        lwIBoundingSphere* b = helper_obj->GetBoundingSphere();
        lwIHelperDummy* d = helper_obj->GetHelperDummy();

        if (b)
        {
            lwBoundingSphereInfo* s;
            DWORD num = b->GetObjNum();

            for (DWORD j = 0; j < num; j++)
            {
                s = b->GetDataInfo(j);

                assert(s->id < LW_MAX_BONEDUMMY_NUM);

                lwMatrix44* mat = GetDummyRTM(s->id);
                if (mat)
                {
                    s->mat = *mat;
                }
            }
        }

        if (d)
        {
            lwHelperDummyInfo* di;
            DWORD num = d->GetObjNum();

            for (DWORD i = 0; i < num; i++)
            {
                di = d->GetDataInfo(i);
                // now only support 2
                if (di->parent_type == 2)
                {
                    lwMatrix44* mat = GetDummyRTM(di->parent_id);
                    if (mat)
                    {
                        //lwMatrix44 inv_mat;
                        //lwMatrix44Inverse(&inv_mat, NULL, &di->mat_local);
                        //lwMatrix44Multiply(&di->mat, &inv_mat, mat);
                        lwMatrix44Multiply(&di->mat, &di->mat_local, mat);
                    }
                }
            }
        }
    }
__addr_ret_ok:
    ret = LW_RET_OK;

    return ret;
}

lwMatrix44* lwAnimCtrlObjBone::GetDummyRTM(DWORD id)
{
    for(DWORD i = 0; i < _dummy_rtm_num; i++)
    {
        if(_dummy_rtm_seq[i].id == id)
            return &_dummy_rtm_seq[i].mat;
    }

    return NULL;
}

// lwAnimCtrlObjTexUV
LW_STD_IMPLEMENTATION(lwAnimCtrlObjTexUV)

lwAnimCtrlObjTexUV::lwAnimCtrlObjTexUV(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _anim_ctrl(0)
{
    memset(&_type_info, 0, sizeof(_type_info));
    _type_info.type = THIS_TYPE;
    lwPlayPoseInfo_Construct(&_ppi);
    lwMatrix44Identity(&_rtm);
}
lwAnimCtrlObjTexUV::~lwAnimCtrlObjTexUV()
{
    LW_IF_RELEASE(_anim_ctrl);
}

LW_RESULT lwAnimCtrlObjTexUV::Clone(lwIAnimCtrlObjTexUV** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjTexUV* o = NULL;

    if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&o, THIS_TYPE)))
        goto __ret;

    o->AttachAnimCtrl(_anim_ctrl);
    // lwAnimCtrl_Ref
    if(_anim_ctrl->GetRegisterID() != LW_INVALID_INDEX)
    {
        _res_mgr->AddRefAnimCtrl(_anim_ctrl, 1);
    }

    if(LW_FAILED(o->PlayPose(&_ppi)))
        goto __ret;

    o->SetTypeInfo(&_type_info);
    
    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;

}

lwIAnimCtrlTexUV* lwAnimCtrlObjTexUV::AttachAnimCtrl(lwIAnimCtrlTexUV* ctrl) 
{ 
    lwIAnimCtrlTexUV* ret = _anim_ctrl; 
    _anim_ctrl = ctrl;  
    return ret; 
}
lwIAnimCtrlTexUV* lwAnimCtrlObjTexUV::DetachAnimCtrl()
{
    lwIAnimCtrlTexUV* ret = _anim_ctrl; 
    _anim_ctrl = 0;  
    return ret; 
}

LW_RESULT lwAnimCtrlObjTexUV::SetTypeInfo(lwAnimCtrlObjTypeInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(info->type != THIS_TYPE)
        goto __ret;

    _type_info = *info;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjTexUV::GetTypeInfo(lwAnimCtrlObjTypeInfo* info) 
{ 
    *info = _type_info;
    return LW_RET_OK;
}

LW_RESULT lwAnimCtrlObjTexUV::PlayPose(const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_anim_ctrl == NULL)
        goto __ret;

    if(LW_FAILED(lwPlayPoseSmooth(&_ppi, info, _anim_ctrl->GetPoseCtrl())))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwAnimCtrlObjTexUV::UpdateAnimCtrl()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->UpdatePose(&_ppi)))
        goto __ret;

    if(LW_FAILED(_anim_ctrl->UpdateAnimData(&_ppi)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjTexUV::UpdateObject()
{ 
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->GetRunTimeMatrix(&_rtm)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlObjTexUV::GetRTM(lwMatrix44* mat) 
{ 
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __ret;

    *mat = _rtm;

    ret = LW_RET_OK;
__ret:
    return ret;
}


// lwAnimCtrlObjTexImg
LW_STD_IMPLEMENTATION(lwAnimCtrlObjTexImg)

lwAnimCtrlObjTexImg::lwAnimCtrlObjTexImg(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _anim_ctrl(0)
{
    memset(&_type_info, 0, sizeof(_type_info));
    _type_info.type = THIS_TYPE;
    lwPlayPoseInfo_Construct(&_ppi);
    _rt_tex = 0;
}
lwAnimCtrlObjTexImg::~lwAnimCtrlObjTexImg()
{
    LW_IF_RELEASE(_anim_ctrl);
}
LW_RESULT lwAnimCtrlObjTexImg::Clone(lwIAnimCtrlObjTexImg** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjTexImg* o = NULL;

    if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&o, THIS_TYPE)))
        goto __ret;

    o->AttachAnimCtrl(_anim_ctrl);
    // lwAnimCtrl_Ref
    if(_anim_ctrl->GetRegisterID() != LW_INVALID_INDEX)
    {
        _res_mgr->AddRefAnimCtrl(_anim_ctrl, 1);
    }

    if(LW_FAILED(o->PlayPose(&_ppi)))
        goto __ret;

    o->SetTypeInfo(&_type_info);

    o->_rt_tex = _rt_tex;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;

}
lwIAnimCtrlTexImg* lwAnimCtrlObjTexImg::AttachAnimCtrl(lwIAnimCtrlTexImg* ctrl) 
{ 
    lwIAnimCtrlTexImg* ret = _anim_ctrl; 
    _anim_ctrl = ctrl;  
    return ret; 
}
lwIAnimCtrlTexImg* lwAnimCtrlObjTexImg::DetachAnimCtrl()
{
    lwIAnimCtrlTexImg* ret = _anim_ctrl; 
    _anim_ctrl = 0;  
    return ret; 
}

LW_RESULT lwAnimCtrlObjTexImg::SetTypeInfo(lwAnimCtrlObjTypeInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(info->type != THIS_TYPE)
        goto __ret;

    _type_info = *info;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjTexImg::GetTypeInfo(lwAnimCtrlObjTypeInfo* info) 
{ 
    *info = _type_info;
    return LW_RET_OK;
}

LW_RESULT lwAnimCtrlObjTexImg::PlayPose(const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_anim_ctrl == NULL)
        goto __ret;

    if(LW_FAILED(lwPlayPoseSmooth(&_ppi, info, _anim_ctrl->GetPoseCtrl())))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwAnimCtrlObjTexImg::UpdateAnimCtrl()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->UpdatePose(&_ppi)))
        goto __ret;

    if(LW_FAILED(_anim_ctrl->UpdateAnimData(&_ppi)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjTexImg::UpdateObject()
{ 
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->GetRunTimeTex(&_rt_tex)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlObjTexImg::GetRunTimeTex(lwITex** tex)
{ 
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __ret;

    *tex = _rt_tex;

    ret = LW_RET_OK;
__ret:
    return ret;
}

// lwAnimCtrlObjMtlOpacity
LW_STD_IMPLEMENTATION(lwAnimCtrlObjMtlOpacity)

lwAnimCtrlObjMtlOpacity::lwAnimCtrlObjMtlOpacity(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _anim_ctrl(0)
{
    memset(&_type_info, 0, sizeof(_type_info));
    _type_info.type = THIS_TYPE;
    lwPlayPoseInfo_Construct(&_ppi);
    _rt_opacity = 0;
}
lwAnimCtrlObjMtlOpacity::~lwAnimCtrlObjMtlOpacity()
{
    LW_IF_RELEASE(_anim_ctrl);
}
LW_RESULT lwAnimCtrlObjMtlOpacity::Clone(lwIAnimCtrlObjMtlOpacity** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjMtlOpacity* o = NULL;

    if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&o, THIS_TYPE)))
        goto __ret;

    o->AttachAnimCtrl(_anim_ctrl);
    // lwAnimCtrl_Ref
    if(_anim_ctrl->GetRegisterID() != LW_INVALID_INDEX)
    {
        _res_mgr->AddRefAnimCtrl(_anim_ctrl, 1);
    }

    if(LW_FAILED(o->PlayPose(&_ppi)))
        goto __ret;

    o->SetTypeInfo(&_type_info);

    o->_rt_opacity = _rt_opacity;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;

}
lwIAnimCtrlMtlOpacity* lwAnimCtrlObjMtlOpacity::AttachAnimCtrl(lwIAnimCtrlMtlOpacity* ctrl) 
{ 
    lwIAnimCtrlMtlOpacity* ret = _anim_ctrl; 
    _anim_ctrl = ctrl;  
    return ret; 
}
lwIAnimCtrlMtlOpacity* lwAnimCtrlObjMtlOpacity::DetachAnimCtrl()
{
    lwIAnimCtrlMtlOpacity* ret = _anim_ctrl; 
    _anim_ctrl = 0;  
    return ret; 
}

LW_RESULT lwAnimCtrlObjMtlOpacity::SetTypeInfo(lwAnimCtrlObjTypeInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(info->type != THIS_TYPE)
        goto __ret;

    _type_info = *info;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjMtlOpacity::GetTypeInfo(lwAnimCtrlObjTypeInfo* info) 
{ 
    *info = _type_info;
    return LW_RET_OK;
}

LW_RESULT lwAnimCtrlObjMtlOpacity::PlayPose(const lwPlayPoseInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_anim_ctrl == NULL)
        goto __ret;

    if(LW_FAILED(lwPlayPoseSmooth(&_ppi, info, _anim_ctrl->GetPoseCtrl())))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwAnimCtrlObjMtlOpacity::UpdateAnimCtrl()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->UpdatePose(&_ppi)))
        goto __ret;

    if(LW_FAILED(_anim_ctrl->UpdateAnimData(&_ppi)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwAnimCtrlObjMtlOpacity::UpdateObject()
{ 
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __addr_ret_ok;

    if(LW_FAILED(_anim_ctrl->GetRunTimeOpacity(&_rt_opacity)))
        goto __ret;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlObjMtlOpacity::GetRunTimeOpacity(float* opacity)
{ 
    LW_RESULT ret = LW_RET_FAILED;

    if(!IsPlaying())
        goto __ret;

    *opacity = _rt_opacity;

    ret = LW_RET_OK;
__ret:
    return ret;
}

// lwAnimCtrlAgent
LW_STD_IMPLEMENTATION(lwAnimCtrlAgent)

lwAnimCtrlAgent::lwAnimCtrlAgent(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _obj_seq(0), _obj_num(0)
{
}

lwAnimCtrlAgent::~lwAnimCtrlAgent()
{
    for(DWORD i = 0; i < _obj_num; i++)
    {
        _obj_seq[i]->Release();
    }

    LW_SAFE_DELETE_A(_obj_seq);
}

LW_RESULT lwAnimCtrlAgent::AddAnimCtrlObj(lwIAnimCtrlObj* obj)
{
    if(_obj_num == 0)
    {
        _obj_seq = LW_NEW(lwIAnimCtrlObj*[1]);
    }
    else
    {
        lwIAnimCtrlObj** new_buf = LW_NEW(lwIAnimCtrlObj*[_obj_num+1]);
        memcpy(new_buf, _obj_seq, sizeof(lwIAnimCtrlObj*) * _obj_num);
        LW_DELETE_A(_obj_seq);
        _obj_seq = new_buf;
    }

    _obj_seq[_obj_num++] = obj;

    return LW_RET_OK;
}
lwIAnimCtrlObj* lwAnimCtrlAgent::RemoveAnimCtrlObj(lwAnimCtrlObjTypeInfo* info)
{
    lwIAnimCtrlObj* o;
    lwAnimCtrlObjTypeInfo o_info;
    for(DWORD i = 0; i < _obj_num; i++)
    {
        o = _obj_seq[i];
        o->GetTypeInfo(&o_info);

        if(o_info.type == info->type 
            && o_info.data[0] == info->data[0]
            && o_info.data[1] == info->data[1])
        {
            if(_obj_num == 1)
            {
                LW_DELETE_A(_obj_seq);
                _obj_seq = 0;
            }
            else
            {
                lwIAnimCtrlObj** new_buf = LW_NEW(lwIAnimCtrlObj*[_obj_num-1]);
                memcpy(new_buf, _obj_seq, sizeof(lwIAnimCtrlObj*) * i);
                memcpy(&new_buf[i], &_obj_seq[i+1], sizeof(lwIAnimCtrlObj*) * (_obj_num - i - 1));
                LW_DELETE_A(_obj_seq);
                _obj_seq = new_buf;
            }
            _obj_num -= 1;

            return o;
        }
    }

    return NULL;
}

lwIAnimCtrlObj* lwAnimCtrlAgent::GetAnimCtrlObj(lwAnimCtrlObjTypeInfo* info)
{
    lwIAnimCtrlObj* o;
    lwAnimCtrlObjTypeInfo o_info;
    for(DWORD i = 0; i < _obj_num; i++)
    {
        o = _obj_seq[i];
        o->GetTypeInfo(&o_info);

        if(o_info.type == info->type 
            && o_info.data[0] == info->data[0]
            && o_info.data[1] == info->data[1])
        {
            return o;
        }
    }
    return NULL;
}


LW_RESULT lwAnimCtrlAgent::Update()
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        if(LW_FAILED(_obj_seq[i]->UpdateAnimCtrl()))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwAnimCtrlAgent::Clone(lwIAnimCtrlAgent** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjTypeInfo type_info;
    lwIAnimCtrlAgent* agent = 0;


    lwIAnimCtrlObj* obj = 0;

    if(LW_FAILED(_res_mgr->CreateAnimCtrlAgent(&agent)))
        goto __ret;

    for(DWORD i = 0; i < _obj_num; i++)
    {
        _obj_seq[i]->GetTypeInfo(&type_info);

        switch(type_info.type)
        {
        case ANIM_CTRL_TYPE_MAT:
            ret = ((lwIAnimCtrlObjMat*)_obj_seq[i])->Clone((lwIAnimCtrlObjMat**)&obj);
            break;
        case ANIM_CTRL_TYPE_BONE:
            ret = ((lwIAnimCtrlObjBone*)_obj_seq[i])->Clone((lwIAnimCtrlObjBone**)&obj);
            break;
        case ANIM_CTRL_TYPE_TEXUV:
            ret = ((lwIAnimCtrlObjTexUV*)_obj_seq[i])->Clone((lwIAnimCtrlObjTexUV**)&obj);
            break;
        case ANIM_CTRL_TYPE_TEXIMG:
            ret = ((lwIAnimCtrlObjTexImg*)_obj_seq[i])->Clone((lwIAnimCtrlObjTexImg**)&obj);
            break;
        case ANIM_CTRL_TYPE_MTLOPACITY:
            ret = ((lwIAnimCtrlObjMtlOpacity*)_obj_seq[i])->Clone((lwIAnimCtrlObjMtlOpacity**)&obj);
            break;
        default:
            __asm int 3;
        }

        if(LW_FAILED(ret))
            goto __ret;

        if(LW_FAILED(agent->AddAnimCtrlObj(obj)))
            goto __ret;

        obj = 0;
    }

    *ret_obj = agent;
    agent = 0;

    ret = LW_RET_OK;
__ret:
    if(obj)
        obj->Release();
    if(agent)
        agent->Release();
    return ret;
}

LW_RESULT lwAnimCtrlAgent::ExtractAnimData(lwIAnimDataInfo* data_info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjTypeInfo type_info;

    lwAnimDataInfo* a = (lwAnimDataInfo*)data_info;
    
    for(DWORD i = 0; i < _obj_num; i++)
    {
        lwIAnimCtrlObj* aco = _obj_seq[i];
        aco->GetTypeInfo(&type_info);

        DWORD ctrl_type = type_info.type;
        DWORD subset = type_info.data[0];
        DWORD stage = type_info.data[1];

        switch(ctrl_type)
        {
        case ANIM_CTRL_TYPE_MAT:
            {
                lwIAnimCtrlMatrix* ctrl_obj = (lwIAnimCtrlMatrix*)aco->GetAnimCtrl();
                lwAnimDataMatrix* out_data = LW_NEW(lwAnimDataMatrix);
                if(LW_FAILED(ctrl_obj->ExtractAnimData(out_data)))
                    goto __ret;
                a->anim_mat = out_data;
            }
            break;
        case ANIM_CTRL_TYPE_BONE:
            {
                lwIAnimCtrlBone* ctrl_obj = (lwIAnimCtrlBone*)aco->GetAnimCtrl();
                lwAnimDataBone* out_data = LW_NEW(lwAnimDataBone);
                if(LW_FAILED(ctrl_obj->ExtractAnimData(out_data)))
                    goto __ret;
                a->anim_bone = out_data;
            }
            break;
        case ANIM_CTRL_TYPE_TEXUV:
            {
                lwIAnimCtrlTexUV* ctrl_obj = (lwIAnimCtrlTexUV*)aco->GetAnimCtrl();;  
                lwAnimDataTexUV* out_data = LW_NEW(lwAnimDataTexUV);
                if(LW_FAILED(ctrl_obj->ExtractAnimData(out_data)))
                    goto __ret;
                a->anim_tex[subset][stage] = out_data;
            }
            break;
        case ANIM_CTRL_TYPE_TEXIMG:
            {
                lwIAnimCtrlTexImg* ctrl_obj = (lwIAnimCtrlTexImg*)aco->GetAnimCtrl();           
                lwAnimDataTexImg* out_data = LW_NEW(lwAnimDataTexImg);
                if(LW_FAILED(ctrl_obj->ExtractAnimData(out_data)))
                    goto __ret;
                a->anim_img[subset][stage] = out_data;
            }
            break;
        default:
            goto __ret;
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_END
