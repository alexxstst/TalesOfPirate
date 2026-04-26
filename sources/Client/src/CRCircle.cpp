#include "stdafx.h"
#include "crcircle.h"

#include "EffectObj.h"
#include "EffectRecordStore.h"
#include "ShadeRecordStore.h"

#include "Scene.h"

CCircle3DCursor::CCircle3DCursor() {
	_pShade = NULL;
	_pScene = NULL;

	_nRadius = -1;

	_IsHide = FALSE;
}

CCircle3DCursor::~CCircle3DCursor() {
}

void CCircle3DCursor::Init(CGameScene* p) {
	_pShade = NULL;
	_pScene = NULL;
	_nRadius = -1;
	_IsHide = FALSE;

	_pScene = p;
}

void CCircle3DCursor::SetRadius(unsigned int r) {
	if (_nRadius == r) {
		if (_pShade && _pShade->IsHide() != _IsHide) {
			_pShade->SetHide(_IsHide);
		}
		return;
	}

	_nRadius = r;
	if (_pShade) {
		_pShade->SetValid(FALSE);
		_pShade = NULL;
	}

	CShadeInfo* pInfo = NULL;
	pInfo = GetShadeInfo(r <= 0 ? 6 : 4);
	if (!pInfo)
		return;

	CShadeInfo Info = *pInfo;
	if (_nRadius > 0) Info.fsize = (float)_nRadius / 100.0f;

	_pShade = _pScene->GetFirstInvalidShadeObj();
	if (!_pShade || !_pShade->Create(&Info)) {
		_pShade = NULL;
		return;
	}
	const auto v = D3DXVECTOR3(0, 0, 0);
	_pShade->Emission(0, &v, nullptr);
	_pShade->setTypeID(0);
	_pShade->setUpSea(true);
	_pShade->SetValid(TRUE);
}

void CCircle3DCursor::Clear() {
	if (_pShade) {
		_pShade->SetValid(FALSE);
		_pShade = NULL;
	}

	_pScene = NULL;
}

void CCircle3DCursor::MouseDown(int nButton) {
}

void CCircle3DCursor::MouseUp(int nButton) {
}

void CCircle3DCursor::MoveTo(D3DXVECTOR3& stPos) {
	if (_pShade) {
		_pShade->MoveTo(&stPos);
	}
}

void CCircle3DCursor::SetIsShow(bool bShow) {
	_IsHide = bShow ? FALSE : TRUE;
	if (_pShade) {
		_pShade->SetHide(_IsHide);
	}
}

void CCircle3DCursor::FrameMove(DWORD dwTime) {
}

void CCircle3DCursor::Render() {
}
