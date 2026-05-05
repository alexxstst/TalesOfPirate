#include "StdAfx.h"

#include "GlobalInc.h"
#include "MPModelEff.h"

#include ".\mpparticlesys.h"

#include "MPMap.h"

D3DXMATRIX* GetMatrixRotation(D3DXMATRIX* pout, const D3DXVECTOR3* Point, const D3DXVECTOR3* aixs, float angle) {
	D3DXMATRIX r, r2;
	D3DXMATRIX r1 = D3DXMATRIX(1, 0, 0, 0,
							   0, 1, 0, 0,
							   0, 0, 1, 0,
							   -Point->x, -Point->y, -Point->z, 1);
	D3DXMatrixRotationAxis(&r2, aixs, angle);
	r = r1 * r2;
	r1 = D3DXMATRIX(1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					Point->x, Point->y, Point->z, 1);
	r = r * r1;
	*pout = r;
	return pout;
}

void GetDirRotation(D3DXVECTOR2* pOut, D3DXVECTOR3* pDir) {
	float fDist = D3DXVec3Length(pDir);
	if (pDir->z == 0) {
		pOut->x = 0;
	}
	else {
		const auto v = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pOut->x = asinf(D3DXVec3Dot(pDir, &v) / fDist);
	}
	if (pDir->x == 0 && pDir->y == 0) {
		pOut->y = 0;
	}
	else {
		const D3DXVECTOR3 v[] = {D3DXVECTOR3(pDir->x, pDir->y, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)};
		fDist = D3DXVec3Length(&v[0]);
		pOut->y = acosf(D3DXVec3Dot(&v[0], &v[1]) / fDist);
		if (pDir->x >= 0.0f) {
			pOut->y = -pOut->y;
		}
	}
}

#include "mpfont.h"
#include "MPRender.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
CMPParticle::CMPParticle(void) {
	m_vPos = D3DXVECTOR3(0, 0, 0);
	m_vOldPos = D3DXVECTOR3(0, 0, 0);
	m_vVel = D3DXVECTOR3(0, 0, 0);
	m_vAccel = D3DXVECTOR3(0, 0, 0);
	m_bLive = false;
	m_fLife = 0;
	m_fCurTime = 0;
	m_wCurFrame = 0;
	m_fFrameTime = 0;

	m_SCurColor = 0xffffffff;
	m_fSize = 1.0f;
	m_vCurAngle = D3DXVECTOR3(0, 0, 0);

	m_fPartTime = 0;
}

CMPParticle::~CMPParticle(void) {
}

CMPPartSys::CMPPartSys() {
	_bPlay = false;
	_bStop = true;
	_bLoop = true;
	_wDeath = _iParNum;
	_bUseBone = false;
	D3DXMatrixIdentity(&_SBoneMat);
	_vScale = D3DXVECTOR3(1, 1, 1);

	_fDelayTime = 0;
	_fPlayTime = 0;
	_fCurPlayTime = 0;

	//_vecParticle.resize(100);//100


	_pcPath = NULL;

	_pCModel = NULL;
	_CPPart = NULL;
	_bUseBone = false;

	_bModelRange = false; //
	_strVirualModel = ""; //
	_vecPointRange.clear(); //
	_wVecNum = 0; //

	_vOffset = D3DXVECTOR3(0, 0, 0);

	// Шрифт более не хранится в CMPPartSys — см. font-api-refactor-todo.md.

	_bFontCom = false;
	_pcEffFont = NULL;
	_vecText.clear();
	_vFontDir = D3DXVECTOR3(0, 1, 0);

	_iRenderIdx = 3;
	_bUseZ = true;
	m_bShade = false;

	m_pMap = NULL;
	m_pCResMagr = NULL;

	_pTex = NULL;
	_iDummy1 = -1;
	_iDummy2 = -1;
	_pItem = NULL;

	_iRoadom = 2;
	_bModelDir = false;
	_vTemDir = D3DXVECTOR2(0, 0);

	_bMediay = false;
}

CMPPartSys::~CMPPartSys() {
	SAFE_DELETE(_pcPath);
	{
		SAFE_DELETE_ARRAY(_CPPart);
	}

	SAFE_DELETE_ARRAY(_pcEffFont);
}

//-----------------------------------------------------------------------------
void CMPPartSys::setFontEffectCom(VEC_string& vecText, int num,
								  CMPResManger* pCResMagr,
								  D3DXVECTOR3* pvDir,
								  int iTexID,
								  D3DXCOLOR dwColor, bool bUseBack, bool bmain) {
	if (num <= 0) {
		_bFontCom = false;
		SAFE_DELETE_ARRAY(_pcEffFont);
		_vecText.clear();
		return;
	}
	_bFontCom = true;
	SAFE_DELETE_ARRAY(_pcEffFont);
	_pcEffFont = new CEffectFont[num];

	_vecText.clear();
	for (int n = 0; n < num; n++) {
		_pcEffFont[n].CreateEffectFont(pCResMagr->m_pDev, pCResMagr, iTexID, dwColor, bUseBack, bmain);
		_pcEffFont[n].SetRenderText((CHAR*)(LPCSTR)vecText[n].c_str());
		_vecText.push_back(vecText[n]);
	}
	_vFontDir = *pvDir;
	_vFontDir.z = 0;
	SetSysNum(num);
}

//-----------------------------------------------------------------------------
void CMPPartSys::unFontEffCom() {
	_bFontCom = false;
	SAFE_DELETE_ARRAY(_pcEffFont);
	_vecText.clear();
}

//-----------------------------------------------------------------------------
void CMPPartSys::setUseZBuff(bool bUseZ) {
	_bUseZ = bUseZ;

	if (_CPPart) {
		if (_bMediay) {
			for (int t = 0; t < _iParNum; t++) {
				_CPPart[t].UseZBuffer(bUseZ);
			}
		}
		else
			_CPPart->UseZBuffer(bUseZ);
	}
}

//-----------------------------------------------------------------------------
bool CMPPartSys::UpdateDelay() {
	if (_fPlayTime <= 0) {
		if (_fDelayTime > 0) {
			if (_fCurPlayTime >= _fDelayTime) {
				return true;
			}
			_fCurPlayTime += *_pfDailTime;
			return false;
		}
		return true;
	}
	else {
		if (_fDelayTime <= 0) {
			if (_fCurPlayTime >= _fPlayTime) {
				Stop();
				return _bPlay;
			}
			_fCurPlayTime += *_pfDailTime;
			return true;
		}
		else {
			if (_fCurPlayTime >= _fDelayTime) {
				if (!IsCreatePart()) {
					switch (_iType) {
					case PARTTICLE_BLAST:
						_CreateBlast(this,NULL);
						break;
					case PARTTICLE_BLAST2:
						_CreateBlast2(this,NULL);
						break;
					case PARTTICLE_BLAST3:
						_CreateBlast3(this,NULL);
						break;
					}
				}
				if (_fCurPlayTime >= _fPlayTime) {
					Stop();
					return _bPlay;
				}
				_fCurPlayTime += *_pfDailTime;
				return true;
			}
			_fCurPlayTime += *_pfDailTime;
			return false;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
bool CMPPartSys::IsCreatePart() {
	switch (_iType) {
	case PARTTICLE_BLAST:
	case PARTTICLE_BLAST2:
	case PARTTICLE_BLAST3:
		if (_vecParticle[0]->m_bLive)
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool CMPPartSys::IsPartArray() {
	if (_iType == PARTTICLE_MODEL || _iType == PARTTICLE_STRIP || _iType == PARTTICLE_ARRAW)
		return false;
	return true;
}

bool CMPPartSys::Create(int iType, const s_string& strPartName, int iNumPart,
						const s_string& strModelName, const s_string& strTexName,
						D3DXVECTOR3 vRange, WORD wFrameCount, bool bBillBoard,
						CMPResManger* pCResMagr) {
	_iType = iType;

	_strPartName = strPartName;

	_iParNum = iNumPart;
	_vecParticle.setsize(_iParNum);

	_strTexName = strTexName;
	_lpCurTex = NULL;
	int id = pCResMagr->GetTextureID(_strTexName);
	if (id < 0) {
		MessageBox(NULL, _strTexName.c_str(), "ERROR", 0);
		return false;
	}

	_pTex = pCResMagr->GetTextureByIDlw(id);
	if (_pTex) {
		_lpCurTex = _pTex->GetTex(); //pCResMagr->GetTextureByID(id);
	}

	{
		SAFE_DELETE_ARRAY(_CPPart);
	}

	_strModelName = strModelName;
	_pCModel = NULL;
	id = pCResMagr->GetMeshID(_strModelName);
	if (id < 0) {
		id = pCResMagr->GetEffectID(_strModelName);
		if (id < 0) {
			MessageBox(NULL, _strModelName.c_str(), "ERROR", 0);
			return false;
		}
		else {
			_CPPart = new CMPModelEff[1];
			_CPPart->BindingEffect(pCResMagr->GetEffectByID(id));
			_CPPart->BindingRes(pCResMagr);
		}
	}
	else
		_pCModel = pCResMagr->GetMeshByID(id);

	_fRange[0] = vRange.x; //!
	_fRange[1] = vRange.y; //!
	_fRange[2] = vRange.z; //!
	_vPos = D3DXVECTOR3(-_fRange[0] / 2, -_fRange[1] / 2, -_fRange[2] / 2);

	_wFrameCount = wFrameCount;

	_pdwVShader = 0L;
	_pfDailTime = pCResMagr->GetDailTime();
	_pMatViewProj = pCResMagr->GetViewProjMat();

	if (_pCModel) {
		_bBillBoard = bBillBoard;
		_SpmatBBoard = pCResMagr->GetBBoardMat();
	}
	else {
		_bBillBoard = false;
		_SpmatBBoard = NULL;
	}

	_pCEffectFile = pCResMagr->GetEffectFile();

	SetOpertion();

	return true;
}

//-----------------------------------------------------------------------------
void CMPPartSys::SetOpertion() {
	m_bShade = false;

	switch (_iType) {
	case PARTTICLE_SNOW:
		FrameUpdate = _FrameMoveSnow;
		break;
	case PARTTICLE_FIRE:
		FrameUpdate = _FrameMoveFire;
		break;
	case PARTTICLE_BLAST:
		FrameUpdate = _FrameMoveBlast;
		break;
	case PARTTICLE_RIPPLE:
		FrameUpdate = _FrameMoveRipple;
		break;
	case PARTTICLE_MODEL:
		FrameUpdate = _FrameMoveModel;
		break;
	case PARTTICLE_STRIP:
		FrameUpdate = _FrameMoveStrip;
		break;
	case PARTTICLE_WIND:
		FrameUpdate = _FrameMoveWind;
		break;
	case PARTTICLE_ARRAW:
		FrameUpdate = _FrameMoveArraw;
		break;
	case PARTTICLE_ROUND:
		FrameUpdate = _FrameMoveRound;
		break;
	case PARTTICLE_BLAST2:
		FrameUpdate = _FrameMoveBlast2;
		break;
	case PARTTICLE_BLAST3:
		FrameUpdate = _FrameMoveBlast3;
		break;
	case PARTTICLE_SHRINK:
		FrameUpdate = _FrameMoveShrink;
		break;
	case PARTTICLE_SHADE:
		FrameUpdate = _FrameMoveShade;
		m_bShade = true;
		break;
	case PARTTICLE_RANGE:
		FrameUpdate = _FrameMoveRange;
		break;
	case PARTTICLE_RANGE2:
		FrameUpdate = _FrameMoveRange2;
		break;
	case PARTTICLE_DUMMY:
		FrameUpdate = _FrameMoveDummy;
		break;
	case PARTTICLE_LINE_SINGLE:
		FrameUpdate = _FrameMoveLineSingle;
		break;
	case PARTTICLE_LINE_ROUND:
		FrameUpdate = _FrameMoveLineRound;
		break;
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::SetSkillCtrl(SkillCtrl* pCtrl) {
	if (!pCtrl)
		return;

	_fRange[0] *= pCtrl->fSize;
	_fRange[1] *= pCtrl->fSize;
	_fRange[2] *= pCtrl->fSize;

	SetSysNum((int)(_iParNum * pCtrl->fSize));

	if (pCtrl->fSize > -1) {
		for (WORD n = 0; n < _wFrameCount; n++) {
			*_vecFrameSize[n] *= pCtrl->fSize;
		}

		if (m_bShade)
			m_cShade.Create(_strTexName, m_pCResMagr, *_vecFrameSize[0]);
	}
}


//-----------------------------------------------------------------------------
void CMPPartSys::SetType(int iType, CMPResManger* pCResMagr) {
	_iType = iType;
	SetOpertion();

	if (m_bShade) {
		m_cShade.Create(_strTexName, pCResMagr, *_vecFrameSize[0]);
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::SetFrameCount(WORD wFrameCount) {
	Reset(false);

	if (wFrameCount > 100)
		wFrameCount = 100;
	float tsize = 1;
	D3DXVECTOR3 tangle(0, 0, 0);
	D3DXCOLOR tcolor(1, 1, 1, 1);
	WORD tw;
	if (wFrameCount > _wFrameCount) {
		tw = wFrameCount - _wFrameCount;
	}
	_vecFrameSize.setsize(wFrameCount);
	_vecFrameAngle.setsize(wFrameCount);
	_vecFrameColor.setsize(wFrameCount);
	if (wFrameCount > _wFrameCount) {
		for (int n = 0; n < tw; n++) {
			*_vecFrameSize[_wFrameCount + n] = tsize;
			*_vecFrameAngle[_wFrameCount + n] = tangle;
			*_vecFrameColor[_wFrameCount + n] = tcolor;
		}
	}
	_wFrameCount = wFrameCount;
}

void CMPPartSys::GetRes(int& idtex, int& idmodel, int& ideff) {
	idtex = -1;
	idmodel = -1;
	ideff = -1;
	if (m_bShade) {
		idtex = m_pCResMagr->GetTextureID(_strTexName);
		return;
	}
	int id = m_pCResMagr->GetMeshID(_strModelName);
	if (id < 0) {
		id = m_pCResMagr->GetEffectID(_strModelName);
		if (id < 0) {
		}
		else {
			ideff = id;
		}
	}
	else {
		if (id >= 7)
			idmodel = id;
	}

	if (id >= 0) {
		id = m_pCResMagr->GetTextureID(_strTexName);
		idtex = id;
	}
}

void CMPPartSys::SetMediayEff(bool bMediay) {
	if (_bMediay == bMediay)
		return;
	if (bMediay) {
		if (_iType == PARTTICLE_SNOW || _iType == PARTTICLE_FIRE ||
			_iType == PARTTICLE_SHRINK) {
			_bMediay = bMediay;
			BindingRes(m_pCResMagr);
		}
	}
	else {
		_bMediay = bMediay;
		BindingRes(m_pCResMagr);
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::BindingRes(CMPResManger* pCResMagr) {
	m_pCResMagr = pCResMagr;

	{
		SAFE_DELETE_ARRAY(_CPPart);
	}

	_pdwVShader = 0L;
	_pfDailTime = pCResMagr->GetDailTime();
	_pMatViewProj = pCResMagr->GetViewProjMat();
	_pCEffectFile = pCResMagr->GetEffectFile();

	SetOpertion();

	if (m_bShade) {
		m_cShade.Create(_strTexName, pCResMagr, *_vecFrameSize[0]);

		m_cShade.SetAlphaType(_eSrcBlend, _eDestBlend);
		return;
	}

	if (_pCModel && _pCModel->IsItem()) {
		pCResMagr->DeleteMesh(*_pCModel);
	}
	_pCModel = NULL;

	int id = pCResMagr->GetMeshID(_strModelName);
	if (id < 0) {
		id = pCResMagr->GetEffectID(_strModelName);
		if (id < 0) {
			MessageBox(NULL, _strModelName.c_str(), "ERROR", 0);
			return;
		}
		else {
			if (_bMediay) {
				_CPPart = new CMPModelEff[_iParNum];
				for (int t = 0; t < _iParNum; t++) {
					_CPPart[t].BindingEffect(pCResMagr->GetEffectByID(id));
					_CPPart[t].BindingRes(pCResMagr);
				}
			}
			else {
				_CPPart = new CMPModelEff[1];
				_CPPart->BindingEffect(pCResMagr->GetEffectByID(id));
				_CPPart->BindingRes(pCResMagr);
			}
		}
	}
	else {
		_pCModel = pCResMagr->GetMeshByID(id);
	}

	if (_pCModel) {
		id = pCResMagr->GetTextureID(_strTexName);
		if (id < 0) {
			_pTex = NULL;
			_lpCurTex = NULL;
			MessageBox(NULL, _strTexName.c_str(), "ERROR", 0);
			goto __ret;
		}
		_pTex = pCResMagr->GetTextureByIDlw(id);
		if (_pTex) {
			_lpCurTex = _pTex->GetTex();
		}

		if (_pCModel->IsItem()) {
			_pTex = pCResMagr->GetTextureByIDlw(id);
		}
	}

__ret:
	if (_pCModel)
		_SpmatBBoard = pCResMagr->GetBBoardMat();
	else {
		_SpmatBBoard = pCResMagr->GetBBoardMat();
	}

	//		//
}

void CMPPartSys::SetTextureName(const s_string& strTexName, CMPResManger* pCResMagr) {
	_strTexName = strTexName;
	int id;
	id = pCResMagr->GetTextureID(_strTexName);
	if (id < 0) {
		MessageBox(NULL, _strTexName.c_str(), "ERROR", 0);
		_lpCurTex = NULL;
		return;
	}
	_pTex = pCResMagr->GetTextureByIDlw(id);

	_lpCurTex = _pTex->GetTex();

	if (m_bShade)
		m_cShade.setFrameTexture(_strTexName, pCResMagr);
}

void CMPPartSys::SetModelName(const s_string& strModelName, CMPResManger* pCResMagr) {
	_strModelName = strModelName;
	{
		SAFE_DELETE_ARRAY(_CPPart);
	}

	if (_pCModel && _pCModel->IsItem()) {
		pCResMagr->DeleteMesh(*_pCModel);
	}
	_pCModel = NULL;
	int id = pCResMagr->GetMeshID(_strModelName);
	if (id < 0) {
		_pCModel = NULL;
		id = pCResMagr->GetEffectID(_strModelName);
		if (id < 0) {
			MessageBox(NULL, _strModelName.c_str(), "ERROR", 0);
			return;
		}
		else {
			if (_bMediay) {
				_CPPart = new CMPModelEff[_iParNum];
				for (int t = 0; t < _iParNum; t++) {
					_CPPart[t].BindingEffect(pCResMagr->GetEffectByID(id));
					_CPPart[t].BindingRes(pCResMagr);
					_CPPart[t].Play(0);
				}
			}
			else {
				_CPPart = new CMPModelEff[1];
				_CPPart->BindingEffect(pCResMagr->GetEffectByID(id));
				_CPPart->BindingRes(pCResMagr);
				_CPPart->Play(0);
			}
		}
	}
	else {
		_pCModel = pCResMagr->GetMeshByID(id);
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::SetEmissionPath(CEffPath* pcPath) {
	if (!pcPath) {
		_bModelRange = false;
		_vecPointRange.clear();
	}
	else {
		_bModelRange = true;
		_wVecNum = pcPath->m_iFrameCount;
		_vecPointRange.resize(_wVecNum);
		for (WORD n = 0; n < _wVecNum; ++n) {
			_vecPointRange[n] = pcPath->m_vecPath[n];
		}
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::InitParam() {
	_bPlay = false;
	_bStop = true;
	_bLoop = true;
	_wDeath = _iParNum;
	_bUseBone = false;
	D3DXMatrixIdentity(&_SBoneMat);
	_vScale = D3DXVECTOR3(1, 1, 1);

	_fLife = 4.5f;
	_vDir = D3DXVECTOR3(1.23f, 1.25f, 3.48f);
	_fVecl = 2.8f;
	_vAccel = D3DXVECTOR3(0, 0, 0);

	_fStep = 0.0f; //
	_fCurTime = 0;
	_fDelayTime = 0;
	_fPlayTime = 0;
	_fCurPlayTime = 0;

	_vecFrameSize.clear();
	_vecFrameAngle.clear();
	_vecFrameColor.clear();

	float fv = 2.0f;
	D3DXCOLOR sv = 0xffffffff;
	for (int n = 0; n < _wFrameCount; n++) {
		fv = 1.0f;
		_vecFrameSize.push_back(fv);
		auto v = D3DXVECTOR3(0, 0, 0);
		_vecFrameAngle.push_back(v);
		_vecFrameColor.push_back(sv);
	}
	_eSrcBlend = D3DBLEND_SRCALPHA;
	_eDestBlend = D3DBLEND_INVSRCALPHA;

	_eMinFilter = D3DTEXF_POINT;
	_eMagFilter = D3DTEXF_POINT;
}

void CMPPartSys::SetLoop(bool bLoop) {
	_bLoop = bLoop;
	if (_CPPart) {
		if (_bMediay) {
			for (int t = 0; t < _iParNum; t++) {
				_CPPart[t].Play(!_bLoop);
			}
		}
		else {
			_CPPart->Play(!_bLoop);
		}
	}
	if (_bLoop)
		SetPlayTime(0);
}

//-----------------------------------------------------------------------------
void CMPPartSys::Play(int iTime) {
	//	return;
	_fCurPlayTime = 0;

	switch (_iType) {
	case PARTTICLE_BLAST: {
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		if (_fDelayTime <= 0)
			_CreateBlast(this,NULL);
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}
		return;
	}
	case PARTTICLE_RIPPLE: {
		_bStop = false;
		//	return;
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		_wDeath = 0;
		_fCurTime = _fStep;
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}
		return;
	}
	case PARTTICLE_MODEL: {
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		_CreateModel(this, _vecParticle[0]);
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(!_bLoop);
			}
			else
				_CPPart->Play(!_bLoop);
		}

		return;
	}
	case PARTTICLE_STRIP: {
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		_CreateStrip(this, _vecParticle[0]);
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(!_bLoop);
			}
			else
				_CPPart->Play(!_bLoop);
		}
		return;
	}
	case PARTTICLE_WIND: {
		_bStop = false;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		_bPlay = true;
		_wDeath = 0;
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}
		return;
	}
	case PARTTICLE_ARRAW: {
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		_CreateArraw(this, _vecParticle[0]);
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(!_bLoop);
			}
			else
				_CPPart->Play(!_bLoop);
		}
		return;
	}
	case PARTTICLE_ROUND: {
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		_CreateRound(this,NULL);
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}
		break;
	}
	case PARTTICLE_BLAST2: {
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		if (_fDelayTime <= 0)
			_CreateBlast2(this,NULL);
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}
		break;
	}
	case PARTTICLE_BLAST3: {
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;
		if (_fDelayTime <= 0)
			_CreateBlast3(this,NULL);
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}
		return;
	}
	case PARTTICLE_SHADE: {
		_bPlay = true;
		_CreateShade(this, _vecParticle[0]);
		return;
	}
	case PARTTICLE_RANGE: {
		_bPlay = true;
		_CreateRange(this,NULL);
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}
		_wDeath = 0;
		_fCurTime = 0;
		return;
	}
	case PARTTICLE_RANGE2: {
		_bPlay = true;
		_bStop = false;
		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}
		_wDeath = 0;
		_fCurTime = 0;
		_vecParticle[0]->m_fLife = 0;
		return;
	}
	case PARTTICLE_LINE_SINGLE: {
		_bPlay = true;
		_bLoop = iTime == 0 ? true : false;
		_fCurTime = 0;

		if (_CPPart) {
			if (_bMediay) {
				for (int t = 0; t < _iParNum; t++)
					_CPPart[t].Play(0);
			}
			else
				_CPPart->Play(0);
		}

		_CreateLineSingle(this, _vecParticle[0]);
	}
	} //	end of switch

	_bStop = false;
	_bPlay = true;
	_bLoop = iTime == 0 ? true : false;
	_fCurTime = 0;
	if (_CPPart) {
		if (_bMediay) {
			for (int t = 0; t < _iParNum; t++)
				_CPPart[t].Play(0);
		}
		else
			_CPPart->Play(0);
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::Stop() {
	CMPParticle* pParticle;
	switch (_iType) {
	case PARTTICLE_SNOW:
	case PARTTICLE_FIRE:
		if (_bStop)
			return;
		_wDeath = 0;
		for (WORD n = 0; n < _iParNum; ++n) {
			pParticle = _vecParticle[n];
			if (!pParticle->m_bLive)
				_wDeath++;
		}
		_bStop = true;
		break;
	case PARTTICLE_BLAST:
	case PARTTICLE_RIPPLE:
	case PARTTICLE_MODEL:
	case PARTTICLE_STRIP:
	case PARTTICLE_WIND:
	case PARTTICLE_ARRAW:
	case PARTTICLE_ROUND:
	case PARTTICLE_BLAST2:
	case PARTTICLE_BLAST3:
	case PARTTICLE_SHADE:
	case PARTTICLE_RANGE:
	case PARTTICLE_RANGE2:
	case PARTTICLE_DUMMY:
	case PARTTICLE_LINE_SINGLE:
	case PARTTICLE_LINE_ROUND:
		_bPlay = false;
		break;
	case PARTTICLE_SHRINK:
		_wDeath = 0;
		_bStop = true;
		break;
	}
}

bool CMPPartSys::IsPlaying() {
	switch (_iType) {
	case PARTTICLE_SNOW:
	case PARTTICLE_FIRE:
		return !_bStop;
	case PARTTICLE_BLAST:
	case PARTTICLE_RIPPLE:
	case PARTTICLE_MODEL:
	case PARTTICLE_STRIP:
	case PARTTICLE_WIND:
	case PARTTICLE_ARRAW:
	case PARTTICLE_ROUND:
	case PARTTICLE_BLAST2:
	case PARTTICLE_BLAST3:
	case PARTTICLE_SHADE:
	case PARTTICLE_RANGE:
	case PARTTICLE_RANGE2:
	case PARTTICLE_DUMMY:
	case PARTTICLE_LINE_SINGLE:
	case PARTTICLE_LINE_ROUND:
		return _bPlay;
	case PARTTICLE_SHRINK:
		return !_bStop;
	}

	return false;
}

//-----------------------------------------------------------------------------
void CMPPartSys::MoveTo(const D3DXVECTOR3* vPos, MPMap* pmap) {
	_vPos = D3DXVECTOR3(vPos->x - _fRange[0] / 2, vPos->y - _fRange[1] / 2, vPos->z - _fRange[2] / 2);

	_vPos += _vOffset;

	_vSavePos = _vPos;
	m_pMap = pmap;
}

//-----------------------------------------------------------------------------
void CMPPartSys::BindingBone(D3DXMATRIX* pMatBone) {
	if (_bBillBoard) {
		MoveTo((D3DXVECTOR3*)&pMatBone->_41);
	}
	else {
		if (!_pCModel) {
			if ((_iType == PARTTICLE_MODEL || _iType == PARTTICLE_STRIP)
				&& !_bModelDir) {
				_bUseBone = true;
				_SBoneMat = *pMatBone;
				_SBoneMat._41 += _vOffset.x;
				_SBoneMat._42 += _vOffset.y;
				_SBoneMat._43 += _vOffset.z;
			}
			else {
				MoveTo((D3DXVECTOR3*)&pMatBone->_41);
			}
		}
		else {
			if (_pCModel->IsItem() || (_iType == PARTTICLE_RIPPLE)) {
				_bUseBone = true;
				_SBoneMat = *pMatBone;
			}
			else
				MoveTo((D3DXVECTOR3*)&pMatBone->_41);
		}
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::FrameMove(DWORD dwDailTime) {
	if (!_bPlay)
		return;
	if (IsDelay())
		return;

	if (_pcPath) {
		_pcPath->FrameMove(*_pfDailTime);
		_vPos = _vSavePos + *_pcPath->GetCurPos();
	}

	FrameUpdate(this, dwDailTime);
}

//-----------------------------------------------------------------------------
void CMPPartSys::Render() {
	if (!_bPlay)
		return;
	if (UpdateDelay())
		RenderSoft();
}

//-----------------------------------------------------------------------------
void CMPPartSys::RenderVS() {
}

//-----------------------------------------------------------------------------
void CMPPartSys::RenderSoft() {
	//	return;
	if (m_bShade) {
		CMPParticle* pParticle;
		pParticle = _vecParticle[0];

		m_cShade.setColor(pParticle->m_SCurColor);
		m_cShade.Render();
		return;
	}

	if (_bFontCom) {
		_pCEffectFile->SetTechnique(_iRenderIdx);
		_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
		_pCEffectFile->Pass(0);
		_pCEffectFile->GetDev()->SetVertexShader(NULL);
		_pCEffectFile->GetDev()->SetFVF(EFFECT_VER_FVF);

		_pCEffectFile->GetDev()->SetRenderState(D3DRS_SRCBLEND, _eSrcBlend);
		_pCEffectFile->GetDev()->SetRenderState(D3DRS_DESTBLEND, _eDestBlend);

		_pCEffectFile->GetDev()->SetSamplerState(0, D3DSAMP_MAGFILTER, _eMagFilter);
		_pCEffectFile->GetDev()->SetSamplerState(0, D3DSAMP_MINFILTER, _eMinFilter);

		if (!_bUseZ) {
			_pCModel->GetDev()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}
		else {
			_pCModel->GetDev()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

		{
			for (WORD n = 0; n < (WORD)_vecText.size(); ++n) {
				if (_vecParticle[n]->m_bLive) {
					_pCEffectFile->GetDev()->SetRenderState(D3DRS_TEXTUREFACTOR,
															_vecParticle[n]->m_SCurColor);
					_pCEffectFile->GetDev()->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
					_pCEffectFile->GetDev()->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
					_pcEffFont[n].RenderEffectFont(&_vecParticle[n]->m_SCurMat);
					_pCEffectFile->GetDev()->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
					_pCEffectFile->GetDev()->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				}
			}
		}
		_pCEffectFile->End();
		return;
	}
	if (_pCModel) {
		_pCModel->Begin();

		_pCEffectFile->SetTechnique(_iRenderIdx);
		if (!_bUseZ) {
			_pCModel->GetDev()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}
		else {
			_pCModel->GetDev()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
		_pCEffectFile->Pass(0);
		_pCEffectFile->GetDev()->SetVertexShader(NULL);
		_pCEffectFile->GetDev()->SetFVF(EFFECT_VER_FVF);

		if (_pTex && _pTex->IsLoadingOK())
			_pCModel->m_pDev->SetTexture(0, _pTex->GetTex());
		else {
			_pCEffectFile->End();
			return;
		}


		_pCModel->GetDev()->SetRenderState(D3DRS_SRCBLEND, _eSrcBlend);
		_pCModel->GetDev()->SetRenderState(D3DRS_DESTBLEND, _eDestBlend);

		_pCModel->GetDev()->SetSamplerState(0, D3DSAMP_MAGFILTER, _eMagFilter);
		_pCModel->GetDev()->SetSamplerState(0, D3DSAMP_MINFILTER, _eMinFilter);


		{
			for (WORD n = 0; n < _iParNum; ++n) {
				if (_vecParticle[n]->m_bLive) {
					_pCModel->GetDev()->SetRenderState(D3DRS_TEXTUREFACTOR,
													   _vecParticle[n]->m_SCurColor);
					_pCModel->GetDev()->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
					_pCModel->GetDev()->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
					if (_pCModel->IsItem()) {
						_pCModel->SetMatrix((lwMatrix44*)&_vecParticle[n]->m_SCurMat);

						lwITex* tex;
						lwITex* tex2;

						if (_pTex) {
							_pCModel->ResetItemTexture(0, _pTex, &tex);
							_pCModel->ResetItemTexture(1, _pTex, &tex2);
						}

						_pCModel->FrameMove(0);
					}
					else
						_pCModel->GetDev()->SetTransformWorld(
							&_vecParticle[n]->m_SCurMat);


					_pCModel->RenderModel();
					_pCModel->GetDev()->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					_pCModel->GetDev()->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
					_pCModel->GetDev()->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
				}
			}
		}
		_pCModel->End();

		_pCEffectFile->End();
	}
	else {
		CMPParticle* pParticle;

		if (_iType == PARTTICLE_ARRAW) {
			pParticle = _vecParticle[0];
			_CPPart->Scaling(pParticle->m_fSize * _vScale.x, pParticle->m_fSize * _vScale.y,
							 pParticle->m_fSize * _vScale.z);

			_CPPart->MoveTo(pParticle->m_vPos.x, pParticle->m_vPos.y,
							pParticle->m_vPos.z);
			if (pParticle->m_SCurColor.a < 1.0f)
				_CPPart->SetAlpha(pParticle->m_SCurColor.a);
			_CPPart->Render();

			return;
		}
		if (_iType == PARTTICLE_SHRINK) {
			for (WORD n = 0; n < _iParNum; ++n) {
				pParticle = _vecParticle[n];

				if (pParticle->m_bLive) {
					if (_bMediay) {
						_CPPart[n].Scaling(pParticle->m_fSize * _vScale.x, pParticle->m_fSize * _vScale.y,
										   pParticle->m_fSize * _vScale.z);
						_CPPart[n].MoveTo(pParticle->m_vPos.x, pParticle->m_vPos.y,
										  pParticle->m_vPos.z);

						_CPPart[n].RotatingXZ(pParticle->m_vCurAngle.x, pParticle->m_vCurAngle.y);
						if (pParticle->m_SCurColor.a < 1.0f)
							_CPPart[n].SetAlpha(pParticle->m_SCurColor.a);
						_CPPart[n].Render();
					}
					else {
						_CPPart->Scaling(pParticle->m_fSize * _vScale.x, pParticle->m_fSize * _vScale.y,
										 pParticle->m_fSize * _vScale.z);
						{
							_CPPart->MoveTo(pParticle->m_vPos.x, pParticle->m_vPos.y,
											pParticle->m_vPos.z);

							_CPPart->RotatingXZ(pParticle->m_vCurAngle.x, pParticle->m_vCurAngle.y);
						}
						if (pParticle->m_SCurColor.a < 1.0f)
							_CPPart->SetAlpha(pParticle->m_SCurColor.a);
						_CPPart->Render();
					}
				}
			}
			return;
		}

		CMPModelEff* pPart;
		for (WORD n = 0; n < _iParNum; ++n) {
			pParticle = _vecParticle[n];

			if (_bMediay)
				pPart = &_CPPart[n];
			else
				pPart = _CPPart;

			if (pParticle->m_bLive) {
				pPart->Scaling(pParticle->m_fSize * _vScale.x, pParticle->m_fSize * _vScale.y,
							   pParticle->m_fSize * _vScale.z);

				if (_bUseBone) {
					pPart->BindingBone(pParticle->m_SCurMat, true);
				}
				else if (_bBillBoard) {
					D3DXMATRIX tm = *_SpmatBBoard;
					tm._41 = pParticle->m_vPos.x;
					tm._42 = pParticle->m_vPos.y;
					tm._43 = pParticle->m_vPos.z;

					pPart->BindingBone(tm, true);
				}
				else {
					pPart->MoveTo(pParticle->m_vPos.x, pParticle->m_vPos.y,
								  pParticle->m_vPos.z);
					if (_bModelDir) {
						D3DXMatrixIdentity(&pPart->m_SMatTempRota);
					}
					if (pParticle->m_vCurAngle.x != 0)
						pPart->RotatingPitchPart(pParticle->m_vCurAngle.x);
					if (pParticle->m_vCurAngle.y != 0)
						pPart->RotatingRollPart(pParticle->m_vCurAngle.y);
					if (pParticle->m_vCurAngle.z != 0)
						pPart->RotatingYawPart(pParticle->m_vCurAngle.z);

					if (_bModelDir) {
						D3DXMATRIX mat;
						if (_iType == PARTTICLE_MODEL || _iType == PARTTICLE_STRIP) {
							RotatingXZ(&mat, _vTemDir.x, _vTemDir.y);
						}
						else {
							RotatingXZ(&mat, pParticle->m_vOldPos.x, pParticle->m_vOldPos.y);
						}
						pPart->m_SMatTempRota *= mat;
					}
					D3DXMATRIX tm; // = *_SpmatBBoard;
					pPart->BindingBone(tm, false);
				}
				if (pParticle->m_SCurColor.a < 1.0f)
					pPart->SetAlpha(pParticle->m_SCurColor.a);
				pPart->Render();
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::setYaw(float fYaw) {
	if (_CPPart) {
		for (WORD n = 0; n < _wFrameCount; ++n) {
			_vecFrameAngle[n]->z = fYaw;
		}
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::setPitch(float fPitch) {
	if (_CPPart) {
		for (WORD n = 0; n < _wFrameCount; ++n) {
			_vecFrameAngle[n]->x = fPitch;
		}
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::setRoll(float fRoll) {
	if (_CPPart) {
		for (WORD n = 0; n < _wFrameCount; ++n) {
			_vecFrameAngle[n]->y = fRoll;
		}
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::setScale(float fx, float fy, float fz) {
	_vScale.x = fx;
	_vScale.y = fy;
	_vScale.z = fz;
}

//-----------------------------------------------------------------------------
void CMPPartSys::setDir(float fx, float fy, float fz) {
	if (!_bModelDir)
		return;
	switch (_iType) {
	case PARTTICLE_FIRE: {
		SetSysDirX(fx);
		SetSysDirY(fy);
		SetSysDirZ(fz);

		if (_CPPart) {
			D3DXVECTOR3 vDir(fx, fy, fz);
			GetDirRotation(&_vTemDir, &vDir);
		}
		break;
	}
	case PARTTICLE_MODEL:
	case PARTTICLE_STRIP: {
		if (!_CPPart)
			return;

		D3DXVECTOR3 vDir(fx, fy, fz);
		GetDirRotation(&_vTemDir, &vDir);

		////X
		break;
	}
	}
}

//-----------------------------------------------------------------------------
void CMPPartSys::setFontEffect(CMPFont* /*pFont*/) {
	// No-op. Прямой CMPFont*-аргумент устарел; поле _pFont удалено.
	// Отрисовка частиц-с-текстом была закомментирована ранее, а новый
	// путь — принимать FontSlot / имя и резолвить в FontManager на
	// момент отрисовки. См. memory/font-api-refactor-todo.md.
}

//-----------------------------------------------------------------------------
void CMPPartSys::setFontEffText(const char* pszText) {
	_strText = pszText;
}

//-----------------------------------------------------------------------------
void CMPPartSys::Reset(bool bLife) {
	_bPlay = false;
	_fCurTime = 0;
	_bPlay = false;
	_bStop = true;
	_bUseBone = false;
	D3DXMatrixIdentity(&_SBoneMat);
	_fCurPlayTime = 0;

	const auto v = D3DXVECTOR3(0, 0, 0);
	MoveTo(&v);
	for (WORD n = 0; n < _iParNum; ++n) {
		_vecParticle[n]->m_bLive = bLife;
		_vecParticle[n]->Reset(v);
	}
	if (_CPPart) {
		if (_bMediay)
			for (int t = 0; t < _iParNum; t++)
				_CPPart[t].Reset();
		else
			_CPPart->Reset();
	}
	if (_pcPath)
		_pcPath->Reset();

	if (_bModelDir) {
		_vDir.x = 0;
		_vDir.y = 0;
		_vDir.z = 0;
	}
}

//-----------------------------------------------------------------------------
bool _CreateDummy(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_fCurTime += *pPart->_pfDailTime;

	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;

		D3DXVec3Normalize(&pCtrl->m_vOldPos, &pPart->_vDir);

		float dist = Randf(pPart->_fDummyDist, pPart->_iParNum);
		pCtrl->m_vPos = pPart->_vDummyPos + pPart->_vDummyDir * dist;

		pCtrl->m_bLive = true;
		pCtrl->m_fLife = Randf(pPart->_fLife / pPart->_iRoadom, pPart->_fLife);

		pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;

		pCtrl->m_fCurTime = 0;
		pCtrl->m_wCurFrame = 0;


		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void _FrameMoveDummy(CMPPartSys* pPart, DWORD dwDailTime) {
	if (!pPart->GetDummyPosList())
		pPart->_bPlay = false;

	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;

	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}

	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;


			pParticle->m_vVel = pParticle->m_vOldPos * (pPart->_fVecl * *pPart->_pfDailTime);
			if (rand() % 2)
				pParticle->m_vAccel = (pPart->_vAccel * *pPart->_pfDailTime);
			else
				pParticle->m_vAccel = -(pPart->_vAccel * *pPart->_pfDailTime);

			pParticle->m_vVel += pParticle->m_vAccel;

			pParticle->m_vPos += pParticle->m_vVel;
			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y,
												   pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
			}
		}
		else {
			_CreateDummy(pPart, pParticle);
		}
	}
}

//-----------------------------------------------------------------------------
bool _CreateLineSingle(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_fCurTime += *pPart->_pfDailTime;

	if (!pPart->GetDummyPosList())
		pPart->_bPlay = false;
	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;

		D3DXVec3Normalize(&pCtrl->m_vOldPos, &pPart->_vDir);

		pCtrl->m_vPos = pPart->_vDummyPos;

		pCtrl->m_bLive = true;
		pCtrl->m_fLife = Randf(pPart->_fLife / pPart->_iRoadom, pPart->_fLife);
		pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;
		pCtrl->m_fCurTime = 0;
		pCtrl->m_wCurFrame = 0;
		pCtrl->m_vVel = (pPart->_fDummyDist / pCtrl->m_fLife) * pPart->_vDummyDir;

		if (pPart->_CPPart) {
			if (pPart->_bMediay) {
				pPart->_CPPart[pPart->_idt].Reset();
				pPart->_CPPart[pPart->_idt].Play(0);
			}
		}

		pCtrl->m_fPartTime = ((float)Rand(100)) / 1000.0f;

		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void _FrameMoveLineSingle(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;

	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}

	//_vDummyDir
	//_fDummyDist
	//_vDummyPos

	CMPParticle* pParticle(0);
	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;


			float fDailTime = *(pPart->_pfDailTime);
			pParticle->m_vPos += pParticle->m_vVel * fDailTime + 0.5 * pParticle->m_vAccel * fDailTime * fDailTime;

			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y,
												   pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
			}
		}
		else {
			_CreateLineSingle(pPart, pParticle);
		}
	}
}

//-----------------------------------------------------------------------------
bool _CreateLineRound(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_fCurTime += *pPart->_pfDailTime;

	if (!pPart->GetDummyPosList())
		pPart->_bPlay = false;
	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;

		D3DXVec3Normalize(&pCtrl->m_vOldPos, &pPart->_vDir);

		pCtrl->m_vPos = pPart->_vDummyPos;

		pCtrl->m_bLive = true;
		pCtrl->m_fLife = Randf(pPart->_fLife / pPart->_iRoadom, pPart->_fLife);
		pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;
		pCtrl->m_fCurTime = 0;
		pCtrl->m_wCurFrame = 0;
		pCtrl->m_vVel = (pPart->_fDummyDist / pCtrl->m_fLife * 2) * pPart->_vDummyDir;

		if (pPart->_CPPart) {
			if (pPart->_bMediay) {
				pPart->_CPPart[pPart->_idt].Reset();
				pPart->_CPPart[pPart->_idt].Play(0);
			}
		}

		pCtrl->m_fPartTime = ((float)Rand(100)) / 1000.0f;

		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void _FrameMoveLineRound(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;

	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}

	//_vDummyDir
	//_fDummyDist
	//_vDummyPos

	CMPParticle* pParticle(0);
	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;


			float fDailTime = *(pPart->_pfDailTime);
			if (pParticle->m_fCurTime > pParticle->m_fLife / 2
				&& pParticle->m_fCurTime - fDailTime < pParticle->m_fLife / 2)
				pParticle->m_vVel = -pParticle->m_vVel;

			pParticle->m_vPos += pParticle->m_vVel * fDailTime/* + 0.5 * pParticle->m_vAccel * fDailTime *fDailTime*/;

			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y,
												   pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
			}
		}
		else {
			_CreateLineSingle(pPart, pParticle);
		}
	}
}

//-----------------------------------------------------------------------------

bool _CreateRange2(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_fCurTime += *pPart->_pfDailTime;

	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;

		D3DXVec3Normalize(&pCtrl->m_vOldPos, &pPart->_vDir);

		pCtrl->m_vPos = pPart->_vPos + D3DXVECTOR3(Randf(pPart->_fRange[0]),
												   Randf(pPart->_fRange[1]), pPart->_fRange[2]);

		pCtrl->m_bLive = true;
		pCtrl->m_SCurColor = D3DXCOLOR(1, 1, 1, 1);


		if (pPart->_CPPart) {
			float dirxz[2];

			float fdist = D3DXVec3Length(&pCtrl->m_vOldPos);

			if (pCtrl->m_vOldPos.z == 0) {
				dirxz[0] = 0;
			}
			else {
				const auto v = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				dirxz[0] = asinf(D3DXVec3Dot(&pCtrl->m_vOldPos, &v) / fdist);
			}
			if (pCtrl->m_vOldPos.x == 0 && pCtrl->m_vOldPos.y == 0) {
				dirxz[1] = 0;
			}
			else {
				const D3DXVECTOR3 v[] = {
					D3DXVECTOR3(pCtrl->m_vOldPos.x, pCtrl->m_vOldPos.y, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)
				};
				fdist = D3DXVec3Length(&v[0]);
				dirxz[1] = acosf(D3DXVec3Dot(&v[0], &v[1]) / fdist);
				if (pCtrl->m_vOldPos.x >= 0.0f) {
					dirxz[1] = -dirxz[1];
				}
			}
			pPart->_CPPart->RotatingXZ(dirxz[0], dirxz[1]);
		}

		return true;
	}
	return false;
}

void _FrameMoveRange2(CMPPartSys* pPart, DWORD dwDailTime) {
	CMPParticle* pParticle;


	pParticle = pPart->_vecParticle[0];
	pParticle->m_fLife += *pPart->_pfDailTime;
	if (pParticle->m_fLife > pPart->_fLife)
		pPart->_bStop = true;

	bool bcon = false;

	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}
	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			bcon = true;
			pParticle->m_vVel = pParticle->m_vOldPos * (pPart->_fVecl * *pPart->_pfDailTime);

			pParticle->m_vPos += pParticle->m_vVel;

			if (pPart->m_pMap) {
				float fei = pPart->m_pMap->GetGridHeight((int)pParticle->m_vPos.x * 2, (int)pParticle->m_vPos.y * 2);
				if (pParticle->m_vPos.z < fei || pParticle->m_vPos.z > 50.0f) {
					pParticle->m_bLive = false;
					pParticle->m_vPos.z = fei;
					pPart->m_pCResMagr->SendResMessage(pPart->m_strHitEff, pParticle->m_vPos, pPart->m_pMap);
					continue;
				}
			}
			else {
				if (pParticle->m_vPos.z < 0 || pParticle->m_vPos.z > 50.0f) {
					pParticle->m_bLive = false;
					pPart->m_pCResMagr->SendResMessage(pPart->m_strHitEff, pParticle->m_vPos, pPart->m_pMap);
					continue;
				}
			}

			pParticle->m_fSize = *pPart->_vecFrameSize[0];

			if (!pPart->_CPPart) {
				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
			}
		}
		else {
			if (!pPart->_bStop)
				_CreateRange2(pPart, pParticle);
		}
	}
	if (pPart->_bStop) {
		if (!bcon) {
			pPart->_bPlay = false;
			return;
		}
	}
}

bool _CreateRange(CMPPartSys* pPart, CMPParticle* pCtrl) {
	D3DXVECTOR3 pos(0, 0, 0);
	CMPParticle* pParticle;
	for (int n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];

		pos = D3DXVECTOR3(Randf(pPart->_fRange[0]),
						  Randf(pPart->_fRange[1]), Randf(pPart->_fRange[2]));

		pParticle->m_vOldPos.x = pos.x;
		pParticle->m_vOldPos.y = pos.y;
		pParticle->m_vOldPos.z = pos.z;

		pParticle->m_vPos = pPart->_vPos;

		pParticle->m_bLive = true;
		pParticle->m_SCurColor = D3DXCOLOR(1, 1, 1, 1);
		D3DXMatrixIdentity(&pParticle->m_SCurMat);
		pParticle->m_vCurAngle = D3DXVECTOR3(0, 0, 0);
	}
	return true;
}

void _FrameMoveRange(CMPPartSys* pPart, DWORD dwDailTime) {
	CMPParticle* pParticle;

	float dirxz[2];

	if (pPart->_CPPart) {
		if (pPart->_pcPath) {
			if (pPart->_fLife > 1) {
				if (pPart->_pcPath->IsEnd()) {
					pPart->_wDeath++;
					if (pPart->_wDeath >= (int)pPart->_fLife) {
						pPart->_bPlay = false;
						return;
					}
					_CreateRange(pPart,NULL);
					pPart->_pcPath->Reset();
					return;
				}
			}
			D3DXVECTOR3* pstart = pPart->_pcPath->GetNextPos();
			D3DXVECTOR3* pend = pPart->_pcPath->GetCurPos();
			D3DXVECTOR3 vdir = *pend - *pstart;
			D3DXVec3Normalize(&vdir, &vdir);
			float fdist = D3DXVec3Length(&vdir);

			if (vdir.z == 0) {
				dirxz[0] = 0;
			}
			else {
				const auto v = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				dirxz[0] = asinf(D3DXVec3Dot(&vdir, &v) / fdist);
			}
			if (vdir.x == 0 && vdir.y == 0) {
				dirxz[1] = 0;
			}
			else {
				const D3DXVECTOR3 v[] = {D3DXVECTOR3(vdir.x, vdir.y, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f)};
				fdist = D3DXVec3Length(&v[0]);
				dirxz[1] = acosf(D3DXVec3Dot(&v[0], &v[1]) / fdist);
				if (vdir.x >= 0.0f) {
					dirxz[1] = -dirxz[1];
				}
			}
			pPart->_CPPart->RotatingXZ(dirxz[0], dirxz[1]);
		}
		pPart->_CPPart->FrameMove(dwDailTime);
	}
	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			pParticle->m_vPos = pParticle->m_vOldPos + pPart->_vPos;

			pParticle->m_fSize = *pPart->_vecFrameSize[0];

			if (!pPart->_CPPart) {
				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
			}
		}
	}
}

bool _CreateShade(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_iParNum = 1;
	pCtrl->m_bLive = true;

	pCtrl->m_fFrameTime = pPart->_fLife;

	pCtrl->m_fCurTime = 0;
	pCtrl->m_wCurFrame = 0;

	return true;
}

void _FrameMoveShade(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;

	pParticle = pPart->_vecParticle[0];
	pParticle->m_vPos = pPart->_vPos;

	pPart->m_cShade.MoveTo(pParticle->m_vPos, pPart->m_pMap);
	pPart->m_cShade.FrameMove(0);

	wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
	if (wCurFrame == pPart->_wFrameCount) {
		pPart->_bPlay = false;
		return;
	}
	else {
		if (wCurFrame == pPart->_wFrameCount - 1)
			wNextFrame = wCurFrame;
		else
			wNextFrame = wCurFrame + 1;
	}
	fLerp = pParticle->GetLerpValue();
	D3DXColorLerp(&pParticle->m_SCurColor,
				  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);
}


bool _CreateShrink(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_fCurTime += *pPart->_pfDailTime;

	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;


		D3DXVECTOR3 vpos = pPart->_vPos + D3DXVECTOR3(pPart->_fRange[0] / 2, pPart->_fRange[1] / 2,
													  pPart->_fRange[2] / 2);
		pCtrl->m_vOldPos = vpos;

		pCtrl->m_vPos = pPart->_vPos + D3DXVECTOR3(Randf(pPart->_fRange[0]),
												   Randf(pPart->_fRange[1]), Randf(pPart->_fRange[2]));

		pCtrl->m_vAccel = vpos - pCtrl->m_vPos;
		D3DXVec3Normalize(&pCtrl->m_vAccel, &pCtrl->m_vAccel);

		if (pPart->_CPPart) {
			D3DXVECTOR2 vangle;
			GetDirRotation(&vangle, &pCtrl->m_vAccel);

			pCtrl->m_vCurAngle.x = vangle.x;
			pCtrl->m_vCurAngle.y = vangle.y;

			if (pPart->_bMediay) {
				pPart->_CPPart[pPart->_idt].Reset();
				pPart->_CPPart[pPart->_idt].Play(0);
			}
		}

		pCtrl->m_bLive = true;
		pCtrl->m_fLife = Randf(pPart->_fLife / pPart->_iRoadom, pPart->_fLife);

		pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;

		pCtrl->m_fCurTime = 0;
		pCtrl->m_wCurFrame = 0;
		pCtrl->m_fPartTime = ((float)Rand(100)) / 1000.0f;

		return true;
	}
	return false;
}

void _FrameMoveShrink(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;


	if (pPart->_CPPart) {
		if (pPart->_bMediay) {
			for (WORD n = 0; n < pPart->_iParNum; ++n)
				pPart->_CPPart[n].FrameMove(dwDailTime);
		}
		else
			pPart->_CPPart->FrameMove(dwDailTime);
	}

	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;

			pParticle->m_vPos += pParticle->m_vAccel * (pPart->_fVecl * *pPart->_pfDailTime);
			if (PointPointRange(&pParticle->m_vPos, &pParticle->m_vOldPos, 0.5f)) {
				pParticle->m_bLive = false;
				if (pPart->_bStop) {
					pPart->_wDeath++;
					if (pPart->_wDeath >= pPart->_iParNum)
						pPart->_bPlay = false;
				}
				continue;
			}

			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y, pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				//D3DXVec3Lerp(&pParticle->m_vCurAngle,pPart->_vecFrameAngle[wCurFrame],
			}
		}
		else {
			if (!pPart->_bStop) {
				pPart->_idt = n;
				_CreateShrink(pPart, pParticle);
			}
		}
	}
}

bool _CreateBlast3(CMPPartSys* pPart, CMPParticle* pCtrl) {
	float angle = (float)(D3DX_PI * 2) / pPart->_iParNum;
	D3DXVECTOR3 dir(1, 1, -1);
	D3DXVECTOR4 pos(0, 0, 0, 0);
	D3DXMATRIX mat;
	CMPParticle* pParticle;
	for (int n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];

		D3DXMatrixRotationYawPitchRoll(&mat, 0, 0, angle * n);
		D3DXVec3Transform(&pos, &dir, &mat);

		pParticle->m_vOldPos.x = pos.x;
		pParticle->m_vOldPos.y = pos.y;
		pParticle->m_vOldPos.z = pos.z;

		pParticle->m_vOldPos += pPart->_vDir;


		pParticle->m_vPos = pPart->_vPos;

		pParticle->m_bLive = true;
		pParticle->m_fLife = pPart->_fLife;

		pParticle->m_fFrameTime = pParticle->m_fLife / pPart->_wFrameCount;

		pParticle->m_fCurTime = 0;
		pParticle->m_wCurFrame = 0;

		pParticle->m_fPartTime = ((float)Rand(100)) / 1000.0f;
	}
	pPart->_wDeath = 0;
	pPart->_fCurTime = 0;
	return true;
}

void _FrameMoveBlast3(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;
	D3DXVECTOR4 pos(0, 0, 0, 0);
	D3DXMATRIX mat;

	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}
	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				pPart->_bPlay = false;
				return;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;

			//pParticle->m_vPos.y = pos.y;0

			float fvel = pPart->_fVecl * *pPart->_pfDailTime;

			pParticle->m_vPos += pParticle->m_vOldPos * fvel;
			pParticle->m_vOldPos += pPart->_vAccel * *pPart->_pfDailTime;

			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y, pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
				if (pPart->_bUseBone) {
				}
			}
		}
	}
}

bool _CreateBlast2(CMPPartSys* pPart, CMPParticle* pCtrl) {
	float angle = (float)(D3DX_PI * 2) / pPart->_iParNum;
	D3DXVECTOR3 dir(pPart->_vFontDir);
	D3DXVECTOR4 pos(0, 0, 0, 0);
	D3DXMATRIX mat;
	CMPParticle* pParticle;
	for (int n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];


		pParticle->m_vOldPos.x = dir.x;
		pParticle->m_vOldPos.y = dir.y;
		pParticle->m_vOldPos.z = dir.z;


		pParticle->m_vPos = pPart->_vPos;

		pParticle->m_bLive = true;
		pParticle->m_fLife = pPart->_fLife;

		pParticle->m_fFrameTime = pParticle->m_fLife / pPart->_wFrameCount;

		pParticle->m_fCurTime = 0;
		pParticle->m_wCurFrame = 0;

		pParticle->m_fPartTime = ((float)Rand(100)) / 1000.0f;

		pParticle->m_vAccel.x = 0;
		pParticle->m_vAccel.y = pPart->_vecFrameAngle[0]->x;
		pParticle->m_vAccel.z = 0;
	}
	pPart->_wDeath = 0;
	pPart->_fCurTime = 0;
	return true;
}

void _FrameMoveBlast2(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;
	D3DXVECTOR4 pos(0, 0, 0, 0);
	D3DXMATRIX mat;

	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}
	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			if (pParticle->m_vAccel.x == 0) {
				wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime,
												   pPart->_wFrameCount, pParticle->m_fFrameTime / 3);
				if (wCurFrame == 1) {
					pParticle->m_vAccel.x += 1;
					pParticle->m_vCurAngle = pParticle->m_vPos;
				}
			}
			else
				wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);

			float fvel{};

			if (wCurFrame == pPart->_wFrameCount) {
				pPart->_bPlay = false;
				return;
			}
			else {
				if (wCurFrame == 1) {
					if (pParticle->m_vAccel.z == 0) {
						pParticle->m_vPos.x = pParticle->m_vCurAngle.x + pParticle->m_vAccel.y;
						pParticle->m_vAccel.z = 1;
					}
					else if (pParticle->m_vAccel.z == 1) {
						pParticle->m_vPos.x = pParticle->m_vCurAngle.x - pParticle->m_vAccel.y;
						pParticle->m_vAccel.z = 2;
					}
					else if (pParticle->m_vAccel.z == 2) {
						pParticle->m_vPos.x = pParticle->m_vCurAngle.x;
						pParticle->m_vAccel.z = 3;
						wCurFrame++;
						continue;
					}

					goto _ret;
					//return;
				}
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;


			fvel = pPart->_fVecl * *pPart->_pfDailTime;
			if (pParticle->m_vAccel.x == 0) {
				fvel *= 3;
			}
			pParticle->m_vPos += pParticle->m_vOldPos * fvel;
			pParticle->m_vOldPos += pPart->_vAccel * *pPart->_pfDailTime;

			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);
		_ret:
			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y, pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
				if (pPart->_bUseBone) {
					D3DXMatrixIdentity(&pParticle->m_SCurMat);
				}
			}
		}
	}
}

bool _CreateRound(CMPPartSys* pPart, CMPParticle* pCtrl) {
	float angle = (float)(D3DX_PI * 2) / pPart->_iParNum;
	D3DXVECTOR3 dir(0, pPart->_fRange[1], 0);
	D3DXVECTOR4 pos(0, 0, 0, 0);
	D3DXMATRIX mat;
	CMPParticle* pParticle;
	for (int n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];

		D3DXMatrixRotationZ(&mat, angle * n);
		D3DXVec3Transform(&pos, &dir, &mat);

		pParticle->m_vOldPos.x = pos.x;
		pParticle->m_vOldPos.y = pos.y;
		pParticle->m_vOldPos.z = pos.z;

		pParticle->m_bLive = true;
		pParticle->m_fLife = pPart->_fLife;

		pParticle->m_fFrameTime = pParticle->m_fLife / pPart->_wFrameCount;

		pParticle->m_fCurTime = 0;
		pParticle->m_wCurFrame = 0;

		pParticle->m_fPartTime = ((float)Rand(100)) / 1000.0f;
	}
	pPart->_wDeath = 0;
	pPart->_fCurTime = 0;
	return true;
}

void _FrameMoveRound(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;
	D3DXVECTOR4 pos(0, 0, 0, 0);
	D3DXMATRIX mat;

	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}

	pPart->_fCurTime += pPart->_fVecl * *pPart->_pfDailTime;
	if (pPart->_fCurTime >= 6.283185f)
		pPart->_fCurTime = pPart->_fCurTime - 6.283185f;
	D3DXMatrixRotationZ(&mat, pPart->_fCurTime);

	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				pParticle->m_bLive = true;
				pParticle->m_fCurTime = 0;
				pParticle->m_wCurFrame = 0;
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = 0;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;

			D3DXVec3Transform(&pos, &pParticle->m_vOldPos, &mat);
			pParticle->m_vPos.x = pos.x;
			pParticle->m_vPos.y = pos.y;
			pParticle->m_vPos.z = pos.z;
			D3DXVECTOR3 vt = pPart->_vPos + D3DXVECTOR3(pPart->_fRange[0] / 2, pPart->_fRange[1] / 2,
														pPart->_fRange[2] / 2);;
			pParticle->m_vPos += vt;


			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y, pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
				if (pPart->_bUseBone) {
					D3DXMatrixIdentity(&pParticle->m_SCurMat);
				}
			}
		}
	}
}

bool _CreateArraw(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_vecBone.resize(256);
	pPart->_iParNum = 1;

	pCtrl->m_bLive = true;

	pCtrl->m_fLife = pPart->_fLife;

	pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;

	pCtrl->m_fCurTime = 0;
	pCtrl->m_wCurFrame = 0;

	return true;
}

void _FrameMoveArraw(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;

	pParticle = pPart->_vecParticle[0];
	pParticle->m_vPos = pPart->_vPos + D3DXVECTOR3(pPart->_fRange[0] / 2,
												   pPart->_fRange[1] / 2, pPart->_fRange[2] / 2);

	wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
	if (wCurFrame == pPart->_wFrameCount) {
		_CreateArraw(pPart, pParticle);
		return;
	}
	else {
		if (wCurFrame == pPart->_wFrameCount - 1)
			wNextFrame = wCurFrame;
		else
			wNextFrame = wCurFrame + 1;
	}
	fLerp = pParticle->GetLerpValue();
	pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
		(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;
	D3DXColorLerp(&pParticle->m_SCurColor,
				  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

	if (pPart->_pCModel) {
		D3DXMatrixScaling(&pParticle->m_SCurMat,
						  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

		pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
		pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
		pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
		if (pPart->_bBillBoard)
			D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
							   &pParticle->m_SCurMat);
	}
	else {
		if (pPart->_CPPart) {
			pPart->_CPPart->FrameMove(dwDailTime);
		}
	}
}

bool _CreateModel(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_iParNum = 1;

	pCtrl->m_bLive = true;

	pCtrl->m_fLife = pPart->_fLife;

	pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;

	pCtrl->m_fCurTime = 0;
	pCtrl->m_wCurFrame = 0;

	return true;
}

void _FrameMoveModel(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;

	pParticle = pPart->_vecParticle[0];
	pParticle->m_vPos = pPart->_vPos + D3DXVECTOR3(pPart->_fRange[0] / 2,
												   pPart->_fRange[1] / 2, pPart->_fRange[2] / 2);

	wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
	if (wCurFrame == pPart->_wFrameCount) {
		//	return;
		_CreateModel(pPart, pParticle);
		return;
	}
	else {
		if (wCurFrame == pPart->_wFrameCount - 1)
			wNextFrame = wCurFrame;
		else
			wNextFrame = wCurFrame + 1;
	}
	fLerp = pParticle->GetLerpValue();
	pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
		(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;
	D3DXColorLerp(&pParticle->m_SCurColor,
				  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

	if (pPart->_pCModel) {
		D3DXMatrixScaling(&pParticle->m_SCurMat,
						  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

		pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
		pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
		pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
		if (pPart->_bBillBoard)
			D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
							   &pParticle->m_SCurMat);
		else {
			D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
						 pPart->_vecFrameAngle[wNextFrame], fLerp);
			D3DXMATRIX tm;
			D3DXMatrixRotationYawPitchRoll(&tm,
										   pParticle->m_vCurAngle.y, pParticle->m_vCurAngle.x,
										   pParticle->m_vCurAngle.z);
			D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
							   &pParticle->m_SCurMat);

			if (pPart->_bUseBone) {
				D3DXMatrixMultiply(&pParticle->m_SCurMat, &pPart->_SBoneMat,
								   &pParticle->m_SCurMat);
			}
		}
	}
	else {
		D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
					 pPart->_vecFrameAngle[wNextFrame], fLerp);
		if (pPart->_bUseBone) {
			pParticle->m_SCurMat = pPart->_SBoneMat;
		}
		if (pPart->_CPPart) {
			pPart->_CPPart->FrameMove(dwDailTime);
			if (!pPart->_CPPart->IsPlay()) {
				pPart->_bPlay = false;
				return;
			}
		}
	}
}

bool _CreateStrip(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_iParNum = 1;
	pCtrl->m_bLive = true;
	return true;
}

void _FrameMoveStrip(CMPPartSys* pPart, DWORD dwDailTime) {
	CMPParticle* pParticle;

	pParticle = pPart->_vecParticle[0];
	pParticle->m_vPos = pPart->_vPos;
	pParticle->m_SCurColor = *pPart->_vecFrameColor[0];

	if (pPart->_pCModel) {
		float fsize = *pPart->_vecFrameSize[0];
		D3DXMatrixScaling(&pParticle->m_SCurMat, fsize, fsize, fsize);

		pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
		pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
		pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
		if (pPart->_bBillBoard)
			D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard, &pParticle->m_SCurMat);
		else {
			pParticle->m_vCurAngle = *pPart->_vecFrameAngle[0];
			D3DXMATRIX tm;
			D3DXMatrixRotationYawPitchRoll(&tm,
										   pParticle->m_vCurAngle.y, pParticle->m_vCurAngle.x,
										   pParticle->m_vCurAngle.z);
			D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
							   &pParticle->m_SCurMat);
			if (pPart->_bUseBone) {
				D3DXMatrixMultiply(&pParticle->m_SCurMat, &pPart->_SBoneMat,
								   &pParticle->m_SCurMat);
			}
		}
	}
	else {
		pParticle->m_fSize = *pPart->_vecFrameSize[0];
		pParticle->m_vCurAngle = *pPart->_vecFrameAngle[0];

		if (pPart->_bUseBone) {
			pParticle->m_SCurMat = pPart->_SBoneMat;
		}
		if (pPart->_CPPart) {
			pPart->_CPPart->FrameMove(dwDailTime);
			if (!pPart->_CPPart->IsPlay()) {
				pPart->_bPlay = false;
				return;
			}
		}
	}
}

bool _CreateWind(CMPPartSys* pPart, CMPParticle* pCtrl) {
	//float fFrameTime = pParticle->m_fLife / pPart->_wFrameCount;//

	//	pParticle->m_fLife		=   Randf(pPart->_fVecl);//pPart->_fLife;//


	pPart->_fCurTime += *pPart->_pfDailTime;

	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;

		pCtrl->m_vPos = pPart->_vPos + D3DXVECTOR3(Randf(pPart->_fRange[0]),
												   Randf(pPart->_fRange[1]), Randf(pPart->_fRange[2]));
		pCtrl->m_vOldPos = pCtrl->m_vPos;
		pCtrl->m_vAccel = D3DXVECTOR3(0, 0, 0);

		pCtrl->m_bLive = true;
		pCtrl->m_fLife = pPart->_fLife;

		pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;

		pCtrl->m_vVel.z = pPart->_fVecl;

		pCtrl->m_fCurTime = 0;
		pCtrl->m_wCurFrame = 0;

		pCtrl->m_fPartTime = ((float)Rand(100)) / 1000.0f;
		return true;
	}
	return false;
}

void _FrameMoveWind(CMPPartSys* pPart, DWORD dwDailTime) {
	//	//size
	//	//
	//		int icurp = m * iNum + n;//
	//		//


	//			//D3DXColorLerp( &pParticle->m_SCurColor,

	//				D3DXMatrixScaling(&pParticle->m_SCurMat,

	//				D3DXMatrixMultiply(&pParticle->m_SCurMat,


	//			//D3DXVec3Lerp(&pParticle->m_vCurAngle,pPart->_vecFrameAngle[wCurFrame],

	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;


	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}
	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				if (pPart->_bStop) {
					pPart->_wDeath++;
					if (pPart->_wDeath == pPart->_iParNum) {
						pPart->_bPlay = false;
						pPart->_wDeath = 0;
						pPart->Reset(false);
						return;
					}
				}
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;
			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			pParticle->m_vAccel.y += pPart->_vDir.y * *pPart->_pfDailTime;
			pParticle->m_vAccel.z += pPart->_vDir.z * *pPart->_pfDailTime;

			D3DXMATRIX tm;
			D3DXVECTOR4 tpos;

			float* fz = &pParticle->m_vCurAngle.z;
			*fz += pParticle->m_vVel.z * *pPart->_pfDailTime;
			if (*fz >= 6.283185f)
				*fz = *fz - 6.283185f;
			D3DXMatrixRotationZ(&tm, *fz);
			D3DXVec3Transform(&tpos, &pParticle->m_vAccel, &tm);
			pParticle->m_vPos = (D3DXVECTOR3)tpos + pParticle->m_vOldPos;

			if (pPart->_pCModel) {
				//	D3DXMatrixScaling(&tm,
				{
					D3DXMatrixScaling(&pParticle->m_SCurMat,
									  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);
				}
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat,
									   pPart->_SpmatBBoard, &pParticle->m_SCurMat);

				//GetMatrixRotation(&tm, &pParticle->m_vPos,\
				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm, &pParticle->m_SCurMat);
			}
			else {
				//D3DXVec3Lerp(&pParticle->m_vCurAngle,pPart->_vecFrameAngle[wCurFrame],
			}
		}
		else {
			if (!pPart->_bStop)
				_CreateWind(pPart, pParticle);
		}
	}
}

bool _CreateFire(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_fCurTime += *pPart->_pfDailTime;

	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;


		D3DXVec3Normalize(&pCtrl->m_vOldPos, &pPart->_vDir);
		if (pPart->_bUseBone) {
			pPart->_vPos.x = pPart->_SBoneMat._41;
			pPart->_vPos.y = pPart->_SBoneMat._42;
			pPart->_vPos.z = pPart->_SBoneMat._43;
		}
		if (!pPart->_bModelRange) {
			pCtrl->m_vPos = pPart->_vPos + D3DXVECTOR3(Randf(pPart->_fRange[0]),
													   Randf(pPart->_fRange[1]), Randf(pPart->_fRange[2]));
		}
		else {
			pCtrl->m_vPos = pPart->_vPos + pPart->_vecPointRange[Rand(pPart->_wVecNum)];
		}
		pCtrl->m_bLive = true;
		pCtrl->m_fLife = Randf(pPart->_fLife / pPart->_iRoadom, pPart->_fLife);

		pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;

		pCtrl->m_fCurTime = 0;
		pCtrl->m_wCurFrame = 0;

		pCtrl->m_fPartTime = ((float)Rand(100)) / 1000.0f;

		if (pPart->_bModelDir && pPart->_CPPart) {
			pCtrl->m_vOldPos.x = pPart->_vTemDir.x;
			pCtrl->m_vOldPos.y = pPart->_vTemDir.y;
		}

		if (pPart->_CPPart) {
			if (pPart->_bMediay) {
				pPart->_CPPart[pPart->_idt].Reset();
				pPart->_CPPart[pPart->_idt].Play(0);
			}
		}
		return true;
	}
	return false;
}

void _FrameMoveFire(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;

	if (pPart->_bStop == false) {
		if (pPart->m_strHitEff != "") {
			if (pPart->m_pMap) {
				if (pPart->_pcPath) {
					if (pPart->_pcPath->IsEnd()) {
						D3DXVECTOR3 VPos = pPart->_vPos;
						VPos.z = pPart->m_pMap->GetGridHeight((int)VPos.x * 2, (int)VPos.y * 2);
						if (VPos.z < 0)
							VPos.z = 0.1f;
						pPart->Stop();
						pPart->m_pCResMagr->SendResMessage(pPart->m_strHitEff, VPos, pPart->m_pMap);
					}
				}
				else {
					pPart->Stop();
				}
			}
			else {
				if (pPart->_vPos.z <= 0.1f || pPart->_vPos.z > 50.0f) {
					pPart->Stop();
					pPart->m_pCResMagr->SendResMessage(pPart->m_strHitEff, *pPart->_pcPath->GetEnd(),NULL);
				}
			}
		}
	}

	if (pPart->_CPPart) {
		if (pPart->_bMediay) {
			for (WORD n = 0; n < pPart->_iParNum; ++n)
				pPart->_CPPart[n].FrameMove(dwDailTime);
		}
		else
			pPart->_CPPart->FrameMove(dwDailTime);
	}

	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				if (pPart->_bStop) {
					pPart->_wDeath++;
					if (pPart->_wDeath >= pPart->_iParNum) {
						pPart->_bPlay = false;
						return;
					}
				}
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;


			pParticle->m_vVel = pParticle->m_vOldPos * (pPart->_fVecl * *pPart->_pfDailTime);
			if (rand() % 2)
				pParticle->m_vAccel = (pPart->_vAccel * *pPart->_pfDailTime);
			else
				pParticle->m_vAccel = -(pPart->_vAccel * *pPart->_pfDailTime);

			pParticle->m_vVel += pParticle->m_vAccel;

			pParticle->m_vPos += pParticle->m_vVel;
			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y,
												   pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);

				{
				}
			}
		}
		else {
			if (!pPart->_bStop) {
				pPart->_idt = n;
				_CreateFire(pPart, pParticle);
			}
		}
	}
}

bool _CreateSnow(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_fCurTime += *pPart->_pfDailTime;

	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;

		if (pPart->_iRoadom == 1)
			pCtrl->m_vOldPos = D3DXVECTOR3(pPart->_fVecl, pPart->_fVecl, pPart->_fVecl);
		else {
			float flerp = pPart->_fVecl / pPart->_iRoadom;
			pCtrl->m_vOldPos = D3DXVECTOR3(Randf(flerp, pPart->_fVecl), Randf(flerp, pPart->_fVecl),
										   Randf(flerp, pPart->_fVecl));
		}
		pCtrl->m_vAccel = pPart->_vAccel; //;
		pCtrl->m_vOldPos.x *= Rand(2) ? pPart->_vDir.x : -pPart->_vDir.x;
		pCtrl->m_vOldPos.y *= Rand(2) ? pPart->_vDir.y : -pPart->_vDir.y;
		pCtrl->m_vOldPos.z *= pPart->_vDir.z;
		if (!(pCtrl->m_vOldPos.z != 0.0f))
			return false;

		if (!pPart->_bModelRange) {
			pCtrl->m_vPos = pPart->_vPos + D3DXVECTOR3(Randf(pPart->_fRange[0]),
													   Randf(pPart->_fRange[1]), Randf(pPart->_fRange[2]));
		}
		else {
			pCtrl->m_vPos = pPart->_vPos + pPart->_vecPointRange[Rand(pPart->_wVecNum)];
		}
		pCtrl->m_bLive = true;
		pCtrl->m_fLife = Randf(pPart->_fLife / pPart->_iRoadom, pPart->_fLife);

		pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;

		pCtrl->m_fCurTime = 0;
		pCtrl->m_wCurFrame = 0;
		pCtrl->m_fPartTime = ((float)Rand(100)) / 1000.0f;

		if (pPart->_CPPart) {
			if (pPart->_bMediay) {
				pPart->_CPPart[pPart->_idt].Reset();
				pPart->_CPPart[pPart->_idt].Play(0);
			}
		}
		return true;
	}
	return false;
}

void _FrameMoveSnow(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;

	if (pPart->_CPPart) {
		if (pPart->_bMediay) {
			for (WORD n = 0; n < pPart->_iParNum; ++n)
				pPart->_CPPart[n].FrameMove(dwDailTime);
		}
		else
			pPart->_CPPart->FrameMove(dwDailTime);
	}

	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				if (pPart->_bStop) {
					pPart->_wDeath++;
					if (pPart->_wDeath >= pPart->_iParNum) {
						pPart->_bPlay = false;
						return;
					}
				}
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;


			pParticle->m_vVel = pParticle->m_vOldPos * *pPart->_pfDailTime;
			if (rand() % 2)
				pParticle->m_vVel += (pParticle->m_vAccel * *pPart->_pfDailTime);
			else
				pParticle->m_vVel -= (pParticle->m_vAccel * *pPart->_pfDailTime);

			pParticle->m_vPos += pParticle->m_vVel;

			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y, pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
				if (pPart->_bUseBone) {
					D3DXMatrixIdentity(&pParticle->m_SCurMat);
				}
			}
		}
		else {
			if (!pPart->_bStop) {
				pPart->_idt = n;
				_CreateSnow(pPart, pParticle);
			}
		}
	}
}

//-----------------------------------------------------------------------------
bool _CreateBlast(CMPPartSys* pPart, CMPParticle* pCtrl) {
	CMPParticle* pParticle;
	for (int n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		pParticle->m_vOldPos = D3DXVECTOR3(Randf(pPart->_fVecl),
										   -Randf(pPart->_fVecl), Randf(pPart->_fVecl));
		pParticle->m_vAccel = pPart->_vAccel; //;
		pParticle->m_vOldPos.x *= Rand(2) ? pPart->_vDir.x : -pPart->_vDir.x;
		pParticle->m_vOldPos.y *= Rand(2) ? pPart->_vDir.y : -pPart->_vDir.y;
		pParticle->m_vOldPos.z *= pPart->_vDir.z;

		if (!pPart->_bModelRange) {
			pParticle->m_vPos = pPart->_vPos + D3DXVECTOR3(Randf(pPart->_fRange[0]),
														   Randf(pPart->_fRange[1]), Randf(pPart->_fRange[2]));
		}
		else {
			pParticle->m_vPos = pPart->_vPos + pPart->_vecPointRange[Rand(pPart->_wVecNum)];
		}
		pParticle->m_bLive = true;
		pParticle->m_fLife = Randf(pPart->_fLife / pPart->_iRoadom, pPart->_fLife);

		pParticle->m_fFrameTime = pParticle->m_fLife / pPart->_wFrameCount;

		pParticle->m_fCurTime = 0;
		pParticle->m_wCurFrame = 0;
		pParticle->m_fPartTime = -1;
	}
	pPart->_wDeath = 0;
	return true;
}

void _FrameMoveBlast(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;

	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}

	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				pPart->_wDeath++;
				if (pPart->_wDeath == pPart->_iParNum) {
					if (pPart->_bLoop) {
						_CreateBlast(pPart,NULL);
					}
					else
						pPart->_bPlay = false;
				}
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;


			pParticle->m_vVel = pParticle->m_vOldPos * *pPart->_pfDailTime;

			pParticle->m_vPos += pParticle->m_vVel;
			pParticle->m_vOldPos += pParticle->m_vAccel * *pPart->_pfDailTime;

			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				D3DXMatrixScaling(&pParticle->m_SCurMat,
								  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

				pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
				pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
				pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat, pPart->_SpmatBBoard,
									   &pParticle->m_SCurMat);
				else {
					D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
								 pPart->_vecFrameAngle[wNextFrame], fLerp);
					D3DXMATRIX tm;
					D3DXMatrixRotationYawPitchRoll(&tm,
												   pParticle->m_vCurAngle.y, pParticle->m_vCurAngle.x,
												   pParticle->m_vCurAngle.z);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm,
									   &pParticle->m_SCurMat);
				}
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
				if (pPart->_bUseBone) {
					D3DXMatrixIdentity(&pParticle->m_SCurMat);
				}
			}
		}
	}
}

bool _CreateRipple(CMPPartSys* pPart, CMPParticle* pCtrl) {
	pPart->_fCurTime += *pPart->_pfDailTime;

	if (pPart->_fCurTime >= pPart->_fStep) {
		pPart->_fCurTime = 0;
		if (!pPart->_bModelRange) {
			pCtrl->m_vPos = pPart->_vPos + D3DXVECTOR3(Randf(pPart->_fRange[0]),
													   Randf(pPart->_fRange[1]), Randf(pPart->_fRange[2]));
		}
		else {
			pCtrl->m_vPos = pPart->_vPos + pPart->_vecPointRange[Rand(pPart->_wVecNum)];
		}
		pCtrl->m_bLive = true;
		pCtrl->m_fLife = Randf(pPart->_fLife / pPart->_iRoadom, pPart->_fLife);

		pCtrl->m_fFrameTime = pCtrl->m_fLife / pPart->_wFrameCount;

		if (pPart->_bUseBone)
			pCtrl->m_SBoneMat = pPart->_SBoneMat; //Z
		pCtrl->m_fCurTime = 0;
		pCtrl->m_wCurFrame = 0;

		pCtrl->m_fPartTime = ((float)Rand(100)) / 1000.0f;

		return true;
	}
	return false;
}

void _FrameMoveRipple(CMPPartSys* pPart, DWORD dwDailTime) {
	WORD wCurFrame;
	WORD wNextFrame;
	float fLerp;
	CMPParticle* pParticle;


	if (pPart->_wDeath >= pPart->_iParNum)
		return;
	if (pPart->_CPPart) {
		pPart->_CPPart->FrameMove(dwDailTime);
	}
	for (WORD n = 0; n < pPart->_iParNum; ++n) {
		pParticle = pPart->_vecParticle[n];
		if (pParticle->m_bLive) {
			wCurFrame = pParticle->GetCurFrame(*pPart->_pfDailTime, pPart->_wFrameCount);
			if (wCurFrame == pPart->_wFrameCount) {
				if (pPart->_bStop) {
					pPart->_wDeath++;
					if (pPart->_wDeath == pPart->_iParNum) {
						pPart->Reset(false);
						return;
					}
				}
				continue;
			}
			else {
				if (wCurFrame == pPart->_wFrameCount - 1)
					wNextFrame = wCurFrame;
				else
					wNextFrame = wCurFrame + 1;
			}
			fLerp = pParticle->GetLerpValue();
			pParticle->m_fSize = *pPart->_vecFrameSize[wCurFrame] +
				(*pPart->_vecFrameSize[wNextFrame] - *pPart->_vecFrameSize[wCurFrame]) * fLerp;
			D3DXColorLerp(&pParticle->m_SCurColor,
						  pPart->_vecFrameColor[wCurFrame], pPart->_vecFrameColor[wNextFrame], fLerp);

			if (pPart->_pCModel) {
				if (pPart->_bUseBone) {
					D3DXMATRIX tm;
					D3DXMatrixScaling(&tm,
									  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);
					D3DXMatrixMultiply(&pParticle->m_SCurMat, &tm, &pParticle->m_SBoneMat);
				}
				else {
					D3DXMatrixScaling(&pParticle->m_SCurMat,
									  pParticle->m_fSize, pParticle->m_fSize, pParticle->m_fSize);

					pParticle->m_SCurMat._41 = pParticle->m_vPos.x;
					pParticle->m_SCurMat._42 = pParticle->m_vPos.y;
					pParticle->m_SCurMat._43 = pParticle->m_vPos.z;
				}
				if (pPart->_bBillBoard)
					D3DXMatrixMultiply(&pParticle->m_SCurMat,
									   pPart->_SpmatBBoard, &pParticle->m_SCurMat);
			}
			else {
				D3DXVec3Lerp(&pParticle->m_vCurAngle, pPart->_vecFrameAngle[wCurFrame],
							 pPart->_vecFrameAngle[wNextFrame], fLerp);
				if (pPart->_bUseBone) {
					pParticle->m_SCurMat = pPart->_SBoneMat;
				}
			}
		}
		else {
			if (!pPart->_bStop)
				_CreateRipple(pPart, pParticle);
		}
	}
}

// CMPPartSys::SaveToFile / LoadFromFile перенесены в
// Corsairs::Engine::Render::PartCtrlLoader::{Save,Load}PartSys.
#if 0
bool CMPPartSys::SaveToFile(FILE* t_pFile) {
	int tm;

	fwrite(&_iType, sizeof(int), 1, t_pFile);

	// Имена в .par-файле — фиксированный 32-байтный буфер.
	auto writeFixedName = [&](std::string_view src) {
		char t_pszName[32]{};
		std::memcpy(t_pszName, src.data(),
					std::min<std::size_t>(src.size(), sizeof(t_pszName) - 1));
		fwrite(t_pszName, sizeof(char), 32, t_pFile);
	};
	writeFixedName(_strPartName);

	fwrite(&_iParNum, sizeof(int), 1, t_pFile);

	writeFixedName(_strTexName);
	writeFixedName(_strModelName);

	fwrite(&_fRange[0], sizeof(float), 1, t_pFile);
	fwrite(&_fRange[1], sizeof(float), 1, t_pFile);
	fwrite(&_fRange[2], sizeof(float), 1, t_pFile);

	fwrite(&_wFrameCount, sizeof(WORD), 1, t_pFile);

	for (WORD n = 0; n < _wFrameCount; n++) {
		fwrite(_vecFrameSize[n], sizeof(float), 1, t_pFile);
	}
	for (WORD n = 0; n < _wFrameCount; n++) {
		fwrite(_vecFrameAngle[n], sizeof(D3DXVECTOR3), 1, t_pFile);
	}
	for (WORD n = 0; n < _wFrameCount; n++) {
		fwrite(_vecFrameColor[n], sizeof(D3DXCOLOR), 1, t_pFile);
	}
	fwrite(&_bBillBoard, sizeof(bool), 1, t_pFile);

	tm = (int)_eSrcBlend;
	fwrite(&tm, sizeof(int), 1, t_pFile);
	tm = (int)_eDestBlend;
	fwrite(&tm, sizeof(int), 1, t_pFile);
	tm = (int)_eMinFilter;
	fwrite(&tm, sizeof(int), 1, t_pFile);
	tm = (int)_eMagFilter;
	fwrite(&tm, sizeof(int), 1, t_pFile);

	fwrite(&_fLife, sizeof(float), 1, t_pFile);
	fwrite(&_fVecl, sizeof(float), 1, t_pFile);
	fwrite(&_vDir, sizeof(D3DXVECTOR3), 1, t_pFile);
	fwrite(&_vAccel, sizeof(D3DXVECTOR3), 1, t_pFile);
	fwrite(&_fStep, sizeof(float), 1, t_pFile);

	fwrite(&_bModelRange, sizeof(bool), 1, t_pFile);
	writeFixedName(_strVirualModel);

	fwrite(&_vOffset, sizeof(D3DXVECTOR3), 1, t_pFile);
	fwrite(&_fDelayTime, sizeof(float), 1, t_pFile);
	fwrite(&_fPlayTime, sizeof(float), 1, t_pFile);

	bool busepsth = _pcPath ? true : false;
	fwrite(&busepsth, sizeof(bool), 1, t_pFile);
	if (busepsth)
		_pcPath->SavePath(t_pFile);

	fwrite(&m_bShade, sizeof(bool), 1, t_pFile);

	writeFixedName(m_strHitEff);

	if (_bModelRange) {
		fwrite(&_wVecNum, sizeof(WORD), 1, t_pFile);
		fwrite(&_vecPointRange.front(), sizeof(D3DXVECTOR3), _wVecNum, t_pFile);
	}
	fwrite(&_iRoadom, sizeof(int), 1, t_pFile);
	fwrite(&_bModelDir, sizeof(bool), 1, t_pFile);

	fwrite(&_bMediay, sizeof(bool), 1, t_pFile);

	return true;
}

bool CMPPartSys::LoadFromFile(FILE* t_pFile, DWORD dwVersion) {
	int tm;

	fread(&_iType, sizeof(int), 1, t_pFile);

	char t_pszName[32];
	fread(t_pszName, sizeof(char), 32, t_pFile);
	_strPartName = t_pszName;

	fread(&_iParNum, sizeof(int), 1, t_pFile);
	_vecParticle.setsize(_iParNum);

	fread(t_pszName, sizeof(char), 32, t_pFile);

	s_string sFileName = t_pszName;
	transform(sFileName.begin(), sFileName.end(),
			  sFileName.begin(),
			  tolower);

	char psname[32];
	memset(psname, 0, 32);


	if (sFileName.rfind(".dds") == std::string::npos && sFileName.rfind(".tga") == std::string::npos) {
		_strTexName = sFileName;
	}
	else {
		_strTexName = sFileName.substr(0, sFileName.rfind('.'));
	}


	fread(t_pszName, sizeof(char), 32, t_pFile);
	if (IsDefaultMesh(t_pszName))
		_strModelName = t_pszName;
	else {
		sFileName = t_pszName;
		transform(sFileName.begin(), sFileName.end(),
				  sFileName.begin(),
				  tolower);
		_strModelName = sFileName;
	}
	fread(&_fRange[0], sizeof(float), 1, t_pFile);
	fread(&_fRange[1], sizeof(float), 1, t_pFile);
	fread(&_fRange[2], sizeof(float), 1, t_pFile);

	fread(&_wFrameCount, sizeof(WORD), 1, t_pFile);

	_vecFrameSize.setsize(_wFrameCount);
	for (WORD n = 0; n < _wFrameCount; n++) {
		fread(_vecFrameSize[n], sizeof(float), 1, t_pFile);
	}
	_vecFrameAngle.setsize(_wFrameCount);
	for (WORD n = 0; n < _wFrameCount; n++) {
		fread(_vecFrameAngle[n], sizeof(D3DXVECTOR3), 1, t_pFile);
	}
	_vecFrameColor.setsize(_wFrameCount);
	for (WORD n = 0; n < _wFrameCount; n++) {
		fread(_vecFrameColor[n], sizeof(D3DXCOLOR), 1, t_pFile);
	}
	fread(&_bBillBoard, sizeof(bool), 1, t_pFile);

	fread(&tm, sizeof(int), 1, t_pFile);
	_eSrcBlend = (D3DBLEND)tm;
	fread(&tm, sizeof(int), 1, t_pFile);
	_eDestBlend = (D3DBLEND)tm;
	fread(&tm, sizeof(int), 1, t_pFile);
	_eMinFilter = (D3DTEXTUREFILTERTYPE)tm;
	fread(&tm, sizeof(int), 1, t_pFile);
	_eMagFilter = (D3DTEXTUREFILTERTYPE)tm;

	fread(&_fLife, sizeof(float), 1, t_pFile);
	fread(&_fVecl, sizeof(float), 1, t_pFile);
	fread(&_vDir, sizeof(D3DXVECTOR3), 1, t_pFile);
	fread(&_vAccel, sizeof(D3DXVECTOR3), 1, t_pFile);
	fread(&_fStep, sizeof(float), 1, t_pFile);

	if (dwVersion > 3) {
		fread(&_bModelRange, sizeof(bool), 1, t_pFile);
		fread(t_pszName, sizeof(char), 32, t_pFile);
	}
	if (dwVersion > 4) {
		fread(&_vOffset, sizeof(D3DXVECTOR3), 1, t_pFile);
	}
	if (dwVersion > 5) {
		fread(&_fDelayTime, sizeof(float), 1, t_pFile);
		fread(&_fPlayTime, sizeof(float), 1, t_pFile);
	}
	if (dwVersion > 8) {
		SAFE_DELETE(_pcPath);
		bool busepsth;
		fread(&busepsth, sizeof(bool), 1, t_pFile);
		if (busepsth) {
			_pcPath = new CEffPath;
			_pcPath->LoadPath(t_pFile);
			_pcPath->Reset();
		}
	}
	if (dwVersion > 9) {
		fread(&m_bShade, sizeof(bool), 1, t_pFile);
	}
	if (dwVersion > 10) {
		fread(t_pszName, sizeof(char), 32, t_pFile);
		m_strHitEff = t_pszName;
	}
	if (dwVersion > 11) {
		if (_bModelRange) {
			fread(&_wVecNum, sizeof(WORD), 1, t_pFile);
			_vecPointRange.resize(_wVecNum);
			fread(&_vecPointRange.front(), sizeof(D3DXVECTOR3), _wVecNum, t_pFile);
		}
	}
	if (dwVersion > 12) {
		fread(&_iRoadom, sizeof(int), 1, t_pFile);
	}
	else {
		if (_iType == PARTTICLE_SNOW || _iType == PARTTICLE_FIRE || _iType == PARTTICLE_DUMMY)
			_iRoadom = 4;
		else
			_iRoadom = 2;
	}
	if (dwVersion > 13)
		fread(&_bModelDir, sizeof(bool), 1, t_pFile);

	if (dwVersion > 14)
		fread(&_bMediay, sizeof(bool), 1, t_pFile);

	return true;
}
#endif

void CopyPartSys(CMPPartSys* part1, CMPPartSys* part2) {
	part1->_iType = part2->_iType;

	part1->_strPartName = part2->_strPartName;

	part1->_iParNum = part2->_iParNum;
	part1->_vecParticle.setsize(part2->_iParNum);

	part1->_strTexName = part2->_strTexName;
	part1->_lpCurTex = NULL; //part2->_lpCurTex;

	part1->_strModelName = part2->_strModelName;
	part1->_pCModel = NULL; //part2->_pCModel;

	part1->_fRange[0] = part2->_fRange[0]; //!
	part1->_fRange[1] = part2->_fRange[1]; //!
	part1->_fRange[2] = part2->_fRange[2]; //!

	part1->_wFrameCount = part2->_wFrameCount;

	part1->_pdwVShader = 0L;
	part1->_pfDailTime = part2->_pfDailTime;
	part1->_pMatViewProj = part2->_pMatViewProj;

	part1->_bBillBoard = part2->_bBillBoard;
	part1->_SpmatBBoard = NULL; //part2->_SpmatBBoard;

	part1->_pCEffectFile = part2->_pCEffectFile;

	part1->m_bShade = part2->m_bShade;

	part1->SetOpertion();


	part1->_fLife = part2->_fLife;
	part1->_vDir = part2->_vDir;
	part1->_fVecl = part2->_fVecl;
	part1->_vAccel = part2->_vAccel;
	part1->_fStep = part2->_fStep;
	part1->_vecFrameSize.clear();
	part1->_vecFrameAngle.clear();
	part1->_vecFrameColor.clear();

	part1->_vecFrameSize.resize(part1->_wFrameCount);
	part1->_vecFrameAngle.resize(part1->_wFrameCount);
	part1->_vecFrameColor.resize(part1->_wFrameCount);
	for (int n = 0; n < part1->_wFrameCount; n++) {
		part1->_vecFrameSize.push_back(*part2->_vecFrameSize[n]);
		part1->_vecFrameAngle.push_back(*part2->_vecFrameAngle[n]);
		part1->_vecFrameColor.push_back(*part2->_vecFrameColor[n]);
	}
	part1->_eSrcBlend = part2->_eSrcBlend;
	part1->_eDestBlend = part2->_eDestBlend;

	part1->_eMinFilter = part2->_eMinFilter;
	part1->_eMagFilter = part2->_eMagFilter;

	part1->_fCurTime = 0;
	part1->_bPlay = false;
	part1->_bStop = true;
	part1->_bLoop = true;
	part1->_wDeath = part1->_iParNum;
	part1->_bUseBone = false;
	D3DXMatrixIdentity(&part1->_SBoneMat);

	part1->_vPos = D3DXVECTOR3(-part1->_fRange[0] / 2, -part1->_fRange[1] / 2, -part1->_fRange[2] / 2);

	part1->_bModelRange = part2->_bModelRange;
	part1->_strVirualModel = part2->_strVirualModel;

	if (part1->_bModelRange) {
		part1->_wVecNum = part2->_wVecNum;
		part1->_vecPointRange.resize(part1->_wVecNum);
		part1->_vecPointRange = part2->_vecPointRange;
	}

	part1->_vOffset = part2->_vOffset;

	part1->_fDelayTime = part2->_fDelayTime;
	part1->_fPlayTime = part2->_fPlayTime;

	if (part2->_pcPath) {
		if (!part1->_pcPath)
			part1->_pcPath = new CEffPath;
		part1->_pcPath->Copy(part2->_pcPath);
	}
	else {
		SAFE_DELETE(part1->_pcPath);
	}
	part1->m_strHitEff = part2->m_strHitEff;

	part1->_iDummy1 = -1;
	part1->_iDummy2 = -1;
	part1->_pItem = NULL;

	part1->_iRoadom = part2->_iRoadom;
	part1->_bModelDir = part2->_bModelDir;

	part1->_bMediay = part2->_bMediay;
}

//-----------------------------------------------------------------------------
void CMPPartSys::LoadPathFile(char* pszfile) {
	SAFE_DELETE(_pcPath);
	_pcPath = new CEffPath;
	_pcPath->LoadPathFromFile(pszfile);
	_pcPath->Reset();
}

//-----------------------------------------------------------------------------
void CMPPartSys::LoadPathFileLet(char* pszfile) {
	SAFE_DELETE(_pcPath);
	_pcPath = new CEffPath;
	_pcPath->LoadPathFromFileLet(pszfile);
	_pcPath->Reset();
}

//-----------------------------------------------------------------------------
void CMPPartSys::SetPathVel(float fvel) {
	if (_pcPath)
		_pcPath->SetVel(fvel);
}

float CMPPartSys::GetPathVel() {
	if (_pcPath)
		return _pcPath->GetVel();
	return 0;
}

void CMPPartSys::DeletePath() {
	SAFE_DELETE(_pcPath);
}

void CMPPartSys::SetSysNum(int iParNum) {
	if (iParNum > 100) {
		_iParNum = 100;
	}
	else if (_iParNum <= 0) {
		_iParNum = 1;
	}
	else {
		_iParNum = iParNum;
	}

	_vecParticle.setsize(_iParNum);
}

void CMPPartSys::SetFrameSize(int iFrame, float fsize, CMPResManger* pCResMagr) {
	if (iFrame < _wFrameCount) {
		*_vecFrameSize[iFrame] = fsize;
	}

	if (m_bShade) {
		m_cShade.Create(_strTexName, pCResMagr, *_vecFrameSize[0]);
	}
}

void CMPPartSys::SetAlpha(float falpha) {
	for (int n = 0; n < _wFrameCount; n++) {
		_vecFrameColor[n]->a = falpha;
	}
}
