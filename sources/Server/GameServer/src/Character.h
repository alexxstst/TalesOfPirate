//=============================================================================
// FileName: Character.h
// Creater: ZhangXuedong
// Date: 2004.10.19
// Comment: CCharacter class
//=============================================================================

#ifndef CHARACTER_H
#define CHARACTER_H

#include "MoveAble.h"
#include "GameCommon.h"
#include "Mission.h"
#include "Timer.h"
#include "Kitbag.h"
#include "ShipSet.h"
#include "Action.h"

#define defMAX_ITEM_NUM			10
#define defCHA_SCRIPT_TIMER		1 * 1000
#define defDEFAULT_PING_VAL		500
#define defPING_RECORD_NUM		6
#define defPING_INTERVAL		20 * 1000
#define defCHA_SCRIPT_PARAM_NUM	1

extern CCharacter*		g_pCSystemCha;		// 

struct SLean // 
{
	dbc::uLong	ulPacketID; // ID
	dbc::Char	chState;	// 0.1
	dbc::Long	lPose;
	dbc::Long	lAngle;
	dbc::Long	lPosX, lPosY;
	dbc::Long	lHeight;
};

struct SSeat
{
	dbc::Char	chIsSeat;
	dbc::Short	sAngle;
	dbc::Short	sPose;
};

struct STempChaPart
{
	short	sPartID;
	short	sItemID;
};

struct SCheatX
{
	uInt Xtype;			//1: 2:
	uInt Xerror;		//
	uInt Xright;		//
	uInt Xcount;		//
	uInt Xn;
	DWORD dwLastTime;	//
	DWORD dwInterval;	//
	std::string Xnum;		//X number
};

enum EActControl
{
	enumACTCONTROL_MOVE,		// 
	enumACTCONTROL_USE_GSKILL,	// 
	enumACTCONTROL_USE_MSKILL,	// 
	enumACTCONTROL_BEUSE_SKILL,	// 
	enumACTCONTROL_TRADE,		// 
	enumACTCONTROL_USE_ITEM,	// 
	enumACTCONTROL_BEUSE_ITEM,	// 
	enumACTCONTROL_INVINCIBLE,	// 
	enumACTCONTROL_EYESHOT,		// 
	enumACTCONTROL_NOHIDE,		// 
	enumACTCONTROL_NOSHOW,		// 
	enumACTCONTROL_ITEM_OPT,	// 
	enumACTCONTROL_TALKTO_NPC,	// NPC
	enumACTCONTROL_MAX,
};

enum ESwitchMapType
{
	enumSWITCHMAP_CARRY,	// 
	enumSWITCHMAP_DIE,		// 
};

enum ELogAssetsType	// 
{
	enumLASSETS_INIT,		// 
	enumLASSETS_TRADE,		// 
	enumLASSETS_BANK,		// 
	enumLASSETS_PICKUP,		// 
	enumLASSETS_THROW,		// 
	enumLASSETS_DELETE,		// 
};

namespace mission
{
	class CStallData;
	class CTradeData;
	class CTalkNpc;
}

#define LOOK_SELF   0
#define LOOK_OTHER  1
#define LOOK_TEAM   2

class CHateMgr;
class CAction;
class CActionCache;

namespace net { namespace msg {
	struct ChaBaseInfo;
	struct ChaSkillBagInfo;
	struct ChaKitbagInfo;
	struct ChaShortcutInfo;
	struct ChaSkillStateInfo;
	struct ChaAttrInfo;
	struct BoatData;
}}

class CCharacter : public CMoveAble
{
	friend class CChaSpawn;
	friend class PlayerStorage;
	friend class Guild;
	friend class CTableGuild;
public:
	CCharacter();
	~CCharacter();

	void	Initially();
	void	Finally();

	bool	IsPlayerCha(void); // 
	bool	IsGMCha(); // GM0-10GM
	bool	IsGMCha2(); // GM
	bool	IsPlayerCtrlCha(void); // 
	bool	IsPlayerMainCha(void); // 
	bool	IsPlayerFocusCha(void); //  IsPlayerCtrlCha
	bool	IsPlayerOwnCha(void); // 
	CCharacter	*GetPlyCtrlCha(void);
	CCharacter	*GetPlyMainCha(void);

	void		SetIMP(int x, bool sync = true);
	int			GetIMP();

	float		GetDropRate();
	float       GetExpRate();

	void ItemUnlockRequest(const net::msg::CmItemUnlockAskMessage& msg);

	void	WriteInt64PartInfo(net::WPacket& packet);
	void	SwitchMap(SubMap *pCSrcMap, cChar *szTarMapName, Long lTarX, Long lTarY, bool bNeedOutSrcMap = true, Char chSwitchType = enumSWITCHMAP_CARRY, Long lTMapCpyNO = -1);

	virtual void	ProcessPacket(uShort usCmd, net::RPacket& pk);

	//    (  ProcessPacket) 
	void Handle_GuildBankCmd(const net::msg::PmGuildBankMessage& msg);
	void Handle_PushToGuildBank(const std::string& strItem);
	void Handle_Ping(const net::msg::CmPingResponseMessage& msg);
	void Handle_Say(const net::msg::CmSayMessage& msg);
	void Handle_RequestTalk(uLong npcId, net::RPacket& pk);
	void Handle_DailyBuffRequest();
	void Handle_RefreshData(const net::msg::CmRefreshDataMessage& msg);
	void Handle_MapMask();
	void Handle_ItemForgeAsk(const net::msg::CmItemForgeGroupAskMessage& msg);
	void Handle_ItemLotteryAsk(const net::msg::CmItemLotteryGroupAskMessage& msg);
	void Handle_LifeSkillAsk(const net::msg::CmLifeSkillCraftMessage& msg);
	void Handle_LifeSkillAsr(const net::msg::CmLifeSkillCraftMessage& msg);
	void Handle_StoreCommand(uShort usCmd, net::RPacket& pk);
	void Handle_TigerStart(const net::msg::CmTigerStartMessage& msg);
	void Handle_TigerStop(const net::msg::CmTigerStopMessage& msg);
	void Handle_VolunteerOpen(const net::msg::CmVolunteerOpenMessage& msg);
	void Handle_VolunteerList(const net::msg::CmVolunteerListMessage& msg);
	void Handle_VolunteerSel(const net::msg::CmVolunteerSelMessage& msg);
	void Handle_VolunteerAsr(const net::msg::CmVolunteerAsrMessage& msg);
	void Handle_KitbagTempSync();
	void Handle_ItemLockAsk(const net::msg::CmItemLockAskMessage& msg);
	void Handle_GameRequestPin(const net::msg::CmGameRequestPinMessage& msg);
	void Handle_MasterInvite(const net::msg::CmMasterInviteMessage& msg);
	void Handle_MasterAsr(const net::msg::CmMasterAsrMessage& msg);
	void Handle_MasterDel(const net::msg::CmMasterDelMessage& msg);
	void Handle_PrenticeDel(const net::msg::CmPrenticeDelMessage& msg);

	virtual void	Run(uLong ulCurTick);
	virtual	void	RunEnd( DWORD dwCurTime );
	virtual void	OnScriptTimer(DWORD dwExecTime, bool bNotice = false);
	virtual void	StartExit();
	virtual void	CancelExit();
	virtual void	Exit();

	void	CheatRun(DWORD dwCurTime);
	void	CheatCheck(cChar *answer);
	void	CheatConfirm();
	void	InitCheatX();
	DWORD	GetCheatInterval(int state);

	// 
	bool		Cmd_EnterMap(dbc::cChar* l_map, dbc::Long lMapCopyNO, dbc::uLong l_x, dbc::uLong l_y, dbc::Char chLogin = 1);
	void		Cmd_BeginMove(dbc::Short sPing, Point *pPath, dbc::Char chPointNum, dbc::Char chStopState = enumEXISTS_WAITING);
	void		Cmd_BeginMoveDirect(Entity *pTar);
	void		Cmd_BeginSkill(dbc::Short sPing, Point *pPath, dbc::Char chPointNum,
				CSkillRecord *pSkill, dbc::Long lSkillLv, dbc::Long lTarInfo1, dbc::Long lTarInfo2, dbc::Char chStopState = enumEXISTS_WAITING);
	void		Cmd_BeginSkillDirect(dbc::Long lSkillNo, Entity *pTar, bool bIntelligent = true);
	void		Cmd_BeginSkillDirect2(dbc::Long lSkillNo, dbc::Long lSkillLv, dbc::Long lPosX, dbc::Long lPosY);
	dbc::Short	Cmd_UseItem(dbc::Short sSrcKbPage, dbc::Short sSrcKbGrid, dbc::Short sTarKbPage, dbc::Short sTarKbGrid);
	dbc::Short	Cmd_UseEquipItem(dbc::Short sKbPage, dbc::Short sKbGrid, bool bRefresh = true,bool rightHand = false);
	dbc::Short	Cmd_UseExpendItem(dbc::Short sSrcKbPage, dbc::Short sSrcKbGrid, dbc::Short sTarKbPage, dbc::Short sTarKbGrid, bool bRefresh = true);
	dbc::Short	Cmd_UnfixItem(dbc::Char chLinkID, dbc::Short *psItemNum, dbc::Char chDir, dbc::Long lParam1, dbc::Long lParam2, bool bPriority = true, bool bRefresh = true, bool bForcible = false);
	dbc::Short	Cmd_PickupItem(dbc::uLong ulID, dbc::Long lHandle);
	dbc::Short	Cmd_ThrowItem(dbc::Short sKbPage, dbc::Short sKbGrid, dbc::Short *psThrowNum, dbc::Long lPosX, dbc::Long lPosY, bool bRefresh = true, bool bForcible = false);
	dbc::Short	Cmd_ItemSwitchPos(dbc::Short sKbPage, dbc::Short sSrcGrid, dbc::Short sSrcNum, dbc::Short sTarGrid);
	dbc::Short	Cmd_DelItem(dbc::Short sKbPage, dbc::Short sKbGrid, dbc::Short *psThrowNum, bool bRefresh = true, bool bForcible = false);
	dbc::Short	Cmd_BankOper(dbc::Char chSrcType, dbc::Short sSrcGridID, dbc::Short sSrcNum, dbc::Char chTarType, dbc::Short sTarGridID);
	dbc::Short	Cmd_GuildBankOper(dbc::Char chSrcType, dbc::Short sSrcGridID, dbc::Short sSrcNum, dbc::Char chTarType, dbc::Short sTarGridID);
	
    //(sSrcGrid:   sSrcNum:   sTarGrid:)
    dbc::Short  Cmd_DragItem(dbc::Short sSrcGrid, dbc::Short sSrcNum, dbc::Short sTarGrid);
    
	void Cmd_SetInPK(bool bInPK = true);
	void Cmd_SetInGymkhana(bool bInGymkhana = true);
	void Cmd_SetPKGuild(bool v);

	void		Cmd_ReassignAttr(const net::msg::CmSynAttrMessage& msg);
	dbc::Short	Cmd_RemoveItem(dbc::Long lItemID, dbc::Long lItemNum, dbc::Char chFromType, dbc::Short sFromID, dbc::Char chToType, dbc::Short sToID, bool bRefresh = true, bool bForcible = true);

	void		Cmd_ChangeHair(net::RPacket& pk);											// 
	void		Prl_ChangeHairResult(int nScriptID, const char* szReason, BOOL bNoticeAll = FALSE); // 
	void		Prl_OpenHair();															// 	

	void		Cmd_FightAsk(dbc::Char chType, dbc::Long lTarID, dbc::Long lTarHandle);
	void		Cmd_FightAnswer(bool bFight);
	void		Cmd_ItemRepairAsk(dbc::Char chPosType, dbc::Char chPosID);
	void		Cmd_ItemRepairAnswer(bool bRepair);
	void		Cmd_ItemForgeAsk(dbc::Char chType, SForgeItem *pSItem);
	void		Cmd_ItemForgeAnswer(bool bForge);

	// ADd by lark.li 20080515 begin
	void		Cmd_ItemLotteryAsk(SLotteryItem *pSItem);
	void		Cmd_ItemLotteryAnswer(bool bForge);
	// End
	
	//
	void		Cmd_Garner2_Reorder(short index);

	//
	void		Cmd_LifeSkillItemAsk(long dwType, SLifeSkillItem *pSItem);
	void		Cmd_LifeSkillItemAsR(long dwType,SLifeSkillItem *pSItem);
    //
    void        Cmd_LockKitbag();
    void        Cmd_UnlockKitbag(const char szPassword[]);
    void        Cmd_CheckKitbagState();
    void        Cmd_SetKitbagAutoLock(Char cAuto);

	//
	BOOL		Cmd_AddVolunteer();
	BOOL		Cmd_DelVolunteer();
	void		Cmd_ListVolunteer(short sPage, short sNum);
	BOOL		Cmd_ApplyVolunteer(const char *szName);
	CCharacter	*FindVolunteer(const char *szName);
	
	virtual void	ReflectINFof(Entity *srcent, net::WPacket chginf);
	virtual CCharacter *IsCharacter();





	void	TradeClear( CPlayer& player );
	bool	TradeAction(bool bLock = true);
	bool	StallAction(bool bLock = true);
	bool	HairAction(bool bLock = true);
	bool	RepairAction(bool bLock = true);
	bool	ForgeAction(bool bLock = true);

	void	BickerNotice( const char szData[], ... );
	void	SystemNotice( const char szData[], ... );
	void	PopupNotice( const char szData[], ... );
	void	ColourNotice( DWORD rgb, const char szData[], ... );
	bool	IsPKSilver();

	//
	void	SetTradeData( mission::CTradeData* pData );
	mission::CTradeData* GetTradeData();

	//
	void	SetBoat( CCharacter* pBoat );
	CCharacter* GetBoat();
	bool	IsBoat(void);
	
	// npc
	BOOL	SafeSale( BYTE byIndex, BYTE byCount, WORD& wItemID, DWORD& dwMoney );
	BOOL	SafeBuy( WORD wItemID, BYTE byCount, BYTE byIndex, DWORD& dwMoney );
	BOOL	SafeSaleGoods( DWORD dwBoatID, BYTE byIndex, BYTE byCount, WORD& wItemID, DWORD& dwMoney );
	BOOL	SafeBuyGoods( DWORD dwBoatID, WORD wItemID, BYTE byCount, BYTE byIndex, DWORD& dwMoney );
	BOOL	GetSaleGoodsItem( DWORD dwBoatID, BYTE byIndex, WORD& wItemID );

	BOOL	ExchangeReq(short sSrcID, short sSrcNum, short sTarID, short sTarNum);

	bool	SetNarmalSkillState(bool bAdd = true, dbc::uChar uchStateID = 1, dbc::uChar uchStateLv = 1);
	bool	HasTradeAction(void);
	//
	BOOL	SetMissionPage( DWORD dwNpcID, BYTE byPrev, BYTE byNext, BYTE byState );
	BOOL	GetMissionPage( DWORD dwNpcID, BYTE& byPrev, BYTE& byNext, BYTE& byState );
	BOOL	SetTempData( DWORD dwNpcID, WORD wID, BYTE byState, BYTE byType );
	BOOL	GetTempData( DWORD dwNpcID, WORD& wID, BYTE& byState, BYTE& byType );

	// 
	BOOL	SaveMissionData();	// 

	BOOL	AddMissionState( DWORD dwNpcID, BYTE byID, BYTE byState );
	BOOL	ResetMissionState( mission::CTalkNpc& npc );
	
	BOOL	GetMissionState( DWORD dwNpcID, BYTE& byState );
	BOOL	GetNumMission( DWORD dwNpcID, BYTE& byNum );
	BOOL	GetMissionInfo( DWORD dwNpcID, BYTE byIndex, BYTE& byID, BYTE& byState );
	BOOL	GetCharMission( DWORD dwNpcID, BYTE byID, BYTE& byState );
	BOOL	GetNextMission( DWORD dwNpcID, BYTE& byIndex, BYTE& byID, BYTE& byState );
	BOOL	ClearMissionState( DWORD dwNpcID );
	
	BOOL	AddTrigger( const mission::TRIGGER_DATA& Data );
	BOOL	ClearTrigger( WORD wTriggerID );
	BOOL	DeleteTrigger( WORD wTriggerID );
	BOOL	GetMisScriptID( WORD wID, WORD& wScriptID );
	BOOL	AddRole( WORD wID, WORD wParam );
	BOOL	HasRole( WORD wID );
	BOOL	ClearRole( WORD wID );
	BOOL	IsRoleFull();
	BOOL	SetFlag( WORD wID, WORD wFlag );
	BOOL	ClearFlag( WORD wID, WORD wFlag );
	BOOL	IsFlag( WORD wID, WORD wFlag );
	BOOL	IsValidFlag( WORD wFlag );
	BOOL	SetRecord( WORD wRec );
	BOOL	ClearRecord( WORD wRec );
	BOOL	IsRecord( WORD wRec );
	BOOL	IsValidRecord( WORD wRec );

	// 
	BOOL	HasRandMission( WORD wRoleID );
	BOOL	AddRandMission( WORD wRoleID, WORD wScriptID, BYTE byType, BYTE byLevel, DWORD dwExp, DWORD dwMoney, USHORT sPrizeData, USHORT sPrizeType, BYTE byNumData );
	BOOL	SetRandMissionData( WORD wRoleID, BYTE byIndex, const mission::MISSION_DATA& RandData );
	BOOL	GetRandMission( WORD wRoleID, BYTE& byType, BYTE& byLevel, DWORD& dwExp, DWORD& dwMoney, USHORT& sPrizeData, USHORT& sPrizeType, BYTE& byNumData );
	BOOL	GetRandMissionData( WORD wRoleID, BYTE byIndex, mission::MISSION_DATA& RandData );

	// npc(NPC)
	BOOL	HasSendNpcItemFlag( WORD wRoleID, WORD wNpcID );
	BOOL	NoSendNpcItemFlag( WORD wRoleID, WORD wNpcID );
	BOOL	HasRandMissionNpc( WORD wRoleID, WORD wNpcID, WORD wAreaID );

	// 
	BOOL	TakeRandNpcItem( WORD wRoleID, WORD wNpcID, const char szNpc[] );
	BOOL	TakeAllRandItem( WORD wRoleID );

	// 
	BOOL	IsMisNeedItem( USHORT sItemID );
	BOOL	GetMisNeedItemCount( WORD wRoleID, USHORT sItemID, USHORT& sCount );
	void	RefreshNeedItem( USHORT sItemID );

	// 
	void	MisLog();
	void	MisLogInfo( WORD wMisID );
	void	MisLogClear( WORD wMisID );

	// 
	BOOL	SetMissionComplete( WORD wRoleID );
	BOOL	SetMissionFailure( WORD wRoleID );
	BOOL	HasMissionFailure( WORD wRoleID );

	// 
	BOOL	CompleteRandMission( WORD wRoleID );
	BOOL	FailureRandMission( WORD wRoleID );
	BOOL	AddRandMissionNum( WORD wRoleID );
	BOOL	ResetRandMission( WORD wRoleID );
	BOOL	ResetRandMissionNum( WORD wRoleID );
	BOOL	HasRandMissionCount( WORD wRoleID, WORD wCount );
	BOOL	GetRandMissionCount( WORD wRoleID, WORD& wCount );
	BOOL	GetRandMissionNum( WORD wRoleID, WORD& wNum );

	// NPC
	BOOL	ConvoyNpc( WORD wRoleID, BYTE byIndex, WORD wNpcCharID, BYTE byAiType );
	BOOL	ClearConvoyNpc( WORD wRoleID, BYTE byIndex );
	BOOL	ClearAllConvoyNpc( WORD wRoleID );
	BOOL	HasConvoyNpc( WORD wRoleID, BYTE byIndex );
	BOOL	IsConvoyNpc( WORD wRoleID, BYTE byIndex, WORD wNpcCharID );

	// 
	void	AddMoney( const char szName[], DWORD dwMoney );
	BOOL	TakeMoney( const char szName[], DWORD dwMoney );
	BOOL	HasMoney( DWORD dwMoney );
	BOOL	AddItem( USHORT sItemID, USHORT sCount, const char szName[], BYTE byAddType = enumITEM_INST_TASK, BYTE bySoundType = enumSYN_KITBAG_FROM_NPC, BOOL isTradable = true, LONG expiration = 0, short* posID = NULL);
	BOOL	TakeItem( USHORT sItemID, USHORT sCount, const char szName[] );
	BOOL	GiveItem( USHORT sItemID, USHORT sCount, BYTE byAddType, BYTE bySoundType, BOOL isTradable = true, LONG expiration = 0, Short* posID = NULL);
	int		GiveItemReturnPosition(USHORT sItemID, USHORT sCount, BYTE byAddType, BYTE bySoundType);
	BOOL	MakeItem( USHORT sItemID, USHORT sCount, USHORT& sItemPos, BYTE byAddType = enumITEM_INST_TASK, BYTE bySoundType = enumSYN_KITBAG_FROM_NPC );
	BOOL	HasItem( USHORT sItemID, USHORT sCount );
	BOOL	GetNumItem( USHORT sItemID, USHORT& sCount );
	BOOL	HasLeaveBagGrid( USHORT sNum );
	BOOL	HasLeaveBagTempGrid( USHORT sNum );
	BOOL	HasItemBagTemp(USHORT sItemID, USHORT sCount);
	BOOL	TakeItemBagTemp(USHORT sItemID, USHORT sCount, const char szName[]);

	//  (   InfoNet.h)
	struct ItemInfo
	{
		short       itemID;
		unsigned char itemNum;
		unsigned char itemFlute;
		short       itemAttrID[5];
		short       itemAttrVal[5];
	};

	//
	BOOL	AddItem2KitbagTemp( USHORT sItemID, USHORT sCount, ItemInfo *pItemAttr, BYTE bySoundType = enumSYN_KITBAG_FROM_NPC );
	BOOL	AddItem2KitbagTemp( USHORT sItemID, USHORT sCount, const char szName[], BYTE byAddType = enumITEM_INST_TASK, BYTE bySoundType = enumSYN_KITBAG_FROM_NPC );
	BOOL	GiveItem2KitbagTemp( USHORT sItemID, USHORT sCount, ItemInfo *pItemAttr, BYTE bySoundType );
	BOOL	GiveItem2KitbagTemp( USHORT sItemID, USHORT sCount, BYTE byAddType, BYTE bySoundType );

	// 
	BOOL	SetProfession( BYTE byPf );

	bool	LearnSkill(dbc::Short sSkillID, dbc::Char chLv, bool bSetLv = true, bool bUsePoint = true, bool bLimit = true); // 
	bool	AddSkillState(dbc::uChar uchFightID, dbc::uLong ulSrcWorldID, dbc::Long lSrcHandle, dbc::Char chObjType, dbc::Char chObjHabitat, dbc::Char chEffType,
			dbc::uChar uchStateID, dbc::uChar uchStateLv, dbc::Long lOnTick, dbc::Char chType = enumSSTATE_ADD_UNDEFINED, bool bNotice = true); // 
	bool	DelSkillState(dbc::uChar uchStateID, bool bNotice = true); // 

	//
	bool	GetActControl(dbc::Char chCtrlType);
	void	Hide();
	void	Show();

	// 
	void	RestoreHp( BYTE byHpRate );
	void	RestoreSp( BYTE bySpRate );
	void	RestoreAllHp();
	void	RestoreAllSp();
	void	RestoreAll();

	BOOL	ViewItemInfo( const net::msg::CmActionViewItemData& msg );

	BOOL	AddAttr( int nIndex, DWORD dwValue, dbc::Short sNotiType = enumATTRSYN_TASK ); // ATTR_CEXPCFightAble::AddExp
	BOOL	TakeAttr( int nIndex, DWORD dwValue, dbc::Short sNotiType = enumATTRSYN_TASK );

	bool	    IsInPK(void);
	bool	    IsInGymkhana(void);
	void	    SetPKCtrl(dbc::Char chCtrl);
	dbc::Char   GetPKCtrl(void);
	bool	    CanPK(void);
	bool	    IsInArea(dbc::Short sAreaMask);
	void	SetRelive(Char chType = enumEPLAYER_RELIVE_ORIGIN, Char chLv = 0, cChar *szInfo = 0);
	void	Reset(void);

	virtual void BreakAction(net::RPacket* pk = nullptr);
	virtual void EndAction(net::RPacket* pk = nullptr);
	// 
	virtual void AfterObjDie(CCharacter *pCAtk, CCharacter *pCDead);
	virtual void AfterPeekItem(dbc::Short sItemID, dbc::Short sNum);
	virtual void AfterEquipItem(dbc::Short sItemID, dbc::uShort sTriID);
	virtual void EntryMapUnit( BYTE byMapID, WORD wxPos, WORD wyPos );
	virtual void OnMissionTime(); // 
	virtual void OnLevelUp( USHORT sLevel );
	virtual void OnSailLvUp( USHORT sLevel );
	virtual void OnLifeLvUp( USHORT sLevel );
	virtual void OnCharBorn();

	// 
	BOOL	IsNeedRepair();
	BOOL	IsNeedSupply();
	void	RepairBoat();
	void	SupplyBoat();
	void	BoatDie( CCharacter& Attacker, CCharacter& Boat );
	BOOL	OnBoatDie( CCharacter& Attacker );
	BOOL	GetBoatID( BYTE byIndex, DWORD& dwBoatID );
	BOOL	BoatCreate( const BOAT_DATA& Data );
	BOOL	BoatUpdate( BYTE byIndex, const BOAT_DATA& Data );
	BOOL	BoatLoad( const BOAT_LOAD_INFO& Info );
	
	// 
	BOOL	AdjustTradeItemCess( USHORT sLowCess, USHORT sData );
	BOOL	GetTradeItemData( BYTE& byLevel, USHORT& sCess );
	BOOL	SetTradeItemLevel( BYTE byLevel );	
	BOOL	HasTradeItemLevel( BYTE byLevel );
	BOOL	GetTradeItemLevel( BYTE& byLevel );
	BOOL	BoatTrade( USHORT sBerthID );	

	BOOL	BoatBerth( USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir );	
	BOOL	BoatLaunch( BYTE byIndex, USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir );
	BOOL	BoatBerthList( DWORD dwNpcID, BYTE byType, USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir );
	BOOL	BoatSelLuanch( BYTE byIndex );
	BOOL	BoatSelected( BYTE byType, BYTE byIndex );
	BOOL	HasAllBoatInBerth( USHORT sBerthID );
	BOOL	HasBoatInBerth( USHORT sBerthID );
	BOOL	HasDeadBoatInBerth( USHORT sBerthID );
	void	SetBoatLook( const stNetChangeChaPart& Info );
	BOOL	BoatPackBagList( USHORT sBerthID, BYTE byType, BYTE byLevel );
	BOOL	BoatPackBag( BYTE byIndex );
	BOOL	PackBag( CCharacter& boat, BYTE byType, BYTE byLevel );
	BOOL	PackBag( CCharacter& Boat, USHORT sItemID, USHORT sCount, USHORT sPileID, USHORT& sNumPack );
	void	SetBoatAttrChangeFlag(bool bSet = true);
	void	SyncBoatAttr( dbc::Short sSynType, bool bAllBoat = true ); // 

	// 
	BOOL	BoatAdd( CCharacter& Boat );
	BOOL	BoatClear( CCharacter& Boat );
	BOOL	BoatAdd( DWORD dwDBID );
	BOOL	BoatClear( DWORD dwDBID );

	// 
	BOOL	SetEntityState( DWORD dwEntityID, BYTE byState );
	void	SetEntityTime( DWORD dwTime );
	DWORD	GetEntityTime();

	// 
	BOOL	HasGuild();

	// 
	void	SetStallData( mission::CStallData* pData );
	mission::CStallData* GetStallData();
	BYTE	GetStallNum();

	//add by jilinlee 2007/4/20
	//
	BOOL IsReadBook();
	void SetReadBookState(bool bIsReadBook = false);


	// 
	void	ChangeItem(bool bEquip, SItemGrid *pItemCont, dbc::Char chLinkID); // 
	void	SkillRefresh(); // 
	// 
	void	NewChaInit(void);
	// 
	void	ChaInitEquip(void);
	void	ResetBirthInfo(void);

	// 
	void	SynKitbagNew(dbc::Char chType); // 
	void    SynKitbagTmpNew(dbc::Char chType); // 
	void	SynShortcut(); // 
	void	SynLook(dbc::Char chSynType = enumSYN_LOOK_SWITCH); // 
	void	SynLook(dbc::Char chLookType, bool verbose);
	bool	ItemForge(SItemGrid *pItem, dbc::Char chAddLv = 1); // 
	void	SynSkillBag(dbc::Char chType); // 
	void	SynPKCtrl(void); // PK
	void	SynAddItemCha(CCharacter *pCItemCha);
	void	SynDelItemCha(CCharacter *pCItemCha);
	void	CheckPing(void);
	void	SendPreMoveTime(void);
	void	SynSideInfo(void);
	void	SynBeginItemRepair(void);
	void	SynBeginItemForge(void);
	
	void	SynBeginItemLottery(void);	//Add by lark.li 20080513

	void	SynBeginItemUnite(void);
	void	SynBeginItemMilling(void);
	void	SynBeginItemFusion();
	void    SynBeginItemUpgrade();
	void    SynBeginItemEidolonMetempsychosis();
	void	SynBeginItemEidolonFusion();
	void	SynBeginItemPurify();
	void	SynBeginItemFix();
	void	SynBeginItemEnergy();
	void	SynBeginGetStone();
	void	SynBeginTiger();
	void	SynAppendLook(void);
	void	SynItemUseSuc(dbc::Short sItemID);
	void	SynKitbagCapacity(void);
	void	SynEspeItem(void);
	void	SynVolunteerState(BOOL bState);
	void	SynTigerString(cChar *szString);

	//

	// 
	void	WriteBaseInfo(net::WPacket &pk, dbc::Char chLookType = LOOK_SELF);
	void	WritePKCtrl(net::WPacket &pk);
	void	WriteSkillbag(net::WPacket &pk, int nSynType);
	void	WriteKitbag(CKitbag &CKb, net::WPacket &pk, int nSynType);
	net::msg::ChaKitbagInfo BuildKitbagInfo(CKitbag &CKb, int nSynType);
	void	WriteLookData(net::WPacket &pk, dbc::Char chLookType = LOOK_SELF, dbc::Char chSynType = enumSYN_LOOK_SWITCH);
	net::msg::ChaLookInfo BuildLookInfo(dbc::Char chLookType = LOOK_SELF, dbc::Char chSynType = enumSYN_LOOK_SWITCH);
	bool	WriteAppendLook(CKitbag &CKb, net::WPacket &pk, bool bInit = false);
	void	WriteInt64cut(net::WPacket &pk);
	void	WriteBoat(net::WPacket &pk);
	void	WriteItemChaBoat(net::WPacket &pk, CCharacter *pCBoat);
	void	WriteSideInfo(net::WPacket &pk);
	// Fill*     (CommandMessages.h)
	void	FillBaseInfo(net::msg::ChaBaseInfo &b, dbc::Char chLookType = LOOK_SELF);
	void	FillSkillBag(net::msg::ChaSkillBagInfo &s, int nSynType);
	void	FillKitbag(net::msg::ChaKitbagInfo &k, CKitbag &CKb, int nSynType);
	void	FillShortcut(net::msg::ChaShortcutInfo &s);
	void	FillBoats(std::vector<net::msg::BoatData> &boats);
	//

	// 
	void	FailedActionNoti(dbc::Char chType, dbc::Char chReason);
	// 
	void	TerminalMessage(dbc::Long lMessageID);
	// 
	void	ItemOprateFailed(dbc::Short sFailedID);

	void		SetMotto(dbc::cChar *szMotto);
	dbc::cChar*	GetMotto(void);
	void		SetIcon(dbc::uShort usIcon);
	dbc::uShort	GetIcon(void);
	void		SetGuildName(dbc::cChar *szGuildName);
	dbc::cChar*	GetGuildName(void);
	dbc::cChar*	GetValidGuildName(void);
	void		SetGuildMotto(dbc::cChar *szGuildMotto);
	dbc::cChar*	GetGuildMotto(void);
	dbc::cChar*	GetValidGuildMotto(void) ;
	void		SetGuildID( DWORD dwGuildID );
	DWORD		GetGuildID();
	DWORD		GetValidGuildID();
	void		SetGuildState( uLong lState );
	uLong		GetGuildState();
	void		SetEnterGymkhana(bool bEnter = true);
	void		SyncGuildInfo();
	void		SetStallName(dbc::cChar *szStallName);
	dbc::cChar*	GetStallName(void);
	void		SynStallName(void);

	void			AddBlockCnt();
	BYTE			GetBlockCnt();
	void			SetBlockCnt(BYTE cnt);

	virtual void	AfterAttrChange(int nIdx, dbc::Long lOldVal, dbc::Long lNewVal);
	virtual void	Die();	// 
	void			JustDie(CCharacter *pCSrcCha);	// 
	void			MoveCity(dbc::cChar *szCityName, Long lMapCpyNO = -1, Char chSwitchType = enumSWITCHMAP_CARRY);
	void			BackToCity(bool Die = false, cChar *szCityName = 0, Long lMapCpyNO = -1, Char chSwitchType = enumSWITCHMAP_DIE);
	void			BackToCityEx(bool Die = false, cChar *szCityName = 0, Long lMapCpyNO = -1, Char chSwitchType = enumSWITCHMAP_DIE);
	void			SetToMainCha(bool bBoatDie = false);
	bool			CanSeen(CCharacter *pCCha);
	bool			CanSeen(CCharacter *pCCha, bool bThisEyeshot, bool bThisNoHide, bool bThisNoShow);
	bool			IsHide();
	SItemGrid*		GetEquipItem(dbc::Char chPart);
	DWORD			GetTeamID();
	bool			IsTeamLeader();
	dbc::Long		GetSideID();
	void			SetSideID(dbc::Long lSideID);
	SItemGrid*		GetItem(dbc::Char chPosType, dbc::Long lItemID);
	SItemGrid*		GetItem2(dbc::Char chPosType, dbc::Long lPosID);
	bool			SetEquipValid(dbc::Char chEquipPos, bool bValid, bool bSyn = true);
	bool			SetKitbagItemValid(dbc::Short sPosID, bool bValid, bool bRecheckAttr = true, bool bSyn = true);
	bool			SetKitbagItemValid(SItemGrid* pSItem, bool bValid, bool bRecheckAttr = true, bool bSyn = true);
	bool			ItemIsAppendLook(SItemGrid* pSItem);
	void			SetLookChangeFlag(bool bChange = false);
	void			SetEspeItemChangeFlag(bool bChange = false);
	dbc::Char		GetLookChangeNum(void);
	bool			CheckForgeItem(SForgeItem *pSItem = NULL);
	bool			DoForgeLikeScript(dbc::cChar *cszFunc, dbc::Long &lRet);
	bool			DoLifeSkillcript(dbc::cChar *cszFunc, dbc::Long &lRet);
	bool			DoTigerScript(dbc::cChar *cszFunc);
	void			SetInOutMapQueue(bool bOutMap = true);
	bool			InOutMapQueue(void);
	bool			AddKitbagCapacity(dbc::Short sAddVal);
	void			CheckItemValid(SItemGrid* pCItem);
	void			CheckEspeItemGrid(void);
	dbc::Short		KbPushItem(bool bRecheckAttr, bool bSynAttr, SItemGrid *pGrid, dbc::Short &sPosID, dbc::Short sType = 0, bool bCommit = true, bool bSureOpr = false);
	dbc::Short		KbPopItem(bool bRecheckAttr, bool bSynAttr, SItemGrid *pGrid, dbc::Short sPosID, dbc::Short sType = 0, bool bCommit = true);
	dbc::Short		KbClearItem(bool bRecheckAttr, bool bSynAttr, dbc::Short sPosID, dbc::Short sType = 0);
	dbc::Short		KbClearItem(bool bRecheckAttr, bool bSynAttr, SItemGrid *pGrid, dbc::Short sNum = 0);
	dbc::Short		KbRegroupItem(bool bRecheckAttr, bool bSynAttr, dbc::Short sSrcPosID, dbc::Short sSrcNum, dbc::Short sTarPosID, dbc::Short sType = 0);
	void			ResetScriptParam(void);
	dbc::Long		GetScriptParam(dbc::Char chID);
	bool			SetScriptParam(dbc::Char chID, dbc::Long lVal);
	void			CheckBagItemValid(CKitbag* pCBag);
	void			CheckLookItemValid(void);
	bool			String2LookDate(std::string &strData);
	bool			String2KitbagData(std::string &strData);
    bool            String2KitbagTmpData(std::string &strData);


	void	SetKitbagRecDBID(long lDBID);
	long	GetKitbagRecDBID(void);

    //ID
    void	SetKitbagTmpRecDBID(long lDBID);
	long	GetKitbagTmpRecDBID(void);

	void	LogAssets(dbc::Char chLType);
	bool	SaveAssets(void);
	bool	IsRangePoint(dbc::Long lPosX, dbc::Long lPosY, dbc::Long lDist);
	bool	IsRangePoint(const Point &SPos, dbc::Long lDist);
	bool	IsRangePoint2(dbc::Long lPosX, dbc::Long lPosY, dbc::Long lDist2);
	bool	IsRangePoint2(const Point &SPos, dbc::Long lDist2);
	void	SetDBSaveInterval(long lIntl);
	long	GetDBSaveInterval(void);
	void	ResetPosState(void);

	int		GetLotteryIssue();

	DWORD				m_dwBoatCtrlTick; // 

	// AI----------------------------------------------------------------
	//  CAICharacter, Character
	BYTE				m_AIType;
	CCharacter*			m_AITarget;		
	CCharacter*			m_HostCha;		  // 
	int					m_nPatrolX;       // 
	int					m_nPatrolY;
	short				m_sChaseRange;    // , 
	BYTE				m_btPatrolState;  // , 0 1
	                                      //           1 2
                                          //           2 12
	                                      //           3 21
public:

	void	ResetAIState();				  // AI, 

	BOOL		GetChaRelive();
	void		SetChaRelive();
	void		ResetChaRelive();

	void		SetVolunteer(BOOL bVol);
	BOOL		IsVolunteer();
	void		SetInvited(BOOL bInvited);
	BOOL		IsInvited();

	void		SetCredit(long lCredit);
	dbc::Long	GetCredit();
	void			AddMasterCredit(long lCredit);
	unsigned long	GetMasterDBID();

	long			GetStoreItemID();
	void			SetStoreItemID(long lStoreItemID);
	bool			IsStoreBuy();
	void			SetStoreBuy(bool bValue);
	int				GetPetNum();
	void			SetPetNum(int nPetNum);

	bool			CheckStoreTime(DWORD dwInterval);
	void			ResetStoreTime();

	bool			IsStoreEnable();
	void			SetStoreEnable(bool bStoreEnable);

    //
    bool            IsScaleFlag();
    void            SetScaleFlag();
    void            SetExpScale(DWORD scale);
    DWORD           GetExpScale();
    int m_noticeState;//
	int m_retry3;
	int m_retry4;
	int m_retry5;
    int m_retry6;

	std::int64_t guildPermission{};

	unsigned int chatColour;
protected:

public:

    virtual void OnBeginSee(Entity *);
	virtual void OnEndSee(Entity *);
    virtual void OnBeginSeen(CCharacter *pCCha);
	virtual void OnEndSeen(CCharacter *pCCha);
	virtual void AreaChange(void);

	virtual void OnAI(DWORD dwCurTime);
    virtual void OnAreaCheck(DWORD dwCurTime);
	virtual void OnTeamNotice(DWORD dwCurTime);
    virtual void OnDBUpdate(DWORD dwCurTime);

	virtual void BeginAction(const net::msg::CmBeginActionMessage& msg);
	virtual void AfterStepMove(void);
	virtual void SubsequenceFight();
	virtual void SubsequenceMove();

	void	OnDie(DWORD dwCurTime);
	void	SrcFightTar(CFightAble *pTar, dbc::Short sSkillID);
	
	void	DoCommand(dbc::cChar *cszCommand, dbc::uLong ulLen);
	BOOL	DoGMCommand(const char *pszCmd, const char *pszParam);
	void	DoCommand_CheckStatus(dbc::cChar *pszCommand, dbc::uLong ulLen);
	void	HandleHelp(dbc::cChar *pszCommand, dbc::uLong ulLen);

	long	ExecuteEvent(Entity *pCObj, dbc::uShort usEventID);

	void	SetActControl(dbc::Char chCtrlType, bool bSet = true);

	bool		CanLearnSkill(CSkillRecord *pCSkill, dbc::Char chToLv);
	dbc::Short	CanEquipItem(dbc::Short sItemID);
	dbc::Short	CanEquipItemNew(dbc::Short sItemID1, dbc::Short sItemID2 = 0);

	dbc::Short	CanEquipItem(SItemGrid* pSEquipIt);

	dbc::Short	IsItemExpired(SItemGrid* pSEquipIt);

public:
	CKitbag				m_CKitbag;			// 
	CKitbag				*m_pCKitbagTmp;       // 
	stNetShortCut		m_CShortcut;		// 
	long				m_lKbRecDBID;		// ID
	long				m_lKbTmpRecDBID;	// ID
	long				m_lStoreItemID;
	bool				m_bStoreBuy;
	DWORD				m_dwStoreTime;
	bool				m_bStoreEnable;
	//cooldown for ranking
	DWORD GuildBankCD{ 0 };
	DWORD				ShowRankColD;	
	int					m_nPetNum;
	
	stNetChangeChaPart	m_SChaPart;
	bool				m_ActContrl[enumACTCONTROL_MAX];	// 
	CTimer				m_timerScripts;						// HPSP

	CHateMgr*			m_pHate;							// 

	BOOL				m_bRelive;							// 

	BOOL				m_bVol;								// 
	BOOL				m_bInvited;							// 

	//SSkillGrid			*m_pSkillGridTemp;

	// 
	struct
	{
		Char	m_chSelRelive;	// 
		Char	m_chReliveLv;	// 0.
	};

	CActionCache		m_CActCache;

	DWORD	m_dwCellRunTime[16];

	short	m_sTigerItemID[9];	// 9ID
	short	m_sTigerSel[3];		// 
	
private:
	BOOL BoatEnterMap( CCharacter& Boat, DWORD dwxPos, DWORD dwyPos, USHORT sDir );

	dbc::Char			m_szMotto[defMOTTO_LEN];
	dbc::uShort			m_usIcon;

    bool m_expFlag;
    DWORD m_ExpScale;       //  

	struct
	{
		short m_sPoseState; // 0.1
	};
	//add by jilinlee 2007/4/20
	struct SReadBook
	{
		bool bIsReadState;       //0,.1.
        DWORD dwLastReadCallTick; //Reading_Book.
	};
    
	SReadBook m_SReadBook;

	CAction		        m_CAction;
	SLean		        m_SLean;
	SSeat				m_SSeat;

	SCheatX				m_sCheatX;
	
	BYTE				_btBlockCnt;
	STempChaPart        m_STempChaPart;

	int chaIMP;
	CCharacter* mountCha;
	BOOL		IsChaOnMount;
	

	// 
	mission::CTradeData*	m_pTradeData;

	#define CHAEXIT_NONE				0		// ...
	#define CHAEXIT_BEGIN				1<<0	// 	

	BYTE				m_byExit;
	CTimer				m_timerExit;
    CTimer              m_timerAI;
    CTimer              m_timerAreaCheck;
	CTimer				m_timerDBUpdate;
	CTimer				m_timerDie;
	CTimer				m_timerMission;			// 
    CTimer				m_timerSkillState;		// 
	CTimer              m_timerTeam;			// Team
	struct
	{
		CTimer			m_timerPing;
		dbc::uLong		m_ulPingDataLen;
	};

	std::bitset<8>		m_chPKCtrl;

	dbc::Long			m_lSideID;				// 
	bool				m_bInOutMapQueue;
	struct // Ping
	{
		DWORD	m_dwPing;
		DWORD	m_dwPingRec[defPING_RECORD_NUM];
		DWORD	m_dwPingSendTick;
	};

	struct // for test net state
	{
		dbc::uLong	m_ulNetSendLen;
		CTimer		m_timerNetSendFreq;
	};

	dbc::Long	m_lScriptParam[defCHA_SCRIPT_PARAM_NUM];

protected:

    DWORD   _dwLastAreaTick;
    BYTE    _btLastAreaNo;

	DWORD	_dwLastSayTick;
	

public:
	void    ResetLifeTime(DWORD dwTime);
	BOOL    CheckLifeTime();
	DWORD   GetLifeTime();

	DWORD	_dwLifeTime;
	DWORD	_dwLifeTimeTick;

	bool    IsOfflineMode() const;
	DWORD	_dwStallTick;

	bool	appCheck[enumEQUIP_NUM];

	BYTE	requestType;
	Square	requestPos; // must check if player is in same position

	bool    IsReqPosEqualRealPos();
};

// 
extern Point		g_SSkillPoint;
extern bool			g_bBeatBack;
extern unsigned char	g_uchFightID;
extern char			g_chUseItemFailed[2];
extern char			g_chUseItemGiveMission[2];
//
extern bool IsPersistStateID(unsigned char uchStateID);
extern char* SStateData2String(CCharacter *pCCha, char *szSStateBuf, int nLen, char chSaveType);

/**
 * [p]ersist
 * Saves any player stats that were configured in GameServer[x].cfg
 * 07.27.2018
 */
extern bool		Strin2SStateData(CCharacter *pCCha, std::string &strData);
//Add by lark.li 20080723
extern char*	ChaExtendAttr2String(CCharacter *pCCha, char *szAttrBuf, int nLen);
extern bool		Strin2ChaExtendAttr(CCharacter *pCCha, std::string &strAttr);
 //
extern void TL(int nType, const char *pszCha1, const char *pszCha2, const char *pszTrade);

#endif // CHARACTER_H
