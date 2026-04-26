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

    return LW_RET_OK;
}

LW_END
