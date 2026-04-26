#pragma once

/************************************************************************/
/* class CMPEffectCtrl*/
/************************************************************************/
class CMPEffectCtrl;

#pragma warning(disable: 4275)

#include "EffParamRecord.h"
#include "EffParamRecordStore.h"
#include "GroupParamRecord.h"
#include "GroupParamRecordStore.h"
#include "AssetDatabase.h"
#include "TableData.h"

inline EFF_Param* GetEFFParam(int nTypeID) {
	return EffParamRecordStore::Instance()->Get(nTypeID);
}

inline Group_Param* GetGroupParam(int nTypeID) {
	return GroupParamRecordStore::Instance()->Get(nTypeID);
}

class CMPEffectCtrl {
public:
	CMPEffectCtrl(void);
	~CMPEffectCtrl(void);

public:
	void Emission(WORD wID, D3DXVECTOR3* vBegin, D3DXVECTOR3* vEnd);

	void SetTarget(D3DXVECTOR3* vTarget);

	void FrameMove(DWORD dwDailTime);

	void Render();

public:
	void MoveTo(D3DXVECTOR3* vPos, MPMap* pmap = NULL);

	void BindingBone(D3DXMATRIX* pMatBone);

	void SetFontEffect(char* pszText, CMPFont* pFont);

	void Reset() {
		_CPartCtrl.Reset();
	}

	void Clear() {
		_CPartCtrl.Clear();
	}

	void CopyPartCtrl(CMPPartCtrl* pPart) {
		_CPartCtrl.CopyPartCtrl(pPart);
	}

	void BindingRes(CMPResManger* pResMagr) {
		_CPartCtrl.BindingRes(pResMagr);
	}

public:
	void SetItemDummy(MPSceneItem* pItem, int dummy1, int dummy2) {
		_CPartCtrl.SetItemDummy(pItem, dummy1, dummy2);
	}

	void SetSkillCtrl(SkillCtrl* pCtrl) {
		_CPartCtrl.SetSkillCtrl(pCtrl);
	}

	void setUseZBuff(bool bUseZ) {
		_CPartCtrl.setUseZBuff(bUseZ);
	}

	void setDir(D3DXVECTOR3* pDir) {
		_CPartCtrl.setDir(pDir);
	}

public:
	CMPPartCtrl* GetPartCtrl() {
		return &_CPartCtrl;
	}

public:
	CMPPartCtrl _CPartCtrl;
};


class CMagicCtrl;
inline void Part_trace(CMagicCtrl* pEffCtrl, void* pParam);
inline void Part_drop(CMagicCtrl* pEffCtrl, void* pParam);
inline void Part_fly(CMagicCtrl* pEffCtrl, void* pParam);
inline void Part_fshade(CMagicCtrl* pEffCtrl, void* pParam);
inline void Part_arc(CMagicCtrl* pEffCtrl, void* pParam);
inline void Part_dirlight(CMagicCtrl* pEffCtrl, void* pParam);
inline void Part_dist(CMagicCtrl* pEffCtrl, void* pParam);
inline void Part_dist2(CMagicCtrl* pEffCtrl, void* pParam);

class CMagicCtrl // : public CMPEffectCtrl
{
public:
	CMagicCtrl(void);
	~CMagicCtrl(void);

public:

public:
	bool Create(int iID, CMPResManger* pCResMagr);


	void MoveTo(const D3DXVECTOR3* vPos);

	void FrameMove(DWORD dwDailTime);

	void Render();


	void SetSkillCtrl(SkillCtrl* pCtrl) {
		for (int n = 0; n < _iModelNum; ++n) {
			_CpModel[n]->Scaling(pCtrl->fSize, pCtrl->fSize, pCtrl->fSize);
		}
		for (int n = 0; n < _iPartNum; ++n) {
			_vecPartCtrl[n].SetSkillCtrl(pCtrl);
		}
	}

	void SetTargetID(int iID) {
		_iTargetID = iID;
	}

	int GetTargetID() {
		return _iTargetID;
	}

	void Emission(D3DXVECTOR3* vStart, D3DXVECTOR3* vTarget);
	void CalculateEmission(D3DXVECTOR3* vStart, D3DXVECTOR3* vTarget);

	void Stop() {
		if (_pPartResult) {
			_bStop = true;
			_pPartResult->MoveTo(&_vPos,NULL);
			_pPartResult->setDir(&_vDir);
			_pPartResult->Play(1);
		}
		else
			_bPlay = false;
	}

	bool IsPlaying() {
		return _bPlay;
	}

	bool IsStop() {
		return _bStop;
	}

	void SetInValid() {
		_bPlay = false;
	}

	D3DXVECTOR3* GetPos() {
		return &_vPos;
	}


	void Reset();

	virtual void HitObj() {
	}


	int GetLightID();

	void ResetDir(D3DXVECTOR3* vTarget);

	void SetVel(float fvel) {
		_fVel = fvel;
	}

	void SetTargetDist(float fDist) {
		if (_iRnederIdx == 6 || _iRnederIdx == 7)
			_fStartDist = fDist;
	}

public:
	void (*MagicUpdate)(CMagicCtrl* pMagicCtrl, void* pParam);
	friend void Part_trace(CMagicCtrl* pEffCtrl, void* pParam);
	friend void Part_drop(CMagicCtrl* pEffCtrl, void* pParam);
	friend void Part_fly(CMagicCtrl* pEffCtrl, void* pParam);
	friend void Part_fshade(CMagicCtrl* pEffCtrl, void* pParam);
	friend void Part_arc(CMagicCtrl* pEffCtrl, void* pParam);
	friend void Part_dirlight(CMagicCtrl* pEffCtrl, void* pParam);
	friend void Part_dist(CMagicCtrl* pEffCtrl, void* pParam);
	friend void Part_dist2(CMagicCtrl* pEffCtrl, void* pParam);


	int GetRenderIdx() {
		return _iRnederIdx;
	}

	int GetModelNum() {
		return _iModelNum;
	}

	CMPModelEff* GetModelEff(int id) {
		return _CpModel[id];
	}

	int GetPartNum() {
		return _iPartNum;
	}

	CMPPartCtrl* GetPartCtrl(int id) {
		return &_vecPartCtrl[id];
	}

	bool IsDail() {
		return (_fDailTime > 0 && _fCurEmiTime < _fDailTime);
	}

	void SetDailTime(float fTime) {
		_fDailTime = fTime;
	}

	void SetAlpha(float falpha) {
		int n;
		for (n = 0; n < _iModelNum; n++) {
			_CpModel[n]->SetAlpha(falpha);
		}
		for (n = 0; n < _iPartNum; n++) {
			_vecPartCtrl[n].SetAlpha(falpha);
		}
	}

public:
	int m_iIdx;
	CMPResManger* m_pCResMagr;

protected:
	int _iModelNum;
	std::vector<CMPModelEff*> _CpModel;
	int _iPartNum;
	std::vector<CMPPartCtrl> _vecPartCtrl;
	std::vector<int> _vecDummy;

	CMPPartCtrl* _pPartResult;

	bool _bPlay;
	bool _bStop;

	float _fDailTime;
	float _fCurEmiTime;

	int _iRnederIdx;
	int _iLightID;

	s_string _strResult;

protected:
	int _iTargetID;
	D3DXVECTOR3 _vPos;
	D3DXVECTOR3 _vDir;
	float _fVel;
	D3DXVECTOR3 _vTarget; //
	float _fDirXZ[2]; //XZ

	float _fStartDist;
	float _fCurDist;

	float _fTargDist;
	D3DXVECTOR3 _vTargDir;
	D3DXVECTOR3 _vOldPos;
	D3DXVECTOR3 _vOldTarget;

	float _fDist;

	float _fHalfHei;
	D3DXVECTOR3 _vArcOrg;
	D3DXVECTOR3 _vArcAxis;
	float _fCurArc;

	int _iCurSNum;
	float _fCurTime;
};

#pragma warning(default: 4275)
