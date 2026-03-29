//
#pragma once

#include "lwHeader.h"
#include "lwInterfaceExt.h"
#include "lwStdInc.h"
#include "lwDirectX.h"
#include "lwClassDecl.h"

LW_BEGIN

struct lwCoordInfo
{
    DWORD type;
    lwMatrix44 mat;
};

enum lwCoordIndex
{
    COORDSYS_0 =            0,
    COORDSYS_1 =            1,
    COORDSYS_2 =            2,
    COORDSYS_3 =            3,
    COORDSYS_4 =            4,
    COORDSYS_5 =            5,
    COORDSYS_6 =            6,
    COORDSYS_7 =            7,

    COORD_SEQ_SIZE =        8,
};

class lwCoordinateSys : public lwICoordinateSys
{
    LW_STD_DECLARATION();

private:
    DWORD _active_id;
    lwMatrix44 _coord_mat_seq[COORD_SEQ_SIZE];

public:
    lwCoordinateSys();

    LW_RESULT SetActiveCoordSys(DWORD id);
    LW_RESULT SetCoordSysMatrix(DWORD id, const lwMatrix44* mat);
    LW_RESULT GetCoordSysMatrix(DWORD id, lwMatrix44* mat);
    LW_RESULT GetCurCoordSysMatrix(lwMatrix44* mat);
    LW_RESULT GetActiveCoordSys(DWORD* id);
};


LW_END
