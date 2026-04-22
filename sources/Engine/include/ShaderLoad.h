//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwClassDecl.h"
#include "lwInterface.h"
#include "lwShaderTypes.h"
#include "lwIUtil.h"
LW_USING

enum UserVertexDeclarationType
{
    VDTU_BEGIN = VDT_USER_DEFINE,
    VDTU_EFFECT1,
};

enum UserShaderType
{
    VSTU_BEGIN = VST_USER_DEFINE,

    VSTU_SKINMESH0_TT1,
    VSTU_SKINMESH1_TT1,
    VSTU_SKINMESH2_TT1,
    VSTU_SKINMESH3_TT1,

    VSTU_SKINMESH0_TT2,
    VSTU_SKINMESH1_TT2,
    VSTU_SKINMESH2_TT2,
    VSTU_SKINMESH3_TT2,

    VSTU_SKINMESH0_TT3,
    VSTU_SKINMESH1_TT3,
    VSTU_SKINMESH2_TT3,
    VSTU_SKINMESH3_TT3,

	VSTU_EFFECT_E1,
	VSTU_EFFECT_E2,
	VSTU_EFFECT_E3,
	VSTU_EFFECT_E4,

	VSTU_SHADE_E6,

	VSTU_MINIMAP_E6,
};

LW_RESULT LoadShader0(lwISysGraphics* sys_graphics);
LW_RESULT LoadShader1(lwISysGraphics* sys_graphics);
