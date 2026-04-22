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

namespace mission {
	class CStallData;
}

enum ELoginChaType {
	enumLOGIN_CHA_MAIN,
	enumLOGIN_CHA_BOAT,
};

class GateServer;

#define MAX_TEAM_MEMBER 5
#define MAX_BANK_NUM	1
#define ACT_NAME_LEN	64
#define PLAYER_INVALID_FLAG			0xF0F0F0F0L


//NOTE(Ogge): This should describe a player as a entity
class CPlayer : public GatePlayer, public mission::CCharMission {
public:
	CPlayer();

	void         Init(GateServer* pGate, dbc::uLong ulGtAddr);
	bool         IsValidFlag();
	void         SetPassword(const char szPassword[]);
	const char*  GetPassword();

	void Free();
	void Initially();
	void Finally();

	void         SetID(dbc::Long lID);
	dbc::Long    GetID(void);

	void         SetHandle(dbc::Long lHandle);
	dbc::Long    GetHandle(void);

	bool         IsPlayer(void);

	virtual void OnLogoff();

	bool         IsValid(void);

	void         SetActLoginID(DWORD id);
	DWORD        GetActLoginID();

	void         SetDBActId(DWORD dwDBActId);
	DWORD        GetDBActId(void);

	void         SetActName(dbc::cChar* szActName);
	dbc::cChar*  GetActName(void);

	void         SetGMLev(dbc::Char chGMLev);
	dbc::uChar   GetGMLev(void);

	void         SetMapMaskDBID(long lID);
	long         GetMapMaskDBID(void);

	void         SetBankDBID(long lID, char chBankNO);
	long         GetBankDBID(char chBankNO);

	void SetIMP(long imp, bool sync = true);

	long         GetIMP();

	long m_lIMP;

	void         SetLoginCha(dbc::uLong ulType, dbc::uLong ulID);
	dbc::uLong   GetLoginChaType(void);
	dbc::uLong   GetLoginChaID(void);

	//mission::CCharMission& GetMission() { return *(mission::CCharMission*)this; }

	void AddTeamMember(uplayer*);
	void ClearTeamMember();

	bool         CanReceiveRequests();
	void         SetCanReceiveRequests(bool x);
	int          GetTeamMemberCnt();

	DWORD        GetTeamMemberDBID(int nNo);

	DWORD        getTeamLeaderID();
	CCharacter* GetTeamMemberCha(int nNo);
	void NoticeTeamMemberData();
	void NoticeTeamLeaderID(void);

	void         setTeamLeaderID(DWORD dwID);

	void LeaveTeam(void);
	void UpdateTeam(void);

	bool         IsTeamLeader(void);

	bool         HasTeam(void);

	void         BeginGetTeamPly(void);

	CPlayer* GetNextTeamPly(void);

	void         SetChallengeType(dbc::Char chType);
	dbc::Char    GetChallengeType(void);

	bool SetChallengeParam(dbc::Char chParamID, dbc::Long lParamVal);
	dbc::Long GetChallengeParam(dbc::Char chParamID);
	bool HasChallengeObj(void);
	void ClearChallengeObj(bool bAll = true);

	void         StartChallengeTimer(void);

	bool OpenRepair(CCharacter* pCNpc);

	CCharacter*  GetRepairman(void);

	bool SetRepairPosInfo(dbc::Char chPosType, dbc::Char chPosID);

	SItemGrid*   GetRepairItem(void);
	bool         CheckRepairItem(void);
	bool         IsRepairEquipPos(void);
	dbc::Char    GetRepairPosID(void);
	bool         IsInRepair(void);
	void         SetInRepair(bool bInR = true);

	bool OpenForge(CCharacter* pCNpc);

	bool OpenLottery(CCharacter* pCNpc); //Add by lark.li 20080514

	bool OpenUnite(CCharacter* pCNpc);
	bool OpenMilling(CCharacter* pCNpc);
	bool OpenFusion(CCharacter* pCNpc);
	bool OpenUpgrade(CCharacter* pCNpc);
	bool OpenEidolonMetempsychosis(CCharacter* pCNpc);
	bool OpenEidolonFusion(CCharacter* pCNpc);
	bool OpenPurify(CCharacter* pCNpc);
	bool OpenFix(CCharacter* pCNpc);
	bool OpenEnergy(CCharacter* pCNpc);
	bool OpenGetStone(CCharacter* pCNpc);
	bool OpenTiger(CCharacter* pCNpc);

	CCharacter*  GetForgeman(void);
	bool         IsInForge(void);
	bool         IsInLifeSkill(void);
	void         SetInForge(bool bInForge = true);
	void         SetInLifeSkill(bool bInLiftSkill = true);
	void         SetForgeInfo(dbc::Char chType, SForgeItem* pSItem);
	void         SetLifeSkillInfo(long lType, SLifeSkillItem* pLifeSkill);
	dbc::Char    GetForgeType(void);
	SForgeItem*  GetForgeItem(void);
	SLifeSkillItem* GetLifeSkillItem();

	void         SetMainCha(CCharacter* pMainCha);
	void         SetCtrlCha(CCharacter* pCtrlCha);
	CCharacter*  GetMainCha(void);
	CCharacter*  GetCtrlCha(void);

	CCharacter*  GetMakingBoat();
	void         SetMakingBoat(CCharacter* pBoat);

	BYTE         GetNumBoat();

	void GetBerthBoat(USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data);
	void GetAllBerthBoat(USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data);
	void GetDeadBerthBoat(USHORT sBerthID, BYTE& byNumBoat, BOAT_BERTH_DATA& Data);
	void SetBerth(USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir);
	void GetBerth(USHORT& sBerthID, USHORT& sxPos, USHORT& syPos, USHORT& sDir);
	BOOL HasAllBoatInBerth(USHORT sBerthID);
	BOOL HasBoatInBerth(USHORT sBerthID);
	BOOL HasDeadBoatInBerth(USHORT sBerthID);

	BOOL         IsBoatFull();
	BOOL         IsLuanchOut();
	void         SetLuanchOut(DWORD dwID);
	DWORD        GetLuanchID();

	CCharacter* GetLuanchOut();

	CCharacter* GetBoat(BYTE byIndex);

	CCharacter* GetBoat(DWORD dwBoatDBID);
	BYTE GetBoatIndexByDBID(DWORD dwBoatDBID);
	BOOL AddBoat(CCharacter& Boat);
	BOOL ClearBoat(DWORD dwBoatDBID);
	void RefreshBoatAttr(void);

	mission::CStallData* GetStallData();
	void                 SetStallData(mission::CStallData* pData);

	void         SetMMaskLightSize(long lSize);
	long         GetMMaskLightSize(void);

	bool         SetMapMaskBase64(const char* pMask);
	const char*  GetMapMaskBase64();
	BYTE*        GetMapMask(long& lLen);

	bool RefreshMapMask(const char* szMapName, long lPosX, long lPosY);

	void         SetMaskMapName(const char* szMapName);
	const char*  GetMaskMapName(void);

	bool         IsMapMaskChange(void);
	void         SetMapMaskChange(void);
	void         ResetMapMaskChange(void);
	float        GetMapMaskOpenScale(const char* szMapName);

	char         GetCurBankNum(void);
	bool         AddBankDBID(long lDBID);
	CKitbag*     GetBank(char chBankNO = 0);

	char* BankDBIDData2String(char* szSStateBuf, int nLen);
	bool Strin2BankDBIDData(std::string& strData);
	bool AddBank(void);
	bool SaveBank(char chBankNO = -1);
	bool SetBankChangeFlag(char chBankNO = -1, bool bChange = true);
	bool SynBank(char chBankNO = -1, char chType = enumSYN_KITBAG_INIT);
	bool SynGuildBank(CKitbag* bag, char chType);
	bool OpenBank(CCharacter* pCNpc);
	bool OpenGuildBank();
	bool GetGuildGold();
	bool BankCanOpen(CCharacter* pCNpc);
	void CloseBank(void);

	CCharacter*  GetBankNpc(void);

	bool SetBankSaveFlag(char chBankNO = -1, bool bChange = true);
	bool BankWillSave(char chBankNO = 0);
	bool BankHasItem(USHORT sItemID, USHORT& sCount);

	void SystemNotice(const char szData[], ...);
	void Run(DWORD dwCurTime);
	void CheckChaItemFinalData(void);
	bool String2BankData(char chBankNO, std::string& strData);

	long         GetMoBean();
	void         SetMoBean(long lMoBean);
	long         GetRplMoney();
	void         SetRplMoney(long lRplMoney);
	long         GetVipType();
	void         SetVipType(long lVipType);
	short        IsGarnerWiner();
	void         SetGarnerWiner(short siswiner);

	uplayer _Team[MAX_TEAM_MEMBER];

	std::string& GetLifeSkillinfo();

	dbc::Char m_szGuildName[defGUILD_NAME_LEN]; //ID0.
	dbc::Char m_szGuildMotto[defGUILD_MOTTO_LEN];
	dbc::Char m_szStallName[ROLE_MAXNUM_STALL_NUM];
	bool m_bIsGuildLeader; //0-;1-
	struct {
		uint32_t m_GuildState{}; //,:enum EGuildState

		uLong m_GuildStatus; //
		uLong m_lGuildID; //ID.
		uLong m_lTempGuildID; //
		dbc::Char m_szTempGuildName[defGUILD_NAME_LEN]; //.
	};

protected:
	int _nTeamMemberCnt;
	DWORD _dwTeamLeaderID;

private:
	dbc::Long m_lID;
	dbc::Long m_lHandle;

	bool bReceiveRequests{true};
	bool bIsValid;
	char m_szPassword[ROLE_MAXSIZE_PASSWORD2 + 1];

	long m_lMoBean; //
	long m_lRplMoney; //
	long m_lVipID; //VIP

	DWORD m_dwLoginID; //  Account DB ID
	DWORD m_dwDBActId; // ID
	dbc::Char m_chGMLev; // GM
	long m_lMapMaskDBID;
	dbc::Char m_chActName[ACT_NAME_LEN]; //

	CCharacter* m_pCtrlCha; //

	CCharacter* m_pMainCha; //
	DWORD m_dwLaunchID; // ID
	BYTE m_byNumBoat; //
	CCharacter* m_Boat[MAX_CHAR_BOAT];

	dbc::uLong m_ulLoginCha[2]; // ID.0 1

	//
	struct {
		USHORT m_sBerthID; //
		USHORT m_sxPos;
		USHORT m_syPos;
		USHORT m_sDir;
	};

	//
	struct {
		long m_lLightSize;
		CMaskData m_CMapMask;
		char m_szMaskMapName[MAX_MAPNAME_LENGTH];
		char m_chMapMaskChange;
	};

	//
	struct {
		CCharacter* m_pCBankNpc;
		char m_chBankNum;
		long m_lBankDBID[MAX_BANK_NUM];
		CKitbag m_CBank[MAX_BANK_NUM];
		bool m_bBankChange[MAX_BANK_NUM];
	};

	//
	CCharacter* m_pMakingBoat;

	//
	mission::CStallData* m_pStallData;

	//
	struct {
		dbc::Char m_chChallengeType; //
		dbc::Long m_lChallengeParam[2 + MAX_TEAM_MEMBER * 2 * 2]; //
		CTimer m_timerChallenge; //
	};

	//
	struct {
		bool m_bInRepair;
		dbc::Char m_chRepairPosType;
		dbc::Char m_chRepairPosID;
		SItemGrid m_SRepairItem;
		SItemGrid* m_pSRepairItem;
		CCharacter* m_pCRepairman;
	};

	// /
	struct {
		bool m_bInForge;
		dbc::Char m_chForgeType;
		SForgeItem m_SForgeItem;
		CCharacter* m_pCForgeman;
	};

	//
	struct {
		CCharacter* m_pCLotteryman;
	};

	//Add by sunny.sun20080716
	struct {
		CCharacter* m_pCAmphitheaterman;
	};

	struct {
		bool m_bInLiftSkill;
		dbc::Long m_lLifeSkillType;
		SLifeSkillItem m_pSLifeSkillItem;
		std::string m_strLifeSkillinfo;
	};

	dbc::Short m_sGetTeamPlyCount;

	DWORD m_dwValidFlag;
	short m_sGarnerWiner;
};

extern CPlayer* CreatePlayer(GateServer* pGate, dbc::uLong ulGateAddr, dbc::uLong ulChaDBId);
extern void ReleasePlayer(CPlayer* pPlayer);

#endif // PLAYER_H
