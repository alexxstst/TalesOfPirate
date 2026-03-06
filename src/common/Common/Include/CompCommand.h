//=============================================================================
// FileName: CompCommand.h
// Creater: ZhangXuedong
// Date: 2004.11.22
// Comment: compositive command
//=============================================================================

#pragma once

#include "util.h"
#include "ItemAttrType.h"
#include "ChaAttrType.h"
#include "ItemContent.h"
#include <algorithm>

#define defPROTOCOL_HAVE_PACKETID // ID

enum EActionType // server,client 
{
	enumACTION_NONE = 0,
	enumACTION_MOVE,		// 
	enumACTION_SKILL,		// 
	enumACTION_SKILL_SRC,	// 
	enumACTION_SKILL_TAR,	// 
	enumACTION_LOOK,		// 
	enumACTION_KITBAG,		// 
	enumACTION_SKILLBAG,	// 
	enumACTION_ITEM_PICK,	// 
	enumACTION_ITEM_THROW,	// 
	enumACTION_ITEM_UNFIX,	// 
	enumACTION_ITEM_USE,	// 
	enumACTION_ITEM_POS,	// 
	enumACTION_ITEM_DELETE,	// 
	enumACTION_ITEM_INFO,	// 
	enumACTION_ITEM_FAILED,	// 
	enumACTION_LEAN,		// 
	enumACTION_CHANGE_CHA,	// 
	enumACTION_EVENT,		// 
    enumACTION_FACE,        // ,
	enumACTION_STOP_STATE,	// 
	enumACTION_SKILL_POSE,	// Pose
	enumACTION_PK_CTRL,		// PK
	enumACTION_LOOK_ENERGY,	// 

	enumACTION_TEMP,		// 

    enumACTION_SHORTCUT,    // ,:,
	enumACTION_BANK,		// 
	enumACTION_CLOSE_BANK,	// 

	enumACTION_KITBAGTMP,		//
	enumACTION_KITBAGTMP_DRAG,	//
	
	enumACTION_GUILDBANK,
	enumACTION_REQUESTGUILDBANK,
	enumACTION_REQUESTGUILDLOGS,
	enumACTION_UPDATEGUILDLOGS,

	enumMAX_ACTION_NUM		// 
};

enum EAttrSynType // server->client 
{
	enumATTRSYN_INIT,			// 
	enumATTRSYN_ITEM_EQUIP,		// ,
	enumATTRSYN_ITEM_MEDICINE,	// ,
    enumATTRSYN_ATTACK,			// ,,
	enumATTRSYN_TASK,			// 
	enumATTRSYN_TRADE,			// 
	enumATTRSYN_REASSIGN,		// 
	enumATTRSYN_SKILL_STATE,	// 
	enumATTRSYN_AUTO_RESUME,	// HP,SP
	enumATTRSYN_CHANGE_JOB,		// 
	enumATTRSYN_RECTIFY,		// 
};

enum EItemAppearType // server->client 
{
	enumITEM_APPE_MONS,		// 
	enumITEM_APPE_THROW,	// 
	enumITEM_APPE_NATURAL,	// 
};

enum ESynKitbagType // server->client 
{
	enumSYN_KITBAG_INIT,	// 
	enumSYN_KITBAG_EQUIP,	// 
	enumSYN_KITBAG_UNFIX,	// 
	enumSYN_KITBAG_PICK,	// 
	enumSYN_KITBAG_THROW,	// 
	enumSYN_KITBAG_SWITCH,	// 
	enumSYN_KITBAG_TRADE,	// 
	enumSYN_KITBAG_FROM_NPC,// NPC
	enumSYN_KITBAG_TO_NPC,	// NPC
	enumSYN_KITBAG_SYSTEM,	// 
	enumSYN_KITBAG_FORGES,	// 
	enumSYN_KITBAG_FORGEF,	// 
	enumSYN_KITBAG_BANK,	// 
	enumSYN_KITBAG_ATTR,	// 
};

enum ESynSkillBagType // server->client 
{
	enumSYN_SKILLBAG_INIT,	// 
	enumSYN_SKILLBAG_ADD,	// 
	enumSYN_SKILLBAG_MODI,	// 
};

enum ESynLookType
{
	enumSYN_LOOK_SWITCH,	// 
	enumSYN_LOOK_CHANGE,	// 
};

enum EEquipLinkPos // 
{
	enumEQUIP_HEAD		= 0,	// ,,,,,
	enumEQUIP_FACE		= 1,
	enumEQUIP_BODY		= 2,
	enumEQUIP_GLOVE		= 3,	// 
	enumEQUIP_SHOES		= 4,	// 

	enumEQUIP_NECK		= 5,	// :,
	enumEQUIP_LHAND		= 6,	// 		-- Link
	enumEQUIP_HAND1		= 7,	// 
	enumEQUIP_HAND2		= 8,
	enumEQUIP_RHAND		= 9,	// 
	enumEQUIP_Jewelry1	= 10,
	enumEQUIP_Jewelry2	= 11,
	enumEQUIP_Jewelry3	= 12,
	enumEQUIP_Jewelry4	= 13,
	enumEQUIP_WING		= 14,
	
	enumEQUIP_CLOAK		= 15,
	enumEQUIP_FAIRY		= 16,
	enumEQUIP_REAR		= 17,
	enumEQUIP_MOUNT		= 18,

	enumEQUIP_HEADAPP = 19,
	enumEQUIP_FACEAPP = 20,
	enumEQUIP_BODYAPP = 21,
	enumEQUIP_GLOVEAPP = 22,
	enumEQUIP_SHOESAPP = 23,
	
	enumEQUIP_FAIRYAPP = 24,
	enumEQUIP_GLOWAPP = 25,
	
	enumEQUIP_DAGGERAPP = 26,
	enumEQUIP_GUNAPP = 27,
	enumEQUIP_SWORD1APP = 28,
	enumEQUIP_GREATSWORDAPP = 29,
	enumEQUIP_STAFFAPP = 30,
	enumEQUIP_BOWAPP = 31,
	enumEQUIP_SWORD2APP = 32,
	enumEQUIP_SHIELDAPP = 33,
	
	
	enumEQUIP_PART_NUM  = 5,	// 
	enumEQUIP_NUM		= 34,	// 
    enumEQUIP_BOTH_HAND = 9999,  // ,
    enumEQUIP_TOTEM		= 9998,  // ,
};

enum EBoatLinkPos	// 
{
	enumBOAT_BODY		= 0,	// 
	enumBOAT_HEADER		= 1,	// 	
	enumBOAT_ENGINE		= 2,	// 
	enumBOAT_FLAG		= 3,	// 
	enumBOAT_MOTOR0		= 4,	// 4
	enumBOAT_MOTOR1		= 5,
	enumBOAT_MOTOR2		= 6,
	enumBOAT_MOTOR3		= 7,
};

enum EMoveState
{
	enumMSTATE_ON		= 0x00,	// 
	enumMSTATE_ARRIVE	= 0x01,	// 
	enumMSTATE_BLOCK	= 0x02,	// 
	enumMSTATE_CANCEL	= 0x04,	// 
	enumMSTATE_INRANGE	= 0x08, // 
	enumMSTATE_NOTARGET	= 0x10, // 
	enumMSTATE_CANTMOVE	= 0x20,	// 
};

enum EFightState
{// enumFSTATE_TARGET_NO
	enumFSTATE_ON			= 0x0000,	// 
	enumFSTATE_STOP			= 0x0001,	// 

	enumFSTATE_TARGET_NO	= 0x0010,	// 
	enumFSTATE_TARGET_OUT	= 0x0020,	// 
	enumFSTATE_TARGET_IMMUNE= 0x0040,	// 
	enumFSTATE_CANCEL		= 0x0080,	// 
	enumFSTATE_DIE			= 0x0100,	// 
    enumFSTATE_TARGET_DIE   = 0x0200,	// 
	enumFSTATE_OFF			= 0x0400,	// 
	enumFSTATE_NO_EXPEND	= 0x0800,	// MP
};

enum EExistState
{
	// 
	enumEXISTS_NATALITY,	// 
	enumEXISTS_WAITING,		// 
	enumEXISTS_SLEEPING,	// 
	enumEXISTS_MOVING,		// 
	enumEXISTS_FIGHTING,	// 
	//
	// 
	enumEXISTS_WITHERING,	// 
	enumEXISTS_RESUMEING,	// 
	enumEXISTS_DIE,			// 
	//
};

enum ESkillStateAdd
{
	enumSSTATE_ADD_UNDEFINED		= 0,	// 
	enumSSTATE_ADD_EQUALORLARGER	= 1,	// 
	enumSSTATE_ADD_LARGER			= 2,	// 
	enumSSTATE_NOTADD				= 3,	// 
	enumSSTATE_ADD					= 4,	// 
};

enum ERangeType
{
	enumRANGE_TYPE_NONE				= 0,	// 
	enumRANGE_TYPE_STICK			= 1,	// 
	enumRANGE_TYPE_FAN				= 2,	// 
	enumRANGE_TYPE_SQUARE			= 3,	// 
	enumRANGE_TYPE_CIRCLE			= 4,	// 
};

// 
enum EFailedActionReason
{
	enumFACTION_ACTFORBID,		// 
	enumFACTION_EXISTACT,		// 
	enumFACTION_MOVEPATH,		// 
	enumFACTION_CANTMOVE,		// 
	enumFACTION_NOSKILL,		// 
	enumFACTION_NOOBJECT,		// 
	enumFACTION_ITEM_INEXIST,	// 
	enumFACTION_SKILL_NOTMATCH,	// 
};

enum ESkillUseState
{
	enumSUSTATE_INACTIVE,		// 
	enumSUSTATE_ACTIVE,			// 
};

enum EItemOperateResult
{
	enumITEMOPT_SUCCESS,		// 
	enumITEMOPT_ERROR_NONE,		// 
	enumITEMOPT_ERROR_KBFULL,	// 
	enumITEMOPT_ERROR_UNUSE,	// 
	enumITEMOPT_ERROR_UNPICKUP,	// 
	enumITEMOPT_ERROR_UNTHROW,	// 
	enumITEMOPT_ERROR_UNDEL,	// 
	enumITEMOPT_ERROR_KBLOCK,	// 
	enumITEMOPT_ERROR_DISTANCE,	// 
	enumITEMOPT_ERROR_EQUIPLV,	// 
	enumITEMOPT_ERROR_EQUIPJOB,	// 
	enumITEMOPT_ERROR_STATE,	// 
	enumITEMOPT_ERROR_PROTECT,	// 
	enumITEMOPT_ERROR_AREA,		// 
	enumITEMOPT_ERROR_BODY,		// 
	enumITEMOPT_ERROR_TYPE,		// 
	enumITEMOPT_ERROR_INVALID,	// 
	enumITEMOPT_ERROR_KBRANGE,	// 
	enumITEMOPT_ERROR_EXPIRATION	// Item expiration by Mdr
};

enum EEntitySeenType // 
{
	enumENTITY_SEEN_NEW,		// 
	enumENTITY_SEEN_SWITCH,		// 
};

enum EPlayerReliveType
{
	enumEPLAYER_RELIVE_NONE,	// 
	enumEPLAYER_RELIVE_CITY,	// 
	enumEPLAYER_RELIVE_ORIGIN,	// 
	enumEPLAYER_RELIVE_NORIGIN,	// 
	enumEPLAYER_RELIVE_MAP,		// 
	enumEPLAYER_RELIVE_NOMAP,	// 
};

enum EUseSkill
{
	enumESKILL_SUCCESS,				// 
	enumESKILL_FAILD_NPC,			// NPC
	enumESKILL_FAILD_NOT_SKILLED,	// 
	enumESKILL_FAILD_SAFETY_BELT,	// 
	enumESKILL_FAILD_NOT_LAND,		// 
	enumESKILL_FAILD_NOT_SEA,		// 
	enumESKILL_FAILD_ONLY_SELF,		// 
	enumESKILL_FAILD_ONLY_DIEPLY,	// 
	enumESKILL_FAILD_FRIEND,		// 
	enumESKILL_FAILD_NOT_FRIEND,	// 
	enumESKILL_FAILD_NOT_PALYER,	// 
	enumESKILL_FAILD_NOT_MONS,		// 
	enumESKILL_FAILD_ESP_MONS,		// 
	enumESKILL_FAILD_SELF,			// 
};

enum EEnterMapType // Loading
{
	enumENTER_MAP_EDGE,				// 
	enumENTER_MAP_CARRY,			// 
};

enum EFightType // 
{
	enumFIGHT_NONE		= 0, // 
	enumFIGHT_TEAM		= 1, // 
	enumFIGHT_MONOMER	= 2, // 
	enumFIGHT_GUILD		= 3, // 
};

/*	2008-7-28	yangyinyu	close!		//	IDID0ID
#define defKITBAG_CUR_VER					113	// 
*/
#define defKITBAG_CUR_VER111				111	// @
#define defLOOK_CUR_VER						111	// 
#define defLOOK_CUR_VER110					110
#define defITEM_INSTANCE_ATTR_NUM_VER110	10

#define defMAP_GARNER_WIDTH					4096
#define defMAP_GARNER_HEIGHT				4096
#define defMAP_DARKBLUE_WIDTH				4096
#define defMAP_DARKBLUE_HEIGHT				4096
#define defMAP_MAGICSEA_WIDTH				4096
#define defMAP_MAGICSEA_HEIGHT				4096
#define defMAP_EASTGOAF_WIDTH				1024
#define defMAP_EASTGOAF_HEIGHT				1024
#define defMAP_LONETOWER_WIDTH				1024
#define defMAP_LONETOWER_HEIGHT				1024

#define defMAX_KBITEM_NUM_PER_TYPE		48 // 
#define defDEF_KBITEM_NUM_PER_TYPE		24 // 
#define defESPE_KBGRID_NUM				4  // 

//#pragma pack(push)
//#pragma pack(1)

typedef struct stNetChangeChaPart		// ,:-:0,CharacterRecord
{
	stNetChangeChaPart() {sVer = 0; sTypeID = 0;}

	short	sVer;
	short	sTypeID;
	union
	{
		struct
		{
			std::array<SItemGrid, enumEQUIP_NUM> SLink;
			short       sHairID;        // 
		};

		struct
		{
			USHORT sPosID;		// ID
			USHORT sBoatID;				// ID
			USHORT sHeader;				// 
			USHORT sBody;				// 
			USHORT sEngine;				// 
			USHORT sCannon;				// 
			USHORT sEquipment;			// 
		};
	};
} LOOK;


//#pragma pack(pop)

//NOTE(Ogge): Intended use is for Selection Scene
struct Look_Minimal
{
	Look_Minimal() = default;
	Look_Minimal(LOOK const& look)
	{
		typeID = look.sTypeID;

		std::transform(std::begin(look.SLink), std::end(look.SLink), std::begin(equip_IDs),
			[](SItemGrid const& item)
			{
				return item.sID;
			}
		);
	}

	uint16_t typeID{};
	std::array<uint16_t, EEquipLinkPos::enumEQUIP_NUM> equip_IDs{};
};


#define defMAX_ITEM_FORGE_GROUP	5

struct SForgeItem
{
	
	struct
	{
		short	sGridNum;
		struct
		{
			short	sGridID;
			short	sItemNum;
		} SGrid[defMAX_KBITEM_NUM_PER_TYPE];
	} SGroup[defMAX_ITEM_FORGE_GROUP];
};

#define defMAX_ITEM_LOTTERY_GROUP	7

struct SLotteryItem
{
	struct
	{
		short	sGridNum;
		struct
		{
			short	sGridID;
			short	sItemNum;
		} SGrid[defMAX_KBITEM_NUM_PER_TYPE];
	} SGroup[defMAX_ITEM_LOTTERY_GROUP];
};

#define defMAX_ITEM_LIFESKILL_GROUP	6
struct SLifeSkillItem
{
		short sGridID[defMAX_ITEM_LIFESKILL_GROUP];
		short sbagCount;
		short sReturn;
};


#define defMAX_CITY_NUM		8
extern const char	*g_szCityName[defMAX_CITY_NUM];

//ID
const int PLAY_NUM = 4;
extern const long g_PartIdRange[PLAY_NUM][enumEQUIP_NUM + 1][2];
//end

#define defMOTTO_LEN		40
#define defGUILD_NAME_LEN	17	// 
#define defGUILD_MOTTO_LEN	51	// 
#define defPICKUP_DISTANCE	700 //  old range: 350
#define defTHROW_DISTANCE	350 // 
#define defBANK_DISTANCE	350 // 
#define defRANGE_TOUCH_DIS	350 // 

// 
const DWORD MAX_FAST_ROW = 3;
const DWORD MAX_FAST_COL = 12;
const DWORD SHORT_CUT_NUM = MAX_FAST_ROW * MAX_FAST_COL;
#define defItemShortCutType       1
#define defSkillFightShortCutType 2
#define defSkillLifeShortCutType  3
#define defSkillSailShortCutType  4

struct stNetShortCut
{
    char     chType[SHORT_CUT_NUM];     // 1-,2-,3-,0-
    short    byGridID[SHORT_CUT_NUM];   // ,
};
//
inline bool assert_shortcut_range(int index)
{
	return index > 0 && index < SHORT_CUT_NUM;
}
// Returns the shortcut index or -1 if none.
inline int IsItemShortCut(const stNetShortCut& shortcuts, int inventory_index) {
	for (int i = 0; i < SHORT_CUT_NUM; ++i)
	{
		if (shortcuts.chType[i] == defItemShortCutType &&
			shortcuts.byGridID[i] == inventory_index)
		{
			return i;
		}
	}
	return -1;
}

inline void SetItemShortCut(stNetShortCut& shortcuts, int shortcut_index, int inventory_index)
{
	if (!assert_shortcut_range(shortcut_index))
	{
		return;
	}

	shortcuts.byGridID[shortcut_index] = inventory_index;
	shortcuts.chType[shortcut_index] = defItemShortCutType;
}

inline void SwapItemShortCut(stNetShortCut& shortcuts, int inventory_index1, int inventory_index2)
{
	const int shortcut1 = IsItemShortCut(shortcuts, inventory_index1);
	const int shortcut2 = IsItemShortCut(shortcuts, inventory_index2);
	if (shortcut1 != -1 && shortcut2 != -1)
	{
		std::swap(shortcuts.byGridID[shortcut1], shortcuts.byGridID[shortcut2]);
		std::swap(shortcuts.chType[shortcut1], shortcuts.chType[shortcut2]);
	}
	else if (shortcut1 != -1)
	{
		SetItemShortCut(shortcuts, shortcut1, inventory_index2);
	}
	else if (shortcut2 != -1)
	{
		SetItemShortCut(shortcuts, shortcut2, inventory_index1);
	}
}

enum EPoseState
{
	enumPoseStand   = 0,
	enumPoseLean    = 1,
	enumPoseSeat	= 2,
};

enum EGuildState		//16
{
	emGldMembStatNormal		=0,			//
	emGldMembStatTry		=1,			//

	//emGldPermMgr			=0x1,		//
	//emGldPermBank			=0x2,		//
	//emGldPermBuild			=0x4,		//
	
	emGldPermSpeak		= 1,		
	emGldPermMgr			= 2,	
	emGldPermViewBank			= 4,	
	emGldPermDepoBank			= 8,	
	emGldPermTakeBank			= 16,
	emGldPermRecruit			= 32,	
	emGldPermKick			= 64,		
	emGldPermMotto		= 128,		
	emGldPermAttr		= 256,	
	emGldPermEnter = 512,
	emGldPermPlace = 1024,
	emGldPermRem = 2048,
	emGldPermDisband = 4096,
	emGldPermLeader = 8192,

	emGldPermMax = 0xFFFFFFFF,//0x7FFFFFF,
	emGldPermDefault = 1,
	emGldPermNum = 12,			// guildhouse objects: 14

	emMaxMemberNum			=80,
	emMaxTryMemberNum		=40,

	emGuildInitVal			=0x0000,	//
	emGuildGetName			=0x0001,	//
	emGuildReplaceOldTry	=0x0002,	//
};

#define defKITBAG_ITEM_NUM	100

//#define defLOOK_DATA_STRING_LEN	2048
#define defLOOK_DATA_STRING_LEN 8192

enum EAreaMask
{
	enumAREA_TYPE_LAND	    = 0x0001,	// 0 10
    enumAREA_TYPE_NOT_FIGHT = 0x0002,	// 1 10
    enumAREA_TYPE_PK		= 0x0004,	// 2 1PK
	enumAREA_TYPE_BRIDGE	= 0x0008,	// 3 1
	enumAREA_TYPE_NOMONSTER	= 0x0010,	// 4 1
    enumAREA_TYPE_MINE		= 0x0020,	// 5 1
    enumAREA_TYPE_FIGHT_ASK	= 0x0040,	// 6 1PK
};

extern const char* g_GetAreaName( int nValue );

#define defCHA_TERRITORY_DISCRETIONAL	2	// 
#define defCHA_TERRITORY_LAND			0	// 
#define defCHA_TERRITORY_SEA			1	// 
