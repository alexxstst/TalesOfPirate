
/* * * * * * * * * * * * * * * * * * * *
*
*      ��Ϣ����������Ϣ����
*                  by Jampe
*
* * * * * * * * * * * * * * * * * * * */


#if !defined __INFONET_H__
#define __INFONET_H__


#include <time.h>


#define     INFO_DEFAULT                0       //  ��Ϣ����Ĭ��
#define     INFO_SUCCESS                1       //  ��Ϣ���سɹ�
#define     INFO_FAILED                 2       //  ��Ϣ����ʧ��

#define     INFO_LOGIN                  1000    //  ��½����
#define     INFO_REQUEST_ACCOUNT        1010    //  ��ȡ�ʺ���Ϣ
#define     INFO_EXCEPTION_SERVICE      1900    //  �ܾ�����
#define     INFO_REQUEST_STORE          2001    //  ��ȡ�̳��б�
#define     INFO_REQUEST_AFFICHE        2002    //  ��ȡ�����б�
#define     INFO_STORE_BUY              2011    //  �������
#define     INFO_EXCHANGE_MONEY         2012    //  �һ�����
#define     INFO_REGISTER_VIP           2013    //  ����VIP
#define     INFO_REQUEST_HISTORY        2021    //  ��ȡ���׼�¼
#define     INFO_SND_GM_MAIL            4500    //  ����GM�ʼ�
#define     INFO_RCV_GM_MAIL            4501    //  ��ȡGM�ظ�


#define     INFO_MAN_DEFAULT            INFO_DEFAULT    //  ����Ĭ��ֵ
#define     INFO_MAN_SUCCESS            10              //  ������Ϣ���سɹ�
#define     INFO_MAN_FAILED             11              //  ������Ϣ����ʧ��

#define     INFO_MAN_ADD                0               //  ��������
#define     INFO_MAN_MODIFY             1               //  �����޸�
#define     INFO_MAN_DEL                2               //  ����ɾ��

#define     INFO_MAN_ITEM               6000            //  ��Ʒ����
#define     INFO_MAN_CLASS              6001            //  �������
#define     INFO_MAN_AFFICHE            6002            //  �������
#define     INFO_MAN_ALL_ITEM           6003            //  ��������δ�ϼܵ���Ʒ
#define     INFO_GET_GM_MAIL            6500            //  ��ȡGM�ʼ�
#define     INFO_RPY_GM_MAIL            6501            //  �ظ�GM�ʼ�
#define     INFO_MAIL_QUEUE             6502            //  �ʼ��б���

#define     INFO_MAN_VIP                7000            //  VIP����
#define     INFO_MAN_RATE               7001            //  �޸Ļ���



#define     INFO_TYPE_HISTORY           0               //  ���׼�¼
#define     INFO_TYPE_EXCHANGE          1               //  �һ���¼
#define     INFO_TYPE_VIP               2               //  VIP�����¼

#define     INFO_CHECK_LIVE             0               //  �������

typedef unsigned char       _byte;


//  �ʻ���Ϣ
typedef struct _RoleInfo_
{
    long        actID;          //	�ʺű��
    char        actName[21];    //	�ʺ�����
    long        chaID;          //	��ɫ���
    char        chaName[17];    //	��ɫ����
    long        moBean;         //	Ħ������
    long        rplMoney;       //	��������
    long        vip;            //  VIP����(20-50)��0Ϊ��vip
}RoleInfo, *pRoleInfo;



//  ���׼�¼
typedef struct _HistoryInfo_
{
    time_t      tradeTime;      //	����ʱ��
    long        comID;          //	��Ʒ���
    char        comName[21];    //	��Ʒ����
    int         tradeResult;    //  ���        0 �ɹ���1 �������Ʒ�۸�, 2 ����, 3 �۷�ʧ��
    long        tradeMoney;     //	���׽��
}HistoryInfo, *pHistoryInfo;



//  ������Ϣ
typedef struct _AfficheInfo_
{
    long        affiID;         //	������
    char        affiTitle[64];  //	�������
    char        comID[33];      //	������Ʒ���
    time_t      affiTime;       //  ����ʱ��
}AfficheInfo, *pAfficheInfo;



//  ������Ϣ
typedef struct _ClassInfo_
{
    short        clsID;        //	������
    char         clsName[17];   //	��������
    short        parentID;      //	��������
}ClassInfo, *pClassInfo;



//  ������Ϣ
typedef struct _ItemInfo_
{
    short       itemID;         //	���߱��
    _byte       itemNum;        //	���߸���
    _byte       itemFlute;      //	������
    short       itemAttrID[5];  //	������Ա��
    short       itemAttrVal[5]; //	�������ֵ
}ItemInfo, *pItemInfo;



//  ��Ʒ��Ϣ
typedef struct _StoreInfo_
{
    long        comID;          //	��Ʒ���
    char        comName[21];    //	��Ʒ����
    long        comClass;       //	��Ʒ����
    time_t      comTime;        //	��Ʒ�ϼ�ʱ��
    long        comPrice;       //	�۸�
    char        comRemark[129]; //	��Ʒ��ע
    int         comNumber;      //  ��Ʒʣ�������-1�����ƣ�0�¼�
    time_t      comExpire;      //  ��Ʒ����ʱ�䡣
    _byte       itemNum;        //  ���߸���
    _byte       isHot;          //  �Ƿ�����
    time_t      beginTime;      //  �ϼ�ʱ��
}StoreInfo, *pStoreInfo;



//  ��Ʒ�ṹ
typedef struct _StoreStruct_
{
    StoreInfo   store_head;     //  ��Ʒ��Ϣͷ
    pItemInfo   item_body;      //  ������Ϣ��
}StoreStruct, *pStoreStruct;



//  ��½��Ϣ
typedef struct _LoginInfo_
{
    _byte       id;             //  ���������
    char        ip[16];         //  ��½IP
    char        pwd[33];        //  ��֤��
    _byte       ban;            //  �Ƿ������
}LoginInfo, *pLoginInfo;


//  VIP��Ϣ
typedef struct _VIPInfo_
{
    int         vipID;          //  VIP���(���ͣ���Χ��20-50֮��)
    int         vipPrice;       //  VIP�۸�
}VIPInfo, *pVIPInfo;


//  Mail��Ϣ
typedef struct _MailInfo_
{
    long id;                    //  �ʼ����
    char title[33];				//	�ʼ�����
    char description[513];		//	�ʼ�����
    long actID;				    //	�������ʺ�ID
    char actName[64];			//	�������ʺ�����
    long chaID;				    //	�����߽�ɫID
    char chaName[64];			//	�����߽�ɫ����
    time_t sendTime;			//	����ʱ��
}MailInfo, *pMailInfo;


//  ��Ϣͷ
typedef struct _NetMessageHead_
{
    unsigned char msgChk[4];    //  ��Ч������Ϣ��ʶ��Ϊ 0x4A, 0x4D, 0x50, 0x53
    long msgID;                 //  ��Ϣ���
    long subID;                 //  ��Ϣ�ӱ��
    long long msgOrder;         //  ��Ϣ����
    long msgSection;            //  С�����
    long msgExtend;             //  ��չ�ֶ�
    long msgSize;               //  ��Ϣ�峤��
    long msgEncSize;            //  ���ܺ󳤶�
}NetMessageHead, *pNetMessageHead;



//  ������Ϣ
typedef struct _NetMessage_
{
    NetMessageHead      msgHead;    //  ��Ϣͷ
    void*               msgBody;    //  ��Ϣ��
}NetMessage, *pNetMessage;


#define PNM_SUCCESS             0           //  ���ͳɹ�
#define PNM_FAILED              1           //  �����Э��
#define PNM_UNKNOWN             2           //  δ֪Э��
#define PNM_EXCEPTION           3           //  �쳣


//  ��ȡ��Ϣͷ
extern int GetMessageHead(pNetMessageHead mh, unsigned char* msg, int size);

//  ������Ϣ
extern int PeekNetMessage(pNetMessage* nm, unsigned char** buf, long& bufsize, void** tmpbuf, long& tmpsize);

//  �ͷ���Ϣ
extern void FreeNetMessage(pNetMessage msg);

//  ������Ϣ
extern bool BuildNetMessage(pNetMessage nm, long msgID, long subID, long extend, long section, unsigned char* body, long size);

//  ��ȡ��Կ
extern _byte* GetInfoKey();

//  ��ȡ����(����GameServerʹ��)
extern _byte* GetPassword();

class InfoThread;
//  �ͻ��˽ӿ�
class InfoNetBase
{
public:
    InfoNetBase();
    virtual ~InfoNetBase();
    virtual bool RunInfoService(const char* ip, short port);  //  ���пͻ��˷���
    virtual bool StopInfoService(); //  ֹͣ�ͻ��˷���
    virtual bool PostInfoSend(long msgID, long subID, long extend, long section, unsigned char* body, long size, bool resnd = false);   //  ������Ϣ
    virtual bool PostInfoSend(pNetMessage msg, bool resnd = false);     //  ������Ϣ
    virtual bool IsConnect();

public:
    virtual void OnConnect(bool result) = 0;            //  ����ʱ��Ӧ
    virtual void OnNetMessage(pNetMessage msg) = 0;     //  �û��ӿ�Э�飬msg���û��ֹ��ͷš�
    virtual void OnResend(pNetMessage msg) = 0;         //  �ط���ɺ��Զ��ͷ�msg��
    virtual void OnDisconnect() = 0;        //  �Ͽ�ʱ��Ӧ

protected:
    virtual bool Connect();
    virtual bool Disconnect();      //  �Ͽ�
    virtual bool PostInfoRecv();    //  Ͷ�ݽ���
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
