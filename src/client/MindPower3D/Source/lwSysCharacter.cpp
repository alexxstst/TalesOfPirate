//
#include "stdafx.h"
#include "lwSysCharacter.h"
#include "lwPhysique.h"

LW_BEGIN

lwSysCharacter::lwSysCharacter()
{
    _pool_skeleton = LW_NEW( lwObjectPoolSkeleton );
    _pool_skinmesh = LW_NEW( lwObjectPoolSkin );
}

lwSysCharacter::~lwSysCharacter()
{
    LW_SAFE_DELETE( _pool_skeleton );
    LW_SAFE_DELETE( _pool_skinmesh );
}


LW_RESULT lwSysCharacter::QuerySkeleton( DWORD* ret_id, const char* file )
{
    //DWORD pool_num = lwObjectPoolSkeleton::POOL_SIZE;
    //lwSkeleton* sk;
    //for( int i = 0; i < lwObjectPoolSkeleton::POOL_SIZE; i++ )
    //{
    //    if( _pool_skeleton->GetObj( i ) 
    //}
    //if( param.id != LW_INVALID_INDEX )
    //{
    //    *ret_id = param.id;
    //}
    //else
    //{
    //}

    return LW_RET_OK;
}

LW_END
