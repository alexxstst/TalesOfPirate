#include "Stdafx.h"
#include "Character.h"
#include "SubMap.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "CharTrade.h"
#include "LuaAPI.h"
#include "NPC.h"
#include "WorldEudemon.h"
#include "Player.h"
#include "LevelRecord.h"
#include "SailLvRecord.h"
#include "LifeLvRecord.h"
#include "CharForge.h"
#include "Birthplace.h"
#include "GameDB.h"
#include "lua_gamectrl.h"
#include "MapEntry.h"
#include "CommandMessages.h"
#include "LevelRecordStore.h"
#include "LifeLvRecordStore.h"
#include "SailLvRecordStore.h"

using namespace std;


char	g_chUseItemFailed[2];
char	g_chUseItemGiveMission[2];

_DBC_USING

// maincha
bool CCharacter::Cmd_EnterMap(cChar* l_map, Long lMapCopyNO, uLong l_x, uLong l_y, Char chLogin)
{
	Short	sErrCode = ERR_MC_ENTER_ERROR;
	Char	chEnterType = enumENTER_MAP_CARRY;
	CPlayer	*pCPlayer = GetPlayer();

	m_CKitbag.UnLock();

	Square l_shape;
	m_pCChaRecord = GetChaRecordInfo(GetCat());
	if (m_pCChaRecord == NULL)
		goto Error;

	if (m_CChaAttr.GetAttr(ATTR_HP) <= 0)
		setAttr(ATTR_HP, 1);
	if (m_CChaAttr.GetAttr(ATTR_SP) <= 0)
		setAttr(ATTR_SP, 1);
	{
		bool	bNewCha = true;
		if (m_CChaAttr.GetAttr(ATTR_LV) == 0)
			NewChaInit();
		else
		{
			bNewCha = false;
			m_CChaAttr.Init(GetCat(), false);
		}
		setAttr(ATTR_CHATYPE, enumCHACTRL_PLAYER);
		ResetPosState();

		if (pCPlayer->GetLoginChaType() == enumLOGIN_CHA_BOAT) // 
		{
			CCharacter* pBoat = pCPlayer->GetBoat(pCPlayer->GetLoginChaID());
			if (!pBoat)
			{
				//LG("enter_map", " %sID %u!\n", GetLogName(), pCPlayer->GetLoginChaID());
				ToLogService("map", "character {} use boat(ID {})form logging failed(boat is inexistence),be cut off connect!", GetLogName(), pCPlayer->GetLoginChaID());
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}
			if (!BoatEnterMap(*pBoat, 0, 0, 0))
			{
				pBoat->SetToMainCha();
				//LG("enter_map", " %sID %u!\n", GetLogName(), pCPlayer->GetLoginChaID());
				ToLogService("map", "character {}use boat(ID {})form logging failed(put boat failed),be cut off connect!", GetLogName(), pCPlayer->GetLoginChaID());
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}
		}

		{
			CCharacter* pCCtrlCha = pCPlayer->GetCtrlCha();

			CMapRes* pCMapRes = 0;
			SubMap* pCMap = 0;
			if (bNewCha) // 
			{
				SBirthPoint* pSBirthP = GetRandBirthPoint(GetLogName(), GetBirthCity());
				if (pSBirthP)
				{
					SetBirthMap(pSBirthP->szMapName);
					pCMapRes = g_pGameApp->FindMapByName(pSBirthP->szMapName);
					l_x = (pSBirthP->x + 2 - rand() % 4) * 100;
					l_y = (pSBirthP->y + 2 - rand() % 4) * 100;
				}
			}
			else
			{
				if (chLogin == 0) // 
				{
					if (strcmp(l_map, pCCtrlCha->GetBirthMap()))
					{
						//LG("enter_map", " %s(%s)  %s  %s !\n",
						ToLogService("map", "character {}({})'s aim map {} is not matched to focus character {}be cut off connect!",
							GetLogName(), pCCtrlCha->GetLogName(), l_map, pCCtrlCha->GetBirthMap());
						sErrCode = ERR_MC_ENTER_POS;
						goto Error;
					}

					pCMapRes = g_pGameApp->FindMapByName(pCCtrlCha->GetBirthMap());
					l_x = pCCtrlCha->GetPos().x;
					l_y = pCCtrlCha->GetPos().y;
				}
				else // 
				{
					chEnterType = enumENTER_MAP_EDGE;
					pCMapRes = g_pGameApp->FindMapByName(l_map);
				}

				if (chLogin == 0)
				{
					InitCheatX();
				}
			}

			if (!pCMapRes)
			{
				//LG("enter_map", " %s(%s) !\n", GetLogName(), pCCtrlCha->GetLogName());
				ToLogService("map", "player {}({})'s map name or city name is unlawfulbe cut off connect!", GetLogName(), pCCtrlCha->GetLogName());
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}
			pCMap = pCMapRes->GetCopy((Short)lMapCopyNO);
			if (!pCMap)
			{
				//LG("enter_map", " %s(%s) !\n", GetLogName(), pCCtrlCha->GetLogName());
				ToLogService("map", "character {}({}) copy map ID is unlawfulbe cut off connect!", GetLogName(), pCCtrlCha->GetLogName());
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}
			if (pCMapRes->GetCopyStartType() != enumMAPCOPY_START_CONDITION)
				pCMap->Open();
			pCCtrlCha->SetBirthMap(pCMap->GetName());

			l_shape.centre.x = l_x;
			l_shape.centre.y = l_y;
			l_shape.radius = m_pCChaRecord->sRadii;
			if (!pCMap->EnsurePos(&l_shape, pCCtrlCha))
			{
				ToLogService("map", "character {}({}) 's map coordinate[{}, {}]is unlawful, fallback to birth point", GetLogName(), pCCtrlCha->GetLogName(), l_x, l_y);
				SBirthPoint* pFallback = GetRandBirthPoint(GetLogName(), pCCtrlCha->GetBirthCity());
				if (pFallback)
				{
					l_x = (pFallback->x + 2 - rand() % 4) * 100;
					l_y = (pFallback->y + 2 - rand() % 4) * 100;
					pCMapRes = g_pGameApp->FindMapByName(pFallback->szMapName);
					if (pCMapRes)
					{
						pCMap = pCMapRes->GetCopy(-1);
						if (pCMap)
						{
							if (pCMapRes->GetCopyStartType() != enumMAPCOPY_START_CONDITION)
								pCMap->Open();
							pCCtrlCha->SetBirthMap(pCMap->GetName());
						}
					}
					l_shape.centre.x = l_x;
					l_shape.centre.y = l_y;
					if (!pCMap || !pCMap->EnsurePos(&l_shape, pCCtrlCha))
					{
						ToLogService("map", "character {}({}) birth point fallback also failed [{}, {}]", GetLogName(), pCCtrlCha->GetLogName(), l_x, l_y);
						sErrCode = ERR_MC_ENTER_POS;
						goto Error;
					}
					ToLogService("map", "character {}({}) relocated to birth point [{}, {}] on map {}", GetLogName(), pCCtrlCha->GetLogName(), l_x, l_y, pCMap->GetName());
				}
				else
				{
					ToLogService("map", "character {}({}) no birth point found, disconnecting", GetLogName(), pCCtrlCha->GetLogName());
					sErrCode = ERR_MC_ENTER_POS;
					goto Error;
				}
			}

			pCPlayer->CheckChaItemFinalData();
			for (int i = 0; i < enumACTCONTROL_MAX; i++)
				SetActControl(i);
			m_CSkillState.Reset();
			m_CSkillBag.SetState(-1, enumSUSTATE_INACTIVE);
			pCCtrlCha->SkillRefresh();
			pCPlayer->CloseBank();

			// 
			//bodyCheck = false;
			//gloveCheck = false;
			//shoeCheck = false;
			//headCheck = false;

			for (int i = 0; i < enumEQUIP_NUM; i++)
			{
				if (g_IsRealItemID(m_SChaPart.SLink[i].sID))
					ChangeItem(true, &m_SChaPart.SLink[i], i);

				if (m_SChaPart.SLink[i].sID && (m_SChaPart.SLink[i].expiration - std::time(0)) <= 0 && m_SChaPart.SLink[i].expiration != 0) {
					Short	sUnfixNum = 0;
					if (Cmd_UnfixItem(i, &sUnfixNum, 1, 0, -1, false, true, true) == enumITEMOPT_ERROR_KBFULL) {
						m_SChaPart.SLink[i].sID = 0;
					}
				}

			}

			//bodyCheck = true;
			//gloveCheck = true;
			//shoeCheck = true;
			//headCheck = true;
			// 
			//disabled pet slot
			//CheckEspeItemGrid();

			Strin2SStateData(this, g_strChaState[0]);

			net::msg::McEnterMapMessage msg;
			msg.errCode = 0;
			msg.data.emplace();
			auto &d = msg.data.value();

			char cAutoLock;
			char cLock;
			cAutoLock = m_CKitbag.IsPwdAutoLocked() ? 1 : 0;
			cLock = m_CKitbag.IsPwdLocked() ? 1 : 0;

			if (!chLogin && cAutoLock == 1 && cLock == 0)
			{
				Cmd_LockKitbag();
				cLock = m_CKitbag.IsPwdLocked() ? 1 : 0;
			}

			d.autoLock = cAutoLock;
			d.kitbagLock = cLock;
			d.enterType = chEnterType;
			d.isNewCha = bNewCha ? 1 : 0;
			d.mapName = l_map;
			d.canTeam = pCMapRes->CanTeam() ? 1 : 0;
			d.imp = GetIMP();
			FillBaseInfo(d.baseInfo); // 
			m_CSkillBag.SetChangeFlag();
			FillSkillBag(d.skillBag, enumSYN_SKILLBAG_INIT);
			FillSkillState(d.skillState);

			//
			if (bNewCha)
				g_luaAPI.Call("CreatCha", static_cast<CCharacter*>(this));
			else
				g_luaAPI.Call("AttrRecheck", static_cast<CCharacter*>(this));
			{
				CLevelRecord* pCLvRec = 0, * pNLvRec = 0;
				pCLvRec = GetLevelRecordInfo((long)m_CChaAttr.GetAttr(ATTR_LV));
				pNLvRec = GetLevelRecordInfo((long)m_CChaAttr.GetAttr(ATTR_LV) + 1);
				if (pCLvRec)
				{
					setAttr(ATTR_CLEXP, pCLvRec->ulExp);
					if (pNLvRec)
						setAttr(ATTR_NLEXP, pNLvRec->ulExp);
					else
						setAttr(ATTR_NLEXP, pCLvRec->ulExp);
				}
			}
			// 
			{
				CSailLvRecord* pCLvRec = 0, * pNLvRec = 0;
				pCLvRec = GetSailLvRecordInfo((long)m_CChaAttr.GetAttr(ATTR_SAILLV));
				pNLvRec = GetSailLvRecordInfo((long)m_CChaAttr.GetAttr(ATTR_SAILLV) + 1);
				if (pCLvRec)
				{
					setAttr(ATTR_CLV_SAILEXP, pCLvRec->ulExp);
					if (pNLvRec)
						setAttr(ATTR_NLV_SAILEXP, pNLvRec->ulExp);
					else
						setAttr(ATTR_NLV_SAILEXP, pCLvRec->ulExp);
				}
			}
			// 
			{
				CLifeLvRecord* pCLvRec = 0, * pNLvRec = 0;
				pCLvRec = GetLifeLvRecordInfo((long)m_CChaAttr.GetAttr(ATTR_LIFELV));
				pNLvRec = GetLifeLvRecordInfo((long)m_CChaAttr.GetAttr(ATTR_LIFELV) + 1);
				if (pCLvRec)
				{
					setAttr(ATTR_CLV_LIFEEXP, pCLvRec->ulExp);
					if (pNLvRec)
						setAttr(ATTR_NLV_LIFEEXP, pNLvRec->ulExp);
					else
						setAttr(ATTR_NLV_LIFEEXP, pCLvRec->ulExp);
				}
			}
			m_CChaAttr.SetChangeFlag();
			FillAttr(d.attr, enumATTRSYN_INIT);

			m_CKitbag.SetChangeFlag(true);
			FillKitbag(d.kitbag, m_CKitbag, enumSYN_KITBAG_INIT); // 
			FillShortcut(d.shortcut); // 

			SetBoatAttrChangeFlag();
			pCPlayer->RefreshBoatAttr();
			FillBoats(d.boats);

			d.ctrlChaId = pCCtrlCha->GetID();

			net::WPacket pkret = net::msg::serialize(msg);

			// Trailing-  GateServer (loginFlag, playerCount, playerAddr).
			// GateServer         DiscardLast(6).
			net::msg::McEnterMapTrailer trailer;
			trailer.loginFlag = chLogin;
			trailer.playerCount = g_pGameApp->m_dwPlayerCnt;
			trailer.playerAddr = ToAddress(pCPlayer);
			net::msg::serializeEnterMapTrailer(pkret, trailer);

			ReflectINFof(this, pkret);

			if (bNewCha)
				// 
				OnCharBorn();

			pCMap->Enter(&l_shape, pCCtrlCha); // 
			pCMap->AfterPlyEnterMap(pCCtrlCha);
			pCCtrlCha->SynPKCtrl();

			m_timerPing.Reset();
			SendPreMoveTime();

			ResetStoreTime();

			//LG("enter_map", " %s(%s)\n", GetLogName(), pCCtrlCha->GetLogName());
			ToLogService("map", "finish enter game scene {}({})", GetLogName(), pCCtrlCha->GetLogName());
			return true;
		}
	}
Error:
	{
		net::msg::McEnterMapMessage errMsg;
		errMsg.errCode = sErrCode;
		net::WPacket pkret = net::msg::serialize(errMsg);
		ReflectINFof(this, pkret);
	}

	pCPlayer->SetLoginCha(enumLOGIN_CHA_MAIN, 0);
	pCPlayer->SetCtrlCha(GetPlyMainCha());
	game_db.SavePlayerPos(pCPlayer);
	g_pGameApp->GoOutGame(pCPlayer, true);

	//LG("enter_map", " %s(%s)\n", GetLogName(), GetPlyCtrlCha()->GetLogName());
	ToLogService("map", "enter game scene failed {}({})", GetLogName(), GetPlyCtrlCha()->GetLogName());
	return false;
}

//=============================================================================
// sPing 
//       pPath 
//       chPointNum defMOVE_INFLEXION_NUM
//=============================================================================
void CCharacter::Cmd_BeginMove(Short sPing, Point *pPath, Char chPointNum, Char chStopState)
{
if (!IsLiveing()) {
	return;
}
	if (!GetActControl(enumACTCONTROL_MOVE))
	{
		
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		return;
	}

	if (GetMoveState() == enumMSTATE_ON && GetMoveStopState() == enumEXISTS_SLEEPING) // 
	{
		
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		return;
	}

	if (GetMoveState() == enumMSTATE_ON && GetMoveEndPos() == pPath[chPointNum - 1]) // 
	{
		
		return;
	}

	if (m_CChaAttr.GetAttr(ATTR_MSPD) == 0)
	{
		
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		return;
	}

	for (Char chPCount = 0; chPCount < chPointNum; chPCount++)
		if (!m_submap->IsValidPos(pPath[chPCount].x, pPath[chPCount].y))
		{
			
			FailedActionNoti(enumACTION_MOVE, enumFACTION_MOVEPATH);
			return;
		}

	SMoveInit	MoveInit;
	MoveInit.usPing = sPing;
	memcpy(MoveInit.SInflexionInfo.SList, pPath, sizeof(Point) * chPointNum);
	MoveInit.SInflexionInfo.sNum = chPointNum;
	MoveInit.STargetInfo.chType = 0;
	MoveInit.sStopState = chStopState;

	if (m_CAction.GetCurActionNo() >= 0)
	{
		m_CAction.End();
		m_CAction.Add(enumACTION_MOVE, &MoveInit);
		
	}
	else if (m_CAction.GetActionNum() == 0)
	{
		m_CAction.Add(enumACTION_MOVE, &MoveInit);
		m_CAction.DoNext();
		
	}
	else
	{
		m_CAction.End();
	}
}

//=============================================================================
// 
//=============================================================================
void CCharacter::Cmd_BeginMoveDirect(Entity *pTar)
{
	if (!pTar || !g_pGameApp->IsLiveingEntity(pTar->GetID(), pTar->GetHandle())) // 
	{
		return;
	}
	Point	Path[2] = {GetPos(), pTar->GetPos()};
	Cmd_BeginMove(0, Path, 2);
}

//=============================================================================
// sPing 
//       chFightID 
//       pPath 
//       chPointNum defMOVE_INFLEXION_NUM
//       pSkill 
//       lSkillLv 
//       lTarInfo1 lTarInfo2 
//             pSkillID(GetID()), Handle(GetHandle())
//             lSkillNox,y
//=============================================================================
void CCharacter::Cmd_BeginSkill(Short sPing, Point *pPath, Char chPointNum,
								CSkillRecord *pSkill, Long lSkillLv, Long lTarInfo1, Long lTarInfo2, Char chStopState)
{
	if (!IsLiveing() || !pSkill || !pPath )
		return;

	if (GetMoveState() == enumMSTATE_ON && GetMoveStopState() == enumEXISTS_SLEEPING) // 
	{
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		return;
	}

	if (!GetActControl(enumACTCONTROL_MOVE) && pPath[0] != pPath[1])
	{
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		return;
	}

	if (!IsRightSkill(pSkill))
	{
		FailedActionNoti(enumACTION_SKILL, enumFACTION_SKILL_NOTMATCH);
		return;
	}

	SSkillGrid	*pSSkillCont = m_CSkillBag.GetSkillContByID(pSkill->sID);
	if(!pSSkillCont)
	{
		if (IsBoat())
			pSSkillCont = GetPlayer()->GetMainCha()->m_CSkillBag.GetSkillContByID(pSkill->sID);
		if(!pSSkillCont)
		{
			short sItemID = atoi(pSkill->szDescribeHint.c_str());
			if(sItemID > 10)
			{
				CItemRecord* pItemRec = GetItemRecordInfo( sItemID );
				if(pItemRec)
				{
					CSkillRecord *pSkillRec = GetSkillRecordInfo(pSkill->sID);
					if(pSkillRec && !pSkillRec->IsShow())
					{
						BOOL bRet = GetPlayer()->GetMainCha()->LearnSkill( pSkill->sID, 1, true, false, true );
						//LG("", "%s\t(SkillID: %u)\n", GetLogName(), pSkill->sID);
						ToLogService("common", "character:{}\tstudy Item skill(SkillID: {})", GetLogName(), pSkill->sID);
						if(bRet)
						{
							pSSkillCont = m_CSkillBag.GetSkillContByID(pSkill->sID);
						}
					}
				}
			}
		}
		else
		{
			pSSkillCont = 0;
		}
	}
	if (!pSSkillCont)
	{
		if (IsBoat())
			pSSkillCont = GetPlayer()->GetMainCha()->m_CSkillBag.GetSkillContByID(pSkill->sID);
		if (!pSSkillCont)
		{
			//LG("", "%s\t(SkillID: %u)\n", GetLogName(), pSkill->sID);
			ToLogService("errors", LogLevel::Error, "character:{}\t hasn't the skill(SkillID: {})", GetLogName(), pSkill->sID);
			FailedActionNoti(enumACTION_SKILL, enumFACTION_NOSKILL);
			return;
		}
	}
	CSkillTempData	*pCSkillTData = g_pGameApp->GetSkillTData(pSSkillCont->sID, pSSkillCont->chLv);
	if (!pCSkillTData)
	{
		//LG("", "%s\t(SkillID: %u, SkillLv: %u)\n", GetLogName(), pSSkillCont->sID, pSSkillCont->chLv);
		ToLogService("errors", LogLevel::Error, "character:{}\t hasn't get the skill(SkillID: {}, SkillLv: {})'s temp data", GetLogName(), pSSkillCont->sID, pSSkillCont->chLv);
		FailedActionNoti(enumACTION_SKILL, enumFACTION_NOSKILL);

		return;
	}

	if (pSSkillCont->chState != enumSUSTATE_ACTIVE // 
		|| (pCSkillTData->lResumeTime == 0 && !GetActControl(enumACTCONTROL_USE_GSKILL)) // 
		|| (pCSkillTData->lResumeTime > 0 && !GetActControl(enumACTCONTROL_USE_MSKILL))) // 
	{
		FailedActionNoti(enumACTION_SKILL, enumFACTION_ACTFORBID);

		return;
	}

	for (Char chPCount = 0; chPCount < chPointNum; chPCount++)
		if (!m_submap->IsValidPos(pPath[chPCount].x, pPath[chPCount].y))
		{
			FailedActionNoti(enumACTION_MOVE, enumFACTION_MOVEPATH);

			return;
		}

	SMoveInit	MoveInit;
	SFightInit	FightInit;

	if (SkillTarIsEntity(pSkill)) // ID
	{
		Entity *pTarEnt = g_pGameApp->IsMapEntity(lTarInfo1, lTarInfo2);
		if (!pTarEnt) // 
		{
			FailedActionNoti(enumACTION_SKILL, enumFACTION_NOOBJECT);

			return;
		}
		FightInit.chTarType = 1;
		MoveInit.STargetInfo.ulDist = GetSkillDist(pTarEnt, pSkill);

		if (pSkill == m_SFightInit.pCSkillRecord
			&& GetFightState() == enumFSTATE_ON
			&& IsRangePoint(pTarEnt->GetPos(), MoveInit.STargetInfo.ulDist)) // 
		{

			return;
		}
	}
	else
	{
		FightInit.chTarType = 2;
		MoveInit.STargetInfo.ulDist = GetSkillDist(0, pSkill);

		if (pSkill == m_SFightInit.pCSkillRecord
			&& GetFightState() == enumFSTATE_ON
			&& IsRangePoint(lTarInfo1, lTarInfo2, MoveInit.STargetInfo.ulDist)) // 
		{

			return;
		}
	}

	MoveInit.usPing = sPing;
	memcpy(MoveInit.SInflexionInfo.SList, pPath, sizeof(Point) * chPointNum);
	MoveInit.SInflexionInfo.sNum = chPointNum;
	MoveInit.sStopState = chStopState;

	FightInit.pSSkillGrid = pSSkillCont;
	FightInit.pCSkillRecord = pSkill;
	FightInit.lTarInfo1 = lTarInfo1;
	FightInit.lTarInfo2 = lTarInfo2;
	FightInit.sStopState = chStopState;
	FightInit.pCSkillTData = pCSkillTData;

	MoveInit.STargetInfo.chType = FightInit.chTarType;
	MoveInit.STargetInfo.lInfo1 = FightInit.lTarInfo1;
	MoveInit.STargetInfo.lInfo2 = FightInit.lTarInfo2;

	if (!IsPlayerCha())
	{
		if (SetMoveOnInfo(&MoveInit))
		{
			return;
		}
	}

	Show(); // 

	if (m_CAction.GetCurActionNo() >= 0)
	{
		m_CAction.End();
		m_CAction.Add(enumACTION_MOVE, &MoveInit);
		m_CAction.Add(enumACTION_SKILL, &FightInit);
	}
	else if (m_CAction.GetActionNum() == 0)
	{
		m_CAction.Add(enumACTION_MOVE, &MoveInit);
		m_CAction.Add(enumACTION_SKILL, &FightInit);
		m_CAction.DoNext();
	}
	else
	{
		m_CAction.End();
	}
}

void CCharacter::Cmd_BeginSkillDirect(Long lSkillNo, Entity *pTar, bool bIntelligent)
{
	if (!pTar || !g_pGameApp->IsMapEntity(pTar->GetID(), pTar->GetHandle())) // 
	{
		return;
	}

	CSkillRecord *pSkill = GetSkillRecordInfo(lSkillNo);
	if (pSkill == NULL)
	{
		return;
	}

	Long	lTarInfo1, lTarInfo2;
	Point	Path[2] = {GetPos(), pTar->GetPos()};
	if (bIntelligent)
	{
		CCharacter	*pCTarCha = pTar->IsCharacter();
		if (pCTarCha && pCTarCha->GetMoveState() == enumMSTATE_ON)
			Path[1].move(pCTarCha->GetAngle(), 400);
	}
	if (SkillTarIsEntity(pSkill)) // ID
	{
		lTarInfo1 = pTar->GetID();
		lTarInfo2 = pTar->GetHandle();
	}
	else
	{
		lTarInfo1 = pTar->GetPos().x;
		lTarInfo2 = pTar->GetPos().y;
	}
	Cmd_BeginSkill(0, Path, 2, pSkill, 1, lTarInfo1, lTarInfo2);
}

void CCharacter::Cmd_BeginSkillDirect2(Long lSkillNo, Long lSkillLv, Long lPosX, Long lPosY)
{
	CSkillRecord *pSkill = GetSkillRecordInfo(lSkillNo);
	if (pSkill == NULL)
	{
		return;
	}

	Point	Path[2];
	Path[0] = GetPos();
	Path[1].x = lPosX;
	Path[1].y = lPosY;
	if (SkillTarIsEntity(pSkill)) // ID
	{
		return;
	}

	Cmd_BeginSkill(0, Path, 2, pSkill, lSkillLv, lPosX, lPosY);
}

//=============================================================================
// 
//=============================================================================
Short CCharacter::Cmd_UseItem(Short sSrcKbPage, Short sSrcKbGrid, Short sTarKbPage, Short sTarKbGrid)
{
	if (m_CKitbag.IsLock()) // 
		return enumITEMOPT_ERROR_KBLOCK;
    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //
        return enumITEMOPT_ERROR_KBLOCK;
	//add by ALLEN 2007-10-16
    if (GetPlyMainCha()->IsReadBook()) //
        return enumITEMOPT_ERROR_KBLOCK;
   
    if(GetPlyMainCha()->GetStallData())     //  BUG, 
    {
        return enumITEMOPT_ERROR_KBLOCK;
    }

	SItemGrid	*pSGridCont = m_CKitbag.GetGridContByID(sSrcKbGrid);

	

	Short	sItemID = m_CKitbag.GetID(sSrcKbGrid, sSrcKbPage);
	if (sItemID <= 0)
		return enumITEMOPT_ERROR_NONE;
	CItemRecord	*pCItemRec = GetItemRecordInfo(sItemID);
	if (!pCItemRec)
		return enumITEMOPT_ERROR_NONE;

	if (IsItemExpired(pSGridCont) == enumITEMOPT_ERROR_EXPIRATION) {
		return enumITEMOPT_ERROR_EXPIRATION;
	}

	/*if (pSGridCont->dwDBID != 0){
		return enumITEMOPT_ERROR_KBLOCK;
	}*/

	Short	sUseRet;
	if (pCItemRec->szAbleLink[0] == -1) // 
		sUseRet = Cmd_UseExpendItem(sSrcKbPage, sSrcKbGrid, sTarKbPage, sTarKbGrid);
	else // 
		sUseRet = Cmd_UseEquipItem(sSrcKbPage, sSrcKbGrid, true, sTarKbGrid==-2);

	if (sUseRet == enumITEMOPT_SUCCESS && pCItemRec->IsSendUseItem())
		SynItemUseSuc(sItemID);

	return sUseRet;
}

//=============================================================================
// 
//=============================================================================
Short CCharacter::Cmd_UseEquipItem(Short sKbPage, Short sKbGrid, bool bRefresh, bool rightHand)
{
	if (!GetActControl(enumACTCONTROL_ITEM_OPT))
		return enumITEMOPT_ERROR_STATE;

	if (bRefresh)
	{
		m_CChaAttr.ResetChangeFlag();
		SetBoatAttrChangeFlag(false);
		m_CSkillState.ResetChangeFlag();
		SetLookChangeFlag();
		m_CKitbag.SetChangeFlag(false, sKbPage);
	}

	SItemGrid	*pSEquipIt = m_CKitbag.GetGridContByID(sKbGrid, sKbPage);
	if (!pSEquipIt)
		return enumITEMOPT_ERROR_NONE;
	//if (!pSEquipIt->IsValid())
	//	return enumITEMOPT_ERROR_INVALID;
	Short	sItemId = pSEquipIt->sID;
	Short	sItemNum = pSEquipIt->sNum;

	CItemRecord	*pCItemRec = GetItemRecordInfo(sItemId);
	if (!pCItemRec)
	{
		return enumITEMOPT_ERROR_NONE;
	}

	// Modify by ning.yan 20080821  Begin
	//if( sItemId >= 5000 && pCItemRec->sType != enumItemTypeBoat && pSEquipIt->GetFusionItemID() )
	CItemRecord * pItem = GetItemRecordInfo(sItemId);
	if(CItemRecord::IsVaildFusionID(pItem) && pCItemRec->sType != enumItemTypeBoat && pSEquipIt->GetFusionItemID() ) // ning.yan  end
	{
		Short sEquipCon = CanEquipItemNew( sItemId, (Short)pSEquipIt->GetFusionItemID() );
		Short expired = IsItemExpired(pSEquipIt);
		
		if (sEquipCon != enumITEMOPT_SUCCESS)
		{
			return sEquipCon;
		}
		else if (expired != enumITEMOPT_SUCCESS)
		{
			return expired;
		}
	}
	else
	{
		//Short sEquipCon = CanEquipItem(sItemId);
		Short sEquipCon = CanEquipItem(pSEquipIt);
		Short expired = IsItemExpired(pSEquipIt);
		if (sEquipCon != enumITEMOPT_SUCCESS)
		{
			return sEquipCon;
		}
		else if (expired != enumITEMOPT_SUCCESS)
		{
			return expired;
		}
	}

	bool bOccupied = true;

	Char	chEquipPos = -1;
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		if (pCItemRec->szAbleLink[i] == cchItemRecordKeyValue)
			break;
		if (m_SChaPart.SLink[pCItemRec->szAbleLink[i]].sID == 0)
		{
			chEquipPos = pCItemRec->szAbleLink[i];
			break;
		}
	}

	if (chEquipPos == -1) {
		chEquipPos = pCItemRec->szAbleLink[0];
		bOccupied = false;
	}

	Short	sUnfixRet = enumITEMOPT_ERROR_NONE;

	if (rightHand == true){
		int slot = -1;
		switch (pCItemRec->sType){
			case 1:{
				slot = 6;
				break;
			}
			case 26:{
				slot = 8;
				break;
			}
		}
		for (int i = 0; i < enumEQUIP_NUM; i++){
			if (pCItemRec->szAbleLink[i] == slot){
				chEquipPos = slot;
				break;
			}
		}
	}
	
	if (!bOccupied)			// Check inventory capacity only if equipped slot isn't occupied
	{
		short sFreeNum = m_CKitbag.GetCapacity() - m_CKitbag.GetUseGridNum();
		if (sFreeNum < 1)	// Inventory check
			return enumITEMOPT_ERROR_KBFULL;
	}

	SItemGrid SGridCont;
    if (KbPopItem(false, false, &SGridCont, sKbGrid, sKbPage) != enumKBACT_SUCCESS) 
		return enumITEMOPT_ERROR_NONE;

	if (pCItemRec->szNeedLink[0] == -1)
	{
		Short	sUnfixNum = 0;
		sUnfixRet = Cmd_UnfixItem(chEquipPos, &sUnfixNum, 1, 0, -1, false, false);
	}
	else
	{
		char	chNeedL;
		Short	sUnfixNum = 0;
		for (int i = 0; i < enumEQUIP_NUM; i++)
		{
			chNeedL = pCItemRec->szNeedLink[i];
			if (chNeedL == cchItemRecordKeyValue)
				break;
			sUnfixRet = Cmd_UnfixItem(chNeedL, &sUnfixNum, 1, 0, -1, false, false);
			if (sUnfixRet != enumITEMOPT_SUCCESS)
				break;
		}
	}
	if (sUnfixRet == enumITEMOPT_SUCCESS)
	{		
		if (pCItemRec->szNeedLink[0] != -1)
		{
			Short	sVal;
			if (chEquipPos == enumEQUIP_LHAND || chEquipPos == enumEQUIP_RHAND)
				sVal = enumEQUIP_BOTH_HAND;
			else if (chEquipPos == enumEQUIP_BODY)
				sVal = enumEQUIP_TOTEM;
			char	chNeedL;
			for (int i = 0; i < enumEQUIP_NUM; i++)
			{
				chNeedL = pCItemRec->szNeedLink[i];
				if (chNeedL == cchItemRecordKeyValue)
					break;
				m_SChaPart.SLink[chNeedL].sID = sVal;
				m_SChaPart.SLink[chNeedL].SetChange();
			}
		}
		m_SChaPart.SLink[chEquipPos] = SGridCont;
		m_SChaPart.SLink[chEquipPos].SetChange();
		ChangeItem(true, &SGridCont, chEquipPos);
	} else {
		// Add item back if equip fails.
		KbPushItem(true, false, &SGridCont, sKbGrid);
	}

	if (bRefresh)
	{
		GetPlyMainCha()->m_CSkillBag.SetChangeFlag(false);
		GetPlyCtrlCha()->SkillRefresh();
		GetPlyMainCha()->SynSkillBag(enumSYN_SKILLBAG_MODI);

		SynSkillStateToEyeshot();

		if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
			SynLook(LOOK_SELF, true); // sync to self
		else
			SynLook();

		SynKitbagNew(enumSYN_KITBAG_EQUIP);

		g_luaAPI.Call("AttrRecheck", static_cast<CCharacter*>(this));
		if (GetPlayer())
		{
			GetPlayer()->RefreshBoatAttr();
			SyncBoatAttr(enumATTRSYN_ITEM_EQUIP);
		}
		SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);
	}

	AfterEquipItem(sItemId, 0);

	return sUnfixRet;
}

//=============================================================================
// 
//=============================================================================
Short CCharacter::Cmd_UseExpendItem(Short sKbPage, Short sKbGrid, Short sTarKbPage, Short sTarKbGrid, bool bRefresh)
{
	static DWORD dwLastTime = GetTickCount();
	DWORD dwCurTime = GetTickCount();
	if( dwCurTime - dwLastTime < 200 )
	{
		return enumITEMOPT_SUCCESS;
	}
	dwLastTime = dwCurTime;

	if (!GetActControl(enumACTCONTROL_ITEM_OPT))
		return enumITEMOPT_ERROR_STATE;

	if (!GetActControl(enumACTCONTROL_USE_ITEM))
		return enumITEMOPT_ERROR_STATE;

	SItemGrid	*pSGridCont = m_CKitbag.GetGridContByID(sKbGrid, sKbPage);
	if (!pSGridCont)
		return enumITEMOPT_ERROR_NONE;

	SItemGrid	*pSTarGridCont = m_CKitbag.GetGridContByID(sTarKbGrid, sTarKbPage);

	if (pSTarGridCont && pSTarGridCont->dwDBID != 0){
		SystemNotice("Target item is locked.");
		return enumITEMOPT_ERROR_NONE;
	}

	CItemRecord* pCItemRec = GetItemRecordInfo(pSGridCont->sID);
	if (!pCItemRec)
		return enumITEMOPT_ERROR_NONE;
	if (pCItemRec->sType == enumItemTypeScroll)
		if (GetPlyCtrlCha()->IsBoat()) // 
			return enumITEMOPT_ERROR_UNUSE;

	//	2007-11-22 add begin!
	//	.
	//if(	pCItemRec
	//	&&	pCItemRec->chPickTo
	//	&&	GetPlyCtrlCha()->IsBoat()
	//	)
	//	return	enumITEMOPT_ERROR_UNUSE;
	//	2007-11-22 add end!


	// 
	bool	bUseSuccess;
	if (pCItemRec->szAttrEffect == "0")
	{
		bUseSuccess = false;
	}
	else
	{
		if (bRefresh)
		{
			m_CChaAttr.ResetChangeFlag();
			SetBoatAttrChangeFlag(false);
			m_CSkillState.ResetChangeFlag();
			m_CKitbag.SetChangeFlag(false, sKbPage);
		}

		g_chUseItemFailed[0] = 0;
		
		// Add by lark.li 20080721 begin
		g_chUseItemGiveMission[0] = 0;
		// End

		g_luaAPI.Call(pCItemRec->szAttrEffect.c_str(), static_cast<CCharacter*>(this), pSGridCont, pSTarGridCont);
		
		if (g_chUseItemFailed[0] == 1) // 
			bUseSuccess = false;
		else
			bUseSuccess = true;

		// Add by lark.li 20080721 begin
		// 
		if (g_chUseItemGiveMission[0] == 1)
		{
			return enumITEMOPT_SUCCESS;
		}
		// End
	}

	if (!bUseSuccess)
	{
		//ColourNotice(0xBC0000, "Failed to use %s", pCItemRec->szName);
		return enumITEMOPT_ERROR_UNUSE;
	}

	if (pCItemRec->sType == enumItemTypeScroll)
		return enumITEMOPT_SUCCESS;

	SItemGrid	SGridCont;
	SGridCont.sNum = 1;
	if (KbPopItem(false, false, &SGridCont, sKbGrid, sKbPage) != enumKBACT_SUCCESS) // 
		return enumITEMOPT_ERROR_NONE;

	// 
	RefreshNeedItem( SGridCont.sID );

	char	szPlyName[100];
	if (IsBoat())
		sprintf(szPlyName, "%s%s%s%s", GetName(), "(", GetPlyMainCha()->GetName(), ")");
	else
		sprintf(szPlyName, "%s", GetName());
	char	szMsg[128];
	sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00001), pCItemRec->szName.c_str(), SGridCont.sID, SGridCont.sNum);
	ToLogService("trade", "[CHA_EXPEND] {} : {}", szPlyName, szMsg);

	if (bRefresh)
	{
		// 
		SynSkillStateToEyeshot();

		// 
		SynKitbagNew(enumSYN_KITBAG_SYSTEM);

		// 
		g_luaAPI.Call("AttrRecheck", static_cast<CCharacter*>(this));
		if (GetPlayer())
		{
			GetPlayer()->RefreshBoatAttr();
			SyncBoatAttr(enumATTRSYN_ITEM_MEDICINE);
		}
		SynAttrToSelf(enumATTRSYN_ITEM_MEDICINE);
	}

	return enumITEMOPT_SUCCESS;
}

//=============================================================================
// 
// chLinkID 
// psItemNum 0.
// chDir 0 [lParam1lParam2]xy
// 1 [lParam1lParam2]
// 2 
//=============================================================================
Short CCharacter::Cmd_UnfixItem(Char chLinkID, Short *psItemNum, Char chDir, Long lParam1, Long lParam2, bool bPriority, bool bRefresh, bool bForcible)
{
	//mothannakh cooldown	//this cooldown needed since the spam of this packet crash all players clients 
	//DWORD dwLastTime = GetTickCount();	//static 
	//if (GetPlyMainCha()->SwitchItemColD > dwLastTime)
	//{
	//	BickerNotice("Please Calm Down Don't Spam! ");
	//	return enumITEMOPT_ERROR_PROTECT;	//return false 
	//}
	//cooldown end end
	//fix if invnetory full delete the switch item @mothannakh
	if (GetPlyMainCha()->m_CKitbag.IsFull())
	{
		BickerNotice("Please Make Sure You Have 5 slots Empty ! ");
		return enumITEMOPT_ERROR_KBFULL;
	}
	
	//fix end
	if (!bForcible)
	{
		if (!GetActControl(enumACTCONTROL_ITEM_OPT))
			return enumITEMOPT_ERROR_STATE;
		if (m_CKitbag.IsLock()) // 
			return enumITEMOPT_ERROR_KBLOCK;
	}
	if (chLinkID < 0 || chLinkID >= enumEQUIP_NUM)
		return enumITEMOPT_ERROR_NONE;

	if (m_SChaPart.SLink[chLinkID].sID == 0)
		return enumITEMOPT_SUCCESS;
	else if (m_SChaPart.SLink[chLinkID].sID == enumEQUIP_BOTH_HAND) // 
	{
		if (chLinkID == enumEQUIP_LHAND) // 
			chLinkID = enumEQUIP_RHAND;
		else // 
			chLinkID = enumEQUIP_LHAND;
	}
	else if (m_SChaPart.SLink[chLinkID].sID == enumEQUIP_TOTEM) // 
	{
		chLinkID = enumEQUIP_BODY;
	}

	Short	sItemID = m_SChaPart.SLink[chLinkID].sID;
	CItemRecord	*pCItemRec = GetItemRecordInfo(sItemID);
	if (!pCItemRec)
		return enumITEMOPT_ERROR_NONE;

	if (bPriority)// 
	{
		char	chAbleL;
		for (int i = 0; i < enumEQUIP_NUM; i++)
		{
			chAbleL = pCItemRec->szAbleLink[i];
			if (chAbleL == cchItemRecordKeyValue)
				break;
			if (m_SChaPart.SLink[chAbleL].sID != 0)
				chLinkID = chAbleL;
		}
	}

	bool	bOptKb = chDir == 1 ? true : false;
	if (bRefresh)
	{
		m_CChaAttr.ResetChangeFlag();
		SetBoatAttrChangeFlag(false);
		m_CSkillState.ResetChangeFlag();
		SetLookChangeFlag();
		if (bOptKb)
			m_CKitbag.SetChangeFlag(false, (Short)lParam1);
	}

	if (*psItemNum == 0 || (*psItemNum != 0 && *psItemNum > m_SChaPart.SLink[chLinkID].sNum))
		*psItemNum = m_SChaPart.SLink[chLinkID].sNum;
	SItemGrid	SUnfixCont = m_SChaPart.SLink[chLinkID];
	SUnfixCont.sNum = *psItemNum;

	CCharacter	*pCCtrlCha = GetPlyCtrlCha(), *pCMainCha = GetPlyMainCha();
    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //,
        return enumITEMOPT_ERROR_KBLOCK;
	//add by ALLEN 2007-10-16
		if (GetPlyMainCha()->IsReadBook()) //,
        return enumITEMOPT_ERROR_KBLOCK;
	if (chDir == 1) // 
	{
		Short sKbPushRet = KbPushItem(false, false, &SUnfixCont, (Short&)lParam2, (Short)lParam1);
		if (sKbPushRet == enumKBACT_ERROR_FULL) // 
		{
			return enumITEMOPT_ERROR_KBFULL;
			//chDir = 0;
			//pCCtrlCha->GetTrowItemPos(&lParam1, &lParam2);
		}
		else if (sKbPushRet != enumKBACT_SUCCESS)
			return enumITEMOPT_ERROR_NONE;
	}
	if (chDir == 0) // 
	{
		pCItemRec = GetItemRecordInfo(SUnfixCont.sID);
		if (!pCItemRec)
			return enumITEMOPT_ERROR_NONE;
		if(pCItemRec->chIsThrow != 1 || !SUnfixCont.GetInstAttr(ITEMATTR_TRADABLE) ) // 
			return enumITEMOPT_ERROR_UNTHROW;

		if(	SUnfixCont.dwDBID	)
		{
			return	enumITEMOPT_ERROR_UNTHROW;
		};

		SubMap	*pCMap = pCCtrlCha->GetSubMapFar();
		if (!pCMap)
			return enumITEMOPT_ERROR_KBLOCK;
		pCMap->ItemSpawn(&SUnfixCont, lParam1, lParam2, enumITEM_APPE_THROW, pCCtrlCha->GetID(), pCMainCha->GetID(), pCMainCha->GetHandle());

		char	szPlyName[100];
		if (IsBoat())
			sprintf(szPlyName, "%s%s%s%s", GetName(), "(", GetPlyMainCha()->GetName(), ")");
		else
			sprintf(szPlyName, "%s", GetName());
		char	szMsg[128];
		sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00002), pCItemRec->szName.c_str(), SUnfixCont.sID, *psItemNum);
		ToLogService("trade", "[CHA_SYS] {} : {}", szPlyName, szMsg);
	}
	else // 
	{
	}

	if (*psItemNum == m_SChaPart.SLink[chLinkID].sNum) // 
	{
		ChangeItem(0, &m_SChaPart.SLink[chLinkID], chLinkID);

		// Link
		if (pCItemRec->szNeedLink[0] == -1)
		{
			m_SChaPart.SLink[chLinkID].sID = 0;
			m_SChaPart.SLink[chLinkID].SetChange();
		}
		else
		{
			char	chNeedL;
			for (int i = 0; i < enumEQUIP_NUM; i++)
			{
				chNeedL = pCItemRec->szNeedLink[i];
				if (chNeedL == cchItemRecordKeyValue)
					break;
				m_SChaPart.SLink[chNeedL].sID = 0;
				m_SChaPart.SLink[chNeedL].SetChange();
			}
		}
	}
	else
	{
		m_SChaPart.SLink[chLinkID].sNum -= *psItemNum;
		m_SChaPart.SLink[chLinkID].SetChange();
	}

	if (bRefresh)
	{
		GetPlyMainCha()->m_CSkillBag.SetChangeFlag(false);
		GetPlyCtrlCha()->SkillRefresh(); // 
		GetPlyMainCha()->SynSkillBag(enumSYN_SKILLBAG_MODI);

		// 
		SynSkillStateToEyeshot();

		// 
		if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
			SynLook(LOOK_SELF, true); // sync to self
		else
			SynLook();

		// 
		g_luaAPI.Call("AttrRecheck", static_cast<CCharacter*>(this));
		if (GetPlayer())
		{
			GetPlayer()->RefreshBoatAttr();
			SyncBoatAttr(enumATTRSYN_ITEM_EQUIP);
		}
		SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);

		// 
		if (bOptKb)
			SynKitbagNew(enumSYN_KITBAG_UNFIX);
	}
	//if all went fine add our cooldown
		//GetPlyMainCha()->SwitchItemColD = dwLastTime + 100;
		game_db.SavePlayer(GetPlayer(), enumSAVE_TYPE_TIMER);
	//cooldown end 
	return enumITEMOPT_SUCCESS;
}

//=================================================================================================
// 
// WorldIDHandle
#include "item.h"
//=================================================================================================
Short CCharacter::Cmd_PickupItem(uLong ulID, Long lHandle)
{
	if (!GetActControl(enumACTCONTROL_ITEM_OPT))
		return enumITEMOPT_ERROR_STATE;

	Entity	*pCEnt = g_pGameApp->IsLiveingEntity(ulID, lHandle);
	if (!pCEnt)
		return enumITEMOPT_ERROR_NONE;
	CItem	*pCItem = pCEnt->IsItem();
	if (!pCItem)
		return enumITEMOPT_ERROR_NONE;
	CItemRecord* pItem = GetItemRecordInfo( pCItem->m_SGridContent.sID );
	if( pItem == NULL )
		return enumITEMOPT_ERROR_NONE;

	if(pItem->chIsPick != 1) // 
		return enumITEMOPT_ERROR_UNPICKUP;

	CCharacter	*pCCtrlCha = GetPlyCtrlCha(), *pCMainCha = GetPlyMainCha();
	// 
	SubMap	*pCMap = pCCtrlCha->GetSubMapFar();
	if (!pCMap)
		return enumITEMOPT_ERROR_KBLOCK;
	uShort	usAreaAttr = pCMap->GetAreaAttr(pCEnt->GetPos());
	if ((g_IsLand(usAreaAttr) != g_IsLand(GetAreaAttr())) && (g_IsLand(usAreaAttr))) //
		return enumITEMOPT_ERROR_AREA;

	CCharacter	*pKitbagCha = this;
	CKitbag	*pCKitbag = &m_CKitbag;
	if (!g_IsLand(usAreaAttr)) // 
	{
		if (!IsBoat())
			return enumITEMOPT_ERROR_DISTANCE;
		if (pItem->chPickTo == enumITEM_PICKTO_KITBAG) // 
		{
			pKitbagCha = GetPlayer()->GetMainCha();
			pCKitbag = &pKitbagCha->m_CKitbag;
		}
	}

	if (pCKitbag->IsLock())
		return enumITEMOPT_ERROR_KBLOCK;

    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //
        return enumITEMOPT_ERROR_KBLOCK;

	//add by ALLEN 2007-10-16
	if (GetPlyMainCha()->IsReadBook()) //
        return enumITEMOPT_ERROR_KBLOCK;

	if (pCItem->GetProtChaID() != 0) // 
	{
		if (pCItem->GetProtChaID() != pCMainCha->GetID())
		{
			if (pCItem->GetProtType() == enumITEM_PROT_OWN)
				return enumITEMOPT_ERROR_PROTECT;

			Entity	*pBelongEnt = g_pGameApp->IsLifeEntity(pCItem->GetProtChaID(), pCItem->GetProtChaHandle());
			if (pBelongEnt)
			{
				CPlayer	*pBelongPlayer = pBelongEnt->IsCharacter()->GetPlayer();
				if (pBelongPlayer && (!pBelongPlayer->getTeamLeaderID() || pBelongPlayer->getTeamLeaderID() != GetPlayer()->getTeamLeaderID()))
				{
					return enumITEMOPT_ERROR_PROTECT;
				}
			}
		}
	}

	// 
	if( pItem->sType == enumItemTypeBoat )
	{
		if( GetPlayer()->IsBoatFull() )
		{
			//SystemNotice( "%s!", pItem->szName );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00003), pItem->szName.c_str() );
			return enumITEMOPT_ERROR_UNUSE;
		}
	}

	if (!IsRangePoint(pCEnt->GetPos(), defPICKUP_DISTANCE))
		return enumITEMOPT_ERROR_DISTANCE;

	pCKitbag->SetChangeFlag(false, 0);
	Short	sPickupNum = pCItem->m_SGridContent.sNum;
	Short sPushPos = defKITBAG_DEFPUSH_POS;

	Short sPushRet = pKitbagCha->KbPushItem(true, true, &pCItem->m_SGridContent, sPushPos);
	if (sPushRet != enumKBACT_SUCCESS)
	{
		if (sPushRet == enumKBACT_ERROR_FULL)
		{
			sPickupNum -= pCItem->m_SGridContent.sNum;
			if (sPickupNum == 0)
			{
				//ColourNotice(0xBC0000, "Unable to pick up %s", pItem->szName);
				return enumITEMOPT_ERROR_KBFULL;
			}
		}
		else
			return enumITEMOPT_ERROR_NONE;
	}

	// 
	if( pItem->sType == enumItemTypeBoat )
	{
		DWORD dwBoatID = pCItem->m_SGridContent.GetDBParam( enumITEMDBP_INST_ID );
		// 
		if (SaveAssets())
		{
			if( !game_db.SaveBoatTempData( dwBoatID, this->GetPlayer()->GetDBChaId() ) )
			{
				//LG( "", "%sID[0x%X]!ID[0x%X]", 
				ToLogService("errors", LogLevel::Error, "character{}ID[0x{:X}]pick up captain provebut boat data storage failed!boat data ID[0x{:X}]", 
					this->GetName(), this->GetPlayer()->GetDBChaId(), dwBoatID );
			}
		}
		else
		{
			//LG( "", "%sID[0x%X]!ID[0x%X]", 
			ToLogService("errors", LogLevel::Error, "character{}ID[0x{:X}]pick up captain provebut kitbag data storage failed!boat data ID[0x{:X}]", 
				this->GetName(), this->GetPlayer()->GetDBChaId(), dwBoatID );
		}

		if( !BoatAdd( dwBoatID ) )
		{
			//SystemNotice( "!ID[0x%X]", dwBoatID );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00004), dwBoatID );
			//LG( "", "%sID[0x%X]!ID[0x%X]", 
			ToLogService("errors", LogLevel::Error, "character{}ID[0x{:X}]pick up captain proveadd boat failed!boat dataID[0x{:X}]", 
				this->GetName(), this->GetPlayer()->GetDBChaId(), dwBoatID );
		}
	}

	if (sPickupNum > 0)
		AfterPeekItem(pCItem->m_SGridContent.sID, sPickupNum);

	char	szPlyName[100];
	if (IsBoat())
		sprintf(szPlyName, "%s%s%s%s", GetName(), "(", GetPlyMainCha()->GetName(), ")");
	else
		sprintf(szPlyName, "%s", GetName());
	char	szMsg[128];
	sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00005), pItem->szName.c_str(), pCItem->m_SGridContent.sID, sPickupNum);
	ToLogService("trade", "[SYS_CHA] {} : {}", szPlyName, szMsg);

	//ColourNotice(0xb5eb8e, "Picked up x%d %s", sPickupNum, pItem->szName);

	//
	char szTeamMsg[128];

	auto& pickupFmt = LanguageRecordStore::Instance()->GetKeyString("GM_CHARACTERCMD_CPP_00006");
	snprintf(szTeamMsg, sizeof(szTeamMsg), "%s",
		SafeVFormat(pickupFmt, szPlyName, sPickupNum, pItem->szName.c_str()).c_str());

	//  :      
	auto WtPk = net::msg::serialize(net::msg::McSysInfoMessage{ szTeamMsg });
	SENDTOCLIENT2(WtPk, GetPlayer()->GetTeamMemberCnt(), GetPlayer()->_Team);

	if (sPushRet == enumKBACT_SUCCESS)
		pCItem->Free();

	pKitbagCha->SynKitbagNew(enumSYN_KITBAG_PICK);

	pKitbagCha->LogAssets(enumLASSETS_PICKUP);

	return enumITEMOPT_SUCCESS;
}

//(sSrcGrid:   sSrcNum:   sTarGrid:)
Short CCharacter::Cmd_DragItem(Short sSrcGrid, Short sSrcNum, Short sTarGrid)
{
    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //
        return enumITEMOPT_ERROR_KBLOCK;

	//add by ALLEN 2007-10-16
	if (GetPlyMainCha()->IsReadBook()) //
        return enumITEMOPT_ERROR_KBLOCK;

    USHORT sItemID = m_pCKitbagTmp->GetID(sSrcGrid, 0);
    if (sItemID <= 0)
		return enumITEMOPT_ERROR_NONE;
    CItemRecord* pItem = GetItemRecordInfo( sItemID );
	if( pItem == NULL )
		return enumITEMOPT_ERROR_NONE;

    SItemGrid Grid;
    Grid.sNum = sSrcNum;
    m_pCKitbagTmp->Pop(&Grid, sSrcGrid);

    m_CKitbag.Push(&Grid, sTarGrid);
    SynKitbagNew(enumSYN_KITBAG_SWITCH);
    if(Grid.sNum > 0)
    {
       // SystemNotice("!");
		 SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00007));
        m_pCKitbagTmp->Push(&Grid, sSrcGrid);
        SynKitbagTmpNew(enumSYN_KITBAG_SWITCH);
        return enumITEMOPT_ERROR_KBFULL;
    }

    SynKitbagTmpNew(enumSYN_KITBAG_SWITCH);
    
    return enumITEMOPT_SUCCESS;
}

// 
// psThrowNum 0
Short CCharacter::Cmd_ThrowItem(Short sKbPage, Short sKbGrid, Short *psThrowNum, Long lPosX, Long lPosY, bool bRefresh, bool bForcible)
{
    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //
        return enumITEMOPT_ERROR_KBLOCK;

	//add by ALLEN 2007-10-16
    if (GetPlyMainCha()->IsReadBook()) //
        return enumITEMOPT_ERROR_KBLOCK;

	if (!bForcible)
	{
		if (!GetActControl(enumACTCONTROL_ITEM_OPT))
			return enumITEMOPT_ERROR_STATE;
		if (m_CKitbag.IsLock()) // 
			return enumITEMOPT_ERROR_KBLOCK;
	}

	Point	STarP = {lPosX, lPosY};
	if (!IsRangePoint(STarP, defTHROW_DISTANCE))
		return enumITEMOPT_ERROR_DISTANCE;

	USHORT sItemID = m_CKitbag.GetID(sKbGrid, sKbPage);
	CItemRecord* pItem = GetItemRecordInfo( sItemID );
	if( pItem == NULL )
		return enumITEMOPT_ERROR_NONE;

    //  
    if(IsBoat())
    {
        if(enumITEM_PICKTO_KITBAG == pItem->chPickTo)
        {
            return enumITEMOPT_ERROR_UNTHROW;
        }
    }

	if(pItem->chIsThrow != 1 || !m_CKitbag.GetGridContByID(sKbGrid)->GetInstAttr(ITEMATTR_TRADABLE)) // 
		return enumITEMOPT_ERROR_UNTHROW;

	SItemGrid*	grid	=	m_CKitbag.GetGridContByID(	sKbGrid		);
	if(	grid	&&	grid->dwDBID	)
	{
		return	enumITEMOPT_ERROR_UNTHROW;
	};

	// 
	if( pItem->sType == enumItemTypeBoat )
	{
		DWORD dwBoatID = m_CKitbag.GetDBParam( enumITEMDBP_INST_ID, sKbGrid ); 
		CCharacter* pBoat = this->GetPlayer()->GetBoat( dwBoatID );
		if( pBoat )
		{
			game_db.SaveBoat( *pBoat, enumSAVE_TYPE_TIMER );
		}

		if( !BoatClear( dwBoatID ) )
		{
			//SystemNotice( "%s!", pItem->szName );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00008), pItem->szName.c_str() );
			return enumITEMOPT_ERROR_UNUSE;
		}
	}

	CCharacter	*pCCtrlCha = GetPlyCtrlCha(), *pCMainCha = GetPlyMainCha();
	SubMap	*pCMap = pCCtrlCha->GetSubMapFar();
	if (!pCMap)
		return enumITEMOPT_ERROR_KBLOCK;
	uShort	usAreaAttr = pCMap->GetAreaAttr(lPosX, lPosY);
	if (g_IsLand(usAreaAttr) != g_IsLand(GetAreaAttr())) //
		return enumITEMOPT_ERROR_AREA;

	if (*psThrowNum != 0)
	{
		Short	sCurNum = m_CKitbag.GetNum(sKbGrid, sKbPage);
		if (sCurNum <= 0)
			return enumITEMOPT_ERROR_NONE;
		if (*psThrowNum > sCurNum)
			*psThrowNum = sCurNum;
	}

	if (bRefresh)
		m_CKitbag.SetChangeFlag(false, sKbPage);
	SItemGrid GridCont;
	GridCont.sNum = *psThrowNum;
	Short sRet = KbPopItem(bRefresh, bRefresh, &GridCont, sKbGrid, sKbPage); // 
	if( sRet != enumKBACT_SUCCESS )
		return enumITEMOPT_ERROR_NONE;
	*psThrowNum = GridCont.sNum;

	// 
	RefreshNeedItem( sItemID );

	// 
	if (bRefresh)
		SynKitbagNew(enumSYN_KITBAG_THROW);

	char	szPlyName[100];
	if (IsBoat())
		sprintf(szPlyName, "%s%s%s%s", GetName(), "(", GetPlyMainCha()->GetName(), ")");
	else
		sprintf(szPlyName, "%s", GetName());
	char	szMsg[128];
	sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00009), pItem->szName.c_str(), GridCont.sID, GridCont.sNum);
	ToLogService("trade", "[CHA_SYS] {} : {}", szPlyName, szMsg);

	pCMap->ItemSpawn(&GridCont, lPosX, lPosY, enumITEM_APPE_THROW, pCCtrlCha->GetID(), pCMainCha->GetID(), pCMainCha->GetHandle(), 10 * 1000); // 10

	LogAssets(enumLASSETS_THROW);

	return enumITEMOPT_SUCCESS;
}

// .
Short CCharacter::Cmd_ItemSwitchPos(Short sKbPage, Short sSrcGrid, Short sSrcNum, Short sTarGrid)
{
	if (!GetActControl(enumACTCONTROL_ITEM_OPT))
		return enumITEMOPT_ERROR_STATE;

	if (m_CKitbag.IsLock()) // 
		return enumITEMOPT_ERROR_KBLOCK;

    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //
        return enumITEMOPT_ERROR_KBLOCK;

	//add by ALLEN 2007-10-16
	if (GetPlyMainCha()->IsReadBook()) //
        return enumITEMOPT_ERROR_KBLOCK;

	m_CKitbag.SetChangeFlag(false, sKbPage);
	Short	sKbOptRet = KbRegroupItem(true, true, sSrcGrid, sSrcNum, sTarGrid);
	if (sKbOptRet == enumKBACT_SUCCESS || sKbOptRet == enumKBACT_ERROR_FULL) // 
		SynKitbagNew(enumSYN_KITBAG_SWITCH);
	else if (sKbOptRet == enumKBACT_ERROR_RANGE)
		return enumITEMOPT_ERROR_KBRANGE;
	else
		return enumITEMOPT_ERROR_NONE;

	return enumITEMOPT_SUCCESS;
}

// 
// psThrowNum 0
Short CCharacter::Cmd_DelItem(Short sKbPage, Short sKbGrid, dbc::Short *psThrowNum, bool bRefresh, bool bForcible)
{
	if (!bForcible)
	{	
		//	2008-9-8	yangyinyu	add	begin!
		//		<>	
		if(	GetPlyMainCha()->m_CKitbag.IsLock()	)
			return	enumITEMOPT_ERROR_KBLOCK;
		//	2008-9-8	yangyinyu	add	end!

		if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //
			return enumITEMOPT_ERROR_KBLOCK;

		if (!GetActControl(enumACTCONTROL_ITEM_OPT))
			return enumITEMOPT_ERROR_STATE;
		//add by ALLEN 2007-10-16
		if (GetPlyMainCha()->IsReadBook()) //
			return enumITEMOPT_ERROR_KBLOCK;
		if (m_CKitbag.IsLock()) // 
			return enumITEMOPT_ERROR_KBLOCK;
	}

	USHORT sItemID = m_CKitbag.GetID(sKbGrid, sKbPage);
	USHORT sItemNum = m_CKitbag.GetNum(sKbGrid, sKbPage);
	// 
	CItemRecord* pItem = GetItemRecordInfo( sItemID );
	if( pItem == NULL )
		return enumITEMOPT_ERROR_NONE;

	if(pItem->chIsDel != 1) // 
		return enumITEMOPT_ERROR_UNDEL;

	/*	2008-8-13	
	//	2008-8-1	yangyinyu	add	begin!
	//	
	if(	m_CKitbag.GetGridContByID( sKbGrid )->dwDBID )
	{
		return	enumITEMOPT_ERROR_UNDEL;
	};
	//	2008-8-1	yangyinyu	add	end!
*/
	//if(pItem->sType == enumItemTypeMission)
	//	return enumITEMOPT_ERROR_UNTHROW;

	DWORD dwBoatID;
	// 
	if( pItem->sType == enumItemTypeBoat )
	{
		dwBoatID = m_CKitbag.GetDBParam( enumITEMDBP_INST_ID, sKbGrid );
		if( !BoatClear( dwBoatID ) )
		{
			//SystemNotice( "%s!", pItem->szName );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00010), pItem->szName.c_str() );
			return enumITEMOPT_ERROR_UNUSE;
		}
	}

	if (*psThrowNum != 0)
	{
		Short	sCurNum = m_CKitbag.GetNum(sKbGrid, sKbPage);
		if (sCurNum <= 0)
			return enumITEMOPT_ERROR_NONE;
		if (*psThrowNum > sCurNum)
			*psThrowNum = sCurNum;
	}

	if (bRefresh)
		m_CKitbag.SetChangeFlag(false, sKbPage);

	CKitbag& Bag = GetPlyMainCha()->m_CKitbag;
	SItemGrid* pGridCont = Bag.GetGridContByID( sKbGrid );
	if(	pGridCont->dwDBID )
	{
		//SystemNotice( "Item is bind, cannot be traded!" );
		return enumITEMOPT_ERROR_UNDEL;
	};

	SItemGrid GridCont;
	GridCont.sNum = *psThrowNum;
	Short sRet = KbPopItem(bRefresh, bRefresh, &GridCont, sKbGrid, sKbPage); // 
	if( sRet != enumKBACT_SUCCESS )
		return enumITEMOPT_ERROR_NONE;
	*psThrowNum = GridCont.sNum;

	// 
	RefreshNeedItem( sItemID );

	if (bRefresh)
		SynKitbagNew(enumSYN_KITBAG_THROW);

	if( pItem->sType == enumItemTypeBoat )
	{
		// 
		if (SaveAssets())
		{
			game_db.SaveBoatTempData( dwBoatID, this->GetPlayer()->GetDBChaId(), 1 );
		}
	}

	char	szPlyName[100];
	if (IsBoat())
		sprintf(szPlyName, "%s%s%s%s", GetName(), "(", GetPlyMainCha()->GetName(), ")");
	else
		sprintf(szPlyName, "%s", GetName());
	char	szMsg[128];
	sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00011), pItem->szName.c_str(), sItemID, GridCont.sNum);
	ToLogService("trade", "[CHA_DELETE] {} : {}", szPlyName, szMsg);

	LogAssets(enumLASSETS_DELETE);

	return enumITEMOPT_SUCCESS;
}

Short CCharacter::Cmd_GuildBankOper(Char chSrcType, Short sSrcGridID, Short sSrcNum, Char chTarType, Short sTarGridID){
	CKitbag	pCSrcBag, pCTarBag;
	CCharacter	*pCMainCha = GetPlyMainCha();

	int guildID = pCMainCha->GetGuildID();
	if (guildID == 0){
		return enumITEMOPT_ERROR_KBLOCK; // todo, different ret code.
	}
	
	int canTake = (emGldPermTakeBank&pCMainCha->guildPermission);
	int canGive = (emGldPermDepoBank&pCMainCha->guildPermission);

	if (chSrcType != 0 && canTake != emGldPermTakeBank){
		return enumITEMOPT_ERROR_KBLOCK;
	}
	else if (chTarType != 0 && canGive != emGldPermDepoBank){
		return enumITEMOPT_ERROR_KBLOCK;
	}

	if (chSrcType == 0){
		pCSrcBag = pCMainCha->m_CKitbag;
	}else{
		game_db.GetGuildBank(guildID, &pCSrcBag);
	}
	if (chSrcType == chTarType){
		pCTarBag = pCSrcBag;
	}else{
		if (chTarType == 0){
			pCTarBag = pCMainCha->m_CKitbag;
		}
		else{
			game_db.GetGuildBank(guildID, &pCTarBag); 
		}
	}

	if (pCSrcBag.IsLock() || pCTarBag.IsLock() || pCSrcBag.IsPwdLocked() || pCMainCha->IsReadBook()){
		return enumITEMOPT_ERROR_KBLOCK;
	}
	//else if (!GetPlayer()->GetBankNpc()){
	//	return enumITEMOPT_ERROR_DISTANCE;
	//}

	pCSrcBag.SetChangeFlag(false);
	pCTarBag.SetChangeFlag(false);

	if (chSrcType == chTarType)
	{
		if (chSrcType == 0)
		{
			if (!pCMainCha->KbRegroupItem(true, true, sSrcGridID, sSrcNum, sTarGridID) == enumKBACT_SUCCESS) 
				return enumITEMOPT_ERROR_NONE;
		}
		else
		{
			if (!pCSrcBag.Regroup(sSrcGridID, sSrcNum, sTarGridID) == enumKBACT_SUCCESS)
				return enumITEMOPT_ERROR_NONE;
		}
	}
	else
	{
		Short	sSrcItemID = pCSrcBag.GetID(sSrcGridID);

		CItemRecord* pItem = GetItemRecordInfo(sSrcItemID);
		if (pItem == NULL)
			return enumITEMOPT_ERROR_NONE;
		if (chSrcType == 0 && chTarType == 1){
			// kong@pkodev.net 09.22.2017
			SItemGrid *pGridCont = pCSrcBag.GetGridContByID(sSrcGridID);
			if (pGridCont->dwDBID){
				return enumITEMOPT_ERROR_TYPE;
			}
			if (auto ret = g_luaAPI.CallR<int>("OnBankItem", pCMainCha, pGridCont)) {
				if (!ret.value())
					return enumITEMOPT_ERROR_TYPE;
			}
			if (!pItem->chIsTrade || !pCSrcBag.GetGridContByID(sSrcGridID)->GetInstAttr(ITEMATTR_TRADABLE)){
				return enumITEMOPT_ERROR_TYPE;
			}
		}

		Short	sLeftNum;
		Short	sOpRet;
		SItemGrid	SPopItem;
		SPopItem.sNum = sSrcNum;
		if (chSrcType == 0)
			sOpRet = pCMainCha->KbPopItem(true, true, &SPopItem, sSrcGridID);
		else
			sOpRet = pCSrcBag.Pop(&SPopItem, sSrcGridID);
		if (sOpRet != enumKBACT_SUCCESS)
			return enumITEMOPT_ERROR_NONE;
		if (chTarType == 0)
			sOpRet = pCMainCha->KbPushItem(true, true, &SPopItem, sTarGridID);
		else
			sOpRet = pCTarBag.Push(&SPopItem, sTarGridID);
		sLeftNum = sSrcNum - SPopItem.sNum;
		if (sOpRet != enumKBACT_SUCCESS)
		{
			if (sOpRet == enumKBACT_ERROR_FULL)
			{
				if (chSrcType == 0)
					pCMainCha->KbPushItem(true, true, &SPopItem, sSrcGridID);
				else
					pCSrcBag.Push(&SPopItem, sSrcGridID);
			}
			else
			{
				SPopItem.sNum = sSrcNum;
				if (chSrcType == 0)
					pCMainCha->KbPushItem(true, true, &SPopItem, sSrcGridID);
				else
					pCSrcBag.Push(&SPopItem, sSrcGridID);
				return enumITEMOPT_ERROR_NONE;
			}
		}
	}

	if (chSrcType == 0)
		SynKitbagNew(enumSYN_KITBAG_BANK);
	else
	{
		GetPlayer()->SynGuildBank(&pCSrcBag, enumSYN_KITBAG_BANK);
		GetPlayer()->SetBankSaveFlag(0);
	}
	if (chSrcType != chTarType)
	{
		if (chTarType == 0)
			SynKitbagNew(enumSYN_KITBAG_BANK);
		else
		{
			GetPlayer()->SynGuildBank(&pCTarBag, enumSYN_KITBAG_BANK);
			GetPlayer()->SetBankSaveFlag(0);
		}
	}

	if (chSrcType != 0){
		game_db.UpdateGuildBank(guildID, &pCSrcBag);
	}
	else{
		game_db.UpdateGuildBank(guildID, &pCTarBag);
	}
	
	auto WtPk = net::msg::serialize(net::msg::MmUpdateGuildBankMessage{
		static_cast<int64_t>(guildID)
	});
	ReflectINFof(this, WtPk);

	return enumITEMOPT_SUCCESS;
}

// ..
Short CCharacter::Cmd_BankOper(Char chSrcType, Short sSrcGridID, Short sSrcNum, Char chTarType, Short sTarGridID)
{
	CKitbag	*pCSrcBag, *pCTarBag;
	CCharacter	*pCMainCha = GetPlyMainCha();
	if (chSrcType == 0)
		pCSrcBag = &pCMainCha->m_CKitbag;
	else
		pCSrcBag = GetPlayer()->GetBank();
	if (chSrcType == chTarType)
		pCTarBag = pCSrcBag;
	else
	{
		if (chTarType == 0)
			pCTarBag = &pCMainCha->m_CKitbag;
		else
			pCTarBag = GetPlayer()->GetBank();
	}
	if (pCSrcBag->IsLock() || pCTarBag->IsLock()) // 
		return enumITEMOPT_ERROR_KBLOCK;

    if (pCSrcBag->IsPwdLocked()) //
        return enumITEMOPT_ERROR_KBLOCK;
	//add by ALLEN 2007-10-16
	if (pCMainCha->IsReadBook()) //
        return enumITEMOPT_ERROR_KBLOCK;

	// 
	if (!GetPlayer()->GetBankNpc())
		return enumITEMOPT_ERROR_DISTANCE;

	pCSrcBag->SetChangeFlag(false);
	pCTarBag->SetChangeFlag(false);

	if (pCSrcBag == pCTarBag) // 
	{
		if (pCSrcBag == &pCMainCha->m_CKitbag)
		{
			if (!pCMainCha->KbRegroupItem(true, true, sSrcGridID, sSrcNum, sTarGridID) == enumKBACT_SUCCESS) // 
				return enumITEMOPT_ERROR_NONE;
		}
		else
		{
			if (!pCSrcBag->Regroup(sSrcGridID, sSrcNum, sTarGridID) == enumKBACT_SUCCESS) // 
				return enumITEMOPT_ERROR_NONE;
		}
	}
	else
	{
		Short	sSrcItemID = pCSrcBag->GetID(sSrcGridID);

		CItemRecord* pItem = GetItemRecordInfo(sSrcItemID);
		if(pItem == NULL)
			return enumITEMOPT_ERROR_NONE;
		if (chSrcType == 0 && chTarType == 1) // 
		{
			//if(pItem->sType == enumItemTypeBoat || pItem->sType == enumItemTypeTrade || pItem->sType == enumItemTypeBravery) // d?3h?hj??3A?hj???????h
				//return enumITEMOPT_ERROR_TYPE;

			// kong@pkodev.net 09.22.2017
			SItemGrid *pGridCont = pCSrcBag->GetGridContByID(sSrcGridID);
			if (auto ret = g_luaAPI.CallR<int>("OnBankItem", pCMainCha, pGridCont)) {
				if (!ret.value())
					return enumITEMOPT_ERROR_TYPE;
			}
		}

		Short	sLeftNum;
		Short	sOpRet;
		SItemGrid	SPopItem;
		SPopItem.sNum = sSrcNum;
		if (pCSrcBag == &pCMainCha->m_CKitbag)
			sOpRet = pCMainCha->KbPopItem(true, true, &SPopItem, sSrcGridID);
		else
			sOpRet = pCSrcBag->Pop(&SPopItem, sSrcGridID);
		if (sOpRet != enumKBACT_SUCCESS)
			return enumITEMOPT_ERROR_NONE;
		if (pCTarBag == &pCMainCha->m_CKitbag)
			sOpRet = pCMainCha->KbPushItem(true, true, &SPopItem, sTarGridID);
		else
			sOpRet = pCTarBag->Push(&SPopItem, sTarGridID);
		sLeftNum = sSrcNum - SPopItem.sNum;
		if (sOpRet != enumKBACT_SUCCESS)
		{
			if (sOpRet == enumKBACT_ERROR_FULL)
			{
				if (pCSrcBag == &pCMainCha->m_CKitbag)
					pCMainCha->KbPushItem(true, true, &SPopItem, sSrcGridID);
				else
					pCSrcBag->Push(&SPopItem, sSrcGridID);
			}
			else
			{
				SPopItem.sNum = sSrcNum;
				if (pCSrcBag == &pCMainCha->m_CKitbag)
					pCMainCha->KbPushItem(true, true, &SPopItem, sSrcGridID);
				else
					pCSrcBag->Push(&SPopItem, sSrcGridID);
				return enumITEMOPT_ERROR_NONE;
			}
		}

		char	szPlyName[100];
		if (IsBoat())
			sprintf(szPlyName, "%s%s%s%s", GetName(), "(", pCMainCha->GetName(), ")");
		else
			sprintf(szPlyName, "%s", GetName());
		char	szMsg[128];
		sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00012), chSrcType == 0 ? RES_STRING(GM_CHARACTERCMD_CPP_00013) : RES_STRING(GM_CHARACTERCMD_CPP_00014), chTarType == 0 ? RES_STRING(GM_CHARACTERCMD_CPP_00013) : RES_STRING(GM_CHARACTERCMD_CPP_00014), pItem->szName.c_str(), sSrcItemID, sLeftNum);
		ToLogService("trade", "[CHA_BANK] {} : {}", szPlyName, szMsg);
	}

	if (chSrcType == 0)
		SynKitbagNew(enumSYN_KITBAG_BANK);
	else
	{
		GetPlayer()->SynBank(0, enumSYN_KITBAG_BANK);
		GetPlayer()->SetBankSaveFlag(0);
	}
	if (chSrcType != chTarType)
	{
		if (chTarType == 0)
			SynKitbagNew(enumSYN_KITBAG_BANK);
		else
		{
			GetPlayer()->SynBank(0, enumSYN_KITBAG_BANK);
			GetPlayer()->SetBankSaveFlag(0);
		}
	}

	return enumITEMOPT_SUCCESS;
}

void CCharacter::Cmd_ReassignAttr(const net::msg::CmSynAttrMessage& msg)
{
	m_CChaAttr.ResetChangeFlag();
	SetBoatAttrChangeFlag(false);

	const char chAttrNum = static_cast<char>(msg.attrs.size());
	if (chAttrNum <= 0 || chAttrNum > 6)
	{
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00015));
		return;
	}

	Long	lBaseAttr[ATTR_LUK - ATTR_STR + 1] = {0};
	Long	lBaseAttrBalanceVal[ATTR_LUK - ATTR_STR + 1] =
	{
		(long)m_CChaAttr.GetAttrMaxVal(ATTR_BSTR) - (long)m_CChaAttr.GetAttr(ATTR_BSTR),
		(long)m_CChaAttr.GetAttrMaxVal(ATTR_BDEX) - (long)m_CChaAttr.GetAttr(ATTR_BDEX),
		(long)m_CChaAttr.GetAttrMaxVal(ATTR_BAGI) - (long)m_CChaAttr.GetAttr(ATTR_BAGI),
		(long)m_CChaAttr.GetAttrMaxVal(ATTR_BCON) - (long)m_CChaAttr.GetAttr(ATTR_BCON),
		(long)m_CChaAttr.GetAttrMaxVal(ATTR_BSTA) - (long)m_CChaAttr.GetAttr(ATTR_BSTA),
		(long)m_CChaAttr.GetAttrMaxVal(ATTR_BLUK) - (long)m_CChaAttr.GetAttr(ATTR_BLUK)
	};
	Long	lAttrPoint = 0;
	bool	bSetCorrect = true;
	Short	sAttrID{}, sAttrOppID{};
	for (char i = 0; i < chAttrNum; i++)
	{
		sAttrID = static_cast<Short>(msg.attrs[i].attrId);
		if (sAttrID < ATTR_STR || sAttrID > ATTR_LUK)
			continue;
		sAttrOppID = sAttrID - ATTR_STR;
		lBaseAttr[sAttrOppID] = static_cast<Long>(msg.attrs[i].value);
		if (lBaseAttr[sAttrOppID] < 0) // 0
		{
			bSetCorrect = false;
			break;
		}
		if (lBaseAttr[sAttrOppID] > lBaseAttrBalanceVal[sAttrOppID]) // 
			lBaseAttr[sAttrOppID] = lBaseAttrBalanceVal[sAttrOppID];

		lAttrPoint += lBaseAttr[sAttrOppID];
	}
	if (!bSetCorrect || lAttrPoint > m_CChaAttr.GetAttr(ATTR_AP))
	{
		SystemNotice("wrong attribute assignment scheme");
		return;
	}

	setAttr(ATTR_AP, m_CChaAttr.GetAttr(ATTR_AP) - lAttrPoint);
	for (short i = ATTR_BSTR; i <= ATTR_BLUK; i++)
	{
		if (lBaseAttr[i - ATTR_BSTR] > 0)
		{
			setAttr(i, m_CChaAttr.GetAttr(i) + lBaseAttr[i - ATTR_BSTR]);
		}
	}
	g_luaAPI.Call("AttrRecheck", static_cast<CCharacter*>(this));
	if (GetPlayer())
	{
		GetPlayer()->RefreshBoatAttr();
		SyncBoatAttr(enumATTRSYN_REASSIGN);
	}

	SynAttr(enumATTRSYN_REASSIGN);
}

// 
// lItemNum  0sFromID
// chFromType  1.2.0
// sFromID  -1chFromType
// chToType  0.1.2Cmd_UnfixItemchDir
// bForcible
Short CCharacter::Cmd_RemoveItem(Long lItemID, Long lItemNum, Char chFromType, Short sFromID, Char chToType, Short sToID, bool bRefresh, bool bForcible)
{
    
    
	if (!bForcible)
	{	
		//	2008-9-8	yangyinyu	add	begin!
		//		<>	
		if(	GetPlyMainCha()->m_CKitbag.IsLock()	)
			return	enumITEMOPT_ERROR_KBLOCK;
		//	2008-9-8	yangyinyu	add	end!

		if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //
			 return enumITEMOPT_ERROR_KBLOCK;

		//add by ALLEN 2007-10-16
		if (GetPlyMainCha()->IsReadBook()) //
        return enumITEMOPT_ERROR_KBLOCK;

		if (!GetActControl(enumACTCONTROL_ITEM_OPT))
			return enumITEMOPT_ERROR_STATE;
		if (m_CKitbag.IsLock()) // 
			return enumITEMOPT_ERROR_KBLOCK;
	}

	bool	bEquipChange = false;

	if (bRefresh)
	{
		m_CChaAttr.ResetChangeFlag();
		SetBoatAttrChangeFlag(false);
		m_CSkillState.ResetChangeFlag();
		SetLookChangeFlag();
		m_CKitbag.SetChangeFlag(false, 0);
	}

	Short	sOptRet = enumITEMOPT_ERROR_NONE;
	if (chFromType == 1 || chFromType == 0) // 
	{
		Long	lParam1, lParam2;
		if (chToType == 0) // 
		{
			GetTrowItemPos(&lParam1, &lParam2);
		}
		else if (chToType == 1) // 
		{
			lParam1 = 0;
			lParam2 = -1;
		}
		else if (chToType == 2) // 
		{
		}

		if (sFromID >= 0 && sFromID < enumEQUIP_NUM)
		{
			Short	sOptNum = (Short)lItemNum;
			sOptRet = Cmd_UnfixItem((Char)sFromID, &sOptNum, chToType, lParam1, lParam2, true, false, bForcible);
			if (sOptRet == enumITEMOPT_SUCCESS)
			{
				bEquipChange = true;
				if (lItemNum != 0)
				{
					if (lItemNum > sOptNum)
						lItemNum -= sOptNum;
					else
						goto ItemRemoveEnd;
				}
			}
		}
		else
		{
			Short	sOptNum;
			for (Char i = enumEQUIP_HEAD; i < enumEQUIP_NUM; i++)
			{
				if (m_SChaPart.SLink[i].sID == (Short)lItemID)
				{
					sOptNum = (Short)lItemNum;
					sOptRet = Cmd_UnfixItem(i, &sOptNum, chToType, lParam1, lParam2, true, false, bForcible);
					if (sOptRet == enumITEMOPT_SUCCESS)
					{
						bEquipChange = true;
						if (lItemNum != 0)
						{
							if (lItemNum > sOptNum)
								lItemNum -= sOptNum;
							else
								goto ItemRemoveEnd;
						}
					}
				}
			}
		}
	}
	if (chFromType == 2 || chFromType == 0) // 
	{
		Long	lParam1, lParam2;
		if (sFromID >= 0 && sFromID < m_CKitbag.GetCapacity())
		{
			Short	sOptNum = (Short)lItemNum;
			if (chToType == 0) // 
			{
				GetTrowItemPos(&lParam1, &lParam2);
				sOptRet = Cmd_ThrowItem(0, sFromID, &sOptNum, lParam1, lParam2, false, bForcible);
			}
			else if (chToType == 2) // 
				sOptRet = Cmd_DelItem(0, sFromID, &sOptNum, false, bForcible);
			if (sOptRet == enumITEMOPT_SUCCESS)
			{
				if (lItemNum != 0)
				{
					if (lItemNum > sOptNum)
						lItemNum -= sOptNum;
					else
						goto ItemRemoveEnd;
				}
			}
		}
		else
		{
			Short	sOptNum;
			Short	sUseGNum = m_CKitbag.GetUseGridNum();
			Short	sPosID;
			if (sUseGNum > 0)
			{
				for (Short i = sUseGNum - 1; i >= 0; i--)
				{
					sPosID = m_CKitbag.GetPosIDByNum(i);
					if (m_CKitbag.GetID(sPosID) == (Short)lItemID)
					{
						sOptNum = (Short)lItemNum;
						if (chToType == 0) // 
						{
							GetTrowItemPos(&lParam1, &lParam2);
							sOptRet = Cmd_ThrowItem(0, sPosID, &sOptNum, lParam1, lParam2, false, bForcible);
						}
						else if (chToType == 2) // 
							sOptRet = Cmd_DelItem(0, sPosID, &sOptNum, false, bForcible);
						if (sOptRet == enumITEMOPT_SUCCESS)
						{
							if (lItemNum != 0)
							{
								if (lItemNum > sOptNum)
									lItemNum -= sOptNum;
								else
									goto ItemRemoveEnd;
							}
						}
					}
				}
			}
		}
	}

ItemRemoveEnd:
	if (bRefresh)
	{
		if (bEquipChange)
		{
			GetPlyMainCha()->m_CSkillBag.SetChangeFlag(false);
			GetPlyCtrlCha()->SkillRefresh(); // 
			GetPlyMainCha()->SynSkillBag(enumSYN_SKILLBAG_MODI);

			// 
			SynSkillStateToEyeshot();

			// 
			if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
				SynLook(LOOK_SELF, true); // sync to self (item removed from inventory)
			else
				SynLook();

			// 
			g_luaAPI.Call("AttrRecheck", static_cast<CCharacter*>(this));
			if (GetPlayer())
			{
				GetPlayer()->RefreshBoatAttr();
				SyncBoatAttr(enumATTRSYN_ITEM_EQUIP);
			}
			SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);
		}
		// 
		SynKitbagNew(enumSYN_KITBAG_EQUIP);
	}

	return enumITEMOPT_SUCCESS;
}

// 
void CCharacter::Cmd_FightAsk(dbc::Char chType, dbc::Long lTarID, dbc::Long lTarHandle)
{
	CDynMapEntryCell	*pCTeamFightEntry = g_CDMapEntry.GetEntry(g_szTFightMapName);
	if (!pCTeamFightEntry) // 
	{
		//SystemNotice("PK!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00016));
		return;
	}

	Entity	*pCTarEnti = g_pGameApp->IsValidEntity(lTarID, lTarHandle);
	CCharacter	*pCTarCha;
	if (!pCTarEnti || !(pCTarCha = pCTarEnti->IsCharacter()))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00017));
		return;
	}
	if (!IsLiveing() || !pCTarCha->IsLiveing())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00018));
		return;
	}
	SubMap	*pCMap = GetSubMap(), *pCTarMap = pCTarCha->GetSubMap();
	if (!pCMap || !(pCMap->GetAreaAttr(GetPos()) & enumAREA_TYPE_FIGHT_ASK))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00019));
		return;
	}
	if (!pCTarMap || !(pCTarMap->GetAreaAttr(pCTarCha->GetPos()) & enumAREA_TYPE_FIGHT_ASK))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00020));
		return;
	}

	CPlayer	*pCPly = GetPlayer(), *pCTarPly = pCTarCha->GetPlayer();
	if (!pCPly || !pCTarPly)
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00021));
		return;
	}

	if (!IsRangePoint(pCTarCha->GetPos(), 6 * 100))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00022));
		return;
	}
	if (pCPly->HasChallengeObj())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00023));
		return;
	}
	if (pCTarPly->HasChallengeObj())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00024));
		return;
	}
	if (chType == enumFIGHT_TEAM)
	{
		if (pCPly->getTeamLeaderID() == 0)
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00025));
			return;
		}
		if (pCTarPly->getTeamLeaderID() == 0)
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00026));
			return;
		}
	}
	else if (chType == enumFIGHT_MONOMER)
	{
		if (pCTarPly->getTeamLeaderID() != 0 && pCTarPly->getTeamLeaderID() == pCPly->getTeamLeaderID())
		{
			//SystemNotice("PK!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00027));
			return;
		}
	}

	string	strScript = "check_can_enter_";
	strScript += pCTeamFightEntry->GetTMapName();
	if (auto ret = g_luaAPI.CallR<int>(strScript.c_str(), static_cast<CCharacter*>(this), pCTeamFightEntry))
	{
		if (!ret.value())
			return;
	}
	if (auto ret = g_luaAPI.CallR<int>(strScript.c_str(), pCTarCha, pCTeamFightEntry))
	{
		if (!ret.value())
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00028));
			return;
		}
	}

	//  :    McTeamFightAskMessage (count-first)
	net::msg::McTeamFightAskMessage fightMsg;

	// :            
	auto addPlayer = [&](CCharacter* pCha) {
		net::msg::TeamFightPlayerEntry entry;
		entry.name = pCha->GetName();
		entry.lv = static_cast<int64_t>((Char)pCha->getAttr(ATTR_LV));
		entry.job = g_GetJobName((Short)pCha->getAttr(ATTR_JOB));
		if (auto ret = g_luaAPI.CallR<int>("Get_ItemAttr_Join", static_cast<CCharacter*>(pCha)))
			entry.fightNum = ret.value_or(0);
		if (auto ret = g_luaAPI.CallR<int>("Get_ItemAttr_Win", static_cast<CCharacter*>(pCha)))
			entry.victoryNum = ret.value_or(0);
		fightMsg.players.push_back(std::move(entry));
	};

	Char	chObjStart = 2;
	//  
	Char	chSrcObjNum = 0;
	pCPly->SetChallengeType(chType);
	pCPly->SetChallengeParam(chObjStart++, pCPly->GetID());
	pCPly->SetChallengeParam(chObjStart++, pCPly->GetHandle());
	pCPly->StartChallengeTimer();
	addPlayer(this);
	chSrcObjNum++;
	if (chType == enumFIGHT_TEAM)
	{
		CPlayer	*pCTeamMem;
		CCharacter	*pCTeamCha;
		pCPly->BeginGetTeamPly();
		while (pCTeamMem = pCPly->GetNextTeamPly())
		{
			pCTeamCha = pCTeamMem->GetCtrlCha();
			if (pCTeamMem->HasChallengeObj())
			{
				//pCTeamCha->SystemNotice("!");
				pCTeamCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00023));
				continue;
			}
			if (!pCTeamCha->GetSubMap() || !(pCTeamCha->GetSubMap()->GetAreaAttr(pCTeamCha->GetPos()) & enumAREA_TYPE_FIGHT_ASK))
			{
				//pCTeamCha->SystemNotice("!");
				pCTeamCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00029));
				continue;
			}
			if (auto ret = g_luaAPI.CallR<int>(strScript.c_str(), pCTeamCha, pCTeamFightEntry))
			{
				if (!ret.value())
					continue;
			}

			pCTeamMem->SetChallengeType(chType);
			pCTeamMem->SetChallengeParam(0, 1);
			pCTeamMem->SetChallengeParam(2, pCPly->GetID());
			pCTeamMem->SetChallengeParam(3, pCPly->GetHandle());
			pCTeamMem->StartChallengeTimer();
			addPlayer(pCTeamCha);
			chSrcObjNum++;

			pCPly->SetChallengeParam(chObjStart++, pCTeamMem->GetID());
			pCPly->SetChallengeParam(chObjStart++, pCTeamMem->GetHandle());
		}
	}

	//  
	Char	chTarObjNum = 0;
	pCTarPly->SetChallengeType(chType);
	pCTarPly->SetChallengeParam(0, 1);
	pCTarPly->SetChallengeParam(2, pCPly->GetID());
	pCTarPly->SetChallengeParam(3, pCPly->GetHandle());
	pCTarPly->StartChallengeTimer();
	addPlayer(pCTarCha);
	chTarObjNum++;
	if (chType == enumFIGHT_TEAM)
	{
		CPlayer	*pCTeamMem;
		CCharacter	*pCTeamCha;
		pCTarPly->BeginGetTeamPly();
		while (pCTeamMem = pCTarPly->GetNextTeamPly())
		{
			pCTeamCha = pCTeamMem->GetCtrlCha();
			if (pCTeamMem->HasChallengeObj())
			{
				//pCTeamCha->SystemNotice("!");
				pCTeamCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00023));
				continue;
			}
			if (!pCTeamCha->GetSubMap() || !(pCTeamCha->GetSubMap()->GetAreaAttr(pCTeamCha->GetPos()) & enumAREA_TYPE_FIGHT_ASK))
			{
				//pCTeamCha->SystemNotice("!");
				pCTeamCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00019));
				continue;
			}
			if (auto ret = g_luaAPI.CallR<int>(strScript.c_str(), pCTeamCha, pCTeamFightEntry))
			{
				if (!ret.value())
					continue;
			}

			pCTeamMem->SetChallengeType(chType);
			pCTeamMem->SetChallengeParam(0, 1);
			pCTeamMem->SetChallengeParam(2, pCPly->GetID());
			pCTeamMem->SetChallengeParam(3, pCPly->GetHandle());
			pCTeamMem->StartChallengeTimer();
			addPlayer(pCTeamCha);
			chTarObjNum++;

			pCPly->SetChallengeParam(chObjStart++, pCTeamMem->GetID());
			pCPly->SetChallengeParam(chObjStart++, pCTeamMem->GetHandle());
		}
	}
	pCPly->SetChallengeParam(chObjStart++, pCTarPly->GetID());
	pCPly->SetChallengeParam(chObjStart++, pCTarPly->GetHandle());

	// :    count-first
	fightMsg.srcCount = chSrcObjNum;
	fightMsg.tarCount = chTarObjNum;
	auto WtPk = net::msg::serialize(fightMsg);
	if (chType == enumFIGHT_TEAM)
	{
		SENDTOCLIENT2(WtPk, pCPly->GetTeamMemberCnt(), pCPly->_Team);
		SENDTOCLIENT2(WtPk, pCTarPly->GetTeamMemberCnt(), pCTarPly->_Team);
	}
	pCTarEnti->ReflectINFof(this, WtPk);
	ReflectINFof(this, WtPk);

	pCPly->SetChallengeParam(0, chSrcObjNum + chTarObjNum);
	pCPly->SetChallengeParam(1, 0);
}

// 
void CCharacter::Cmd_FightAnswer(bool bFight)
{
	CPlayer	*pCPly = GetPlayer();
	if (!pCPly->HasChallengeObj())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00030));
		return;
	}

	Long	lID, lHandle;
	lID = pCPly->GetChallengeParam(2);
	lHandle = pCPly->GetChallengeParam(3);
	Long	lFightType = pCPly->GetChallengeType();

	CPlayer	*pCSrcPly, *pCTarPly;
	pCSrcPly = g_pGameApp->IsValidPlayer(lID, lHandle);
	if (!pCSrcPly || !pCSrcPly->HasChallengeObj())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00031));
		return;
	}
	Char	chObjNum = (Char)pCSrcPly->GetChallengeParam(0);
	pCTarPly = g_pGameApp->IsValidPlayer(pCSrcPly->GetChallengeParam(2 + (chObjNum -1) * 2), pCSrcPly->GetChallengeParam(2 + (chObjNum -1) * 2 + 1));
	if (!pCTarPly || !pCTarPly->HasChallengeObj())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00031));
		return;
	}

	// 
	bool	bHasPly = false;
	Char	chLoop = (Char)pCSrcPly->GetChallengeParam(0);
	if (chLoop > MAX_TEAM_MEMBER * 2)
		chLoop = MAX_TEAM_MEMBER * 2;
	for (Char i = 0; i < chLoop; i++)
	{
		if (pCSrcPly->GetChallengeParam(i * 2 + 2) == pCPly->GetID() && pCSrcPly->GetChallengeParam(i * 2 + 2 + 1) == pCPly->GetHandle())
			bHasPly = true;
	}
	if (!bHasPly) // PK
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00032));
		return;
	}

	if (!bFight)
	{
		std::string	strNoti = GetName();
		//strNoti += " PK!";
		strNoti += RES_STRING(GM_CHARACTERCMD_CPP_00033);
		//  :    PK-
		auto WtPk = net::msg::serialize(net::msg::McSysInfoMessage{ strNoti });
		pCSrcPly->GetCtrlCha()->ReflectINFof(this, WtPk);
		if (lFightType == enumFIGHT_TEAM)
		{
			SENDTOCLIENT2(WtPk, pCPly->GetTeamMemberCnt(), pCPly->_Team);
			SENDTOCLIENT2(WtPk, pCSrcPly->GetTeamMemberCnt(), pCSrcPly->_Team);
		}

		pCSrcPly->ClearChallengeObj();
		return;
	}
	pCSrcPly->SetChallengeParam(1, pCSrcPly->GetChallengeParam(1) + 1);
	if (pCSrcPly->GetChallengeParam(0) != pCSrcPly->GetChallengeParam(1)) // PK
		return;

	CDynMapEntryCell	*pCTeamFightEntry = g_CDMapEntry.GetEntry(g_szTFightMapName);
	if (!pCTeamFightEntry) // PK
	{
		//std::string	strNoti = "PK!";
		std::string	strNoti = RES_STRING(GM_CHARACTERCMD_CPP_00034);
		//  : PK-  
		auto WtPk = net::msg::serialize(net::msg::McSysInfoMessage{ strNoti });
		pCSrcPly->GetCtrlCha()->ReflectINFof(this, WtPk);
		if (lFightType == enumFIGHT_TEAM)
		{
			SENDTOCLIENT2(WtPk, pCPly->GetTeamMemberCnt(), pCPly->_Team);
			SENDTOCLIENT2(WtPk, pCSrcPly->GetTeamMemberCnt(), pCSrcPly->_Team);
		}

		pCSrcPly->ClearChallengeObj();
		return;
	}

	// 
	CMapEntryCopyCell	CMCpyCell(20, 0);
	CMapEntryCopyCell	*pCMCpyCell;
	if (!(pCMCpyCell = pCTeamFightEntry->AddCopy(&CMCpyCell)))
	{
		//std::string	strNoti = "!";
		std::string	strNoti = RES_STRING(GM_CHARACTERCMD_CPP_00035);
		//  :   
		auto WtPk = net::msg::serialize(net::msg::McSysInfoMessage{ strNoti });
		ReflectINFof(this, WtPk);
		pCSrcPly->GetCtrlCha()->ReflectINFof(this, WtPk);
		if (lFightType == enumFIGHT_TEAM)
		{
			SENDTOCLIENT2(WtPk, pCPly->GetTeamMemberCnt(), pCPly->_Team);
			SENDTOCLIENT2(WtPk, pCSrcPly->GetTeamMemberCnt(), pCSrcPly->_Team);
		}

		pCSrcPly->ClearChallengeObj();
		return;
	}
	if (!pCMCpyCell->HasFreePlyCount((Short)pCSrcPly->GetChallengeParam(0))) // 
	{
		//std::string	strNoti = "!";
		std::string	strNoti = RES_STRING(GM_CHARACTERCMD_CPP_00036);
		//  :  
		auto WtPk = net::msg::serialize(net::msg::McSysInfoMessage{ strNoti });
		ReflectINFof(this, WtPk);
		pCSrcPly->GetCtrlCha()->ReflectINFof(this, WtPk);
		if (lFightType == enumFIGHT_TEAM)
		{
			SENDTOCLIENT2(WtPk, pCPly->GetTeamMemberCnt(), pCPly->_Team);
			SENDTOCLIENT2(WtPk, pCSrcPly->GetTeamMemberCnt(), pCSrcPly->_Team);
		}

		pCTeamFightEntry->ReleaseCopy(pCMCpyCell);
		pCSrcPly->ClearChallengeObj();
		return;
	}

	string	strScript1 = "after_get_map_copy_";
	strScript1 += pCTeamFightEntry->GetTMapName();
	g_luaAPI.Call(strScript1.c_str(), pCMCpyCell, pCSrcPly, pCTarPly, static_cast<int>(lFightType));
	pCTeamFightEntry->SynCopyParam((Short)pCMCpyCell->GetPosID());
	// PK
	CPlayer	*pCFightMem;
	CCharacter	*pCFightCha;

	string	strScript = "begin_enter_";
	strScript += pCTeamFightEntry->GetTMapName();
	string	strScript2 = "check_can_enter_";
	strScript2 += pCTeamFightEntry->GetTMapName();

	bool	bSide1 = false, bSide2 = false;
	CCharacter	*pCEnterCha[MAX_TEAM_MEMBER * 2];
	Long	lEnterChaNum = 0;
	pCSrcPly->ClearChallengeObj();
	for (Char i = 0; i < chLoop; i++)
	{
		pCFightMem = g_pGameApp->IsValidPlayer(pCSrcPly->GetChallengeParam(i * 2 + 2), pCSrcPly->GetChallengeParam(i * 2 + 2 + 1));
		if (!pCFightMem)
			continue;
		pCFightCha = pCFightMem->GetCtrlCha();
		if (!pCFightCha->IsLiveing())
		{
			//pCFightCha->SystemNotice("PK");
			pCFightCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00037));
			continue;
		}
		if (!pCFightCha->GetSubMap() || !(pCFightCha->GetSubMap()->GetAreaAttr(pCFightCha->GetPos()) & enumAREA_TYPE_FIGHT_ASK))
		{
			//pCFightCha->SystemNotice("PKPK");
			pCFightCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00038));
			continue;
		}
		if (auto ret = g_luaAPI.CallR<int>(strScript2.c_str(), pCFightCha, pCTeamFightEntry))
		{
			if (!ret.value())
				continue;
		}
		if (lFightType == enumFIGHT_TEAM)
		{
			if (pCFightMem->getTeamLeaderID() == pCSrcPly->GetID())
				bSide1 = true;
			else if (pCFightMem->getTeamLeaderID() == pCTarPly->GetID())
				bSide2 = true;
			else
			{
				//pCFightCha->SystemNotice("PK");
				pCFightCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00039));
				continue;
			}
		}
		else
		{
			if (pCFightMem == pCSrcPly)
				bSide1 = true;
			else if (pCFightMem == pCTarPly)
				bSide2 = true;
			else
			{
				//pCFightCha->SystemNotice("PK");
				pCFightCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00040));
				continue;
			}
		}

		pCEnterCha[lEnterChaNum] = pCFightCha;
		lEnterChaNum++;
	}

	if (!(bSide1 & bSide2)) // 
	{
		for (Long i = 0; i < lEnterChaNum; i++)
			//pCEnterCha[i]->SystemNotice("PK!");
			pCEnterCha[i]->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00041));
		pCTeamFightEntry->ReleaseCopy(pCMCpyCell);
		return;
	}
	for (Long i = 0; i < lEnterChaNum; i++)
		g_luaAPI.Call(strScript.c_str(), pCEnterCha[i], pCMCpyCell);
	pCMCpyCell->AddCurPlyNum((Short)lEnterChaNum);

	// temp for test
	//std::string	strPrint = "";
	std::string	strPrint = RES_STRING(GM_CHARACTERCMD_CPP_00042);
	Char	szPrint[10];
	itoa(pCMCpyCell->GetPosID(), szPrint, 10);
	strPrint += szPrint;
	strPrint += ".";
	//strPrint += "";
	strPrint += RES_STRING(GM_CHARACTERCMD_CPP_00043);
	itoa(lEnterChaNum, szPrint, 10);
	strPrint += szPrint;
	strPrint += ".";
	//LG("", "%s\n", strPrint.c_str());
	ToLogService("common", "{}", strPrint.c_str());
	//
	pCTeamFightEntry->SynCopyRun((Short)pCMCpyCell->GetPosID(), enumMAPCOPY_START_CDT_PLYNUM, lEnterChaNum);

	return;
}

// 
void CCharacter::Cmd_ItemRepairAsk(dbc::Char chPosType, dbc::Char chPosID)
{
	CPlayer	*pCPly = GetPlayer();
	if (!pCPly)
		return;
	if (pCPly->IsInRepair())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00044));
		return;
	}

	CCharacter	*pCRepairman = pCPly->GetRepairman();
	if (!pCRepairman)
		return;
	if (!IsRangePoint(pCRepairman->GetPos(), 6 * 100))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00045));
		return;
	}
	if (!pCPly->SetRepairPosInfo(chPosType, chPosID))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00046));
		return;
	}
	CItemRecord	*pCItemRec = GetItemRecordInfo(pCPly->GetRepairItem()->sID);
	if (!pCItemRec)
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00046));
		return;
	}
	if (g_luaAPI.HasFunction("can_repair_item"))
	{
		auto ret = g_luaAPI.CallR<int>("can_repair_item", pCRepairman, static_cast<CCharacter*>(this), pCPly->GetRepairItem());
		if (ret.value_or(0) == 0)
			return;
	}
	if (!g_luaAPI.HasFunction("get_item_repair_money"))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00047));
		return;
	}
	auto repairMoney = g_luaAPI.CallR<int>("get_item_repair_money", pCPly->GetRepairItem());

	//  :
	auto WtPk = net::msg::serialize(net::msg::McItemRepairAskMcMessage{
		pCItemRec->szName, repairMoney.value_or(0)
	});
	ReflectINFof(this, WtPk);

	pCPly->SetInRepair();
}

// 
void CCharacter::Cmd_ItemRepairAnswer(bool bRepair)
{
	CPlayer	*pCPly = GetPlayer();
	if (!pCPly)
		return;
	if (!pCPly->IsInRepair())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00048));
		return;
	}

	if (bRepair)
	{
		CCharacter	*pCRepairman = pCPly->GetRepairman();
		if (!pCRepairman)
			goto EndItemRepair;
		if (!IsRangePoint(pCRepairman->GetPos(), 6 * 100))
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00045));
			goto EndItemRepair;
		}
		if (!pCPly->CheckRepairItem())
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00049));
			goto EndItemRepair;
		}
		if (g_luaAPI.HasFunction("can_repair_item"))
		{
			auto ret = g_luaAPI.CallR<int>("can_repair_item", pCRepairman, static_cast<CCharacter*>(this), pCPly->GetRepairItem());
			if (ret.value_or(0) == 0)
				goto EndItemRepair;
		}
		bool	bEquipValid = true;
		if (!pCPly->IsRepairEquipPos())
		{
			m_CKitbag.SetChangeFlag(false);
			m_CChaAttr.ResetChangeFlag();
		}
		else
		{
			bEquipValid = pCPly->GetRepairItem()->IsValid();
			m_CSkillBag.SetChangeFlag(false);
			m_CChaAttr.ResetChangeFlag();
			SetBoatAttrChangeFlag(false);
			m_CSkillState.ResetChangeFlag();
			SetLookChangeFlag();
		}
		if (g_luaAPI.HasFunction("begin_repair_item"))
		{
			g_luaAPI.Call("begin_repair_item", pCRepairman, static_cast<CCharacter*>(this), pCPly->GetRepairItem());
			if (!pCPly->IsRepairEquipPos())
			{
				CheckItemValid(pCPly->GetRepairItem());
				SynKitbagNew(enumSYN_KITBAG_EQUIP);
				SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);
			}
			else
			{
				if (!bEquipValid)
					SetEquipValid(pCPly->GetRepairPosID(), true, false);
				g_luaAPI.Call("AttrRecheck", static_cast<CCharacter*>(this));
				SynSkillBag(enumSYN_SKILLBAG_MODI);
				SynSkillStateToEyeshot();
				GetPlayer()->RefreshBoatAttr();
				SyncBoatAttr(enumATTRSYN_ITEM_EQUIP);
				SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);

				if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
					SynLook(LOOK_SELF, true); // sync to self (repairing equipments)
				else
					SynLook(enumSYN_LOOK_SWITCH);
			}
		}
	}

EndItemRepair:
	pCPly->SetInRepair(false);
}

// 
void CCharacter::Cmd_ItemForgeAsk(dbc::Char chType, SForgeItem *pSItem)
{
	CPlayer	*pCPly = GetPlayer();

    if(m_CKitbag.IsPwdLocked())
    {
        //SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		goto EndItemForgeAsk;
    }

	//add by ALLEN 2007-10-16
	if(IsReadBook())
    {
       // SystemNotice("!");
		 SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00051));
		goto EndItemForgeAsk;
    }

	if (!pSItem)
		goto EndItemForgeAsk;

	if (pCPly->IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00052));
		goto EndItemForgeAsk;
	}

	if( pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData() )
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00053));
		return;
	}

	{
	CCharacter	*pCForgeman = pCPly->GetForgeman();
	if (!pCForgeman)
		goto EndItemForgeAsk;
	if (!IsRangePoint(pCForgeman->GetPos(), 6 * 100))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00045));
		goto EndItemForgeAsk;
	}

	if (!CheckForgeItem(pSItem))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00054));
		goto EndItemForgeAsk;
	}

	pCPly->SetForgeInfo(chType, pSItem);

		SItemGrid* sig_ = NULL;
		for (int i = 0; i < defMAX_ITEM_FORGE_GROUP; i++)
		{
			if (pSItem->SGroup[i].sGridNum)
			{
				sig_ = pCPly->GetMainCha()->m_CKitbag.GetGridContByID(pSItem->SGroup[i].SGrid->sGridID);
			};
			if (sig_ && sig_->dwDBID)
			{
				SystemNotice("Item is locked, operation failed!");
				goto	EndItemForgeAsk;
			};
		};

		const char* szCheckCanScript;
		const char* szGetMoneyScript;
		if (chType == 1) // 
		{
			szCheckCanScript = "can_forge_item";
			szGetMoneyScript = "get_item_forge_money";
		}
		else if (chType == 2) // 
		{
			szCheckCanScript = "can_unite_item";
			szGetMoneyScript = "get_item_unite_money";
		}
		else if (chType == 3) // 
		{
			szCheckCanScript = "can_milling_item";
			szGetMoneyScript = "get_item_milling_money";
		}
		else if (chType == 4) // 
		{
			szCheckCanScript = "can_fusion_item";
			szGetMoneyScript = "get_item_fusion_money";
		}
		else if (chType == 5) // 
		{
			szCheckCanScript = "can_upgrade_item";
			szGetMoneyScript = "get_item_upgrade_money";
		}
		else if (chType == 6) // 
		{
			szCheckCanScript = "can_jlborn_item";
			szGetMoneyScript = "get_item_jlborn_money";
		}
		else if (chType == 7) // 
		{
			szCheckCanScript = "can_tichun_item";
			szGetMoneyScript = "get_item_tichun_money";
		}
		else if (chType == 8) // 
		{
			szCheckCanScript = "can_energy_item";
			szGetMoneyScript = "get_item_energy_money";
		}
		else if (chType == 9) // 
		{
			szCheckCanScript = "can_getstone_item";
			szGetMoneyScript = "get_item_getstone_money";
		}
		else if (chType == 10) // 
		{
			szCheckCanScript = "can_shtool_item";
			szGetMoneyScript = "get_item_shtool_money";
		}
		else
		{
			//SystemNotice( "!%d", chType );
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00055), chType);
			return;
		}

		Long	lCheckCan;
		if (!DoForgeLikeScript(szCheckCanScript, lCheckCan))
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
			goto EndItemForgeAsk;
		}
		if (lCheckCan == 0)
			goto EndItemForgeAsk;

		{
			Long	lOptMoney;
			if (!DoForgeLikeScript(szGetMoneyScript, lOptMoney))
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
				goto EndItemForgeAsk;
			}

			pCPly->SetInForge();

			if (lOptMoney == 0)
			{
				Cmd_ItemForgeAnswer(true);
			}
			else
			{
				//  :    
				auto WtPk = net::msg::serialize(net::msg::McItemForgeAskMessage{
					static_cast<int64_t>(chType), static_cast<int64_t>(lOptMoney)
				});
				ReflectINFof(this, WtPk);
			}

			SItemGrid* sig = NULL;
			for (int i = 0; i < defMAX_ITEM_FORGE_GROUP; i++)
			{
				if (pSItem->SGroup[i].sGridNum)
				{
					sig = pCPly->GetMainCha()->m_CKitbag.GetGridContByID(pSItem->SGroup[i].SGrid->sGridID);
				};
				if (sig)
				{
					break;
				};
			};
		}
		return;
	}

EndItemForgeAsk:
	//  :   (worldId=0  )
	{
		auto WtPk = net::msg::serialize(net::msg::McItemForgeAnswerMessage{0, static_cast<int64_t>(chType), 0});
		ReflectINFof(this, WtPk);
	}

	ForgeAction(false);
}

// Add by lark.li 20080515 begin
void CCharacter::Cmd_ItemLotteryAsk(SLotteryItem *pSItem)
{
	CPlayer	*pCPly = GetPlayer();

	if (!pSItem)
		goto EndItemLotteryAsk;

	{
		SItemGrid* pItemGrid = NULL;
		SItemGrid* pItemLottery = this->m_CKitbag.GetGridContByID(pSItem->SGroup[0].SGrid[0].sGridID);;

		{
			char buffer[defMAX_ITEM_LOTTERY_GROUP - 1][2];
			memset(buffer, 0, sizeof(buffer));

			for (int i = 1; i < defMAX_ITEM_LOTTERY_GROUP; i++)
			{
				pItemGrid = this->m_CKitbag.GetGridContByID(pSItem->SGroup[i].SGrid[0].sGridID);

				if (pItemGrid->sID == 5839)
					buffer[i - 1][0] = 'X';
				else
					itoa(pItemGrid->sID - 5829, buffer[i - 1], 16);
			}

			if (pItemLottery->sID = 5828)
			{
				int issue = this->GetLotteryIssue();

				pItemLottery->sInstAttr[0][0] = ITEMATTR_VAL_STR;
				pItemLottery->sInstAttr[0][1] = 10000 + issue;	// 4 1-3
				time_t t = time(0);
				tm* nowTm = localtime(&t);

				pItemLottery->sInstAttr[1][0] = ITEMATTR_VAL_AGI;
				pItemLottery->sInstAttr[1][1] = nowTm->tm_mday * 100 + nowTm->tm_hour;	// 12 34		2210

				// 0112 X11
				pItemLottery->sInstAttr[2][0] = ITEMATTR_VAL_DEX;
				pItemLottery->sInstAttr[2][1] = buffer[0][0] * 100 + buffer[1][0];	// 121 342	
				pItemLottery->sInstAttr[3][0] = ITEMATTR_VAL_CON;
				pItemLottery->sInstAttr[3][1] = buffer[2][0] * 100 + buffer[3][0];	// 123 344	
				pItemLottery->sInstAttr[4][0] = ITEMATTR_VAL_STA;
				pItemLottery->sInstAttr[4][1] = buffer[4][0] * 100 + buffer[5][0];	// 125 346	

				game_db.AddLotteryTicket(this, issue, buffer);
			}

			for (int i = 1; i < defMAX_ITEM_LOTTERY_GROUP; i++)
			{
				pItemGrid = this->m_CKitbag.GetGridContByID(pSItem->SGroup[i].SGrid[0].sGridID);

				KbPopItem(true, true, pItemGrid, pSItem->SGroup[i].SGrid[0].sGridID);
			}

			this->m_CKitbag.SetChangeFlag(true);
			Cmd_ItemLotteryAnswer(true);

			return;
		}
	}
EndItemLotteryAsk:
	//  :   ()
	auto WtPk = net::msg::serialize(net::msg::McItemLotteryAsrMessage{ 0 });
	ReflectINFof(this, WtPk);
}

void CCharacter::Cmd_ItemLotteryAnswer(bool bLottery)
{
	SynKitbagNew(enumSYN_KITBAG_EQUIP);
	SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);

	//  :   ()
	auto WtPk = net::msg::serialize(net::msg::McItemLotteryAsrMessage{ 0 });
	ReflectINFof(this, WtPk);
}
// End

void CCharacter::Cmd_LifeSkillItemAsk(long dwType, SLifeSkillItem *pSItem)
{
	if(m_CKitbag.IsPwdLocked())
	{
		//SystemNotice(".");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00057));
		return;
	}

	//add by ALLEN 2007-10-16
	if(IsReadBook())
	{
		//SystemNotice(".");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00058));
		return;
	}

	CPlayer	*pCPly = GetPlayer();
	if(pCPly == NULL || pSItem == NULL)
	{
		return;
	}

	string& strLifeSkillinfo = GetPlayer()->GetLifeSkillinfo();

	long lCheckCan = 0,lOptMoney =-1;

	if(m_CKitbag.IsPwdLocked())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		goto EndItemForgeAsk;
	}

	if (!pSItem)
	{
		goto EndItemForgeAsk;
	}


	if( pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData() )
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00053));
		goto EndItemForgeAsk;
	}

	pCPly->SetLifeSkillInfo(dwType,pSItem);
	const char* szCheckCanScript;
	const char* szGetMoneyScript;

	switch(dwType)
	{
		case 0:
		case 3:			
		case 2:
			{	
				szCheckCanScript = "can_manufacture_item";
				szGetMoneyScript = "end_manufacture_item";
				break;
			}
		case 1:
			{
				szCheckCanScript = "can_fenjie_item";
				szGetMoneyScript = "end_fenjie_item";
				break;
			}
		default:
		{
			//SystemNotice( "!%d", dwType );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00055), dwType );
			goto EndItemForgeAsk;
			break;
		}

	}

	if (!DoLifeSkillcript(szCheckCanScript, lCheckCan))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
		goto EndItemForgeAsk;
	}
	if (lCheckCan == 0)
	{
		goto EndItemForgeAsk;
	}
	if (!DoLifeSkillcript(szGetMoneyScript, lOptMoney))
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
		goto EndItemForgeAsk;
	}

	pCPly->SetInLifeSkill(false);
	
	if(-1 == lOptMoney)
		SynKitbagNew(enumSYN_KITBAG_FORGEF);
	else
		SynKitbagNew(enumSYN_KITBAG_FORGES);

EndItemForgeAsk:
	pCPly->SetInLifeSkill(false);
	{
		std::string skillInfo;
		if(1==dwType)
		{
			string	strVer[2];
			Util_ResolveTextLine(strLifeSkillinfo.c_str(),strVer,2,',');
			skillInfo = strVer[1];
		}
		else
			skillInfo = strLifeSkillinfo;
		auto WtPk = net::msg::serialize(net::msg::McLifeSkillMessage{
			static_cast<int64_t>(dwType), static_cast<int64_t>((short)lOptMoney), skillInfo
		});
		ReflectINFof(this, WtPk);
	}
}

void CCharacter::Cmd_LifeSkillItemAsR(long dwType, SLifeSkillItem *pSItem)
{
	if(m_CKitbag.IsPwdLocked())
	{
		//SystemNotice(".");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00057));
		return;
	}
	//add by ALLEN 2007-10-16
	if(IsReadBook())
	{
		//SystemNotice(".");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00058));
		return;
	}

	CPlayer	*pCPly = GetPlayer();
	if(pCPly == NULL || pSItem == NULL)
		return;
	long lCheckCan = 0,lOptMoney =-1;

	if (pCPly->IsInLifeSkill())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00052));
		return;
	}

	if(m_CKitbag.IsPwdLocked())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		return;
	}

	if (!pSItem)
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		return;
	}


	if( pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData() )
	{
		SystemNotice("!");
		return;
	}

	//Char	*szCheckCanScript;
	//Char	*szGetMoneyScript;


	const char* cszFunc;
	switch(dwType)
	{
		case 0:
			cszFunc =  "begin_manufacture_item";
			break;
		case 1:
			cszFunc =  "begin_manufacture3_item";
			break;
		case 2:
			cszFunc =  "begin_manufacture1_item";
			break;
		case 3:
			cszFunc =  "begin_manufacture2_item";
			break;
	}

	lua_getglobal(g_pLuaState,cszFunc);
	if(!lua_isfunction(g_pLuaState,-1))
	{
		lua_pop(g_pLuaState,1);
		SystemNotice("begin_manufacture_item");
		return ;
	}
	int	nParamNum = 0;
	int nRetNum = 1;

	luabridge::push(g_pLuaState, static_cast<CCharacter*>(this));
	nParamNum++;
	lua_pushnumber(g_pLuaState,pSItem->sbagCount);
	nParamNum++;

	for (int i = 0; i < pSItem->sbagCount; i++)
	{
		lua_pushnumber(g_pLuaState, pSItem->sGridID[i]);
		nParamNum ++;
	}

	lua_pushnumber(g_pLuaState,pSItem->sReturn);
	nParamNum++;
	int nState = lua_pcall(g_pLuaState, nParamNum, LUA_MULTRET, 0);
	if (nState != 0)
	{
		ToLogService("lua", LogLevel::Error, "DoString {}", cszFunc);
		lua_callalert(g_pLuaState, nState);
		lua_settop(g_pLuaState, 0);
		return ;
	}


	short stime = (short)lua_tonumber(g_pLuaState, -2);
	//---------
	//char time[20];
	//SystemNotice(time);
	//----------
	const char * cszContent = lua_tostring(g_pLuaState,-1);
	string& strLifeSkillinfo = GetPlayer()->GetLifeSkillinfo();
	strLifeSkillinfo = cszContent;
	lua_settop(g_pLuaState, 0);
		
	if(stime != 0)
		pCPly->SetInLifeSkill();

	{
		std::string skillInfo;
		if(1==dwType)
		{
			string	strVer[2];
			Util_ResolveTextLine(cszContent,strVer,2,',');
			skillInfo = strVer[0];
		}
		auto l_wpk = net::msg::serialize(net::msg::McLifeSkillAsrMessage{
			static_cast<int64_t>(dwType), static_cast<int64_t>(stime), skillInfo
		});
		ReflectINFof(this,l_wpk);
	}
}
//
void CCharacter::Cmd_LockKitbag()
{
    Char sState;//0: 1:
    cChar *szPwd = GetPlayer()->GetPassword();
    if(!m_CKitbag.IsPwdLocked()){ 
		m_CKitbag.PwdLock();
    }
    sState = m_CKitbag.IsPwdLocked() ? 1 : 0;

    //  :   
    auto WtPk = net::msg::serialize(net::msg::McKitbagCheckAnswerMessage{ static_cast<int64_t>(sState) });
	ReflectINFof(this, WtPk);
}

//
void CCharacter::Cmd_UnlockKitbag( const char szPassword[] )
{
    Char sState;//0:  1:

    CPlayer	*pCply = GetPlayer();
    cChar *szPwd2 = pCply->GetPassword();

    if((szPwd2[0] == 0) || (!strcmp(szPassword, szPwd2)))
    {
        m_CKitbag.PwdUnlock();
    }
    sState = m_CKitbag.IsPwdLocked() ? 1 : 0;

    //  :   
    auto WtPk = net::msg::serialize(net::msg::McKitbagCheckAnswerMessage{ static_cast<int64_t>(sState) });
	ReflectINFof(this, WtPk);
}

//  
void CCharacter::Cmd_CheckKitbagState()
{
    Char sState;//0:  1:
    sState = m_CKitbag.IsPwdLocked() ? 1 : 0;

    //  :   
    auto WtPk = net::msg::serialize(net::msg::McKitbagCheckAnswerMessage{ static_cast<int64_t>(sState) });
	ReflectINFof(this, WtPk);
}

void CCharacter::Cmd_SetKitbagAutoLock(Char cAuto)
{
    m_CKitbag.PwdAutoLock(cAuto);
    //game_db.SavePlayer(GetPlayer(), enumSAVE_TYPE_TRADE);
    if(cAuto == 0)
        SystemNotice(RES_STRING(GM_CHARACTER_CPP_00133));
    else
        SystemNotice(RES_STRING(GM_CHARACTER_CPP_00134));
}

BOOL CCharacter::Cmd_AddVolunteer()
{
	// check if level is higher than 10
	if (GetLevel() < 8) {
		PopupNotice("Only players lv8 and above can volunteer!");
		return false;
	}
	// check if map is garner2

	BOOL ret = g_pGameApp->AddVolunteer(this);
	if(!ret)
	{
		SystemNotice(RES_STRING(GM_CHARACTER_CPP_00135));
	}
	else
	{
		SystemNotice(RES_STRING(GM_CHARACTER_CPP_00136));
	}
	return ret;
}

BOOL CCharacter::Cmd_DelVolunteer()
{
	BOOL ret = g_pGameApp->DelVolunteer(this);
	if(!ret)
	{
		SystemNotice(RES_STRING(GM_CHARACTER_CPP_00137));
	}
	else
	{
		SystemNotice(RES_STRING(GM_CHARACTER_CPP_00138));
	}
	return ret;
}

void CCharacter::Cmd_ListVolunteer(short sPage, short sNum)
{
}

BOOL CCharacter::Cmd_ApplyVolunteer(const char *szName)
{
	return true;
}

CCharacter* CCharacter::FindVolunteer(const char *szName)
{
	SVolunteer *pVolInfo = g_pGameApp->FindVolunteer(szName);
	if(!pVolInfo)
	{
		return NULL;
	}

	CCharacter *pCha = NULL;
	CPlayer *pPly = g_pGameApp->GetPlayerByDBID(pVolInfo->ulID);
	if(pPly)
	{
		pCha = pPly->GetMainCha();
	}

	return pCha;
}

// 
void CCharacter::Cmd_ItemForgeAnswer(bool bForge)
{
	CPlayer	*pCPly = GetPlayer();
	Char	chType = pCPly->GetForgeType();

    if(m_CKitbag.IsPwdLocked())
    {
        //SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		goto EndItemForge;
    }

	//add by ALLEN 2007-10-16
	if(IsReadBook())
    {
        //SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00058));
		goto EndItemForge;
    }

    if (!pCPly->IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00059));
		goto EndItemForge;
	}

	if( pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData() )
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00060));
		return;
	}

	{
		CCharacter* pCForgeman = pCPly->GetForgeman();
		if (!pCForgeman)
		{
			goto EndItemForge;
		}
		if (!IsRangePoint(pCForgeman->GetPos(), 6 * 100))
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00045));
			goto EndItemForge;
		}

		if (bForge)
		{
			if (!CheckForgeItem())
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00054));
				goto EndItemForge;
			}

			const char* szCheckCanScript;
			const char* szBeginScript;
			if (chType == 1) // 
			{
				szCheckCanScript = "can_forge_item";
				szBeginScript = "begin_forge_item";
			}
			else if (chType == 2) // 
			{
				szCheckCanScript = "can_unite_item";
				szBeginScript = "begin_unite_item";
			}
			else if (chType == 3) // 
			{
				szCheckCanScript = "can_milling_item";
				szBeginScript = "begin_milling_item";
			}
			else if (chType == 4) // 
			{
				szCheckCanScript = "can_fusion_item";
				szBeginScript = "begin_fusion_item";
			}
			else if (chType == 5) // 
			{
				szCheckCanScript = "can_upgrade_item";
				szBeginScript = "begin_upgrade_item";
			}
			else if (chType == 6) // 
			{
				szCheckCanScript = "can_jlborn_item";
				szBeginScript = "begin_jlborn_item";
			}
			else if (chType == 7) // 
			{
				szCheckCanScript = "can_tichun_item";
				szBeginScript = "begin_tichun_item";
			}
			else if (chType == 8) // 
			{
				szCheckCanScript = "can_energy_item";
				szBeginScript = "begin_energy_item";
			}
			else if (chType == 9) // 
			{
				szCheckCanScript = "can_getstone_item";
				szBeginScript = "begin_getstone_item";
			}
			else if (chType == 10) // 
			{
				szCheckCanScript = "can_shtool_item";
				szBeginScript = "begin_shtool_item";
			}
			else
			{
				//SystemNotice( "!%d", chType );
				SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00055), chType);
				return;
			}

			Long	lCheckCan;
			if (!DoForgeLikeScript(szCheckCanScript, lCheckCan))
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
				goto EndItemForge;
			}
			if (lCheckCan == 0)
				goto EndItemForge;

			m_CChaAttr.ResetChangeFlag();
			m_CKitbag.SetChangeFlag(false);
			Long	lBegin;
			if (!DoForgeLikeScript(szBeginScript, lBegin))
			{
				//SystemNotice("!");
				SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
				goto EndItemForge;
			}
			SynKitbagNew(enumSYN_KITBAG_EQUIP);
			SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);
			if (lBegin == 0)
				goto EndItemForge;
			else
			{
				//  :   ()
				auto WtPk = net::msg::serialize(net::msg::McItemForgeAnswerMessage{
					GetID(), static_cast<int64_t>(chType), static_cast<int64_t>((Char)lBegin)
				});
				NotiChgToEyeshot(WtPk);

				pCPly->SetInForge(false);
				ForgeAction(false);
				return;
			}
		}
	}

EndItemForge:
	//  :   (worldId=0  )
	{
		auto WtPk = net::msg::serialize(net::msg::McItemForgeAnswerMessage{0, static_cast<int64_t>(chType), 0});
		ReflectINFof(this, WtPk);
	}

	pCPly->SetInForge(false);
	ForgeAction(false);
}
void CCharacter::Cmd_Garner2_Reorder(short index)
{
	SItemGrid *pSGridCont =  m_CKitbag.GetGridContByID(index);
	if(3849 != pSGridCont->sID)
	{
		//SystemNotice(",,.");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00061));
	}
	else
	{
		auto pk = net::msg::serialize(net::msg::MpGarner2UpdateMessage{
			static_cast<int64_t>(GetPlayer()->GetDBChaId()),
			std::string(GetName()),
			static_cast<int64_t>(GetLevel()),
			std::string(g_szJobName[getAttr( ATTR_JOB )]),
			static_cast<int64_t>(pSGridCont->GetInstAttr(ITEMATTR_MAXENERGY))
		});
		ReflectINFof(this,pk);
	}
}
