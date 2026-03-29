//
#include "stdafx.h"


#include "lwRenderCtrlEmb.h"

LW_BEGIN

lwIRenderCtrlVS* __RenderCtrlVSProcFixedFuntion()
{
    return LW_NEW(lwRenderCtrlVSFixedFunction);
}

LW_RESULT lwInitInternalRenderCtrlVSProc(lwIResourceMgr* mgr)
{
    mgr->RegisterRenderCtrlProc(RENDERCTRL_VS_FIXEDFUNCTION, __RenderCtrlVSProcFixedFuntion);

    return LW_RET_OK;
}

// lwRenderCtrlVSFixedFunction
LW_STD_IMPLEMENTATION(lwRenderCtrlVSFixedFunction);

LW_RESULT lwRenderCtrlVSFixedFunction::Clone(lwIRenderCtrlVS** obj)
{
    this_type* o = LW_NEW(this_type);
    *o = *this;

    *obj = o;

    return LW_RET_OK;
}
LW_RESULT lwRenderCtrlVSFixedFunction::Initialize(lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}

LW_RESULT lwRenderCtrlVSFixedFunction::BeginSet(lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();
    dev_obj->SetTransformWorld(agent->GetGlobalMatrix());
    
    return LW_RET_OK;
}
LW_RESULT lwRenderCtrlVSFixedFunction::EndSet(lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}
LW_RESULT lwRenderCtrlVSFixedFunction::BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();

    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();

    if(anim_agent == 0)
        goto __ret;

    {
        DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
        lwIAnimCtrlObj* animctrl_obj;
        lwAnimCtrlObjTypeInfo type_info;

        for (DWORD i = 0; i < animobj_num; i++)
        {
            animctrl_obj = anim_agent->GetAnimCtrlObj(i);
            animctrl_obj->GetTypeInfo(&type_info);

            BOOL play_type = animctrl_obj->IsPlaying();

            if ((type_info.data[0] == subset) && play_type)
            {
                DWORD anim_type = type_info.type;
                DWORD stage_id = type_info.data[1];

                if (anim_type == ANIM_CTRL_TYPE_TEXUV)
                {
                    lwIAnimCtrlObjTexUV* this_ctrl = (lwIAnimCtrlObjTexUV*)animctrl_obj;

                    lwMatrix44 mat;
                    this_ctrl->GetRTM(&mat);

                    dev_obj->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage_id), &mat);
                    dev_obj->SetTextureStageState(stage_id, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
                }
                // moved into Primitive.Update procedure
                //else if(anim_type == ANIM_CTRL_TYPE_MTLOPACITY)
                //{
                //    lwIAnimCtrlObjMtlOpacity* this_ctrl = (lwIAnimCtrlObjMtlOpacity*)animctrl_obj;

                //    float opacity = 1.0f;
                //    if(LW_SUCCEEDED(this_ctrl->GetRunTimeOpacity(&opacity)))
                //    {
                //        agent->GetMtlTexAgent()->SetOpacity(opacity);
                //    }
                //}
            }
        }
    }
__ret:
    return LW_RET_OK;
}
LW_RESULT lwRenderCtrlVSFixedFunction::EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();

    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();

    if (anim_agent == 0)
        goto __ret;
    {
        DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
        lwIAnimCtrlObj* animctrl_obj;
        lwAnimCtrlObjTypeInfo type_info;

        for (DWORD i = 0; i < animobj_num; i++)
        {
            animctrl_obj = anim_agent->GetAnimCtrlObj(i);
            animctrl_obj->GetTypeInfo(&type_info);

            BOOL play_type = animctrl_obj->IsPlaying();

            if ((type_info.data[0] == subset) && (play_type != PLAY_INVALID))
            {
                DWORD anim_type = type_info.type;
                DWORD stage_id = type_info.data[1];

                if (anim_type == ANIM_CTRL_TYPE_TEXUV)
                {
                    dev_obj->SetTextureStageState(stage_id, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
                }
                //else if(anim_type == ANIM_CTRL_TYPE_TEXIMG)
                //{
                //    lwIAnimCtrlObjTexImg* this_ctrl = (lwIAnimCtrlObjTexImg*)animctrl_obj;
                //    lwITex* this_tex = this_ctrl->GetRunTimeTex();
                //    this_tex->EndSet();
                //}
            }
        }
    }
__ret:
    return LW_RET_OK;
}


LW_END
