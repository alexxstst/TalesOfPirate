#include "stdafx.h"
#include "Scene.h"
#include "Character.h"
#include "SceneObj.h"
#include "SceneItem.h"
#include "EffectObj.h"
#include "EffectRecordStore.h"
#include "MPModelEff.h"
#include "MPFont.h"
#include "SceneObjRecordStore.h"
#include "GameApp.h"
#include "GameConfig.h"
#include "MPEditor.h"
#include "SceneObjFile.h"
#include "SmallMap.h"
#include "CharacterAction.h"
#include "CharacterRecord.h"
#include "stmove.h"
#include "Actor.h"
#include "UIGuiData.h"
#include "LuaInterface.h"
#include "UICursor.h"
#include "GlobalVar.h"
#include "UIFormMgr.h"
#include "MapRecordStore.h"
#include "GameAppMsg.h"
#include "DrawPointList.h"
#include "terrainattrib.h"
#include "PacketCmd.h"
#include "ItemRecord.h"
#include "SceneLight.h"
#include "SceneObjRecordStore.h"
#include "Track.h"
#include "CommFunc.h"
#include "uiminimapform.h"
#include "uiequipform.h"
#include "uistartform.h"
#include "scenesign.h"
#include "event.h"
#include "cameractrl.h"
#include "uiboatform.h"
#include "UIsystemform.h"
#include "ui3dcompent.h"
#include "uiboxform.h"
#include "LootFilter.h"

using namespace Corsairs::Client::Audio;

using namespace std;

#ifndef USE_DSOUND
#include "AudioThread.h"
extern CAudioThread g_AudioThread;
#endif

const int VOICE_DIS = 1400;

float CGameScene::_fSoundSize = 128.0f;
bool CGameScene::_IsUseSound = true;

CMinimap* CGameScene::_pSmallMap = NULL;
BOOL CGameScene::_bShowMinimap = TRUE;
e3DMouseState CGameScene::_e3DMouseState = enum3DNone;

bool CGameScene::_IsShowPath = false;

CCharacter* CGameScene::_pMainCha = NULL;

//CCharacter2D*	CGameScene::_pCha2D[4];

CBigMap* CGameScene::_pBigMap = NULL;
CLargerMap* CGameScene::_pLargerMap = NULL;


// Added by clp
std::ostream& operator <<(ostream& os, const ReallyBigObjectInfo& info) {
	os << info.typeID << ' ';
	os << info.position.x << ' ' << info.position.y << ' ' << info.position.z << ' ';
	os << info.orientation.w << ' ' << info.orientation.x << ' ' << info.orientation.y << ' ' << info.orientation.z <<
		' ';
	os << info.terrainHeight;
	os << endl;
	return os;
}

std::istream& operator >>(istream& is, ReallyBigObjectInfo& info) {
	is >> info.typeID;
	is >> info.position.x >> info.position.y >> info.position.z;
	is >> info.orientation.w >> info.orientation.x >> info.orientation.y >> info.orientation.z;
	is >> info.terrainHeight;
	return is;
}

void operator <<(FILE* file, const ReallyBigObjectInfo& info) {
	fprintf(file, "%d %f %f %f %f %f %f %f %f\n",
			&info.typeID,
			&info.position.x,
			&info.position.y,
			&info.position.z,
			&info.orientation.w,
			&info.orientation.x,
			&info.orientation.y,
			&info.orientation.z,
			&info.terrainHeight);
}

bool operator <(const ReallyBigObjectInfo& info1, const ReallyBigObjectInfo& info2) {
	return (
		info1.typeID < info2.typeID ||
		info1.position.x < info2.position.x ||
		info1.position.y < info2.position.y ||
		info1.position.z < info2.position.z ||
		info1.orientation.w < info2.orientation.w ||
		info1.orientation.x < info2.orientation.x ||
		info1.orientation.y < info2.orientation.y ||
		info1.orientation.z < info2.orientation.z ||
		info1.terrainHeight < info2.terrainHeight);
}


// Added by clp
void CGameScene::_RecordRBO() {
	for (int i = 0; i < GetSceneObjCnt(); i++) {
		CSceneObj* object = GetSceneObj(i);
		if (this->IsInRBOList(object)) {
			if (object->IsValid() && object->recordRBO) {
				struct ReallyBigObjectInfo info;
				memset(&info, 0, sizeof(ReallyBigObjectInfo));

				info.orientation.w = object->getYaw();
				info.position.x = object->GetCurX();
				info.position.y = object->GetCurY();
				info.position.z = object->getHeightOff();
				info.terrainHeight = object->getRBOHeight();
				info.typeID = object->getTypeID();
				g_ObjFile.GetRBOinfoList().insert(info);
			}
		}
	}
	g_ObjFile.end_RBO();

	for (std::set<CSceneObj*>::iterator itr = _reallyBigObjectList.begin();
		 itr != _reallyBigObjectList.end(); ++itr) {
		(*itr)->SetValid(false);
	}
}

void CGameScene::_ReadRBO() {
	// bug//by clp
	_reallyBigObjectList.clear();
	ifstream file(("map\\" + _stInit.strMapFile + ".rbo").c_str());
	struct ReallyBigObjectInfo info;
	char c1 = file.get();
	char c2 = file.get();
	if (c1 == '\\' && c2 == '\\') {
		string temp;
		getline(file, temp, '\n');
	}
	else {
		file.putback(c2);
		file.putback(c1);
	}

	while (file >> info) {
		CSceneObjInfo* pInfo = GetSceneObjInfo(info.typeID);
		CSceneObj* pObj = AddSceneObj(info.typeID);

		//
		if (pObj) {
			AddRBO(pObj);
			pObj->setHeightOff(info.position.z);
			pObj->setPos(info.position.x, info.position.y);
			pObj->setYaw(info.orientation.w);
			pObj->setRBOHeight(info.terrainHeight);

			pObj->GetObject()->CullPrimitive();
			bool nonVisible = pObj->GetObject()->GetCullingPrimitiveNum() == pObj->GetObject()->GetPrimitiveNum();
			if (nonVisible) {
				pObj->SetHide(TRUE);
			}
		}
	}
	file.close();
}

void CGameScene::SetupVertexFog(MPIDeviceObject* dev_obj, float Start, float End, DWORD Color, DWORD Mode,
								BOOL UseRange, FLOAT Density) {
	// Enable fog blending.
	dev_obj->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// Set the fog color.
	dev_obj->SetRenderState(D3DRS_FOGCOLOR, Color);

	// Set fog parameters.
	if (D3DFOG_LINEAR == Mode) {
		dev_obj->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		dev_obj->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&Start));
		dev_obj->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&End));
	}
	else {
		dev_obj->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		dev_obj->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));
	}

	// Enable range-based fog if desired (only supported for
	// vertex fog). For this example, it is assumed that UseRange
	// is set to a nonzero value only if the driver exposes the
	// D3DPRASTERCAPS_FOGRANGE capability.
	// Note: This is slightly more performance intensive
	//       than non-range-based fog.
	if (UseRange)
		dev_obj->SetRenderState(
			D3DRS_RANGEFOGENABLE,
			TRUE);
}


HRESULT scene_OnLostDevice() {
	g_pGameApp->GetCurScene()->OnLostDevice();
	g_pGameApp->OnLostDevice();
	return TRUE;
}

HRESULT scene_OnResetDevice() {
	g_pGameApp->GetCurScene()->OnResetDevice();
	g_pGameApp->OnResetDevice();
	return TRUE;
}

void CGameScene::OnLostDevice() {
	SAFE_DELETE(_pSmallMap);

	SAFE_DELETE(_pLargerMap);

	g_CEffBox.ReleaseBox();
}

void CGameScene::OnResetDevice() {
	//float lerpx =   float(838) / float(1024);
	//float lerpy =  float(24) / float(768);

	//RECT rc;
	//rc.left = (LONG)(ResMgr.GetBackBufferWidth() * lerpx);
	//rc.top  = (LONG)(ResMgr.GetBackBufferHeight() * lerpy);
	//rc.right= (LONG)(rc.left + 160 * (ResMgr.GetBackBufferWidth() == 1024 ? 1 : lerpx));
	//rc.bottom = (LONG)(rc.top + 160* (ResMgr.GetBackBufferHeight() == 768 ? 1 : lerpx));
	//static int n = 0;


	g_CEffBox.Create(g_Render.GetDevice());
	g_CEffBox.setColor(0xffff0000);
	g_CEffBox.setWriteFrame(FALSE);
	g_CEffBox.ShowLine(TRUE);

	//RECT rc;
	//rc.left = (LONG)(ResMgr.GetBackBufferWidth() - (128 +6));
	//rc.top  = 8;
	//rc.right= (LONG)(rc.left + 128);
	//rc.bottom = (LONG)(rc.top + 128);

	RECT rc;
	CCompent* pRect = g_stUIMap.GetMinimapRect();

	//rc.left = pRect->GetX();// ResMgr.GetBackBufferWidth() - (128 + pRect->GetLeft());
	//rc.top  = pRect->GetY();//pRect->GetTop();
	//rc.right= pRect->GetX2();//rc.left + pRect->GetRight();
	//rc.bottom = pRect->GetY2();//pRect->GetBottom();

	int lenx = pRect->GetRight() - pRect->GetLeft();

	rc.left = ResMgr.GetBackBufferWidth() - (lenx + pRect->GetLeft());
	rc.top = pRect->GetTop();
	rc.right = rc.left + lenx;
	rc.bottom = rc.top + lenx;

	SAFE_DELETE(_pSmallMap);
	_pSmallMap = new CMinimap;
	_pSmallMap->Create(g_Render.GetDevice(), rc, this, 128);

	C3DCompent* pD3d = g_stUIMap.GetBigmapRect();
	rc.left = pD3d->GetX();
	rc.top = pD3d->GetY();
	rc.right = pD3d->GetX2();
	rc.bottom = pD3d->GetY2();

	SAFE_DELETE(_pLargerMap);
	_pLargerMap = new CLargerMap;
	_pLargerMap->Create(g_Render.GetDevice(), rc, g_pGameApp->GetCurScene(), 500);
}

void CGameScene::RegisterFunc() {
}

void CGameScene::ShowMinimap(BOOL bShow) {
	_bShowMinimap = bShow;

	CForm* frmMinimap = CFormMgr::s_Mgr.Find("frmMinimap");
	if (frmMinimap) {
		if (bShow) frmMinimap->SetIsShow(true);
		else frmMinimap->SetIsShow(false);
	}

	/*
	CCompent*pRect = g_stUIMap.GetMinimapRect();
	if(pRect)
	{
		if(bShow) pRect->SetIsShow(true);
		else      pRect->SetIsShow(false);
	}*/
}


//CForm* frm = g_stUIMain.GetMinimapForm();
//frm->GetX(), GetX2(), GetY(), GetY2()

bool CGameScene::_InitScene() {
	//SAFE_DELETE( CGameScene::_pSmallMap );
	//if(_pSmallMap)	return true;

	//rc.left = 25;
	//rc.top  = 25;
	//rc.right= rc.left + 64;
	//rc.bottom = rc.top + 64;

	//for (int n = 0; n < 4; n++)
	//{
	//	if(n > 0)
	//	{
	//		rc.top  += 80;
	//		rc.bottom = rc.top + 64;
	//	}
	//	_pCha2D[n] = new CCharacter2D;
	//	_pCha2D[n]->Create(rc);
	//	//_pCha2D[n]->LoadCha(n + 1);
	//}
	_pBigMap = new CBigMap;

	MPRegisterOutputLoseDeviceProc(scene_OnLostDevice);
	MPRegisterOutputResetDeviceProc(scene_OnResetDevice);

	return true;
}

bool CGameScene::_ClearScene() {
	SAFE_DELETE(_pLargerMap);

	SAFE_DELETE(_pSmallMap);

	//SAFE_DELETE( _pCha2D[0] );
	//SAFE_DELETE( _pCha2D[1] );
	//SAFE_DELETE( _pCha2D[2] );
	//SAFE_DELETE( _pCha2D[3] );
	//SAFE_DELETE(_pBigMap);

	return true;
}

bool CGameScene::_Clear() {
	//SAFE_DELETE( _pSmallMap );
	//SAFE_DELETE( m_cAniWnd );
	_RecordRBO();

	g_pGameApp->GetCursor()->SceneClear();

	ToLogService("common", "CGameScene::_Clear");
	return true;
}

CGameScene::CGameScene(stSceneInitParam& param)
	:
	_pEventMgr(std::make_unique<CEventMgr>(this)),
	_bShowTerrain(FALSE),
	_bShowSceneObj(TRUE),
	_bShowSceneItem(TRUE),
	_nShowChair(0),
	_dwChaTEXFilter(D3DTEXF_LINEAR),
	_stInit(param),
	_pSelCha(NULL),
	_bEnableSceneObjCulling(TRUE),
	_dwMapID(1),
	_bEnableCamDrag(FALSE),
	m_dwValidSceneObjCnt(0),
	m_dwSceneObjPolyCnt(0),
	_pMouseInItem(NULL),
	_pMapInfo(NULL),
	m_bShowTerrain(true) {
	g_CEffBox.Create(g_Render.GetDevice());
	g_CEffBox.setColor(0xffff0000);
	g_CEffBox.setWriteFrame(FALSE);
	g_CEffBox.ShowLine(TRUE);

	CPathBox.Create(g_Render.GetDevice(), 0.25f);

	_nSceneLightCnt = 100;
	_pSceneLightArray = new SceneLight[_nSceneLightCnt];

	_bLightEnable[0] = 1; // parallel light is always running
	_bLightEnable[1] = 0;
	_bLightEnable[2] = 0;

	m_dwEnvColor.a = 1.0f;
	m_dwEnvColor.r = 1.0f;
	m_dwEnvColor.g = 1.0f;
	m_dwEnvColor.b = 1.0f;

	_vecTempShade.resize(200);

	//// begin debug by lsh
	//SceneLight s;
	//s.type = SceneLight::SL_LIGHT;
	//s.x = 730;
	//s.y = 1486;
	//s.z = 0;
	//memset(&s.light, 0, sizeof(s.light));
	//s.range = 200.0f;
	//s.pos = D3DXVECTOR3(730.0f, 1486.0f, 0.0f);
	//s.attenuation1 = 0.5f;

	//s.amb.a = 1.0f;
	//s.amb.r = 1.0f;
	//s.amb.g = 0.0f;
	//s.amb.b = 0.0f;

	//s.dif.a = 1.0f;
	//s.dif.r = 1.0f;
	//s.dif.g = 0.0f;
	//s.dif.b = 0.0f;
	//AddSceneLight(&s);
	//// end

	//g_CFont.UseSoft(false);

	//SIZE size;
	//g_CFont.GetTextSize("hello world",&size);

	//_iw = 200;
	//_ih = 0;

	//LOGFONT LogFont;
	//ZeroMemory( &LogFont, sizeof(LogFont) );
	//LogFont.lfHeight			= -12;
	//LogFont.lfWidth				= 0;
	//LogFont.lfEscapement		= 0;
	//LogFont.lfOrientation		= 0;
	//LogFont.lfWeight			= 0;
	//LogFont.lfItalic			= 0;
	//LogFont.lfUnderline			= 0;
	//LogFont.lfStrikeOut			= FALSE;
	//LogFont.lfCharSet			= DEFAULT_CHARSET;
	//LogFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
	//LogFont.lfClipPrecision		= CLIP_DEFAULT_PRECIS;
	//LogFont.lfQuality			= ANTIALIASED_QUALITY;
	//LogFont.lfPitchAndFamily	= VARIABLE_PITCH;
	//lstrcpy( LogFont.lfFaceName, "" );

	//D3DXCreateFontIndirect(g_Render.GetDevice(),&LogFont,&temFont);

	//temPanel = new CUIPanel;
	//temPanel->Create(g_Render.GetDevice());

	//D3DXCreateSprite( g_Render.GetDevice(), &_2DSprite );
}

CGameScene::~CGameScene() {
	_ClearMemory();
}

void CGameScene::SetFrameMove(DWORD dwTime) {
	_FrameMove(dwTime);
}

void CGameScene::_ClearAllCha() {
	SAFE_DELETE_ARRAY(_pChaArray);
	_nChaCnt = 0;
}

void CGameScene::_ClearAllSceneObj() {
	SAFE_DELETE_ARRAY(_pSceneObjArray);
	_nSceneObjCnt = 0;
}

void CGameScene::_ClearAllSceneItem() {
	SAFE_DELETE_ARRAY(_pSceneItemArray);
	_nSceneItemCnt = 0;
}

void CGameScene::_ClearAllEff() {
	SAFE_DELETE_ARRAY(_pEffectArray);
	_nEffCnt = 0;
}

void CGameScene::_CreateChaArray(int nChaCnt) {
	_ClearAllCha();

	_pChaArray = new CCharacter[nChaCnt];
	_free_chas.ReSize(nChaCnt);
	for (int i = 0; i < nChaCnt; i++) {
		_pChaArray[i].setID(i);
		_pChaArray[i].SetScene(this);

		_free_chas.Push(&_pChaArray[i]);
	}

	_nChaCnt = nChaCnt;
}

void CGameScene::_CreateSceneObjArray(int nSceneObjCnt) {
	_ClearAllSceneObj();

	_pSceneObjArray = new CSceneObj[nSceneObjCnt];

	for (int i = 0; i < nSceneObjCnt; i++) {
		_pSceneObjArray[i].setID(i);
	}

	_nSceneObjCnt = nSceneObjCnt;
}

void CGameScene::_CreateSceneItemArray(int nSceneItemCnt) {
	_ClearAllSceneItem();

	_pSceneItemArray = new CSceneItem[nSceneItemCnt];
	_free_items.ReSize(nSceneItemCnt);
	for (int i = 0; i < nSceneItemCnt; i++) {
		_pSceneItemArray[i].setID(i);
		_pSceneItemArray[i].SetScene(this);

		_free_items.Push(&_pSceneItemArray[i]);
	}

	_nSceneItemCnt = nSceneItemCnt;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CGameScene::_CreateEffectArray(int nEffCnt) {
	_pEffectArray = new CEffectObj[nEffCnt];
	_free_effs.ReSize(nEffCnt);
	for (int i = 0; i < nEffCnt; i++) {
		_pEffectArray[i].setID(i);
		_pEffectArray[i].SetScene(this);

		_free_effs.Push(&_pEffectArray[i]);
	}

	_nEffCnt = nEffCnt;
}

CEffectObj* CGameScene::GetFirstInvalidEffObj() {
	CEffectObj* pObj = _free_effs.Pop();
	if (pObj) return pObj;

	if (_pEffectArray == NULL) return NULL;

	for (int i = 0; i < _nEffCnt; i++) {
		if (_pEffectArray[i].IsValid() == FALSE) {
			return &_pEffectArray[i];
		}
	}
	return NULL;
}

CEffectObj* CGameScene::GetEffect(int nArrayID) {
	if (nArrayID >= _nEffCnt || nArrayID < 0) return NULL;
	return &_pEffectArray[nArrayID];
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CCharacter* CGameScene::_GetFirstInvalidCha() {
	CCharacter* pCha = _free_chas.Pop();
	if (pCha) return pCha;

	if (_pChaArray == NULL) return NULL;

	for (int i = 0; i < _nChaCnt; i++) {
		pCha = &_pChaArray[i];
		if (pCha->IsValid() == FALSE) return pCha;
	}
	return NULL;
}

CSceneObj* CGameScene::_GetFirstInvalidSceneObj(int nTypeID, BOOL& bCreate) {
	if (_pSceneObjArray == NULL) return NULL;
	int i;
	for (i = 0; i < _nSceneObjCnt; i++) {
		CSceneObj* pObj = &_pSceneObjArray[i];
		if (pObj->IsValid() == FALSE && pObj->getTypeID() == nTypeID) {
			return pObj;
		}
	}
	for (i = 0; i < _nSceneObjCnt; i++) {
		CSceneObj* pObj = &_pSceneObjArray[i];
		if (pObj->IsValid() == FALSE) {
			bCreate = TRUE;
			return pObj;
		}
	}
	return NULL;
}

CSceneItem* CGameScene::_GetFirstInvalidSceneItem() {
	CSceneItem* pObj = _free_items.Pop();
	if (pObj) return pObj;

	if (_pSceneItemArray == NULL)
		return NULL;

	for (int i = 0; i < _nSceneItemCnt; i++) {
		pObj = &_pSceneItemArray[i];

		if (pObj->IsValid() == FALSE)
			return pObj;
	}
	return NULL;
}

CCharacter* CGameScene::GetCha(int nArrayID) {
	if (nArrayID >= _nChaCnt || nArrayID < 0) return NULL;
	return &_pChaArray[nArrayID];
}

CSceneObj* CGameScene::GetSceneObj(int nArrayID) {
	if (nArrayID >= _nSceneObjCnt || nArrayID < 0) return NULL;
	return &_pSceneObjArray[nArrayID];
}

CSceneItem* CGameScene::GetSceneItem(int nArrayID) {
	if (nArrayID >= _nSceneItemCnt || nArrayID < 0)
		return NULL;

	return &_pSceneItemArray[nArrayID];
}

CCharacter* CGameScene::AddCharacter(const LoadChaInfo* info) {
	CCharacter* pCha = _GetFirstInvalidCha();
	if (pCha) {
		pCha->Destroy();
		pCha->SetScene(this);
		if (pCha->LoadCha(info) == 0)
			return NULL;
		pCha->SetValid(TRUE);
		pCha->EnableAI(TRUE);

		HandleSceneMsg(SCENEMSG_CHA_CREATE, pCha->getID(), info->cha_id);
	}
	return pCha;
}

CSceneItem* CGameScene::HitSceneItemText(int nScrX, int nScrY) {
	CSceneItem* pObj = HitTestSceneItem(nScrX, nScrY);
	if (pObj) {
		_pMouseInItem = pObj;
	}

	if (!_IsShowItemName) return pObj;

	for (int i = 0; i < _nSceneItemCnt; i++) {
		pObj = &_pSceneItemArray[i];
		if (pObj->IsValid() && !pObj->IsHide() && pObj->IsHitText(nScrX, nScrY)) {
			return pObj;
		}
	}

	return NULL;
}

CSceneItem* CGameScene::HitTestSceneItem(int nScrX, int nScrY) {
	CSceneItem* pObj;
	CSceneItem* pObjXXX = 0;

	float dis = 0.0f;
	MPPickInfo info;
	MPVector3 org, ray;

	g_Render.GetPickRayVector(nScrX, nScrY, (D3DXVECTOR3*)&org, (D3DXVECTOR3*)&ray);
	MPVector3Normalize(&ray);

	for (int i = 0; i < _nSceneItemCnt; i++) {
		pObj = &_pSceneItemArray[i];

		if (pObj->IsValid() && pObj->IsHide() == FALSE && pObj->getAttachedCharacterID() == -1) {
			if (SUCCEEDED(pObj->HitTestPrimitive( &info, &org, &ray ))) {
				if ((pObjXXX == NULL) || (dis > info.dis)) {
					pObjXXX = pObj;
					dis = info.dis;
				}
			}
		}
	}

	return pObjXXX;
}

CSceneObj* CGameScene::HitTestSceneObj(int nScrX, int nScrY) {
	CSceneObj* pObj;
	CSceneObj* pObjXXX = 0;

	float dis = 0.0f;
	MPPickInfo info;
	MPVector3 org, ray;

	g_Render.GetPickRayVector(nScrX, nScrY, (D3DXVECTOR3*)&org, (D3DXVECTOR3*)&ray);
	MPVector3Normalize(&ray);

	for (int i = 0; i < _nSceneObjCnt; i++) {
		pObj = &_pSceneObjArray[i];

		if (pObj->IsValid() && pObj->IsHide() == FALSE) {
			if (SUCCEEDED(pObj->HitTestPrimitive( &info, &org, &ray ))) {
				if ((pObjXXX == NULL) || (dis > info.dis)) {
					pObjXXX = pObj;
					dis = info.dis;
				}
			}
		}
	}

	return pObjXXX;
}

CCharacter* CGameScene::HitTestCharacter(int nScrX, int nScrY) {
	CCharacter* pObj;
	CCharacter* pObjXXX = 0;

	float dis = 0.0f;
	MPPickInfo info;
	MPVector3 org, ray;

	g_Render.GetPickRayVector(nScrX, nScrY, (D3DXVECTOR3*)&org, (D3DXVECTOR3*)&ray);
	MPVector3Normalize(&ray);

	for (int i = 0; i < _nChaCnt; i++) {
		pObj = &_pChaArray[i];

		if (pObj->IsValid() && !pObj->IsHide()) {
			if (SUCCEEDED(pObj->HitTestPrimitive( &info, &org, &ray ))) {
				if ((pObjXXX == NULL) || (dis > info.dis)) {
					pObjXXX = pObj;
					dis = info.dis;
				}
			}
		}
	}

	return pObjXXX;
}

CCharacter* CGameScene::HitCharacter(int nScrX, int nScrY) {
	CCharacter* pObj;
	CCharacter* pObjXXX = 0;

	float dis = 0.0f;
	MPPickInfo info;
	MPVector3 org, ray;

	g_Render.GetPickRayVector(nScrX, nScrY, (D3DXVECTOR3*)&org, (D3DXVECTOR3*)&ray);
	MPVector3Normalize(&ray);

	for (int i = 0; i < _nChaCnt; i++) {
		pObj = &_pChaArray[i];

		if (pObj->IsValid() && !pObj->IsHide()) {
			if (SUCCEEDED(pObj->HitTestPrimitive( &info, &org, &ray ))) {
				if (!pObjXXX) {
					pObjXXX = pObj;
					dis = info.dis;
				}
				else if (pObj->getChaCtrlType() > pObjXXX->getChaCtrlType()) {
					pObjXXX = pObj;
					dis = info.dis;
				}
				else if (pObj->getChaCtrlType() == pObjXXX->getChaCtrlType() && dis > info.dis) {
					pObjXXX = pObj;
					dis = info.dis;
				}
			}
		}
	}

	return pObjXXX;
}

int CGameScene::HitTestSceneObjTerrainForInfluence(D3DXVECTOR3* t_pos, const D3DXVECTOR3* nPos) {
	MPPickInfo p;
	CSceneObj* pObj;
	DWORD s = 0;
	MPVector3 h(0.0f, 0.0f, 0.0f);
	HRESULT ret;


	D3DXVECTOR3 org(*nPos);
	D3DXVECTOR3 ray(0.0f, 0.0f, -1.0f);

	const float __max_visible_size = 25.0f;

	for (int i = 0; i < _nSceneObjCnt; i++) {
		pObj = &_pSceneObjArray[i];


		if (pObj && pObj->IsValid()) {
			D3DXVECTOR3 x_pos = pObj->getPos() - *nPos;

			if (x_pos.x * x_pos.x + x_pos.y * x_pos.y > __max_visible_size * __max_visible_size) {
				//static int discard_num = 0;
				//LG( "ppp", "Discard: %d\n",++discard_num );
				continue;
			}

			//static int hit_num = 0;
			//LG( "ppp", "Hit: %d\n",++hit_num );

			if (SUCCEEDED(ret = pObj->HitTestHelperMesh( &p, (MPVector3*)&org, (MPVector3*)&ray, "block" ))) {
				if (s == 0 || h.z < p.pos.z) {
					h = p.pos;
					s = 2;
				}
			}
			if (SUCCEEDED(ret = pObj->HitTestHelperMesh( &p, (MPVector3*)&org, (MPVector3*)&ray, "terrain" ))) {
				if (s == 0 || h.z < p.pos.z) {
					h = p.pos;
					s = 1;
				}
			}
		}
	}


	//MPTile* tile = GetCurMap()->GetGroupTile( (int)org->x, (int)org->y, 0 );
	float fHeight = 0.0f;
	if (_pTerrain) {
		_pTerrain->GetHeight(h.x, h.y);
	}


	if (s && h.z < fHeight - 0.3f) //
	{
		//h.z = 0;
		s = 0;
	}

	*(MPVector3*)t_pos = h;

	return s;
}

int CGameScene::HitTestSceneObjTerrain(D3DXVECTOR3* t_pos, const D3DXVECTOR3* org, const D3DXVECTOR3* ray) {
	MPPickInfo p;
	CSceneObj* pObj;
	DWORD s = 0;
	MPVector3 h(0.0f, 0.0f, 0.0f);
	HRESULT ret;

	for (int i = 0; i < _nSceneObjCnt; i++) {
		pObj = &_pSceneObjArray[i];

		if (pObj && pObj->IsValid()) {
			//if( FAILED( pObj->HitTest( &p, (MPVector3*)org, (MPVector3*)ray ) ) )
			//    continue;

			if (SUCCEEDED(ret = pObj->HitTestHelperMesh( &p, (MPVector3*)org, (MPVector3*)ray, "block" ))) {
				if (s == 0 || h.z < p.pos.z) {
					h = p.pos;
					s = 2;
				}
			}
			if (SUCCEEDED(ret = pObj->HitTestHelperMesh( &p, (MPVector3*)org, (MPVector3*)ray, "terrain" ))) {
				if (s == 0 || h.z < p.pos.z) {
					h = p.pos;
					s = 1;
				}
			}
		}
	}


	//MPTile* tile = GetCurMap()->GetGroupTile( (int)org->x, (int)org->y, 0 );
	float fHeight = 0.0f;
	if (_pTerrain) {
		_pTerrain->GetHeight(h.x, h.y);
	}


	if (s && h.z < fHeight - 0.3f) //
	{
		h.z = 0;
		s = 0;
	}

	//if( s && h.z < g_pGameApp->GetMouseMap()->z )
	//{
	//    h = *(MPVector3*)g_pGameApp->GetMouseMap();
	//    s = 0;
	//}

	*(MPVector3*)t_pos = h;

	return s;
}


int CGameScene::HitTestSceneObjTerrain(D3DXVECTOR3* t_pos, const D3DXVECTOR3* pos) {
	MPVector3 org(*(MPVector3*)pos);
	MPVector3 ray(0.0f, 0.0f, -1.0f);

	return HitTestSceneObjTerrain(t_pos, (D3DXVECTOR3*)&org, (D3DXVECTOR3*)&ray);
}

int CGameScene::HitTestSceneObjChair(D3DXMATRIX* t_mat, int* h, const D3DXVECTOR3* nOrg, const D3DXVECTOR3* nRay) {
	MPPickInfo u;
	CSceneObj* pObj;

#if 1
	list<int>::iterator it = _SceneObjIdx[SCENEOBJ_TYPE_POSE].begin();
	list<int>::iterator it_end = _SceneObjIdx[SCENEOBJ_TYPE_POSE].end();

	for (; it != it_end; ++it) {
		int i = (*it);
#else
		for (int i = 0; i < _nSceneObjCnt; i++) {

#endif
		pObj = &_pSceneObjArray[i];

		if (pObj && pObj->IsValid()) {
			MPIPrimitive* p = pObj->GetPrimitive(0);
			if (p == 0)
				continue;

			lwIHelperObject* ho = p->GetHelperObject();
			if (ho == 0)
				continue;

			lwIHelperBox* hb = ho->GetHelperBox();
			if (hb == 0)
				continue;

			lwHelperBoxInfo* hbi = hb->GetDataInfoWithID(0);

			if (_tcscmp(hbi->name, "chair") == 0) {
				if (SUCCEEDED(pObj->HitTestPrimitive( &u, (MPVector3*)nOrg, (MPVector3*)nRay ))) {
					MPMatrix44Multiply((MPMatrix44*)t_mat, &hbi->mat, p->GetMatrixGlobal());

					*h = pObj->getHeightOff();
					return 1;
				}
			}

			//if( SUCCEEDED( pObj->HitTestPrimitiveHelperBox( &u, (MPVector3*)nOrg, (MPVector3*)nRay, "chair" ) ) )
			//{
			//    MPIPrimitive* p = pObj->GetPrimitive( u.data );

			//    lwIHelperBox* hb = p->GetHelperObject()->GetHelperBox();
			//    lwHelperBoxInfo* hbi = hb->GetDataInfoWithID( u.obj_id );

			//    MPMatrix44Multiply( (MPMatrix44*)t_mat, &hbi->mat, p->GetMatrixGlobal() );

			//    *h = pObj->getHeightOff();
			//    return 1;
			//}
		}
	}


	//return HitTestChair( t_mat, nOrg, nRay );

	return 0;
}

int CGameScene::HitTestSceneObjWall(D3DXMATRIX* t_mat, const D3DXVECTOR3* nOrg, const D3DXVECTOR3* nRay) {
	MPPickInfo u;
	CSceneObj* pObj;

#if 1
	list<int>::iterator it = _SceneObjIdx[SCENEOBJ_TYPE_POSE].begin();
	list<int>::iterator it_end = _SceneObjIdx[SCENEOBJ_TYPE_POSE].end();

	for (; it != it_end; ++it) {
		int i = (*it);
#else
		for (int i = 0; i < _nSceneObjCnt; i++) {

#endif
		pObj = &_pSceneObjArray[i];

		if (pObj->IsValid()) {
			MPIPrimitive* p = pObj->GetPrimitive(0);
			if (p == 0)
				continue;

			lwIHelperMesh* hm = p->GetHelperObject()->GetHelperMesh();

			if (hm) {
				lwHelperMeshInfo* hmi = hm->GetDataInfoWithID(0);

				if (_tcscmp(hmi->name, "wall") == 0) {
					if (SUCCEEDED(pObj->HitTestPrimitive( &u, (MPVector3*)nOrg, (MPVector3*)nRay ))) {
						MPMatrix44Multiply((MPMatrix44*)t_mat, &hmi->mat, p->GetMatrixGlobal());

						return 1;
					}
				}
			}

			//if( SUCCEEDED( pObj->HitTestPrimitiveHelperMesh( &u, (MPVector3*)nOrg, (MPVector3*)nRay, "wall" ) ) )
			//{
			//    MPIPrimitive* p = pObj->GetPrimitive( u.data );

			//    lwIHelperMesh* hb = p->GetHelperObject()->GetHelperMesh();
			//    lwHelperMeshInfo* hbi = hb->GetDataInfoWithID( u.obj_id );

			//    MPMatrix44Multiply( (MPMatrix44*)t_mat, &hbi->mat, p->GetMatrixGlobal() );
			//    return 1;
			//}
		}
	}

	return 0;
}

int CGameScene::HitTestSceneObjChair(D3DXVECTOR3* t_pos, int* t_angle, int* h, const D3DXVECTOR3* nOrg,
									 const D3DXVECTOR3* nRay) {
	D3DXMATRIX mat;

	if (HitTestSceneObjChair(&mat, h, nOrg, nRay) == 0)
		return 0;

	*t_pos = *(D3DXVECTOR3*)&mat._41;
	float a = atan2(mat._22, mat._21);
	*t_angle = Radian2Angle(a);

	return 1;
}

int CGameScene::HitTestSceneObjWall(D3DXVECTOR3* t_pos, int* t_angle, const D3DXVECTOR3* nOrg,
									const D3DXVECTOR3* nRay) {
	D3DXMATRIX mat;

	if (HitTestSceneObjWall(&mat, nOrg, nRay) == 0)
		return 0;

	*t_pos = *(D3DXVECTOR3*)&mat._41;
	*t_angle = (int)(atan2(mat._22, mat._21) * 180 / D3DX_PI);
	return 1;
}

void CGameScene::ShowSceneObjTerrain(BOOL bShow) {
	for (int i = 0; i < _nSceneObjCnt; i++) {
		_pSceneObjArray[i].ShowHelperMesh(bShow);
	}
	_bShowTerrain = bShow;
}

int CGameScene::GetMainChaPickRay(MPVector3* org, MPVector3* ray) {
	if (_pMainCha == 0)
		return 0;

#define USE_MAINCHARACTER_POS

#if(defined USE_CAMERA_TARGET)
	MPVector3 target = (MPVector3&)GetMainCha()->GetPos();
	*(D3DXVECTOR3*)(org) = g_pGameApp->GetMainCam()->m_EyePos;
#endif

#if(defined USE_MAINCHARACTER_POS)
	MPVector3 target = (MPVector3&)GetMainCha()->GetPos();
	*org = target;
	org->z += 100.0f;
#endif

	MPVector3Sub(ray, &target, org);
	MPVector3Normalize(ray);

	return 1;
}

void CGameScene::EnableChaTexLinearFilter(BOOL flag) {
	_dwChaTEXFilter = flag == 1 ? D3DTEXF_LINEAR : D3DTEXF_POINT;
}

CEffectObj* CGameScene::HitTestEffectObj(int nScrX, int nScrY) {
	MPVector3 org, ray;
	g_Render.GetPickRayVector(nScrX, nScrY, (D3DXVECTOR3*)&org, (D3DXVECTOR3*)&ray);
	MPVector3Normalize(&ray);
	CEffectObj* pEffObj = NULL;

	//g_CEffBox.Show(FALSE);

	for (int i = 0; i < _nEffCnt; i++) {
		pEffObj = &_pEffectArray[i];
		if (pEffObj->IsValid()) {
			pEffObj->ShowBox(FALSE);

			if (pEffObj->IsSceneEffect())
				if (pEffObj->HitTestPrimitive(org, ray)) {
					//g_CEffBox.setPos(pEffObj->getPos());
					//g_CEffBox.Show(TRUE);
					pEffObj->ShowBox(TRUE);
					return pEffObj;
				}
		}
	}
	return NULL;
}


void CGameScene::UnhideAllSceneObj() {
	CSceneObj* pObj;
	for (int i = 0; i < _nSceneObjCnt; i++) {
		pObj = &_pSceneObjArray[i];

		if (pObj && pObj->IsValid()) {
			pObj->SetHide(FALSE);
		}
	}
}

void CGameScene::ResetAllSceneObjPos() {
	CSceneObj* pObj;
	CEffectObj* pEffObj;

	int i;
	for (i = 0; i < _nSceneObjCnt; i++) {
		pObj = &_pSceneObjArray[i];
		if (pObj->IsValid())
			pObj->setPos(pObj->GetCurX(), pObj->GetCurY());
	}
	for (i = 0; i < _nEffCnt; i++) {
		pEffObj = &_pEffectArray[i];
		if (pEffObj->IsValid())
			pEffObj->setPos(pEffObj->GetCurX(), pEffObj->GetCurY());
	}
}

int CGameScene::GetValidSceneObjCnt() {
	int nValidCnt = 0;
	for (int i = 0; i < _nSceneObjCnt; i++) {
		CSceneObj* pObj = &_pSceneObjArray[i];
		if (pObj->IsValid()) {
			nValidCnt++;
		}
	}
	return nValidCnt;
}

bool CGameScene::_CreateMemory() {
	ToLogService("common", "Create - Map {}, Eff: {}, Cha: {}, Obj: {}, Item: {}", _stInit.strMapFile.c_str(),
				 _stInit.nMaxEff, _stInit.nMaxCha, _stInit.nMaxObj, _stInit.nMaxItem);
	ToLogService("common", "Creatememory {}", _stInit.strMapFile.c_str());
	_pMainCha = NULL;

	LoadMap(_stInit.strMapFile.c_str());

	_nChaCnt = 0;
	_nSceneObjCnt = 0;
	_nSceneItemCnt = 0;
	_nEffCnt = 0;

	if (_stInit.nMaxEff > 0) {
		switch (g_stUISystem.m_sysProp.m_videoProp.bResolution) {
		case 0:
			_CreateEffectArray(_stInit.nMaxEff);
			break;
		case 1:
			_CreateEffectArray(_stInit.nMaxEff * 2);
			break;
		case 2:
			_CreateEffectArray(_stInit.nMaxEff * 3);
			break;
		case 3:
			_CreateEffectArray(_stInit.nMaxEff * 4);
			break;
		case 4:
			_CreateEffectArray(_stInit.nMaxEff * 5);
			break;
		case 5:
			_CreateEffectArray(_stInit.nMaxEff * 6);
			break;
		default:
			_CreateEffectArray(_stInit.nMaxEff);
			break;
		}
	}
	if (_stInit.nMaxCha > 0) {
		switch (g_stUISystem.m_sysProp.m_videoProp.bResolution) {
		case 0:
			_CreateChaArray(_stInit.nMaxCha);
			break;
		case 1:
			_CreateChaArray(_stInit.nMaxCha * 2);
			break;
		case 2:
			_CreateChaArray(_stInit.nMaxCha * 3);
			break;
		case 3:
			_CreateChaArray(_stInit.nMaxCha * 4);
			break;
		case 4:
			_CreateChaArray(_stInit.nMaxCha * 5);
			break;
		case 5:
			_CreateChaArray(_stInit.nMaxCha * 6);
			break;
		default:
			_CreateChaArray(_stInit.nMaxCha);
			break;
		}
	}
	if (_stInit.nMaxObj > 0) {
		switch (g_stUISystem.m_sysProp.m_videoProp.bResolution) {
		case 0:
			_CreateSceneObjArray(_stInit.nMaxObj);
			break;
		case 1:
			_CreateSceneObjArray(_stInit.nMaxObj * 2);
			break;
		case 2:
			_CreateSceneObjArray(_stInit.nMaxObj * 3);
			break;
		case 3:
			_CreateSceneObjArray(_stInit.nMaxObj * 4);
			break;
		case 4:
			_CreateSceneObjArray(_stInit.nMaxObj * 5);
			break;
		case 5:
			_CreateSceneObjArray(_stInit.nMaxObj * 6);
			break;
		default:
			_CreateSceneObjArray(_stInit.nMaxObj);
			break;
		}
	}
	if (_stInit.nMaxItem > 0) {
		switch (g_stUISystem.m_sysProp.m_videoProp.bResolution) {
		case 0:
			_CreateSceneItemArray(_stInit.nMaxItem);
			break;
		case 1:
			_CreateSceneItemArray(_stInit.nMaxItem * 2);
			break;
		case 2:
			_CreateSceneItemArray(_stInit.nMaxItem * 3);
			break;
		case 3:
			_CreateSceneItemArray(_stInit.nMaxItem * 4);
			break;
		case 4:
			_CreateSceneItemArray(_stInit.nMaxItem * 5);
			break;
		case 5:
			_CreateSceneItemArray(_stInit.nMaxItem * 6);
			break;
		default:
			_CreateSceneItemArray(_stInit.nMaxItem);
			break;
		}
	}
	ToLogService("common", "create array");

	_CreateShadeArray(300);

	//lemon add@2005.2.17
	_pcPugMgr = std::make_unique<CPugMgr>();
	_pcPugMgr->InitMemory(GetTerrain());

	ToLogService("common", "CreateShadeArray");
	ToLogService("common", "init ok");

	_ReadRBO();

	return true;
}

bool CGameScene::_ClearMemory() {
	ToLogService("common", "\tClear - Map {}, Eff: {}, Cha: {}, Obj: {}, Item: {}", _stInit.strMapFile.c_str(),
				 _stInit.nMaxEff, _stInit.nMaxCha, _stInit.nMaxObj, _stInit.nMaxItem);

	_pMainCha = NULL;
	//if( CGameApp::IsMusicSystemValid() )	// music
	// ::mus_mgr_reset();

	_areaeffs.clear();

	_pTerrain.reset();

	_ClearAllShade();
	_ClearAllEff(); // ,,

	_ClearAllCha();
	_ClearAllSceneObj();
	_ClearAllSceneItem();
	//lemon add@2005.2.17
	_pcPugMgr.reset();

	delete[] _pSceneLightArray;
	_pSceneLightArray = NULL;
	_nSceneLightCnt = 0;

	_pMainCha = NULL;
	_pSelCha = NULL;

	_pEventMgr->Clear();

	delete[] _TranspObjStateSeq;
	_TranspObjStateSeq = 0;
	_dwTranspObjStateNum = 0;
	_dwTranspObjStateID = 0;

	CSynchroManage::I()->Reset();
	return true;
}

// nCurX, nCurYnDist
long CGameScene::AddCharacterBlock(int nCurX, int nCurY, int nDist, BYTE* byBlockBuff, int nBuffWidth, int sRadii) {
	return 0;

	long lRet = 1;
	__int64 nDistX, nDistY;

	int nPosX, nPosY;
	int nStartX, nEndX, nStartY, nEndY;
	int i, j, k;
	int nBuffHeight = nBuffWidth;
	int nCharacterBlockRadius = 40; //
	const int nBlockUnitWidth = 50, nBlockUnitHeight = 50;
	int nRange;

	if (_pChaArray == NULL || byBlockBuff == NULL)
		return 0;

	nDist *= nDist;

	//float f1, f2, f3;
	for (i = 0; i < _nChaCnt; i++) {
		CCharacter* pCha = &_pChaArray[i];
		if (!pCha->IsValid())
			continue;

		if (pCha == _pMainCha)
			continue;

		nCharacterBlockRadius = pCha->GetDefaultChaInfo()->sRadii + sRadii;

		nPosX = pCha->GetCurX();
		nPosY = pCha->GetCurY();
		nDistX = nCurX * nBlockUnitWidth - nPosX;
		nDistY = nCurY * nBlockUnitHeight - nPosY;
		nStartX = (nPosX - nCharacterBlockRadius) / nBlockUnitWidth;
		nEndX = (nPosX + nCharacterBlockRadius - 1) / nBlockUnitWidth;
		nStartY = (nPosY - nCharacterBlockRadius) / nBlockUnitHeight;
		nEndY = (nPosY + nCharacterBlockRadius - 1) / nBlockUnitHeight;

		if (nDistX * nDistX + nDistY * nDistY <= nDist) {
			for (j = nStartY; j <= nEndY; j++) {
				for (k = nStartX; k <= nEndX; k++) {
					nRange = (nBuffHeight / 2 - (nCurY - j)) * nBuffWidth + (nBuffWidth / 2 - (nCurX - k));
					if (nRange >= 0 && nRange < 65535) {
						byBlockBuff[nRange] = 1;
					}
					else {
						ToLogService(
							"common",
							"msgCGameScene::AddCharacterBlock nRange[{}] isn't Valid, Cha name[{}], pos:[{}, {}]",
							nRange, pCha->getName().c_str(), pCha->GetCurX(), pCha->GetCurY());
						return 0;
					}
				}
			}
		}
	}

	return lRet;
}

bool CGameScene::_HandleSuperKey() {
#ifdef _DEBUG
	if (g_pGameApp->IsKeyDown(DIK_V)) {
		EnableSceneObjCulling(1 - IsSceneObjCulling());
		TipI(IsSceneObjCulling(), GetLanguageString(329).c_str(), GetLanguageString(330).c_str());
	}
	else if (g_pGameApp->IsKeyDown(DIK_F5)) {
		static BOOL g_bEnablePrint = FALSE;
		g_bEnablePrint = 1 - g_bEnablePrint;
		g_Render.EnablePrint(INFO_FPS, g_bEnablePrint);
		g_Render.EnablePrint(INFO_DEBUG, g_bEnablePrint);
		TipI(g_bEnablePrint, GetLanguageString(331).c_str(), GetLanguageString(332).c_str());
	}
	else if (g_pGameApp->IsKeyDown(DIK_F6)) {
		//g_pGameApp->ResetGameCamera();
		Tip(GetLanguageString(333).c_str());
	}
	else if (g_pGameApp->IsKeyDown(DIK_F7)) {
		this->ShowSceneObjTerrain(1 - this->IsSceneObjTerrainVisible());
		TipI(this->IsSceneObjTerrainVisible(), GetLanguageString(334).c_str(), GetLanguageString(335).c_str());
	}
	else if (g_pGameApp->IsKeyDown(DIK_F8)) {
		this->ShowSceneObj(1 - this->IsSceneObjVisible());
		TipI(this->IsSceneObjVisible(), GetLanguageString(336).c_str(), GetLanguageString(337).c_str());
	}
	else if (g_pGameApp->IsKeyDown(DIK_F9)) //activesection
	{
		/*
        //
		// MPMap::DynamicLoading()section
		const int	ciSectionWidth = 8;			// tile
		const int	ciSectionHeight = 8;		// tile
		const int	ciRefreshWidth = 3;			// sectionsection
		const int	ciRefreshHeight = 3;		// sectionsection
		const int	ciMaxObjWidth = 2;			// section
		const int	ciMaxObjHeight = 2;			// section
		int			iShowSizeX, iShowSizeY;		// tile
		int			iShowCenterX, iShowCenterY;	// tile
		int			iSaveSectionBuffer;
		int			iOriginalWidth, iOriginalHeight;
		float		fOriginalCenterX, fOriginalCenterY;

		fOriginalCenterX = _pTerrain->GetShowCenterX();
		fOriginalCenterY = _pTerrain->GetShowCenterY();
		iOriginalWidth = _pTerrain->GetShowWidth();
		iOriginalHeight = _pTerrain->GetShowHeight();

		iShowCenterX = ((int)_pTerrain->GetShowCenterX() / ciSectionWidth) * ciSectionWidth + ciSectionWidth / 2;
		iShowCenterY = ((int)_pTerrain->GetShowCenterY() / ciSectionHeight) * ciSectionHeight + ciSectionHeight / 2;
		_pTerrain->SetShowCenter((float)iShowCenterX, (float)iShowCenterY);
		iShowSizeX = (ciMaxObjWidth * 2 + ciRefreshWidth) * ciSectionWidth;
		iShowSizeY = (ciMaxObjHeight * 2 + ciRefreshHeight) * ciSectionHeight;
		_pTerrain->SetShowSize(iShowSizeX, iShowSizeY);
		iSaveSectionBuffer = (iShowSizeX / ciSectionWidth) * (iShowSizeY / ciSectionHeight);
		_pTerrain->SetSectionBufferSize(iSaveSectionBuffer);

		_pTerrain->ClearAllSection(TRUE);
		_pTerrain->DynamicLoading(GetTickCount());
		g_pGameApp->GetCurScene()->ResetAllSceneObjPos();

		int		i, j;
		int		iCurSectionY, iCurSectionX;
		for (i = 0; i < ciRefreshHeight; i ++)
		{
			iCurSectionY = (int)_pTerrain->GetShowCenterY() / ciSectionHeight - ciRefreshHeight / 2 + i;
			if (iCurSectionY < 0)
				continue;
			for (j = 0; j < ciRefreshWidth; j ++)
			{
				iCurSectionX = (int)_pTerrain->GetShowCenterX() / ciSectionWidth - ciRefreshWidth / 2 + j;
				if (iCurSectionX < 0)
					continue;
				g_ObjInfluence.RefreshSectionObjInfluenceInfo(iCurSectionY * _pTerrain->GetSectionCntX() + iCurSectionX);
			}
		}
		_pTerrain->SetSectionBufferSize(MAX_SECTION_BUFFER);
		_pTerrain->SetShowCenter(fOriginalCenterX, fOriginalCenterY);
		_pTerrain->SetShowSize(iOriginalWidth, iOriginalHeight);
		_pTerrain->ClearAllSection(TRUE);
		_pTerrain->DynamicLoading(GetTickCount());
        */
	}
	else if (g_pGameApp->IsKeyDown((BYTE)DIKEYBOARD_SYSRQ)) {
		if (g_pGameApp->IsEnableSpAvi()) {
			g_pGameApp->EnableSprintAvi(FALSE);
			return false;
		}
		if (g_pGameApp->IsCtrlPress())
			g_pGameApp->EnableSprintAvi(TRUE);
		else
			g_pGameApp->EnableSprintScreen(TRUE);

		if (g_pGameApp->IsShiftPress()) {
			g_pGameApp->SetStartMinimap(0, 0, GetTerrain()->GetWidth() / SHOWRSIZE,
										GetTerrain()->GetHeight() / SHOWRSIZE);
			//g_pGameApp->SetStartMinimap(1500,3000,2, 2);

			g_pGameApp->EnableSprintSmMap(TRUE);
		}
	}
	else if (g_pGameApp->IsKeyDown(DIK_INSERT)) {
		g_pGameApp->EnableSprintAvi(TRUE);
	}
	else if (g_pGameApp->IsKeyDown(DIK_HOME)) {
	}
	else if (g_pGameApp->IsKeyDown(DIK_9)) {
		int nWidth = _pTerrain->GetShowWidth();
		int nHeight = _pTerrain->GetShowHeight();
		if (nWidth > 8 && nHeight > 8) {
			_pTerrain->SetShowSize(nWidth - 1, nHeight - 1);
		}
	}
	else if (g_pGameApp->IsKeyDown(DIK_0)) {
		int nWidth = _pTerrain->GetShowWidth();
		int nHeight = _pTerrain->GetShowHeight();
		_pTerrain->SetShowSize(nWidth + 1, nHeight + 1);
	}
	else if (g_pGameApp->IsKeyContinue(DIK_F11)) {
		this->ShowChairObj(1 - this->IsShowChairObj());
		TipI(this->IsShowChairObj(), GetLanguageString(215).c_str(), GetLanguageString(216).c_str());
	}
	else if (g_pGameApp->IsKeyDown(DIK_X)) {
		_IsShowPath = !_IsShowPath;
		g_pGameApp->AddTipText("%s", GetLanguageString(338).c_str());
	}
	else if (g_pGameApp->IsKeyDown(DIK_M)) {
		if (!_pBigMap->IsLoad())
			_pBigMap->Create();
		else
			_pBigMap->Destory();
	}
#endif

	return false;
}

CCharacter* CGameScene::SelectCharacter() {
	if (_pSelCha) {
		// _pSelCha->ShowBoundingObjectPrimitive( 0 );
		_pSelCha->ShowHelperObject(0);
		// _pSelCha->SetCircleColor(0xff0000ff);
	}

	_pSelCha = HitTestCharacter(g_pGameApp->GetMouseX(), g_pGameApp->GetMouseY());
	if (_pSelCha) {
		// _pSelCha->ShowBoundingObjectPrimitive( 1 );
		if (g_Editor.IsEnable()) _pSelCha->ShowHelperObject(1);

		// _pSelCha->SetCircleColor(0xffff0000);
		// _pCurScene->SetMainCha(_pSelCha->getID());
	}
	return _pSelCha;
}

CShadeEff* CGameScene::GetShadeObj(int nArrayID) {
	if (nArrayID >= _nShadeCnt || nArrayID < 0) return NULL;
	return &_pShadeArray[nArrayID];
}

CShadeEff* CGameScene::GetFirstInvalidShadeObj() {
	if (_pShadeArray == NULL) return NULL;

	for (int i = 0; i < _nShadeCnt; i++) {
		if (_pShadeArray[i].IsValid() == FALSE)
			return &_pShadeArray[i];
	}
	return NULL;
}

void CGameScene::_CreateShadeArray(int nShadeCnt) {
	_pShadeArray = new CShadeEff[nShadeCnt];

	for (int i = 0; i < nShadeCnt; i++) {
		_pShadeArray[i].setID(i);
		_pShadeArray[i].SetScene(this);
	}

	_nShadeCnt = nShadeCnt;
}

void CGameScene::_ClearAllShade() {
	SAFE_DELETE_ARRAY(_pShadeArray);
	_nShadeCnt = 0;
}

BOOL CGameScene::IsPointVisible(float fX, float fY) {
	if (!_pTerrain) return TRUE;

	return _pTerrain->IsPointVisible(fX, fY);
}

bool CGameScene::GetIsBlockWalk(CCharacter* pCha, int nX, int nY) {
	if (_pTerrain) {
		return _pTerrain->IsGridBlock(nX / 50, nY / 50) || !g_IsMoveAble(
			pCha->getChaCtrlType(), pCha->GetDefaultChaInfo()->chTerritory,
			(EAreaMask)_pTerrain->GetTile(nX / 100, nY / 100)->sRegion);
	}
	return true;
}

void CGameScene::ClearSceneEffect() {
	for (int i = 0; i < _nEffCnt; i++) {
		if (_pEffectArray[i].IsValid()) {
			if (_pEffectArray[i].IsSceneEffect())
				_pEffectArray[i].SetValid(FALSE);
		}
	}
}

bool CGameScene::SwitchMap(int nMapID) {
	CWaitCursor wait;

	CMapInfo* pMapInfo = ::GetMapInfo(nMapID);
	if (!pMapInfo) {
		ToLogService("map", "msgCGameScene::SwitchMap - GetMapInfo({}) return NULL", nMapID);
		return false;
	}

	_dwMapID = nMapID;
	if (_pTerrain) {
		g_pGameApp->SendMessage(APP_SWITCH_MAP, nMapID);
	}

	_Clear();
	_ClearMemory();
	_stInit.strMapFile = pMapInfo->DataName.c_str();

	_CreateMemory();
	_Init();
	return true;
}

void CGameScene::Reload() {
	_Clear();
	_ClearMemory();
	_CreateMemory();
	_Init();
}

bool CGameScene::LoadMap(const char* file) {
	string strMapFile = file;
	if (strMapFile.empty()) return false;

	_pMapInfo = ::GetMapInfo(file);

	_pTerrain = std::make_unique<MPTerrain>();
	// g_ObjInfluence.SetTerrain( _pTerrain );

	extern long CALLBACK TerrainNotice(int nFlag, int nSectionX, int nSectionY, unsigned long dwParam,
									   MPTerrain* pThis);
	_pTerrain->SetMapProcFN(TerrainNotice);
	string map = strMapFile + ".map";
	map = "map/" + map;

	_pTerrain->Load(map.c_str(), GlobalAppConfig.IsEditor());

	map = strMapFile + ".obj";
	map = "map/" + map;
	g_ObjFile.Init(_TEXT((char*)map.c_str()), false);

	_pTerrain->SetShowSize(33, 33);

	// begin debug by lsh
	//
	const char* map_name[] =
	{
		"garner",
		"eastgoaf",
		"magicsea",
	};


	D3DCOLORVALUE map_env[] =
	{
		{0.6f, 0.6f, 0.6f, 1.0f},
		{0.6f, 0.6f, 0.6f, 1.0f},
		{0.6f, 0.6f, 0.6f, 1.0f},
	};

	for (DWORD i = 0; i < 3; i++) {
		if (strcmp(file, map_name[i]) == 0) {
			m_dwEnvColor = map_env[i];
			break;
		}
	}
	// end
	return true;
}

CCharacter* CGameScene::SearchByID(unsigned long id) {
	CCharacter* pCha;
	for (int i = 0; i < _nChaCnt; i++) {
		pCha = &_pChaArray[i];
		if (pCha->IsValid() && pCha->getAttachID() == id) {
			return pCha;
		}
	}

	return NULL;
}

CCharacter* CGameScene::SearchByHumanID(unsigned long id) {
	CCharacter* pCha;
	for (int i = 0; i < _nChaCnt; i++) {
		pCha = &_pChaArray[i];
		if (pCha->IsValid() && pCha->getHumanID() == id) {
			return pCha;
		}
	}

	return NULL;
}

CCharacter* CGameScene::SearchByName(const char* name) {
	CCharacter* pCha;
	for (int i = 0; i < _nChaCnt; i++) {
		pCha = &_pChaArray[i];
		if (pCha->IsValid() && strcmp(pCha->getName().c_str(), name) == 0) {
			return pCha;
		}
	}

	return NULL;
}

CCharacter* CGameScene::SearchByHumanName(const char* name) {
	CCharacter* pCha;
	for (int i = 0; i < _nChaCnt; i++) {
		pCha = &_pChaArray[i];
		if (pCha->IsValid() && strcmp(pCha->getHumanName(), name) == 0) {
			return pCha;
		}
	}

	return NULL;
}

CSceneItem* CGameScene::SearchItemByID(unsigned long id) {
	CSceneItem* pItem;
	for (int i = 0; i < _nSceneItemCnt; i++) {
		pItem = &_pSceneItemArray[i];
		if (pItem->IsValid() && pItem->getAttachID() == id) {
			return pItem;
		}
	}
	return NULL;
}

void CGameScene::SetMainCha(int nChaID) {
	CCharacter* pCha = GetCha(nChaID);
	if (!pCha) return;

	_pMainCha = pCha;
	pCha->EnableAI(FALSE);

	pCha->ResetReadySkill();

	_UserLeve.AllTrue();

	//   //
	//   CCameraCtrl *pCam = g_pGameApp->GetMainCam();
	//   MPTerrain *pTerr = GetTerrain();

	//   D3DXVECTOR3 vecCha = pCha->GetPos();
	//   vecCha.z = GetGridHeight(vecCha.x,vecCha.y);

	//pCam->InitModel(pCha->IsBoat() ? 3 : 0,&vecCha);

	////pCam->SetFollowObj(vecCha);
	//   //g_pGameApp->GetCameraTrack()->Reset(vecCha.x,vecCha.y,vecCha.z);

	////pCam->InitPos( vecCha.x,vecCha.y, vecCha.z );
	//   pCam->SetBufVel( pCha->getMoveSpeed(), nChaID);


	//   pCam->FrameMove(0);
	////g_pGameApp->ResetGameCamera( pCha->IsBoat() ? 3 : 0 );

	//pCam->SetViewTransform();
	//   //g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
	//   //g_Render.SetCurrentView(MPRender::VIEW_WORLD);
}

bool CGameScene::_Init() {
	g_pGameApp->GetCursor()->SceneInit(this);
	ToLogService("common", "3d cursor init");

	g_Render.SetDirectLightDir(GlobalAppConfig.GetLightDir()[0], GlobalAppConfig.GetLightDir()[1], GlobalAppConfig.GetLightDir()[2]);
	g_Render.SetDirectLightColor(GlobalAppConfig.GetLightColor()[0], GlobalAppConfig.GetLightColor()[1], GlobalAppConfig.GetLightColor()[2], 1.0f);


	_IsShowItemName = false;
	_pMouseInItem = NULL;

	_UserLeve.AllTrue();

	CUIInterface::All_SwitchMap();

	_dwTranspObjStateID = 0;
	_dwTranspObjStateNum = 1024;
	_TranspObjStateSeq = new SceneTranspObjStateDesc[_dwTranspObjStateNum];
	memset(_TranspObjStateSeq, 0, sizeof(SceneTranspObjStateDesc) * _dwTranspObjStateNum);

	CBoxMgr::CloseAllBox();
	ToLogService("common", "CGameScene::_Init");
	return true;
}

int CGameScene::UpdateSceneAnimLight() {
	list<int>::iterator it = _SceneObjIdx[SCENEOBJ_TYPE_POINTLIGHT].begin();
	list<int>::iterator it_end = _SceneObjIdx[SCENEOBJ_TYPE_POINTLIGHT].end();

	// point light
	int u = 0;
	SceneLight* s;

	for (; it != it_end; ++it) {
		u = (*it);
		s = (SceneLight*)&_pSceneObjArray[u];

		// update lighting animation
		if (s->type != SceneLight::SL_LIGHT) {
			__debugbreak();
		}

		if (s->UpdateAnimLight()) {
			if (_pTerrain) {
				_pTerrain->UpdateRender();
			}
		}
	}

	return 1;
}

int CGameScene::BeginUpdateSceneObjLight(const CSceneObj* obj, BOOL is_enable) {
	if (is_enable == 0)
		return 1;

	D3DXVECTOR3* obj_pos = &const_cast<CSceneObj*>(obj)->getPos();

	int dis[2] = {0, 0};
	int id[2] = {-1, -1};

	int a;
	int u = 0;
	// dev
	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;

	const int max_effected_dis = 100 * 100;

	SceneLight* s;


	list<int>::iterator it = _SceneObjIdx[SCENEOBJ_TYPE_POINTLIGHT].begin();
	list<int>::iterator it_end = _SceneObjIdx[SCENEOBJ_TYPE_POINTLIGHT].end();

	for (; it != it_end; ++it) {
		u = (*it);
		s = (SceneLight*)&_pSceneObjArray[u];

		s->pos = _pSceneObjArray[(*it)].getPos();

		if (s->type != SceneLight::SL_LIGHT) {
			MessageBox(0, "SceneLight::SL_LIGHT error", "x", MB_OK);
		}

		a = s->GetSquareMagnitude((int)obj_pos->x, (int)obj_pos->y, (int)obj_pos->z);

		if (id[0] == -1 || dis[0] > a) {
			if (id[1] == -1 || dis[1] > a) {
				id[0] = id[1];
				dis[0] = dis[1];

				id[1] = u;
				dis[1] = a;
			}
			else {
				id[0] = u;
				dis[0] = a;
			}
		}
	}

	if (id[1] != -1) {
		if (this->_pTerrain) {
			_pTerrain->UpdateRender();
		}
		// first light
		s = (SceneLight*)&_pSceneObjArray[id[1]];
		D3DLIGHTX light;
		s->GetLightData(&light);
		dev_obj->SetLight(1, &light);
		dev_obj->LightEnable(1, 1);

		if (id[0] != -1) {
			// second light
			s = (SceneLight*)&_pSceneObjArray[id[0]];
			s->GetLightData(&light);
			dev_obj->SetLight(2, &light);
			dev_obj->LightEnable(2, 1);
		}
	}

	return 1;
}

int CGameScene::EndUpdateSceneObjLight(const CSceneObj* obj) {
	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;

	dev_obj->LightEnable(1, 0);
	dev_obj->LightEnable(2, 0);

	return 1;
}

void CGameScene::PlayEnvSound(const char* szFile, int nX, int nY) {
	if (!CGameApp::IsMusicSystemValid() || !_IsUseSound) return;

	static float& x = g_pGameApp->GetMainCam()->m_RefPos.x;
	static float& y = g_pGameApp->GetMainCam()->m_RefPos.y;
	static int dis = 0;
	dis = GetDistance(nX, nY, (int)(x * 100.0f), (int)(y * 100.0f));
	if (dis <= VOICE_DIS) {
#ifdef USE_DSOUND
		//	g_pGameApp->m_pAudioPlayer->CreateInstance( szFile );
		/*dis = (int)(_fSoundSize * ( 1.0f - (float)dis / (float)VOICE_DIS ));
		ulong musid = AudioSDL::get_instance()->get_resID(szFile, TYPE_WAV);
		if( musid )
		{
			AudioSDL::get_instance()->volume(musid, dis);
			AudioSDL::get_instance()->play(musid, false);
		}*/
		g_pGameApp->PlaySample(szFile);

#else
		dis = (int)(_fSoundSize * (1.0f - (float)dis / (float)VOICE_DIS));
		//::cmn_snd_play(szFile, dis);
		std::uint32_t musid = AudioSDL::Instance().GetResourceId(szFile, AudioType::Sfx);
		AudioSDL::Instance().SetVolume(musid, float(dis) / 128.0f);
		//AudioSDL::Instance().Play(musid);

		//
		g_AudioThread.play(musid, false);
#endif
	}
}

void CGameScene::PlayEnvSound(int nX, int nY) {
	if (!CGameApp::IsMusicSystemValid()) return;

	static list<int>::iterator it;
	static CSceneObjInfo* pInfo;
	static int r;
	static int dis;
	for (it = _SceneObjIdx[SCENEOBJ_TYPE_ENVSOUND].begin(); it != _SceneObjIdx[SCENEOBJ_TYPE_ENVSOUND].end(); it++) {
		CSceneObj* pObj = &_pSceneObjArray[*it];
		if (pObj->IsValid() && !pObj->IsHide()) {
			pInfo = GetSceneObjInfo(pObj->getTypeID());
			if (!pInfo || pInfo->_envSoundDis <= 0) continue;

			dis = GetDistance(pObj->GetCurX(), pObj->GetCurY(), nX, nY);
			if (dis <= pInfo->_envSoundDis) {
				dis = (int)(_fSoundSize * (1.0f - (float)dis / (float)pInfo->_envSoundDis));
				if (pObj->GetMusicID() == -1) {
					static char szSoundName[256] = {0};
					sprintf(szSoundName, "music/sound/%s.wav", pInfo->_envSound.c_str());
					//r = ::env_snd_add( szSoundName );
					//if( r != -1 )
					//{
					//	env_snd_vol( r, dis );
					//	env_snd_play( r );
					//	pObj->SetMusicID( r );
					//}

#ifdef USE_AUDIODLL
					//g_pGameApp->m_pAudioPlayer->CreateInstance( szSoundName );
					std::uint32_t m = AudioSDL::Instance().GetResourceId(szSoundName, AudioType::Sfx);
#else
					std::uint32_t m = AudioSDL::Instance().GetResourceId(szSoundName, AudioType::Sfx);
#endif
					continue;
				}
				//env_snd_vol( pObj->GetMusicID(), dis );
				//GetAudioDevice()->volume(m, dis);
			}
			else {
				if (pObj->GetMusicID() != -1) {
					//env_snd_del( pObj->GetMusicID() );
					pObj->SetMusicID(-1);
				}
			}
		}
	}
}

void CGameScene::SetSoundSize(float fVol) {
	if (fVol >= 0.0 && fVol <= 1.0) {
		_fSoundSize = fVol * 128.0f;
	}
}

int CGameScene::SetTextureLOD(DWORD level) {
	int i;

	for (i = 0; i < _nChaCnt; i++) {
		CCharacter* pCha = &_pChaArray[i];
		if (pCha->IsValid() && !pCha->IsHide()) {
			pCha->SetTextureLOD(level);
		}
	}

	list<int>::iterator it;
	for (it = _SceneObjIdx[0].begin(); it != _SceneObjIdx[0].end(); it++) {
		int nArrayID = (*it);
		CSceneObj* pObj = &_pSceneObjArray[nArrayID];
		if (pObj->IsValid() && pObj->IsHide() == FALSE) {
			pObj->SetTextureLOD(level);
		}
	}

	for (i = 0; i < _nSceneItemCnt; i++) {
		CSceneItem* pObj = &_pSceneItemArray[i];
		if (pObj->IsValid() && pObj->IsHide() == FALSE) {
			pObj->SetTextureLOD(level);
		}
	}

	TerrainRecordStore::Instance()->ForEach([&](const MPTerrainInfo& info) {
		MPITex* tex = GetTexByID(info.nTextureID);
		if (tex) {
			tex->SetLOD(level);
		}
	});

	return 1;
}

void CGameScene::RefreshLevel() {
	if (!g_stUIBoat.GetHuman()) return;

	int nMainLevel = g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_LV);
	CCharacter* pCha = NULL;
	for (int i = 0; i < _nChaCnt; i++) {
		pCha = &_pChaArray[i];
		if (pCha->IsValid()) {
			pCha->RefreshLevel(nMainLevel);
		}
	}
}

CEffectObj* CGameScene::CreateEffect(int nEffectID, int nX, int nY, bool isLoop) {
	CEffectObj* pEffect = GetFirstInvalidEffObj();
	if (!pEffect) return NULL;

	if (!pEffect->Create(nEffectID))
		return NULL;

	D3DXVECTOR3 pos;
	pos.x = (float)nX / 100.0f;
	pos.y = (float)nY / 100.0f;
	pos.z = GetGridHeight(pos.x, pos.y);
	pEffect->setLoop(isLoop);
	pEffect->SetUpdateHieght(true);
	pEffect->Emission(-1, &pos, NULL);
	pEffect->SetValid(TRUE);
	return pEffect;
}

void CGameScene::LoadingCall() {
	ToLogService("common", "LoadingCall:{}\t{}", _stInit.strMapFile.c_str(), GetTickCount());

	CCharacter* pObj = NULL;
	for (int i = 0; i < _nChaCnt; i++) {
		pObj = &_pChaArray[i];
		if (pObj->IsValid() && !pObj->IsHide()) {
			pObj->LoadingCall();
		}
	}
	pObj = GetMainCha();
	if (!pObj) return;

	RefreshLevel();

	//////////////////////////////////////////////////////////////////////////
	SAFE_DELETE(_pSmallMap);
	_pSmallMap = new CMinimap;

	RECT rc;

	CCompent* pRect = g_stUIMap.GetMinimapRect();

	int lenx = pRect->GetRight() - pRect->GetLeft();


	rc.left = ResMgr.GetBackBufferWidth() - (lenx + pRect->GetLeft());
	rc.top = pRect->GetTop();
	rc.right = rc.left + lenx;
	rc.bottom = rc.top + lenx;

	_pSmallMap->Create(g_Render.GetDevice(), rc, g_pGameApp->GetCurScene(), lenx);

	//CCompent*pRect = g_stUIMap.GetL

	C3DCompent* pD3d = g_stUIMap.GetBigmapRect();
	rc.left = pD3d->GetX();
	rc.top = pD3d->GetY();
	rc.right = pD3d->GetX2();
	rc.bottom = pD3d->GetY2();

	SAFE_DELETE(_pLargerMap);
	_pLargerMap = new CLargerMap;
	_pLargerMap->Create(g_Render.GetDevice(), rc, g_pGameApp->GetCurScene(), 500);

	ToLogService("common", "create small map");
	//////////////////////////////////////////////////////////////////////////

	//
	CCameraCtrl* pCam = g_pGameApp->GetMainCam();

	D3DXVECTOR3 vecCha = pObj->GetPos();
	vecCha.z = GetGridHeight(vecCha.x * 100, vecCha.y * 100);

	pCam->InitModel(pObj->IsBoat() ? 3 : 0, &vecCha);

	//pCam->SetFollowObj(vecCha);
	//g_pGameApp->GetCameraTrack()->Reset(vecCha.x,vecCha.y,vecCha.z);

	pCam->InitPos(vecCha.x, vecCha.y, vecCha.z);
	pCam->SetBufVel(pObj->getMoveSpeed(), pObj->getID());


	pCam->FrameMove(0);
	//g_pGameApp->ResetGameCamera( pCha->IsBoat() ? 3 : 0 );

	pCam->SetViewTransform();

	//
	//CCameraCtrl *pCam = g_pGameApp->GetMainCam();
	if (pCam->m_bEnableRota != (BOOL)g_stUISystem.m_sysProp.m_videoProp.bCameraRotate) {
		pCam->EnableRotation(g_stUISystem.m_sysProp.m_videoProp.bCameraRotate);
	}
}

BYTE CGameScene::GetTileTexAttr(int ix, int iy) {
	if (!_pTerrain)
		return 0;
	MPTile* pTile = _pTerrain->GetTile(ix, iy);
	if (!pTile)
		return 0;
	if (_pTerrain->GetGridHeight(ix * 2, iy * 2) > _pTerrain->GetTileHeight(ix, iy))
		return 0;
	MPTerrainInfo* pInfo = GetTerrainInfo(pTile->TexLayer[0].btTexNo);
	if (!pInfo)
		return 0;
	return pInfo->btAttr;
}

void CGameScene::AddAreaEff(CEffectObj* pEffectObj) {
	_areaeffs.push_back(pEffectObj);
}

bool CGameScene::DelAreaEff(long nAreaID, int nEffectID) {
	int nCount = 0;
	if (nEffectID == 0) {
		for (areaeffs::iterator it = _areaeffs.begin(); it != _areaeffs.end(); it++) {
			if ((*it)->getTag() == nAreaID) {
				(*it)->SetValid(FALSE);
				(*it) = NULL;
				nCount++;
			}
		}
	}
	else {
		for (areaeffs::iterator it = _areaeffs.begin(); it != _areaeffs.end(); it++) {
			if ((*it)->getTag() == nAreaID && (*it)->getIdxID() == nEffectID) {
				(*it)->SetValid(FALSE);
				(*it) = NULL;
				nCount++;
			}
		}
	}
	if (nCount > 0) {
		_areaeffs.remove(NULL);
		return true;
	}
	return false;
}

CSceneItem* CGameScene::FilterItemsByItemID(unsigned long id, bool shouldHide) {
	CSceneItem* pItem;

	for (int i = 0; i < _nSceneItemCnt; i++) {
		pItem = &_pSceneItemArray[i];

		if (pItem->IsValid()) {
			CItemRecord* itemData = pItem->GetItemInfo();
			if (itemData) {
				if (itemData->lID == id) {
					pItem->SetHide(shouldHide);
				}
			}
		}
	}

	if (!shouldHide) {
		g_lootFilter->RemoveFilteredItem(id);
	}
	else {
		g_lootFilter->AddFilteredItem(id);
	}

	return NULL;
}

void CGameScene::PlayEnvSound(int nSoundNo, int nX, int nY) {
	if (nSoundNo == -1) {
		return;
	}

	CMusicInfo* pInfo = GetMusicInfo(nSoundNo);
	if (!pInfo || pInfo->nType != 1) {
		return;
	}

	PlayEnvSound(pInfo->DataName.c_str(), nX, nY);
}

float CGameScene::GetGridHeight(float fX, float fY) {
	if (!_pTerrain) {
		return 0.0f;
	}
	return _pTerrain->GetGridHeight(static_cast<int>(fX * 2.0f), static_cast<int>(fY * 2.0f));
}

float CGameScene::GetTerrainHeight(float fX, float fY) {
	if (!_pTerrain) {
		return 0.0f;
	}
	float h = _pTerrain->GetHeight(fX, fY);
	return h < SEA_LEVEL ? SEA_LEVEL : h;
}

int CGameScene::GetGridRegion(int x, int y) {
	if (!_pTerrain) {
		return 0;
	}
	return _pTerrain->GetTile(x, y)->sRegion;
}
