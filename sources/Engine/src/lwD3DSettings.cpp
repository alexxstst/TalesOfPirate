//
#include "stdafx.h"
#include "lwD3DSettings.h"

LW_BEGIN

LW_RESULT lwInitDefaultD3DCreateParam( lwD3DCreateParam* param, HWND hwnd )
{
    IDirect3DX* d3d = Direct3DCreateX( D3D_SDK_VERSION );

    if( d3d == NULL )
        return LW_RET_FAILED;

    param->adapter = D3DADAPTER_DEFAULT;
    param->dev_type = D3DDEVTYPE_HAL;
    param->hwnd = hwnd;
    param->behavior_flag = D3DCREATE_HARDWARE_VERTEXPROCESSING;

    D3DDISPLAYMODE mode;
    d3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &mode );

    memset( &param->present_param, 0, sizeof( param->present_param ) );
    param->present_param.AutoDepthStencilFormat = D3DFMT_D24S8;
    param->present_param.BackBufferCount = 1;
    param->present_param.BackBufferFormat = mode.Format;
    param->present_param.BackBufferHeight = mode.Height;
    param->present_param.BackBufferWidth = mode.Width;
    param->present_param.EnableAutoDepthStencil = 1;
    param->present_param.hDeviceWindow = hwnd;
    param->present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;
    param->present_param.Windowed = 1;

#if ( defined LW_USE_DX9 )
    param->present_param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
#endif



    LW_SAFE_RELEASE( d3d );
    
    return LW_RET_OK;
}

LW_RESULT lwLoadD3DSettings( lwD3DCreateParam* param, const char* file )
{
    memset( param, 0, sizeof(lwD3DCreateParam) );

    char buf[LW_MAX_NAME];

    GetPrivateProfileString( "D3DADAPTER", "adapter", "", buf, LW_MAX_NAME, file );
    param->adapter = atoi( buf );

    GetPrivateProfileString( "D3DDEVTYPE", "devtype", "", buf, LW_MAX_NAME, file );
    param->dev_type = (D3DDEVTYPE)atoi( buf );

    GetPrivateProfileString( "BEHAVIOR", "behavior", "", buf, LW_MAX_NAME, file );
    param->behavior_flag = atoi( buf );

    // present_param
    GetPrivateProfileString( "PRESENT_PARAM", "windowed", "", buf, LW_MAX_NAME, file );
    param->present_param.Windowed = atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "backbuffer_count", "", buf, LW_MAX_NAME, file );
    param->present_param.BackBufferCount = atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "backbuffer_format", "", buf, LW_MAX_NAME, file );
    param->present_param.BackBufferFormat = (D3DFORMAT)atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "backbuffer_height", "", buf, LW_MAX_NAME, file );
    param->present_param.BackBufferHeight = atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "backbuffer_width", "", buf, LW_MAX_NAME, file );
    param->present_param.BackBufferWidth = atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "enable_depthstencil", "", buf, LW_MAX_NAME, file );
    param->present_param.EnableAutoDepthStencil = atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "depthstencil_format", "", buf, LW_MAX_NAME, file );
    param->present_param.AutoDepthStencilFormat = (D3DFORMAT)atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "multisample_type", "",  buf, LW_MAX_NAME, file );
    param->present_param.MultiSampleType = (D3DMULTISAMPLE_TYPE)atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "refresh_rate", "", buf, LW_MAX_NAME, file );
    param->present_param.FullScreen_RefreshRateInHz = atoi( buf );

    GetPrivateProfileString( "PRESENT_PARAM", "present_interval", "", buf, LW_MAX_NAME, file );
#if ( defined LW_USE_DX9 )
    param->present_param.PresentationInterval = atoi( buf );
#endif



    return LW_RET_OK;
}

LW_RESULT lwSaveD3DSettings( const char* file, const lwD3DCreateParam* param )
{
    char buf[LW_MAX_NAME];

    WritePrivateProfileString( "D3DADAPTER", "adapter", itoa( param->adapter, buf, 10 ), file );
    WritePrivateProfileString( "D3DDEVTYPE", "devtype", itoa( param->dev_type, buf, 10 ), file );
    WritePrivateProfileString( "BEHAVIOR", "behavior", itoa( param->behavior_flag, buf, 10 ), file );
    // present_param
    WritePrivateProfileString( "PRESENT_PARAM", "windowed", itoa( param->present_param.Windowed, buf, 10 ), file );
    WritePrivateProfileString( "PRESENT_PARAM", "backbuffer_count", itoa( param->present_param.BackBufferCount, buf, 10 ), file );
    WritePrivateProfileString( "PRESENT_PARAM", "backbuffer_format", itoa( param->present_param.BackBufferFormat, buf, 10 ), file );
    WritePrivateProfileString( "PRESENT_PARAM", "backbuffer_height", itoa( param->present_param.BackBufferHeight, buf, 10 ), file );
    WritePrivateProfileString( "PRESENT_PARAM", "backbuffer_width", itoa( param->present_param.BackBufferWidth, buf, 10 ), file );
    WritePrivateProfileString( "PRESENT_PARAM", "enable_depthstencil", itoa( param->present_param.EnableAutoDepthStencil, buf, 10 ), file );
    WritePrivateProfileString( "PRESENT_PARAM", "depthstencil_format", itoa( param->present_param.AutoDepthStencilFormat, buf, 10 ), file );
    WritePrivateProfileString( "PRESENT_PARAM", "multisample_type", itoa( param->present_param.MultiSampleType, buf, 10 ), file );
    WritePrivateProfileString( "PRESENT_PARAM", "refresh_rate", itoa( param->present_param.FullScreen_RefreshRateInHz, buf, 10 ), file );

#if ( defined LW_USE_DX9 )
    WritePrivateProfileString( "PRESENT_PARAM", "present_interval", itoa( param->present_param.PresentationInterval, buf, 10 ), file );
#endif

    return LW_RET_OK;
}


LW_END
