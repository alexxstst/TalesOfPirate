#pragma once

#include "CRCursorObj.h"

class CShadeInfo;
class CShadeEff;

class CCircle3DCursor : public CursorObj {
public:
	CCircle3DCursor();
	~CCircle3DCursor();

	virtual void Init(CGameScene* p);
	virtual void Clear();

	void MouseDown(int nButton);
	void MouseUp(int nButton);
	void MoveTo(D3DXVECTOR3& stPos);

	void FrameMove(DWORD dwTime);
	void Render();

	void SetRadius(unsigned int r);
	void SetIsShow(bool bShow);

protected:
	CShadeEff* _pShade;
	CGameScene* _pScene;

	int _nRadius;
	BOOL _IsHide;
};
