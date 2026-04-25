//
#include "stdafx.h"


#include "lwPhysique.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwPathInfo.h"
#include "lwAnimCtrl.h"
#include "lwRenderImp.h"
#include "lwResourceMgr.h"
#include "lwD3D.h"
#include "lwShaderMgr.h"
#include "lwItem.h"
#include "lwExpObj.h"

using namespace std;
LW_BEGIN

unsigned int __stdcall __load_bone(void* param)
{
	lwPhysiqueBoneInfo* bone = (lwPhysiqueBoneInfo*)param;
	try
	{
		bone->p->LoadBoneCatch( *bone );
		bone->p->decCount();
		delete bone;
	}
	catch( ... )
	{
		
	}	
    return 0;
}

unsigned int __stdcall __load_pri(void* param)
{
	lwPhysiquePriInfo* pri = (lwPhysiquePriInfo*)param;
	try
	{
		pri->p->LoadPriCatch( *pri );
		pri->p->decCount();
		delete pri;
	}
	catch( ... )
	{
		
	}	
	
    return 0;
}

lwGeomManager g_GeomManager;

lwGeomManager::lwGeomManager()
{
	m_GeomobjMap.clear();
	m_AnimDataMap.clear();
}

lwGeomManager::~lwGeomManager()
{
	GEOMOBJ_MAP::iterator pos1 = m_GeomobjMap.begin();
	for( ; pos1 != m_GeomobjMap.end(); pos1++ )
	{
		delete (*pos1).second;
	}
	m_GeomobjMap.clear();

	ANIMDATA_MAP::iterator pos2 = m_AnimDataMap.begin();
	for( ; pos2 != m_AnimDataMap.end(); pos2++ )
	{
		(*pos2).second->Release();
	}
	m_AnimDataMap.clear();
}

lwGeomObjInfo* lwGeomManager::GetGeomObjInfo( const char file[] )
{
	GEOMOBJ_MAP::iterator pos = m_GeomobjMap.find( file );
	if( pos != m_GeomobjMap.end() )
	{
		return (*pos).second;
	}
	return NULL;
}

bool lwGeomManager::LoadGeomobj( const char file[] )
{
	static char path[ LW_MAX_PATH ];
	sprintf( path, "%s%s", "model\\character\\", file );
	lwGeomObjInfo* pInfo = new lwGeomObjInfo;
	if( LW_FAILED(pInfo->Load( path )) )
	{
		delete pInfo;
		return false;
	}
	m_GeomobjMap[file] = pInfo;
	return true;
}

lwIAnimDataBone* lwGeomManager::GetBoneData( const char file[] )
{
	ANIMDATA_MAP::iterator pos = m_AnimDataMap.find( file );
	if( pos != m_AnimDataMap.end() )
	{
		return (*pos).second;
	}
	return NULL;
}

bool lwGeomManager::LoadBoneData( const char file[] )
{
	static char path[ LW_MAX_PATH ];
	sprintf( path, "%s%s", "animation\\", file );
	lwIAnimDataBone* i_data = LW_NEW(lwAnimDataBone);
	if( LW_FAILED( i_data->Load( path ) ) )
	{
		i_data->Release();
		return false;
	}
	m_AnimDataMap[file] = i_data;
	return true;
}



// lwPhysique
LW_STD_IMPLEMENTATION( lwPhysique )

//#define DYNAMIC_LOADING

// begin construct
lwPhysique::lwPhysique( lwIResourceMgr* res_mgr )
: _res_mgr( res_mgr ), _scene_mgr(0), _id( LW_INVALID_INDEX ), _anim_agent( 0 )
{
    _file_name[ 0 ] = '\0';
    lwMatrix44Identity( &_mat_base );
    memset( _obj_seq, 0, sizeof(lwIPrimitive*) * LW_MAX_SUBSKIN_NUM );
    _opacity = 1.0f;
	_count = 0;
	_start = false;
	_end = false;
}
// end construct

lwPhysique::lwPhysique()
: _id( LW_INVALID_INDEX ), _anim_agent( 0 )
{
    _res_mgr = lwSysGraphics::GetActiveIGraphicsSystem()->GetResourceMgr();

    _file_name[ 0 ] = '\0';
    lwMatrix44Identity( &_mat_base );
    memset( _obj_seq, 0, sizeof(lwIPrimitive*) * LW_MAX_SUBSKIN_NUM );
}

lwPhysique::~lwPhysique()
{
    Destroy();
}

LW_RESULT lwPhysique::GetLinkCtrlMatrix( lwMatrix44* mat, DWORD link_id )
{
    LW_RESULT ret = LW_RET_FAILED;

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_BONE;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    lwIAnimCtrlObjBone* bone_ctrl = (lwIAnimCtrlObjBone*)_anim_agent->GetAnimCtrlObj(&type_info);
    if(bone_ctrl == NULL)
        goto __ret;

    {
        lwMatrix44* rtm = bone_ctrl->GetDummyRTM(link_id);

        if (rtm == NULL)
            goto __ret;

        lwMatrix44Multiply(mat, rtm, &_mat_base);
    }
    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwPhysique::DestroyPrimitive( DWORD part_id )
{
    LW_RESULT ret = LW_RET_FAILED;

    if( _obj_seq[ part_id ] == 0 )
    {
        ret = LW_RET_OK;
        goto __ret;
    }

    if( LW_FAILED( _obj_seq[ part_id ]->Destroy() ) )
        goto __ret;

    _obj_seq[ part_id ]->Release();
    _obj_seq[ part_id ] = 0;

    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwPhysique::Destroy()
{
    LW_RESULT ret = LW_RET_FAILED;

#ifdef DYNAMIC_LOADING
	while( _count > 0 || (_start && !_end) )
	{
		Sleep( 1 );
	}
#endif 

    for( DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++ )
    {
        if( LW_FAILED( DestroyPrimitive( i ) ) )
        {
            assert( 0 && "call DestroyPrimitive in lwPhysique::Destroy error" );
        }
    }

    LW_SAFE_RELEASE( _anim_agent );

    return LW_RET_OK;
}

LW_RESULT lwPhysique::LoadBoneCatch( lwPhysiqueBoneInfo& info )
{
	if(LW_FAILED(info.data->Load(info.str.c_str())))
		goto __addr_1;

	if(LW_FAILED(info.cb->LoadData(info.data)))
		goto __addr_1;

	info.cb->SetResFile(&info.res);

	info.bc->AttachAnimCtrl(info.cb);
	info.bc->SetTypeInfo(&info.tp);

__addr_1:
	info.data->Release();

	return LW_RET_OK;
}

LW_RESULT lwPhysique::LoadBone( const char* file )
{
    LW_RESULT ret = LW_RET_FAILED;

    lwResFileAnimData res;

    lwIAnimCtrlObjBone* bone_ctrl;

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_BONE;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    // check existing bone
    if(_anim_agent && _anim_agent->GetAnimCtrlObj(&type_info))
    {
        bone_ctrl = (lwIAnimCtrlObjBone*)_anim_agent->RemoveAnimCtrlObj(&type_info);
        LW_SAFE_RELEASE(bone_ctrl);
        //goto __ret;
    }

    if(_anim_agent == NULL)
    {
        if(LW_FAILED(_res_mgr->CreateAnimCtrlAgent(&_anim_agent)))
            goto __ret;
    }

    if(LW_FAILED(_res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&bone_ctrl, ANIM_CTRL_TYPE_BONE)))
        goto __ret;

    if(LW_FAILED(_anim_agent->AddAnimCtrlObj(bone_ctrl)))
        goto __ret;

    {
    lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
    lwISystem* sys = sys_graphics->GetSystem();
    lwIPathInfo* path_info = 0;
    sys->GetInterface( (LW_VOID**)&path_info, LW_GUID_PATHINFO );

    char path[ LW_MAX_PATH ];
    sprintf( path, "%s%s", path_info->GetPath( PATH_TYPE_ANIMATION ), file );

    DWORD ret_id;    
    
    res.obj_id = 0;
    res.res_type = RES_FILE_TYPE_GENERIC;
    res.anim_type = ANIM_CTRL_TYPE_BONE;
    _tcscpy( res.file_name, path );

    if(LW_SUCCEEDED(_res_mgr->QueryAnimCtrl(&ret_id, &res)))
    {        
        lwIAnimCtrlBone* anim_ctrl = NULL;
        if(LW_FAILED(_res_mgr->GetAnimCtrl((lwIAnimCtrl**)&anim_ctrl, ret_id)))
            goto __ret;
        _res_mgr->AddRefAnimCtrl(anim_ctrl, 1);
        bone_ctrl->AttachAnimCtrl(anim_ctrl);
    }
    else
    {
#ifdef DYNAMIC_LOADING
        lwIAnimCtrlBone* ctrl_bone;
        if (LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_bone, ANIM_CTRL_TYPE_BONE)))
            goto __ret;

        lwIAnimDataBone* i_data = LW_NEW(lwAnimDataBone);

        lwPhysiqueBoneInfo* bi = new lwPhysiqueBoneInfo;
        bi->p = this;
        bi->str = path;
        bi->data = i_data;
        bi->cb = ctrl_bone;
        bi->tp = type_info;
        bi->bc = bone_ctrl;
        bi->res = res;

        lwIThreadPoolMgr* tp_mgr = _res_mgr->GetThreadPoolMgr();
        lwIThreadPool* tp = tp_mgr->GetThreadPool(THREAD_POOL_LOADPHY);

        incCount();
        if (LW_FAILED(tp->RegisterTask(__load_bone, (void*)bi)))
        {
            char szData[128];
            sprintf(szData, "Load bone file error!%s.", path);
            MessageBox(NULL, szData, "Error", MB_OK);
            return LW_RET_FAILED;
        }
#else
        lwIAnimCtrlBone* ctrl_bone;
        lwIAnimDataBone* i_data = g_GeomManager.GetBoneData(file);
        if (i_data == NULL)
        {
            i_data = LW_NEW(lwAnimDataBone);
            if (LW_FAILED(i_data->Load(path)))
                goto __addr_1;
        }
        if (LW_FAILED(_res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_bone, ANIM_CTRL_TYPE_BONE)))
            goto __addr_1;

        if (LW_FAILED(ctrl_bone->LoadData(i_data)))
            goto __addr_1;

        ctrl_bone->SetResFile(&res);

        bone_ctrl->AttachAnimCtrl(ctrl_bone);
        bone_ctrl->SetTypeInfo(&type_info);

        //i_data->Release();
    }
        goto __addr_2;
__addr_1:        
        goto __ret;

__addr_2:
        ;
#endif 
    }

    RegisterSceneMgr(_res_mgr->GetSysGraphics()->GetSceneMgr());

    ret = LW_RET_OK;
__ret:
    if(ret == LW_RET_FAILED)
    {
        _anim_agent->Release();
        _anim_agent = 0;
    }

    return ret;
}

LW_RESULT lwPhysique::LoadPrimitive(DWORD part_id, lwIGeomObjInfo* geom_info)
{
    if( part_id < 0 || part_id >= LW_MAX_SUBSKIN_NUM )
        return ERR_INVALID_PARAM;


    lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
    lwISystem* sys = sys_graphics->GetSystem();
    lwIPathInfo* path_info = sys->GetPathInfo();
    lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
    BYTE create_helper_primitive = opt_mgr->GetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE);

    char* tex_path = path_info->GetPath( PATH_TYPE_TEXTURE_CHARACTER );

    lwGeomObjInfo* info = (lwGeomObjInfo*)geom_info;

    // query mesh pool
    lwIPrimitive* imp;
    lwIMeshAgent* mesh_agent;
    lwIMtlTexAgent* mtltex_agent;

    _res_mgr->CreatePrimitive( &imp );
    _res_mgr->CreateMeshAgent( &mesh_agent );
    
    
    // mesh
    mesh_agent->LoadMesh( &info->mesh );

    imp->SetMeshAgent(mesh_agent);

    // material and texture
    for( DWORD j = 0; j < info->mtl_num; j++ )
    {
        _res_mgr->CreateMtlTexAgent( &mtltex_agent );

        lwMtlTexInfo* mti = &info->mtl_seq[ j ];

        mtltex_agent->SetTranspType(mti->transp_type);
        mtltex_agent->SetMaterial( &mti->mtl );

        mtltex_agent->GetMtlRenderStateSet()->Load(mti->rs_set, LW_MTL_RS_NUM);

        mtltex_agent->SetOpacity(mti->opacity);

        for( DWORD i = 0; i < LW_MAX_TEXTURESTAGE_NUM; i++ )
        {
            lwTexInfo* tex_info = &mti->tex_seq[ i ];

            if(tex_info->stage == LW_INVALID_INDEX)
                break;

            mtltex_agent->LoadTextureStage(tex_info, tex_path);
        }

        imp->SetMtlTexAgent( j, mtltex_agent );
    }
    // end

    // =======================
    // create render ctrl agent
    imp->LoadRenderCtrl(&info->rcci);
    // state ctrl
    imp->SetState(&info->state_ctrl);

    // create anim ctrl agent and anim ctrl obj bone
    lwIAnimCtrlObj* ctrl_obj = NULL;
    lwIAnimCtrlAgent* anim_agent = NULL;
    _res_mgr->CreateAnimCtrlObj(&ctrl_obj, ANIM_CTRL_TYPE_BONE);

    if((anim_agent = imp->GetAnimAgent()) == 0)
    {
        _res_mgr->CreateAnimCtrlAgent(&anim_agent);
        imp->SetAnimCtrlAgent(anim_agent);
    }

    anim_agent->AddAnimCtrlObj(ctrl_obj);
    

    // 
    imp->SetID(info->id);
    imp->SetParentID( info->parent_id );

    // local matrix
    imp->SetMatrixLocal( &info->mat_local );

    if( info->helper_size > 0 )
    {
        lwIHelperObject* h;
        _res_mgr->CreateHelperObject(&h);
        if(LW_FAILED(h->LoadHelperInfo(&info->helper_data, create_helper_primitive)))
        {
            LG_MSGBOX("LoadHelperInfo error");
        }
        imp->SetHelperObject(h);
    }

    // ObjImpLoadMesh
    if( info->anim_size > 0 )
    {
        imp->LoadAnimData( &info->anim_data, tex_path, 0 );
    }

    LW_SAFE_RELEASE( _obj_seq[ part_id ] );

    _obj_seq[ part_id ] = imp;


    _res_mgr->RegisterObject( &_id, this, OBJ_TYPE_CHARACTER );

    return LW_RET_OK;

}

LW_RESULT lwPhysique::LoadPriCatch( const lwPhysiquePriInfo& info )
{
	return LW_RET_OK;
}

LW_RESULT lwPhysique::LoadPrimitive( DWORD part_id, const char* file )
{
    if( part_id < 0 || part_id >= LW_MAX_SUBSKIN_NUM )
        return ERR_INVALID_PARAM;


    lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
    lwISystem* sys = sys_graphics->GetSystem();
    lwIPathInfo* path_info = sys->GetPathInfo();
    lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
    BYTE create_helper_primitive = opt_mgr->GetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE);

    char* tex_path = path_info->GetPath( PATH_TYPE_TEXTURE_CHARACTER );

    // query mesh pool
    DWORD ret_id;

    char path[ LW_MAX_PATH ];
    sprintf( path, "%s%s", path_info->GetPath(PATH_TYPE_MODEL_CHARACTER), file );
    
    lwResFile res;
    res.obj_id = 0;
    res.res_type = RES_FILE_TYPE_GEOMETRY;
    _tcscpy( res.file_name, path );

    //lwGeomObjInfo info;
    //if( LW_FAILED( info.Load( path ) ) )
    //{
    //    return LW_RET_FAILED;
    //}

	lwGeomObjInfo* pInfo = g_GeomManager.GetGeomObjInfo( file );
	if( !pInfo )
	{
		g_GeomManager.LoadGeomobj( file );
		pInfo = g_GeomManager.GetGeomObjInfo( file );
		if( !pInfo )
			return LW_RET_FAILED;
	}

    lwIPrimitive* imp;
    lwIMeshAgent* mesh_agent;
    lwIMtlTexAgent* mtltex_agent;
    lwIMesh* mesh;
    lwITex* tex;

    _res_mgr->CreatePrimitive( &imp );
    _res_mgr->CreateMeshAgent( &mesh_agent );
    
    
    // mesh
    lwResFileMesh rfm;
    rfm.obj_id = 0;
    rfm.res_type = RES_FILE_TYPE_GEOMETRY;
    _tcscpy( rfm.file_name, path );

    if( LW_SUCCEEDED( _res_mgr->QueryMesh( &ret_id, &rfm ) ) )
    {
        _res_mgr->GetMesh( &mesh, ret_id );
        _res_mgr->AddRefMesh( mesh, 1 );

        mesh_agent->SetMesh( mesh );
    }
    else
    {
		mesh_agent->LoadMesh( &pInfo->mesh/*&info.mesh*/ );
    }

    mesh_agent->GetMesh()->SetResFile( &rfm );

    imp->SetMeshAgent(mesh_agent);

    // material and texture
    DWORD tex_id;    

    for( DWORD j = 0; j < pInfo->mtl_num; j++ )
    {
        _res_mgr->CreateMtlTexAgent( &mtltex_agent );

        lwMtlTexInfo* mti = &pInfo->mtl_seq[ j ];

        mtltex_agent->SetTranspType(mti->transp_type);
        mtltex_agent->SetMaterial( &mti->mtl );

        mtltex_agent->GetMtlRenderStateSet()->Load(mti->rs_set, LW_MTL_RS_NUM);

        mtltex_agent->SetOpacity(mti->opacity);

        for( DWORD i = 0; i < LW_MAX_TEXTURESTAGE_NUM; i++ )
        {
            lwTexInfo* tex_info = &mti->tex_seq[ i ];

            if(tex_info->stage == LW_INVALID_INDEX)
                break;

            sprintf( path, "%s%s", tex_path, tex_info->file_name );

            if( LW_SUCCEEDED( _res_mgr->QueryTex( &tex_id, path ) ) )
            {
                _res_mgr->GetTex( &tex, tex_id );
                _res_mgr->AddRefTex( tex, 1 );

                lwITex* ret_tex = 0;
                mtltex_agent->SetTex( i, tex, &ret_tex );
                //LW_SAFE_RELEASE(ret_tex);
                if(ret_tex)
                {
                    if(ret_tex->GetRegisterID() != LW_INVALID_INDEX)
                        _res_mgr->UnregisterTex(ret_tex);
                    else
                        ret_tex->Release();
                }
            }
            else
            {
                {
                    tex_info->level = D3DX_DEFAULT;
                }

                mtltex_agent->LoadTextureStage(tex_info, tex_path);
            }
        }

        imp->SetMtlTexAgent( j, mtltex_agent );
    }
    // end

    // =======================
    // create render ctrl agent
    imp->LoadRenderCtrl(&pInfo->rcci);
    // state ctrl
    imp->SetState(&pInfo->state_ctrl);

    // create anim ctrl agent and anim ctrl obj bone
    lwIAnimCtrlObj* ctrl_obj = NULL;
    lwIAnimCtrlAgent* anim_agent = NULL;
    _res_mgr->CreateAnimCtrlObj(&ctrl_obj, ANIM_CTRL_TYPE_BONE);

    if((anim_agent = imp->GetAnimAgent()) == 0)
    {
        _res_mgr->CreateAnimCtrlAgent(&anim_agent);
        imp->SetAnimCtrlAgent(anim_agent);
    }

    anim_agent->AddAnimCtrlObj(ctrl_obj);
    

    // 
    imp->SetID(pInfo->id);
    imp->SetParentID( pInfo->parent_id );

    // local matrix
    imp->SetMatrixLocal( &pInfo->mat_local );

    if( pInfo->helper_size > 0 )
    {
        lwIHelperObject* h;
        _res_mgr->CreateHelperObject(&h);
        if(LW_FAILED(h->LoadHelperInfo(&pInfo->helper_data, create_helper_primitive)))
        {
            LG_MSGBOX("LoadHelperInfo error");
        }
        imp->SetHelperObject(h);
    }

    // ObjImpLoadMesh
    if( pInfo->anim_size > 0 )
    {
        imp->LoadAnimData( &pInfo->anim_data, tex_path, &res );
    }

    LW_SAFE_RELEASE( _obj_seq[ part_id ] );

    _obj_seq[ part_id ] = imp;


    _res_mgr->RegisterObject( &_id, this, OBJ_TYPE_CHARACTER );

    return LW_RET_OK;

}

LW_RESULT lwPhysique::Update()
{
    LW_RESULT ret = LW_RET_FAILED;

    // update physique bone animation
    if( _anim_agent )
    {
        if(LW_FAILED(_anim_agent->Update()))
            goto __ret;

        lwAnimCtrlObjTypeInfo type_info;
        type_info.type = ANIM_CTRL_TYPE_BONE;
        type_info.data[0] = LW_INVALID_INDEX;
        type_info.data[1] = LW_INVALID_INDEX;

        lwIAnimCtrlObjBone* ctrl_obj = (lwIAnimCtrlObjBone*)_anim_agent->GetAnimCtrlObj(&type_info);
        lwIAnimCtrlBone* ctrl = (lwIAnimCtrlBone*)ctrl_obj->GetAnimCtrl();


        if(LW_FAILED(ctrl_obj->UpdateObject(ctrl_obj, NULL)))
            goto __ret;

        lwIPrimitive* pri;
        lwIAnimCtrlObjBone* pri_ctrl;

        for( DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++ )
        {
            if( (pri = _obj_seq[ i ]) == 0 )
                continue;

            pri_ctrl = (lwIAnimCtrlObjBone*)pri->GetAnimAgent()->GetAnimCtrlObj(&type_info);

            if(pri_ctrl == 0)
            {
                LG_MSGBOX("crash!!!, call jack");
                __debugbreak();
            }

            if(LW_FAILED(ctrl_obj->UpdateObject(pri_ctrl, pri->GetMeshAgent()->GetMesh())))
                goto __ret;
        }
    }    

    // update object
    lwIPrimitive* imp;
    lwIPrimitive* pp;

    for( DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++ )
    {
        if( (imp = _obj_seq[ i ]) == 0 )
            continue;


        // 
        lwMatrix44 mat( _mat_base );
        pp = imp;
        while( pp->GetParentID() != LW_INVALID_INDEX )
        {
            if( _obj_seq[ pp->GetParentID() ] )
            {
                lwMatrix44Multiply( &mat, _obj_seq[ pp->GetParentID() ]->GetMatrixLocal(), &mat );
                pp = _obj_seq[ pp->GetParentID() ];
            }
            else
            {
                break;
            }
        }

        imp->SetMatrixParent( &mat );

        imp->Update();
    }


    ret = LW_RET_OK;

__ret:
    return ret;
}

LW_RESULT lwPhysique::Render()
{
    LW_RESULT ret = LW_RET_FAILED;

    if( _state_ctrl.GetState( STATE_VISIBLE ) == 0 )
        goto __addr_ret_ok;

    {
        lwIPrimitive* p;

        IDirect3DDeviceX* device = _res_mgr->GetDeviceObject()->GetDevice();
        device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
        device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        device->SetTexture(1, 0);

        for (DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++)
        {
            if ((p = _obj_seq[i]) == 0)
                continue;

            if (_scene_mgr && p->GetState(STATE_TRANSPARENT))
            {
                if (LW_FAILED(_scene_mgr->AddTransparentPrimitive(p)))
                    goto __ret;
            }
            else
            {

                if (mIndexColourFilterList.find(i) != mIndexColourFilterList.end())
                {
                    IDirect3DDeviceX* device = _res_mgr->GetDeviceObject()->GetDevice();

                    static IDirect3DTextureX* texture = 0;
                    if (!texture)
                    {
                        texture = _res_mgr->getMonochromaticTexture(mIndexColourFilterList[i].first,
                            mIndexColourFilterList[i].second);
                    }
                    device->SetTexture(1, texture);
                    device->SetTextureStageState(1, D3DTSS_COLOROP, mIndexTextureOPList[i]);
                    device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
                    device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

                    device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
                    device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
                    device->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
                }


                if (LW_FAILED(p->Render()))
                    goto __ret;

                if (mIndexColourFilterList.find(i) != mIndexColourFilterList.end())
                {
                    IDirect3DDeviceX* device = _res_mgr->GetDeviceObject()->GetDevice();
                    device->SetTexture(1, 0);
                    device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
                    device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
                    device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTOP_DISABLE);
                    device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTOP_DISABLE);

                    device->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTOP_DISABLE);
                    device->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTOP_DISABLE);
                    device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
                }
            }
        }
    }
__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwPhysique::HitTestPrimitive( lwPickInfo* info, const lwVector3* org, const lwVector3* ray )
{
    lwIPrimitive* imp_generic;

    lwPickInfo x, xx;
    DWORD obj_id = LW_INVALID_INDEX;

    for( DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++ )
    {
        if( (imp_generic = _obj_seq[ i ]) == 0 )
            continue;

        if( LW_SUCCEEDED( imp_generic->HitTest( &x, org, ray ) ) )
        {
            if( (obj_id == LW_INVALID_INDEX) || (xx.dis > x.dis) )
            {
                xx = x;
                obj_id = i;
            }
        }
    }

    if( obj_id == LW_INVALID_INDEX )
    {
        return LW_RET_FAILED;
    }
    else
    {
        *info = xx;
        info->obj_id = obj_id;
        
        return LW_RET_OK;
    }
}
LW_RESULT lwPhysique::HitTestPhysique( lwPickInfo* info, const lwVector3* org, const lwVector3* ray )
{
    return LW_RET_FAILED;
}

void lwPhysique::ShowHelperObject( int show )
{
    lwIHelperObject* helper_obj;

    for( DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++ )
    {
        if( _obj_seq[ i ] == 0 )
            continue;

        if((helper_obj = _obj_seq[i]->GetHelperObject()) != 0)
        {
            helper_obj->SetVisible(show);
        }
    }

}
void lwPhysique::ShowBoundingObjectPhysique( int show )
{
}

LW_RESULT lwPhysique::SetItemLink( const lwItemLinkInfo* info )
{
    LW_RESULT ret = LW_RET_FAILED;

    if( _anim_agent == NULL )
        goto __ret;

    {
        lwAnimCtrlObjTypeInfo type_info;
        type_info.type = ANIM_CTRL_TYPE_BONE;
        type_info.data[0] = LW_INVALID_INDEX;
        type_info.data[1] = LW_INVALID_INDEX;

        lwIAnimCtrlObjBone* ctrl_obj = (lwIAnimCtrlObjBone*)_anim_agent->GetAnimCtrlObj(&type_info);
        if (ctrl_obj == NULL)
            goto __ret;

        {
            lwIAnimCtrlBone* ctrl_bone = (lwIAnimCtrlBone*)ctrl_obj->GetAnimCtrl();

            if (ctrl_bone == NULL)
                goto __ret;

            if (ctrl_bone->GetDummyRTM(info->link_parent_id) == NULL)
                goto __ret;

            if (info->obj->GetPrimitive() == 0)
                goto __ret;

            ret = info->obj->SetLinkCtrl(this, info->link_parent_id, info->link_item_id);
        }
    }
__ret:
    return ret;
}

LW_RESULT lwPhysique::ClearItemLink( lwIItem* obj )
{
    return obj->ClearLinkCtrl();
}

void lwPhysique::SetOpacity(float opacity)
{
    _opacity = opacity;

    for(DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++)
    {
        if(_obj_seq[i] == 0)
            continue;

        _obj_seq[i]->SetOpacity(opacity);
    }

}
LW_RESULT lwPhysique::SetTextureLOD(DWORD level)
{
    for( DWORD i = 0; i < LW_MAX_SUBSKIN_NUM; i++ )
    {
        if(_obj_seq[i] == 0)
            continue;

        _obj_seq[i]->SetTextureLOD(level);
    }

    return LW_RET_OK;
}

LW_END
