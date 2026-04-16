#include "stdafx.h"
#include "EffectObj.h"
#include "SceneObj.h"
#include "Character.h"
#include "SceneItem.h"

#include "EffectRecordStore.h"
#include "ShadeRecordStore.h"

#include "effdelay.h"
#include "Scene.h"
#include "shipset.h"
#include "GameApp.h"

//extern CMPResManger  ResMgr;

CEffectBox  g_CEffBox;
CEffectBox  CPathBox;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void	GetNameFromFileName(char *pszOut, char* pszIn, char* postfix)
{
	lstrcpy(pszOut,pszIn);
	char* s = pszOut;
	s += lstrlen(pszIn) -lstrlen(postfix);

	*s = '\0';
}
D3DXMATRIX * GetMatrixRotaPoint(D3DXMATRIX *pout,D3DXVECTOR3 *Point,\
										 D3DXVECTOR3 *aixs,float angle)
{
	D3DXMATRIX r, r2;
	D3DXMATRIX r1 = D3DXMATRIX(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-Point->x, -Point->y, -Point->z, 1);
	D3DXMatrixRotationAxis(&r2,aixs,angle);
	r = r1 * r2;
	r1 = D3DXMATRIX(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		Point->x, Point->y, Point->z, 1);
	r = r * r1;
	*pout = r;
	return pout;
}
/************************************************************************/
/*CMagicEff*/
/************************************************************************/
void (*OpertionList[])(CMagicEff* pEffCtrl) = 
{
	Part_bind,
	Part_follow,
	Part_foldir,
	//Part_drop,
	//Part_fly,
	//Part_trace,
	//Part_fshade,
	//Part_arc,
	//Part_dirlight,
};
void (*MagicList[])(CMagicCtrl* pEffCtrl, void*	pParam) = 
{
		Part_drop,
		Part_fly,
		Part_trace,
		Part_fshade,
		Part_arc,
		Part_dirlight,
		Part_dist,
};

void (*GroupList[])(CMagicEff* pEffCtrl,D3DXVECTOR3* pStart,D3DXVECTOR3* pEnd) = 
{
	Part_fan,
	Part_sequence,
};

inline  void	Part_bind(CMagicEff* pEffCtrl)
{
	CGameScene*	pScene = pEffCtrl->_pScene;
	CCharacter* pCha = NULL;
	CSceneObj*	pObj = NULL;
	CSceneItem*	pItem = NULL;

	lwMatrix44	tMat;
	lwMatrix44	tMat2;


	if(!pEffCtrl->_pObj)
		goto __ret;
	if (!pEffCtrl->_pObj->IsValid())
		goto __ret;

	switch( pEffCtrl->getTypeID() )
	{
	case 3:
		{
			pItem = (CSceneItem*)pEffCtrl->_pObj;

			CCharacterModel* character = pItem->getParentCharacter();
			if( character )
			{
				lwMatrix44 matrix;

				//int parentDummy = pItem->getParentDummy();
				//character->GetObjDummyRunTimeMatrix( &tMat2, parentDummy );
				//pItem->GetDummyLocalMatrix( &tMat, pEffCtrl->_iDummy);
				//lwMatrix44Multiply( &matrix, &tMat, &tMat2 );

				pItem->GetObjDummyRunTimeMatrix(&matrix,pEffCtrl->_iDummy);
				pEffCtrl->BindingBone((D3DXMATRIX*)&matrix);
			}
			return;
		}
	default:
		pCha = (CCharacter*)pEffCtrl->_pObj;
		if(pCha->IsBoat())
		{
			if(!pCha->IsUpdate())
			{
				if(pCha->GetObjDummyRunTimeMatrix(&tMat,pEffCtrl->_iDummy,0))
				{
					const auto v = D3DXVECTOR3(0, 0, 0);
					pEffCtrl->MoveTo(&v);
				}
			}else
				goto skip;
		}else
		{
	skip:
			if(pCha->GetObjDummyRunTimeMatrix(&tMat,pEffCtrl->_iDummy))
			{
				g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(55), pEffCtrl->_iIdxID, pEffCtrl->_iDummy, pCha->GetDefaultChaInfo()->szName));
				const auto v = D3DXVECTOR3(0, 0, 0);
				pEffCtrl->MoveTo(&v);
				return;
			}
		}
		pEffCtrl->SetEffectDir(pEffCtrl->_pObj->getYaw());
		pEffCtrl->BindingBone((D3DXMATRIX*)&tMat);
		return;
	}
__ret:
	;
}
inline  void	Part_follow(CMagicEff* pEffCtrl)
{
	CGameScene*	pScene = pEffCtrl->_pScene;
	CCharacter* pCha = NULL;
	CSceneObj*	pObj = NULL;
	CSceneItem*	pItem = NULL;

	lwMatrix44	tMat;

	D3DXVECTOR3 tpos;
	int iangle;

	if(!pEffCtrl->_pObj)
		goto __ret;
	if (!pEffCtrl->_pObj->IsValid())
		goto __ret;

	iangle = pEffCtrl->_pObj->getYaw();
	switch(pEffCtrl->getTypeID())
	{
	case 1:	//
		pCha = (CCharacter*)pEffCtrl->_pObj;
		pEffCtrl->SetEffectDir(iangle);

		if(pCha->IsBoat())
		{
			tpos = D3DXVECTOR3 (pCha->GetPos());
			tpos.z = SEA_LEVEL+0.01f;
			D3DXMatrixRotationZ((D3DXMATRIX*)&tMat,
				(float)pCha->getYaw() * 0.01745329f + D3DX_PI);
			tMat._41 = tpos.x;
			tMat._42 = tpos.y;
			tMat._43 = tpos.z + pEffCtrl->_nHeightOff/100;
			pEffCtrl->BindingBone((D3DXMATRIX*)&tMat);

		}
		else
		{
			tpos = pCha->GetPos();
			pEffCtrl->MoveTo(&tpos);
		}
		break;
	case 2: //
		lwMatrix44Identity(&tMat);
		pObj = (CSceneObj*)pEffCtrl->_pObj;
		tMat._41 = pObj->getPos().x;
		tMat._42 = pObj->getPos().y;
		tMat._43 = pObj->getPos().z;
		pEffCtrl->BindingBone((D3DXMATRIX*)&tMat);
		break;
	default:
		goto __ret;
	}
	return;
__ret:
	pEffCtrl->SetValid(FALSE);
}

inline  void	Part_foldir(CMagicEff* pEffCtrl)
{
	CGameScene*	pScene = pEffCtrl->_pScene;
	CCharacter* pCha = NULL;
	CSceneObj*	pObj = NULL;
	CSceneItem*	pItem = NULL;

	lwMatrix44	tMat;
	D3DXVECTOR3 tpos;

	if(!pEffCtrl->_pObj)
		goto __ret;
	if (!pEffCtrl->_pObj->IsValid())
		goto __ret;
	switch(pEffCtrl->getTypeID())
	{
	case 1:	//
		pCha = (CCharacter*)pEffCtrl->_pObj;
		{
			tpos = pCha->GetPos();
			pEffCtrl->MoveTo(&tpos);
		}
		break;
	default:
		goto __ret;
	}
	return;
__ret:
	pEffCtrl->SetValid(FALSE);
}

inline  void	Part_trace(CMagicCtrl* pEffCtrl, void*	pParam)
{
	CMagicEff *pEff = (CMagicEff *)pParam;

	CGameScene*	pScene = pEff->GetScene();
	CCharacter* pCha = NULL;
	D3DXVECTOR3 vTarget;

	pEffCtrl->Render();

	if(pEffCtrl->_fStartDist < 1)
	{
		pEffCtrl->_vPos	 = pEffCtrl->_vOldTarget;
		pEffCtrl->Stop();
		return;
	}
	float fDist  = pEffCtrl->_fVel * *ResMgr.GetDailTime();
	if(fDist * fDist > pEffCtrl->_fDist)
	{
		pEffCtrl->_vPos	 = pEffCtrl->_vOldTarget;
		pEffCtrl->Stop();
		return;
	}
	if(fDist > 1.5f)
		fDist = 1.5f;
	pEffCtrl->_vPos	 += pEffCtrl->_vDir * fDist;


	if(PointInstrPointRange(&pEffCtrl->_vPos, &pEffCtrl->_vOldTarget, 1.0f))
	{
		pEffCtrl->Stop();
		return;
	}
	if(pEff->HitTestMap(&pEffCtrl->_vPos))
	{
		pEffCtrl->Stop();
		return; 
	}

	if(pEffCtrl->_iTargetID == -1)
	{
		vTarget = pEffCtrl->_vOldTarget;
	}else
	{
		pCha	= pScene->GetCha(pEffCtrl->_iTargetID);
		if(!pCha)
			pEffCtrl->Stop();

		vTarget = pCha->GetPos();
		vTarget.z += 1.0f;
	}

	//!
	if(PointInstrPointRange(&vTarget, &pEffCtrl->_vOldTarget,1.0f))
	{
		return;
	}

	
	pEffCtrl->ResetDir(&vTarget);

	//!
	pEffCtrl->_vTargDir = vTarget - pEffCtrl->_vOldTarget;
	//!
	pEffCtrl->_fTargDist = D3DXVec3LengthSq(&pEffCtrl->_vTargDir);
	D3DXVec3Normalize(&pEffCtrl->_vTargDir, &pEffCtrl->_vTargDir);

	//!.
	//	 =  /  * 
	float flerp = (pEffCtrl->_fTargDist / pEffCtrl->_fDist) * fDist;

	//!
	pEffCtrl->_vPos	 += pEffCtrl->_vTargDir * flerp;
	//!
	pEffCtrl->_vOldTarget = vTarget;
	pEffCtrl->_vDir = vTarget - pEffCtrl->_vPos;
	pEffCtrl->_fDist = D3DXVec3LengthSq(&pEffCtrl->_vDir);
	D3DXVec3Normalize(&pEffCtrl->_vDir,&pEffCtrl->_vDir);

}

inline  void	Part_drop(CMagicCtrl* pEffCtrl, void*	pParam)
{
	CMagicEff *pEff = (CMagicEff *)pParam;
	CGameScene*	pScene = pEff->GetScene();
	pEffCtrl->Render();
	float fDist  = pEffCtrl->_fVel * *ResMgr.GetDailTime();

	pEffCtrl->_fCurDist += fDist;
	if(pEffCtrl->_fCurDist > pEffCtrl->_fStartDist)
	{
		pEffCtrl->_vPos = pEffCtrl->_vOldTarget;
		pEffCtrl->Stop();
		return;
	}
	pEffCtrl->_vPos	 += pEffCtrl->_vDir * fDist;

	if(pEff->HitTestMap(&pEffCtrl->_vPos))
	{
		pEffCtrl->Stop();
		return;
	}
}
inline  void	Part_fly(CMagicCtrl* pEffCtrl, void*	pParam)
{
	CMagicEff *pEff = (CMagicEff *)pParam;
	CGameScene*	pScene = pEff->GetScene();
	pEffCtrl->Render();
	float fDist  = pEffCtrl->_fVel * *ResMgr.GetDailTime();

	pEffCtrl->_fCurDist += fDist;
	if(pEffCtrl->_fCurDist > pEffCtrl->_fStartDist)
	{
		pEffCtrl->_vPos = pEffCtrl->_vOldTarget;
		pEffCtrl->Stop();
		return;
	}
	pEffCtrl->_vPos += pEffCtrl->_vDir * fDist;
}


inline  void	Part_fshade(CMagicCtrl* pEffCtrl, void*	pParam)
{
	CMagicEff *pEff = (CMagicEff *)pParam;
	CGameScene*	pScene = pEff->GetScene();
	pEffCtrl->Render();

	if(PointInstrPointRange(&pEffCtrl->_vPos, &pEffCtrl->_vTarget,0.5f))
	{
		pEffCtrl->Stop();
		return;
	}

	pEffCtrl->_vPos += pEffCtrl->_vDir * (pEffCtrl->_fVel* *ResMgr.GetDailTime());
	if(pEff->HitTestMap(&pEffCtrl->_vPos))
	{
		pEffCtrl->_vPos = pEffCtrl->_vOldTarget;
		pEffCtrl->Stop();
		return;
	}

	D3DXVECTOR3 tvPos = pEffCtrl->_vPos;
	float size = 3;
	float alpha = 1;
	for (int n = 0; n < pEffCtrl->_iCurSNum; n++)
	{
		size -= (float)3 / pEffCtrl->_iCurSNum; 
		alpha -= (float)1 / pEffCtrl->_iCurSNum;
		tvPos += (-pEffCtrl->_vDir) * 0.5f;
		for (int m = 0; m < pEffCtrl->GetModelNum(); m++)
		{
			pEffCtrl->GetModelEff(m)->SetAlpha(alpha);
			pEffCtrl->GetModelEff(m)->MoveTo(tvPos.x,tvPos.y,tvPos.z);
			pEffCtrl->GetModelEff(m)->Render();
		}
	}
	pEffCtrl->GetModelEff(0)->Scaling(3,3,3);
	pEffCtrl->GetModelEff(0)->SetAlpha(1);

	pEffCtrl->_fCurTime += *ResMgr.GetDailTime();
	if(pEffCtrl->_fCurTime > 0.6f)
	{
		pEffCtrl->_fCurTime = 0.0f;
		pEffCtrl->_iCurSNum++;
	}
}

inline  void	Part_arc(CMagicCtrl* pEffCtrl, void*	pParam)
{
	CMagicEff *pEff = (CMagicEff *)pParam;

	if( pEff->IsFlyEff() && ( pEff->GetStartTime() + 10000 < CGameApp::GetCurTick() ) )
	{
		pEffCtrl->Stop();
		return;
	}

	pEffCtrl->Render();
	pEffCtrl->_fCurArc += (pEffCtrl->_fVel* *ResMgr.GetDailTime());
	D3DXVECTOR3 vp = pEffCtrl->_vOldPos + pEffCtrl->_vDir* pEffCtrl->_fCurArc;
	D3DXVECTOR3 vd = vp - pEffCtrl->_vArcOrg;
	D3DXVec3Normalize(&vd,&vd);
	pEffCtrl->_vPos = pEffCtrl->_vArcOrg + (vd * (pEffCtrl->_fHalfHei ));

	if(PointInstrPointRange(&pEffCtrl->_vPos, &pEffCtrl->_vTarget,0.5f))
	{		
		pEffCtrl->Stop();
		return;
	}

	if(pEff->HitTestMap(&pEffCtrl->_vPos))
		return;

}
inline  void	Part_dirlight(CMagicCtrl* pEffCtrl, void*	pParam)
{
	CMagicEff *pEff = (CMagicEff *)pParam;
	CGameScene*	pScene = pEff->GetScene();
	pEffCtrl->_vPos = pEffCtrl->_vOldPos;
	pEffCtrl->MoveTo(&pEffCtrl->_vPos);

	bool	bover = true;
	for (int n = 0; n < pEffCtrl->GetModelNum(); ++n)
	{
		pEffCtrl->GetModelEff(n)->Render();
		if(pEffCtrl->GetModelEff(n)->IsPlay())
		{
			bover = false;
		}
	}
	if(bover)
	{
		pEffCtrl->Stop();
	}
}

inline  void	Part_dist(CMagicCtrl* pEffCtrl, void*	pParam)
{
	CMagicEff *pEff = (CMagicEff *)pParam;

	CGameScene*	pScene = pEff->GetScene();

	pEffCtrl->Render();

	float fDist  = pEffCtrl->_fVel * *ResMgr.GetDailTime();
	pEffCtrl->_fCurDist += fDist;
	pEffCtrl->_vPos	 += pEffCtrl->_vDir * fDist;

	if(pEffCtrl->_fCurDist > pEffCtrl->_fStartDist)
		pEffCtrl->Stop();
}
inline  void	Part_dist2(CMagicCtrl* pEffCtrl, void*	pParam)
{
	CMagicEff *pEff = (CMagicEff *)pParam;

	CGameScene*	pScene = pEff->GetScene();

	pEffCtrl->Render();

	float fDist  = pEffCtrl->_fVel * *ResMgr.GetDailTime();
	pEffCtrl->_fCurDist += fDist;
	pEffCtrl->_vPos	 += pEffCtrl->_vDir * fDist;

	float falpha = 1.0f - (pEffCtrl->_fCurDist / pEffCtrl->_fStartDist);
	pEffCtrl->SetAlpha(falpha);
	if(pEffCtrl->_fCurDist > pEffCtrl->_fStartDist)
		pEffCtrl->Stop();
}
inline  void	Part_fan(CMagicEff* pEffCtrl,D3DXVECTOR3* pStart,D3DXVECTOR3* pEnd)
{
	if(!pEffCtrl->_bGroupMagic)
		return;
	int	  nSize = (INT)pEffCtrl->_pMagicCtrl.size();

	D3DXVECTOR3 vstart,vend;
	vstart = *pStart;
	vend = *pEnd;
	vend.z = vstart.z;

	if(nSize == 1)
	{
		pEffCtrl->_pMagicCtrl[0]->SetTargetID(pEffCtrl->_iTargetID);
		pEffCtrl->_pMagicCtrl[0]->Emission(&vstart,&vend);
		return;
	}
	float fAngle = pEffCtrl->_fFanAngle;
	float fstep = fAngle / ((float)nSize - 1);
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vDir2;
	D3DXMATRIX	mat;
	vDir2 = vend - vstart;
	float dist = D3DXVec3Length(&vDir2);
	D3DXVec3Normalize(&vDir2,&vDir2);

	for (INT n = 0; n < nSize; ++n)
	{
		D3DXMatrixRotationZ(&mat,(-(fAngle/2))  + (fstep * (float)n));
		D3DXVec3TransformCoord(&vDir,&vDir2,&mat);

		vend = vstart + vDir * dist; 

		pEffCtrl->_pMagicCtrl[n]->SetTargetID(pEffCtrl->_iTargetID);
		pEffCtrl->_pMagicCtrl[n]->Emission(&vstart,&vend);
	}
}
inline  void	Part_sequence(CMagicEff* pEffCtrl,D3DXVECTOR3* pStart,D3DXVECTOR3* pEnd)
{
	if(!pEffCtrl->_bGroupMagic)
		return;
	D3DXVECTOR3 vstart,vend;
	vstart = *pStart;
	vend = *pEnd;

	int	  nSize = (INT)pEffCtrl->_pMagicCtrl.size();
	for (INT n = 0; n < nSize; ++n)
	{
		pEffCtrl->_pMagicCtrl[n]->SetDailTime((float)n * 0.2f);
		pEffCtrl->_pMagicCtrl[n]->SetTargetID(pEffCtrl->_iTargetID);
		pEffCtrl->_pMagicCtrl[n]->Emission(&vstart,&vend);
	}
}

//////////////////////////////////////////////////////////////////////////

CMagicEff::CMagicEff()
{
	_nYaw = 0;
	_nPitch = 0;
	_nRoll = 0;
	_fHei = 0;
	GroupEmission = NULL;
	RenderUpdate = NULL;
	_pDailTime = NULL;
	_iDummy = -1;
	_nHeightOff = -1;
	_iIdxID = -1;

	_dwAttachID = -1;

	_pScene = NULL;
	_pTerrain = NULL;

	_iOwnerID = -1;
	_iTargetID = -1;

	_iGroupIdx = -1;
	_vPos = D3DXVECTOR3(0,0,0);
	_bUpdateHei = false;

	_fVel = 0;

	_vMin = D3DXVECTOR3(0,0,0);
	_vMax = D3DXVECTOR3(0,0,0);
	_bShowBox = FALSE;
	SetHide(FALSE);
	_bMagic = FALSE;
	_bFoneEff = FALSE;

	_bMagicEm = FALSE;

	_bGroupMagic = FALSE;

	_bDail = false;
	_fsCurTime = 0;
	_fsDailTime= 0;
	_isID = -1;
	_vsBegin = D3DXVECTOR3(-1,-1,-1);
	_vsEnd = D3DXVECTOR3(-1,-1,-1);

    _pEffDelay = new CEffDelay( this );

	_eType = EFF_SCENE;
	_pObj = NULL;

	_nTag = 0;

	_bPlay = false;
	_pEffCtrl = NULL;
	_fFanAngle = 0.75f;
	_fBaseSize = -1;

	_pObj = NULL;
}

CMagicEff::~CMagicEff()
{
    delete _pEffDelay;
	SAFE_DELETE(_pEffCtrl);
	for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
	{
		SAFE_DELETE(_pMagicCtrl[n]);
	}
	_pMagicCtrl.clear();
}

void	CMagicEff::Clear()
{
	_nYaw = 0;
	_nPitch = 0;
	_nRoll = 0;
	_fHei = 0;
	GroupEmission = NULL;

	RenderUpdate = NULL;
	_iDummy = -1;
	_iGroupIdx = -1;

	_nHeightOff = -1;
	_iIdxID = -1;

	_dwAttachID = -1;

	_iOwnerID = -1;

	_iTargetID = -1;

	_vPos = D3DXVECTOR3(0,0,0);

	_fVel = 0;

	_bUpdateHei = false;

	_vMin = D3DXVECTOR3(0,0,0);
	_vMax = D3DXVECTOR3(0,0,0);
	_bShowBox = FALSE;
	SetHide(FALSE);
	_bFoneEff = FALSE;
	_bMagicEm = FALSE;
	_bGroupMagic = FALSE;

	_bDail = false;
	_fsCurTime = 0;
	_fsDailTime= 0;
	_isID = -1;
	_vsBegin = D3DXVECTOR3(-1,-1,-1);
	_vsEnd = D3DXVECTOR3(-1,-1,-1);

	SAFE_DELETE(_pEffCtrl);
	for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
	{
		SAFE_DELETE(_pMagicCtrl[n]);
	}
	_pMagicCtrl.clear();
	_fFanAngle = 0.75f;
	_fBaseSize = -1;

	_pObj = NULL;
}

void	CMagicEff::SetScene(CGameScene* pScene)	
{  
	_pScene = pScene; _pTerrain=_pScene->GetTerrain();
}

BOOL	CMagicEff::Create(int iIdxID)
{
	_dwStartTime = CGameApp::GetCurTick();
	if(iIdxID >= 1000 && iIdxID < 3000)//ID[1000,3000)
	{
		_bDail = false;
		_fsCurTime = 0;
		_fsDailTime= 0;
		_isID = -1;
		_pDailTime = ResMgr.GetDailTime();

		_pObj = NULL;

		if(	iIdxID < 2000)
		{
			if(!CreateMagic(iIdxID))			// Magic ID[1000,2000)
				return FALSE;
		}else
			if(!CreateGroupMagic( iIdxID))		// Group Magic ID[2000, 3000)
				return FALSE;

		SkillCtrl ctrl;
		ctrl.fSize = 1.0f;
		SetSkillCtrl(&ctrl);
		return TRUE;
	}
	//
	if (_bMagic||_bGroupMagic)
	{
		_bMagic = FALSE;
		_bGroupMagic = FALSE;
		setIdxID(-1);
	}

	if(iIdxID == 0)
		return FALSE;

	CMagicInfo* pInfo = GetMagicInfo(iIdxID);
	if(!pInfo)
	{
		return FALSE;
	}


	if(getIdxID() == iIdxID)
	{
		_bDail = false;
		_fsCurTime = 0;
		_fsDailTime= 0;
		_isID = -1;

		_pEffCtrl->Reset();

		if(iIdxID< 100)
			setTypeID(0);
		else if((iIdxID>= 100 && iIdxID <400) || (iIdxID>= 650 && iIdxID <1000)) // 650~1000
		{
			setTypeID(4);
			_bMagicEm = TRUE;
			if((int)pInfo->fPlayTime == 1)
			{
				_bloop = false;
			}
			else
			{
				_bloop = true;
			}
		}
		else if(iIdxID >= 400)
			setTypeID(1);

		setAttachID(pInfo->nObjType);
		setIdxID(iIdxID);

		_iDummy = pInfo->nDummy[0];
		_iOwnerID = -1;

		SkillCtrl ctrl;
		ctrl.fSize = 1.0f;
		SetSkillCtrl(&ctrl);

		//  by lh 2005-10-27
		int nEffType = pInfo->nEffType;
		switch(nEffType)
		{
		case 1:
		case 2:
		case 3:
 			nEffType = pInfo->nObjType;
			break;
		case 4:
			nEffType = pInfo->nObjType;
			break;
		case 5:
			nEffType = pInfo->nObjType;
			break;
		default:
			nEffType = -1;
		}
		RenderUpdate = NULL;
		if(nEffType >= 0) 
			RenderUpdate = OpertionList[nEffType];		
		return TRUE;
	}

	Clear();
	_pEffCtrl = new CMPEffectCtrl;

	_fBaseSize = pInfo->fBaseSize;

	Eff_Property  Property;
	Property.m_iEffType = pInfo->nEffType;
	switch(Property.m_iEffType)
	{
	case 0:
		Property.m_iIdxRender = -1;//
		break;
	case 1:
	case 2:
	case 3:
 		Property.m_iIdxRender = pInfo->nObjType;//dummy
		break;
	case 4:
		Property.m_iIdxRender = pInfo->nObjType;//
		break;
	case 5:
		Property.m_iIdxRender = pInfo->nObjType;// 
		break;
	default:
		return FALSE;
	}

	Property.m_strName = pInfo->szDataName;
	if(!Create(&Property,&ResMgr))
	{
		Clear();
		return FALSE;
	}

	_nHeightOff = pInfo->nHeightOff;

	if(iIdxID< 100)
		setTypeID(0);
	else if((iIdxID>= 100 && iIdxID <400) || (iIdxID>= 800 && iIdxID <1000) || // 800~1000
		(iIdxID >= 564 && iIdxID < 600))	// 07
	{
		setTypeID(4);
		_bMagicEm = TRUE;
		if((int)pInfo->fPlayTime == 1)
		{
			_bloop = false;
		}
		else
		{
			_bloop = true;
		}
	}
	else if(iIdxID >= 400)
	{
		setTypeID(1);
	}
	setAttachID(pInfo->nObjType);
	setIdxID(iIdxID);
	_iDummy = pInfo->nDummy[0];
	if(pInfo->nDummy2 != -1)
	{
		_pEffCtrl->setUseZBuff(false);
	}
	_pEffCtrl->Reset();

	SkillCtrl ctrl;
	ctrl.fSize = 1.0f;
	SetSkillCtrl(&ctrl);


	return TRUE;
}


BOOL	CMagicEff::Create(Eff_Property* pProperty,CMPResManger	*pCResMagr)
{
	if(!pProperty || !pCResMagr)
		return FALSE;

	int id = pCResMagr->GetPartCtrlID(pProperty->m_strName);
	if(id < 0)
	{
		ToLogService("errors", LogLevel::Error, "msgCan't find eff file [{}]", pProperty->m_strName.c_str());
		return FALSE;
	}

	_pEffCtrl->Clear();
	CMPPartCtrl* tctrl = pCResMagr->GetPartCtrlByID(id);
	if(!tctrl)
		return FALSE;
	_pEffCtrl->CopyPartCtrl(tctrl);
	_pEffCtrl->BindingRes(pCResMagr);
	_pDailTime = pCResMagr->GetDailTime();

	RenderUpdate = NULL;
	if(pProperty->m_iIdxRender >= 0) 
		RenderUpdate = OpertionList[pProperty->m_iIdxRender];
	return TRUE;
}

BOOL	CMagicEff::CreateMagic(int iIdxID)
{
	_bMagic = TRUE;
	_bGroupMagic = FALSE;

	if(getIdxID() == iIdxID)
	{
		_pMagicCtrl[0]->MagicUpdate = MagicList[_pMagicCtrl[0]->GetRenderIdx()];

		_pMagicCtrl[0]->Reset();
		_iOwnerID = -1;
		return TRUE;
	}
	Clear();
	_pMagicCtrl.resize(1);
	_pMagicCtrl[0] = new CMagicCtrl;
	if(!_pMagicCtrl[0]->Create(iIdxID - 1000,&ResMgr))
	{
		return FALSE;
	}
	setTypeID(4);
	setIdxID(iIdxID);

	int id  =_pMagicCtrl[0]->GetRenderIdx();
	_pMagicCtrl[0]->MagicUpdate = MagicList[id];
	_pMagicCtrl[0]->Reset();
	_bMagic = TRUE;
	return TRUE;
}

BOOL	CMagicEff::CreateGroupMagic(int iIdxID)
{
	_bMagic = FALSE;
	_bGroupMagic = TRUE;

	if(getIdxID() == iIdxID)
	{
		for (WORD n = 0; n < (WORD)_pMagicCtrl.size(); ++n)
		{
			_pMagicCtrl[n]->MagicUpdate = MagicList[_pMagicCtrl[n]->GetRenderIdx()];
			_pMagicCtrl[n]->Reset();
		}
		_iOwnerID = -1;
		GroupEmission = GroupList[_iGroupIdx];
		return TRUE;
	}
	Clear();

	Group_Param*		psGroupParam;
	psGroupParam		= GetGroupParam(iIdxID - 2000);
	if(!psGroupParam)
		return FALSE;
	_pMagicCtrl.resize(psGroupParam->nTotalNum);

	int idx = 0;
	int id;
	int m;
	int n;
	for (n = 0; n < psGroupParam->nTypeNum; ++n )
	{
		for ( m = 0; m < psGroupParam->nNum[n]; ++m)
		{
			_pMagicCtrl[idx] = new CMagicCtrl;
			if(!_pMagicCtrl[idx]->Create(psGroupParam->nTypeID[n],&ResMgr))
				return FALSE;
			id  =_pMagicCtrl[idx]->GetRenderIdx();
			_pMagicCtrl[idx]->MagicUpdate = MagicList[id];
			_pMagicCtrl[idx]->Reset();
			idx++;
		}
	}
	_iGroupIdx = psGroupParam->nRenderIdx;
	GroupEmission = GroupList[_iGroupIdx];

	setTypeID(-1);
	setIdxID(iIdxID);
	_bGroupMagic = TRUE;

	return TRUE;
}

void	CMagicEff::setFollowObj(CSceneNode* pObj , NODE_TYPE eType, int iDummy, int iDummy2)
{
	if(	_bMagic||_bGroupMagic)
		return;

	_pObj = pObj;
	if(iDummy == -1)
	{
		if(_iDummy == -1)
		{
			if(eType == NODE_ITEM)
				setTypeID(3);
			else if(eType == NODE_CHA)
				setTypeID(1);

			setAttachID(1);
			RenderUpdate = OpertionList[1];
		}
		else
		{
			if(eType == NODE_ITEM)
				setTypeID(3);
			else if(eType == NODE_CHA)
				setTypeID(1);

			MPMatrix44 mat;
			if(_pObj->GetRunTimeMatrix( &mat, iDummy ) )
			{
				_pEffCtrl->BindingBone((D3DXMATRIX*)&mat);
			}
		}
	}
	else
	{
		if(_iDummy != -2)
		{
			_iDummy = iDummy; 

			setAttachID(0);
			RenderUpdate = OpertionList[0];

			if(eType == NODE_CHA)
				setTypeID(1);
			else if(eType == NODE_ITEM)
				setTypeID(3);
		}
		else
		{
			if(_iDummy == -2)
			{
				MPMatrix44 mat;
				if(_pObj->GetRunTimeMatrix( &mat, iDummy ) )
				{
					_pEffCtrl->BindingBone((D3DXMATRIX*)&mat);
				}
			}
		}
	}
	if(_pEffCtrl->GetPartCtrl()->GetStripNum() > 0)
	{
		RenderUpdate = NULL;

		if(eType == NODE_CHA)
		{
			CCharacter* pCha = (CCharacter*)pObj;
			_pEffCtrl->GetPartCtrl()->SetStripCharacter((MPCharacter*)pCha);
		}
		else if(eType == NODE_ITEM)
		{
			CSceneItem*	pItem = (CSceneItem*)pObj;
			_pEffCtrl->GetPartCtrl()->SetStripItem((MPSceneItem*)pItem);
		}
	}
	else if(eType == NODE_ITEM)
	{
		CSceneItem*	pItem = (CSceneItem*)pObj;
		_pEffCtrl->SetItemDummy(pItem, 1, 2);
	}
}

void	CMagicEff::SetEffectDir(int iAngle)
{
	if(	_bMagic||_bGroupMagic)
		return;
	D3DXMATRIX mat;
	D3DXVECTOR3 vPos(0,-1,0);
	D3DXMatrixRotationZ(&mat,((float)iAngle * 0.01745329f));
	D3DXVec3TransformCoord(&vPos,&vPos,&mat);

	_pEffCtrl->setDir(&vPos);
}
void	CMagicEff::SetEffectMatrix(MPMatrix44* pmat)
{
	if(	_bMagic||_bGroupMagic || !pmat)
		return;
	_pEffCtrl->BindingBone((D3DXMATRIX*)pmat);
}

void	CMagicEff::FrameMove(DWORD dwDailTime)
{
	if(_bDail)
	{
		_fsCurTime += *ResMgr.GetDailTime();
		if(_fsCurTime >= _fsDailTime)
		{
			_bDail = false;
			Emission(_isID,_vsBegin.x < 0? NULL : &_vsBegin,_vsEnd.x < 0? NULL : &_vsEnd);
		}
		return;
	}

	if(!IsValid())
		return;
	if(IsHide())
		return;

	if(RenderUpdate)
		RenderUpdate(this);


	if (_bMagic||_bGroupMagic)
	{
		if(!_bPlay)
		{
			SetValid(FALSE);
			return;
		}

		for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
		{
			_pMagicCtrl[n]->FrameMove(dwDailTime);
		}
	}else
	{
		if(_bUpdateHei)
		{
			static DWORD dwtime = 0;
			if(_vPos.z <= 0)
			{
				if(dwDailTime - dwtime > 200)
				{
					setPos(int(_vPos.x * 100), int(_vPos.y * 100));
					dwtime = dwDailTime;
				}
			}else
				_bUpdateHei = false;
		}
		_pEffCtrl->FrameMove(dwDailTime);
	}


}
void	CMagicEff::Render()
{
	if(_bDail)
		return;


	if(!IsValid())
		return;
	if(IsHide())
		return;

	if (_bMagic||_bGroupMagic)
	{
		if(IsHide())
			return;

		RenderMagic();
		bool bov = true;
		for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
		{
			if(_pMagicCtrl[n]->IsPlaying())
			{
				if(_pTerrain)
					if(_pTerrain->IsPointVisible(_pMagicCtrl[n]->GetPos()->x, \
						_pMagicCtrl[n]->GetPos()->y)) 
					{
						bov = false;
					}
			}
		}
		if(bov)
			SetValid(FALSE);

		return;
	}
	if(_pTerrain)
		if(!_pTerrain->IsPointVisible(_vPos.x, _vPos.y)) 
		{
			if (!_pEffCtrl->GetPartCtrl()->IsPlaying())
			{
				SetValid(FALSE);
				return;
			}
			if(_bMagicEm && !_pObj)
				SetValid(FALSE);
			return;
		}

	if(IsShowBox())
	{
		g_CEffBox.Show(true);
		g_CEffBox.setPos(_vPos);
		g_CEffBox.setColor(0xff00ff00);
		g_CEffBox.setWriteFrame(TRUE);
		g_CEffBox.ShowLine(TRUE);
		g_CEffBox.ShowBox(TRUE);
		g_CEffBox.setScale(3,3,3);
		g_CEffBox.Render();
		g_CEffBox.Show(false);
	}
	if (!_pEffCtrl->GetPartCtrl()->IsPlaying())
	{
		SetValid(FALSE);
		return;
	}
	_pEffCtrl->Render();
}
void	CMagicEff::RenderMagic()
{
	for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
	{
		if(_pMagicCtrl[n]->IsDail())
			continue;
		if(_pMagicCtrl[n]->MagicUpdate)
		{
			if(_pMagicCtrl[n]->IsStop())
				_pMagicCtrl[n]->Render();
			else
				_pMagicCtrl[n]->MagicUpdate(_pMagicCtrl[n],(void*)this);
		}else
			_pMagicCtrl[n]->SetInValid();
	}
}

BOOL	CMagicEff::HitTestMap(D3DXVECTOR3* vPos)
{
	float fz = GetScene()->GetGridHeight(vPos->x,vPos->y);
	if(vPos->z  < fz)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL	CMagicEff::HitTestMap()
{
	_vMapTarget = _vPos;
	_vMapTarget.z = GetScene()->GetGridHeight(_vMapTarget.x,_vMapTarget.y);

	if(_vPos.z  < _vMapTarget.z)
	{
		SetValid(FALSE);
		return TRUE;
	}
	return FALSE;
}

void	CMagicEff::HitObj()
{
	_pScene->HandleSceneMsg(SCENEMSG_EFFECT_HIT, getIdxID(), getID());
}

void CMagicEff::_UpdateValid(BOOL bValid)
{
	if(!bValid)
	{
        _pEffDelay->Exec();

		_iDummy = -1; 

		setTypeID(-1);
		setAttachID(0);
		RenderUpdate = NULL;
		_pScene->HandleSceneMsg(SCENEMSG_EFFECT_VALID, getIdxID(), getID());
		SetFontEffect("20",NULL);
		_bPlay = false;
	}
	else
	{
		_pScene->HandleSceneMsg(SCENEMSG_EFFECT_CREATE, getIdxID(), getID());
	}
}

void CMagicEff::_UpdatePos()
{
	float fX = (float)_nCurX / 100.0f;
	float fY = (float)_nCurY / 100.0f;

	_fHei = _pScene->GetGridHeight(fX, fY);

	_UpdateHeight();
}

void CMagicEff::_UpdateHeight()
{
    float fX = (float)_nCurX / 100.0f;
	float fY = (float)_nCurY / 100.0f;
	const auto v = D3DXVECTOR3(fX, fY, _fHei + (float)(_nHeightOff) / 100.0f);
	MoveTo(&v);
}

void	CMagicEff::Emission(int iID, D3DXVECTOR3* vBegin, D3DXVECTOR3* vEnd, int iTime)
{
	try
	{

	if(_bDail)
	{
		_fsCurTime  = 0;
		_isID = iID;
		_vsBegin = vBegin ? *vBegin : D3DXVECTOR3(-1,-1,-1);
		_vsEnd = vEnd ? *vEnd : D3DXVECTOR3(-1,-1,-1);
		return;
	}

	_iTargetID = iID;
	if (_bMagic)
	{
		if(vBegin && vEnd) 
		{
			for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
			{
				_pMagicCtrl[n]->SetTargetID(iID);
				_pMagicCtrl[n]->Emission(vBegin,vEnd);
			}
			_bPlay = TRUE;
			return;
		}
		assert(0);
	}
	if(_bGroupMagic)
	{
		if(vBegin && vEnd) 
		{

			if(GroupEmission)
				GroupEmission(this,vBegin,vEnd);
			_bPlay = TRUE;
			return;
		}
		assert(0);
	}
	if(vBegin)
		MoveTo(vBegin);

	if(_bMagicEm)
	{
		if (809 <= _iIdxID && _iIdxID <= 818 )
			_pEffCtrl->GetPartCtrl()->Play(0);	// 
		else
			_pEffCtrl->GetPartCtrl()->Play(!_bloop/*0*/);
		if(_bloop)
			_pEffCtrl->GetPartCtrl()->SetPlayType(0);
	}
	else
	{
		_pEffCtrl->GetPartCtrl()->Play(0);
		_pEffCtrl->GetPartCtrl()->SetPlayType(0);
	}
	_bPlay = TRUE;
	}
	catch(...)
	{
		__asm int 3;
	}
}
 
void	CMagicEff::Stop()
{
	_pEffCtrl->GetPartCtrl()->Stop();
}
void	CMagicEff::End()
{
	_pEffCtrl->GetPartCtrl()->End();
}

void	CMagicEff::MoveTo(const D3DXVECTOR3* vPos)
{
	_vPos = *vPos;

	if(_bMagic)
	{
		for (INT n = 0; n < (INT)_pMagicCtrl.size(); ++n)
		{
			_pMagicCtrl[n]->MoveTo(vPos);
		}
		return;
	}
	_vMin = D3DXVECTOR3(-0.5f,-0.5f,-0.5f);
	_vMax = D3DXVECTOR3(0.5f,0.5f,0.5f);
	_vMin += *vPos;
	_vMax += *vPos;

	_pEffCtrl->GetPartCtrl()->MoveTo(&_vPos,_pTerrain);
}

void	CMagicEff::BindingBone(D3DXMATRIX* pMatBone)
{
	_vPos = *(D3DXVECTOR3*)&pMatBone->_41;
	_pEffCtrl->GetPartCtrl()->BindingBone(pMatBone);
}

BOOL	CMagicEff::HitTestPrimitive( lwVector3& org, lwVector3& ray)
{
	return D3DXBoxBoundProbe(&_vMin, &_vMax, (D3DXVECTOR3*)&org,(D3DXVECTOR3*)&ray);
}

void	CMagicEff::_UpdateYaw()
{
	_pEffCtrl->GetPartCtrl()->setYaw((float)_nYaw / 100 /*Angle2Radian((float)_nYaw)*/);
}
void	CMagicEff::_UpdatePitch()
{
	_pEffCtrl->GetPartCtrl()->setPitch((float)_nPitch / 100/*Angle2Radian((float)_nPitch) - D3DX_PI/2*/);
}
void	CMagicEff::_UpdateRoll()
{
	_pEffCtrl->GetPartCtrl()->setRoll((float)_nRoll / 100/*Angle2Radian((float)_nRoll) - D3DX_PI/2*/);
}

void	CMagicEff::_UpdateScale(float fx,float fy,float fz)
{
	_pEffCtrl->GetPartCtrl()->setScale(fx,fy,fz);
}

void	CMagicEff::SetInvalidByTime(DWORD dwDailTime)
{
	if (_iIdxID >=1 && _iIdxID < 100)
	{	// 
		return;
	}

	if (_iIdxID >=3000)
	{	// 
		return;
	}

	if (_bloop)
	{	// 
		return;
	}

	const DWORD time = 10 * 60 * 1000;
	if (dwDailTime - _dwStartTime > time )
	{
		SetValid(false);
		ToLogService("ui", "SetInvalidByTime _iIdxID:{}", _iIdxID);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*                                                                      */
/************************************************************************/
CShadeEff::CShadeEff()
{
	CMPShadeCtrl::CMPShadeCtrl();
	 SetHide(FALSE);
	_dwTypeID = 0;
	_iChaID = -1;
	_iIdxID  = 0;
	_bUpSea = false;
}
CShadeEff::~CShadeEff()
{
	CMPShadeCtrl::~CMPShadeCtrl();
}
void	CShadeEff::SetScene(CGameScene* pScene)
{  
	_pScene = pScene; _pTerrain = _pScene->GetTerrain();
}

bool	CShadeEff::Create(int iIdxID)
{
	SetHide( FALSE );
	_bUpSea = false;
	if(getIdxID() == iIdxID)
		return true;

	 CShadeInfo* pInfo = GetShadeInfo(iIdxID);
	 if(!Create( pInfo))
		 return false;

	 setIdxID(iIdxID);
	 return true;
}
bool	CShadeEff::Create( CShadeInfo* pInfo)
{
	_bUpSea = false;
	SetHide( FALSE );
	if(!pInfo)
		return false;
	if(pInfo->fsize>9)
		pInfo->fsize = 9;
    std::string str = pInfo->szDataName;
	if(!CMPShadeCtrl::Create(str,  &ResMgr,  pInfo->fsize, 
		pInfo->nAni!=0, pInfo->nRow,  pInfo->nCol))
		return false;
	if (pInfo->nUseAlphaTest)
	{
		//AlphaTest
		SetRenderIndex(4);
	}
	switch(pInfo->nAlphaType) 
	{
	case 1:
		SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_ONE);
		break;
	case 2:
		SetAlphaType(D3DBLEND_SRCCOLOR,D3DBLEND_INVSRCCOLOR);
		break;
	default:
		SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA);
		break;
	}
	DWORD dwColor =D3DCOLOR_ARGB(pInfo->nColorA,pInfo->nColorR,pInfo->nColorG,pInfo->nColorB);
	setColor(dwColor);
	setTypeID(pInfo->nType);

	return true;
}

bool	CShadeEff::Create(s_string strTexName, float fSize, 
						   bool bAni,int iRow, int iColnum)
{
	_bUpSea = false;
	SetHide( FALSE );

	return CMPShadeCtrl::Create(strTexName,  &ResMgr,  fSize,  bAni, iRow,  iColnum);
}

bool	CShadeEff::CreateAttachLight(int iIdxID, float fRange,D3DXCOLOR dwcolor)
{
	_bUpSea = false;

	CShadeInfo* pInfo = GetShadeInfo(iIdxID);
	if(!pInfo)
		return false;
    std::string str = pInfo->szDataName;
	if(!CMPShadeCtrl::Create(str,  &ResMgr,  fRange, 
		pInfo->nAni!=0, pInfo->nRow,  pInfo->nCol))
		return false;
	if (pInfo->nUseAlphaTest)
	{
		//AlphaTest
		SetRenderIndex(4);
	}
	switch(pInfo->nAlphaType) 
	{
	case 1:
		SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_ONE);
		break;
	case 2:
		SetAlphaType(D3DBLEND_SRCCOLOR,D3DBLEND_INVSRCCOLOR);
		break;
	default:
		SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA);
		break;
	}
	setColor(dwcolor);
	setTypeID(3);

	setIdxID(iIdxID);
	return true;
}

void	CShadeEff::Emission(WORD wID, const D3DXVECTOR3* vBegin, D3DXVECTOR3* vEnd)
{
	_vPos = *vBegin;
	MoveTo (&_vPos);
	CMPShadeCtrl::Play(wID);
}

void	CShadeEff::MoveTo (D3DXVECTOR3* SVerPos)
{
	_vPos = *SVerPos;
	CMPShadeCtrl::MoveTo(_vPos,_pTerrain);
}

void	CShadeEff::FrameMove(DWORD dwDailTime)
{
	if(!IsValid() || IsHide())
		return;
	CMPShadeCtrl::FrameMove(dwDailTime);
}
void	CShadeEff::Render()
{
	if(!IsValid() || IsHide())
		return;
  
	CCharacter*  pCha = NULL;
	CSceneItem* pItem = NULL;
	CSceneObj*	pObj = NULL;

	switch(getTypeID())
	{
	case 1://
		if(_iChaID >= 0)
		{
			pCha = _pScene->GetCha(_iChaID);
			if(pCha)
			{
				_vPos = pCha->GetPos();
				CMPShadeCtrl::MoveTo(_vPos,_pTerrain);
			}
		}
		break;
	case 2://
		if(_iChaID >= 0)
		{
			pItem = _pScene->GetSceneItem(_iChaID);
			if(pItem)
			{
				_vPos = pItem->getPos();
				CMPShadeCtrl::MoveTo(_vPos,_pTerrain);
			}
		}
		break;
	case 3://
		if(_iChaID >= 0)
		{
			pObj = _pScene->GetSceneObj(_iChaID);
			if(pObj)
			{
				_vPos = pObj->pos;
				D3DXCOLOR color = (D3DXCOLOR)pObj->dif;
				color.a = 0.25f;
				setColor(color);
				CMPShadeCtrl::MoveTo(_vPos,_pTerrain);
			}
		}
		break;

	default:
		break;
	}
	if(_pTerrain)
		if(!_pTerrain->IsPointVisible(_vPos.x, _vPos.y)) 
		{
			return;
		}

	CMPShadeCtrl::Render();
}

CPug::CPug()
{
	_bValid = false;
	_dwColor = 0xffffffff;
}
CPug::~CPug()
{
}

bool	CPug::Create(D3DXVECTOR3* pvPos, float fangle, MPMap* pMap)
{
    std::string str = "pug.tga";
	if(!_cShadeEff.Create(str,&ResMgr))
		return false;

	_fAngle = fangle;
	_vPos = *pvPos;
	_cShadeEff.MoveTo(_vPos, pMap, _fAngle);
	_cShadeEff.SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA);
	_cShadeEff.setColor(_dwColor);

	_fCurTime = 0;
	_bValid = true;

	return true;
}
void	CPug::MoveTo(MPMap* pMap)
{
	_cShadeEff.MoveTo(_vPos, pMap, _fAngle);
}

void	CPug::FrameMove(DWORD dwTime)
{
	_fCurTime += *ResMgr.GetDailTime();
	if(_fCurTime > 6)
		_bValid = false;
	else
	{
		_dwColor.a = 0.0f;//1.0f - _fCurTime / 6.0f ;
		_cShadeEff.setColor(_dwColor);
	}
}
void	CPug::Render()
{
	_cShadeEff.Render();
}

CPugMgr::CPugMgr()
{
	_bPug = false;
	_bLeft = false;
}
CPugMgr::~CPugMgr()
{
	ClearMemory();
}

void	CPugMgr::InitMemory(MPMap* pMap)
{
	_pMap = pMap;
	_vecPugArray.clear();
	_vecPugArray.resize(MAXPUG_COUNT);
	_vecValidID.resize(MAXPUG_COUNT);
	_vecValidID.setsize(MAXPUG_COUNT);
	for(WORD iw = 0; iw <  MAXPUG_COUNT; iw++)
	{
		_vecPugArray[iw] = NULL;
		*_vecValidID[iw] = iw;
	}
}
void	CPugMgr::ClearMemory()
{
	for(WORD iw = 0; iw <  MAXPUG_COUNT; iw++)
	{
		SAFE_DELETE(_vecPugArray[iw]);
	}
	_vecPugArray.clear();
	_vecValidID.resize(0);
}

void	CPugMgr::NewPug(D3DXVECTOR3* pvPos, float fangle)
{
	if(!_bPug)
		return;
	if(_vecValidID.size() <= 0)
		return;
	WORD idx = *_vecValidID.front();

	_vecPugArray[idx] = new CPug;

	D3DXMATRIX mat;
	D3DXVECTOR3 vPos = *pvPos;
	D3DXVECTOR3 vCross(0,1,0);
	D3DXMatrixRotationZ(&mat,fangle + 1.570796f);
	D3DXVECTOR4 tvc;
	D3DXVec3Transform(&tvc,&vCross,&mat);
	vCross.x = tvc.x;
	vCross.y = tvc.y;
	vCross.z = tvc.z;

	if(_bLeft)
	{
		vPos += vCross * 0.2f;
		_bLeft = false;
	}
	else
	{
		vPos -= vCross * 0.2f;
		_bLeft = true;
	}

	if(!_vecPugArray[idx]->Create(&vPos, fangle, _pMap))
	{
		SAFE_DELETE(_vecPugArray[idx]);
		return;
	}
	_vecValidID.pop_front();
}

void	CPugMgr::FrameMove(DWORD dwTime)
{
	for(WORD iw = 0; iw <  MAXPUG_COUNT; iw++)
	{
		if(_vecPugArray[iw])
		{
			if(_vecPugArray[iw]->IsValid())
			{
				_vecPugArray[iw]->FrameMove(dwTime);
				if(!_vecPugArray[iw]->IsValid())
				{
					SAFE_DELETE(_vecPugArray[iw]);
					_vecValidID.push_front(iw);
				}
			}
		}
	}
}
void	CPugMgr::Render()
{
	for(WORD iw = 0; iw <  MAXPUG_COUNT; iw++)
	{
		if(_vecPugArray[iw])
		{
			if(_vecPugArray[iw]->IsValid())
			{
				_vecPugArray[iw]->Render();
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////

CNavigationBar	CNavigationBar::g_cNaviBar;

void CNavigationBar::SetTarget(const char* pszName, D3DXVECTOR3& pTarget)
{
	s_string	strTar = pszName;
	_strName = g_pGameApp->GetCurScene()->GetTerrainName();
	if(_strName == "garner")
		_strName = GetLanguageString(56);
	else if(_strName == "magicsea")
		_strName = GetLanguageString(57);
	else if(_strName == "darkblue")
		_strName = GetLanguageString(58);

	{
		_vTarget = pTarget;
		if(_strName == GetLanguageString(56))
		{
			if(strTar == GetLanguageString(57))
			{
				_vTarget.x += 4096;
			}
			else if(strTar == GetLanguageString(58))
			{
				_vTarget.x += 4096 * 2;
			}
		}
		if(_strName == GetLanguageString(57))
		{
			_vTarget.x += 4096;
			if(strTar == GetLanguageString(56))
			{
				_vTarget.x -= 4096;
			}
			else if(strTar == GetLanguageString(58))
			{
				_vTarget.x += 4096;
			}
		}
		if(_strName == GetLanguageString(58))
		{
			_vTarget.x += 4096 * 2;

			if(strTar == GetLanguageString(56))
			{
				_vTarget.x -= 4096 * 2;
			}
			else if(strTar == GetLanguageString(57))
			{
				_vTarget.x -= 4096;
			}
		}
	}
}
void		CNavigationBar::Render()
{
	if(!_bShow)
		return;
	CGameScene* pScene = g_pGameApp->GetCurScene();
	if(!pScene)
		return;

	CCharacter* pCha = g_pGameApp->GetCurScene()->GetMainCha();

	if(!pCha)
		return;

	if (!_pShadeEff)
	{
		_pShadeEff = new CMPShadeCtrl;
        std::string str = "inarraw.tga";
		if(!_pShadeEff->Create(str,&ResMgr))
		{
			SAFE_DELETE(_pShadeEff);
			return;
		}
		_pShadeEff->SetAlphaType(D3DBLEND_SRCALPHA,D3DBLEND_INVSRCALPHA);
		_pShadeEff->setColor(0xffffffff);
		_pShadeEff->GetShadeMap()->SetUpSea(true);
	}
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vPos = pCha->GetPos();
	s_string strName = pScene->GetTerrainName();

	if(strName == "magicsea")
	{
		vPos.x += 4096;
	}
	else if(strName == "darkblue")
	{
		vPos.x += 4096 * 2;
	}

	vPos.x = (float)(int)vPos.x;
	vPos.y = (float)(int)vPos.y;

	const auto v = _vTarget - vPos;
	D3DXVec3Normalize(&vDir, &(v));
	vDir.z = 0;

	float fangle;

	if(vDir.x == 0 && vDir.y == 0)
	{
		fangle = 0;
	}
	else
	{
		float fDist;
		const D3DXVECTOR3 v[] = { D3DXVECTOR3(vDir.x, vDir.y, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f) };
		fDist = D3DXVec3Length(&v[0]);
		fangle = acosf(D3DXVec3Dot(&v[0], &v[1]) / fDist);
		if (vDir.x >= 0.0f) 
		{
			fangle = -fangle;
		}
	}
	vPos = pCha->GetPos();
	vPos += vDir * 3;
	_pShadeEff->MoveTo(vPos,pScene->GetTerrain(), fangle);

	_pShadeEff->FrameMove(CGameApp::GetCurTick());
	_pShadeEff->Render();
}
