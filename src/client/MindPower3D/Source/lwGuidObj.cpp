//
#include "stdafx.h"
#include "lwGuidObj.h"
#include "lwSystem.h"
//#include "lwSysGraphics.h"
//#include "lwPhysique.h"
#include "lwAnimCtrl.h"
#include "lwPoseCtrl.h"
#include "lwStreamObj.h"
#include "lwExpObj.h"
#include "lwAnimKeySetPRS.h"
#include "lwTreeNode.h"
#include "lwMisc.h"
#include "lwTimer.h"
#include "lwFile.h"
#include "lwDDSFile.h"
#include "lwDirectoryBrowser.h"
#include "lwConsole.h"
#include "lwThreadPool.h"
#include "lwSystemInfo.h"
#include "lwMisc.h"
#include "lwHeap.h"
#include "lwFileStream.h"

#if(!defined USE_MINDPOWER)
//#include "lwAction.h"
#include "lwTextFileLoader.h"
#endif

LW_BEGIN

typedef void* (*lwCreateGuidObjProc)();


static void* __guid_proc_NULL()
{
    return LW_NULL;
}
static void* __guid_proc_lwSystem()
{
    return LW_NEW(lwSystem);
}
//static void* __guid_proc_lwDyanmicStreamMgr()
//{
//    return LW_NEW(lwDynamicStreamMgr);
//}
//static void* __guid_proc_lwSysGraphics()
//{
//    return LW_NEW(lwSysGraphics);
//}
//static void* __guid_proc_lwAnimCtrlBone()
//{
//    return LW_NEW(lwAnimCtrlBone);
//}
static void* __guid_proc_lwPoseCtrl()
{
    return LW_NEW(lwPoseCtrl);
}
static void* __guid_proc_lwAnimDataBone()
{
    return LW_NEW(lwAnimDataBone);
}
static void* __guid_proc_lwAnimDataTexUV()
{
    return LW_NEW(lwAnimDataTexUV);
}
static void* __guid_proc_lwMeshInfo()
{
    return LW_NEW(lwMeshInfo);
}
static void* __guid_proc_lwAnimKeySetPRS()
{
    return LW_NEW(lwAnimKeySetPRS2);
}
static void* __guid_proc_lwAnimKeySetFloat()
{
    return LW_NEW(lwAnimKeySetFloat);
}
static void* __guid_proc_lwTreeNode()
{
    return LW_NEW(lwTreeNode);
}
static void* __guid_proc_lwRenderStateAtomSet()
{
    return LW_NEW(lwRenderStateAtomSet);
}
static void* __guid_proc_lwTimer()
{
    return LW_NEW(lwTimer);
}
static void* __guid_proc_lwTimerThread()
{
    return LW_NEW(lwTimerThread);
}
static void* __guid_proc_lwTimerPeriod()
{
    return LW_NEW(lwTimerPeriod);
}
static void* __guid_proc_lwFile()
{
    return LW_NEW(lwFile);
}
static void* __guid_proc_lwFileDialog()
{
    return LW_NEW(lwFileDialog);
}
static void* __guid_proc_lwFileStream()
{
    return LW_NEW(lwFileStream);
}
static void* __guid_proc_lwDDSFile()
{
    return LW_NEW(lwDDSFile);
}
static void* __guid_proc_lwDirectoryBrowser()
{
    return LW_NEW(lwDirectoryBrowser);
}
static void* __guid_proc_lwConsole()
{
    return LW_NEW(lwConsole);
}
static void* __guid_proc_lwThreadPool()
{
    return LW_NEW(lwThreadPool);
}
static void* __guid_proc_lwGeomObjInfo()
{
    return LW_NEW(lwGeomObjInfo);
}
static void* __guid_proc_lwModelObjInfo()
{
    return LW_NEW(lwModelObjInfo);
}
static void* __guid_proc_lwSystemInfo()
{
    return LW_NEW(lwSystemInfo);
}
static void* __guid_proc_lwBuffer()
{
    return LW_NEW(lwBuffer);
}
static void* __guid_proc_lwHeap()
{
    return LW_NEW(lwHeap);
}

#if(!defined USE_MINDPOWER)
//static void* __guid_proc_lwActionSmith()
//{
//    return LW_NEW(lwActionSmith);
//}
#define __guid_proc_lwActionSmith __guid_proc_NULL
static void* __guid_proc_lwTextFileLoader()
{
    return LW_NEW(lwTextFileLoader);
}

#else
    
#define __guid_proc_lwActionSmith __guid_proc_NULL
#define __guid_proc_lwTextFileLoader __guid_proc_NULL

#endif

static lwCreateGuidObjProc __guid_proc_buf[LW_GUID_NUM] =
{
    __guid_proc_lwSystem,
    __guid_proc_NULL,//__guid_proc_lwSysGraphics,    
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL,//__guid_proc_lwDyanmicStreamMgr,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL, //__guid_proc_lwAnimCtrlBone, // 10
    __guid_proc_NULL,
    __guid_proc_lwPoseCtrl,
    __guid_proc_lwAnimDataBone,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL,
    __guid_proc_NULL, // 20
    __guid_proc_lwActionSmith,

    __guid_proc_lwAnimDataTexUV,
    __guid_proc_lwMeshInfo,
    __guid_proc_NULL,
    __guid_proc_lwAnimKeySetPRS,
    __guid_proc_lwAnimKeySetFloat,
    __guid_proc_lwTreeNode,
    __guid_proc_lwRenderStateAtomSet,
    __guid_proc_lwTimer,
    __guid_proc_lwTimerThread,
    __guid_proc_lwTimerPeriod,
    __guid_proc_lwFile,
    __guid_proc_lwFileDialog,
    __guid_proc_lwFileStream,
    __guid_proc_lwDDSFile,
    __guid_proc_lwDirectoryBrowser,
    __guid_proc_lwConsole,
    __guid_proc_lwThreadPool,
    __guid_proc_lwTextFileLoader,
    __guid_proc_lwGeomObjInfo,
    __guid_proc_lwModelObjInfo,
    __guid_proc_lwSystemInfo, // 41
    __guid_proc_lwBuffer,
    __guid_proc_lwHeap,

};

LW_RESULT lwGUIDCreateObject(LW_VOID** obj, lwGUID guid)
{
    return (*obj = __guid_proc_buf[LW_LODWORD(guid)]()) ? LW_RET_OK : LW_RET_FAILED;

}

LW_END
