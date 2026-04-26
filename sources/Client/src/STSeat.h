#pragma once
#include "STStateObj.h"

class CSeatState : public CActionState {
public:
	CSeatState(CActor* p);

	virtual std::string_view GetExplain() {
		return "CSeatState";
	}

	virtual void FrameMove();

	virtual void BeforeNewState() {
		PopState();
	}

	void SetAngle(int nAngle) {
		_nAngle = nAngle;
	}

	void SetPos(int nX, int nY) {
		_nPosX = nX;
		_nPosY = nY;
	}

	void SetPose(int nPos) {
		_nPose = nPos;
	}

	void SetHeight(int nHeight) {
		_nHeight = nHeight;
	}

	int GetPosX() {
		return _nPosX;
	}

	int GetPosY() {
		return _nPosY;
	}

	int GetHeight() {
		return _nHeight;
	}

	int GetAngle() {
		return _nAngle;
	}

	int GetPose() {
		return _nPose;
	}

protected:
	virtual bool _Start();
	virtual void _End();

	virtual bool _IsAllowCancel() {
		return true;
	}

protected:
	int _nPose;
	int _nAngle;
	int _nPosX, _nPosY;
	int _nHeight;
	bool _IsSeat;

private:
	int _nOldChaHeight;
	int _nOldX, _nOldY;
	CCharacter* _pCha;
};
