#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwObjectPool.h"
#include "lwClassDecl.h"
#include "lwITypes.h"
#include "lwDirectX.h"
#include "lwExpObj.h"
#include "lwGraphicsUtil.h"
#include "lwInterfaceExt.h"
#include "lwShaderMgr.h"
#include "lwPreDefinition.h"
#include "lwMisc.h"
#include "lwPathInfo.h"
#include "lwSyncObj.h"

#include <list>
#include <vector>
LW_BEGIN



enum lwResStateEnum
{
    RES_STATE_INVALID =         0x000,
    RES_STATE_INIT =            0x0001,
    RES_STATE_SYSTEMMEMORY =    0x0002,
    RES_STATE_VIDEOMEMORY =     0x0004,
    RES_STATE_LOADTEST =        0x0010,
    RES_STATE_LOADTEST_0 =      0x0020,
    RES_STATE_LOADING_VM =         0x1000,
};

#if 0
struct lwTexLogInfo
{
    char file[LW_MAX_PATH];
    DWORD width;
    DWORD height;
    D3DFORMAT fmt;
    DWORD devmem_size;

    void Save(FILE* fp)
    {
        char buf[512];
        sprintf(buf, "file:%s\nwidth: %d\theight: %d\tformat: %d\tmem_size:%d\n\n",
            file, width, height, fmt, devmem_size);

        fwrite(buf, strlen(buf), 1, fp);
    }
};

class lwTexLogMgr
{
    typedef list<lwTexLogInfo*> lwListTexLog;
private:
    lwListTexLog _list_texlog;

public:
    lwTexLogMgr()
    {};
    ~lwTexLogMgr()
    {
        lwListTexLog::iterator it = _list_texlog.begin();
        for(; it != _list_texlog.end(); ++it)
        {
            LW_DELETE((*it));
        }
    }

    void Add(const char* file, DWORD width, DWORD height, D3DFORMAT fmt, DWORD devmem_size)
    {
        lwTexLogInfo* info = LW_NEW(lwTexLogInfo);
        _tcscpy(info->file, file);
        info->width = width;
        info->height = height;
        info->fmt = fmt;
        info->devmem_size = devmem_size;

        _list_texlog.push_back(info);
    }

    void Save(const char* file)
    {
        lwListTexLog::iterator it;
        FILE* fp = fopen(file, "wt");
        if(fp == 0)
            goto __ret;

        it = _list_texlog.begin();
        for(; it != _list_texlog.end(); ++it)
        {
            (*it)->Save(fp);
        }

        DWORD total_size = 0;
        DWORD scene_size = 0;
        DWORD terrain_size = 0;
        DWORD ui_size = 0;
        DWORD mini_map = 0;

        it = _list_texlog.begin();
        for(; it != _list_texlog.end(); ++it)
        {
            total_size += (*it)->devmem_size;

            if(_tcsstr((*it)->file, "scene"))
            {
                scene_size += (*it)->devmem_size;
            }
            else if(_tcsstr((*it)->file, "terrain"))
            {
                terrain_size += (*it)->devmem_size;
            }
            else if(_tcsstr((*it)->file, "ui"))
            {
                ui_size += (*it)->devmem_size;
            }
            else if(_tcsstr((*it)->file, "minimap"))
            {
                mini_map += (*it)->devmem_size;
            }
        }

        char buf[256];
        sprintf(buf, "total: %d, scene: %d, terrain: %d, ui: %d, map: %d\n",
            total_size, scene_size, terrain_size, ui_size, mini_map);
        fwrite(buf, strlen(buf), 1, fp);

__ret:
        if(fp)
        {
            fclose(fp);
        }
    }
};
#else

struct lwTexLogFilterInfo
{
    char filter_str[LW_MAX_PATH];
    DWORD size;
    DWORD num;

    BOOL Filter(DWORD op_type, const char* file, DWORD file_size)
    {
        if(_tcsstr(file, filter_str) == 0)
            return 0;

        if(op_type == 1)
        {
            size += file_size;
            num += 1;
        }
        else
        {
            size -= file_size;
            num -= 1;
        }

        return 1;
    }
};
class lwTexLogMgr
{
    typedef std::vector<lwTexLogFilterInfo> lwTexLogFilterInfoSeq;
private:
    FILE* _fp;
    DWORD _total_size;
    DWORD _total_num;
    lwTexLogFilterInfoSeq _v_filter_entity;

public:
    lwTexLogMgr()
    {
        _fp = 0;
        _total_size = 0;
        _total_num = 0;
        _v_filter_entity.reserve(20);
    };
    ~lwTexLogMgr()
    {
        if(_fp)
        {
            char buf[256];
            lwTexLogFilterInfoSeq::iterator it = _v_filter_entity.begin();
            for(; it != _v_filter_entity.end(); ++it)
            {
                sprintf(buf, "type: %s, num: %d, size: %d\n", (*it).filter_str, (*it).num, (*it).size);
                fwrite(buf, strlen(buf), 1, _fp);
            }

            sprintf(buf, "Total num: %d, Total size: %d\n", _total_num, _total_size);
            fwrite(buf, strlen(buf), 1, _fp);


            fclose(_fp);
        }
    }

    LW_RESULT OpenLogFile(const char* file)
    {
        return (_fp = fopen(file, "wt")) ? LW_RET_OK : LW_RET_FAILED;
    }
    LW_RESULT AddTexType(const char* name)
    {
        lwTexLogFilterInfo info;
        info.num = 0;
        info.size = 0;
        _tcscpy(info.filter_str, name);

        _v_filter_entity.push_back(info);
        return LW_RET_OK;
    }

    LW_RESULT Log(DWORD op_type, const char* file, DWORD width, DWORD height, D3DFORMAT fmt, DWORD devmem_size)
    {
        if(_fp == 0)
            return LW_RET_FAILED;

        char op[2][64] =
        {
            "Load",
            "Release"
        };
        char* p;

        if(op_type == 1)
        {
            _total_size += devmem_size;
            _total_num += 1;
            p = op[0];
        }
        else
        {
            _total_size -= devmem_size;
            _total_num -= 1;
            p = op[1];
        }

        char buf[512];
        sprintf(buf, "%s file:%s\nwidth: %d\theight: %d\tformat: %d\tmem_size:%d\n",
            p, file, width, height, fmt, devmem_size);

        fwrite(buf, strlen(buf), 1, _fp);


        lwTexLogFilterInfoSeq::iterator it = _v_filter_entity.begin();
        for(; it != _v_filter_entity.end(); ++it)
        {
            if((*it).Filter(op_type, file, devmem_size))
            {
                //break;
            }
            sprintf(buf, "[%s]: num: %d, size: %d\t[%d]\n", (*it).filter_str, (*it).num, (*it).size, _total_size);
            fwrite(buf, strlen(buf), 1, _fp);
        }

        return LW_RET_OK;
    }
};

#endif


class lwTex : public lwITex
{
LW_STD_DECLARATION()

public:
    lwIResourceMgr* _res_mgr;
    int _ref;

    DWORD _reg_id;
    DWORD _state; //...
    DWORD _tex_type; // file texture or user-defined texture

    DWORD _level;
    DWORD _usage;
    DWORD _format;
    D3DPOOL _pool;
    void* _data;
    DWORD _data_size;

    DWORD _stage;
    DWORD _byte_alignment_flag;
    DWORD _colorkey_type;
    lwColorValue4b _colorkey;
    char _file_name [ LW_MAX_PATH ];

    lwTexDataInfo _data_info;

    IDirect3DTextureX* _tex;

    //lwTextureStageStateSetTex2 _tss_set;
    //lwRenderStateSetTex2 _rs_set;
    lwRenderStateAtomSet _rsa_0;

    DWORD _load_type;
    DWORD _load_mask;
    DWORD _load_flag;
    DWORD _mt_flag;

public:
    lwTex(lwIResourceMgr* res_mgr);
    ~lwTex();

    int AddRef(int i) { return _ref += i; }
    int GetRef() { return _ref; }

    LW_RESULT LoadTexInfo( const lwTexInfo* info, const char* tex_path );
    LW_RESULT LoadSystemMemory();
    LW_RESULT LoadVideoMemory();
    LW_RESULT LoadVideoMemoryMT();
    LW_RESULT LoadVideoMemoryEx();
    LW_RESULT LoadVideoMemoryDirect();
    LW_RESULT UnloadSystemMemory();
    LW_RESULT UnloadVideoMemory();
    LW_RESULT Unload();

    void SetRegisterID( DWORD id ) { _reg_id = id; }
    DWORD GetRegisterID() const { return _reg_id; }

    char* GetFileName() { return _file_name; }
    DWORD GetStage() const { return _stage; }
    DWORD GetState() const { return _state; }
    IDirect3DTextureX* GetTex() { return _tex; }
    lwTexDataInfo* GetDataInfo() { return &_data_info; }
    lwColorValue4b GetColorKey() const { return _colorkey; }
    void GetTexInfo(lwTexInfo* info);

    // Runtime-only указатель на пользовательские данные текстуры
    // (используется только для TEX_TYPE_DATA). См. комментарий в lwITex.
    void  SetUserData(void* data) { _data = data; }
    void* GetUserData() const { return _data; }

    void SetFileName( const char* file ) { _tcscpy( _file_name, file ); }
    void SetState( DWORD state ) { _state = state; }
    void SetStage( DWORD stage ) { _stage = stage; }
    void SetTexFormat( DWORD fmt ) { _format = fmt; }
    void SetColorKeyType( DWORD type, const lwColorValue4b* c ) { _colorkey_type = type; if( c ) { _colorkey = *c; } }
    DWORD SetLOD(DWORD level);
    void SetLoadFlag(DWORD flag) { _load_flag = flag; }

    void SetMTFlag(DWORD flag) { _mt_flag = flag; }
    DWORD GetMTFlag() { return _mt_flag; }
    BOOL IsLoadingOK() const;

    LW_RESULT Register();
    LW_RESULT Unregister();

    LW_RESULT BeginPass();
    LW_RESULT EndPass();
    LW_RESULT BeginSet();
    LW_RESULT EndSet();
    LW_RESULT LoseDevice();
    LW_RESULT ResetDevice();

    void SetLoadResType(DWORD type) { _load_type = type; }
    void SetLoadResMask(DWORD add_mask, DWORD remove_mask);
    DWORD GetLoadResMask() { return _load_mask; }
    DWORD GetLoadResType() { return _load_type; }    

};

// remarks by lsh
// lwMeshlwMeshInfo
// 1lwMeshInfoCconstructdestruct
//    lwMesh
// 2lwMeshInfoclasslwIMeshInfo
//    lwMesh
// 
class lwMesh : public lwIMesh
{

LW_STD_DECLARATION()

private:
public:
    lwIResourceMgr* _res_mgr;

    DWORD _reg_id;
    DWORD _state; //...

    DWORD _stream_type;
    DWORD _colorkey;
    LW_HANDLE _vb_id;
    LW_HANDLE _ib_id;
    lwIVertexBuffer* _svb;
    lwIIndexBuffer* _sib;

    lwResFileMesh _res_file;

    lwMeshDataInfo _data_info;
    lwMeshInfo _mesh_info;
    lwMeshInfo* _mesh_info_ptr;

    lwRenderStateAtomSet _rsa_0;

    DWORD _mt_flag;

public:
    lwMesh( lwIResourceMgr* res_mgr );
    ~lwMesh();

    LW_RESULT LoadSystemMemory( const lwMeshInfo* info );
    LW_RESULT LoadSystemMemoryMT( const lwMeshInfo* info );
    LW_RESULT LoadSystemMemory();
    LW_RESULT LoadVideoMemory();
    LW_RESULT LoadVideoMemoryMT();
    LW_RESULT LoadVideoMemoryEx();
    LW_RESULT UnloadSystemMemory();
    LW_RESULT UnloadVideoMemory();
    LW_RESULT Unload();
   
    LW_RESULT ExtractMesh(lwMeshInfo* info);

    void SetStreamType(DWORD type) { _stream_type = type; }
    DWORD GetStreamType() const { return _stream_type; }

    DWORD GetRegisterID() const { return _reg_id; }
    lwResFileMesh* GetResFileMesh() { return &_res_file; }
    DWORD GetState() const { return _state; }
    lwMeshInfo* GetMeshInfo() { return &_mesh_info; }

    LW_HANDLE GetVBHandle() const { return _vb_id; }
    LW_HANDLE GetIBHandle() const { return _ib_id; }

    void SetRegisterID( DWORD id ) { _reg_id = id; }
    void SetState( DWORD state ) { _state = state; }
    void SetColorkey(DWORD key) { _colorkey = key; }
    LW_RESULT SetResFile( const lwResFileMesh* info );

    LW_RESULT Register();
    LW_RESULT Unregister();

    LW_RESULT BeginSet();
    LW_RESULT EndSet();
    LW_RESULT DrawSubset( DWORD subset );
    LW_RESULT LoseDevice();
    LW_RESULT ResetDevice();

    lwILockableStreamVB* GetLockableStreamVB();
    lwILockableStreamIB* GetLockableStreamIB();

    BOOL IsLoadingOK() const;
    void SetMTFlag(DWORD flag) { _mt_flag = flag; }
    DWORD GetMTFlag() { return _mt_flag; }

};

class lwMtlTexAgent : public lwIMtlTexAgent
{
LW_STD_DECLARATION()

    enum { RSA_SET_SIZE =       8 };
private:
public:

    lwIResourceMgr* _res_mgr;

    float _opacity;
    DWORD _transp_type;
    lwMaterial _mtl;
    BOOL _render_flag;

    lwRenderStateAtomSet _rsa_0;


#ifdef MTLTEXAGENT_OPACITY_RSA_FLAG
    lwRenderStateAtomSet _rsa_opacity;
    //lwRenderStateAtom _opacity_rsa_seq[5];
#endif

    lwITex* _tex_seq[ LW_MAX_MTL_TEX_NUM ]; // multi-texture blending    
    lwMatrix44* _uvmat[LW_MAX_MTL_TEX_NUM];
    lwITex* _tt_tex[LW_MAX_MTL_TEX_NUM];

    DWORD _opacity_reserve_rs[2]; // D3DRS_SRCBLEND, D3DRS_DESTBLEND

public:
    lwMtlTexAgent( lwIResourceMgr* mgr );
    ~lwMtlTexAgent();

    void SetOpacity(float opacity) { _opacity = opacity; }
    void SetTranspType(DWORD type) { _transp_type = type; }
    void SetMaterial( const lwMaterial* mtl ) { _mtl = *mtl; }
    void SetRenderFlag(BOOL flag) { _render_flag = flag; }
    lwIRenderStateAtomSet* GetMtlRenderStateSet() { return (lwIRenderStateAtomSet*)&_rsa_0; }


    float GetOpacity() const { return _opacity; }
    DWORD GetTransparencyType() const { return _transp_type; }
    lwMaterial* GetMaterial() { return &_mtl; }
    lwITex* GetTex( DWORD stage ) { return _tex_seq[ stage ]; }
    BOOL GetRenderFlag() { return _render_flag; }


    LW_RESULT BeginPass();
    LW_RESULT EndPass();
    LW_RESULT BeginSet();
    LW_RESULT EndSet();

    LW_RESULT SetTex(DWORD stage, lwITex* obj, lwITex** ret_obj);
    LW_RESULT LoadMtlTex( lwMtlTexInfo* info, const char* tex_path );
    LW_RESULT LoadTextureStage(const lwTexInfo* info, const char* tex_path);

    LW_RESULT ExtractMtlTex(lwMtlTexInfo* info);

    LW_RESULT DestroyTextureStage( DWORD stage );
    LW_RESULT Destroy();
    LW_RESULT Clone( lwIMtlTexAgent** ret_obj );
    LW_RESULT SetTextureTransformMatrix(DWORD stage, const lwMatrix44* mat);
    LW_RESULT SetTextureTransformImage(DWORD stage, lwITex* tex);

    LW_RESULT SetTextureLOD(DWORD level);
    BOOL IsTextureLoadingOK() const;

};


class lwMeshAgent : public lwIMeshAgent
{
LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwIMesh* _mesh_obj;
    lwRenderStateSetMesh2 _rs_set;
    DWORD _mt_flag;

public:
    lwMeshAgent( lwIResourceMgr* res_mgr );
    ~lwMeshAgent();

    void SetRenderState( DWORD begin_end, DWORD state, DWORD value ) 
    { lwSetRenderStateSet( &_rs_set, begin_end, state, value ); }
    void SetRenderState( lwRenderStateSetMtl2* rs_set ) { _rs_set = *rs_set; }
    void SetMesh( lwIMesh* mesh ) { _mesh_obj = mesh; }
    lwIMesh* GetMesh() { return _mesh_obj; }

    void SetMTFlag(DWORD flag) { _mt_flag = flag; }
    LW_RESULT BeginSet();
    LW_RESULT EndSet();
    LW_RESULT DrawSubset( DWORD subset );    
    LW_RESULT LoadMesh( const lwMeshInfo* info );
    LW_RESULT LoadMesh( const lwResFileMesh* info );
    LW_RESULT DestroyMesh();
    LW_RESULT Destroy();
    LW_RESULT Clone( lwIMeshAgent** ret_obj );
};


class lwResBufMgr : public lwIResBufMgr
{
    struct lwModelObjInfoMap
    {
        lwModelObjInfo info;
        char file[LW_MAX_PATH];
        DWORD size;
        DWORD hit_time;
    };

    typedef lwObjectPoolVoidPtr10240 lwPoolSysMemTex;
    typedef lwObjectPoolVoidPtr10240 lwPoolModelObj;
    
    LW_STD_DECLARATION()

private:
    lwIResourceMgr* _res_mgr;
    lwPoolSysMemTex _pool_sysmemtex;
    lwPoolModelObj _pool_modelobj;
    lwCriticalSection _lock_sysmemtex;

    DWORD _modelobj_data_size;
    DWORD _sysmemtex_data_size;

    DWORD _lmt_modelobj_data_size;
    DWORD _lmt_modelobj_data_time;

public:
    lwResBufMgr(lwIResourceMgr* res_mgr);
    ~lwResBufMgr();

    LW_RESULT Destroy();
    LW_RESULT RegisterSysMemTex(LW_HANDLE* handle, const lwSysMemTexInfo* info);
    LW_RESULT QuerySysMemTex(lwSysMemTexInfo* info);
    LW_RESULT QuerySysMemTex(lwSysMemTexInfo** info, const char* file);
    LW_RESULT GetSysMemTex(lwSysMemTexInfo** info, LW_HANDLE handle);
    LW_RESULT UnregisterSysMemTex(LW_HANDLE handle);

    LW_RESULT RegisterModelObjInfo(LW_HANDLE handle, const char* file);
    LW_RESULT RegisterModelObjInfo(LW_HANDLE* handle, const char* file);
    LW_RESULT QueryModelObjInfo(lwIModelObjInfo** info, const char* file);
    LW_RESULT GetModelObjInfo(lwIModelObjInfo** info, LW_HANDLE handle);
    LW_RESULT UnregisterModelObjInfo(LW_HANDLE handle);

    void SetLimitModelObjInfo(DWORD lmt_size, DWORD lmt_time) { _lmt_modelobj_data_size = lmt_size;  _lmt_modelobj_data_time = lmt_time; }
    LW_RESULT FilterModelObjInfoSize();
};

class lwThreadPoolMgr : public lwIThreadPoolMgr
{
    LW_STD_DECLARATION()

    enum
    {
        CRITICALSECTION_SEQ_SIZE = 1,
    };

private:
    lwIThreadPool* _pool_seq[THREAD_POOL_SIZE];
    CRITICAL_SECTION _cs_seq[CRITICALSECTION_SEQ_SIZE];

public:
    lwThreadPoolMgr();
    ~lwThreadPoolMgr();

    LW_RESULT Create();
    LW_RESULT Destroy();

    lwIThreadPool* GetThreadPool(DWORD type) { return _pool_seq[type]; }
    void LockCriticalSection(DWORD type) { ::EnterCriticalSection(&_cs_seq[type]); }
    void UnlockCriticalSection(DWORD type) { ::LeaveCriticalSection(&_cs_seq[type]); }
};

// lwResourceMgr
class lwResourceMgr : public lwIResourceMgr
{
    typedef lwObjectPoolVoidPtr10240 lwPoolMesh;
    typedef lwObjectPoolVoidPtr40960 lwPoolTex;
    typedef lwObjectPoolVoidPtr1024 lwPoolMeshRender;
    typedef lwObjectPoolVoidPtr1024 lwPoolTexRender;
    typedef lwObjectPoolVoidPtr1024 lwPoolAnimCtrl;

LW_STD_DECLARATION()

private:
    lwISysGraphics* _sys_graphics;
    lwIDeviceObject* _dev_obj;
    lwIStaticStreamMgr* _static_stream_mgr;
    lwIDynamicStreamMgr* _dynamic_stream_mgr;
    lwILockableStreamMgr* _lockable_stream_mgr;
    lwISurfaceStreamMgr* _surface_stream_mgr;
    lwIShaderMgr* _shader_mgr;
    lwIResBufMgr* _resbuf_mgr;
    lwIThreadPoolMgr* _thread_pool_mgr;

    lwPoolMesh _pool_mesh;    
    lwPoolTex _pool_tex;
    lwPoolAnimCtrl _pool_animctrl;

    lwObjectPoolVoidPtr10240 _pool_model;
    lwObjectPoolVoidPtr1024 _pool_physique;
    lwObjectPoolVoidPtr1024 _pool_item;

    
    char _texture_path[LW_MAX_PATH];

    // render ctrl proc sequence
    lwRenderCtrlVSCreateProc _render_ctrl_proc_seq[ LW_RENDER_CTRL_PROC_NUM ];

    lwByteSet _byte_set;
    lwAssObjInfo _assobj_info;

    // begin debug information
    LW_DWORD _mesh_size_sm;
    LW_DWORD _mesh_size_vm;
    LW_DWORD _tex_size_sm;
    LW_DWORD _tex_size_vm;

public:
    lwTexLogMgr _texlog_mgr;
    // end

public:
    lwResourceMgr( lwISysGraphics* sys );
    ~lwResourceMgr();

    lwISysGraphics* GetSysGraphics() { return _sys_graphics; }
    lwIDeviceObject* GetDeviceObject() { return _dev_obj; }
    lwIShaderMgr* GetShaderMgr() { return _shader_mgr; }
    lwIStaticStreamMgr* GetStaticStreamMgr() { return _static_stream_mgr; }
    lwIDynamicStreamMgr* GetDynamicStreamMgr() { return _dynamic_stream_mgr; }
    lwILockableStreamMgr* GetLockableStreamMgr() { return _lockable_stream_mgr; }
    lwISurfaceStreamMgr* GetSurfaceStreamMgr() { return _surface_stream_mgr; }
    lwIResBufMgr* GetResBufMgr() { return _resbuf_mgr; }
    lwIThreadPoolMgr* GetThreadPoolMgr() { return _thread_pool_mgr; }
    lwIByteSet* GetByteSet() { return &_byte_set; }

    LW_RESULT SetAssObjInfo(DWORD mask, const lwAssObjInfo* info);
    LW_RESULT GetAssObjInfo(lwAssObjInfo* info);

    LW_RESULT CreateMesh( lwIMesh** ret_obj );
    LW_RESULT CreateTex( lwITex** ret_obj );
    LW_RESULT CreateAnimCtrl( lwIAnimCtrl** ret_obj, DWORD type );
    LW_RESULT CreateAnimCtrlObj( lwIAnimCtrlObj** ret_obj, DWORD type );

    LW_RESULT CreateMeshAgent( lwIMeshAgent** ret_obj );
    LW_RESULT CreateMtlTexAgent( lwIMtlTexAgent** ret_obj );
    LW_RESULT CreateAnimCtrlAgent( lwIAnimCtrlAgent** ret_obj );
    LW_RESULT CreateRenderCtrlAgent(lwIRenderCtrlAgent** ret_obj);
    LW_RESULT CreateRenderCtrlVS( lwIRenderCtrlVS** ret_obj, DWORD type );

    LW_RESULT CreatePrimitive( lwIPrimitive** ret_obj );
    LW_RESULT CreateHelperObject(lwIHelperObject** ret_obj);

    LW_RESULT CreatePhysique( lwIPhysique** ret_obj );
    LW_RESULT CreateModel( lwIModel** ret_obj );
    LW_RESULT CreateItem( lwIItem** ret_obj );

    LW_RESULT CreateNode(lwINode** ret_obj, DWORD type);
    LW_RESULT CreateNodeObject(lwINodeObject** ret_obj);

    LW_RESULT CreateStaticStreamMgr(lwIStaticStreamMgr** mgr);
    LW_RESULT CreateDynamicStreamMgr(lwIDynamicStreamMgr** mgr);

    LW_RESULT GetMesh( lwIMesh** ret_obj, DWORD id );
    LW_RESULT GetTex( lwITex** ret_obj, DWORD id );
    LW_RESULT GetAnimCtrl( lwIAnimCtrl** ret_obj, DWORD id );

    // 
    // BoundingBox
    LW_RESULT RegisterMesh( lwIMesh* obj );
    LW_RESULT RegisterTex( lwITex* obj );
    LW_RESULT RegisterAnimCtrl( lwIAnimCtrl* obj );
    LW_RESULT RegisterRenderCtrlProc( DWORD id, lwRenderCtrlVSCreateProc proc );

    LW_RESULT UnregisterMesh( lwIMesh* obj );
    LW_RESULT UnregisterTex( lwITex* obj );
    LW_RESULT UnregisterAnimCtrl( lwIAnimCtrl* obj );

    LW_RESULT ClearAllMesh();
    LW_RESULT ClearAllTex();
    LW_RESULT ClearAllAnimCtrl();

    LW_RESULT AddRefMesh( lwIMesh* obj, DWORD ref_cnt );
    LW_RESULT AddRefTex( lwITex* obj, DWORD ref_cnt );
    LW_RESULT AddRefAnimCtrl( lwIAnimCtrl* ret_obj, DWORD ref_cnt );
    
    LW_ULONG QueryTexRefCnt(lwITex* obj);

    LW_RESULT QueryMesh( DWORD* ret_id, const lwResFileMesh* rfm );
    LW_RESULT QueryTex( DWORD* ret_id, const char* file_name );
    // RegisterAnimDataRES_FILE_TYPE_GENERICRES_FILE_TYPE_GEOMETRY
    LW_RESULT QueryAnimCtrl( DWORD* ret_id, const lwResFileAnimData* info );

    LW_VOID ReleaseObject();
    LW_RESULT RegisterObject( DWORD* ret_id, void* obj, DWORD type );
    LW_RESULT UnregisterObject( void** ret_obj, DWORD id, DWORD type );
    LW_RESULT QueryObject( void** ret_obj, DWORD type, const char* file_name );
    LW_RESULT QueryModelObject( void** ret_obj, DWORD model_id );

    LW_RESULT LoseDevice();
    LW_RESULT ResetDevice();

	void SetTexturePath(const char* path) { _tcscpy(_texture_path, path); }
	char* GetTexturePath() { return _texture_path; }

	public:
		IDirect3DTextureX* getMonochromaticTexture( D3DCOLOR colour, const std::string& filterTexture );

		const char* getTextureOperationDescription( size_t operation );

	private:
		IDirect3DTextureX* _createMonochromaticTexture(
			D3DCOLOR colour,
			const std::string& filterTexture,
			size_t width, size_t height );

		typedef std::pair < D3DCOLOR, std::string > ColourFilterPair;
		typedef std::map < ColourFilterPair, IDirect3DTextureX* > ColorFilterPairTextureList;
		ColorFilterPairTextureList mColorFilterTextureList;
};


LW_END
