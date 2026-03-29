/* Define Notify Message Const */

#define cBTI_Opened        10000
#define cBTI_Connected     10001
#define cBTI_GAME          10002
#define cBTI_STOP          10003
#define cBTI_BUYS          10004
#define cBTI_SALE          10005
#define cBTI_DEAL          10006
#define cBTI_GIVE          10007
#define cBTI_BILL          10008
#define cBTI_DATA          10009
#define cBTI_TIME          10010
#define cBTI_Disconnected  10098
#define cBTI_Closed        10099


/* Define Error Code Const */

#define cBTIErr_MemberIllegal       -10000
#define cBTIErr_MemberSuspended     -10001
#define cBTIErr_PassportInvalid     -10002
#define cBTIErr_CreditInsufficient  -10003
#define cBTIErr_GameInvalid         -10004


/* Define Information Code Const */

#define cBTIInfo_Status      10000
#define cBTIInfo_Credit      10001
#define cBTIInfo_Sex         10002
#define cBTIInfo_Birthday    10003
#define cBTIInfo_ApplyDate   10004
#define cBTIInfo_ExpireDate  10005


/* Define BTI Function Call */

#ifdef __cplusplus
extern "C" {
#endif

LONG ipBTI_Initial(LONG HWnd);
LONG ipBTI_Terminate();

LONG ipBTI_Open(BSTR *IP_A,BSTR *IP_B);
LONG ipBTI_Close();

LONG   ipBTI_IPLong(BSTR *IPText);
LPCSTR ipBTI_IPText(LONG IPLong);

LONG ipBTI_GAME(BSTR *Player,BSTR *Passport);
LONG ipBTI_STOP(BSTR *Player,LONG ErrNo);

LONG ipBTI_BUYS(BSTR *Player,LONG Credit,BSTR *ObjName,LONG ObjCount,BSTR *OrdID,BSTR *OrdTime,BSTR *Memo);
LONG ipBTI_SALE(BSTR *Player,LONG Credit,BSTR *ObjName,LONG ObjCount,BSTR *OrdID,BSTR *OrdTime,BSTR *Memo);

LONG ipBTI_DEAL(BSTR *Player_P,BSTR *Player_M,LONG Credit,BSTR *ObjName,LONG ObjCount,BSTR *OrdID,BSTR *OrdTime,BSTR *Memo);

LONG ipBTI_GIVE(BSTR *Player,LONG Credit,BSTR *SvcName,LONG SvcCount,BSTR *OrdID,BSTR *OrdTime,BSTR *Memo);
LONG ipBTI_BILL(BSTR *Player,LONG Credit,BSTR *SvcName,LONG SvcCount,BSTR *OrdID,BSTR *OrdTime,BSTR *Memo);

LONG ipBTI_DATA(BSTR *Player,LONG InfoCode);

LONG ipBTI_TIME(BSTR **PlayerList,LONG Hour,LONG Number);

#ifdef __cplusplus
}
#endif
