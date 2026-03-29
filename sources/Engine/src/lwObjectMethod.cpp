//
#include "stdafx.h"
#include "lwObjectMethod.h"


LW_BEGIN

lwMatrixCtrl::lwMatrixCtrl()
:
_mat_ptr( 0 ),
_angle_yaw( D3DX_PI ),
_angle_pitch( 0.0f ),
_angle_roll( 0.0f ),

// Added by clp
_scale( 1.f, 1.f, 1.f )
{
}

void lwMatrixCtrl::SetScale( const lwVector3 &scale )
{
	_scale = scale;
}

void lwMatrixCtrl::SetPos( float* pos )
{
	lwMatrix44Translate( _mat_ptr, pos[0], pos[1], pos[2], OP_MATRIX_REPLACE );
}

void lwMatrixCtrl::SetYaw( float yaw )
{
	_angle_yaw = yaw;
}
void lwMatrixCtrl::SetPitch( float pitch )
{
	_angle_pitch = pitch;
}
void lwMatrixCtrl::SetRoll( float roll )
{
    _angle_roll = roll;
}
void lwMatrixCtrl::UpdateYawPitchRoll()
{
    lwMatrix44 m( lwMatrix44_Identity );

    lwMatrix44 m_yaw = lwMatrix44RotateZ( -_angle_yaw + LW_PI );
    lwMatrix44 m_pitch = lwMatrix44RotateY( - _angle_pitch );
    lwMatrix44 m_roll = lwMatrix44RotateX( - _angle_roll );
    lwMatrix44 m_scale = lwMatrix44Scale(_scale.x, _scale.y, _scale.z);

    lwMatrix44Multiply( &m, &m_yaw, &m_pitch );
    lwMatrix44Multiply( &m, &m, &m_roll );
    lwMatrix44Multiply( &m, &m, &m_scale);


    memcpy( _mat_ptr, &m, sizeof(float) * 12 );
}

LW_END
