//
#include "stdafx.h"
#include "RenderStateMgr.h"

RenderStateMgr::RenderStateMgr()
:_dev_obj(0), _rsa_scene(0), _rsa_cha(0), _rsa_sceneobj(0), 
_rsa_sceneitem(0), _rsa_terrain(0), _rsa_transpobj(0)
{
}

RenderStateMgr::~RenderStateMgr()
{
    SAFE_RELEASE(_rsa_scene);
    SAFE_RELEASE(_rsa_cha);
    SAFE_RELEASE(_rsa_sceneobj);
    SAFE_RELEASE(_rsa_sceneitem);
    SAFE_RELEASE(_rsa_terrain);
    SAFE_RELEASE(_rsa_transpobj);

}

HRESULT RenderStateMgr::Init(MPIDeviceObject* dev_obj)
{
    _dev_obj = dev_obj;

    DWORD rs_id = 0;
    DWORD tss_id = 0;

    //begin scene
    {
        MPGUIDCreateObject((LW_VOID**)&_rsa_scene, LW_GUID_RENDERSTATEATOMSET);
        _rsa_scene->Allocate(20);

        rs_id = 0;
        _rsa_scene->SetStateValue(rs_id++, D3DRS_ALPHABLENDENABLE, TRUE);
        _rsa_scene->SetStateValue(rs_id++, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        _rsa_scene->SetStateValue(rs_id++, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        _rsa_scene->SetStateValue(rs_id++, D3DRS_ZENABLE, TRUE);
        _rsa_scene->SetStateValue(rs_id++, D3DRS_ZWRITEENABLE, TRUE);
        _rsa_scene->SetStateValue(rs_id++, D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    }


    { // begin rsa_scnobj

        MPGUIDCreateObject((LW_VOID**)&_rsa_sceneobj, LW_GUID_RENDERSTATEATOMSET);
        _rsa_sceneobj->Allocate(20);

        rs_id = 0;
        _rsa_sceneobj->SetStateValue(rs_id++, D3DRS_ALPHABLENDENABLE, TRUE);
        _rsa_sceneobj->SetStateValue(rs_id++, D3DRS_AMBIENT, 0xffa0a0a0);
        _rsa_sceneobj->SetStateValue(rs_id++, D3DRS_LIGHTING, TRUE);

        // init texture stage state
        tss_id = 10;
        _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

        // check best filter supporting
        D3DCAPSX* dev_caps = dev_obj->GetDeviceCaps();
        if(dev_caps->RasterCaps  & D3DPRASTERCAPS_ANISOTROPY)
        {
            _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_MAXANISOTROPY, dev_caps->MaxAnisotropy);
        }
        else
        {
            _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            _rsa_sceneobj->SetStateValue(tss_id++, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        }

        // light value
        memset(&_scnobj_lgt, 0, sizeof(_scnobj_lgt));
        _scnobj_lgt.Type = D3DLIGHT_DIRECTIONAL;
        _scnobj_lgt.Direction.x = -1.0f;
        _scnobj_lgt.Direction.y = -1.0f;
        _scnobj_lgt.Direction.z = -0.5f;
        D3DXVec3Normalize((D3DXVECTOR3*)&_scnobj_lgt.Direction, (D3DXVECTOR3*)&_scnobj_lgt.Direction);
        _scnobj_lgt.Diffuse.a = 1.0f;
        _scnobj_lgt.Diffuse.r = 1.0f;
        _scnobj_lgt.Diffuse.g = 1.0f;
        _scnobj_lgt.Diffuse.b = 1.0f;
    }
    // end
    
    // begin _rsa_cha
    {
        MPGUIDCreateObject((LW_VOID**)&_rsa_cha, LW_GUID_RENDERSTATEATOMSET);
        _rsa_cha->Allocate(20);

        rs_id = 0;
        _rsa_cha->SetStateValue(rs_id++, D3DRS_LIGHTING, TRUE);
        _rsa_cha->SetStateValue(rs_id++, D3DRS_AMBIENT, 0xffc0c0c0);

        tss_id = 10;
        // check best filter supporting
        D3DCAPSX* dev_caps = dev_obj->GetDeviceCaps();
        if(dev_caps->RasterCaps  & D3DPRASTERCAPS_ANISOTROPY)
        {
            _rsa_cha->SetStateValue(tss_id++, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_cha->SetStateValue(tss_id++, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_cha->SetStateValue(tss_id++, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_cha->SetStateValue(tss_id++, D3DSAMP_MAXANISOTROPY, dev_caps->MaxAnisotropy);
        }
        else
        {
            _rsa_cha->SetStateValue(tss_id++, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            _rsa_cha->SetStateValue(tss_id++, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            _rsa_cha->SetStateValue(tss_id++, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        }

        // character lighting
        memset(&_cha_lgt, 0, sizeof(_cha_lgt));
        _cha_lgt.Type = D3DLIGHT_DIRECTIONAL;
        _cha_lgt.Direction.x = -1.0f;
        _cha_lgt.Direction.y = -1.0f;
        _cha_lgt.Direction.z = -1.0f;
        D3DXVec3Normalize((D3DXVECTOR3*)&_cha_lgt.Direction, (D3DXVECTOR3*)&_cha_lgt.Direction);
        _cha_lgt.Diffuse.a = 1.0f;
        _cha_lgt.Diffuse.r = 1.0f;
        _cha_lgt.Diffuse.g = 1.0f;
        _cha_lgt.Diffuse.b = 1.0f;

    }
    // end

    // begin _rsa_transpobj
    {
        MPGUIDCreateObject((LW_VOID**)&_rsa_transpobj, LW_GUID_RENDERSTATEATOMSET);
        _rsa_transpobj->Allocate(20);

        rs_id = 0;
        _rsa_transpobj->SetStateValue(rs_id++, D3DRS_LIGHTING, FALSE);
        _rsa_transpobj->SetStateValue(rs_id++, D3DRS_ALPHABLENDENABLE, TRUE);
        _rsa_transpobj->SetStateValue(rs_id++, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        _rsa_transpobj->SetStateValue(rs_id++, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        tss_id = 10;
        // check best filter supporting
        D3DCAPSX* dev_caps = dev_obj->GetDeviceCaps();
        if(dev_caps->RasterCaps  & D3DPRASTERCAPS_ANISOTROPY)
        {
            _rsa_transpobj->SetStateValue(tss_id++, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_transpobj->SetStateValue(tss_id++, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_transpobj->SetStateValue(tss_id++, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
            _rsa_transpobj->SetStateValue(tss_id++, D3DSAMP_MAXANISOTROPY, dev_caps->MaxAnisotropy);
        }
        else
        {
            _rsa_transpobj->SetStateValue(tss_id++, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            _rsa_transpobj->SetStateValue(tss_id++, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            _rsa_transpobj->SetStateValue(tss_id++, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        }

    }
    // end

    return 0L;
}

HRESULT RenderStateMgr::BeginScene()
{
    if(_rsa_scene)
    {
        _rsa_scene->BeginRenderState(_dev_obj, 0, 10);
    }
    return 0L;
}
HRESULT RenderStateMgr::BeginCharacter()
{
    if(_rsa_cha)
    {
        _rsa_cha->BeginRenderState(_dev_obj, 0, 10);
        _rsa_cha->BeginTextureStageState(_dev_obj, 10, 10, 0);
    }
    
    _dev_obj->GetLight(0, &_cha_lgt_old);
    _dev_obj->GetLightEnable(0, &_cha_lgt_enable_old);

    _cha_lgt.Direction = _cha_lgt_old.Direction;

    _dev_obj->SetLight(0, &_cha_lgt);
    _dev_obj->LightEnable(0, TRUE);

    return 0L;
}
HRESULT RenderStateMgr::BeginSceneObject()
{
    if(_rsa_sceneobj)
    {
        _rsa_sceneobj->BeginRenderState(_dev_obj, 0, 10);
        _rsa_sceneobj->BeginTextureStageState(_dev_obj, 10, 10, 0);
    }

    for(DWORD i = 0; i < 3; i++)
    {
        _dev_obj->GetLightEnable(i, &_scnobj_lgt_enable_old[i]);
        if(_scnobj_lgt_enable_old[i])
        {
            _dev_obj->GetLight(i, &_scnobj_lgt_old[i]);
        }
    }

    _dev_obj->SetLight(0, &_scnobj_lgt);
    _dev_obj->LightEnable(0, TRUE);

    return 0L;
}
HRESULT RenderStateMgr::BeginSceneItem()
{
    return 0L;
}
HRESULT RenderStateMgr::BeginTerrain()
{
    return 0L;
}
HRESULT RenderStateMgr::BeginTranspObject()
{
    if(_rsa_transpobj)
    {
        _rsa_transpobj->BeginRenderState(_dev_obj, 0, 10);
        _rsa_transpobj->BeginTextureStageState(_dev_obj, 10, 10, 0);
    }
    return 0L;
}

HRESULT RenderStateMgr::EndScene()
{
    if(_rsa_scene)
    {
        _rsa_scene->EndRenderState(_dev_obj, 0, 10);
    }

    return 0L;
}
HRESULT RenderStateMgr::EndCharacter()
{
    if(_rsa_cha)
    {
        _rsa_cha->EndRenderState(_dev_obj, 0, 10);
        _rsa_cha->EndTextureStageState(_dev_obj, 10, 10, 0);
    }

    _dev_obj->SetLight(0, &_cha_lgt_old);
    _dev_obj->LightEnable(0, _cha_lgt_enable_old);

    return 0L;
}
HRESULT RenderStateMgr::EndSceneObject()
{
    if(_rsa_sceneobj)
    {
        _rsa_sceneobj->EndRenderState(_dev_obj, 0, 10);
        _rsa_sceneobj->EndTextureStageState(_dev_obj, 10, 10, 0);
    }

    for(DWORD i = 0; i < 3; i++)
    {
        _dev_obj->LightEnable(i, _scnobj_lgt_enable_old[i]);
        if(_scnobj_lgt_enable_old[i])
        {
            _dev_obj->SetLight(i, &_scnobj_lgt_old[i]);
        }
    }

    return 0L;
}
HRESULT RenderStateMgr::EndSceneItem()
{
    return 0L;
}
HRESULT RenderStateMgr::EndTerrain()
{
    return 0L;
}
HRESULT RenderStateMgr::EndTranspObject()
{
    if(_rsa_transpobj)
    {
        _rsa_transpobj->EndRenderState(_dev_obj, 0, 10);
        _rsa_transpobj->EndTextureStageState(_dev_obj, 10, 10, 0);
    }
    return 0L;
}
