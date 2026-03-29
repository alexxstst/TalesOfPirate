//

#include "stdafx.h"
#include "MPCharacter.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwInterface.h"
#include "lwExpObj.h"
#include "lwIUtil.h"
#include "lwgraphicsutil.h"
#include "lwPhysique.h"

LW_BEGIN


MPCharacter::MPCharacter( lwISysGraphics* sys_graphics )
: _link_item_num( 0 ),
  _pose_proc( 0 ), _proc_param( 0 )
{
    sys_graphics->GetResourceMgr()->CreatePhysique( &_physique );

    BindMatrix( _physique->GetMatrix() );
}

MPCharacter::MPCharacter()
: _link_item_num( 0 ),
  _pose_proc( 0 ), _proc_param( 0 )
{
    lwSysGraphics::GetActiveIGraphicsSystem()->GetResourceMgr()->CreatePhysique( &_physique );

    BindMatrix( _physique->GetMatrix() );
}

MPCharacter::~MPCharacter()
{
    Destroy();
    _physique->Release();
}

void MPCharacter::Destroy()
{
    _physique->Destroy();
    //LW_SAFE_DELETE( _weapon );

    for( DWORD i = 0; i < _link_item_num; i++ )
    {
        if( _link_item_seq[ i ].obj )
        {
            _link_item_seq[ i ].obj->GetObject()->ClearLinkCtrl();
        }
    }

}

LW_RESULT MPCharacter::PlayPose( const lwPlayPoseInfo* info )
{
	if( !_physique->isLoaded() ) return 0;
    lwIAnimCtrlAgent* agent = _physique->GetAnimCtrlAgent();
    if( agent == NULL )
        return LW_RET_FAILED;

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_BONE;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    lwIAnimCtrlObj* ctrl_obj = agent->GetAnimCtrlObj(&type_info);
    if(ctrl_obj == NULL)
        return LW_RET_FAILED;

    return ctrl_obj->PlayPose(info );
}

LW_RESULT MPCharacter::PlayPose( DWORD pose_id, DWORD play_type, float start_frame, float velocity, DWORD blend_flag, DWORD blend_src_num )
{
    lwPlayPoseInfo info;
    memset( &info, 0, sizeof( info ) );
    info.bit_mask = PPI_MASK_DEFAULT;
    if(blend_flag == 1)
    {
        info.bit_mask |= (PPI_MASK_BLENDINFO |PPI_MASK_BLENDINFO_SRCFRAMENUM);
        info.blend_info.src_frame_num = (float)blend_src_num;
    }

    info.pose = pose_id;
    info.frame = start_frame;
    info.velocity = velocity;
    info.type = play_type;

    return PlayPose( &info );
}

LW_RESULT MPCharacter::PlayObjImpPose( DWORD obj_id, DWORD ctrl_type, const lwPlayPoseInfo* info )
{
	if( !_physique->isLoaded() ) return 0;
    lwIPrimitive* p = _physique->GetPrimitive( obj_id );
    if( p == NULL )
        return LW_RET_FAILED;

    lwIAnimCtrlAgent* agent = p->GetAnimAgent();
    if(agent == NULL)
        return LW_RET_FAILED;

    DWORD subset;
    DWORD stage;
    
    switch(ctrl_type)
    {
    case ANIM_CTRL_TYPE_MAT:
        subset = LW_INVALID_INDEX;
        stage = LW_INVALID_INDEX;
        break;
    case ANIM_CTRL_TYPE_BONE:
        subset = LW_INVALID_INDEX;
        stage = LW_INVALID_INDEX;
        break;
    case ANIM_CTRL_TYPE_TEXUV:
        subset = 0;
        stage = 0;
        break;
    }

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ctrl_type;
    type_info.data[0] = subset;
    type_info.data[1] = stage;

    lwIAnimCtrlObj* ctrl_obj = agent->GetAnimCtrlObj(&type_info);
    if(ctrl_obj == NULL)
        return LW_RET_FAILED;

    return ctrl_obj->PlayPose(info);

}


LW_RESULT MPCharacter::PlayObjImpPose( DWORD obj_id, DWORD ctrl_type, DWORD pose_id, DWORD play_type, float start_frame, float velocity )
{
    lwPlayPoseInfo info;
    memset( &info, 0, sizeof( info ) );
    info.bit_mask = PPI_MASK_DEFAULT;

    info.pose = pose_id;
    info.frame = start_frame;
    info.velocity = velocity; // the f does this change?
    info.type = play_type;

    return PlayObjImpPose( obj_id, ctrl_type, &info );
}

LW_RESULT MPCharacter::SetPoseKeyFrameProc( lwPoseKeyFrameProc proc, void* proc_param )
{
    lwPlayPoseInfo info;
    info.bit_mask = PPI_MASK_PROC;
    info.proc = proc;
    info.proc_param = proc_param;

    _pose_proc = proc;
    _proc_param = proc_param;

    return PlayPose( &info );
}

LW_RESULT MPCharacter::SetObjImpPoseKeyFrameProc( DWORD obj_id, DWORD ctrl_type, lwPoseKeyFrameProc proc, void* proc_param )
{
    lwPlayPoseInfo info;
    info.bit_mask = PPI_MASK_PROC;
    info.proc = proc;
    info.proc_param = proc_param;

    return PlayObjImpPose( obj_id, ctrl_type, &info );
}

void MPCharacter::FrameMove()
{
	if( _physique->isLoaded() )
	{
		_physique->Update();
	}
}

void MPCharacter::Render()
{
	if( _physique->isLoaded() )
	{
		_physique->Render();
	}

}

LW_RESULT MPCharacter::Load( const MPChaLoadInfo* info )
{
    LW_RESULT ret = LW_RET_FAILED;

    if( strlen( info->bone ) > 0 )
    {
        if( LW_FAILED( _physique->LoadBone( info->bone ) ) )
            goto __ret;
    }
  

    for( int i = 0; i < LW_MAX_SUBSKIN_NUM; i++ )
    {
        if( strlen( info->part[i] ) == 0 )
            continue;

        if( LW_FAILED( LoadPart( i, info->part[i] ) ) )
            goto __ret;
    }
    ret = LW_RET_OK;

__ret:
    return ret;

}


LW_RESULT MPCharacter::Load( DWORD obj_id, DWORD group_id, DWORD* skin_id_seq, int load_bone )
{
    LW_RESULT ret = LW_RET_FAILED;

    
    if( load_bone == 1 )
    {
        char path_bone[ LW_MAX_PATH ];
        sprintf( path_bone, "%04d.lab", obj_id );

        if( LW_FAILED( _physique->LoadBone( path_bone ) ) )
            goto __ret;
    }
 
    DWORD file_id;

    for( int i = 0; i < LW_MAX_SUBSKIN_NUM; i++ )
    {
        if( skin_id_seq[ i ] == 0 )
            continue;

        file_id = obj_id * 1000000 + group_id * 10000 + i;

        if( LW_FAILED( LoadPart( i, file_id ) ) )
            goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;
}
void MPCharacter::StartLoad()
{
	_physique->Start();
}

void MPCharacter::EndLoad()
{
	_physique->End();
}
	
bool MPCharacter::IsLoaded()
{
	return _physique->isLoaded();
}

extern lwGeomManager g_GeomManager;

BOOL MPCharacter::InitBone( const char* file )
{
	g_GeomManager.LoadBoneData( file );
	return TRUE;
}

LW_RESULT MPCharacter::LoadBone( const char* file )
{
	//char szData[128];
	//sprintf( szData, "%s\n", file );
	//LG( "bone", szData );
    return _physique->LoadBone( file );
}

LW_RESULT MPCharacter::LoadPart( DWORD part_id, DWORD file_id )
{
    LW_RESULT ret = LW_RET_OK;

    char path_mesh[ LW_MAX_NAME ];
    sprintf( path_mesh, "%010d.lgo", file_id );

    
    if( LW_SUCCEEDED( ret = _physique->CheckPrimitive( part_id ) ) )
    {
        if( LW_FAILED( ret = _physique->DestroyPrimitive( part_id ) ) )
            goto __ret;
    }

    if( LW_FAILED( ret = _physique->LoadPrimitive( part_id, path_mesh ) ) )
    {
        LG_MSGBOX("Load MPCharacter %s error", path_mesh);
        goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;

}
LW_RESULT MPCharacter::LoadPart( DWORD part_id, const char* file )
{
    LW_RESULT ret = LW_RET_OK;
    
    if( LW_SUCCEEDED( ret = _physique->CheckPrimitive( part_id ) ) )
    {
        if( LW_FAILED( ret = _physique->DestroyPrimitive( part_id ) ) )
            goto __ret;
    }

    if( LW_FAILED( ret = _physique->LoadPrimitive( part_id, file ) ) )
    {
        LG_MSGBOX("Load MPCharacter %s error", file);
        goto __ret;
    }

    ret = LW_RET_OK;

__ret:
    return ret;

}
LW_RESULT MPCharacter::DestroyPart( DWORD part_id )
{
    LW_RESULT ret = LW_RET_OK;

    if( LW_SUCCEEDED( ret = _physique->CheckPrimitive( part_id ) ) )
    {
        if( LW_FAILED( ret = _physique->DestroyPrimitive( part_id ) ) )
            goto __ret;
    }
    ret = LW_RET_OK;

__ret:
    return ret;

}


lwIPoseCtrl* MPCharacter::GetPoseCtrl()
{
	lwIPoseCtrl* ret = NULL;
	if (!_physique->isLoaded()) 
	{
		goto __ret;
	}

	{
		lwIAnimCtrlAgent* anim_agent = _physique->GetAnimCtrlAgent();
		if (anim_agent == NULL)
			goto __ret;
		{
			lwAnimCtrlObjTypeInfo type_info;
			type_info.type = ANIM_CTRL_TYPE_BONE;
			type_info.data[0] = LW_INVALID_INDEX;
			type_info.data[1] = LW_INVALID_INDEX;


			lwIAnimCtrlObjBone* ctrl_obj = (lwIAnimCtrlObjBone*)anim_agent->GetAnimCtrlObj(&type_info);
			if (ctrl_obj == NULL)
				goto __ret;

			ret = ctrl_obj->GetAnimCtrl()->GetPoseCtrl();
		}
	}
__ret:
    return ret;  
}
lwIPoseCtrl* MPCharacter::GetObjImpPoseCtrl( DWORD skin_id, DWORD ctrl_type )
{
    lwIPrimitive* p = _physique->GetPrimitive( skin_id );
    if( p == NULL )
        return NULL;

    lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();

    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ctrl_type;

    switch(ctrl_type)
    {
    case ANIM_CTRL_TYPE_MAT:
        {
            type_info.data[0] = LW_INVALID_INDEX;
            type_info.data[1] = LW_INVALID_INDEX;
            lwIAnimCtrlObjMat* ctrl_obj = (lwIAnimCtrlObjMat*)anim_agent->GetAnimCtrlObj(&type_info);
            if(ctrl_obj == NULL)
                return NULL;

            return ctrl_obj->GetAnimCtrl()->GetPoseCtrl();
        }
        break;
    case ANIM_CTRL_TYPE_BONE:
        {
            type_info.data[0] = LW_INVALID_INDEX;
            type_info.data[1] = LW_INVALID_INDEX;
            lwIAnimCtrlObjBone* ctrl_obj = (lwIAnimCtrlObjBone*)anim_agent->GetAnimCtrlObj(&type_info);
            if(ctrl_obj == NULL)
                return NULL;

            return ctrl_obj->GetAnimCtrl()->GetPoseCtrl();
        }
        break;
    case ANIM_CTRL_TYPE_TEXUV:
        {
            type_info.data[0] = 0;
            type_info.data[1] = 0;
            lwIAnimCtrlObjTexUV* ctrl_obj = (lwIAnimCtrlObjTexUV*)anim_agent->GetAnimCtrlObj(&type_info);
            if(ctrl_obj == NULL)
                return NULL;

            return ctrl_obj->GetAnimCtrl()->GetPoseCtrl();
        }
        break;
    }


    return 0;
}

lwPlayPoseInfo* MPCharacter::GetPlayPoseInfo()
{
    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ANIM_CTRL_TYPE_BONE;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    lwIAnimCtrlObj* ctrl_obj = _physique->GetAnimCtrlAgent()->GetAnimCtrlObj(&type_info);
    return ctrl_obj->GetPlayPoseInfo();
}

lwPlayPoseInfo* MPCharacter::GetObjImpPlayPoseInfo( DWORD obj_id, DWORD ctrl_type )
{
    lwAnimCtrlObjTypeInfo type_info;
    type_info.type = ctrl_type;
    type_info.data[0] = LW_INVALID_INDEX;
    type_info.data[1] = LW_INVALID_INDEX;

    lwIPrimitive* p = _physique->GetPrimitive(obj_id);
    lwIAnimCtrlObj* ctrl_obj = p->GetAnimAgent()->GetAnimCtrlObj(&type_info);
    return ctrl_obj->GetPlayPoseInfo();
}

LW_RESULT MPCharacter::HitTestPrimitive( lwPickInfo* info, const lwVector3* org, const lwVector3* ray )
{
    return _physique->HitTestPrimitive( info, org, ray );
}
LW_RESULT MPCharacter::HitTestPhysique( lwPickInfo* info, const lwVector3* org, const lwVector3* ray )
{
    return _physique->HitTestPhysique( info, org, ray );
}

void MPCharacter::ShowHelperObject( int show )
{
    _physique->ShowHelperObject( show );
}
void MPCharacter::ShowBoundingObjectPhysique( int show )
{
    _physique->ShowBoundingObjectPhysique( show );
}
LW_RESULT MPCharacter::AttachItem( const MPItemLinkInfo* info )
{
    lwItemLinkInfo ili;
    ili.id = 0;
    ili.obj = info->obj->GetObject();
    ili.link_item_id = info->link_item_id;
    ili.link_parent_id = info->link_parent_id;

    if( info->data == -1 )
    {
        if( LW_FAILED( _physique->SetItemLink( &ili ) ) )
            return LW_RET_FAILED;
    }
    else
    {
        assert(0);
        // 
        //lwIPrimitive* p = _physique->GetPrimitive( info->data );
        //if( p == NULL )
        //    return LW_RET_FAILED;

        //if( LW_FAILED( p->SetItemLink( &ili ) ) )
        //    return LW_RET_FAILED;

    }

    return LW_RET_OK;

}

LW_RESULT MPCharacter::SetItemLink( const lwSceneItemLinkInfo* info )
{
    if( _link_item_num >= LW_MAX_LINK_ITEM_NUM )
        return LW_RET_FAILED;

    lwItemLinkInfo ili;
    ili.id = info->id;
    ili.obj = info->obj->GetObject();
    ili.link_item_id = info->link_item_id;
    ili.link_parent_id = info->link_parent_id;

    if( LW_FAILED( _physique->SetItemLink( &ili ) ) )
        return LW_RET_FAILED;

    _link_item_seq[ _link_item_num ].obj = info->obj;
    _link_item_seq[ _link_item_num ].data = info->data;
    _link_item_num++;

    return LW_RET_OK;
}

LW_RESULT MPCharacter::UnwieldItem( const MPSceneItem* obj )
{
    for( DWORD i = 0; i < _link_item_num; i++ )
    {
        if( _link_item_seq[ i ].obj == obj )
        {
            //_physique->ClearItemLink( _link_item_seq[ i ]->GetObject() );
            _link_item_seq[ i ].obj->GetObject()->ClearLinkCtrl();

            _link_item_num -= 1;

            for( DWORD j = i; j < _link_item_num; j++ )
            {
                _link_item_seq[ j ] = _link_item_seq[ j + 1 ];
            }

            return LW_RET_OK;
        }
    }

    return LW_RET_FAILED;
}
LW_RESULT MPCharacter::SetSubObjItemLink( DWORD obj_id, const lwSceneItemLinkInfo* info )
{
    if( _link_item_num >= LW_MAX_LINK_ITEM_NUM )
        return LW_RET_FAILED;

    assert(0);

    //lwItemLinkInfo ili;
    //ili.id = info->id;
    //ili.obj = info->obj->GetObject();
    //ili.link_item_id = info->link_item_id;
    //ili.link_parent_id = info->link_parent_id;

    //lwIPrimitive* p = _physique->GetPrimitive( obj_id );
    //if( p == NULL )
    //    return LW_RET_FAILED;

    //if( LW_FAILED( p->SetItemLink( &ili ) ) )
    //    return LW_RET_FAILED;

    //_link_item_seq[ _link_item_num ].obj = info->obj;
    //_link_item_seq[ _link_item_num ].data = info->data;
    //_link_item_num++;

    return LW_RET_OK;

}

LW_RESULT MPCharacter::ClearItemLink( DWORD type )
{
    DWORD new_item_num = 0;
    MPLinkInfo new_item_buf[ LW_MAX_LINK_ITEM_NUM ];

    for( DWORD i = 0; i < _link_item_num; i++ )
    {
        if( _link_item_seq[ i ].data != type )
        {
            new_item_buf[ new_item_num++ ] = _link_item_seq[i];
        }
        else
        {
            _link_item_seq[ i ].obj->GetObject()->ClearLinkCtrl();
            _link_item_seq[ i ].obj = 0;
        }
    }

    _link_item_num = new_item_num;
    memcpy( _link_item_seq, new_item_buf, sizeof( MPLinkInfo ) * _link_item_num );

    return LW_RET_OK;
}

DWORD MPCharacter::CheckPosePlayingStatus()
{
    const lwIPoseCtrl* ctrl = GetPoseCtrl();

    if( ctrl == 0 )
    {
        // no bone pose ctrl, we return 0;
        return 0;
    }

    return ctrl->IsPosePlaying( GetPlayPoseInfo() );
}

MPSceneItem* MPCharacter::GetLinkItem( DWORD id )
{
    if( id < 0 || id >= _link_item_num )
        return NULL;

    return _link_item_seq[ id ].obj;
}

void MPCharacter::SetObjState( DWORD state, BYTE value )
{
    _physique->SetObjState( state, value );
}
DWORD MPCharacter::GetObjState( DWORD state ) const
{
    return _physique->GetObjState( state );
}

LW_RESULT MPCharacter::GetObjDummyRunTimeMatrix( lwMatrix44* mat, DWORD obj_id, DWORD dummy_id )
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIPrimitive* p = _physique->GetPrimitive( obj_id );   
    if(p == NULL)
        goto __ret;
    {
        lwIHelperObject* h = p->GetHelperObject();
        if (h == 0)
            goto __ret;

        lwIHelperDummy* dummy = h->GetHelperDummy();
        if (dummy == NULL)
            goto __ret;

        lwHelperDummyInfo* dummy_info = dummy->GetDataInfoWithID(dummy_id);

        if (dummy_info == NULL)
            goto __ret;

        lwMatrix44Multiply(mat, &dummy_info->mat, dummy->GetMatrixParent());

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}

LW_RESULT MPCharacter::GetObjDummyRunTimeMatrix( lwMatrix44* mat, DWORD dummy_id )
{
    LW_RESULT ret = LW_RET_FAILED;

    lwIAnimCtrlAgent* anim_agent = _physique->GetAnimCtrlAgent();
    if(anim_agent == 0)
        goto __ret;

    {
        lwAnimCtrlObjTypeInfo type_info;
        type_info.type = ANIM_CTRL_TYPE_BONE;
        type_info.data[0] = LW_INVALID_INDEX;
        type_info.data[1] = LW_INVALID_INDEX;


        lwIAnimCtrlObjBone* ctrl_obj_bone = (lwIAnimCtrlObjBone*)anim_agent->GetAnimCtrlObj(&type_info);
        if (ctrl_obj_bone == 0)
            goto __ret;

        lwMatrix44* m = ctrl_obj_bone->GetDummyRTM(dummy_id);

        if (m == NULL)
            return LW_RET_FAILED;

        lwMatrix44Multiply(mat, m, _mat_ptr);
        //*mat = *m;

        ret = LW_RET_OK;
    }
__ret:
    return ret;
}

void MPCharacter::SetMaterial(const D3DMATERIALX* mtl)
{
    lwPhysiqueSetMaterial(_physique, (lwMaterial*)mtl);
}
void MPCharacter::SetOpacity(float opacity)
{
    _physique->SetOpacity(opacity);
}
void MPCharacter::SetTextureLOD(DWORD level)
{
    _physique->SetTextureLOD(level);
}
float MPCharacter::GetOpacity()
{
    return _physique->GetOpacity();
}

LW_END
