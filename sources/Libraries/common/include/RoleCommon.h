// RoleCommon.h Created by knight-gongjian 2004.12.1.
//---------------------------------------------------------
#pragma once

#ifndef _ROLECOMMON_H_
#define _ROLECOMMON_H_

//---------------------------------------------------------

// 
#define ROLE_MAXNUM_DESCPAGE				12		// 
#define ROLE_MAXNUM_FUNCPAGE				4		// 
#define ROLE_MAXNUM_FLAGSIZE				16		// 1)128
#define ROLE_MAXNUM_RECORDSIZE				256     // 11024
#define ROLE_MAXSIZE_DBMISSION				2048 	// 
#define ROLE_MAXSIZE_DBTRIGGER				2048	// 
#define ROLE_MAXSIZE_DBMISCOUNT				512		// 
#define ROLE_MAXSIZE_DBRECORD				1024	// 
#define ROLE_MAXNUM_INDEXSIZE				8		// 8
#define ROLE_MAXNUM_FUNCITEM				8		// 8
#define ROLE_MAXNUM_DESPSIZE				1024    // 
#define ROLE_MAXNUM_NEEDDESPSIZE			256     // 
#define ROLE_MAXNUM_FUNCITEMSIZE			64      // 
#define ROLE_MAXNUM_CAPACITY				64		// npc
#define ROLE_MAXNUM_TRADEITEM				120     // npc
#define ROLE_MAXNUM_TRADEDATA				18      // 
#define ROLE_MAXNUM_ITEMTRADE				99		// 
#define ROLE_MAXNUM_CHARTRIGGER				64      // 
#define ROLE_MAXNUM_NPCTRIGGER				4		// NPC
#define ROLE_MAXNUM_FLAG					16		// 20
#define ROLE_MAXNUM_MISSIONSTATE			32		// NPC
#define ROLE_MAXNUM_MISSION_STEP			16		// NPC
#define ROLE_MAXNUM_RANDMISSION				10		// 4
#define ROLE_MAXNUM_INSIDE_NPCCOUNT			24		// NPCE
#define ROLE_MAXSIZE_TRADEDATA				2000	// 
#define ROLE_MAXSIZE_STALLDATA				2000	// 
#define ROLE_MAXSIZE_MSGPROC				16		// npc
#define ROLE_MAXVALUE_PARAM					16      // 
#define ROLE_MAXNUM_MISNEED					8		// 
#define ROLE_MAXNUM_MISPRIZE				8		// 
#define ROLE_MAXSIZE_MISNAME				32		// 
#define ROLE_MAXNUM_FORGE					12		// 
#define ROLE_MAXNUM_MISSION					10		// 10
#define ROLE_MAXNUM_RAND_DATA				4		// 
#define ROLE_MAXNUM_MISSIONCOUNT			32		// 
#define ROLE_MAXNUM_EUDEMON					4		// 
#define ROLE_MAXNUM_MAPNPC					512		// NPC
#define ROLE_MAXNUM_STALL_GOODS				24		// 
#define ROLE_MAXNUM_STALL_NUM				64		// 
#define ROLE_MAXSIZE_PASSWORD2				32		// 

// npc
#define ROLE_CLOSE_DESC						""

// 
#define ROLE_FIRSTPAGE						0		// 
#define ROLE_CLOSEPAGE						-1		// 

// npc
#define ROLE_TRADE_SALE						0		// 
#define ROLE_TRADE_BUY						1		// 
#define ROLE_TRADE_SALE_GOODS				2		// 
#define ROLE_TRADE_BUY_GOODS				3		// 
#define ROLE_TRADE_SELECT_BOAT				4		// 

// 
#define ROLE_TRADE_START					1		// 
#define ROLE_MAXNUM_TRADETIME				50000	// 
#define ROLE_MAXSIZE_TRADEDIST				80*80	// 

//npc
#define ROLE_MIS_ACCEPT						1<<0	// 
#define ROLE_MIS_DELIVERY					1<<1	// 
#define ROLE_MIS_PENDING					1<<2	// 
#define ROLE_MIS_IGNORE						1<<3	// 

// 
#define ROLE_MIS_PREV						0		// 
#define ROLE_MIS_NEXT						1		// 
#define ROLE_MIS_PREV_END					2		// 
#define ROLE_MIS_NEXT_END					3		// 
#define ROLE_MIS_SEL						4		// 
#define ROLE_MIS_TALK						5		// 
#define ROLE_MIS_BTNACCEPT					6		// 
#define ROLE_MIS_BTNDELIVERY				7		// 
#define ROLE_MIS_BTNPENDING					8		// 
#define ROLE_MIS_LOG						9		// 

#define ROLE_MIS_PENDING_FLAG				0		// 
#define ROLE_MIS_COMPLETE_FLAG				1		// 
#define ROLE_MIS_FAILURE_FALG				2		// 

#define	EM_OK								0		// 
#define EM_FAILER							-1		// 

// 
#define ROLE_MIS_TRIGGER_HEADER				DWORD(*(LPDWORD)"trig")
#define ROLE_MIS_MISINFO_HEADER				DWORD(*(LPDWORD)"misn")

#define ROLE_MIS_RECORD_EDITION				0x0003	// 	
#define ROLE_MIS_TRIGGER_EDITION			0x0003	// 
#define ROLE_MIS_MISINFO_EDITION			0x0003	// 
#define ROLE_MIS_MISCOUNT_EDITION			0x0003  // 

namespace mission
{
	// 
	enum TRADE_OPTYPE	
	{ 
		TRADE_SALE				= 0,     // 
		TRADE_BUY				= 1,     // 
		TRADE_GOODS				= 2,	 // 
		TRADE_DRAGTO_ITEM		= 3,     // 
		TRADE_DRAGTO_TRADE		= 4,     // 
		TRADE_DRAGMONEY_ITEM	= 5,	 // 
		TRADE_DRAGMONEY_TRADE	= 6,	 // 
		TRADE_SUCCESS			= 7,	 // 
		TRADE_FAILER			= 8,	 // 
	};

	// 
	enum TRADE_ITEMTYPE 
	{ 
		TI_WEAPON 					= 0,	// 
		TI_DEFENCE 					= 1,	// 
		TI_OTHER    				= 2,	// 
		TI_SYNTHESIS 				= 3,	// 
		MAXTRADE_ITEMTYPE			= 4,
	};

	// 
	enum TRADE_CHAR_TYPE
	{
		TRADE_CHAR					= 0,	// 
		TRADE_BOAT					= 1,	// 
	};

	// 
	enum TRIGGER_TIME_TYPE
	{
		TT_CYCLETIME	= 0,			// 
		TT_MULTITIME	= 1,			// n
	};

	// 
	enum TRIGGER_EVENT 
	{
		TE_MAP_INIT		= 0,			// 		
		TE_NPC			= 1,			// npc		
		TE_KILL			= 2,			// 		1 2ID, 3
		TE_GAME_TIME	= 3,			// 		1 2 34
		TE_CHAT			= 4,			// 	12ID 3
		TE_GET_ITEM		= 5,			// 		1 2ID 3
		TE_EQUIP_ITEM	= 6,			// 		1ID 2 3
		TE_GOTO_MAP     = 7,			// 	1ID 2x3, y, 45
		TE_LEVEL_UP     = 8,			// 			12
		TE_MAXNUM_TYPE,					// 
	};

	// 
	enum MIS_TYPE
	{
		MIS_TYPE_NOMAL  = 0,			// 
		MIS_TYPE_RAND	= 1,			// 
		MIS_TYPE_WORLD	= 2,			// 
	};

	// 
	enum MIS_SHOW_TYPE
	{
		MIS_ALLWAYS_SHOW	= 0, 	 	// 
		MIS_COMPLETE_SHOW	= 1, 		// 
	};

	// 
	enum MIS_NEED_TYPE
	{
		MIS_NEED_ITEM		= 0,		//  (1ID2)
		MIS_NEED_KILL		= 1,		//  (1ID2)
		MIS_NEED_SEND		= 2,        //  (1npcid)
		MIS_NEED_CONVOY		= 3, 		//  (1ID2x3y)
		MIS_NEED_EXPLORE	= 4,		//  (1ID2x3y)
		MIS_NEED_DESP		= 5,		// 
	};

	// 
	enum MIS_PRIZE_TYPE
	{
		MIS_PRIZE_ITEM		= 0,		//  (1ID2)
		MIS_PRIZE_MONEY		= 1,		//  (1)
		MIS_PRIZE_FAME		= 2,		//  (1)
		MIS_PRIZE_CESS		= 3,		//  (1)
	};

	// 
	enum MIS_PRIZE_SELTYPE
	{
		PRIZE_SELONE        = 0,		// 
		PRZIE_SELALL        = 1,		// 
	};

	// 
	enum MIS_RAND_TYPE
	{
		MIS_RAND_KILL		  = 0,   	//  (1ID2)
		MIS_RAND_GET		  = 1,		//  (1ID2)
		MIS_RAND_SEND		  = 2,		//    (1ID2)
		MIS_RAND_CONVOY		  = 3,		// NPC  (1ID2)
		MIS_RAND_EXPLORE	  = 4,		//  (1ID2x3y)
	};

	// 
	enum MIS_EXP_TYPE
	{
		MIS_EXP_NOMAL 			= 0,	// 
		MIS_EXP_SAIL			= 1,	// 
		MIS_EXP_LIFE			= 2,	// 
	};

	// 
	enum MIS_HELP_TYPE
	{
		MIS_HELP_DESP			= 0,	// 
		MIS_HELP_IMAGE			= 1,	// 
		MIS_HELP_SOUND			= 2,	// 
		MIS_HELP_BICKER			= 3,	// 
	};

	// 
	enum MIS_TREENODE_TYPE
	{
		MIS_TREENODE_INVALID		= 0,	// 
		MIS_TREENODE_NORMAL			= 1,	// 
		MIS_TREENODE_HISTORY		= 2,	// 
		MIS_TREENODE_GUILD			= 3,	// 
	};

	enum ENTITY_TYPE
	{
		BASE_ENTITY					= 0,	// 
		RESOURCE_ENTITY				= 1,	// 
		TRANSIT_ENTITY				= 2,	// 
		BERTH_ENTITY				= 3,	// 
	};

	enum ENTITY_STATE
	{
		ENTITY_DISABLE				= 0,	// 
		ENTITY_ENABLE				= 1,	// 
	};

	enum ENTITY_ACTION
	{
		ENTITY_START_ACTION			= 0,	// 
		ENTITY_END_ACTION			= 1,	// 
		ENTITY_INTERMIT_ACTION		= 2,	// 
	};

	enum GOODS_TYPE
	{
		RES_WOOD					= 0,	// 
		RES_MINE					= 1,	// 
	};

	enum  BOAT_LIST_TYPE
	{
		BERTH_TRADE_LIST			= 0,	// 
		BERTH_LUANCH_LIST			= 1,	// 
		BERTH_BAG_LIST				= 2,	// 
		BERTH_REPAIR_LIST			= 3,	// 
		BERTH_SALVAGE_LIST			= 4,	// 
		BERTH_SUPPLY_LIST			= 5,	// 
		BERTH_BOATLEVEL_LIST		= 6		// 
	};

	enum VIEW_ITEM_TYPE
	{
		VIEW_CHAR_BAG				= 0,	// 
		VIEW_CHARTRADE_SELF			= 1,	// 
		VIEW_CHARTRADE_OTHER		= 2,	// 
	};

	// 
	typedef struct _NET_STALL_GOODS
	{
		DWORD dwMoney;
		BYTE  byCount;
		BYTE  byIndex;
		BYTE  byGrid;

	} NET_STALL_GOODS, *PNET_STALL_GOODS;

	typedef struct _NET_STALL_ALLDATA
	{
		BYTE byNum;
		NET_STALL_GOODS Info[ROLE_MAXNUM_STALL_GOODS];

	} NET_STALL_ALLDATA, *PNET_STALL_ALLDATA;
}

#endif // _ROLECOMMON_H_

//---------------------------------------------------------
