//
#include "lwHeader.h"
#include "lwObjectPool.h"

LW_BEGIN

typedef lwObjectPoolVoidPtr1024 lwObjectPoolSkeleton;
typedef lwObjectPoolVoidPtr1024 lwObjectPoolSkin;


class lwSysCharacter
{
private:
    lwObjectPoolSkeleton* _pool_skeleton;
    lwObjectPoolSkin* _pool_skinmesh;

public:
    lwSysCharacter();
    ~lwSysCharacter();

    LW_RESULT QuerySkeleton( DWORD* ret_id, const char* file );
};

LW_END
