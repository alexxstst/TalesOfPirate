//

#include "stdafx.h"
#include "lwxRenderCtrlVS.h"

LW_BEGIN

lwIRenderCtrlVS* __RenderCtrlVSProcVSVertexBlend_dx8()
{
    return LW_NEW(lwxRenderCtrlVSVertexBlend_dx8);
}
#if(defined LW_USE_DX9)
lwIRenderCtrlVS* __RenderCtrlVSProcVSVertexBlend_dx9()
{
    return LW_NEW(lwxRenderCtrlVSVertexBlend);
}
#endif

LW_RESULT lwInitUserRenderCtrlVSProc(lwIResourceMgr* mgr)
{
    mgr->RegisterRenderCtrlProc(RENDERCTRL_VS_VERTEXBLEND, __RenderCtrlVSProcVSVertexBlend_dx8);
#if(defined LW_USE_DX9)
    mgr->RegisterRenderCtrlProc(RENDERCTRL_VS_VERTEXBLEND_DX9, __RenderCtrlVSProcVSVertexBlend_dx9);
#endif
    return LW_RET_OK;
}

// lwxRenderCtrlVSVertexBlend_dx8
LW_STD_IMPLEMENTATION(lwxRenderCtrlVSVertexBlend_dx8);

lwxRenderCtrlVSVertexBlend_dx8::lwxRenderCtrlVSVertexBlend_dx8()
: mPixelShaderReload( false ),
  mPixelShader( -1 )
{
}


LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::Clone(lwIRenderCtrlVS** obj)
{
    this_type* o = LW_NEW(this_type);
    *o = *this;

    *obj = o;

    return LW_RET_OK;
}

LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::Initialize(lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}

LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::BeginSet(lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    // ===== declaraes antecipadas (evita warnings com goto) =====
    lwIResourceMgr*              res_mgr   = 0;
    lwIDeviceObject*             dev_obj   = 0;
    lwMatrix44*                  mat_global= 0;
    const lwMatrix44*            pViewProj = 0;
    lwMatrix44                   mat;
    lwVector4                    const_base(1.0f, 0.0f, 0.0f, 765.01f);
    lwVector4                    light_dir(0.0f, 0.0f, 0.0f, 0.0f);
    DWORD                        rs_amb = 0;

    lwIAnimCtrlAgent*            anim_agent = 0;
    DWORD                        animobj_num = 0;
    lwIAnimCtrlObj*              animctrl_obj = 0;
    lwAnimCtrlObjTypeInfo        type_info;

    // DX9: deixar declaradas antes dos gotos
#if defined(LW_USE_DX9)
    IDirect3DVertexShaderX*      vs   = 0;
    IDirect3DVertexDeclarationX* decl = 0;
#endif

    // ===== validaes bsicas =====
    if (!agent)
        goto __ret;

    res_mgr = agent->GetResourceMgr();
    if (!res_mgr)
        goto __ret;

    dev_obj = res_mgr->GetDeviceObject();
    if (!dev_obj)
        goto __ret;

    // ===== FOG: salvar e desabilitar =====
    dev_obj->GetRenderState(D3DRS_FOGENABLE, &_rs_fog);
    if (_rs_fog)
        dev_obj->SetRenderState(D3DRS_FOGENABLE, FALSE);

    // ===== matrizes =====
    mat_global = agent->GetGlobalMatrix();
    if (!mat_global)
        goto __ret;

    pViewProj = dev_obj->GetMatViewProj();
    if (!pViewProj)
        goto __ret;

    mat = *pViewProj;
    lwMatrix44Multiply(&mat, mat_global, &mat);
    lwMatrix44Transpose(&mat, &mat);

    // ===== ambiente / luz =====
    dev_obj->GetRenderState(D3DRS_AMBIENT, &rs_amb);
    _rs_amb.a = LW_ARGB_A(rs_amb);
    _rs_amb.r = LW_ARGB_R(rs_amb);
    _rs_amb.g = LW_ARGB_G(rs_amb);
    _rs_amb.b = LW_ARGB_B(rs_amb);

    dev_obj->GetLight(0, &_lgt);
    dev_obj->GetRenderState(D3DRS_LIGHTING, &_rs_lgt);
    dev_obj->GetLightEnable(0, &_lgt_enable);

    if (_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        *(lwVector3*)&light_dir = *(lwVector3*)&_lgt.Direction;
        light_dir.x = -light_dir.x;
        light_dir.y = -light_dir.y;
        light_dir.z = -light_dir.z;

        lwMatrix44 mat_light;
        lwMatrix44InverseNoScaleFactor(&mat_light, mat_global);
        lwVec3Mat44MulNormal((lwVector3*)&light_dir, &mat_light);
    }

    // ===== constantes de VS =====
    dev_obj->SetVertexShaderConstantF(VS_CONST_REG_BASE,      (float*)&const_base, 1);
    dev_obj->SetVertexShaderConstantF(VS_CONST_REG_VIEWPROJ,  (float*)&mat,        4);
    dev_obj->SetVertexShaderConstantF(VS_CONST_REG_LIGHT_DIR, (float*)&light_dir,  1);

    // ===== paleta de ossos =====
    anim_agent = agent->GetAnimCtrlAgent();
    if (!anim_agent)
        goto __ret;

    animobj_num = anim_agent->GetAnimCtrlObjNum();

    {
        DWORD idx;
        for (idx = 0; idx < animobj_num; ++idx)
        {
            animctrl_obj = anim_agent->GetAnimCtrlObj(idx);
            if (!animctrl_obj)
                continue;

            animctrl_obj->GetTypeInfo(&type_info);
            if (type_info.type == ANIM_CTRL_TYPE_BONE)
            {
                lwIAnimCtrlObjBone* bone_ctrl = (lwIAnimCtrlObjBone*)animctrl_obj;
                DWORD bone_num = bone_ctrl->GetBoneRTTMNum();
                const lwMatrix44* rtmat = (const lwMatrix44*)bone_ctrl->GetBoneRTMSeq();

                if (bone_num == 0 || !rtmat)
                    goto __ret;

                // buffer esttico: 50 ossos * (3 registros * 4 floats) = 600 floats
                static float __this_buf[50 * 12];
                if (bone_num > 50)
                    bone_num = 50; // evita overflow, mantido simples

                DWORD bi;
                for (bi = 0; bi < bone_num; ++bi)
                {
                    lwMatrix44Transpose((lwMatrix44*)&__this_buf[bi * 12], &rtmat[bi]);
                }

                dev_obj->SetVertexShaderConstantF(VS_CONST_REG_MAT_PALETTE, __this_buf, bone_num * 3);
                break;
            }
        }
    }

    // ===== pixel shader (bloco adicionado, mantido prximo ao original) =====
    if (!mPixelShaderName.empty())
    {
        IDirect3DDeviceX* device = dev_obj->GetDevice();
        if (device)
        {
            if (mPixelShaderReload)
            {
                // opcional: liberar anterior para evitar leak ao recarregar
                if (mPixelShader)
                {
                    device->SetPixelShader(0);
                    ((IDirect3DPixelShaderX*)mPixelShader)->Release();
                    mPixelShader = 0;
                }

                LPD3DXBUFFER codeBuffer  = 0;
                LPD3DXBUFFER errorBuffer = 0;
                HRESULT hr = D3DXAssembleShaderFromFile(
                    mPixelShaderName.c_str(), NULL, NULL, 0, &codeBuffer, &errorBuffer);

                if (SUCCEEDED(hr) && codeBuffer)
                {
                    hr = device->CreatePixelShader(
                        (DWORD*)codeBuffer->GetBufferPointer(),
                        (IDirect3DPixelShaderX**)&mPixelShader);
                }
                else
                {
                    mPixelShader = 0;
                    if (errorBuffer)
                    {
                        const char* str = (const char*)errorBuffer->GetBufferPointer();
                        MessageBox(0, str, "D3DXAssembleShaderFromFile Failed", 0);
                    }
                }

                if (errorBuffer) { errorBuffer->Release(); errorBuffer = 0; }
                if (codeBuffer)  { codeBuffer->Release();  codeBuffer  = 0; }
                mPixelShaderReload = false;
            }

            device->SetPixelShader((IDirect3DPixelShaderX*)mPixelShader);
        }
    }

    // ===== vertex shader =====
    {
        lwIShaderMgr* shader_mgr = res_mgr->GetShaderMgr();
        if (!shader_mgr)
            goto __ret;

        if (LW_FAILED(shader_mgr->QueryVertexShader(&vs, agent->GetVertexShader())))
            goto __ret;

        if (LW_FAILED(shader_mgr->QueryVertexDeclaration(&decl, agent->GetVertexDeclaration())))
            goto __ret;

        dev_obj->SetVertexDeclarationForced(decl);
        dev_obj->SetVertexShader(vs);
    }

    ret = LW_RET_OK;

__ret:
    // restaura fog se tnhamos desligado e falhou
    if (LW_FAILED(ret) && _rs_fog)
        dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);

    return ret;
}
LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::EndSet(lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();

    if(_rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }

	dev_obj->GetDevice()->SetPixelShader(0);

#if(defined LW_USE_DX9)
    dev_obj->SetVertexShader(NULL);
#endif
    return LW_RET_OK;
}
LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();
    lwIMtlTexAgent* mtltex_agent = agent->GetMtlTexAgent();

    lwMaterial* mtl = mtltex_agent->GetMaterial();

    lwColorValue4f amb_dif[2];
    lwColorValue4f* c;
    amb_dif[0];

    if(_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        c = &amb_dif[0];
        c->r = (_lgt.Ambient.r + _rs_amb.r) * mtl->amb.r;
        c->g = (_lgt.Ambient.g + _rs_amb.g) * mtl->amb.g;
        c->b = (_lgt.Ambient.b + _rs_amb.b) * mtl->amb.b;
        c->a = (_lgt.Ambient.a + _rs_amb.a) * mtl->amb.a;
            
        c = &amb_dif[1];
        c->r = _lgt.Diffuse.r * mtl->dif.r;
        c->g = _lgt.Diffuse.g * mtl->dif.g;
        c->b = _lgt.Diffuse.b * mtl->dif.b;
        c->a = _lgt.Diffuse.a * mtl->dif.a;
    }
    else
    {
        c = &amb_dif[0];
        c->r = _rs_amb.r * mtl->amb.r;
        c->g = _rs_amb.g * mtl->amb.g;
        c->b = _rs_amb.b * mtl->amb.b;
        c->a = _rs_amb.a * mtl->amb.a;

        c = &amb_dif[1];
        c->r = c->g = c->b = c->a = 0.0f;
    }
    
    //c = &amb_dif[0];
    //if(c->r > 1.0f)
    //    c->r = 1.0f;
    //if(c->g > 1.0f)
    //    c->g = 1.0f;
    //if(c->b > 1.0f)
    //    c->b = 1.0f;
    //if(c->a > 1.0f)
    //    c->a = 1.0f;

    dev_obj->SetVertexShaderConstantF(VS_CONST_REG_LIGHT_AMB, (float*)&amb_dif, 2);

    // set texture uv data
    DWORD stage_tab[3] =
    {
        VS_CONST_REG_TS0_UVMAT,
        VS_CONST_REG_TS1_UVMAT,
        VS_CONST_REG_TS2_UVMAT,
    };

    lwIAnimCtrlAgent* anim_agent = agent->GetAnimCtrlAgent();
    DWORD animobj_num = anim_agent->GetAnimCtrlObjNum();
    lwIAnimCtrlObj* animctrl_obj;
    lwAnimCtrlObjTypeInfo type_info;

    for(DWORD i = 0; i < animobj_num; i++)
    {
        animctrl_obj = anim_agent->GetAnimCtrlObj(i);
        animctrl_obj->GetTypeInfo(&type_info);

        if((type_info.data[0] == subset) && (type_info.type == ANIM_CTRL_TYPE_TEXUV))
        {
            lwIAnimCtrlObjTexUV* texuv_ctrl = (lwIAnimCtrlObjTexUV*)animctrl_obj;
            DWORD stage_id = stage_tab[type_info.data[1]];
            lwMatrix44 mat, mat_src;
            texuv_ctrl->GetRTM(&mat_src);
            lwMatrix44Transpose(&mat, &mat_src);

            dev_obj->SetVertexShaderConstantF(stage_id, (float*)&mat, 4);
        }
    }

    return LW_RET_OK;
}
LW_RESULT lwxRenderCtrlVSVertexBlend_dx8::EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}


#if(defined LW_USE_DX9)

// lwxRenderCtrlVSVertexBlend
LW_STD_IMPLEMENTATION(lwxRenderCtrlVSVertexBlend);


LW_RESULT lwxRenderCtrlVSVertexBlend::Clone(lwIRenderCtrlVS** obj)
{
    this_type* o = LW_NEW(this_type);
    *o = *this;

    *obj = o;

    return LW_RET_OK;
}
LW_RESULT lwxRenderCtrlVSVertexBlend::Initialize(lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIShaderMgr* shader_mgr = res_mgr->GetShaderMgr();

    lwVertexShaderInfo* vs_info = shader_mgr->GetVertexShaderInfo(agent->GetVertexShader());
    if(FAILED(D3DXGetShaderConstantTable((DWORD*)vs_info->data, &_const_tab)))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwxRenderCtrlVSVertexBlend::BeginSet(lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    // --- declaraes antecipadas (evita warnings com goto) ---
    lwIResourceMgr*             res_mgr      = 0;
    lwIDeviceObject*            dev_obj      = 0;
    IDirect3DDeviceX*           dev          = 0;
    lwIMeshAgent*               mesh_agent   = 0;
    lwIMesh*                    mesh         = 0;
    DWORD                       blend_factor = 0;

    lwMatrix44*                 mat_global   = 0;
    lwMatrix44                  mat; // viewProj * global (sem transpose, como no seu cdigo)
    lwVector3                   light_dir(0.0f, 0.0f, 0.0f);

    DWORD                       rs_amb       = 0;

    lwIAnimCtrlAgent*           anim_agent   = 0;
    DWORD                       animobj_num  = 0;
    lwIAnimCtrlObj*             animctrl_obj = 0;

    // --- validaes mnimas preservando o estilo original ---
    if (!agent)
        goto __ret;

    res_mgr = agent->GetResourceMgr();
    if (!res_mgr)
        goto __ret;

    dev_obj = res_mgr->GetDeviceObject();
    if (!dev_obj)
        goto __ret;

    dev = dev_obj->GetDevice();
    if (!dev)
        goto __ret;

    mesh_agent = agent->GetMeshAgent();
    if (!mesh_agent)
        goto __ret;

    mesh = mesh_agent->GetMesh();
    if (!mesh || !mesh->GetMeshInfo())
        goto __ret;

    blend_factor = mesh->GetMeshInfo()->bone_infl_factor;

    // --- FOG: salvar e desabilitar (restauro no __ret em caso de falha) ---
    dev_obj->GetRenderState(D3DRS_FOGENABLE, &_rs_fog);
    if (_rs_fog)
        dev_obj->SetRenderState(D3DRS_FOGENABLE, FALSE);

    // --- matrizes (mantido sem transpose, como no seu cdigo) ---
    mat_global = agent->GetGlobalMatrix();
    if (!mat_global)
        goto __ret;

    {
        const lwMatrix44* viewProj = dev_obj->GetMatViewProj();
        if (!viewProj)
            goto __ret;

        mat = *viewProj;
        lwMatrix44Multiply(&mat, mat_global, &mat);
        // lwMatrix44Transpose(&mat, &mat); // (mantido comentado como estava)
    }

    // --- ambiente e luz (mantido) ---
    dev_obj->GetRenderState(D3DRS_AMBIENT, &rs_amb);
    _rs_amb.a = LW_ARGB_A(rs_amb);
    _rs_amb.r = LW_ARGB_R(rs_amb);
    _rs_amb.g = LW_ARGB_G(rs_amb);
    _rs_amb.b = LW_ARGB_B(rs_amb);

    dev_obj->GetLight(0, &_lgt);
    dev_obj->GetRenderState(D3DRS_LIGHTING, &_rs_lgt);
    dev_obj->GetLightEnable(0, &_lgt_enable);

    if (_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        light_dir = *(lwVector3*)&_lgt.Direction;
        light_dir.x = -light_dir.x;
        light_dir.y = -light_dir.y;
        light_dir.z = -light_dir.z;

        lwMatrix44 mat_light;
        lwMatrix44InverseNoScaleFactor(&mat_light, mat_global);
        lwVec3Mat44MulNormal(&light_dir, &mat_light);
    }

    // --- constantes de shader (mantido seu _const_tab) ---
    if (FAILED(_const_tab->SetInt(dev, "blend_num", blend_factor)))
        goto __ret;

    if (FAILED(_const_tab->SetMatrix(dev, "mat_viewproj", &mat)))
        goto __ret;

    if (FAILED(_const_tab->SetValue(dev, "light_dir", &light_dir, sizeof(light_dir))))
        goto __ret;

    // --- paleta de ossos (mantido) ---
    anim_agent = agent->GetAnimCtrlAgent();
    if (!anim_agent)
        goto __ret;

    animobj_num = anim_agent->GetAnimCtrlObjNum();

    {
        DWORD i;
        for (i = 0; i < animobj_num; ++i)
        {
            animctrl_obj = anim_agent->GetAnimCtrlObj(i);
            if (!animctrl_obj)
                continue;

            lwAnimCtrlTypeInfo info;
            animctrl_obj->GetTypeInfo(&info);
            if (info.type == ANIM_CTRL_TYPE_BONE)
            {
                lwIAnimCtrlObjBone* bone_ctrl = (lwIAnimCtrlObjBone*)animctrl_obj;
                DWORD reg_num = bone_ctrl->GetBoneRTTMNum();
                if (reg_num == 0)
                    goto __ret;

                // Se GetConstantByName falhar, retorna NULL e SetMatrixArray falha:
                if (FAILED(_const_tab->SetMatrixArray(
                        dev,
                        _const_tab->GetConstantByName(NULL, "mat_bonepallette"),
                        (D3DXMATRIX*)bone_ctrl->GetBoneRTMSeq(),
                        reg_num)))
                {
                    goto __ret;
                }

                break; // usa o primeiro bone controller encontrado
            }
        }
    }

    // --- vertex shader + decl (mantido) ---
    {
        lwIShaderMgr* shader_mgr = res_mgr->GetShaderMgr();
        if (!shader_mgr)
            goto __ret;

        IDirect3DVertexShaderX*      shader = 0;
        IDirect3DVertexDeclarationX* decl   = 0;

        if (LW_FAILED(shader_mgr->QueryVertexShader(&shader, agent->GetVertexShader())))
            goto __ret;

        if (LW_FAILED(shader_mgr->QueryVertexDeclaration(&decl, agent->GetVertexDeclaration())))
            goto __ret;

        dev_obj->SetVertexDeclarationForced(decl);
        dev_obj->SetVertexShader(shader);
    }

    ret = LW_RET_OK;

__ret:
    // restaura FOG se falhou e tnhamos desligado
    if (LW_FAILED(ret) && _rs_fog)
        dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);

    return ret;
}
LW_RESULT lwxRenderCtrlVSVertexBlend::EndSet(lwIRenderCtrlAgent* agent)
{
    lwIResourceMgr* res_mgr = agent->GetResourceMgr();
    lwIDeviceObject* dev_obj = res_mgr->GetDeviceObject();

    if(_rs_fog)
    {
        dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }

    dev_obj->SetVertexShader(NULL);

    return LW_RET_OK;

}
LW_RESULT lwxRenderCtrlVSVertexBlend::BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    LW_RESULT ret = LW_RET_FAILED;

    // --- declaraes antecipadas (evita warnings com goto) ---
    lwIResourceMgr*     res_mgr       = 0;
    lwIDeviceObject*    dev_obj       = 0;
    lwIMtlTexAgent*     mtltex_agent  = 0;
    IDirect3DDeviceX*   dev           = 0;
    lwMaterial*         mtl           = 0;

    lwColorValue4f      amb_dif[2];
    lwColorValue4f*     c             = 0;
    DWORD               rs_amb        = 0;

    lwIAnimCtrlAgent*   anim_agent    = 0;
    DWORD               animobj_num   = 0;
    lwIAnimCtrlObj*     animctrl_obj  = 0;

    // tabela de estgios para UV (mantida como no original)
    DWORD stage_tab[3] =
    {
        VS_CONST_REG_TS0_UVMAT,
        VS_CONST_REG_TS1_UVMAT,
        VS_CONST_REG_TS2_UVMAT,
    };

    // --- validaes mnimas ---
    if (!agent)
        goto __ret;

    res_mgr = agent->GetResourceMgr();
    if (!res_mgr)
        goto __ret;

    dev_obj = res_mgr->GetDeviceObject();
    if (!dev_obj)
        goto __ret;

    mtltex_agent = agent->GetMtlTexAgent();
    if (!mtltex_agent)
        goto __ret;

    dev = dev_obj->GetDevice();
    if (!dev)
        goto __ret;

    mtl = mtltex_agent->GetMaterial();
    if (!mtl)
        goto __ret;

    // --- ambient state (mantido) ---
    dev_obj->GetRenderState(D3DRS_AMBIENT, &rs_amb);
    _rs_amb.a = LW_ARGB_A(rs_amb);
    _rs_amb.r = LW_ARGB_R(rs_amb);
    _rs_amb.g = LW_ARGB_G(rs_amb);
    _rs_amb.b = LW_ARGB_B(rs_amb);

    // --- combinaes amb/dif conforme luz direcional (mantido) ---
    if (_rs_lgt && _lgt_enable && _lgt.Type == D3DLIGHT_DIRECTIONAL)
    {
        c = &amb_dif[0];
        c->r = (_lgt.Ambient.r + _rs_amb.r) * mtl->amb.r;
        c->g = (_lgt.Ambient.g + _rs_amb.g) * mtl->amb.g;
        c->b = (_lgt.Ambient.b + _rs_amb.b) * mtl->amb.b;
        c->a = (_lgt.Ambient.a + _rs_amb.a) * mtl->amb.a;

        c = &amb_dif[1];
        c->r = _lgt.Diffuse.r * mtl->dif.r;
        c->g = _lgt.Diffuse.g * mtl->dif.g;
        c->b = _lgt.Diffuse.b * mtl->dif.b;
        c->a = _lgt.Diffuse.a * mtl->dif.a;
    }
    else
    {
        c = &amb_dif[0];
        c->r = _rs_amb.r * mtl->amb.r;
        c->g = _rs_amb.g * mtl->amb.g;
        c->b = _rs_amb.b * mtl->amb.b;
        c->a = _rs_amb.a * mtl->amb.a;

        c = &amb_dif[1];
        c->r = c->g = c->b = c->a = 0.0f;
    }

    // --- envia constantes para o VS via constant table (mantido) ---
    if (!_const_tab)
        goto __ret;

    if (FAILED(_const_tab->SetVector(dev, "mtl_amb", (D3DXVECTOR4*)&amb_dif[0])))
        goto __ret;

    if (FAILED(_const_tab->SetVector(dev, "mtl_dif", (D3DXVECTOR4*)&amb_dif[1])))
        goto __ret;

    // --- UV anim (mantido; s adicionei bounds-check) ---
    anim_agent = agent->GetAnimCtrlAgent();
    if (!anim_agent)
        goto __ret;

    animobj_num = anim_agent->GetAnimCtrlObjNum();

    {
        DWORD i;
        for (i = 0; i < animobj_num; ++i)
        {
            animctrl_obj = anim_agent->GetAnimCtrlObj(i);
            if (!animctrl_obj)
                continue;

            lwAnimCtrlTypeInfo info;
            animctrl_obj->GetTypeInfo(&info);

            // checa subset e tipo
            if (info.data[0] == subset && info.type == ANIM_CTRL_TYPE_TEXUV)
            {
                // garante ndice vlido em stage_tab
                if (info.data[1] >= 0 && info.data[1] < 3)
                {
                    DWORD stage_id = stage_tab[info.data[1]];
                    lwIAnimCtrlObjTexUV* texuv_ctrl = (lwIAnimCtrlObjTexUV*)animctrl_obj;

                    lwMatrix44 rtm, mat;
                    texuv_ctrl->GetRTM(&rtm);
                    lwMatrix44Transpose(&mat, &rtm);

                    // Se quiser reativar o envio para VS, descomente:
                    // dev_obj->SetVertexShaderConstantF(stage_id, (float*)&mat, 4);

                    // OU, se existir constante nomeada no shader (ex.: "ts0_uvmat" / "ts1_uvmat"...):
                    // if (FAILED(_const_tab->SetMatrix(dev, "ts0_uvmat", &mat)))
                    //     goto __ret;
                }
                // se no for [0..2], ignora silenciosamente
            }
        }
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwxRenderCtrlVSVertexBlend::EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent)
{
    return LW_RET_OK;
}


#endif


LW_END
