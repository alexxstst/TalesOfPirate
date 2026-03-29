/* Define Encryption Const */

#define cCode_NONE 0
#define cCode_OSBP 1
#define cCode_DES  2
#define cCode_RSA  3


/* Define Window Message Const */

#define cPAI_Succ 10000
#define cPAI_Fail 10001
#define cPAI_Miss 10002
#define cPAI_Null 10003
#define cPAI_Void 10004


/* Define PAI Function Call */

#ifdef __cplusplus
extern "C" {
#endif

VOID   ipPAI_Init(BSTR *Location);
LONG   ipPAI_Login(LONG HWnd,LONG Code,LONG Port,BSTR *USER,BSTR *PASS,BSTR *GSVR);
LPCSTR ipPAI_Data();
LPCSTR ipPAI_Reply();
LPCSTR ipPAI_Passport();
LONG   ipPAI_Version();

#ifdef __cplusplus
}
#endif
