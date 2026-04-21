//
#include "stdafx.h"

#include "lwIUtil.h"
#include "lwNodeObject.h"
#include "lwAnimKeySetPRS.h"

LW_BEGIN

LW_RESULT lwResetDevice(lwISysGraphics* sys_graphics, const D3DPRESENT_PARAMETERS* d3dpp)
{
    return 0;
}


LW_RESULT lwLoadTex(lwITex** out, lwIResourceMgr* res_mgr, const char* file, const char* tex_path, D3DFORMAT fmt)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwITex* tex;

    if(LW_FAILED(res_mgr->CreateTex(&tex)))
        goto __ret;

    lwTexInfo tex_info;
    lwTexInfo_Construct(&tex_info);

    tex_info.stage = 0; // default stage 0
    tex_info.type = TEX_TYPE_FILE;
    tex_info.level = D3DX_DEFAULT;
    tex_info.format = fmt;
    tex_info.pool = D3DPOOL_DEFAULT;
    _tcscpy(tex_info.file_name, file);

    if(LW_FAILED(tex->LoadTexInfo(&tex_info, tex_path)))
        goto __ret;

    if(LW_FAILED(tex->LoadVideoMemory()))
        goto __ret;

    *out = tex;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwLoadTex(lwITex** out, lwIResourceMgr* res_mgr, const lwTexInfo* info)
{
    return lwLoadTex(out, res_mgr, info, nullptr);
}

LW_RESULT lwLoadTex(lwITex** out, lwIResourceMgr* res_mgr, const lwTexInfo* info, void* user_data)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwITex* tex;

    if(LW_FAILED(res_mgr->CreateTex(&tex)))
        goto __ret;

    // Для TEX_TYPE_DATA пользовательский буфер пробрасывается через отдельный
    // API (а не через поле `lwTexInfo::data`, которое убрано из-за
    // несовместимости размера `void*` между x86 и x64).
    if(user_data)
        tex->SetUserData(user_data);

    if(LW_FAILED(tex->LoadTexInfo(info, 0)))
        goto __ret;

    if(LW_FAILED(tex->LoadVideoMemory()))
        goto __ret;

    *out = tex;

    ret = LW_RET_OK;
__ret:
    return ret;
}

void lwPhysiqueSetMaterial(lwIPhysique* phy, const lwMaterial* mtl)
{
    lwIPrimitive* p;

    for(DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++)
    {
        if((p = phy->GetPrimitive(i)) == 0)
            continue;

        p->SetMaterial(mtl);
    }
}

lwIAnimCtrl* lwItemGetAnimCtrl(lwIItem* item, DWORD ctrl_type)
{
    lwIAnimCtrl* ret = 0;

    if(item == 0)
        goto __ret;

    {
        lwIPrimitive* p = item->GetPrimitive();
        if (p == 0)
            goto __ret;

        {
            lwIAnimCtrlAgent* agent = p->GetAnimAgent();
            if (agent == 0)
                goto __ret;
            {
                lwAnimCtrlObjTypeInfo type_info;
                type_info.type = ctrl_type;
                type_info.data[0] = LW_INVALID_INDEX;
                type_info.data[1] = LW_INVALID_INDEX;

                lwIAnimCtrlObj* ctrl_obj = agent->GetAnimCtrlObj(&type_info);
                if (ctrl_obj == 0)
                    goto __ret;

                ret = ctrl_obj->GetAnimCtrl();
            }
        }
    }
__ret:
    return ret;
}

lwPlayPoseInfo* lwItemGetPlayPoseInfo(lwIItem* item, DWORD ctrl_type)
{
	lwPlayPoseInfo* ret = 0;

	if (item == 0)
		goto __ret;
	{
		lwIPrimitive* p = item->GetPrimitive();
		if (p == 0)
			goto __ret;

		lwIAnimCtrlAgent* agent = p->GetAnimAgent();
		if (agent == 0)
			goto __ret;
		{
			lwAnimCtrlObjTypeInfo type_info;
			type_info.type = ctrl_type;
			type_info.data[0] = LW_INVALID_INDEX;
			type_info.data[1] = LW_INVALID_INDEX;

			lwIAnimCtrlObj* ctrl_obj = agent->GetAnimCtrlObj(&type_info);
			if (ctrl_obj == 0)
				goto __ret;

			ret = ctrl_obj->GetPlayPoseInfo();
		}
	}
__ret:
    return ret;
}


LW_RESULT lwPrimitiveSetRenderCtrl(lwIPrimitive* p, DWORD ctrl_type)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIRenderCtrlAgent* render_agent = p->GetRenderCtrlAgent();
    if(render_agent == 0)
        goto __ret;

    if(LW_FAILED(render_agent->SetRenderCtrl(ctrl_type)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwPrimitiveSetVertexShader(lwIPrimitive* p, DWORD shader_type)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIRenderCtrlAgent* render_agent = p->GetRenderCtrlAgent();
    if(render_agent == 0)
        goto __ret;

    render_agent->SetVertexShader(shader_type);

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwPrimitiveGetObjHeight(lwIPrimitive* p, float* out_height)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIHelperObject* h_obj = p->GetHelperObject();
    if(h_obj == 0)
        goto __ret;

    //lwIBoundingSphere* bs = h_obj->GetBoundingSphere();
    //if(bs)
    //{
    //    lwBoundingSphereInfo* bsi = bs->GetObjInfo(0);        
    //    bsi->sphere.c
    //}
    {
        lwIBoundingBox* bb = h_obj->GetBoundingBox();
        if (bb)
        {
            lwVector3 pos;
            lwMatrix44 mat;
            lwBoundingBoxInfo* bbi = bb->GetDataInfo(0);
            pos = bbi->box.c;
            pos.z += bbi->box.r.z;

            lwMatrix44Multiply(&mat, &bbi->mat, p->GetMatrixGlobal());
            lwVec3Mat44Mul(&pos, &mat);
            *out_height = pos.z;
        }
        else
        {
            goto __ret;
        }

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}

// ======= anim proc
typedef LW_RESULT (*lwTexUVAnimProc)(lwIAnimCtrlTexUV* ctrl_texuv);

LW_RESULT lwTexUVAnim0(lwIAnimCtrlTexUV* ctrl_texuv)
{
    lwIAnimKeySetPRS* keyset_prs;
    keyset_prs = LW_NEW(lwAnimKeySetPRS2);

    // position
    {
        lwKeyVector3 buf[2];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
        buf[1].key = 359;
        buf[1].slerp_type = AKST_LINEAR;
        buf[1].data = lwVector3(1.0f, 1.0f, 0.0f);

        keyset_prs->AddKeyPosition(0, buf, 2);
    }

    ctrl_texuv->SetAnimKeySetPRS(keyset_prs);

    //const int t = 120;
    //lwMatrix44 uvmat_seq[t];
    //float u = 0.0f;
    //float v = 0.0f;
    //float x = 1.0f / t;
    //for( DWORD i = 0; i < t; i++ )
    //{
    //    lwMatrix44Identity(&uvmat_seq[i]);
    //    uvmat_seq[ i ]._31 = u;
    //    uvmat_seq[ i ]._32 = v;
    //    u += x;
    //    v += x;
    //};
    //ctrl_texuv->LoadData(uvmat_seq, t);

    return LW_RET_OK;
}

LW_RESULT lwTexUVAnim1(lwIAnimCtrlTexUV* ctrl_texuv)
{
    lwIAnimKeySetPRS* keyset_prs;
    keyset_prs = LW_NEW(lwAnimKeySetPRS2);

    // rotation
    {
        lwVector3 axis(0.0f, 0.0f, 1.0f);

        lwKeyQuaternion buf[3];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[0].data, &axis, 0.0f);
        buf[1].key = 60;
        buf[1].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[1].data, &axis, LW_PI);    
        buf[2].key = 119;
        buf[2].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[2].data, &axis, LW_2_PI);    

        keyset_prs->AddKeyRotation(0, buf, 3);
    }

    ctrl_texuv->SetAnimKeySetPRS(keyset_prs);

    //const int t = 120;
    //lwMatrix44 uvmat_seq[t];
    //float u = 0.0f;
    //float v = 0.0f;
    //float x = 1.0f / t;
    //float y = LW_2_PI / t;

    //for( DWORD i = 0; i < t; i++ )
    //{
    //    lwMatrix44Identity(&uvmat_seq[i]);
    //    uvmat_seq[i]._11 = cosf(u);
    //    uvmat_seq[i]._12 = -sinf(v);
    //    uvmat_seq[i]._21 = sinf(u);
    //    uvmat_seq[i]._22 = cosf(v);

    //    u += y;
    //    v += y;
    //};

    //ctrl_texuv->LoadData(uvmat_seq, t);

    return LW_RET_OK;
}
LW_RESULT lwTexUVAnim2(lwIAnimCtrlTexUV* ctrl_texuv)
{
    lwIAnimKeySetPRS* keyset_prs;
    keyset_prs = LW_NEW(lwAnimKeySetPRS2);

    // position
    {
        lwKeyVector3 buf[2];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
        buf[1].key = 359;
        buf[1].slerp_type = AKST_LINEAR;
        buf[1].data = lwVector3(1.0f, 1.0f, 0.0f);

        keyset_prs->AddKeyPosition(0, buf, 2);
    }

    // rotation
    {
        lwVector3 axis(0.0f, 0.0f, 1.0f);

        lwKeyQuaternion buf[3];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[0].data, &axis, 0.0f);
        buf[1].key = 180;
        buf[1].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[1].data, &axis, LW_PI);    
        buf[2].key = 359;
        buf[2].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[2].data, &axis, LW_2_PI);    

        keyset_prs->AddKeyRotation(0, buf, 3);
    }

    ctrl_texuv->SetAnimKeySetPRS(keyset_prs);

    //const int t = 120;
    //lwMatrix44 uvmat_seq[t];
    //float u = 0.0f;
    //float v = 0.0f;
    //float m = 0.0f;
    //float n = 0.0f;
    //float x = LW_2_PI / t;
    //for( DWORD i = 0; i < t; i++ )
    //{
    //    lwMatrix44Identity(&uvmat_seq[i]);
    //    uvmat_seq[i]._31 = m;
    //    uvmat_seq[i]._32 = n;
    //    uvmat_seq[i]._11 = cosf(u);
    //    uvmat_seq[i]._12 = -sinf(v);
    //    uvmat_seq[i]._21 = sinf(u);
    //    uvmat_seq[i]._22 = cosf(v);
    //    m += x;
    //    n += x;
    //    u += x;
    //    v += x;
    //};
    //ctrl_texuv->LoadData(uvmat_seq, t);

    return LW_RET_OK;
}
LW_RESULT lwTexUVAnim3(lwIAnimCtrlTexUV* ctrl_texuv)
{
    lwIAnimKeySetPRS* keyset_prs;
    keyset_prs = LW_NEW(lwAnimKeySetPRS2);

    // rotation
    lwVector3 axis(0.0f, 0.0f, 1.0f);

    lwKeyQuaternion buf[3];
    buf[0].key = 0;
    buf[0].slerp_type = AKST_LINEAR;
    lwQuaternionRotationAxis(&buf[0].data, &axis, 0.0f);
    buf[1].key = 360;
    buf[1].slerp_type = AKST_LINEAR;
    lwQuaternionRotationAxis(&buf[1].data, &axis, LW_PI);    
    buf[2].key = 719;
    buf[2].slerp_type = AKST_LINEAR;
    lwQuaternionRotationAxis(&buf[2].data, &axis, LW_2_PI);    

    keyset_prs->AddKeyRotation(0, buf, 3);
    ctrl_texuv->SetAnimKeySetPRS(keyset_prs);

    //const int t = 360;
    //lwMatrix44 uvmat_seq[t];
    //float u = 0.0f;
    //float v = 0.0f;
    //float x = 1.0f / t;
    //float y = LW_2_PI / t;

    //for( DWORD i = 0; i < t; i++ )
    //{
    //    lwMatrix44Identity(&uvmat_seq[i]);
    //    uvmat_seq[i]._11 = cosf(u);
    //    uvmat_seq[i]._12 = -sinf(v);
    //    uvmat_seq[i]._21 = sinf(u);
    //    uvmat_seq[i]._22 = cosf(v);

    //    u += y;
    //    v += y;
    //};

    //ctrl_texuv->LoadData(uvmat_seq, t);

    return LW_RET_OK;
}

LW_RESULT lwTexUVAnim4(lwIAnimCtrlTexUV* ctrl_texuv)
{
    lwIAnimKeySetPRS* keyset_prs;
    keyset_prs = LW_NEW(lwAnimKeySetPRS2);

    lwKeyVector3 buf[2];
    buf[0].key = 0;
    buf[0].slerp_type = AKST_LINEAR;
    buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
    buf[1].key = 119;
    buf[1].slerp_type = AKST_LINEAR;
    buf[1].data = lwVector3(1.0f, 1.0f, 0.0f);

    keyset_prs->AddKeyPosition(0, buf, 2);
    ctrl_texuv->SetAnimKeySetPRS(keyset_prs);

    //const int t = 360;
    //lwMatrix44 uvmat_seq[t];
    //float u = 0.0f;
    //float v = 0.0f;
    //float x = 1.0f / t;
    //for( DWORD i = 0; i < t; i++ )
    //{
    //    lwMatrix44Identity(&uvmat_seq[i]);
    //    uvmat_seq[ i ]._31 = u;
    //    uvmat_seq[ i ]._32 = v;
    //    u += x;
    //    v += x;
    //};
    //ctrl_texuv->LoadData(uvmat_seq, t);

    return LW_RET_OK;
}

static const int __texuv_animproc_num = 5;
static lwTexUVAnimProc __texuv_animproc_seq[__texuv_animproc_num] =
{
    lwTexUVAnim0,
    lwTexUVAnim1,
    lwTexUVAnim2,
    lwTexUVAnim3,
    lwTexUVAnim4,
};

LW_RESULT lwPrimitiveTexLit(lwIPrimitive* p, const char* file, const char* tex_path, DWORD color_op, DWORD anim_type)
{
    // ----begin----
    lwITex* tex;
    lwIResourceMgr* res_mgr = p->GetResourceMgr();
    res_mgr->CreateTex(&tex);
    lwTexInfo tex_info;
    lwTexInfo_Construct(&tex_info);

    tex_info.stage = 1;
    tex_info.type = TEX_TYPE_FILE;
    tex_info.level = D3DX_DEFAULT;
    tex_info.format = D3DFMT_A8R8G8B8;
    tex_info.pool = D3DPOOL_DEFAULT;
    tex_info.colorkey_type = COLORKEY_TYPE_NONE;

    //_tcscpy( tex_info.file_name, "boxlight2.tga" );
    //_tcscpy( tex_info.file_name, "shlight.tga" );
    //_tcscpy( tex_info.file_name, "multi009.tga" );
    //@@_tcscpy( tex_info.file_name, "ocean_h.08.bmp" );
    //_tcscpy( tex_info.file_name, "water_gsc012.BMP" );
    _tcscpy( tex_info.file_name, file );

    RSA_VALUE(&tex_info.tss_set[0], D3DTSS_COLOROP, color_op);
    RSA_VALUE(&tex_info.tss_set[1], D3DTSS_COLORARG1, D3DTA_TEXTURE );
    RSA_VALUE(&tex_info.tss_set[2], D3DTSS_COLORARG2, D3DTA_CURRENT );
    RSA_VALUE(&tex_info.tss_set[3], D3DTSS_ALPHAOP, D3DTOP_DISABLE );
    RSA_VALUE(&tex_info.tss_set[4], D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    RSA_VALUE(&tex_info.tss_set[5], D3DTSS_TEXCOORDINDEX, 0 );

    tex->LoadTexInfo(&tex_info, tex_path);

    lwIMtlTexAgent* mtltex_agent = p->GetMtlTexAgent(0);
    //mtltex_agent->LoadTextureStage(&tex_info,
    lwITex* ret_tex = 0;
    mtltex_agent->SetTex(tex_info.stage, tex, &ret_tex);
    LW_SAFE_RELEASE(ret_tex);

    // anim_ctrl
    lwIAnimCtrlObjTexUV* ctrl_obj_texuv;
    lwIAnimCtrlTexUV* ctrl_texuv;
    res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj_texuv, ANIM_CTRL_TYPE_TEXUV);
    res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_texuv, ANIM_CTRL_TYPE_TEXUV);

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_TEXUV;
    type_info.data[0] = 0;
    type_info.data[1] = 1;
    ctrl_obj_texuv->SetTypeInfo(&type_info);

    ctrl_obj_texuv->AttachAnimCtrl(ctrl_texuv);

    lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
    if(anim_agent == 0)
    {
        res_mgr->CreateAnimCtrlAgent(&anim_agent);
        p->SetAnimCtrlAgent(anim_agent);
    }

    anim_agent->AddAnimCtrlObj(ctrl_obj_texuv);


    __texuv_animproc_seq[anim_type](ctrl_texuv);

    lwPlayPoseInfo ppi;
    ppi.bit_mask = PPI_MASK_DEFAULT;
    ppi.type = PLAY_LOOP;
    ppi.velocity = 1.0f;
    ppi.pose = 0;
    ppi.frame = 0;
    ppi.data = 0;
    ctrl_obj_texuv->PlayPose(&ppi);

    return LW_RET_OK;
}

LW_RESULT lwPrimitiveTexLitC(lwIPrimitive* p, const char* file, const char* tex_path, DWORD anim_type)
{
    // ----begin----
    lwITex* tex;
    lwIResourceMgr* res_mgr = p->GetResourceMgr();
    res_mgr->CreateTex(&tex);
    lwTexInfo tex_info;
    lwTexInfo_Construct(&tex_info);
    tex_info.stage = 1;
    tex_info.type = TEX_TYPE_FILE;
    tex_info.level = D3DX_DEFAULT;
    tex_info.format = D3DFMT_A8R8G8B8;
    tex_info.pool = D3DPOOL_DEFAULT;
    tex_info.colorkey_type = COLORKEY_TYPE_NONE;

    //_tcscpy( tex_info.file_name, "boxlight2.tga" );
    //_tcscpy( tex_info.file_name, "shlight.tga" );
    //_tcscpy( tex_info.file_name, "multi009.tga" );
    //@@_tcscpy( tex_info.file_name, "ocean_h.08.bmp" );
    //_tcscpy( tex_info.file_name, "water_gsc012.BMP" );
    _tcscpy( tex_info.file_name, file );

    RSA_VALUE(&tex_info.tss_set[0], D3DTSS_COLOROP, D3DTOP_MODULATE2X    );
    RSA_VALUE(&tex_info.tss_set[1], D3DTSS_COLORARG1, D3DTA_TEXTURE );
    RSA_VALUE(&tex_info.tss_set[2], D3DTSS_COLORARG2, D3DTA_CURRENT );
    RSA_VALUE(&tex_info.tss_set[3], D3DTSS_ALPHAOP, D3DTOP_DISABLE );
    RSA_VALUE(&tex_info.tss_set[4], D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

    tex->LoadTexInfo(&tex_info, tex_path);

    lwIMtlTexAgent* mtltex_agent = p->GetMtlTexAgent(0);
    //mtltex_agent->LoadTextureStage(&tex_info,
    lwITex* ret_tex = 0;
    mtltex_agent->SetTex(tex_info.stage, tex, &ret_tex);
    LW_SAFE_RELEASE(ret_tex);

    // anim_ctrl
    lwIAnimCtrlObjTexUV* ctrl_obj_texuv;
    lwIAnimCtrlTexUV* ctrl_texuv;
    res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj_texuv, ANIM_CTRL_TYPE_TEXUV);
    res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_texuv, ANIM_CTRL_TYPE_TEXUV);

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_TEXUV;
    type_info.data[0] = 0;
    type_info.data[1] = 1;
    ctrl_obj_texuv->SetTypeInfo(&type_info);

    ctrl_obj_texuv->AttachAnimCtrl(ctrl_texuv);


    lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
    if(anim_agent == 0)
    {
        res_mgr->CreateAnimCtrlAgent(&anim_agent);
        p->SetAnimCtrlAgent(anim_agent);
    }

    anim_agent->AddAnimCtrlObj(ctrl_obj_texuv);

    __texuv_animproc_seq[anim_type](ctrl_texuv);

    lwPlayPoseInfo ppi;
    ppi.bit_mask = PPI_MASK_DEFAULT;
    ppi.type = PLAY_LOOP;
    ppi.velocity = 1.0f; // Mdr.st
    ppi.pose = 0;
    ppi.frame = 0;
    ppi.data = 0;
    ctrl_obj_texuv->PlayPose(&ppi);

    return LW_RET_OK;
}

#if 1
LW_RESULT lwPrimitiveTexUnLitA(lwIPrimitive* p)
{
    lwIMtlTexAgent* mtltex_agent = p->GetMtlTexAgent(0);

    for(DWORD i = 1; i < 4; i++)
    {
        if(LW_FAILED(mtltex_agent->DestroyTextureStage(i)))
            return LW_RET_FAILED;
    }

    return LW_RET_OK;
}

LW_RESULT lwPrimitiveTexLitA(lwIPrimitive* p, const char* alpha_file, const char* tex_file, const char* tex_path, DWORD anim_type)
{
    // ----begin----
    lwITex* tex;
    lwITex* ret_tex = 0;
    lwIMtlTexAgent* mtltex_agent = 0;
    lwIResourceMgr* res_mgr = p->GetResourceMgr();
    lwTexInfo tex_info;

    lwTexInfo_Construct(&tex_info);
    tex_info.stage = 1;
    tex_info.type = TEX_TYPE_FILE;
    tex_info.level = D3DX_DEFAULT;
    tex_info.format = D3DFMT_A8R8G8B8;
    tex_info.pool = D3DPOOL_DEFAULT;


    tex_info.colorkey_type = COLORKEY_TYPE_NONE;

    _tcscpy( tex_info.file_name, alpha_file );

    RSA_VALUE(&tex_info.tss_set[0], D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
    RSA_VALUE(&tex_info.tss_set[1], D3DTSS_COLORARG1, D3DTA_CURRENT );
    RSA_VALUE(&tex_info.tss_set[2], D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    RSA_VALUE(&tex_info.tss_set[3], D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    RSA_VALUE(&tex_info.tss_set[4], D3DTSS_ALPHAARG2, D3DTA_CURRENT );
    
    res_mgr->CreateTex(&tex);
    tex->LoadTexInfo(&tex_info, tex_path);

    mtltex_agent = p->GetMtlTexAgent(0);
    //mtltex_agent->LoadTextureStage(&tex_info,

    mtltex_agent->SetTex(tex_info.stage, tex, &ret_tex);
    LW_SAFE_RELEASE(ret_tex);



    lwTexInfo_Construct(&tex_info);

    tex_info.stage = 2;
    tex_info.type = TEX_TYPE_FILE;
    tex_info.level = D3DX_DEFAULT;
    tex_info.format = D3DFMT_A8R8G8B8;
    tex_info.pool = D3DPOOL_DEFAULT;
    tex_info.colorkey_type = COLORKEY_TYPE_NONE;

    _tcscpy( tex_info.file_name, tex_file );

    RSA_VALUE(&tex_info.tss_set[0], D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);//D3DTOP_MODULATEINVALPHA_ADDCOLOR);//D3DTOP_MODULATEALPHA_ADDCOLOR );
    RSA_VALUE(&tex_info.tss_set[1], D3DTSS_COLORARG2, D3DTA_TEXTURE );
    RSA_VALUE(&tex_info.tss_set[2], D3DTSS_COLORARG1, D3DTA_CURRENT );
    RSA_VALUE(&tex_info.tss_set[3], D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
    RSA_VALUE(&tex_info.tss_set[4], D3DTSS_ALPHAARG1, D3DTA_CURRENT );

    res_mgr->CreateTex(&tex);
    tex->LoadTexInfo(&tex_info, tex_path);

    mtltex_agent = p->GetMtlTexAgent(0);
    //mtltex_agent->LoadTextureStage(&tex_info,
    mtltex_agent->SetTex(tex_info.stage, tex, &ret_tex);
    LW_SAFE_RELEASE(ret_tex);

    // stage 3
    lwTexInfo_Construct(&tex_info);

    lwITex* tex_0 = mtltex_agent->GetTex(0);
    tex_0->GetTexInfo(&tex_info);
    tex_info.stage = 3;

    RSA_VALUE(&tex_info.tss_set[0], D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    RSA_VALUE(&tex_info.tss_set[1], D3DTSS_COLORARG1, D3DTA_CURRENT );
    RSA_VALUE(&tex_info.tss_set[2], D3DTSS_ALPHAOP, D3DTOP_MODULATE );
    RSA_VALUE(&tex_info.tss_set[3], D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    RSA_VALUE(&tex_info.tss_set[4], D3DTSS_ALPHAARG2, D3DTA_TFACTOR );

    res_mgr->CreateTex(&tex);
    tex->LoadTexInfo(&tex_info, NULL);
    mtltex_agent->SetTex(tex_info.stage, tex, &ret_tex);
    LW_SAFE_RELEASE(ret_tex);

    // anim ctrl
    lwIAnimCtrlObjTexUV* ctrl_obj_texuv;
    lwIAnimCtrlTexUV* ctrl_texuv;
    res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj_texuv, ANIM_CTRL_TYPE_TEXUV);
    res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_texuv, ANIM_CTRL_TYPE_TEXUV);

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_TEXUV;
    type_info.data[0] = 0;
    type_info.data[1] = 2;
    ctrl_obj_texuv->SetTypeInfo(&type_info);

    ctrl_obj_texuv->AttachAnimCtrl(ctrl_texuv);

    lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
    if(anim_agent == 0)
    {
        res_mgr->CreateAnimCtrlAgent(&anim_agent);
        p->SetAnimCtrlAgent(anim_agent);
    }

    anim_agent->AddAnimCtrlObj(ctrl_obj_texuv);

    __texuv_animproc_seq[anim_type](ctrl_texuv);

    lwPlayPoseInfo ppi;
    ppi.bit_mask = PPI_MASK_DEFAULT;
    ppi.type = PLAY_LOOP;
    ppi.velocity = 1.0f;
    ppi.pose = 0;
    ppi.frame = 0;
    ppi.data = 0;
    ctrl_obj_texuv->PlayPose(&ppi);

    return LW_RET_OK;
}
#endif

#if 1
LW_RESULT lwPrimitiveTexLitA(lwIPrimitive* p, const char* tex_file, const char* tex_path, DWORD anim_type)
{
    // ----begin----
    lwITex* tex;
    lwITex* ret_tex = 0;
    lwIMtlTexAgent* mtltex_agent = 0;
    lwIResourceMgr* res_mgr = p->GetResourceMgr();
    lwTexInfo tex_info;

    lwTexInfo_Construct(&tex_info);

    tex_info.stage = 1;
    tex_info.type = TEX_TYPE_FILE;
    tex_info.level = D3DX_DEFAULT;
    tex_info.format = D3DFMT_A8R8G8B8;
    tex_info.pool = D3DPOOL_DEFAULT;
    tex_info.colorkey_type = COLORKEY_TYPE_NONE;

    _tcscpy( tex_info.file_name, tex_file );

    RSA_VALUE(&tex_info.tss_set[0], D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR );
    RSA_VALUE(&tex_info.tss_set[1], D3DTSS_COLORARG1, D3DTA_CURRENT );
    RSA_VALUE(&tex_info.tss_set[2], D3DTSS_COLORARG2, D3DTA_TEXTURE );
    RSA_VALUE(&tex_info.tss_set[3], D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
    RSA_VALUE(&tex_info.tss_set[4], D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );

    
    res_mgr->CreateTex(&tex);
    tex->LoadTexInfo(&tex_info, tex_path);

    mtltex_agent = p->GetMtlTexAgent(0);
    //mtltex_agent->LoadTextureStage(&tex_info,

    mtltex_agent->SetTex(tex_info.stage, tex, &ret_tex);
    LW_SAFE_RELEASE(ret_tex);

    // anim_ctrl
    lwIAnimCtrlObjTexUV* ctrl_obj_texuv;
    lwIAnimCtrlTexUV* ctrl_texuv;
    res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj_texuv, ANIM_CTRL_TYPE_TEXUV);
    res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_texuv, ANIM_CTRL_TYPE_TEXUV);

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_TEXUV;
    type_info.data[0] = 0;
    type_info.data[1] = 1;
    ctrl_obj_texuv->SetTypeInfo(&type_info);

    ctrl_obj_texuv->AttachAnimCtrl(ctrl_texuv);

    lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
    if(anim_agent == 0)
    {
        res_mgr->CreateAnimCtrlAgent(&anim_agent);
        p->SetAnimCtrlAgent(anim_agent);
    }

    anim_agent->AddAnimCtrlObj(ctrl_obj_texuv);


    __texuv_animproc_seq[anim_type](ctrl_texuv);

    lwPlayPoseInfo ppi;
    ppi.bit_mask = PPI_MASK_DEFAULT;
    ppi.type = PLAY_LOOP;
    ppi.velocity = 1.0f;
    ppi.pose = 0;
    ppi.frame = 0;
    ppi.data = 0;
    ctrl_obj_texuv->PlayPose(&ppi);

    return LW_RET_OK;
}
#endif

lwPoseInfo* lwAnimCtrlAgentGetPoseInfo(lwIAnimCtrlAgent* agent, DWORD subset, DWORD stage, DWORD type, DWORD id)
{
    lwPoseInfo* ret = 0;

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = type;
    type_info.data[0] = subset;
    type_info.data[1] = stage;

    lwIAnimCtrlObj* ctrl_obj = agent->GetAnimCtrlObj(&type_info);
    if(ctrl_obj == 0)
        goto __ret;

    {
        lwIAnimCtrl* ctrl = ctrl_obj->GetAnimCtrl();
        if (ctrl == 0)
            goto __ret;

        lwIPoseCtrl* c = ctrl->GetPoseCtrl();
        if (c == 0)
            goto __ret;

        ret = c->GetPoseInfo(id);
    }
__ret:
    return ret;
}

static DWORD __tree_node_release_proc(lwITreeNode* node, void* param)
{
    LW_RELEASE(node);
    return TREENODE_PROC_RET_CONTINUE;
}

void lwReleaseTreeNodeList(lwITreeNode* node)
{
    node->EnumTree(__tree_node_release_proc, 0, TREENODE_PROC_POSTORDER);
}

// lwNodeObject
LW_RESULT lwINodeObjectA::PlayDefaultPose(lwINodeObject* obj)
{
    return lwNodeObject_PlayDefaultPose(obj);
}

LW_END
