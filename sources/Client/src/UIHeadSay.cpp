#include "StdAfx.h"
#include "UIText.h"
#include "SkillStateRecordStore.h"
#include "uiheadsay.h"
#include "GameApp.h"
#include "Character.h"
#include "uiitem.h"
#include "UILabel.h"
#include "UIProgressBar.h"
#include "SkillStateRecord.h"
#include "chastate.h"
#include "uiboatform.h"
#include "uiMiniMapForm.h"
#include "uistartForm.h"
#include "globalvar.h"
#include "uiequipform.h"

using namespace std;
using namespace GUI;

bool g_IsShowModel = true;
bool g_IsShowShop = true;
bool g_IsShowHp = false;
bool g_IsShowNames = false;
bool g_IsShowStates = false;
bool CHeadSay::_ShowEnemyNames = true;
bool CHeadSay::_ShowBars = false;
bool CHeadSay::_ShowPercentages = false;
bool CHeadSay::_ShowInfo = false;

//---------------------------------------------------------------------------
// calss CHeadSay
//---------------------------------------------------------------------------
int CHeadSay::_nMaxShowTime = 100;
CGuiPic* CHeadSay::_pImgLife = new CGuiPic(NULL, 2);
CGuiPic* CHeadSay::_pImgMana = new CGuiPic(NULL, 2);

DWORD CHeadSay::_dwBkgColor = 0x90000000;

unsigned int CHeadSay::_nFaceFrequency = 6;
CGuiPic* CHeadSay::_pImgFace = new CGuiPic[FACE_MAX];
CGuiPic* CHeadSay::_pImgEvil = new CGuiPic[EVIL_MAX];

CGuiPic* CHeadSay::_pImgTeamLeaderFlag = new CGuiPic;
CGuiPic* CHeadSay::_pImgGuildLeaderFlag = new CGuiPic;

CGuiPic* CHeadSay::_pImgShopHidden = new CGuiPic;

CGuiPic CHeadSay::_ImgShop[3];
CGuiPic CHeadSay::_ImgShop2[3];
int CHeadSay::_nShopFrameWidth = 0;
int CHeadSay::_nShopFontYOff = 0;

// ()   + ()  []
char CHeadSay::s_sNamePart[NAME_PART_NUM][64] = {0};


char CHeadSay::s_szName[1024] = {0};

DWORD CHeadSay::s_dwNamePartsColors[NAME_PART_NUM][2] = //	,
{
	COLOR_WHITE, COLOR_BLACK, //	(
	0, COLOR_BLACK, //	
	COLOR_WHITE, COLOR_BLACK, //	)
	0, COLOR_BLACK, //	
	COLOR_BLACK, 0, //	(
	COLOR_BLACK, 0, //	
	COLOR_BLACK, 0, //	)
	COLOR_WHITE, COLOR_BLACK, //	[
	COLOR_SHIP_NAME, COLOR_BLACK, //	
	COLOR_WHITE, COLOR_BLACK, //	]
};

char CHeadSay::s_szConsortiaNamePart[4][64] = {0};


char CHeadSay::s_szConsortiaName[256] = {0};


// Member Methods---------------------------------------------------------

bool CHeadSay::Init() {
	_pImgTeamLeaderFlag->LoadImage("texture/ui/flag.tga", 12, 12, 0, 0, 0, 1.0, 1.0);
	_pImgGuildLeaderFlag->LoadImage("texture/ui/guildflag.tga", 16, 16, 0, 0, 0, 0.8, 0.8);
	_pImgShopHidden->LoadImage("texture/ui/hidestall.tga", 25, 32, 0, 0, 0, 1.0, 1.0);

	_nShopFrameWidth = _ImgShop[0].GetWidth() - 1;
	_nShopFontYOff = (_ImgShop[0].GetHeight() - ui::GetHeight(GetLanguageString(489).c_str())) / 2;

	for (int i = 0; i < EVIL_MAX; i++) {
		const std::string buffer = std::format("texture/icon/evil_lv{}.png", i + 1);

		if (!_pImgEvil)
			break;

		_pImgEvil[i].LoadImage(buffer.c_str(), 16, 16, 0, 0, 0, 0.0, 0.0);
	}
	return true;
}

bool CHeadSay::Clear() {
	if (_pImgLife) {
		delete _pImgLife;
		_pImgLife = NULL;
	}

	if (_pImgMana) {
		delete _pImgMana;
		_pImgMana = NULL;
	}

	if (_pImgFace) {
		delete [] _pImgFace;
		_pImgFace = NULL;
	}

	if (_pImgTeamLeaderFlag) {
		delete _pImgTeamLeaderFlag;
		_pImgTeamLeaderFlag = NULL;
	}

	if (_pImgGuildLeaderFlag) {
		delete _pImgGuildLeaderFlag;
		_pImgGuildLeaderFlag = NULL;
	}

	if (_pImgShopHidden) {
		delete _pImgShopHidden;
		_pImgShopHidden = NULL;
	}

	if (_pImgEvil) {
		delete[] _pImgEvil;
	}

	return true;
}

CHeadSay::CHeadSay(CCharacter* p)
	: _pOwn(p), _nShowTime(0), _pObj(NULL), _fLifeW(1.0f), _nFaceTime(0), _pCurFace(NULL), _IsShowLife(false),
	  _IsShowMana(false)
	  , _IsShowName(false), _nChaNameOffX(0), _fScale(0.1f), _dwNameColor(COLOR_WHITE), _sEvilLevel(0),
	  _IsShowEvil(false), isGuildMember(false), isTeamMember(false), RenderDebuff(false) {
}

void CHeadSay::AddItem(CItemEx* obj) {
	if (_pObj) delete _pObj;

	_pObj = obj;

	_nShowTime = _nMaxShowTime;
	_fScale = 0.1f;
}

void CHeadSay::SetName(const std::string& name) {
	_nChaNameOffX = 0 - ui::GetWidth(name) / 2;
}

void CHeadSay::RenderStateIcons(CCharacter* cha, int x, int y, float scale, float spacing, int rowSize,
								bool Rendertimer) {
	if (!cha->IsMainCha())
		Rendertimer = false;

	CGuiPic stateIcon;
	int stateCount = 0;
	bool IsStatesPerLevel = false;
	bool RenderIcon = false;
	SkillStateRecordStore::Instance()->ForEach([&](const CSkillStateRecord& state) {
		int i = state.Id;
		if (cha->GetStateMgr()->HasSkillState(i)) {
			auto* pState = &state;
			if (pState) {
				if (0 != stricmp(pState->szIcon[0], "0")) {
					if (0 == stricmp(pState->szIcon[cha->GetStateMgr()->GetStateLv(i) - 1], "0")) {
						IsStatesPerLevel = false;
					}
					else IsStatesPerLevel = true;
					RenderIcon = true;
				}
				if (RenderIcon) {
					const std::string buf = std::format("texture/icon/{}.png",
							(IsStatesPerLevel
								 ? pState->szIcon[cha->GetStateMgr()->GetStateLv(i) - 1]
								 : pState->szIcon[0]));
					stateIcon.LoadImage(buf.c_str(), 32, 32, 0, 0, 0, scale, scale);

					int yspace = 0;
					if (Rendertimer) yspace = 22;

					int xi = x + ((stateCount % rowSize) * spacing);
					int yi = y + ((stateCount / rowSize) * (spacing + yspace));
					stateIcon.Render(xi, yi);


					CChaStateMgr::stChaState stateData = cha->GetStateMgr()->GetStateData(i);

					if (stateData.lTimeRemaining > 0) {
						int hours = stateData.lTimeRemaining / 3600;
						int minutes = stateData.lTimeRemaining / 60;
						int seconds = stateData.lTimeRemaining % 60;
						int centeredX;
						const std::string szTime = std::format("{}m", minutes);
						if (minutes >= 10) {
							centeredX = (xi + (32 * scale) / 2) - 8;
						}
						else {
							centeredX = (xi + (32 * scale) / 2) - 5;
						}
						GetRender().FillFrame(xi, yi + (32 * scale), xi + (32 * scale) + 1,
											  yi + (32 * scale) + ui::GetHeight("3"));
						ui::Render(0, szTime, centeredX, yi + (32 * scale), COLOR_WHITE, 1.0f); //szTime
					}

					stateCount++;

					int xm = g_pGameApp->GetMouseX();
					int ym = g_pGameApp->GetMouseY();

					//check if we need to render hint.
					if (xm >= xi && xm <= xi + (32 * scale)) {
						if (ym >= yi && ym <= yi + (32 * scale)) {
							char desc[300];
							g_pGameApp->ShowStateHint(xm, ym, stateData);
						}
					}
				}
			}
		}
	});

	//hide states form if has 0 active states 
	if (stateCount > 0 && g_IsShowStates) {
		int w = 19 * stateCount;
		int h = stateCount <= 9 ? 45 : stateCount >= 18 ? 120 : 94;
		if (w > 250) w = 250;
		g_stUIEquip.stateDrags->SetSize(w, h);
		g_stUIEquip.stateDrags->Refresh();
		//g_stUIEquip.stateDrags->SetIsShow(true);
	}
	else {
		g_stUIEquip.stateDrags->SetSize(0, 0);
		g_stUIEquip.stateDrags->Refresh();
		//g_stUIEquip.stateDrags->SetIsShow(false);
	}
}

void CHeadSay::Render(D3DXVECTOR3& pos) {
	static int x = 0, y = 0;
	static int nSayTotalWidth = 32 * ui::GetWidth("a"); //32
	g_Render.WorldToScreen(pos.x, pos.y, pos.z + _pOwn->GetDefaultChaInfo()->fHeight, &x, &y);

	/*
	if(_pOwn->getHumanID() == g_stUIStart.targetInfoID && g_stUIStart.frmTargetInfo->GetIsShow()){
		float scale = 0.45;
		float spacing = scale * 32 + 2;
		RenderStateIcons(_pOwn,200,40, scale,spacing,10);
	}	
	*/

	if (_ShowBars && (_IsShowLife || (_pOwn->IsPlayer() && _ShowInfo)) && !_fLifeW <= 0) //
	{
		static int x1 = 0, y1 = 0;
		g_Render.WorldToScreen(pos.x, pos.y, pos.z, &x1, &y1);

		int nLifeWidth = _pImgLife->GetWidth();
		_pImgLife->SetScaleW(1, _fLifeW);
		int nOffset = (int)((x - g_Render.GetScrWidth() / 2) * 0.02f);

		DWORD hpcolour = 0xA0FF0000;
		int red, green;
		if (_fLifeW > 0.5f) {
			red = 510 - (_fLifeW * 2 * 255);
			green = 255;
			hpcolour = 0xB3000000 + (256 * green) + (65536 * red);
		}
		else if (_fLifeW <= 0.5f && _fLifeW > 0) {
			red = 255;
			green = _fLifeW * 2 * 255;
			hpcolour = 0xB3000000 + (256 * green) + (65536 * red); //B3
		}

		_pImgLife->RenderAll(x - nLifeWidth / 2 - nOffset, y1 + 20, hpcolour);

		std::string hpInfo;

		if (_ShowPercentages || (_pOwn->GetIsPK() && !isTeamMember)) {
			hpInfo = std::format("{:.0f}%", _fLifeW * 100);
		}
		else {
			hpInfo = std::format("{}/{}", _fCurHp, _fMxHp);
		}

		if (_pOwn->GetStateMgr()->HasSkillState(83)) {
			if (isTeamMember || isGuildMember || (_ShowInfo && _pOwn->IsPlayer()) || _IsShowLife) {
				if (_pOwn->IsPlayer()) {
					int nManaWidth = _pImgMana->GetWidth();
					_pImgMana->SetScaleW(1, _fManaW);

					DWORD spcolour = 0xA0FF0000;
					if (_fManaW > 0) {
						int red = 255 - (_fManaW * 255);
						int blue = _fManaW * 255;
						spcolour = 0xB3000000 + blue + (65536 * red);
					}

					_pImgMana->RenderAll(x - nManaWidth / 2 - nOffset, y1 + 28, spcolour);

					std::string spInfo;

					if (_ShowPercentages || (_pOwn->GetIsPK() && !isTeamMember)) {
						spInfo = std::format("{:.0f}%", _fManaW * 100);
					}
					else {
						spInfo = std::format("{}/{}", _fCurSp, _fMxSp);
					}
					ui::BRender(spInfo, x - nOffset - (ui::GetWidth(spInfo) / 2), y1 + 32, spcolour, COLOR_BLACK);
				}
			}
		}

		ui::BRender(hpInfo, x - nOffset - (ui::GetWidth(hpInfo) / 2), y1 + 8, hpcolour, COLOR_BLACK);

	}


	if (g_IsShowStates) {
		float scale = 0.60;
		float spacing = scale * 32 + 2;
		int picX = 0;
		int picY = 0;
		int count = 9;
		if (_pOwn == CGameScene::GetMainCha()) {
			picX = g_stUIEquip.stateDrags->GetLeft();
			picY = g_stUIEquip.stateDrags->GetTop();
			RenderStateIcons(_pOwn, picX, picY, scale, spacing, count, true);
		}
	}


	if (g_stUIMap.IsPKSilver()) {
		// 
		return;
	}

	if (_pOwn->GetIsFly())
		y -= 30;


	if (_pOwn->IsTeamLeader()) {
		y -= 20;
		GetLeaderPic()->Render(x - 6, y);
	}

	if (g_IsShowShop && _pOwn->IsShop()) {
		if (_pOwn->GetStateMgr()->GetStateLv(99) == 3) {
			_RenderShop2(_pOwn->getShopName(), x, y);
		}
		else {
			_RenderShop(_pOwn->getShopName(), x, y);
		}

		static int nImageHeight = _ImgShop[0].GetHeight();
		y -= nImageHeight;
	}

	if (!g_IsShowShop && _pOwn->IsShop()) {
		_pImgShopHidden->Render(x - (_pImgShopHidden->GetWidth() / 2), y - 30);
	}

	if (_nShowTime <= 0) {
#ifdef _LOG_NAME_                     //
		const int LINE_HEIGHT_STEP = 14; //
		int iNameHeightStep = LINE_HEIGHT_STEP; //

		// 
		if (CCharacter::IsShowLogName) {
			int nNameLength = 0 - ui::GetWidth(_pOwn->getLogName()) / 2;
			ui::BRender(_pOwn->getLogName(), x + nNameLength, y - iNameHeightStep, _dwNameColor, COLOR_BLACK);
			//_dwNameColor
		}
		else if (_IsShowName || g_IsShowNames || (_pOwn->GetIsPK() && _ShowEnemyNames) || (_ShowInfo && _pOwn->
			IsPlayer())) {
			//
			if (!_pOwn->getGuildName().empty()) {
				iNameHeightStep += LINE_HEIGHT_STEP; //
				int iGuildNameHeightStep = LINE_HEIGHT_STEP;

				//
				if (!_pOwn->getGuildName().empty()) {
					strncpy(s_szConsortiaNamePart[0], _pOwn->getGuildName().c_str(), NAME_LENGTH);
				}
				else {
					strncpy(s_szConsortiaNamePart[0], "", NAME_LENGTH);
				}
				//
				if (!_pOwn->getGuildMotto().empty()) {
					strncpy(s_szConsortiaNamePart[1], "(", strlen("("));
					strncpy(s_szConsortiaNamePart[2], _pOwn->getGuildMotto().c_str(), NAME_LENGTH);
					strncpy(s_szConsortiaNamePart[3], ")", strlen(")"));
				}
				else {
					strncpy(s_szConsortiaNamePart[1], "", NAME_LENGTH);
					strncpy(s_szConsortiaNamePart[2], "", NAME_LENGTH);
					strncpy(s_szConsortiaNamePart[3], "", NAME_LENGTH);
				}

				//ZeroMemory(s_szConsortiaName, sizeof(s_szConsortiaName));
				s_szConsortiaName[0] = '\0';
				for (int i(0); i < 4; i++) {
					strncat(s_szConsortiaName, s_szConsortiaNamePart[i], NAME_LENGTH);
				}
				int nNameLength = 0 - ui::GetWidth(s_szConsortiaName) / 2;
				int iStartPosX = 0 - ui::GetWidth(s_szConsortiaName) / 2;

				// render
				int perm = (_pOwn->getGuildPermission() & emGldPermLeader);
				if (perm == emGldPermLeader) {
					//GetGuildLeaderPic()->TintColour( 255, 0, 0 );
					GetGuildLeaderPic()->Render(x + iStartPosX - 13, y - iGuildNameHeightStep - 2);
				}

				for (int i(0); i < 4; i++) {
					ui::BRender(s_szConsortiaNamePart[i], x + iStartPosX, y - iGuildNameHeightStep,
								_pOwn->getGuildColor(), COLOR_BLACK);
					iStartPosX += ui::GetWidth(s_szConsortiaNamePart[i]);
				}
			}

			//
			if (!_pOwn->GetPreName().empty()) {
				strncpy(s_sNamePart[PRENAME_SEP1_INDEX], "(", strlen("("));
				strncpy(s_sNamePart[PRENAME_INDEX], _pOwn->GetPreName().c_str(), NAME_LENGTH);
				strncpy(s_sNamePart[PRENAME_SEP2_INDEX], ")", strlen(")"));
				s_dwNamePartsColors[PRENAME_INDEX][0] = _pOwn->GetPreColor();
			}
			else {
				strncpy(s_sNamePart[PRENAME_SEP1_INDEX], "", NAME_LENGTH);
				strncpy(s_sNamePart[PRENAME_INDEX], "", NAME_LENGTH);
				strncpy(s_sNamePart[PRENAME_SEP2_INDEX], "", NAME_LENGTH);
				s_dwNamePartsColors[PRENAME_INDEX][0] = 0;
			}

			//
			if (!_pOwn->getSecondName().empty()) {
				strncpy(s_sNamePart[MOTTO_NAME_SEP1_INDEX], "(", strlen("("));
				strncpy(s_sNamePart[MOTTO_NAME_INDEX], _pOwn->getSecondName().c_str(), NAME_LENGTH);
				strncpy(s_sNamePart[MOTTO_NAME_SEP2_INDEX], ")", strlen(")"));
			}
			else {
				strncpy(s_sNamePart[MOTTO_NAME_SEP1_INDEX], "", NAME_LENGTH);
				strncpy(s_sNamePart[MOTTO_NAME_INDEX], "", NAME_LENGTH);
				strncpy(s_sNamePart[MOTTO_NAME_SEP2_INDEX], "", NAME_LENGTH);
			}

			//

			// Added by NINJA#5684 June 2021 credits to Igor Silva#2195
			if (_ShowEnemyNames == true && _pOwn->IsPlayer() && CGameScene::GetMainCha()) {
				if (!_pOwn->IsMainCha() && _pOwn->GetIsPK()) {
					if (_pOwn->GetTeamLeaderID() && _pOwn->GetTeamLeaderID() == CGameApp::GetCurScene()->GetMainCha()->
						GetTeamLeaderID())
						SetNameColor(COLOR_GREEN);
					else if (_pOwn->getGuildID() && _pOwn->getGuildID() == CGameApp::GetCurScene()->GetMainCha()->
						getGuildID())
						SetNameColor(COLOR_GREEN);
					else
						SetNameColor(COLOR_RED);
				}
				else {
					SetNameColor(COLOR_WHITE);
				}
			}

			s_dwNamePartsColors[NAME_INDEX][0] = _dwNameColor;

			// Added by Mdr.st May 2020 - FPO alpha
			/* if (_ShowEnemyNames == true && _pOwn->GetIsPK() && !_pOwn->IsMainCha() && !_pOwn->IsNPC() ) {
				s_dwNamePartsColors[NAME_INDEX][0] = COLOR_RED;
			}
			if (_ShowEnemyNames == true && _pOwn->GetHeadSay()->isTeamMember && _pOwn->GetHeadSay()->isGuildMember && !_pOwn->IsMainCha() && _pOwn->GetIsPK()){
				s_dwNamePartsColors[NAME_INDEX][0] = COLOR_GREEN;
			} */

			// End

			// _dwNameColor;
			if (_pOwn->IsBoat()) {
				strncpy(s_sNamePart[NAME_INDEX], _pOwn->getHumanName().c_str(), NAME_LENGTH);
				strncpy(s_sNamePart[BOAT_NAME_SEP1_INDEX], "[", strlen("["));
				strncpy(s_sNamePart[BOAT_NAME_INDEX], _pOwn->getName().c_str(), NAME_LENGTH);
				strncpy(s_sNamePart[BOAT_NAME_SEP2_INDEX], "]", strlen("]"));
			}
			else {
				strncpy(s_sNamePart[BOAT_NAME_SEP1_INDEX], "", NAME_LENGTH);
				strncpy(s_sNamePart[BOAT_NAME_INDEX], "", NAME_LENGTH);
				strncpy(s_sNamePart[BOAT_NAME_SEP2_INDEX], "", NAME_LENGTH);
				strncpy(s_sNamePart[NAME_INDEX], _pOwn->getName().c_str(), NAME_LENGTH);

				if (_pOwn->IsMonster()) {
					//10
					if (_ShowInfo && !_IsShowName) {
						return;
					}

					// 
					static int nMainLevel(0);
					if (g_stUIBoat.GetHuman()) {
						nMainLevel = g_stUIBoat.GetHuman()->getGameAttr()->get(ATTR_LV);
					}

					// 
					static int nMonsterLevel(0);
					nMonsterLevel = _pOwn->getGameAttr()->get(ATTR_LV);

					std::string namePart;
					if (nMonsterLevel - nMainLevel <= 10) {
						namePart = std::format("Lv{} {}", nMonsterLevel, _pOwn->getName());
					}
					else {
						namePart = std::format("??? {}", _pOwn->getName());
					}
					strncpy(s_sNamePart[NAME_INDEX], namePart.c_str(), NAME_LENGTH);
				}
			}

			/*if (_pOwn->IsNPC())
			{
				s_dwNamePartsColors[NAME_INDEX][0] = 0xFF9BA1E9; // D3DXCOLOR(109,87,218,1); 
			}*/

			// 
			s_szName[0] = '\0';
			//ZeroMemory( s_szName, sizeof(s_szName) );
			for (int i(0); i < NAME_PART_NUM; i++) {
				strncat(s_szName, s_sNamePart[i], NAME_LENGTH);
			}
			int nNameLength = 0 - ui::GetWidth(s_szName) / 2;
			int iStartPosX = 0 - ui::GetWidth(s_szName) / 2;

			if (_IsShowEvil) {
				if (_pImgEvil) {
					if (_sEvilLevel != EVIL_MAX) {
						_pImgEvil[_sEvilLevel].Render(x + iStartPosX - 16, y - iNameHeightStep);
					}
				}
			}

			// render


			for (int i(0); i < NAME_PART_NUM; i++) {
				if (s_dwNamePartsColors[i][1]) {
					ui::BRender(s_sNamePart[i], x + iStartPosX, y - iNameHeightStep, s_dwNamePartsColors[i][0],
								s_dwNamePartsColors[i][1]);
				}
				else {
					ui::Render(s_sNamePart[i], x + iStartPosX, y - iNameHeightStep, s_dwNamePartsColors[i][0]);
				}
				iStartPosX += ui::GetWidth(s_sNamePart[i]);
			}
		}
#else
		if (_IsShowName) {
			if (_pOwn->IsBoat()) //+
			{
				static string sNameBuf;
				sNameBuf = _pOwn->getHumanName();
				sNameBuf += "--";
				sNameBuf += _pOwn->getName();

				int nNameLength = 0 - ui::GetWidth(sNameBuf.c_str()) / 2;
				ui::BRender(sNameBuf.c_str(), x + nNameLength, y - LINE_HEIGHT_STEP, _dwNameColor, COLOR_BLACK);
				if (_pOwn->IsShowSecondName()) {
					if (!_pOwn->getSecondName().empty()) {
						string strSec = "(";
						strSec += _pOwn->getSecondName();
						strSec += ")";
						ui::Render(strSec.c_str(), x - nNameLength, y - LINE_HEIGHT_STEP, COLOR_BLACK);
					}
				}
			}
			else //
			{
				int nNameLength = 0 - ui::GetWidth(_pOwn->getName()) / 2;
				ui::BRender(_pOwn->getName().c_str(), x + nNameLength, y - LINE_HEIGHT_STEP, _dwNameColor, COLOR_BLACK);
				if (_pOwn->IsShowSecondName()) {
					if (!_pOwn->getSecondName().empty()) {
						string strSec = "(";
						strSec += _pOwn->getSecondName();
						strSec += ")";
						ui::Render(strSec.c_str(), x - nNameLength, y - LINE_HEIGHT_STEP, COLOR_BLACK);
					}
				}
			} //end of if (_pOwn->IsBoat())
		}
#endif
	}
	else if (_nShowTime > 0) //
	{
		_nShowTime--;

		if (_pObj) {
			int left = x - (int)(_fScale * _pObj->GetWidth() / 2);
			int nLine = _pObj->GetLineNum();

			string str = _pObj->GetString();

			if (str.find("#") != std::string::npos) {
				if (nLine == 1) {
					GetRender().FillFrame(left - 2, y - 20 - 32, left + (int)(_fScale * _pObj->GetWidth()) + 4, y - 14,
										  _dwBkgColor);
					_pObj->RenderEx(left, y - 18 - 32, 32, _fScale);
				}
				else if (nLine == 2) {
					GetRender().FillFrame(left - 2, y - 20 - 44, left + (int)(_fScale * _pObj->GetWidth()) + 4, y - 14,
										  _dwBkgColor);
					_pObj->RenderEx(left, y - 18 - 44, 24, _fScale);
				}
				else if (nLine == 3) {
					GetRender().FillFrame(left - 2, y - 20 - 76, left + (int)(_fScale * _pObj->GetWidth()) + 4, y - 14,
										  _dwBkgColor);
					_pObj->RenderEx(left, y - 18 - 76, 24, _fScale);
				}
			}
			else {
				if (nLine == 1) {
					GetRender().FillFrame(left - 2, y - 20 - 18, left + (int)(_fScale * _pObj->GetWidth()) + 4, y - 14,
										  _dwBkgColor);
					_pObj->RenderEx(left, y - 18 - 18, 12, _fScale);
				}
				else if (nLine == 2) {
					GetRender().FillFrame(left - 2, y - 20 - 36, left + (int)(_fScale * _pObj->GetWidth()) + 4, y - 14,
										  _dwBkgColor);
					_pObj->RenderEx(left, y - 18 - 36, 12, _fScale);
				}
				else if (nLine == 3) {
					GetRender().FillFrame(left - 2, y - 20 - 54, left + (int)(_fScale * _pObj->GetWidth()) + 4, y - 14,
										  _dwBkgColor);
					_pObj->RenderEx(left, y - 18 - 54, 12, _fScale);
				}
			}
			_fScale += 0.2f;
			if (_fScale > 1.0f)
				_fScale = 1.0f;

			if (_nShowTime == 4)
				_fScale = 0.8f;
			else if (_nShowTime == 3)
				_fScale = 0.6f;
			else if (_nShowTime == 2)
				_fScale = 0.4f;
			else if (_nShowTime == 1)
				_fScale = 0.2f;
		}
	}

	if (_pCurFace && _nFaceTime > 0) //
	{
		_nFaceTime--;
		y -= 20;
		_nCurFaceCycle++;
		if (_nCurFaceCycle > _nFaceFrequency) {
			_nCurFaceCycle = 0;

			_nCurFaceFrame++;
			if (_nCurFaceFrame >= _pCurFace->GetMax()) {
				_nCurFaceFrame = 0;
			}
			_pCurFace->SetFrame(_nCurFaceFrame);
		}
		_pCurFace->Render(x - _pCurFace->GetWidth() / 2, y - 20);
	}

#ifdef _LOG_NAME_                     //
	if (CCharacter::IsShowLogName) {
		CChaStateMgr* pState = _pOwn->GetStateMgr();
		int nCount = pState->GetSkillStateNum();
		for (int i = 0; i < nCount; i++) {
			y -= 20;
			ui::Render(pState->GetSkillState(i)->szName.c_str(), x, y, COLOR_WHITE);
		}
	}
#endif
}

bool CHeadSay::SetFaceID(unsigned int faceid) {
	if (faceid >= FACE_MAX) return false;

	_nCurFaceFrame = 0;
	_nCurFaceCycle = 0;
	_nFaceTime = _nMaxShowTime;
	_pCurFace = &_pImgFace[faceid];
	return true;
}

void CHeadSay::SetLifeNum(int num, int max) {
	if (max == 0) return;

	if (num < 0) num = 0;
	if (num > max) num = max;
	if (_pImgLife) {
		_fLifeW = (float)num / (float)max;
		_fCurHp = num;
		_fMxHp = max;
	}
}

void CHeadSay::SetManaNum(int num, int max) {
	if (max == 0) return;

	if (num < 0) num = 0;
	if (num > max) num = max;
	if (_pImgMana) {
		_fManaW = (float)num / (float)max;
		_fCurSp = num;
		_fMxSp = max;
	}
}

void CHeadSay::_RenderShop(std::string_view szShopName, int x, int y) {
	static int nImageHeight = _ImgShop[0].GetHeight();
	static int nImageWidth = _ImgShop[0].GetWidth();
	y -= nImageHeight;

	static int nFontWidth = 0;
	nFontWidth = ui::GetWidth(szShopName);
	const DWORD dwImgColor = 0xffffffff;


	if (nFontWidth < 40) {
		static int nFontTrueWidth = 0;
		nFontTrueWidth = nFontWidth;
		nFontWidth = 40;

		x -= _nShopFrameWidth + nFontWidth / 2;

		_nShopX0 = x;
		_nShopY0 = y;
		_nShopX1 = x + nFontWidth + nImageWidth;
		_nShopY1 = y + nImageHeight;

		_ImgShop[0].Render(x, y, dwImgColor);
		x += _nShopFrameWidth;

		_ImgShop[2].Render(x + nFontWidth, y, dwImgColor);

		_ImgShop[1].SetScaleW(nFontWidth);
		_ImgShop[1].Render(x, y, dwImgColor);
		//ui::BRender( szShopName, x + (nFontWidth - nFontTrueWidth) / 2, y+_nShopFontYOff, COLOR_BLACK, COLOR_WHITE );
		ui::Render(szShopName, x + (nFontWidth - nFontTrueWidth) / 2, y + _nShopFontYOff, COLOR_BLACK);
	}
	else {
		x -= _nShopFrameWidth + nFontWidth / 2;

		_nShopX0 = x;
		_nShopY0 = y;
		_nShopX1 = x + nFontWidth + nImageWidth;
		_nShopY1 = y + nImageHeight;

		_ImgShop[0].Render(x, y, dwImgColor);
		x += _nShopFrameWidth;

		_ImgShop[2].Render(x + nFontWidth, y, dwImgColor);

		_ImgShop[1].SetScaleW(nFontWidth);
		_ImgShop[1].Render(x, y, dwImgColor);
		//ui::BRender( szShopName, x, y+_nShopFontYOff, COLOR_BLACK, COLOR_WHITE );
		ui::Render(szShopName, x, y + _nShopFontYOff, COLOR_BLACK);
	}
}

void CHeadSay::_RenderShop2(std::string_view szShopName, int x, int y) {
	static int nImageHeight = _ImgShop2[0].GetHeight();
	static int nImageWidth = _ImgShop2[0].GetWidth();
	y -= nImageHeight;

	static int nFontWidth = 0;
	nFontWidth = ui::GetWidth(szShopName);
	const DWORD dwImgColor = 0xffffffff;
	if (nFontWidth < 40) {
		static int nFontTrueWidth = 0;
		nFontTrueWidth = nFontWidth;
		nFontWidth = 40;

		x -= _nShopFrameWidth + nFontWidth / 2;

		_nShopX0 = x;
		_nShopY0 = y;
		_nShopX1 = x + nFontWidth + nImageWidth;
		_nShopY1 = y + nImageHeight;

		_ImgShop2[0].Render(x, y, dwImgColor);
		x += _nShopFrameWidth;

		_ImgShop2[2].Render(x + nFontWidth, y, dwImgColor);

		_ImgShop2[1].SetScaleW(nFontWidth);
		_ImgShop2[1].Render(x, y, dwImgColor);
		//ui::BRender( szShopName, x + (nFontWidth - nFontTrueWidth) / 2, y+_nShopFontYOff, COLOR_BLACK, COLOR_WHITE );
		ui::Render(szShopName, x + (nFontWidth - nFontTrueWidth) / 2, y + _nShopFontYOff, COLOR_BLACK);
	}
	else {
		x -= _nShopFrameWidth + nFontWidth / 2;

		_nShopX0 = x;
		_nShopY0 = y;
		_nShopX1 = x + nFontWidth + nImageWidth;
		_nShopY1 = y + nImageHeight;

		_ImgShop2[0].Render(x, y, dwImgColor);
		x += _nShopFrameWidth;

		_ImgShop2[2].Render(x + nFontWidth, y, dwImgColor);

		_ImgShop2[1].SetScaleW(nFontWidth);
		_ImgShop2[1].Render(x, y, dwImgColor);
		//ui::BRender( szShopName, x, y+_nShopFontYOff, COLOR_BLACK, COLOR_WHITE );
		ui::Render(szShopName, x, y + _nShopFontYOff, COLOR_BLACK);
	}
}

void CHeadSay::RenderText(const char* szShopName, int x, int y) {
	static int nImageHeight = _ImgShop[0].GetHeight();
	static int nImageWidth = _ImgShop[0].GetWidth();
	// y -= nImageHeight;
	y -= 100;

	static int nFontWidth = 0;
	nFontWidth = ui::GetWidth(szShopName);
	const DWORD dwImgColor = 0xffffffff;

	if (nFontWidth < 40) {
		static int nFontTrueWidth = 0;
		nFontTrueWidth = nFontWidth;
		nFontWidth = 40;

		x -= _nShopFrameWidth + nFontWidth / 2;

		_ImgShop[0].Render(x, y, dwImgColor);
		x += _nShopFrameWidth;

		_ImgShop[2].Render(x + nFontWidth, y, dwImgColor);

		_ImgShop[1].SetScaleW(nFontWidth);
		_ImgShop[1].Render(x, y, dwImgColor);
		ui::BRender(szShopName, x + (nFontWidth - nFontTrueWidth) / 2, y + _nShopFontYOff, COLOR_BLACK, COLOR_WHITE);
	}
	else {
		x -= _nShopFrameWidth + nFontWidth / 2;

		_ImgShop[0].Render(x, y, dwImgColor);
		x += _nShopFrameWidth;

		_ImgShop[2].Render(x + nFontWidth, y, dwImgColor);

		_ImgShop[1].SetScaleW(nFontWidth);
		_ImgShop[1].Render(x, y, dwImgColor);
		ui::BRender(szShopName, x, y + _nShopFontYOff, COLOR_BLACK, COLOR_WHITE);
	}
}

void CHeadSay::SetEvilLevel(short sMaxEnergy) {
	if (sMaxEnergy < 4) {
		_sEvilLevel = 0;
	}
	else if (sMaxEnergy == 4 || sMaxEnergy == 5) {
		_sEvilLevel = 1;
	}
	else if (sMaxEnergy == 6) {
		_sEvilLevel = 2;
	}
	else if (sMaxEnergy == 7) {
		_sEvilLevel = 3;
	}
	else if (sMaxEnergy >= 8 && sMaxEnergy <= 10) {
		_sEvilLevel = 4;
	}
	else {
		_sEvilLevel = EVIL_MAX;
	}
}

void CHeadSay::SetIsShowEvil(bool bShow) {
	_IsShowEvil = bShow;
}
