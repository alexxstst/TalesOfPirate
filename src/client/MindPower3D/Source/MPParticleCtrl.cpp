#include "StdAfx.h"
//#include <mindpower.h>
#include "GlobalInc.h"
#include "MPModelEff.h"
#include "MPRender.h"
#include ".\mpparticlectrl.h"

using namespace std;

///************************************************************************/
///*                                                                      */
///************************************************************************/
//CMPParticleCtrl::CMPParticleCtrl(void)
//{
//}
//
//CMPParticleCtrl::~CMPParticleCtrl(void)
//{
//}
///************************************************************************/
///*                                                                      */
///************************************************************************/
//
//CMPParticleTrace::CMPParticleTrace(void)
//{
//	_pCModelEff = NULL;
//	_pCShade	= NULL;
//	m_CFire = NULL;
//	m_CBlast = NULL;
//}
//
//CMPParticleTrace::~CMPParticleTrace(void)
//{
//	SAFE_DELETE(_pCModelEff);
//	SAFE_DELETE(_pCShade);
//
//	SAFE_DELETE(m_CFire);
//	SAFE_DELETE(m_CBlast);
//
//}
////float f = 0;
//
//void	CMPParticleTrace::BindingRes(CMPResManger* pResMagr)
//{
//	m_CFire = new CMPFire;
//
//	m_CFire->Init("",5,"Flare.tga",MESH_PLANERECT,D3DXVECTOR2(0,0),5,true);
//	m_CFire->BindingRes(pResMagr);
//	m_CFire->setParticleVel(10);
//	m_CFire->setParticleLife(10);
//	m_CFire->setParticleSizeSmall(1.0f);
//	m_CFire->SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_ONE);
//	m_CFire->setParticleColor(0xfffff080);
//	//m_CFire->setParticleFrameColor(m_CFire->getFrameCount() - 1,0x00000000);
//
//
//	m_CBlast = new CMPBlast;
//	m_CBlast->Init("",10,"Flare.tga",MESH_PLANERECT,D3DXVECTOR2(1.0f,1.0f),5,true);
//	m_CBlast->BindingRes(pResMagr);
//	m_CBlast->setParticleColor(0xffff7000);
//
//	m_bEmiss	= false;
//	m_pfDailTime = pResMagr->GetDailTime();
//	m_vPos		= D3DXVECTOR3(0,0,0);
//	m_vDir		= D3DXVECTOR3(0,0,0);
//	m_fVel		= 7.0f;
//	m_fRadius = 0.2f;
//
//	_pCModelEff = new CMPModelEff;
//	s_string str = "ballfire.eff";
//	int id = pResMagr->GetEffectID(str);
//	_pCModelEff->BindingEffect(pResMagr->GetEffectByID(id));
//	_pCModelEff->BindingRes(pResMagr);
//	_pCModelEff->Scaling(2.f,2.f,2.f);
//
//	//
//	_pCShade	= new CMPShadeMap;
//	_pCShade->setTextureName("boxlight2.tga");
//	_pCShade->BoundingRes(pResMagr);
//	_pCShade->CreateShadeMap(4.0f);
//	//_pCShade->CreateSpliteTexture(4,1);
//	_pCShade->SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_ONE);
//	_pCShade->SetColor(0xffff8000);
//	//_pCShade->Play();
//
//}
//void	CMPParticleTrace::MoveTo(D3DXVECTOR3 vPos,MPMap* pMap)
//{
//	if(m_bEmiss)
//	{
//		_pCModelEff->MoveTo(m_vPos.x,m_vPos.y,m_vPos.z);
//
//		_pCShade->MoveTo(m_vPos,pMap);
//	}
//}
//
//void	CMPParticleTrace::FrameMove(DWORD	dwDailTime)
//{
//	if(m_bEmiss)
//	{
//
//		//! ID
//
//		//SetTarget(D3DXVECTOR3(f,20,0));
//
//		m_CFire->MoveTo(m_vPos);
//
//		m_CFire->FrameMove(dwDailTime);
//
//
//		_pCModelEff->FrameMove(dwDailTime);
//	
//		_pCShade->FrameMove(dwDailTime);
//
//	}
//	m_CBlast->FrameMove(dwDailTime);
//
//}
//
//void	CMPParticleTrace::Render()
//{
//
//	if(m_bEmiss)
//	{
//		m_CFire->Render();
//		_pCModelEff->Render();
//		_pCShade->Render();
//
//	}
//	m_CBlast->Render();
//}
//
//void	CMPParticleTrace::Emission(WORD wID, D3DXVECTOR3 vBegin, D3DXVECTOR3 vEnd)
//{
//	if(m_bEmiss)
//		return;
//	m_wID = wID;
//
//	m_CPath[0] = vBegin;
//	m_CPath[1] = vEnd;
//	m_vPos		= vBegin;

//
//	m_CFire->Reset(false);
//	m_CFire->MoveTo(vBegin);
//
//	m_vDir = vEnd - vBegin;
//	m_fDist = D3DXVec3LengthSq(&m_vDir);
//
//	D3DXVec3Normalize(&m_vDir,&m_vDir);
//	m_bEmiss	= true;
//
//	_pCModelEff->Play();
//	_pCShade->Show(TRUE);
//
//	_bHit = false;
//	//f = 0;
//}
//void	CMPParticleTrace::Blast()
//{
//		m_bEmiss	= false;
//		m_CBlast->MoveTo(m_vPos);
//		m_CBlast->Play();
//
//		_pCModelEff->Stop();
//		_pCShade->Show(FALSE);
//
//		_bHit =  true;
//}
//
//void	CMPParticleTrace::SetTarget(D3DXVECTOR3 vTarget)
//{
//	float fDist  = m_fVel * *m_pfDailTime;
//	m_vPos	 += m_vDir * fDist;
//	if(PointInstrPointRange(&m_vPos,&m_CPath[1] ,m_fRadius))
//	{
//		//m_vPos.z -= 0.9f;
//		Blast();
//		return;
//	}
//	//!
//	if(vTarget  == m_CPath[1])
//		return;
//	//!
//	m_vTargDir = vTarget - m_CPath[1];
//	//!
//	m_fTargDist = D3DXVec3LengthSq(&m_vTargDir);
//	D3DXVec3Normalize(&m_vTargDir, &m_vTargDir);
//
//	//!.
//	//	 =  /  * 
//	float flerp = (m_fTargDist / m_fDist) * fDist;
//	//m_fDist = fDist;//D3DXVec3Length(&(m_vPos - m_vOldPos));
//
//	//!
//	m_vPos	 += m_vTargDir * flerp;
//	//!
//	m_CPath[1] = vTarget;
//	m_vDir = vTarget - m_vPos;
//	m_fDist = D3DXVec3LengthSq(&m_vDir);
//	D3DXVec3Normalize(&m_vDir,&m_vDir);
//}
//
///************************************************************************/
///*                                                                      */
///************************************************************************/
//
//CMPParticleRipple::CMPParticleRipple(void)
//{
//}
//
//CMPParticleRipple::~CMPParticleRipple(void)
//{
//	SAFE_DELETE(m_CRippleStop);
//	SAFE_DELETE(m_CRippleMove);
//}
//
//void	CMPParticleRipple::BindingRes(CMPResManger* pResMagr)
//{
//
//	m_CRippleStop = new CMPRipple;
//	m_CRippleStop->Init("",5,"Ripple.tga",MESH_PLANERECT,D3DXVECTOR2(0.f,0.f),4,false);
//	m_CRippleStop->BindingRes(pResMagr);
//	m_CRippleStop->setParticleLife(100);
//	m_CRippleStop->SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA);
//	m_CRippleStop->setParticleSize(1.0f);
//	//m_CRipple->Play();
//
//	m_CRippleMove = new CMPRipple;
//	m_CRippleMove->Init("2",13,"Ripple2.tga",MESH_PLANERECT,D3DXVECTOR2(0.f,0.f),4,false);
//	m_CRippleMove->BindingRes(pResMagr);
//	m_CRippleMove->SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA);
//	m_CRippleMove->setParticleLife(200);
//	m_CRippleMove->setParticleSize(2.0f);
//
//
//	//m_CRipple.Play();
//
//	m_pfDailTime = pResMagr->GetDailTime();
//	//m_vPos		= D3DXVECTOR3(0,0,0);
//}
//
//void	CMPParticleRipple::FrameMove(DWORD	dwDailTime)
//{
//
//		//! ID
//
//
//		//SetTarget(D3DXVECTOR3(0,20,0));
//
//	m_CRippleStop->FrameMove(dwDailTime);
//
//	m_CRippleMove->FrameMove(dwDailTime);
//}
//
//void	CMPParticleRipple::Render()
//{
//	m_CRippleStop->Render();
//	m_CRippleMove->Render();
//}
//
//void	CMPParticleRipple::Emission(WORD wID, D3DXVECTOR3 vBegin, D3DXVECTOR3 vEnd)
//{
//	//m_wID = wID;
//	Walk();
//}
//
//void	CMPParticleRipple::SetTarget(D3DXVECTOR3 vTarget, D3DXVECTOR3 vTarget2,float fAngle)
//{	
//	m_CRippleMove->SetAngle(fAngle);
//	m_CRippleStop->SetAngle(fAngle);
//	if(m_vPos == vTarget)
//		return;
//	m_vPos = vTarget;
//	m_CRippleMove->MoveTo(vTarget2);
//	m_CRippleStop->MoveTo(vTarget);
//}
//void	CMPParticleRipple::Walk()
//{
//	//if(!m_CRippleMove->IsPlaying())
//		m_CRippleMove->Play();
//	m_CRippleStop->Stop();
//	//m_CRippleStop->Reset(m_vPos);
//
//}
//void	CMPParticleRipple::Stop()
//{
//	m_CRippleMove->Stop();
//	//m_CRippleMove->Reset(m_vPos);
//
//	//if(!m_CRippleStop->IsPlaying())
//		m_CRippleStop->Play();
//}


/************************************************************************/
/* class CMPPartCtrl*/
/************************************************************************/
CMPPartCtrl::CMPPartCtrl(void)
{
	m_strName = "";
	m_iPartNum = 0;
#ifndef USE_GAME
	//m_vecPartSys.resize(MAX_PART_SYS);//6
#endif
	m_fLength = 0;//0
	m_fCurTime = 0;

	m_iStripNum = 0;
	m_pcStrip = NULL;

	m_iModelNum = 0;
	//m_iShadeNum = 0;;
}
CMPPartCtrl::~CMPPartCtrl(void)
{
	SAFE_DELETE_ARRAY(m_pcStrip);
	for (int n = 0; n <m_iModelNum; n++ )
	{
		SAFE_DELETE(m_vecModel[n]);
	}
	m_vecModel.clear();
	//m_vecShade.clear();
}

void	CMPPartCtrl::SetStripCharacter(MPCharacter* pCha)
{
	for ( int n = 0; n < m_iStripNum; ++n)
	{
		m_pcStrip[n].AttachCharacter(pCha);
	}
}
void	CMPPartCtrl::SetStripItem(MPSceneItem* pItem,bool bloop)
{
	for ( int n = 0; n < m_iStripNum; ++n)
	{
		m_pcStrip[n].AttachItem(pItem);
		m_pcStrip[n].SetLoop(bloop);
	}
}

void	CMPPartCtrl::SetItemDummy(MPSceneItem* pItem, int dummy1, int dummy2)
{
	for (int n = 0; n < m_iPartNum; ++n)
	{
		m_vecPartSys[n]->SetItemDummy(pItem,dummy1,dummy2);
	}
}


CChaModel*	CMPPartCtrl::NewModel(const s_string& strID,CMPResManger* pResMagr)
{
	m_iModelNum++;
	if( m_iModelNum > 1)
	{
		m_iModelNum = 1;
		return NULL;
	}
	m_vecModel.resize(m_iModelNum);

	m_vecModel[m_iModelNum-1] = new CChaModel;
	m_vecModel[m_iModelNum-1]->m_pDev = pResMagr->m_pDev;

	m_vecModel[m_iModelNum-1]->LoadScript(strID);
	m_vecModel[m_iModelNum-1]->SetVel(120);
	m_vecModel[m_iModelNum-1]->SetPlayType(PLAY_LOOP);
	m_vecModel[m_iModelNum-1]->SetCurPose(1);
	m_vecModel[m_iModelNum-1]->PlayPose(0,PLAY_PAUSE);

	return m_vecModel[m_iModelNum-1];
}
CChaModel*	CMPPartCtrl::GetModel(int iIdx)
{
	return m_vecModel[iIdx];
}

CMPStrip*	CMPPartCtrl::NewStrip(const s_string& strTex,CMPResManger* pResMagr)
{
	SAFE_DELETE_ARRAY(m_pcStrip);

	m_iStripNum = 1;
	m_pcStrip = new CMPStrip[m_iStripNum];
	m_pcStrip[0].CreateStrip(pResMagr->m_pDev,strTex,pResMagr);
	return &m_pcStrip[0];
}

CMPStrip*	CMPPartCtrl::GetStrip(int iIdx)
{ 
	return &m_pcStrip[iIdx];
}

void	CMPPartCtrl::SetPlayType(int nType)
{
	if(nType == 0)//
	{
		for (int n = 0; n < m_iPartNum; ++n)
		{
			m_vecPartSys[n]->SetPlayTime(0);
			//m_vecPartSys[n]->SetDelayTime(0);
			m_vecPartSys[n]->SetLoop(true);
		}
		for (int n = 0; n <m_iModelNum; n++ )
		{
			m_vecModel[n]->SetPlayType(PLAY_LOOP);
			m_vecModel[n]->Play();
		}
	//}else if(nType == 1)//
	//{
	//	for (int n = 0; n < m_iPartNum; ++n)
	//	{
	//		m_vecPartSys[n]->SetPlayTime(0);
	//		m_vecPartSys[n]->SetDelayTime(0); 
	//		m_vecPartSys[n]->SetLoop(false);
	//	}
	//}else if(nType == 2)//
	//{
	//	for (int n = 0; n < m_iPartNum; ++n)
	//	{
	//		m_vecPartSys[n]->SetLoop(true);
	//	}
	}
}


void	CMPPartCtrl::Play(int iTime)
{

	for (int n = 0; n < m_iPartNum; ++n)
	{
		m_vecPartSys[n]->Play(iTime);
	}
	for (int n = 0; n < m_iStripNum; ++n)
	{
		m_pcStrip[n].Play();
	}
	for (int n = 0; n <m_iModelNum; n++ )
	{
		m_vecModel[n]->Play();
	}
}
bool	CMPPartCtrl::IsPlaying()
{
	if(m_iPartNum>0)
	{
		for (int n = 0; n < m_iPartNum; ++n)
		{
			if(m_vecPartSys[n]->IsPlaying())
				return true;
		}
		//Reset();
	}else if(m_iStripNum>0)
	{
		for (int n = 0; n < m_iStripNum; ++n)
		{
			if(m_pcStrip[n].IsPlaying())
				return true;
		}
	}else
	{
		for (int n = 0;n <m_iModelNum; n++)
		{
			if(m_vecModel[n]->IsPlaying())
				return true;
		}
	}
	return false;
}
void	CMPPartCtrl::CopyPartCtrl(CMPPartCtrl* pPart)
{
	m_strName = pPart->m_strName;

	m_vecPartSys.resize(pPart->m_iPartNum);
 
	for (int n = 0; n < pPart->m_iPartNum; ++n)
	{
		AddPartSys(pPart->m_vecPartSys[n]);
	}
	SAFE_DELETE_ARRAY(m_pcStrip);
	m_iStripNum = pPart->m_iStripNum;
	if(m_iStripNum)
	{
		m_pcStrip = new CMPStrip[m_iStripNum];
		for (int n = 0; n < m_iStripNum; ++n)
		{
			m_pcStrip[n].CopyStrip(&pPart->m_pcStrip[n]);
		}
	}
	for (int n = 0; n <m_iModelNum; n++ )
	{
		SAFE_DELETE(m_vecModel[n]);
	}
	m_iModelNum = pPart->m_iModelNum;
	CChaModel* pModel;
	char psID[32];
	m_vecModel.resize(m_iModelNum);
	for (int n = 0; n <m_iModelNum; n++ )
	{
		pModel = pPart->m_vecModel[n];
		m_vecModel[n] = new CChaModel;
		sprintf(psID,"%d",pModel->GetID());
		if(!m_vecModel[n]->LoadScript(psID))
		{
			ToLogService("error", "msgLoadScript {}", psID);
		}
		m_vecModel[n]->SetVel(pModel->GetVel());
		m_vecModel[n]->SetPlayType(pModel->GetPlayType());
		m_vecModel[n]->SetCurPose(pModel->GetCurPose());
		m_vecModel[n]->SetCurColor(pModel->GetCurColor());
		m_vecModel[n]->SetSrcBlend(pModel->GetSrcBlend());
		m_vecModel[n]->SetDestBlend(pModel->GetDestBlend());
		m_vecModel[n]->PlayPose(0,PLAY_PAUSE);
	}
}

void	CMPPartCtrl::BindingRes(CMPResManger* pResMagr)
{
	m_pfDailTime = pResMagr->GetDailTime();
	for (int n = 0; n < m_iPartNum; ++n)
	{
		m_vecPartSys[n]->BindingRes(pResMagr);
	}
	for ( int n = 0; n < m_iStripNum; ++n)
	{
		m_pcStrip[n].BindingRes(pResMagr);
	}
	for (int n = 0; n <m_iModelNum; n++ )
	{
		m_vecModel[n]->m_pDev = pResMagr->m_pDev;
	}
}

CMPPartSys*		CMPPartCtrl::AddPartSys(CMPPartSys* part)
{
	m_iPartNum++;
	if( m_iPartNum > MAX_PART_SYS)
	{
		m_iPartNum = MAX_PART_SYS;
		return NULL;
	}
	m_vecPartSys.setsize(m_iPartNum);
	CopyPartSys(m_vecPartSys[m_iPartNum-1],part);

	//Reset();
	return m_vecPartSys[m_iPartNum-1];
}

CMPPartSys*	CMPPartCtrl::NewPartSys()
{
	m_iPartNum++;
	if( m_iPartNum > MAX_PART_SYS)
	{
		m_iPartNum = MAX_PART_SYS;
		return NULL;
	}
	m_vecPartSys.setsize(m_iPartNum);
	return m_vecPartSys[m_iPartNum-1];
}
void	CMPPartCtrl::DeletePartSys(int iID)
{
	if(m_iPartNum == 0)
		return;
	m_iPartNum--;
	m_vecPartSys.remove(iID);
}

void	CMPPartCtrl::Clear()
{
	m_iPartNum = 0;
	m_vecPartSys.clear();
}
void CMPPartCtrl::UpdateStrip()
{
	for ( int n = 0; n < m_iStripNum; ++n)
	{
		m_pcStrip[n].UpdateFrame();
	}
}
void	CMPPartCtrl::FrameMove(DWORD	dwDailTime)
{
	for (int n = 0; n < m_iPartNum; ++n)
	{
		m_vecPartSys[n]->FrameMove(dwDailTime);
	}
	for (int n = 0; n < m_iStripNum; ++n)
	{
		m_pcStrip[n].FrameMove();
	}
	for (int n = 0; n <m_iModelNum; n++ )
	{
		if(m_vecModel[n]->IsPlaying())
		{
			m_vecModel[n]->UpdateMatrix();
			m_vecModel[n]->FrameMove();
		}
	}
}
void	CMPPartCtrl::Render()
{
	for (int n = 0; n < m_iPartNum; ++n)
	{
		m_vecPartSys[n]->Render();
	}
	for (int n = 0; n < m_iStripNum; ++n)
	{
		m_pcStrip[n].Render();
	}
	for (int n = 0; n <m_iModelNum; n++ )
	{
		if(m_vecModel[n]->IsPlaying())
		{
			m_vecModel[n]->Begin();
			m_vecModel[n]->Render();
			m_vecModel[n]->End();
		}
	}
}

bool	CMPPartCtrl::SaveToFile(char* pszName)
{
	if (strcmp(pszName, "no") == 0 || strcmp(pszName, "yes") == 0)
	{
		int i;
		i=1;
	}

	FILE* t_pFile;
	t_pFile = fopen(pszName, "wb");
	if(!t_pFile)
	{
		ToLogService("error", "msg {},", pszName);
		return false;
	}

	//!
	DWORD t_dwVersion = CMPPartCtrl::ParVersion;
	fwrite(&t_dwVersion,sizeof(t_dwVersion),1,t_pFile);

	char pszPartName[32];
	lstrcpy(pszPartName,m_strName.c_str());
	fwrite(pszPartName,sizeof(char),32,t_pFile);

	fwrite(&m_iPartNum,sizeof(int),1,t_pFile);

	fwrite(&m_fLength,sizeof(float),1,t_pFile);

	for (int n = 0; n < m_iPartNum; ++n)
	{
		m_vecPartSys[n]->SaveToFile(t_pFile);
	}
	//////////////////////////////////////////////////////////////////////////
	fwrite(&m_iStripNum,sizeof(int),1,t_pFile);
	for (int n = 0; n < m_iStripNum; ++n)
	{
		m_pcStrip[n].SaveToFile(t_pFile);
	}
	fwrite(&m_iModelNum,sizeof(int),1,t_pFile);
	for (int n = 0; n < m_iModelNum; ++n)
	{
		m_vecModel[n]->SaveToFile(t_pFile);
	}
	fclose(t_pFile);
	return true;
}
bool	CMPPartCtrl::LoadFromFile(char* pszName)
{
	FILE* t_pFile;
	;
	if ((t_pFile = fopen(pszName, "rb")) == NULL)
	{
		ToLogService("error", "msg[{}] was not opened.(CMPPartCtrl::LoadFromFile)", pszName);
		return false;
	}
	if (strcmp(pszName, "no") == 0 || strcmp(pszName, "yes") == 0)
	{
		int i;
		i=1;
	}
	string sName = pszName;

	//!
	DWORD t_dwVersion;
	fread(&t_dwVersion,sizeof(t_dwVersion),1,t_pFile);
	if ( t_dwVersion > CMPPartCtrl::ParVersion)
	{
		ToLogService("error", "msg[{}][{}][{}] (CMPPartCtrl::LoadFromFile)", pszName, t_dwVersion, CMPPartCtrl::ParVersion);
		return false;
	}
	if(t_dwVersion < 2)
	{
		ToLogService("error", "msg[{}][{}][{}] (CMPPartCtrl::LoadFromFile)", pszName, t_dwVersion, 2);
		return false;
	}

	char pszPartName[32];
	fread(pszPartName,sizeof(char),32,t_pFile);
	m_strName = pszPartName;

	fread(&m_iPartNum,sizeof(int),1,t_pFile);
#ifdef USE_GAME
	m_vecPartSys.resize(m_iPartNum);
#endif
	m_vecPartSys.setsize(m_iPartNum);

	if(t_dwVersion >= 3)
	{
		fread(&m_fLength,sizeof(float),1,t_pFile);
	}
	else
	{
		m_fLength = 0;
	}
	for (int n = 0; n < m_iPartNum; ++n)
	{
		if (!m_vecPartSys[n]->LoadFromFile(t_pFile,t_dwVersion))
		{
			ToLogService("error", "msg[{}][{}].(CMPPartCtrl::LoadFromFile)", pszName, n);
			return false;
		}
		if(t_dwVersion < 6)
		{
			m_vecPartSys[n]->SetPlayTime(m_fLength);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if(t_dwVersion >= 7)
	{
		SAFE_DELETE_ARRAY(m_pcStrip);

		fread(&m_iStripNum,sizeof(int),1,t_pFile);

		m_pcStrip = new CMPStrip[m_iStripNum];

		for (int n = 0; n < m_iStripNum; ++n)
		{
			if (!m_pcStrip[n].LoadFromFile(t_pFile,t_dwVersion))
			{
				ToLogService("error", "msg[{}][{}]Strip.(CMPPartCtrl::LoadFromFile)", pszName, n);
				delete m_pcStrip;
				return false;
			}
		}
	}
	if(t_dwVersion >= 8)
	{
		fread(&m_iModelNum,sizeof(int),1,t_pFile);
		m_vecModel.resize(m_iModelNum);
		for (int n = 0; n < m_iModelNum; ++n)
		{
			m_vecModel[n] = new CChaModel;
			m_vecModel[n]->LoadFromFile(t_pFile);
		}
	}
	fclose(t_pFile);
	return true;
}

bool	CMPPartCtrl::LoadFromMemory(CMemoryBuf*	pbuf)
{
	//!
	DWORD t_dwVersion;
	pbuf->mread(&t_dwVersion,sizeof(t_dwVersion),1);
	if ( t_dwVersion > CMPPartCtrl::ParVersion)
	{
		ToLogService("error", "msg[{}][{}] (CMPPartCtrl::LoadFromFile)", t_dwVersion, CMPPartCtrl::ParVersion);
		return false;
	}
	if(t_dwVersion < 2)
	{
		ToLogService("error", "msg[{}][{}] (CMPPartCtrl::LoadFromFile)", t_dwVersion, 2);
		return false;
	}

	char pszPartName[32];
	pbuf->mread(pszPartName,sizeof(char),32);
	m_strName = pszPartName;

	pbuf->mread(&m_iPartNum,sizeof(int),1);
#ifdef USE_GAME
	m_vecPartSys.resize(m_iPartNum);
#endif
	m_vecPartSys.setsize(m_iPartNum);

	if(t_dwVersion >= 3)
	{
		pbuf->mread(&m_fLength,sizeof(float),1);
	}
	else
	{
		m_fLength = 0;
	}
	for (int n = 0; n < m_iPartNum; ++n)
	{
		m_vecPartSys[n]->LoadFromMemory(pbuf,t_dwVersion);
		if(t_dwVersion < 6)
		{
			m_vecPartSys[n]->SetPlayTime(m_fLength);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if(t_dwVersion >= 7)
	{
		SAFE_DELETE_ARRAY(m_pcStrip);

		pbuf->mread(&m_iStripNum,sizeof(int),1);

		if(m_iStripNum > 0)
		{
			m_pcStrip = new CMPStrip[m_iStripNum];

			for ( int n = 0; n < m_iStripNum; ++n)
			{
				m_pcStrip[n].LoadFromMemory(pbuf,t_dwVersion);
			}
		}
	}

	if(t_dwVersion >= 8)
	{
		pbuf->mread(&m_iModelNum,sizeof(int),1);
		m_vecModel.resize(m_iModelNum);
		for ( int n = 0; n < m_iModelNum; ++n)
		{
			m_vecModel[n] = new CChaModel;
			m_vecModel[n]->LoadFromMemory(pbuf);
		}
	}
	return true;
}

void	CMPPartCtrl::GetRes(CMPResManger* pResMagr,std::vector<INT>& vecTex,std::vector<INT>& vecModel,
								   std::vector<INT>& vecEff)
{
	char pszPath[MAX_PATH];
	char pszNewPath[MAX_PATH];
	std::vector<INT>::	iterator it;
	int id = -1;

	int n;
	int idtex, idmodel,ideff;
	s_string strName;
	if(m_iStripNum > 0)
	{
		for ( n = 0; n < m_iStripNum; ++n)
		{
			strName = m_pcStrip[n].GetTexName();
			id = pResMagr->GetTextureID(strName);
			if(id >= 0)
			{
				it = std::find( vecTex.begin(), vecTex.end(), id );
				if(it == vecTex.end())
				{
					vecTex.push_back(id);
					sprintf(pszPath,"texture/effect/%s",strName.c_str());
					sprintf(pszNewPath,"effect/new/texture/%s",strName.c_str());
					if(!::CopyFile(pszPath,pszNewPath,FALSE))
					{
					}
				}
			}
		}
	}else if(m_iPartNum > 0)
	{
		for ( n = 0; n < m_iPartNum; ++n)
		{
			m_vecPartSys[n]->GetRes(idtex,  idmodel,  ideff);
			if(idtex >= 0)
			{
				it = std::find( vecTex.begin(), vecTex.end(), idtex );
				if(it == vecTex.end())
				{
					vecTex.push_back(idtex);
				}
			}
			if(idmodel >= 0)
			{
				it = std::find( vecModel.begin(), vecModel.end(), idmodel );
				if(it == vecModel.end())
				{
					vecModel.push_back(idmodel);
				}
			}
			if(ideff >= 0)
			{
				it = std::find( vecEff.begin(), vecEff.end(), ideff );
				if(it == vecEff.end())
				{
					vecEff.push_back(ideff);
				}
			}
		}
	}
}

void	CMPPartCtrl::GetHitRes(CMPResManger* pResMagr,std::vector<s_string>& vecPar)
{
	if(m_iStripNum > 0)
	{
		return;
	}else if(m_iPartNum > 0)
	{
		s_string strname;
		int id,n;
		for ( n = 0; n < m_iPartNum; ++n)
		{
			strname = m_vecPartSys[n]->GetHitEff();
			
			id = pResMagr->GetPartCtrlID(strname);
			if(id >= 0)
			{
				vecPar.push_back(strname);
			}
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

static void keyframe_proc( DWORD type, DWORD pose_id, DWORD key_id, DWORD key_frame, void* param )
{
	CChaModel* pCha = ((CChaModel*)param);
	switch( type )
	{
	case KEYFRAME_TYPE_BEGIN:
		{//if(pCha->GetStrip())
		//	pCha->GetStrip()->Play(0);
			pCha->SetPlaying(true);
		break;
		}
	case KEYFRAME_TYPE_KEY:
		{
			//if(pCha->GetStrip())
			//	pCha->GetStrip()->UpdateStrip();
			break;
		}
	case KEYFRAME_TYPE_END:
		//if(pCha->GetStrip())
		//	pCha->GetStrip()->UpdateStrip();
		if(pCha->GetPlayType()!= PLAY_LOOP)
		{
			pCha->SetPlaying(false);
		}
		break;
	}
}

bool	CChaModel::LoadScript(const s_string& strModel)
{
	MPChaLoadInfo	sModel;
	SChaAction	sAction;

	if(!CScriptFile::m_ctScript.ReadSection((char*)(LPCSTR)strModel.c_str()))
		return false;
	if( !CScriptFile::m_ctScript.ReadLine( "",sModel.bone,64 ) )
		return false;

	_iID = atoi((char*)(LPCSTR)strModel.c_str());

	char str[64];
	for( short i = 0; i < 5; i++ )
	{
		sprintf(str,"%d",i+1);
		if( !CScriptFile::m_ctScript.ReadLine( str,sModel.part[i],64 ) )
			return false;
	}
	if(!LoadChaModel(sModel))
		return false;
	if(!CScriptFile::m_ctScript.GetCharAction(atoi((char*)(LPCSTR)strModel.c_str()), &sAction))
		return false;
	if(!LoadPose(sAction))
		return false;
	SetPoseKeyFrameProc( keyframe_proc, (void*)this );

	return true;
}


bool	CChaModel::LoadChaModel(MPChaLoadInfo& info)
{
	if(strlen( info.bone ) <= 0)
		return false;
	if( FAILED( MPCharacter::Load( &info ) ) )
	{
		return false;
	}
	return true;
}
bool	CChaModel::LoadPose(SChaAction& SCharAct)
{
	lwPoseInfo pi;
	memset( &pi, 0, sizeof(pi) );
	for (int iActCount = 0; iActCount < SCharAct.m_iMaxActionNum; iActCount ++)
	{
		if (SCharAct.m_SActionInfo[iActCount].m_sActionNO < 1)
			continue;
		pi.start = SCharAct.m_SActionInfo[iActCount].m_sStartFrame;
		pi.end = SCharAct.m_SActionInfo[iActCount].m_sEndFrame;
		pi.key_frame_num =
			SCharAct.m_SActionInfo[iActCount].m_sKeyFrameNum < MAX_KEY_FRAME_NUM ? SCharAct.m_SActionInfo[iActCount].m_sKeyFrameNum : MAX_KEY_FRAME_NUM;
		for (int iKeyFrameCount = 0; iKeyFrameCount < (short)pi.key_frame_num; iKeyFrameCount ++)
			pi.key_frame_seq[iKeyFrameCount] = SCharAct.m_SActionInfo[iActCount].m_sKeyFrame[iKeyFrameCount];

		if( GetPoseCtrl() )
		{
			GetPoseCtrl()->InsertPose( SCharAct.m_SActionInfo[iActCount].m_sActionNO, &pi, 1 );
		}
	}
	return true;
}
void	CChaModel::PlayPose(DWORD id, DWORD type)
{
	MPCharacter::PlayPose( id, type, 0.0f, _fVel );
}

void	CChaModel::Begin()
{
	m_pDev->GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pDev->SetRenderState( D3DRS_AMBIENT, 0xffffffff );

	m_pDev->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDev->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDev->GetDevice()->SetRenderState(D3DRS_SRCBLEND,  _eSrcBlend);	 
	m_pDev->GetDevice()->SetRenderState(D3DRS_DESTBLEND, _eDestBlend);
#ifdef USE_RENDER
	D3DMATERIALX mtl;
	mtl.Ambient = (D3DCOLORVALUE)_dwCurColor;
	mtl.Diffuse = (D3DCOLORVALUE)_dwCurColor;
	SetMaterial(&mtl);
	SetOpacity(_dwCurColor.a);
#else
	m_pDev->SetRenderState(D3DRS_DITHERENABLE,FALSE);
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 

	m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, _dwCurColor );

	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	m_pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  

	m_pDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT);		
	m_pDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT);
#endif

}
void	CChaModel::End()
{
	m_pDev->GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE);

	m_pDev->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDev->GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
}

void	CChaModel::SaveToFile(FILE* file)
{
	fwrite(&_iID,sizeof(int),1,file);
	fwrite(&_fVel,sizeof(float),1,file);
	fwrite(&_iPlayType,sizeof(int),1,file);
	fwrite(&_iCurPose,sizeof(int),1,file);

	int eblend = (int)_eSrcBlend;
	fwrite(&eblend,sizeof(int),1,file);
	eblend = (int)_eDestBlend;
	fwrite(&eblend,sizeof(int),1,file);
	fwrite(&_dwCurColor,sizeof(D3DXCOLOR),1,file);

}
void	CChaModel::LoadFromFile(FILE* file)
{
	fread(&_iID,sizeof(int),1,file);
	fread(&_fVel,sizeof(float),1,file);
	fread(&_iPlayType,sizeof(int),1,file);
	fread(&_iCurPose,sizeof(int),1,file);

	int eblend;
	fread(&eblend,sizeof(int),1,file);
	_eSrcBlend = (D3DBLEND)eblend;
	fread(&eblend,sizeof(int),1,file);
	_eDestBlend = (D3DBLEND)eblend;
	fread(&_dwCurColor,sizeof(D3DXCOLOR),1,file);

	char psID[32];
	sprintf(psID,"%d",_iID);
	LoadScript(psID);
	SetVel((int)(_fVel * 1000));
	SetPlayType(_iPlayType);
	SetCurPose(_iCurPose);
	PlayPose(_iCurPose,PLAY_PAUSE);
}

void	CChaModel::LoadFromMemory(CMemoryBuf* pbuf)
{
	pbuf->mread(&_iID,sizeof(int),1);
	pbuf->mread(&_fVel,sizeof(float),1);
	pbuf->mread(&_iPlayType,sizeof(int),1);
	pbuf->mread(&_iCurPose,sizeof(int),1);

	int eblend;
	pbuf->mread(&eblend,sizeof(int),1);
	_eSrcBlend = (D3DBLEND)eblend;
	pbuf->mread(&eblend,sizeof(int),1);
	_eDestBlend = (D3DBLEND)eblend;
	pbuf->mread(&_dwCurColor,sizeof(D3DXCOLOR),1);

	char psID[32];
	sprintf(psID,"%d",_iID);
	LoadScript(psID);
	SetVel((int)(_fVel * 1000));
	SetPlayType(_iPlayType);
	SetCurPose(_iCurPose);
	PlayPose(_iCurPose,PLAY_PAUSE);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

bool	CMPLink::Create(MPCharacter *pChaMain,int iDummy1,MPCharacter *pChaTag, int iDummy2, \
			   char* pszTex, int iTexNum,CMPResManger* pResMgr,D3DXVECTOR3* pEyePos,MPRender*	pDev)
{
	m_pDev = pDev;
	_pCEffFile = pResMgr->GetEffectFile();
	_fDailTime = pResMgr->GetDailTime();
	_pEyePos = pEyePos;

#if 0
	lwMatrix44 mat1,mat2;
	if(pChaMain->GetObjDummyRunTimeMatrix(&mat1,iDummy1) != 0)
		return false;
	if(pChaTag->GetObjDummyRunTimeMatrix(&mat2,iDummy2) != 0)
		return false;

	_vStart = (*(D3DXVECTOR3*)&mat1._41);
	_vEnd = (*(D3DXVECTOR3*)&mat2._41);
#else
	_vStart = D3DXVECTOR3(0,0,0);
	_vEnd = D3DXVECTOR3(10,0,0);
#endif
	_vdir =  _vEnd - _vStart;
	_fdist = D3DXVec3Length( &_vdir );
	D3DXVec3Normalize(&_vdir,&_vdir);

	//D3DXVec3Cross(&_vcross,&D3DXVECTOR3(0,0,-1), &_vdir);

	int n;

	_pFrame = new MPFrame[LINK_FACE];
	for (n = 0; n < LINK_FACE; n++)
	{
		_pFrame[n].vPos1.m_dwDiffuse = 0xffffffff;
		_pFrame[n].vPos2.m_dwDiffuse =  0xffffffff;
	}

	_pChaMain = pChaMain;
	_pChaTag = pChaTag;
	_iDummy1 = iDummy1;
	_iDummy2 = iDummy2;

	char pszName[32];
	_pTex = new lwITex*[iTexNum];
	s_string strName;
	int id;
	for ( n = 0; n < iTexNum; n++)
	{
		_pTex[n] = NULL;
		sprintf(pszName,"%s%d",pszTex,n);
		strName = pszName;
		id = pResMgr->GetTextureID(strName);
		if(id < 0)
			return false;
		_pTex[n] = pResMgr->GetTextureByIDlw(id);
	}
	return true;
}

void	CMPLink::ColArc(float fradius)
{
	//if(!_pPath)
	//	_pPath = new D3DXVECTOR3[LINK_FACE];

	D3DXVECTOR3 veyedir,vcross;
	const auto v = *_pEyePos - _vStart;

	D3DXVec3Normalize(&veyedir,&v);
	D3DXVec3Cross(&vcross,&_vdir,&veyedir);
	D3DXVec3Normalize(&vcross,&vcross);
	//D3DXVec3Cross(&vcross,&veyedir,&vcross);

	//vcross = D3DXVECTOR3(0,0,1);

	D3DXVECTOR3 vOrg(-fradius,0,0);
	D3DXVECTOR3 vTemp;


	D3DXMATRIX mat;
	int n;
	int num = 10;
	float fstep = (D3DX_PI / 2.0f) / float(num);
	float fhei;
	float ft;
	for (n = 0; n < num; ++n)
	{
		D3DXMatrixRotationY(&mat,n * fstep);
		D3DXVec3TransformCoord(&vTemp,&vOrg,&mat);
		fhei = vTemp.z;

		ft = fradius + vTemp.x;
		if(ft < 0.00001f)
		{
			ft = 0.0f;
		}
		vTemp = _vStart + _vdir * ft;
		vTemp.z = fhei;

		ft = ft / _fdist;

		//vTemp = _vStart + _vdir * (n * 1.0f);

		_pFrame[n].vPos1.m_SPos = vTemp + (-vcross * 1.0f);
		_pFrame[n].vPos2.m_SPos = vTemp + (vcross * 1.0f);

		if(n ==0)
		{
			_pFrame[n].vPos1.m_SUV.x = 0;
			_pFrame[n].vPos1.m_SUV.y = 1;

			_pFrame[n].vPos2.m_SUV.x = 0;
			_pFrame[n].vPos2.m_SUV.y = 0;
		}else
		{
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

void	CMPLink::GetPhysique()
{
#if 0
	lwMatrix44 mat1,mat2;
	if(_pChaMain->GetObjDummyRunTimeMatrix(&mat1,_iDummy1) != 0)
		return false;
	if(_pChaTag->GetObjDummyRunTimeMatrix(&mat2,_iDummy2) != 0)
		return false;

	_vStart = (*(D3DXVECTOR3*)&mat1._41);
	_vEnd = (*(D3DXVECTOR3*)&mat2._41);
#else
	_vStart = D3DXVECTOR3(0,0,0);
	_vEnd = D3DXVECTOR3(10,0,0);
#endif

	_vdir =  _vEnd - _vStart;
	_fdist = D3DXVec3Length( &_vdir );
	D3DXVec3Normalize(&_vdir,&_vdir);

	//D3DXVec3Cross(&_vcross,&D3DXVECTOR3(0,0,-1), &_vdir);

	//
	float frad = 3.0f;
	if(_fdist > frad * 2)
	{
		_fRadius = frad - (frad * ((_fdist / frad - 1.0f) /10.0f) );
	}else
	{
		_fRadius = frad + ( (frad - _fdist) /10.0f);
	}
	ColArc(_fRadius);
}


void	CMPLink::FrameMove()
{
	GetPhysique();

	_fCurTime += *_fDailTime;
	if(_fCurTime > 0.15f)
	{
		_iCurTex++;
		if(_iCurTex >= 4)
			_iCurTex = 0;
		_fCurTime = 0;
	}
}

void	CMPLink::Render()
{

	_pCEffFile->SetTechnique(0);
	_pCEffFile->Begin();
	_pCEffFile->Pass(0);

	//m_pDev->SetRenderStateForced( D3DRS_ALPHABLENDENABLE,FALSE );

	m_pDev->SetRenderState( D3DRS_SRCBLEND,D3DBLEND_SRCALPHA );
	m_pDev->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_ONE);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	m_pDev->SetTransformWorld(&mat);
	m_pDev->SetTexture(0,_pTex[_iCurTex]->GetTex());
	m_pDev->SetVertexShader(NULL);
	m_pDev->SetFVF(LINK_FVF);
	m_pDev->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
		20, _pFrame, sizeof(LinkVer));

	_pCEffFile->End();
}

