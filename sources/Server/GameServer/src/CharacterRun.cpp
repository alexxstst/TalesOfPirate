#include "stdafx.h"
#include "Character.h"
#include "Player.h"
#include "GameDB.h"
#include "GameApp.h"
#include "SubMap.h"

//----------------------------------------------
//       ����Character�����Ķ�ʱѭ������
//----------------------------------------------

// ��ѭ�����
void CCharacter::Run(DWORD dwCurTime)
{
		MPTimer	t;
	Char	chCount = 0;

	t.Begin();

	if (m_pCPlayer && !m_pCPlayer->IsValid())
		return;
	if (!GetSubMap())
		return;

	bool	bIsLiveing = IsLiveing();

	extern CGameApp* g_pGameApp;
	g_pGameApp->m_dwRunStep = 1000 + m_ID;

	m_dwCellRunTime[chCount++] = t.End();

	// ����(���������)����ʱ����
	if (!IsPlayerCha() && !IsNpc())
	{
		if (CheckLifeTime()) // ʱ�䵽
		{
			if (m_HostCha && m_HostCha->IsPlayerCha())
			{
				int nPetNum = m_HostCha->GetPlyMainCha()->GetPetNum();
				if (nPetNum > 0)
					m_HostCha->GetPlyMainCha()->SetPetNum(nPetNum - 1);
			}
			// ����һ���ű���Ϊ�¼�֪ͨ 
			g_CParser.DoString("event_cha_lifetime", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, DOSTRING_PARAM_END);
			Free(); // �ͷ�, ��Ұ֪ͨ
			// char szLua[255];
			// lua_dostring(g_pLuaState, szLua);
			return;
		}
	}

	//�������ʱ����
	/*if(IsPlayerCha() && !IsGMCha2() && ((!(GetAreaAttr() & enumAREA_TYPE_NOT_FIGHT)) || IsBoat()) && !GetPlyCtrlCha()->GetSubMap()->GetMapRes()->CanPK())
	{
		GetPlyMainCha()->CheatRun(dwCurTime);
	}*/

	//add by jilinlee 2007/4/20
	//�Ƿ��ڶ���״̬
	if (IsReadBook())
	{
		if (bIsLiveing)
		{
			if (m_SReadBook.dwLastReadCallTick == 0)
			{
				m_SReadBook.dwLastReadCallTick = dwCurTime;
			}

			static DWORD dwReadBookTime = 0;
			if (dwReadBookTime == 0 && g_CParser.DoString("ReadBookTime", enumSCRIPT_RETURN_NUMBER, 1, DOSTRING_PARAM_END))
			{
				dwReadBookTime = g_CParser.GetReturnNumber(0);
			}
			//else 
			//	dwReadBookTime = 600*1000;   //ȡ�����Ļ���Ĭ��Ϊʮ���ӡ�
			if (dwCurTime - m_SReadBook.dwLastReadCallTick >= dwReadBookTime)
			{
				//���ýű�����
				char chSkillLv = 0;
				static short sSkillID = 0;
				if (sSkillID == 0 && g_CParser.DoString("ReadBookSkillId", enumSCRIPT_RETURN_NUMBER, 1, DOSTRING_PARAM_END))
				{
					sSkillID = g_CParser.GetReturnNumber(0);
				}
				SSkillGrid* pSkill = this->m_CSkillBag.GetSkillContByID(sSkillID); //���鼼�ܵļ���ID
				if (pSkill)
				{
					chSkillLv = pSkill->chLv;
					g_CParser.DoString("Reading_Book", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, enumSCRIPT_PARAM_NUMBER, 1, chSkillLv, DOSTRING_PARAM_END);
				}
				m_SReadBook.dwLastReadCallTick = dwCurTime;
			}
		}
		else
			SetReadBookState(FALSE);

	}

	t.Begin();
	if (bIsLiveing)
		m_CActCache.Run();
	m_dwCellRunTime[chCount++] = t.End();

	t.Begin();
	if (IsPlayerCha())
	{
		if (IsOfflineMode())
		{
			if (GetTickCount() > _dwStallTick)
			{
				_dwStallTick = 0;
				//TODO(Ogge): Lets call TM_KICKCHA instead?
				g_pGameApp->ReleaseGamePlayer(GetPlayer());
				return;
			}
		}

		DWORD	dwResumeExecTime = m_timerScripts.IsOK(dwCurTime);
		if (dwResumeExecTime > 0 && !IsOfflineMode())
		{
			OnScriptTimer(dwResumeExecTime, true);
		}
	}
	m_dwCellRunTime[chCount++] = t.End();

	t.Begin();
	if (IsPlayerOwnCha())
		GetPlayer()->Run(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();

	// ��ʱ�����
	t.Begin();
	if (m_timerAI.IsOK(dwCurTime))         OnAI(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();
	t.Begin();
	if (m_timerAreaCheck.IsOK(dwCurTime))  OnAreaCheck(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();
	t.Begin();
	if (m_timerDie.IsOK(dwCurTime))        OnDie(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();
	t.Begin();
	if (m_timerMission.IsOK(dwCurTime))	   OnMissionTime();
	m_dwCellRunTime[chCount++] = t.End();
	t.Begin();
	if (m_timerTeam.IsOK(dwCurTime))       OnTeamNotice(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();

	t.Begin();
	if (bIsLiveing)
		if (m_timerSkillState.IsOK(dwCurTime)) OnSkillState(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();
	t.Begin();
	if (bIsLiveing)
		OnMove(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();
	t.Begin();
	if (bIsLiveing)
		OnFight(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();
	t.Begin();
	if (bIsLiveing)
		if (m_timerDBUpdate.IsOK(dwCurTime))   OnDBUpdate(dwCurTime);
	m_dwCellRunTime[chCount++] = t.End();

	t.Begin();
	if (IsPlayerCtrlCha())
	{
		if (m_timerPing.IsOK(dwCurTime))
			CheckPing();

		if (m_timerNetSendFreq.IsOK(dwCurTime) && m_ulNetSendLen > 0)
		{
			WPACKET WtPk = GETWPACKET();
			WRITE_CMD(WtPk, 0xffff);
			for (uLong i = 0; i < m_ulNetSendLen; i++)
				WRITE_CHAR(WtPk, rand() / 255);
			ReflectINFof(this, WtPk);
		}
	}
	m_dwCellRunTime[chCount++] = t.End();

}

void CCharacter::RunEnd(DWORD dwCurTime)
{
		if (m_byExit == CHAEXIT_BEGIN && m_timerExit.IsOK(dwCurTime))
		{
			// ��ʽ�˳�
			Exit();
		}
}

void CCharacter::StartExit()
{
		//LG( "��ʱ�˳�", "StartExit: Name = %s,exitcode = %d\n", this->GetName(), m_byExit );
		ToLogService("common", "StartExit: Name = {},exitcode = {}", this->GetName(), m_byExit);
	if (m_byExit != CHAEXIT_BEGIN)
	{
		DWORD dwExitTime = 20 * 1000;
		m_byExit = CHAEXIT_BEGIN;
		m_timerExit.Begin(dwExitTime);

		WPACKET	l_wpk = GETWPACKET();
		WRITE_CMD(l_wpk, CMD_MC_STARTEXIT);
		WRITE_LONG(l_wpk, dwExitTime);
		ReflectINFof(this, l_wpk);
	}
}

void CCharacter::CancelExit()
{
		//LG( "��ʱ�˳�", "CancelExit: Name = %s,exitcode = %d\n", this->GetName(), m_byExit );
		ToLogService("common", "CancelExit: Name = {},exitcode = {}", this->GetName(), m_byExit);
	if (m_byExit == CHAEXIT_BEGIN)
	{
		m_byExit = CHAEXIT_NONE;
		m_timerExit.Reset();

		WPACKET	l_wpk = GETWPACKET();
		WRITE_CMD(l_wpk, CMD_MC_CANCELEXIT);
		ReflectINFof(this, l_wpk);
	}
}

void CCharacter::Exit()
{
		// ��ʽ�˳�
		//LG( "��ʱ�˳�", "Exit: Name = %s, exitcode = %d\n", this->GetName(), m_byExit );
		ToLogService("common", "Exit: Name = {}, exitcode = {}", this->GetName(), m_byExit);
	WPACKET	l_wpk = GETWPACKET();
	WRITE_CMD(l_wpk, CMD_MT_PALYEREXIT);
	ReflectINFof(this, l_wpk);
	g_pGameApp->GoOutGame(this->GetPlayer(), true);

	m_byExit = CHAEXIT_NONE;
	m_timerExit.Reset();
}

// ��ʱ������仯
void CCharacter::OnAreaCheck(DWORD dwCurTime)
{
}

void CCharacter::OnDBUpdate(DWORD dwCurTime)
{
		CPlayer* pCPlayer = GetPlayer();
	if (!pCPlayer)
		return;
	if (!pCPlayer->IsPlayer() || pCPlayer->GetMainCha() != this)
		return;

	ToLogService("map", "OnDBUpdate start!");
	game_db.SavePlayer(pCPlayer, enumSAVE_TYPE_TIMER);
	ToLogService("map", "OnDBUpdate end!");
}

BOOL CCharacter::SaveMissionData()
{
		CPlayer* pCPlayer = GetPlayer();
	if (!pCPlayer) return FALSE;
	if (!game_db.SaveMissionData(pCPlayer, pCPlayer->GetDBChaId()))
	{
		//SystemNotice( "�½���ɫ��%s���洢��ɫ��ʼ������Ϣʧ�ܣ�ID[0x%X]", this->GetName(), pCPlayer->GetDBChaId() );
		SystemNotice(RES_STRING(GM_CHARACTERRUN_CPP_00001), this->GetName(), pCPlayer->GetDBChaId());
		return FALSE;
	}
	return TRUE;
}

void CCharacter::OnTeamNotice(DWORD dwCurTime)
{
		CPlayer* pCPlayer = GetPlayer();
	if (!pCPlayer)	return;

	pCPlayer->NoticeTeamMemberData();
}

// �ű���ʱ��������HP�ظ����������ĵ�
void CCharacter::OnScriptTimer(DWORD dwExecTime, bool bNotice)
{
		if (!IsPlayerCha())
			return;

	Long	lOldHP = (long)getAttr(ATTR_HP);
	m_CChaAttr.ResetChangeFlag();
	if (IsPlayerCha())
		m_CKitbag.SetChangeFlag(false);
	g_CParser.DoString("cha_timer", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, this, enumSCRIPT_PARAM_NUMBER, 2, defCHA_SCRIPT_TIMER / 1000, dwExecTime, DOSTRING_PARAM_END);

	// ���ں���ʱ����������
	if (lOldHP > 0 && getAttr(ATTR_HP) <= 0)
	{
		if (IsBoat() && IsPlayerCha())
		{
			SetItemHostObj(0);
			ItemCount(this);
			SetDie(g_pCSystemCha);
			Die();
			GetPlayer()->GetMainCha()->BoatDie(*this, *this);
		}
	}

	if (bNotice)
	{
		SynAttr(enumATTRSYN_AUTO_RESUME);
		if (IsPlayerCha())
			SynKitbagNew(enumSYN_KITBAG_ATTR);
	}
}
