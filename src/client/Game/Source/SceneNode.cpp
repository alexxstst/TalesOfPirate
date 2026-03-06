#include "stdafx.h"
#include "SceneNode.h"
#include "Scene.h"

#include "EffectObj.h"

CSceneNode::CSceneNode()
: _dwID(0), _dwTypeID(0), _dwAttachID(0), _dwFrameCnt(0), _dwRenderCnt(0)
, _bValid(0), _nCurX(0), _nCurY(0), _nYaw(0), _nPitch(0), _nRoll(0), _bHide(0)
, _pScene(NULL),_nHeightOff(0), _nPoseHeightOff(0),_nSize(0), lTag(0)
{
	_iEffNum = 0;
	memset(_iEffID,-1,sizeof(_iEffID));
	_iShadeNum = 0;
	memset(_iShadeID,-1,sizeof(_iShadeID));
}

void	CSceneNode::SetShadeShow(int iIdx,bool	bShow)
{
	if(_iShadeNum <= 0 || iIdx >= _iShadeNum)return;

	CShadeEff *pShade = _pScene->GetShadeObj(_iShadeID[iIdx]);
	if(pShade && pShade->IsValid())
	{
		pShade->SetHide(!bShow);
	}
}

void	CSceneNode::SetEffectShow(int iIdx,bool	bShow)
{
	if(_iEffNum <= 0 || iIdx >= _iEffNum)return;

	CEffectObj *pEff = _pScene->GetEffect(_iEffID[iIdx]);
	if(pEff && pEff->IsValid())
	{
		pEff->SetHide(!bShow);
	}
}
