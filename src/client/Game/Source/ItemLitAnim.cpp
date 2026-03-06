//
#include "stdafx.h"
#include "ItemLitAnim.h"

lwLitAnimProc __lit_proc[__lit_anim_num] =
{
    NULL,
    lwLitAnimTexCoord0,
    lwLitAnimTexCoord1,
    lwLitAnimTexCoord2,
    lwLitAnimTexCoord360posv,
    lwLitAnimTexCoord360posuv,
    lwLitAnimTexCoord3,
    lwLitAnimTexCoord_360_pos_rot_neg,
    lwLitAnimTexCoord4,
};

LW_RESULT lwLitAnimTexCoord0(lwIAnimKeySetPRS* ctrl)
{
    // rotation
    {
        lwVector3 axis(0.0f, 0.0f, 1.0f);

        lwKeyQuaternion buf[3];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[0].data, &axis, 0.0f);
        buf[1].key = 60;
        buf[1].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[1].data, &axis, LW_PI);    
        buf[2].key = 119;
        buf[2].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[2].data, &axis, LW_2_PI);    

        ctrl->AddKeyRotation(0, buf, 3);
    }

    return LW_RET_OK;
}

LW_RESULT lwLitAnimTexCoord1(lwIAnimKeySetPRS* ctrl)
{
    lwKeyVector3 buf[2];
    buf[0].key = 0;
    buf[0].slerp_type = AKST_LINEAR;
    buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
    buf[1].key = 119;
    buf[1].slerp_type = AKST_LINEAR;
    buf[1].data = lwVector3(1.0f, 1.0f, 0.0f);

    ctrl->AddKeyPosition(0, buf, 2);

    return LW_RET_OK;
}
LW_RESULT lwLitAnimTexCoord2(lwIAnimKeySetPRS* ctrl)
{
    // position
    {
        lwKeyVector3 buf[2];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
        buf[1].key = 359;
        buf[1].slerp_type = AKST_LINEAR;
        buf[1].data = lwVector3(0.0f, 1.0f, 0.0f);

        ctrl->AddKeyPosition(0, buf, 2);
    }

    return LW_RET_OK;
}
LW_RESULT lwLitAnimTexCoord360posv(lwIAnimKeySetPRS* ctrl)
{
    // position
    {
        lwKeyVector3 buf[2];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
        buf[1].key = 359;
        buf[1].slerp_type = AKST_LINEAR;
        buf[1].data = lwVector3(1.0f, 0.0f, 0.0f);

        ctrl->AddKeyPosition(0, buf, 2);
    }

    return LW_RET_OK;
}
LW_RESULT lwLitAnimTexCoord360posuv(lwIAnimKeySetPRS* ctrl)
{
    // position
    {
        lwKeyVector3 buf[2];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
        buf[1].key = 359;
        buf[1].slerp_type = AKST_LINEAR;
        buf[1].data = lwVector3(1.0f, 1.0f, 0.0f);

        ctrl->AddKeyPosition(0, buf, 2);
    }

    return LW_RET_OK;
}


LW_RESULT lwLitAnimTexCoord3(lwIAnimKeySetPRS* ctrl)
{
    // position
    {
        lwKeyVector3 buf[2];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
        buf[1].key = 359;
        buf[1].slerp_type = AKST_LINEAR;
        buf[1].data = lwVector3(1.0f, 1.0f, 0.0f);

        ctrl->AddKeyPosition(0, buf, 2);
    }

    // rotation
    {
        lwVector3 axis(0.0f, 0.0f, 1.0f);

        lwKeyQuaternion buf[3];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[0].data, &axis, 0.0f);
        buf[1].key = 180;
        buf[1].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[1].data, &axis, LW_PI);    
        buf[2].key = 359;
        buf[2].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[2].data, &axis, LW_2_PI);    

        ctrl->AddKeyRotation(0, buf, 3);
    }

    return LW_RET_OK;
}
// 360f-pos/rot-neg
LW_RESULT lwLitAnimTexCoord_360_pos_rot_neg(lwIAnimKeySetPRS* ctrl)
{
    // position
    {
        lwKeyVector3 buf[2];
        buf[0].key = 0;
        buf[0].slerp_type = AKST_LINEAR;
        buf[0].data = lwVector3(0.0f, 0.0f, 0.0f);
        buf[1].key = 359;
        buf[1].slerp_type = AKST_LINEAR;
        buf[1].data = lwVector3(1.0f, 1.0f, 0.0f);

        ctrl->AddKeyPosition(0, buf, 2);
    }

    // rotation
    {
        lwVector3 axis(0.0f, 0.0f, 1.0f);

        lwKeyQuaternion buf[3];
        buf[0].key = 359;
        buf[0].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[0].data, &axis, 0.0f);
        buf[1].key = 180;
        buf[1].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[1].data, &axis, LW_PI);    
        buf[2].key = 0;
        buf[2].slerp_type = AKST_LINEAR;
        lwQuaternionRotationAxis(&buf[2].data, &axis, LW_2_PI);    

        ctrl->AddKeyRotation(0, buf, 3);
    }

    return LW_RET_OK;
}


LW_RESULT lwLitAnimTexCoord4(lwIAnimKeySetPRS* ctrl)
{
    // rotation
    lwVector3 axis(0.0f, 0.0f, 1.0f);

    lwKeyQuaternion buf[3];
    buf[0].key = 0;
    buf[0].slerp_type = AKST_LINEAR;
    lwQuaternionRotationAxis(&buf[0].data, &axis, 0.0f);
    buf[1].key = 360;
    buf[1].slerp_type = AKST_LINEAR;
    lwQuaternionRotationAxis(&buf[1].data, &axis, LW_PI);    
    buf[2].key = 719;
    buf[2].slerp_type = AKST_LINEAR;
    lwQuaternionRotationAxis(&buf[2].data, &axis, LW_2_PI);    

    ctrl->AddKeyRotation(0, buf, 3);

    return LW_RET_OK;
}

