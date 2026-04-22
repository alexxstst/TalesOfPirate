//=============================================================================
// FileName: CommFunc.h
// Creater: ZhangXuedong
// Date: 2005.01.06
// Comment: 
//	2005.4.28	Arcol	add the text filter manager class: CTextFilter
//=============================================================================

#ifndef COMMFUNC_H
#define COMMFUNC_H

#include "CompCommand.h"
#include "SkillRecord.h"
#include "CharacterRecord.h"
#include "ItemRecord.h"
#include "ItemAttrType.h"
#include "JobType.h"
#include "NetRetCode.h"
#include <regex>
#include <string_view>
#include <algorithm>
#include <bitset>
#include "i18n.h"


extern bool KitbagStringConv(short sKbCapacity, std::string& strData);

//=============================================================================
/*---------------------------------------------------------------
* :
* nPart - ID,nValue -
*
*/
extern bool g_IsValidLook(int nType, int nPart, long nValue);

/*---------------------------------------------------------------
* ulAreaMask
* true false
*/
inline bool g_IsSea(unsigned short usAreaMask) {
	return !(usAreaMask & enumAREA_TYPE_LAND);
}

inline bool g_IsLand(unsigned short usAreaMask) {
	return (usAreaMask & enumAREA_TYPE_LAND) || (usAreaMask & enumAREA_TYPE_BRIDGE);
}

// ID
// ,-1,
extern int g_GetItemSkill(int nLeftItemID, int nRightItemID);

extern BOOL IsDist(int x1, int y1, int x2, int y2, DWORD dwDist);

//
extern int g_IsRightSkillTar(int nTChaCtrlType, bool bTIsDie, bool bTChaBeSkilled, int nTChaArea,
							 int nSChaCtrlType, int nSSkillObjType, int nSSkillObjHabitat, int nSSkillEffType,
							 bool bIsTeammate, bool bIsFriend, bool bIsSelf);

/*---------------------------------------------------------------
* :ID
* :1-,0-,-1-
*/
extern int g_IsUseSkill(stNetChangeChaPart* pSEquip, int nSkillID);
extern bool g_IsRealItemID(int nItemID);

inline int g_IsUseSkill(stNetChangeChaPart* pSEquip, CSkillRecord* p) {
	if (!p) return -1;

	return g_IsUseSkill(pSEquip, p->Id);
}

inline int g_IsUseSeaLiveSkill(long lFitNo, CSkillRecord* p) {
	if (!p) return -1;

	for (int i = 0; i < defSKILL_ITEM_NEED_NUM; i++) {
		if (p->sItemNeed[0][i][0] == cchSkillRecordKeyValue)
			break;

		if (p->sItemNeed[0][i][0] == enumSKILL_ITEM_NEED_ID) {
			if (p->sItemNeed[0][i][1] == lFitNo)
				return 1;
		}
	}

	return 0;
}

inline bool g_IsPlyCtrlCha(int nChaCtrlType) {
	if (nChaCtrlType == enumCHACTRL_PLAYER || nChaCtrlType == enumCHACTRL_PLAYER_PET)
		return true;
	return false;
}

inline bool g_IsMonsCtrlCha(int nChaCtrlType) {
	if (nChaCtrlType == enumCHACTRL_MONS
		|| nChaCtrlType == enumCHACTRL_MONS_TREE
		|| nChaCtrlType == enumCHACTRL_MONS_MINE
		|| nChaCtrlType == enumCHACTRL_MONS_FISH
		|| nChaCtrlType == enumCHACTRL_MONS_DBOAT
		|| nChaCtrlType == enumCHACTRL_MONS_REPAIRABLE
	)
		return true;
	return false;
}

inline bool g_IsNPCCtrlCha(int nChaCtrlType) {
	if (nChaCtrlType == enumCHACTRL_NPC || nChaCtrlType == enumCHACTRL_NPC_EVENT)
		return true;
	return false;
}

inline bool g_IsChaEnemyCtrlSide(int nSCtrlType, int nTCtrlType) {
	if (g_IsPlyCtrlCha(nSCtrlType) && g_IsPlyCtrlCha(nTCtrlType))
		return false;
	if (g_IsMonsCtrlCha(nSCtrlType) && g_IsMonsCtrlCha(nTCtrlType))
		return false;
	return true;
}

inline bool g_IsFileExist(const char* szFileName) {
	FILE* fp = NULL;
	if (NULL == (fp = fopen(szFileName, "rb")))
		return false;
	if (fp) fclose(fp);
	return true;
}

extern void String2Item(const char* pszData, SItemGrid* SGridCont);
extern bool LookData2String(const stNetChangeChaPart& pLook, std::string& strData);
extern bool String2LookData(stNetChangeChaPart& pLook, const std::string& strData);
extern char* ShortcutData2String(const stNetShortCut* pShortcut, char* szShortcutBuf, int nLen);
extern bool String2ShortcutData(stNetShortCut* pShortcut, std::string& strData);


inline long g_ConvItemAttrTypeToCha(long lItemAttrType) {
	if (lItemAttrType >= ITEMATTR_COE_STR && lItemAttrType <= ITEMATTR_COE_PDEF)
		return lItemAttrType + (ATTR_ITEMC_STR - ITEMATTR_COE_STR);
	else if (lItemAttrType >= ITEMATTR_VAL_STR && lItemAttrType <= ITEMATTR_VAL_PDEF)
		return lItemAttrType + (ATTR_ITEMV_STR - ITEMATTR_VAL_STR);
	else
		return 0;
}

//
inline short g_GetRangeParamNum(char RangeType) {
	short sParamNum = 0;
	switch (RangeType) {
	case enumRANGE_TYPE_STICK:
		sParamNum = 2;
		break;
	case enumRANGE_TYPE_FAN:
		sParamNum = 2;
		break;
	case enumRANGE_TYPE_SQUARE:
		sParamNum = 1;
		break;
	case enumRANGE_TYPE_CIRCLE:
		sParamNum = 1;
		break;
	}

	return sParamNum + 1;
}

//=============================================================================
// chChaType
// chChaTerrType
// bIsBlock
// ulAreaMask
// true false
//=============================================================================
inline bool g_IsMoveAble(char chChaCtrlType, char chChaTerrType, unsigned short usAreaMask) {
	bool bRet1 = false;
	if (chChaTerrType == defCHA_TERRITORY_DISCRETIONAL)
		bRet1 = true;
	else if (chChaTerrType == defCHA_TERRITORY_LAND) {
		if (usAreaMask & enumAREA_TYPE_LAND || usAreaMask & enumAREA_TYPE_BRIDGE)
			bRet1 = true;
	}
	else if (chChaTerrType == defCHA_TERRITORY_SEA) {
		if (!(usAreaMask & enumAREA_TYPE_LAND))
			bRet1 = true;
	}

	bool bRet2 = true;
	if (usAreaMask & enumAREA_TYPE_NOT_FIGHT) //
	{
		if (g_IsMonsCtrlCha(chChaCtrlType))
			bRet2 = false;
	}

	return bRet1 && bRet2;
}

inline const char* g_GetJobName(short sJobID) {
	if (sJobID < 0 || sJobID >= MAX_JOB_TYPE)
		return g_szJobName[0];

	return g_szJobName[sJobID];
}

inline short g_GetJobID(const char* szJobName) {
	for (short i = 0; i < MAX_JOB_TYPE; i++) {
		if (!strcmp(g_szJobName[i], szJobName))
			return i;
	}

	return 0;
}

inline const char* g_GetCityName(short sCityID) {
	if (sCityID < 0 || sCityID >= defMAX_CITY_NUM)
		return "";

	return g_szCityName[sCityID];
}

inline short g_GetCityID(const char* szCityName) {
	for (short i = 0; i < defMAX_CITY_NUM; i++) {
		if (!strcmp(g_szCityName[i], szCityName))
			return i;
	}

	return -1;
}

inline bool g_IsSeatPose(int pose) {
	return 16 == pose;
}

//
inline bool g_IsValidFightState(int nState) {
	return nState < enumFSTATE_TARGET_NO;
}

inline bool g_ExistStateIsDie(char chState) {
	if (chState >= enumEXISTS_WITHERING)
		return true;

	return false;
}

inline const char* g_GetItemAttrExplain(int v) {
	switch (v) {
	case ITEMATTR_COE_STR: return "Strength Bonus"; // "";
	case ITEMATTR_COE_AGI: return "Agility Bonus"; // "";
	case ITEMATTR_COE_DEX: return "Accuracy Bonus"; // "";
	case ITEMATTR_COE_CON: return "Constitution Bonus"; // "";
	case ITEMATTR_COE_STA: return "Spirit Bonus"; // "";
	case ITEMATTR_COE_LUK: return "Luck Bonus"; // "";
	case ITEMATTR_COE_ASPD: return "Attack Speed Bonus"; // "";
	case ITEMATTR_COE_ADIS: return "Attack Range Bonus"; // "";
	case ITEMATTR_COE_MNATK: return "Minimum Attack Bonus"; // "";
	case ITEMATTR_COE_MXATK: return "Maximum Attack Bonus"; // "";
	case ITEMATTR_COE_DEF: return "Defense Bonus"; // "";
	case ITEMATTR_COE_MXHP: return "Maximum HP Bonus"; // "HP";
	case ITEMATTR_COE_MXSP: return "Maximum SP Bonus"; // "SP";
	case ITEMATTR_COE_FLEE: return "Dodge Rate Bonus"; // "";
	case ITEMATTR_COE_HIT: return "Hit Rate Bonus"; // "";
	case ITEMATTR_COE_CRT: return "Critical Hitrate Bonus"; // "";
	case ITEMATTR_COE_MF: return "Drop Rate Bonus"; // "";
	case ITEMATTR_COE_HREC: return "HP Recovery Speed Bonus"; // "HP";
	case ITEMATTR_COE_SREC: return "SP Recovery Speed Bonus"; // "SP";
	case ITEMATTR_COE_MSPD: return "Movement Speed Bonus"; // "";
	case ITEMATTR_COE_COL: return "Material Mining Speed Bonus"; // "";

	case ITEMATTR_VAL_STR: return "Strength Bonus"; // "";
	case ITEMATTR_VAL_AGI: return "Agility Bonus"; // "";
	case ITEMATTR_VAL_DEX: return "Accuracy Bonus"; // "";
	case ITEMATTR_VAL_CON: return "Constitution Bonus"; // "";
	case ITEMATTR_VAL_STA: return "Spirit Bonus"; // "";
	case ITEMATTR_VAL_LUK: return "Luck Bonus"; // "";
	case ITEMATTR_VAL_ASPD: return "Attack Speed Bonus"; // "";
	case ITEMATTR_VAL_ADIS: return "Attack Range Bonus"; // "";
	case ITEMATTR_VAL_MNATK: return "Minimum Attack Bonus"; // "";
	case ITEMATTR_VAL_MXATK: return "Maximum Attack Bonus"; // "";
	case ITEMATTR_VAL_DEF: return "Defense Bonus"; // "";
	case ITEMATTR_VAL_MXHP: return "Maximum HP Bonus"; // "HP";
	case ITEMATTR_VAL_MXSP: return "Maximum SP Bonus"; // "SP";
	case ITEMATTR_VAL_FLEE: return "Dodge Rate Bonus"; // "";
	case ITEMATTR_VAL_HIT: return "Hit Rate Bonus"; // "";
	case ITEMATTR_VAL_CRT: return "Critical Hitrate Bonus"; // "";
	case ITEMATTR_VAL_MF: return "Drop Rate Bonus"; // "";
	case ITEMATTR_VAL_HREC: return "HP Recovery Speed Bonus"; // "HP";
	case ITEMATTR_VAL_SREC: return "SP Recovery Speed Bonus"; // "SP";
	case ITEMATTR_VAL_MSPD: return "Movement Speed Bonus"; // "";
	case ITEMATTR_VAL_COL: return "Material Mining Speed Bonus"; // "";

	case ITEMATTR_VAL_PDEF: return "Physical Resist Bonus"; // "";
	case ITEMATTR_MAXURE: return "Max Durability"; // "";
	case ITEMATTR_MAXENERGY: return "Max Energy"; // "";
	default: return "Unknown tools characteristics"; // "";
	}
}

inline const char* g_GetServerError(int error_code) //
{
	switch (error_code) {
	case ERR_AP_INVALIDUSER: return "Invalid Account"; // "";
	case ERR_AP_INVALIDPWD: return "Password incorrect"; // "";
	case ERR_AP_ACTIVEUSER: return "Account activation failed"; // "";
	case ERR_AP_DISABLELOGIN: return "Your cha is currently in logout save mode, please try logging in again later.";
	// "";
	case ERR_AP_LOGGED: return "This account is already online"; // "";
	case ERR_AP_BANUSER: return "Account has been banned"; // "";
	case ERR_AP_GPSLOGGED: return "This GroupServer has login"; // "GroupServer";
	case ERR_AP_GPSAUTHFAIL: return "This GroupServer Verification failed"; // "GroupServer";
	case ERR_AP_SAVING: return "Saving your character, please try again in 15 seconds..."; // "15...";
	case ERR_AP_LOGINTWICE: return "Your account is logged on far away"; // "";
	case ERR_AP_ONLINE: return "Your account is already online"; // "";
	case ERR_AP_DISCONN: return "GroupServer disconnected"; // "GroupServer";
	case ERR_AP_UNKNOWNCMD: return "unknown agreement, don\'t deal with"; // "";
	case ERR_AP_TLSWRONG: return "local saving error"; // "";
	case ERR_AP_NOBILL: return "This account has expired, please topup!"; // "";


	case ERR_PT_LOGFAIL: return "GateServer to GroupServer login failed"; // "GateServerGroupServer";
	case ERR_PT_SAMEGATENAME: return "GateServer and login GateServer have similar name"; // "GateServerGateServer";

	case ERR_PT_INVALIDDAT: return "Ineffective data model"; // "";
	case ERR_PT_INERR: return "server link operation integrality error "; // "";
	case ERR_PT_NETEXCP: return "Account server has encountered a malfunction"; // "";	// GroupServerAccuntServer
	case ERR_PT_DBEXCP: return "database server malfunction"; // "";	// GroupServerDatabase
	case ERR_PT_INVALIDCHA: return "Current account does not have a request (Select/Delete) to character"; // "(/)";
	case ERR_PT_TOMAXCHA: return "reached the maximum number of characters you can create"; // "";
	case ERR_PT_BONUSCHARS: return "You don't have the necessary levels to create another character";
	case ERR_PT_SAMECHANAME: return "Character name already exist"; // "";
	case ERR_PT_INVALIDBIRTH: return "illegal birth place"; // "";
	case ERR_PT_TOOBIGCHANM: return "Character name is too long"; // "";
	case ERR_PT_ISGLDLEADER: return
			"Guild must have a leader, please disband your guild first then delete your character"; // "";
	case ERR_PT_ERRCHANAME: return "Illegal character name"; // "";
	case ERR_PT_SERVERBUSY: return "System is busy, please try again later"; // "";
	case ERR_PT_TOOBIGPW2: return "second code length illegal"; // "";
	case ERR_PT_INVALID_PW2: return "Cha second password not created"; // "";
	case ERR_PT_BADBOY: return
			"My child, you are very bold. You have been reported to the authority. Please do not commit the offense again!";
	// "BT";
	case ERR_PT_BANUSER: return RES_STRING(CO_COMMFUNC_H_00031); // "";
	case ERR_PT_PBANUSER: return RES_STRING(CO_COMMFUNC_H_00108); // "";
	case ERR_MC_NETEXCP: return "Discovered exceptional line error on GateServer"; // "GateServer";
	case ERR_MC_NOTSELCHA: return "current not yet handled character state"; // "";
	case ERR_MC_NOTPLAY: return "Currently not in gameplay, unable to send ENDPLAY command"; // "ENDPLAY";
	case ERR_MC_NOTARRIVE: return "target map cannot be reached"; // "";
	case ERR_MC_TOOMANYPLY: return "This server is currently full, please select another server!"; // ", !";
	case ERR_MC_NOTLOGIN: return "Youa re not login"; // "";
	case ERR_MC_VER_ERROR: return "Client version error, server refused connection!"; // ",";
	case ERR_MC_ENTER_ERROR: return "failed to enter map!"; // "";
	case ERR_MC_ENTER_POS: return "Map position illegal, you\'ll be sent back to your birth city, please relog!"; // "";
	case ERR_MC_BANUSER: return RES_STRING(CO_COMMFUNC_H_00031); // "";
	case ERR_MC_PBANUSER: return RES_STRING(CO_COMMFUNC_H_00108); // "";
	case ERR_TM_OVERNAME: return "GameServer name repeated"; // "GameServer";
	case ERR_TM_OVERMAP: return "GameServerMapNameRepeated"; // "GameServer";
	case ERR_TM_MAPERR: return "GameServer map assign language error"; // "GameServer";

	case ERR_SUCCESS: return "Jack is too BT, correct also will ask me if anything is wrong!"; // "JackBT";
	default: {
		int l_error_code = error_code;
		l_error_code /= 500;
		l_error_code *= 500;
		static char l_buffer[500];
		char l_convt[20];
		switch (l_error_code) {
		case ERR_MC_BASE: return strcat(strcpy(l_buffer, itoa(error_code, l_convt, 10)),
										"(GameServer/GateServer->Client return error code space 1-500)");
		//"(GameServer/GateServer->Client1500)");
		case ERR_PT_BASE: return strcat(strcpy(l_buffer, itoa(error_code, l_convt, 10)),
										"(GroupServer->GateServer return error code range 501-1000)");
		//"(GroupServer->GateServer5011000)");
		case ERR_AP_BASE: return strcat(strcpy(l_buffer, itoa(error_code, l_convt, 10)),
										"(AccountServer->GroupServe return error code from 1001-1500)");
		//"(AccountServer->GroupServer10011500)");
		case ERR_MT_BASE: return strcat(strcpy(l_buffer, itoa(error_code, l_convt, 10)),
										"(GameServer->GateServer return error code range 1501-2000)");
		//"(GameServer->GateServer15012000)");
		default: return strcat(strcpy(l_buffer, itoa(error_code, l_convt, 10)),
							   "(Jack is too insane, he made a mistake that I don\'t even know.)"); //"(JackBT)");
		}
	}
	}
}


inline bool isAlphanumeric(std::string_view text) {
	return std::all_of(text.begin(), text.end(), ::isalnum);
}

inline bool isNumeric(std::string_view text) {
	return std::all_of(text.begin(), text.end(), ::isdigit);
}

inline bool isEmail(const char* email) {
	const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	//const std::regex pattern("([\\w\\.\\_\\-] + )@([\\w\\.\\_\\-] + )(\\.(\\w + )) +");
	return std::regex_match(email, pattern);
}


//GBK
//nametrue;
//lenname=strlen(name),NULL
inline bool IsValidName(const std::string& name) {
	bool l_ishan = false;
	//if (len == 0)
	//	return 0;
	for (unsigned short i = 0; i < name.size(); i++) {
		if (!name[i]) {
			return false;
		}
		else if (l_ishan) {
			if (name[i - 1] == 0xA1 && name[i] == 0xA1) //
			{
				return false;
			}
			if (name[i] > 0x3F && name[i] < 0xFF && name[i] != 0x7F) {
				l_ishan = false;
			}
			else {
				return false;
			}
		}
		else if (name[i] > 0x80 && name[i] < 0xFF) {
			l_ishan = true;
		}
		else if ((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z') || (name[i] >= '0'
			&& name[i] <= '9')) {
		}
		else {
			return false;
		}
	}
	return !l_ishan;
}

inline const char* g_GetUseItemFailedInfo(short sErrorID) {
	switch (sErrorID) {
	case enumITEMOPT_SUCCESS:
		return "Item operation succesful"; // "";
		break;
	case enumITEMOPT_ERROR_NONE:
		return "Equipment does not exist"; // "";
		break;
	case enumITEMOPT_ERROR_KBFULL:
		return "Inventory is full"; // "";
		break;
	case enumITEMOPT_ERROR_UNUSE:
		return "Failed to use item"; // "";
		break;
	case enumITEMOPT_ERROR_UNPICKUP:
		return "Rl??EC?"; // "";
		break;
	case enumITEMOPT_ERROR_UNTHROW:
		return "Item cannot be thrown"; // "";
		break;
	case enumITEMOPT_ERROR_UNDEL:
		return "Item cannot be destroyed"; // "";
		break;
	case enumITEMOPT_ERROR_KBLOCK:
		return "inventory is currently locked"; // "";
		break;
	case enumITEMOPT_ERROR_DISTANCE:
		return "Distance too far"; // "";
		break;
	case enumITEMOPT_ERROR_EQUIPLV:
		return "Equipment level mismatch"; // "";
		break;
	case enumITEMOPT_ERROR_EQUIPJOB:
		return "Does not meet the class requirement for the equipment"; // "";
		break;
	case enumITEMOPT_ERROR_STATE:
		return "Unable to operate items under the current condition"; // "";
		break;
	case enumITEMOPT_ERROR_PROTECT:
		return "Item is being protected"; // "";
		break;
	case enumITEMOPT_ERROR_AREA:
		return "different region type"; // "";
		break;
	case enumITEMOPT_ERROR_BODY:
		return "type of build does not match"; // "";
		break;
	case enumITEMOPT_ERROR_TYPE:
		return "Unable to store this item"; // "";
		break;
	case enumITEMOPT_ERROR_INVALID:
		return "Item not in used"; // "";
		break;
	case enumITEMOPT_ERROR_KBRANGE:
		return "out of inventory range"; // "";
		break;
	case enumITEMOPT_ERROR_EXPIRATION:
		return "This item is expired.";
		break;
	default:
		return "Unknown item usage failure code"; // "";
		break;
	}
}

class CTextFilter {
public:
	static constexpr int eTableMax = 5;
	enum eFilterTable { NAME_TABLE = 0, DIALOG_TABLE = 1, MAX_TABLE = eTableMax };

	/*
	 * Warning : не использовать MAX_TABLE — это sentinel-значение. Чтобы расширить
	 * enum выше eTableMax(5), увеличить константу eTableMax.
	 */

	CTextFilter();
	~CTextFilter() = default;

	static bool  Add(eFilterTable eTable, std::string_view filterText);
	static bool  IsLegalText(eFilterTable eTable, std::string_view text);
	static bool  Filter(eFilterTable eTable, std::string& text);
	static bool  LoadFile(std::string_view fileName, eFilterTable eTable = NAME_TABLE);
	static const BYTE* GetNowSign(eFilterTable eTable);

private:
	static bool  ReplaceText(std::string& text, std::string_view filterText);
	static bool  CheckLegalText(std::string_view text, std::string_view illegalText);

	static std::vector<std::string> m_FilterTable[eTableMax];
	static BYTE m_NowSign[eTableMax][8];
};

#endif // COMMFUNC_H
