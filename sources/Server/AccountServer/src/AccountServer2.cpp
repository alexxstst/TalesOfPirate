#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"

#include <cstdlib>
#include <crtdbg.h>
#include <string_view>

#include "AccountServer2.h"
#include "gamecommon.h"
#include "inifile.h"
#include "util.h"
#include "GlobalVariable.h"

using namespace std;

#pragma warning(disable : 4800)

//TLSIndex g_TlsIndex;
AuthQueue g_Auth;				// ÈÏÖ¤·þÎñ¶ÔÏó

LoginTmpList    tmpLogin;       //   µÇÂ½ÁÙÊ±ÁÐ±í

uLong	NetBuffer[]		= {100, 10, 0};
bool	g_logautobak	= true;

volatile long AccountServer2::m_nMembersCount=0;

int isValidMacAddress(const char* mac);

// Tcp Server
AccountServer2::AccountServer2(ThreadPool* proc,ThreadPool* comm)
    : TcpServerApp(this, proc, comm, false),
    RPCMGR(this), m_GsHeap(1, 100), m_GsList(NULL), m_GsNumber(0)
{
	m_GsHeap.Init();

    IniFile inf(g_strCfgFile.c_str());
    IniSection& is = inf["net"];
    const auto& ip = is["listen_ip"];
    unsigned short port = std::stoi(is["listen_port"]);

    // the offset of "packet length" is 0
    // the field of "packet length" is 2 bytes
    // the max length of packet is 4K bytes
    // the max length of send queue is 100
    SetPKParse(0, 2, 4 * 1024, 100);

#ifdef _DEBUG
    BeginWork(200);
#else
    BeginWork(30);
#endif
    
    OpenListenSocket(port, ip.c_str());

	ResetMembersCount();
}
AccountServer2::~AccountServer2()
{
    ShutDown(12 * 1000);
}

void AccountServer2::IncreaseMembers(long nCount)
{
	InterlockedExchangeAdd(&m_nMembersCount, nCount);
}

void AccountServer2::DecreaseMembers(long nCount)
{
	InterlockedExchangeAdd(&m_nMembersCount, -nCount);
}

void AccountServer2::ResetMembersCount()
{
	InterlockedExchange(&m_nMembersCount, 0);
}

long AccountServer2::GetMembersCount()
{
	return m_nMembersCount;
}


bool AccountServer2::OnConnect(DataSocket* datasock)
{
    return true;
}
void AccountServer2::OnProcessData(DataSocket* datasock, RPacket& rpkt)
{
    unsigned short usCmd = rpkt.ReadCmd();

    switch (usCmd) {
        // GroupServerÐ­Òé
	case CMD_PA_CHANGEPASS:{
		string name = rpkt.ReadString();
		string pass = rpkt.ReadString();
		g_MainDBHandle.UpdatePassword(name, pass);
		break;
	}
	case CMD_PA_REGISTER:{
		string name = rpkt.ReadString();
		string pass = rpkt.ReadString();
		string email = rpkt.ReadString();
		g_MainDBHandle.InsertUser(name, pass,email);
		break;
	}
    case CMD_PA_LOGOUT:
		{
			g_Auth.AddPK(datasock, rpkt);
			break;
		}

        // ÈÏÖ¤ÀàÐ­Òé
    case CMD_PA_USER_LOGOUT:
		{
			g_Auth.AddPK(datasock, rpkt);
			break;
		}

        // ¼Æ·ÑÀàÐ­Òé-ÓÃ»§¿ªÊ¼¼Æ·Ñ
    case CMD_PA_USER_BILLBGN:
		{
			IncreaseMembers();
			sUserLog *pUserLog = new sUserLog;
			pUserLog->bLogin=true;
			pUserLog->strUserName=rpkt.ReadString();
			pUserLog->strPassport=rpkt.ReadString();
			if (!PostMessage(g_hMainWnd, WM_USER_LOG_MAP, 0, (LPARAM)pUserLog))
			{
				LG("AccountServer", "AccountServer2::OnProcessData, CMD_PA_USER_BILLBGN: PostMessage WM_USER_LOG_MAP failed!\n");
				delete pUserLog;
			}
			
			
			//g_Auth.AddPK(datasock, rpkt);

			//LG("MemberCount", "Member Login! Now Count=%d\n", GetMembersCount());
			//printf("Member Login! Now Count=%d\r\n", GetMembersCount());
			//g_BillThread.AddPK(datasock, rpkt);
			break;
		}

		// ¼Æ·ÑÀàÐ­Òé-ÓÃ»§Í£Ö¹¼Æ·Ñ
    case CMD_PA_USER_BILLEND:
		{
			DecreaseMembers();
			sUserLog *pUserLog = new sUserLog;
			pUserLog->bLogin=false;
			pUserLog->strUserName=rpkt.ReadString();
			if (!PostMessage(g_hMainWnd, WM_USER_LOG_MAP, 0, (LPARAM)pUserLog))
			{
				LG("AccountServer", "AccountServer2::OnProcessData, CMD_PA_USER_BILLEND: PostMessage WM_USER_LOG_MAP failed!\n");
				delete pUserLog;
			}


			//g_Auth.AddPK(datasock, rpkt);

			//LG("MemberCount", "Member Logout! Now Count=%d\n", GetMembersCount());
			//printf("Member Logout! Now Count=%d\r\n", GetMembersCount());
			//g_BillThread.AddPK(datasock, rpkt);
			break;
		}

		// ¼Æ·ÑÀàÐ­Òé-ËùÓÐÓÃ»§½áÊø¼Æ·Ñ
    case CMD_PA_GROUP_BILLEND_AND_LOGOUT:
		{
			ResetMembersCount();
			//g_Auth.AddPK(datasock, rpkt);

			//printf("CMD_PA_GROUP_BILLEND_AND_LOGOUT! Now Count=%d\r\n", GetMembersCount());
			//LG("MemberCount", "CMD_PA_GROUP_BILLEND_AND_LOGOUT! Now Count=%d\n", GetMembersCount());
			//g_Bill.AddPK(datasock, rpkt);
			//g_BillThread.AddPK(datasock, rpkt);
			break;
		}
	case CMD_PA_GMBANACCOUNT:
		{
			string actName = rpkt.ReadString();
			g_MainDBHandle.OperAccountBan( actName, 3 );
			break;
		}
	case CMD_PA_GMUNBANACCOUNT:
		{
			string actName = rpkt.ReadString();
			g_MainDBHandle.OperAccountBan( actName, 0 );
			break;
		}
        // ÆäËüÎ´ÖªÐ­Òé
    default:
        LG("As2Excp", "Unknown usCmd=[%d]\n", usCmd);
    }
}
WPacket AccountServer2::OnServeCall(DataSocket* datasock, RPacket& rpkt)
{
    unsigned short usCmd = rpkt.ReadCmd();

    switch (usCmd) {
        // GroupServerÐ­Òé
    case CMD_PA_LOGIN:
        return Gs_Login(datasock, rpkt);

        // ÈÏÖ¤ÀàÐ­Òé
    case CMD_PA_USER_LOGIN:
        return g_Auth.SyncPK(datasock, rpkt, 20 * 1000);

        // ÆäËüÎ´ÖªÐ­Òé
    default:
		LG("As2Excp", "Unknown usCmd=[%d]\n", usCmd);
        return ProcessUnknownCmd(rpkt);
    }
}
WPacket AccountServer2::ProcessUnknownCmd(RPacket rpkt)
{
	WPacket wpkt = GetWPacket();
	wpkt.WriteShort(ERR_AP_UNKNOWNCMD);
	return wpkt;
}
void AccountServer2::OnDisconnect(DataSocket* datasock, int reason)
{
    Gs_Logout(datasock);
}

// GroupServerÏà¹Ø·½·¨
void AccountServer2::Gs_Init()
{
    m_GsNumber = 0;
    m_GsList = NULL;
    m_GsHeap.Init();
}
GroupServer2* AccountServer2::Gs_Find(char const* szGroupName)
{
    GroupServer2* curr = m_GsList;
    GroupServer2* prev = NULL;

    while (curr != NULL) {
        if (curr->m_strName.compare(szGroupName) == 0) break;

        prev = curr;
        curr = curr->m_next;
    }

    return curr;
}
bool AccountServer2::Gs_Auth(char const* szGroupName, char const* szGroupPwd)
{    
	std::string_view pwd;
    IniFile inf(g_strCfgFile.c_str());

    try {
        pwd = inf["gs"][szGroupName];
    } catch (...) {
        return false;
    }
	return pwd == szGroupPwd; 
}
void AccountServer2::Gs_ListAdd(GroupServer2* Gs)
{
    Gs->m_next = m_GsList;
    m_GsList = Gs;
    ++ m_GsNumber;
}
void AccountServer2::Gs_ListDel(GroupServer2* Gs)
{
    GroupServer2* curr = m_GsList;
    GroupServer2* prev = NULL;

    while (curr) {
        if (curr == Gs) break;

        prev = curr;
        curr = curr->m_next;
    }

    if (curr) {
        if (prev) {
            prev->m_next = curr->m_next;
        } else {
            m_GsList = curr->m_next;
        }

        -- m_GsNumber;
    }
}
void AccountServer2::Gs_Exit() {}
WPacket AccountServer2::Gs_Login(DataSocket* datasock, RPacket& rpkt)
{
/*
2005-4-14 add by Arcol: 
·¢ÏÖ´Ëº¯Êý»á²úÉú¶à¸öÏß³Ìµ÷ÓÃ,×ÔÉíÐèÒªÒ»¸öÏß³ÌÍ¬²½Ëø
·¢ÏÖ´Ëº¯ÊýÓëGs_Logout¿ÉÄÜ´æÔÚ¶àÏß³ÌÍ¬²½ÔËÐÐµÄÇé¿ö,´Ëº¯ÊýÐèÒªÓëGs_Logout½¨Á¢Ò»¸öÏß³ÌÍ¬²½Ëø
Èô´Ëº¯ÊýÄÜ¸ßËÙ·µ»Ø(²»°üº¬Êý¾Ý¿â²Ù×÷),³öÏÖÏß³ÌÍ¬²½ÎÊÌâ»úÂÊºÜµÍ
Ê¹ÓÃ´ó·¶Î§Ïß³ÌÍ¬²½Ëø¿ÉÒÔ½â¾öÍ¬Ò»º¯ÊýÄÚ²¿×ÊÔ´³åÍ»ÎÊÌâ,µ«ÎÞ·¨½â¾ö½»²æº¯Êýµ÷ÓÃµÄ³åÍ»(ËäÈ»´ËÇé¿ö³öÏÖ¼¸ÂÊ¸ü¼ÓµÍ)
×îÓÐÐ§µÄ·½·¨ÊÇ°ÑGs_LoginºÍGs_LogoutÖ®¼äµÄºÍ×ÔÉíµÄÏß³Ìµ÷ÓÃ×ª»»³É¶ÓÁÐÏûÏ¢·ÃÎÊ,Òò½á¹¹¸Ä¶¯±È½Ï´ó,Ä¿Ç°Î´²ÉÓÃ´Ë·½·¨
*/

	bool bAuthSuccess = false;
    bool bAlreadyLogin = false;
    char const* szGroupName = rpkt.ReadString();
    char const* szGroupPwd = rpkt.ReadString();

    if (FindGroup(szGroupName) != NULL) {
        bAlreadyLogin = true;
    } else {
        bAuthSuccess = Gs_Auth(szGroupName, szGroupPwd);
    }

    WPacket wpkt = GetWPacket();
    if (bAlreadyLogin) {
        wpkt.WriteShort(ERR_AP_GPSLOGGED);
    } else {
        if (bAuthSuccess) {
            GroupServer2* pGs = m_GsHeap.Get();
            pGs->m_strName = szGroupName;
            pGs->m_strAddr = datasock->GetPeerIP();
            pGs->m_datasock = datasock;

            if (AddGroup(pGs)) {
                // ¼ÓÈëµ½List³É¹¦
                datasock->SetPointer(pGs);
                wpkt.WriteShort(ERR_SUCCESS);
				//cout << "[" << szGroupName << "] Add Successfully!" << endl;
				LG("GroupServer", "[%s] Add Successfully!\n", szGroupName);
            } else {
                // ¼ÓÈëµ½ListÊ§°Ü£¬ËµÃ÷ÓÐÍ¬ÃûGroupServer¸Õ¸ÕµÇÂ¼£¬»ò´æÔÚÒì³£
                pGs->Free();
                bAlreadyLogin = true;
                wpkt.WriteShort(ERR_AP_GPSAUTHFAIL);
            }
        } else {
            wpkt.WriteShort(ERR_AP_GPSAUTHFAIL);
        }
    }

    if (bAlreadyLogin) Disconnect(datasock, 1000);
    return wpkt;
}
void AccountServer2::Gs_Logout(DataSocket* datasock)
{
/*
2005-4-14 add by Arcol: 
·¢ÏÖ´Ëº¯Êý»á²úÉú¶à¸öÏß³Ìµ÷ÓÃ,×ÔÉíÐèÒªÒ»¸öÏß³ÌÍ¬²½Ëø
·¢ÏÖ´Ëº¯ÊýÓëGs_Login¿ÉÄÜ´æÔÚ¶àÏß³ÌÍ¬²½ÔËÐÐµÄÇé¿ö,´Ëº¯ÊýÐèÒªÓëGs_Login½¨Á¢Ò»¸öÏß³ÌÍ¬²½Ëø
Èô´Ëº¯ÊýÄÜ¸ßËÙ·µ»Ø(²»°üº¬Êý¾Ý¿â²Ù×÷),³öÏÖÏß³ÌÍ¬²½ÎÊÌâ»úÂÊºÜµÍ
Ê¹ÓÃ´ó·¶Î§Ïß³ÌÍ¬²½Ëø¿ÉÒÔ½â¾öÍ¬Ò»º¯ÊýÄÚ²¿×ÊÔ´³åÍ»ÎÊÌâ,µ«ÎÞ·¨½â¾ö½»²æº¯Êýµ÷ÓÃµÄ³åÍ»(ËäÈ»´ËÇé¿ö³öÏÖ¼¸ÂÊ¸ü¼ÓµÍ)
×îÓÐÐ§µÄ·½·¨ÊÇ°ÑGs_LoginºÍGs_LogoutÖ®¼äµÄºÍ×ÔÉíµÄÏß³Ìµ÷ÓÃ×ª»»³É¶ÓÁÐÏûÏ¢·ÃÎÊ,Òò½á¹¹¸Ä¶¯±È½Ï´ó,Ä¿Ç°Î´²ÉÓÃ´Ë·½·¨
ÓÉÓÚGroupServer¶Ï¿ªÁ¬½Óºó²»Çå¿Õ×´Ì¬,ËùÒÔAccountServerÒ²²»Ó¦¸ÃÇå¿ÕÓÃ»§×´Ì¬,ÔÚµÈ´ý×Ô¶¯ÖØÁ¬½Óºó±ãÄÜ»Ö¸´Õý³£,ÕâÒªÇóGroupServerÖØÆôºóAccountServerÒ²±ØÐëÖØÆô
*/
    std::string strGroupName;

    GroupServer2* pGs = (GroupServer2 *)datasock->GetPointer();
    if ((pGs == NULL) || (pGs->m_datasock != datasock)) return;

    strGroupName = pGs->m_strName;
	LG("GroupServer", "[%s] disconnected!\n", strGroupName.c_str());

    if (DelGroup(datasock)) {
        WPacket wpkt = GetWPacket();
        wpkt.WriteCmd(CMD_PA_LOGOUT);
        wpkt.WriteString(strGroupName.c_str());
		auto rpkt = RPacket(wpkt);
        OnProcessData(datasock, rpkt);
    }
}
GroupServer2* AccountServer2::FindGroup(char const* szGroup)
{
    GroupServer2* pGs = NULL;

    m_GsListLock.lock();
    try {
        pGs = Gs_Find(szGroup);
    } catch (...) {
        LG("As2Excp", "Exception raised from KickAccount when find GroupServer: [%s]\n", szGroup);
    }
    m_GsListLock.unlock();

    return pGs;
}
void AccountServer2::DisplayGroup()
{
extern void ClearGroupList();
extern BOOL AddGroupToList(char const* strName, char const* strAddr, char const* strStatus);
    
    GroupServer2* pGs = m_GsList;
    ClearGroupList();
    m_GsListLock.lock();
    try {
        while (pGs != NULL) {
            //AddGroupToList(pGs->GetName(), pGs->GetAddr(), "ÒÑÁ¬½Ó");
			AddGroupToList(pGs->GetName(), pGs->GetAddr(), RES_STRING(AS_ACCOUNTSERVER2_CPP_00001));
            pGs = pGs->m_next;
        }
    } catch (...) {}
    m_GsListLock.unlock();
}
bool AccountServer2::AddGroup(GroupServer2* pGs)
{
    bool bAlreadyLogin = false;

    m_GsListLock.lock();
    try {
        if (Gs_Find(pGs->m_strName.c_str()) != NULL) {
            bAlreadyLogin = true;
        }

        if (!bAlreadyLogin) {
            Gs_ListAdd(pGs);
        }
    } catch (...) {
        LG("As2Excp", "Exception raised from AddGroup() when add [%s]\n", pGs->m_strName.c_str());
        bAlreadyLogin = true; // ²»ÔÊÐí´ËGroupServerµÇÂ¼
    }
    m_GsListLock.unlock();

    return !bAlreadyLogin;
}
bool AccountServer2::DelGroup(DataSocket* datasock)
{
    bool bDel = false;
    GroupServer2* pGs = NULL;

    m_GsListLock.lock();
    try {
        // ÔÙ×÷Ò»´Î¼ì²â!
        pGs = (GroupServer2 *)datasock->GetPointer();
        if ((pGs != NULL) && (pGs->m_datasock == datasock)) {
            Gs_ListDel(pGs);
            datasock->SetPointer(NULL);
            bDel = true;
            pGs->Free();
        }
    } catch (...) {
        LG("As2Excp", "Exception raised from AddGroup() when add [%s]\n", pGs->m_strName.c_str());        
    }
    m_GsListLock.unlock();

    return bDel;
}

// Auth
AuthQueue::AuthQueue() : PKQueue(false) {}
AuthQueue::~AuthQueue() {}
void AuthQueue::ProcessData(DataSocket* datasock, RPacket& rpkt)
{
    bool bRetry = true;
    unsigned short usCmd = rpkt.ReadCmd();

    // µÃµ½µ±Ç°Ïß³Ì¶ÔÏó
    AuthThread* pThis = (AuthThread *)(g_TlsIndex.GetPointer());
    if (pThis == NULL) return;

    while (bRetry) {
        try
		{
            switch (usCmd)
			{
            case CMD_PA_LOGOUT:
				{
					pThis->LogoutGroup(datasock, rpkt);
				}
                break;

			case CMD_PA_USER_LOGOUT:
			{
				pThis->AccountLogout(rpkt);
			}
			break;

			case CMD_PA_USER_BILLBGN:
				{
					pThis->BeginBilling(rpkt);
				}
				break;

			case CMD_PA_USER_BILLEND:
				{
					pThis->EndBilling(rpkt);
				}
				break;

			case CMD_PA_GROUP_BILLEND_AND_LOGOUT:
				{
					pThis->ResetBilling();
				}
				break;

            default:
                LG("AuthProcessData", "Unknown usCmd=[%d], Skipped...\n", usCmd);
                break;
            }
            bRetry = false;
        }
		catch (CSQLException* se)
		{
            LG("AuthProcessDataExcp", "SQL Exception: %s\n", se->m_strError.c_str());

			// ÖØÁ¬
			pThis->Reconnt();
        }
		catch (...)
		{
            LG("AuthProcessDataExcp", "unknown exception raised from AuthQueue::ProcessData()\n");
            bRetry = false; // ·ÇÊý¾Ý¿âÔì³ÉµÄÒì³£·Å¹ý
        }
    }
}

WPacket AuthQueue::ServeCall(DataSocket* datasock, RPacket& rpkt)
{
    bool bRetry = true;
    unsigned short usCmd = rpkt.ReadCmd();
	WPacket wpkt = datasock->GetWPacket();

    // µÃµ½µ±Ç°Ïß³Ì¶ÔÏó
    AuthThread* pThis = (AuthThread *)(g_TlsIndex.GetPointer());
	if (pThis == NULL)
	{
        LG("AuthExcp", "pThis = NULL\n");
		wpkt.WriteShort(ERR_AP_TLSWRONG);
        return wpkt;
    }

    while (bRetry)
	{
        try
		{
            switch (usCmd)
			{
				case CMD_PA_USER_LOGIN:
				{
					return pThis->QueryAndAccountLogin(datasock, rpkt);
				}

				default:
				{
					LG("AuthServeCall", "Unknown usCmd=[%d], Skipped...\n", usCmd);
					wpkt.WriteShort(ERR_AP_UNKNOWNCMD);
					return wpkt;
				}
            }
        }
		catch (CSQLException* se)
		{
            LG("AuthServeCallExcp", "SQL Exception: %s\n", se->m_strError.c_str());
            
            // ÖØÁ¬
            pThis->Reconnt();
        }
		catch (...)
		{
            LG("AuthServeCallExcp", "unknown exception raised from AuthQueue::ServerCall()\n");
            bRetry = false; // ·ÇÊý¾Ý¿âÔì³ÉµÄÒì³£·Å¹ý
        }
    }

    wpkt.WriteShort(ERR_AP_UNKNOWN);
    return wpkt;
}


//  LoginTmpList
LoginTmpList::LoginTmpList()
{
    InitializeCriticalSection(&m_cs);
}

LoginTmpList::~LoginTmpList()
{
    DeleteCriticalSection(&m_cs);
}

bool LoginTmpList::Insert(const std::string& name)
{
    bool ret = false;
    Lock();
    if(!Query(name, false))
    {
        m_list.push_back(name);
        ret = true;
    }
    UnLock();
    return ret;
}

bool LoginTmpList::Remove(const std::string& name)
{
    bool ret = false;
    TmpNameList::iterator it;
    Lock();
    for(it = m_list.begin(); it != m_list.end(); it++)
    {
        if((*it) == name)
        {
            ret = true;
            m_list.erase(it);
            break;
        }
    }
    UnLock();
    return ret;
}

bool LoginTmpList::Query(const std::string& name, bool lock/* = true*/)
{
    bool ret = false;
    TmpNameList::iterator it;
    if(lock)
    {
        Lock();
    }
    for(it = m_list.begin(); it != m_list.end(); it++)
    {
        if((*it) == name)
        {
            ret = true;
            break;
        }
    }
    if(lock)
    {
        UnLock();
    }
    return ret;
}

void LoginTmpList::Lock()
{
    EnterCriticalSection(&m_cs);
}

void LoginTmpList::UnLock()
{
    LeaveCriticalSection(&m_cs);
}



// AuthThread
Sema AuthThread::m_Sema(0, AuthThreadPool::AT_MAXNUM);
AuthThread::AuthThread(int nIndex) : m_pAuth(NULL), m_nIndex(nIndex) {}
AuthThread::~AuthThread() {Exit();}
void AuthThread::Init()
{
    g_TlsIndex.SetPointer(NULL);
    m_pAuth = NULL;

	SetRunLabel(-1);

    while (!Connect()) {
		if (GetExitFlag()) return;
        Sleep(5000);
    }
    g_TlsIndex.SetPointer(this);
    ResetAccount();

	SetRunLabel(0);
}
void AuthThread::Exit()
{
    Disconn();
    g_TlsIndex.SetPointer(NULL);
}
bool AuthThread::Connect()
{
    bool ret = true;
    if (m_pAuth != NULL) return true;

    // ´´½¨¶ÔÏó
    try {
        m_pAuth = new CSQLDatabase();
    } catch (std::bad_alloc& ba) {
        LG("AuthDBExcp", "AuthThread::Connect() new failed : %s\n", ba.what());
        m_pAuth = NULL;
        return false;
    } catch (...) {
        LG("AuthDBExcp", "AuthThread::Connect() unknown exception\n");
        m_pAuth = NULL;
        return false;
    }

    ret = m_pAuth->Open(m_strDsn.c_str());
    if (ret) {
        m_pAuth->SetAutoCommit(true);
    } else {
        delete m_pAuth;
        m_pAuth = NULL;
    }

    return ret;
}
void AuthThread::Disconn()
{
    if (m_pAuth != NULL) {
        try {
            m_pAuth->Close();
            delete m_pAuth;
        } catch (...) {
            LG("AuthExcp", "Exception raised when AuthThread::Disconn()\n");
        }
        m_pAuth = NULL;
    }    
}
void AuthThread::Reconnt()
{
	Disconn();
	while (!Connect()) {
		LG("As2", RES_STRING(AS_ACCOUNTSERVER2_CPP_00002));
		if (GetExitFlag()) return;

		Sleep(5000);
	}
}
void AuthThread::LoadConfig()
{
    try {
		IniFile inf(g_strCfgFile.c_str());
		IniSection& is = inf["db"];
		m_strDsn = is["dsnAccountDb"];
    } catch (excp& e) {
        cout << e.what() << endl;
        getchar();
        ExitProcess(-1);
    }
    //  TOM±íÃûÒÑÐÞ¸Ä
	//if (g_TomService.IsEnable())
	//{
	//	m_strAccountTableName="tom_account";
	//}
}

void AuthThread::LogUserLogin(int nUserID, string strUserName, string strIP)
{
	sUserLog *pUserLog = new sUserLog;
	pUserLog->bLogin=true;
	pUserLog->nUserID=nUserID;
	pUserLog->strUserName=strUserName;
	pUserLog->strLoginIP=strIP;
	if (!PostMessage(g_hMainWnd, WM_USER_LOG, 0, (LPARAM)pUserLog))
	{
		LG("AccountServer", "AuthThread::LogUserLogin: PostMessage WM_USER_LOG failed!\n");
		delete pUserLog;
	}
}

void AuthThread::LogUserLogout(int nUserID)
{
	sUserLog* pUserLog = new sUserLog;
	pUserLog->bLogin = false;
	pUserLog->nUserID = nUserID;
	if (!PostMessage(g_hMainWnd, WM_USER_LOG, 0, (LPARAM)pUserLog))
	{
		LG("AccountServer", "AuthThread::LogUserLogout: PostMessage WM_USER_LOG failed!\n");
		delete pUserLog;
	}
}

void AuthThread::QueryAccount(RPacket rpkt)
{
	unsigned short usNameLen;
	char const* pName = NULL;
	char szSql[512] = { 0 };
	SetRunLabel(11);

	// È¡°üÖÐµÄÐÅÏ¢£¬µÚÒ»¸öÊÇÎÞÓÃÐÅÏ¢£¬¶ªÆú(shit!)
	pName = rpkt.ReadString(&usNameLen);
	// µÚ¶þ¸öÊÇÕÊºÅÃûÐÅÏ¢	

	LG("PASSWD", "From GroupServer [%s] = [%d]\n", pName, strlen(pName));
	if ((pName == NULL) || (!IsValidName(pName, usNameLen)))
	{
		LG("AuthExcp", "NULL or INVALID Name field\n");
		m_AcctInfo.bExist = false;
		return;
	}

	m_AcctInfo.strName = pName;
	m_AcctInfo.pDat = rpkt.ReadString(); // ¼ÓÃÜÐÅÏ¢
	m_AcctInfo.strMAC = rpkt.ReadString();
	in_addr ipAddr;
	ipAddr.S_un.S_addr = rpkt.ReadLong();
	m_AcctInfo.strIP = inet_ntoa(ipAddr);

	if (isValidMacAddress(m_AcctInfo.strMAC.c_str()))
	{
		m_AcctInfo.bExist = false;
		return;
	}

	// ²éÑ¯¶ÔÏó
	SetRunLabel(12);
	CSQLRecordset rs(*m_pAuth);

	// ×éÖ¯SQL²éÑ¯Óï¾ä
	sprintf(szSql, "select id, password, sid, login_status, login_group, ban, datediff(s, enable_login_time, getdate()) as protect_time from account_login where name='%s'",
		m_AcctInfo.strName.c_str());
	rs << szSql; //account_login±íÖÐname×Ö¶ÎÒ»¶¨Òª×öÎ¨Ò»Ô¼Êø

	// Ö´ÐÐ²éÑ¯
	rs.SQLExecDirect();
	SetRunLabel(13);

	// ²úÉú²éÑ¯½á¹û
	if (rs.SQLFetch())
	{
		int n = 1;
		m_AcctInfo.bExist = true;
		m_AcctInfo.nId = rs.nSQLGetData(n++);
		m_AcctInfo.strPwdDigest = rs.SQLGetData(n++);
		m_AcctInfo.nSid = rs.nSQLGetData(n++);
		m_AcctInfo.nStatus = rs.nSQLGetData(n++);
		m_AcctInfo.strGroup = rs.SQLGetData(n++);
		// Modify by lark.li 20080825 begin
		//m_AcctInfo.bBan=rs.bSQLGetData(n++);
		m_AcctInfo.nBan = rs.nSQLGetData(n++);
		// End
		m_AcctInfo.nProtectTime = rs.nSQLGetData(n++);

		if (!tmpLogin.Insert(m_AcctInfo.strName))
		{
			m_AcctInfo.nStatus = ACCOUNT_ONLINE;
			LG("AuthExcp", "Account %s multilogin at same times.", m_AcctInfo.strName.c_str());
		}
	}
	else
	{
		m_AcctInfo.bExist = false;
	}
}
bool AuthThread::IsValidName(char const* szName, unsigned short usNameLen)
{
//TomµÄ°æ±¾ÕÊºÅÔÊÐíÓÐ"_" "-" "."3ÖÖ×Ö·û
	if (usNameLen>32) return false;
	if (usNameLen > 20) return false;
	const unsigned char *l_name =reinterpret_cast<const unsigned char *>(szName);
	bool l_ishan	=false;
	for(unsigned short i=0;i<usNameLen;i++)
	{
		if(!l_name[i])
		{
			return false;
		}else if(l_ishan)
		{
			if(l_name[i-1] ==0xA1 && l_name[i] ==0xA1)	//¹ýÂËÈ«½Ç¿Õ¸ñ
			{
				return false;
			}
			if(l_name[i] >0x3F && l_name[i] <0xFF && l_name[i] !=0x7F)
			{
				l_ishan =false;
			}else
			{
				return false;
			}
		}else if(l_name[i]>0x80 && l_name[i] <0xFF)
		{
			l_ishan	=true;
		}else if((l_name[i] >='A' && l_name[i] <='Z') ||(l_name[i] >='a' && l_name[i] <='z') ||(l_name[i] >='0' && l_name[i] <='9'))
		{

		}else if (l_name[i] == '.' || l_name[i] == '_' || l_name[i] == '-')
		{
			return false;
		}
		else
		{
			return false;
		}
	}
	return !l_ishan;

	//if (usNameLen > 20) return false;
	//if (strstr(szName, "'") != NULL) return false;
	//if (strstr(szName, "-") != NULL) return false;
	//return true;
}

WPacket AuthThread::AccountLogin(DataSocket* datasock)
{
	DWORD dwStartCount = GetTickCount();

	SetRunLabel(14);
	WPacket wpkt = datasock->GetWPacket();
	GroupServer2* pFromGroupServer = (GroupServer2*)datasock->GetPointer();
	if (pFromGroupServer == NULL)
	{
		// ´ËGroupServerÒÑ¶Ïµô
		LG("AuthExcp", "pFromGroupServer = NULL\n");
		wpkt.WriteShort(ERR_AP_DISCONN);
		return wpkt;
	}
	SetRunLabel(15);

	wpkt.WriteCmd(0);	//added by andor.zhang,±ÜÃâ»º´æÖØÓÃÔ­ÒòGroupServer¶Áµ½CMD_AP_KICKUSERÃüÁîÂë

	// Modify by lark.li 20080825 begin
	if (!m_AcctInfo.bExist)
	{
		// ²»´æÔÚ´ËÓÃ»§
		wpkt.WriteShort(ERR_AP_INVALIDUSER);
		return wpkt;
	}
	if (m_AcctInfo.nBan == 2)
	{
		tmpLogin.Remove(m_AcctInfo.strName);
		wpkt.WriteShort(ERR_AP_BANUSER);
		return wpkt;
	}
	else if (m_AcctInfo.nBan == 1)
	{
		tmpLogin.Remove(m_AcctInfo.strName);
		wpkt.WriteShort(ERR_AP_PBANUSER);
		return wpkt;
	}
	//Add by sunny.sun 20090827
	else if (m_AcctInfo.nBan == 3)
	{
		tmpLogin.Remove(m_AcctInfo.strName);
		wpkt.WriteShort(ERR_AP_BANUSER);
		return wpkt;
	}
	// End

	SetRunLabel(16);

	const bool password_match = m_AcctInfo.strPwdDigest == m_AcctInfo.pDat;
	// Change this hash !!
	if (!password_match)
	{	// ÈÏÖ¤Ê§°Ü
		wpkt.WriteShort(ERR_AP_INVALIDPWD);
		LG("AccountAuth", "Thread#%d Auth [%s] (id=%d) failed: invalid password!\n", m_nIndex, m_AcctInfo.strName.c_str(), m_AcctInfo.nId);
		tmpLogin.Remove(m_AcctInfo.strName);
		return wpkt;
	}
	SetRunLabel(17);

	int nKeyLen = 0;
	char szKey[1024] = { 0 };
	char lpszSQL[2048] = { 0 };
	char szSql[2048] = { 0 };
	const long nSid = GenSid(m_AcctInfo.strName.c_str());

	if (m_AcctInfo.nStatus == ACCOUNT_OFFLINE)
	{
		sprintf(lpszSQL, "update account_login set login_status=%d, login_group='%s', enable_login_time=getdate(), last_login_time=getdate(), last_login_mac='%s', last_login_ip='%s' where id=%d",
			ACCOUNT_ONLINE, pFromGroupServer->GetName(), m_AcctInfo.strMAC.c_str(), m_AcctInfo.strIP.c_str(), m_AcctInfo.nId);
		if (m_pAuth->ExecuteSQL(lpszSQL))
		{
			SetRunLabel(18);
			wpkt.WriteShort(ERR_SUCCESS);
			wpkt.WriteLong(m_AcctInfo.nId);
			//ChapSvr.GetSessionEncKey(szKey, sizeof szKey, nKeyLen);
			//wpkt.WriteSequence(szKey, nKeyLen);
			//ChapSvr.GetSessionClrKey(szKey, sizeof szKey, nKeyLen);
			//wpkt.WriteSequence(szKey, nKeyLen);
			wpkt.WriteLong(nSid);
			LogUserLogin(m_AcctInfo.nId, m_AcctInfo.strName.c_str(), m_AcctInfo.strIP.c_str());
		}
		else
		{
			wpkt.WriteShort(ERR_AP_UNKNOWN);
			LG("AccountAuth", "Thread#%d Auth [%s] (id=%d) failed: update database error where normal login!\n", m_nIndex, m_AcctInfo.strName.c_str(), m_AcctInfo.nId);
			SetRunLabel(19);
			goto login_over;
		}
	}
	else if (m_AcctInfo.nStatus == ACCOUNT_ONLINE)
	{
		//µÇÂ½Ç°×´Ì¬ÊÇÒÑÔÚÏß(ÖØ¸´µÇÂ½)
		sprintf(lpszSQL, "update account_login set login_status=%d, login_group='%s', last_login_time=getdate(), last_login_mac='%s', last_login_ip='%s' where id=%d",
			ACCOUNT_SAVING, pFromGroupServer->GetName(), m_AcctInfo.strMAC.c_str(), m_AcctInfo.strIP.c_str(), m_AcctInfo.nId);
		if (m_pAuth->ExecuteSQL(lpszSQL))
		{
#ifdef _RELOGIN_MODE_
			if (pFromGroupServer->IsSame(m_AcctInfo.strGroup))
			{
				wpkt.WriteCmd(CMD_AP_RELOGIN);
			}
			else
			{
				KickAccount(m_AcctInfo.strGroup, m_AcctInfo.nId);
			}
			wpkt.WriteShort(ERR_SUCCESS);
			wpkt.WriteLong(m_AcctInfo.nId);
			//ChapSvr.GetSessionEncKey(szKey, sizeof szKey, nKeyLen);
			//wpkt.WriteSequence(szKey, nKeyLen);
			//ChapSvr.GetSessionClrKey(szKey, sizeof szKey, nKeyLen);
			//wpkt.WriteSequence(szKey, nKeyLen);
			wpkt.WriteLong(nSid);
#else
			if (pFromGroupServer->IsSame(m_AcctInfo.strGroup))
			{
				wpkt.WriteCmd(CMD_AP_KICKUSER);
				wpkt.WriteShort(ERR_AP_ONLINE);
				wpkt.WriteLong(m_AcctInfo.nId);
			}
			else
			{
				KickAccount(m_AcctInfo.strGroup, m_AcctInfo.nId);
			}
			SetRunLabel(20);
			goto login_over;
#endif

		}
		else
		{
			wpkt.WriteShort(ERR_AP_UNKNOWN);
			LG("AccountAuth", "Thread#%d Auth [%s] (id=%d) failed: update database error when relogin!\n", m_nIndex, m_AcctInfo.strName.c_str(), m_AcctInfo.nId);
			SetRunLabel(21);
			goto login_over;
		}
	}
	else if (m_AcctInfo.nStatus == ACCOUNT_SAVING)
	{
		//帐号在锁定状态(保存角色)
		if (m_AcctInfo.nProtectTime >= 0 && m_AcctInfo.nProtectTime < SAVING_TIME)
		{
			wpkt.WriteShort(ERR_AP_SAVING);
			SetRunLabel(22);
			goto login_over;
		}
		else
		{
			sprintf(lpszSQL, "update account_login set login_status=%d, login_group='%s', enable_login_time=getdate(), last_login_time=getdate(), last_login_mac='%s', last_login_ip='%s' where id=%d",
				ACCOUNT_ONLINE, pFromGroupServer->GetName(), m_AcctInfo.strMAC.c_str(), m_AcctInfo.strIP.c_str(), m_AcctInfo.nId);
			if (m_pAuth->ExecuteSQL(lpszSQL)) {
				SetRunLabel(23);
				wpkt.WriteShort(ERR_SUCCESS);
				wpkt.WriteLong(m_AcctInfo.nId);
				//ChapSvr.GetSessionEncKey(szKey, sizeof szKey, nKeyLen);
				//wpkt.WriteSequence(szKey, nKeyLen);
				//ChapSvr.GetSessionClrKey(szKey, sizeof szKey, nKeyLen);
				//wpkt.WriteSequence(szKey, nKeyLen);
				wpkt.WriteLong(nSid);
				LogUserLogin(m_AcctInfo.nId, m_AcctInfo.strName.c_str(), m_AcctInfo.strIP.c_str());
			}
			else {
				wpkt.WriteShort(ERR_AP_UNKNOWN);
				LG("AccountAuth", "Thread#%d Auth [%s] (id=%d) failed: update database error when login without locked!\n", m_nIndex, m_AcctInfo.strName.c_str(), m_AcctInfo.nId);
				SetRunLabel(24);
				goto login_over;
			}
		}
	}
	else
	{
		//µÇÂ½Ç°×´Ì¬ÊÇ²»È·¶¨
		wpkt.WriteShort(ERR_AP_UNKNOWN);
		LG("AccountAuth", "Thread#%d Auth [%s] (id=%d) failed: unknown last login status!\n", m_nIndex, m_AcctInfo.strName.c_str(), m_AcctInfo.nId);
		SetRunLabel(25);
		goto login_over;
	}
	SetRunLabel(0);

login_over:

	DWORD dwEndCount = GetTickCount() - dwStartCount;
	AuthThreadPool::RunLast[m_nIndex] = dwEndCount;
	if (dwEndCount > AuthThreadPool::RunConsume[m_nIndex])
	{
		AuthThreadPool::RunConsume[m_nIndex] = dwEndCount;
	}
	tmpLogin.Remove(m_AcctInfo.strName);
	return wpkt;
}

#if 0
WPacket AuthThread::AccountLogin(DataSocket* datasock)
{
	DWORD dwStart = 0, dwEnd = 0;
	dwStart = GetTickCount();

	//////////////////////////////////////////////////////////////////////////
	SetRunLabel(14);
	WPacket wpkt = datasock->GetWPacket();
	GroupServer2* pGs = (GroupServer2*)datasock->GetPointer();
	if (pGs == NULL)
	{
		LG("AuthExcp", "pGs = NULL\n");
		wpkt.WriteShort(ERR_AP_DISCONN);
		return wpkt; // ��GroupServer�Ѷϵ�
	}
	SetRunLabel(15);

	wpkt.WriteCmd(0);	//added by andor.zhang,���⻺������ԭ��GroupServer����CMD_AP_KICKUSER������
	if (m_AcctInfo.bExist)
	{
		KCHAPs ChapSvr;
		bool bVerify = false;
		ChapSvr.NewSessionKey();
		if (g_TomService.IsEnable())	//ʹ��Tom�ķ�����֤
		{
			//bVerify=g_TomService.VerifyLoginMember(m_AcctInfo.strName.c_str(), m_AcctInfo.pDat);
			//CTomService::eState eUserState=g_TomService.CheckMemberState(m_AcctInfo.strName.c_str());
			//if (eUserState==CTomService::eState_AllowLogin)				//�ɹ���½
			//{
			//}
			//else if (eUserState==CTomService::eState_LoginLocked)		//�ظ���½
			//{
			//}
		}
		else							//ʹ���������֤��ʽ
		{
			bVerify = ChapSvr.DoAuth(m_AcctInfo.strName.c_str(), m_AcctInfo.strChapString.c_str(), m_AcctInfo.pDat, m_AcctInfo.usDatLen, m_AcctInfo.strPwdDigest.c_str());
			ChapSvr.NewSessionKey();
			if (bVerify)
			{
				// ��֤�ɹ�������Sid
				LG("AccountAuth", "Thread#%d Auth [%s] successfully\n", m_nIndex, m_AcctInfo.strName.c_str());
				int nSid = GenSid(m_AcctInfo.strName.c_str());
				int nKeyLen = 0;
				char szKey[1024] = { 0 };

				// ����¼�߼��ж�
				char szSql[2048] = { 0 };
				char szFmt[2048] = "update l set l.login_status=case l.login_status when %d then %d \
								   when %d then %d when %d then %d end, l.enable_login_time=case l.login_status when %d then \
								   dateadd(second, %d, getdate()) when %d then dateadd(second, %d, getdate()) when %d then \
								   dateadd(second, %d, getdate()) end, l.sid=case l.login_status when %d then %d when %d then %d \
								   when %d then %d end, l.login_group=case l.login_status when %d then '%s' when %d then l.login_group \
								   when %d then '%s' end from account_login l where l.id=%d and (l.login_status=%d or (l.login_status=%d \
								   and l.enable_login_time<=getdate()) or (l.login_status=%d and l.enable_login_time<=getdate()))";
				sprintf(szSql, szFmt,
					ACCOUNT_OFFLINE, ACCOUNT_ONLINE, // ���߱�����
					ACCOUNT_ONLINE, ACCOUNT_SAVING, // ���߱����
					//ACCOUNT_SAVING, ACCOUNT_ONLINE, // ���̱�����    2006-2-24 arcol_test
					ACCOUNT_SAVING, ACCOUNT_SAVING, // ���̱����    2006-2-24 arcol_test �滻

					ACCOUNT_OFFLINE, RELOGIN_TIME,
					ACCOUNT_ONLINE, SAVING_TIME,
					//ACCOUNT_SAVING, RELOGIN_TIME,    2006-2-24 arcol_test
					ACCOUNT_SAVING, SAVING_TIME,	// 2006-2-24 arcol_test �滻

					ACCOUNT_OFFLINE, nSid,
					ACCOUNT_ONLINE, INVALID_SID,
					ACCOUNT_SAVING, nSid,

					ACCOUNT_OFFLINE, pGs->GetName(),
					ACCOUNT_ONLINE,
					ACCOUNT_SAVING, pGs->GetName(),

					m_AcctInfo.nId,

					ACCOUNT_OFFLINE,
					ACCOUNT_ONLINE,
					ACCOUNT_SAVING);

				/*-------���Ͼ�Arcol����Ϊ��������
				update l set l.login_status=case l.login_status
										when ACCOUNT_OFFLINE then ACCOUNT_ONLINE
										when ACCOUNT_ONLINE then ACCOUNT_SAVING
										when ACCOUNT_SAVING then ACCOUNT_ONLINE end,
							 l.enable_login_time=case l.login_status
										when ACCOUNT_OFFLINE then dateadd(second, RELOGIN_TIME, getdate())
										when ACCOUNT_ONLINE then dateadd(second, SAVING_TIME, getdate())
										when ACCOUNT_SAVING then dateadd(second, RELOGIN_TIME, getdate()) end,
							 l.sid=case l.login_status
										when ACCOUNT_OFFLINE then nSid
										when ACCOUNT_ONLINE then INVALID_SID
										when ACCOUNT_SAVING then nSid end,
							 l.login_group=case l.login_status
										when ACCOUNT_OFFLINE then 'pGs->GetName()'
										when ACCOUNT_ONLINE then l.login_group
										when ACCOUNT_SAVING then 'pGs->GetName()' end from account_login l
				where l.id=m_AcctInfo.nId
				and
				(l.login_status=ACCOUNT_OFFLINE or
				(l.login_status=ACCOUNT_ONLINE and l.enable_login_time<=getdate()) or
				(l.login_status=ACCOUNT_SAVING and l.enable_login_time<=getdate())
				)";
				*/
				SetRunLabel(16);
				m_pAuth->ExecuteSQL(szSql);
				SetRunLabel(17);

				// �õ��µ��˺�״̬
				int nLinesAffected = 0;
				bool bIsFromSameGroup = pGs->IsSame(m_AcctInfo.strGroup);	//Arcol 06-2-14 ����֮ǰ�汾��BUG������ʹ�õ��ж��Ѿ���Ч����ǰ���ж�
				string strLastLoginGroup = m_AcctInfo.strGroup;				//Arcol 06-2-14 ����֮ǰ�汾��BUG������ʹ�õ��ж��Ѿ���Ч����ǰ���ж�

				{
					CSQLRecordset rs(*m_pAuth);
					sprintf(szSql, "select @@rowcount, login_status, login_group from account_login where id=%d",
						m_AcctInfo.nId);
					rs.SQLExecDirect(szSql);
					if (rs.SQLFetch())
					{
						int n = 1;
						nLinesAffected = rs.nSQLGetData(n++);
						m_AcctInfo.nStatus = rs.nSQLGetData(n++);
						m_AcctInfo.strGroup = rs.SQLGetData(n++);
					}
					else
					{
						LG("LoginAccountWarning", "SQLFetch no data on Account [%s]\n",
							m_AcctInfo.strName.c_str());
					}
				}

				// ����״̬��֯���ݰ�
				if (nLinesAffected == 1)
				{
					if (m_AcctInfo.nStatus == ACCOUNT_ONLINE)
					{
						// �ʺųɹ���¼
						sprintf(szSql, "update account_login set last_login_time=getdate(), last_login_mac = '%s' where id=%d",
							m_AcctInfo.strMAC.c_str(), m_AcctInfo.nId);
						m_pAuth->ExecuteSQL(szSql);

						wpkt.WriteShort(ERR_SUCCESS);
						wpkt.WriteLong(m_AcctInfo.nId);
						ChapSvr.GetSessionEncKey(szKey, sizeof szKey, nKeyLen);
						wpkt.WriteSequence(szKey, nKeyLen);
						ChapSvr.GetSessionClrKey(szKey, sizeof szKey, nKeyLen);
						wpkt.WriteSequence(szKey, nKeyLen);
						wpkt.WriteLong(nSid);
					}
					else if (m_AcctInfo.nStatus == ACCOUNT_SAVING)
					{
						// �ʺ��ظ���¼
						sprintf(szSql, "update account_login set last_logout_time=getdate(), total_live_time=total_live_time+datediff(second, last_login_time, getdate()) where id=%d",
							m_AcctInfo.nId);
						m_pAuth->ExecuteSQL(szSql);

						//if (pGs->IsSame(m_AcctInfo.strGroup))					//Arcol 06-2-14 ����֮ǰ�汾��BUG������ʹ�õ��ж��Ѿ���Ч����Ҫ��ǰ���ж�
						if (bIsFromSameGroup)
						{
							wpkt.WriteCmd(CMD_AP_RELOGIN);
							//wpkt.WriteShort(ERR_AP_ONLINE);					//��ģʽ
							//wpkt.WriteLong(m_AcctInfo.nId);					//��ģʽ
						}
						else
						{
							// �ߵ���ǰ��¼�ġ����Բ�ͬGroupServer�Ŀͻ���
							//KickAccount(m_AcctInfo.strGroup, m_AcctInfo.nId);	//Arcol 06-2-14 ����֮ǰ�汾��BUG
							KickAccount(strLastLoginGroup, m_AcctInfo.nId);
						}

						/*��ģʽ*/
						sprintf(szSql, "update account_login set last_login_time=getdate(), last_login_mac = '%s' where id=%d",
							m_AcctInfo.strMAC.c_str(), m_AcctInfo.nId);
						m_pAuth->ExecuteSQL(szSql);

						wpkt.WriteShort(ERR_SUCCESS);
						wpkt.WriteLong(m_AcctInfo.nId);
						ChapSvr.GetSessionEncKey(szKey, sizeof szKey, nKeyLen);
						wpkt.WriteSequence(szKey, nKeyLen);
						ChapSvr.GetSessionClrKey(szKey, sizeof szKey, nKeyLen);
						wpkt.WriteSequence(szKey, nKeyLen);
						wpkt.WriteLong(nSid);
						/*��ģʽ*/
					}
					else
					{
						wpkt.WriteShort(ERR_AP_UNKNOWN);
						LG("LoginAccountWarning", "Wrong Account Status [%d]\n", m_AcctInfo.nStatus);
					}
				}
				else
				{
					// �ʺ��Դ���ACCOUNT_SAVING״̬��RELOGIN����״̬
					wpkt.WriteShort(ERR_AP_SAVING);
				}
			}
		}
		if (!bVerify)
		{	// ��֤ʧ��
			wpkt.WriteShort(ERR_AP_INVALIDPWD);
			LG("AccountAuth", "Thread#%d Auth [%s] failed\n", m_nIndex, m_AcctInfo.strName.c_str());
		}
	}
	else
	{
		// �����ڴ��û�
		wpkt.WriteShort(ERR_AP_INVALIDUSER);
	}

	SetRunLabel(0);

	//////////////////////////////////////////////////////////////////////////
	dwEnd = GetTickCount() - dwStart;
	AuthThreadPool::RunLast[m_nIndex] = dwEndCount;
	if (dwEnd > AuthThreadPool::RunConsume[m_nIndex])
		AuthThreadPool::RunConsume[m_nIndex] = dwEnd;

	return wpkt;
}

void AuthThread::AccountLogout(RPacket rpkt)
{
	char szSql[512] = { 0 };
	int nId = rpkt.ReadLong();
	int nSid = rpkt.ReadLong();

	CSQLUpdate s("account_login");
	sprintf(szSql, "(id=%d and sid=%d and login_status=%d)", nId, nSid, ACCOUNT_ONLINE);    //2006-2-24 arcol
	s.SetWhere(szSql);
	s.SetColumn("login_status", ACCOUNT_SAVING);
	s.SetColumn("sid", INVALID_SID);
	sprintf(szSql, "enable_login_time=dateadd(second, %d, getdate())", SAVING_TIME);
	s.SetColumn(szSql);
	sprintf(szSql, "last_logout_time=getdate()");
	s.SetColumn(szSql);
	sprintf(szSql, "total_live_time=total_live_time+datediff(second, last_login_time, getdate())");
	s.SetColumn(szSql);

	SetRunLabel(21);
	m_pAuth->ExecuteSQL(s.GetStatement());
	SetRunLabel(0);
}
#endif

void AuthThread::AccountLogout(RPacket rpkt)
{
	SetRunLabel(99);
	char lpszSQL[2048] = { 0 };
	int nID = rpkt.ReadLong();
	//sprintf(lpszSQL, "update account_login set login_status=%d, login_group='', enable_login_time=getdate(), last_logout_time=getdate() where id=%d", ACCOUNT_OFFLINE, nID);
	sprintf(lpszSQL, "update account_login set login_status=%d, login_group='', enable_login_time=getdate(), last_logout_time=getdate(), total_live_time=total_live_time+datediff(second, last_login_time, getdate()) where id=%d", ACCOUNT_OFFLINE, nID);  //  ��������ʱ�� by jampe
	if (m_pAuth->ExecuteSQL(lpszSQL))
	{
		SetRunLabel(0);
	}
	LogUserLogout(nID);
}

void AuthThread::BeginBilling(RPacket& rpkt)
{
	//g_BillService.PlayerLogin(rpkt);
}

void AuthThread::EndBilling(RPacket& rpkt)
{
	//g_BillService.PlayerLogout(rpkt);
}

void AuthThread::ResetBilling()
{
	//g_BillService.AllPlayerReset();
}

void AuthThread::LogoutGroup(DataSocket* datasock, RPacket rpkt)
{
	/*
	2005-4-14 added by arcol:
	·þÎñÆ÷¶Ï¿ªºó»áÁ¢¼´×Ô¶¯ÖØÁ¬,Òò´Ë²»Çå³ýÕÊºÅ×´Ì¬,ÒªÇóGroupServerÖØÆôºóAccountServerÒ²ÐèÒªÖØÆô
	*/

	/*
		char szSql[512] = {0};
		std::string strName;
		char const* pszGroup = rpkt.ReadString();
		LG("As2Logout", "GroupServer: [%s] ¶Ï¿ª£¬Çå³ý´ÓÆäµÇÂ¼ÉÏµÄËùÓÐÕÊºÅµÄµÇÂ¼×´Ì¬\n", pszGroup);

		SetRunLabel(23);
		try {
			CSQLRecordset rs(*m_pAuth);
			sprintf(szSql, "select name from account_login where (login_group='%s' and login_status=%d)",
				pszGroup, ACCOUNT_ONLINE);
			rs << szSql;
			rs.SQLExecDirect();
			while (rs.SQLFetch()) {
				strName = rs.SQLGetData(1);
				WPacket wpkt = datasock->GetWPacket();
				wpkt.WriteCmd(CMD_PA_USER_BILLEND);
				wpkt.WriteString(strName.c_str());
				//g_Bill.AddPK(datasock, (RPacket)wpkt);
				//g_BillThread.AddPK(datasock, (RPacket)wpkt);

			}
		} catch (CSQLException* se) {
			LG("LogoutGroup", "Select SQL Exception: %s\n", se->m_strError.c_str());
			delete se;
		}
	*/


	/*
		SetRunLabel(24);
		try {
			CSQLUpdate s("account_login");
			sprintf(szSql, "(login_group='%s' and login_status=%d)", pszGroup, ACCOUNT_ONLINE);
			s.SetWhere(szSql);
			//s.SetColumn("login_status", ACCOUNT_SAVING);	//2006-2-24 Arcol_test
			s.SetColumn("login_status", ACCOUNT_OFFLINE);	//2006-2-24 Arcol_test
			s.SetColumn("sid", INVALID_SID);
			sprintf(szSql, "enable_login_time=dateadd(second, %d, getdate())", SAVING_TIME);
			s.SetColumn(szSql);
			sprintf(szSql, "last_logout_time=getdate()");
			s.SetColumn(szSql);
			sprintf(szSql, "total_live_time=total_live_time+datediff(second, last_login_time, getdate())");
			s.SetColumn(szSql);

			SetRunLabel(25);
			m_pAuth->ExecuteSQL(s.GetStatement());
		} catch (CSQLException* se) {
			LG("LogoutGroup", "Update SQL Exception: %s\n", se->m_strError.c_str());
			delete se;
		}
	*/

	SetRunLabel(0);
}

CSQLDatabase* AuthThread::GetSQLDatabase()
{
	return m_pAuth;
}

long AuthThread::GenSid(char const* szName)
{
#define SHA1_DIGEST_LEN 20
	char md[SHA1_DIGEST_LEN];

	// ²úÉúÐÅÏ¢Ô´
	char buf[256];
	int buf_len = sprintf(buf, "%s%d", szName, GetTickCount());
	if (buf_len >= sizeof buf)
		throw std::out_of_range("buffer overflow in GenSid()\n");

	// Éú³ÉÕªÒª
	sha1((unsigned char*)buf, buf_len, (unsigned char*)md);

	// È¡³öÇ°4¸ö×Ö½Ú
	long* ptr = (long*)md;
	return ptr[0];
}
void AuthThread::ResetAccount()
{
	/*
	2005-4-17 added by Arcol:
	ÕâÀï´úÂë¿´µÃÎÒÍ·ÔÎ,ÈçÓÐÄÜÁ¦½¨ÒéÖØÐ´,Ä¿Ç°Íâ²¿Ö÷Ïß³ÌÓÐCDataBaseCtrl¶ÔÏó¿ÉÒÔ³õÊ¼»¯¼°ºóÆÚ»¯Êý¾Ý¿â
	×¢Òâ:ÕâÀïµÄm_nIndexÎª0Ê±²»´ú±íÒ»¶¨ÊÇÊ×´ÎÖ´ÐÐ,Òò´Ë,¿ÉÄÜ³öÏÖÒ»ÖÖÇé¿ö,µ±ÆäËüÏß³ÌÒÑ¾­¿ªÕ¹ÑéÖ¤·þÎñ¹¤×÷,¶ø´ËÊ±0ºÅË÷ÒýÏß³Ì²Å½øÀ´³õÊ¼»¯±í¸ñ»áÔì³É±í¸ñ×ÊÔ´·ÃÎÊ³åÍ»,µ¼ÖÂ³õÊ¼»¯Ê§°Ü
	*/
	//return;	//·ÅÆúÊ¹ÓÃºóÃæµÄ´úÂë 2005-4-17

	if (m_nIndex == 0) {
		// Ö»ÓÐµÚÒ»¸öÏß³Ì±»ÔÊÐíÐÞ¸Ä
		CSQLUpdate s("account_login");
		s.SetColumn("login_status", ACCOUNT_OFFLINE);
		s.SetColumn("sid", INVALID_SID);
		s.SetColumn("enable_login_time=getdate()");
		s.SetColumn("login_group", "");

		try {
            m_pAuth->ExecuteSQL(s.GetStatement());
	        //LG("As2", "AuthThread#%dÁ¬½ÓAuthÊý¾Ý¿â³É¹¦\n", m_nIndex);
	        LG("As2", RES_STRING(AS_ACCOUNTSERVER2_CPP_00005), m_nIndex);
		} catch (CSQLException* pEx) {
			LG("AuthDBExcp", "AuthThread::ResetAccount() ExecuteSQL failed : %s\n",
				pEx->m_strError.c_str());
		} catch (...) {
			LG("AuthDBExcp", "Unknown exception raised from AuthThread::ResetAccount()\n");
		}

        // ¼¤»îÒ»¸öÏß³Ì
        AuthThread::m_Sema.unlock();
    } else {
        // ÆäËüÏß³Ì²»ÐÞ¸Ä£¬²¢µÈ´ýÐÞ¸ÄÍê³É
        AuthThread::m_Sema.lock();
        //LG("As2", "AuthThread#%dÁ¬½ÓAuthÊý¾Ý¿â³É¹¦\n", m_nIndex);
        LG("As2", RES_STRING(AS_ACCOUNTSERVER2_CPP_00005), m_nIndex);

		// ÔÙ¼¤»îÒ»¸öÏß³Ì
		AuthThread::m_Sema.unlock();
	}
}
void AuthThread::KickAccount(std::string& strGroup, int nId)
{
	GroupServer2* pGs = g_As2->FindGroup(strGroup.c_str());
	if (pGs != NULL) {
		WPacket wpkt = pGs->GetWPacket();
		wpkt.WriteCmd(CMD_AP_KICKUSER);
		wpkt.WriteShort(ERR_AP_LOGINTWICE);
		wpkt.WriteLong(nId);
		pGs->SendData(wpkt);
	}
}
int AuthThread::Run()
{
	Init();
	while (!GetExitFlag()) {
		g_Auth.PeekPacket(1000); // ¸øÓÚ1ÃëµÄÊ±¼äÀ´²É¼¯¶ÓÁÐÖÐµÄÍøÂç°ü
	}
	Exit();

	ExitThread();
	return 0;
}
void AuthThread::SetRunLabel(int nRunLabel)
{
	AuthThreadPool::RunLabel[m_nIndex] = nRunLabel;
}

//
int volatile AuthThreadPool::RunLabel[AT_MAXNUM] = { 0 };
DWORD volatile AuthThreadPool::RunLast[AT_MAXNUM] = { 0 };
DWORD volatile AuthThreadPool::RunConsume[AT_MAXNUM] = { 0 };
unsigned int volatile AuthThreadPool::uiAuthCount = 0;
void AuthThreadPool::IncAuthCount() { ++uiAuthCount; }
unsigned int AuthThreadPool::GetAuthCount() { return uiAuthCount; }
AuthThreadPool::AuthThreadPool()
{
	for (int i = 0; i < AT_MAXNUM; ++i) {
		m_Pool[i] = new AuthThread(i);
	}
	AuthThread::LoadConfig();
}
AuthThreadPool::~AuthThreadPool()
{
	for (int i = 0; i < AT_MAXNUM; ++i) {
		if (m_Pool[i] != NULL) {
			delete m_Pool[i];
			m_Pool[i] = NULL;
		}
	}
}
void AuthThreadPool::Launch()
{
	for (int i = 0; i < AT_MAXNUM; ++i) {
		m_Pool[i]->Launch();
	}
}
void AuthThreadPool::NotifyToExit()
{
	for (int i = 0; i < AT_MAXNUM; ++i) {
		m_Pool[i]->NotifyToExit();
	}
}
void AuthThreadPool::WaitForExit()
{
	//printf("ÕýÔÚµÈ´ýÏß³Ì³ØÄÚµÄ×ÓÏß³ÌÍË³ö£¬ÇëÉÔºò\n");
	printf(RES_STRING(AS_ACCOUNTSERVER2_CPP_00006));
	for (int i = 0; i < AT_MAXNUM; ++i) {
		m_Pool[i]->WaitForExit(-1);
	}
}

int isValidMacAddress(const char* mac) {
	int i = 0;
	int s = 0;
	while (*mac) {
		if (isxdigit(*mac)) {
			i++;
		}
		else if (*mac == ':' || *mac == '-') {

			if (i == 0 || i / 2 - 1 != s)
				break;

			++s;
		}
		else {
			s = -1;
		}
		++mac;
	}
	return (i == 12 && (s == 5 || s == 0));
}

// ÍøÂç¿ò¼Ü¶ÔÏó
AccountServer2* g_As2 = NULL;
