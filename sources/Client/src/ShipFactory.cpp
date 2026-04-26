#include "stdafx.h"
#include "ShipFactory.h"
#include "GameApp.h"
#include "EffectObj.h"
#include "GameConfig.h"
#include "MPEditor.h"
#include "Character.h"
#include "UITemplete.h"
#include "PacketCmd.h"
#include "UIBoatForm.h"
#include "UIBourseForm.h"
#include "UITradeForm.h"

using namespace std;

//---------------------------------------------------------------------------
#define ERR_CHK(p, s) { if(p == 0) { ToLogService("common", LogLevel::Error, "ShipFactory Load error: {}", s); } }

//---------------------------------------------------------------------------
static xShipFactory* __xsf = 0;

//---------------------------------------------------------------------------
void xShipFactory::__ButtonYesNo(CGuiData* pSender, int x, int y, DWORD dwKey) {
	if (__xsf) __xsf->Close(pSender);
}

//---------------------------------------------------------------------------
void xShipFactory::__ProcBtnItem(CGuiData* sender, int x, int y, DWORD key) {
	__xsf->UpdateShipPart(sender);
}

//---------------------------------------------------------------------------
void xShipFactory::__ProcBtnView3d(CGuiData* sender, int x, int y, DWORD key) {
	string name = sender->GetName();

	if (name == "btnLeft") {
		__xsf->yaw_degree += 2;
	}
	else if (name == "btnRight") {
		__xsf->yaw_degree -= 2;
	}
	else if (name == "btnTop") {
		__xsf->pitch_degree += 2;
	}
	else if (name == "btnDown") {
		__xsf->pitch_degree -= 2;
	}
	__xsf->UpdateBoatAttr();
}

//---------------------------------------------------------------------------
void xShipFactory::__Proc3DView(C3DCompent* pSender, int x, int y) {
	__xsf->Render3DView(x, y);
}

//---------------------------------------------------------------------------
void xShipFactory::__ProcShipName(CGuiData* pSender) {
	__xsf->CheckShipName();
}

//---------------------------------------------------------------------------
// Open/close ship cabin UI event (Michael Chen 2005-05-26)
void xShipFactory::__ButtonOpenRoom(CGuiData* pSender, int x, int y, DWORD dwKey) {
	if (!__xsf)
		return;

	if (!__xsf->sbf.wnd_boat_room)
		return;

	__xsf->sbf.wnd_boat_room->SetIsShow(true);
	__xsf->sbf.btn_room_open->SetIsShow(false);
	__xsf->sbf.btn_room_close->SetIsShow(true);
}

// When form is closed, notify server to cancel ship creation (Michael Chen 2005-06-06)
void xShipFactory::__HideForm(CGuiData* pSender) {
	if (STATE_CREATE == __xsf->m_state)
		CS_CancelBoat();
}

//---------------------------------------------------------------------------
void xShipFactory::__ButtonCloseRoom(CGuiData* pSender, int x, int y, DWORD dwKey) {
	if (!__xsf)
		return;

	if (!__xsf->sbf.wnd_boat_room)
		return;

	__xsf->sbf.wnd_boat_room->SetIsShow(false);
	__xsf->sbf.btn_room_open->SetIsShow(true);
	__xsf->sbf.btn_room_close->SetIsShow(false);
}

//---------------------------------------------------------------------------
void xShipFactory::__CheckShip(CGuiData* pSender) {
	__xsf->UpdateBoatAttr();
}

//end Modify By Michael 2005-05-26

//---------------------------------------------------------------------------
const DWORD xShipFactory::__PartRange[4][2] =
{
	{1, 7}, // head
	{8, 14}, // body
	{15, 21}, // power
	{57, 72}, // weapon
};

//---------------------------------------------------------------------------
xShipFactory::xShipFactory() : m_count(0), m_dwBoatID(-1) {
	__xsf = this;

	scene = 0;
	form_mgr = 0;
	ship = 0;
	memset(&sbf, 0, sizeof(sbf));

	yaw_degree = 0;
	pitch_degree = 0;

	type = 1;

	memset(item_part, 0, sizeof(item_part));
}

//---------------------------------------------------------------------------
xShipFactory::~xShipFactory() {
	SAFE_DELETE(ship);
}


//---------------------------------------------------------------------------
BOOL xShipFactory::Init(CGameScene* s) {
	scene = s;
	form_mgr = &CFormMgr::s_Mgr;

	// init ui ctrl
	sbf.wnd = form_mgr->Find("frmShipBiuld", enumMainForm);
	ERR_CHK(sbf.wnd, "frmShipBiuld");
	sbf.wnd->evtHide = __HideForm;


	if (sbf.wnd == 0)
		return 0;

	sbf.view3d = (C3DCompent*)sbf.wnd->Find("ui3dship");
	ERR_CHK(sbf.view3d, "ui3dship");
	sbf.view3d->SetRenderEvent(__Proc3DView);

	const char* btn_str[] =
	{
		"btnHeadLeft", // head
		"btnHeadRight",
		"btnPowerLeft", // power
		"btnPowerRight",
		"btnSignLeft", // sign
		"btnSignRight",
		"btnCanonLeft", // weapon
		"btnCanonRight",
	};

	for (int i = 0; i < SBF_BTN_ITEM_NUM; i++) {
		sbf.btn_item[i] = (CTextButton*)sbf.wnd->Find(btn_str[i]);
		ERR_CHK(sbf.btn_item[i], btn_str[i]);
		sbf.btn_item[i]->evtMouseClick = __ProcBtnItem;
	}

	const char* txt_str[] =
	{
		"labAtackShow", // attack
		"labDefenceShow", // defence
		"labSailorShow", // endure
		"labCanonShow", // supply
		"labTimeShow", // speed
		"labSpeedShow", // cooddown
		"labFlyspeedShow", // distance
		"labContentShow", // capacity
		"labMenShow", // supply_consume_rate
		"labShipBody", // body
		"labShipHead", // head
		"labShipPower", // power
		"labShipSign", // sign
		"labShipCanon", // weapon
	};

	for (int i = 0; i < SBF_TEXT_PROP_NUM; i++) {
		sbf.lbl_prop[i] = (CLabelEx*)sbf.wnd->Find(txt_str[i]);
		ERR_CHK(sbf.lbl_prop[i], txt_str[i]);
	}

	const char* view3d_str[] =
	{
		"btnLeft",
		"btnRight",
		"btnTop",
		"btnDown",
	};
	for (int i = 0; i < SBF_BTN_VIEW3D_NUM; i++) {
		sbf.btn_view3d[i] = (CTextButton*)sbf.wnd->Find(view3d_str[i]);
		ERR_CHK(sbf.btn_view3d[i], view3d_str[i]);
		sbf.btn_view3d[i]->evtMouseClick = __ProcBtnView3d;
	}

	sbf.name = (CEdit*)sbf.wnd->Find("edtShipName");
	ERR_CHK(sbf.name, "edtShipName");
	sbf.name->evtEnter = __ProcShipName;

	sbf.btn_create = (CTextButton*)sbf.wnd->Find("btnYes");
	ERR_CHK(sbf.btn_create, "btnYes");
	sbf.btn_create->evtMouseClick = __ButtonYesNo;

	sbf.btn_cancel = (CTextButton*)sbf.wnd->Find("btnNo");
	ERR_CHK(sbf.btn_cancel, "btnNo");
	sbf.btn_cancel->evtMouseClick = __ButtonYesNo;

	/* Open/close ship cabin panel -- Michael Chen 2005-05-25 */
	sbf.btn_room_open = (CTextButton*)sbf.wnd->Find("btnROpen");
	ERR_CHK(sbf.btn_room_open, "btnROpen");
	sbf.btn_room_open->evtMouseClick = __ButtonOpenRoom;
	sbf.btn_room_open->SetIsShow(true);

	sbf.btn_room_close = (CTextButton*)sbf.wnd->Find("btnRClose");
	ERR_CHK(sbf.btn_room_close, "btnRClose");
	sbf.btn_room_close->evtMouseClick = __ButtonCloseRoom;
	sbf.btn_room_close->SetIsShow(false);

	sbf.chkShip = (CCheckBox*)sbf.wnd->Find("chkShip");
	ERR_CHK(sbf.chkShip, "chkShip");
	sbf.chkShip->evtCheckChange = __CheckShip;

	sbf.imgMoneyTitle = (CImage*)sbf.wnd->Find("imgMomeyTitle");
	ERR_CHK(sbf.imgMoneyTitle, "imgMomeyTitle");
	sbf.imgSpaceTitle = (CImage*)sbf.wnd->Find("imgSpace");
	ERR_CHK(sbf.imgSpaceTitle, "imgSpace");

	sbf.imgShipOrigin = (CImage*)sbf.wnd->Find("imgShipo");
	ERR_CHK(sbf.imgMoneyTitle, "imgMomeyTitle");

	sbf.lbl_ship_level = (CLabelEx*)sbf.wnd->Find("labShipLv");
	ERR_CHK(sbf.lbl_ship_level, "labShipLv");

	sbf.lbl_ship_exp = (CLabelEx*)sbf.wnd->Find("labShipExp");
	ERR_CHK(sbf.lbl_ship_exp, "labShipExp");


	return 1;
}

//---------------------------------------------------------------------------
void xShipFactory::ShowFlipBtn(int show, const char* name) {
	for (DWORD i = 0; i < SBF_BTN_ITEM_NUM; i++) {
		if (sbf.btn_item[i]) {
			sbf.btn_item[i]->SetIsShow(show ? true : false);
		}
	}

	if (sbf.btn_create) {
		sbf.btn_create->SetIsShow(show ? true : false);
	}

	if (sbf.name) {
		if (name) {
			sbf.name->SetCaption(name);
			sbf.name->SetIsEnabled(0);
		}
		else {
			//sbf.name->SetCaption("");
			sbf.name->SetIsEnabled(1);
			//sbf.name->set
		}
	}
	else {
		sbf.name->SetIsEnabled(1);
	}
}

//---------------------------------------------------------------------------
void xShipFactory::Show(int flag) {
	if (flag) {
		yaw_degree = 0;
		pitch_degree = 0;

		sbf.wnd->Show();

		{
			// save loading res mt flag
			lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
			mt_flag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
			res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
		}
	}
	else {
		sbf.wnd->Hide();

		{
			// reset loading res mt flag
			lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
			res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, mt_flag);
		}
	}
}

//---------------------------------------------------------------------------
BOOL xShipFactory::IsVisible() {
	return sbf.wnd ? sbf.wnd->GetIsShow() : 0;
}

//---------------------------------------------------------------------------
void xShipFactory::Render3DView(int x, int y) {
	if (ship == 0)
		return;

	D3DXVECTOR3 org, ray;

	DWORD sx = x + 180;
	DWORD sy = y + 90;


	g_Render.LookAt(D3DXVECTOR3(0.0f, 10.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MPRender::VIEW_3DUI);

	lwMatrix44 old_mat = *ship->GetMatrix();

	ship->SetUIYaw(120 + yaw_degree);
	ship->SetUIPitch(pitch_degree);
	ship->SetUIScaleDis(20);
	ship->RenderForUI(x, y);
	ship->SetMatrix(&old_mat);

	g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());
}

//---------------------------------------------------------------------------
BOOL xShipFactory::CreateShip(DWORD type_id, DWORD* part_buf) {
	SAFE_DELETE(ship);

	ship = new(CCharacter);
	if (ship->LoadShip(type_id, part_buf) == 0)
		return 0;

	ship->PlayPose(1, PLAY_LOOP);

	return 1;
}

//---------------------------------------------------------------------------
BOOL xShipFactory::ChangePart(DWORD part_id, DWORD file_id) {
	return ship->LoadPart(part_id, file_id);
}

//---------------------------------------------------------------------------
BOOL xShipFactory::UpdateShipPart(CGuiData* sender) {
	int i = 0;
	for (; i < SBF_BTN_ITEM_NUM; i++) {
		if (sender == sbf.btn_item[i])
			break;
	}

	if (i == SBF_BTN_ITEM_NUM)
		return 0;

	char b = (i % 2) ? -1 : +1;
	item_id[i / 2] += b;

	CS_UpdateBoat(item_id[ITEM_HEAD], item_id[ITEM_POWER], item_id[ITEM_WEAPON], item_id[ITEM_GOODS]);
	/*
	if(i == 0)
	{
		yaw_degree += 2;
	}
	else if( i == 1)
	{
		yaw_degree -= 2;
	}
	else if(i == 2)
	{
		pitch_degree += 2;
	}
	else if( i == 3)
	{
		pitch_degree -= 2;
	}
	*/
	/*
	int j = i / 2;
	int k = i % 2;

	if(k)
	{
		if(++item_id[j] > __PartRange[j][k])
		{
			item_id[j] = __PartRange[j][0];
		}
	}
	else
	{
		if(--item_id[j] < __PartRange[j][k])
		{
			item_id[j] = __PartRange[j][1];
		}
	}

	// update property
	DWORD prop_att[2] = { 0, 0 };
	DWORD prop_res[2] = { 0, 0 };
	DWORD prop_speed = 0;
	DWORD prop_dis = 0;
	DWORD prop_def = 0;
	DWORD prop_supply[2] = { 0, 0 };
	DWORD prop_cd = 0;
	DWORD prop_cap[2] = { 0, 0 };

	for(int i = 0; i < 4; i++)
	{
		info = GetXShipInfo(item_id[i]);
		prop_att[0] = info->prop_att[0];
		prop_att[1] = info->prop_att[1];
		prop_res[0] = info->prop_res;
		prop_res[1] = prop_res[0];
		prop_speed = info->prop_speed;
		prop_dis = info->prop_dis;
		prop_def = info->prop_def;
		prop_supply[0] = info->prop_sp;
		prop_supply[1] = prop_supply[0];
		prop_cd = info->prop_cd;
		prop_cap[0] = info->prop_cap;
		prop_cap[0] = prop_cap[1];
	}

	sbf.lbl_prop[SBF_TEXT_ATTACK]->SetCaption(std::format("{} / {}", prop_att[0], prop_att[1]).c_str());
	sbf.lbl_prop[SBF_TEXT_RESISTANCE]->SetCaption(std::format("{} / {}", prop_res[0], prop_res[1]).c_str());
	sbf.lbl_prop[SBF_TEXT_SUPPLY]->SetCaption(std::format("{} / {}", prop_supply[0], prop_supply[1]).c_str());
	sbf.lbl_prop[SBF_TEXT_CAPACITY]->SetCaption(std::format("{} / {}", prop_cap[0], prop_cap[1]).c_str());

	sbf.lbl_prop[SBF_TEXT_DEFENCE]->SetCaption(std::format("{}", prop_def).c_str());
	sbf.lbl_prop[SBF_TEXT_SPEED]->SetCaption(std::format("{}", prop_speed).c_str());
	sbf.lbl_prop[SBF_TEXT_COOLDOWN]->SetCaption(std::format("{}", prop_cd).c_str());
	sbf.lbl_prop[SBF_TEXT_DISTANCE]->SetCaption(std::format("{}", prop_dis).c_str());

	//
	if(j <= 2)
	{
		info = GetXShipInfo(item_id[j]);
		if(info == 0)
			return 0;

		if(!ChangePart(i, info->model_id))
			return 0;
	}
	*/

	return 1;
}

//---------------------------------------------------------------------------
BOOL xShipFactory::CheckShipName() {
	if (sbf.name == 0) {
		return 0;
	}

	strcpy(ship_name, sbf.name->GetCaption());
	size_t l = _tcslen(ship_name);
	if (l == 0) {
		g_pGameApp->MsgBox(GetLanguageString(390));
		return 0;
	}
	else if (l < MIN_LENGTH || l > MAX_LENGTH) {
		g_pGameApp->MsgBox(SafeVFormat(GetLanguageString(391), static_cast<int>(MIN_LENGTH),
											 static_cast<int>(MAX_LENGTH)));
		return 0;
	}

	// Check if ship name contains illegal characters
	string sBoatName(sbf.name->GetCaption());
	if (!CTextFilter::IsLegalText(CTextFilter::NAME_TABLE, sBoatName) ||
		!IsValidName(sBoatName)) {
		g_pGameApp->MsgBox(GetLanguageString(51));
		return 0;
	}


	return 1;
}

//---------------------------------------------------------------------------
BOOL xShipFactory::GetCabinByID() {
	// Use our boat ID
	if (m_dwBoatID == -1) return FALSE;

	/* Find boat by boat ID. Michael Chen (2005-05-26). */
	CBoat* pkBoat = g_stUIBoat.FindBoat(m_dwBoatID);
	if (!pkBoat) {
		// If not found, it is another player's boat; currently viewing other player's boat
		pkBoat = g_stUIBoat.GetOtherBoat();
		m_state = STATE_INFO_OTHER;
	}

	// Check if trade form is open; if so, notify user to close it before opening cabin
	CForm* pBoatRoom = pkBoat->GetForm();
	if (!pBoatRoom) return FALSE;
	CForm* pForm = dynamic_cast<CForm*>(pBoatRoom->GetParent());
	// The boat cabin form is shared, so check whether it is in use by the user
	CForm* pTradeForm = g_stUIBourse.GetForm();
	if (pTradeForm && pTradeForm->GetIsShow() && pForm == pTradeForm) {
		g_pGameApp->SysInfo(GetLanguageString(392));
		return FALSE;
	}
	CForm* pChangeForm = g_stUITrade.GetForm();
	if (pChangeForm && pChangeForm->GetIsShow() && pForm == pChangeForm) {
		g_pGameApp->SysInfo(GetLanguageString(392));
		return FALSE;
	}

	// Check if current cabin form is already open
	if (sbf.wnd_boat_room) {
		sbf.wnd_boat_room->SetIsShow(false);
	}

	/*If boat is currently sailing.*/
	CCharacter* boat = g_stUIBoat.FindBoat(__xsf->m_dwBoatID)->GetCha();
	if (boat->IsInMiniMap()) {
		sbf.wnd_boat_room = pBoatRoom;
		sbf.wnd_boat_room->SetParent(sbf.wnd);
		sbf.wnd_boat_room->SetPos(0, 242);
		sbf.wnd_boat_room->Refresh();
		sbf.wnd_boat_room->SetIsShow(false);
		sbf.btn_room_open->SetIsShow(true);
	}
	else {
		sbf.wnd_boat_room = NULL;
		sbf.btn_room_open->SetIsShow(false);
	}


	//sbf.wnd_boat_room = pBoatRoom;
	//sbf.wnd_boat_room->SetParent(sbf.wnd);
	//sbf.wnd_boat_room->SetPos(0, 242);
	//sbf.wnd_boat_room->Refresh();
	//sbf.wnd_boat_room->SetIsShow(false);
	return TRUE;
	/*end of Modify by Michael Chen. */
}

//---------------------------------------------------------------------------
BOOL xShipFactory::SetShipModelInfo(DWORD* dwModelInfo, size_t size) {
	if (memcmp(item_part, dwModelInfo, size)) {
		if (CreateShip(500, dwModelInfo) == 0)
			return 0;
		memcpy(item_part, dwModelInfo, size);
	}

	return 1;
}

//---------------------------------------------------------------------------
BOOL xShipFactory::Update(xShipBuildInfo* info, BOOL flag, const char* name) {
	if (m_state == STATE_CREATE) {
		sbf.chkShip->SetIsChecked(true);
		sbf.chkShip->SetIsShow(false);

		sbf.imgShipOrigin->SetIsShow(false);

		sbf.btn_room_close->SetIsShow(false);
		sbf.btn_room_open->SetIsShow(false);

		sbf.btn_cancel->SetIsShow(true);
	}
	else if (m_state == STATE_INFO_OTHER) {
		sbf.chkShip->SetIsChecked(true);
		sbf.chkShip->SetIsShow(true);
		sbf.chkShip->SetIsEnabled(false);
		sbf.imgShipOrigin->SetIsShow(true);

		sbf.btn_room_close->SetIsShow(false);
		sbf.btn_room_open->SetIsShow(false);

		sbf.btn_cancel->SetIsShow(false);
	}
	else {
		sbf.chkShip->SetIsChecked(false);
		sbf.chkShip->SetIsShow(true);

		sbf.imgShipOrigin->SetIsShow(true);
		sbf.btn_room_close->SetIsShow(false);
		//sbf.btn_room_open->SetIsShow(true);

		sbf.btn_cancel->SetIsShow(false);
	} //end of if

	// Set ship model info
	if (!SetShipModelInfo(info->dwBuf, sizeof(info->dwBuf)))
		return 0;

	item_id[ITEM_HEAD] = info->byHeader;
	item_id[ITEM_POWER] = info->byEngine;
	item_id[ITEM_WEAPON] = info->byCannon;
	item_id[ITEM_GOODS] = info->byEquipment;

	sbf.lbl_prop[SBF_TEXT_ATTACK]->SetCaption(std::format("{}/{}", info->dwMinAttack, info->dwMaxAttack).c_str());
	sbf.lbl_prop[SBF_TEXT_ENDURE]->SetCaption(std::format("{}/{}", info->dwCurEndure, info->dwMaxEndure).c_str());
	sbf.lbl_prop[SBF_TEXT_SUPPLY]->SetCaption(std::format("{}/{}", info->dwCurSupply, info->dwMaxSupply).c_str());
	sbf.lbl_prop[SBF_TEXT_CAPACITY]->SetCaption(std::format("{}/{}", 0, info->sCapacity).c_str());

	sbf.lbl_prop[SBF_TEXT_DEFENCE]->SetCaption(std::format("{}", info->dwDefence).c_str());
	sbf.lbl_prop[SBF_TEXT_SPEED]->SetCaption(std::format("{}", info->dwSpeed).c_str());
	sbf.lbl_prop[SBF_TEXT_COOLDOWN]->SetCaption(std::format("{}", info->dwAttackTime).c_str());
	sbf.lbl_prop[SBF_TEXT_DISTANCE]->SetCaption(std::format("{}", info->dwConsume).c_str());
	sbf.lbl_prop[SBF_TEXT_BODY]->SetCaption(info->szBody);
	sbf.lbl_prop[SBF_TEXT_HEAD]->SetCaption(info->szHeader);
	sbf.lbl_prop[SBF_TEXT_POWER]->SetCaption(info->szEngine);
	sbf.lbl_prop[SBF_TEXT_WEAPON]->SetCaption(info->szCannon);
	sbf.lbl_prop[SBF_TEXT_SIGN]->SetCaption(info->szEquipment);
	sbf.lbl_prop[SBF_TEXT_MONEY]->SetCaption(std::format("{}", info->dwMoney).c_str());

	ShowFlipBtn(flag, name);

	if (IsVisible() == 0) {
		Show(1);
	}

	return 1;
}

//---------------------------------------------------------------------------
BOOL xShipFactory::UpdateBoatCreate(xShipBuildInfo* info, BOOL flag, const char* name) {
	ClearBoatAttr();

	Update(info, flag, name);
	/* Show price when creating ship */
	sbf.imgMoneyTitle->SetIsShow(true);
	sbf.imgSpaceTitle->SetIsShow(false);

	char buf[128];
	sbf.lbl_prop[SBF_TEXT_MONEY]->SetCaption(itoa(info->dwMoney, buf, 10));

	sbf.lbl_ship_level->SetCaption("1");
	sbf.lbl_ship_exp->SetCaption("0");

	return 1;
}

//---------------------------------------------------------------------------
BOOL xShipFactory::UpdateBoatInfo(xShipBuildInfo* info, BOOL flag, const char* name) {
	// Get cabin info
	if (!GetCabinByID())
		return FALSE;

	ClearBoatAttr();

	Update(info, flag, name);

	CBoat* pkBoat = g_stUIBoat.FindBoat(__xsf->m_dwBoatID);
	if (!pkBoat)
		return 0;

	SGameAttr* pkAttr = pkBoat->GetCha()->getGameAttr();
	if (!pkAttr)
		return 0;


	UpdateBoatAttr();

	/* Show berth info when viewing ship */
	sbf.imgMoneyTitle->SetIsShow(false);
	sbf.imgSpaceTitle->SetIsShow(true);
	sbf.lbl_prop[SBF_TEXT_MONEY]->SetCaption(info->szBerth);

	ShowFlipBtn(flag, name);

	return 1;
}

//---------------------------------------------------------------------------
BOOL xShipFactory::UpdateBoatFreedomTrade(const char* name, DWORD* dwModelInfo, size_t size) {
	// Clear original boat attributes
	ClearBoatAttr();

	sbf.chkShip->SetIsShow(false);
	sbf.imgShipOrigin->SetIsShow(false);
	sbf.btn_room_close->SetIsShow(false);
	sbf.btn_room_open->SetIsShow(false);
	GetCabinByID();
	if (sbf.wnd_boat_room)
		sbf.wnd_boat_room->SetIsShow(true);

	// Set ship model info
	if (!SetShipModelInfo(dwModelInfo, sizeof(size)))
		return 0;

	// Set ship name
	ShowFlipBtn(0, name); // 1 when creating, 0 when viewing

	// Set ship attributes
	UpdateBoatAttr();

	return 1;
}

//---------------------------------------------------------------------------
void xShipFactory::SetBoatAttr() {
	sbf.lbl_prop[SBF_TEXT_ATTACK]->SetCaption(std::format("{}/{}", m_pkBoatInfo->dwMinAttack, m_pkBoatInfo->dwMaxAttack).c_str());
	sbf.lbl_prop[SBF_TEXT_ENDURE]->SetCaption(std::format("{}/{}", m_pkBoatInfo->dwCurEndure, m_pkBoatInfo->dwMaxEndure).c_str());
	sbf.lbl_prop[SBF_TEXT_SUPPLY]->SetCaption(std::format("{}/{}", m_pkBoatInfo->dwCurSupply, m_pkBoatInfo->dwMaxSupply).c_str());

	sbf.lbl_prop[SBF_TEXT_SPEED]->SetCaption(std::format("{}", m_pkBoatInfo->dwSpeed).c_str());
	sbf.lbl_prop[SBF_TEXT_DISTANCE]->SetCaption(std::format("{}", m_pkBoatInfo->dwDistance).c_str());
	sbf.lbl_prop[SBF_TEXT_DEFENCE]->SetCaption(std::format("{}", m_pkBoatInfo->dwDefence).c_str());
}

//---------------------------------------------------------------------------
void xShipFactory::SetManBoatAttr() {
	sbf.lbl_prop[SBF_TEXT_ATTACK]->SetCaption(std::format("{}/{}", m_pkBoatInfo->dwMinAttack, m_pkBoatInfo->dwMaxAttack).c_str());
	sbf.lbl_prop[SBF_TEXT_ENDURE]->SetCaption(std::format("{}/{}", m_pkBoatInfo->dwCurEndure, m_pkBoatInfo->dwMaxEndure).c_str());
	sbf.lbl_prop[SBF_TEXT_SUPPLY]->SetCaption(std::format("{}/{}", m_pkBoatInfo->dwCurSupply, m_pkBoatInfo->dwMaxSupply).c_str());

	sbf.lbl_prop[SBF_TEXT_SPEED]->SetCaption(std::format("{}", m_pkBoatInfo->dwSpeed).c_str());
	sbf.lbl_prop[SBF_TEXT_DISTANCE]->SetCaption(std::format("{}", m_pkBoatInfo->dwDistance).c_str());
	sbf.lbl_prop[SBF_TEXT_DEFENCE]->SetCaption(std::format("{}", m_pkBoatInfo->dwDefence).c_str());
}

//---------------------------------------------------------------------------
BOOL xShipFactory::Close(CGuiData* sender) {
	if (sender == sbf.btn_create) {
		if (CheckShipName()) {
			CS_CreateBoat(ship_name, item_id[ITEM_HEAD], item_id[ITEM_POWER],
						  item_id[ITEM_WEAPON], item_id[ITEM_GOODS]);
		}
		else {
			// g_pGameApp->MsgBox( "Invalid ship name" );
			goto __ret;
		}
	}
	else if (sender == sbf.btn_cancel) {
		CS_CancelBoat();
	}

	Show(0);
__ret:
	return 1;
}

//---------------------------------------------------------------------------
void xShipFactory::FrameMove() {
	if (sbf.wnd && sbf.wnd->GetIsShow()) {
		if (g_pGameApp->IsMouseContinue(0)) {
			int x = g_pGameApp->GetMouseX();
			int y = g_pGameApp->GetMouseY();

			if (sbf.btn_view3d[0]->InRect(x, y)) {
				yaw_degree += 2;
			}
			else if (sbf.btn_view3d[1]->InRect(x, y)) {
				yaw_degree -= 2;
			}
			else if (sbf.btn_view3d[2]->InRect(x, y)) {
				pitch_degree += 2;
			}
			else if (sbf.btn_view3d[3]->InRect(x, y)) {
				pitch_degree -= 2;
			}
		}

		m_count = ++m_count % 30;
		if (0 == m_count)
			UpdateBoatAttr();
	}
}

//---------------------------------------------------------------------------

void xShipFactory::ClearBoatAttr() {
	sbf.name->SetCaption("");
	if (sbf.wnd_boat_room)
		sbf.wnd_boat_room->SetIsShow(false);

	sbf.lbl_prop[SBF_TEXT_ATTACK]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_DEFENCE]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_ENDURE]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_SUPPLY]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_SPEED]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_COOLDOWN]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_DISTANCE]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_CAPACITY]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_MONEY]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_BODY]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_HEAD]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_POWER]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_WEAPON]->SetCaption("");
	sbf.lbl_prop[SBF_TEXT_SIGN]->SetCaption("");
	sbf.name->SetCaption("");

	sbf.lbl_ship_level->SetCaption("");
	sbf.lbl_ship_exp->SetCaption("");
}

void xShipFactory::UpdateBoatAttr() {
	if (m_dwBoatID == -1) {
		return;
	}

	CBoat* pkBoat = g_stUIBoat.FindBoat(m_dwBoatID);
	if (!pkBoat) {
		return;
	}

	SGameAttr* pkAttr = pkBoat->GetCha()->getGameAttr();
	if (!pkAttr) {
		return;
	}

	if (sbf.chkShip->GetIsChecked()) {
		/* Show ship base attributes */
		sbf.lbl_prop[SBF_TEXT_ATTACK]->SetCaption(std::format("{}/{}", pkAttr->get(ATTR_BMNATK), pkAttr->get(ATTR_BMXATK)).c_str());
		sbf.lbl_prop[SBF_TEXT_ENDURE]->SetCaption(std::format("{}/{}", pkAttr->get(ATTR_HP), pkAttr->get(ATTR_BMXHP)).c_str());
		sbf.lbl_prop[SBF_TEXT_SUPPLY]->SetCaption(std::format("{}/{}", pkAttr->get(ATTR_SP), pkAttr->get(ATTR_BMXSP)).c_str());

		sbf.lbl_prop[SBF_TEXT_SPEED]->SetCaption(std::format("{}", pkAttr->get(ATTR_BMSPD)).c_str());
		sbf.lbl_prop[SBF_TEXT_DEFENCE]->SetCaption(std::format("{}", pkAttr->get(ATTR_BDEF)).c_str());
		sbf.lbl_prop[SBF_TEXT_DISTANCE]->SetCaption(std::format("{}", pkAttr->get(ATTR_BSREC)).c_str());
	}
	else {
		/* Show ship actual attributes (affected by sailors) */
		sbf.lbl_prop[SBF_TEXT_ATTACK]->SetCaption(std::format("{}/{}", pkAttr->get(ATTR_MNATK), pkAttr->get(ATTR_MXATK)).c_str());
		sbf.lbl_prop[SBF_TEXT_ENDURE]->SetCaption(std::format("{}/{}", pkAttr->get(ATTR_HP), pkAttr->get(ATTR_MXHP)).c_str());
		sbf.lbl_prop[SBF_TEXT_SUPPLY]->SetCaption(std::format("{}/{}", pkAttr->get(ATTR_SP), pkAttr->get(ATTR_MXSP)).c_str());

		sbf.lbl_prop[SBF_TEXT_SPEED]->SetCaption(std::format("{}", pkAttr->get(ATTR_MSPD)).c_str());
		sbf.lbl_prop[SBF_TEXT_DEFENCE]->SetCaption(std::format("{}", pkAttr->get(ATTR_DEF)).c_str());
		sbf.lbl_prop[SBF_TEXT_DISTANCE]->SetCaption(std::format("{}", pkAttr->get(ATTR_SREC)).c_str());
	}

	CGoodsGrid* grd = g_stUIBoat.FindBoat(m_dwBoatID)->GetGoodsGrid();
	int nCurCapacity = grd->GetCurNum();
	int nMaxCapacity = grd->GetMaxNum();
	sbf.lbl_prop[SBF_TEXT_CAPACITY]->SetCaption(std::format("{}/{}", nCurCapacity, nMaxCapacity).c_str());

	// Attack cooldown
	int v = pkAttr->get(ATTR_ASPD);
	if (v == 0)
		sbf.lbl_prop[SBF_TEXT_COOLDOWN]->SetCaption("-1");
	else
		sbf.lbl_prop[SBF_TEXT_COOLDOWN]->SetCaption(std::format("{}", 100000 / v).c_str());

	// Ship level and experience
	sbf.lbl_ship_level->SetCaption(std::format("{}", pkAttr->get(ATTR_LV)).c_str());
	sbf.lbl_ship_exp->SetCaption(std::format("{}", pkAttr->get(ATTR_CEXP)).c_str());

	//
	//sbf.lbl_prop[SBF_TEXT_BODY]->SetCaption(info->szBody);
	//sbf.lbl_prop[SBF_TEXT_HEAD]->SetCaption(info->szHeader);
	//sbf.lbl_prop[SBF_TEXT_POWER]->SetCaption(info->szEngine);
	//sbf.lbl_prop[SBF_TEXT_WEAPON]->SetCaption(info->szCannon);
	//sbf.lbl_prop[SBF_TEXT_SIGN]->SetCaption(info->szEquipment);
}

//---------------------------------------------------------------------------
void xShipFactory::Test() {
	xShipBuildInfo info;

	DWORD id[8] = {2000000000, 2000000001, 2000000002, 2000000004, 0, 0, 0, 0};
	memcpy(info.dwBuf, id, sizeof(id));

	info.dwMinAttack = 9;
	info.dwMaxAttack = 99;
	info.dwCurEndure = 8;
	info.dwMaxEndure = 88;
	info.dwCurSupply = 7;
	info.dwMaxSupply = 77;
	info.sCapacity = 6;
	info.sCapacity = 66;
	info.dwDefence = 5;
	info.dwSpeed = 4;
	info.dwAttackTime = 3;
	info.dwDistance = 2;

	Update(&info, 0, "okkkk");
}

//---------------------------------------------------------------------------
// xShipLaunchList
static xShipLaunchList* __xsl = 0;

//---------------------------------------------------------------------------
void xShipLaunchList::__ProcSelectChange(CGuiData* pSender) {
	__xsl->SelectItem(pSender);
}

//---------------------------------------------------------------------------
void xShipLaunchList::__ButtonClose(CGuiData* pSender, int x, int y, DWORD dwKey) {
	__xsl->wnd->Close();
}

//---------------------------------------------------------------------------
xShipLaunchList::xShipLaunchList() {
	__xsl = this;

	scene = 0;
	form_mgr = 0;
	wnd = 0;
	memo = 0;
}

//---------------------------------------------------------------------------
xShipLaunchList::~xShipLaunchList() {
}

//---------------------------------------------------------------------------
BOOL xShipLaunchList::Init(CGameScene* s) {
	scene = s;
	form_mgr = &CFormMgr::s_Mgr;

	wnd = form_mgr->Find("frmNPCShip", enumMainForm);
	ERR_CHK(wnd, "npcShip");

	memo = (CMemo*)wnd->Find("memCtrl");
	ERR_CHK(memo, "memCtrl");
	memo->evtSelectChange = __ProcSelectChange;

	btn_close = (CTextButton*)wnd->Find("btnClose");
	ERR_CHK(btn_close, "btnClose");
	btn_close->evtMouseClick = __ButtonClose;

	btn_cancel = (CTextButton*)wnd->Find("btnNo");
	ERR_CHK(btn_cancel, "btnNo");
	btn_cancel->evtMouseClick = __ButtonClose;

	return 1;
}

//---------------------------------------------------------------------------
void xShipLaunchList::Update(DWORD num, const BOAT_BERTH_DATA* data,
							 const eFlag flag/* =eLaunch */) {
	m_eFlag = flag;

	memo->Init();
	memo->reset();

	memo->SetCaption(GetLanguageString(393).c_str());
	memo->SetIsHaveItem(1);
	memo->SetItemRowNum(num);
	//memo->SetRowHeight(20);

	for (DWORD i = 0; i < num; i++) {
		memo->AddItemRowContent(i, data->szName[i]);
	}
	memo->ProcessCaption();

	wnd->Refresh();
	wnd->Show();
}

//---------------------------------------------------------------------------
void xShipLaunchList::SelectItem(CGuiData* pSender) {
	if (memo != pSender)
		return;

	if (!memo->GetIsHaveItem())
		return;

	int sel_item = memo->GetSelectItem();
	if (sel_item == -1)
		return;

	if (eLaunch == m_eFlag) {
		g_stUIBoat.GetHuman()->RemoveCloakGlow();
		CS_SelectBoat(m_dwNpcID, sel_item);
	}
	else if (eTrade == m_eFlag || eRepair == m_eFlag || eSalvage == m_eFlag || eSupply == m_eFlag || eUpgrade ==
		m_eFlag) {
		CS_SelectBoatList(m_dwNpcID, m_byType, sel_item);
	}
	else if (eBag == m_eFlag) {
		CS_SelectBoatBag(m_dwNpcID, sel_item);
	}

	wnd->Close();
}


//---------------------------------------------------------------------------
void xShipLaunchList::Test() {
	int n = 3;
	BOAT_BERTH_DATA data;
	strcpy(data.szName[0], "aaaa");
	strcpy(data.szName[1], "bbbb");
	strcpy(data.szName[2], "cccc");

	Update(n, &data);
}

//---------------------------------------------------------------------------
//  add by Philip.Wu  2006-06-02
//  Close ship selection form, called from xShipMgr internally
//
void xShipLaunchList::CloseForm() {
	if (wnd && wnd->GetIsShow()) {
		wnd->SetIsShow(false);
	}
}


//---------------------------------------------------------------------------
// xShipMgr
xShipMgr::xShipMgr() {
	_factory = 0;
	_launch_list = 0;
}

//---------------------------------------------------------------------------
xShipMgr::~xShipMgr() {
	SAFE_DELETE(_factory);
	SAFE_DELETE(_launch_list);
}

//---------------------------------------------------------------------------
BOOL xShipMgr::Init(CGameScene* s) {
	_factory = new xShipFactory;
	_launch_list = new xShipLaunchList;

	_factory->Init(s);
	_launch_list->Init(s);

	return 1;
}

//---------------------------------------------------------------------------
void xShipMgr::FrameMove() {
	if (_factory) {
		_factory->FrameMove();
	}
}

//---------------------------------------------------------------------------
//  add by Philip.Wu  2006-06-02
//  Close ship selection form, called externally
//
void xShipMgr::CloseForm() {
	if (_launch_list) {
		_launch_list->CloseForm();
		g_stUIBoat.GetHuman()->RenderCloakGlow();
	}
}
