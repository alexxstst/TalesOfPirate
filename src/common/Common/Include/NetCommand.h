/**************************************************************************************************************
*
*			(cmd)											(Created by Andor.Zhang in 2004.11)
*
**************************************************************************************************************/
#pragma once

#define NET_PROTOCOL_ENCRYPT   1	// 
#define CMD_INVALID		       0	//
//
//                 (16bit)
//			(CMD_CM_XXXCM)
//				CM	(C)lient		->Ga(m)eServer
//				MC	Ga(m)eServer	->(C)lient
//						......
//				("")
//	(5001CMD_CM_BASE+1)
//	MSG_MSG_TEAM_CANCLE_TIMEOUT;1(8bit)WriteChar/ReadChar
//
/*=====================================================================================================
*					
*/
#define CMD_CM_BASE			   0    // (C)lient       -> Ga(m)eServer
#define CMD_MC_BASE			 500    // Ga(m)eServer   -> (C)lient
#define CMD_TM_BASE			1000	// Ga(t)eServer   -> Ga(m)eServer
#define CMD_MT_BASE			1500	// Ga(m)eServer   -> Ga(t)eServer
#define CMD_TP_BASE         2000    // Ga(t)eServer   -> Grou(p)Server
#define CMD_PT_BASE         2500    // Grou(p)Server  -> Ga(t)eServer
#define CMD_PA_BASE         3000    // Grou(p)Server  -> (A)ccountServer
#define CMD_AP_BASE         3500    // (A)ccoutServer -> Grou(p)Server
#define CMD_MM_BASE			4000	// Ga(m)eServer	  -> Ga(m)eServer
#define CMD_PM_BASE         4500    // Grou(p)Server  -> Ga(m)eServer
#define	CMD_PC_BASE			5000	// Grou(p)Server  -> (C)lient
#define CMD_MP_BASE			5500	// Ga(m)eServer   -> Grou(p)Server
#define CMD_CP_BASE			6000	// (C)lient		  -> Grou(p)Server
#define CMD_OS_BASE			6500	// M(o)nitor	  -> (S)erver
#define CMD_SO_BASE			7000	//(S)erver		  ->  M(o)nitor
#define CMD_TC_BASE			7500	// Ga(t)eServer   -> (C)lient
/*=====================================================================================================
*				(C)lient -> Ga(m)eServer
*/



#define CMD_CM_ROLEBASE			CMD_CM_BASE	+ 300	//(300-400)
#define CMD_CM_GULDBASE			CMD_CM_BASE	+ 400	//(400-430)
#define CMD_CM_CHARBASE			CMD_CM_BASE	+ 430	//(430-450)

#define CMD_CM_SAY			    CMD_CM_BASE + 1		//
#define CMD_CM_BEGINACTION		CMD_CM_BASE + 6
	//char	12345
	//	1
	//		long	ping
	//		Point	player
	//	2
	//		char	12
	//			1
	//				long	0ID
	//				
	//				long	ID
	//				Point	
	//			2
	//				long	ping
	//				Point	player
	//				long	0ID
	//				
	//				long	ID
	//				Point	
	//	3
	//	4
	//	5
	//		ulong	ID0
	//	6
	//		char	01

#define CMD_CM_ENDACTION	    CMD_CM_BASE + 7
	//
#define CMD_CM_SYNATTR			CMD_CM_BASE + 8		// 
#define CMD_CM_SYNSKILLBAG		CMD_CM_BASE + 9		// 
#define CMD_CM_DIE_RETURN		CMD_CM_BASE + 10	// 
#define CMD_CM_SKILLUPGRADE		CMD_CM_BASE + 11
#define CMD_CM_PING				CMD_CM_BASE + 15
#define CMD_CM_REFRESH_DATA		CMD_CM_BASE + 16
#define CMD_CM_CHECK_PING		CMD_CM_BASE + 17	// Ping
#define CMD_CM_MAP_MASK			CMD_CM_BASE + 18

#define CMD_CM_UPDATEHAIR		CMD_CM_BASE + 20	// 
#define CMD_CM_TEAM_FIGHT_ASK	CMD_CM_BASE + 21	// 
#define CMD_CM_TEAM_FIGHT_ASR	CMD_CM_BASE + 22	// (ANSWER)
#define CMD_CM_ITEM_REPAIR_ASK	CMD_CM_BASE + 23	// 
#define CMD_CM_ITEM_REPAIR_ASR	CMD_CM_BASE + 24	// 
#define CMD_CM_ITEM_FORGE_ASK	CMD_CM_BASE + 25	// 
#define CMD_CM_ITEM_FORGE_ASR	CMD_CM_BASE + 26	// 
#define CMD_CM_TIGER_START		CMD_CM_BASE + 27	 // 
#define CMD_CM_TIGER_STOP		CMD_CM_BASE + 28	 // 
//
#define CMD_CM_ITEM_FORGE_CANACTION CMD_CM_BASE +29   //
//
#define CMD_CM_KITBAG_LOCK      CMD_CM_BASE + 31    // 
#define CMD_CM_KITBAG_UNLOCK    CMD_CM_BASE + 32    // 
#define CMD_CM_KITBAG_CHECK     CMD_CM_BASE + 33    // 
#define CMD_CM_KITBAG_AUTOLOCK	CMD_CM_BASE + 34	// 
#define CMD_CM_KITBAGTEMP_SYNC	CMD_CM_BASE + 35	// 
#define CMD_CM_KITBAGTEMPlocks	CMD_CM_BASE + 36


//
#define CMD_CM_STORE_OPEN_ASK	CMD_CM_BASE + 41	// 
#define CMD_CM_STORE_LIST_ASK	CMD_CM_BASE + 42	// 
#define CMD_CM_STORE_BUY_ASK	CMD_CM_BASE + 43	// 
#define CMD_CM_STORE_CHANGE_ASK	CMD_CM_BASE + 44	// 
#define CMD_CM_STORE_QUERY		CMD_CM_BASE + 45	// 
#define CMD_CM_STORE_VIP		CMD_CM_BASE + 46	// VIP
#define CMD_CM_STORE_AFFICHE	CMD_CM_BASE + 47	// 
#define CMD_CM_STORE_CLOSE		CMD_CM_BASE + 48

//
#define CMD_CM_BLACKMARKET_EXCHANGE_REQ	CMD_CM_BASE + 51 // 
#define CMD_CM_CHEAT_CHECK		CMD_CM_BASE + 52

//
#define CMD_CM_VOLUNTER_LIST	CMD_CM_BASE + 61	// 
#define CMD_CM_VOLUNTER_ADD		CMD_CM_BASE + 62	// 
#define CMD_CM_VOLUNTER_DEL		CMD_CM_BASE + 63	// 
#define CMD_CM_VOLUNTER_SEL		CMD_CM_BASE + 64	// 
#define CMD_CM_VOLUNTER_OPEN	CMD_CM_BASE + 65	// 
#define CMD_CM_VOLUNTER_ASR		CMD_CM_BASE + 66	// 

//
#define CMD_CM_MASTER_INVITE	CMD_CM_BASE + 71	// 
#define CMD_CM_MASTER_ASR		CMD_CM_BASE + 72
#define CMD_CM_MASTER_DEL		CMD_CM_BASE + 73
#define CMD_CM_PRENTICE_DEL		CMD_CM_BASE + 74
#define CMD_CM_PRENTICE_INVITE	CMD_CM_BASE + 75
#define CMD_CM_PRENTICE_ASR		CMD_CM_BASE + 76

//
#define CMD_CM_LIFESKILL_ASR	CMD_CM_BASE + 80	//
#define CMD_CM_LIFESKILL_ASK	CMD_CM_BASE + 81	//

#define CMD_CM_BIDUP			CMD_CM_BASE + 86
#define CMD_CM_STALLSEARCH		CMD_CM_BASE + 87


#define CMD_CM_SAY2CAMP			CMD_CM_BASE + 91
#define CMD_CM_GM_SEND			CMD_CM_BASE + 92
#define CMD_CM_GM_RECV			CMD_CM_BASE + 93
#define CMD_CM_PK_CTRL			CMD_CM_BASE + 94

// Add by lark.li 20080514 begin
#define CMD_CM_ITEM_LOTTERY_ASK	CMD_CM_BASE + 95	// 
#define CMD_CM_ITEM_LOTTERY_ASR	CMD_CM_BASE + 96	// 
#define CMD_CM_RANK				CMD_CM_BASE + 97
#define CMD_CM_DailyBuffRequest	CMD_CM_BASE + 110

// End

// Add by lark.li 20080707 begin
#define CMD_CM_CAPTAIN_CONFIRM_ASR	CMD_CM_BASE + 97	// 
// End

#define CMD_CM_ITEM_AMPHITHEATER_ASK	CMD_CM_BASE + 98 //add by sunny.sun 20080726

#define	CMD_CM_ITEM_LOCK_ASK	CMD_CM_BASE	+	99
#define	CMD_CM_ITEM_UNLOCK_ASK	CMD_CM_BASE	+	100
#define	CMD_CM_GAME_REQUEST_PIN	CMD_CM_BASE	+	101

//Begin 
	#define CMD_CM_GUILD_PUTNAME	CMD_CM_GULDBASE	+ 1		//
	#define CMD_CM_GUILD_TRYFOR		CMD_CM_GULDBASE	+ 2		//
	#define CMD_CM_GUILD_TRYFORCFM	CMD_CM_GULDBASE	+ 3		//
	#define CMD_CM_GUILD_LISTTRYPLAYER	CMD_CM_GULDBASE	+ 4	//

	#define CMD_CM_GUILD_APPROVE	CMD_CM_GULDBASE	+ 5	//
	#define CMD_CM_GUILD_REJECT		CMD_CM_GULDBASE	+ 6	//
	#define	CMD_CM_GUILD_KICK		CMD_CM_GULDBASE	+ 7
	#define CMD_CM_GUILD_LEAVE		CMD_CM_GULDBASE	+ 8
	#define CMD_CM_GUILD_DISBAND	CMD_CM_GULDBASE	+ 9
	#define CMD_CM_GUILD_MOTTO		CMD_CM_GULDBASE	+ 10
	#define CMD_CM_GUILD_CHALLENGE  CMD_CM_GULDBASE + 11 // 
	#define	CMD_CM_GUILD_LEIZHU		CMD_CM_GULDBASE + 12 // 
	#define 	CMD_CM_GUILD_PERM			CMD_CM_GULDBASE + 13
//End 

//Begin CMD_CM_CHARBASE
	#define CMD_CM_LOGIN			CMD_CM_CHARBASE + 1		//(/)
	#define CMD_CM_LOGOUT		    CMD_CM_CHARBASE + 2		//
	#define CMD_CM_BGNPLAY		    CMD_CM_CHARBASE + 3		//
	#define CMD_CM_ENDPLAY			CMD_CM_CHARBASE + 4		////
	#define CMD_CM_NEWCHA			CMD_CM_CHARBASE	+ 5		//
	#define CMD_CM_DELCHA			CMD_CM_CHARBASE	+ 6		//
	#define CMD_CM_CANCELEXIT		CMD_CM_CHARBASE + 7		//
	#define CMD_CM_REGISTER			CMD_CM_CHARBASE + 8		//
//End 

//Begin
	#define CMD_CM_REQUESTNPC		CMD_CM_ROLEBASE + 1 // NPC
	#define CMD_CM_REQUESTTALK		CMD_CM_ROLEBASE + 1	// NPC
	#define CMD_CM_TALKPAGE			CMD_CM_ROLEBASE + 2 // NPC
	#define CMD_CM_FUNCITEM			CMD_CM_ROLEBASE + 3 // 

	#define CMD_CM_REQUESTTRADE		CMD_CM_ROLEBASE + 8 // 
	#define CMD_CM_TRADEITEM		CMD_CM_ROLEBASE + 9 // 
	#define CMD_CM_REQUESTAGENCY	CMD_CM_ROLEBASE + 10 // npc

	//
	#define CMD_CM_CHARTRADE_REQUEST		CMD_CM_ROLEBASE + 12 // 
	#define CMD_CM_CHARTRADE_ACCEPT			CMD_CM_ROLEBASE + 13 // 
	#define CMD_CM_CHARTRADE_REJECT			CMD_CM_ROLEBASE + 14 // 
	#define CMD_CM_CHARTRADE_CANCEL			CMD_CM_ROLEBASE + 15 // 
	#define CMD_CM_CHARTRADE_ITEM			CMD_CM_ROLEBASE + 16 // 
	#define CMD_CM_CHARTRADE_VALIDATEDATA	CMD_CM_ROLEBASE + 17 // 
	#define CMD_CM_CHARTRADE_VALIDATE		CMD_CM_ROLEBASE + 18 // 
	#define CMD_CM_CHARTRADE_MONEY			CMD_CM_ROLEBASE + 19 // 

	// 
	#define CMD_CM_MISSION			CMD_CM_ROLEBASE + 22 // 
	#define CMD_CM_MISSIONLIST		CMD_CM_ROLEBASE + 23 // 
	#define CMD_CM_MISSIONTALK		CMD_CM_ROLEBASE + 24 // 
	#define	CMD_CM_MISLOG			CMD_CM_ROLEBASE + 25 // 
	#define	CMD_CM_MISLOGINFO		CMD_CM_ROLEBASE + 26 // 
	#define CMD_CM_MISLOG_CLEAR		CMD_CM_ROLEBASE + 27 // 
	
	// 
	#define CMD_CM_STALL_ALLDATA	CMD_CM_ROLEBASE + 30 // 
	#define CMD_CM_STALL_OPEN		CMD_CM_ROLEBASE + 31 // 
	#define CMD_CM_STALL_BUY		CMD_CM_ROLEBASE + 32 // 
	#define CMD_CM_STALL_CLOSE		CMD_CM_ROLEBASE + 33 // 

	// 
	#define CMD_CM_FORGE			CMD_CM_ROLEBASE + 35 // 

    
	// 
	#define CMD_CM_CREATE_BOAT		CMD_CM_ROLEBASE + 38 // 
	#define CMD_CM_UPDATEBOAT_PART	CMD_CM_ROLEBASE + 39 // 
	#define CMD_CM_BOAT_CANCEL		CMD_CM_ROLEBASE + 40 // 
	#define CMD_CM_BOAT_LUANCH		CMD_CM_ROLEBASE + 41 // 
	#define CMD_CM_BOAT_BAGSEL		CMD_CM_ROLEBASE + 42 // 
	#define CMD_CM_BOAT_SELECT		CMD_CM_ROLEBASE + 43 // 
	#define CMD_CM_BOAT_GETINFO		CMD_CM_ROLEBASE + 44 //	

	// 
	#define CMD_CM_ENTITY_EVENT		CMD_CM_ROLEBASE + 45 // 

	// 
	#define CMD_CM_CREATE_PASSWORD2	CMD_CM_ROLEBASE + 46 // 
	#define CMD_CM_UPDATE_PASSWORD2 CMD_CM_ROLEBASE + 47 //  

    //
    #define CMD_CM_READBOOK_START	CMD_CM_ROLEBASE + 48 // 
	#define CMD_CM_READBOOK_CLOSE	CMD_CM_ROLEBASE + 49 // 

#define CMD_CM_SEND_PRIVATE_KEY		CMD_CM_ROLEBASE + 55
#define CMD_CM_REQUEST_DROP_RATE	CMD_CM_ROLEBASE + 56
#define CMD_CM_REQUEST_EXP_RATE		CMD_CM_ROLEBASE + 57
#define CMD_CM_OFFLINE_MODE			CMD_CM_ROLEBASE + 58 
//End 

//Begin
#define CMD_CM_GARNER2_REORDER		CMD_CM_ROLEBASE + 71			//
//End


#define CMD_CM_ANTIINDULGENCE       CMD_CM_ROLEBASE + 72   //  


/*=====================================================================================================
*		Ga(m)eServer -> (C)lient
*/
#define CMD_MC_ROLEBASE			CMD_MC_BASE	+ 300	//(300-400)
#define CMD_MC_GULDBASE			CMD_MC_BASE	+ 400	//(400-430)
#define CMD_MC_CHARBASE			CMD_MC_BASE	+ 430	//(430-450)

#define CMD_MC_SAY			    CMD_MC_BASE + 1		//
#define CMD_MC_MAPCRASH			CMD_MC_BASE	+ 3
#define CMD_MC_CHABEGINSEE		CMD_MC_BASE + 4
#define CMD_MC_CHAENDSEE	    CMD_MC_BASE + 5
#define CMD_MC_ITEMBEGINSEE		CMD_MC_BASE + 6
#define CMD_MC_ITEMENDSEE	    CMD_MC_BASE + 7

#define CMD_MC_NOTIACTION		CMD_MC_BASE + 8
	//long	ID
	//char	123
	//	1
	//		char	0x000x010x020x040x08()
	//		Point	player
	//	2
	//		char	0x000x010x020x040x08
	//		long	0ID
	//		
	//		long	ID
	//		Point	
	//	3
	//		long	ID
	//		long	ID
	//		
	//		long	ID
	//		long	
	//	4
#define CMD_MC_SYNATTR			CMD_MC_BASE + 9		// 
#define CMD_MC_SYNSKILLBAG		CMD_MC_BASE + 10	// 
#define CMD_MC_SYNASKILLSTATE	CMD_MC_BASE + 11	// 
#define CMD_MC_PING				CMD_MC_BASE + 15
#define CMD_MC_ENTERMAP			CMD_MC_BASE	+ 16	// GameServerClient
#define CMD_MC_SYSINFO			CMD_MC_BASE	+ 17	// 
#define CMD_MC_ALARM			CMD_MC_BASE	+ 18
#define CMD_MC_TEAM             CMD_MC_BASE + 19    // GameServer(, HP,SP)
#define CMD_MC_FAILEDACTION		CMD_MC_BASE + 20	// 
#define CMD_MC_MESSAGE			CMD_MC_BASE + 21	// 
#define CMD_MC_ASTATEBEGINSEE	CMD_MC_BASE + 22	// 
#define CMD_MC_ASTATEENDSEE	    CMD_MC_BASE + 23
#define CMD_MC_TLEADER_ID       CMD_MC_BASE + 24    // GameServer
#define CMD_MC_CHA_EMOTION      CMD_MC_BASE + 25    // GameServer   

#define CMD_MC_QUERY_CHA        CMD_MC_BASE + 26    // 
#define CMD_MC_QUERY_CHAITEM    CMD_MC_BASE + 27    // 
#define CMD_MC_CALL_CHA         CMD_MC_BASE + 28    // 
#define CMD_MC_GOTO_CHA         CMD_MC_BASE + 29    // 
#define CMD_MC_KICK_CHA         CMD_MC_BASE + 30    // 
#define CMD_MC_SYNDEFAULTSKILL	CMD_MC_BASE + 31	// 
#define CMD_MC_ADD_ITEM_CHA		CMD_MC_BASE + 32	// 
#define CMD_MC_DEL_ITEM_CHA		CMD_MC_BASE + 33	// 
#define CMD_MC_QUERY_CHAPING    CMD_MC_BASE + 34    // GameServerping
#define CMD_MC_QUERY_RELIVE		CMD_MC_BASE + 35	// 
#define CMD_MC_PREMOVE_TIME		CMD_MC_BASE + 36	// 
#define CMD_MC_CHECK_PING		CMD_MC_BASE + 37	// Ping
#define CMD_MC_MAP_MASK			CMD_MC_BASE + 38

#define CMD_MC_OPENHAIR			CMD_MC_BASE + 39	// 
#define CMD_MC_UPDATEHAIR_RES	CMD_MC_BASE + 40    // 
#define CMD_MC_EVENT_INFO		CMD_MC_BASE + 41	// 
#define CMD_MC_SIDE_INFO		CMD_MC_BASE + 42	// 
#define CMD_MC_TEAM_FIGHT_ASK	CMD_MC_BASE + 43	// 
#define CMD_MC_ITEM_REPAIR_ASK	CMD_MC_BASE + 44	// 
#define CMD_MC_ITEM_REPAIR_ASR	CMD_MC_BASE + 45	// 
#define CMD_MC_BEGIN_ITEM_REPAIR	CMD_MC_BASE + 46	// 
#define CMD_MC_APPEND_LOOK		CMD_MC_BASE + 47	// 
#define CMD_MC_ITEM_FORGE_ASK	CMD_MC_BASE + 48	// 
#define CMD_MC_ITEM_FORGE_ASR	CMD_MC_BASE + 49	// 
#define CMD_MC_ITEM_USE_SUC		CMD_MC_BASE + 50	// 


#define CMD_MC_KITBAG_CAPACITY	CMD_MC_BASE + 51	// 
#define CMD_MC_ESPE_ITEM		CMD_MC_BASE + 52	// 

//
#define CMD_MC_KITBAG_CHECK_ASR     CMD_MC_BASE + 53    // 
#define CMD_MC_KITBAGTEMP_SYNC		CMD_MC_BASE + 54	// 

//
#define CMD_MC_STORE_OPEN_ASR	CMD_MC_BASE + 61	// 
#define CMD_MC_STORE_LIST_ASR	CMD_MC_BASE + 62	// 
#define CMD_MC_STORE_BUY_ASR	CMD_MC_BASE + 63	// 
#define CMD_MC_STORE_CHANGE_ASR	CMD_MC_BASE + 64	// 
#define CMD_MC_STORE_QUERY		CMD_MC_BASE + 65	// 
#define CMD_MC_STORE_VIP		CMD_MC_BASE + 66	// VIP
#define CMD_MC_STORE_AFFICHE	CMD_MC_BASE + 67	// 
#define CMD_MC_POPUP_NOTICE		CMD_MC_BASE + 68

//
#define CMD_MC_BLACKMARKET_EXCHANGEDATA		CMD_MC_BASE + 71	//
#define CMD_MC_BLACKMARKET_EXCHANGE_ASR		CMD_MC_BASE + 72	//
#define CMD_MC_BLACKMARKET_EXCHANGEUPDATE	CMD_MC_BASE + 73	//
#define CMD_MC_BLACKMARKET_TRADEUPDATE		CMD_MC_BASE + 74	//
#define CMD_MC_EXCHANGEDATA					CMD_MC_BASE + 75	//

//
#define CMD_MC_VOLUNTER_LIST	CMD_MC_BASE + 81	// 
#define CMD_MC_VOLUNTER_STATE	CMD_MC_BASE + 82	// 
#define CMD_MC_VOLUNTER_SEL		CMD_MC_BASE + 83	// 
#define CMD_MC_VOLUNTER_OPEN	CMD_MC_BASE + 84	// 
#define CMD_MC_VOLUNTER_ASK		CMD_MC_BASE + 85	// 

#define CMD_MC_LISTAUCTION		CMD_MC_BASE + 86

#define CMD_MC_RANK				CMD_MC_BASE + 87
#define CMD_MC_STALLSEARCH		CMD_MC_BASE + 88


//
#define CMD_MC_MASTER_ASK		CMD_MC_BASE + 91
#define CMD_MC_PRENTICE_ASK		CMD_MC_BASE + 92
#define	CMD_MC_CHAPLAYEFFECT	CMD_MC_BASE + 93

#define CMD_MC_SAY2CAMP			CMD_MC_BASE + 96
#define CMD_MC_GM_MAIL			CMD_MC_BASE + 97
#define CMD_MC_CHEAT_CHECK		CMD_MC_BASE + 98
#define	CMD_CM_ITEM_LOCK_ASR	CMD_MC_BASE	+ 99
#define	CMD_CM_ITEM_UNLOCK_ASR	CMD_MC_BASE	+ 100

#define	CMD_MC_ITEM_UNLOCK_ASR	CMD_MC_BASE	+ 95
#define CMD_MC_REQUEST_DROP_RATE	CMD_MC_BASE + 123
#define CMD_MC_REQUEST_EXP_RATE	CMD_MC_BASE + 124

//Begin 
	#define CMD_MC_GUILD_GETNAME	CMD_MC_GULDBASE	+ 1		//
	#define CMD_MC_LISTGUILD		CMD_MC_GULDBASE	+ 2		//
	#define CMD_MC_GUILD_TRYFORCFM	CMD_MC_GULDBASE	+ 3		//
	#define CMD_MC_GUILD_LISTTRYPLAYER	CMD_MC_GULDBASE	+ 4	//
	#define CMD_MC_GUILD_LISTCHALL	CMD_MC_GULDBASE + 5		//
	#define CMD_MC_GUILD_MOTTO		CMD_MC_GULDBASE	+ 10	//
	#define CMD_MC_GUILD_LEAVE		CMD_MC_GULDBASE + 11    //
	#define CMD_MC_GUILD_KICK		CMD_MC_GULDBASE + 12	//
	#define	CMD_MC_GUILD_INFO		CMD_MC_GULDBASE + 13	//
//End 

//Begin CMD_MC_CHARBASE
	#define CMD_MC_LOGIN			CMD_MC_CHARBASE + 1
	#define CMD_MC_LOGOUT		    CMD_MC_CHARBASE + 2
	#define CMD_MC_BGNPLAY		    CMD_MC_CHARBASE + 3
	#define CMD_MC_ENDPLAY			CMD_MC_CHARBASE + 4
	#define CMD_MC_NEWCHA			CMD_MC_CHARBASE	+ 5
	#define CMD_MC_DELCHA			CMD_MC_CHARBASE	+ 6
	#define CMD_MC_STARTEXIT		CMD_MC_CHARBASE + 7		// 
	#define CMD_MC_CANCELEXIT		CMD_MC_CHARBASE + 8		// 
	#define CMD_MC_CREATE_PASSWORD2 CMD_MC_CHARBASE + 11	// 
	#define CMD_MC_UPDATE_PASSWORD2 CMD_MC_CHARBASE + 12	// 
	#define CMD_MC_SEND_SERVER_PUBLIC_KEY CMD_MC_CHARBASE + 13
	#define CMD_MC_SEND_HANDSHAKE		CMD_MC_CHARBASE + 14
//End 

//Begin 
	#define CMD_MC_TALKPAGE			CMD_MC_ROLEBASE + 1 // 
	#define CMD_MC_FUNCPAGE			CMD_MC_ROLEBASE + 2 // 
	#define CMD_MC_CLOSETALK		CMD_MC_ROLEBASE + 3 // 
	#define CMD_MC_HELPINFO			CMD_MC_ROLEBASE + 4 // 
	#define CMD_MC_TRADEPAGE		CMD_MC_ROLEBASE + 5 // 
	#define CMD_MC_TRADERESULT		CMD_MC_ROLEBASE + 6 // 
	#define	CMD_MC_TRADE_DATA		CMD_MC_ROLEBASE + 7 // NPC
	#define	CMD_MC_TRADE_ALLDATA	CMD_MC_ROLEBASE + 8 // NPC
	#define CMD_MC_TRADE_UPDATE		CMD_MC_ROLEBASE + 9 // 
	#define CMD_MC_EXCHANGE_UPDATE	CMD_MC_ROLEBASE + 10// 

	//
	#define CMD_MC_CHARTRADE				CMD_MC_ROLEBASE + 11 //  
	#define CMD_MC_CHARTRADE_REQUEST		CMD_MC_ROLEBASE + 12 // 
	#define CMD_MC_CHARTRADE_CANCEL			CMD_MC_ROLEBASE + 13 //  
	#define CMD_MC_CHARTRADE_ITEM			CMD_MC_ROLEBASE + 14 // 
	#define CMD_MC_CHARTRADE_PAGE			CMD_MC_ROLEBASE + 15 // 
	#define CMD_MC_CHARTRADE_VALIDATEDATA	CMD_MC_ROLEBASE + 16 // 
	#define CMD_MC_CHARTRADE_VALIDATE		CMD_MC_ROLEBASE + 17 // 
	#define CMD_MC_CHARTRADE_RESULT		   	CMD_MC_ROLEBASE + 18 // 
	#define CMD_MC_CHARTRADE_MONEY			CMD_MC_ROLEBASE + 19 // 
	#define CMD_MC_MISSION			CMD_MC_ROLEBASE + 22// 
	#define CMD_MC_MISSIONLIST		CMD_MC_ROLEBASE + 23 // 
	#define CMD_MC_MISSIONTALK		CMD_MC_ROLEBASE + 24 // 
	#define CMD_MC_NPCSTATECHG		CMD_MC_ROLEBASE + 25 // npc
	#define CMD_MC_TRIGGER_ACTION	CMD_MC_ROLEBASE + 26 // 
	#define CMD_MC_MISPAGE			CMD_MC_ROLEBASE + 27 // 
	#define	CMD_MC_MISLOG			CMD_MC_ROLEBASE + 28 // 
	#define	CMD_MC_MISLOGINFO		CMD_MC_ROLEBASE + 29 // 
	#define	CMD_MC_MISLOG_CHANGE	CMD_MC_ROLEBASE + 30 // 
	#define CMD_MC_MISLOG_CLEAR		CMD_MC_ROLEBASE + 31 // 
	#define CMD_MC_MISLOG_ADD		CMD_MC_ROLEBASE + 32 // 

	#define CMD_MC_BEGIN_ITEM_FUSION	CMD_MC_ROLEBASE + 33	 // 
	#define CMD_MC_BEGIN_ITEM_UPGRADE	CMD_MC_ROLEBASE + 34	 // 
	#define CMD_MC_BEGIN_ITEM_FORGE		CMD_MC_ROLEBASE + 35	 // 
	#define CMD_MC_BEGIN_ITEM_UNITE		CMD_MC_ROLEBASE + 36	 // 
	#define CMD_MC_BEGIN_ITEM_MILLING	CMD_MC_ROLEBASE + 37	 // 

	#define CMD_MC_CREATEBOAT		CMD_MC_ROLEBASE + 38 // 
	#define CMD_MC_UPDATEBOAT		CMD_MC_ROLEBASE + 39 // 
	#define CMD_MC_UPDATEBOAT_PART	CMD_MC_ROLEBASE + 40 // 
	#define CMD_MC_BERTH_LIST		CMD_MC_ROLEBASE + 41 // 
	#define CMD_MC_BOAT_LIST		CMD_MC_ROLEBASE + 42 // 
	#define CMD_MC_BOAT_ADD			CMD_MC_ROLEBASE + 43 // 
	#define CMD_MC_BOAT_CLEAR		CMD_MC_ROLEBASE + 44 // 
	#define CMD_MC_BOATINFO			CMD_MC_ROLEBASE + 45 // 
	#define CMD_MC_BOAT_BAGLIST		CMD_MC_ROLEBASE + 46 // 

	#define CMD_MC_BEGIN_ITEM_EIDOLON_METEMPSYCHOSIS CMD_MC_ROLEBASE + 47 // 
	#define CMD_MC_BEGIN_ITEM_EIDOLON_FUSION CMD_MC_ROLEBASE + 48 // 
	#define CMD_MC_BEGIN_ITEM_PURIFY 	CMD_MC_ROLEBASE + 49	 // 
	
	// 
	#define CMD_MC_ENTITY_BEGINESEE	CMD_MC_ROLEBASE + 50 // 
	#define CMD_MC_ENTITY_ENDSEE	CMD_MC_ROLEBASE + 51 // 
	#define CMD_MC_ENTITY_CHGSTATE	CMD_MC_ROLEBASE + 52 // 

	// 
	#define	CMD_MC_STALL_ALLDATA	CMD_MC_ROLEBASE + 54 // 
	#define CMD_MC_STALL_UPDATE		CMD_MC_ROLEBASE + 55 // 
	#define CMD_MC_STALL_DELGOODS	CMD_MC_ROLEBASE + 56 // 
	#define CMD_MC_STALL_CLOSE		CMD_MC_ROLEBASE + 57 // 
	#define CMD_MC_STALL_START		CMD_MC_ROLEBASE + 58 // 
	#define CMD_MC_STALL_NAME		CMD_MC_ROLEBASE + 59 // 

	// 
	#define CMD_MC_BICKER_NOTICE	CMD_MC_ROLEBASE + 60 // 

	#define CMD_MC_BEGIN_ITEM_ENERGY	CMD_MC_ROLEBASE + 71	 // 
	#define CMD_MC_BEGIN_GET_STONE		CMD_MC_ROLEBASE + 72	 // 
	#define CMD_MC_BEGIN_TIGER			CMD_MC_ROLEBASE + 73	 // 
	#define CMD_MC_TIGER_ITEM_ID		CMD_MC_ROLEBASE + 74	 // ID
	#define CMD_MC_TIGER_STOP			CMD_MC_ROLEBASE + 75	 // 
	#define CMD_MC_BEGIN_ITEM_FIX		CMD_MC_ROLEBASE + 76	 // 
	#define CMD_MC_BEGIN_GM_SEND		CMD_MC_ROLEBASE + 77	 // GM
	#define CMD_MC_BEGIN_GM_RECV		CMD_MC_ROLEBASE + 78	 // GM

	// Add by lark.li 20080514 begin
	#define CMD_MC_BEGIN_ITEM_LOTTERY		CMD_MC_ROLEBASE + 79	 // 
	// End
	#define CMD_MC_BEGIN_ITEM_AMPHITHEATER		CMD_MC_ROLEBASE + 80	 //add by sunny.sun20080716	

	#define CMD_MC_COLOUR_NOTICE	CMD_MC_ROLEBASE + 92
	#define CMD_MC_UPDATEGUILDBANKGOLD	CMD_MC_ROLEBASE + 93

//
#define CMD_MC_LIFESKILL_BGING  CMD_MC_BASE + 100	//
#define CMD_MC_LIFESKILL_ASR	CMD_MC_BASE + 101	//
#define CMD_MC_LIFESKILL_ASK	CMD_MC_BASE + 102	//

// Add by lark.li 20080515 begin
#define CMD_MC_ITEM_LOTTERY_ASK	CMD_MC_BASE + 103	// 
#define CMD_MC_ITEM_LOTTERY_ASR	CMD_MC_BASE + 104	// 
// End

// Add by lark.li 20080707 begin
#define CMD_MC_CAPTAIN_ASK	CMD_MC_BASE + 105	// 
// End

#define CMD_MC_ITEM_AMPHITHEATER_ASR  CMD_MC_BASE + 106//Add by sunny.sun 20080726

//End 


#define CMD_MC_UPDATEIMP  CMD_MC_BASE + 111

#define CMD_MC_GUILDNOTICE  CMD_MC_BASE + 112

#define CMD_MC_REQUESTPIN  CMD_MC_BASE + 113

#define CMD_MC_RecDailyBuffInfo  CMD_MC_BASE + 115

/*=====================================================================================================
*					Ga(t)eServer <->Ga(m)eServer
*/
#define CMD_TM_ENTERMAP			CMD_TM_BASE	+ 3		//ID(x,y)
#define CMD_TM_GOOUTMAP			CMD_TM_BASE + 4		//
#define CMD_TM_LOGIN_ACK		CMD_TM_BASE + 5
#define CMD_TM_CHANGE_PERSONINFO	CMD_TM_BASE	+6	//
#define CMD_TM_MAPENTRY			CMD_TM_BASE + 7
#define CMD_TM_MAPENTRY_NOMAP	CMD_TM_BASE + 8
#define CMD_TM_KICKCHA			CMD_TM_BASE + 9
#define CMD_TM_OFFLINE_MODE		CMD_TM_BASE + 11

//---------------------------------------------------

#define CMD_MT_LOGIN			CMD_MT_BASE + 1		//GameServerGateServer

#define CMD_MT_SWITCHMAP		CMD_MT_BASE + 4		//

#define CMD_MT_KICKUSER			CMD_MT_BASE + 5		//GameServer
#define CMD_MT_MAPENTRY			CMD_MT_BASE + 6		//

#define CMD_MT_PALYEREXIT		CMD_MT_BASE + 8		//Gate

/*=====================================================================================================
*					Grou(p)eServer <->Ga(m)eServer 
*/
#define CMD_PM_TEAM             CMD_PM_BASE + 1     // GroupServer
//CMD_PM_TEAMBegin
	#define TEAM_MSG_ADD            1                   // : 
	#define TEAM_MSG_LEAVE          2                   // : 
	#define TEAM_MSG_UPDATE         3                   // : ,GameServer
	
	#define TEAM_MSG_GROUP_ADD      4                   // 
	#define TEAM_MSG_GROUP_BREAK    5                   // 

	#define TEAM_MSG_KICK			6					// 
#define CMD_PM_GUILD_DISBAND	CMD_PM_BASE	+ 2		//
//CMD_PM_TEAMEnd

#define CMD_PM_GUILDINFO		CMD_PM_BASE + 4		// GroupServer
#define CMD_PM_GUILD_CHALLMONEY CMD_PM_BASE + 5     // 
#define CMD_PM_GUILD_CHALL_PRIZEMONEY CMD_PM_BASE + 6     // 
//Begin
#define CMD_PM_GARNER2_UPDATE	CMD_PM_BASE +7
//End
#define CMD_PM_TEAM_CREATE		CMD_PM_BASE + 8
#define CMD_PM_SAY2ALL			CMD_PM_BASE + 9
#define CMD_PM_SAY2TRADE		CMD_PM_BASE + 10
#define CMD_PM_EXPSCALE         CMD_PM_BASE + 11        //  , 
#define CMD_PM_GUILDBANK		CMD_PM_BASE + 15
#define CMD_PM_PUSHTOGUILDBANK		CMD_PM_BASE + 18

//---------------------------------------------------
#define CMD_MP_ENTERMAP			CMD_MP_BASE	+ 1
#define CMD_MP_GUILD_APPROVE	CMD_MP_BASE	+ 2
#define CMD_MP_GUILD_CREATE		CMD_MP_BASE	+ 3
#define CMD_MP_GUILD_KICK		CMD_MP_BASE	+ 4
#define CMD_MP_GUILD_LEAVE		CMD_MP_BASE	+ 5
#define CMD_MP_GUILD_DISBAND	CMD_MP_BASE	+ 6
#define CMD_MP_GUILD_MOTTO		CMD_MP_BASE	+ 10
#define CMD_MP_GUILD_CHALLMONEY CMD_MP_BASE + 13			//
#define CMD_MP_GUILD_CHALL_PRIZEMONEY	CMD_MP_BASE + 14    //

//Begin
#define CMD_MP_GARNER2_UPDATE	CMD_MP_BASE +7
#define CMD_MP_GARNER2_CGETORDER CMD_MP_BASE+15
//End

#define CMD_MP_TEAM_CREATE		CMD_MP_BASE + 16

#define CMD_MP_MASTER_CREATE	CMD_MP_BASE + 17
#define CMD_MP_MASTER_DEL		CMD_MP_BASE + 18
#define CMD_MP_MASTER_FINISH	CMD_MP_BASE + 19
#define CMD_MP_SAY2ALL			CMD_MP_BASE + 20
#define CMD_MP_SAY2TRADE		CMD_MP_BASE + 21
#define CMD_MP_GM1SAY1			CMD_MP_BASE + 22//add by sunny.sun20080805
#define CMD_MP_GM1SAY			CMD_MP_BASE + 23//add by sunny.sun20080821
#define CMD_MP_GMBANACCOUNT		CMD_MP_BASE	+ 24
#define CMD_MP_GMUNBANACCOUNT	CMD_MP_BASE	+ 25
#define CMD_MP_GUILD_PERM		CMD_MP_BASE + 27
#define CMD_MP_GUILDBANK		CMD_MP_BASE + 28
#define CMD_MP_PUSHTOGUILDBANK	CMD_MP_BASE + 31
#define CMD_MP_GUILDNOTICE		CMD_MP_BASE + 32
#define CMD_MP_CANRECEIVEREQUESTS	CMD_MP_BASE + 33
#define CMD_MP_MUTE_PLAYER		CMD_MP_BASE + 34

/*=====================================================================================================
*					Ga(t)eServer <->Grou(p)Server 
*/
#define	CMD_TP_LOGIN			CMD_TP_BASE + 1		//GateServerGroupServer
#define CMD_TP_LOGOUT			CMD_TP_BASE + 2		//GateServerGroupServer
#define CMD_TP_USER_LOGIN		CMD_TP_BASE	+ 3		//
#define CMD_TP_USER_LOGOUT		CMD_TP_BASE + 4		//
#define CMD_TP_BGNPLAY			CMD_TP_BASE	+ 5		//
#define CMD_TP_ENDPLAY			CMD_TP_BASE	+ 6		////
#define CMD_TP_NEWCHA			CMD_TP_BASE	+ 7		//
#define CMD_TP_DELCHA			CMD_TP_BASE	+ 8		//
#define CMD_TP_PLAYEREXIT		CMD_TP_BASE + 9		//
#define CMD_TP_REQPLYLST		CMD_TP_BASE	+ 10	//GateServer
#define CMD_TP_DISC				CMD_TP_BASE + 11
#define CMD_TP_ESTOPUSER_CHECK	CMD_TP_BASE + 12
#define CMD_TP_CREATE_PASSWORD2	CMD_TP_BASE + 13	// 
#define CMD_TP_UPDATE_PASSWORD2 CMD_TP_BASE + 14	// 

#define CMD_TP_SYNC_PLYLST		CMD_TP_BASE + 15	// 
#define CMD_TP_REGISTER			CMD_TP_BASE + 16		//
#define CMD_TP_CHANGEPASS		CMD_TP_BASE + 17

//---------------------------------------------------
#define CMD_PT_KICKUSER			CMD_PT_BASE	+ 11
#define CMD_PT_ESTOPUSER		CMD_PT_BASE + 12	// 
#define CMD_PT_DEL_ESTOPUSER	CMD_PT_BASE + 13	// 
#define CMD_PT_REGISTER			CMD_PT_BASE + 15
#define CMD_PT_KICKPLAYINGPLAYER CMD_PT_BASE + 16
/*=====================================================================================================
*					Grou(p)Server <-> (A)ccountServer
*/
#define CMD_PA_LOGIN            CMD_PA_BASE + 2
#define CMD_PA_USER_LOGIN       CMD_PA_BASE + 4
#define CMD_PA_USER_LOGOUT      CMD_PA_BASE + 5
#define CMD_PA_USER_DISABLE		CMD_PA_BASE + 6
#define CMD_PA_USER_LOGIN1		CMD_PA_BASE + 13
#define CMD_PA_USER_LOGIN2		CMD_PA_BASE + 14
#define CMD_PA_USER_BILLBGN		CMD_PA_BASE + 20
#define CMD_PA_USER_BILLEND		CMD_PA_BASE + 21
#define CMD_PA_GROUP_BILLEND_AND_LOGOUT		CMD_PA_BASE + 22
#define CMD_PA_LOGOUT			CMD_PA_BASE + 23
#define CMD_PA_GMBANACCOUNT		CMD_PA_BASE + 24
#define CMD_PA_GMUNBANACCOUNT		CMD_PA_BASE + 25
#define CMD_PA_REGISTER			CMD_PA_BASE + 26
#define CMD_PA_CHANGEPASS			CMD_PA_BASE + 27

//---------------------------------------------------
#define CMD_AP_LOGIN	        CMD_AP_BASE + 2
#define CMD_AP_USER_LOGIN		CMD_AP_BASE + 3
#define CMD_AP_RELOGIN			CMD_AP_BASE + 4
#define CMD_AP_KICKUSER			CMD_AP_BASE	+ 11
#define CMD_AP_USER_LOGIN1		CMD_AP_BASE + 12
#define CMD_AP_USER_LOGIN2		CMD_AP_BASE + 13
#define CMD_AP_EXPSCALE         CMD_AP_BASE + 14    //  
#define CMD_AP_REGISTER			CMD_AP_BASE + 15
/*=====================================================================================================
*					Ga(m)eServer <-> Ga(m)eServer
*
*/
#define CMD_MM_GATE_RELEASE		CMD_MM_BASE + 1
#define CMD_MM_GATE_CONNECT     CMD_MM_BASE + 2
#define CMD_MM_QUERY_CHA		CMD_MM_BASE + 3		// 
#define CMD_MM_QUERY_CHAITEM	CMD_MM_BASE + 4		// 
#define CMD_MM_CALL_CHA			CMD_MM_BASE + 5		// 
#define CMD_MM_GOTO_CHA		    CMD_MM_BASE + 6		// 
#define CMD_MM_KICK_CHA			CMD_MM_BASE + 7		// 
#define CMD_MM_GUILD_REJECT		CMD_MM_BASE + 8		// 
#define CMD_MM_GUILD_APPROVE	CMD_MM_BASE + 9		// 
#define CMD_MM_GUILD_KICK		CMD_MM_BASE	+ 10	// 
#define CMD_MM_GUILD_DISBAND	CMD_MM_BASE	+ 11	// 
#define CMD_MM_QUERY_CHAPING	CMD_MM_BASE + 12	// GameServerping
#define CMD_MM_NOTICE			CMD_MM_BASE + 13	// 
#define CMD_MM_GUILD_MOTTO		CMD_MM_BASE + 14	// 
#define CMD_MM_DO_STRING		CMD_MM_BASE + 15	// 
#define CMD_MM_CHA_NOTICE		CMD_MM_BASE + 16	// 
#define CMD_MM_LOGIN			CMD_MM_BASE + 17	// 
#define CMD_MM_GUILD_CHALL_PRIZEMONEY			CMD_MM_BASE + 18	// 
#define CMD_MM_ADDCREDIT		CMD_MM_BASE + 19	// 
#define CMD_MM_STORE_BUY		CMD_MM_BASE + 20	// 
#define CMD_MM_ADDMONEY			CMD_MM_BASE + 21
#define CMD_MM_AUCTION			CMD_MM_BASE + 22
#define CMD_MM_UPDATEGUILDBANK			CMD_MM_BASE + 23
#define CMD_MM_UPDATEGUILDBANKGOLD		CMD_MM_BASE + 24

/*=====================================================================================================
*					Grou(p)Server <-> (C)lient
*/
//BEGIN
	#define	CMD_CP_TEAM_INVITE		CMD_CP_BASE	+ 1		//
	#define	CMD_CP_TEAM_ACCEPT		CMD_CP_BASE	+ 2		//
	#define	CMD_CP_TEAM_REFUSE		CMD_CP_BASE	+ 3		//
	#define	CMD_CP_TEAM_LEAVE		CMD_CP_BASE	+ 4		//
	#define CMD_CP_TEAM_KICK		CMD_CP_BASE + 5		//
//END
//BEGIN
	#define CMD_CP_FRND_INVITE		CMD_CP_BASE	+ 11	//
	#define CMD_CP_FRND_ACCEPT		CMD_CP_BASE	+ 12	//
	#define CMD_CP_FRND_REFUSE		CMD_CP_BASE	+ 13	//
	#define CMD_CP_FRND_DELETE		CMD_CP_BASE	+ 14	//
	#define CMD_CP_FRND_CHANGE_GROUP	CMD_CP_BASE	+ 15	//
	#define CMD_CP_FRND_REFRESH_INFO	CMD_CP_BASE	+ 16	//(
	#define CMD_CP_CHANGE_PERSONINFO	CMD_CP_BASE	+ 17	//

// Add by lark.li 20080804 begin
	#define CMD_CP_FRND_DEL_GROUP	CMD_CP_BASE	+ 18
	#define CMD_CP_FRND_ADD_GROUP	CMD_CP_BASE	+ 19
	#define CMD_CP_FRND_MOVE_GROUP	CMD_CP_BASE	+ 20
// End

// Add by lark.li 20080808 begin
	#define CMD_CP_QUERY_PERSONINFO	CMD_CP_BASE	+ 21
// End

//END

#define CMD_CP_PING				CMD_CP_BASE	+ 22

// 
#define CMD_CP_REPORT_WG		CMD_CP_BASE + 23 // 

//BEGIN
#define CMD_CP_MASTER_REFRESH_INFO		CMD_CP_BASE	+ 31	//
#define CMD_CP_PRENTICE_REFRESH_INFO	CMD_CP_BASE	+ 32	//

//END

//Begin

//End
//Begin 
	#define CMD_CP_GM1SAY			CMD_CP_BASE	+ 400		//GM
	#define CMD_CP_SAY2TRADE		CMD_CP_BASE	+ 401		//
	#define CMD_CP_SAY2ALL		    CMD_CP_BASE + 402		//
	#define CMD_CP_SAY2YOU			CMD_CP_BASE	+ 403		//P2P()
	#define CMD_CP_SAY2TEM			CMD_CP_BASE	+ 404		//
	#define CMD_CP_SAY2GUD			CMD_CP_BASE	+ 405		//(Guild)

	#define CMD_CP_SESS_CREATE		CMD_CP_BASE	+ 406		//
	#define CMD_CP_SESS_SAY			CMD_CP_BASE	+ 407		//
	#define CMD_CP_SESS_ADD			CMD_CP_BASE	+ 408		//
	#define CMD_CP_SESS_LEAVE		CMD_CP_BASE	+ 409		//
	#define	CMD_CP_REFUSETOME		CMD_CP_BASE	+ 410		//
	#define CMD_CP_GM1SAY1			CMD_CP_BASE	+ 411		// Add by sunny.sun20080804
	#define CMD_CP_GUILDBANK		CMD_CP_BASE + 414
	#define CMD_CP_CHANGEPASS		CMD_CP_BASE + 415
//End 
//Begin
//End
//---------------------------------------------------
//BEGIN
	#define	CMD_PC_TEAM_INVITE		CMD_PC_BASE	+ 1			//
	#define CMD_PC_TEAM_REFRESH		CMD_PC_BASE	+ 2			//CMD_PM_TEAM
	#define	CMD_PC_TEAM_CANCEL		CMD_PC_BASE	+ 3			//
	//CMD_PC_TEAM_CANCELBegin
		#define MSG_TEAM_CANCLE_BUSY		1	//
		#define MSG_TEAM_CANCLE_TIMEOUT		2	//
		#define MSG_TEAM_CANCLE_OFFLINE		3	//
		#define MSG_TEAM_CANCLE_ISFULL		4	//
		#define MSG_TEAM_CANCLE_CANCEL		5	///
	//CMD_PC_TEAM_CANCELEnd
//END
//BEGIN
	#define CMD_PC_FRND_INVITE		CMD_PC_BASE	+ 11
	#define CMD_PC_FRND_REFRESH		CMD_PC_BASE	+ 12
	//CMD_PC_FRND_REFRESHBegin
		#define MSG_FRND_REFRESH_START          1   // : 
		#define MSG_FRND_REFRESH_ADD            2   // : 
		#define MSG_FRND_REFRESH_DEL            3   // : 
		#define MSG_FRND_REFRESH_ONLINE	        4	// : 
		#define MSG_FRND_REFRESH_OFFLINE        5   // : 
	//CMD_PC_FRND_REFRESHEnd
	#define CMD_PC_FRND_CANCEL		CMD_PC_BASE	+ 13
	//CMD_PC_FRND_CANCELBegin
		#define MSG_FRND_CANCLE_BUSY			1	//
		#define MSG_FRND_CANCLE_TIMEOUT			2	//
		#define MSG_FRND_CANCLE_OFFLINE			3	//
		#define MSG_FRND_CANCLE_INVITER_ISFULL	4	//
		#define MSG_FRND_CANCLE_SELF_ISFULL		5	//
		#define MSG_FRND_CANCLE_CANCEL			6	//
	//CMD_PC_FRND_CANCELEnd
	#define	CMD_PC_FRND_CHANGE_GROUP	CMD_PC_BASE + 15	//
	#define	CMD_PC_FRND_REFRESH_INFO	CMD_PC_BASE	+ 16	//
	#define CMD_PC_CHANGE_PERSONINFO	CMD_PC_BASE	+ 17	//

// Add by lark.li 20080804 begin
	#define	CMD_PC_FRND_DEL_GROUP		CMD_PC_BASE + 18	//
	#define	CMD_PC_FRND_ADD_GROUP		CMD_PC_BASE + 19	//
	#define	CMD_PC_FRND_MOVE_GROUP	CMD_PC_BASE + 20	//
// End

// Add by lark.li 20080808 begin
#define CMD_PC_QUERY_PERSONINFO	CMD_PC_BASE	+ 21	//
// End
	//
//END

#define CMD_PC_PING				CMD_PC_BASE	+ 22

//Begin
	#define CMD_PC_GUILD				CMD_PC_BASE	+ 30
	//CMD_PC_GUILDBegin
		#define MSG_GUILD_START					1
		#define MSG_GUILD_ADD					2
		#define MSG_GUILD_DEL					3
		#define MSG_GUILD_ONLINE				4
		#define MSG_GUILD_OFFLINE				5
		#define MSG_GUILD_STOP					6
#define CMD_PC_GUILD_PERM				CMD_PC_BASE	+ 31
#define CMD_PC_GM_INFO					CMD_PC_BASE	+ 32

	//CMD_PC_GUILDEnd
//End

//BEGIN
#define CMD_PC_MASTER_REFRESH		CMD_PC_BASE	+ 41
	//CMD_PC_MASTER_REFRESHBegin
	#define MSG_MASTER_REFRESH_START          1   // : 
	#define MSG_MASTER_REFRESH_ADD            2   // : 
	#define MSG_MASTER_REFRESH_DEL            3   // : 
	#define MSG_MASTER_REFRESH_ONLINE	      4	  // : 
	#define MSG_MASTER_REFRESH_OFFLINE        5   // : 
	#define MSG_PRENTICE_REFRESH_START        6   // : 
	#define MSG_PRENTICE_REFRESH_ADD          7   // : 
	#define MSG_PRENTICE_REFRESH_DEL          8   // : 
	#define MSG_PRENTICE_REFRESH_ONLINE	      9   // : 
	#define MSG_PRENTICE_REFRESH_OFFLINE     10   // : 
	//CMD_PC_MASTER_REFRESHEnd
#define CMD_PC_MASTER_CANCEL		CMD_PC_BASE	+ 42
	//CMD_PC_MASTER_CANCELBegin
	#define MSG_MASTER_CANCLE_BUSY				1	//
	#define MSG_MASTER_CANCLE_TIMEOUT			2	//
	#define MSG_MASTER_CANCLE_OFFLINE			3	//
	#define MSG_MASTER_CANCLE_INVITER_ISFULL	4	//
	#define MSG_MASTER_CANCLE_SELF_ISFULL		5	//
	#define MSG_MASTER_CANCLE_CANCEL			6	//
	//CMD_PC_MASTER_CANCELEnd
#define	CMD_PC_MASTER_REFRESH_INFO		CMD_PC_BASE	+ 43	//
#define CMD_PC_PRENTICE_REFRESH_INFO	CMD_PC_BASE	+ 44	//
#define CMD_PC_REGISTER	CMD_PC_BASE	+ 45	//

//
//END

//Begin 
#define CMD_PC_GM1SAY			CMD_PC_BASE	+ 400		//GM
#define CMD_PC_SAY2TRADE		CMD_PC_BASE	+ 401		//
#define CMD_PC_SAY2ALL		    CMD_PC_BASE + 402		//
#define CMD_PC_SAY2YOU			CMD_PC_BASE	+ 403		//P2P()
#define CMD_PC_SAY2TEM			CMD_PC_BASE	+ 404		//
#define CMD_PC_SAY2GUD			CMD_PC_BASE	+ 405		//(Guild)

#define CMD_PC_SESS_CREATE		CMD_PC_BASE	+ 406		//
#define CMD_PC_SESS_SAY			CMD_PC_BASE	+ 407		//
#define CMD_PC_SESS_ADD			CMD_PC_BASE	+ 408		//
#define CMD_PC_SESS_LEAVE		CMD_PC_BASE	+ 409		//
#define CMD_PC_GM1SAY1			CMD_PC_BASE	+ 411		////Add by sunny.sun20080804

#define CMD_PC_ERRMSG			CMD_PC_BASE	+ 414	
#define CMD_PC_GUILDNOTICE		CMD_PC_BASE	+ 417
#define CMD_PC_REFRESH_SELECT	CMD_PC_BASE + 418

//End 
//Begin
#define CMD_PC_GARNER2_ORDER	CMD_PC_BASE +101
//End
#define CMD_OS_LOGIN			CMD_OS_BASE + 1
#define CMD_OS_PING				CMD_OS_BASE + 2
#define CMD_SO_LOGIN			CMD_SO_BASE + 1
#define CMD_SO_PING				CMD_SO_BASE + 2
#define CMD_SO_WARING			CMD_SO_BASE + 3
#define CMD_SO_EXCEPTION		CMD_SO_BASE + 4

#define CMD_SO_ON_LINE			CMD_SO_BASE + 5
#define CMD_SO_OFF_LINE			CMD_SO_BASE + 6
#define CMD_SO_ENTER_MAP		CMD_SO_BASE + 7
#define CMD_SO_LEAVE_MAP		CMD_SO_BASE + 8




// Ga(t)eServer <-> (C)lient 
#define CMD_TC_DISCONNECT CMD_TC_BASE + 2

