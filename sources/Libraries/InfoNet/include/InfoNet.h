
/* * * * * * * * * * * * * * * * * * * *
*
*      
*                  by Jampe
*
* * * * * * * * * * * * * * * * * * * */


#if !defined __INFONET_H__
#define __INFONET_H__


#include <time.h>


#define     INFO_DEFAULT                0       //  
#define     INFO_SUCCESS                1       //  
#define     INFO_FAILED                 2       //  

#define     INFO_LOGIN                  1000    //  
#define     INFO_REQUEST_ACCOUNT        1010    //  
#define     INFO_EXCEPTION_SERVICE      1900    //  
#define     INFO_REQUEST_STORE          2001    //  
#define     INFO_REQUEST_AFFICHE        2002    //  
#define     INFO_STORE_BUY              2011    //  
#define     INFO_EXCHANGE_MONEY         2012    //  
#define     INFO_REGISTER_VIP           2013    //  VIP
#define     INFO_REQUEST_HISTORY        2021    //  
#define     INFO_SND_GM_MAIL            4500    //  GM
#define     INFO_RCV_GM_MAIL            4501    //  GM


#define     INFO_MAN_DEFAULT            INFO_DEFAULT    //  
#define     INFO_MAN_SUCCESS            10              //  
#define     INFO_MAN_FAILED             11              //  

#define     INFO_MAN_ADD                0               //  
#define     INFO_MAN_MODIFY             1               //  
#define     INFO_MAN_DEL                2               //  

#define     INFO_MAN_ITEM               6000            //  
#define     INFO_MAN_CLASS              6001            //  
#define     INFO_MAN_AFFICHE            6002            //  
#define     INFO_MAN_ALL_ITEM           6003            //  
#define     INFO_GET_GM_MAIL            6500            //  GM
#define     INFO_RPY_GM_MAIL            6501            //  GM
#define     INFO_MAIL_QUEUE             6502            //  

#define     INFO_MAN_VIP                7000            //  VIP
#define     INFO_MAN_RATE               7001            //  



#define     INFO_TYPE_HISTORY           0               //  
#define     INFO_TYPE_EXCHANGE          1               //  
#define     INFO_TYPE_VIP               2               //  VIP

#define     INFO_CHECK_LIVE             0               //  

typedef unsigned char       _byte;


//  
typedef struct _RoleInfo_
{
    long        actID;          //	
    char        actName[21];    //	
    long        chaID;          //	
    char        chaName[17];    //	
    long        moBean;         //	
    long        rplMoney;       //	
    long        vip;            //  VIP(20-50)0vip
}RoleInfo, *pRoleInfo;



//  
typedef struct _HistoryInfo_
{
    time_t      tradeTime;      //	
    long        comID;          //	
    char        comName[21];    //	
    int         tradeResult;    //          0 1 , 2 , 3 
    long        tradeMoney;     //	
}HistoryInfo, *pHistoryInfo;



//  
typedef struct _AfficheInfo_
{
    long        affiID;         //	
    char        affiTitle[64];  //	
    char        comID[33];      //	
    time_t      affiTime;       //  
}AfficheInfo, *pAfficheInfo;



//  
typedef struct _ClassInfo_
{
    short        clsID;        //	
    char         clsName[17];   //	
    short        parentID;      //	
}ClassInfo, *pClassInfo;



//  
typedef struct _ItemInfo_
{
    short       itemID;         //	
    _byte       itemNum;        //	
    _byte       itemFlute;      //	
    short       itemAttrID[5];  //	
    short       itemAttrVal[5]; //	
}ItemInfo, *pItemInfo;



//  
typedef struct _StoreInfo_
{
    long        comID;          //	
    char        comName[21];    //	
    long        comClass;       //	
    time_t      comTime;        //	
    long        comPrice;       //	
    char        comRemark[129]; //	
    int         comNumber;      //  -10
    time_t      comExpire;      //  
    _byte       itemNum;        //  
    _byte       isHot;          //  
    time_t      beginTime;      //  
}StoreInfo, *pStoreInfo;



//  
typedef struct _StoreStruct_
{
    StoreInfo   store_head;     //  
    pItemInfo   item_body;      //  
}StoreStruct, *pStoreStruct;



//  
typedef struct _LoginInfo_
{
    _byte       id;             //  
    char        ip[16];         //  IP
    char        pwd[33];        //  
    _byte       ban;            //  
}LoginInfo, *pLoginInfo;


//  VIP
typedef struct _VIPInfo_
{
    int         vipID;          //  VIP(20-50)
    int         vipPrice;       //  VIP
}VIPInfo, *pVIPInfo;


//  Mail
typedef struct _MailInfo_
{
    long id;                    //  
    char title[33];				//	
    char description[513];		//	
    long actID;				    //	ID
    char actName[64];			//	
    long chaID;				    //	ID
    char chaName[64];			//	
    time_t sendTime;			//	
}MailInfo, *pMailInfo;


//  
typedef struct _NetMessageHead_
{
    unsigned char msgChk[4];    //   0x4A, 0x4D, 0x50, 0x53
    long msgID;                 //  
    long subID;                 //  
    long long msgOrder;         //  
    long msgSection;            //  
    long msgExtend;             //  
    long msgSize;               //  
    long msgEncSize;            //  
}NetMessageHead, *pNetMessageHead;



//  
typedef struct _NetMessage_
{
    NetMessageHead      msgHead;    //  
    void*               msgBody;    //  
}NetMessage, *pNetMessage;


#define PNM_SUCCESS             0           //  
#define PNM_FAILED              1           //  
#define PNM_UNKNOWN             2           //  
#define PNM_EXCEPTION           3           //  


//  
extern int GetMessageHead(pNetMessageHead mh, unsigned char* msg, int size);

//  
extern int PeekNetMessage(pNetMessage* nm, unsigned char** buf, long& bufsize, void** tmpbuf, long& tmpsize);

//  
extern void FreeNetMessage(pNetMessage msg);

//  
extern bool BuildNetMessage(pNetMessage nm, long msgID, long subID, long extend, long section, unsigned char* body, long size);

//  
extern _byte* GetInfoKey();

//  (GameServer)
extern _byte* GetPassword();

class InfoThread;
//  
class InfoNetBase
{
public:
    InfoNetBase();
    virtual ~InfoNetBase();
    virtual bool RunInfoService(const char* ip, short port);  //  
    virtual bool StopInfoService(); //  
    virtual bool PostInfoSend(long msgID, long subID, long extend, long section, unsigned char* body, long size, bool resnd = false);   //  
    virtual bool PostInfoSend(pNetMessage msg, bool resnd = false);     //  
    virtual bool IsConnect();

public:
    virtual void OnConnect(bool result) = 0;            //  
    virtual void OnNetMessage(pNetMessage msg) = 0;     //  msg
    virtual void OnResend(pNetMessage msg) = 0;         //  msg
    virtual void OnDisconnect() = 0;        //  

protected:
    virtual bool Connect();
    virtual bool Disconnect();      //  
    virtual bool PostInfoRecv();    //  
    friend class InfoThread;

protected:
    void*       m_client;
    void*       m_list;
    int         m_state;
    char        m_ip[260];
    short       m_port;
};

#ifndef __INFO_NET__
    #pragma comment(lib, "InfoNet.lib")
#endif

#endif
