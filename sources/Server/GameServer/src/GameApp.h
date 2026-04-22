//=============================================================================
// FileName: GameApp.h
// Creater: ZhangXuedong
// Date: 2004.11.04
// Comment: CChaMgr class
//=============================================================================

#ifndef GAMEAPP_H
#define GAMEAPP_H


//#define DISABLE_GM_CMD				//	gm


#include "GameAppNet.h"
#include "TrackedPool.h"
#include "point.h"
#include "RoleData.h"
#include "Config.h"
#include "Identity.h"
#include "Player.h"
#include "SkillStateRecord.h"
#include "SkillStateRecordStore.h"
#include "Script.h"
#include "AreaRecord.h"
#include "SkillTemp.h"
#include "StateCell.h"
#include "LuaAPI.h"
#include "CommFunc.h"
#include "GamePool.h"
#include "MapRes.h"
#include "PicSet.h"

#define defMAX_CHARINFO_NO 2500

class CCharacter;
class CPlayer;
class CItem;
class CPassengerMgr;

namespace mission
{
 class CTalkNpc;
 class CNpc;
 class CTradeData;
 class CStallData;
}

class SubMap;
class CCharacter;
class CItemRecordAttr;

class AssetDatabase;

class GateServer;


#define MAX_DBLOG_POOL 100

struct SDBLogData
{
	int		nLoc;			// pool
	char	szLog[8192];	//
	SDBLogData():nLoc(0) {}
};


class CDBLogMgr // dblog
{

public:

	CDBLogMgr();

	// Log 5, 8000
	void Log(const char *type, const char *c1, const char *c2, const char *c3, const char *c4, const char *p, BOOL bAddToList = TRUE);

	// Add by lark.li 20080324 begin
	void TradeLog(const char* action, const char *pszChaFrom, const char *pszChaTo, const char *pszTrade);
	// End

	void HandleLogList();
	void FlushLogList();  // GameServer, logDB

	int  GetLogLeft();
	int  SetPerLogCnt(int nCnt);
	int  GetPerLogCnt();

protected:

	std::list<SDBLogData*>	_LogList;
	int					_nPerLogCnt;				// log
	int					_nLogLeft;					// log
	SDBLogData			_LogPool[MAX_DBLOG_POOL];	// log, pool
	int					_nPoolUseLoc;				// pool
};

struct SVolunteer
{
	char szName[defENTITY_NAME_LEN];	//
	unsigned long ulID;					// ID
	long lLevel;						//
	long lJob;							//
	char szMapName[256];				//
};

class	CGameApp : public CDBLogMgr
{
public:

    CGameApp();
	~CGameApp();

	BOOL	Init();
	BOOL    InitMap();
    void	Run(DWORD dwCurTime);

	// 4
	void	ProcessNetMsg(int nMsgType, GateServer *pGate, net::RPacket& pkt);
	void    OnGateConnected(GateServer *pGate, net::RPacket& pkt);
	void	OnGateDisconnect(GateServer *pGate, net::RPacket& pkt);
	void	ProcessPacket(GateServer* pGate, net::RPacket& pkt);
	void	ProcessTeamMsg(GateServer *pGate, const net::msg::PmTeamMessage& teamMsg);
	void	ProcessGuildMsg(GateServer *pGate, const net::msg::PmGuildInfoMessage& msg);
	void	ProcessGuildChallMoney(GateServer *pGate, const net::msg::PmGuildChallMoneyMessage& msg);
	void	ProcessGuildChallPrizeMoney(GateServer *pGate, const net::msg::PmGuildChallPrizeMoneyMessage& msg);
	void	ProcessDynMapEntry(GateServer *pGate, const net::msg::MapEntryMessage& mapMsg);
	void	ProcessInterGameMsg(unsigned short usCmd, GateServer *pGate, net::RPacket& pkt);
	void	Handle_GuildApprove(const net::msg::MmGuildApproveMessage& msg);
	void	Handle_CallCha(const net::msg::MmCallChaMessage& msg);
	void	Handle_GotoCha(GateServer* pGate, long lGatePlayerID, long lGatePlayerAddr, const net::msg::MmGotoChaMessage& msg);
	void	Handle_KickCha(const net::msg::MmKickChaMessage& msg);
	void	Handle_GuildChallPrizeMoney(const net::msg::MmGuildChallPrizeMoneyMessage& msg);
	void	Handle_AddCredit(const net::msg::MmAddCreditMessage& msg);
	void	Handle_StoreBuy(const net::msg::MmStoreBuyMessage& msg);
	void	Handle_AddMoney(const net::msg::MmAddMoneyMessage& msg);
	void	ProcessGroupBroadcast(unsigned short usCmd, GateServer *pGate, net::RPacket& pkt);
	void	ProcessGarner2Update(const net::msg::PmGarner2UpdateLegacyMessage& msg);

	//------------------------------------


    CPlayer*    CreateGamePlayer(const char szPassword[], uLong ulChaDBId, uLong ulWorldId, const char *cszMapName, char chType);
    void        ReleaseGamePlayer(CPlayer*);
	void		GoOutGame(CPlayer* pPlayer, bool bOffLine, bool mOffLine = false);
	CPlayer*	GetNewPlayer();
	CPlayer*	GetPlayer(long lHandle);
	CPlayer*	IsValidPlayer(long lID, long lHandle);
    CCharacter* GetNewCharacter();
	CItem*		GetNewItem();
	mission::CTalkNpc*	GetNewTNpc();
	Entity*		GetEntity(long lHandle);
	Entity*		IsValidEntity(unsigned long ulID, long lHandle);
	Entity*		IsLiveingEntity(unsigned long ulID, long lHandle);
	Entity*		IsMapEntity(unsigned long ulID, long lHandle);
	Entity*		IsLifeEntity(unsigned long ulID, long lHandle);
	void		AddPlayerIdx(DWORD dwDBID, CPlayer* pPlayer);
	void		DelPlayerIdx(DWORD dwDBID);
	CPlayer*    GetPlayerByDBID(DWORD dwDBID);
	CPlayer*	GetPlayerByMainChaName(	const	char*	sMainChaName	);
	void		AfterPlayerLogin(const char *cszPlyName);
	void		NoticePlayerLogin(CPlayer *pCPlayer);

	CMapRes*	GetMap(int no);
	CMapRes*	FindMapByName(const char *mapname, bool bIncUnRun = false);
	void		LoadAllTable(void);
	void		LoadCharacterInfo(void);
	void		LoadSkillInfo(void);
	void		LoadItemInfo(void);
	void		SetGlobalRates(float droprate, float exprate);
	float		GetGlobalDropRate();
	float		GetGlobalExpRate();
	// npc
	BOOL		ReloadNpcInfo( CCharacter& character );
	mission::CNpc* FindNpc( const char szName[] );

	// NPC
	BOOL		SummonNpc( BYTE byMapID, USHORT sAreaID, const char szNpc[], USHORT sTime );

	//
	mission::CEventEntity* CreateEntity( BYTE byType );

	void		NotiGameReset(unsigned long ulLeftSec);
	void		SaveAllPlayer(void);

	void		SetEntityEnableLog(bool bValid = true);
	CSkillTempData*	GetSkillTData(short sSkillNo, char chSkillLv);
	void		SetSkillTDataRange(short *psRange);
	void		SetSkillTDataState(short *psState);
	void		InitSStateTraOnTime();
	long		GetSStateTraOnTime(unsigned char uchStateID, unsigned char uchStateLv);

	void		DataStatistic(void); //
	CCharacter*	FindPlayerChaByName(const char* cszChaName);
	CCharacter*	FindPlayerChaByNameLua(const char* cszChaName);
	int FindPlayerChaByActNameLua(const char* cszChaName,CCharacter* chas[3]);
	bool		DealAllInGuild(int guildID, const char* luaFunc, const char* luaParam);
	CCharacter*	FindPlayerChaByID(unsigned long ulChaID);
	CCharacter* FindMainPlayerChaByID(unsigned long ulChaID);
	CCharacter*	FindChaByID(unsigned long ulChaID);
	CCharacter*	FindChaByName(const char* cszChaName);
	CPlayer*	FindPlayerByDBChaID(unsigned long ulDBChaID);

	void		WorldNotice(const char *szString); // GameServer
	void		GuildNotice(unsigned long guildID, const char *szString);
	void		ScrollNotice(const char * szString,int SetNum, DWORD color = 0xFFFFFF00);//Add by sunny.sun20080804
	void		GMNotice(const char * szString);//add by sunny.sun 20080821
	void		LocalNotice(const char *szString); // GameServer
	void		ChaNotice(const char *szNotiString, const char *szChaName = ""); //

	bool		IsChaAttrMaxValInit(void);
	void		ChaAttrMaxValInit(bool bSet);

	void		CheckSeeWithTeamChange(bool CanSeen[][2], CPlayer **pCPlayerList, char chMemberCnt);
	void		RefreshTeamEyeshot(bool CanSeenOld[][2], bool CanSeenNew[][2], CPlayer **pCPlayerList, char chMemberCnt, char chRefType);
	void		RefreshTeamEyeshot(CPlayer **pCPlayerList, char chMemberCnt, char chRefType);

	BOOL		AddVolunteer(CCharacter *pCha);
	BOOL		DelVolunteer(CCharacter *pCha);
	SVolunteer	*GetVolInfo(int nIndex);
	int			GetVolNum();
	SVolunteer	*FindVolunteer(const char *szName);

	void		BanAccount(const char *szString);
	void		UnbanAccount(const char *szString);
	void		CanReceiveRequests(uLong chaID, bool CanSend);
	short		GetMapNum();

	DWORD   m_dwFPS;
	DWORD   m_dwRunCnt;
	DWORD	m_dwChaCnt;
	DWORD	m_dwPlayerCnt;
	DWORD	m_dwActiveMgrUnit;
	std::atomic<LONG>   m_dwRunStep;	//

	BOOL	m_bExecLuaCmd;
	std::string	m_strMapNameList;

	TrackedPool<CPassengerMgr>				m_CabinPool{"Cabin"};
	TrackedPool<mission::CTradeData>		m_TradeDataPool{"TradeData"};
	TrackedPool<mission::CStallData>		m_StallDataPool{"StallData"};
	TrackedPool<CSkillTempData>				m_SkillTDataPool{"SkillTData"};
	TrackedPool<CStateCell>					m_MapStateCellPool{"StateCell"};
	TrackedPool<CChaListNode>				m_ChaListPool{"ChaList"};
	TrackedPool<CStateCellNode>				m_StateCellNodePool{"StateCellNode"};

	//
	dbc::Long	m_lCabinHeapNum;
	dbc::Long	m_lTradeDataHeapNum;
	dbc::Long	m_lSkillTDataHeapNum;
	dbc::Long	m_lMapMgrUnitHeapNum;
	dbc::Long	m_lEntityListHeapNum;
	dbc::Long	m_lMgrNodeHeapNum;
	//

	Identity			m_Ident;
	Identity			m_ItemIdent;

	struct // GameServer
	{
		unsigned long	m_ulLeftSec;
	    CTimer			m_CTimerReset;
	};

protected:
	void	MgrUnitRun(DWORD dwCurTime);
	void	GameItemRun(DWORD dwCurTime);
	void	MapMgrRun(DWORD dwCurTime);

protected:
	std::vector<CMapRes*> m_MapList;
	short	 m_mapnum;

	std::map<DWORD, CPlayer*>  _PlayerIdx;  // DB IDPlayer

	std::vector<SVolunteer>	m_vecVolunteerList;	//

	struct
	{
		CPlayer		*pCPlayerL;	  // socket
		CPlayer		*pCCurPlayer; //
	} m_GatePlayer[MAX_GATE];

public:
	CPicSet			   *m_PicSet;


private:

	DWORD   _dwFPS;
    DWORD   _dwLastTick;
    DWORD   _dwRunCnt;
    DWORD   _dwTempRunCnt;
	float   m_fGlobalDropRate;
	float   m_fGlobalExpRate;
	//
	struct
	{
		CSkillTempData	*m_pCSkillTData[defMAX_SKILL_NO + 1][defMAX_SKILL_LV + 1];
		short			m_sSkillSetNo;
		char			m_chSkillSetLv;
	};

	long	m_lSStateTraOnTime[AREA_STATE_MAXID + 1][SKILL_STATE_LEVEL + 1];	//

    CTimer	m_CTimerItem;

	bool	m_bChaAttrMaxValInit;
};

enum EChaTimerAction
{
	enumCHA_TIMEER_ENTERMAP,
};


struct SSwitchMapInfo //
{
	SubMap		*pSrcMap;
	char		szSrcMapName[256];
	Point		SSrcPos;
	char		szTarMapName[256];
	Point		STarPos;
};

extern bool             g_bLogEntity;

extern CGameApp*        g_pGameApp;
extern std::unordered_map<int, CItemRecordAttr> g_itemAttrMap;
extern CCharacter*		g_pCSystemCha;		//
extern SubMap *			g_pScriptMap;		//
extern long				g_lDeftMMaskLight;	//
extern std::string			g_strChaState[2];	// 01
extern uLong			g_ulCurID;
extern Long				g_lCurHandle;
extern HANDLE			hConsole;

#define C_PRINT(s, ...) \
	SetConsoleTextAttribute(hConsole, 14); \
	printf(s, __VA_ARGS__); \
	SetConsoleTextAttribute(hConsole, 10);

#define C_TITLE(s) \
	char szPID[32]; \
	_snprintf_s(szPID,sizeof(szPID),_TRUNCATE, "%d", GetCurrentProcessId()); \
	std::string strConsoleT; \
	strConsoleT += "[PID:"; \
	strConsoleT += szPID; \
	strConsoleT += "]"; \
	strConsoleT += s; \
	SetConsoleTitle(strConsoleT.c_str());

#define defINVALID_CHA_ID		0
#define defINVALID_CHA_HANDLE	-1

#endif // GAMEAPP_H
