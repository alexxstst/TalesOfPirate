#include <algorithm>

#include "StdAfx.h"

#include "GlobalInc.h"
//#include "MPModelEff.h"
#include "MPModelEff.h"

#include "i_effect.h"
#include "MPRender.h"
#include "CharacterActionStore.h"
//#include "mpresmanger.h"

extern CMPResManger ResMgr;


I_Effect::I_Effect(void) {
	m_pDev = NULL;

	ReleaseAll();

	_bBillBoard = false;
	_bSizeSame = true;
	_bAngleSame = true;
	_bPosSame = true;
	_bColorSame = true;

	_eSrcBlend = D3DBLEND_SRCALPHA;
	_eDestBlend = D3DBLEND_INVSRCALPHA;

	_iUseParam = 0;
	m_ilast = m_inext = 0;
	m_flerp = 0;

	_bRotaLoop = false;
	_vRotaLoop = D3DXVECTOR4(0, 0, 0, 0);

	_bAlpha = true;
	_bRotaBoard = true;
}

I_Effect::~I_Effect(void) {
	//ReleaseAll();
}

void I_Effect::DestroyTobMesh(CMPResManger* resMgr) {
	if (m_pCModel) {
		if (!IsTobMesh(m_strModelName))
			resMgr->DeleteMesh(*m_pCModel);
		else
			resMgr->DeleteTobMesh(*m_pCModel);
	}
}


//
void I_Effect::Init(MPRender* pDev, EFFECT_TYPE eType, WPARAM wParam, LPARAM lParam)
{
	m_pDev = pDev;
	_eEffectType = eType;
	_fLength = 5.0f; //!2
	_wFrameCount = (WORD)wParam; //!

	_vecFrameTime.resize(_wFrameCount);

	_vecFrameSize.resize(_wFrameCount);
	_vecFrameAngle.resize(_wFrameCount);
	_vecFramePos.resize(_wFrameCount);
	_vecFrameColor.resize(_wFrameCount);

	_iUseParam = 0;
	_CylinderParam.resize(_wFrameCount);

	for (WORD n = 0; n < _wFrameCount; n++) {
		_vecFrameTime[n] = _fLength / _wFrameCount;

		_vecFrameSize[n] = //D3DXVECTOR3((rand()%100)/10,(rand()%100)/10,1.0f);//*/
			D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		_vecFrameAngle[n] = /*D3DXVECTOR4(0,1,1,(rand()%3));//*/
			D3DXVECTOR3(0, 0, 0);
		_vecFramePos[n] = /*D3DXVECTOR3(0,(rand()%5),-(rand()%5));//*/
			D3DXVECTOR3(0, 0, 0);

		_vecFrameColor[n] = /*D3DCOLOR_ARGB((int)(255 - 255/(n + 1)),255,255,255);
		//*/D3DCOLOR_ARGB(255, 255, 255, 255);
	}
	//!VS
	_bBillBoard = (bool)lParam;
	if (_bBillBoard)
		_iVSIndex = 2;
	if (_eEffectType == EFFECT_MODEL)
		_iVSIndex = 1;

	//_vecFrameTime[0]	= 1.75f;
	//_vecFrameTime[1]	= 1.25f;
	//_vecFrameTime[2]	= 2.00f;
	////_vecFrameTime[3]	= 1.5f;

	//_vecFrameColor[0]	=D3DCOLOR_ARGB(0,255,255,255);
	//_vecFrameColor[1]	=D3DCOLOR_ARGB(0,255,255,255);
	//_vecFrameColor[2]	=D3DCOLOR_ARGB(255,255,255,255);
	////_vecFrameColor[3]	=D3DCOLOR_ARGB(200,255,255,255);

	//m_pCModel			= (CEffectModel*)lParam;

	//m_strModelName		= m_pCModel->m_strName;

	//m_CTexCoordlist.GetCoordFromModel(m_pCModel);
	//m_CTextruelist.GetTextureFromModel(m_pCModel);
}

void I_Effect::Reset() {
	m_CTexCoordlist.Reset();
	m_CTextruelist.Reset();
}

void I_Effect::ReleaseAll() {
	_eEffectType = EFFECT_NONE;
	_fLength = 0.0f;
	_wFrameCount = 0;

	_vecFrameTime.clear();
	_vecFrameSize.clear();
	_vecFrameAngle.clear();
	_vecFramePos.clear();
	_vecFrameColor.clear();

	m_CTexCoordlist.Clear();
	m_CTextruelist.Clear();

	_CylinderParam.clear();
	m_pCModel = NULL;
	m_strModelName = "";

	m_strEffectName = "";
	_SpmatBBoard = NULL;

	_iVSIndex = 0;

	//SAFE_RELEASE(m_oldtex);
}

void I_Effect::BindingResInit(CMPResManger* m_CResMagr) {
	if (_eEffectType == EFFECT_FRAMETEX) {
	}
	else if (m_pCModel) {
		m_CTexCoordlist.GetCoordFromModel(m_pCModel);
		m_CTextruelist.GetTextureFromModel(m_pCModel);
		if (IsCylinderMesh(m_pCModel->m_strName)) {
			_iUseParam = 0;

			for (int n = 0; n < _wFrameCount; ++n) {
				_CylinderParam[n].iSegments = m_nSegments;
				_CylinderParam[n].fTopRadius = m_rRadius;
				_CylinderParam[n].fBottomRadius = m_rBotRadius;
				_CylinderParam[n].fHei = m_rHeight;
				_CylinderParam[n].Create();
			}
		}
	}
}

//-----------------------------------------------------------------------------
int I_Effect::BoundingRes(CMPResManger* m_CResMagr, const char* pszParentName) {
	//!0123shade
	int t_iID = 0;

	//
	if (_eEffectType == EFFECT_FRAMETEX) {
		for (WORD n = 0; n < m_CTexFrame.m_wTexCount; ++n) {
			m_CTexFrame.m_vecTexs[n] =
				m_CResMagr->GetTextureByNamelw(m_CTexFrame.m_vecTexName[n]);
			if (!m_CTexFrame.m_vecTexs[n]) {
				char pszMsg[64];
				sprintf(pszMsg, "[%s][%s]",
						pszParentName, m_CTextruelist.m_vecTexName.c_str());
				g_logManager.InternalLog(LogLevel::Error, "errors", pszMsg);
				return 1;
			}
		}
	}
	else {
		m_CTextruelist.m_pTex =
			m_CResMagr->GetTextureByNamelw(m_CTextruelist.m_vecTexName);
		if (!m_CTextruelist.m_pTex) {
			char pszMsg[64];
			sprintf(pszMsg, "[%s][%s]",
					pszParentName, m_CTextruelist.m_vecTexName.c_str());
			g_logManager.InternalLog(LogLevel::Error, "errors", pszMsg);
			return 1;
		}
		m_CTextruelist.m_lpCurTex = m_CTextruelist.m_pTex->GetTex();
	}


	_SpmatBBoard = m_CResMagr->GetBBoardMat();

	//
	if (m_pCModel && m_pCModel->m_strName == m_strModelName) {
		return 0;
	}
	if (m_pCModel) {
		if (!IsTobMesh(m_strModelName)) {
			m_CResMagr->DeleteMesh(*m_pCModel);
			m_pCModel = 0;
		}
		else {
			m_CResMagr->DeleteTobMesh(*m_pCModel);
			m_pCModel = 0;
		}
	}


	if (!_bBillBoard && IsTobMesh(m_strModelName)) {
		if (!m_pCModel) {
			m_pCModel = m_CResMagr->NewTobMesh();
			if (!m_pCModel->CreateTob(m_strModelName, m_nSegments, m_rHeight, m_rRadius, m_rBotRadius)) {
				char pszMsg[64];
				sprintf(pszMsg, "[%s][%s]",
						pszParentName, m_strModelName.c_str());
				g_logManager.InternalLog(LogLevel::Error, "errors", pszMsg);
				return 2;
			}
		}
	}
	else {
		m_pCModel = m_CResMagr->GetMeshByName(m_strModelName);
		if (!m_pCModel) {
			char pszMsg[64];
			sprintf(pszMsg, "[%s][%s]",
					pszParentName, m_CTextruelist.m_vecTexName.c_str());
			g_logManager.InternalLog(LogLevel::Error, "errors", pszMsg);
			return 2;
		}

		if (_bBillBoard) {
			m_strModelName = MESH_PLANERECT;
		}
		else {
			if (_eEffectType == EFFECT_FRAMETEX)
				m_CTexFrame.GetCoordFromModel(m_pCModel);
		}
	}
	//if(IsItem())
	//{
	//		//char pszn[128];
	//		//sprintf(pszn, "texture\\effect\\%s",m_CTextruelist.m_vecTexName.c_str());
	//		//m_pCModel->ResetItemTexture(pszn);
	//}

	// Success
	return 0;
}

//-----------------------------------------------------------------------------
void I_Effect::SetModel(CEffectModel* pCModel) {
	if (m_pCModel && !IsTobMesh(m_pCModel->m_strName)) {
		m_pCModel->SetUsing(false);
		m_pCModel = 0;
	}
	m_pCModel = pCModel;
	m_strModelName = m_pCModel->m_strName;
}

//-----------------------------------------------------------------------------
void I_Effect::ResetModel() {
	if (m_pCModel->IsItem()) {
		m_pCModel->SetUsing(false);
		m_pCModel = 0;
	}
}

void I_Effect::DeleteItem(CMPResManger* pResMgr) {
	if (m_pCModel)
		if (!IsTobMesh(m_strModelName)) {
			pResMgr->DeleteMesh(*m_pCModel);
			m_pCModel = 0;
		}
		else {
			pResMgr->DeleteTobMesh(*m_pCModel);
			m_pCModel = 0;
		}
}

void I_Effect::GetRes(CMPResManger* pCResMagr, std::vector<INT>& vecTex, std::vector<INT>& vecModel) {
	int t_iID = 0;
	std::vector<INT>::iterator it;

	if (_eEffectType == EFFECT_FRAMETEX) {
		for (WORD n = 0; n < m_CTexFrame.m_wTexCount; ++n) {
			t_iID = pCResMagr->GetTextureID(m_CTexFrame.m_vecTexName[n]);
			if (t_iID != -1) {
				it = std::find(vecTex.begin(), vecTex.end(), t_iID);
				if (it == vecTex.end()) {
					vecTex.push_back(t_iID);
				}
			}
		}
	}
	else {
		t_iID = pCResMagr->GetTextureID(m_CTextruelist.m_vecTexName);
		if (t_iID != -1) {
			it = std::find(vecTex.begin(), vecTex.end(), t_iID);
			if (it == vecTex.end()) {
				vecTex.push_back(t_iID);
			}
		}
	}

	if (_bBillBoard) {
	}
	else {
		if (IsTobMesh(m_strModelName)) {
		}
		else {
			t_iID = pCResMagr->GetMeshID(m_strModelName);
			if (t_iID >= 7) {
				it = std::find(vecModel.begin(), vecModel.end(), t_iID);
				if (it == vecModel.end()) {
					vecModel.push_back(t_iID);
				}
			}
		}
	}
}

void I_Effect::ChangeModel(CEffectModel* pCModel, CMPResManger* pCResMagr) {
	if (!pCModel)
		return;

	if (_bBillBoard) {
		if (pCModel->m_strName != MESH_PLANERECT)
			return;
	}
	if (m_pCModel && !IsTobMesh(m_pCModel->m_strName)) {
		pCResMagr->DeleteMesh(*m_pCModel);
		m_pCModel = 0;
	}

	if (IsTobMesh(pCModel->m_strName)) {
		if (IsTobMesh(m_pCModel->m_strName))
			pCResMagr->DeleteTobMesh(*pCModel);

		m_pCModel = pCResMagr->NewTobMesh();
		m_pCModel->CreateTob(pCModel->m_strName,
							 pCModel->m_nSegments, pCModel->m_rHeight, pCModel->m_rRadius, pCModel->m_rBotRadius);
		m_strModelName = pCModel->m_strName;

		m_nSegments = pCModel->m_nSegments;
		m_rHeight = pCModel->m_rHeight;
		m_rRadius = pCModel->m_rRadius;
		m_rBotRadius = pCModel->m_rBotRadius;
		if (IsCylinderMesh(pCModel->m_strName)) {
			_iUseParam = 0;
			_CylinderParam.resize(_wFrameCount);

			for (int n = 0; n < _wFrameCount; ++n) {
				_CylinderParam[n].iSegments = m_nSegments;
				_CylinderParam[n].fTopRadius = m_rRadius;
				_CylinderParam[n].fBottomRadius = m_rBotRadius;
				_CylinderParam[n].fHei = m_rHeight;
				_CylinderParam[n].Create();
			}
		}
		return;
	}
	else {
		m_pCModel = pCModel;
		m_strModelName = pCModel->m_strName;
		//SetModel(tpCModel);
		if (_eEffectType == EFFECT_FRAMETEX) {
			m_CTexFrame.GetCoordFromModel(m_pCModel);
		}
		else
			m_CTextruelist.GetTextureFromModel(m_pCModel);
	}
}

//!
bool I_Effect::SaveToFile(FILE* pFile) {
	char t_pszName[32];
	lstrcpy(t_pszName, m_strEffectName.c_str());
	fwrite(t_pszName, sizeof(char), 32, pFile);

	int t_temp;
	//!
	t_temp = (int)_eEffectType;
	fwrite(&t_temp, sizeof(int), 1, pFile);

	t_temp = (int)_eSrcBlend;
	fwrite(&t_temp, sizeof(int), 1, pFile);
	t_temp = (int)_eDestBlend;
	fwrite(&t_temp, sizeof(int), 1, pFile);

	///////////////!
	//!
	fwrite(&_fLength, sizeof(float), 1, pFile);
	//!
	fwrite(&_wFrameCount, sizeof(WORD), 1, pFile);
	//!
	for (WORD n = 0; n < _wFrameCount; n++) {
		fwrite(&_vecFrameTime[n], sizeof(float), 1, pFile);
	}
	//!
	for (WORD n = 0; n < _wFrameCount; n++) {
		fwrite(&_vecFrameSize[n], sizeof(D3DXVECTOR3), 1, pFile);
	}
	//!
	for (WORD n = 0; n < _wFrameCount; n++) {
		fwrite(&_vecFrameAngle[n], sizeof(D3DXVECTOR3), 1, pFile);
	}
	//!
	for (WORD n = 0; n < _wFrameCount; n++) {
		fwrite(&_vecFramePos[n], sizeof(D3DXVECTOR3), 1, pFile);
	}
	//!
	for (WORD n = 0; n < _wFrameCount; n++) {
		fwrite(&_vecFrameColor[n], sizeof(D3DXCOLOR), 1, pFile);
	}
	///////////////!
	//!
	fwrite(&m_CTexCoordlist.m_wVerCount, sizeof(WORD), 1, pFile);
	fwrite(&m_CTexCoordlist.m_wCoordCount, sizeof(WORD), 1, pFile);
	fwrite(&m_CTexCoordlist.m_fFrameTime, sizeof(float), 1, pFile);

	for (WORD n = 0; n < m_CTexCoordlist.m_wCoordCount; ++n) {
		fwrite(&m_CTexCoordlist.m_vecCoordList[n].front(),
			   sizeof(D3DXVECTOR2), m_CTexCoordlist.m_wVerCount, pFile);
	}
	///////////////!
	//m_CTextruelist.GetTextureFromModel(m_pCModel);
	//!				   ntn
	fwrite(&m_CTextruelist.m_wTexCount, sizeof(WORD), 1, pFile);
	//!
	fwrite(&m_CTextruelist.m_fFrameTime, sizeof(float), 1, pFile);

	lstrcpy(t_pszName, m_CTextruelist.m_vecTexName.c_str());
	fwrite(t_pszName, sizeof(char), 32, pFile);

	for (WORD n = 0; n < m_CTextruelist.m_wTexCount; n++) {
		fwrite(&m_CTextruelist.m_vecTexList[n].front(),
			   sizeof(D3DXVECTOR2), m_CTexCoordlist.m_wVerCount, pFile);
	}
	//!
	lstrcpy(t_pszName, m_pCModel->m_strName.c_str());
	fwrite(t_pszName, sizeof(char), 32, pFile);

	fwrite(&_bBillBoard, sizeof(bool), 1, pFile);
	fwrite(&_iVSIndex, sizeof(int), 1, pFile);


	//
	fwrite(&m_pCModel->m_nSegments, sizeof(int), 1, pFile);
	fwrite(&m_pCModel->m_rHeight, sizeof(float), 1, pFile);
	fwrite(&m_pCModel->m_rRadius, sizeof(float), 1, pFile);
	fwrite(&m_pCModel->m_rBotRadius, sizeof(float), 1, pFile);

	fwrite(&m_CTexFrame.m_wTexCount, sizeof(WORD), 1, pFile);
	fwrite(&m_CTexFrame.m_fFrameTime, sizeof(float), 1, pFile);
	for (WORD n = 0; n < m_CTexFrame.m_wTexCount; ++n) {
		lstrcpy(t_pszName, m_CTexFrame.m_vecTexName[n].c_str());
		fwrite(t_pszName, sizeof(char), 32, pFile);
	}
	fwrite(&m_CTexFrame.m_fFrameTime, sizeof(float), 1, pFile);

	fwrite(&_iUseParam, sizeof(int), 1, pFile);
	if (_iUseParam > 0) {
		for (int n = 0; n < _wFrameCount; ++n) {
			fwrite(&_CylinderParam[n].iSegments, sizeof(int), 1, pFile);
			fwrite(&_CylinderParam[n].fHei, sizeof(float), 1, pFile);
			fwrite(&_CylinderParam[n].fTopRadius, sizeof(float), 1, pFile);
			fwrite(&_CylinderParam[n].fBottomRadius, sizeof(float), 1, pFile);
		}
	}
	fwrite(&_bRotaLoop, sizeof(bool), 1, pFile);
	fwrite(&_vRotaLoop, sizeof(D3DXVECTOR4), 1, pFile);

	fwrite(&_bAlpha, sizeof(bool), 1, pFile);

	fwrite(&_bRotaBoard, sizeof(bool), 1, pFile);

	return true;
}

//!
bool I_Effect::LoadFromFile(FILE* pFile, DWORD dwVersion) {
	ReleaseAll();

	char t_pszName[32];
	fread(t_pszName, sizeof(char), 32, pFile);
	m_strEffectName = t_pszName;

	int t_temp;
	//!
	fread(&t_temp, sizeof(int), 1, pFile);
	_eEffectType = (EFFECT_TYPE)t_temp;

	fread(&t_temp, sizeof(int), 1, pFile);
	_eSrcBlend = (D3DBLEND)t_temp;
	fread(&t_temp, sizeof(int), 1, pFile);
	_eDestBlend = (D3DBLEND)t_temp;

	///////////////!
	//!
	fread(&_fLength, sizeof(float), 1, pFile);
	//!
	fread(&_wFrameCount, sizeof(WORD), 1, pFile);

	//!
	_vecFrameTime.resize(_wFrameCount);
	for (WORD n = 0; n < _wFrameCount; n++) {
		fread(&_vecFrameTime[n], sizeof(float), 1, pFile);
	}
	//!
	_vecFrameSize.resize(_wFrameCount);
	for (WORD n = 0; n < _wFrameCount; n++) {
		fread(&_vecFrameSize[n], sizeof(D3DXVECTOR3), 1, pFile);
	}
	//!
	_vecFrameAngle.resize(_wFrameCount);
	for (WORD n = 0; n < _wFrameCount; n++) {
		fread(&_vecFrameAngle[n], sizeof(D3DXVECTOR3), 1, pFile);
	}
	//!
	_vecFramePos.resize(_wFrameCount);
	for (WORD n = 0; n < _wFrameCount; n++) {
		fread(&_vecFramePos[n], sizeof(D3DXVECTOR3), 1, pFile);
	}
	//!
	_vecFrameColor.resize(_wFrameCount);
	for (WORD n = 0; n < _wFrameCount; n++) {
		fread(&_vecFrameColor[n], sizeof(D3DXCOLOR), 1, pFile);
	}
	///////////////!
	//!
	fread(&m_CTexCoordlist.m_wVerCount, sizeof(WORD), 1, pFile);
	//!
	fread(&m_CTexCoordlist.m_wCoordCount, sizeof(WORD), 1, pFile);
	//!
	fread(&m_CTexCoordlist.m_fFrameTime, sizeof(float), 1, pFile);
	m_CTexCoordlist.m_vecCoordList.resize(m_CTexCoordlist.m_wCoordCount);
	for (WORD n = 0; n < m_CTexCoordlist.m_wCoordCount; n++) {
		m_CTexCoordlist.m_vecCoordList[n].resize(m_CTexCoordlist.m_wVerCount);
		fread(&m_CTexCoordlist.m_vecCoordList[n].front(),
			  sizeof(D3DXVECTOR2), m_CTexCoordlist.m_wVerCount, pFile);
	}
	///////////////!
	//!
	fread(&m_CTextruelist.m_wTexCount, sizeof(WORD), 1, pFile);
	//!
	fread(&m_CTextruelist.m_fFrameTime, sizeof(float), 1, pFile);

	fread(t_pszName, sizeof(char), 32, pFile);
	//s_string str = t_pszName;
	//
	char psname[32];
	memset(psname, 0, 32);
	char* pDataName = _strlwr(_strdup(t_pszName));

	if ((strstr(pDataName, ".dds") == NULL) && strstr(pDataName, ".tga") == NULL) {
		m_CTextruelist.m_vecTexName = pDataName;
	}
	else {
		int len = lstrlen(pDataName);
		memcpy(psname, pDataName, len - 4);
		m_CTextruelist.m_vecTexName = psname;
	}
	SAFE_DELETE_ARRAY(pDataName);


	//m_CTextruelist.m_vecTexName.resize(m_CTextruelist.m_wTexCount);
	m_CTextruelist.m_vecTexList.resize(m_CTextruelist.m_wTexCount);
	for (WORD n = 0; n < m_CTextruelist.m_wTexCount; n++) {
		m_CTextruelist.m_vecTexList[n].resize(m_CTexCoordlist.m_wVerCount);

		fread(&m_CTextruelist.m_vecTexList[n].front(),
			  sizeof(D3DXVECTOR2), m_CTexCoordlist.m_wVerCount, pFile);
	}

	fread(t_pszName, sizeof(char), 32, pFile);
	m_strModelName = t_pszName;

	fread(&_bBillBoard, sizeof(bool), 1, pFile);
	fread(&_iVSIndex, sizeof(int), 1, pFile);

	if (dwVersion > 1) {
		//
		fread(&m_nSegments, sizeof(int), 1, pFile);
		fread(&m_rHeight, sizeof(float), 1, pFile);
		fread(&m_rRadius, sizeof(float), 1, pFile);
		fread(&m_rBotRadius, sizeof(float), 1, pFile);
	}
	if (dwVersion > 2) {
		fread(&m_CTexFrame.m_wTexCount, sizeof(WORD), 1, pFile);
		fread(&m_CTexFrame.m_fFrameTime, sizeof(float), 1, pFile);

		m_CTexFrame.m_vecTexName.resize(m_CTexFrame.m_wTexCount);
		m_CTexFrame.m_vecTexs.resize(m_CTexFrame.m_wTexCount);

		for (WORD n = 0; n < m_CTexFrame.m_wTexCount; ++n) {
			fread(t_pszName, sizeof(char), 32, pFile);
			m_CTexFrame.m_vecTexName[n] = t_pszName;
		}
		fread(&m_CTexFrame.m_fFrameTime, sizeof(float), 1, pFile);
	}
	_iUseParam = 0;
	_CylinderParam.resize(_wFrameCount);

	if (dwVersion > 3) {
		fread(&_iUseParam, sizeof(int), 1, pFile);
		if (_iUseParam > 0) {
			for (int n = 0; n < _wFrameCount; ++n) {
				fread(&_CylinderParam[n].iSegments, sizeof(int), 1, pFile);
				fread(&_CylinderParam[n].fHei, sizeof(float), 1, pFile);
				fread(&_CylinderParam[n].fTopRadius, sizeof(float), 1, pFile);
				fread(&_CylinderParam[n].fBottomRadius, sizeof(float), 1, pFile);

				_CylinderParam[n].Create();
			}
		}
		else {
			if (IsCylinderMesh(m_strModelName)) {
				for (int n = 0; n < _wFrameCount; ++n) {
					_CylinderParam[n].iSegments = m_nSegments;
					_CylinderParam[n].fTopRadius = m_rRadius;
					_CylinderParam[n].fBottomRadius = m_rBotRadius;
					_CylinderParam[n].fHei = m_rHeight;
					_CylinderParam[n].Create();
				}
			}
		}
	}
	else {
		//_iUseParam = 0;
		//_CylinderParam.resize(_wFrameCount);
		if (IsCylinderMesh(m_strModelName)) {
			_iUseParam = 0;

			for (int n = 0; n < _wFrameCount; ++n) {
				_CylinderParam[n].iSegments = m_nSegments;
				_CylinderParam[n].fTopRadius = m_rRadius;
				_CylinderParam[n].fBottomRadius = m_rBotRadius;
				_CylinderParam[n].fHei = m_rHeight;
				_CylinderParam[n].Create();
			}
		}
	}
	if (dwVersion > 4) {
		fread(&_bRotaLoop, sizeof(bool), 1, pFile);
		fread(&_vRotaLoop, sizeof(D3DXVECTOR4), 1, pFile);
	}
	if (dwVersion > 5)
		fread(&_bAlpha, sizeof(bool), 1, pFile);
	if (dwVersion > 6)
		fread(&_bRotaBoard, sizeof(bool), 1, pFile);

	IsSame();
	return true;
}

void I_Effect::CopyEffect(I_Effect* pEff) {
	m_pDev = pEff->m_pDev;;

	//!
	m_CTexCoordlist.Copy(&pEff->m_CTexCoordlist);
	//!
	m_CTextruelist.Copy(&pEff->m_CTextruelist);
	//!
	m_CTexFrame.Copy(&pEff->m_CTexFrame);

	if (m_pCModel && !IsTobMesh(m_pCModel->m_strName)) {
		m_pCModel->SetUsing(false);
		m_pCModel = 0;
	}
	m_pCModel = 0;
	m_strModelName = pEff->m_strModelName;

	m_nSegments = pEff->m_nSegments;
	m_rHeight = pEff->m_rHeight;
	m_rRadius = pEff->m_rRadius;
	m_rBotRadius = pEff->m_rBotRadius;

	m_strEffectName = pEff->m_strEffectName;
	_eEffectType = pEff->_eEffectType;
	_fLength = pEff->_fLength;
	_wFrameCount = pEff->_wFrameCount;

	_vecFrameTime.resize(_wFrameCount);
	_vecFrameSize.resize(_wFrameCount);
	_vecFrameAngle.resize(_wFrameCount);
	_vecFramePos.resize(_wFrameCount);
	_vecFrameColor.resize(_wFrameCount);

	_vecFrameTime = pEff->_vecFrameTime;
	_vecFrameSize = pEff->_vecFrameSize;
	_vecFrameAngle = pEff->_vecFrameAngle;
	_vecFramePos = pEff->_vecFramePos;
	_vecFrameColor = pEff->_vecFrameColor;

	_iUseParam = pEff->_iUseParam;
	_CylinderParam.resize(_wFrameCount);
	if (_iUseParam > 0) {
		_CylinderParam = pEff->_CylinderParam;
		for (int n = 0; n < _wFrameCount; ++n) {
			_CylinderParam[n].Create();
		}
	}
	else {
		if (IsCylinderMesh(m_strModelName)) {
			for (int n = 0; n < _wFrameCount; ++n) {
				_CylinderParam[n].iSegments = m_nSegments;
				_CylinderParam[n].fTopRadius = m_rRadius;
				_CylinderParam[n].fBottomRadius = m_rBotRadius;
				_CylinderParam[n].fHei = m_rHeight;
				_CylinderParam[n].Create();
			}
		}
	}

	_bBillBoard = pEff->_bBillBoard;
	_bRotaLoop = pEff->_bRotaLoop;
	_vRotaLoop = pEff->_vRotaLoop;
	_bRotaBoard = pEff->_bRotaBoard;
	_bAlpha = pEff->_bAlpha;

	_iVSIndex = pEff->_iVSIndex;


	_bSizeSame = pEff->_bSizeSame;
	_bAngleSame = pEff->_bAngleSame;
	_bPosSame = pEff->_bPosSame;
	_bColorSame = pEff->_bColorSame;

	_eSrcBlend = pEff->_eSrcBlend;
	_eDestBlend = pEff->_eDestBlend;
}

bool I_Effect::IsChangeably() {
	if (m_pCModel)
		return m_pCModel->IsChangeably();
	return true;
}

void I_Effect::IsSame() {
	_bSizeSame = true;
	_bAngleSame = true;
	_bPosSame = true;
	_bColorSame = true;

	int m;
	for (int n = 1; n < _wFrameCount; n++) {
		m = n - 1;
		if (_vecFrameSize[n] != _vecFrameSize[m])
			_bSizeSame = false;
		if (_vecFrameAngle[n] != _vecFrameAngle[m])
			_bAngleSame = false;
		if (_vecFramePos[n] != _vecFramePos[m])
			_bPosSame = false;
		if (_vecFrameColor[n] != _vecFrameColor[m])
			_bColorSame = false;
	}
}

void I_Effect::SpliteTexture(int iRow, int iCol) {
	if (_eEffectType == EFFECT_FRAMETEX) {
	}
	else
		m_CTextruelist.CreateSpliteTexture(iRow, iCol);
}

void I_Effect::SetTextureTime(float ftime) {
	if (_eEffectType == EFFECT_FRAMETEX) {
		m_CTexFrame.m_fFrameTime = ftime;
	}
	else
		m_CTextruelist.m_fFrameTime = ftime;
}

void I_Effect::SetTexture() {
	if (IsItem()) {
		lwITex* tex = NULL;
		lwITex* tex2 = NULL;

		if (_eEffectType == EFFECT_FRAMETEX) {
			if (m_CTexFrame.m_lpCurTex && m_CTexFrame.m_lpCurTex->IsLoadingOK()) {
				m_pCModel->ResetItemTexture(0, m_CTexFrame.m_lpCurTex, &tex);
				m_pCModel->ResetItemTexture(1, m_CTexFrame.m_lpCurTex, &tex2);
			}
		}
		else {
			if (m_CTextruelist.m_pTex && m_CTextruelist.m_pTex->IsLoadingOK()) {
				m_pCModel->ResetItemTexture(0, m_CTextruelist.m_pTex, &tex);
				m_pCModel->ResetItemTexture(1, m_CTextruelist.m_pTex, &tex2);
			}
			else
				return;
		}
	}
	else {
		if (_eEffectType == EFFECT_FRAMETEX) {
			if (m_CTexFrame.m_lpCurTex && m_CTexFrame.m_lpCurTex->IsLoadingOK())
				m_pDev->SetTexture(0, m_CTexFrame.m_lpCurTex->GetTex());
		}
		else {
			if (m_CTextruelist.m_pTex && m_CTextruelist.m_pTex->IsLoadingOK())
				m_pDev->SetTexture(0, m_CTextruelist.m_pTex->GetTex());
			else
				return;
		}
	}
}

void I_Effect::SetVertexShader() {
	m_pDev->SetVertexShader(ResMgr.GetVShaderByID(_iVSIndex));
	m_pDev->SetVertexDeclaration(ResMgr.GetVDeclByID(_iVSIndex));
}

void I_Effect::Render() {
	m_pDev->SetRenderState(D3DRS_SRCBLEND, _eSrcBlend);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, _eDestBlend);

	if (m_pCModel) {
		if (IsUseParam())
			m_pCModel->RenderTob(&_CylinderParam[m_ilast], &_CylinderParam[m_inext], m_flerp);
		else
			m_pCModel->RenderModel();
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CEffectModel::CEffectModel() {
	m_pDev = NULL;
	m_strName = "";

	_lwMesh = NULL;
	m_pRes = NULL;
	m_vEffVer = 0;

	_dwVerCount = 0;
	_dwFaceCount = 0;

	m_bChangeably = false;
	m_oldtex = NULL;
	m_bItem = false;
}

bool CEffectModel::Copy(const CEffectModel& rhs) {
	ReleaseModel();

	MPSceneItem::Copy(&rhs);

	InitDevice(rhs.m_pDev, rhs.m_pRes);

	GetObject()->GetPrimitive()->SetState(STATE_TRANSPARENT, 0);

	m_strName = rhs.m_strName;

	return true;
}

CEffectModel::CEffectModel(MPRender* pDev, lwIResourceMgr* pRes)
{
	m_pDev = pDev;
	m_strName = "";

	_lwMesh = NULL;

	m_pRes = pRes;
	m_vEffVer = 0;

	_dwVerCount = 0;
	_dwFaceCount = 0;
	m_bChangeably = false;
	m_oldtex = NULL;
	m_oldtex2 = NULL;

	m_bItem = false;

	m_iID = -1;
}

CEffectModel::~CEffectModel() {
	ReleaseModel();
}
void CEffectModel::InitDevice(MPRender* pDev, lwIResourceMgr* pRes)
{
	m_pDev = pDev;
	m_pRes = pRes;
}
MPRender* CEffectModel::GetDev()
{
	return m_pDev;
}

void CEffectModel::ReleaseModel() {
	if (!_lwMesh) {
		if (m_bItem) {
			lwITex* oldtex;
			this->ResetItemTexture(0, m_oldtex, &oldtex);
			this->ResetItemTexture(1, m_oldtex2, &oldtex);

			//oldtex->Release();
			Destroy();
		}
	}
	SAFE_RELEASE(_lwMesh);

	SAFE_DELETE_ARRAY(m_vEffVer);
	m_bItem = false;

	m_iID = -1;
}


bool CEffectModel::CreateTriangle() {
	//ReleaseModel();

	//// mesh
	//// test lock method
	//lwFVFStruct_XyzDiffuse* buf;
	//lwILockableStreamVB* lsvb = g_mesh0->GetLockableStreamVB();
	//lsvb->Lock(0, 0, (void**)&buf, D3DLOCK_DISCARD);
	//buf->dif = D3DCOLOR_XRGB(255, 0, 0);
	//lsvb->Unlock();
	//lockg_mesh0->LoadVideoMemory();
	//vbrender
	_dwVerCount = 3;
	_dwFaceCount = 1;

	if (_lwMesh != NULL)
		SAFE_RELEASE(_lwMesh);


	m_pRes->CreateMesh(&_lwMesh);
	_lwMesh->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = EFFECT_VER_FVF;
	mi.pt_type = D3DPT_TRIANGLEFAN;
	mi.subset_num = 1;
	mi.vertex_num = 3;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq = LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);
	mi.vertex_seq[0] = lwVector3(-0.5f, 0, 0);
	mi.vertex_seq[1] = lwVector3(0, 0, 0.5f);
	mi.vertex_seq[2] = lwVector3(0.5f, 0, 0);
	mi.blend_seq[0].weight[0] = 0;
	mi.blend_seq[1].weight[0] = 1;
	mi.blend_seq[2].weight[0] = 2;
	mi.vercol_seq[0] = 0xffffffff;
	mi.vercol_seq[1] = 0xffffffff;
	mi.vercol_seq[2] = 0xffffffff;
	mi.texcoord0_seq[0] = lwVector2(0.0f, 1.0f);
	mi.texcoord0_seq[1] = lwVector2(0.5f, 0);
	mi.texcoord0_seq[2] = lwVector2(1.0f, 1.0f);
	lwSubsetInfo_Construct(&mi.subset_seq[0], 1, 0, 3, 0);

	if (LW_FAILED(_lwMesh->LoadSystemMemory(&mi)))
		return 0;

	if (LW_FAILED(_lwMesh->LoadVideoMemory()))
		return 0;

	_lpSVB = _lwMesh->GetLockableStreamVB();
	_lpSIB = _lwMesh->GetLockableStreamIB();


	m_strName = MESH_TRI;

	return true;
}

bool CEffectModel::CreatePlaneTriangle() {
	//ReleaseModel();

	_dwVerCount = 3;
	_dwFaceCount = 1;

	if (_lwMesh != NULL)
		SAFE_RELEASE(_lwMesh);
	m_pRes->CreateMesh(&_lwMesh);
	_lwMesh->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = EFFECT_VER_FVF;
	mi.pt_type = D3DPT_TRIANGLEFAN;
	mi.subset_num = 1;
	mi.vertex_num = 3;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq = LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);
	mi.vertex_seq[0] = lwVector3(-0.5f, 0.5f, 0);
	mi.vertex_seq[1] = lwVector3(0, -0.5f, 0);
	mi.vertex_seq[2] = lwVector3(0.5f, 0.5f, 0);
	mi.blend_seq[0].weight[0] = 0;
	mi.blend_seq[1].weight[0] = 1;
	mi.blend_seq[2].weight[0] = 2;
	mi.vercol_seq[0] = 0xffffffff;
	mi.vercol_seq[1] = 0xffffffff;
	mi.vercol_seq[2] = 0xffffffff;
	mi.texcoord0_seq[0] = lwVector2(0.0f, 1.0f);
	mi.texcoord0_seq[1] = lwVector2(0.5f, 0);
	mi.texcoord0_seq[2] = lwVector2(1.0f, 1.0f);
	lwSubsetInfo_Construct(&mi.subset_seq[0], 1, 0, 3, 0);


	if (LW_FAILED(_lwMesh->LoadSystemMemory(&mi)))
		return 0;

	if (LW_FAILED(_lwMesh->LoadVideoMemory()))
		return 0;

	_lpSVB = _lwMesh->GetLockableStreamVB();
	_lpSIB = _lwMesh->GetLockableStreamIB();

	m_strName = MESH_PLANETRI;
	return true;
}

bool CEffectModel::CreateRect() {
	//ReleaseModel();

	_dwVerCount = 4;
	_dwFaceCount = 2;

	if (_lwMesh != NULL)
		SAFE_RELEASE(_lwMesh);

	m_pRes->CreateMesh(&_lwMesh);
	_lwMesh->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = EFFECT_VER_FVF;
	mi.pt_type = D3DPT_TRIANGLEFAN;
	mi.subset_num = 1;
	mi.vertex_num = 4;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq = LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);
	mi.vertex_seq[0] = lwVector3(-0.5f, 0, 0);
	mi.vertex_seq[1] = lwVector3(-0.5f, 0, 1.0f);
	mi.vertex_seq[2] = lwVector3(0.5f, 0, 1.0f);
	mi.vertex_seq[3] = lwVector3(0.5f, 0, 0);

	mi.blend_seq[0].weight[0] = 0;
	mi.blend_seq[1].weight[0] = 1;
	mi.blend_seq[2].weight[0] = 2;
	mi.blend_seq[3].weight[0] = 3;

	mi.vercol_seq[0] = 0xffffffff;
	mi.vercol_seq[1] = 0xffffffff;
	mi.vercol_seq[2] = 0xffffffff;
	mi.vercol_seq[3] = 0xffffffff;

	mi.texcoord0_seq[0] = lwVector2(0.0f, 1.0f);
	mi.texcoord0_seq[1] = lwVector2(0.0f, 0);
	mi.texcoord0_seq[2] = lwVector2(1.0f, 0.0f);
	mi.texcoord0_seq[3] = lwVector2(1.0f, 1.0f);

	lwSubsetInfo_Construct(&mi.subset_seq[0], 2, 0, 4, 0);

	_lwMesh->LoadSystemMemory(&mi);
	_lwMesh->LoadVideoMemory();
	_lpSVB = _lwMesh->GetLockableStreamVB();
	_lpSIB = _lwMesh->GetLockableStreamIB();

	m_strName = MESH_RECT;

	return true;
}

bool CEffectModel::CreateRectZ() {
	//ReleaseModel();

	_dwVerCount = 4;
	_dwFaceCount = 2;

	if (_lwMesh != NULL)
		SAFE_RELEASE(_lwMesh);

	m_pRes->CreateMesh(&_lwMesh);
	_lwMesh->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = EFFECT_VER_FVF;
	mi.pt_type = D3DPT_TRIANGLEFAN;
	mi.subset_num = 1;
	mi.vertex_num = 4;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq = LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);
	mi.vertex_seq[0] = lwVector3(0, 0, 0);
	mi.vertex_seq[1] = lwVector3(0, 0, 1);
	mi.vertex_seq[2] = lwVector3(0, 1, 1);
	mi.vertex_seq[3] = lwVector3(0, 1, 0);

	mi.blend_seq[0].weight[0] = 0;
	mi.blend_seq[1].weight[0] = 1;
	mi.blend_seq[2].weight[0] = 2;
	mi.blend_seq[3].weight[0] = 3;

	mi.vercol_seq[0] = 0xffffffff;
	mi.vercol_seq[1] = 0xffffffff;
	mi.vercol_seq[2] = 0xffffffff;
	mi.vercol_seq[3] = 0xffffffff;

	mi.texcoord0_seq[0] = lwVector2(0.0f, 1.0f);
	mi.texcoord0_seq[1] = lwVector2(0.0f, 0);
	mi.texcoord0_seq[2] = lwVector2(1.0f, 0.0f);
	mi.texcoord0_seq[3] = lwVector2(1.0f, 1.0f);

	lwSubsetInfo_Construct(&mi.subset_seq[0], 2, 0, 4, 0);

	_lwMesh->LoadSystemMemory(&mi);
	_lwMesh->LoadVideoMemory();
	_lpSVB = _lwMesh->GetLockableStreamVB();
	_lpSIB = _lwMesh->GetLockableStreamIB();

	m_strName = MESH_RECTZ;

	return true;
}

bool CEffectModel::CreatePlaneRect() {
	//ReleaseModel();

	_dwVerCount = 4;
	_dwFaceCount = 2;

	if (_lwMesh != NULL)
		SAFE_RELEASE(_lwMesh);
	m_pRes->CreateMesh(&_lwMesh);
	_lwMesh->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = EFFECT_VER_FVF;
	mi.pt_type = D3DPT_TRIANGLEFAN;
	mi.subset_num = 1;
	mi.vertex_num = 4;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq = LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);
	mi.vertex_seq[0] = lwVector3(-0.5f, -0.5f, 0);
	mi.vertex_seq[1] = lwVector3(-0.5f, 0.5f, 0);
	mi.vertex_seq[2] = lwVector3(0.5f, 0.5f, 0);
	mi.vertex_seq[3] = lwVector3(0.5f, -0.5f, 0);

	mi.blend_seq[0].weight[0] = 0;
	mi.blend_seq[1].weight[0] = 1;
	mi.blend_seq[2].weight[0] = 2;
	mi.blend_seq[3].weight[0] = 3;

	mi.vercol_seq[0] = 0xffffffff;
	mi.vercol_seq[1] = 0xffffffff;
	mi.vercol_seq[2] = 0xffffffff;
	mi.vercol_seq[3] = 0xffffffff;

	mi.texcoord0_seq[0] = lwVector2(0.0f, 1.0f);
	mi.texcoord0_seq[1] = lwVector2(0.0f, 0);
	mi.texcoord0_seq[2] = lwVector2(1.0f, 0.0f);
	mi.texcoord0_seq[3] = lwVector2(1.0f, 1.0f);

	lwSubsetInfo_Construct(&mi.subset_seq[0], 2, 0, 4, 0);

	_lwMesh->LoadSystemMemory(&mi);
	_lwMesh->LoadVideoMemory();
	_lpSVB = _lwMesh->GetLockableStreamVB();
	_lpSIB = _lwMesh->GetLockableStreamIB();

	m_strName = MESH_PLANERECT;

	return true;
}


bool CEffectModel::CreateCone(int nSeg, float fHei, float fRadius) {
	//ReleaseModel();

	m_nSegments = nSeg;
	m_rHeight = fHei;
	m_rRadius = 0;
	m_rBotRadius = fRadius;

	_dwVerCount = m_nSegments * 3;
	_dwFaceCount = m_nSegments * 2;

	if (_lwMesh != NULL)
		SAFE_RELEASE(_lwMesh);

	m_pRes->CreateMesh(&_lwMesh);
	_lwMesh->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = EFFECT_VER_FVF;
	mi.pt_type = D3DPT_TRIANGLESTRIP;
	mi.subset_num = 1;
	mi.vertex_num = _dwVerCount;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq = LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);

	int nCurrentSegment;
	//lwVector3* pVertex = mi.vertex_seq;
	//lwVector2* pCoord = mi.texcoord0_seq;
	int idx = 0;

	float rDeltaSegAngle = (2.0f * D3DX_PI / m_nSegments);
	float rSegmentLength = 1.0f / (float)m_nSegments;
	float ny0 = (90.0f - (float)D3DXToDegree(atan(m_rHeight / m_rBotRadius))) / 90.0f;
	for (nCurrentSegment = 0; nCurrentSegment <= m_nSegments; nCurrentSegment++) {
		float x0 = m_rBotRadius * sinf(nCurrentSegment * rDeltaSegAngle);
		float z0 = m_rBotRadius * cosf(nCurrentSegment * rDeltaSegAngle);

		mi.vertex_seq[idx].x = 0.0f;
		mi.vertex_seq[idx].z = m_rHeight;
		mi.vertex_seq[idx].y = 0.0f;
		mi.texcoord0_seq[idx].x = 1.0f - (rSegmentLength * (float)nCurrentSegment);
		mi.texcoord0_seq[idx].y = 0.0f;
		idx++;

		mi.vertex_seq[idx].x = x0;
		mi.vertex_seq[idx].z = 0.0f;
		mi.vertex_seq[idx].y = z0;
		mi.texcoord0_seq[idx].x = 1.0f - (rSegmentLength * (float)nCurrentSegment);
		mi.texcoord0_seq[idx].y = 1.5f;
		idx++;
	}
	for (WORD n = 0; n < _dwVerCount; n++) {
		mi.blend_seq[n].weight[0] = n;
		mi.vercol_seq[n] = 0xffffffff;
	}

	lwSubsetInfo_Construct(&mi.subset_seq[0], _dwFaceCount, 0, _dwVerCount, 0);

	if (LW_FAILED(_lwMesh->LoadSystemMemory(&mi)))
		return 0;

	if (LW_FAILED(_lwMesh->LoadVideoMemory()))
		return 0;
	_lpSVB = _lwMesh->GetLockableStreamVB();
	//_lpSIB = _lwMesh->GetLockableStreamIB();

	m_strName = MESH_CONE;
	m_bChangeably = true;
	return true;
}

bool CEffectModel::CreateCylinder(int nSeg, float fHei, float fTopRadius, float fBottomRadius) {
	//ReleaseModel();

	m_nSegments = nSeg;
	m_rHeight = fHei;
	m_rRadius = fTopRadius;
	m_rBotRadius = fBottomRadius;

	_dwVerCount = m_nSegments * 3;
	_dwFaceCount = m_nSegments * 2;

	if (_lwMesh != NULL)
		SAFE_RELEASE(_lwMesh);

	if (m_vEffVer != NULL)
		SAFE_DELETE_ARRAY(m_vEffVer);

	m_vEffVer = new SEFFECT_VERTEX[_dwVerCount];
	m_pRes->CreateMesh(&_lwMesh);
	_lwMesh->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = EFFECT_VER_FVF;
	mi.pt_type = D3DPT_TRIANGLESTRIP;
	mi.subset_num = 1;
	mi.vertex_num = _dwVerCount;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq = LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);

	int nCurrentSegment;
	//lwVector3* pVertex = mi.vertex_seq;
	//lwVector2* pCoord = mi.texcoord0_seq;
	int idx = 0;

	float rDeltaSegAngle = (2.0f * D3DX_PI / m_nSegments);
	float rSegmentLength = 1.0f / (float)m_nSegments;
	float ny0 = (90.0f - (float)D3DXToDegree(atan(m_rHeight / m_rRadius))) / 90.0f;
	for (nCurrentSegment = 0; nCurrentSegment <= m_nSegments; nCurrentSegment++) {
		float x0 = m_rRadius * sinf(nCurrentSegment * rDeltaSegAngle);
		float z0 = m_rRadius * cosf(nCurrentSegment * rDeltaSegAngle);

		mi.vertex_seq[idx].x = x0;
		mi.vertex_seq[idx].z = m_rHeight;
		mi.vertex_seq[idx].y = z0;

		m_vEffVer[idx].m_SPos = (D3DXVECTOR3)mi.vertex_seq[idx];

		mi.texcoord0_seq[idx].x = 1.0f - (rSegmentLength * (float)nCurrentSegment);
		mi.texcoord0_seq[idx].y = 0.0f;

		m_vEffVer[idx].m_SUV = (D3DXVECTOR2)mi.texcoord0_seq[idx];

		idx++;

		x0 = m_rBotRadius * sinf(nCurrentSegment * rDeltaSegAngle);
		z0 = m_rBotRadius * cosf(nCurrentSegment * rDeltaSegAngle);
		mi.vertex_seq[idx].x = x0;
		mi.vertex_seq[idx].z = 0.0f;
		mi.vertex_seq[idx].y = z0;

		m_vEffVer[idx].m_SPos = (D3DXVECTOR3)mi.vertex_seq[idx];

		mi.texcoord0_seq[idx].x = 1.0f - (rSegmentLength * (float)nCurrentSegment);
		mi.texcoord0_seq[idx].y = 1.0f;

		m_vEffVer[idx].m_SUV = (D3DXVECTOR2)mi.texcoord0_seq[idx];

		idx++;
	}
	for (WORD n = 0; n < _dwVerCount; n++) {
		mi.blend_seq[n].weight[0] = n;
		mi.vercol_seq[n] = 0xffffffff;

		m_vEffVer[n].m_fIdx = n;
		m_vEffVer[n].m_dwDiffuse = 0xffffffff;
	}

	lwSubsetInfo_Construct(&mi.subset_seq[0], _dwFaceCount, 0, _dwVerCount, 0);

	if (LW_FAILED(_lwMesh->LoadSystemMemory(&mi)))
		return 0;

	if (LW_FAILED(_lwMesh->LoadVideoMemory()))
		return 0;
	_lpSVB = _lwMesh->GetLockableStreamVB();
	//_lpSIB = _lwMesh->GetLockableStreamIB();

	m_strName = MESH_CYLINDER;
	m_bChangeably = true;
	return true;
}

bool CEffectModel::CreateShadeModel(WORD wVerNum, WORD wFaceNum, int iGridCrossNum, bool usesoft) {
	//ReleaseModel();
	_dwVerCount = wVerNum;
	_dwFaceCount = wFaceNum;

	m_pRes->CreateMesh(&_lwMesh);
	//if(usesoft)
	_lwMesh->SetStreamType(STREAM_LOCKABLE);

	lwMeshInfo mi;
	mi.fvf = EFFECT_SHADE_FVF;
	mi.pt_type = D3DPT_TRIANGLELIST;
	mi.subset_num = 1;
	mi.vertex_num = wVerNum;
	mi.vertex_seq = LW_NEW(lwVector3[mi.vertex_num]);
	mi.blend_seq = LW_NEW(lwBlendInfo[mi.vertex_num]);
	mi.vercol_seq = LW_NEW(DWORD[mi.vertex_num]);
	mi.texcoord0_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.texcoord1_seq = LW_NEW(lwVector2[mi.vertex_num]);
	mi.subset_seq = LW_NEW(lwSubsetInfo[mi.subset_num]);
	mi.index_num = wFaceNum * 6;
	mi.index_seq = LW_NEW(DWORD[mi.index_num ]);
	int nIndex = 9; //!VS9
	for (DWORD n = 0; n < mi.vertex_num; n++) {
		mi.vercol_seq[n] = 0xffffffff;
		mi.vertex_seq[n] = lwVector3(0, 0, 0); //!
		mi.texcoord0_seq[n] = lwVector2(0, 0);
		mi.texcoord1_seq[n].x = (float)nIndex;
		nIndex++;
		mi.texcoord1_seq[n].y = (float)nIndex;
		nIndex++;
	}
	//ZeroMemory(mi.index_seq,sizeof(DWORD) * mi.index_num);
	//nIndex = 0;
	//int rad = 10;
	//for( int nY = 0; nY < rad; nY++ )
	//{
	//	for( int nX = 0; nX < rad; nX++ )
	//	{
	//		mi.index_seq[nIndex++] = nX + nY * (rad + 1);
	//		mi.index_seq[nIndex++] = (nX+1) + nY * (rad + 1);
	//		mi.index_seq[nIndex++] = nX + (nY+1) * (rad + 1);

	//		mi.index_seq[nIndex++] = nX + (nY+1) * (rad + 1);
	//		mi.index_seq[nIndex++] = (nX+1) + nY * (rad + 1);
	//		mi.index_seq[nIndex++] = (nX+1) + (nY+1) * (rad + 1);

	//	}
	//}
	nIndex = 0;
	for (int nY = 0; nY < iGridCrossNum; nY++) {
		for (int nX = 0; nX < iGridCrossNum; nX++) {
			mi.index_seq[nIndex++] = nX + nY * (iGridCrossNum + 1);
			mi.index_seq[nIndex++] = (nX + 1) + nY * (iGridCrossNum + 1);
			mi.index_seq[nIndex++] = nX + (nY + 1) * (iGridCrossNum + 1);

			mi.index_seq[nIndex++] = nX + (nY + 1) * (iGridCrossNum + 1);
			mi.index_seq[nIndex++] = (nX + 1) + nY * (iGridCrossNum + 1);
			mi.index_seq[nIndex++] = (nX + 1) + (nY + 1) * (iGridCrossNum + 1);
		}
	}
	lwSubsetInfo_Construct(&mi.subset_seq[0], wFaceNum, 0, wVerNum, 0);

	if (LW_FAILED(_lwMesh->LoadSystemMemory(&mi)))
		g_logManager.InternalLog(LogLevel::Error, "errors", "Shadow model load system memory failed");
	if (LW_FAILED(_lwMesh->LoadVideoMemory()))
		g_logManager.InternalLog(LogLevel::Error, "errors", "Shadow model load video memory failed");

	if (usesoft) {
		_lpSVB = _lwMesh->GetLockableStreamVB();
		_lpSIB = _lwMesh->GetLockableStreamIB();
	}
	else {
		_lpSVB = NULL;
		_lpSIB = NULL;
	}
	return true;
}

bool CEffectModel::LoadModel(const char* pszName) {
	//ReleaseModel();
	m_strName = pszName;

	//m_pModel = new MPSceneItem;

	// begin by lsh
	//Load(pszName,1);
	if (Load(pszName, 1) == 0) {
		m_oldtex = this->GetPrimitive()->GetMtlTexAgent(0)->GetTex(0);
		if (this->GetPrimitive()->GetMtlTexAgent(1))
			m_oldtex2 = this->GetPrimitive()->GetMtlTexAgent(1)->GetTex(0);
		else
			m_oldtex2 = NULL;
		//PlayDefaultAnimation();
		m_bItem = true;
	}
	else {
		m_oldtex = NULL;
		m_oldtex2 = NULL;
		m_bItem = false;
	}
	// end


	return true;
}


void CEffectModel::FrameMove(DWORD dwDailTime) {
	if (!_lwMesh)
		MPSceneItem::FrameMove();
}

void CEffectModel::Begin() {
	//vs
	//m_pDev->SetVertexShader(EFFECT_VER_FVF);
	if (_lwMesh)
		_lwMesh->BeginSet();
}

void CEffectModel::SetRenderNum(WORD wVer, WORD wFace) {
	lwMeshInfo* mi = _lwMesh->GetMeshInfo();
	lwSubsetInfo_Construct(&mi->subset_seq[0], wFace, 0, wVer, 0);
}

void CEffectModel::RenderModel() {
	if (!_lwMesh) {
		this->GetPrimitive()->RenderSubset(0);
		if (this->GetPrimitive()->GetMtlTexAgent(1)) {
			m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, TRUE);
			this->GetPrimitive()->RenderSubset(1);
		}
		//MPSceneItem::Render();
	}
	else {
		//g_Render.GetInterfaceMgr()->dev_obj->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
		//g_Render.GetInterfaceMgr()->dev_obj->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 2);

		//m_pDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

		if (LW_FAILED(_lwMesh->DrawSubset(0)))
			g_logManager.LogError("errors", "CEffectModel {}", (TCHAR*)m_strName.c_str());
	}
}

void CEffectModel::End() {
	//this been added when we fixed circle shadow not sure 100% of it but so far it works
	//@moth
	m_pDev->SetVertexShader(nullptr);
	m_pDev->SetFVF(EFFECT_VER_FVF);
	//end
	if (_lwMesh)
		_lwMesh->EndSet();
	//same as commit before
	m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CEffectModel::RenderTob(ModelParam* last, ModelParam* next, float lerp) {
	for (WORD n = 0; n < _dwVerCount; ++n) {
		D3DXVec3Lerp(&m_vEffVer[n].m_SPos, &last->vecVer[n], &next->vecVer[n], lerp);
	}
	m_pDev->SetVertexShader(NULL);
	m_pDev->SetFVF(EFFECT_VER_FVF);

	m_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, _dwFaceCount, m_vEffVer, sizeof(SEFFECT_VERTEX));
}

void ModelParam::Create() {
	DWORD dwVerCount = iSegments * 3;

	vecVer.resize(dwVerCount);

	int nCurrentSegment;

	int idx = 0;

	float rDeltaSegAngle = (2.0f * D3DX_PI / iSegments);
	float rSegmentLength = 1.0f / (float)iSegments;
	float ny0 = (90.0f - (float)D3DXToDegree(atan(fHei / fTopRadius))) / 90.0f;
	for (nCurrentSegment = 0; nCurrentSegment <= iSegments; nCurrentSegment++) {
		float x0 = fTopRadius * sinf(nCurrentSegment * rDeltaSegAngle);
		float z0 = fTopRadius * cosf(nCurrentSegment * rDeltaSegAngle);

		vecVer[idx].x = x0;
		vecVer[idx].z = fHei;
		vecVer[idx].y = z0;

		idx++;

		x0 = fBottomRadius * sinf(nCurrentSegment * rDeltaSegAngle);
		z0 = fBottomRadius * cosf(nCurrentSegment * rDeltaSegAngle);
		vecVer[idx].x = x0;
		vecVer[idx].z = 0.0f;
		vecVer[idx].y = z0;

		idx++;
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CTexCoordList::CTexCoordList() {
	Clear();
}

CTexCoordList::~CTexCoordList() {
}

void CTexCoordList::Clear() {
	m_wVerCount = 0;
	//
	m_wCoordCount = 0;
	//
	m_fFrameTime = 0.0f;
	//
	m_vecCoordList.clear();

	////!
	//m_wCurIndex = 0;
	////!
	//m_fCurTime = 0.0f;
}


void CTexCoordList::CreateTranslateCoord() {
	m_fFrameTime = 4.0f;

	m_wVerCount = 4;
	m_wCoordCount = 2;
	m_vecCoordList.resize(m_wCoordCount);
	D3DXVECTOR2 t_SVer[4];
	m_vecCoordList[0].resize(m_wVerCount);
	m_vecCoordList[1].resize(m_wVerCount);
	//t_SVer[0]			= D3DXVECTOR2(0, 0.0f);
	//t_SVer[1]			= D3DXVECTOR2(0, 1.0f);
	//t_SVer[2]			= D3DXVECTOR2(1.0f, 1.0f);
	//t_SVer[3]			= D3DXVECTOR2(1.0f, 0.0f);
	t_SVer[0] = D3DXVECTOR2(0, 0.1f);
	t_SVer[1] = D3DXVECTOR2(0, 0.0f);
	t_SVer[2] = D3DXVECTOR2(1.0f, 0.0f);
	t_SVer[3] = D3DXVECTOR2(1.0f, 0.1f);

	for (WORD i = 0; i < m_wVerCount; ++i) {
		m_vecCoordList[0][i] = t_SVer[i];
	}
	//t_SVer[0]			= D3DXVECTOR2(0, 0.1f);
	//t_SVer[1]			= D3DXVECTOR2(0, 0.1f);
	//t_SVer[2]			= D3DXVECTOR2(1.0f, 0.1f);
	//t_SVer[3]			= D3DXVECTOR2(1.0f, 0.1f);
	t_SVer[0] = D3DXVECTOR2(0, 1.2f);
	t_SVer[1] = D3DXVECTOR2(0, 0.0f);
	t_SVer[2] = D3DXVECTOR2(1.0f, 0.0f);
	t_SVer[3] = D3DXVECTOR2(1.0f, 1.2f);

	for (WORD i = 0; i < m_wVerCount; ++i) {
		m_vecCoordList[1][i] = t_SVer[i];
	}
}

void CTexCoordList::GetCoordFromModel(CEffectModel* pCModel) {
	if (!pCModel->IsBoard())
		return;
	m_wVerCount = (WORD)pCModel->GetVerCount();
	m_wCoordCount = 1;
	m_fFrameTime = 3.0f;
	m_vecCoordList.clear();
	m_vecCoordList.resize(m_wCoordCount);

	SEFFECT_VERTEX* pVertex;
	pCModel->Lock((BYTE**)&pVertex);

	for (WORD i = 0; i < m_wCoordCount; i++) {
		m_vecCoordList[i].resize(m_wVerCount);
		for (DWORD n = 0; n < m_wVerCount; n++) {
			m_vecCoordList[i][n] = pVertex[n].m_SUV;
		}
	}

	pCModel->Unlock();
}

void CTexCoordList::Reset() {
	////!
	//m_wCurIndex = 0;
	////!
	//m_fCurTime = 0.0f;

	//for(WORD i = 0; i < m_wVerCount; ++i)
	//{
	//	m_vecCurCoord[i] = m_vecCoordList[0][i];
	//}
}

void CTexCoordList::GetCurCoord(S_BVECTOR<D3DXVECTOR2>& vecOutCoord, WORD& wCurIndex, float& fCurTime,
								float fDailTime) {
	if (m_wCoordCount == 1) {
		//vecOutCoord = m_vecCoordList[0];
		vecOutCoord.clear();
		for (int n = 0; n < m_wVerCount; ++n) {
			vecOutCoord.push_back(m_vecCoordList[0][n]);
		}
		return;
	}
	WORD t_wNextIndex;
	float t_fLerp;
	fCurTime += fDailTime;
	if (fCurTime > m_fFrameTime) {
		wCurIndex++;
		fCurTime = 0.0f;
	}
	if (wCurIndex >= m_wCoordCount) {
		wCurIndex = 0;
	}
	if (wCurIndex == m_wCoordCount - 1) {
		t_wNextIndex = wCurIndex;
	}
	else {
		t_wNextIndex = wCurIndex + 1;
	}
	t_fLerp = fCurTime / m_fFrameTime;

	//vecOutCoord.setsize(m_wVerCount);
	for (WORD n = 0; n < m_wVerCount; ++n) {
		D3DXVec2Lerp(vecOutCoord[n],
					 &m_vecCoordList[wCurIndex][n],
					 &m_vecCoordList[t_wNextIndex][n], t_fLerp);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CTexList::CTexList() {
	Clear();
}

CTexList::~CTexList() {
	//for( DWORD i = 0; i < m_vecTexList.size(); ++i )
	//{
	//	SAFE_RELEASE(m_vecTexList[i]);
	//}
}

void CTexList::Clear() {
	m_wTexCount = 0;
	m_fFrameTime = 0.1f;

	m_vecTexName = "";
	m_vecTexList.clear();

	//m_wCurIndex = 0;
	//m_fCurTime	= 0.0f;
	m_lpCurTex = NULL;
}

void CTexList::SetTextureName(const s_string& pszName) {
	m_vecTexName = pszName;
}

void CTexList::Reset() {
	////!
	//m_wCurIndex = 0;
	////!
	//m_fCurTime = 0.0f;

	//m_lpCurTex = m_vecTexList[0];
}

//!
void CTexList::CreateSpliteTexture(int iRow, int iColnum) {
	m_wTexCount = iRow * iColnum;
	float fw = 1.0f / iRow;
	float fh = 1.0f / iColnum;

	D3DXVECTOR2 suv[4];
	suv[0] = D3DXVECTOR2(0, 1.0f);
	suv[1] = D3DXVECTOR2(0, 0);
	suv[2] = D3DXVECTOR2(1.0f, 0.0f);
	suv[3] = D3DXVECTOR2(1.0f, 1.0f);

	m_vecTexList.clear();
	m_vecTexList.resize(m_wTexCount);
	for (WORD h = 0; h < iColnum; h++) {
		for (WORD w = 0; w < iRow; w++) {
			m_vecTexList[w + h * iRow].resize(4);

			m_vecTexList[w + h * iRow][0].x = w * fw + suv[0].x;
			m_vecTexList[w + h * iRow][0].y = h * fh + fh;

			m_vecTexList[w + h * iRow][1].x = w * fw + suv[1].x;
			m_vecTexList[w + h * iRow][1].y = h * fh;

			m_vecTexList[w + h * iRow][2].x = m_vecTexList[w + h * iRow][1].x + fw;
			m_vecTexList[w + h * iRow][2].y = m_vecTexList[w + h * iRow][1].y;

			m_vecTexList[w + h * iRow][3].x = m_vecTexList[w + h * iRow][0].x + fw;
			m_vecTexList[w + h * iRow][3].y = m_vecTexList[w + h * iRow][0].y;
		}
	}
}

void CTexList::GetTextureFromModel(CEffectModel* pCModel) {
	if (!pCModel->IsBoard())
		return;
	//if(m_wTexCount != 0)
	//	return;

	WORD t_wVerCount = (WORD)pCModel->GetVerCount();
	m_wTexCount = 1;
	//m_fFrameTime	= 3.0f;
	m_vecTexList.clear();
	m_vecTexList.resize(m_wTexCount);

	SEFFECT_VERTEX* pVertex;
	pCModel->Lock((BYTE**)&pVertex);

	for (WORD i = 0; i < m_wTexCount; i++) {
		m_vecTexList[i].resize(t_wVerCount);
		for (WORD n = 0; n < t_wVerCount; n++) {
			m_vecTexList[i][n] = pVertex[n].m_SUV;
		}
	}

	pCModel->Unlock();
}

void CTexList::GetCurTexture(S_BVECTOR<D3DXVECTOR2>& coord, WORD& wCurIndex, float& fCurTime, float fDailTime) {
	if (m_wTexCount == 1) {
		//coord =  m_vecTexList[0];
		//coord.clear();
		for (WORD i = 0; i < (WORD)coord.size(); ++i) {
			//coord.push_back( m_vecTexList[0][i]);
			*coord[i] = m_vecTexList[0][i];
		}
		return;
	}
	fCurTime += fDailTime;
	if (fCurTime > m_fFrameTime) {
		wCurIndex++;
		fCurTime = 0.0f;
	}
	if (wCurIndex >= m_wTexCount) {
		wCurIndex = 0;
	}
	//coord =   m_vecTexList[wCurIndex];
	//coord.clear();
	for (WORD i = 0; i < coord.size(); ++i) {
		//coord.push_back( m_vecTexList[wCurIndex][i]);
		*coord[i] = m_vecTexList[wCurIndex][i];
	}
}

void CTexList::Remove() {
	if (m_wTexCount <= 1) {
		return;
	}
	m_vecTexList.pop_back();
	m_wTexCount--;
}

CTexFrame::CTexFrame() {
	m_wTexCount = 0;
	m_fFrameTime = 0.1f;

	m_vecTexName.clear();
	m_vecTexs.clear();

	m_lpCurTex = NULL;
}

CTexFrame::~CTexFrame() {
	m_vecTexName.clear();
	m_vecTexs.clear();
}

void CTexFrame::GetCoordFromModel(CEffectModel* pCModel) {
	if (!pCModel->IsBoard())
		return;

	WORD t_wVerCount = (WORD)pCModel->GetVerCount();
	m_vecCoord.clear();
	m_vecCoord.resize(t_wVerCount);

	SEFFECT_VERTEX* pVertex;
	pCModel->Lock((BYTE**)&pVertex);
	for (WORD n = 0; n < t_wVerCount; n++) {
		m_vecCoord[n] = pVertex[n].m_SUV;
	}
	pCModel->Unlock();
}

void CTexFrame::AddTexture(const s_string& pszName) {
	m_vecTexName.push_back(pszName);
	m_wTexCount++;
	m_vecTexs.resize(m_wTexCount);
}

lwITex* CTexFrame::GetCurTexture(WORD& wCurIndex, float& fCurTime, float fDailTime) {
	if (m_wTexCount == 0) {
		return m_lpCurTex = NULL;
	}
	if (m_wTexCount == 1) {
		return m_lpCurTex = m_vecTexs[0];
	}
	fCurTime += fDailTime;
	if (fCurTime > m_fFrameTime) {
		wCurIndex++;
		fCurTime = 0.0f;
	}
	if (wCurIndex >= m_wTexCount) {
		wCurIndex = 0;
	}
	return m_lpCurTex = m_vecTexs[wCurIndex];
}

void CTexFrame::Remove() {
	m_wTexCount = 0;
	m_fFrameTime = 0.1f;

	m_vecTexName.clear();
	m_vecTexs.clear();

	m_lpCurTex = NULL;
}

/************************************************************************/
/*CEffectFont*/
/************************************************************************/
CEffectFont::CEffectFont() {
	_strText = "0123456789+-";
	_iTextNum = 12;
	m_vecTexName = "number";
	_strBackBmp = "backnumber";
	_lpBackTex = NULL;
	_bUseBack = FALSE;
	_iTextureID = -1;

	//_lwBackMesh = NULL;
}

CEffectFont::~CEffectFont() {
	//ReleaseModel();
	//SAFE_RELEASE(_lwBackMesh);
}

bool CEffectFont::CreateEffectFont(MPRender* pDev,
								   CMPResManger* pCResMagr, int iTexID, D3DXCOLOR dwColor, bool bUseBack, bool bmain)
{
	//if(!pDev)
	//	return false;
	m_pRes = pCResMagr->m_pSysGraphics->GetResourceMgr();

	m_pDev = pDev;
	_bUseBack = bUseBack;
	_dwColor = dwColor;
	_iTextureID = iTexID;
	s_string str[] =
	{
		"addblood",
		"subblood",
		"addsp",
		"subsp",
		"addblood",
		"subblood",
		"bao",
		"submiss",
	};
	char psName[64];
	if (!bmain) {
		sprintf(psName, "%s", str[_iTextureID].c_str());
	}
	else {
		//if(_iTextureID == 6)
		//	sprintf(psName,"%s.tga",str[_iTextureID].c_str());
		//else
		sprintf(psName, "%s2", str[_iTextureID].c_str());
	}

	m_vecTexName = psName;
	//switch(_iTextureID)
	//{
	//case 0:
	//	m_vecTexName = ".tga";
	//	break;
	//case 1:
	//	m_vecTexName = ".tga";
	//	break;
	//case 2:
	//	m_vecTexName = "sp.tga";
	//	break;
	//case 3:
	//	m_vecTexName = "sp.tga";
	//	break;
	//case 4:
	//	m_vecTexName = ".tga";
	//	break;
	//case 5:
	//	m_vecTexName = ".tga";
	//	break;
	//case 6:
	//	m_vecTexName = ".tga";
	//	break;
	//}
	int id = pCResMagr->GetTextureID(m_vecTexName);
	if (id < 0) {
		g_logManager.LogError("errors", "CEffectFont texture {} not found", m_vecTexName.c_str());
		m_lpCurTex = NULL;
		m_pTex = NULL;
	}
	else {
		//m_lpCurTex = pCResMagr->GetTextureByID(id);
		m_pTex = pCResMagr->GetTextureByIDlw(id);

		m_lpCurTex = m_pTex->GetTex();
	}

	id = pCResMagr->GetTextureID(_strBackBmp);
	if (id < 0) {
		g_logManager.LogError("errors", "CEffectFont texture {} not found", _strBackBmp.c_str());
		_lpBackTex = NULL;
	}
	else {
		//pCResMagr->GetTextureByID(id);
		_lpBackTex = pCResMagr->GetTextureByIDlw(id);
	}

	float fx = 0.5f;
	float fy = 0.7f;
	if (_iTextureID == 7) {
		_iTextNum = 1;
		_vecCurText.push_back(0);
		fx = 2.0f;
		fy = 1.0f;
	}
	else
		_iTextNum = 12;

	CreateSpliteTexture(_iTextNum, 1);

	//ReleaseModel();
	_dwVerCount = _iTextNum * 2 * 3;
	_dwFaceCount = _iTextNum * 2;


	//SEFFECT_VERTEX t_SEffVer[4];
	t_SEffVer[0].m_SPos = D3DXVECTOR3(-fx, -fy, 0);
	t_SEffVer[0].m_fIdx = 0;
	t_SEffVer[0].m_dwDiffuse = 0xffffffff;
	t_SEffVer[0].m_SUV = D3DXVECTOR2(0.0f, 1.0f);

	t_SEffVer[1].m_SPos = D3DXVECTOR3(-fx, fy, 0);
	t_SEffVer[1].m_fIdx = 1;
	t_SEffVer[1].m_dwDiffuse = 0xffffffff;
	t_SEffVer[1].m_SUV = D3DXVECTOR2(0.0f, 0);

	t_SEffVer[2].m_SPos = D3DXVECTOR3(fx, fy, 0);
	t_SEffVer[2].m_fIdx = 2;
	t_SEffVer[2].m_dwDiffuse = 0xffffffff;
	t_SEffVer[2].m_SUV = D3DXVECTOR2(1.0f, 0.0f);

	t_SEffVer[3].m_SPos = D3DXVECTOR3(fx, -fy, 0);
	t_SEffVer[3].m_fIdx = 3;
	t_SEffVer[3].m_dwDiffuse = 0xffffffff;
	t_SEffVer[3].m_SUV = D3DXVECTOR2(1.0f, 1.0f);

	//HRESULT hr;
	//hr	= m_pDev->CreateVertexBuffer(sizeof(SEFFECT_VERTEX) * 4,
	//	D3DUSAGE_WRITEONLY |D3DUSAGE_DYNAMIC,
	//	EFFECT_VER_FVF,
	//	D3DPOOL_DEFAULT, &_lpBackVB);
	//if( FAILED(hr) )
	//	return false;

	//SEFFECT_VERTEX *pVertex;
	//_lpBackVB->Lock(0, 0, (BYTE**)&pVertex, 0   );
	//memcpy(pVertex, t_SEffVer, sizeof(SEFFECT_VERTEX) * 4);
	//_lpBackVB->Unlock();


	m_vEffVer = new SEFFECT_VERTEX[_dwVerCount];
	for (int m = 0; m < _iTextNum; m++) {
		for (int n = 0; n < 4; n++) {
			m_vEffVer[m * 4 + n].m_SPos.x = t_SEffVer[n].m_SPos.x + m;
			m_vEffVer[m * 4 + n].m_SPos.y = t_SEffVer[n].m_SPos.y;
			m_vEffVer[m * 4 + n].m_SPos.z = 0;
			m_vEffVer[m * 4 + n].m_fIdx = float(n);
			m_vEffVer[m * 4 + n].m_dwDiffuse = 0xffffffff;
			m_vEffVer[m * 4 + n].m_SUV = m_vecTexList[m][n];
		}
	}

	//hr	= m_pDev->CreateVertexBuffer(sizeof(SEFFECT_VERTEX) * _dwVerCount,
	//	D3DUSAGE_WRITEONLY |D3DUSAGE_DYNAMIC,
	//	EFFECT_VER_FVF,
	//	D3DPOOL_DEFAULT, &_lpVB);
	//if( FAILED(hr) )
	//	return false;

	//SEFFECT_VERTEX *pVertex;
	//_lpVB->Lock(0, 0, (BYTE**)&pVertex, 0   );
	//memcpy(pVertex, t_pSEffVer, sizeof(SEFFECT_VERTEX) * _dwVerCount);
	//_lpVB->Unlock();
	//SAFE_DELETE_ARRAY(t_pSEffVer);
	m_strName = "FONTEFFECT";
	return true;
}

void CEffectFont::SetRenderText(char* pszText) {
	if (_iTextureID == 7)
		return;

	int pos;
	char pszt[3];
	int len = lstrlen(pszText);
	_vecCurText.clear();
	//SEFFECT_VERTEX *pVertex;
	//_lpVB->Lock(0, 0, (BYTE**)&pVertex, 0   );

	for (int m = 0; m < len; m++) {
		if (pszText[m] & 0x80) {
			pszt[0] = pszText[m];
			pszt[1] = pszText[m + 1];
			pszt[2] = '\0';
		}
		else {
			pszt[0] = pszText[m];
			pszt[1] = '\0';
			pszt[2] = '\0';
		}
		pos = (int)_strText.find(pszt);
		if (pos != -1) {
			_vecCurText.push_back(pos);
			for (int n = 0; n < 4; n++) {
				m_vEffVer[m * 4 + n].m_SUV = m_vecTexList[pos][n];
			}
		}
	}

	//_lpVB->Unlock();

	//_lpBackVB->Lock(0, 0, (BYTE**)&pVertex, 0   );
	t_SEffVer[0].m_SPos.x -= 4.0f;
	t_SEffVer[0].m_SPos.y -= 2.0f;
	t_SEffVer[1].m_SPos.x -= 4.0f;
	t_SEffVer[1].m_SPos.y += 2.0f;
	t_SEffVer[2].m_SPos.x += len + 2.5f;
	t_SEffVer[2].m_SPos.y += 2.0f;
	t_SEffVer[3].m_SPos.x += len + 2.5f;
	t_SEffVer[3].m_SPos.y -= 2.0f;

	//_lpBackVB->Unlock();
}

void CEffectFont::RenderEffectFontBack(D3DXMATRIX* pmat) {
	//m_pDev->SetRenderState( D3DRS_TEXTUREFACTOR,
	//	0xffffffff );
	m_pDev->SetVertexShader(NULL);
	m_pDev->SetFVF(EFFECT_VER_FVF);

	if (_lpBackTex && _lpBackTex->IsLoadingOK())
		m_pDev->SetTexture(0, _lpBackTex->GetTex());
	else
		return;


	//m_pDev->SetStreamSource(0,_lpBackVB,sizeof(SEFFECT_VERTEX));
	//D3DXMATRIX	mat;
	//D3DXMatrixIdentity(&mat);
	//D3DXMatrixScaling(&mat,(float)_vecCurText.size()+0.5f,2,1);
	//D3DXMatrixTranslation(&matt,0,0,0 );
	//D3DXMatrixMultiply(&mat,&mat,&matt);
	//D3DXMatrixMultiply(&mat,&mat,pmat);
	m_pDev->SetTransformWorld(pmat);

	if (FAILED(m_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&t_SEffVer,sizeof(SEFFECT_VERTEX))))
		g_logManager.InternalLog(LogLevel::Error, "errors", "CEffectFont draw failed");
}

void CEffectFont::RenderEffectFont(D3DXMATRIX* pmat) {
	if (_bUseBack) {
		RenderEffectFontBack(pmat);
	}
	//m_pDev->SetRenderState( D3DRS_TEXTUREFACTOR,_dwColor );
	m_pDev->SetTransformWorld(pmat);


	if (m_pTex && m_pTex->IsLoadingOK())
		m_pDev->SetTexture(0, m_pTex->GetTex());
	else
		return;


	//m_pDev->SetStreamSource(0,_lpVB,sizeof(SEFFECT_VERTEX));
	for (int n = 0; n < (WORD)_vecCurText.size(); n++) {
		//m_pDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, n * 4, 2);
		if (FAILED(m_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,&m_vEffVer[n * 4],sizeof(SEFFECT_VERTEX))))
			g_logManager.InternalLog(LogLevel::Error, "errors", "CEffectFont draw2 failed");
	}
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
CCharacterActionCache CCharacterActionCache::_cache;

bool CCharacterActionCache::LoadActionDataFromStore() {
	Free();

	auto* store = CharacterActionStore::Instance();
	short maxType = store->GetMaxCharacterType();
	if (maxType < 1) {
		g_logManager.LogError("errors", "CCharacterActionCache::LoadActionDataFromStore: store пуст");
		return false;
	}

	_characterActions.resize(maxType);
	_maxCharacterType = maxType;

	for (short t = 0; t < maxType; t++) {
		_characterActions[t]._characterType = t + 1;
	}

	// Первый проход: определяем максимальный actionNo по каждому типу.
	std::vector<short> maxActionPerType(maxType, 0);
	store->ForEach([&](const CCharacterActionInfo& r) {
		int t = r._characterType - 1;
		if (t < 0 || t >= _maxCharacterType) {
			return;
		}

		maxActionPerType[t] = (std::max)(r._actionNo, maxActionPerType[t]);
	});

	// Резервируем слоты под actions каждого типа.
	for (short t = 0; t < _maxCharacterType; t++) {
		short maxAct = maxActionPerType[t];
		if (maxAct < 1) {
			continue;
		}
		_characterActions[t]._actions.resize(maxAct);
	}

	// Второй проход: заполнение данных.
	store->ForEach([&](const CCharacterActionInfo& r) {
		int t = r._characterType - 1;
		if (t < 0 || t >= _maxCharacterType) {
			return;
		}
		SChaAction& block = _characterActions[t];
		if (r._actionNo < 1 || static_cast<size_t>(r._actionNo) > block._actions.size()) {
			return;
		}

		ActionInfo& info = block._actions.at(r._actionNo - 1);
		info._actionNo = r._actionNo;
		info._startFrame = r._startFrame;
		info._endFrame = r._endFrame;
		info._keyFrames = r._keyFrames;
		block._validCount++;
	});

	// Подсчёт типов, в которых есть хотя бы одно действие.
	_actualCharacterType = 0;
	for (short t = 0; t < _maxCharacterType; t++) {
		if (_characterActions[t]._validCount > 0) {
			_actualCharacterType++;
		}
	}

	return true;
}

void CCharacterActionCache::Free() {
	_characterActions.clear();
	_maxCharacterType = 0;
	_actualCharacterType = 0;
}

const SChaAction* CCharacterActionCache::GetCharAction(int characterType) const {
	if (characterType < 1 || static_cast<size_t>(characterType) > _characterActions.size()) {
		return nullptr;
	}
	const SChaAction& block = _characterActions[characterType - 1];
	return block.IsValid() ? &block : nullptr;
}

long StringGetT(char* out, long out_max, const char* in, long* in_from, const char* end_list, long end_len) {
	long offset = -1; // set offset of get string to -1 for the first do process
	long i; // temp variable

	--(*in_from); // dec (*in_from) for the first do process
	do {
		out[++offset] = in[++(*in_from)];
		for (i = end_len - 1; i >= 0; --i) {
			if (out[offset] == end_list[i]) {
				out[offset] = 0x00;
				break;
			}
		}
	}
	while (out[offset] && offset < out_max);
	return offset;
}

void StringSkipCompartmentT(const char* in, long* in_from, const char* skip_list, long skip_len) {
	long i; // temp variable

	while (in[(*in_from)]) {
		for (i = skip_len - 1; i >= 0; --i) {
			if (in[(*in_from)] == skip_list[i])
				break;
		}
		if (i < 0) break; // dismatch skip conditions, finished
		else ++(*in_from); // match skip conditions, skip it
	}
}

s_string& I_Effect::getEffectModelName() {
	return m_strModelName;
}

bool I_Effect::IsItem() {
	if (m_pCModel) {
		return m_pCModel->IsItem();
	}
	if (strstr(m_strModelName.c_str(), ".lgo")) {
		return true;
	}
	return false;
}

void I_Effect::GetLerpSize(D3DXVECTOR3* pSOut, WORD wIdx1, WORD wIdx2, float fLerp) {
	if (_wFrameCount == 1 || _bSizeSame) {
		*pSOut = _vecFrameSize[0];
		return;
	}
	D3DXVec3Lerp(pSOut, &_vecFrameSize[wIdx1], &_vecFrameSize[wIdx2], fLerp);
}

void I_Effect::GetLerpAngle(D3DXVECTOR3* pSOut, WORD wIdx1, WORD wIdx2, float fLerp) {
	if (_wFrameCount == 1 || _bAngleSame) {
		*pSOut = _vecFrameAngle[0];
		return;
	}
	D3DXVec3Lerp(pSOut, &_vecFrameAngle[wIdx1], &_vecFrameAngle[wIdx2], fLerp);
}

void I_Effect::GetLerpPos(D3DXVECTOR3* pSOut, WORD wIdx1, WORD wIdx2, float fLerp) {
	if (_wFrameCount == 1 || _bPosSame) {
		*pSOut = _vecFramePos[0];
		return;
	}
	D3DXVec3Lerp(pSOut, &_vecFramePos[wIdx1], &_vecFramePos[wIdx2], fLerp);
}

void I_Effect::GetLerpColor(D3DXCOLOR* pSOut, WORD wIdx1, WORD wIdx2, float fLerp) {
	if (_wFrameCount == 1 || _bColorSame) {
		*pSOut = _vecFrameColor[0];
		return;
	}
	D3DXColorLerp(pSOut, &_vecFrameColor[wIdx1], &_vecFrameColor[wIdx2], fLerp);
}

void CTexCoordList::Copy(CTexCoordList* pList) {
	m_wVerCount = pList->m_wVerCount;
	m_wCoordCount = pList->m_wCoordCount;
	m_fFrameTime = pList->m_fFrameTime;
	m_vecCoordList.resize(m_wCoordCount);
	for (int n = 0; n < m_wCoordCount; ++n) {
		m_vecCoordList[n].resize(m_wVerCount);
		m_vecCoordList[n] = pList->m_vecCoordList[n];
	}
}

void CTexList::Copy(CTexList* pList) {
	m_wTexCount = pList->m_wTexCount;
	m_fFrameTime = pList->m_fFrameTime;
	m_vecTexList.resize(m_wTexCount);
	for (int n = 0; n < m_wTexCount; ++n) {
		m_vecTexList[n].resize(4);
		m_vecTexList[n] = pList->m_vecTexList[n];
	}
	m_vecTexName = pList->m_vecTexName;
	m_lpCurTex = NULL;
	m_pTex = NULL;
}

void CTexFrame::Copy(CTexFrame* pList) {
	m_wTexCount = pList->m_wTexCount;
	m_fFrameTime = pList->m_fFrameTime;
	m_vecTexName.resize(m_wTexCount);
	m_vecTexs.resize(m_wTexCount);
	for (int n = 0; n < m_wTexCount; ++n) {
		m_vecTexName[n] = pList->m_vecTexName[n];
	}
	m_vecCoord.resize(pList->m_vecCoord.size());
	m_vecCoord = pList->m_vecCoord;
}

void CEffectModel::Lock(BYTE** pvEffVer) {
	if (_lpSVB == 0) {
		*pvEffVer = 0;
		return;
	}

	if (LW_FAILED(_lpSVB->Lock(0, 0, (void**)pvEffVer, 0))) {
		MessageBox(NULL, "lock error msglock error", "error", 0);
		*pvEffVer = 0;
		assert(false);
	}
}

void CEffectModel::Unlock() {
	_lpSVB->Unlock();
}

void CEffectModel::LockIB(BYTE** pIdx) {
	if (LW_FAILED(_lpSIB->Lock(0, 0, (void**)pIdx, 0))) {
		MessageBox(NULL, "lock error msglock error", "error", 0);
		assert(false);
	}
}

void CEffectModel::UnlockIB() {
	_lpSIB->Unlock();
}

void I_Effect::SetTobParam(int nFrame, int nSegments, float rHeight, float rRadius, float rBotRadius) {
	_iUseParam = 0;
	_CylinderParam[nFrame].iSegments = nSegments;
	_CylinderParam[nFrame].fTopRadius = rRadius;
	_CylinderParam[nFrame].fBottomRadius = rBotRadius;
	_CylinderParam[nFrame].fHei = rHeight;
	_CylinderParam[nFrame].Create();

	_iUseParam = 1;
}

void I_Effect::GetTobParam(int nFrame, int& nSegments, float& rHeight, float& rRadius, float& rBotRadius) {
	nSegments = _CylinderParam[nFrame].iSegments;
	rRadius = _CylinderParam[nFrame].fTopRadius;
	rBotRadius = _CylinderParam[nFrame].fBottomRadius;
	rHeight = _CylinderParam[nFrame].fHei;
}

void I_Effect::GetRotaLoopMatrix(D3DXMATRIX* pmat, float& pCurRota, float fTime) {
	pCurRota += _vRotaLoop.w * fTime;
	if (pCurRota >= 6.283185f) {
		pCurRota = pCurRota - 6.283185f;
	}
	const auto v = D3DXVECTOR3(_vRotaLoop.x, _vRotaLoop.y, _vRotaLoop.z);
	D3DXMatrixRotationAxis(pmat, &v, pCurRota);
}

void Transpose(D3DMATRIX& result, D3DMATRIX& m) {
	result.m[0][0] = m.m[0][0];
	result.m[0][1] = m.m[1][0];
	result.m[0][2] = m.m[2][0];
	result.m[0][3] = m.m[3][0];
	result.m[1][0] = m.m[0][1];
	result.m[1][1] = m.m[1][1];
	result.m[1][2] = m.m[2][1];
	result.m[1][3] = m.m[3][1];
	result.m[2][0] = m.m[0][2];
	result.m[2][1] = m.m[1][2];
	result.m[2][2] = m.m[2][2];
	result.m[2][3] = m.m[3][2];
	result.m[3][0] = m.m[0][3];
	result.m[3][1] = m.m[1][3];
	result.m[3][2] = m.m[2][3];
	result.m[3][3] = m.m[3][3];
}

CMemoryBuf::CMemoryBuf()
{
	_pData = NULL;
	_lpos = 0;
	_size = 0;
}

CMemoryBuf::~CMemoryBuf()
{
	SAFE_DELETE_ARRAY(_pData);
}

bool CMemoryBuf::LoadFile(char* pszName)
{
	FILE* t_pFile = fopen(pszName, "rb");
	if (!t_pFile)
	{
		return false;
	}
	fseek(t_pFile, 0, SEEK_END);
	_size = ftell(t_pFile);
	SAFE_DELETE_ARRAY(_pData);
	_pData = new BYTE[_size];
	fseek(t_pFile, 0, SEEK_SET);
	fread(_pData, sizeof(BYTE), _size, t_pFile);
	fclose(t_pFile);
	_lpos = 0;
	return true;
}

void CMemoryBuf::mseek(long ioffset, int ipos)
{
	if (ipos == SEEK_CUR)
	{
		_lpos += ioffset;
	}
	if (ipos == SEEK_SET)
	{
		_lpos = ioffset;
	}
	if (ipos == SEEK_END)
	{
		_lpos = _size;
	}
}

void CMemoryBuf::mread(void* pmem, size_t psize, size_t pcount)
{
	memcpy(pmem, &_pData[_lpos], pcount * psize);
	_lpos += (long)pcount * (long)psize;
}
