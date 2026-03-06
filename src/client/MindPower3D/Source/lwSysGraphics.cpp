//

#include "stdafx.h"
#include "lwSysGraphics.h"
#include "lwDeviceObject.h"
#include "lwResourceMgr.h"
#include "lwSceneMgr.h"

LW_BEGIN

lwISysGraphics* lwSysGraphics::__sys_graphics = LW_NULL;


LW_STD_RELEASE( lwSysGraphics );
//LW_STD_IMPLEMENTATION( lwSysGraphics );

LW_RESULT lwSysGraphics::GetInterface( LW_VOID** i, lwGUID guid )
{
    LW_RESULT ret = LW_RET_FAILED;

    switch( guid )
    {
    case LW_GUID_DEVICEOBJECT:
        *i = (LW_VOID*)_dev_obj;
        ret = LW_RET_OK;
        break;
    case LW_GUID_RESOURCEMGR:
        *i = (LW_VOID*)( static_cast< lwIResourceMgr* >( _res_mgr ) );
        ret = LW_RET_OK;
        break;
    default:
        ret = LW_RET_NULL;
    }

    return ret;
}

lwSysGraphics::lwSysGraphics( lwSystem* sys )
: _sys(sys), _dev_obj(0), _res_mgr(0), _scene_mgr(0), _lose_dev_proc(0), _reset_dev_proc(0)
{
    _dev_obj = LW_NEW(lwDeviceObject(this));
    _res_mgr = LW_NEW(lwResourceMgr(this));  
    _scene_mgr = LW_NEW(lwSceneMgr(this));  
}

lwSysGraphics::~lwSysGraphics()
{
    LW_IF_RELEASE(_scene_mgr);
    LW_IF_RELEASE(_res_mgr);
    LW_IF_RELEASE(_dev_obj);
}

LW_RESULT lwSysGraphics::CreateDeviceObject(lwIDeviceObject** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwDeviceObject* o = LW_NEW(lwDeviceObject(this));

    if(o == 0)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwSysGraphics::CreateResourceManager(lwIResourceMgr** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwResourceMgr* o = LW_NEW(lwResourceMgr(this));

    if(o == 0)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwSysGraphics::CreateSceneManager(lwISceneMgr** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwSceneMgr* o = LW_NEW(lwSceneMgr(this));

    if(o == 0)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwSysGraphics::ToggleFullScreen(D3DPRESENT_PARAMETERS* d3dpp, lwWndInfo* wnd_info)
{
    LW_RESULT ret = LW_RET_FAILED;

    HWND hwnd = wnd_info->hwnd;

    //d3dcp->present_param.Windowed
    if(_lose_dev_proc)
    {
        if(LW_FAILED((*_lose_dev_proc)()))
            goto __ret;
    }

    if(LW_FAILED(_res_mgr->LoseDevice()))
        goto __ret;

    //AdjustWindowForChange
    LONG style;
    if(d3dpp->Windowed)
    {
        // Set windowed-mode style
        style = wnd_info->windowed_style;
    }
    else
    {
        // Set fullscreen-mode style
        style = WS_POPUP|WS_VISIBLE;
    }

    if(SetWindowLong(hwnd, GWL_STYLE, style) == 0)
        goto __ret;

    if(LW_FAILED(_dev_obj->ResetDevice(d3dpp)))
        goto __ret;

    if(d3dpp->Windowed)
    {
        if(SetWindowPos(hwnd, HWND_NOTOPMOST,
            wnd_info->left,
            wnd_info->top,
            wnd_info->width,
            wnd_info->height,
            SWP_SHOWWINDOW) == 0)
        {
            goto __ret;
        }
    }

    if(LW_FAILED(_dev_obj->UpdateWindowRect()))
        goto __ret;

    if(LW_FAILED(_dev_obj->ResetDeviceStateCache()))
        goto __ret;
    
    if(LW_FAILED(_dev_obj->ResetDeviceTransformMatrix()))
        goto __ret;

    if(LW_FAILED(_dev_obj->InitCapsInfo()))
        goto __ret;


    if(LW_FAILED(_res_mgr->ResetDevice()))
        goto __ret;

    if(_reset_dev_proc)
    {
        if(LW_FAILED((*_reset_dev_proc)()))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwSysGraphics::TestCooperativeLevel()
{
    LW_RESULT ret = LW_RET_FAILED;
    
    HRESULT hr = _dev_obj->GetDevice()->TestCooperativeLevel();

    //LG("ttt", "&&& %d\n", GetTickCount());

    if(SUCCEEDED(hr))
    {
        ret = LW_RET_OK;
        goto __ret;
    }

    if(hr == D3DERR_DEVICELOST)
    {
        ret = D3DERR_DEVICELOST;
        goto __ret;
    }

    if(hr != D3DERR_DEVICENOTRESET)
        goto __ret;

    {
        // hr == D3DERR_DEVICENOTRESET
        lwD3DCreateParam* d3dcp = _dev_obj->GetD3DCreateParam();

        // check windowed mode and use desktop backbuffer format
        if (d3dcp->present_param.Windowed == 1)
        {
            D3DDISPLAYMODE dm;
            _dev_obj->GetDirect3D()->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);
            d3dcp->present_param.BackBufferFormat = dm.Format;
        }

        if (_lose_dev_proc)
        {
            if (LW_FAILED((*_lose_dev_proc)()))
                goto __ret;
        }

        if (LW_FAILED(_res_mgr->LoseDevice()))
            goto __ret;

        if (LW_FAILED(_dev_obj->ResetDevice(&d3dcp->present_param)))
            goto __ret;

        //LG("ttt", "*** %\n", GetTickCount());

        if (LW_FAILED(_dev_obj->ResetDeviceStateCache()))
            goto __ret;

        if (LW_FAILED(_dev_obj->ResetDeviceTransformMatrix()))
            goto __ret;

        if (LW_FAILED(_res_mgr->ResetDevice()))
            goto __ret;

        if (_reset_dev_proc)
        {
            if (LW_FAILED((*_reset_dev_proc)()))
                goto __ret;
        }
    }
    ret = LW_RET_OK_1;
__ret:
    return ret;
}

LW_END
