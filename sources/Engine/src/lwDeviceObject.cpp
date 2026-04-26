//
#include "stdafx.h"
#include "lwDeviceObject.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwPathInfo.h"
#include "lwErrorCode.h"
#include "lwD3D.h"
#include "lwGraphicsUtil.h"
#include "lwPrimitive.h"
#include "lwResourceMgr.h"

// Del by lark.li 20080611

LW_BEGIN

LW_STD_IMPLEMENTATION(lwDeviceObject)

inline DWORD FTODW(float v)
{
    return *(DWORD*)&v;
}

LW_RESULT lwDeviceObject::InitStateCache()
{
    int i, j;

	// render state
	for(i = 0; i < LW_MAX_RENDERSTATE_NUM; ++i)
    {
		_rs_value[i] = LW_INVALID_RS_VALUE;
	}

	// texture stage
	for(j = 0; j < LW_MAX_TEXTURESTAGE_NUM; ++j)
    {
		for(i = 0; i < LW_MAX_TEXTURESTAGESTATE_NUM; ++i)
        {
			_tss_value[j][i] = LW_INVALID_TSS_VALUE;
		}

        _tex_seq[j] = NULL;
		_dev->SetTexture(j, NULL);
	}

    // sample stage
    for(j = 0; j < LW_MAX_SAMPLESTAGE_NUM; j++)
    {
        for(i = 0; i < LW_MAX_SAMPLESTATE_NUM; i++)
        {
            _ss_value[j][i] = LW_INVALID_SS_VALUE;
        }
    }

	SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	SetRenderState(D3DRS_LASTPIXEL, TRUE);
	SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
    SetRenderState(D3DRS_DITHERENABLE, FALSE);      
	SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    SetRenderState(D3DRS_FOGENABLE, FALSE);
	SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	SetRenderState(D3DRS_FOGCOLOR, 0);
	SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
	SetRenderState(D3DRS_FOGSTART, 0);
	SetRenderState(D3DRS_FOGEND, 0);
	SetRenderState(D3DRS_FOGDENSITY, FTODW(1.0f));
	SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

	SetRenderState(D3DRS_STENCILENABLE, FALSE);
	SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	SetRenderState(D3DRS_STENCILREF, 0);
	SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff);

	SetRenderState(D3DRS_WRAP0, 0);
	SetRenderState(D3DRS_WRAP1, 0);
	SetRenderState(D3DRS_WRAP2, 0);
	SetRenderState(D3DRS_WRAP3, 0);
	SetRenderState(D3DRS_WRAP4, 0);
	SetRenderState(D3DRS_WRAP5, 0);
	SetRenderState(D3DRS_WRAP6, 0);
	SetRenderState(D3DRS_WRAP7, 0);    
	SetRenderState(D3DRS_CLIPPING, TRUE);
    SetRenderState(D3DRS_LIGHTING, TRUE);
	SetRenderState(D3DRS_AMBIENT, 0x00000000);
	SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	SetRenderState(D3DRS_COLORVERTEX, TRUE);
	SetRenderState(D3DRS_LOCALVIEWER, TRUE);
	SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
	SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR2);
	SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	SetRenderState(D3DRS_CLIPPLANEENABLE, 0);


	SetRenderState(D3DRS_POINTSIZE , FTODW(1.0f));
	SetRenderState(D3DRS_POINTSIZE_MIN , FTODW(1.0f));
	SetRenderState(D3DRS_POINTSPRITEENABLE , FALSE);
	SetRenderState(D3DRS_POINTSCALEENABLE , FALSE);
	SetRenderState(D3DRS_POINTSCALE_A , FTODW(1.0f));
	SetRenderState(D3DRS_POINTSCALE_B , FTODW(0.0f));
	SetRenderState(D3DRS_POINTSCALE_C , FTODW(0.0f));
	SetRenderState(D3DRS_MULTISAMPLEANTIALIAS , FALSE); // default value is TRUE
	SetRenderState(D3DRS_MULTISAMPLEMASK, 0xffffffff);

    SetRenderState(D3DRS_PATCHEDGESTYLE,D3DPATCHEDGE_DISCRETE);
    SetRenderState(D3DRS_DEBUGMONITORTOKEN, D3DDMT_ENABLE);
    SetRenderState(D3DRS_POINTSIZE_MAX, FTODW(64.0f));
	SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
	SetRenderState(D3DRS_TWEENFACTOR, 0);
	SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    SetRenderState(D3DRS_POSITIONDEGREE, D3DDEGREE_CUBIC);
    SetRenderState(D3DRS_NORMALDEGREE, D3DDEGREE_LINEAR);
    SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, FALSE);
    SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
    SetRenderState(D3DRS_MINTESSELLATIONLEVEL, FTODW(1.0f));
    SetRenderState(D3DRS_MAXTESSELLATIONLEVEL, FTODW(1.0f));
    SetRenderState(D3DRS_ADAPTIVETESS_X, FTODW(0.0f));
    SetRenderState(D3DRS_ADAPTIVETESS_Y, FTODW(1.0f));
    SetRenderState(D3DRS_ADAPTIVETESS_Z, FTODW(0.0f));
    SetRenderState(D3DRS_ADAPTIVETESS_W, FTODW(0.0f));
    SetRenderState(D3DRS_ENABLEADAPTIVETESSELLATION, FALSE);
    SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
    SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);
    SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
    SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);
    SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
    SetRenderState(D3DRS_COLORWRITEENABLE1, 0x0000000f);
    SetRenderState(D3DRS_COLORWRITEENABLE2, 0x0000000f);
    SetRenderState(D3DRS_COLORWRITEENABLE3, 0x0000000f);
    SetRenderState(D3DRS_BLENDFACTOR, 0xffffffff);
    SetRenderState(D3DRS_SRGBWRITEENABLE, 0);
    SetRenderState(D3DRS_DEPTHBIAS, 0);
    SetRenderState(D3DRS_WRAP8, 0);
    SetRenderState(D3DRS_WRAP9, 0);
    SetRenderState(D3DRS_WRAP10, 0);
    SetRenderState(D3DRS_WRAP11, 0);
    SetRenderState(D3DRS_WRAP12, 0);
    SetRenderState(D3DRS_WRAP13, 0);
    SetRenderState(D3DRS_WRAP14, 0);
    SetRenderState(D3DRS_WRAP15, 0);
    SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
    SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
    SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);

	for(i = 0; i < LW_MAX_TEXTURESTAGE_NUM; ++i)
    {
		if(i == 0)
        {
			SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		}
        else
        {
			SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE	);
			SetTextureStageState(i, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			SetTextureStageState(i, D3DTSS_COLORARG2, D3DTA_CURRENT);
			SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			SetTextureStageState(i, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			SetTextureStageState(i, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		}


        SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, i);
    }

	for(i = 0; i < LW_MAX_SAMPLESTAGE_NUM; ++i)
    {
		SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 1);
		SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	}

    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::InitCapsInfo()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_d3d == 0)
        goto __ret;

    _d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &_display_mode);
    _dev->GetDeviceCaps(&_dev_caps);

    {
        lwBackBufferFormatsItemInfo* bbfii = &_bbf_caps.fmt_seq[0];

        // get adapater fmt
        bbfii->format = _display_mode.Format;
        bbfii->windowed = _d3d_create_param.present_param.Windowed;

        // back buffer formats
        // texture format
        {
            D3DFORMAT check_fmt[] =
            {
                D3DFMT_R8G8B8,
                    D3DFMT_A8R8G8B8,
                    D3DFMT_X8R8G8B8,
                    D3DFMT_R5G6B5,
                    D3DFMT_X1R5G5B5,
                    D3DFMT_A1R5G5B5,
                    D3DFMT_A4R4G4B4,
                    D3DFMT_A8,
                    D3DFMT_A8R3G3B2,
                    D3DFMT_X4R4G4B4,
                    D3DFMT_A2B10G10R10,

                    D3DFMT_A8P8,
                    D3DFMT_P8,

                    D3DFMT_L8,
                    D3DFMT_A8L8,
                    D3DFMT_A4L4,

                    D3DFMT_V8U8,
                    D3DFMT_L6V5U5,
                    D3DFMT_X8L8V8U8,
                    D3DFMT_Q8W8V8U8,
                    D3DFMT_V16U16,
                    D3DFMT_A2W10V10U10,

                    D3DFMT_UYVY,
                    D3DFMT_YUY2,
                    D3DFMT_DXT1,
                    D3DFMT_DXT2,
                    D3DFMT_DXT3,
                    D3DFMT_DXT4,
                    D3DFMT_DXT5,
            };


            int fmt_num = LW_ARRAY_LENGTH(check_fmt);

            int j = 0;
            for (int i = 0; i < fmt_num; i++)
            {
                // capability probe: failure here means the format is not supported,
                // which is normal and not logged to avoid spam.
                if (FAILED(_d3d->CheckDeviceFormat(
                    _d3d_create_param.adapter,
                    _d3d_create_param.dev_type,
                    bbfii->format,
                    0,
                    D3DRTYPE_TEXTURE,
                    check_fmt[i])))
                {
                    continue;
                }

                bbfii->fmt_texture[j++] = check_fmt[i];
            }
        }

        { // depthstencil format
            D3DFORMAT check_fmt[] =
            {
                D3DFMT_D16,
                D3DFMT_D24X8,
                D3DFMT_D24S8
            };

            int fmt_num = LW_ARRAY_LENGTH(check_fmt);

            int j = 0;
            for (int i = 0; i < fmt_num; i++)
            {
                // capability probes: failure означает «формат не поддерживается»,
                // это нормально и не логируется (иначе спам).
                if (FAILED(_d3d->CheckDeviceFormat(
                    _d3d_create_param.adapter,
                    _d3d_create_param.dev_type,
                    bbfii->format,
                    D3DUSAGE_DEPTHSTENCIL,
                    D3DRTYPE_SURFACE,
                    check_fmt[i])))
                {
                    continue;
                }

                if (FAILED(_d3d->CheckDepthStencilMatch(
                    _d3d_create_param.adapter,
                    _d3d_create_param.dev_type,
                    bbfii->format,
                    bbfii->format,
                    check_fmt[i])))
                {
                    continue;
                }

                bbfii->fmt_depthstencil[j++] = check_fmt[i];
            }
        }

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::ResetDeviceStateCache()
{
    DWORD i, j;
    DWORD v;

	// render state
	for(i = 0; i < LW_MAX_RENDERSTATE_NUM; ++i)
    {
        if((v = _rs_value[i]) == LW_INVALID_RS_VALUE)
            continue;
        SetRenderStateForced((D3DRENDERSTATETYPE)i, v);
	}

	// texture stage
	for(j = 0; j < LW_MAX_TEXTURESTAGE_NUM; ++j)
    {
		for(i = 0; i < LW_MAX_TEXTURESTAGESTATE_NUM; ++i)
        {
            if((v = _tss_value[j][i]) == LW_INVALID_TSS_VALUE)
                continue;
            SetTextureStageStateForced(j, (D3DTEXTURESTAGESTATETYPE)i, v);
		}
	}

    // sampler state
	for(j = 0; j < LW_MAX_SAMPLESTAGE_NUM; ++j)
    {
        for(i = 0; i < LW_MAX_SAMPLESTATE_NUM; i++)
        {
            if((v = _ss_value[j][i]) == LW_INVALID_SS_VALUE)
                continue;
            SetSamplerStateForced(j, (D3DSAMPLERSTATETYPE)i, v);
        }
    }

    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::ResetDeviceTransformMatrix()
{
    for(DWORD i = 0; i < LW_MAX_LIGHT_NUM; i++)
    {
        LightEnableForced(i, _light_enable[i]);
        if(_light_enable[i])
        {
            SetLight(i, &_light_seq[i]);
        }
    }

    SetTransformView(&_mat_view);
    SetTransformProj(&_mat_proj);
    SetTransformWorld(&_mat_world);

    return LW_RET_OK;
}

// lwDeviceObject
lwDeviceObject::lwDeviceObject(lwSysGraphics* sys_graphics)
: _sys_graphics(sys_graphics), _d3d(0), _dev(0), 
  _fvf_value(D3DFMT_UNKNOWN), _shader_value(0)
{

    _svb_head = 0;
    _sib_head = 0;

    memset(&_dev_caps, 0, sizeof(_dev_caps));
    memset(&_display_mode, 0, sizeof(_display_mode));
    memset(&_bbf_caps, 0, sizeof(_bbf_caps));
    
    // watch video memory
    memset(&_watch_vm_info, 0, sizeof(_watch_vm_info));

    memset(&_d3d_create_param, 0, sizeof(_d3d_create_param));
    memset(_vb_value, 0, sizeof(_vb_value));
    _ib_value = 0;

    memset(_light_enable, 0, sizeof(_light_enable));
    memset(_light_seq, 0, sizeof(_light_seq));

    memset(&_rc_window, 0, sizeof(_rc_window));
    memset(&_rc_client, 0, sizeof(_rc_client));

}
lwDeviceObject::~lwDeviceObject()
{
    _ClearStreamBuffer();

    LW_IF_RELEASE(_dev);
    LW_IF_RELEASE(_d3d);
}

LW_RESULT lwDeviceObject::CreateDirect3D()
{
    LW_RESULT ret = LW_RET_FAILED;
    
    if((_d3d = Direct3DCreateX(D3D_SDK_VERSION)) == 0)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwDeviceObject::CreateDevice(lwD3DCreateParam* param)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_d3d == 0)
        goto __ret;

    if(param->present_param.BackBufferFormat == D3DFMT_UNKNOWN)
    {
        D3DDISPLAYMODE d3ddm;
        _d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
        param->present_param.BackBufferFormat = d3ddm.Format;
    }

#if(defined LW_SHADER_DEBUG_VS || defined LW_SHADER_DEBUG_PS)
    if(param->dev_type != D3DDEVTYPE_REF)
    {
        param->behavior_flag &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        param->behavior_flag &= ~D3DCREATE_PUREDEVICE;                            
        param->behavior_flag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
    {
        IDirect3DDeviceX* dev;
        //Create a Direct3D device.
        HRESULT hr = S_OK;
        hr = _d3d->CreateDevice(param->adapter,
            param->dev_type,
            param->hwnd,
            param->behavior_flag,
            &param->present_param,
            &dev);
        if (FAILED(hr))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateDevice failed: adapter={}, dev_type={}, behavior_flag=0x{:08X}, hr=0x{:08X}",
                         __FUNCTION__, static_cast<long long>(param->adapter),
                         static_cast<long long>(param->dev_type),
                         static_cast<std::uint32_t>(param->behavior_flag),
                         static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        SetDevice(dev);

        _d3d_create_param = *param;

        UpdateWindowRect();

        // init viewport
        if (HRESULT hr2 = _dev->GetViewport(&_viewport); FAILED(hr2))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] GetViewport failed: hr=0x{:08X}",
                         __FUNCTION__, static_cast<std::uint32_t>(hr2));
            goto __ret;
        }

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::SetDirect3D(IDirect3DX* d3d)
{
    _d3d = d3d;
    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::SetDevice(IDirect3DDeviceX* dev)
{ 
    _dev = dev;
    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::ResetDevice(D3DPRESENT_PARAMETERS* d3dpp)
{
    LW_RESULT ret = LW_RET_FAILED;

	HRESULT hr = S_OK;

	while( ( hr = _dev->TestCooperativeLevel() ) == D3DERR_DEVICELOST )
	{
		// the device cannot be reset at this time
		Sleep( 100 );
	}

	// Del by lark.l i20080611

	hr = _dev->Reset( d3dpp );

	// Del by lark.l i20080611

	if(FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] Reset failed: hr=0x{:08X}",
		             __FUNCTION__, static_cast<std::uint32_t>(hr));
		goto __ret;
	}

	hr = _dev->GetViewport(&_viewport);

    if(FAILED(hr))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] GetViewport failed: hr=0x{:08X}",
                     __FUNCTION__, static_cast<std::uint32_t>(hr));
        goto __ret;
    }

    _d3d_create_param.present_param = *d3dpp;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::CreateVertexBuffer(UINT length, DWORD usage, DWORD fvf, D3DPOOL pool, IDirect3DVertexBufferX** vb, HANDLE* handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    ret = _dev->CreateVertexBufferX(length, usage, fvf, pool, vb, handle);
    if(FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] CreateVertexBufferX failed: length={}, usage=0x{:08X}, fvf=0x{:08X}, pool={}, hr=0x{:08X}",
                     __FUNCTION__, static_cast<long long>(length),
                     static_cast<std::uint32_t>(usage), static_cast<std::uint32_t>(fvf),
                     static_cast<long long>(pool), static_cast<std::uint32_t>(ret));
        goto __ret;
    }

    _watch_vm_info.alloc_vb_size += length;
    _watch_vm_info.alloc_vb_cnt += 1;

__ret:
    return ret;
}
LW_RESULT lwDeviceObject::CreateIndexBuffer(UINT length, DWORD usage, D3DFORMAT fmt, D3DPOOL pool, IDirect3DIndexBufferX** ib, HANDLE* handle)
{
    LW_RESULT ret = LW_RET_FAILED;

    ret = _dev->CreateIndexBufferX(length, usage, fmt, pool, ib, handle);
    if(FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] CreateIndexBufferX failed: length={}, usage=0x{:08X}, fmt={}, pool={}, hr=0x{:08X}",
                     __FUNCTION__, static_cast<long long>(length),
                     static_cast<std::uint32_t>(usage), static_cast<long long>(fmt),
                     static_cast<long long>(pool), static_cast<std::uint32_t>(ret));
        goto __ret;
    }

    _watch_vm_info.alloc_ib_size += length;
    _watch_vm_info.alloc_ib_cnt += 1;

__ret:
    return ret;
}

LW_RESULT lwDeviceObject::CreateTexture(IDirect3DTextureX** out_tex, const lwTexDataInfo* info, DWORD level, DWORD usage, DWORD format, D3DPOOL pool)
{
    LW_RESULT ret = LW_RET_FAILED;

    IDirect3DTextureX* t;
    D3DLOCKED_RECT lock_rc;
    IDirect3DSurfaceX* surface;

    if (pool == D3DPOOL_DEFAULT) {
        // D3DPOOL_DEFAULT без D3DUSAGE_DYNAMIC нельзя лочить напрямую — заливаем через
        // временную SYSTEMMEM-текстуру и UpdateTexture (DMA в VRAM).
        IDirect3DTextureX* mem_tex;

        if (HRESULT hr = _dev->CreateTextureX(info->width, info->height, level, usage, (D3DFORMAT)format, D3DPOOL_DEFAULT, &t, 0); FAILED(hr)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateTextureX(DEFAULT) failed: w={}, h={}, level={}, usage=0x{:08X}, format={}, hr=0x{:08X}",
                         __FUNCTION__, static_cast<long long>(info->width),
                         static_cast<long long>(info->height), static_cast<long long>(level),
                         static_cast<std::uint32_t>(usage), static_cast<long long>(format),
                         static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        if (HRESULT hr = _dev->CreateTextureX(info->width, info->height, level, 0, (D3DFORMAT)format, D3DPOOL_SYSTEMMEM, &mem_tex, 0); FAILED(hr)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateTextureX(SYSTEMMEM staging) failed: w={}, h={}, level={}, format={}, hr=0x{:08X}",
                         __FUNCTION__, static_cast<long long>(info->width),
                         static_cast<long long>(info->height), static_cast<long long>(level),
                         static_cast<long long>(format), static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        if (HRESULT hr = mem_tex->GetSurfaceLevel(0, &surface); FAILED(hr)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] mem_tex->GetSurfaceLevel(0) failed: hr=0x{:08X}",
                         __FUNCTION__, static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        if (HRESULT hr = surface->LockRect(&lock_rc, 0, 0); FAILED(hr)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] surface->LockRect (staging) failed: hr=0x{:08X}",
                         __FUNCTION__, static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        memcpy(lock_rc.pBits, info->data, info->size);

        surface->UnlockRect();
        surface->Release();

        if (HRESULT hr = _dev->UpdateTexture(mem_tex, t); FAILED(hr)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] UpdateTexture (mem->default) failed: hr=0x{:08X}",
                         __FUNCTION__, static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        mem_tex->Release();
    }
    else {
        // MANAGED / SYSTEMMEM / SCRATCH lockable напрямую — пишем в LockRect без staging.
        if (HRESULT hr = _dev->CreateTextureX(info->width, info->height, level, usage, (D3DFORMAT)format, pool, &t, 0); FAILED(hr)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] CreateTextureX(pool={}) failed: w={}, h={}, level={}, usage=0x{:08X}, format={}, hr=0x{:08X}",
                         __FUNCTION__, static_cast<long long>(pool),
                         static_cast<long long>(info->width), static_cast<long long>(info->height),
                         static_cast<long long>(level), static_cast<std::uint32_t>(usage),
                         static_cast<long long>(format), static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        if (HRESULT hr = t->GetSurfaceLevel(0, &surface); FAILED(hr)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] t->GetSurfaceLevel(0) failed: hr=0x{:08X}",
                         __FUNCTION__, static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        if (HRESULT hr = surface->LockRect(&lock_rc, 0, 0); FAILED(hr)) {
            ToLogService("errors", LogLevel::Error,
                         "[{}] surface->LockRect failed: hr=0x{:08X}",
                         __FUNCTION__, static_cast<std::uint32_t>(hr));
            goto __ret;
        }

        memcpy(lock_rc.pBits, info->data, info->size);

        surface->UnlockRect();
        surface->Release();
    }

    *out_tex = t;

    _watch_vm_info.alloc_tex_size += info->size;
    _watch_vm_info.alloc_tex_cnt += 1;

    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwDeviceObject::CreateTexture(IDirect3DTextureX** out_tex, UINT width, UINT height, UINT level, DWORD usage, D3DFORMAT format, D3DPOOL pool)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_RESULT r = _dev->CreateTextureX(width, height, level, usage, format, pool, out_tex, NULL); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] CreateTextureX failed: w={}, h={}, level={}, usage=0x{:08X}, format={}, pool={}, hr=0x{:08X}",
                     __FUNCTION__, static_cast<long long>(width),
                     static_cast<long long>(height), static_cast<long long>(level),
                     static_cast<std::uint32_t>(usage), static_cast<long long>(format),
                     static_cast<long long>(pool), static_cast<std::uint32_t>(r));
        goto __ret;
    }

    D3DSURFACE_DESC desc;
    if(LW_RESULT r = (*out_tex)->GetLevelDesc(0, &desc); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] GetLevelDesc(0) failed: hr=0x{:08X}",
                     __FUNCTION__, static_cast<std::uint32_t>(r));
        goto __ret;
    }

    _watch_vm_info.alloc_tex_size += lwGetSurfaceSize(desc.Width, desc.Height, desc.Format);
    _watch_vm_info.alloc_tex_cnt += 1;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::CreateTextureFromFileInMemory(IDirect3DTextureX** out_tex, void* data, UINT data_size, UINT width, UINT height, UINT mip_level, DWORD usage, D3DFORMAT format, D3DPOOL pool, DWORD filter, DWORD mip_filter, D3DCOLOR colorkey, D3DXIMAGE_INFO* src_info, PALETTEENTRY* palette)
{
    LW_RESULT ret = LW_RET_FAILED;

    IDirect3DTextureX* t = 0;
    {
        HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
            _dev, // device
            data, // data
            data_size, // data size
            D3DX_DEFAULT_NONPOW2, // width
            D3DX_DEFAULT_NONPOW2, // height
            mip_level, // mipmap level
            usage, // usage
            format, // format
            pool, // d3dpool
            filter, // filter
            mip_filter,
            colorkey, // colorkey
            src_info, // D3DXIMAGE_INFO
            palette, // PALETTEENTRY
            &t); // out
        if(FAILED(hr))
        {
            ToLogService("errors", LogLevel::Error,
                         "[{}] D3DXCreateTextureFromFileInMemoryEx failed: data_size={}, mip_level={}, usage=0x{:08X}, format={}, pool={}, hr=0x{:08X}",
                         __FUNCTION__, static_cast<long long>(data_size),
                         static_cast<long long>(mip_level), static_cast<std::uint32_t>(usage),
                         static_cast<long long>(format), static_cast<long long>(pool),
                         static_cast<std::uint32_t>(hr));
            goto __ret;
        }
    }

    D3DSURFACE_DESC desc;
    if(LW_RESULT r = t->GetLevelDesc(0, &desc); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] GetLevelDesc(0) failed: hr=0x{:08X}",
                     __FUNCTION__, static_cast<std::uint32_t>(r));
        goto __ret;
    }

    _watch_vm_info.alloc_tex_size += lwGetSurfaceSize(desc.Width, desc.Height, desc.Format);
    _watch_vm_info.alloc_tex_cnt += 1;

    *out_tex = t;

    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwDeviceObject::CreateVertexBuffer(lwIVertexBuffer** out_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwVertexBuffer* obj = LW_NEW(lwVertexBuffer(this));
    if(obj == 0)
        goto __ret;

    obj->_next = _svb_head;

    if(_svb_head)
    {
        _svb_head->_prev = obj;
    }
    _svb_head = obj;

    *out_obj = obj;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::CreateIndexBuffer(lwIIndexBuffer** out_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIndexBuffer* obj = LW_NEW(lwIndexBuffer(this));
    if(obj == 0)
        goto __ret;

    obj->_next = _sib_head;

    if(_sib_head)
    {
        _sib_head->_prev = obj;
    }
    _sib_head = obj;

    *out_obj = obj;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::CreateRenderTarget(IDirect3DSurfaceX** o_surface, UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality, BOOL lockable, HANDLE* handle)
{
    return _dev->CreateRenderTargetX(width, height, format, multi_sample, multi_sample_quality, lockable, o_surface, handle);
}

LW_RESULT lwDeviceObject::CreateDepthStencilSurface(IDirect3DSurfaceX** o_surface, UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multi_sample, DWORD multi_sample_quality, BOOL discard, HANDLE* handle)
{
    return _dev->CreateDepthStencilSurfaceX(width, height, format, multi_sample, multi_sample_quality, discard, o_surface, handle);
}
LW_RESULT lwDeviceObject::CreateCubeTexture(IDirect3DCubeTextureX** o_tex, UINT edge_length, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, HANDLE* handle)
{
    return _dev->CreateCubeTextureX(edge_length, levels, usage, format, pool, o_tex, handle);
}
LW_RESULT lwDeviceObject::CreateOffscreenPlainSurface(IDirect3DSurfaceX** surface, UINT width, UINT height, D3DFORMAT format, DWORD pool, HANDLE* handle)
{
    return _dev->CreateOffscreenPlainSurface(width, height, format, (D3DPOOL)pool, surface, handle);
}

LW_RESULT lwDeviceObject::ReleaseVertexBuffer(IDirect3DVertexBufferX* vb)
{
    if(!vb) return LW_RET_OK;
    D3DVERTEXBUFFER_DESC desc;
    if (SUCCEEDED(vb->GetDesc(&desc))) {
        _watch_vm_info.alloc_vb_size -= desc.Size;
        if (_watch_vm_info.alloc_vb_size < 0) _watch_vm_info.alloc_vb_size = 0;
    }
    vb->Release();
    return LW_RET_OK;
}
void lwDeviceObject::_ReleaseStreamBuffer(lwVertexBuffer* buf)
{
    if(_svb_head == buf)
    {
        _svb_head = buf->_next;
    }

    buf->_RemoveThisFromList();
}
void lwDeviceObject::_ReleaseStreamBuffer(lwIndexBuffer* buf)
{
    if(_sib_head == buf)
    {
        _sib_head = buf->_next;
    }

    buf->_RemoveThisFromList();
}
void lwDeviceObject::_ClearStreamBuffer()
{
#if(defined ON_DEVELOP)
    if(_svb_head || _sib_head)
    {
        lwMessageBox("resource leak in stream buffer");
    }
#endif

    lwVertexBuffer* svb;
    while(_svb_head)
    {
        svb = _svb_head;
        _svb_head = _svb_head->_next;
        svb->Release();
    }

    lwIndexBuffer* sib;
    while(_sib_head)
    {
        sib = _sib_head;
        _sib_head = _sib_head->_next;
        sib->Release();
    }
}

LW_RESULT lwDeviceObject::ReleaseIndexBuffer(IDirect3DIndexBufferX* ib)
{
    if(!ib) return LW_RET_OK;
    D3DINDEXBUFFER_DESC desc;
    if (SUCCEEDED(ib->GetDesc(&desc))) {
        _watch_vm_info.alloc_ib_size -= desc.Size;
        if (_watch_vm_info.alloc_ib_size < 0) _watch_vm_info.alloc_ib_size = 0;
    }
    ib->Release();
    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::ReleaseTex(IDirect3DTextureX* tex)
{
    D3DSURFACE_DESC desc;
    tex->GetLevelDesc(0, &desc);
    _watch_vm_info.alloc_tex_size -= lwGetSurfaceSize(desc.Width, desc.Height, desc.Format);

    tex->Release();
    return LW_RET_OK;
}



LW_RESULT lwDeviceObject::SetFVF(DWORD fvf)
{
    if(_fvf_value != fvf)
    {
        _fvf_value = fvf;
        return _dev->SetFVF(fvf);
    }
    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::SetVertexShader(IDirect3DVertexShaderX* shader)
{
    if(_shader_value != shader)
    {
        _shader_value = shader;
        return _dev->SetVertexShader(shader);
    }
    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::SetVertexShaderForced(IDirect3DVertexShaderX* shader)
{
    _shader_value = shader;
    return _dev->SetVertexShader(shader);
}

LW_RESULT lwDeviceObject::SetVertexDeclaration(IDirect3DVertexDeclarationX* decl)
{
    if(_decl_value != decl)
    {
        _decl_value = decl;
        return _dev->SetVertexDeclaration(decl);
    }
    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::SetVertexDeclarationForced(IDirect3DVertexDeclarationX* decl)
{
    _decl_value = decl;
    return _dev->SetVertexDeclaration(decl);
}

LW_RESULT lwDeviceObject::SetVertexShaderConstantF(UINT reg_id, const float* data, UINT v_num)
{
    return _dev->SetVertexShaderConstantF(reg_id, data, v_num);
}



LW_RESULT lwDeviceObject::SetStreamSource(UINT stream_num, IDirect3DVertexBufferX* stream_data, UINT offset_byte, UINT stride)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(stream_num >= LW_MAX_STREAM_NUM)
        goto __ret;

    //stream_dataoffset_byte,stride
    
    ret = _dev->SetStreamSourceX(stream_num, stream_data, offset_byte, stride);
    if(FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] SetStreamSourceX failed: stream_num={}, offset_byte={}, stride={}, hr=0x{:08X}",
                     __FUNCTION__, static_cast<long long>(stream_num),
                     static_cast<long long>(offset_byte), static_cast<long long>(stride),
                     static_cast<std::uint32_t>(ret));
        goto __ret;
    }


//__addr_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::SetIndices(IDirect3DIndexBufferX* index_data, UINT base_vert_index)
{
    LW_RESULT ret = LW_RET_FAILED;

    //index_database_vert_index
    
    ret = _dev->SetIndicesX(index_data, base_vert_index);
    if(FAILED(ret))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] SetIndicesX failed: base_vert_index={}, hr=0x{:08X}",
                     __FUNCTION__, static_cast<long long>(base_vert_index),
                     static_cast<std::uint32_t>(ret));
        goto __ret;
    }


//__addr_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::DrawPrimitive(D3DPRIMITIVETYPE pt_type, UINT start_vertex, UINT count)
{
    _mark_polygon_num += count;

    //return _dev->DrawPrimitiveX(pt_type, _vertex_entry_offset + start_vertex, count);
    return _dev->DrawPrimitiveX(pt_type, start_vertex, count);

}

LW_RESULT lwDeviceObject::DrawIndexedPrimitive(D3DPRIMITIVETYPE pt_type, INT base_vert_index, UINT min_index, UINT vert_num, UINT start_index, UINT count)
{
    _mark_polygon_num += count;

    if(LW_RESULT r = _dev->DrawIndexedPrimitiveX(pt_type, base_vert_index, min_index, vert_num, start_index, count); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] DrawIndexedPrimitiveX failed: pt_type={}, base_vert={}, min_idx={}, vert_num={}, start_idx={}, count={}, hr=0x{:08X}",
                     __FUNCTION__, static_cast<long long>(pt_type),
                     static_cast<long long>(base_vert_index), static_cast<long long>(min_index),
                     static_cast<long long>(vert_num), static_cast<long long>(start_index),
                     static_cast<long long>(count), static_cast<std::uint32_t>(r));
        return LW_RET_FAILED;
    }
    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::DrawPrimitiveUP(D3DPRIMITIVETYPE pt_type, UINT count, const void* data, UINT stride)
{
    _mark_polygon_num += count;

    return _dev->DrawPrimitiveUP(pt_type, count, data, stride);
}
LW_RESULT lwDeviceObject::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE pt_type, UINT min_vert_index, UINT vert_num, UINT count, const void *index_data, D3DFORMAT index_data_fmt, const void* vert_data, UINT vert_stride)
{
    _mark_polygon_num += count;

    return _dev->DrawIndexedPrimitiveUP(pt_type, min_vert_index, vert_num, count, index_data, index_data_fmt, vert_data, vert_stride);
}

LW_RESULT lwDeviceObject::SetRenderState(D3DRENDERSTATETYPE state, DWORD value)
{
    LW_RESULT ret = LW_RET_OK;

    DWORD* v = &_rs_value[state];
    if(*v != value)
    {
        *v = value;
        ret = _dev->SetRenderState(state, value);
    }

    return ret;
}
LW_RESULT lwDeviceObject::SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value)
{
    LW_RESULT ret = LW_RET_OK;

    DWORD* v = &_tss_value[stage][type];
    if(*v != value)
    {
        *v = value;
        ret = _dev->SetTextureStageState(stage, type, value);
    }

    return ret;
}

LW_RESULT lwDeviceObject::SetSamplerState(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value)
{
    LW_RESULT ret = LW_RET_OK;

    DWORD* s = &_ss_value[sampler][type];
    if(*s != value)
    {
        *s = value;
        ret = _dev->SetSamplerState(sampler, type, value);
    }

    return ret;
}

LW_RESULT lwDeviceObject::SetTexture(DWORD stage, IDirect3DBaseTextureX* tex)
{
    LW_RESULT ret = LW_RET_OK;

    IDirect3DBaseTextureX** t = &_tex_seq[stage];
    if(*t != tex)
    {
        *t = tex;
        ret = _dev->SetTexture(stage, tex);
    }

    return ret;
}


LW_RESULT lwDeviceObject::SetRenderStateForced(D3DRENDERSTATETYPE state, DWORD value)
{
    _rs_value[state] = value;
    return _dev->SetRenderState(state, value);
}
LW_RESULT lwDeviceObject::SetTextureStageStateForced(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value)
{
    _tss_value[stage][type] = value;
    return _dev->SetTextureStageState(stage, type, value);
}

LW_RESULT lwDeviceObject::SetSamplerStateForced(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value)
{
    _ss_value[sampler][type] = value;
    return _dev->SetSamplerState(sampler, type, value);
}

LW_RESULT lwDeviceObject::SetTextureForced(DWORD stage, IDirect3DTextureX* tex)
{
    _tex_seq[stage] = tex;
    return _dev->SetTexture(stage, tex);
}

LW_RESULT lwDeviceObject::GetRenderState(DWORD state, DWORD* value)
{
    *value = _rs_value[state];
    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::GetTextureStageState(DWORD stage, DWORD state, DWORD* value)
{
    *value = _tss_value[stage][state];
    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::GetSamplerState(DWORD sampler, D3DSAMPLERSTATETYPE state, DWORD* value)
{
    *value = _ss_value[sampler][state];
    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::GetTexture(DWORD stage, IDirect3DBaseTextureX** tex)
{
    *tex = _tex_seq[stage];
    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::SetLight(DWORD id, const D3DLIGHTX* light)
{
    _light_seq[id] = *light;
    return _dev->SetLight(id, light);
}
LW_RESULT lwDeviceObject::GetLight(DWORD id, D3DLIGHTX* light)
{
    *light = _light_seq[id];
    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::LightEnable(DWORD id, BOOL flag)
{
    if(flag == _light_enable[id])
        return LW_RET_OK;
    _light_enable[id] = flag;
    return _dev->LightEnable(id, flag);
}
LW_RESULT lwDeviceObject::LightEnableForced(DWORD id, BOOL flag)
{
    _light_enable[id] = flag;
    return _dev->LightEnable(id, flag);
}
LW_RESULT lwDeviceObject::GetLightEnable(DWORD id, BOOL* flag)
{
    *flag = _light_enable[id];
    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::SetTransform(D3DTRANSFORMSTATETYPE state, const lwMatrix44* mat)
{
    return _dev->SetTransform(state, (D3DMATRIX*)mat);
}

LW_RESULT lwDeviceObject::SetTransformView(const lwMatrix44* mat) 
{ 
    _mat_view = *mat; 
    lwMatrix44Multiply(&_mat_viewproj, &_mat_view, &_mat_proj); 
    return _dev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&_mat_view); 
}
LW_RESULT lwDeviceObject::SetTransformProj(const lwMatrix44* mat) 
{ 
    _mat_proj = *mat; 
    lwMatrix44Multiply(&_mat_viewproj, &_mat_view, &_mat_proj); 
    return _dev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&_mat_proj); 
}
LW_RESULT lwDeviceObject::SetTransformWorld(const lwMatrix44* mat) 
{ 
    _mat_world = *mat;
    return _dev->SetTransform(D3DTS_WORLD, (D3DMATRIX*)mat); 
}

LW_RESULT lwDeviceObject::SetViewPort(const D3DVIEWPORTX* vp)
{
    _viewport = *vp;
    return _dev->SetViewport(vp);
}
LW_RESULT lwDeviceObject::GetViewPort(D3DVIEWPORTX* vp)
{
    *vp = _viewport;
    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::UpdateWindowRect()
{
    ::GetWindowRect(_d3d_create_param.hwnd, &_rc_window);
    ::GetClientRect(_d3d_create_param.hwnd, &_rc_client);

    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::GetWindowRect(RECT* rc_wnd, RECT* rc_client)
{
    if(rc_wnd)
    {
        *rc_wnd = _rc_window;
    }
    if(rc_client)
    {
        *rc_client = _rc_client;
    }
    return LW_RET_OK;
}

void lwDeviceObject::BeginBenchMark()
{
    _mark_vertex_num = 0;
    _mark_polygon_num = 0;
}
void lwDeviceObject::EndBenchMark()
{
}

LW_RESULT lwDeviceObject::CheckCurrentDeviceFormat(DWORD type, D3DFORMAT check_fmt)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwBackBufferFormatsItemInfo* bbfii = &_bbf_caps.fmt_seq[0];
    D3DFORMAT* fmt_buf;
    int fmt_num;

    switch(type)
    {
    case BBFI_TEXTURE:
        fmt_buf = &bbfii->fmt_texture[0];
        fmt_num = LW_ARRAY_LENGTH(bbfii->fmt_texture);
        break;
    case BBFI_VOLUMETEXTURE:
        fmt_buf = &bbfii->fmt_volumetexture[0];
        fmt_num = LW_ARRAY_LENGTH(bbfii->fmt_volumetexture);
        break;
    case BBFI_CUBETEXTURE:
        fmt_buf = &bbfii->fmt_cubetexture[0];
        fmt_num = LW_ARRAY_LENGTH(bbfii->fmt_cubetexture);
        break;
    case BBFI_DEPTHSTENCIL:
        fmt_buf = &bbfii->fmt_depthstencil[0];
        fmt_num = LW_ARRAY_LENGTH(bbfii->fmt_depthstencil);
        break;
    case BBFI_MULTISAMPLE:
        fmt_buf = &bbfii->fmt_multisample[0];
        fmt_num = LW_ARRAY_LENGTH(bbfii->fmt_multisample);
        break;
    default:
        ret = ERR_INVALID_PARAM;
        goto __ret;
    }

    for(int i = 0; i < fmt_num; i++)
    {
        if(fmt_buf[i] == check_fmt)
            goto __ret_ok;

        if(fmt_buf[i] == 0)
            goto __ret;
    }

__ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwDeviceObject::ScreenToWorld(lwVector3* org, lwVector3* ray, int x, int y)
{
    lwScreenToWorld(org, ray, x, y, _viewport.Width, _viewport.Height, &_mat_proj, &_mat_view);

    return LW_RET_OK;
}
LW_RESULT lwDeviceObject::WorldToScreen(int* x, int* y, float* z, const lwVector3* v)
{
    lwWorldToScreen(x, y, z, v, _viewport.Width, _viewport.Height, &_mat_proj, &_mat_view);

    return LW_RET_OK;
}

LW_RESULT lwDeviceObject::DumpRenderState(const char* file)
{
    DWORD i, j;
    FILE* fp = fopen(file, "wt");

    if(fp)
    {
        fprintf(fp, "render state:\n");

        for(i = 0; i < LW_MAX_RENDERSTATE_NUM; ++i)
        {
            fprintf(fp, "%d\t\t\t%d\n", i, _rs_value[i]);
        }

        fprintf(fp, "texture stage state:\n");
        // texture stage
        for(j = 0; j < LW_MAX_TEXTURESTAGE_NUM; ++j)
        {
            for(i = 0; i < LW_MAX_TEXTURESTAGESTATE_NUM; ++i)
            {
                fprintf(fp, "%d\t%d\t\t%d\n", j, i, _tss_value[j][i]);
            }
        }

        fclose(fp);
    }

    return LW_RET_OK;
}


LW_END
