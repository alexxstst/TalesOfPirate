//
#pragma once

#include "stdafx.h"

typedef LW_RESULT (*lwLitAnimProc)(lwIAnimKeySetPRS* ctrl);

// 120f-rot
LW_RESULT lwLitAnimTexCoord0(lwIAnimKeySetPRS* ctrl);
// 120f-pos
LW_RESULT lwLitAnimTexCoord1(lwIAnimKeySetPRS* ctrl);
// 360f-pos-u
LW_RESULT lwLitAnimTexCoord2(lwIAnimKeySetPRS* ctrl);
// 360f-pos-v
LW_RESULT lwLitAnimTexCoord360posv(lwIAnimKeySetPRS* ctrl);
// 360f-pos-uv
LW_RESULT lwLitAnimTexCoord360posuv(lwIAnimKeySetPRS* ctrl);
// 360f-pos/rot
LW_RESULT lwLitAnimTexCoord3(lwIAnimKeySetPRS* ctrl);
// 360f-pos/rot-neg
LW_RESULT lwLitAnimTexCoord_360_pos_rot_neg(lwIAnimKeySetPRS* ctrl);
// 720f-rot
LW_RESULT lwLitAnimTexCoord4(lwIAnimKeySetPRS* ctrl);

static const char* __lit_anim_type[] =
{
    "Anim Type NULL",
    "Anim Type 120-rot",
    "Anim Type 120-pos",
    "Anim Type 360-pos-u",
    "Anim Type 360-pos-v",
    "Anim Type 360-pos-uv",
    "Anim Type 360-pos/rot",
    "Anim Type 360-pos/rot-neg",
    "Anim Type 720-rot",
};

const DWORD __lit_anim_num = LW_ARRAY_LENGTH(__lit_anim_type);

extern lwLitAnimProc __lit_proc[__lit_anim_num];
