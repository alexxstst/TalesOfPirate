/**************************************************************************************************************
*
*														(Created by Andor.Zhang in 2004.12)
*
**************************************************************************************************************/
#ifndef NETRETCODE_H
#define NETRETCODE_H

#define ERR_SUCCESS	0				// 00

//
//					(16bit)
//			(ERR_CM_XXXCM)
//				CM	(C)lient		->Ga(m)eServer
//				MC	Ga(m)eServer	->(C)lient
//						......
//				("")
//	(16bit,WriteInt64/ReadShort)
//	(5001ERR_MC_BASE+1)
//
/*=====================================================================================================
*					
*/
#define ERR_MC_BASE			   0				//GameServer/GateServer->Client1500
#define ERR_PT_BASE			 500				//GroupServer->GateServer5011000
#define ERR_AP_BASE			1000				//AccountServer->GroupServer10011500
#define ERR_MT_BASE			1500				//GameServer->GateServer15012000
#define ERR_TM_BASE         2000                //GateServer->GameServer20002500
#define ERR_OS_BASE         2500                //M(o)nitorServer->(S)erver25003000

/*=====================================================================================================
*				AccountServer->GroupServer
*/
#define ERR_AP_INVALIDUSER	ERR_AP_BASE	+ 1		//
#define ERR_AP_INVALIDPWD	ERR_AP_BASE	+ 2		//
#define ERR_AP_ACTIVEUSER   ERR_AP_BASE + 3     //
#define ERR_AP_LOGGED		ERR_AP_BASE + 4		//
#define ERR_AP_DISABLELOGIN ERR_AP_BASE + 5     //Login
#define ERR_AP_BANUSER      ERR_AP_BASE + 6     //
#define ERR_AP_PBANUSER     ERR_AP_BASE + 7     //
#define ERR_AP_SBANUSER		ERR_AP_BASE + 8	

#define ERR_AP_GPSLOGGED    ERR_AP_BASE + 11    //GroupServer
#define ERR_AP_GPSAUTHFAIL  ERR_AP_BASE + 12    //GroupServer

#define ERR_AP_UNKNOWN		ERR_AP_BASE + 100	//
#define ERR_AP_OFFLINE		ERR_AP_BASE + 101	//
#define ERR_AP_LOGIN1		ERR_AP_BASE + 102	//1
#define ERR_AP_LOGIN2		ERR_AP_BASE + 103	//2
#define ERR_AP_ONLINE		ERR_AP_BASE + 104	//
#define ERR_AP_SAVING		ERR_AP_BASE + 105	//
#define ERR_AP_LOGINTWICE	ERR_AP_BASE + 106   //
#define ERR_AP_DISCONN		ERR_AP_BASE + 107	//group
#define ERR_AP_UNKNOWNCMD	ERR_AP_BASE + 108	//
#define ERR_AP_TLSWRONG		ERR_AP_BASE + 109	//
#define ERR_AP_NOBILL		ERR_AP_BASE + 110	//
#define ERR_AP_NOALT        ERR_AP_BASE + 111   // No alts
#define ERR_AP_3ACCS        ERR_AP_BASE + 112   // max 3 accs

/*=====================================================================================================
*				GroupServer->GateServer
*/
#define	ERR_PT_LOGFAIL		ERR_PT_BASE	+ 1		//GateServerGroupServer
#define ERR_PT_SAMEGATENAME	ERR_PT_BASE	+ 2		//GateServerGateServer

#define ERR_PT_INVALIDDAT	ERR_PT_BASE	+ 20	//
#define ERR_PT_INERR		ERR_PT_BASE + 21	//
#define ERR_PT_NETEXCP		ERR_PT_BASE	+ 22	//GroupServerAccuntServer
#define ERR_PT_DBEXCP		ERR_PT_BASE + 23	//GroupServerDatabase
#define ERR_PT_INVALIDCHA	ERR_PT_BASE + 24	//(/)
#define ERR_PT_TOMAXCHA		ERR_PT_BASE	+ 25	//
#define ERR_PT_SAMECHANAME	ERR_PT_BASE	+ 26	//
#define ERR_PT_INVALIDBIRTH	ERR_PT_BASE	+ 27	//
#define ERR_PT_TOOBIGCHANM	ERR_PT_BASE	+ 28	//
#define ERR_PT_KICKUSER		ERR_PT_BASE	+ 29
#define ERR_PT_ISGLDLEADER	ERR_PT_BASE	+ 30	//
#define ERR_PT_ERRCHANAME	ERR_PT_BASE	+ 31	//
#define ERR_PT_SERVERBUSY   ERR_PT_BASE + 32	//
#define ERR_PT_TOOBIGPW2	ERR_PT_BASE + 33	//
#define ERR_PT_INVALID_PW2  ERR_PT_BASE + 34	//

// Add by lark.li 20080825 begin
#define ERR_PT_BANUSER      ERR_PT_BASE + 35     //1?a?o?
#define ERR_PT_PBANUSER     ERR_PT_BASE + 36     //??????o?
// End

#define ERR_PT_GMISLOG     ERR_PT_BASE + 37
#define ERR_PT_MULTICHA	   ERR_PT_BASE + 38
#define ERR_PT_BONUSCHARS	   ERR_PT_BASE + 39

#define ERR_PT_BADBOY		ERR_PT_BASE + 50	//

/*=====================================================================================================
*				GameServer/GateServer->Client
*/
#define ERR_MC_NETEXCP		ERR_MC_BASE	+ 1		//GateServer
#define ERR_MC_NOTSELCHA	ERR_MC_BASE	+ 2		//
#define ERR_MC_NOTPLAY		ERR_MC_BASE + 3		//ENDPLAY
#define ERR_MC_NOTARRIVE	ERR_MC_BASE + 4		//
#define ERR_MC_TOOMANYPLY	ERR_MC_BASE	+ 5		//
#define ERR_MC_NOTLOGIN		ERR_MC_BASE	+ 6		//
#define ERR_MC_VER_ERROR	ERR_MC_BASE + 7		//
#define ERR_MC_ENTER_ERROR	ERR_MC_BASE + 8		//
#define ERR_MC_ENTER_POS	ERR_MC_BASE + 9		//
#define ERR_MC_BANUSER      ERR_MC_BASE + 10     //
#define ERR_MC_PBANUSER     ERR_MC_BASE + 11     //

/*=====================================================================================================
*				GateServer->GameServer
*/
#define ERR_TM_OVERNAME     ERR_TM_BASE + 1     //GameServer
#define ERR_TM_OVERMAP      ERR_TM_BASE + 2     //GameServer
#define ERR_TM_MAPERR       ERR_TM_BASE + 3     //GameServer

#define ERR_OS_NOTMATCH_VERSION	ERR_OS_BASE + 1		// 
#define ERR_OS_RELOGIN	ERR_OS_BASE + 2		// 


namespace ReturnCode
{
	enum class OfflineMode : uint8_t
	{
		Success,
		Disabled,
		Refuse,
		Dead,
		Unknown
	};
}

#endif
