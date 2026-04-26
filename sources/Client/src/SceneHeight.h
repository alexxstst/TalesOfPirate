#pragma once

#include "Scene.h"

// FrameMove
class CSceneHeight {
public:
	CSceneHeight(CSceneNode* pNode) : _pNode(pNode) {
		Reset();
	}

	void Reset() {
		_dwLastTime = 0;
		_fSceneHeight = -12345.0f;
		_nPosX = -1;
		_nPosY = -1;
	}

	bool FrameMove(DWORD dwTime) {
		if (dwTime < _dwLastTime) return false;
		_dwLastTime = dwTime + 500;

		float fHeight = -12345.0f;
		if (_nPosX == _pNode->GetCurX() && _nPosY == _pNode->GetCurY()) {
			fHeight = _pNode->GetScene()->GetGridHeight((float)_pNode->GetCurX() / 100.0f,
														(float)_pNode->GetCurY() / 100.0f);
			if (_fSceneHeight == fHeight) {
				return false;
			}
		}

		_nPosX = _pNode->GetCurX();
		_nPosY = _pNode->GetCurY();
		_fSceneHeight = fHeight;
		return true;
	}

private:
	CSceneNode* _pNode;
	DWORD _dwLastTime;
	float _fSceneHeight;
	int _nPosX, _nPosY;
};
