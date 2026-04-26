//
#pragma once


#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwInterface.h"
#include "lwITypes.h"
#include "lwITypes2.h"
#include "lwMath.h"
#include "lwShaderTypes.h"
#include "lwIFunc.h"
#include "lwIUtil.h"

LW_USING;

typedef lwAnimCtrlObjTypeInfo MPAnimCtrlObjTypeInfo;
typedef lwVector2 MPVector2;
typedef lwVector3 MPVector3;
typedef lwMatrix44 MPMatrix44;
typedef lwBoundingBoxInfo MPBoundingBoxInfo;
typedef lwPickInfo MPPickInfo;
typedef lwPlayPoseInfo MPPlayPoseInfo;
typedef lwPoseInfo MPPoseInfo;
typedef lwInterfaceMgr MPInterfaceMgr;
typedef lwMeshInfo MPMeshInfo;
typedef lwWatchDevVideoMemInfo MPWatchDevVideoMemInfo;

typedef lwIAnimCtrl MPIAnimCtrl;
typedef lwIAnimCtrlAgent MPIAnimCtrlAgent;
typedef lwIAnimCtrlBone MPIAnimCtrlBone;
typedef lwIAnimCtrlObjBone MPIAnimCtrlObjBone;
typedef lwIBoundingBox MPIBoundingBox;
typedef lwIDeviceObject MPIDeviceObject;
typedef lwStaticStreamMgrDebugInfo MPStaticStreamMgrDebugInfo;
typedef lwD3DCreateParamAdjustInfo MPD3DCreateParamAdjustInfo;
typedef lwDwordByte4 MPDwordByte4;
typedef lwIHelperObject MPIHelperObject;
typedef lwIMesh MPIMesh;
typedef lwIPathInfo MPIPathInfo;
typedef lwIPhysique MPIPhysique;
typedef lwIPoseCtrl MPIPoseCtrl;
typedef lwIPrimitive MPIPrimitive;
typedef lwIRenderStateAtomSet MPIRenderStateAtomSet;
typedef lwIResBufMgr MPIResBufMgr;
typedef lwIResourceMgr MPIResourceMgr;
typedef lwISceneMgr MPISceneMgr;
typedef lwIStaticStreamMgr MPIStaticStreamMgr;
typedef lwISysGraphics MPISysGraphics;
typedef lwISystemInfo MPISystemInfo;
typedef lwISystem MPISystem;
typedef lwITex MPITex;
typedef lwITimer MPITimer;
typedef lwITimerPeriod MPITimerPeriod;

// macro
#ifndef MP_NEW
#define MP_NEW LW_NEW
#endif

#ifndef MP_DELETE
#define MP_DELETE LW_DELETE
#endif

#ifndef MP_DELETE_A
#define MP_DELETE_A LW_DELETE_A
#endif

// method

#ifndef MPMatrix44Multiply
#define MPMatrix44Multiply lwMatrix44Multiply
#endif

#ifndef MPRegisterOutputLoseDeviceProc
#define MPRegisterOutputLoseDeviceProc lwRegisterOutputLoseDeviceProc
#endif

#ifndef MPRegisterOutputResetDeviceProc
#define MPRegisterOutputResetDeviceProc lwRegisterOutputResetDeviceProc
#endif

#ifndef MPVec3Mat44Mul
#define MPVec3Mat44Mul lwVec3Mat44Mul
#endif

#ifndef MPVector3Normalize
#define MPVector3Normalize lwVector3Normalize
#endif

#ifndef MPVector3Slerp
#define MPVector3Slerp lwVector3Slerp
#endif

#ifndef MPVector3Sub
#define MPVector3Sub lwVector3Sub
#endif
