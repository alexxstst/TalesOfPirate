#pragma once

#include "i_effect.h"
class MPRender;

inline int Rand(int _imin, int _imax) {
	if (_imin == 0 && _imax == 0)return 0;
	return _imin + (rand() % (_imax - _imin));
}

inline int Rand(int _ix) {
	if (_ix == 0)return 0;
	return rand() % _ix;
}

inline float Randf(float _fx, int count) {
	if (_fx == 0)
		return 0;
	if (count == 0)
		return 0;
	float d = _fx / (float)count;
	return (float)Rand(count) * d;
}

inline float Randf(float _fx) {
	if (_fx == 0)
		return 0;
	if (_fx == 1)
		return (float)(rand() % 1000) / 1000;
	int x = Rand((int)_fx);
	float ft = _fx - (int)_fx;
	if (ft > 0 && ((int)ft) == 0)
		return (float)x + (float)Rand((int)(ft * 1000)) / 1000;

	return (float)x + (float)(rand() % 1000) / 1000;
}

inline float Randf(float _f1, float _f2) {
	if (_f1 == _f2)return _f2;
	float x = Randf(_f2 - _f1);

	return _f1 + x;
}


inline bool PointInstrPointRange(D3DXVECTOR3* vPoint1, D3DXVECTOR3* vPoint2, float fRange) {
	return ((fabs(vPoint1->x - vPoint2->x) < fRange) &&
			(fabs(vPoint1->y - vPoint2->y) < fRange))/* &&
			(fabs(vPoint1->z - vPoint2->z) < fRange))*/;
}

inline bool PointPointRange(D3DXVECTOR3* vPoint1, D3DXVECTOR3* vPoint2, float fRange) {
	return ((fabs(vPoint1->x - vPoint2->x) < fRange) &&
		(fabs(vPoint1->y - vPoint2->y) < fRange) &&
		(fabs(vPoint1->z - vPoint2->z) < fRange));
}

D3DXMATRIX* GetMatrixRotation(D3DXMATRIX* pout, const D3DXVECTOR3* Point, const D3DXVECTOR3* aixs, float angle);
void GetDirRotation(D3DXVECTOR2* pOut, D3DXVECTOR3* pDir);
/************************************************************************/
/*class CMPParticle*/
/************************************************************************/
class CMPParticle // : public CMPModelEff
{
public:
	CMPParticle(void);
	~CMPParticle(void);

public:
	WORD GetCurFrame(float fDailTime, WORD wTotalFrame) {
		m_fCurTime += fDailTime;
		if (m_fCurTime >= m_fFrameTime) {
			m_wCurFrame++;
			if (m_wCurFrame >= wTotalFrame) {
				m_wCurFrame = wTotalFrame;
				m_bLive = false;
			}
			m_fCurTime = 0;
		}
		return m_wCurFrame;
	}

	WORD GetCurFrame(float fDailTime, WORD wTotalFrame, float frametime) {
		m_fCurTime += fDailTime;
		if (m_fCurTime >= frametime) {
			m_wCurFrame++;
			if (m_wCurFrame >= wTotalFrame) {
				m_wCurFrame = wTotalFrame;
				m_bLive = false;
			}
			m_fCurTime = 0;
		}
		return m_wCurFrame;
	}

	float GetLerpValue() {
		return m_fCurTime / m_fFrameTime;
	}

	void Reset(const D3DXVECTOR3& vPos) {
		m_fCurTime = 0;
		m_wCurFrame = 0;
		m_vOldPos = m_vPos = vPos;
		D3DXMatrixIdentity(&m_SCurMat);
		D3DXMatrixIdentity(&m_SBoneMat);

		m_fFrameTime = 0;
		m_vCurAngle = D3DXVECTOR3(0, 0, 0);
		m_fSize = 0;
		m_SCurColor = D3DXCOLOR(0, 0, 0, 0);

		m_fPartTime = 0;
	}

public:
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vOldPos;
	D3DXVECTOR3 m_vVel;
	D3DXVECTOR3 m_vAccel;
	bool m_bLive;
	float m_fLife;

	float m_fCurTime;
	WORD m_wCurFrame;
	D3DXCOLOR m_SCurColor;
	float m_fSize;
	D3DXVECTOR3 m_vCurAngle;
	D3DXMATRIX m_SCurMat;
	float m_fFrameTime;
	D3DXMATRIX m_SBoneMat;

	float m_fPartTime;
};

#define		PARTTICLE_SNOW			1
#define		PARTTICLE_FIRE			2
#define		PARTTICLE_BLAST			3
#define		PARTTICLE_RIPPLE		4
#define		PARTTICLE_MODEL			5
#define		PARTTICLE_STRIP			6
#define		PARTTICLE_WIND			7
#define		PARTTICLE_ARRAW			8
#define		PARTTICLE_ROUND			9
#define		PARTTICLE_BLAST2		10
#define		PARTTICLE_BLAST3		11

#define		PARTTICLE_SHRINK		12

#define		PARTTICLE_SHADE			13

#define		PARTTICLE_RANGE			14
#define		PARTTICLE_RANGE2		15

#define		PARTTICLE_DUMMY			16
#define		PARTTICLE_LINE_SINGLE	17
#define		PARTTICLE_LINE_ROUND	18

struct SkillCtrl {
	SkillCtrl() {
		fDelayTime = -1;
		fPlayTime = -1;
		iParNum = -1;
		fRange[0] = fRange[0] = fRange[0] = -1;
		fSize = -1;
	}

	float fDelayTime; //
	float fPlayTime; //
	int iParNum; //
	float fRange[3]; //!
	float fSize; //
};


/************************************************************************/
/* class CMPPartSys*/ //PARTTICLE_BLAST2
/************************************************************************/

#define	 MUD_PART

class CMPModelEff;
class CMPFont;
class CEffectFont;
class CEffPath;

class CMPPartSys;
void CopyPartSys(CMPPartSys* part1, CMPPartSys* part2);

bool _CreateSnow(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveSnow(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateBlast(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveBlast(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateRipple(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveRipple(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateModel(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveModel(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateStrip(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveStrip(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateWind(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveWind(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateFire(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveFire(CMPPartSys* pPart, DWORD dwDailTime);

//
bool _CreateArraw(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveArraw(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateRound(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveRound(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateBlast2(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveBlast2(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateBlast3(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveBlast3(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateShrink(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveShrink(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateShade(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveShade(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateRange(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveRange(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateRange2(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveRange2(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateDummy(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveDummy(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateLineSingle(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveLineSingle(CMPPartSys* pPart, DWORD dwDailTime);

bool _CreateLineRound(CMPPartSys* pPart, CMPParticle* pCtrl);
void _FrameMoveLineRound(CMPPartSys* pPart, DWORD dwDailTime);

class CMPPartSys //: public CEffectBase
{
public:
	CMPPartSys();
	~CMPPartSys();

	bool Create(int iType, const s_string& strPartName, int iNumPart,
				const s_string& strModelName, const s_string& strTexName,
				D3DXVECTOR3 vRange, WORD wFrameCount, bool bBillBoard,
				CMPResManger* pCResMagr);

	void InitParam();

	void BindingRes(CMPResManger* pCResMagr);

	void FrameMove(DWORD dwDailTime);

	void Render();
	void RenderVS();
	void RenderSoft();

public:
	//edit////////////////////////////////////////////////////////////////////////
	void SetOpertion();
	void SetSkillCtrl(SkillCtrl* pCtrl);

	int GetType() {
		return _iType;
	}

	void SetType(int iType, CMPResManger* pCResMagr);

	s_string GetSysName() {
		return _strPartName;
	}

	void SetSysName(const s_string& strPartName) {
		_strPartName = strPartName;
	}

	float GetSysLife() {
		return _fLife;
	}

	void SetSysLife(float fLife) {
		_fLife = fLife;
	}

	float GetSysVel() {
		return _fVecl;
	}

	void SetSysVel(float fVecl) {
		_fVecl = fVecl;
	}

	int GetSysNum() {
		return _iParNum;
	}

	void SetSysNum(int iParNum);

	float GetSysStep() {
		return _fStep;
	}

	void SetSysStep(float fstep) {
		_fStep = fstep;
	}

	D3DXVECTOR3 GetSysDir() {
		return _vDir;
	}

	void SetSysDirX(float fx) {
		_vDir.x = fx;
	}

	void SetSysDirY(float fy) {
		_vDir.y = fy;
	}

	void SetSysDirZ(float fz) {
		_vDir.z = fz;
	}

	D3DXVECTOR3 GetSysAccel() {
		return _vAccel;
	}

	void SetSysAccelX(float fx) {
		_vAccel.x = fx;
	}

	void SetSysAccelY(float fy) {
		_vAccel.y = fy;
	}

	void SetSysAccelZ(float fz) {
		_vAccel.z = fz;
	}

	float GetSysRangeX() {
		return _fRange[0];
	}

	float GetSysRangeY() {
		return _fRange[1];
	}

	float GetSysRangeZ() {
		return _fRange[2];
	}

	void SetSysRangeX(float fx) {
		_fRange[0] = fx;
	}

	void SetSysRangeY(float fy) {
		_fRange[1] = fy;
	}

	void SetSysRangeZ(float fz) {
		_fRange[2] = fz;
	}

	WORD GetFrameCount() {
		return _wFrameCount;
	}

	void SetFrameCount(WORD wFrameCount);

	float GetFrameSize(int iFrame) {
		if (iFrame < _wFrameCount)
			return *_vecFrameSize[iFrame];
		return 0;
	}

	void SetFrameSize(int iFrame, float fsize, CMPResManger* pCResMagr);

	D3DXVECTOR3 GetFrameAngle(int iFrame) {
		//if(iFrame <_wFrameCount)
		return *_vecFrameAngle[iFrame];
		//return D3DXVECTOR3(0,0,0);
	}

	void SetFrameAngle(int iFrame, D3DXVECTOR3 vAngle) {
		if (iFrame < _wFrameCount)
			*_vecFrameAngle[iFrame] = vAngle;
	}

	D3DXCOLOR GetFrameColor(int iFrame) {
		//if(iFrame <_wFrameCount)
		return *_vecFrameColor[iFrame];
		//return 0x00000000;
	}

	void SetFrameColor(int iFrame, DWORD dwColor) {
		if (iFrame < _wFrameCount)
			*_vecFrameColor[iFrame] = dwColor;

		//if(m_bShade)
		//	m_cShade.setColor(*_vecFrameColor[0]);
	}

	void SetAlpha(float falpha);

	bool IsBillBoard() {
		return _bBillBoard;
	}

	void SetBillBoard(bool bBBoard) {
		_bBillBoard = bBBoard;
	}

	s_string GetTextureName() {
		return _strTexName;
	}

	void SetTextureName(const s_string& strTexName, CMPResManger* pCResMagr);

	s_string GetModelName() {
		return _strModelName;
	}

	void SetModelName(const s_string& strModelName, CMPResManger* pCResMagr);

	//s_string			GetVirualModelName(){return _strVirualModel;}
	void SetEmissionPath(CEffPath* pcPath);


	D3DBLEND GetSrcBlend() {
		return _eSrcBlend;
	}

	void SetSrcBlend(D3DBLEND srcblend) {
		_eSrcBlend = srcblend;
		if (m_bShade)
			m_cShade.SetAlphaType(_eSrcBlend, _eDestBlend);
	}

	D3DBLEND GetDestBlend() {
		return _eDestBlend;
	}

	void SetDestBlend(D3DBLEND destblend) {
		_eDestBlend = destblend;
		if (m_bShade)
			m_cShade.SetAlphaType(_eSrcBlend, _eDestBlend);
	}

	D3DTEXTUREFILTERTYPE GetMagFilter() {
		return _eMagFilter;
	}

	void SetMagFilter(D3DTEXTUREFILTERTYPE MagFilter) {
		_eMagFilter = MagFilter;
	}

	D3DTEXTUREFILTERTYPE GetMinFilter() {
		return _eMinFilter;
	}

	void SetMinFilter(D3DTEXTUREFILTERTYPE MinFilter) {
		_eMinFilter = MinFilter;
	}

	void SetPosOffset(float fx, float fy, float fz) {
		_vOffset.x = fx;
		_vOffset.y = fy;
		_vOffset.z = fz;
	}

	D3DXVECTOR3& GetPosOffset() {
		return _vOffset;
	}

public:
	void Reset(bool bLife);

	void Play(int iTime);

	void Stop();

	void End() {
		_bStop = true;
	}

	bool IsPlaying();

	void MoveTo(const D3DXVECTOR3* vPos, MPMap* pmap = nullptr);

	void BindingBone(D3DXMATRIX* pMatBone);

	void setYaw(float fYaw);
	void setPitch(float fPitch);
	void setRoll(float fRoll);

	void setScale(float fx, float fy, float fz);

	void setDir(float fx, float fy, float fz);

	void setFontEffect(CMPFont* pFont);
	void setFontEffText(const char* pszText);
	void setFontEffectCom(VEC_string& vecText, int num,
						  CMPResManger* pCResMagr, D3DXVECTOR3* pvDir, int iTexID,
						  D3DXCOLOR dwColor = 0xffffffff, bool bUseBack = false, bool bmain = false);
	void unFontEffCom();

	void setRenderIdx(int idx) {
		_iRenderIdx = idx;
	}

	void setUseZBuff(bool bUseZ);
	//void setTarget(D3DXVECTOR3* vTarget);

	bool IsDelay() {
		return (_fDelayTime > 0 && _fCurPlayTime < _fDelayTime);
	}

	bool UpdateDelay();
	bool IsCreatePart();

	void SetDelayTime(float ftime) {
		_fDelayTime = ftime;
	}

	void SetPlayTime(float ftime) {
		_fPlayTime = ftime;
	}

	float GetDelayTime() {
		return _fDelayTime;
	}

	float GetPlayTime() {
		return _fPlayTime;
	}

	void SetLoop(bool bLoop);

	void LoadPathFile(char* pszfile);
	void LoadPathFileLet(char* pszfile);
	void SetPathVel(float fvel);
	void DeletePath();
	float GetPathVel();

	s_string& GetHitEff() {
		return m_strHitEff;
	}

	void SetHitEff(const s_string& streff) {
		m_strHitEff = streff;
	}

	bool GetDummyPosList() {
		if (!_pItem || _iDummy1 == -1 || _iDummy2 == -1) {
			return false;
		}
		D3DMATRIX matDummy1, matDummy2;
		_pItem->GetObjDummyRunTimeMatrix((lwMatrix44*)&matDummy1, _iDummy1);
		_pItem->GetObjDummyRunTimeMatrix((lwMatrix44*)&matDummy2, _iDummy2);
		//_bModelRange  = true;

		D3DXVECTOR3* pv1 = (D3DXVECTOR3*)&matDummy1._41;
		D3DXVECTOR3* pv2 = (D3DXVECTOR3*)&matDummy2._41;
		_vDummyDir = *pv1 - *pv2;
		_fDummyDist = D3DXVec3Length(&_vDummyDir);
		D3DXVec3Normalize(&_vDummyDir, &_vDummyDir);
		_vDummyPos = *pv2;
		return true;
	}

	void SetItemDummy(MPSceneItem* pItem, int idummy1, int idummy2) {
		if (_iType != PARTTICLE_DUMMY && _iType != PARTTICLE_LINE_SINGLE)
			return;
		if (!pItem || idummy1 < 0 || idummy2 < 0)
			return;
		_pItem = pItem;

		_iDummy1 = idummy1;
		_iDummy2 = idummy2;
	}

	int GetRoadom() {
		return _iRoadom;
	}

	void SetRoadom(int iRoadom) {
		_iRoadom = iRoadom;
	}

	bool IsModelDir() {
		return _bModelDir;
	}

	void SetModelDir(bool bDir) {
		_bModelDir = bDir;
	}

	void SetMediayEff(bool bMediay);

	bool IsMediay() {
		return _bMediay;
	}

public:
	//!
	bool SaveToFile(FILE* t_pFile);
	//!
	bool LoadFromFile(FILE* t_pFile, DWORD dwVersion);

	bool LoadFromMemory(CMemoryBuf* pbuf, DWORD dwVersion);
	bool IsPartArray();

	void GetRes(int& idtex, int& idmodel, int& ideff);

protected:
	friend void CopyPartSys(CMPPartSys* part1, CMPPartSys* part2);

	void (*FrameUpdate)(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateSnow(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveSnow(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateBlast(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveBlast(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateRipple(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveRipple(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateModel(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveModel(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateStrip(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveStrip(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateWind(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveWind(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateFire(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveFire(CMPPartSys* pPart, DWORD dwDailTime);

	//
	friend bool _CreateArraw(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveArraw(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateRound(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveRound(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateBlast2(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveBlast2(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateBlast3(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveBlast3(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateShrink(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveShrink(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateShade(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveShade(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateRange(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveRange(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateRange2(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveRange2(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateDummy(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveDummy(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateLineSingle(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveLineSingle(CMPPartSys* pPart, DWORD dwDailTime);

	friend bool _CreateLineRound(CMPPartSys* pPart, CMPParticle* pCtrl);
	friend void _FrameMoveLineRound(CMPPartSys* pPart, DWORD dwDailTime);

protected:
	bool _bPlay;
	bool _bStop;
	bool _bLoop;
	WORD _wDeath;
	bool _bUseBone;
	D3DXMATRIX _SBoneMat;

	float _fLife;
	D3DXVECTOR3 _vDir;
	float _fVecl;
	D3DXVECTOR3 _vAccel;
	//float								_fInfXY;//XY

	float _fStep; //
	float _fCurTime;

	float _fDelayTime; //
	float _fPlayTime; //
	float _fCurPlayTime;

	D3DXVECTOR3 _vScale;

	// bool _bFontEff; — удалено вместе с _pFont (использовалось только
	// в связке setFontEffect и в закомментированном коде отрисовки).
	// CMPFont* _pFont; — удалено. Шрифт не хранится в поле: использование
	// было закомментировано, а правильный способ — резолвить через
	// FontManager::Instance().Get(FontSlot::...) в клиенте на момент отрисовки.
	// См. memory/font-api-refactor-todo.md.
	s_string _strText;

	bool _bFontCom;
	CEffectFont* _pcEffFont;
	VEC_string _vecText;
	D3DXVECTOR3 _vFontDir;

	bool _bUseZ;

protected:
	int _iType;

	s_string _strPartName;

	int _iParNum;
	S_BVECTOR<CMPParticle> _vecParticle;


	s_string _strTexName;
	IDirect3DTextureX* _lpCurTex;
	lwITex* _pTex;
	//lwITex*									_oldtex;


	s_string _strModelName;
	CEffectModel* _pCModel;
	CMPModelEff* _CPPart; //CMPModelEff

	bool _bMediay;

	int _idt;
	//#ifdef MUD_PART
	//	S_BVECTOR<CMPModelEff>					_vecPart;
	//#endif

	D3DXVECTOR3 _vPos;

	D3DXVECTOR3 _vOffset;


	bool _bModelRange; //
	s_string _strVirualModel; //
	std::vector<D3DXVECTOR3> _vecPointRange; //
	WORD _wVecNum; //

	float _fRange[3]; //!

	WORD _wFrameCount;
	S_BVECTOR<float> _vecFrameSize;
	S_BVECTOR<D3DXVECTOR3> _vecFrameAngle;
	S_BVECTOR<D3DXCOLOR> _vecFrameColor;

	S_BVECTOR<D3DXVECTOR3> _vecBone; //

	DWORD* _pdwVShader;
	float* _pfDailTime;
	D3DXMATRIX* _pMatViewProj;

	bool _bBillBoard;
	D3DXMATRIX* _SpmatBBoard;

	int _iRenderIdx;
	CMPEffectFile* _pCEffectFile;

	D3DBLEND _eSrcBlend;
	D3DBLEND _eDestBlend;

	D3DTEXTUREFILTERTYPE _eMinFilter;
	D3DTEXTUREFILTERTYPE _eMagFilter;

	//
	CEffPath* _pcPath;
	D3DXVECTOR3 _vSavePos;

	//shade
	bool m_bShade;
	CMPShadeCtrl m_cShade;
	MPMap* m_pMap;

	CMPResManger* m_pCResMagr;
	s_string m_strHitEff;

	//dummy
	int _iDummy1, _iDummy2;
	MPSceneItem* _pItem;
	D3DXVECTOR3 _vDummyPos, _vDummyDir;
	float _fDummyDist;

	int _iRoadom;
	bool _bModelDir;
	D3DXVECTOR2 _vTemDir;
};

inline void RotatingXZ(D3DXMATRIX* pmat, float fAngleX, float fAngleZ) {
	D3DXMATRIX mat;
	D3DXMatrixRotationX(&mat, fAngleX);
	D3DXMatrixRotationZ(pmat, fAngleZ);
	D3DXMatrixMultiply(pmat, &mat, pmat);
}


///************************************************************************/
///* class CMPParticleMagr*/
///************************************************************************/
//
//#define	 PARTTYPE_RAIN		1
//#define	 PARTTYPE_BLAST		2
//#define	 PARTTYPE_LINK		3
//#define	 PARTTYPE_STRIP		4
//
//
//class CMPMagrPart: public CEffectBase
//{
//public:
//	CMPMagrPart(void){}
//	~CMPMagrPart(void){}
//
//public:
//	bool				Create(int iType, s_string	strPartName,int iNumPart,s_string strModelName,
//									D3DXVECTOR2 vRange, WORD wFrameCount, CMPResManger	*pCResMagr);
//	//
//	void				InitParam(D3DXVECTOR3 vDir,D3DXVECTOR3	vVecl,
//									D3DXVECTOR3 vAccel, int iLife, int iInfXY  =0);
//
//
//	void				MoveTo(D3DXVECTOR3 vPos);
//
//	void				FrameMove(DWORD	dwDailTime);
//
//	void				Render();
//	void				RenderVS();
//	void				RenderSoft();
//
//
//	//void				play(int Time);
//	void				Stop()
//	{
//		_bPlay = false;
//	}
//	
//	bool				IsPlaying()
//	{
//		return _bPlay;
//	}
//
//	void				Emission(int iID, D3DXVECTOR3& vTarget);
//	void				SetTarget(D3DXVECTOR3& vTarget);
//	////////////////////////////////////////////////////////
//	int					GetType()			{ return _iType;}
//
//	///////////////////////////////////////////////////////
//	void				setFrameSizeSame(float	fsize);
//	void				setFrameSizeBig(float	fsize);
//	void				setFrameSizeSmall(float	fsize);
//
//public:
//	//
//	void				(*FrameUpdate)(CMPMagrPart* pPart,DWORD	dwDailTime);
//
//
//	friend bool	_CreateRain(CMPMagrPart* pPart, CMPParticle* pCtrl);
//	friend void	_FrameMoveRain(CMPMagrPart* pPart,DWORD	dwDailTime);
//
//	friend bool	_CreateBlast(CMPMagrPart* pPart,CMPParticle* pCtrl,CMPModelEff* pEff);
//	friend void	_FrameMoveBlast(CMPMagrPart* pPart,DWORD	dwDailTime);
//
//	friend bool	_CreateLink(CMPMagrPart* pPart,CMPParticle* pCtrl,CMPModelEff* pEff);
//	friend void	_FrameMoveLink(CMPMagrPart* pPart,DWORD	dwDailTime);
//
//	friend bool	_CreateStrip(CMPMagrPart* pPart,CMPParticle* pCtrl,CMPModelEff* pEff);
//	friend void	_FrameMoveStrip(CMPMagrPart* pPart,DWORD	dwDailTime);
//
//protected:
//	s_string								_strPartName;
//	s_string								_strModelName;
//
//
//protected:
//	S_BVECTOR<CMPModelEff>				_vecParticle;
//	S_BVECTOR<CMPParticle>				_vecCtrl;
//
//
//	S_BVECTOR<float>					_vecFrameSize;
//	S_BVECTOR<D3DXVECTOR3>				_vecFrameAngle;
//
//	int									_iType;
//	bool								_bPlay;
//	bool								_bLoop;
//
//	int									_iParNum;
//	WORD								_wFrameCount;
//
//	D3DXVECTOR3							_vPos;
//	D3DXVECTOR3							_vOldPos;
//
//	D3DXVECTOR2							_vRange;	//!
//
//
//	float*								_pfDailTime;
///////////////////////////////////////////////////
//	int									_iLife;
//	D3DXVECTOR3							_vDir;
//	D3DXVECTOR3							_vVecl;
//	D3DXVECTOR3							_vAccel;
//	int									_iInfXY;//XY
//
//	WORD								_wDeath;//
//
//	D3DXVECTOR3							_vTarget;//
//	float								_fDirXZ[2];//XZ
//};
//
//////////////////////
//bool	_CreateRain(CMPMagrPart* pPart,CMPParticle* pCtrl);
//void	_FrameMoveRain(CMPMagrPart* pPart,DWORD	dwDailTime);
//
//
//bool	_CreateBlast(CMPMagrPart* pPart,CMPParticle* pCtrl,CMPModelEff* pEff);
//void	_FrameMoveBlast(CMPMagrPart* pPart,DWORD	dwDailTime);
//
//
//bool	_CreateLink(CMPMagrPart* pPart,CMPParticle* pCtrl,CMPModelEff* pEff);
//void	_FrameMoveLink(CMPMagrPart* pPart,DWORD	dwDailTime);
//
//bool	_CreateStrip(CMPMagrPart* pPart,CMPParticle* pCtrl,CMPModelEff* pEff);
//void	_FrameMoveStrip(CMPMagrPart* pPart,DWORD	dwDailTime);
