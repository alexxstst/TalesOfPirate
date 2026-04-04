#pragma once
#include "GameCommon.h"
#include "Point.h"
#include "Kitbag.h"
#include "SkillStateType.h"
#include "Tools.h"
#include "Shipset.h"
#include "uigoodsgrid.h"

class CActor;
class CCharacter;
class CSceneItem;
struct xShipBuildInfo;
struct BOAT_BERTH_DATA;

#define defMOVE_LIST_POINT_NUM	32
struct SMoveList
{
	POINT	SPos[defMOVE_LIST_POINT_NUM];
	int		nPointNum;
	DWORD   dwState;
};

#define defMAX_POS_NUM	32

struct stNetMoveInfo // enumACTION_MOVE
{
    DWORD dwAveragePing; //  NetIF::GetAveragePing() 
    POINT pos_buf[defMAX_POS_NUM]; // 
    DWORD pos_num; // 
	stNetMoveInfo() :
    dwAveragePing(0), pos_num(0){};
};

struct stNetSkillInfo // enumACTION_SKILL
{
	BYTE	byFightID;
	char	chMove;		// 12

	long	lSkillID;		// 0>0

	// WorldID,Handlex,y
	struct
	{
		long	lTarInfo1;
		long	lTarInfo2;
	};

	stNetMoveInfo	SMove;
};

struct stNetNotiMove // enumACTION_MOVE
{
    short    sState;                    // CompCommand.h EMoveState
    short    sStopState;                // enumEXISTS_WAITINGenumEXISTS_SLEEPING
    POINT    SPos[defMAX_POS_NUM];    // 
    long    nPointNum;                // 
	stNetNotiMove() :
    sState(0), sStopState(0), nPointNum(0){};
};

struct stEffect
{
	long	lAttrID;	// ID
	LONG64	lVal;		// 
};

struct stSkillState
{
	BYTE	chID;
	BYTE	chLv;       // ,
	unsigned long		lTimeRemaining;
};

struct stAreaSkillState
{
	BYTE	chID;
	BYTE	chLv;
	long	lWorldID;	// 
	unsigned char	uchFightID;
};

struct stNetNotiSkillRepresent // enumACTION_SKILL_SRC
{
	BYTE	byFightID;
	short	sAngle;         // server
	short	sState;			// CompCommand.h EFightState
	short	sStopState;		// enumEXISTS_WAITINGenumEXISTS_SLEEPING
	char	chCrt;          // 01

	long	lSkillID;		// 
	long	lSkillSpeed;	// 
	char	chObjType;		// 01
	long	lTargetID;		// ID
	POINT	STargetPoint;	// 
	short	sExecTime;	    // ,

	CSizeArray<stEffect>		SEffect;	// 
	CSizeArray<stSkillState>	SState;		// 
};

struct stNetNotiSkillEffect //  enumACTION_SKILL_TAR
{
	BYTE	byFightID;
	short	sState;			// CompCommand.h EFightState
	bool	bDoubleAttack;	// 
	bool	bMiss;			// Miss;
	bool	bBeatBack;		// 
	Point	SPos;			// 
	long	lSkillID;		// ID
	char	chObjType;		// 01
	long	lSrcID;			// ID
	Point	SSrcPos;		// 
	Point	SSkillTPos;		// 
	short	sExecTime;		// ,

	// 
	CSizeArray<stEffect>		SEffect;	// 
	CSizeArray<stSkillState>	SState;		// 

	// 
	short						sSrcState;	// CompCommand.h EFightState
	CSizeArray<stEffect>		SSrcEffect;	// 
	CSizeArray<stSkillState>	SSrcState;	// 
};

struct stNetPKCtrl
{
	bool	bInGymkhana{};	// Own PK switch
	bool	bInPK{};			// Whether it is an arena, that is, whether the PK switch can be operated
	bool	pkGuild{}; // Can pk guild members

	void	Exec( CCharacter* pCha );
	void	Exec( unsigned long	ulWorldID );
};

struct stNetChaSideInfo
{
	char	chSideID{};
};

struct stNetAppendLook
{
	short	sLookID[defESPE_KBGRID_NUM];
	bool	bValid[defESPE_KBGRID_NUM];

	void	Exec(unsigned long	ulWorldID);
	void	Exec(CCharacter* pCha);
};

class CSceneNode;
class CEvent;
struct stNetEvent
{
	long			lEntityID;
	char			chEntityType;					// 1-,2-
	unsigned short	usEventID;
	std::string		cszEventName;

	CEvent*			ChangeEvent();					// 

	CEvent*			Exec( CSceneNode* pNode );		// 

};

struct stNetLookInfo
{
	char	chSynType;	//  ESynLookTypeenumSYN_LOOK_CHANGESLook.sID0
	stNetChangeChaPart	SLook;
};

struct stNetActorCreate				// 
{
	stNetActorCreate() = default;

	unsigned long	ulWorldID{};
	unsigned long	ulCommID{};		// ID
	std::string szCommName;	// 
	long			lHandle{};		// 
	unsigned long	ulChaID{};
	char			chCtrlType{};		// NPCCompCommand.h EChaCtrlType
	int			chGuildPermission{};		// NPCCompCommand.h EChaCtrlType
	Circle			SArea{};
	short			sAngle{};			// server
	unsigned long	ulTLeaderID{};	// ID0
	short			sState{};			//  0x000x010x02
	std::string 			strMottoName;
	short			sIcon{};
	long			lGuildID{};
	std::string 			strGuildName;
	std::string 			strGuildMotto;
	std::string			strStallName;
	std::string szName;
	char			chSeeType{enumENTITY_SEEN_NEW};		// ,EEntitySeenType
	char			chGMLv{};			// GM
	int 			chIsPlayer{};
	
	
	stNetChaSideInfo	SSideInfo{};	// 
	stNetLookInfo		SLookInfo{};
	stNetPKCtrl		SPKCtrl{};
	stNetEvent		SEvent{};
	stNetAppendLook	SAppendLook{};

	char			chMainCha{};		// 0-,1-,2-

	CCharacter*		CreateCha();
	void			SetValue( CCharacter* pCha );
};

struct stNetNPCShow
{
	BYTE			byNpcType;	 //  
    BYTE            byNpcState;  // NPC,NPC

	void			SetNpcShow( CCharacter* pCha );
};

struct stNetLeanInfo // 
{
	char	chState;

	long	lPose;
	long	lAngle;
	long	lPosX, lPosY;
	long	lHeight;
};

struct stNetSwitchMap // 
{
	short	sEnterRet;
	std::string szMapName;
	char	chEnterType;
	bool	bIsNewCha;
	bool	bCanTeam;	// 
};

// 
struct stNetSysInfo		//
{
	std::string m_sysinfo;		//
};

struct stNetSay			// 
{
	unsigned long	m_srcid;	// ID
	const char	*	m_content;	// 
};

// 
// 1.,
// 2.,
// 3.,
// 
// 
// 
// 1.,
//    :1.,
//         2.

struct stNetItemCreate	// 
{
	long	lWorldID;
	long	lHandle;		// 
	long	lEntityAddr;	// 
	long	lID;
	Point	SPos;
	short	sAngle;
	short	sNum;			// 
	char	chAppeType;		// CompCommand.h EItemAppearType
	long	lFromID;		// ID0

	stNetEvent	SEvent;
};

struct stNetItemUnfix	// 
{
	char    chLinkID;	// Link
	short	sGridID;	// ,,-1,,,-2,
    long	lPosX;		// 
    long	lPosY;
};

struct stNetItemPick	// 
{
	long	lWorldID;
	long	lHandle;
};

struct stNetItemThrow	// 
{
	short	sGridID;	// 
	long	lNum;		// 
	long	lPosX;		// 
	long	lPosY;
};

struct stNetItemPos		// 
{
	short	sSrcGridID;	// 
	short	sSrcNum;	// ,
	short	sTarGridID;	// 
};

struct stNetBank		// 
{
	char	chSrcType;	//  0 1
	short	sSrcID;		// 
	short	sSrcNum;	// 
	char	chTarType;	// 
	short	sTarID;		// 
};

struct stNetTempKitbag	// 
{
	short   sSrcGridID;	// 
	short	sSrcNum;	// 
	short	sTarGridID;	// 
};

struct stNetUseItem		// 
{
	stNetUseItem() {sTarGridID = -1;}
	short	sGridID;	// 
	short	sTarGridID;	// 
};

struct stNetDelItem		// 
{
	short	sGridID;	// 
};

struct stNetItemInfo	// 
{
	char	chType;
	short	sGridID;	// 
};

struct stTempChangeChaPart
{
	DWORD			dwPartID;
	DWORD           dwItemID;
};

struct stNetKitbag				    // 
{
	char	chBagType;				// 0 1
	char	chType;					// CompCommand.hESynKitbagType
    int     nKeybagNum;             // 
    struct stGrid
    {
	    short	    sGridID;		// ID
	    SItemGrid	SGridContent;
    };
    int     nGridNum;               // 
    stGrid  Grid[defMAX_KBITEM_NUM_PER_TYPE];     
};

struct stNetSkillBag			    // 
{
	char			chType;			// CompCommand.hESynSkillBagType
	CSizeArray<SSkillGridEx>		SBag;	// 
};

struct stNetDefaultSkill
{
	short	sSkillID;

	void	Exec(void);
};

struct stNetSkillState				// 
{
	char	chType;					// 
	CSizeArray<stSkillState> SState;
};

struct stNetChangeCha				// 
{
	unsigned long	ulMainChaID;	// ID
};

struct stNetActivateEvent
{
	long	lTargetID;
	long	lHandle;
	short	sEventID;
};

struct stNetFace                    // ,enumACTION_FACE
{
    short   sPose;              
    short	sAngle;
};

struct stLookEnergy
{
	short	sEnergy[enumEQUIP_NUM];
};

// 
typedef struct _NET_FUNCITEM
{
	char szFunc[ROLE_MAXNUM_FUNCITEMSIZE];
} NET_FUNCITEM, *PNET_FUNCITEM;

typedef struct _NET_MISITEM
{
	char szMis[ROLE_MAXNUM_FUNCITEMSIZE];
	BYTE byState;

} NET_MISITEM, *PNET_MISITEM;

typedef struct _NET_FUNCPAGE
{
	char szTalk[ROLE_MAXNUM_DESPSIZE];
	NET_FUNCITEM FuncItem[ROLE_MAXNUM_FUNCITEM];
	NET_MISITEM  MisItem[ROLE_MAXNUM_CAPACITY];

} NET_FUNCPAGE, *PNET_FUNCPAGE;

// 
typedef struct _NET_MISSIONLIST
{
	BYTE byListType;
	BYTE byPrev;
	BYTE byNext;
	BYTE byPrevCmd;
	BYTE byNextCmd;
	BYTE byItemCount;
	NET_FUNCPAGE FuncPage;

} NET_MISSIONLIST, *PNET_MISSIONLIST;


#define HELPINFO_DESPSIZE 4096

// 
typedef struct _NET_HELPINFO
{
	BYTE byType;
	union
	{
		char szDesp[HELPINFO_DESPSIZE];//[ROLE_MAXNUM_DESPSIZE];
		USHORT sID;
	};

} NET_HELPINFO, *PNET_HELPINFO;

// 
typedef struct _NET_MISNEED
{
	BYTE byType;
	union
	{
		struct
		{
			WORD wParam1;
			WORD wParam2;
			WORD wParam3;
		};
		char szNeed[ROLE_MAXNUM_NEEDDESPSIZE];
	};

} NET_MISNEED, *PNET_MISNEED;

typedef struct _NET_MISPRIZE
{
	BYTE byType;
	WORD wParam1;
	WORD wParam2;

} NET_MISPRIZE, *PNET_MISPRIZE;

typedef struct _NET_MISPAGE
{
	BYTE byNeedNum;
	NET_MISNEED MisNeed[ROLE_MAXNUM_MISNEED];

	BYTE byPrizeSelType;
	BYTE byPrizeNum;
	NET_MISPRIZE MisPrize[ROLE_MAXNUM_MISPRIZE];

	// 
	char szName[ROLE_MAXSIZE_MISNAME];
	char szDesp[ROLE_MAXNUM_DESPSIZE];

} NET_MISPAGE, *PNET_MISPAGE;

typedef struct _NET_MISLOG
{
	BYTE byType;
	BYTE byState;
	WORD wMisID;

} NET_MISLOG, *PNET_MISLOG;

typedef struct _NET_MISLOG_LIST
{
	BYTE	   byNumLog;
	NET_MISLOG MisLog[ROLE_MAXNUM_MISSION];

} NET_MISLOG_LIST, *PNET_MISLOG_LIST;

// 
typedef struct _NET_TRADEPAGE
{
	BYTE   byCount;
	USHORT sItemID[ROLE_MAXNUM_TRADEITEM];
	USHORT sCount[ROLE_MAXNUM_TRADEITEM];
	DWORD  dwPrice[ROLE_MAXNUM_TRADEITEM];
	BYTE   byLevel[ROLE_MAXNUM_TRADEITEM];

} NET_TRADEPAGE, *PNET_TRADEPAGE;

typedef struct _NET_TRADEINFO
{
	NET_TRADEPAGE TradePage[mission::MAXTRADE_ITEMTYPE];

	_NET_TRADEINFO()
	{
		Clear();
	}

	void Clear()
	{
		memset( TradePage, 0, sizeof(NET_TRADEPAGE)*mission::MAXTRADE_ITEMTYPE );
	}

} NET_TRADEINFO, *PNETTRADEINFO;

struct NET_CHARTRADE_BOATDATA
{
	char szName[BOAT_MAXSIZE_BOATNAME];
	USHORT sBoatID;
	USHORT sLevel;
	DWORD dwExp;
	DWORD dwHp;
	DWORD dwMaxHp;
	DWORD dwSp;
	DWORD dwMaxSp;
	DWORD dwMinAttack;
	DWORD dwMaxAttack;
	DWORD dwDef;
	DWORD dwSpeed;
	DWORD dwShootSpeed;
	BYTE  byHasItem;
	BYTE  byCapacity;
	DWORD dwPrice;

};

// 
typedef struct _NET_CHARTRADE_ITEMDATA
{
	BYTE byForgeLv;
	BYTE byHasAttr;

	std::array<std::array<short, 2>, defITEM_INSTANCE_ATTR_NUM> sInstAttr;
	std::array<short, 2> sEndure;
	std::array<short, 2> sEnergy;

	std::array<long, enumITEMDBP_MAXNUM> lDBParam;
	bool	bValid;
	bool	bItemTradable;
	long	expiration;

} NET_CHARTRADE_ITEMDATA, *PNET_CHARTRADE_ITEMDATA;

// 
#define MAX_GUILD_CHALLLEVEL				3	// 
typedef struct _NET_GUILD_CHALLINFO
{
	BYTE byIsLeader;
	BYTE byLevel[MAX_GUILD_CHALLLEVEL];
	BYTE byStart[MAX_GUILD_CHALLLEVEL];
	char szGuild[MAX_GUILD_CHALLLEVEL][64];
	char szChall[MAX_GUILD_CHALLLEVEL][64];
	DWORD dwMoney[MAX_GUILD_CHALLLEVEL];

} NET_GUILD_CHALLINFO, *PNET_GUILD_CHALLINFO;

struct NetChaBehave
{
	std::string		sCharName;			//
	std::string		sJob;				//
	short			iDegree;			//
	Look_Minimal look_minimal;
};

struct stNetShortCutChange
{
	//stNetShortCutChange() : shyGrid2(-1){};
	char     chIndex;
	char     chType;
	short    shyGrid;
	//short    shyGrid2;
};

struct stNetNpcMission
{
    BYTE    byType;     // 
    USHORT  sNum;		// 
    USHORT  sID;	    // ID
    USHORT  sCount;     //     
};

struct stNetAreaState
{
	short				sAreaX;			// 
	short				sAreaY;
	char				chStateNum;
	stAreaSkillState	State[AREA_STATE_NUM];
};

struct stNetChaAttr
{
	char		chType;
	short		sNum;
	stEffect	SEff[MAX_ATTR_CLIENT];
};

struct stNetQueryRelive
{
	char		chType;	// CompCommandEPlayerReliveType
	std::string	szSrcChaName;
};

// 
struct stNetOpenHair
{
	void Exec();
};

// 
struct stNetUpdateHair
{
	short	sScriptID;			// HairID,0
	short	sGridLoc[4];		// 
};

// 
struct stNetUpdateHairRes
{
	unsigned long	ulWorldID;			// 
	int				nScriptID;			// ID
	std::string		szReason;			// ,:ok,fail,:

	void	Exec();
};

// 
struct stNetTeamFightAsk
{
	struct
	{
		std::string	szName;
		std::string	szJob;
		char		chLv;
		unsigned short usVictoryNum;
		unsigned short usFightNum;
	}Info[10];
	char	chSideNum1;
	char	chSideNum2;

	void	Exec();
};

struct stNetItemRepairAsk
{
	std::string	cszItemName;
	long	lRepairMoney;

	void	Exec();
};

struct stSCNetItemForgeAsk
{
	char	chType;
	long	lMoney;

	void	Exec();
};

struct stNetItemForgeAnswer
{
	long	lChaID;
	char	chType;
	char	chResult;
	void	Exec();
};

// Add by lark.li 20080516 begin
struct stSCNetItemLotteryAsk
{
	long	lMoney;

	void	Exec();
};

struct stNetItemLotteryAnswer
{
	long	lChaID;
	char	chResult;
	void	Exec();
};
// End

#define	defMAX_FORGE_GROUP_NUM	defMAX_ITEM_FORGE_GROUP

struct SForgeCell
{
	SForgeCell() : sCellNum(0), pCell(0) {}
	~SForgeCell() 
	{
		delete[] pCell;
	}

	short	sCellNum;
	struct  SCell
	{
		short	sPosID;
		short	sNum;
	} *pCell;
};

struct stNetItemForgeAsk
{
	char	chType;	// 12
	SForgeCell	SGroup[defMAX_FORGE_GROUP_NUM];
};

struct SLotteryCell
{
	SLotteryCell() : sCellNum(0), pCell(0) {}
	~SLotteryCell() 
	{
		delete[] pCell;
	}

	short	sCellNum;
	struct  SCell
	{
		short	sPosID;
		short	sNum;
	} *pCell;
};

#define	defMAX_LOTTERY_GROUP_NUM	7

struct stNetItemLotteryAsk
{
	SLotteryCell	SGroup[defMAX_LOTTERY_GROUP_NUM];
};

struct stNetEspeItem
{
	char	chNum;
	struct
	{
		short	sPos;
		short	sEndure;
		short	sEnergy;
		bool	bItemTradable;
		long	expiration;
	} SContent[4];
};

// 
struct stBlackTrade
{
	short sIndex;		// 
	short sSrcID;		// ID
	short sSrcNum;		// 
	short sTarID;		// ID
	short sTarNum;		// 
	short sTimeNum;		// time
};


struct stChurchChallenge
{
	short sChurchID;	// id
	char  szName[32];	// name
	char  szChaName[32];// 
	short sCount;		// 
	long  nBasePrice;	// 
	long  nMinbid;		// 
	long  nCurPrice;	// 

	stChurchChallenge()	{ memset(this, 0, sizeof(stChurchChallenge)); }
};

struct BankLog {
		unsigned short type;
		time_t time;
		unsigned long long parameter; // ItemID or Gold value
		short quantity; // 1-99 for items, 0 for gold;
		short userID; // chaID of the actor
};



extern void	NetLoginSuccess(char byPassword, uint8_t maxCharacters, std::span<const NetChaBehave> characters);
extern void NetLoginFailure(unsigned short Errno);	                //
extern void	NetBeginPlay(unsigned short Errno);
extern void	NetEndPlay(uint8_t maxCharacters, std::span<const NetChaBehave> characters);
extern void NetNewCha(unsigned short Errno);
extern void NetDelCha(unsigned short Errno);
extern void NetCreatePassword2(unsigned short Errno);
extern void NetUpdatePassword2(unsigned short Errno);

extern void NetActorDestroy(unsigned int nID, char chSeeType);
extern void NetActorMove(unsigned int nID, stNetNotiMove &list);
extern void NetActorSkillRep(unsigned int nID, stNetNotiSkillRepresent &skill);
extern void NetActorSkillEff(unsigned int nID, stNetNotiSkillEffect &skill);
extern void NetActorLean(unsigned int nID, stNetLeanInfo &lean);
extern void NetSwitchMap(stNetSwitchMap	&switchmap);
extern void NetSysInfo(stNetSysInfo &sysinfo);
extern void NetSideInfo(const char szName[], const char szInfo[]);
extern void NetBickerInfo( const char szData[] );
extern void NetColourInfo( unsigned int rgb, const char szData[] );
extern void NetSay(stNetSay &netsay,DWORD dwColour = 0xffffffff);
extern CSceneItem* NetCreateItem(stNetItemCreate &pSCreateInfo);
extern void NetItemDisappear(unsigned int nID);
extern void NetChangeChaPart( CCharacter* pCha, stNetLookInfo &SLookInfo );
extern void NetChangeChaPart( unsigned int nID, stNetLookInfo &SLookInfo );
extern void NetTempChangeChaPart( unsigned int nID, stTempChangeChaPart &SPart );
extern void NetActorChangeCha(unsigned int nID, stNetChangeCha &SChangeCha);
extern void NetShowTalk( const char szTalk[], BYTE byCmd, DWORD dwNpcID );
extern void NetShowHelp( const NET_HELPINFO& Info );
extern void NetShowMapCrash(const char szTalk[]);
extern void NetShowFunction( BYTE byFuncPage, BYTE byItemNum, BYTE byMisNum, const NET_FUNCPAGE& FuncArray, DWORD dwNpcID );
extern void NetShowMissionList( DWORD dwNpcID, const NET_MISSIONLIST& MisList );
extern void NetShowMisPage( DWORD dwNpcID, BYTE byCmd, const NET_MISPAGE& page );
extern void	NetMisLogList( NET_MISLOG_LIST& List );
extern void NetShowMisLog( WORD wMisID, const NET_MISPAGE& page );
extern void NetMisLogClear( WORD wMisID );
extern void NetMisLogAdd( WORD wMisID, BYTE byState );
extern void NetMisLogState( WORD wID, BYTE byState );
extern void NetCloseTalk( DWORD dwNpcID );
extern void NetCreateBoat( const xShipBuildInfo& Info );
extern void NetUpdateBoat( const xShipBuildInfo& Info );
extern void NetBoatInfo( DWORD dwBoatID, const char szName[], const xShipBuildInfo& Info );
extern void NetShowBoatList( DWORD dwNpcID, BYTE byNumBoat,  const BOAT_BERTH_DATA& Data, BYTE byType = mission::BERTH_LUANCH_LIST );
extern void NetChangeChaLookEnergy( unsigned int nID, stLookEnergy &SLookEnergy );
extern void NetQueryRelive(unsigned int nID, stNetQueryRelive &SQueryRelive);
extern void NetPreMoveTime(unsigned long ulTime);
extern void NetMapMask(unsigned int nID, BYTE *pMask, long lLen);

// npc
extern void NetShowTrade( const NET_TRADEINFO& TradeInfo, BYTE byCmd, DWORD dwNpcID, DWORD dwParam );
extern void NetUpdateTradeAllData( const NET_TRADEINFO& TradeInfo, BYTE byCmd, DWORD dwNpcID, DWORD dwParam );
extern void NetUpdateTradeData( DWORD dwNpcID, BYTE byPage, BYTE byIndex, USHORT sItemID, USHORT sCount, DWORD dwPrice );

// npc
extern void NetTradeResult( BYTE byCmd, BYTE byIndex, BYTE byCount, USHORT sItemID, DWORD dwMoney );

// 
extern void NetShowCharTradeRequest( BYTE byType, DWORD dwRequestID );

// 
extern void NetShowCharTradeInfo( BYTE byType, DWORD dwAcceptID, DWORD dwRequestID );

// 
extern void NetCancelCharTrade( DWORD dwCharID );

// 
extern void NetValidateTradeData( DWORD dwCharID );

// 
extern void NetValidateTrade( DWORD dwCharID );

// 
extern void NetTradeAddBoat( DWORD dwCharID, BYTE byOpType, USHORT sItemID, BYTE byIndex, 
							BYTE byCount, BYTE byItemIndex, const NET_CHARTRADE_BOATDATA& Data );

// 
extern void NetTradeAddItem( DWORD dwCharID, BYTE byOpType, USHORT sItemID, BYTE byIndex, 
							BYTE byCount, BYTE byItemIndex, const NET_CHARTRADE_ITEMDATA& Data );

// 
extern void NetTradeShowMoney( DWORD dwCharID, DWORD dwMoney );
extern void NetTradeShowIMP( DWORD dwCharID, DWORD dwMoney );

// 
extern void NetTradeSuccess();
extern void NetTradeFailed();

// 
extern void NetStallInfo( DWORD dwCharID, BYTE byNum, const char szName[] );
extern void NetStallAddBoat( BYTE byGrid, USHORT sItemID, BYTE byCount, DWORD dwMoney, NET_CHARTRADE_BOATDATA& Data );
extern void NetStallAddItem( BYTE byGrid, USHORT sItemID, BYTE byCount, DWORD dwMoney, NET_CHARTRADE_ITEMDATA& Data );
extern void NetStallDelGoods( DWORD dwCharID, BYTE byGrid, BYTE byCount );
extern void NetStallClose( DWORD dwCharID );
extern void NetStallSuccess( DWORD dwCharID );
extern void NetStallName(DWORD dwCharID, const char *szStallName);
extern void NetStartExit( DWORD dwExitTime );
extern void NetCancelExit();
extern void NetSynAttr( DWORD dwWorldID, char chType, short sNum, stEffect *pEffect );
extern void NetFace(DWORD dwCharID, stNetFace& netface, char chType);
extern void NetChangeKitbag(DWORD dwChaID, stNetKitbag& SKitbag);
extern void NetNpcStateChange( DWORD dwChaID, BYTE byState );
extern void NetEntityStateChange( DWORD dwEntityID, BYTE byState );
extern void NetShortCut( DWORD dwChaID, stNetShortCut& stShortCut );  
extern void NetTriggerAction( stNetNpcMission& info );
extern void NetShowForge();

extern void NetShowUnite();
extern void NetShowMilling();
extern void NetShowFusion();
extern void NetShowUpgrade();
extern void NetShowEidolonMetempsychosis();
extern void NetShowEidolonFusion();
extern void NetShowPurify();
extern void NetShowGetStone();
extern void NetShowRepairOven();
extern void NetShowEnergy();
extern void NetShowTiger();

extern void NetSynSkillBag(DWORD dwCharID, stNetSkillBag *pSSkillBag);
extern void NetSynSkillState(DWORD dwCharID, stNetSkillState *pSSkillState);
extern void NetAreaStateBeginSee(stNetAreaState *pState);
extern void NetAreaStateEndSee(stNetAreaState *pState);
extern void NetFailedAction( char chState );
extern void NetShowMessage( long lMes );
extern void NetChaTLeaderID(long lID, long lLeaderID);
extern void NetChaEmotion( long lID, short sEmotion );

extern void NetChaSideInfo( long lID, stNetChaSideInfo &SNetSideInfo );
extern void NetBeginRepairItem(void);

extern void NetItemUseSuccess(unsigned int nID, short sItemID);
extern void NetKitbagCapacity(unsigned int nID, short sKbCap);
extern void NetKitbagLockedSpaces(short slots,CGoodsGrid* grd);
extern void NetEspeItem(unsigned int nID, stNetEspeItem& SEspeItem);

extern void NetKitbagCheckAnswer(bool bLock);
extern void NetChaPlayEffect(unsigned int uiWorldID, int nEffectID);

extern void NetChurchChallenge(const stChurchChallenge* pInfo);

