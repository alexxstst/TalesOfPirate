#pragma once

class CArcTrack {
public:
	CArcTrack() : _isEnd(true) {
	}

	void Start(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vEnd, float fVel, float fHei);
	void FrameMove();

	D3DXVECTOR3 GetPos() {
		return _vPos;
	}

	bool IsEnd() {
		return _isEnd;
	}

private:
	D3DXVECTOR3 _vStart, _vEnd, _vOrg, _vPos;
	float _fHei, _fVel, _fCurAngle;
	bool _isEnd;
	DWORD _dwLastTime;
	DWORD _dwOverTime;
};
