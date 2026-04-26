#pragma once

class MPRender;

class MPMap;
class CMPResManger;

#define		SHADE_SINGLE		1
#define		SHADE_ANI			2

#define  MAXRADIUS	9.0f;//!,7vs,7soft
#define	 MAXTILE	5 //!

class CMPShadeMap // : public CEffectBase
{
public:
	CMPShadeMap(void);
	~CMPShadeMap(void);

public:
	void setTextureName(s_string& strName);
	s_string getTextureName() {
		return _strTexName;
	}

	void BoundingRes(CMPResManger* m_CResMagr);
	//!4
	bool CreateShadeMap(float fRadius = 0.8f);

	bool SetGridNum(int iNum);

	virtual void FrameMove(DWORD dwDailTime);
	virtual void Render();

	virtual void RenderVS();
	//!VS
	void RenderSoft();
	virtual void MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float fAngle = 0);

	bool IsShow() {
		return _bShow;
	}

	void Show(bool bShow) {
		_bShow = bShow;
	}

	void setFrameTexture(s_string& strTexName, CMPResManger* pCResMagr);

	virtual void setFrameTime(float fTime) {
	}

	virtual void setTexFrameTime(float fTime) {
	}

	virtual void setFrameColor(int iIdx, D3DCOLOR SColor) {
	}

	virtual void SetColor(D3DXCOLOR dwColor) {
		_dwColor = dwColor;
	}

	virtual float getFrameTime() {
		return 0;
	}

	virtual float getTexFrameTime() {
		return 0;
	}

	virtual void getFrameColor(int iIdx, D3DCOLOR* pSColor) {
	}

	virtual void getColor(D3DXCOLOR* pSColor) {
		*pSColor = _dwColor;
	}

	void SetAlphaType(D3DBLEND eSrcBlend, D3DBLEND eDestBlend) {
		_eSrcBlend = eSrcBlend;
		_eDestBlend = eDestBlend;
	}

	void UseSoft(bool bUseSoft) {
		m_bUseSoft = bUseSoft;
	}

	void SetRenderIndex(int idx) {
		_iIdxTech = idx;
	}

	void SetUpSea(bool bUp) {
		_UpSea = bUp;
	}

	void SetUpdate() {
		_bUpdate = true;
	}

	bool SaveToFile(FILE* pFile);
	bool LoadFromFile(FILE* pFile);

	void FillVertex();

public:
	int m_iType;
	bool m_bUseSoft;
	DWORD m_dwVsConst;

protected:
	bool _bShow;
	CEffectModel* _pModel;

	D3DXVECTOR3 _SVerPos;
	int _iVerNum;
	int _iIndexNum;
	int _iFaceCount;

	D3DXVECTOR3 _SShadePos[MAX_SHADER_VERNUM];
	D3DXVECTOR2 _SShadeUV[MAX_SHADER_VERNUM];
	WORD _wIndex[MAX_SHADER_IDXNUM];

	D3DXCOLOR _dwColor;


	s_string _strTexName;
	lwITex* _lpCurTex;

	float _fRadius;
	float _fGridRadius;
	int _iGridCrossNum;
	int _iGridNum;
	float _fGridMax;

	float* _pfDailTime;
	int _iIdxTech;
	CMPEffectFile* _pCEffectFile;

	D3DXMATRIX* _pMatViewProj;

	D3DBLEND _eSrcBlend;
	D3DBLEND _eDestBlend;

	bool _bUpdate;
	bool _UpSea;
};

class CMPShadeEX : public CMPShadeMap {
public:
	CMPShadeEX(int iFrameCount = 4);
	~CMPShadeEX(void);

public:
	bool CreateSpliteTexture(int iRow, int iColnum);

	bool IsPlaying() {
		return _bShow;
	}

	void Play(int iTime = 0) /*iTime: 0 == loop, */
	{
		//	return;
		Reset();
		_bShow = true;
		if (iTime == 0)
			_bLoop = true;
		else
			_bLoop = false;
	}

	void Stop() {
		_bShow = false;
	}

	void Reset() {
		_iCurFrame = 0;
		_fCurTime = 0;
		_fTexCurTime = 0;
		_iCurTex = 0;
	}

	void setFrameCount(int iCount);
	void setFrameTime(float fTime);
	void setTexFrameTime(float fTime);
	void setFrameColor(int iIdx, D3DCOLOR SColor);
	void setColor(D3DXCOLOR SColor);

	int getFrameCount() {
		return _iFrameCount;
	}

	float getFrameTime() {
		return _fFrameTime;
	}

	float getTexFrameTime() {
		return _fTexFrameTime;
	}

	void getFrameColor(int iIdx, D3DCOLOR* pSColor) {
		*pSColor = _vecFrameColor[iIdx];
	}

	void getColor(D3DXCOLOR* pSColor) {
		*pSColor = _dwColor;
	}

	void FrameMove(DWORD dwDailTime);
	void MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float fAngle = 0);

	bool SaveToFile(FILE* pFile);
	bool LoadFromFile(FILE* pFile);

protected:
	bool _bLoop;
	int _iFrameCount;
	int _iCurFrame;
	float _fFrameTime;
	std::vector<D3DXCOLOR> _vecFrameColor;
	float _fCurTime;
	float _fLerp;

	float _fTexFrameTime;
	float _fTexCurTime;
	int _iNumTex;
	int _iCurTex;
	std::vector<D3DXVECTOR2> _vecTexOffset;
	std::vector<D3DXVECTOR2> _vecTexSave;
	int _iRow;
	int _iCol;
};

class CMPShadeCtrl {
public:
	CMPShadeCtrl(void);
	~CMPShadeCtrl(void);

public:
	bool Create(s_string& strTexName,
				CMPResManger* pCResMagr,
				float fSize = 1.0f,
				bool bAni = false,
				int iRow = 1, int iColnum = 1);

	bool IsPlaying() {
		if (_pShadeMap->m_iType == SHADE_SINGLE)
			return _pShadeMap->IsShow();
		else
			return ((CMPShadeEX*)_pShadeMap)->IsPlaying();
	}

	void Play(int iTime = 0) /*iTime: 0 == loop, */
	{
		if (_pShadeMap->m_iType == SHADE_SINGLE)
			_pShadeMap->Show(true);
		else
			((CMPShadeEX*)_pShadeMap)->Play(iTime);
	}

	void Reset() {
		if (_pShadeMap->m_iType == SHADE_ANI)
			((CMPShadeEX*)_pShadeMap)->Reset();
	}

	void setFrameCount(int iCount);
	void setFrameTime(float fTime);
	void setTexFrameTime(float fTime);
	void setFrameColor(int iIdx, D3DCOLOR SColor);
	void setColor(D3DXCOLOR SColor);

	int getFrameCount();
	float getFrameTime();
	float getTexFrameTime();
	void getFrameColor(int iIdx, D3DCOLOR* pSColor);
	void getColor(D3DXCOLOR* pSColor);

	void setFrameTexture(s_string& strTexName, CMPResManger* pCResMagr);
	void SetAlphaType(D3DBLEND eSrcBlend, D3DBLEND eDestBlend);

	void SetRenderIndex(int idx) {
		_pShadeMap->SetRenderIndex(idx);
	}

	void SetUpdate() {
		_pShadeMap->SetUpdate();
	}

	void MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float fAngle = 0);

	void FrameMove(DWORD dwDailTime);

	void Render();

	CMPShadeMap* GetShadeMap() {
		return _pShadeMap;
	}

	bool SaveToFile(char* pchName);
	bool LoadFromFile(char* pchName);

protected:
	CMPShadeMap* _pShadeMap;
};
