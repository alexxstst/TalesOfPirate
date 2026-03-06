//
#include "stdafx.h"
#include "lwCamera.h"

LW_BEGIN

// lwCamera
////////////////////////////////////////
void lwCamera::GetMatrixCameraToView( lwMatrix44* mat, const lwMatrix44* mat_cam )
{
	mat->m[0][0] = mat_cam->m[0][0];
	mat->m[1][0] = mat_cam->m[0][1];
	mat->m[2][0] = mat_cam->m[0][2];
	
	mat->m[0][1] = mat_cam->m[1][0];
	mat->m[1][1] = mat_cam->m[1][1];
	mat->m[2][1] = mat_cam->m[1][2];

	mat->m[0][2] = mat_cam->m[2][0];
	mat->m[1][2] = mat_cam->m[2][1];
	mat->m[2][2] = mat_cam->m[2][2];

	mat->m[3][0] = -lwVector3Dot((const lwVector3*)&mat_cam->m[0][0],(const lwVector3*)&mat_cam->m[3][0]);
	mat->m[3][1] = -lwVector3Dot((const lwVector3*)&mat_cam->m[1][0],(const lwVector3*)&mat_cam->m[3][0]);
	mat->m[3][2] = -lwVector3Dot((const lwVector3*)&mat_cam->m[2][0],(const lwVector3*)&mat_cam->m[3][0]);

	mat->m[0][3] = 0.0f;
	mat->m[1][3] = 0.0f;
	mat->m[2][3] = 0.0f;
	mat->m[3][3] = 1.0f;

}
void lwCamera::GetMatrixViewToCamera( lwMatrix44* mat, const lwMatrix44* mat_view)
{
	mat->m[0][0] = mat_view->m[0][0];
	mat->m[1][0] = mat_view->m[0][1];
	mat->m[2][0] = mat_view->m[0][2];
	
	mat->m[0][1] = mat_view->m[1][0];
	mat->m[1][1] = mat_view->m[1][1];
	mat->m[2][1] = mat_view->m[1][2];

	mat->m[0][2] = mat_view->m[2][0];
	mat->m[1][2] = mat_view->m[2][1];
	mat->m[2][2] = mat_view->m[2][2];


	float e1[] = { mat_view->_11, mat_view->_21, mat_view->_31, -mat_view->_41 };
	float e2[] = { mat_view->_12, mat_view->_22, mat_view->_32, -mat_view->_42 };
	float e3[] = { mat_view->_13, mat_view->_23, mat_view->_33, -mat_view->_43 };

    lwGetEquation3( (float*)&mat->_41, e1, e2, e3 );

	mat->m[0][3] = 0.0f;
	mat->m[1][3] = 0.0f;
	mat->m[2][3] = 0.0f;
	mat->m[3][3] = 1.0f;
}

lwCamera::lwCamera()
{
	lwMatrix44Identity(&_m);
}


int lwCamera::SetPerspectiveFov(float fov,float aspect,float zn,float zf)
{
	//Here we specify the field of view, aspect ration and near and far clipping planes.
    //D3DXMATRIX m_proj;
    //D3DXMatrixPerspectiveFovLH(&m_proj, fov, aspect, zn, zf);
    //_dev->SetTransform(D3DTS_PROJECTION, &m_proj);

	_fov = fov;
	_aspect = aspect;
	_znear = zn;
	_zfar = zf;

	return 1;
}

void lwCamera::SetPosition(lwVector3* pos)
{
	lwMatrix44Translate(&_m,pos->x,pos->y,pos->z,OP_MATRIX_LOCAL);
}
void lwCamera::SetPosition(lwVector3* face,lwVector3* up)
{
}
void lwCamera::SetPosition(lwVector3* pos,lwVector3* face,lwVector3* up)
{
}
void lwCamera::Rotate(DWORD axis,float angle)
{
	lwMatrix44RotateAxis(&_m,axis,angle,OP_MATRIX_LOCAL);
}

void lwCamera::SetTransform()
{
    D3DXMATRIX m_view;

	//_m.m[2][0] = 100.0f;
	//_m.m[2][1] = 0.0f;
	//_m.m[2][2] = 0.0f;

	//D3DXMatrixLookAtLH( &m_view, 
	//	(D3DXVECTOR3*)(&_m.m[3][0]), //Camera Position
	//	(D3DXVECTOR3*)(&_m.m[2][0]), //Look At Position
	//	(D3DXVECTOR3*)(&_m.m[1][0]));  //Up Direction

	//zaxis = normal(At - Eye)
	//	xaxis = normal(cross(Up, zaxis))
	//	yaxis = cross(zaxis, xaxis)

	//	xaxis.x           yaxis.x           zaxis.x          0
	//	xaxis.y           yaxis.y           zaxis.y          0
	//	xaxis.z           yaxis.z           zaxis.z          0
	//	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1

	//_m.m[2][0] = 0.0f;
	//_m.m[2][1] = 0.0f;
	//_m.m[2][2] = 1.0f;
	
	m_view.m[0][0] = _m.m[0][0];
	m_view.m[1][0] = _m.m[0][1];
	m_view.m[2][0] = _m.m[0][2];
	
	m_view.m[0][1] = _m.m[1][0];
	m_view.m[1][1] = _m.m[1][1];
	m_view.m[2][1] = _m.m[1][2];

	m_view.m[0][2] = _m.m[2][0];
	m_view.m[1][2] = _m.m[2][1];
	m_view.m[2][2] = _m.m[2][2];

	m_view.m[3][0] = -lwVector3Dot((const lwVector3*)&_m.m[0][0],(const lwVector3*)&_m.m[3][0]);
	m_view.m[3][1] = -lwVector3Dot((const lwVector3*)&_m.m[1][0],(const lwVector3*)&_m.m[3][0]);
	m_view.m[3][2] = -lwVector3Dot((const lwVector3*)&_m.m[2][0],(const lwVector3*)&_m.m[3][0]);

	m_view.m[0][3] = 0.0f;
	m_view.m[1][3] = 0.0f;
	m_view.m[2][3] = 0.0f;
	m_view.m[3][3] = 1.0f;

    //_dev->SetTransform(D3DTS_VIEW, &m_view);
 
}


void lwCamera::GetMatrixProj( lwMatrix44* mat )
{
	D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)mat, _fov, _aspect, _znear, _zfar);
}
void lwCamera::GetMatrixView( lwMatrix44* mat )
{
    lwCamera::GetMatrixCameraToView( mat, &_m );
}

LW_END
