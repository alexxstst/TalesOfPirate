//
#include "stdafx.h"


#include "lwResourceMgr.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwDeviceObject.h"
#include "lwPathInfo.h"
#include "lwITypes.h"
#include "lwRenderImp.h"
#include "lwRenderCtrlEmb.h"
#include "lwItem.h"
#include "lwModel.h"
#include "lwPhysique.h"
#include "lwAnimCtrl.h"
#include "lwStreamObj.h"
#include "lwD3D.h"
#include "lwGraphicsUtil.h"
#include "lwMisc.h"
#include "lwNodeObject.h"
#include "lwDDS.h"
#include "lwAnimCtrlObj.h"
#include "lwFileEncode.h"
#include "GlobalInc.h"

#include <vector>
#include "lwThreadPool.h"

using namespace std;

LW_BEGIN


unsigned int __stdcall __thread_proc_load_tex(void* param)
{

    lwITex* tex = (lwITex*)param;

    if(LW_FAILED(tex->LoadSystemMemory()))
    {
        tex->SetLoadResMask(LOADINGRES_MASK_LOADSM_FAILED, 0);
    }

    if(LW_FAILED(tex->LoadVideoMemory()))
    {
        tex->SetLoadResMask(LOADINGRES_MASK_LOADVM_FAILED, 0);
    }

    tex->SetLoadResMask(LOADINGRES_MASK_RTMT1, 0);

    return 0;
}

unsigned int __stdcall __thread_proc_load_mesh(void* param)
{

    lwIMesh* mesh = (lwIMesh*)param;

    mesh->LoadVideoMemory();

    return 0;
}


// lwTex
//LW_STD_IMPLEMENTATION(lwTex)
LW_STD_GETINTERFACE(lwTex)

LW_RESULT lwTex::Release()
{
    LW_RESULT ret;

    if(--_ref > 0)
        return LW_RET_OK;
    
    if(_load_type == LOADINGRES_TYPE_RUNTIME_MT)
    {
        while((_load_mask & LOADINGRES_MASK_RTMT0) && !(_load_mask & LOADINGRES_MASK_RTMT1))
        {
            lwIThreadPool* tp = _res_mgr->GetThreadPoolMgr()->GetThreadPool(THREAD_POOL_LOADRES);

            if(LW_SUCCEEDED(tp->FindTask(__thread_proc_load_tex, (void*)this)))
            {
                if(_res_mgr->QueryTexRefCnt(this) == 1)
                {
                    if(LW_FAILED(tp->RemoveTask(__thread_proc_load_tex, (void*)this)))
                    {
                        LG_MSGBOX("fatal error when release texture, call jack");
                    }
                }

                break;
            }
            else
            {
                Sleep(1);
            }
        }
    }

    if(LW_FAILED(ret = _res_mgr->UnregisterTex(this)))
        goto __ret;

    if(_reg_id == LW_INVALID_INDEX)
    {
#if(defined USE_TEXLOG_MGR)
        ((lwResourceMgr*)_res_mgr)->_texlog_mgr.Log(
            0,
            _file_name,
            _data_info.width,
            _data_info.height,
            (D3DFORMAT)_format,
            _data_info.size);
#endif
        Unload();
        LW_DELETE(this);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

lwTex::lwTex(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _reg_id(LW_INVALID_INDEX),
  _stage(LW_INVALID_INDEX), _tex(0), _colorkey_type(COLORKEY_TYPE_NONE),
  _state(RES_STATE_INVALID), _byte_alignment_flag(0),
  _level(0), _usage(0), _format(0), _pool(D3DPOOL_FORCE_DWORD)
{
    _ref = 1;

    _mt_flag = 0;
    _load_flag = 0;

    _load_type = 0;
    _load_mask = 0;

    _data = 0;

    _file_name[0] = '\0';

    _rsa_0.Allocate(LW_TEX_TSS_NUM);

    memset(&_data_info, 0, sizeof(_data_info));
}

lwTex::~lwTex()
{
}

LW_RESULT lwTex::Register()
{
    assert(0 && "invalid proc");
    return _res_mgr->RegisterTex(this);
}
LW_RESULT lwTex::Unregister()
{
    return _res_mgr->UnregisterTex(this);
}

LW_RESULT lwTex::BeginPass()
{
    LW_RESULT ret = RES_PASS_ERROR;

    if(_load_type == LOADINGRES_TYPE_RUNTIME)
    {
        if(!(_load_mask & LOADINGRES_MASK_RT0))
        {
            SetLoadResMask(LOADINGRES_MASK_RT0, 0);

//#if(defined RESTRICT_LOADING_TEXTURE)
//            lwISceneMgr* scn_mgr = _res_mgr->GetSysGraphics()->GetSceneMgr();
//            lwSceneFrameInfo* sfi = scn_mgr->GetSceneFrameInfo();
//            if(sfi->_tex_cpf > 4)
//                return LW_RET_OK_1;
//
//            sfi->_tex_cpf++;
//#endif

            if(LW_FAILED(LoadVideoMemory()))
            {
                SetLoadResMask(LOADINGRES_MASK_LOADVM_FAILED, 0);
            }

        }
    }
    else if(_load_type == LOADINGRES_TYPE_RUNTIME_MT)
    {
        if(!(_load_mask & LOADINGRES_MASK_RTMT0))
        {
            SetLoadResMask(LOADINGRES_MASK_RTMT0, 0);

            lwIThreadPoolMgr* tp_mgr = _res_mgr->GetThreadPoolMgr();
            lwIThreadPool* tp = tp_mgr->GetThreadPool(THREAD_POOL_LOADRES);

            if(LW_FAILED(tp->RegisterTask(__thread_proc_load_tex, (void*)this)))
            {
                SetLoadResMask(LOADINGRES_MASK_RTMTREG_FAILED, 0);
            }
            
            ret = RES_PASS_SKIPTHISDRAW;
            goto __ret;
        }
        if(!(_load_mask &LOADINGRES_MASK_RTMT1))
        {
            ret = RES_PASS_SKIPTHISDRAW;
            goto __ret;
        }
    }


    ret = RES_PASS_DEFAULT;

__ret:
    return ret;
}

LW_RESULT lwTex::EndPass()
{
    return LW_RET_OK;
}

void lwTex::SetLoadResMask(DWORD add_mask, DWORD remove_mask)
{
    if(add_mask)
    {
        _load_mask |= add_mask;
    }
    if(remove_mask)
    {
        _load_mask &= ~remove_mask;
    }
}

LW_RESULT lwTex::BeginSet()
{
    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

//#define USE_MULTI_TEX_COLORKEY
//#ifdef USE_MULTI_TEX_COLORKEY
//    if(_colorkey.color > 0)
//    {
//        dev_obj->SetTexture(0, _tex);
        //dev_obj->SetTexture(1, _tex);
        //dev_obj->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        //dev_obj->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
        //dev_obj->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
        //dev_obj->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        //dev_obj->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
//    }
//    else
//    {
//        dev_obj->SetTexture(_stage, _tex);
//    }
//#else


    dev_obj->SetTexture(_stage, _tex);


    //if(_stage == 2)
    //{
    //    dev_obj->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255,255,255,255));
    //}

    // texture stage state

/*
    _rsa_0.BeginTextureStageState(dev_obj, 0, LW_TEX_TSS_NUM, _stage);
*/

    //for(i = 0; i < _tss_set.SEQUENCE_SIZE; i++)
    //{
    //    rsv = &_tss_set.rsv_seq[0][i];

    //    if(rsv->state == LW_INVALID_INDEX)
    //        break;

    //    dev_obj->SetTextureStageState(_stage, (D3DTEXTURESTAGESTATETYPE)rsv->state, rsv->value);
    //}
//#endif

    return LW_RET_OK;
}
LW_RESULT lwTex::EndSet()
{
    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

#ifdef USE_MULTI_TEX_COLORKEY
    if(_colorkey.color > 0)
    {
        dev_obj->SetTexture(0, 0);
        //dev_obj->SetTexture(1, 0);
        //dev_obj->SetTexture(0, _tex);
        //dev_obj->SetTexture(1, _tex);
        //dev_obj->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    }
    else
    {
        dev_obj->SetTexture(0, 0);
        dev_obj->SetTexture(1, 0);
    }
#else

    dev_obj->SetTexture(_stage, NULL);

    // texture stage state
    _rsa_0.EndTextureStageState(dev_obj, 0, LW_TEX_TSS_NUM, _stage);
    //for(i = 0; i < _tss_set.SEQUENCE_SIZE; i++)
    //{
    //    rsv = &_tss_set.rsv_seq[1][i];

    //    if(rsv->state == LW_INVALID_INDEX)
    //        break;

    //    dev_obj->SetTextureStageState(_stage, (D3DTEXTURESTAGESTATETYPE)rsv->state, rsv->value);
    //}
#endif
    return LW_RET_OK;

}

void lwTex::GetTexInfo(lwTexInfo* info)
{
     info->stage = _stage;
     info->type = _tex_type;

     info->level = _level;
     info->usage = _usage;
     info->format = (D3DFORMAT)_format;
     info->pool = _pool;

     info->colorkey_type = _colorkey_type;
     info->colorkey = _colorkey;

     info->byte_alignment_flag = _byte_alignment_flag;
     
     memcpy(info->tss_set, _rsa_0.GetStateSeq(), sizeof(lwRenderStateAtom) * LW_TEX_TSS_NUM);

    switch(_tex_type)
    {
    case TEX_TYPE_FILE:
        _tcscpy(info->file_name, _file_name);
        info->width = _data_info.width;
        info->height = _data_info.height;
        break;
    case TEX_TYPE_DATA:

        // info->data удалено (было `void*` разного размера на x86/x64).
        // Пользовательский указатель теперь получают через `lwITex::GetUserData()`.
        if(_data_info.data)
        {
             info->width = _data_info.width;
             info->height = _data_info.height;
        }

        break;
    case TEX_TYPE_SIZE:
        info->width = _data_info.width;
        info->height = _data_info.height;
        break;
    default:
        assert(0 && "invalid tex type");
    }
}

LW_RESULT lwTex::LoadTexInfo(const lwTexInfo* info, const char* tex_path)
{
    LW_RESULT ret = LW_RET_FAILED;

    _stage = info->stage;
    _tex_type = info->type;

    _level = info->level;
    _usage = info->usage;
    _format = info->format;
    _pool = info->pool;

    _colorkey_type = info->colorkey_type;
    _colorkey = info->colorkey;

    _byte_alignment_flag = info->byte_alignment_flag;
    
    _rsa_0.Load(info->tss_set, LW_TEX_TSS_NUM);

    switch(info->type)
    {
    case TEX_TYPE_FILE:
        if(tex_path)
        {
            sprintf(_file_name, "%s%s", tex_path, info->file_name);
        }
        else
        {
            strcpy(_file_name, info->file_name);
        }
        break;
    case TEX_TYPE_DATA:

        // Ранее пользовательский указатель брался из `info->data`, но это поле
        // убрано из `lwTexInfo` (x86/x64 binary-format mismatch). Теперь
        // пользователь должен выставить указатель через `SetUserData()`
        // ДО вызова `LoadTexInfo`.
        if(_data == 0)
            goto __ret;

        _data_size = info->width;

        _data_info.data = _data;
        _data_info.width = info->width;
        _data_info.height = info->height;
        _data_info.size = _data_info.width * _data_info.height;
        if(_byte_alignment_flag)
        {
            _data_info.pitch = lwGetTexFlexibleSize(_data_info.width) * lwGetTexFlexibleSize(_data_info.height);
        }
        else
        {
            _data_info.pitch = _data_info.width * _data_info.height;
        }
        break;
    case TEX_TYPE_SIZE:
        _data_info.width = info->width;
        _data_info.height = info->height;
        break;
    default:
        assert(0 && "invalid tex type");
        goto __ret;
    }

    // format
    if(_format == D3DFMT_UNKNOWN)
    {
        _format = D3DFMT_A8R8G8B8;
    }

    _state |= RES_STATE_INIT;

    ret = LW_RET_OK;
__ret:
    return ret;
    
}

LW_RESULT lwTex::LoadSystemMemory()
{
    LW_RESULT ret = LW_RET_FAILED;


    if(_state & RES_STATE_SYSTEMMEMORY)
        goto __addr_ret_ok;

    if((_state & RES_STATE_INIT) == 0)
        goto __ret;

    switch(_tex_type)
    {
    case TEX_TYPE_FILE:
        //if(LW_FAILED(lwLoadTexDataInfo(&_data_info, _file_name, _format, _colorkey_type, &_colorkey, _byte_alignment_flag)))
        //    goto __ret;
        break;
    case TEX_TYPE_DATA:
        break;
    case TEX_TYPE_SIZE:
        break;
    default:
        goto __ret;
    }

    _state |= RES_STATE_SYSTEMMEMORY;

__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwTex::LoadVideoMemory()
{
    if(_load_flag == 1)
    {
        return LoadVideoMemoryDirect();
    }
 
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

    if(_state & RES_STATE_VIDEOMEMORY)
        goto __addr_ret_ok;

    if((_state & RES_STATE_SYSTEMMEMORY) == 0)
    {
        if(LW_FAILED(LoadSystemMemory()))
            goto __ret;
    }

	// added by clp
	_format = D3DFMT_A8R8G8B8;

    if(_tex_type == TEX_TYPE_DATA)
    {
        if(LW_FAILED(dev_obj->CreateTextureFromFileInMemory(
            &_tex,
            _data,
            _data_size,
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            _level, // mip map levels
            0, // usage
            (D3DFORMAT)_format,
            _pool,
            D3DX_DEFAULT, // filter
            D3DX_DEFAULT, // mipmap filter
            _colorkey.color, // colorkey
            NULL, // D3DXIMAGE_INFO
            NULL // PALETTEENTRY
           )))
        {
            goto __ret;
        }

        D3DSURFACE_DESC desc;
        if(LW_FAILED(_tex->GetLevelDesc(0, &desc)))
            goto __ret;

        _data_info.width = desc.Width;
        _data_info.height = desc.Height;
        _data_info.size = lwGetSurfaceSize(desc.Width, desc.Height, desc.Format);

#if(defined USE_TEXLOG_MGR)
        ((lwResourceMgr*)_res_mgr)->_texlog_mgr.Log(
            1,
            "User tex",
            _data_info.width,
            _data_info.height,
            (D3DFORMAT)_format,
            _data_info.size);
#endif
    }
    else if(_tex_type == TEX_TYPE_FILE)
    {
        if(_tcslen(_file_name) == 0)
            goto __addr_ret_ok;

#if(defined USE_D3DX_LOAD_TEXTURE)

#if(defined USE_RES_BUF_MGR)

        lwIResBufMgr* resbuf_mgr = _res_mgr->GetResBufMgr();
        //lwSysMemTexInfo info;
        //info.colorkey = _colorkey.color;
        //info.format = (D3DFORMAT)_format;
        //info.level = _level;     
        //info.filter = D3DX_DEFAULT;
        //info.mip_filter = D3DX_DEFAULT;

        //_tcscpy(info.file_name, _file_name);
        lwSysMemTexInfo* info;

#if(defined LOAD_RES_BUF_RUNTIME)

//#undef USE_DDS_FILE
#if(defined USE_DDS_FILE)
        // dds
        lwDDSHeader* dds_header = 0;
        BOOL dds_flag = 1;
        char dds_file[LW_MAX_FILE];
        _tcscpy(dds_file, _file_name);
        char* p = _tcsrchr(dds_file, '.');
        if(p && (_tcsicmp(&p[1], "bmp") == 0 || _tcsicmp(&p[1], "tga") == 0))
        {
            p[1] = 'd';
            p[2] = 'd';
            p[3] = 's';
        }

        if(LW_SUCCEEDED(resbuf_mgr->QuerySysMemTex(&info, dds_file)))
            goto __load_check_dds;

        lwSysMemTexInfo smti;
        smti.colorkey = _colorkey.color;
        smti.format = (D3DFORMAT)_format;
        smti.level = _level;     
        smti.filter = D3DX_DEFAULT;
        smti.mip_filter = D3DX_DEFAULT;
        _tcscpy(smti.file_name, dds_file);

        LW_HANDLE handle;
        if(LW_SUCCEEDED(resbuf_mgr->RegisterSysMemTex(&handle, &smti)))
        {
            if(LW_FAILED(resbuf_mgr->GetSysMemTex(&info, handle)))
                goto __ret;

            goto __load_check_dds;
        }

        // here we check origin file existing
        {
            dds_flag = 0;

            if(LW_SUCCEEDED(resbuf_mgr->QuerySysMemTex(&info, _file_name)))
                goto __load_check_dds;

            lwSysMemTexInfo smti;
            smti.colorkey = _colorkey.color;
            smti.format = (D3DFORMAT)_format;
            smti.level = _level;     
            smti.filter = D3DX_DEFAULT;
            smti.mip_filter = D3DX_DEFAULT;
            _tcscpy(smti.file_name, _file_name);

            if(LW_FAILED(resbuf_mgr->RegisterSysMemTex(&handle, &smti)))
                goto __ret;

            if(LW_FAILED(resbuf_mgr->GetSysMemTex(&info, handle)))
                goto __ret;

        }

__load_check_dds:

        DWORD mip_level;
        D3DFORMAT fmt;
        DWORD colorkey;
        DWORD mip_filter;

        if(dds_flag == 1)
        {
            dds_header = (lwDDSHeader*)((BYTE*)info->buf->GetData() + sizeof(DWORD));

            mip_level = dds_header->mipmap_count;
            colorkey = 0;
            mip_filter = D3DX_DEFAULT;

            switch(dds_header->ddspf.four_cc)
            {
            case MAKEFOURCC('D', 'X', 'T', '1'):
                fmt = D3DFMT_DXT1;
                break;
            case MAKEFOURCC('D', 'X', 'T', '3'):
                fmt = D3DFMT_DXT3;
                break;
            case MAKEFOURCC('D', 'X', 'T', '5'):
                fmt = D3DFMT_DXT5;
                break;
            default:
                fmt = D3DFMT_UNKNOWN;
                break;
            }
        }
        else
        {
            mip_level = _level;
            fmt = (D3DFORMAT)_format;
            colorkey = _colorkey.color;
            mip_filter = colorkey ? D3DX_FILTER_POINT : D3DX_DEFAULT;
        }

        if(LW_FAILED(dev_obj->CreateTextureFromFileInMemory(
            &_tex,
            info->buf->GetData(),
            info->buf->GetSize(),
            D3DX_DEFAULT,
            D3DX_DEFAULT,
            mip_level, // mip map levels
            0, // usage
            (D3DFORMAT)fmt,
            _pool,
            D3DX_DEFAULT, // filter
            mip_filter, // mipmap filter
            colorkey, // colorkey
            NULL, // D3DXIMAGE_INFO
            NULL // PALETTEENTRY
           )))
        {
            goto __ret;
        }

        D3DSURFACE_DESC desc;
        if(LW_FAILED(_tex->GetLevelDesc(0, &desc)))
            goto __ret;

        _data_info.width = desc.Width;
        _data_info.height = desc.Height;
        _data_info.size = lwGetSurfaceSize(desc.Width, desc.Height, desc.Format);

#if(defined USE_TEXLOG_MGR)
        ((lwResourceMgr*)_res_mgr)->_texlog_mgr.Log(
            1,
            info->file_name,
            _data_info.width,
            _data_info.height,
            (D3DFORMAT)_format,
            _data_info.size);
#endif


        goto __use_dds;
#else
        if(LW_FAILED(resbuf_mgr->QuerySysMemTex(&info, _file_name)))
        {
            lwSysMemTexInfo smti;
            smti.colorkey = _colorkey.color;
            smti.format = (D3DFORMAT)_format;
            smti.level = _level;     
            smti.filter = D3DX_DEFAULT;
            smti.mip_filter = D3DX_DEFAULT;
            _tcscpy(smti.file_name, _file_name);

            LW_HANDLE handle;

            if(LW_FAILED(resbuf_mgr->RegisterSysMemTex(&handle, &smti)))
                goto __ret;

            if(LW_FAILED(resbuf_mgr->GetSysMemTex(&info, handle)))
                goto __ret;

        }

        goto __load_it;

#endif

#endif
        if(LW_SUCCEEDED(resbuf_mgr->QuerySysMemTex(&info, _file_name)))
        {

__load_it:

#if(defined USE_D3DX_LOAD_TEXTURE_UPDATETEXTURE)
            if(LW_FAILED(dev_obj->CreateTexture(&_tex, info->width, info->height, info->level, 0, (D3DFORMAT)info->format, D3DPOOL_DEFAULT)))
                goto __ret;

            if(LW_FAILED(dev_obj->GetDevice()->UpdateTexture(info->sys_mem_tex, _tex)))
                goto __ret;
#endif

#if(defined USE_D3DX_LOAD_TEXTURE_CREATEFROMMEMORY)

            if(LW_FAILED(dev_obj->CreateTextureFromFileInMemory(
                &_tex,
                info->buf->GetData(),
                info->buf->GetSize(),
                D3DX_DEFAULT,
                D3DX_DEFAULT,
                _level, // mip map levels
                0, // usage
                (D3DFORMAT)_format,
                _pool,
                D3DX_DEFAULT, // filter
                _colorkey.color ? D3DX_FILTER_POINT : D3DX_DEFAULT, // mipmap filter
                _colorkey.color, // colorkey
                NULL, // D3DXIMAGE_INFO
                NULL // PALETTEENTRY
               )))
            {
                goto __ret;
            }

            D3DSURFACE_DESC desc;
            if(LW_FAILED(_tex->GetLevelDesc(0, &desc)))
                goto __ret;

            _data_info.width = desc.Width;
            _data_info.height = desc.Height;
            _data_info.size = lwGetSurfaceSize(desc.Width, desc.Height, desc.Format);

#if(defined USE_TEXLOG_MGR)
            ((lwResourceMgr*)_res_mgr)->_texlog_mgr.Log(
                1,
                info->file_name,
                _data_info.width,
                _data_info.height,
                (D3DFORMAT)_format,
                _data_info.size);
#endif

#endif
        }
		else
        {
            if(FAILED(D3DXCreateTextureFromFileEx(dev_obj->GetDevice(),
                this->_file_name, //
                0, //
                0, //
                _level, //mipmap1
                0, //
                (D3DFORMAT)_format, //
                _pool, //D3DPOOL_MANAGED, //DXGraphics
                D3DX_FILTER_LINEAR, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, //
                D3DX_DEFAULT,//D3DX_DEFAULT,//D3DX_FILTER_NONE, //mipmap
                _colorkey.color,//0xffff00ff, //
                NULL, //
                NULL, //
                &_tex)))//
            {
                goto __ret;
            }

        }

#else
        if(FAILED(D3DXCreateTextureFromFileEx(dev_obj->GetDevice(),
            this->_file_name, //
            0, //
            0, //
            _level, //mipmap1
            0, //
            (D3DFORMAT)_format, //
            _pool, //D3DPOOL_MANAGED, //DXGraphics
            D3DX_FILTER_LINEAR, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, //
            D3DX_DEFAULT,//D3DX_DEFAULT,//D3DX_FILTER_NONE, //mipmap
            _colorkey.color,//0xffff00ff, //
            NULL, //
            NULL, //
            &_tex)))//
        {
            goto __ret;
        }
#endif

#else
        if(_tex_type == TEX_TYPE_FILE)
        {
            if(LW_FAILED(lwLoadTexDataInfo(&_data_info, _file_name, _format, _colorkey_type, &_colorkey, _byte_alignment_flag)))
                goto __ret;
        }

        if(LW_FAILED(dev_obj->CreateTexture(&_tex, &_data_info, _level, _usage, _format, _pool)))
            goto __ret;

#endif

    }
    else if(_tex_type == TEX_TYPE_SIZE)
    {
        // Используем запрошенный формат из lwTexInfo (_format). Раньше здесь
        // был хардкод D3DFMT_A4R4G4B4 — он ломал FontRender, который пишет в
        // атлас D3DFMT_A8R8G8B8. Для шрифтов A4 (16 уровней альфы) даёт
        // заметные ступеньки на кеглях 12-13px.
        if(LW_FAILED(dev_obj->CreateTexture(&_tex, _data_info.width, _data_info.height, _level, _usage, (D3DFORMAT)_format, _pool)))
            goto __ret;
    }

__use_dds:
    _state |= RES_STATE_VIDEOMEMORY;


__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    _state |= RES_STATE_LOADTEST;
    /*if(LW_FAILED(ret))
    {
        lwMessageBox("load texture file error with: (%d) %s", ret, this->_file_name);
		
    }*/

    return ret;
}

LW_RESULT lwTex::LoadVideoMemoryMT()
{


    lwIThreadPoolMgr* tp_mgr = _res_mgr->GetThreadPoolMgr();
    lwIThreadPool* tp = tp_mgr->GetThreadPool(THREAD_POOL_LOADRES);

    if(LW_FAILED(tp->RegisterTask(__thread_proc_load_tex, (void*)this)))
        return LW_RET_FAILED;

    _mt_flag = 1;

    return LW_RET_OK;
}


LW_RESULT lwTex::LoadVideoMemoryEx()
{

    if(_res_mgr->GetByteSet()->GetValue(OPT_RESMGR_LOADTEXTURE_MT) == 1)
    {
        return LoadVideoMemoryMT();
    }
    else
    {
#if(defined LOAD_TEXTURE_RESOURCE_DIRECTLY)
        return LoadVideoMemory();
#else
        return LW_RET_OK;
#endif
    }
}

LW_RESULT lwTex::LoadVideoMemoryDirect()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_state & RES_STATE_VIDEOMEMORY)
        goto __addr_ret_ok;

    if((_state & RES_STATE_SYSTEMMEMORY) == 0)
    {
        if(LW_FAILED(LoadSystemMemory()))
            goto __ret;
    }

    _state |= RES_STATE_LOADTEST;
    {
        lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

        if (_tex_type != TEX_TYPE_FILE)
            goto __ret;

        if (LW_FAILED(lwLoadTexDataInfo(&_data_info, _file_name, _format, _colorkey_type, &_colorkey, _byte_alignment_flag)))
            goto __ret;

        if (LW_FAILED(dev_obj->CreateTexture(&_tex, &_data_info, _level, _usage, _format, _pool)))
            goto __ret;


        _state |= RES_STATE_VIDEOMEMORY;


        D3DSURFACE_DESC desc;
        if (LW_FAILED(_tex->GetLevelDesc(0, &desc)))
            goto __ret;

        _data_info.width = desc.Width;
        _data_info.height = desc.Height;
        _data_info.size = lwGetSurfaceSize(desc.Width, desc.Height, desc.Format);

#if(defined USE_TEXLOG_MGR)
        ((lwResourceMgr*)_res_mgr)->_texlog_mgr.Log(
            1,
            _file_name,
            _data_info.width,
            _data_info.height,
            (D3DFORMAT)_format,
            _data_info.size);
#endif
	}
__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    return ret;

}

LW_RESULT lwTex::UnloadSystemMemory()
{
    LW_RESULT ret = LW_RET_FAILED;

    if((_state & RES_STATE_SYSTEMMEMORY) == 0)
        goto __addr_ret_ok;

    switch(_tex_type)
    {
    case TEX_TYPE_FILE:
    case TEX_TYPE_DATA:
        //LW_SAFE_DELETE_A(_data_info.data);
        memset(&_data_info, 0, sizeof(_data_info));
        break;
    case TEX_TYPE_SIZE:
        break;
    default:
        goto __ret;
    }

    _state &= ~RES_STATE_SYSTEMMEMORY;

__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwTex::UnloadVideoMemory()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

    if((_state & RES_STATE_VIDEOMEMORY) == 0)
        goto __addr_ret_ok;

    if(LW_FAILED(dev_obj->ReleaseTex(_tex)))
    {
        goto __ret;
    }

    _tex = 0;

    _state &= ~(RES_STATE_VIDEOMEMORY | RES_STATE_LOADTEST | RES_STATE_LOADTEST_0);

__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    return ret;

}
LW_RESULT lwTex::Unload()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_FAILED(UnloadVideoMemory()))
        goto __ret;

    if(LW_FAILED(UnloadSystemMemory()))
        goto __ret;

    ret = LW_RET_OK;

__ret:
    return ret;

}

DWORD lwTex::SetLOD(DWORD level)
{
    if((_tex == NULL) || (_pool != D3DPOOL_MANAGED))
        return 0;

    return _tex->SetLOD(level);
}

BOOL lwTex::IsLoadingOK() const
{
    return _state & RES_STATE_LOADTEST;
}

LW_RESULT lwTex::LoseDevice()
{
    LW_RESULT ret = LW_RET_FAILED;
    
    if(_pool == D3DPOOL_DEFAULT)
    {
        if(LW_FAILED(UnloadVideoMemory()))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwTex::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;
    
    if(_pool == D3DPOOL_DEFAULT)
    {
        if(LW_FAILED(LoadVideoMemory()))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}


// lwMesh
//LW_STD_IMPLEMENTATION(lwMesh)
LW_STD_GETINTERFACE(lwMesh)

LW_RESULT lwMesh::Release()
{
    LW_RESULT ret;

    if(_mt_flag == 1)
    {
        while(IsLoadingOK() == 0)
        {
            Sleep(1);
        }
    }


    if(LW_FAILED(ret = _res_mgr->UnregisterMesh(this)))
        goto __ret;

    if(_reg_id == LW_INVALID_INDEX)
    {
        Unload();
        LW_DELETE(this);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

lwMesh::lwMesh(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _state(RES_STATE_INVALID), _reg_id(LW_INVALID_INDEX),
  _vb_id(LW_INVALID_INDEX), _ib_id(LW_INVALID_INDEX), _stream_type(STREAM_STATIC),
  _svb(0), _sib(0)
{
    _mt_flag = 0;
    _colorkey = 0;

    lwMeshDataInfo_Construct(&_data_info);

    _mesh_info_ptr = &_mesh_info;
}

lwMesh::~lwMesh()
{
    lwMeshDataInfo_Destruct(&_data_info);
}

LW_RESULT lwMesh::Register()
{
    assert(0 && "invlaid call with lwMesh::Register");
    return _res_mgr->RegisterMesh(this);
}
LW_RESULT lwMesh::Unregister()
{
    return _res_mgr->UnregisterMesh(this);
}

LW_RESULT lwMesh::SetResFile(const lwResFileMesh* info) 
{     
    _res_file = *info;
    _state |= RES_STATE_INIT;

    return LW_RET_OK;
}

LW_RESULT lwMesh::LoadSystemMemory(const lwMeshInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;


    if(_state & RES_STATE_SYSTEMMEMORY)
        goto __addr_ret_ok;

    if(LW_FAILED(lwMeshInfo_Copy(&_mesh_info, info)))
        goto __ret;

    _mesh_info_ptr = &_mesh_info;

    lwMeshDataInfo_Destruct(&_data_info);
    lwMeshDataInfo_Construct(&_data_info);

    if(LW_FAILED(lwLoadMeshDataInfo(&_data_info, info)))
        goto __ret;

    _rsa_0.Load(info->rs_set, LW_MESH_RS_NUM);

    _state |= RES_STATE_SYSTEMMEMORY;

__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    return ret;

}

LW_RESULT lwMesh::LoadSystemMemoryMT(const lwMeshInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;


    if(_state & RES_STATE_SYSTEMMEMORY)
        goto __addr_ret_ok;

    //if(LW_FAILED(lwMeshInfo_Copy(&_mesh_info, info)))
    //    goto __ret;

    _mesh_info_ptr = const_cast<lwMeshInfo*>(info);

    _rsa_0.Load(info->rs_set, LW_MESH_RS_NUM);

    _state |= RES_STATE_SYSTEMMEMORY;

__addr_ret_ok:
    ret = LW_RET_OK;

//__ret:
    return ret;

}

LW_RESULT lwMesh::LoadSystemMemory()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_state & RES_STATE_SYSTEMMEMORY)
        goto __addr_ret_ok;

    else if((_state & RES_STATE_INIT) == 0)
        goto __ret;

    __debugbreak();

    //if(LW_FAILED(lwLoadMeshInfoFromResFile(&_mesh_info, &_res_file)))
    //    goto __ret;
    
__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwMesh::LoadVideoMemory()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();
    lwIStaticStreamMgr* ssm;
    lwILockableStreamMgr* lsm;

    if(_state & RES_STATE_VIDEOMEMORY)
        goto __addr_ret_ok;

    else if((_state & RES_STATE_SYSTEMMEMORY) == 0)
    {
        if(LW_FAILED(LoadSystemMemory()))
            goto __ret;
    }

    switch(_stream_type)
    {
    case STREAM_GENERIC:
        if(_data_info.vb_size > 0)
        {
            if(LW_FAILED(dev_obj->CreateVertexBuffer(&_svb)))
                goto __ret;

            if(LW_FAILED(_svb->Create(_data_info.vb_size, 0, _mesh_info_ptr->fvf, D3DPOOL_DEFAULT, _data_info.vb_stride, NULL)))
                goto __ret;

            if(LW_FAILED(_svb->LoadData(_data_info.vb_data, _data_info.vb_size, 0, 0)))
                goto __ret;
        }
        if(_data_info.ib_size > 0)
        {
            if(LW_FAILED(dev_obj->CreateIndexBuffer(&_sib)))
                goto __ret;

            if(LW_FAILED(_sib->Create(_data_info.ib_size, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, NULL)))
                goto __ret;

            if(LW_FAILED(_sib->LoadData(_data_info.ib_data, _data_info.ib_size, 0, 0)))
                goto __ret;
        }

        break;
    case STREAM_STATIC:
        ssm =  _res_mgr->GetStaticStreamMgr();

        if(LW_FAILED(ssm->RegisterVertexBuffer(&_vb_id, _data_info.vb_data, _data_info.vb_size, _data_info.vb_stride)))
            goto __ret;

        if(_data_info.ib_size > 0)
        {
            if(LW_FAILED(ssm->RegisterIndexBuffer(&_ib_id, _data_info.ib_data, _data_info.ib_size, _data_info.ib_stride)))
                goto __ret;
        }
        break;
    case STREAM_LOCKABLE:
        lsm = _res_mgr->GetLockableStreamMgr();

        if(LW_FAILED(lsm->RegisterVertexBuffer(&_vb_id, _data_info.vb_data, _data_info.vb_size, D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY, _mesh_info_ptr->fvf)))
            goto __ret;

        if(_data_info.ib_size > 0)
        {
            if(LW_FAILED(lsm->RegisterIndexBuffer(&_ib_id, _data_info.ib_data, _data_info.ib_size, D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY, D3DFMT_INDEX16)))
                goto __ret;
        }
        break;
    default:
        LG_MSGBOX("invalid stream type called lwMesh::LoadVideoMemory");
    }

    _state |= RES_STATE_VIDEOMEMORY;

__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    _state |= RES_STATE_LOADTEST;
    return ret;
}

LW_RESULT lwMesh::LoadVideoMemoryMT()
{    

    SetStreamType(STREAM_GENERIC);

    lwIThreadPoolMgr* tp_mgr = _res_mgr->GetThreadPoolMgr();
    lwIThreadPool* tp = tp_mgr->GetThreadPool(THREAD_POOL_LOADRES);

    if(LW_FAILED(tp->RegisterTask(__thread_proc_load_mesh, (void*)this)))
        return LW_RET_FAILED;

    _mt_flag = 1;

    return LW_RET_OK;

}

LW_RESULT lwMesh::LoadVideoMemoryEx()
{

    if(_res_mgr->GetByteSet()->GetValue(OPT_RESMGR_LOADMESH_MT) == 1)
    {
        return LoadVideoMemoryMT();
    }
    else
    {
#if(defined LOAD_VERTEX_RESOURCE_DIRECTLY)
        return LoadVideoMemory();
#else
        return LW_RET_OK;
#endif
    }
}

LW_RESULT lwMesh::UnloadSystemMemory()
{
    LW_RESULT ret = LW_RET_FAILED;

    // warning: mesh data cannot free system memory
    goto __addr_ret_ok;

    if((_state & RES_STATE_SYSTEMMEMORY) == 0)
        goto __addr_ret_ok;

    _state &= ~RES_STATE_SYSTEMMEMORY;

__addr_ret_ok:
    ret = LW_RET_OK;

//__ret:
    return ret;

}
LW_RESULT lwMesh::UnloadVideoMemory()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();
    lwIStaticStreamMgr* ssm;
    lwILockableStreamMgr* lsm;

    if((_state & RES_STATE_VIDEOMEMORY) == 0)
    {
        ret = LW_RET_OK;
        goto __ret;
    }

    switch(_stream_type)
    {
    case STREAM_GENERIC:
        LW_SAFE_RELEASE(_svb);
        LW_SAFE_RELEASE(_sib);
        break;
    case STREAM_STATIC:
        ssm = _res_mgr->GetStaticStreamMgr();

        if(_vb_id != LW_INVALID_INDEX)
        {
            if(LW_FAILED(ssm->UnregisterVertexBuffer(_vb_id)))
                goto __ret;

            _vb_id = LW_INVALID_INDEX;
        }

        if(_ib_id != LW_INVALID_INDEX)
        {
            if(LW_FAILED(ssm->UnregisterIndexBuffer(_ib_id)))
                goto __ret;

            _ib_id = LW_INVALID_INDEX;
        }
        break;
    case STREAM_LOCKABLE:
        lsm = _res_mgr->GetLockableStreamMgr();

        if(_vb_id != LW_INVALID_INDEX)
        {
            if(LW_FAILED(lsm->UnregisterVertexBuffer(_vb_id)))
                goto __ret;

            _vb_id = LW_INVALID_INDEX;
        }

        if(_ib_id != LW_INVALID_INDEX)
        {
            if(LW_FAILED(lsm->UnregisterIndexBuffer(_ib_id)))
                goto __ret;

            _ib_id = LW_INVALID_INDEX;
        }            
        break;
    default:
        LG_MSGBOX("invalid stream type called lwMesh::UnloadVideoMemory");
    }


    _state &= ~RES_STATE_VIDEOMEMORY;

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwMesh::Unload()
{
    LW_RESULT ret = LW_RET_OK;

    if(LW_FAILED(ret = UnloadVideoMemory()))
        goto __ret;

    if(LW_FAILED(ret = UnloadSystemMemory()))
        goto __ret;

__ret:
    return ret;
}


LW_RESULT lwMesh::BeginSet()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();
    lwIStaticStreamMgr* ssm;
    lwILockableStreamMgr* lsm;

//#if(defined MULTITHREAD_LOADING_MESH)
    if(IsLoadingOK() == 0)
        goto __ret_ok;
//#endif

    if(!(_state & RES_STATE_VIDEOMEMORY) && (_state | RES_STATE_INIT))
    {
        if(LW_FAILED(LoadVideoMemory()))
            goto __ret;
    }

    switch(_stream_type)
    {
    case STREAM_GENERIC:
        if(_svb)
        {
            if(LW_FAILED(_svb->BindDevice(0, 0)))
                goto __ret;
        }
        if(_sib)
        {
            if(LW_FAILED(_sib->BindDevice()))
                goto __ret;
        }
        break;
    case STREAM_STATIC:
        ssm = _res_mgr->GetStaticStreamMgr();

        if(_vb_id != LW_INVALID_INDEX)
        {
            ssm->BindVertexBuffer(_vb_id, 0);
        }
        if(_ib_id != LW_INVALID_INDEX)
        {
            ssm->BindIndexBuffer(_ib_id);
        }
        break;
    case STREAM_LOCKABLE:
        lsm = _res_mgr->GetLockableStreamMgr();

        if(_vb_id != LW_INVALID_INDEX)
        {
            lsm->BindVertexBuffer(_vb_id, 0, 0, _data_info.vb_stride);
        }
        if(_ib_id != LW_INVALID_INDEX)
        {
            lsm->BindIndexBuffer(_ib_id, 0);
        }
        break;
    default:
        LG_MSGBOX("invalid stream type called lwMesh::BeginSet");
    }

    dev_obj->SetFVF(_mesh_info_ptr->fvf);

    _rsa_0.BeginRenderState(dev_obj, 0, LW_MESH_RS_NUM);

__ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwMesh::EndSet()
{
    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

    _rsa_0.EndRenderState(dev_obj, 0, LW_MESH_RS_NUM);

    return LW_RET_OK;
}

LW_RESULT lwMesh::DrawSubset(DWORD subset)
{
    LW_RESULT ret = LW_RET_FAILED;

    if((_state & RES_STATE_VIDEOMEMORY) == 0)
        goto __ret;

    {
        lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();
        lwIStaticStreamMgr* ssm;
        lwILockableStreamMgr* lsm;

        lwSubsetInfo* i = &_mesh_info_ptr->subset_seq[subset];

        DWORD start_index = 0;
        DWORD base_vert_index = 0;

        switch (_stream_type)
        {
        case STREAM_GENERIC:

            break;
        case STREAM_STATIC:
            ssm = _res_mgr->GetStaticStreamMgr();

            if (_ib_id == LW_INVALID_INDEX)
            {
                start_index = i->start_index + ssm->GetVertexEntityOffset();
            }
            else
            {
#if(defined LW_USE_DX9_STREAMOFFSET)
                base_vert_index = 0;
#else
                base_vert_index = ssm->GetVertexEntityOffset();
#endif
                start_index = i->start_index + ssm->GetIndexEntityOffset();
            }
            break;
        case STREAM_LOCKABLE:
            lsm = _res_mgr->GetLockableStreamMgr();

            if (_ib_id == LW_INVALID_INDEX)
            {
                start_index = i->start_index;
            }
            else
            {
#if(defined LW_USE_DX9_STREAMOFFSET)
                base_vert_index = 0;
#else
                base_vert_index = 0;
#endif
                start_index = i->start_index;
            }
            break;
        default:
            LG_MSGBOX("invalid stream type called lwMesh::DrawSubset");
        }


#if(defined USE_STENCILBUFFER_FILTER_COLORKEY)
        if (_colorkey)
        {
            dev_obj->GetDevice()->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);

            dev_obj->SetRenderState(D3DRS_STENCILENABLE, TRUE);
            dev_obj->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
            dev_obj->SetRenderState(D3DRS_STENCILREF, 1);
            dev_obj->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
            dev_obj->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

            dev_obj->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
            dev_obj->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
            dev_obj->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

            //dev_obj->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
            dev_obj->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
            dev_obj->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);

            dev_obj->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            dev_obj->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            dev_obj->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
            dev_obj->SetRenderState(D3DRS_ALPHAREF, 0);

            dev_obj->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
            dev_obj->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

            if (_ib_id == LW_INVALID_INDEX)
            {
                ret = dev_obj->DrawPrimitive(_mesh_info_ptr->pt_type, start_index, i->primitive_num);
            }
            else
            {
                ret = dev_obj->DrawIndexedPrimitive(_mesh_info_ptr->pt_type, base_vert_index, i->min_index, i->vertex_num, start_index, i->primitive_num);
            }


            dev_obj->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
            dev_obj->SetRenderState(D3DRS_ALPHAREF, 0);

            dev_obj->SetRenderState(D3DRS_STENCILREF, 0);
            dev_obj->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
            dev_obj->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
            dev_obj->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
            dev_obj->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
            dev_obj->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

            dev_obj->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            dev_obj->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

            //dev_obj->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
            //dev_obj->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
            //dev_obj->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);


            if (_ib_id == LW_INVALID_INDEX)
            {
                ret = dev_obj->DrawPrimitive(_mesh_info_ptr->pt_type, start_index, i->primitive_num);
            }
            else
            {
                ret = dev_obj->DrawIndexedPrimitive(_mesh_info_ptr->pt_type, base_vert_index, i->min_index, i->vertex_num, start_index, i->primitive_num);
            }

            dev_obj->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
            dev_obj->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
            dev_obj->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        }
        else
        {
            if (_ib_id == LW_INVALID_INDEX)
            {
                ret = dev_obj->DrawPrimitive(_mesh_info_ptr->pt_type, start_index, i->primitive_num);
            }
            else
            {
                ret = dev_obj->DrawIndexedPrimitive(_mesh_info_ptr->pt_type, base_vert_index, i->min_index, i->vertex_num, start_index, i->primitive_num);
            }
        }
#else
        if (_stream_type == STREAM_GENERIC)
        {
            if (_sib == 0)
            {
                ret = dev_obj->DrawPrimitive(_mesh_info_ptr->pt_type, 0, i->primitive_num);
            }
            else
            {
                ret = dev_obj->DrawIndexedPrimitive(_mesh_info_ptr->pt_type, 0, i->min_index, i->vertex_num, i->start_index, i->primitive_num);
            }
        }
        else
        {
            if (_ib_id == LW_INVALID_INDEX)
            {
                ret = dev_obj->DrawPrimitive(_mesh_info_ptr->pt_type, start_index, i->primitive_num);
            }
            else
            {
                ret = dev_obj->DrawIndexedPrimitive(_mesh_info_ptr->pt_type, base_vert_index, i->min_index, i->vertex_num, start_index, i->primitive_num);
            }
        }
#endif
	}
__ret:
    return ret;
}

LW_RESULT lwMesh::LoseDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    switch(_stream_type)
    {
    case STREAM_GENERIC:
    case STREAM_STATIC:
        if(LW_FAILED(UnloadVideoMemory()))
            goto __ret;
        break;
    case STREAM_LOCKABLE:
        break;
    default:
        assert(0);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwMesh::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    switch(_stream_type)
    {
    case STREAM_GENERIC:
    case STREAM_STATIC:
        if(LW_FAILED(LoadVideoMemory()))
            goto __ret;
        break;
    case STREAM_LOCKABLE:
        break;
    default:
        assert(0);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

lwILockableStreamVB* lwMesh::GetLockableStreamVB()
{
    lwILockableStreamVB* s = 0;

    if(_stream_type != STREAM_LOCKABLE || _vb_id == LW_INVALID_INDEX)
        goto __ret;

    {
        lwILockableStreamMgr* lsm = _res_mgr->GetLockableStreamMgr();

        s = lsm->GetStreamVB(_vb_id);
    }
__ret:
    return s;
}
lwILockableStreamIB* lwMesh::GetLockableStreamIB()
{
    lwILockableStreamIB* s = 0;

    if(_stream_type != STREAM_LOCKABLE || _vb_id == LW_INVALID_INDEX)
        goto __ret;
    {
        lwILockableStreamMgr* lsm = _res_mgr->GetLockableStreamMgr();

        s = lsm->GetStreamIB(_ib_id);
    }
__ret:
    return s;
}

BOOL lwMesh::IsLoadingOK() const
{
    return (_mt_flag == 0) || (_state & RES_STATE_VIDEOMEMORY);        
}

LW_RESULT lwMesh::ExtractMesh(lwMeshInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_FAILED(lwExtractMeshData(
        info, 
        _data_info.vb_data, 
        _data_info.ib_data, 
        _data_info.vb_size / _data_info.vb_stride, 
        _data_info.ib_size / _data_info.ib_stride,
        (D3DFORMAT)_mesh_info.fvf,
        D3DFMT_INDEX16
       )))
    {
        goto __ret;
    }

    info->fvf = _mesh_info.fvf;
    info->pt_type = _mesh_info.pt_type;
    info->subset_num = _mesh_info.subset_num;
    info->bone_index_num = _mesh_info.bone_index_num;
    memcpy(info->subset_seq, _mesh_info.subset_seq, sizeof(lwSubsetInfo) * info->subset_num);
    memcpy(info->bone_index_seq, _mesh_info.bone_index_seq, sizeof(BYTE) * info->bone_index_num);
    memcpy(&info->rs_set, _rsa_0.GetStateSeq(), sizeof(lwRenderStateAtom) * _rsa_0.GetStateNum());
    ret = LW_RET_OK;
__ret:
    return ret;
}

// lwMtlTexAgent
LW_STD_IMPLEMENTATION(lwMtlTexAgent)

lwMtlTexAgent::lwMtlTexAgent(lwIResourceMgr* mgr)
: _res_mgr(mgr), _opacity(1.0f), _transp_type(MTLTEX_TRANSP_FILTER)
{
    for(DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
    {
        _tex_seq[i] = NULL;
    }
    memset(_uvmat, 0, sizeof(_uvmat));
    memset(_tt_tex, 0, sizeof(_tt_tex));

    // opacity rsa init
#if(defined MTLTEXAGENT_OPACITY_RSA_FLAG)
    //RSA_VALUE(&_opacity_rsa_seq[0], D3DRS_ZWRITEENABLE, 1);
    //RSA_VALUE(&_opacity_rsa_seq[1], D3DRS_TEXTUREFACTOR, 0xffffffff);
    //RSA_VALUE(&_opacity_rsa_seq[2], D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    //RSA_VALUE(&_opacity_rsa_seq[3], D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    //RSA_VALUE(&_opacity_rsa_seq[4], D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    _rsa_opacity.Allocate(5);
    _rsa_opacity.SetStateValue(0, D3DRS_ZWRITEENABLE, 1);
    _rsa_opacity.SetStateValue(1, D3DRS_TEXTUREFACTOR, 0xffffffff);
    _rsa_opacity.SetStateValue(2, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    _rsa_opacity.SetStateValue(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    _rsa_opacity.SetStateValue(4, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

#endif

    _opacity_reserve_rs[0] = LW_INVALID_INDEX;
    _opacity_reserve_rs[1] = LW_INVALID_INDEX;

    _rsa_0.Allocate(RSA_SET_SIZE);

    _render_flag = TRUE;

}
lwMtlTexAgent::~lwMtlTexAgent()
{
    Destroy();
}

LW_RESULT lwMtlTexAgent::SetTex(DWORD stage, lwITex* obj, lwITex** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(stage < 0 || stage >= LW_MAX_MTL_TEX_NUM)
        goto __ret;

    *ret_obj = _tex_seq[stage];
    _tex_seq[stage] = obj;

    ret = LW_RET_OK;
__ret:
    return ret;
}


LW_RESULT lwMtlTexAgent::LoadMtlTex(lwMtlTexInfo* info, const char* tex_path)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwMtlTexInfo* i = (lwMtlTexInfo*)info;

    _mtl = i->mtl;
    _opacity = i->opacity;
    _transp_type = i->transp_type;

    //SetMtlRenderState(&info->rs_set);
    _rsa_0.Load(info->rs_set, LW_MTL_RS_NUM);

    for(DWORD j = 0; j < LW_MAX_MTL_TEX_NUM; j++)
    {
        if(i->tex_seq[j].stage == LW_INVALID_INDEX)
            continue;

        if(LW_FAILED(LoadTextureStage(&i->tex_seq[j], tex_path)))
            goto __ret;
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwMtlTexAgent::ExtractMtlTex(lwMtlTexInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    info->mtl = _mtl;
    info->opacity = _opacity;
    info->transp_type = _transp_type;

    memcpy(info->rs_set, _rsa_0.GetStateSeq(), sizeof(lwRenderStateAtom) * _rsa_0.GetStateNum());

    for(DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
    {
        if(_tex_seq[i] == 0)
            break;

        _tex_seq[i]->GetTexInfo(&info->tex_seq[i]);

        // here , we discard the path of texture file
        char buf[260];
        _tcscpy(buf, info->tex_seq[i].file_name);
        char* p = _tcsrchr(buf, '\\');
        if(p)
        {
            _tcscpy(info->tex_seq[i].file_name, &p[1]);
        }
    }

    return LW_RET_OK;
}


LW_RESULT lwMtlTexAgent::LoadTextureStage(const lwTexInfo* info, const char* tex_path)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwITex* obj;

    DWORD stage = info->stage;

    if(stage < 0 || stage >= LW_MAX_MTL_TEX_NUM)
        goto __ret;

    if(_tex_seq[stage] != NULL)
        goto __ret;

    if(LW_FAILED(_res_mgr->CreateTex(&obj)))
        goto __ret;
    
    if(LW_FAILED(obj->LoadTexInfo(info, tex_path)))
        goto __ret;

    DWORD load_type;

    if(_res_mgr->GetByteSet()->GetValue(OPT_RESMGR_LOADTEXTURE_MT) == 1)
    {
        load_type = LOADINGRES_TYPE_RUNTIME_MT;
    }
    else
    {
        load_type = LOADINGRES_TYPE_RUNTIME;
    }

    obj->SetLoadResType(load_type);

 
    _tex_seq[stage] = obj;


    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwMtlTexAgent::BeginPass()
{
    LW_RESULT r;
    LW_RESULT ret = RES_PASS_DEFAULT;

    lwITex* t;
    for(DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
    {
        if((t = _tex_seq[i]) == NULL)
            break;
        
        if(_tt_tex[i])
        {
            t = _tt_tex[i];
        }

        r = t->BeginPass();

        if(r == RES_PASS_ERROR)
        {
            ret = RES_PASS_ERROR;
        }
        else if((r == RES_PASS_SKIPTHISDRAW) && (ret == RES_PASS_DEFAULT))
        {
            ret = RES_PASS_SKIPTHISDRAW;
        }
    }

    return ret;
}

LW_RESULT lwMtlTexAgent::EndPass()
{
    return LW_RET_OK;
}

LW_RESULT lwMtlTexAgent::DestroyTextureStage(DWORD stage)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_tex_seq[stage] == 0)
        goto __addr_ok;

    //if(_tex_seq[stage]->GetMTFlag() == 1)
    //{
    //    while(_tex_seq[stage]->IsLoadingOK() == 0)
    //    {
    //        ::Sleep(10);
    //    }
    //}

    if(LW_FAILED(_tex_seq[stage]->Release()))
        goto __ret;

    _tex_seq[stage] = NULL;

__addr_ok:
    ret = LW_RET_OK;

__ret:
    return ret;
}


LW_RESULT lwMtlTexAgent::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
    {
        if(LW_FAILED(DestroyTextureStage(i)))
        {
            assert(0 && "call lwMtlTexAgent::Destroy error");
            goto __ret;
        }

        LW_SAFE_DELETE(_uvmat[i]);
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}


LW_RESULT lwMtlTexAgent::BeginSet()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();
    lwITex* tex;    

    dev_obj->SetMaterial(&_mtl);

    //dev_obj->SetTexture(0, NULL);

    if(_transp_type!= MTLTEX_TRANSP_FILTER)
    {
        //lwRenderStateAtom* rsa;
        // src, dest
        DWORD id[2] = { LW_INVALID_INDEX, LW_INVALID_INDEX };

        _rsa_0.FindState(&id[0], D3DRS_SRCBLEND);
        _rsa_0.FindState(&id[1], D3DRS_DESTBLEND);

        DWORD v[2] = {D3DBLEND_ONE, D3DBLEND_ONE};
        switch(_transp_type)
        {
        case MTLTEX_TRANSP_ADDITIVE:
            v[0] = D3DBLEND_ONE;
            v[1] = D3DBLEND_ONE;
            break;
        case MTLTEX_TRANSP_ADDITIVE1: // hight
            v[0] = D3DBLEND_SRCCOLOR;
            v[1] = D3DBLEND_ONE;
            break;
        case MTLTEX_TRANSP_ADDITIVE2: // low
            v[0] = D3DBLEND_SRCCOLOR;
            v[1] = D3DBLEND_INVSRCCOLOR;
            break;
        case MTLTEX_TRANSP_ADDITIVE3: // low-high
            v[0] = D3DBLEND_SRCALPHA;
            v[1] = D3DBLEND_DESTALPHA;
            break;
        case MTLTEX_TRANSP_SUBTRACTIVE:
            v[0] = D3DBLEND_ZERO;
            v[1] = D3DBLEND_INVSRCCOLOR;
            break;
        }
        _rsa_0.SetValue(id[0], v[0]);
        _rsa_0.SetValue(id[1], v[1]);
    }

    // check opacity flag
    if(_opacity != 1.0f)
    {
#if(defined MTLTEXAGENT_OPACITY_RSA_FLAG)
        //RSA_VALUE(&_opacity_rsa_seq[1], D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((BYTE)(_opacity * 255), 0, 0, 0));
        //lwRenderStateAtomBeginSetRS(dev_obj, &_opacity_rsa_seq[0], 2);
        //lwRenderStateAtomBeginSetTSS(0, dev_obj, &_opacity_rsa_seq[2], 3);
        _rsa_opacity.SetStateValue(1, D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((BYTE)(_opacity * 255), 0, 0, 0));
        _rsa_opacity.BeginRenderState(dev_obj, 0, 5);
#else
        dev_obj->SetRenderState(D3DRS_ZWRITEENABLE, 0);
        dev_obj->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((BYTE)(_opacity * 255), 0, 0, 0));
        dev_obj->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        dev_obj->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
#endif

        if(_transp_type!= MTLTEX_TRANSP_FILTER)
        {
            lwRenderStateAtom* rsa;
            // src, dest
            DWORD id[2] = { LW_INVALID_INDEX, LW_INVALID_INDEX };

            _rsa_0.FindState(&id[0], D3DRS_SRCBLEND);
            _rsa_0.FindState(&id[1], D3DRS_DESTBLEND);

            if(_transp_type == MTLTEX_TRANSP_ADDITIVE)
            {
                if(id[0] != LW_INVALID_INDEX)
                {
                    _rsa_0.GetStateAtom(&rsa, id[0]);
                    _opacity_reserve_rs[0] = rsa->value0;
                    rsa->value0 = D3DBLEND_SRCALPHA;
                    rsa->value1 = D3DBLEND_SRCALPHA;                    
                }
            }
            else if(_transp_type == MTLTEX_TRANSP_SUBTRACTIVE)
            {
                // reserved.
            }
        }
    }

    for(DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
    {
        if((tex = _tex_seq[i]) == NULL)
            break;

        // texture transform image sequence
        if(_tt_tex[i])
        {
            tex = _tt_tex[i];
        }

        if(LW_FAILED(tex->BeginSet()))
            goto __ret;

        // texture transform
        //if(_uvmat[i])
        //{
        //    dev_obj->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + i), _uvmat[i]);
        //    dev_obj->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
        //}
    }

    {
        DWORD v = 129;
        lwRenderStateAtom* rsa;
        for (DWORD i = 0; i < _rsa_0.GetStateNum(); i++)
        {
            rsa = &_rsa_0.GetStateSeq()[i];

            if (rsa->state == LW_INVALID_INDEX)
                break;

            if (rsa->state == D3DRS_ALPHAREF)
            {
                if (_opacity != 1.0f)
                {
                    v = (DWORD)(_opacity * 255) - 1;
                    if (v < 0)
                        v = 0;
                    if (v > 129)
                        v = 129;
                }

                _rsa_0.SetStateValue(i, D3DRS_ALPHAREF, v);
            }
        }
        _rsa_0.BeginRenderState(dev_obj, 0, RSA_SET_SIZE);

    }
    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwMtlTexAgent::EndSet()
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();
    lwITex* tex;

    // check opacity flag
    if(_opacity != 1.0f)
    {
#if(defined MTLTEXAGENT_OPACITY_RSA_FLAG)
        //lwRenderStateAtomEndSetRS(dev_obj, &_opacity_rsa_seq[0], 2);
        //lwRenderStateAtomEndSetTSS(0, dev_obj, &_opacity_rsa_seq[2], 3);
        _rsa_opacity.EndRenderState(dev_obj, 0, 5);
#else
        dev_obj->SetRenderState(D3DRS_ZWRITEENABLE, 1);
        dev_obj->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(0xff, 0, 0, 0));
        dev_obj->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
        //dev_obj->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
#endif

        if(_transp_type!= MTLTEX_TRANSP_FILTER)
        {
            if(_opacity_reserve_rs[0] != LW_INVALID_INDEX)
            {
                _rsa_0.ResetStateValue(D3DRS_SRCBLEND, _opacity_reserve_rs[0], 0);
                _opacity_reserve_rs[0] = LW_INVALID_INDEX;
            }
            if(_opacity_reserve_rs[1] != LW_INVALID_INDEX)
            {
                _rsa_0.ResetStateValue(D3DRS_DESTBLEND, _opacity_reserve_rs[1], 0);
                _opacity_reserve_rs[1] = LW_INVALID_INDEX;
            }
        }
    }

    for(DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
    {
        if((tex = _tex_seq[i]) == NULL)
            break;

        if(LW_FAILED(tex->EndSet()))
            goto __ret;

        // texture transform
        //if(_uvmat[i])
        //{
        //    dev_obj->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        //}

    }

    _rsa_0.EndRenderState(dev_obj, 0, RSA_SET_SIZE);

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwMtlTexAgent::Clone(lwIMtlTexAgent** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwMtlTexAgent* o;

    if(LW_FAILED(_res_mgr->CreateMtlTexAgent(reinterpret_cast< lwIMtlTexAgent** >(&o))))
        goto __ret;
    {
        o->_opacity = _opacity;
        o->_transp_type = _transp_type;
        o->_mtl = _mtl;
        o->_rsa_0.Load(_rsa_0.GetStateSeq(), _rsa_0.GetStateNum());

        for (DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
        {
            if (_tex_seq[i])
            {
                o->_tex_seq[i] = _tex_seq[i];
                _res_mgr->AddRefTex(_tex_seq[i], 1);
            }
            if (_uvmat[i])
            {
                o->_uvmat[i] = LW_NEW(lwMatrix44);
                *o->_uvmat[i] = *_uvmat[i];
            }
        }

        *ret_obj = o;

    }
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwMtlTexAgent::SetTextureTransformMatrix(DWORD stage, const lwMatrix44* mat)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(stage < 0 || stage >= LW_MAX_MTL_TEX_NUM)
        goto __ret;

    if(mat)
    {
        if(_uvmat[stage] == NULL)
        {
            _uvmat[stage] = LW_NEW(lwMatrix44);
        }
        *_uvmat[stage] = *mat;
    }
    else
    {
        LW_SAFE_DELETE(_uvmat[stage]);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwMtlTexAgent::SetTextureTransformImage(DWORD stage, lwITex* tex)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(stage < 0 || stage >= LW_MAX_MTL_TEX_NUM)
        goto __ret;

    _tt_tex[stage] = tex;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwMtlTexAgent::SetTextureLOD(DWORD level)
{
    lwITex* tex;

    for(DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
    {
        if((tex = _tex_seq[i]) == NULL)
            break;
        
        tex->SetLOD(level);
    }

    return LW_RET_OK;
}

BOOL lwMtlTexAgent::IsTextureLoadingOK() const
{
    BOOL ret = 1;

    lwITex* t;
    for(DWORD i = 0; i < LW_MAX_MTL_TEX_NUM; i++)
    {
        if((t = _tex_seq[i]) == NULL)
            break;
        
        if(_tt_tex[i])
        {
            t = _tt_tex[i];
        }

        if((t->GetMTFlag() == 1) && (t->IsLoadingOK() == 0))
            return 0;
    }

    return 1;
}

// lwMeshAgent
LW_STD_IMPLEMENTATION(lwMeshAgent)

lwMeshAgent::lwMeshAgent(lwIResourceMgr* res_mgr)
: _res_mgr(res_mgr), _mesh_obj(0)
{
    _mt_flag = 0;
    lwRenderStateSetTemplate_Construct(&_rs_set);
}
lwMeshAgent::~lwMeshAgent()
{
    Destroy();
}

LW_RESULT lwMeshAgent::LoadMesh(const lwMeshInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIMesh* obj = 0;
    
    if(LW_FAILED(_res_mgr->CreateMesh(&obj)))
        goto __ret;

    if(_mt_flag == 1)
    {
        if(LW_FAILED(obj->LoadSystemMemoryMT(info)))
            goto __ret;
    }
    else
    {
        if(LW_FAILED(obj->LoadSystemMemory(info)))
            goto __ret;
    }



    if(LW_FAILED(obj->LoadVideoMemoryEx()))
        goto __ret;

    _mesh_obj = obj;

    ret = LW_RET_OK;

__ret:
    return ret;

}
LW_RESULT lwMeshAgent::LoadMesh(const lwResFileMesh* info)
{
    __debugbreak();

    LW_RESULT ret = LW_RET_FAILED;

    lwMeshInfo i;

    if(LW_FAILED(LoadMesh(&i)))
        goto __ret;

    if(LW_FAILED(_mesh_obj->SetResFile(info)))
        goto __ret;


    ret = LW_RET_OK;

__ret:
    return ret;

}

LW_RESULT lwMeshAgent::DestroyMesh()
{
    LW_RESULT ret = LW_RET_FAILED;
//
//#if(defined MULTITHREAD_LOADING_MESH)
//
//    while(_mesh_obj->IsLoadingOK() == 0)
//    {
//        ::Sleep(10);
//    }
//#endif

    if(_mesh_obj == 0)
        goto __addr_ret_ok;

    if(LW_FAILED(_mesh_obj->Release()))
        goto __ret;

    _mesh_obj = 0;

__addr_ret_ok:
    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwMeshAgent::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;
    
    if(LW_FAILED(DestroyMesh()))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwMeshAgent::Clone(lwIMeshAgent** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIMeshAgent* o = NULL;
    
    if(LW_FAILED(_res_mgr->CreateMeshAgent(&o)))
        goto __ret;

    if(_mesh_obj)
    {
        _res_mgr->AddRefMesh(_mesh_obj, 1);
        o->SetMesh(_mesh_obj);
    }

    o->SetRenderState(&_rs_set);

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwMeshAgent::BeginSet()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(LW_FAILED(_mesh_obj->BeginSet()))
        goto __ret;

    {
        lwRenderStateValue* rsv;
        lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

        // render state
        for (DWORD i = 0; i < _rs_set.SEQUENCE_SIZE; i++)
        {
            rsv = &_rs_set.rsv_seq[0][i];

            if (rsv->state == LW_INVALID_INDEX)
                break;

            dev_obj->SetRenderState((D3DRENDERSTATETYPE)rsv->state, rsv->value);
        }
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwMeshAgent::EndSet()
{
    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

    _mesh_obj->EndSet();

    lwRenderStateValue* rsv;

    // render state
    for(DWORD i = 0; i < _rs_set.SEQUENCE_SIZE; i++)
    {
        rsv = &_rs_set.rsv_seq[1][i];

        if(rsv->state == LW_INVALID_INDEX)
            break;

        dev_obj->SetRenderState((D3DRENDERSTATETYPE)rsv->state, rsv->value);
    }

    return LW_RET_OK;
}

LW_RESULT lwMeshAgent::DrawSubset(DWORD subset)
{
    return _mesh_obj->DrawSubset(subset);
}



// lwResBufMgr
LW_STD_IMPLEMENTATION(lwResBufMgr)

lwResBufMgr::lwResBufMgr(lwIResourceMgr* res_mgr)
:_res_mgr(res_mgr)
{
    _lock_sysmemtex.Create();

    // auto-handle нумеруется от 0x80000000, чтобы не пересекаться с внешними
    // model_id (приходят из SceneObjRecordStore и по факту укладываются в DWORD
    // куда меньше этого значения).
    _next_modelobj_auto_handle = 0x80000000u;

    _modelobj_data_size = 0;
    _sysmemtex_data_size = 0;

    _lmt_modelobj_data_size = 0xffffffff;
    _lmt_modelobj_data_time = 0;
}
lwResBufMgr::~lwResBufMgr()
{
    Destroy();

    _lock_sysmemtex.Destroy();
}

LW_RESULT lwResBufMgr::Destroy()
{
    // sys_mem_tex — собираем handles в вектор, чтобы не модифицировать slot-map
    // во время итерации (UnregisterSysMemTex зовёт _pool_sysmemtex.Unregister).
    {
        std::vector<DWORD> handles;
        handles.reserve(_pool_sysmemtex.GetObjNum());
        _pool_sysmemtex.ForEach([&](DWORD h, void*) {
            handles.push_back(h);
        });
        for (DWORD h : handles) {
            UnregisterSysMemTex(h);
        }
        _pool_sysmemtex.Clear();
    }

    // model_obj — тоже снимаем пачкой ключей, UnregisterModelObjInfo удаляет запись.
    {
        std::vector<DWORD> handles;
        handles.reserve(_pool_modelobj.size());
        for (const auto& kv : _pool_modelobj) {
            handles.push_back(kv.first);
        }
        for (DWORD h : handles) {
            UnregisterModelObjInfo(h);
        }
        _pool_modelobj.clear();
    }

    return LW_RET_OK;
}
LW_RESULT lwResBufMgr::RegisterSysMemTex(LW_HANDLE* handle, const lwSysMemTexInfo* info)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

    lwSysMemTexInfo* o = LW_NEW(lwSysMemTexInfo);
    memset(o, 0, sizeof(lwSysMemTexInfo));

#if(defined USE_D3DX_LOAD_TEXTURE_UPDATETEXTURE)

    IDirect3DTextureX* tex;

    if(FAILED(D3DXCreateTextureFromFileEx(dev_obj->GetDevice(),
        info->file_name, //
        0, //
        0, //
        info->level, //mipmap1
        0, //
        (D3DFORMAT)info->format, //
        D3DPOOL_SYSTEMMEM,//forced
        info->filter, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, //
        info->mip_filter,//D3DX_DEFAULT,//D3DX_FILTER_NONE, //mipmap
        info->colorkey,//0xffff00ff, //
        NULL, //
        NULL, //
        &tex)))//
    {
        goto __ret;
    }

    D3DSURFACE_DESC desc;
    tex->GetLevelDesc(0, &desc);

    o->colorkey = info->colorkey;
    o->format = info->format;
    o->filter = info->filter;
    o->mip_filter = info->mip_filter;
    o->level = info->level;
    _tcscpy(o->file_name, info->file_name);
    o->usage = 0;
    o->sys_mem_tex = tex;
    o->width = desc.Width;
    o->height = desc.Height;
#endif

#if(defined USE_D3DX_LOAD_TEXTURE_CREATEFROMMEMORY)


    o->buf = LW_NEW(lwBuffer);
    if(LW_FAILED(LoadFileInMemory(o->buf, info->file_name, "rb")))
        goto __ret;

    if(_res_mgr->GetByteSet()->GetValue(OPT_RESMGR_TEXENCODE))
    {
        lwTexEncode te;
        te.Decode(o->buf);
    }

    o->colorkey = info->colorkey;
    o->format = info->format;
    o->filter = info->filter;
    o->mip_filter = info->mip_filter;
    o->level = info->level;
    _tcscpy(o->file_name, info->file_name);
    o->usage = 0;

#endif

//#if(defined MULTITHREAD_LOADING_TEXTURE)
//    // critical section
//    lwIThreadPoolMgr* tp_mgr = _res_mgr->GetThreadPoolMgr();
//    tp_mgr->LockCriticalSection(CRITICALSECTION_LOCK_SYSMEMTEX);
//
//    if(LW_FAILED(_pool_sysmemtex.Register(handle, o)))
//    {
//        tp_mgr->UnlockCriticalSection(CRITICALSECTION_LOCK_SYSMEMTEX);
//        goto __ret;
//    }
//
//    tp_mgr->UnlockCriticalSection(CRITICALSECTION_LOCK_SYSMEMTEX);
//#else
//    if(LW_FAILED(_pool_sysmemtex.Register(handle, o)))
//        goto __ret;
//#endif

    {
        _lock_sysmemtex.Lock();

        if(LW_FAILED(_pool_sysmemtex.Register(handle, o)))
        {
            _lock_sysmemtex.Unlock();
            goto __ret;
        }

        _lock_sysmemtex.Unlock();
    }

    o = 0;

    ret = LW_RET_OK;
__ret:
    LW_IF_DELETE(o);
    return ret;

}
LW_RESULT lwResBufMgr::QuerySysMemTex(lwSysMemTexInfo** info, const char* file)
{
    lwSysMemTexInfo* found = nullptr;
    _pool_sysmemtex.ForEach([&](DWORD, void* raw) -> bool {
        auto* obj = static_cast<lwSysMemTexInfo*>(raw);
        if (_tcscmp(obj->file_name, file) == 0) {
            found = obj;
            return false;
        }
        return true;
    });
    if (found) {
        *info = found;
        return LW_RET_OK;
    }
    return LW_RET_FAILED;
}

LW_RESULT lwResBufMgr::QuerySysMemTex(lwSysMemTexInfo* info)
{
    lwSysMemTexInfo* found = nullptr;
    _pool_sysmemtex.ForEach([&](DWORD, void* raw) -> bool {
        auto* obj = static_cast<lwSysMemTexInfo*>(raw);
        if (_tcscmp(obj->file_name, info->file_name) == 0
            && obj->format == info->format
            && obj->level == info->level
            && obj->colorkey == info->colorkey) {
            found = obj;
            return false;
        }
        return true;
    });
    if (found) {
        *info = *found;
        return LW_RET_OK;
    }
    return LW_RET_FAILED;
}
LW_RESULT lwResBufMgr::GetSysMemTex(lwSysMemTexInfo** info, LW_HANDLE handle)
{
    return _pool_sysmemtex.GetObj((void**)info, handle);
}
LW_RESULT lwResBufMgr::UnregisterSysMemTex(LW_HANDLE handle)
{
    lwSysMemTexInfo* obj = 0;
    if(LW_FAILED(_pool_sysmemtex.Unregister((void**)&obj, handle)))
        return LW_RET_FAILED;

    LW_IF_RELEASE(obj->sys_mem_tex);

    LW_IF_RELEASE(obj->buf);
    LW_DELETE(obj);

    return LW_RET_OK;
}

LW_RESULT lwResBufMgr::RegisterModelObjInfo(LW_HANDLE* handle, const char* file)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwModelObjInfoMap* moim = LW_NEW(lwModelObjInfoMap);

    if (LW_FAILED(moim->info.Load(file)))
        goto __ret;

    // Берём свободный auto-handle в пространстве 0x80000000+ (не пересекается
    // с внешними model_id, которые приходят ниже).
    while (_pool_modelobj.count(_next_modelobj_auto_handle) != 0) {
        ++_next_modelobj_auto_handle;
        if (_next_modelobj_auto_handle == 0) {
            _next_modelobj_auto_handle = 0x80000000u;
        }
    }
    *handle = _next_modelobj_auto_handle++;
    _pool_modelobj.emplace(*handle, moim);

    {
        lwITimer* tm = 0;
        _res_mgr->GetSysGraphics()->GetSystem()->GetInterface((LW_VOID**)&tm, LW_GUID_TIMER);
        moim->hit_time = tm->GetTickCount();

        moim->size = moim->info.GetDataSize();
        _tcscpy(moim->file, file);

        _modelobj_data_size += moim->size;

        moim = 0;
    }
    ret = LW_RET_OK;
__ret:
    LW_IF_DELETE(moim);
    return ret;
}
LW_RESULT lwResBufMgr::RegisterModelObjInfo(LW_HANDLE handle, const char* file)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwModelObjInfoMap* moim = LW_NEW(lwModelObjInfoMap);

    if (LW_FAILED(moim->info.Load(file)))
        goto __ret;

    if (!_pool_modelobj.emplace(handle, moim).second)
        goto __ret;

    {
        lwITimer* tm = 0;
        _res_mgr->GetSysGraphics()->GetSystem()->GetInterface((LW_VOID**)&tm, LW_GUID_TIMER);
        moim->hit_time = tm->GetTickCount();

        moim->size = moim->info.GetDataSize();
        _tcscpy(moim->file, file);

        _modelobj_data_size += moim->size;

        moim = 0;
    }
    ret = LW_RET_OK;
__ret:
    LW_IF_DELETE(moim);
    return ret;
}

LW_RESULT lwResBufMgr::QueryModelObjInfo(lwIModelObjInfo** info, const char* file)
{
    for (const auto& kv : _pool_modelobj) {
        if (_tcscmp(kv.second->file, file) == 0) {
            *info = &kv.second->info;
            return LW_RET_OK;
        }
    }
    return LW_RET_FAILED;
}
LW_RESULT lwResBufMgr::GetModelObjInfo(lwIModelObjInfo** info, LW_HANDLE handle)
{
    auto it = _pool_modelobj.find(handle);
    if (it == _pool_modelobj.end()) {
        return LW_RET_FAILED;
    }
    *info = &it->second->info;
    return LW_RET_OK;
}
LW_RESULT lwResBufMgr::UnregisterModelObjInfo(LW_HANDLE handle)
{
    auto it = _pool_modelobj.find(handle);
    if (it == _pool_modelobj.end()) {
        return LW_RET_FAILED;
    }

    lwModelObjInfoMap* obj = it->second;
    _modelobj_data_size -= obj->size;
    _pool_modelobj.erase(it);

    LW_DELETE(obj);
    return LW_RET_OK;
}

LW_RESULT lwResBufMgr::FilterModelObjInfoSize()
{
    if (_lmt_modelobj_data_size >= _modelobj_data_size) {
        return LW_RET_OK;
    }

    lwITimer* tm = 0;
    _res_mgr->GetSysGraphics()->GetSystem()->GetInterface((LW_VOID**)&tm, LW_GUID_TIMER);
    const DWORD this_time = tm->GetTickCount();

    std::vector<DWORD> to_erase;
    to_erase.reserve(_pool_modelobj.size());
    for (const auto& kv : _pool_modelobj) {
        if ((this_time - kv.second->hit_time) > _lmt_modelobj_data_time) {
            to_erase.push_back(kv.first);
        }
    }
    for (DWORD h : to_erase) {
        if (LW_FAILED(UnregisterModelObjInfo(h))) {
            return LW_RET_FAILED;
        }
    }
    return LW_RET_OK;
}

// lwThreadPoolMgr
LW_STD_IMPLEMENTATION(lwThreadPoolMgr)

lwThreadPoolMgr::lwThreadPoolMgr()
{
    memset(_pool_seq, 0, sizeof(_pool_seq));
}

lwThreadPoolMgr::~lwThreadPoolMgr()
{
    Destroy();
}

LW_RESULT lwThreadPoolMgr::Create()
{
    LW_RESULT ret = LW_RET_FAILED;


    DWORD ci[THREAD_POOL_SIZE][2] =
    {
        { 2, 1024 },
		//{ 2, 1024 },
    };

    for(DWORD i = 0; i < THREAD_POOL_SIZE; i++)
    {
        _pool_seq[i] = LW_NEW(lwThreadPool);
        if(!_pool_seq[i]) {
            goto __ret;
        }

        if(LW_FAILED(_pool_seq[i]->Create(ci[i][0], ci[i][1], 0)))
            goto __ret;

        for(DWORD j = 0; j < ci[i][0]; j++)
        {
            DWORD id = ((lwThreadPool*)_pool_seq[i])->GetThreadId(j);
            ToLogService("common", "{}:{}", id, "MindPower::LoadResource Thread");
        }
    }

    _pool_seq[THREAD_POOL_LOADRES]->SetPriority(THREAD_PRIORITY_NORMAL);

    for(DWORD i = 0; i < CRITICALSECTION_SEQ_SIZE; i++)
    {
        ::InitializeCriticalSection(&_cs_seq[i]);
    }

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwThreadPoolMgr::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;

    for(DWORD i = 0; i < THREAD_POOL_SIZE; i++)
    {
        if(_pool_seq[i] == 0)
            continue;

        if(LW_FAILED(_pool_seq[i]->Destroy()))
            goto __ret;

        _pool_seq[i]->Release();
    }

    for(DWORD i = 0; i < CRITICALSECTION_SEQ_SIZE; i++)
    {
        ::DeleteCriticalSection(&_cs_seq[i]);
    }

    ret = LW_RET_OK;
__ret:
    return ret;

}

// lwResourceMgr
LW_STD_IMPLEMENTATION(lwResourceMgr)

lwResourceMgr::lwResourceMgr(lwISysGraphics* sys)
: _sys_graphics(sys), _mesh_size_sm(0), _mesh_size_vm(0), _tex_size_sm(0), _tex_size_vm(0),
  _shader_mgr(0), _thread_pool_mgr(0)
{

    _dev_obj = _sys_graphics->GetDeviceObject();

    _static_stream_mgr = LW_NEW(lwStaticStreamMgr(_dev_obj));
    _dynamic_stream_mgr = LW_NEW(lwDynamicStreamMgr(_dev_obj));
    _lockable_stream_mgr = LW_NEW(lwLockableStreamMgr(this));
    _surface_stream_mgr = LW_NEW(lwSurfaceStreamMgr(this));

    _shader_mgr = LW_NEW(lwShaderMgr(_dev_obj));

#if(defined LW_USE_DX9)
    _shader_mgr->Init(1024, 1024, 0);
#endif

    _resbuf_mgr = LW_NEW(lwResBufMgr(this));

//#if(defined MULTITHREAD_LOADING_TEXTURE || defined MULTITHREAD_LOADING_MESH)
    _thread_pool_mgr = LW_NEW(lwThreadPoolMgr());
    _thread_pool_mgr->Create();
//#endif

    {
        memset(_render_ctrl_proc_seq, 0, sizeof(_render_ctrl_proc_seq));
        lwInitInternalRenderCtrlVSProc(this);
    }

    _texture_path[0] = '\0';

    { // mutithread loading res option flag
        _byte_set.Alloc(OPT_RESMGR_BYTESET_SIZE);
        _byte_set.SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
        _byte_set.SetValue(OPT_RESMGR_LOADMESH_MT, 0);
        _byte_set.SetValue(OPT_RESMGR_TEXENCODE, 0);        
        _byte_set.SetValue(OPT_CREATE_ASSISTANTOBJECT, 0);
    }

    { // init assistant object info data
        _assobj_info.color = 0xffffffff;
        _assobj_info.size = lwVector3(1.0f, 1.0f, 1.0f);
    }

#if(defined USE_TEXLOG_MGR)
    _texlog_mgr.OpenLogFile(".\\log\\game\\tex.log");
    _texlog_mgr.AddTexType("scene");
    _texlog_mgr.AddTexType("character");
    _texlog_mgr.AddTexType("item");
    _texlog_mgr.AddTexType("ui");
    _texlog_mgr.AddTexType("terrain");
    _texlog_mgr.AddTexType("effect");
    _texlog_mgr.AddTexType("minimap");
	//_texlog_mgr.AddTexType("bone");
#endif

}

lwResourceMgr::~lwResourceMgr()
{
    ReleaseObject();

    ClearAllMesh();
    ClearAllTex();
    ClearAllAnimCtrl();

    LW_IF_RELEASE(_static_stream_mgr);
    LW_IF_RELEASE(_dynamic_stream_mgr);
    LW_IF_RELEASE(_lockable_stream_mgr);
    LW_IF_RELEASE(_surface_stream_mgr);
    LW_IF_RELEASE(_shader_mgr);
    LW_IF_RELEASE(_resbuf_mgr);
    LW_IF_RELEASE(_thread_pool_mgr);

	for( ColorFilterPairTextureList::iterator i = mColorFilterTextureList.begin();
		 i != mColorFilterTextureList.end(); ++i )
	{
		if( i->second )
		{
			i->second->Release();
		}
	}
    //
}

// attributes method
LW_RESULT lwResourceMgr::GetAssObjInfo(lwAssObjInfo* info) 
{ 
    if(info)
    {
        *info = _assobj_info;
    }
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::SetAssObjInfo(DWORD mask, const lwAssObjInfo* info)
{
    if(info)
    {
        if(mask & ASSOBJ_MASK_SIZE)
        {
            _assobj_info.size = info->size;
        }
        if(mask & ASSOBJ_MASK_COLOR)
        {
            _assobj_info.color = info->color;
        }
    }
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::ClearAllMesh()
{
    _pool_mesh.ForEach([](DWORD, void* obj) {
        LW_RELEASE(static_cast<lwIMesh*>(obj));
    });
    return LW_RET_OK;
}
LW_RESULT lwResourceMgr::ClearAllTex()
{
    _pool_tex.ForEach([](DWORD, void* obj) {
        LW_RELEASE(static_cast<lwITex*>(obj));
    });
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::ClearAllAnimCtrl()
{
    _pool_animctrl.ForEach([](DWORD, void* obj) {
        static_cast<lwIAnimCtrl*>(obj)->Release();
    });
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::CreateMesh(lwIMesh** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIMesh* o = NULL;
    
    if((o = LW_NEW(lwMesh(this))) == NULL)
        goto __ret;

    if(LW_FAILED(RegisterMesh(o)))
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwResourceMgr::CreateTex(lwITex** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwITex* o = NULL;
    
    if((o = LW_NEW(lwTex(this))) == NULL)
        goto __ret;

    if(LW_FAILED(RegisterTex(o)))
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwResourceMgr::CreateAnimCtrl(lwIAnimCtrl** ret_obj, DWORD type)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrl* c;

    switch(type)
    {
    case ANIM_CTRL_TYPE_BONE:
        c = LW_NEW(lwAnimCtrlBone(this));
        break;
    case ANIM_CTRL_TYPE_MAT:
        c = LW_NEW(lwAnimCtrlMatrix(this));
        break;
    case ANIM_CTRL_TYPE_TEXUV:
        c = LW_NEW(lwAnimCtrlTexUV(this));
        break;
    case ANIM_CTRL_TYPE_TEXIMG:
        c = LW_NEW(lwAnimCtrlTexImg(this));
        break;
    case ANIM_CTRL_TYPE_MTLOPACITY:
        c = LW_NEW(lwAnimCtrlMtlOpacity(this));
        break;
    default:
        assert(0 && "invalid ctrl type in call RegisterAnimData");
    }

     if(LW_FAILED(RegisterAnimCtrl(c)))
         goto __ret;

    *ret_obj = c;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwResourceMgr::CreateAnimCtrlObj(lwIAnimCtrlObj** ret_obj, DWORD type)
{
    LW_RESULT ret = LW_RET_FAILED;

    //lwIAnimCtrl* c = NULL;
    lwIAnimCtrlObj* o = NULL;
    
    //if(LW_FAILED(CreateAnimCtrl(&c, type)))
    //    goto __ret;

    switch(type)
    {
    case ANIM_CTRL_TYPE_MAT:
        o = LW_NEW(lwAnimCtrlObjMat(this));
        break;
    case ANIM_CTRL_TYPE_BONE:
        o = LW_NEW(lwAnimCtrlObjBone(this));
        break;
    case ANIM_CTRL_TYPE_TEXUV:
        o = LW_NEW(lwAnimCtrlObjTexUV(this));
        break;
    case ANIM_CTRL_TYPE_TEXIMG:
        o = LW_NEW(lwAnimCtrlObjTexImg(this));
        break;
    case ANIM_CTRL_TYPE_MTLOPACITY:
        o = LW_NEW(lwAnimCtrlObjMtlOpacity(this));
        break;
    default:
        assert(0 && "invalid ctrl type in call RegisterAnimData");
        goto __ret;
    }

    //o->SetAnimCtrl(c);
    *ret_obj = o;
    
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwResourceMgr::CreateMeshAgent(lwIMeshAgent** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwMeshAgent* o = NULL;
    
    if((o = LW_NEW(lwMeshAgent(this))) == NULL)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwResourceMgr::CreateMtlTexAgent(lwIMtlTexAgent** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwMtlTexAgent* o = NULL;
    
    if((o = LW_NEW(lwMtlTexAgent(this))) == NULL)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwResourceMgr::CreateRenderCtrlAgent(lwIRenderCtrlAgent** ret_obj)
{
    lwIRenderCtrlAgent* o = LW_NEW(lwRenderCtrlAgent(this));
    if(o == NULL)
        return LW_RET_FAILED;

    *ret_obj = o;
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::CreateAnimCtrlAgent(lwIAnimCtrlAgent** ret_obj)
{
    lwIAnimCtrlAgent* o = LW_NEW(lwAnimCtrlAgent(this));
    if(o == NULL)
        return LW_RET_FAILED;

    *ret_obj = o;
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::CreateRenderCtrlVS(lwIRenderCtrlVS** ret_obj, DWORD type)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIRenderCtrlVS* o = 0;
    

    if(_render_ctrl_proc_seq[type] == NULL)
        goto __ret;

    o = _render_ctrl_proc_seq[type]();
    if(o == 0)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwResourceMgr::CreatePrimitive(lwIPrimitive** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwPrimitive* o = NULL;
    
    if((o = LW_NEW(lwPrimitive(this))) == NULL)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;


}
LW_RESULT lwResourceMgr::CreateHelperObject(lwIHelperObject** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwHelperObject* o = NULL;
    
    if((o = LW_NEW(lwHelperObject(this))) == NULL)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwResourceMgr::CreatePhysique(lwIPhysique** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIPhysique* o = NULL;
    
    if((o = LW_NEW(lwPhysique(this))) == NULL)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwResourceMgr::CreateModel(lwIModel** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIModel* o = NULL;
    
    if((o = LW_NEW(lwModel(this))) == NULL)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwResourceMgr::CreateItem(lwIItem** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIItem* o = NULL;
    
    if((o = LW_NEW(lwItem(this))) == NULL)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwResourceMgr::CreateNode(lwINode** ret_obj, DWORD type)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwINode* o = 0;

    switch(type)
    {
    case NODE_PRIMITIVE:
        o = LW_NEW(lwNodePrimitive(this));
        break;
    case NODE_BONECTRL:
        o = LW_NEW(lwNodeBoneCtrl(this));
        break;
    case NODE_DUMMY:
        o = LW_NEW(lwNodeDummy(this));
        break;
    case NODE_HELPER:
        o = LW_NEW(lwNodeHelper(this));
    default:
        goto __ret;
    }

    if(o == 0)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwResourceMgr::CreateNodeObject(lwINodeObject** ret_obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    lwINodeObject* o = NULL;
    
    if((o = LW_NEW(lwNodeObject(this))) == NULL)
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwResourceMgr::CreateStaticStreamMgr(lwIStaticStreamMgr** mgr)
{
    *mgr = LW_NEW(lwStaticStreamMgr(_dev_obj));
    return LW_RET_OK;
}
LW_RESULT lwResourceMgr::CreateDynamicStreamMgr(lwIDynamicStreamMgr** mgr)
{
    *mgr = LW_NEW(lwDynamicStreamMgr(_dev_obj));
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::RegisterMesh(lwIMesh* obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD ret_id;

    if(LW_FAILED(_pool_mesh.Register(&ret_id, obj)))
        goto __ret;

    obj->SetRegisterID(ret_id);

    ret = LW_RET_OK;

__ret:

    return ret;

}

LW_RESULT lwResourceMgr::RegisterTex(lwITex* obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD ret_id;

    if(LW_FAILED(_pool_tex.Register(&ret_id, obj)))
        goto __ret;

    obj->SetRegisterID(ret_id);

    _tex_size_vm += obj->GetDataInfo()->size;

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwResourceMgr::RegisterAnimCtrl(lwIAnimCtrl* obj)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD ret_id;

    if(LW_FAILED(_pool_animctrl.Register(&ret_id, obj)))
        goto __ret;

    obj->SetRegisterID(ret_id);

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwResourceMgr::RegisterRenderCtrlProc(DWORD id, lwRenderCtrlVSCreateProc proc)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(id < 0 || id >= LW_RENDER_CTRL_PROC_NUM)
        goto __ret;

    if(_render_ctrl_proc_seq[id])
        goto __ret;

    _render_ctrl_proc_seq[id] = proc;

    ret = LW_RET_OK;

__ret:
    return ret;
}

//LW_RESULT lwResourceMgr::RegisterAnimData(DWORD* ret_id, const void* data, DWORD anim_type)
//{
//    LW_RESULT ret = LW_RET_FAILED;
//
//    lwAnimCtrl* ctrl;
//
//    switch(anim_type)
//    {
//    case ANIM_CTRL_TYPE_BONE:
//        ctrl = LW_NEW(lwAnimCtrlBone);
//        ((lwAnimCtrlBone*)ctrl)->Create((lwAnimDataBone*)data);
//        break;
//    case ANIM_CTRL_TYPE_MATRIX:
//        ctrl = LW_NEW(lwAnimCtrlMatrix);
//        ((lwAnimCtrlMatrix*)ctrl)->Create((lwAnimDataMatrix*)data);
//        break;
//    case ANIM_CTRL_TYPE_TEXTURE0:
//    case ANIM_CTRL_TYPE_TEXTURE1:
//    case ANIM_CTRL_TYPE_TEXTURE2:
//    case ANIM_CTRL_TYPE_TEXTURE3:
//        ctrl = LW_NEW(lwAnimCtrlTexUV);
//        ((lwAnimCtrlTexUV*)ctrl)->Create((lwAnimDataTexUV*)data);
//        break;
//    case ANIM_CTRL_TYPE_IMAGE0:
//        break;
//    default:
//        assert(0 && "invalid ctrl type in call RegisterAnimData");
//    }
//
//    ret = _pool_animctrl.Register(ret_id, (void*)ctrl);
// 
//    return ret;
//
//}
//
//LW_RESULT lwResourceMgr::RegisterAnimData(DWORD* ret_id, const lwResFileAnimData* info)
//{
//    LW_RESULT ret = LW_RET_FAILED;
//
//    lwAnimCtrl* ctrl;
//
//    switch(info->anim_type)
//    {
//    case ANIM_CTRL_TYPE_BONE:
//        ctrl = LW_NEW(lwAnimCtrlBone);
//        break;
//    case ANIM_CTRL_TYPE_MATRIX:
//        ctrl = LW_NEW(lwAnimCtrlMatrix);
//        break;
//    case ANIM_CTRL_TYPE_TEXTURE0:
//        ctrl = LW_NEW(lwAnimCtrlTexUV);
//        break;
//    case ANIM_CTRL_TYPE_IMAGE0:
//        break;
//    default:
//        assert(0 && "invalid ctrl type in call RegisterAnimData");
//    }
//
//    ctrl->SetResFile(info);
//    
//    if(LW_FAILED(ctrl->LoadResFileData()))
//        goto __ret;
//
//    ret = _pool_animctrl.Register(ret_id, (void*)ctrl);
// 
//__ret:
//    return ret;
//}
//
LW_RESULT lwResourceMgr::QueryTex(DWORD* ret_id, const char* file_name)
{
    DWORD found = LW_INVALID_INDEX;
    _pool_tex.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* obj_tex = static_cast<lwTex*>(raw);
        if (_tcscmp(obj_tex->GetFileName(), file_name) == 0) {
            found = handle;
            return false;
        }
        return true;
    });

    if (found != LW_INVALID_INDEX) {
        *ret_id = found;
        return LW_RET_OK;
    }
    return LW_RET_FAILED;
}

LW_RESULT lwResourceMgr::QueryMesh(DWORD* ret_id, const lwResFileMesh* rfm)
{
    DWORD found = LW_INVALID_INDEX;
    _pool_mesh.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* obj = static_cast<lwMesh*>(raw);
        try {
            if ((obj->GetState() & RES_STATE_INIT) && (obj->GetResFileMesh()->Compare(rfm) == 1)) {
                found = handle;
                return false;
            }
        }
        catch (...) {
            __debugbreak();
        }
        return true;
    });

    if (found != LW_INVALID_INDEX) {
        *ret_id = found;
        return LW_RET_OK;
    }
    return LW_RET_FAILED;
}

LW_RESULT lwResourceMgr::QueryAnimCtrl(DWORD* ret_id, const lwResFileAnimData* info)
{
    DWORD found = LW_INVALID_INDEX;
    _pool_animctrl.ForEach([&](DWORD handle, void* raw) -> bool {
        auto* obj = static_cast<lwIAnimCtrl*>(raw);
        lwResFileAnimData* r = obj->GetResFileInfo();
        if (r->res_type == RES_FILE_TYPE_INVALID) {
            return true;
        }
        if (r->Compare(info) == 1) {
            found = handle;
            return false;
        }
        return true;
    });

    if (found != LW_INVALID_INDEX) {
        *ret_id = found;
        return LW_RET_OK;
    }
    return LW_RET_FAILED;
}

LW_RESULT lwResourceMgr::UnregisterMesh(lwIMesh* obj)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwMesh* o;

    if(LW_FAILED(ret = _pool_mesh.Unregister((void**)&o, obj->GetRegisterID())))
        goto __ret;

    if(ret == LW_RET_OK_1)
    {
        obj->SetRegisterID(LW_INVALID_INDEX);
    }

    //if(ret == LW_RET_OK_1)
    //{
    //    obj->Unload();
    //    obj->Release();
    //}

__ret:
    return ret;
}
LW_RESULT lwResourceMgr::UnregisterTex(lwITex* obj)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwITex* o;

    if(LW_FAILED(ret = _pool_tex.Unregister((void**)&o, obj->GetRegisterID())))
        goto __ret;

    if(ret == LW_RET_OK_1)
    {
        obj->SetRegisterID(LW_INVALID_INDEX);
    }

    //if(ret == LW_RET_OK_1)
    //{
    //    _tex_size_vm -= (obj->GetDataInfo()->size);

    //    obj->Unload();
    //    obj->Release();
    //}

__ret:
    return ret;
}
LW_RESULT lwResourceMgr::UnregisterAnimCtrl(lwIAnimCtrl* obj)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwAnimCtrl* o;

    if(LW_FAILED(ret =_pool_animctrl.Unregister((void**)&o, obj->GetRegisterID())))
        goto __ret;

    if(ret == LW_RET_OK_1)
    {
        obj->SetRegisterID(LW_INVALID_INDEX);
    }

    //if(ret == LW_RET_OK_1)
    //{               
    //    obj->Release();
    //}

__ret:
    return ret;
}


LW_RESULT lwResourceMgr::AddRefMesh(lwIMesh* obj, DWORD ref_cnt)
{
    return _pool_mesh.AddRef(obj->GetRegisterID(), ref_cnt);
}
LW_RESULT lwResourceMgr::AddRefTex(lwITex* obj, DWORD ref_cnt)
{
    return _pool_tex.AddRef(obj->GetRegisterID(), ref_cnt);
}
LW_RESULT lwResourceMgr::AddRefAnimCtrl(lwIAnimCtrl* obj, DWORD ref_cnt)
{
    return _pool_animctrl.AddRef(obj->GetRegisterID(), ref_cnt);
}

LW_ULONG lwResourceMgr::QueryTexRefCnt(lwITex* obj)
{
    DWORD id = obj->GetRegisterID();
    if(id == LW_INVALID_INDEX)
        return 0;

    return _pool_tex.GetRef(id);
}

LW_RESULT lwResourceMgr::GetMesh(lwIMesh** ret_obj, DWORD id)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwMesh* o;

    if(LW_FAILED(ret = _pool_mesh.GetObj((void**)&o, id)))
        goto __ret;

    *ret_obj = static_cast< lwIMesh* >(o);

    ret = LW_RET_OK;

__ret:
    return ret;

}
LW_RESULT lwResourceMgr::GetTex(lwITex** ret_obj, DWORD id)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwTex* o;

    if(LW_FAILED(ret = _pool_tex.GetObj((void**)&o, id)))
        goto __ret;

    *ret_obj = static_cast< lwITex* >(o);

    ret = LW_RET_OK;

__ret:
    return ret;
}
LW_RESULT lwResourceMgr::GetAnimCtrl(lwIAnimCtrl** ret_obj, DWORD id)
{
    LW_RESULT ret = LW_RET_FAILED;
    lwIAnimCtrl* o;

    if(LW_FAILED(ret = _pool_animctrl.GetObj((void**)&o, id)))
        goto __ret;

    *ret_obj = o;

    ret = LW_RET_OK;

__ret:
    return ret;
}

void lwResourceMgr::ReleaseObject()
{
    _pool_model.ForEach([](DWORD, void* raw) {
        static_cast<lwModel*>(raw)->Release();
    });
}

LW_RESULT lwResourceMgr::RegisterObject(DWORD* ret_id, void* obj, DWORD type)
{

    lwSlotMapVoidPtr1024* pool_obj;
    
    switch(type)
    {
    case OBJ_TYPE_MODEL:
        return _pool_model.Register(ret_id, obj);
        break;
    case OBJ_TYPE_CHARACTER:
        pool_obj = &_pool_physique;
        break;
    case OBJ_TYPE_ITEM:
        pool_obj = &_pool_item;
        break;
    default:
        assert(0 && "invaild type");
    }

    return pool_obj->Register(ret_id, obj);
}

LW_RESULT lwResourceMgr::UnregisterObject(void** ret_obj, DWORD id, DWORD type)
{
    lwSlotMapVoidPtr1024* pool_obj;
    
    switch(type)
    {
    case OBJ_TYPE_MODEL:
        return _pool_model.Unregister(ret_obj, id);
        break;
    case OBJ_TYPE_CHARACTER:
        pool_obj = &_pool_physique;
        break;
    case OBJ_TYPE_ITEM:
        pool_obj = &_pool_item;
        break;
    default:
        assert(0 && "invaild type");
    }

    return pool_obj->Unregister(ret_obj, id);
}

LW_RESULT lwResourceMgr::QueryModelObject(void** ret_obj, DWORD model_id)
{
    lwModel* found = nullptr;
    _pool_model.ForEach([&](DWORD, void* raw) -> bool {
        auto* m = static_cast<lwModel*>(raw);
        if (m->GetModelID() == model_id) {
            found = m;
            return false;
        }
        return true;
    });
    if (found) {
        *ret_obj = static_cast<void*>(found);
    }
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::QueryObject(void** ret_obj, DWORD type, const char* file_name)
{
    switch (type) {
    case OBJ_TYPE_MODEL: {
        lwModel* found = nullptr;
        _pool_model.ForEach([&](DWORD, void* raw) -> bool {
            auto* m = static_cast<lwModel*>(raw);
            if (_tcscmp(m->GetFileName(), file_name) == 0) {
                found = m;
                return false;
            }
            return true;
        });
        if (found) {
            *ret_obj = static_cast<void*>(found);
        }
        break;
    }
    case OBJ_TYPE_CHARACTER:
        break;
    case OBJ_TYPE_ITEM:
        break;
    default:
        assert(0 && "invaild type");
    }
    return LW_RET_OK;
}

LW_RESULT lwResourceMgr::LoseDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    _pool_mesh.ForEach([](DWORD, void* raw) {
        static_cast<lwIMesh*>(raw)->LoseDevice();
    });

    _pool_tex.ForEach([](DWORD, void* raw) {
        static_cast<lwITex*>(raw)->LoseDevice();
    });

    // stream manager object
    if(LW_FAILED(_static_stream_mgr->LoseDevice()))
        goto __ret;

    if(LW_FAILED(_dynamic_stream_mgr->LoseDevice()))
        goto __ret;

    if(LW_FAILED(_lockable_stream_mgr->LoseDevice()))
        goto __ret;

    if(LW_FAILED(_surface_stream_mgr->LoseDevice()))
        goto __ret;

    // shader manager object
    if(LW_FAILED(_shader_mgr->LoseDevice()))
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;

}
LW_RESULT lwResourceMgr::ResetDevice()
{
    LW_RESULT ret = LW_RET_FAILED;

    // stream manager object
    if(LW_FAILED(_static_stream_mgr->ResetDevice()))
        goto __ret;

    if(LW_FAILED(_dynamic_stream_mgr->ResetDevice()))
        goto __ret;

    if(LW_FAILED(_lockable_stream_mgr->ResetDevice()))
        goto __ret;

    if(LW_FAILED(_surface_stream_mgr->ResetDevice()))
        goto __ret;

    // shader manager object
    if(LW_FAILED(_shader_mgr->ResetDevice()))
        goto __ret;

    _pool_mesh.ForEach([](DWORD, void* raw) {
        static_cast<lwIMesh*>(raw)->ResetDevice();
    });

    _pool_tex.ForEach([](DWORD, void* raw) {
        static_cast<lwITex*>(raw)->ResetDevice();
    });

    //
    ret = LW_RET_OK;
__ret:
    return ret;
}

IDirect3DTextureX* lwResourceMgr::getMonochromaticTexture( D3DCOLOR colour, const std::string& filterTexture )
{
	if( mColorFilterTextureList.find( ColourFilterPair( colour, filterTexture ) ) != mColorFilterTextureList.end() )
	{
		return mColorFilterTextureList[ ColourFilterPair( colour, filterTexture ) ];
	}
	IDirect3DTextureX* texture = _createMonochromaticTexture( colour, filterTexture, 1, 1 );
	return texture;
}

IDirect3DTextureX* lwResourceMgr::_createMonochromaticTexture(
	D3DCOLOR colour,
	const std::string& filterTexture,
	size_t width, size_t height )
{
	if( filterTexture.empty() )
	{
		IDirect3DTextureX* texture = 0;
		IDirect3DDeviceX* device = _dev_obj->GetDevice();

		HRESULT hr = device->CreateTexture(
			width, height,
			0, D3DUSAGE_DYNAMIC,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&texture, NULL );

		if( FAILED( hr ) )
		{
			return 0;
		}

		D3DLOCKED_RECT lockedRect;
		hr = texture->LockRect( 0, &lockedRect, 0, D3DLOCK_DISCARD );
		if( FAILED( hr ) )
		{
			texture->Release();
			return 0;
		}

		size_t* writer = reinterpret_cast < size_t* > ( lockedRect.pBits );
		for( size_t y = 0; y < height; ++y )
		{
			size_t offset = y * width;

			for( size_t x = 0; x < width; ++x )
			{
				writer[ offset + x ] = colour;
			}
		}
		texture->UnlockRect( 0 );

		mColorFilterTextureList[ ColourFilterPair( colour, filterTexture ) ] = texture;

		return texture;
	}
	else
	{
		IDirect3DTextureX* texture = 0;
		IDirect3DDeviceX* device = _dev_obj->GetDevice();

		HRESULT hr = D3DXCreateTextureFromFile(
			device,
			filterTexture.c_str(),
			&texture );

		if( FAILED( hr ) )
		{
			return 0;
		}

		D3DSURFACE_DESC description;
		hr = texture->GetLevelDesc( 0, &description );
		if( FAILED( hr ) )
		{
			return 0;
		}

		width = description.Width;
		height = description.Height;

		D3DLOCKED_RECT lockedRect;
		hr = texture->LockRect( 0, &lockedRect, 0, D3DLOCK_DISCARD );
		if( FAILED( hr ) )
		{
			texture->Release();
			return 0;
		}

		size_t* writer = reinterpret_cast < size_t* > ( lockedRect.pBits );
		for( size_t y = 0; y < height; ++y )
		{
			size_t offset = y * width;

			for( size_t x = 0; x < width; ++x )
			{
				if( ( writer[ offset + x ] >> 24 ) )
				{
					writer[ offset + x ] = colour;
				}
				else
				{
					writer[ offset + x ] |= 0xFF000000;
					//writer[ offset + x ] = trasparent;
					//writer[ offset + x ] = 0xFF000000;
				}
			}
		}
		texture->UnlockRect( 0 );

		mColorFilterTextureList[ ColourFilterPair( colour, filterTexture ) ] = texture;

		return texture;
	}
}

const char* lwResourceMgr::getTextureOperationDescription( size_t operation )
{
	switch( D3DTEXTUREOP( operation ) )
	{
	case D3DTOP_DISABLE:
		return ( "D3DTOP_DISABLE" );
	case D3DTOP_SELECTARG1:
		return ( "D3DTOP_SELECTARG1" );
	case D3DTOP_SELECTARG2:
		return ( "D3DTOP_SELECTARG2" );
	case D3DTOP_MODULATE:
		return ( "D3DTOP_MODULATE" );
	case D3DTOP_MODULATE2X:
		return ( "D3DTOP_MODULATE2X" );
	case D3DTOP_MODULATE4X:
		return ( "D3DTOP_MODULATE4X" );
	case D3DTOP_ADD:
		return ( "D3DTOP_ADD" );
	case D3DTOP_ADDSIGNED:
		return ( "D3DTOP_ADDSIGNED" );
	case D3DTOP_ADDSIGNED2X:
		return ( "D3DTOP_ADDSIGNED2X" );
	case D3DTOP_SUBTRACT:
		return ( "D3DTOP_SUBTRACT" );
	case D3DTOP_ADDSMOOTH:
		return ( "D3DTOP_ADDSMOOTH" );
	case D3DTOP_BLENDDIFFUSEALPHA:
		return ( "D3DTOP_BLENDDIFFUSEALPHA" );
	case D3DTOP_BLENDTEXTUREALPHA:
		return ( "D3DTOP_BLENDTEXTUREALPHA" );
	case D3DTOP_BLENDFACTORALPHA:
		return ( "D3DTOP_BLENDFACTORALPHA" );
	case D3DTOP_BLENDTEXTUREALPHAPM:
		return ( "D3DTOP_BLENDTEXTUREALPHAPM" );
	case D3DTOP_BLENDCURRENTALPHA:
		return ( "D3DTOP_BLENDCURRENTALPHA" );
	case D3DTOP_PREMODULATE:
		return ( "D3DTOP_PREMODULATE" );
	case D3DTOP_MODULATEALPHA_ADDCOLOR:
		return ( "D3DTOP_MODULATEALPHA_ADDCOLOR" );
	case D3DTOP_MODULATECOLOR_ADDALPHA:
		return ( "D3DTOP_MODULATECOLOR_ADDALPHA" );
	case D3DTOP_MODULATEINVALPHA_ADDCOLOR:
		return ( "D3DTOP_MODULATEINVALPHA_ADDCOLOR" );
	case D3DTOP_MODULATEINVCOLOR_ADDALPHA:
		return ( "D3DTOP_MODULATEINVCOLOR_ADDALPHA" );
	case D3DTOP_BUMPENVMAP:
		return ( "D3DTOP_BUMPENVMAP" );
	case D3DTOP_BUMPENVMAPLUMINANCE:
		return ( "D3DTOP_BUMPENVMAPLUMINANCE" );
	case D3DTOP_DOTPRODUCT3:
		return ( "D3DTOP_DOTPRODUCT3" );
	case D3DTOP_MULTIPLYADD:
		return ( "D3DTOP_MULTIPLYADD" );
	case D3DTOP_LERP:
		return ( "D3DTOP_LERP" );
	}
	return "";
}

LW_END
