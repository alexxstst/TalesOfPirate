#include "Stdafx.h"
#include "Character.h"
#include "SubMap.h"
#include "GameApp.h"
#include "GameAppNet.h"
#include "CharTrade.h"
#include "Parser.h"
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

using namespace std;


char	g_chUseItemFailed[2];
char	g_chUseItemGiveMission[2];

_DBC_USING

// ��maincha���ø�����
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

		if (pCPlayer->GetLoginChaType() == enumLOGIN_CHA_BOAT) // �Դ�����̬��½
		{
			CCharacter* pBoat = pCPlayer->GetBoat(pCPlayer->GetLoginChaID());
			if (!pBoat)
			{
				//LG("enter_map", "��� %s�Դ�ֻ��ID %u����̬��½ʱʧ�ܣ���ֻ�����ڣ������ж�����!\n", GetLogName(), pCPlayer->GetLoginChaID());
				LG("enter_map", "character %s use boat(ID %u)form logging failed(boat is inexistence),be cut off connect!\n", GetLogName(), pCPlayer->GetLoginChaID());
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}
			if (!BoatEnterMap(*pBoat, 0, 0, 0))
			{
				pBoat->SetToMainCha();
				//LG("enter_map", "��� %s�Դ�ֻ��ID %u����̬��½ʱʧ�ܣ��Ŵ�ʧ�ܣ������ж�����!\n", GetLogName(), pCPlayer->GetLoginChaID());
				LG("enter_map", "character %suse boat(ID %u)form logging failed(put boat failed),be cut off connect!\n", GetLogName(), pCPlayer->GetLoginChaID());
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}
		}

		{
			CCharacter* pCCtrlCha = pCPlayer->GetCtrlCha();

			CMapRes* pCMapRes = 0;
			SubMap* pCMap = 0;
			if (bNewCha) // �½�ɫ
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
				if (chLogin == 0) // ��ɫ����
				{
					if (strcmp(l_map, pCCtrlCha->GetBirthMap()))
					{
						//LG("enter_map", "��� %s(%s) ��Ŀ���ͼ�� %s �ͽ����ɫ�ĵ�ͼ %s ����ƥ�䣬���ж�����!\n",
						LG("enter_map", "character %s(%s)'s aim map %s is not matched to focus character %s��be cut off connect!\n",
							GetLogName(), pCCtrlCha->GetLogName(), l_map, pCCtrlCha->GetBirthMap());
						sErrCode = ERR_MC_ENTER_POS;
						goto Error;
					}

					pCMapRes = g_pGameApp->FindMapByName(pCCtrlCha->GetBirthMap());
					l_x = pCCtrlCha->GetPos().x;
					l_y = pCCtrlCha->GetPos().y;
				}
				else // ��ͼ�л�
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
				//LG("enter_map", "��� %s(%s) �ĵ�ͼ����������Ƿ������ж�����!\n", GetLogName(), pCCtrlCha->GetLogName());
				LG("enter_map", "player %s(%s)'s map name or city name is unlawful��be cut off connect!\n", GetLogName(), pCCtrlCha->GetLogName());
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}
			pCMap = pCMapRes->GetCopy((Short)lMapCopyNO);
			if (!pCMap)
			{
				//LG("enter_map", "��� %s(%s) �ĵ�ͼ�����ŷǷ������ж�����!\n", GetLogName(), pCCtrlCha->GetLogName());
				LG("enter_map", "character %s(%s) copy map ID is unlawful��be cut off connect!\n", GetLogName(), pCCtrlCha->GetLogName());
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}
			if (pCMapRes->GetCopyStartType() != enumMAPCOPY_START_CONDITION)
				pCMap->Open();
			pCCtrlCha->SetBirthMap(pCMap->GetName());

			l_shape.centre.x = l_x;
			l_shape.centre.y = l_y;
			l_shape.radius = m_pCChaRecord->sRadii;
			if (!pCMap->EnsurePos(&l_shape, pCCtrlCha)) // ����ʧ��, �ж�����
			{
				//LG("enter_map", "��� %s(%s) �ĵ�ͼ����[%d, %d]�Ƿ������ж�����!\n", GetLogName(), pCCtrlCha->GetLogName(), l_x, l_y);
				LG("enter_map", "character %s(%s) 's map coordinate[%d, %d]is unlawful,be cut off connect!\n", GetLogName(), pCCtrlCha->GetLogName(), l_x, l_y);
				sErrCode = ERR_MC_ENTER_POS;
				goto Error;
			}

			pCPlayer->CheckChaItemFinalData();
			for (int i = 0; i < enumACTCONTROL_MAX; i++)
				SetActControl(i);
			m_CSkillState.Reset();
			m_CSkillBag.SetState(-1, enumSUSTATE_INACTIVE);
			pCCtrlCha->SkillRefresh();
			pCPlayer->CloseBank();

			// ����װ��
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
			// �����������
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
			FillBaseInfo(d.baseInfo); // ��������
			m_CSkillBag.SetChangeFlag();
			FillSkillBag(d.skillBag, enumSYN_SKILLBAG_INIT);
			FillSkillState(d.skillState);

			// ͬ����ɫ����
			if (bNewCha)
				g_CParser.DoString("CreatCha", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
			else
				g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
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
			// ����
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
			// ����
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
			FillKitbag(d.kitbag, m_CKitbag, enumSYN_KITBAG_INIT); // ������
			FillShortcut(d.shortcut); // �����

			SetBoatAttrChangeFlag();
			pCPlayer->RefreshBoatAttr();
			FillBoats(d.boats);

			d.ctrlChaId = pCCtrlCha->GetID();

			WPACKET pkret = net::msg::serialize(msg);

			// Trailing-поля для GateServer (loginFlag, playerCount, playerAddr).
			// GateServer читает их из хвоста пакета и срезает через DiscardLast(6).
			net::msg::McEnterMapTrailer trailer;
			trailer.loginFlag = chLogin;
			trailer.playerCount = g_pGameApp->m_dwPlayerCnt;
			trailer.playerAddr = ToAddress(pCPlayer);
			net::msg::serializeEnterMapTrailer(pkret, trailer);

			ReflectINFof(this, pkret);

			if (bNewCha)
				// ��ɫ��һ�γ����ڵ�ͼ��
				OnCharBorn();

			pCMap->Enter(&l_shape, pCCtrlCha); // �����ͼ���˴��ض��ɹ�
			pCMap->AfterPlyEnterMap(pCCtrlCha);
			pCCtrlCha->SynPKCtrl();

			m_timerPing.Reset();
			SendPreMoveTime();

			ResetStoreTime();

			//LG("enter_map", "����������Ϸ���� %s(%s)\n", GetLogName(), pCCtrlCha->GetLogName());
			LG("enter_map", "finish enter game scene %s(%s)\n", GetLogName(), pCCtrlCha->GetLogName());
			return true;
		}
	}
Error:
	{
		net::msg::McEnterMapMessage errMsg;
		errMsg.errCode = sErrCode;
		WPACKET pkret = net::msg::serialize(errMsg);
		ReflectINFof(this, pkret);
	}

	pCPlayer->SetLoginCha(enumLOGIN_CHA_MAIN, 0);
	pCPlayer->SetCtrlCha(GetPlyMainCha());
	game_db.SavePlayerPos(pCPlayer);
	g_pGameApp->GoOutGame(pCPlayer, true);

	//LG("enter_map", "������Ϸ����ʧ�� %s(%s)\n", GetLogName(), GetPlyCtrlCha()->GetLogName());
	LG("enter_map", "enter game scene failed %s(%s)\n", GetLogName(), GetPlyCtrlCha()->GetLogName());
	return false;
}

//=============================================================================
// ������sPing Ԥ�ƶ�ʱ�䣨���룩��
//       pPath ����Ŀ���·���յ㣻
//       chPointNum ·���յ���������ֵdefMOVE_INFLEXION_NUM��
//=============================================================================
void CCharacter::Cmd_BeginMove(Short sPing, Point *pPath, Char chPointNum, Char chStopState)
{
if (!IsLiveing()) {
	return;
}
	if (!GetActControl(enumACTCONTROL_MOVE))
	{
		
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		//m_CLog.Log("���Ϸ����ƶ������󣨴��ڲ����ƶ���״̬��[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("unlawful move request(being can't move state)[PacketID: %u]\n", m_ulPacketID);
		return;
	}

	if (GetMoveState() == enumMSTATE_ON && GetMoveStopState() == enumEXISTS_SLEEPING) // ��Ҫ����
	{
		
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		//m_CLog.Log("���Ϸ����ƶ������󣨽�ɫ��׼�����ߣ�[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("unlawful move request(character is prepare dormancy)[PacketID: %u]\n", m_ulPacketID);
		return;
	}

	if (GetMoveState() == enumMSTATE_ON && GetMoveEndPos() == pPath[chPointNum - 1]) // �����ƶ������յ���ͬ
	{
		
		//m_CLog.Log("�ƶ���Ŀ����뵱ǰ�ƶ���ͬ�����󱻺���[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("the aim point of move is currently move point,request be ignore [PacketID: %u]\n", m_ulPacketID);
		return;
	}

	if (m_CChaAttr.GetAttr(ATTR_MSPD) == 0)
	{
		
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		//m_CLog.Log("���Ϸ����ƶ��������ƶ��ٶ�Ϊ0��[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("unlawful move request(move speed is zero)[PacketID: %u]\n", m_ulPacketID);
		return;
	}

	for (Char chPCount = 0; chPCount < chPointNum; chPCount++)
		if (!m_submap->IsValidPos(pPath[chPCount].x, pPath[chPCount].y))
		{
			
			FailedActionNoti(enumACTION_MOVE, enumFACTION_MOVEPATH);
			//m_CLog.Log("�ƶ�·������·�����зǷ�\n");
			m_CLog.Log("move path error��pathID is unlawful\n");
			return;
		}

	SMoveInit	MoveInit;
	MoveInit.usPing = sPing;
	memcpy(MoveInit.SInflexionInfo.SList, pPath, sizeof(Point) * chPointNum);
	MoveInit.SInflexionInfo.sNum = chPointNum;
	MoveInit.STargetInfo.chType = 0;
	MoveInit.sStopState = chStopState;

	m_CLog.Log("Add action(Move) ActionData[%d, %d]\n", m_CAction.GetCurActionNo(), m_CAction.GetActionNum());
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
// ������Ŀ��ʵ��
//=============================================================================
void CCharacter::Cmd_BeginMoveDirect(Entity *pTar)
{
	if (!pTar || !g_pGameApp->IsLiveingEntity(pTar->GetID(), pTar->GetHandle())) // ������Ч
	{
		//m_CLog.Log("ʵ�岻����\n");
		m_CLog.Log("entity is inexistence\n");
		return;
	}
	Point	Path[2] = {GetPos(), pTar->GetPos()};
	Cmd_BeginMove(0, Path, 2);
}

//=============================================================================
// ������sPing Ԥ�ƶ�ʱ�䣨���룩��
//       chFightID ������ţ������ڿͻ��˵ı���ƥ��
//       pPath ����Ŀ���·���յ㣻
//       chPointNum ·���յ���������ֵdefMOVE_INFLEXION_NUM��
//       pSkill ���ܼ�¼��
//       lSkillLv ���ܵȼ����ò������ڷ���ҽ�ɫ�����壬������ҽ�ɫ����ȡ�似�ܰ��еĶ�Ӧ�ȼ�
//       lTarInfo1 lTarInfo2 Ŀ����Ϣ��
//             �������pSkill��������ʽ��ʵ�壬��ֱ��ʾID(GetID()), Handle(GetHandle())
//             �������lSkillNo��������ʽ�����꣬��ֱ��ʾ�����x,y
//=============================================================================
void CCharacter::Cmd_BeginSkill(Short sPing, Point *pPath, Char chPointNum,
								CSkillRecord *pSkill, Long lSkillLv, Long lTarInfo1, Long lTarInfo2, Char chStopState)
{
	if (!IsLiveing() || !pSkill || !pPath )
		return;

	if (GetMoveState() == enumMSTATE_ON && GetMoveStopState() == enumEXISTS_SLEEPING) // ��Ҫ����
	{
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		//m_CLog.Log("���Ϸ����ƶ������󣨽�ɫ��׼�����ߣ�[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("unlawful move request(character is prepare dormancy)[PacketID: %u]\n", m_ulPacketID);
		return;
	}

	if (!GetActControl(enumACTCONTROL_MOVE) && pPath[0] != pPath[1])
	{
		FailedActionNoti(enumACTION_MOVE, enumFACTION_ACTFORBID);
		//m_CLog.Log("���Ϸ��ļ������󣨴��ڲ����ƶ���״̬��[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("unlawful skill request(being can't move state)[PacketID: %u]\n", m_ulPacketID);
		return;
	}

	if (!IsRightSkill(pSkill))
	{
		FailedActionNoti(enumACTION_SKILL, enumFACTION_SKILL_NOTMATCH);
		//m_CLog.Log("���Ϸ��ļ������󣨼���ʩ���߲�ƥ�䣩[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("unlawful skill request(skill discharge is not matching)[PacketID: %u]\n", m_ulPacketID);
		return;
	}

	SSkillGrid	*pSSkillCont = m_CSkillBag.GetSkillContByID(pSkill->sID);
	if(!pSSkillCont)
	{
		if (IsBoat())
			pSSkillCont = GetPlayer()->GetMainCha()->m_CSkillBag.GetSkillContByID(pSkill->sID);
		if(!pSSkillCont)
		{
			short sItemID = atoi(pSkill->szDescribeHint);
			if(sItemID > 10)
			{
				CItemRecord* pItemRec = GetItemRecordInfo( sItemID );
				if(pItemRec)
				{
					CSkillRecord *pSkillRec = GetSkillRecordInfo(pSkill->sID);
					if(pSkillRec && !pSkillRec->IsShow())
					{
						BOOL bRet = GetPlayer()->GetMainCha()->LearnSkill( pSkill->sID, 1, true, false, true );
						//LG("���߼���", "��ɫ��%s\tѧϰ�˵��߼���(SkillID: %u)\n", GetLogName(), pSkill->sID);
						LG("Item skill", "character:%s\tstudy Item skill(SkillID: %u)\n", GetLogName(), pSkill->sID);
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
			//LG("���ܴ���", "��ɫ��%s\tû�иü���(SkillID: %u)\n", GetLogName(), pSkill->sID);
			LG("skill error", "character:%s\t hasn't the skill(SkillID: %u)\n", GetLogName(), pSkill->sID);
			FailedActionNoti(enumACTION_SKILL, enumFACTION_NOSKILL);
			//m_CLog.Log("��ɫû�иü���[SkillID: %u] [PacketID: %u]\n", pSkill->sID, m_ulPacketID);
			m_CLog.Log("character hasn't the skill[SkillID: %u] [PacketID: %u]\n", pSkill->sID, m_ulPacketID);
			return;
		}
	}
	CSkillTempData	*pCSkillTData = g_pGameApp->GetSkillTData(pSSkillCont->sID, pSSkillCont->chLv);
	if (!pCSkillTData)
	{
		//LG("���ܴ���", "��ɫ��%s\tû��ȡ���ü���(SkillID: %u, SkillLv: %u)����ʱ����\n", GetLogName(), pSSkillCont->sID, pSSkillCont->chLv);
		LG("skill error", "character:%s\t hasn't get the skill(SkillID: %u, SkillLv: %u)'s temp data\n", GetLogName(), pSSkillCont->sID, pSSkillCont->chLv);
		FailedActionNoti(enumACTION_SKILL, enumFACTION_NOSKILL);
		//m_CLog.Log("��ɫû��ȡ���ü��ܵ���ʱ����[SkillID: %u, SkillLv: %u] [PacketID: %u]\n", pSSkillCont->sID, pSSkillCont->chLv, m_ulPacketID);
		m_CLog.Log("character hasn't get the skill temp data[SkillID: %u, SkillLv: %u] [PacketID: %u]\n", pSSkillCont->sID, pSSkillCont->chLv, m_ulPacketID);

		return;
	}

	if (pSSkillCont->chState != enumSUSTATE_ACTIVE // δ����
		|| (pCSkillTData->lResumeTime == 0 && !GetActControl(enumACTCONTROL_USE_GSKILL)) // ����ʹ����������
		|| (pCSkillTData->lResumeTime > 0 && !GetActControl(enumACTCONTROL_USE_MSKILL))) // ����ʹ��ħ������
	{
		FailedActionNoti(enumACTION_SKILL, enumFACTION_ACTFORBID);
		//m_CLog.Log("���Ϸ��ļ������󣨴��ڲ���ʹ�ü��ܵ�״̬��[PacketID: %u]\n", m_ulPacketID);
		m_CLog.Log("unlawful skill request(being can't use skill state)[PacketID: %u]\n", m_ulPacketID);

		return;
	}

	for (Char chPCount = 0; chPCount < chPointNum; chPCount++)
		if (!m_submap->IsValidPos(pPath[chPCount].x, pPath[chPCount].y))
		{
			FailedActionNoti(enumACTION_MOVE, enumFACTION_MOVEPATH);
			//m_CLog.Log("�ƶ�·������·�����зǷ�\n");
			m_CLog.Log("move path error��path ID is unlawful\n");

			return;
		}

	SMoveInit	MoveInit;
	SFightInit	FightInit;

	if (SkillTarIsEntity(pSkill)) // ���ö�����ID
	{
		Entity *pTarEnt = g_pGameApp->IsMapEntity(lTarInfo1, lTarInfo2);
		if (!pTarEnt) // ���󲻴���
		{
			FailedActionNoti(enumACTION_SKILL, enumFACTION_NOOBJECT);

			return;
		}
		FightInit.chTarType = 1;
		MoveInit.STargetInfo.ulDist = GetSkillDist(pTarEnt, pSkill);

		if (pSkill == m_SFightInit.pCSkillRecord
			&& GetFightState() == enumFSTATE_ON
			&& IsRangePoint(pTarEnt->GetPos(), MoveInit.STargetInfo.ulDist)) // ��ͬ�ļ���
		{
			//m_CLog.Log("��ͬ�ļ������󣬱�����[PacketID: %u]\tTick %u\n", m_ulPacketID, GetTickCount());
			m_CLog.Log("the same skill request��be ignore [PacketID: %u]\tTick %u\n", m_ulPacketID, GetTickCount());

			return;
		}
	}
	else
	{
		FightInit.chTarType = 2;
		MoveInit.STargetInfo.ulDist = GetSkillDist(0, pSkill);

		if (pSkill == m_SFightInit.pCSkillRecord
			&& GetFightState() == enumFSTATE_ON
			&& IsRangePoint(lTarInfo1, lTarInfo2, MoveInit.STargetInfo.ulDist)) // ��ͬ�ļ���
		{
			//m_CLog.Log("��ͬ�ļ������󣬱�����[PacketID: %u]\tTick %u\n", m_ulPacketID, GetTickCount());
			m_CLog.Log("the same skill request��be ignore [PacketID: %u]\tTick %u\n", m_ulPacketID, GetTickCount());

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
			//m_CLog.Log("��ͬ�ļ��ܺ�Ŀ�꣬�����ƶ�·��[PacketID: %u]\tTick %u\n", m_ulPacketID, GetTickCount());
			m_CLog.Log("the same skill and aim��reset move path[PacketID: %u]\tTick %u\n", m_ulPacketID, GetTickCount());
			return;
		}
	}

	Show(); // ���������״̬�������

	m_CLog.Log("Add action(Move&Skill) ActionData[%d, %d]\n", m_CAction.GetCurActionNo(), m_CAction.GetActionNum());
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
	if (!pTar || !g_pGameApp->IsMapEntity(pTar->GetID(), pTar->GetHandle())) // ������Ч
	{
		//m_CLog.Log("ʵ�岻����\n");
		m_CLog.Log("entity is inexistence\n");
		return;
	}

	CSkillRecord *pSkill = GetSkillRecordInfo(lSkillNo);
	if (pSkill == NULL)
	{
		//m_CLog.Log("���ܣ�ID: %u��������\n", lSkillNo);
		m_CLog.Log("skill��ID: %u��is inexistence\n", lSkillNo);
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
	if (SkillTarIsEntity(pSkill)) // ���ö�����ID
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
		//m_CLog.Log("���ܣ�ID: %u��������\n", lSkillNo);
		m_CLog.Log("skill(ID: %u)is inexistence\n", lSkillNo);
		return;
	}

	Point	Path[2];
	Path[0] = GetPos();
	Path[1].x = lPosX;
	Path[1].y = lPosY;
	if (SkillTarIsEntity(pSkill)) // ���ö�����ID
	{
		//m_CLog.Log("���ܣ�ID: %u���Ķ����������\n", lSkillNo);
		m_CLog.Log("skill��ID: %u��'s object isn't the coordinate point\n", lSkillNo);
		return;
	}

	Cmd_BeginSkill(0, Path, 2, pSkill, lSkillLv, lPosX, lPosY);
}

//=============================================================================
// ʹ�õ���
//=============================================================================
Short CCharacter::Cmd_UseItem(Short sSrcKbPage, Short sSrcKbGrid, Short sTarKbPage, Short sTarKbGrid)
{
	if (m_CKitbag.IsLock()) // ����������
		return enumITEMOPT_ERROR_KBLOCK;
    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //��������
        return enumITEMOPT_ERROR_KBLOCK;
	//add by ALLEN 2007-10-16
    if (GetPlyMainCha()->IsReadBook()) //����״̬
        return enumITEMOPT_ERROR_KBLOCK;
   
    if(GetPlyMainCha()->GetStallData())     //  ����BUG, ��̯״̬
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
	if (pCItemRec->szAbleLink[0] == -1) // ������
		sUseRet = Cmd_UseExpendItem(sSrcKbPage, sSrcKbGrid, sTarKbPage, sTarKbGrid);
	else // װ����
		sUseRet = Cmd_UseEquipItem(sSrcKbPage, sSrcKbGrid, true, sTarKbGrid==-2);

	if (sUseRet == enumITEMOPT_SUCCESS && pCItemRec->IsSendUseItem())
		SynItemUseSuc(sItemID);

	return sUseRet;
}

//=============================================================================
// ʹ��װ�������
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

		g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
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
// ʹ�����������
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
		if (GetPlyCtrlCha()->IsBoat()) // ����̬����ʹ�ûسǾ�
			return enumITEMOPT_ERROR_UNUSE;

	//	��ӡ��2007-11-22 add begin!
	//	ˮ�ϲ���ʹ��½�ص��ߣ������ǲ�Ѫ����.
	//if(	pCItemRec
	//	&&	pCItemRec->chPickTo
	//	&&	GetPlyCtrlCha()->IsBoat()
	//	)
	//	return	enumITEMOPT_ERROR_UNUSE;
	//	��ӡ��2007-11-22 add end!


	// Ч������
	bool	bUseSuccess;
	if (strcmp(pCItemRec->szAttrEffect, "0") == 0)
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

		g_CParser.DoString(pCItemRec->szAttrEffect, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 3, this, pSGridCont, pSTarGridCont, DOSTRING_PARAM_END);
		
		if (g_chUseItemFailed[0] == 1) // ʹ�õ���ʧ��
			bUseSuccess = false;
		else
			bUseSuccess = true;

		// Add by lark.li 20080721 begin
		// ���߸�����ɹ�
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
	if (KbPopItem(false, false, &SGridCont, sKbGrid, sKbPage) != enumKBACT_SUCCESS) // ���������Ӧ�ó���
		return enumITEMOPT_ERROR_NONE;

	// ˢ��������߼���
	RefreshNeedItem( SGridCont.sID );

	char	szPlyName[100];
	if (IsBoat())
		sprintf(szPlyName, "%s%s%s%s", GetName(), "(", GetPlyMainCha()->GetName(), ")");
	else
		sprintf(szPlyName, "%s", GetName());
	char	szMsg[128];
	//sprintf(szMsg, "���ĵ��ߣ����� %s[%u]������ %u.", pCItemRec->szName, SGridCont.sID, SGridCont.sNum);
	sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00001), pCItemRec->szName, SGridCont.sID, SGridCont.sNum);
	TL(CHA_EXPEND, szPlyName, "", szMsg);

	if (bRefresh)
	{
		// ͬ��״̬
		SynSkillStateToEyeshot();

		// ֪ͨ���ǵ���������
		SynKitbagNew(enumSYN_KITBAG_SYSTEM);

		// ���¼�������
		g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
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
// ж��װ��
// chLinkID װ����λ
// psItemNum ж��������Ŀ��0��ʾȫ��ж��.�ɹ������󷵻�ʵ�������Ŀ
// chDir жװ����0 ���棬��ʱ[lParam1��lParam2]��ʾ����xy��
// 1 ����������ʱ[lParam1��lParam2]��ʾ������ҳ�ź�λ�ţ������������������ʧ�ܣ�
// 2 ɾ�����ߣ�
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
		if (m_CKitbag.IsLock()) // ����������
			return enumITEMOPT_ERROR_KBLOCK;
	}
	if (chLinkID < 0 || chLinkID >= enumEQUIP_NUM)
		return enumITEMOPT_ERROR_NONE;

	if (m_SChaPart.SLink[chLinkID].sID == 0)
		return enumITEMOPT_SUCCESS;
	else if (m_SChaPart.SLink[chLinkID].sID == enumEQUIP_BOTH_HAND) // ˫�ֵ���
	{
		if (chLinkID == enumEQUIP_LHAND) // ����������
			chLinkID = enumEQUIP_RHAND;
		else // ����������
			chLinkID = enumEQUIP_LHAND;
	}
	else if (m_SChaPart.SLink[chLinkID].sID == enumEQUIP_TOTEM) // ͼ�ڵ���
	{
		chLinkID = enumEQUIP_BODY;
	}

	Short	sItemID = m_SChaPart.SLink[chLinkID].sID;
	CItemRecord	*pCItemRec = GetItemRecordInfo(sItemID);
	if (!pCItemRec)
		return enumITEMOPT_ERROR_NONE;

	if (bPriority)// ��ж�����ȼ���λ��
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
    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //��������,����ж�ص�������
        return enumITEMOPT_ERROR_KBLOCK;
	//add by ALLEN 2007-10-16
		if (GetPlyMainCha()->IsReadBook()) //����,����ж�ص�������
        return enumITEMOPT_ERROR_KBLOCK;
	if (chDir == 1) // �����жװ���������������Ƿ���Է���
	{
		Short sKbPushRet = KbPushItem(false, false, &SUnfixCont, (Short&)lParam2, (Short)lParam1);
		if (sKbPushRet == enumKBACT_ERROR_FULL) // �������������ܲ���
		{
			return enumITEMOPT_ERROR_KBFULL;
			//chDir = 0;
			//pCCtrlCha->GetTrowItemPos(&lParam1, &lParam2);
		}
		else if (sKbPushRet != enumKBACT_SUCCESS)
			return enumITEMOPT_ERROR_NONE;
	}
	if (chDir == 0) // жװ������
	{
		pCItemRec = GetItemRecordInfo(SUnfixCont.sID);
		if (!pCItemRec)
			return enumITEMOPT_ERROR_NONE;
		if(pCItemRec->chIsThrow != 1 || !SUnfixCont.GetInstAttr(ITEMATTR_TRADABLE) ) // ���ɶ���
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
		//sprintf(szMsg, "�ӵ��ߣ�ж�����棩������ %s[%u]������ %u.", pCItemRec->szName, SUnfixCont.sID, *psItemNum);
		sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00002), pCItemRec->szName, SUnfixCont.sID, *psItemNum);
		TL(CHA_SYS, szPlyName, "", szMsg);
	}
	else // ɾ������
	{
	}

	if (*psItemNum == m_SChaPart.SLink[chLinkID].sNum) // ȫ��ж��
	{
		ChangeItem(0, &m_SChaPart.SLink[chLinkID], chLinkID);

		// ���øõ�����ռ��Link��
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
		GetPlyCtrlCha()->SkillRefresh(); // ���ܼ��
		GetPlyMainCha()->SynSkillBag(enumSYN_SKILLBAG_MODI);

		// ͬ��״̬
		SynSkillStateToEyeshot();

		// ֪ͨ��Ұ���������
		if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
			SynLook(LOOK_SELF, true); // sync to self
		else
			SynLook();

		// ���¼�������
		g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
		if (GetPlayer())
		{
			GetPlayer()->RefreshBoatAttr();
			SyncBoatAttr(enumATTRSYN_ITEM_EQUIP);
		}
		SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);

		// ֪ͨ���ǵ���������
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
// �����
// ���ߵ�WorldID��Handle
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

	if(pItem->chIsPick != 1) // ����ʰȡ
		return enumITEMOPT_ERROR_UNPICKUP;

	CCharacter	*pCCtrlCha = GetPlyCtrlCha(), *pCMainCha = GetPlyMainCha();
	// �ж�ʰȡ���
	SubMap	*pCMap = pCCtrlCha->GetSubMapFar();
	if (!pCMap)
		return enumITEMOPT_ERROR_KBLOCK;
	uShort	usAreaAttr = pCMap->GetAreaAttr(pCEnt->GetPos());
	if ((g_IsLand(usAreaAttr) != g_IsLand(GetAreaAttr())) && (g_IsLand(usAreaAttr))) //��ɫ���ڵ�����ͬ�ڵ���
		return enumITEMOPT_ERROR_AREA;

	CCharacter	*pKitbagCha = this;
	CKitbag	*pCKitbag = &m_CKitbag;
	if (!g_IsLand(usAreaAttr)) // ����
	{
		if (!IsBoat())
			return enumITEMOPT_ERROR_DISTANCE;
		if (pItem->chPickTo == enumITEM_PICKTO_KITBAG) // ���߷��뱳��
		{
			pKitbagCha = GetPlayer()->GetMainCha();
			pCKitbag = &pKitbagCha->m_CKitbag;
		}
	}

	if (pCKitbag->IsLock())
		return enumITEMOPT_ERROR_KBLOCK;

    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //��������
        return enumITEMOPT_ERROR_KBLOCK;

	//add by ALLEN 2007-10-16
	if (GetPlyMainCha()->IsReadBook()) //����״̬
        return enumITEMOPT_ERROR_KBLOCK;

	if (pCItem->GetProtChaID() != 0) // ���߱�����
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

	// �ж�ʰȡ����֤��
	if( pItem->sType == enumItemTypeBoat )
	{
		if( GetPlayer()->IsBoatFull() )
		{
			//SystemNotice( "������Я���Ĵ�ֻ������������������ʰȡ��Ʒ��%s��!", pItem->szName );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00003), pItem->szName );
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

	// ����ɫ������ƷЯ���Ĵ�ֻ
	if( pItem->sType == enumItemTypeBoat )
	{
		DWORD dwBoatID = pCItem->m_SGridContent.GetDBParam( enumITEMDBP_INST_ID );
		// �������ݿ����
		if (SaveAssets())
		{
			if( !game_db.SaveBoatTempData( dwBoatID, this->GetPlayer()->GetDBChaId() ) )
			{
				//LG( "ʰȡ��Ʒ����", "��ɫ��%s��ID[0x%X]ʰȡ�˴���֤�������Ǵ�ֻ���ݴ洢ʧ��!��ֻ����ID[0x%X]", 
				LG( "pick up goods error", "character��%s��ID[0x%X]pick up captain prove��but boat data storage failed!boat data ID[0x%X]", 
					this->GetName(), this->GetPlayer()->GetDBChaId(), dwBoatID );
			}
		}
		else
		{
			//LG( "ʰȡ��Ʒ����", "��ɫ��%s��ID[0x%X]ʰȡ�˴���֤�������Ǳ������ݴ洢ʧ��!��ֻ����ID[0x%X]", 
			LG( "pick up goods error", "character��%s��ID[0x%X]pick up captain prove��but kitbag data storage failed!boat data ID[0x%X]", 
				this->GetName(), this->GetPlayer()->GetDBChaId(), dwBoatID );
		}

		if( !BoatAdd( dwBoatID ) )
		{
			//SystemNotice( "ʰȡ����֤�������Ӵ�ֻʧ��!ID[0x%X]", dwBoatID );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00004), dwBoatID );
			//LG( "ʰȡ��Ʒ����", "��ɫ��%s��ID[0x%X]ʰȡ�˴���֤�������Ӵ�ֻʧ��!��ֻ����ID[0x%X]", 
			LG( "pick up goods error", "character��%s��ID[0x%X]pick up captain prove��add boat failed!boat dataID[0x%X]", 
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
	//sprintf(szMsg, "����ߣ����� %s[%u]������ %u.", pItem->szName, pCItem->m_SGridContent.sID, sPickupNum);
	sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00005), pItem->szName, pCItem->m_SGridContent.sID, sPickupNum);
	TL(SYS_CHA, szPlyName, "", szMsg);

	//ColourNotice(0xb5eb8e, "Picked up x%d %s", sPickupNum, pItem->szName);

	//��ȡ��Ʒ��֪ͨ����
	char szTeamMsg[128];
	//sprintf(szTeamMsg, "���Ķ���%s��%u��%s", szPlyName, sPickupNum, pItem->szName);

	CFormatParameter param(3);
	param.setString(0, szPlyName);
	param.setLong(1, sPickupNum);
	param.setString(2, pItem->szName);

	//char szParamMsg[255];
	RES_FORMAT_STRING(GM_CHARACTERCMD_CPP_00006, param, szTeamMsg);
	//sprintf(szTeamMsg, szParamMsg, szPlyName, sPickupNum, pItem->szName);

	WPACKET WtPk = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_SYSINFO);
	WRITE_SEQ(WtPk, szTeamMsg, uShort(strlen(szTeamMsg) + 1));
	SENDTOCLIENT2(WtPk, GetPlayer()->GetTeamMemberCnt(), GetPlayer()->_Team);

	if (sPushRet == enumKBACT_SUCCESS)
		pCItem->Free();

	pKitbagCha->SynKitbagNew(enumSYN_KITBAG_PICK);

	pKitbagCha->LogAssets(enumLASSETS_PICKUP);

	return enumITEMOPT_SUCCESS;
}

//�Ϸ���ʱ�����ĵ���(sSrcGrid:��ʱ������λ��   sSrcNum:����   sTarGrid:������λ��)
Short CCharacter::Cmd_DragItem(Short sSrcGrid, Short sSrcNum, Short sTarGrid)
{
    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //��������
        return enumITEMOPT_ERROR_KBLOCK;

	//add by ALLEN 2007-10-16
	if (GetPlyMainCha()->IsReadBook()) //����״̬
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
       // SystemNotice("����������!");
		 SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00007));
        m_pCKitbagTmp->Push(&Grid, sSrcGrid);
        SynKitbagTmpNew(enumSYN_KITBAG_SWITCH);
        return enumITEMOPT_ERROR_KBFULL;
    }

    SynKitbagTmpNew(enumSYN_KITBAG_SWITCH);
    
    return enumITEMOPT_SUCCESS;
}

// �ӵ���
// psThrowNum �ӳ���Ŀ��0Ϊȫ���ӳ����ɹ������󷵻�ʵ�������
Short CCharacter::Cmd_ThrowItem(Short sKbPage, Short sKbGrid, Short *psThrowNum, Long lPosX, Long lPosY, bool bRefresh, bool bForcible)
{
    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //��������
        return enumITEMOPT_ERROR_KBLOCK;

	//add by ALLEN 2007-10-16
    if (GetPlyMainCha()->IsReadBook()) //����״̬
        return enumITEMOPT_ERROR_KBLOCK;

	if (!bForcible)
	{
		if (!GetActControl(enumACTCONTROL_ITEM_OPT))
			return enumITEMOPT_ERROR_STATE;
		if (m_CKitbag.IsLock()) // ����������
			return enumITEMOPT_ERROR_KBLOCK;
	}

	Point	STarP = {lPosX, lPosY};
	if (!IsRangePoint(STarP, defTHROW_DISTANCE))
		return enumITEMOPT_ERROR_DISTANCE;

	USHORT sItemID = m_CKitbag.GetID(sKbGrid, sKbPage);
	CItemRecord* pItem = GetItemRecordInfo( sItemID );
	if( pItem == NULL )
		return enumITEMOPT_ERROR_NONE;

    //  ���ϲ����������������
    if(IsBoat())
    {
        if(enumITEM_PICKTO_KITBAG == pItem->chPickTo)
        {
            return enumITEMOPT_ERROR_UNTHROW;
        }
    }

	if(pItem->chIsThrow != 1 || !m_CKitbag.GetGridContByID(sKbGrid)->GetInstAttr(ITEMATTR_TRADABLE)) // ���ɶ���
		return enumITEMOPT_ERROR_UNTHROW;

	SItemGrid*	grid	=	m_CKitbag.GetGridContByID(	sKbGrid		);
	if(	grid	&&	grid->dwDBID	)
	{
		return	enumITEMOPT_ERROR_UNTHROW;
	};

	// �ж϶�������֤��
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
			//SystemNotice( "������%s��ʧ�ܣ�������ʹ�øô�!", pItem->szName );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00008), pItem->szName );
			return enumITEMOPT_ERROR_UNUSE;
		}
	}

	CCharacter	*pCCtrlCha = GetPlyCtrlCha(), *pCMainCha = GetPlyMainCha();
	SubMap	*pCMap = pCCtrlCha->GetSubMapFar();
	if (!pCMap)
		return enumITEMOPT_ERROR_KBLOCK;
	uShort	usAreaAttr = pCMap->GetAreaAttr(lPosX, lPosY);
	if (g_IsLand(usAreaAttr) != g_IsLand(GetAreaAttr())) //��ɫ���ڵ�����ͬ�ڵ���
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
	Short sRet = KbPopItem(bRefresh, bRefresh, &GridCont, sKbGrid, sKbPage); // �����������ɹ�
	if( sRet != enumKBACT_SUCCESS )
		return enumITEMOPT_ERROR_NONE;
	*psThrowNum = GridCont.sNum;

	// ˢ��������߼���
	RefreshNeedItem( sItemID );

	// ֪ͨ���ǵ���������
	if (bRefresh)
		SynKitbagNew(enumSYN_KITBAG_THROW);

	char	szPlyName[100];
	if (IsBoat())
		sprintf(szPlyName, "%s%s%s%s", GetName(), "(", GetPlyMainCha()->GetName(), ")");
	else
		sprintf(szPlyName, "%s", GetName());
	char	szMsg[128];
	//sprintf(szMsg, "�ӵ��ߣ����� %s[%u]������ %u.", pItem->szName, GridCont.sID, GridCont.sNum);
	sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00009), pItem->szName, GridCont.sID, GridCont.sNum);
	TL(CHA_SYS, szPlyName, "", szMsg);

	pCMap->ItemSpawn(&GridCont, lPosX, lPosY, enumITEM_APPE_THROW, pCCtrlCha->GetID(), pCMainCha->GetID(), pCMainCha->GetHandle(), 10 * 1000); // �ӳ��ĵ��߽���10��ʱ�䱣��

	LogAssets(enumLASSETS_THROW);

	return enumITEMOPT_SUCCESS;
}

// ���ߵĻ�λ���ϲ������.
Short CCharacter::Cmd_ItemSwitchPos(Short sKbPage, Short sSrcGrid, Short sSrcNum, Short sTarGrid)
{
	if (!GetActControl(enumACTCONTROL_ITEM_OPT))
		return enumITEMOPT_ERROR_STATE;

	if (m_CKitbag.IsLock()) // ����������
		return enumITEMOPT_ERROR_KBLOCK;

    if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //��������
        return enumITEMOPT_ERROR_KBLOCK;

	//add by ALLEN 2007-10-16
	if (GetPlyMainCha()->IsReadBook()) //����״̬
        return enumITEMOPT_ERROR_KBLOCK;

	m_CKitbag.SetChangeFlag(false, sKbPage);
	Short	sKbOptRet = KbRegroupItem(true, true, sSrcGrid, sSrcNum, sTarGrid);
	if (sKbOptRet == enumKBACT_SUCCESS || sKbOptRet == enumKBACT_ERROR_FULL) // ��λ�ɹ���֪ͨ���ǵ���������
		SynKitbagNew(enumSYN_KITBAG_SWITCH);
	else if (sKbOptRet == enumKBACT_ERROR_RANGE)
		return enumITEMOPT_ERROR_KBRANGE;
	else
		return enumITEMOPT_ERROR_NONE;

	return enumITEMOPT_SUCCESS;
}

// ɾ������
// psThrowNum ɾ����Ŀ��0Ϊȫ��ɾ�����ɹ������󷵻�ʵ�������
Short CCharacter::Cmd_DelItem(Short sKbPage, Short sKbGrid, dbc::Short *psThrowNum, bool bRefresh, bool bForcible)
{
	if (!bForcible)
	{	
		//	2008-9-8	yangyinyu	add	begin!
		//	���	<�����к���ҵı���ʵ���������ģ��޷������κεĲ��������ڸù�����Ч������ҿ��Զ�����ɾ����Ʒ>	���е��޸ġ�
		if(	GetPlyMainCha()->m_CKitbag.IsLock()	)
			return	enumITEMOPT_ERROR_KBLOCK;
		//	2008-9-8	yangyinyu	add	end!

		if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //��������
			return enumITEMOPT_ERROR_KBLOCK;

		if (!GetActControl(enumACTCONTROL_ITEM_OPT))
			return enumITEMOPT_ERROR_STATE;
		//add by ALLEN 2007-10-16
		if (GetPlyMainCha()->IsReadBook()) //����״̬
			return enumITEMOPT_ERROR_KBLOCK;
		if (m_CKitbag.IsLock()) // ����������
			return enumITEMOPT_ERROR_KBLOCK;
	}

	USHORT sItemID = m_CKitbag.GetID(sKbGrid, sKbPage);
	USHORT sItemNum = m_CKitbag.GetNum(sKbGrid, sKbPage);
	// �ж��Ƿ񴬳�֤������
	CItemRecord* pItem = GetItemRecordInfo( sItemID );
	if( pItem == NULL )
		return enumITEMOPT_ERROR_NONE;

	if(pItem->chIsDel != 1) // ��������
		return enumITEMOPT_ERROR_UNDEL;

	/*	2008-8-13	�߳�֪ͨ�������١�
	//	2008-8-1	yangyinyu	add	begin!
	//	�������߲������١�
	if(	m_CKitbag.GetGridContByID( sKbGrid )->dwDBID )
	{
		return	enumITEMOPT_ERROR_UNDEL;
	};
	//	2008-8-1	yangyinyu	add	end!
*/
	//if(pItem->sType == enumItemTypeMission)
	//	return enumITEMOPT_ERROR_UNTHROW;

	DWORD dwBoatID;
	// �ж϶�������֤��
	if( pItem->sType == enumItemTypeBoat )
	{
		dwBoatID = m_CKitbag.GetDBParam( enumITEMDBP_INST_ID, sKbGrid );
		if( !BoatClear( dwBoatID ) )
		{
			//SystemNotice( "���١�%s��ʧ�ܣ�������ʹ�øô�!", pItem->szName );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00010), pItem->szName );
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
	Short sRet = KbPopItem(bRefresh, bRefresh, &GridCont, sKbGrid, sKbPage); // �����������ɹ�
	if( sRet != enumKBACT_SUCCESS )
		return enumITEMOPT_ERROR_NONE;
	*psThrowNum = GridCont.sNum;

	// ˢ��������߼���
	RefreshNeedItem( sItemID );

	if (bRefresh)
		SynKitbagNew(enumSYN_KITBAG_THROW);

	if( pItem->sType == enumItemTypeBoat )
	{
		// �������ݿ����
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
	//sprintf(szMsg, "ɾ�����ߣ����� %s[%u]������ %u.", pItem->szName, sItemID, GridCont.sNum);
	sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00011), pItem->szName, sItemID, GridCont.sNum);
	TL(CHA_DELETE, szPlyName, "", szMsg);

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
			if (g_CParser.DoString("OnBankItem", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCMainCha, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pGridCont, DOSTRING_PARAM_END)) {
				if (!g_CParser.GetReturnNumber(0))
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
	
	WPACKET WtPk = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MM_UPDATEGUILDBANK);
	WRITE_LONG(WtPk, pCMainCha->m_ID);
	WRITE_LONG(WtPk, guildID);
	ReflectINFof(this, WtPk);

	return enumITEMOPT_SUCCESS;
}

// ������ز����������������ڵ��ߵĻ�λ���ϲ������.�������ڵ��ߵĻ�λ���ϲ������.�Լ������������м���ߵĽ�����
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
	if (pCSrcBag->IsLock() || pCTarBag->IsLock()) // ����������
		return enumITEMOPT_ERROR_KBLOCK;

    if (pCSrcBag->IsPwdLocked()) //��������
        return enumITEMOPT_ERROR_KBLOCK;
	//add by ALLEN 2007-10-16
	if (pCMainCha->IsReadBook()) //����״̬
        return enumITEMOPT_ERROR_KBLOCK;

	// �ڴ�����ʱ���о�����
	if (!GetPlayer()->GetBankNpc())
		return enumITEMOPT_ERROR_DISTANCE;

	pCSrcBag->SetChangeFlag(false);
	pCTarBag->SetChangeFlag(false);

	if (pCSrcBag == pCTarBag) // ����λ��
	{
		if (pCSrcBag == &pCMainCha->m_CKitbag)
		{
			if (!pCMainCha->KbRegroupItem(true, true, sSrcGridID, sSrcNum, sTarGridID) == enumKBACT_SUCCESS) // ��λ�ɹ���֪ͨ���ǵ���������
				return enumITEMOPT_ERROR_NONE;
		}
		else
		{
			if (!pCSrcBag->Regroup(sSrcGridID, sSrcNum, sTarGridID) == enumKBACT_SUCCESS) // ��λ�ɹ���֪ͨ���ǵ���������
				return enumITEMOPT_ERROR_NONE;
		}
	}
	else
	{
		Short	sSrcItemID = pCSrcBag->GetID(sSrcGridID);

		CItemRecord* pItem = GetItemRecordInfo(sSrcItemID);
		if(pItem == NULL)
			return enumITEMOPT_ERROR_NONE;
		if (chSrcType == 0 && chTarType == 1) // �ӵ�����������
		{
			//if(pItem->sType == enumItemTypeBoat || pItem->sType == enumItemTypeTrade || pItem->sType == enumItemTypeBravery) // d?3h?hj??3ȰA?hj?Ȯ??????h
				//return enumITEMOPT_ERROR_TYPE;

			// kong@pkodev.net 09.22.2017
			SItemGrid *pGridCont = pCSrcBag->GetGridContByID(sSrcGridID);
			if (g_CParser.DoString("OnBankItem", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCMainCha, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pGridCont, DOSTRING_PARAM_END)) {
				if(!g_CParser.GetReturnNumber(0))
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
		//sprintf(szMsg, "���в���[%s-->%s]������ %s[%u]������ %u.", chSrcType == 0 ? "������" : "����", chTarType == 0 ? "������" : "����", pItem->szName, sSrcItemID, sLeftNum);
		sprintf(szMsg, RES_STRING(GM_CHARACTERCMD_CPP_00012), chSrcType == 0 ? RES_STRING(GM_CHARACTERCMD_CPP_00013) : RES_STRING(GM_CHARACTERCMD_CPP_00014), chTarType == 0 ? RES_STRING(GM_CHARACTERCMD_CPP_00013) : RES_STRING(GM_CHARACTERCMD_CPP_00014), pItem->szName, sSrcItemID, sLeftNum);
		TL(CHA_BANK, szPlyName, "", szMsg);
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

void CCharacter::Cmd_ReassignAttr(RPACKET pk)
{
	m_CChaAttr.ResetChangeFlag();
	SetBoatAttrChangeFlag(false);

	const char chAttrNum = READ_CHAR(pk);
	if (chAttrNum <= 0 || chAttrNum > 6)
	{
		//SystemNotice("�ٷ������Եĸ�������");
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
	for (char i = 0; i <= chAttrNum; i++)
	{
		sAttrID = READ_SHORT(pk);
		if (sAttrID < ATTR_STR || sAttrID > ATTR_LUK)
			continue;
		sAttrOppID = sAttrID - ATTR_STR;
		lBaseAttr[sAttrOppID] = READ_LONG(pk);
		if (lBaseAttr[sAttrOppID] < 0) // Ҫ���õ�ֵС��0
		{
			bSetCorrect = false;
			break;
		}
		if (lBaseAttr[sAttrOppID] > lBaseAttrBalanceVal[sAttrOppID]) // �������ֵ
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
	g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
	if (GetPlayer())
	{
		GetPlayer()->RefreshBoatAttr();
		SyncBoatAttr(enumATTRSYN_REASSIGN);
	}

	SynAttr(enumATTRSYN_REASSIGN);
}

// �Ƴ�����
// lItemNum �Ƴ��� 0Ϊ��Ӧ������sFromID����ȫ����Ŀ
// chFromType �Ƴ���λ 1����װ�����Ƴ�.2���ӵ������Ƴ�.0����װ�����͵������Ƴ�
// sFromID �Ƴ����� -1Ϊ��Ӧ��λ��chFromType����ȫ������
// chToType Ŀ�겿λ 0���Ƴ�������.1����������.2��ɾ�����京��ͬCmd_UnfixItem������chDir������
// bForcible���Ƿ�ǿ���Ƴ�������������������ڲ��ܲ������ߵ�״̬ʱ��ʹ�ô˲����ɺ�����Щ����
Short CCharacter::Cmd_RemoveItem(Long lItemID, Long lItemNum, Char chFromType, Short sFromID, Char chToType, Short sToID, bool bRefresh, bool bForcible)
{
    
    
	if (!bForcible)
	{	
		//	2008-9-8	yangyinyu	add	begin!
		//	���	<�����к���ҵı���ʵ���������ģ��޷������κεĲ��������ڸù�����Ч������ҿ��Զ�����ɾ����Ʒ>	���е��޸ġ�
		if(	GetPlyMainCha()->m_CKitbag.IsLock()	)
			return	enumITEMOPT_ERROR_KBLOCK;
		//	2008-9-8	yangyinyu	add	end!

		if (GetPlyMainCha()->m_CKitbag.IsPwdLocked()) //��������
			 return enumITEMOPT_ERROR_KBLOCK;

		//add by ALLEN 2007-10-16
		if (GetPlyMainCha()->IsReadBook()) //����״̬
        return enumITEMOPT_ERROR_KBLOCK;

		if (!GetActControl(enumACTCONTROL_ITEM_OPT))
			return enumITEMOPT_ERROR_STATE;
		if (m_CKitbag.IsLock()) // ����������
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
	if (chFromType == 1 || chFromType == 0) // ��װ�����Ƴ�
	{
		Long	lParam1, lParam2;
		if (chToType == 0) // �Ƶ�����
		{
			GetTrowItemPos(&lParam1, &lParam2);
		}
		else if (chToType == 1) // �Ƶ�������
		{
			lParam1 = 0;
			lParam2 = -1;
		}
		else if (chToType == 2) // ɾ��
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
	if (chFromType == 2 || chFromType == 0) // �ӵ������Ƴ�
	{
		Long	lParam1, lParam2;
		if (sFromID >= 0 && sFromID < m_CKitbag.GetCapacity())
		{
			Short	sOptNum = (Short)lItemNum;
			if (chToType == 0) // �Ƶ�����
			{
				GetTrowItemPos(&lParam1, &lParam2);
				sOptRet = Cmd_ThrowItem(0, sFromID, &sOptNum, lParam1, lParam2, false, bForcible);
			}
			else if (chToType == 2) // ɾ��
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
						if (chToType == 0) // �Ƶ�����
						{
							GetTrowItemPos(&lParam1, &lParam2);
							sOptRet = Cmd_ThrowItem(0, sPosID, &sOptNum, lParam1, lParam2, false, bForcible);
						}
						else if (chToType == 2) // ɾ��
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
			GetPlyCtrlCha()->SkillRefresh(); // ���ܼ��
			GetPlyMainCha()->SynSkillBag(enumSYN_SKILLBAG_MODI);

			// ͬ��״̬
			SynSkillStateToEyeshot();

			// ֪ͨ��Ұ���������
			if (g_Config.m_bBlindChaos && IsPlayerCha() && IsPKSilver())
				SynLook(LOOK_SELF, true); // sync to self (item removed from inventory)
			else
				SynLook();

			// ���¼�������
			g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
			if (GetPlayer())
			{
				GetPlayer()->RefreshBoatAttr();
				SyncBoatAttr(enumATTRSYN_ITEM_EQUIP);
			}
			SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);
		}
		// ֪ͨ���ǵ���������
		SynKitbagNew(enumSYN_KITBAG_EQUIP);
	}

	return enumITEMOPT_SUCCESS;
}

// ��ս���󣨵�����������ս��
void CCharacter::Cmd_FightAsk(dbc::Char chType, dbc::Long lTarID, dbc::Long lTarHandle)
{
	CDynMapEntryCell	*pCTeamFightEntry = g_CDMapEntry.GetEntry(g_szTFightMapName);
	if (!pCTeamFightEntry) // û�����
	{
		//SystemNotice("����ͼ������PK���󣬻��Ӧ��ͼû������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00016));
		return;
	}

	Entity	*pCTarEnti = g_pGameApp->IsValidEntity(lTarID, lTarHandle);
	CCharacter	*pCTarCha;
	if (!pCTarEnti || !(pCTarCha = pCTarEnti->IsCharacter()))
	{
		//SystemNotice("������Ч!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00017));
		return;
	}
	if (!IsLiveing() || !pCTarCha->IsLiveing())
	{
		//SystemNotice("����״̬�����ܽ��в���!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00018));
		return;
	}
	SubMap	*pCMap = GetSubMap(), *pCTarMap = pCTarCha->GetSubMap();
	if (!pCMap || !(pCMap->GetAreaAttr(GetPos()) & enumAREA_TYPE_FIGHT_ASK))
	{
		//SystemNotice("�뵽ָ������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00019));
		return;
	}
	if (!pCTarMap || !(pCTarMap->GetAreaAttr(pCTarCha->GetPos()) & enumAREA_TYPE_FIGHT_ASK))
	{
		//SystemNotice("�Է���������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00020));
		return;
	}

	CPlayer	*pCPly = GetPlayer(), *pCTarPly = pCTarCha->GetPlayer();
	if (!pCPly || !pCTarPly)
	{
		//SystemNotice("����ҽ�ɫ!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00021));
		return;
	}

	if (!IsRangePoint(pCTarCha->GetPos(), 6 * 100))
	{
		//SystemNotice("�������뷶Χ!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00022));
		return;
	}
	if (pCPly->HasChallengeObj())
	{
		//SystemNotice("���Ѿ�������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00023));
		return;
	}
	if (pCTarPly->HasChallengeObj())
	{
		//SystemNotice("�Է��Ѿ�������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00024));
		return;
	}
	if (chType == enumFIGHT_TEAM)
	{
		if (pCPly->getTeamLeaderID() == 0)
		{
			//SystemNotice("�������ģʽ!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00025));
			return;
		}
		if (pCTarPly->getTeamLeaderID() == 0)
		{
			//SystemNotice("�Է��������ģʽ!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00026));
			return;
		}
	}
	else if (chType == enumFIGHT_MONOMER)
	{
		if (pCTarPly->getTeamLeaderID() != 0 && pCTarPly->getTeamLeaderID() == pCPly->getTeamLeaderID())
		{
			//SystemNotice("ͬһ���鲻�ܵ���PK!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00027));
			return;
		}
	}

	string	strScript = "check_can_enter_";
	strScript += pCTeamFightEntry->GetTMapName();
	if (g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, this, pCTeamFightEntry, DOSTRING_PARAM_END))
	{
		if (!g_CParser.GetReturnNumber(0))
			return;
	}
	if (g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pCTarCha, pCTeamFightEntry, DOSTRING_PARAM_END))
	{
		if (!g_CParser.GetReturnNumber(0))
		{
			//SystemNotice("�Է����ܽ�������!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00028));
			return;
		}
	}

	WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_TEAM_FIGHT_ASK);

	Char	chObjStart = 2;
	// ��������
	Char	chSrcObjNum = 0;
	pCPly->SetChallengeType(chType);
	pCPly->SetChallengeParam(chObjStart++, pCPly->GetID());
	pCPly->SetChallengeParam(chObjStart++, pCPly->GetHandle());
	pCPly->StartChallengeTimer();
	WRITE_STRING(WtPk, GetName());
	WRITE_CHAR(WtPk, (Char)getAttr(ATTR_LV));
	WRITE_STRING(WtPk, g_GetJobName((Short)getAttr(ATTR_JOB)));
	if (g_CParser.DoString("Get_ItemAttr_Join", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END))
		WRITE_SHORT(WtPk, g_CParser.GetReturnNumber(0));
	else
		WRITE_SHORT(WtPk, 0);
	if (g_CParser.DoString("Get_ItemAttr_Win", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END))
		WRITE_SHORT(WtPk, g_CParser.GetReturnNumber(0));
	else
		WRITE_SHORT(WtPk, 0);
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
				//pCTeamCha->SystemNotice("���Ѿ�������������!");
				pCTeamCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00023));
				continue;
			}
			if (!pCTeamCha->GetSubMap() || !(pCTeamCha->GetSubMap()->GetAreaAttr(pCTeamCha->GetPos()) & enumAREA_TYPE_FIGHT_ASK))
			{
				//pCTeamCha->SystemNotice("������ָ�������ڣ������ܽ�������!");
				pCTeamCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00029));
				continue;
			}
			if (g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pCTeamCha, pCTeamFightEntry, DOSTRING_PARAM_END))
			{
				if (!g_CParser.GetReturnNumber(0))
					continue;
			}

			pCTeamMem->SetChallengeType(chType);
			pCTeamMem->SetChallengeParam(0, 1);
			pCTeamMem->SetChallengeParam(2, pCPly->GetID());
			pCTeamMem->SetChallengeParam(3, pCPly->GetHandle());
			pCTeamMem->StartChallengeTimer();
			WRITE_STRING(WtPk, pCTeamCha->GetName());
			WRITE_CHAR(WtPk, (Char)pCTeamCha->getAttr(ATTR_LV));
			WRITE_STRING(WtPk, g_GetJobName((Short)pCTeamCha->getAttr(ATTR_JOB)));
			if (g_CParser.DoString("Get_ItemAttr_Join", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCTeamCha, DOSTRING_PARAM_END))
				WRITE_SHORT(WtPk, g_CParser.GetReturnNumber(0));
			else
				WRITE_SHORT(WtPk, 0);
			if (g_CParser.DoString("Get_ItemAttr_Win", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCTeamCha, DOSTRING_PARAM_END))
				WRITE_SHORT(WtPk, g_CParser.GetReturnNumber(0));
			else
				WRITE_SHORT(WtPk, 0);
			chSrcObjNum++;

			pCPly->SetChallengeParam(chObjStart++, pCTeamMem->GetID());
			pCPly->SetChallengeParam(chObjStart++, pCTeamMem->GetHandle());
		}
	}

	// �Է�����
	Char	chTarObjNum = 0;
	pCTarPly->SetChallengeType(chType);
	pCTarPly->SetChallengeParam(0, 1);
	pCTarPly->SetChallengeParam(2, pCPly->GetID());
	pCTarPly->SetChallengeParam(3, pCPly->GetHandle());
	pCTarPly->StartChallengeTimer();
	WRITE_STRING(WtPk, pCTarCha->GetName());
	WRITE_CHAR(WtPk, (Char)pCTarCha->getAttr(ATTR_LV));
	WRITE_STRING(WtPk, g_GetJobName((Short)pCTarCha->getAttr(ATTR_JOB)));
	if (g_CParser.DoString("Get_ItemAttr_Join", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCTarCha, DOSTRING_PARAM_END))
		WRITE_SHORT(WtPk, g_CParser.GetReturnNumber(0));
	else
		WRITE_SHORT(WtPk, 0);
	if (g_CParser.DoString("Get_ItemAttr_Win", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCTarCha, DOSTRING_PARAM_END))
		WRITE_SHORT(WtPk, g_CParser.GetReturnNumber(0));
	else
		WRITE_SHORT(WtPk, 0);
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
				//pCTeamCha->SystemNotice("���Ѿ�������������!");
				pCTeamCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00023));
				continue;
			}
			if (!pCTeamCha->GetSubMap() || !(pCTeamCha->GetSubMap()->GetAreaAttr(pCTeamCha->GetPos()) & enumAREA_TYPE_FIGHT_ASK))
			{
				//pCTeamCha->SystemNotice("�뵽ָ������������!");
				pCTeamCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00019));
				continue;
			}
			if (g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pCTeamCha, pCTeamFightEntry, DOSTRING_PARAM_END))
			{
				if (!g_CParser.GetReturnNumber(0))
					continue;
			}

			pCTeamMem->SetChallengeType(chType);
			pCTeamMem->SetChallengeParam(0, 1);
			pCTeamMem->SetChallengeParam(2, pCPly->GetID());
			pCTeamMem->SetChallengeParam(3, pCPly->GetHandle());
			pCTeamMem->StartChallengeTimer();
			WRITE_STRING(WtPk, pCTeamCha->GetName());
			WRITE_CHAR(WtPk, (Char)pCTeamCha->getAttr(ATTR_LV));
			WRITE_STRING(WtPk, g_GetJobName((Short)pCTeamCha->getAttr(ATTR_JOB)));
			if (g_CParser.DoString("Get_ItemAttr_Join", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCTeamCha, DOSTRING_PARAM_END))
				WRITE_SHORT(WtPk, g_CParser.GetReturnNumber(0));
			else
				WRITE_SHORT(WtPk, 0);
			if (g_CParser.DoString("Get_ItemAttr_Win", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCTeamCha, DOSTRING_PARAM_END))
				WRITE_SHORT(WtPk, g_CParser.GetReturnNumber(0));
			else
				WRITE_SHORT(WtPk, 0);
			chTarObjNum++;

			pCPly->SetChallengeParam(chObjStart++, pCTeamMem->GetID());
			pCPly->SetChallengeParam(chObjStart++, pCTeamMem->GetHandle());
		}
	}
	pCPly->SetChallengeParam(chObjStart++, pCTarPly->GetID());
	pCPly->SetChallengeParam(chObjStart++, pCTarPly->GetHandle());

	WRITE_CHAR(WtPk, chSrcObjNum);
	WRITE_CHAR(WtPk, chTarObjNum);
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

// ��սӦ��
void CCharacter::Cmd_FightAnswer(bool bFight)
{
	CPlayer	*pCPly = GetPlayer();
	if (!pCPly->HasChallengeObj())
	{
		//SystemNotice("û���ܹ�����!");
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
		//SystemNotice("�������Ѿ���Ч!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00031));
		return;
	}
	Char	chObjNum = (Char)pCSrcPly->GetChallengeParam(0);
	pCTarPly = g_pGameApp->IsValidPlayer(pCSrcPly->GetChallengeParam(2 + (chObjNum -1) * 2), pCSrcPly->GetChallengeParam(2 + (chObjNum -1) * 2 + 1));
	if (!pCTarPly || !pCTarPly->HasChallengeObj())
	{
		//SystemNotice("�������Ѿ���Ч!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00031));
		return;
	}

	// ȷ������
	bool	bHasPly = false;
	Char	chLoop = (Char)pCSrcPly->GetChallengeParam(0);
	if (chLoop > MAX_TEAM_MEMBER * 2)
		chLoop = MAX_TEAM_MEMBER * 2;
	for (Char i = 0; i < chLoop; i++)
	{
		if (pCSrcPly->GetChallengeParam(i * 2 + 2) == pCPly->GetID() && pCSrcPly->GetChallengeParam(i * 2 + 2 + 1) == pCPly->GetHandle())
			bHasPly = true;
	}
	if (!bHasPly) // PK��Ϣ���Ҳ��������
	{
		//SystemNotice("����������֮��!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00032));
		return;
	}

	if (!bFight)
	{
		std::string	strNoti = GetName();
		//strNoti += " ȡ����PK����!";
		strNoti += RES_STRING(GM_CHARACTERCMD_CPP_00033);
		WPACKET WtPk	= GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_SYSINFO);
		WRITE_SEQ(WtPk, strNoti.c_str(), (Short)strNoti.length() + 1 );
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
	if (pCSrcPly->GetChallengeParam(0) != pCSrcPly->GetChallengeParam(1)) // ����û��ͬ��PK���������
		return;

	CDynMapEntryCell	*pCTeamFightEntry = g_CDMapEntry.GetEntry(g_szTFightMapName);
	if (!pCTeamFightEntry) // û�ж���PK��ͼ
	{
		//std::string	strNoti = "����ͼ������PK���󣬻��Ӧ��ͼû������!";
		std::string	strNoti = RES_STRING(GM_CHARACTERCMD_CPP_00034);
		WPACKET WtPk	= GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_SYSINFO);
		WRITE_SEQ(WtPk, strNoti.c_str(), (Short)strNoti.length() + 1 );
		pCSrcPly->GetCtrlCha()->ReflectINFof(this, WtPk);
		if (lFightType == enumFIGHT_TEAM)
		{
			SENDTOCLIENT2(WtPk, pCPly->GetTeamMemberCnt(), pCPly->_Team);
			SENDTOCLIENT2(WtPk, pCSrcPly->GetTeamMemberCnt(), pCSrcPly->_Team);
		}

		pCSrcPly->ClearChallengeObj();
		return;
	}

	// ȡ�ø������������ʧ�����Ӧ�÷�������
	CMapEntryCopyCell	CMCpyCell(20, 0);
	CMapEntryCopyCell	*pCMCpyCell;
	if (!(pCMCpyCell = pCTeamFightEntry->AddCopy(&CMCpyCell)))
	{
		//std::string	strNoti = "��ǰû�п��г��أ����Ժ����!";
		std::string	strNoti = RES_STRING(GM_CHARACTERCMD_CPP_00035);
		WPACKET WtPk	= GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_SYSINFO);
		WRITE_SEQ(WtPk, strNoti.c_str(), (Short)strNoti.length() + 1 );
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
	if (!pCMCpyCell->HasFreePlyCount((Short)pCSrcPly->GetChallengeParam(0))) // ��������
	{
		//std::string	strNoti = "���󳡵��������������Ժ����!";
		std::string	strNoti = RES_STRING(GM_CHARACTERCMD_CPP_00036);
		WPACKET WtPk	= GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_SYSINFO);
		WRITE_SEQ(WtPk, strNoti.c_str(), (Short)strNoti.length() + 1 );
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
	g_CParser.DoString(strScript1.c_str(), enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 3, pCMCpyCell, pCSrcPly, pCTarPly, enumSCRIPT_PARAM_NUMBER, 1, lFightType, DOSTRING_PARAM_END);
	pCTeamFightEntry->SynCopyParam((Short)pCMCpyCell->GetPosID());
	// ��ʼ����PK��ͼ
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
			//pCFightCha->SystemNotice("����������״̬�������ܽ���PK");
			pCFightCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00037));
			continue;
		}
		if (!pCFightCha->GetSubMap() || !(pCFightCha->GetSubMap()->GetAreaAttr(pCFightCha->GetPos()) & enumAREA_TYPE_FIGHT_ASK))
		{
			//pCFightCha->SystemNotice("������PK�������ڣ������ܽ���PK");
			pCFightCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00038));
			continue;
		}
		if (g_CParser.DoString(strScript2.c_str(), enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pCFightCha, pCTeamFightEntry, DOSTRING_PARAM_END))
		{
			if (!g_CParser.GetReturnNumber(0))
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
				//pCFightCha->SystemNotice("���Ѿ�������ǰ��������飬�����ܽ���PK");
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
				//pCFightCha->SystemNotice("���Ѿ�������ǰ������֮�У������ܽ���PK");
				pCFightCha->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00040));
				continue;
			}
		}

		pCEnterCha[lEnterChaNum] = pCFightCha;
		lEnterChaNum++;
	}

	if (!(bSide1 & bSide2)) // һ���Ѿ���Ч
	{
		for (Long i = 0; i < lEnterChaNum; i++)
			//pCEnterCha[i]->SystemNotice("�Է��Ѿ�û����Ա�ɽ���PK!");
			pCEnterCha[i]->SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00041));
		pCTeamFightEntry->ReleaseCopy(pCMCpyCell);
		return;
	}
	for (Long i = 0; i < lEnterChaNum; i++)
		g_CParser.DoString(strScript.c_str(), enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, pCEnterCha[i], pCMCpyCell, DOSTRING_PARAM_END);
	pCMCpyCell->AddCurPlyNum((Short)lEnterChaNum);

	// temp for test
	//std::string	strPrint = "������ţ�";
	std::string	strPrint = RES_STRING(GM_CHARACTERCMD_CPP_00042);
	Char	szPrint[10];
	itoa(pCMCpyCell->GetPosID(), szPrint, 10);
	strPrint += szPrint;
	strPrint += ".";
	//strPrint += "����������";
	strPrint += RES_STRING(GM_CHARACTERCMD_CPP_00043);
	itoa(lEnterChaNum, szPrint, 10);
	strPrint += szPrint;
	strPrint += ".";
	//LG("��ڸ�������", "%s\n", strPrint.c_str());
	LG("entrance copy control", "%s\n", strPrint.c_str());
	//
	pCTeamFightEntry->SynCopyRun((Short)pCMCpyCell->GetPosID(), enumMAPCOPY_START_CDT_PLYNUM, lEnterChaNum);

	return;
}

// ������������
void CCharacter::Cmd_ItemRepairAsk(dbc::Char chPosType, dbc::Char chPosID)
{
	CPlayer	*pCPly = GetPlayer();
	if (!pCPly)
		return;
	if (pCPly->IsInRepair())
	{
		//SystemNotice("�Ѿ���������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00044));
		return;
	}

	CCharacter	*pCRepairman = pCPly->GetRepairman();
	if (!pCRepairman)
		return;
	if (!IsRangePoint(pCRepairman->GetPos(), 6 * 100))
	{
		//SystemNotice("����̫Զ!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00045));
		return;
	}
	if (!pCPly->SetRepairPosInfo(chPosType, chPosID))
	{
		//SystemNotice("��Ч�ĵ���!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00046));
		return;
	}
	CItemRecord	*pCItemRec = GetItemRecordInfo(pCPly->GetRepairItem()->sID);
	if (!pCItemRec)
	{
		//SystemNotice("��Ч�ĵ���!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00046));
		return;
	}
	if (g_CParser.StringIsFunction("can_repair_item"))
	{
		g_CParser.DoString("can_repair_item", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 3, pCRepairman, this, pCPly->GetRepairItem(), DOSTRING_PARAM_END);
		if (g_CParser.GetReturnNumber(0) == 0)
			return;
	}
	if (!g_CParser.StringIsFunction("get_item_repair_money"))
	{
		//SystemNotice("δ֪����������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00047));
		return;
	}
	g_CParser.DoString("get_item_repair_money", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pCPly->GetRepairItem(), DOSTRING_PARAM_END);

	WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_ITEM_REPAIR_ASK);
	WRITE_STRING(WtPk, pCItemRec->szName);
	WRITE_LONG(WtPk, g_CParser.GetReturnNumber(0));
	ReflectINFof(this, WtPk);

	pCPly->SetInRepair();
}

// ��������Ӧ��
void CCharacter::Cmd_ItemRepairAnswer(bool bRepair)
{
	CPlayer	*pCPly = GetPlayer();
	if (!pCPly)
		return;
	if (!pCPly->IsInRepair())
	{
		//SystemNotice("û���κ�����!");
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
			//SystemNotice("����̫Զ!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00045));
			goto EndItemRepair;
		}
		if (!pCPly->CheckRepairItem())
		{
			//SystemNotice("�����Ѿ��䶯�����ܼ�������!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00049));
			goto EndItemRepair;
		}
		if (g_CParser.StringIsFunction("can_repair_item"))
		{
			g_CParser.DoString("can_repair_item", enumSCRIPT_RETURN_NUMBER, 1, enumSCRIPT_PARAM_LIGHTUSERDATA, 3, pCRepairman, this, pCPly->GetRepairItem(), DOSTRING_PARAM_END);
			if (g_CParser.GetReturnNumber(0) == 0)
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
		if (g_CParser.StringIsFunction("begin_repair_item"))
		{
			g_CParser.DoString("begin_repair_item", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 3, pCRepairman, this, pCPly->GetRepairItem(), DOSTRING_PARAM_END);
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
				g_CParser.DoString("AttrRecheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
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

// ���߾�������
void CCharacter::Cmd_ItemForgeAsk(dbc::Char chType, SForgeItem *pSItem)
{
	CPlayer	*pCPly = GetPlayer();

    if(m_CKitbag.IsPwdLocked())
    {
        //SystemNotice("������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		goto EndItemForgeAsk;
    }

	//add by ALLEN 2007-10-16
	if(IsReadBook())
    {
       // SystemNotice("����״̬�����ܾ���!");
		 SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00051));
		goto EndItemForgeAsk;
    }

	if (!pSItem)
		goto EndItemForgeAsk;

	if (pCPly->IsInForge())
	{
		//SystemNotice("֮ǰ������û�����!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00052));
		goto EndItemForgeAsk;
	}

	if( pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData() )
	{
		//SystemNotice("����ʧ��!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00053));
		return;
	}

	{
	CCharacter	*pCForgeman = pCPly->GetForgeman();
	if (!pCForgeman)
		goto EndItemForgeAsk;
	if (!IsRangePoint(pCForgeman->GetPos(), 6 * 100))
	{
		//SystemNotice("����̫Զ!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00045));
		goto EndItemForgeAsk;
	}

	if (!CheckForgeItem(pSItem))
	{
		//SystemNotice("�ύ��������ӵ�е��߲����ϣ����󱻾ܾ�!");
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
		if (chType == 1) // ����
		{
			szCheckCanScript = "can_forge_item";
			szGetMoneyScript = "get_item_forge_money";
		}
		else if (chType == 2) // �ϳ�
		{
			szCheckCanScript = "can_unite_item";
			szGetMoneyScript = "get_item_unite_money";
		}
		else if (chType == 3) // ��ĥ
		{
			szCheckCanScript = "can_milling_item";
			szGetMoneyScript = "get_item_milling_money";
		}
		else if (chType == 4) // �ۺ�
		{
			szCheckCanScript = "can_fusion_item";
			szGetMoneyScript = "get_item_fusion_money";
		}
		else if (chType == 5) // װ������
		{
			szCheckCanScript = "can_upgrade_item";
			szGetMoneyScript = "get_item_upgrade_money";
		}
		else if (chType == 6) // ����ת��
		{
			szCheckCanScript = "can_jlborn_item";
			szGetMoneyScript = "get_item_jlborn_money";
		}
		else if (chType == 7) // װ���ᴿ
		{
			szCheckCanScript = "can_tichun_item";
			szGetMoneyScript = "get_item_tichun_money";
		}
		else if (chType == 8) // ���ǳ��
		{
			szCheckCanScript = "can_energy_item";
			szGetMoneyScript = "get_item_energy_money";
		}
		else if (chType == 9) // ��ȡ��ʯ
		{
			szCheckCanScript = "can_getstone_item";
			szGetMoneyScript = "get_item_getstone_money";
		}
		else if (chType == 10) // �����ƹ�
		{
			szCheckCanScript = "can_shtool_item";
			szGetMoneyScript = "get_item_shtool_money";
		}
		else
		{
			//SystemNotice( "�������ʹ���!%d", chType );
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00055), chType);
			return;
		}

		Long	lCheckCan;
		if (!DoForgeLikeScript(szCheckCanScript, lCheckCan))
		{
			//SystemNotice("���ܲ��������󱻾ܾ�!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
			goto EndItemForgeAsk;
		}
		if (lCheckCan == 0)
			goto EndItemForgeAsk;

		{
			Long	lOptMoney;
			if (!DoForgeLikeScript(szGetMoneyScript, lOptMoney))
			{
				//SystemNotice("���ܲ��������󱻾ܾ�!");
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
				WPACKET WtPk = GETWPACKET();
				WRITE_CMD(WtPk, CMD_MC_ITEM_FORGE_ASK);
				WRITE_CHAR(WtPk, chType);
				WRITE_LONG(WtPk, lOptMoney);
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
	WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_ITEM_FORGE_ASR);
	WRITE_CHAR(WtPk, chType);
	WRITE_CHAR(WtPk, 0);
	ReflectINFof(this, WtPk);

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
				pItemLottery->sInstAttr[0][1] = 10000 + issue;	// λ4����ɣ� λ1-λ3���ںţ�
				time_t t = time(0);
				tm* nowTm = localtime(&t);

				pItemLottery->sInstAttr[1][0] = ITEMATTR_VAL_AGI;
				pItemLottery->sInstAttr[1][1] = nowTm->tm_mday * 100 + nowTm->tm_hour;	// λ1λ2��ʱ�䣩 λ3λ4�����ڣ�		22��10��

				// ����0����Ϊ1������1����Ϊ2���Դ����� ����X����Ϊ11
				pItemLottery->sInstAttr[2][0] = ITEMATTR_VAL_DEX;
				pItemLottery->sInstAttr[2][1] = buffer[0][0] * 100 + buffer[1][0];	// λ1λ2����1�� λ3λ4����2��	
				pItemLottery->sInstAttr[3][0] = ITEMATTR_VAL_CON;
				pItemLottery->sInstAttr[3][1] = buffer[2][0] * 100 + buffer[3][0];	// λ1λ2����3�� λ3λ4����4��	
				pItemLottery->sInstAttr[4][0] = ITEMATTR_VAL_STA;
				pItemLottery->sInstAttr[4][1] = buffer[4][0] * 100 + buffer[5][0];	// λ1λ2����5�� λ3λ4����6��	

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
	WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_ITEM_LOTTERY_ASR);
	WRITE_CHAR(WtPk, 0);
	ReflectINFof(this, WtPk);
}

void CCharacter::Cmd_ItemLotteryAnswer(bool bLottery)
{
	SynKitbagNew(enumSYN_KITBAG_EQUIP);
	SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);

	WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_ITEM_LOTTERY_ASR);
	WRITE_CHAR(WtPk, 0);
	ReflectINFof(this, WtPk);
}
// End

void CCharacter::Cmd_LifeSkillItemAsk(long dwType, SLifeSkillItem *pSItem)
{
	if(m_CKitbag.IsPwdLocked())
	{
		//SystemNotice("���������������ܽ�����ز���.");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00057));
		return;
	}

	//add by ALLEN 2007-10-16
	if(IsReadBook())
	{
		//SystemNotice("����״̬�����ܽ�����ز���.");
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
		//SystemNotice("������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		goto EndItemForgeAsk;
	}

	if (!pSItem)
	{
		goto EndItemForgeAsk;
	}


	if( pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData() )
	{
		//SystemNotice("����ʧ��!");
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
			//SystemNotice( "�������ʹ���!%d", dwType );
			SystemNotice( RES_STRING(GM_CHARACTERCMD_CPP_00055), dwType );
			goto EndItemForgeAsk;
			break;
		}

	}

	if (!DoLifeSkillcript(szCheckCanScript, lCheckCan))
	{
		//SystemNotice("���ܲ��������󱻾ܾ�!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
		goto EndItemForgeAsk;
	}
	if (lCheckCan == 0)
	{
		goto EndItemForgeAsk;
	}
	if (!DoLifeSkillcript(szGetMoneyScript, lOptMoney))
	{
		//SystemNotice("���ܲ��������󱻾ܾ�!");
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
	WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_LIFESKILL_ASK);
	WRITE_LONG(WtPk, dwType);
	WRITE_SHORT(WtPk, (short)lOptMoney);
	if(1==dwType)
	{
		string	strVer[2];
		Util_ResolveTextLine(strLifeSkillinfo.c_str(),strVer,2,',');
		WRITE_STRING(WtPk,strVer[1].c_str());
	}
	else
		WRITE_STRING(WtPk,strLifeSkillinfo.c_str());
	ReflectINFof(this, WtPk);
}

void CCharacter::Cmd_LifeSkillItemAsR(long dwType, SLifeSkillItem *pSItem)
{
	if(m_CKitbag.IsPwdLocked())
	{
		//SystemNotice("���������������ܽ�����ز���.");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00057));
		return;
	}
	//add by ALLEN 2007-10-16
	if(IsReadBook())
	{
		//SystemNotice("����״̬�����ܽ�����ز���.");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00058));
		return;
	}

	CPlayer	*pCPly = GetPlayer();
	if(pCPly == NULL || pSItem == NULL)
		return;
	long lCheckCan = 0,lOptMoney =-1;

	if (pCPly->IsInLifeSkill())
	{
		//SystemNotice("֮ǰ������û�����!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00052));
		return;
	}

	if(m_CKitbag.IsPwdLocked())
	{
		//SystemNotice("������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		return;
	}

	if (!pSItem)
	{
		//SystemNotice("������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		return;
	}


	if( pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData() )
	{
		SystemNotice("����ʧ��!");
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
		SystemNotice("begin_manufacture_item��������");
		return ;
	}
	int	nParamNum = 0;
	int nRetNum = 1;

	lua_pushlightuserdata(g_pLuaState, this);
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
		LG("lua_err", "DoString %s\n", cszFunc);
		lua_callalert(g_pLuaState, nState);
		lua_settop(g_pLuaState, 0);
		return ;
	}


	short stime = (short)lua_tonumber(g_pLuaState, -2);
	//---------
	//char time[20];
	//sprintf(time,"time == %d",stime);
	//SystemNotice(time);
	//----------
	const char * cszContent = lua_tostring(g_pLuaState,-1);
	string& strLifeSkillinfo = GetPlayer()->GetLifeSkillinfo();
	strLifeSkillinfo = cszContent;
	lua_settop(g_pLuaState, 0);
		
	if(stime != 0)
		pCPly->SetInLifeSkill();

	WPACKET	l_wpk =GETWPACKET();
	WRITE_CMD(l_wpk,CMD_MC_LIFESKILL_ASR);
	WRITE_LONG(l_wpk,dwType);
	WRITE_SHORT(l_wpk,stime);
	if(1==dwType)
	{
		string	strVer[2];
		Util_ResolveTextLine(cszContent,strVer,2,',');
		WRITE_STRING(l_wpk,strVer[0].c_str());
	}
	else
		WRITE_STRING(l_wpk,"");
	ReflectINFof(this,l_wpk);
}
//��������
void CCharacter::Cmd_LockKitbag()
{
    Char sState;//0:δ���� 1:������
    cChar *szPwd = GetPlayer()->GetPassword();
    if(!m_CKitbag.IsPwdLocked()){ 
		m_CKitbag.PwdLock();
    }
    sState = m_CKitbag.IsPwdLocked() ? 1 : 0;

    WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_KITBAG_CHECK_ASR);
    WRITE_CHAR(WtPk, sState);
	ReflectINFof(this, WtPk);
}

//��������
void CCharacter::Cmd_UnlockKitbag( const char szPassword[] )
{
    Char sState;//0:δ���� 1:������

    CPlayer	*pCply = GetPlayer();
    cChar *szPwd2 = pCply->GetPassword();

    if((szPwd2[0] == 0) || (!strcmp(szPassword, szPwd2)))
    {
        m_CKitbag.PwdUnlock();
    }
    sState = m_CKitbag.IsPwdLocked() ? 1 : 0;

    WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_KITBAG_CHECK_ASR);
    WRITE_CHAR(WtPk, sState);
	ReflectINFof(this, WtPk);
}

//��鱳��״̬
void CCharacter::Cmd_CheckKitbagState()
{
    Char sState;//0:δ���� 1:������
    sState = m_CKitbag.IsPwdLocked() ? 1 : 0;

    WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_KITBAG_CHECK_ASR);
    WRITE_CHAR(WtPk, sState);
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

// ���߾���Ӧ��
void CCharacter::Cmd_ItemForgeAnswer(bool bForge)
{
	CPlayer	*pCPly = GetPlayer();
	Char	chType = pCPly->GetForgeType();

    if(m_CKitbag.IsPwdLocked())
    {
        //SystemNotice("������������!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00050));
		goto EndItemForge;
    }

	//add by ALLEN 2007-10-16
	if(IsReadBook())
    {
        //SystemNotice("����״̬�����ܽ�����ز���!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00058));
		goto EndItemForge;
    }

    if (!pCPly->IsInForge())
	{
		//SystemNotice("û���κ�����!");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00059));
		goto EndItemForge;
	}

	if( pCPly->GetStallData() || pCPly->GetMainCha()->GetTradeData() )
	{
		//SystemNotice("����ʧ��!");
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
			//SystemNotice("����̫Զ!");
			SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00045));
			goto EndItemForge;
		}

		if (bForge)
		{
			if (!CheckForgeItem())
			{
				//SystemNotice("�ύ��������ӵ�е��߲����ϣ����󱻾ܾ�!");
				SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00054));
				goto EndItemForge;
			}

			const char* szCheckCanScript;
			const char* szBeginScript;
			if (chType == 1) // ����
			{
				szCheckCanScript = "can_forge_item";
				szBeginScript = "begin_forge_item";
			}
			else if (chType == 2) // �ϳ�
			{
				szCheckCanScript = "can_unite_item";
				szBeginScript = "begin_unite_item";
			}
			else if (chType == 3) // ��ĥ
			{
				szCheckCanScript = "can_milling_item";
				szBeginScript = "begin_milling_item";
			}
			else if (chType == 4) // �ۺ�
			{
				szCheckCanScript = "can_fusion_item";
				szBeginScript = "begin_fusion_item";
			}
			else if (chType == 5) // װ������
			{
				szCheckCanScript = "can_upgrade_item";
				szBeginScript = "begin_upgrade_item";
			}
			else if (chType == 6) // ����ת��
			{
				szCheckCanScript = "can_jlborn_item";
				szBeginScript = "begin_jlborn_item";
			}
			else if (chType == 7) // װ���ᴿ
			{
				szCheckCanScript = "can_tichun_item";
				szBeginScript = "begin_tichun_item";
			}
			else if (chType == 8) // ���ǳ��
			{
				szCheckCanScript = "can_energy_item";
				szBeginScript = "begin_energy_item";
			}
			else if (chType == 9) // ��ȡ��ʯ
			{
				szCheckCanScript = "can_getstone_item";
				szBeginScript = "begin_getstone_item";
			}
			else if (chType == 10) // �����ƹ�
			{
				szCheckCanScript = "can_shtool_item";
				szBeginScript = "begin_shtool_item";
			}
			else
			{
				//SystemNotice( "�������ʹ���!%d", chType );
				SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00055), chType);
				return;
			}

			Long	lCheckCan;
			if (!DoForgeLikeScript(szCheckCanScript, lCheckCan))
			{
				//SystemNotice("���ܲ��������󱻾ܾ�!");
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
				//SystemNotice("���ܲ��������󱻾ܾ�!");
				SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00056));
				goto EndItemForge;
			}
			SynKitbagNew(enumSYN_KITBAG_EQUIP);
			SynAttrToSelf(enumATTRSYN_ITEM_EQUIP);
			if (lBegin == 0)
				goto EndItemForge;
			else
			{
				WPACKET WtPk = GETWPACKET();
				WRITE_CMD(WtPk, CMD_MC_ITEM_FORGE_ASR);
				WRITE_LONG(WtPk, GetID());
				WRITE_CHAR(WtPk, chType);
				WRITE_CHAR(WtPk, (Char)lBegin);
				NotiChgToEyeshot(WtPk);

				pCPly->SetInForge(false);
				ForgeAction(false);
				return;
			}
		}
	}

EndItemForge:
	WPACKET WtPk	= GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_ITEM_FORGE_ASR);
	WRITE_CHAR(WtPk, chType);
	WRITE_CHAR(WtPk, 0);
	ReflectINFof(this, WtPk);

	pCPly->SetInForge(false);
	ForgeAction(false);
}
void CCharacter::Cmd_Garner2_Reorder(short index)
{
	SItemGrid *pSGridCont =  m_CKitbag.GetGridContByID(index);
	if(3849 != pSGridCont->sID)
	{
		//SystemNotice("�Բ���,��û������֤֮,��ȥ�Ҷ�����Ա����ȡ����֤֮.");
		SystemNotice(RES_STRING(GM_CHARACTERCMD_CPP_00061));
	}
	else
	{
		WPACKET pk	=GETWPACKET();
		WRITE_CMD(pk,CMD_MP_GARNER2_UPDATE);
		WRITE_LONG(pk,GetPlayer()->GetDBChaId());
		WRITE_STRING(pk,GetName());
		WRITE_LONG(pk,GetLevel());
		WRITE_STRING(pk,g_szJobName[getAttr( ATTR_JOB )]);
		WRITE_SHORT(pk,pSGridCont->GetInstAttr(ITEMATTR_MAXENERGY));
		ReflectINFof(this,pk);
	}
}