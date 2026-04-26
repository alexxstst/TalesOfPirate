#pragma once
#include "CRCursorObj.h"

class CShadeEff;

class CRCursor : public CursorObj {
public:
	CRCursor();
	~CRCursor();

	virtual void Init(CGameScene* p);
	virtual void Clear();

	virtual void MouseDown(int nButton);
	virtual void MouseUp(int nButton);
	virtual void FrameMove(DWORD dwTime);
	virtual void Render();
	virtual void MoveTo(D3DXVECTOR3& vPos);

private:
	D3DXVECTOR3 _vPos;
	CShadeEff* _pCursorEff;
};
