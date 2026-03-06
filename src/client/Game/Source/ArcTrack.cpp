#include "stdafx.h"
#include "arctrack.h"
#include "GameApp.h"

void CArcTrack::Start( const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd, float fVel, float fHei )
{
    _vStart = vStart;
    _vEnd = vEnd;
    _fHei = fHei;
    _fVel = fVel / 1000.0f;		// Edit by Mdr.st original 1000.0f

	_vOrg = D3DXVECTOR3(-_fHei,0,0);
	_fCurAngle = 0;
    _isEnd = false;

	_dwLastTime = CGameApp::GetCurTick();
	_dwOverTime = _dwLastTime + 2000;		// Edit by Mdr.st
}

void CArcTrack::FrameMove()
{
    if( _isEnd ) return;

	if( CGameApp::GetCurTick() > _dwOverTime )
	{
		_vPos = _vEnd;
		_isEnd = true;
		return;
	}

	if( CGameApp::GetCurTick()<=_dwLastTime ) return;

    float fres = 0;
    _fCurAngle += _fVel* (float)(CGameApp::GetCurTick() - _dwLastTime);

	D3DXMATRIX		mat;
	D3DXVECTOR4		ver;
	D3DXMatrixRotationY(&mat,_fCurAngle);
	D3DXVec3Transform(&ver, &_vOrg, &mat);

	fres = ver.x + _fHei;
	if (fabs (fres) < 0.0000001 )
		fres =0.0f;
	fres /= _fHei +_fHei;

	if(fres >= 0.9f)
	{
		_vPos = _vEnd;
        _isEnd = true;
	}
	else
	{
		_vPos.z = _vStart.z + ver.z;
		_vPos.x = _vStart.x * (1.0f - fres) + _vEnd.x * fres;
		_vPos.y = _vStart.y * (1.0f - fres) + _vEnd.y * fres;		// Original: 1.0f
	}

	_dwLastTime = CGameApp::GetCurTick();
}
