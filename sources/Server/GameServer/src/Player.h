//=============================================================================
// FileName: Player.h
// Creater: ZhangXuedong
// Date: 2004.10.19
// Comment: CPlayer class
//=============================================================================

#ifndef PLAYER_H
#define PLAYER_H


#include "AttachManage.h"
#include "Kitbag.h"
#include "Mission.h"
#include "GameAppNet.h"
#include <ShipSet.h>
#include "ChaMask.h"
#include "Timer.h"

namespace mission
{
	class CStallData;
}

enum ELoginChaType
{
	enumLOGIN_CHA_MAIN,
	enumLOGIN_CHA_BOAT,
};

class GateServer;

#define MAX_TEAM_MEMBER 5
#define MAX_BANK_NUM	1
#define ACT_NAME_LEN	64
#define PLAYER_INVALID_FLAG			0xF0F0F0F0L


//NOTE(Ogge): This should describe a player as a entity 
class CPlayer : public GatePlayer, public mission::CCharMission
{
public:
	CPlayer();

	void Init(GateServer *pGate, dbc::uLong ulGtAddr)
	{
        SetGate(pGate); SetGateAddr(ulGtAddr); SetDBChaId(0);
        Next = NULL; Prev = NULL;
	}

	bool		IsValidFlag() { return m_dwValidFlag == PLAYER_INVALID_FLAG; }

	void		SetPassword( const char szPassword[] ) { strncpy( m_szPassword, szPassword, ROLE_MAXSIZE_PASSWORD2 ); }
	const char* GetPassword() { return m_szPassword; }

	void		Free();
	void		Initially();
	void		Finally();	
	void		SetID(dbc::Long lID) {m_lID = lID;}
	dbc::Long	GetID(void) {return m_lID;}
	void		SetHandle(dbc::Long lHandle) {m_lHandle = lHandle;}
	dbc::Long	GetHandle(void) {return m_lHandle;}
	void		SetHoldID(dbc::Long lID) {m_lHoldID = lID;}
	dbc::Long	GetHoldID(void) {return m_lHoldID;}

	bool		IsPlayer(void) {return bIsValid && (GetGate() ? true : false);}

    virtual void OnLogoff() {} // player

	bool		IsValid(void) {return bIsValid;}
    void        SetActLoginID(DWORD id){m_dwLoginID = id;}
    DWORD       GetActLoginID(){return m_dwLoginID;}
	void		SetDBActId(DWORD dwDBActId) {m_dwDBActId = dwDBActId;}
	DWORD		GetDBActId(void) {return m_dwDBActId;}
	void		SetActName(dbc::cChar *szActName) {strncpy(m_chActName, szActName, ACT_NAME_LEN - 1); m_chActName[ACT_NAME_LEN - 1] = 0;}
	dbc::cChar*	GetActName(void) {return m_chActName;}
	void		SetGMLev(dbc::Char chGMLev) {m_chGMLev = chGMLev;}
	dbc::uChar	GetGMLev(void) {return (dbc::uChar)m_chGMLev;}
	void		SetMapMaskDBID(long lID) {m_lMapMaskDBID = lID;}
	long		GetMapMaskDBID(void) {return m_lMapMaskDBID;}
	void		SetBankDBID(long lID, char chBankNO) {m_lBankDBID[chBankNO] = lID;}
	long		GetBankDBID(char chBankNO) {return m_lBankDBID[chBankNO];}

	void		SetIMP(long imp,bool sync = true);
	long		GetIMP() { return m_lIMP; }
	long		m_lIMP;

	void		SetLoginCha(dbc::uLong ulType, dbc::uLong ulID) {m_ulLoginCha[0] = ulType, m_ulLoginCha[1] = ulID;}
	dbc::uLong	GetLoginChaType(void) {return m_ulLoginCha[0];}
	dbc::uLong	GetLoginChaID(void) {return m_ulLoginCha[1];}

	//mission::CCharMission& GetMission() { return *(mission::CCharMission*)this; }

	void		AddTeamMember(uplayer *);
	void		ClearTeamMember();

	bool		CanReceiveRequests() { return bReceiveRequests; }				// Add by Mdr September 2020
	void		SetCanReceiveRequests(bool x) { bReceiveRequests = x; }			//
	int			GetTeamMemberCnt()			{	return _nTeamMemberCnt;			} 
	DWORD		GetTeamMemberDBID(int nNo)	{	return _Team[nNo].m_dwDBChaId;  }
	DWORD		getTeamLeaderID()			{	return _dwTeamLeaderID;			}// 0
	CCharacter* GetTeamMemberCha(int nNo); 
	void		NoticeTeamMemberData();
	void		NoticeTeamLeaderID(void);
	void		setTeamLeaderID(DWORD dwID) {	_dwTeamLeaderID = dwID;			}
	void		LeaveTeam(void);
	void		UpdateTeam(void);
	bool		IsTeamLeader(void) {if (getTeamLeaderID() == GetID()) return true; return false;}
	bool		HasTeam(void) {return getTeamLeaderID() != 0 ? true : false;}
	void		BeginGetTeamPly(void) {m_sGetTeamPlyCount = 0;}
	CPlayer*	GetNextTeamPly(void);

	void		SetChallengeType(dbc::Char chType) {m_chChallengeType = chType;}
	dbc::Char	GetChallengeType(void) {return m_chChallengeType;}
	bool		SetChallengeParam(dbc::Char chParamID, dbc::Long lParamVal);
	dbc::Long	GetChallengeParam(dbc::Char chParamID);
	bool		HasChallengeObj(void);
	void		ClearChallengeObj(bool bAll = true);
	void		StartChallengeTimer(void) {m_timerChallenge.Begin(30 * 1000);}

	bool		OpenRepair(CCharacter *pCNpc);
	CCharacter*	GetRepairman(void) {return m_pCRepairman;}
	bool		SetRepairPosInfo(dbc::Char chPosType, dbc::Char chPosID);
	SItemGrid*	GetRepairItem(void) {return m_pSRepairItem;}
	bool		CheckRepairItem(void) {return (m_SRepairItem == *m_pSRepairItem) ? true : false;}
	bool		IsRepairEquipPos(void) {return m_chRepairPosType == 1 ? true : false;}
	dbc::Char	GetRepairPosID(void) {return m_chRepairPosID;}
	bool		IsInRepair(void) {return m_bInRepair;}
	void		SetInRepair(bool bInR = true) {m_bInRepair = bInR;}

	bool		OpenForge(CCharacter *pCNpc);
	
	bool		OpenLottery(CCharacter *pCNpc);	//Add by lark.li 20080514

	bool		OpenUnite(CCharacter *pCNpc);
	bool		OpenMilling(CCharacter *pCNpc);
	bool		OpenFusion(CCharacter *pCNpc);
	bool		OpenUpgrade(CCharacter *pCNpc);
	bool		OpenEidolonMetempsychosis(CCharacter *pCNpc);
	bool		OpenEidolonFusion(CCharacter *pCNpc);
	bool		OpenPurify(CCharacter *pCNpc);
	bool		OpenFix(CCharacter *pCNpc);
	bool		OpenEnergy(CCharacter *pCNpc);
	bool		OpenGetStone(CCharacter *pCNpc);
	bool		OpenTiger(CCharacter *pCNpc);

	CCharacter*	GetForgeman(void) {return m_pCForgeman;}
	bool		IsInForge(void) {return m_bInForge;}
	bool		IsInLifeSkill(void){return m_bInLiftSkill;}
	void		SetInForge(bool bInForge = true) {m_bInForge = bInForge;}
	void		SetInLifeSkill(bool bInLiftSkill = true) {m_bInLiftSkill = bInLiftSkill;}
	void		SetForgeInfo(dbc::Char chType, SForgeItem *pSItem) {m_chForgeType = chType, m_SForgeItem = *pSItem;}
	void		SetLifeSkillInfo(long lType,SLifeSkillItem * pLifeSkill){m_lLifeSkillType = lType,m_pSLifeSkillItem = *pLifeSkill;}
	dbc::Char	GetForgeType(void) {return m_chForgeType;}
	SForgeItem*	GetForgeItem(void) {return &m_SForgeItem;}
	SLifeSkillItem* GetLifeSkillItem(){return &m_pSLifeSkillItem;}
	void		SetMainCha(CCharacter *pMainCha) {m_pMainCha = pMainCha;}
	void		SetCtrlCha(CCharacter *pCtrlCha) {m_pCtrlCha = pCtrlCha;}
	CCharacter	*GetMainCha(void) {return m_pMainCha;}
	CCharacter	*GetCtrlCha(void) {return m_pCtrlCha;}
	
	CCharacter* GetMakingBoat() { return m_pMakingBoat; }
	void		SetMakingBoat( CCharacter* pBoat ) { m_pMakingBoat = pBoat; }

	// 
	BYTE		GetNumBoat() { return m_byNumBoat; }
	void		GetBerthBoat( USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data );
	void		GetAllBerthBoat( USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data );
	void		GetDeadBerthBoat( USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data );
	void		SetBerth( USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir );
	void		GetBerth( USHORT& sBerthID, USHORT& sxPos, USHORT& syPos, USHORT& sDir );
	BOOL		HasAllBoatInBerth( USHORT sBerthID );
	BOOL		HasBoatInBerth( USHORT sBerthID );
	BOOL		HasDeadBoatInBerth( USHORT sBerthID );
	BOOL		IsBoatFull() { return m_byNumBoat >= MAX_CHAR_BOAT; }
	BOOL		IsLuanchOut() { return m_dwLaunchID != -1; }
	void		SetLuanchOut( DWORD dwID ) { m_dwLaunchID = dwID; }
	DWORD		GetLuanchID() { return m_dwLaunchID; }
	CCharacter* GetLuanchOut();
	CCharacter* GetBoat( BYTE byIndex ) { return ( byIndex >= MAX_CHAR_BOAT ) ? NULL : m_Boat[byIndex]; }
	CCharacter* GetBoat( DWORD dwBoatDBID );
	BYTE		GetBoatIndexByDBID(DWORD dwBoatDBID);
	BOOL		AddBoat( CCharacter& Boat );
	BOOL		ClearBoat( DWORD dwBoatDBID );
	void		RefreshBoatAttr(void);

	// 
	mission::CStallData* GetStallData() { return m_pStallData; }
	void		SetStallData( mission::CStallData* pData ) { m_pStallData = pData; }

	// 
	//void		SetMapMask(const char *pMask) {m_CMapMask.InitMaskData(pMask);}
	//const char*	GetMapMask() {return m_CMapMask.GetResultMask();}
	//BYTE*		GetOneMapMask(const char *szMapName, long &lLen) {return m_CMapMask.GetMapMask(szMapName, lLen);}
	void		SetMMaskLightSize(long lSize) {m_lLightSize = lSize;}
	long		GetMMaskLightSize(void) {return m_lLightSize;}
	bool		SetMapMaskBase64(const char *pMask) {return m_CMapMask.InitMaskData(m_szMaskMapName, pMask);}
	const char*	GetMapMaskBase64() {return m_CMapMask.GetResultOneMask(m_szMaskMapName);}
	BYTE*		GetMapMask(long &lLen) {return m_CMapMask.GetMapMask(m_szMaskMapName, lLen);}
	bool		RefreshMapMask(const char *szMapName, long lPosX, long lPosY);
	void		SetMaskMapName(const char *szMapName) {strncpy(m_szMaskMapName, szMapName, MAX_MAPNAME_LENGTH - 1); m_szMaskMapName[MAX_MAPNAME_LENGTH - 1] = '\0';}
	const char*	GetMaskMapName(void) {return m_szMaskMapName;}
	bool		IsMapMaskChange(void) {return m_chMapMaskChange >= 3 ? true : false;}
	void		SetMapMaskChange(void) {m_chMapMaskChange++;}
	void		ResetMapMaskChange(void) {m_chMapMaskChange = 0;}
	float		GetMapMaskOpenScale(const char *szMapName) {return m_CMapMask.GetMapMaskOpenScale(szMapName);}

	// 
	char		GetCurBankNum(void) {return m_chBankNum;}
	bool		AddBankDBID(long lDBID) {if (m_chBankNum >= MAX_BANK_NUM) return false; m_lBankDBID[m_chBankNum] = lDBID; m_chBankNum++; return true;}
	CKitbag*	GetBank(char chBankNO = 0) {if (chBankNO < 0 || chBankNO >= m_chBankNum) return 0; return m_CBank + chBankNO;}
	char*		BankDBIDData2String(char *szSStateBuf, int nLen);
	bool		Strin2BankDBIDData(std::string &strData);
	bool		AddBank(void);
	bool		SaveBank(char chBankNO = -1);
	bool		SetBankChangeFlag(char chBankNO = -1, bool bChange = true);
	bool		SynBank(char chBankNO = -1, char chType = enumSYN_KITBAG_INIT);
	bool		SynGuildBank(CKitbag* bag, char chType);
	bool		OpenBank(CCharacter *pCNpc);
	bool		OpenGuildBank();
	bool		GetGuildGold();
	bool		BankCanOpen(CCharacter *pCNpc);
	void		CloseBank(void);
	CCharacter*	GetBankNpc(void) {return m_pCBankNpc;}
	bool		SetBankSaveFlag(char chBankNO = -1, bool bChange = true);
	bool		BankWillSave(char chBankNO = 0);
	bool		BankHasItem(USHORT sItemID, USHORT& sCount);

	void		SystemNotice( const char szData[], ... );
	void		Run(DWORD dwCurTime);
	void		CheckChaItemFinalData(void);
	bool		String2BankData(char chBankNO, std::string &strData);

	//
	long	GetMoBean() { return m_lMoBean; }
	void	SetMoBean(long lMoBean) { m_lMoBean = lMoBean; }
	long	GetRplMoney() { return m_lRplMoney; }
	void	SetRplMoney(long lRplMoney) { m_lRplMoney = lRplMoney; }
	long	GetVipType() { return m_lVipID; }
	void	SetVipType(long lVipType) { m_lVipID = lVipType; }

	short	IsGarnerWiner(){return m_sGarnerWiner;}
	void	SetGarnerWiner(short siswiner){m_sGarnerWiner = siswiner;}
	uplayer     _Team[MAX_TEAM_MEMBER];

	//
	std::string & GetLifeSkillinfo(){return m_strLifeSkillinfo;}
	dbc::Char	m_szGuildName[defGUILD_NAME_LEN];	//ID0.
	dbc::Char	m_szGuildMotto[defGUILD_MOTTO_LEN];
	dbc::Char	m_szStallName[ROLE_MAXNUM_STALL_NUM];
	bool		m_bIsGuildLeader;	//0-;1-
	struct
	{
		BitMaskStatus m_GuildState;	//,:enum EGuildState

		uLong m_GuildStatus;	//
		uLong m_lGuildID;		//ID.
		uLong m_lTempGuildID;	//
		dbc::Char m_szTempGuildName[defGUILD_NAME_LEN];	//.
	};
protected:
	int         _nTeamMemberCnt;
	DWORD		_dwTeamLeaderID;

private:
	struct // CEntityAlloc
	{

		dbc::Long	m_lID;
		dbc::Long	m_lHandle;
		dbc::Long	m_lHoldID;
	};
	bool	bReceiveRequests{ true };
	bool	bIsValid;
	char	m_szPassword[ROLE_MAXSIZE_PASSWORD2+1];

	long	m_lMoBean;		//
	long	m_lRplMoney;	//
	long	m_lVipID;		//VIP

    DWORD       m_dwLoginID;    //  Account DB ID
	DWORD		m_dwDBActId;	// ID
	dbc::Char	m_chGMLev;		// GM
	long		m_lMapMaskDBID;
	dbc::Char	m_chActName[ACT_NAME_LEN];	// 

	CCharacter*	m_pCtrlCha;		// 

	CCharacter* m_pMainCha;		// 
	DWORD		m_dwLaunchID;	// ID
	BYTE		m_byNumBoat;	// 
	CCharacter* m_Boat[MAX_CHAR_BOAT];

	dbc::uLong	m_ulLoginCha[2];	// ID.0 1 

	// 
	struct
	{
		USHORT		m_sBerthID;	// 
		USHORT		m_sxPos;
		USHORT		m_syPos;
		USHORT		m_sDir;			
	};

	// 
	struct
	{
		long		m_lLightSize;
		CMaskData	m_CMapMask;
		char		m_szMaskMapName[MAX_MAPNAME_LENGTH];
		char		m_chMapMaskChange;
	};

	// 
	struct
	{
		CCharacter	*m_pCBankNpc;
		char		m_chBankNum;
		long		m_lBankDBID[MAX_BANK_NUM];
		CKitbag		m_CBank[MAX_BANK_NUM];
		bool		m_bBankChange[MAX_BANK_NUM];
	};

	// 
	CCharacter*		m_pMakingBoat;

	// 
	mission::CStallData* m_pStallData;

	// 
	struct
	{
		dbc::Char	m_chChallengeType;		// 
		dbc::Long	m_lChallengeParam[2 + MAX_TEAM_MEMBER * 2 * 2];	// 
		CTimer		m_timerChallenge;		// 
	};

	// 
	struct
	{
		bool		m_bInRepair;
		dbc::Char	m_chRepairPosType;
		dbc::Char	m_chRepairPosID;
		SItemGrid	m_SRepairItem;
		SItemGrid*	m_pSRepairItem;
		CCharacter*	m_pCRepairman;
	};

	// /
	struct
	{
		bool		m_bInForge;
		dbc::Char	m_chForgeType;
		SForgeItem	m_SForgeItem;
		CCharacter*	m_pCForgeman;
	};

	//
	struct
	{
		CCharacter*	m_pCLotteryman;
	};
	//Add by sunny.sun20080716
	struct
	{
		CCharacter * m_pCAmphitheaterman;
	};

	struct
	{
		bool			m_bInLiftSkill;
		dbc::Long		m_lLifeSkillType;
		SLifeSkillItem	m_pSLifeSkillItem;
		std::string			m_strLifeSkillinfo;
	};

	dbc::Short		m_sGetTeamPlyCount;

	DWORD			m_dwValidFlag;
	short			m_sGarnerWiner;
};

extern CPlayer*	CreatePlayer(GateServer *pGate, dbc::uLong ulGateAddr, dbc::uLong ulChaDBId);
extern void	ReleasePlayer(CPlayer* pPlayer);

#endif // PLAYER_H
