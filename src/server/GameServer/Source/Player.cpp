//=============================================================================
// FileName: Player.cpp
// Creater: ZhangXuedong
// Date: 2004.10.19
// Comment: CPlayer class
//=============================================================================
#include "stdafx.h"
#include "Player.h"

#include "GameApp.h"
#include "GameDB.h"
#include "CharStall.h"

using namespace std;

_DBC_USING;

CPlayer::CPlayer():
m_sGarnerWiner(0)
{
    Init(NULL, 0);
	m_chGMLev = 0;
	m_dwValidFlag = PLAYER_INVALID_FLAG;

	m_CMapMask.AddMap("garner", defMAP_GARNER_WIDTH, defMAP_GARNER_HEIGHT);
	m_CMapMask.AddMap("magicsea", defMAP_MAGICSEA_WIDTH, defMAP_MAGICSEA_HEIGHT);
	m_CMapMask.AddMap("darkblue", defMAP_DARKBLUE_WIDTH, defMAP_DARKBLUE_HEIGHT);

	// Add by lark.li 20080812 begin
	m_CMapMask.AddMap("winterland", defMAP_DARKBLUE_WIDTH, defMAP_DARKBLUE_HEIGHT);
	// End

	//m_CMapMask.AddMap("eastgoaf", defMAP_EASTGOAF_WIDTH, defMAP_EASTGOAF_HEIGHT);
	//m_CMapMask.AddMap("lonetower", defMAP_LONETOWER_WIDTH, defMAP_LONETOWER_HEIGHT);
	m_lLightSize = g_lDeftMMaskLight;

	//
	m_lMoBean = 0;
	m_lRplMoney = 0;
	m_lVipID = 0;
	m_strLifeSkillinfo = "";
	// Modify by lark.li 20080317
	memset(this->m_szPassword, 0 , sizeof(this->m_szPassword));
	//End
}

void CPlayer::Initially()
{
	bIsValid = true;
    Init(NULL, 0);
    _nTeamMemberCnt = 0;
	m_chGMLev = 0;	
	_dwTeamLeaderID = 0;

	m_szPassword[0] = 0;
	m_dwDBActId = 0;
	m_pCtrlCha = 0;
	m_pMainCha = NULL;
	m_dwLaunchID = -1;
	m_sBerthID = -1;
	m_sxPos = 0;
	m_syPos = 0;
	m_sDir = 0;	
	m_byNumBoat = 0;
	memset( m_Boat, 0, sizeof(CCharacter*)*MAX_CHAR_BOAT );

	m_ulLoginCha[0] = enumLOGIN_CHA_MAIN;
	SetMapMaskDBID(0);
	ResetMapMaskChange();
	m_chBankNum = 0;
	m_pCBankNpc = 0;
	for (char i = 0; i < MAX_BANK_NUM; i++)
	{
		m_lBankDBID[i] = 0;
		m_CBank[i].Init();
	}
	m_szMaskMapName[0] = '\0';

	m_GuildState	 = emGuildInitVal;
	m_bIsGuildLeader = false;
	memset( m_szGuildName, 0, defGUILD_NAME_LEN );
	memset( m_szGuildMotto, 0, defGUILD_MOTTO_LEN );

	CCharMission::Initially();

	m_pMakingBoat = NULL;
	m_pStallData = NULL;

	SetChallengeType(enumFIGHT_NONE);
	SetInRepair(false);
	SetInForge(false);
	m_pCRepairman = NULL;
	m_pCForgeman = NULL;

	bReceiveRequests = true;
}

void CPlayer::Finally()
{
	GetNextPlayer() = NULL;

	m_pMainCha->TradeClear( *this );
	m_pMainCha->Free();
	CCharMission::Finally();

	for( int i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i] )
		{
			if (m_Boat[i]->GetShip())
				m_Boat[i]->GetShip()->Free();
			m_Boat[i]->SetShip(0);
			m_Boat[i]->Free();
			m_Boat[i] = 0;
		}
	}

	m_pCtrlCha = 0;
	m_pMainCha = NULL;

	// 
	if( m_pMakingBoat )
	{
		m_pMakingBoat->Free();
		m_pMakingBoat = NULL;
	}

	if( m_pStallData )
	{
		m_pStallData->Free();
		m_pStallData = NULL;
	}

	bIsValid = false;
}

void CPlayer::Free()
{
	g_pGameApp->m_pCPlySpace->ReturnPly(m_lHandle);
}

void CPlayer::SetIMP(long imp,bool sync) {
	m_lIMP = imp<2000000000 ? imp : 2000000000; 
	if (sync){
		char cmd[64];
		sprintf(cmd, "SetIMPAcc('%s',%d)",GetActName(),GetIMP());
		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MM_DO_STRING);
		WRITE_LONG(WtPk, GetID());
		WRITE_STRING(WtPk, cmd);
		GetMainCha()->ReflectINFof(GetMainCha(), WtPk);
	}
}

BOOL CPlayer::HasAllBoatInBerth( USHORT sBerthID )
{
	for( int i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i]->getAttr( ATTR_BOAT_BERTH ) == sBerthID )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPlayer::HasBoatInBerth( USHORT sBerthID )
{
	for( int i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i]->getAttr( ATTR_BOAT_BERTH ) == sBerthID && m_Boat[i]->getAttr( ATTR_BOAT_ISDEAD ) == 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPlayer::HasDeadBoatInBerth( USHORT sBerthID )
{
	for( int i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i]->getAttr( ATTR_BOAT_BERTH ) == sBerthID && m_Boat[i]->getAttr( ATTR_BOAT_ISDEAD ) != 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CPlayer::GetAllBerthBoat( USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data )
{
	BYTE byIndex = 0;
	for( BYTE i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i]->getAttr( ATTR_BOAT_BERTH ) == sBerthID )
		{
			Data.byID[byIndex] = i;
			Data.byState[byIndex] = (BYTE)m_Boat[i]->getAttr( ATTR_BOAT_ISDEAD );
			strncpy( Data.szName[byIndex], m_Boat[i]->GetName(), BOAT_MAXSIZE_BOATNAME - 1 );
			if( Data.byState[byIndex] != 0 )
			{
				//strcat( Data.szName[byIndex], "  []" );
				strcat( Data.szName[byIndex], RES_STRING(GM_PLAYER_CPP_00001) );
				Data.byState[byIndex] = BS_DEAD;
			}
			else
			{
				if( m_Boat[i]->getAttr( ATTR_HP ) < (m_Boat[i]->getAttr( ATTR_MXHP )/2) )
				{
					//strcat( Data.szName[byIndex], "  []" );
					strcat( Data.szName[byIndex], RES_STRING(GM_PLAYER_CPP_00002) );
					Data.byState[byIndex] = BS_NOHP;
				}
				else if( m_Boat[i]->getAttr( ATTR_SP ) < (m_Boat[i]->getAttr( ATTR_MXSP )/2) )
				{
					//strcat( Data.szName[byIndex], "  []" );	
					strcat( Data.szName[byIndex], RES_STRING(GM_PLAYER_CPP_00003) );	
					Data.byState[byIndex] = BS_NOSP;
				}
				else
				{					
					//strcat( Data.szName[byIndex], "  []" );
					strcat( Data.szName[byIndex], RES_STRING(GM_PLAYER_CPP_00004) );
					Data.byState[byIndex] = BS_GOOD;
				}
			}

			byIndex++;
		}
	}
	byNumBoat = byIndex;
}

void CPlayer::GetBerthBoat( USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data )
{
	BYTE byIndex = 0;
	for( BYTE i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i]->getAttr( ATTR_BOAT_BERTH ) == sBerthID && m_Boat[i]->getAttr( ATTR_BOAT_ISDEAD ) == 0 )
		{
			Data.byID[byIndex] = i;
			Data.byState[byIndex] = 0;
			strncpy( Data.szName[byIndex], m_Boat[i]->GetName(), BOAT_MAXSIZE_BOATNAME - 1 );
			byIndex++;
		}
	}
	byNumBoat = byIndex;
}

void CPlayer::GetDeadBerthBoat( USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data )
{
	BYTE byIndex = 0;
	for( BYTE i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i]->getAttr( ATTR_BOAT_BERTH ) == sBerthID && m_Boat[i]->getAttr( ATTR_BOAT_ISDEAD ) != 0 )
		{			
			Data.byID[byIndex] = i;
			Data.byState[byIndex] = 0;
			strncpy( Data.szName[byIndex], m_Boat[i]->GetName(), BOAT_MAXSIZE_BOATNAME - 1 );
			byIndex++;
		}
	}
	byNumBoat = byIndex;
}

BOOL CPlayer::AddBoat( CCharacter& Boat )
{
	if( IsBoatFull() ) return FALSE;
	
	Boat.SetPlayer( this );
	m_Boat[m_byNumBoat] = &Boat;
	m_byNumBoat++;

	if( m_pMainCha )
	{
		Boat.SetPlayer( this );
		Boat.m_CChaAttr.SetChangeFlag();		
		m_pMainCha->SynAddItemCha( &Boat );
		// m_pMainCha->BoatAdd( Boat );
	}
	return TRUE;
}

BOOL CPlayer::ClearBoat( DWORD dwBoatDBID )
{
	for( BYTE i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i] && m_Boat[i]->getAttr( ATTR_BOAT_DBID ) == dwBoatDBID )
		{
			if( m_pMainCha )
			{
				m_pMainCha->SynDelItemCha( m_Boat[i] );
				//m_pMainCha->BoatClear( *m_Boat[i] );
			}
			m_Boat[i]->Free();

			CCharacter* Boat[MAX_CHAR_BOAT];
			memcpy( Boat, m_Boat, sizeof(CCharacter*)*MAX_CHAR_BOAT );
			memset( m_Boat + i, 0, sizeof(CCharacter*)*(m_byNumBoat - i ) );
			memcpy( m_Boat + i, Boat + i + 1, sizeof(CCharacter*)*(m_byNumBoat - i - 1) );
			m_byNumBoat--;
			return TRUE;
		}
	}
	return FALSE;
}

void CPlayer::RefreshBoatAttr(void)
{
	for (BYTE i = 0; i < m_byNumBoat; i++)
	{
		if (m_Boat[i])
			g_CParser.DoString( "Ship_ExAttrCheck", enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 2, GetMainCha(), m_Boat[i], DOSTRING_PARAM_END );
	}
}

CCharacter* CPlayer::GetBoat( DWORD dwBoatDBID )
{
	for( BYTE i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i] && m_Boat[i]->getAttr( ATTR_BOAT_DBID ) == dwBoatDBID )
		{
			return m_Boat[i];
		}
	}
	return NULL;
}

BYTE CPlayer::GetBoatIndexByDBID(DWORD dwBoatDBID)
{
	for( BYTE i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i] && m_Boat[i]->getAttr( ATTR_BOAT_DBID ) == dwBoatDBID )
		{
			return i;
		}
	}
	return -1;
}

CCharacter* CPlayer::GetLuanchOut()
{
	if( m_dwLaunchID == -1 ) {
		return NULL;
	}

	for( int i = 0; i < m_byNumBoat; i++ )
	{
		if( m_Boat[i]->getAttr( ATTR_BOAT_DBID ) == m_dwLaunchID )
		{
			return m_Boat[i];
		}
	}
	return NULL;
}

void CPlayer::SetBerth( USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir )
{
	m_sBerthID = sBerthID;
	m_sxPos = sxPos;
	m_syPos = syPos;
	m_sDir = sDir;
}

void CPlayer::GetBerth( USHORT& sBerthID, USHORT& sxPos, USHORT& syPos, USHORT& sDir )
{
	sBerthID = m_sBerthID;
	sxPos = m_sxPos;
	syPos = m_syPos;
	sDir = m_sDir;
}

// 
// 
void CPlayer::AddTeamMember(uplayer *pUPlayer)
{
	if(_nTeamMemberCnt>=MAX_TEAM_MEMBER)
	{
		return;
	}
	_Team[_nTeamMemberCnt] = *pUPlayer;
	_nTeamMemberCnt++;
}

// 
void CPlayer::ClearTeamMember()
{
    _dwTeamLeaderID = 0;
	NoticeTeamLeaderID();

	_nTeamMemberCnt = 0;
}

void CPlayer::LeaveTeam()
{
	ClearTeamMember();
}

void CPlayer::UpdateTeam()
{
}

//  : Player
void CPlayer::NoticeTeamMemberData()
{
	int	nTMemberCnt = GetTeamMemberCnt();
	if(nTMemberCnt==0) return;

	CCharacter *pCha = GetCtrlCha();
	CCharacter *pMainCha = pCha->GetPlayer()->GetMainCha();
	
	

    WPACKET	wpk = GETWPACKET();
	WRITE_CMD(wpk, CMD_MC_TEAM);
	WRITE_LONG(wpk, pMainCha->GetID()); // ID
	WRITE_LONG(wpk, (long)pCha->getAttr(ATTR_HP));
	WRITE_LONG(wpk, (long)pCha->getAttr(ATTR_MXHP));
	WRITE_LONG(wpk, (long)pCha->getAttr(ATTR_SP));
	WRITE_LONG(wpk, (long)pCha->getAttr(ATTR_MXSP));
	
	WRITE_LONG(wpk, pMainCha->getAttr(ATTR_LV)); // 	
	pMainCha->WriteLookData(wpk, LOOK_TEAM);	 // , 

	SENDTOCLIENT2(wpk, nTMemberCnt, _Team);
}

void CPlayer::NoticeTeamLeaderID(void)
{
	int	nTMemberCnt = GetTeamMemberCnt();
	if(nTMemberCnt==0) return;

	CCharacter *pCha = GetCtrlCha();

	WPACKET pk	=GETWPACKET();
	WRITE_CMD(pk, CMD_MC_TLEADER_ID);
	WRITE_LONG(pk, pCha->GetID());
	WRITE_LONG(pk, getTeamLeaderID());

	SENDTOCLIENT2(pk, nTMemberCnt, _Team);
	pCha->NotiChgToEyeshot(pk);
}

// 
CCharacter* CPlayer::GetTeamMemberCha(int nNo)
{
	CPlayer *pOther = g_pGameApp->GetPlayerByDBID(_Team[nNo].m_dwDBChaId);
	if (!pOther)
		return 0;
	return pOther->GetCtrlCha();
}

CPlayer* CPlayer::GetNextTeamPly(void)
{
	Short	sMemCnt = (Short)GetTeamMemberCnt();
	CPlayer	*pCPly;
	while (m_sGetTeamPlyCount < sMemCnt)
	{
		pCPly = g_pGameApp->GetPlayerByDBID(_Team[m_sGetTeamPlyCount++].m_dwDBChaId);
		if (pCPly)
			return pCPly;
	}
	return 0;
}

bool CPlayer::SetChallengeParam(dbc::Char chParamID, dbc::Long lParamVal)
{
	m_lChallengeParam[chParamID] = lParamVal;

	return true;
}

Long CPlayer::GetChallengeParam(dbc::Char chParamID)
{
	return m_lChallengeParam[chParamID];
}

bool CPlayer::HasChallengeObj(void)
{
	return GetChallengeType() != enumFIGHT_NONE ? true : false;
}

void CPlayer::ClearChallengeObj(bool bAll)
{
	if (!HasChallengeObj())
		return;

	if (bAll)
	{
		Char	chLoop = (Char)GetChallengeParam(0);
		if (chLoop > MAX_TEAM_MEMBER * 2)
			chLoop = MAX_TEAM_MEMBER * 2;
		CPlayer	*pCPly;
		for (Char i = 0; i < chLoop; i++)
		{
			pCPly = g_pGameApp->IsValidPlayer(GetChallengeParam(i * 2 + 2), GetChallengeParam(i * 2 + 2 + 1));
			if (!pCPly)
				continue;
			pCPly->SetChallengeType(enumFIGHT_NONE);
		}
	}

	SetChallengeType(enumFIGHT_NONE);
}

// chPosType 1.2
bool CPlayer::SetRepairPosInfo(dbc::Char chPosType, dbc::Char chPosID)
{
	m_chRepairPosType = chPosType;
	m_chRepairPosID = chPosID;

	CCharacter	*pCCha = GetCtrlCha();
	if (chPosType == 1)
	{
		if (chPosID < enumEQUIP_HEAD || chPosID >= enumEQUIP_NUM)
			return false;
		if (!g_IsRealItemID(pCCha->m_SChaPart.SLink[chPosID].sID))
			return false;
		m_SRepairItem = pCCha->m_SChaPart.SLink[chPosID];
		m_pSRepairItem = &pCCha->m_SChaPart.SLink[chPosID];
	}
	else if (chPosType == 2)
	{
		SItemGrid	*pSItemCont = pCCha->m_CKitbag.GetGridContByID(chPosID);
		if (!pSItemCont)
			return false;
		m_SRepairItem = *pSItemCont;
		m_pSRepairItem = pSItemCont;
	}

	return true;
}

bool CPlayer::OpenRepair(CCharacter *pCNpc)
{
	m_pCRepairman = pCNpc;
	GetCtrlCha()->SynBeginItemRepair();
	return true;
}

bool CPlayer::OpenForge(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemForge();
	GetCtrlCha()->ForgeAction();
	return true;
}

// Add by lark.li 20080514 begin
bool CPlayer::OpenLottery(CCharacter *pCNpc)
{
	//SystemNotice("");

	m_pCLotteryman = pCNpc;
	GetCtrlCha()->SynBeginItemLottery();
	return true;
}
// End

bool CPlayer::OpenUnite(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemUnite();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenMilling(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemMilling();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenFusion(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemFusion();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenUpgrade(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemUpgrade();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenEidolonMetempsychosis(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemEidolonMetempsychosis();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenEidolonFusion(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemEidolonFusion();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenPurify(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemPurify();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenFix(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemFix();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenEnergy(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginItemEnergy();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenGMSend(CCharacter *pCNpc)
{
	GetCtrlCha()->SynBeginGMSend();
	return true;
}

bool CPlayer::OpenGMRecv(CCharacter *pCNpc)
{
	GetCtrlCha()->SynBeginGMRecv(pCNpc->GetID());
	return true;
}

bool CPlayer::OpenGetStone(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginGetStone();
	GetCtrlCha()->ForgeAction();
	return true;
}

bool CPlayer::OpenTiger(CCharacter *pCNpc)
{
	if (IsInForge())
	{
		//SystemNotice("!");
		SystemNotice(RES_STRING(GM_PLAYER_CPP_00005));
		return false;
	}
	m_pCForgeman = pCNpc;
	GetCtrlCha()->SynBeginTiger();
	GetCtrlCha()->ForgeAction();
	return true;
}

void CPlayer::SystemNotice( const char szData[], ... )
{
	// Modify by lark.li 20080801 begin
	char szTemp[250];
	memset(szTemp, 0, sizeof(szTemp));
	va_list list;
	va_start( list, szData );
	_vsnprintf(szTemp, sizeof(szTemp) - 1, szData, list );
	//vsprintf( szTemp, szData, list );
	// End
	va_end( list );

	WPACKET packet = GETWPACKET();
	WRITE_CMD(packet, CMD_MC_SYSINFO );
	WRITE_SEQ(packet, szTemp, uShort( strlen(szTemp) ) + 1 );
	
	WRITE_LONG(packet, GetDBChaId());
	WRITE_LONG(packet, GetGateAddr());
	WRITE_SHORT(packet, 1);

	GetGate()->SendData(packet);
}

bool CPlayer::RefreshMapMask(const char *szMapName, long lPosX, long lPosY)
{
	if (!strcmp(szMapName, m_szMaskMapName))
	{
		if (m_CMapMask.UpdateMapMask((char *)szMapName, lPosX, lPosY, m_lLightSize))
			SetMapMaskChange();
		return true;
	}

	return false;
}

bool CPlayer::AddBank(void)
{
	return game_db.CreatePlyBank(this);
}

bool CPlayer::SaveBank(char chBankNO)
{
	return game_db.SavePlyBank(this, chBankNO);
}

bool CPlayer::SetBankChangeFlag(char chBankNO, bool bChange)
{
	if (GetCurBankNum() == 0)
		return true;

	char	chStart, chEnd;
	if (chBankNO < 0)
	{
		chStart = 0;
		chEnd = GetCurBankNum() - 1;
	}
	else
	{
		if (chBankNO >= GetCurBankNum())
			return false;
		chStart = chEnd = chBankNO;
	}

	CKitbag	*pCBank;
	for (char i = chStart; i <= chEnd; i++)
	{
		pCBank = GetBank(i);
		if (!pCBank)
			continue;
		pCBank->SetChangeFlag(bChange);
	}

	return true;
}


bool CPlayer::SynGuildBank(CKitbag * bag, char chType){
	int canSeeBank = (m_pMainCha->guildPermission & emGldPermViewBank);
	
	WPACKET WtPk = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_NOTIACTION);
	WRITE_LONG(WtPk, m_pMainCha->GetID());
	WRITE_LONG(WtPk, m_pMainCha->m_ulPacketID);
	WRITE_CHAR(WtPk, enumACTION_GUILDBANK);

	if (canSeeBank != emGldPermViewBank){
		CKitbag emptyBag{};
		emptyBag.SetCapacity(0);
		m_pMainCha->WriteKitbag(emptyBag, WtPk, chType);
	}else{
		m_pMainCha->WriteKitbag(*bag, WtPk, chType);
	}
	
	m_pMainCha->ReflectINFof(m_pMainCha, WtPk);
	return true;
}

bool CPlayer::SynBank(char chBankNO, char chType)
{
	if (GetCurBankNum() == 0)
		return true;

	char chStart, chEnd;
	if (chBankNO < 0)
	{
		chStart = 0;
		chEnd = GetCurBankNum() - 1;
	}
	else
	{
		if (chBankNO >= GetCurBankNum())
			return false;
		chStart = chEnd = chBankNO;
	}

	WPACKET WtPk = GETWPACKET();
	WRITE_CMD(WtPk, CMD_MC_NOTIACTION);	// 
	WRITE_LONG(WtPk, m_pCtrlCha->GetID());
	WRITE_LONG(WtPk, m_pCtrlCha->m_ulPacketID);
	WRITE_CHAR(WtPk, enumACTION_BANK);

	CKitbag	*pCBank;
	for (char i = chStart; i <= chEnd; i++)
	{
		pCBank = GetBank(i);
		if (!pCBank)
			continue;
		m_pCtrlCha->WriteKitbag(*pCBank, WtPk, chType);
	}
	m_pCtrlCha->ReflectINFof(m_pCtrlCha, WtPk);

	return true;
}

bool CPlayer::BankCanOpen(CCharacter *pCNpc)
{
	CCharacter	*pCCtrlCha = GetCtrlCha();
	if (pCCtrlCha != GetMainCha())
	{
		//pCCtrlCha->SystemNotice("");
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00006));
		return false;
	}
	if (m_pCBankNpc)
	{
		//pCCtrlCha->SystemNotice("");
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00007));
		return false;
	}
	if (!pCNpc)
	{
		//pCCtrlCha->SystemNotice("NPC");
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00008));
		return false;
	}
	if (pCCtrlCha->HasTradeAction())
	{
		//pCCtrlCha->SystemNotice("");
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00009));
		return false;
	}
	// Npc
	if (!pCCtrlCha->IsRangePoint(pCNpc->GetPos(), defBANK_DISTANCE))
	{
		//pCCtrlCha->SystemNotice("");
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00010));
		return false;
	}
	if (!pCCtrlCha->TradeAction(true))
	{
		//pCCtrlCha->SystemNotice("");
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00011));
		return false;
	}

	return true;
}

bool CPlayer::OpenBank(CCharacter *pCNpc)
{
	CCharacter	*pCCtrlCha = GetCtrlCha();

	if (!SetBankChangeFlag(0))
	{
		pCCtrlCha->TradeAction(false);
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00012));
		return false;
	}
	if (!SynBank(0))
	{
		pCCtrlCha->TradeAction(false);
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00012));
		return false;
	}
	m_pCBankNpc = pCNpc;
	return true;
}

bool CPlayer::GetGuildGold(){
	CCharacter	*pCCtrlCha = GetCtrlCha();
	int guildID = pCCtrlCha->GetGuildID();
	if (guildID == 0 ){
		return false;
	}
	int canSeeBank = (pCCtrlCha->guildPermission & emGldPermViewBank);
	if (canSeeBank == emGldPermViewBank){
		unsigned long long gold = game_db.GetGuildBankGold(guildID);
		WPACKET WtPk = GETWPACKET();
		WRITE_CMD(WtPk, CMD_MC_UPDATEGUILDBANKGOLD);
		WRITE_STRING(WtPk, to_string(gold).c_str());
		pCCtrlCha->ReflectINFof(pCCtrlCha, WtPk);
		return true;
	}
	return false;
}

bool CPlayer::OpenGuildBank()
{
	CCharacter	*pCCtrlCha = GetCtrlCha();

	if (!SetBankChangeFlag(0))
	{
		pCCtrlCha->TradeAction(false);
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00012));
		return false;
	}

	if (this->m_lGuildID == 0){
		pCCtrlCha->TradeAction(false);
		pCCtrlCha->SystemNotice("You are not in a guild.");
		return false;
	}
	
	int canOpen = (this->GetMainCha()->guildPermission&emGldPermViewBank);
	if (canOpen != emGldPermViewBank){
		pCCtrlCha->TradeAction(false);
		pCCtrlCha->SystemNotice("You do not have permission to view the guild bank.");
		return false;
	}
	CKitbag bag;
	if (!game_db.GetGuildBank(this->m_lGuildID, &bag)){
		pCCtrlCha->TradeAction(false);
		pCCtrlCha->SystemNotice("Unknown Error");
		return false;
	}
	if (!SynGuildBank(&bag,0))
	{
		pCCtrlCha->TradeAction(false);
		pCCtrlCha->SystemNotice(RES_STRING(GM_PLAYER_CPP_00012));
		return false;
	}
	return true;
}

void CPlayer::CloseBank(void)
{
	m_pCBankNpc = 0;
	GetMainCha()->TradeAction(false);
}

bool CPlayer::SetBankSaveFlag(char chBankNO, bool bChange)
{
	if (GetCurBankNum() == 0)
		return false;

	char	chStart, chEnd;
	if (chBankNO < 0)
	{
		chStart = 0;
		chEnd = GetCurBankNum() - 1;
	}
	else
	{
		if (chBankNO >= GetCurBankNum())
			return false;
		chStart = chEnd = chBankNO;
	}

	for (char i = chStart; i <= chEnd; i++)
		m_bBankChange[i] = bChange;

	return true;
}

bool CPlayer::BankWillSave(char chBankNO)
{
	if (GetCurBankNum() == 0 || chBankNO >= GetCurBankNum())
		return false;

	return m_bBankChange[chBankNO];
}

bool CPlayer::BankHasItem(USHORT sItemID, USHORT& sCount)
{
	int nCount = 0;
	CItemRecord* pItem = GetItemRecordInfo( sItemID );
	if( pItem == NULL )
	{
		//SystemNotice( "BankHasItem:!ID = %d", sItemID );
		SystemNotice( RES_STRING(GM_PLAYER_CPP_00013), sItemID );
		return FALSE;
	}

	USHORT sNum = m_CBank[0].GetUseGridNum();
	SItemGrid *pGridCont;
	if( !pItem->GetIsPile() )
	{
		for( int i = 0; i < sNum; i++ )
		{
			pGridCont = m_CBank[0].GetGridContByNum( i );
			if( pGridCont )
			{
				if( sItemID == pGridCont->sID )
				{
					nCount++;
					if( nCount >= sCount )
						break;
				}
			}
		}
	}
	else
	{
		for( int i = 0; i < sNum; i++ )
		{
			pGridCont = m_CBank[0].GetGridContByNum( i );
			if( pGridCont )
			{
				if( sItemID == pGridCont->sID )
				{
					nCount += (USHORT)pGridCont->sNum;;
					if( nCount >= sCount )
						break;
				}
			}
		}
	}

	return nCount >= sCount;
}

char* CPlayer::BankDBIDData2String(char *szSStateBuf, int nLen)
{
	if (!szSStateBuf) return NULL;

	char	szData[512];
	int		nBufLen = 0, nDataLen;
	szSStateBuf[0] = '\0';

	for (char i = 0; i < m_chBankNum; i++)
	{
		sprintf(szData, "%d", m_lBankDBID[i]);
		nDataLen = (int)strlen(szData);
		if (nBufLen + nDataLen >= nLen) return NULL;
		strcat(szSStateBuf, szData);
		nBufLen += nDataLen;

		if (i < m_chBankNum - 1)
		{
			sprintf(szData, ",", m_lBankDBID[i]);
			nDataLen = (int)strlen(szData);
			if (nBufLen + nDataLen >= nLen) return NULL;
			strcat(szSStateBuf, szData);
			nBufLen += nDataLen;
		}
	}

	return szSStateBuf;
}

bool CPlayer::Strin2BankDBIDData(std::string &strData)
{
	if (!strcmp(strData.c_str(), ""))
		return true;

	std::string strList[MAX_BANK_NUM];
	int nSegNum = Util_ResolveTextLine(strData.c_str(), strList, MAX_BANK_NUM, ',');
	if (nSegNum > MAX_BANK_NUM)
		return false;

	short	sTCount = 0;
	for (int i = 0; i < nSegNum; i++)
	{
		AddBankDBID(Str2Int(strList[i]));
	}

	return true;
}

void CPlayer::CheckChaItemFinalData()
{
	// 
	cChar	*szScript = "check_item_final_data";
	CCharacter	*pCMainCha = GetMainCha();
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		if (g_IsRealItemID(pCMainCha->m_SChaPart.SLink[i].sID))
		{
			pCMainCha->m_SChaPart.SLink[i].InitAttr();
			g_CParser.DoString(szScript, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, &pCMainCha->m_SChaPart.SLink[i], DOSTRING_PARAM_END);
		}
	}

	// 
	SItemGrid	*pGridCont;
	Short	sUseNum = pCMainCha->m_CKitbag.GetUseGridNum();
	for (int i = 0; i < sUseNum; i++)
	{
		pGridCont = pCMainCha->m_CKitbag.GetGridContByNum(i);
		if (!pGridCont)
			continue;
		pGridCont->InitAttr();
		g_CParser.DoString(szScript, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pGridCont, DOSTRING_PARAM_END);
	}

	// 
	for (int j = 0; j < MAX_BANK_NUM; j++)
	{
		sUseNum = m_CBank[j].GetUseGridNum();
		for (int i = 0; i < sUseNum; i++)
		{
			pGridCont = m_CBank[j].GetGridContByNum(i);
			if (!pGridCont)
				continue;
			pGridCont->InitAttr();
			g_CParser.DoString(szScript, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pGridCont, DOSTRING_PARAM_END);
		}
	}

	// 
	//for (int i = 0; i < m_byNumBoat; i++)
	//{
	//	if (m_Boat[i])
	//	{
	//		pGridCont = m_Boat[i]->m_CKitbag.GetGridContByNum(i);
	//		if (!pGridCont)
	//			continue;
	//		pGridCont->InitAttr();
	//		g_CParser.DoString(szScript, enumSCRIPT_RETURN_NONE, 0, enumSCRIPT_PARAM_LIGHTUSERDATA, 1, pGridCont, DOSTRING_PARAM_END);
	//	}
	//}
}

bool CPlayer::String2BankData(char chBankNO, std::string &strData)
{
	if (::String2KitbagData(GetBank(chBankNO), strData))
	{
		if (m_pMainCha)
			m_pMainCha->CheckBagItemValid(GetBank(chBankNO));
		return true;
	}

	return false;
}

void CPlayer::Run(DWORD dwCurTime)
{
	if (HasChallengeObj())
		if (m_timerChallenge.IsOK(dwCurTime))
		{
			//SystemNotice("!");
			SystemNotice(RES_STRING(GM_PLAYER_CPP_00014));
			ClearChallengeObj(false);
		}
}
