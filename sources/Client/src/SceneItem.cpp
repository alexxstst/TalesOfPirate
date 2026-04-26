#include "stdafx.h"
#include "UIText.h"
#include "SceneItem.h"
#include "SceneItemSet.h"
#include "Scene.h"
#include "Character.h"
#include "LitEntryStore.h"
#include "UIFont.h"
#include "arctrack.h"
#include "event.h"
#include "sceneheight.h"
#include "ItemLitStore.h"
#include "ItemLitAnim.h"
#include "StoneSet.h"
#include "GameApp.h"
#include "ItemRefineSet.h"
#include "ItemRefineEffectRecordStore.h"
#include "EffectObj.h"
#include "UIItemCommand.h"
#include "LootFilter.h"
#include "lwAnimKeySetPRS.h"

CSceneItem::CSceneItem()
	: CSceneNode(), _nCharacterID(-1), _fTerrainHeight(0.0f), _IsSystem(false), _IsShowName(false)
	  , _IsAlpha(false), _pItemInfo(NULL), _pArcTrack(NULL), _pEvent(NULL), _dwForgeValue(0),
	  mParentCharacter(0), mEffect(0) {
	_vPos = D3DXVECTOR3(0, 0, 0);
	_pSceneHeight = new CSceneHeight(this);
}

CSceneItem::~CSceneItem() {
	if (_pArcTrack) delete _pArcTrack;
	delete _pSceneHeight;
}

BOOL CSceneItem::_Create(int nObjTypeID, int nType) {
	_pItemInfo = GetItemRecordInfo(nObjTypeID);
	if (_pItemInfo == NULL) {
		g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(349), nObjTypeID));
		return FALSE;
	}

	const std::string str = std::format("{}.lgo", _pItemInfo->chModule[nType]);

	if (Load(str.c_str()) == 0) {
		g_logManager.InternalLog(LogLevel::Error, "errors",
								 SafeVFormat(GetLanguageString(350), nObjTypeID, _pItemInfo->szName.c_str(), str.c_str()));
		return FALSE;
	}

	SetValid(TRUE);

	// ShowBoundingObject( 1 );
	//ShowHelperMesh( 1 );

	//PlayDefaultAnimation();

	//if( pInfo->nCharacterID != -1 )
	//{
	//    CCharacter* c = this->GetScene()->GetCha( pInfo->nCharacterID );
	//    if( c )
	//    {
	//        if( c->IsBoat() && pInfo->nShipLinkID != -1 )
	//        {
	//            lwSceneItemLinkInfo ili;
	//            ili.id = 0;
	//            ili.obj = this;
	//            ili.link_item_id = pInfo->nItemLinkID;
	//            ili.link_parent_id = pInfo->nShipLinkID;

	//            c->SetSubObjItemLink( 2, &ili );
	//        }
	//    }
	//}

	_IsSystem = false;
	_IsShowName = false;
	_nCharacterID = -1;
	_fTerrainHeight = 0.0f;

	if (const LitEntryRecord* l = LitEntryStore::Instance()->Find(2, str); l && !l->_textures.empty()) {
		lwIPrimitive* p = GetPrimitive();
		lwPrimitiveTexLit(p, l->_textures[0].c_str(), ".\\texture\\lit\\",
						  static_cast<DWORD>(l->_color_op),
						  static_cast<DWORD>(l->_anim_type));
	}

	if (_pArcTrack) {
		delete _pArcTrack;
		_pArcTrack = NULL;
	}

	LitUnresetTexture();

	_pSceneHeight->Reset();
	return TRUE;
}

int CSceneItem::Load(const char* file) {
	_IsSystem = false;
	_IsShowName = false;
	_nCharacterID = -1;
	_fTerrainHeight = 0.0f;

	_dwTypeID = 0;

	_nHeightOff = 0;
	_nPoseHeightOff = 0;
	_bHide = FALSE;
	_dwAttachID = 0;
	_nSize = 0;
	_dwFrameCnt = 0;
	_dwRenderCnt = 0;
	_nCurX = 0;
	_nCurY = 0;
	_Attr.clear();

	if (MPSceneItem::Load(file) == -1)
		return 0;

	return 1;
}

HRESULT CSceneItem::LitResetTexture(DWORD item_id, DWORD lit_id) {
	HRESULT ret = LW_RET_FAILED;

	int flag = 0;
	lwIMtlTexAgent* mtltex_agent;
	lwIAnimCtrlAgent* anim_agent;
	lwIAnimCtrlObjTexUV* ctrlobj_tc;
	lwIAnimCtrlTexUV* ctrl_tc;
	lwIAnimKeySetPRS* aks;
	lwIResourceMgr* res_mgr;
	MPIPrimitive* pri;

	const ItemLitRecord* lit_info = ItemLitStore::Instance()->Find(
		static_cast<int>(item_id), static_cast<int>(lit_id));
	if (lit_info == nullptr)
		goto __ret;

	pri = MPSceneItem::GetObject()->GetPrimitive();
	mtltex_agent = pri->GetMtlTexAgent(1);
	if (mtltex_agent == 0)
		goto __ret;

	res_mgr = pri->GetResourceMgr();
	mtltex_agent->SetRenderFlag(TRUE);
	mtltex_agent->SetOpacity(lit_info->Opacity);
	mtltex_agent->SetTranspType(static_cast<DWORD>(lit_info->TranspType));

	if (!lit_info->TexFile.empty()) {
		lwIPathInfo* path_info = res_mgr->GetSysGraphics()->GetSystem()->GetPathInfo();
		LW_FAILED_RET(pri->ResetTexture(1, 0, lit_info->TexFile.c_str(), path_info->GetPath(PATH_TYPE_TEXTURE_ITEM)));
	}
	if (lit_info->AnimType > 0) {
		anim_agent = pri->GetAnimAgent();
		if (anim_agent == 0) {
			flag = 1;
			res_mgr->CreateAnimCtrlAgent(&anim_agent);
		}
		res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrlobj_tc, ANIM_CTRL_TYPE_TEXCOORD);
		res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_tc, ANIM_CTRL_TYPE_TEXCOORD);
		aks = LW_NEW(lwAnimKeySetPRS2);

		(*__lit_proc[lit_info->AnimType])(aks);

		lwAnimCtrlTypeInfo acti;
		acti.type = ANIM_CTRL_TYPE_TEXCOORD;
		acti.data[0] = 1;
		acti.data[1] = 0;

		ctrl_tc->SetAnimKeySetPRS(aks);
		ctrlobj_tc->AttachAnimCtrl(ctrl_tc);
		ctrlobj_tc->SetTypeInfo(&acti);
		anim_agent->AddAnimCtrlObj(ctrlobj_tc);

		if (flag == 1)
			pri->SetAnimCtrlAgent(anim_agent);

		lwPlayPoseInfo ppi;
		memset(&ppi, 0, sizeof(ppi));
		ppi.bit_mask = PPI_MASK_DEFAULT;
		ppi.type = PLAY_LOOP;
		ppi.pose = 0;
		ppi.velocity = 1.0f; //Testing Mdr.st
		ctrlobj_tc->PlayPose(&ppi);
	}

	ret = LW_RET_OK;
__ret:
	return ret;
}

HRESULT CSceneItem::LitUnresetTexture() {
	HRESULT ret = LW_RET_FAILED;

	lwIMtlTexAgent* mtltex_agent;
	lwIResourceMgr* res_mgr;
	MPIPrimitive* pri;

	pri = MPSceneItem::GetObject()->GetPrimitive();
	mtltex_agent = pri->GetMtlTexAgent(1);
	if (mtltex_agent == 0)
		goto __ret;

	res_mgr = pri->GetResourceMgr();
	mtltex_agent->SetRenderFlag(FALSE);

	ret = LW_RET_OK;
__ret:
	return ret;
}

void CSceneItem::Render() {
	if (GetScene()->GetTerrain()) {
		if (!GetScene()->GetTerrain()->IsPointVisible((float)_nCurX / 100.0f, (float)_nCurY / 100.0f))
			return;
	}

	if (_pItemInfo && g_lootFilter->HasFilteredItem(_pItemInfo->lID)) {
		SetHide(TRUE);
		setIsShowName(false);
		return;
	}

	CSceneNode::Render();

	// g_Render.EnableMipmap(FALSE);

	MPSceneItem::Render();
}

void CSceneItem::PlayArcAni(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, float fVel, float fHei) {
	if (!_pArcTrack) _pArcTrack = new CArcTrack;
	_pArcTrack->Start(vStart, vEnd, fVel, fHei);
}

void CSceneItem::FrameMove(DWORD dwTimeParam) {
	CSceneNode::FrameMove(dwTimeParam);

	if (_pArcTrack) {
		_pArcTrack->FrameMove();

		SetPos((float*)_pArcTrack->GetPos());

		if (_pArcTrack->IsEnd()) {
			delete _pArcTrack;
			_pArcTrack = NULL;

			setPos(_nCurX, _nCurY);
		}
	}
	else if (!IsHide()) {
		if (_nCharacterID == -1) {
			//
			if (_pSceneHeight->FrameMove(dwTimeParam)) {
				_UpdatePos();
			}
		}
		else {
			_nCurX = (int)(MPSceneItem::GetMatrix()->_41 * 100);
			_nCurY = (int)(MPSceneItem::GetMatrix()->_42 * 100);
		}

		if (_IsAlpha) {
			float f = GetOpacity() + 0.03f;
			if (f >= 1.0f) {
				f = 1.0f;
				_IsAlpha = false;
			}
			SetOpacity(f);
		}
	}

	//if(IsValid())
	//GetScene()->HandleSceneMsg(SCENEMSG_SCENEOBJ_MOVE,_nEffID,getID());
	//else
	//	_pCurScene->HandleSceneMsg(SCENEMSG_SCENEOBJ_DESTROY,_nEffID,getID());

	// by lsh
	// FrameMove
	MPSceneItem::FrameMove();
}

void CSceneItem::_UpdateValid(BOOL bValid) {
	if (bValid == FALSE) {
		SetForgeEffect(0);
		if (_pEvent) {
			_pEvent->SetIsValid(false);
			_pEvent = NULL;
		}
		GetScene()->HandleSceneMsg(SCENEMSG_SCENEITEM_DESTROY, getID());
	}
}

void CSceneItem::_UpdatePos() {
	_vPos.x = (float)_nCurX / 100.0f;
	_vPos.y = (float)_nCurY / 100.0f;

	_fTerrainHeight = _pScene->GetGridHeight(_vPos.x, _vPos.y);
	if (_fTerrainHeight < 0.0f) _fTerrainHeight = 0.0f;

	_vPos.z = _fTerrainHeight + (float)_nHeightOff / 100.0f;
	SetPos((float*)_vPos);
}

void CSceneItem::_UpdateHeight() {
	_vPos.x = (float)_nCurX / 100.0f;
	_vPos.y = (float)_nCurY / 100.0f;

	_fTerrainHeight = _pScene->GetGridHeight(_vPos.x, _vPos.y);
	if (_fTerrainHeight < 0.0f) _fTerrainHeight = 0.0f;

	_vPos.z = _fTerrainHeight + (float)_nHeightOff / 100.0f;
	SetPos((float*)_vPos);
}

void CSceneItem::setAlpla(float alpha) {
	SetOpacity(alpha);
	_IsAlpha = true;
}

void CSceneItem::setIsShowName(bool v) {
	_IsShowName = v;
	if (v) {
		ui::GetSize(_pItemInfo->szName.c_str(), _nNameW, _nNameH);
	}
}

bool CSceneItem::IsHitText(int x, int y) {
	if (_IsShowName) {
		static int nX, nY;
		g_Render.WorldToScreen(_vPos.x, _vPos.y, _vPos.z, &nX, &nY);

		nX -= _nNameW / 2;
		return x >= nX && x <= nX + _nNameW && y >= nY && y <= nY + _nNameH;
	}
	return false;
}

void CSceneItem::RenderUI() {
	if (_IsShowName) {
		static int nX, nY;
		g_Render.WorldToScreen(_vPos.x, _vPos.y, _vPos.z, &nX, &nY);

		nX -= _nNameW / 2;
		GetRender().FillFrame(nX, nY, nX + _nNameW, nY + _nNameH, 0x90000000);
		ui::Render(_pItemInfo->szName.c_str(), nX, nY, 0xffffffff);
	}
}

bool CSceneItem::GetItemHeight(float* out_height) {
	return LW_SUCCEEDED(lwPrimitiveGetObjHeight(GetPrimitive(), out_height));
}

void CSceneItem::SetForgeEffect(DWORD value, int nCharID) {
	if (_dwForgeValue == value || !_pItemInfo) return;
	_dwForgeValue = value;

	nCharID--;
	if (nCharID < 0 || nCharID > 3)
		return;

	SItemForge& Forge = SItemForge::Convert(value, _pItemInfo->Id);
	if (!Forge.IsForge || !Forge.pEffectInfo) {
		LitUnresetTexture();

		//
		int nCount = GetEffectNum();
		CEffectObj* pEffect = NULL;
		for (int i = 0; i < nCount; i++) {
			pEffect = _pScene->GetEffect(GetEffectID(i));
			if (pEffect) {
				pEffect->SetValid(FALSE);
			}
		}
		return;
	}

	CItemRefineEffectInfo* pInfo = Forge.pEffectInfo;
	if (!pInfo) return;

	int Level = Forge.nEffectLevel;

	CSceneItem* pItem = this;
	if (pInfo->nLightID != 0)
		pItem->LitResetTexture(pInfo->nLightID, Level); //
	else
		pItem->LitUnresetTexture();

	//
	int nCount = pItem->GetEffectNum();
	CEffectObj* pEffect = NULL;
	for (int i = 0; i < nCount; i++) {
		pEffect = _pScene->GetEffect(pItem->GetEffectID(i));
		if (pEffect) {
			pEffect->SetValid(FALSE);
		}
	}

	int nEffectID = 0;
	for (int i = 0; i < pInfo->GetEffectNum(nCharID); i++) {
		nEffectID = pInfo->sEffectID[nCharID][i] * 10 + Level;
		pEffect = _pScene->GetFirstInvalidEffObj();

		if (!pEffect) {
			ToLogService("errors", LogLevel::Error, "msgSceneItem SetForgeEffect effect fail,ID[{}]", nEffectID);
			continue;
		}

		// dummy
		if (!pEffect->Create(nEffectID)) {
			ToLogService("errors", LogLevel::Error, "msgSceneItem SetForgeEffect effect fail,ID {}", nEffectID);
			continue;
		}
		pEffect->setFollowObj((CSceneNode*)pItem, NODE_ITEM, pInfo->chDummy[i]);
		float fEffectScale = Forge.pRefineInfo->fChaEffectScale[nCharID];
		pEffect->SetScale(fEffectScale, fEffectScale, fEffectScale);
		pEffect->SetAlpha(SItemForge::GetAlpha(Forge.nLevel));
		pEffect->Emission(-1, NULL, NULL);
		pEffect->SetValid(TRUE);

		pItem->AddEffect(pEffect->getID());
	}
	return;
}

void CSceneItem::SetHide(BOOL bHide) {
	if (_bHide == bHide) return;

	_bHide = bHide;
	if (_iEffNum == 0) return;

	CEffectObj* pEff = NULL;
	for (int i = 0; i < _iEffNum; i++) {
		pEff = GetScene()->GetEffect(_iEffID[i]);
		if (pEff && pEff->IsValid()) {
			pEff->SetHide(bHide);
		}
	}
}

CEffectObj* CSceneItem::bindEffect(int dummyID, int effectID, bool isLoop, int angle) {
	if (effectID <= 0)
		return NULL;

	CEffectObj* effect = GetScene()->GetFirstInvalidEffObj();
	if (!effect)
		return NULL;

	if (!effect->Create(effectID))
		return NULL;

	if (isLoop)
		effect->setLoop(isLoop);

	MPMatrix44 matrix;

	if (dummyID >= 0 && GetRunTimeMatrix(&matrix, dummyID) >= 0) {
		if (angle != -1)
			effect->SetEffectDir(angle);
		effect->setFollowObj((CSceneNode*)this, NODE_ITEM, dummyID);
		effect->Emission(-1, (D3DXVECTOR3*)&matrix._41, NULL);
	}
	else {
		if (angle != -1)
			effect->SetEffectDir(angle);
		effect->setFollowObj((CSceneNode*)this, NODE_ITEM);
		effect->Emission(-1, &GetPos(), NULL);
	}

	effect->SetValid(TRUE);
	AddEffect(effectID);

	mEffect = effect;
	return effect;
}

void CSceneItem::unbindEffect() {
	if (mEffect)
		mEffect->SetValid(FALSE);
	mEffect = NULL;
}
