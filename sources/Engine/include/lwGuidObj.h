//
#pragma once

#include "lwHeader.h"
#include "lwErrorCode.h"

#include "lwFrontAPI.h"

LW_BEGIN

//struct lwGUID
//{
//    union {
//        struct {
//            LW_DWORD id;
//            LW_DWORD data;
//        };
//
//        LW_INT64 guid;
//    };
//};
typedef LW_INT64 lwGUID;

LW_FRONT_API LW_RESULT lwGUIDCreateObject(LW_VOID** obj, lwGUID guid);
#define lwCreateObjectGUID lwGUIDCreateObject

const lwGUID LW_GUID_SYSTEM =               LW_MAKEINT64(0, 1);
const lwGUID LW_GUID_SYSGRAPHICS =          LW_MAKEINT64(1, 0);
const lwGUID LW_GUID_DEVICEOBJECT =         LW_MAKEINT64(2, 0);
const lwGUID LW_GUID_RESOURCEMGR =          LW_MAKEINT64(3, 0);
const lwGUID LW_GUID_DYNAMICSTREAMMGR =     LW_MAKEINT64(4, 1);

const lwGUID LW_GUID_ANIMCTRLBONE =         LW_MAKEINT64(10, 0);
const lwGUID LW_GUID_PATHINFO =             LW_MAKEINT64(11, 0);
const lwGUID LW_GUID_POSECTRL =             LW_MAKEINT64(12, 0);
const lwGUID LW_GUID_ANIMDATABONE =         LW_MAKEINT64(13, 0);
const lwGUID LW_GUID_ANIMDATAMATRIX =       LW_MAKEINT64(14, 0);
const lwGUID LW_GUID_HELPERDUMMY =          LW_MAKEINT64(15, 0);
const lwGUID LW_GUID_HELPERBOX =            LW_MAKEINT64(16, 0);
const lwGUID LW_GUID_HELPERMESH =           LW_MAKEINT64(17, 0);
const lwGUID LW_GUID_ACTIONSMITH =          LW_MAKEINT64(21, 0);
const lwGUID LW_GUID_ANIMDATATEXUV =        LW_MAKEINT64(22, 0);
const lwGUID LW_GUID_MESHINFO =             LW_MAKEINT64(23, 0);
const lwGUID LW_GUID_OPTIONMGR =            LW_MAKEINT64(24, 0);
const lwGUID LW_GUID_ANIMKEYSETPRS =        LW_MAKEINT64(25, 0);
const lwGUID LW_GUID_ANIMKEYSETFLOAT =      LW_MAKEINT64(26, 0);
const lwGUID LW_GUID_TREENODE =             LW_MAKEINT64(27, 0);
const lwGUID LW_GUID_RENDERSTATEATOMSET =   LW_MAKEINT64(28, 0);
const lwGUID LW_GUID_TIMER =                LW_MAKEINT64(29, 0);
const lwGUID LW_GUID_TIMERTHREAD =          LW_MAKEINT64(30, 0);
const lwGUID LW_GUID_TIMERPERIOD =          LW_MAKEINT64(31, 0);
const lwGUID LW_GUID_FILE =                 LW_MAKEINT64(32, 0);
const lwGUID LW_GUID_FILEDIALOG =           LW_MAKEINT64(33, 0);
const lwGUID LW_GUID_FILESTREAM =           LW_MAKEINT64(34, 0);
const lwGUID LW_GUID_DDSFILE =              LW_MAKEINT64(35, 0);
const lwGUID LW_GUID_DIRECTORYBROWSER =     LW_MAKEINT64(36, 0);
const lwGUID LW_GUID_CONSOLE =              LW_MAKEINT64(37, 0);
const lwGUID LW_GUID_THREADPOOL =           LW_MAKEINT64(38, 0);
const lwGUID LW_GUID_TEXTFILELOADER =       LW_MAKEINT64(39, 0);
const lwGUID LW_GUID_GEOMOBJINFO =          LW_MAKEINT64(40, 0);
const lwGUID LW_GUID_MODELOBJINFO =         LW_MAKEINT64(41, 0);
const lwGUID LW_GUID_SYSTEMINFO =           LW_MAKEINT64(42, 0);
const lwGUID LW_GUID_BUFFER =               LW_MAKEINT64(43, 0);
const lwGUID LW_GUID_HEAP =                 LW_MAKEINT64(44, 0);



const LW_DWORD LW_GUID_NUM = 99;

LW_END
