#include "StdAfx.h"
#include "GlobalInc.h"
#include "MPModelEff.h"
#include "MPRender.h"
#include ".\mpparticlectrl.h"
#include <CharacterModelStore.h>
#include <cstring>
#include <algorithm>

using namespace std;

///************************************************************************/
///*                                                                      */
///************************************************************************/
//CMPParticleCtrl::~CMPParticleCtrl(void)
///************************************************************************/
///*                                                                      */
///************************************************************************/
//CMPParticleTrace::~CMPParticleTrace(void)
////float f = 0;
//	//
//		//! ID
//		return;

//		return;
//	//!
//		return;
//	//!
//	//!
//	//!.
//	//	 =  /  * 
//	//!
//	//!
///************************************************************************/
///*                                                                      */
///************************************************************************/
//CMPParticleRipple::~CMPParticleRipple(void)
//		//! ID
//		return;


/************************************************************************/
/* class CMPPartCtrl*/
/************************************************************************/
CMPPartCtrl::CMPPartCtrl(void) {
	m_strName = "";
	m_iPartNum = 0;
#ifndef USE_GAME
	//m_vecPartSys.resize(MAX_PART_SYS);//6
#endif
	m_fLength = 0; //0
	m_fCurTime = 0;

	m_iStripNum = 0;
	m_pcStrip = NULL;

	m_iModelNum = 0;
}

CMPPartCtrl::~CMPPartCtrl(void) {
	SAFE_DELETE_ARRAY(m_pcStrip);
	for (int n = 0; n < m_iModelNum; n++) {
		SAFE_DELETE(m_vecModel[n]);
	}
	m_vecModel.clear();
}

void CMPPartCtrl::SetStripCharacter(MPCharacter* pCha) {
	for (int n = 0; n < m_iStripNum; ++n) {
		m_pcStrip[n].AttachCharacter(pCha);
	}
}

void CMPPartCtrl::SetStripItem(MPSceneItem* pItem, bool bloop) {
	for (int n = 0; n < m_iStripNum; ++n) {
		m_pcStrip[n].AttachItem(pItem);
		m_pcStrip[n].SetLoop(bloop);
	}
}

void CMPPartCtrl::SetItemDummy(MPSceneItem* pItem, int dummy1, int dummy2) {
	for (int n = 0; n < m_iPartNum; ++n) {
		m_vecPartSys[n]->SetItemDummy(pItem, dummy1, dummy2);
	}
}


CChaModel* CMPPartCtrl::NewModel(const s_string& strID, CMPResManger* pResMagr) {
	m_iModelNum++;
	if (m_iModelNum > 1) {
		m_iModelNum = 1;
		return NULL;
	}
	m_vecModel.resize(m_iModelNum);

	m_vecModel[m_iModelNum - 1] = new CChaModel;
	m_vecModel[m_iModelNum - 1]->m_pDev = pResMagr->m_pDev;

	m_vecModel[m_iModelNum - 1]->LoadScript(strID);
	m_vecModel[m_iModelNum - 1]->SetVel(120);
	m_vecModel[m_iModelNum - 1]->SetPlayType(PLAY_LOOP);
	m_vecModel[m_iModelNum - 1]->SetCurPose(1);
	m_vecModel[m_iModelNum - 1]->PlayPose(0, PLAY_PAUSE);

	return m_vecModel[m_iModelNum - 1];
}

CChaModel* CMPPartCtrl::GetModel(int iIdx) {
	return m_vecModel[iIdx];
}

CMPStrip* CMPPartCtrl::NewStrip(const s_string& strTex, CMPResManger* pResMagr) {
	SAFE_DELETE_ARRAY(m_pcStrip);

	m_iStripNum = 1;
	m_pcStrip = new CMPStrip[m_iStripNum];
	m_pcStrip[0].CreateStrip(pResMagr->m_pDev, strTex, pResMagr);
	return &m_pcStrip[0];
}

CMPStrip* CMPPartCtrl::GetStrip(int iIdx) {
	return &m_pcStrip[iIdx];
}

void CMPPartCtrl::SetPlayType(int nType) {
	if (nType == 0) //
	{
		for (int n = 0; n < m_iPartNum; ++n) {
			m_vecPartSys[n]->SetPlayTime(0);
			m_vecPartSys[n]->SetLoop(true);
		}
		for (int n = 0; n < m_iModelNum; n++) {
			m_vecModel[n]->SetPlayType(PLAY_LOOP);
			m_vecModel[n]->Play();
		}
	}
}


void CMPPartCtrl::Play(int iTime) {
	for (int n = 0; n < m_iPartNum; ++n) {
		m_vecPartSys[n]->Play(iTime);
	}
	for (int n = 0; n < m_iStripNum; ++n) {
		m_pcStrip[n].Play();
	}
	for (int n = 0; n < m_iModelNum; n++) {
		m_vecModel[n]->Play();
	}
}

bool CMPPartCtrl::IsPlaying() {
	if (m_iPartNum > 0) {
		for (int n = 0; n < m_iPartNum; ++n) {
			if (m_vecPartSys[n]->IsPlaying())
				return true;
		}
	}
	else if (m_iStripNum > 0) {
		for (int n = 0; n < m_iStripNum; ++n) {
			if (m_pcStrip[n].IsPlaying())
				return true;
		}
	}
	else {
		for (int n = 0; n < m_iModelNum; n++) {
			if (m_vecModel[n]->IsPlaying())
				return true;
		}
	}
	return false;
}

void CMPPartCtrl::CopyPartCtrl(CMPPartCtrl* pPart) {
	m_strName = pPart->m_strName;

	m_vecPartSys.resize(pPart->m_iPartNum);

	for (int n = 0; n < pPart->m_iPartNum; ++n) {
		AddPartSys(pPart->m_vecPartSys[n]);
	}
	SAFE_DELETE_ARRAY(m_pcStrip);
	m_iStripNum = pPart->m_iStripNum;
	if (m_iStripNum) {
		m_pcStrip = new CMPStrip[m_iStripNum];
		for (int n = 0; n < m_iStripNum; ++n) {
			m_pcStrip[n].CopyStrip(&pPart->m_pcStrip[n]);
		}
	}
	for (int n = 0; n < m_iModelNum; n++) {
		SAFE_DELETE(m_vecModel[n]);
	}
	m_iModelNum = pPart->m_iModelNum;
	CChaModel* pModel;
	m_vecModel.resize(m_iModelNum);
	for (int n = 0; n < m_iModelNum; n++) {
		pModel = pPart->m_vecModel[n];
		m_vecModel[n] = new CChaModel;
		const std::string psID = std::format("{}", pModel->GetID());
		if (!m_vecModel[n]->LoadScript(psID)) {
			ToLogService("errors", LogLevel::Error, "LoadScript {}", psID);
		}
		m_vecModel[n]->SetVel(pModel->GetVel());
		m_vecModel[n]->SetPlayType(pModel->GetPlayType());
		m_vecModel[n]->SetCurPose(pModel->GetCurPose());
		m_vecModel[n]->SetCurColor(pModel->GetCurColor());
		m_vecModel[n]->SetSrcBlend(pModel->GetSrcBlend());
		m_vecModel[n]->SetDestBlend(pModel->GetDestBlend());
		m_vecModel[n]->PlayPose(0, PLAY_PAUSE);
	}
}

void CMPPartCtrl::BindingRes(CMPResManger* pResMagr) {
	m_pfDailTime = pResMagr->GetDailTime();
	for (int n = 0; n < m_iPartNum; ++n) {
		m_vecPartSys[n]->BindingRes(pResMagr);
	}
	for (int n = 0; n < m_iStripNum; ++n) {
		m_pcStrip[n].BindingRes(pResMagr);
	}
	for (int n = 0; n < m_iModelNum; n++) {
		m_vecModel[n]->m_pDev = pResMagr->m_pDev;
	}
}

CMPPartSys* CMPPartCtrl::AddPartSys(CMPPartSys* part) {
	m_iPartNum++;
	if (m_iPartNum > MAX_PART_SYS) {
		m_iPartNum = MAX_PART_SYS;
		return NULL;
	}
	m_vecPartSys.setsize(m_iPartNum);
	CopyPartSys(m_vecPartSys[m_iPartNum - 1], part);

	return m_vecPartSys[m_iPartNum - 1];
}

CMPPartSys* CMPPartCtrl::NewPartSys() {
	m_iPartNum++;
	if (m_iPartNum > MAX_PART_SYS) {
		m_iPartNum = MAX_PART_SYS;
		return NULL;
	}
	m_vecPartSys.setsize(m_iPartNum);
	return m_vecPartSys[m_iPartNum - 1];
}

void CMPPartCtrl::DeletePartSys(int iID) {
	if (m_iPartNum == 0)
		return;
	m_iPartNum--;
	m_vecPartSys.remove(iID);
}

void CMPPartCtrl::Clear() {
	m_iPartNum = 0;
	m_vecPartSys.clear();
}

void CMPPartCtrl::UpdateStrip() {
	for (int n = 0; n < m_iStripNum; ++n) {
		m_pcStrip[n].UpdateFrame();
	}
}

void CMPPartCtrl::FrameMove(DWORD dwDailTime) {
	for (int n = 0; n < m_iPartNum; ++n) {
		m_vecPartSys[n]->FrameMove(dwDailTime);
	}
	for (int n = 0; n < m_iStripNum; ++n) {
		m_pcStrip[n].FrameMove();
	}
	for (int n = 0; n < m_iModelNum; n++) {
		if (m_vecModel[n]->IsPlaying()) {
			m_vecModel[n]->UpdateMatrix();
			m_vecModel[n]->FrameMove();
		}
	}
}

void CMPPartCtrl::Render() {
	for (int n = 0; n < m_iPartNum; ++n) {
		m_vecPartSys[n]->Render();
	}
	for (int n = 0; n < m_iStripNum; ++n) {
		m_pcStrip[n].Render();
	}
	for (int n = 0; n < m_iModelNum; n++) {
		if (m_vecModel[n]->IsPlaying()) {
			m_vecModel[n]->Begin();
			m_vecModel[n]->Render();
			m_vecModel[n]->End();
		}
	}
}

bool CMPPartCtrl::SaveToFile(std::string_view pszName) {
	const std::string nameStr{pszName};
	FILE* t_pFile;
	t_pFile = fopen(nameStr.c_str(), "wb");
	if (!t_pFile) {
		ToLogService("errors", LogLevel::Error, " {},", pszName);
		return false;
	}

	//!
	DWORD t_dwVersion = CMPPartCtrl::ParVersion;
	fwrite(&t_dwVersion, sizeof(t_dwVersion), 1, t_pFile);

	// Имя partctrl — фиксированное 32-байтное поле в файле.
	char pszPartName[32]{};
	std::memcpy(pszPartName, m_strName.data(),
				std::min<std::size_t>(m_strName.size(), sizeof(pszPartName) - 1));
	fwrite(pszPartName, sizeof(char), 32, t_pFile);

	fwrite(&m_iPartNum, sizeof(int), 1, t_pFile);

	fwrite(&m_fLength, sizeof(float), 1, t_pFile);

	for (int n = 0; n < m_iPartNum; ++n) {
		m_vecPartSys[n]->SaveToFile(t_pFile);
	}
	fwrite(&m_iStripNum, sizeof(int), 1, t_pFile);
	for (int n = 0; n < m_iStripNum; ++n) {
		m_pcStrip[n].SaveToFile(t_pFile);
	}
	fwrite(&m_iModelNum, sizeof(int), 1, t_pFile);
	for (int n = 0; n < m_iModelNum; ++n) {
		m_vecModel[n]->SaveToFile(t_pFile);
	}
	fclose(t_pFile);
	return true;
}

bool CMPPartCtrl::LoadFromFile(std::string_view pszName) {
	const std::string nameStr{pszName};
	FILE* t_pFile;
	if ((t_pFile = fopen(nameStr.c_str(), "rb")) == NULL) {
		ToLogService("errors", LogLevel::Error, "[{}] was not opened.(CMPPartCtrl::LoadFromFile)", pszName);
		return false;
	}
	const std::string sName = nameStr;

	//!
	DWORD t_dwVersion;
	fread(&t_dwVersion, sizeof(t_dwVersion), 1, t_pFile);
	if (t_dwVersion > CMPPartCtrl::ParVersion) {
		ToLogService("errors", LogLevel::Error, "[{}][{}][{}] (CMPPartCtrl::LoadFromFile)", pszName, t_dwVersion,
					 CMPPartCtrl::ParVersion);
		return false;
	}
	if (t_dwVersion < 2) {
		ToLogService("errors", LogLevel::Error, "[{}][{}][{}] (CMPPartCtrl::LoadFromFile)", pszName, t_dwVersion, 2);
		return false;
	}

	char pszPartName[32];
	fread(pszPartName, sizeof(char), 32, t_pFile);
	m_strName = pszPartName;

	fread(&m_iPartNum, sizeof(int), 1, t_pFile);
#ifdef USE_GAME
	m_vecPartSys.resize(m_iPartNum);
#endif
	m_vecPartSys.setsize(m_iPartNum);

	if (t_dwVersion >= 3) {
		fread(&m_fLength, sizeof(float), 1, t_pFile);
	}
	else {
		m_fLength = 0;
	}
	for (int n = 0; n < m_iPartNum; ++n) {
		if (!m_vecPartSys[n]->LoadFromFile(t_pFile, t_dwVersion)) {
			ToLogService("errors", LogLevel::Error, "[{}][{}].(CMPPartCtrl::LoadFromFile)", pszName, n);
			return false;
		}
		if (t_dwVersion < 6) {
			m_vecPartSys[n]->SetPlayTime(m_fLength);
		}
	}
	if (t_dwVersion >= 7) {
		SAFE_DELETE_ARRAY(m_pcStrip);

		fread(&m_iStripNum, sizeof(int), 1, t_pFile);

		m_pcStrip = new CMPStrip[m_iStripNum];

		for (int n = 0; n < m_iStripNum; ++n) {
			if (!m_pcStrip[n].LoadFromFile(t_pFile, t_dwVersion)) {
				ToLogService("errors", LogLevel::Error, "[{}][{}]Strip.(CMPPartCtrl::LoadFromFile)", pszName, n);
				delete m_pcStrip;
				return false;
			}
		}
	}
	if (t_dwVersion >= 8) {
		fread(&m_iModelNum, sizeof(int), 1, t_pFile);
		m_vecModel.resize(m_iModelNum);
		for (int n = 0; n < m_iModelNum; ++n) {
			m_vecModel[n] = new CChaModel;
			m_vecModel[n]->LoadFromFile(t_pFile);
		}
	}
	fclose(t_pFile);
	return true;
}

void CMPPartCtrl::GetRes(CMPResManger* pResMagr, std::vector<INT>& vecTex, std::vector<INT>& vecModel,
						 std::vector<INT>& vecEff) {
	std::vector<INT>::iterator it;
	int id = -1;

	int n;
	int idtex, idmodel, ideff;
	s_string strName;
	if (m_iStripNum > 0) {
		for (n = 0; n < m_iStripNum; ++n) {
			strName = m_pcStrip[n].GetTexName();
			id = pResMagr->GetTextureID(strName);
			if (id >= 0) {
				it = std::find(vecTex.begin(), vecTex.end(), id);
				if (it == vecTex.end()) {
					vecTex.push_back(id);
					const std::string pszPath = std::format("texture/effect/{}", strName);
					const std::string pszNewPath = std::format("effect/new/texture/{}", strName);
					if (!::CopyFile(pszPath.c_str(), pszNewPath.c_str(), FALSE)) {
					}
				}
			}
		}
	}
	else if (m_iPartNum > 0) {
		for (n = 0; n < m_iPartNum; ++n) {
			m_vecPartSys[n]->GetRes(idtex, idmodel, ideff);
			if (idtex >= 0) {
				it = std::find(vecTex.begin(), vecTex.end(), idtex);
				if (it == vecTex.end()) {
					vecTex.push_back(idtex);
				}
			}
			if (idmodel >= 0) {
				it = std::find(vecModel.begin(), vecModel.end(), idmodel);
				if (it == vecModel.end()) {
					vecModel.push_back(idmodel);
				}
			}
			if (ideff >= 0) {
				it = std::find(vecEff.begin(), vecEff.end(), ideff);
				if (it == vecEff.end()) {
					vecEff.push_back(ideff);
				}
			}
		}
	}
}

void CMPPartCtrl::GetHitRes(CMPResManger* pResMagr, std::vector<s_string>& vecPar) {
	if (m_iStripNum > 0) {
		return;
	}
	else if (m_iPartNum > 0) {
		s_string strname;
		int id, n;
		for (n = 0; n < m_iPartNum; ++n) {
			strname = m_vecPartSys[n]->GetHitEff();

			id = pResMagr->GetPartCtrlID(strname);
			if (id >= 0) {
				vecPar.push_back(strname);
			}
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

static void keyframe_proc(DWORD type, DWORD pose_id, DWORD key_id, DWORD key_frame, void* param) {
	CChaModel* pCha = ((CChaModel*)param);
	switch (type) {
	case KEYFRAME_TYPE_BEGIN: {
		pCha->SetPlaying(true);
		break;
	}
	case KEYFRAME_TYPE_KEY: {
		break;
	}
	case KEYFRAME_TYPE_END:
		if (pCha->GetPlayType() != PLAY_LOOP) {
			pCha->SetPlaying(false);
		}
		break;
	}
}

bool CChaModel::LoadScript(const s_string& strModel) {
	int charType = std::stoi(strModel);
	_iID = charType;

	const CCharacterModelInfo* modelInfo = CharacterModelStore::Instance()->Get(charType);
	if (!modelInfo) {
		return false;
	}
	if (modelInfo->_bone.empty()) {
		return false;
	}

	MPChaLoadInfo sModel;

	auto copyFixed = [](char* dst, size_t cap, std::string_view src) {
		size_t n = (std::min)(src.size(), cap - 1);
		std::memcpy(dst, src.data(), n);
		dst[n] = '\0';
	};

	copyFixed(sModel.bone, sizeof(sModel.bone), modelInfo->_bone);
	for (int i = 0; i < 5; i++) {
		copyFixed(sModel.part[i], sizeof(sModel.part[i]), modelInfo->_skins[i]);
	}

	if (!LoadChaModel(sModel)) {
		return false;
	}

	const SChaAction* pAction = CCharacterActionCache::_cache.GetCharAction(charType);
	if (!pAction) {
		return false;
	}
	if (!LoadPose(*pAction)) {
		return false;
	}
	SetPoseKeyFrameProc(keyframe_proc, (void*)this);

	return true;
}


bool CChaModel::LoadChaModel(MPChaLoadInfo& info) {
	if (info.bone[0] == '\0')
		return false;
	if (HRESULT hr = MPCharacter::Load(&info); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] MPCharacter::Load failed: bone='{}', hr=0x{:08X}",
					 __FUNCTION__, info.bone, static_cast<std::uint32_t>(hr));
		return false;
	}
	return true;
}

bool CChaModel::LoadPose(const SChaAction& SCharAct) {
	lwPoseInfo pi;
	memset(&pi, 0, sizeof(pi));
	for (const auto& action : SCharAct._actions) {
		if (!action.IsValid()) {
			continue;
		}
		pi.start = action._startFrame;
		pi.end = action._endFrame;
		const size_t kfCount = (std::min)(action._keyFrames.size(), static_cast<size_t>(MAX_KEY_FRAME_NUM));
		pi.key_frame_num = static_cast<DWORD>(kfCount);
		for (size_t k = 0; k < kfCount; k++) {
			pi.key_frame_seq[k] = action._keyFrames.at(k);
		}

		if (GetPoseCtrl()) {
			GetPoseCtrl()->InsertPose(action._actionNo, &pi, 1);
		}
	}
	return true;
}

void CChaModel::PlayPose(DWORD id, DWORD type) {
	MPCharacter::PlayPose(id, type, 0.0f, _fVel);
}

void CChaModel::Begin() {
	m_pDev->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDev->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	m_pDev->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDev->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDev->GetDevice()->SetRenderState(D3DRS_SRCBLEND, _eSrcBlend);
	m_pDev->GetDevice()->SetRenderState(D3DRS_DESTBLEND, _eDestBlend);
	D3DMATERIALX mtl;
	mtl.Ambient = (D3DCOLORVALUE)_dwCurColor;
	mtl.Diffuse = (D3DCOLORVALUE)_dwCurColor;
	SetMaterial(&mtl);
	SetOpacity(_dwCurColor.a);
}

void CChaModel::End() {
	m_pDev->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
}

void CChaModel::SaveToFile(FILE* file) {
	fwrite(&_iID, sizeof(int), 1, file);
	fwrite(&_fVel, sizeof(float), 1, file);
	fwrite(&_iPlayType, sizeof(int), 1, file);
	fwrite(&_iCurPose, sizeof(int), 1, file);

	int eblend = (int)_eSrcBlend;
	fwrite(&eblend, sizeof(int), 1, file);
	eblend = (int)_eDestBlend;
	fwrite(&eblend, sizeof(int), 1, file);
	fwrite(&_dwCurColor, sizeof(D3DXCOLOR), 1, file);
}

void CChaModel::LoadFromFile(FILE* file) {
	fread(&_iID, sizeof(int), 1, file);
	fread(&_fVel, sizeof(float), 1, file);
	fread(&_iPlayType, sizeof(int), 1, file);
	fread(&_iCurPose, sizeof(int), 1, file);

	int eblend;
	fread(&eblend, sizeof(int), 1, file);
	_eSrcBlend = (D3DBLEND)eblend;
	fread(&eblend, sizeof(int), 1, file);
	_eDestBlend = (D3DBLEND)eblend;
	fread(&_dwCurColor, sizeof(D3DXCOLOR), 1, file);

	const std::string psID = std::format("{}", _iID);
	LoadScript(psID);
	SetVel((int)(_fVel * 1000));
	SetPlayType(_iPlayType);
	SetCurPose(_iCurPose);
	PlayPose(_iCurPose, PLAY_PAUSE);
}


bool CMPLink::Create(MPCharacter* pChaMain, int iDummy1, MPCharacter* pChaTag, int iDummy2,
					 std::string_view pszTex, int iTexNum, CMPResManger* pResMgr, D3DXVECTOR3* pEyePos,
					 MPRender* pDev) {
	m_pDev = pDev;
	_pCEffFile = pResMgr->GetEffectFile();
	_fDailTime = pResMgr->GetDailTime();
	_pEyePos = pEyePos;

	_vStart = D3DXVECTOR3(0, 0, 0);
	_vEnd = D3DXVECTOR3(10, 0, 0);
	_vdir = _vEnd - _vStart;
	_fdist = D3DXVec3Length(&_vdir);
	D3DXVec3Normalize(&_vdir, &_vdir);


	int n;

	_pFrame = new MPFrame[LINK_FACE];
	for (n = 0; n < LINK_FACE; n++) {
		_pFrame[n].vPos1.m_dwDiffuse = 0xffffffff;
		_pFrame[n].vPos2.m_dwDiffuse = 0xffffffff;
	}

	_pChaMain = pChaMain;
	_pChaTag = pChaTag;
	_iDummy1 = iDummy1;
	_iDummy2 = iDummy2;

	_pTex = new lwITex*[iTexNum];
	int id;
	for (n = 0; n < iTexNum; n++) {
		_pTex[n] = NULL;
		const s_string strName = std::format("{}{}", pszTex, n);
		id = pResMgr->GetTextureID(strName);
		if (id < 0)
			return false;
		_pTex[n] = pResMgr->GetTextureByIDlw(id);
	}
	return true;
}

void CMPLink::ColArc(float fradius) {
	D3DXVECTOR3 veyedir, vcross;
	const auto v = *_pEyePos - _vStart;

	D3DXVec3Normalize(&veyedir, &v);
	D3DXVec3Cross(&vcross, &_vdir, &veyedir);
	D3DXVec3Normalize(&vcross, &vcross);


	D3DXVECTOR3 vOrg(-fradius, 0, 0);
	D3DXVECTOR3 vTemp;


	D3DXMATRIX mat;
	int n;
	int num = 10;
	float fstep = (D3DX_PI / 2.0f) / float(num);
	float fhei;
	float ft;
	for (n = 0; n < num; ++n) {
		D3DXMatrixRotationY(&mat, n * fstep);
		D3DXVec3TransformCoord(&vTemp, &vOrg, &mat);
		fhei = vTemp.z;

		ft = fradius + vTemp.x;
		if (ft < 0.00001f) {
			ft = 0.0f;
		}
		vTemp = _vStart + _vdir * ft;
		vTemp.z = fhei;

		ft = ft / _fdist;


		_pFrame[n].vPos1.m_SPos = vTemp + (-vcross * 1.0f);
		_pFrame[n].vPos2.m_SPos = vTemp + (vcross * 1.0f);

		if (n == 0) {
			_pFrame[n].vPos1.m_SUV.x = 0;
			_pFrame[n].vPos1.m_SUV.y = 1;

			_pFrame[n].vPos2.m_SUV.x = 0;
			_pFrame[n].vPos2.m_SUV.y = 0;
		}
		else {
			_pFrame[n].vPos1.m_SUV.x = ft;
			_pFrame[n].vPos1.m_SUV.y = 1;

			_pFrame[n].vPos2.m_SUV.x = ft;
			_pFrame[n].vPos2.m_SUV.y = 0;
		}
	}
	_pFrame[num].vPos1.m_SPos = _vEnd + (-vcross * 1.0f);
	_pFrame[num].vPos2.m_SPos = _vEnd + (vcross * 1.0f);

	_pFrame[num].vPos1.m_SUV.x = 1;
	_pFrame[num].vPos1.m_SUV.y = 1;

	_pFrame[num].vPos2.m_SUV.x = 1;
	_pFrame[num].vPos2.m_SUV.y = 0;
}

void CMPLink::GetPhysique() {
	_vStart = D3DXVECTOR3(0, 0, 0);
	_vEnd = D3DXVECTOR3(10, 0, 0);

	_vdir = _vEnd - _vStart;
	_fdist = D3DXVec3Length(&_vdir);
	D3DXVec3Normalize(&_vdir, &_vdir);


	float frad = 3.0f;
	if (_fdist > frad * 2) {
		_fRadius = frad - (frad * ((_fdist / frad - 1.0f) / 10.0f));
	}
	else {
		_fRadius = frad + ((frad - _fdist) / 10.0f);
	}
	ColArc(_fRadius);
}


void CMPLink::FrameMove() {
	GetPhysique();

	_fCurTime += *_fDailTime;
	if (_fCurTime > 0.15f) {
		_iCurTex++;
		if (_iCurTex >= 4)
			_iCurTex = 0;
		_fCurTime = 0;
	}
}

void CMPLink::Render() {
	_pCEffFile->SetTechnique(0);
	_pCEffFile->Begin();
	_pCEffFile->Pass(0);


	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	m_pDev->SetTransformWorld(&mat);
	m_pDev->SetTexture(0, _pTex[_iCurTex]->GetTex());
	m_pDev->SetVertexShader(NULL);
	m_pDev->SetFVF(LINK_FVF);
	m_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
										 20, _pFrame, sizeof(LinkVer));

	_pCEffFile->End();
}

void CMPPartCtrl::SetSkillCtrl(SkillCtrl* pCtrl) {
	for (int n = 0; n < m_iPartNum; ++n) {
		if (m_vecPartSys[n]) {
			m_vecPartSys[n]->SetSkillCtrl(pCtrl);
		}
	}
}

void CMPPartCtrl::SetAlpha(float falpha) {
	for (int n = 0; n < m_iPartNum; ++n) {
		if (m_vecPartSys[n]) {
			m_vecPartSys[n]->SetAlpha(falpha);
		}
	}
}

void CMPPartCtrl::Reset() {
	m_fCurTime = 0;
	const auto v = D3DXVECTOR3(0, 0, 0);
	MoveTo(&v);
	for (int n = 0; n < m_iPartNum; ++n) {
		if (m_vecPartSys[n]) {
			m_vecPartSys[n]->Reset(false);
			m_vecPartSys[n]->unFontEffCom();
		}
	}
}

void CMPPartCtrl::Stop() {
	Reset();
	for (int n = 0; n < m_iPartNum; ++n) {
		if (m_vecPartSys[n]) {
			m_vecPartSys[n]->Stop();
		}
	}
}

void CMPPartCtrl::End() {
	for (int n = 0; n < m_iPartNum; ++n) {
		if (m_vecPartSys[n]) {
			m_vecPartSys[n]->End();
		}
	}
}

void CMPPartCtrl::MoveTo(const D3DXVECTOR3* vPos, MPMap* pmap) {
	for (auto n = 0; n < m_iPartNum; ++n) {
		if (m_vecPartSys[n]) {
			m_vecPartSys[n]->MoveTo(vPos, pmap);
		}
	}
	for (auto n = 0; n < m_iModelNum; n++) {
		if (m_vecModel[n]->IsPlaying()) {
			m_vecModel[n]->MoveTo(vPos);
		}
	}
}

void CMPPartCtrl::BindingBone(D3DXMATRIX* pMatBone) {
	for (auto n = 0; n < m_iPartNum; ++n) {
		if (m_vecPartSys[n]) {
			m_vecPartSys[n]->BindingBone(pMatBone);
		}
	}
	for (auto n = 0; n < m_iModelNum; n++) {
		if (m_vecModel[n]->IsPlaying()) {
			m_vecModel[n]->BindingBone(pMatBone);
		}
	}
}
