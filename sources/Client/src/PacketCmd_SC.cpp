
#include "StdAfx.h"
#include "Character.h"
#include "Scene.h"
#include "GameApp.h"
#include "actor.h"
#include "NetProtocol.h"
#include "PacketCmd.h"
#include "GameAppMsg.h"
#include "CharacterRecord.h"
#include "DrawPointList.h"
#include "Algo.h"
#include "NetChat.h"
#include "NetGuild.h"
#include "CommFunc.h"
#include "ShipSet.h"
#include "GameConfig.h"
#include "GameWG.h"
#include "UIEquipForm.h"
#include "UIDoublePwdForm.h"
#include "uisystemform.h"
#include "uiStoreForm.h"
#include "uiBourseForm.h"
#include "uipksilverform.h"
#include "uicomposeform.h"
#include "UIBreakForm.h"
#include "UIFoundForm.h"
#include "UICookingForm.h"
#include "UISpiritForm.h"
#include "UIFindTeamForm.h"
#include "UINpcTradeForm.h"
#include "UIChat.h"
#include "UIMailForm.h"
#include "UIMiniMapForm.h"
#include "UINumAnswer.h"
#include "UIChurchChallenge.h"

#include "uistartform.h"
#include "UIGuildMgr.h"
#include "AreaRecord.h"
#include "UIGuildBankForm.h"
#include "UIGlobalVar.h"
#include "MapSet.h"



#ifdef _TEST_CLIENT
#include "..\..\TestClient\testclient.h"
#endif


#include "SceneObj.h"
#include "Scene.h"

// Типы uChar, uShort, uLong, cChar определены в NetIF.h


//--------------------------------------------------
// ����Э�鴦���淶
// ������������ 
//      PacketCmd_DoSomething(LPPacket pk)
//
// ������������
//      ��ɫ CCharacter *pCha 
//      ���� CCharacter* pMainCha
//      ��� CSceneItem* pItem
//      ���� CGameScene* pScene
//      APP  CGameApp*   g_pGameApp
//--------------------------------------------------
extern char g_szSendKey[4];
extern char g_szRecvKey[4];

static unsigned long g_ulWorldID = 0;

using namespace std;

BOOL SC_UpdateGuildGold(LPRPACKET pk){
	g_stUIGuildBank.UpdateGuildGold(pk.ReadString());
	return true;
}

BOOL SC_ShowStallSearch(LPRPACKET pk){
	uChar l_num		=pk.ReverseReadUInt16();
	NetMC_LISTGUILD_BEGIN();
	for(uChar i =0;i<l_num;++i)
	{
		//char buf[8];
		//sprintf(buf,"%03d>",i+1);
		uLong	 l_id			=i+1;
		cChar	*l_name			=pk.ReadString();//player name
		cChar	*l_motto		=pk.ReadString();//stall name
		cChar	*l_leadername	=pk.ReadString();//location
		uShort	 l_memtotal		=pk.ReadUInt32(); //count remaining
		__int64	 l_exp	=pk.ReadUInt32(); //cost each
		NetMC_LISTGUILD(l_id,l_name,l_motto,l_leadername,l_memtotal,l_exp);
	}
	NetMC_LISTGUILD_END();
	return TRUE;
}

BOOL SC_ShowRanking(LPRPACKET pk){
	uChar l_num		=pk.ReverseReadUInt16();
	NetMC_LISTGUILD_BEGIN();
	for(uChar i =0;i<l_num;++i)
	{
		char buf[8];
		sprintf(buf,"%03d>",i+1);
		uLong	 l_id			=i+1;
		cChar	*l_name			=buf;//rank
		cChar	*l_motto		=pk.ReadString();//name
		cChar	*l_leadername	=pk.ReadString();//job
		uShort	 l_memtotal		=pk.ReadUInt16(); //level
		uLong	 l_explow		=0;
		uLong	 l_exphigh		=0;
		__int64	 l_exp	=l_exphigh *0x100000000 +l_explow;
		NetMC_LISTGUILD(l_id,l_name,l_motto,l_leadername,l_memtotal,l_exp);
	}
	NetMC_LISTGUILD_END();
	return TRUE;
}

// CryptImportPublicKeyInfoEx2 доступна в crypt32.dll (Vista+),
// но объявлена в wincrypt.h только при _WIN32_WINNT >= 0x0600.
// Проект использует 0x0500 — объявляем вручную.
typedef BOOL (WINAPI *PFN_CryptImportPublicKeyInfoEx2)(
	DWORD dwCertEncodingType,
	PCERT_PUBLIC_KEY_INFO pInfo,
	DWORD dwFlags,
	void* pvAuxInfo,
	BCRYPT_KEY_HANDLE* phKey
);

BOOL	SC_SendPublicKey(LPRPACKET pk)
{
	// Читаем SPKI DER публичный ключ из пакета (WriteSequence = [uint16 len][data])
	uShort keySize = 0;
	const char* keyData = pk.ReadSequence(keySize);
	if (!keyData || keySize == 0) {
		LG("enc", "SC_SendPublicKey: empty key data\n");
		return FALSE;
	}

	// Выводим полученный SPKI DER ключ в hex
	{
		std::string hex;
		hex.reserve(keySize * 2);
		for (uShort i = 0; i < keySize; i++) {
			char buf[3];
			sprintf(buf, "%02x", static_cast<unsigned char>(keyData[i]));
			hex += buf;
		}
		LG("enc", "SC_SendPublicKey: received SPKI DER key (%u bytes):\n%s\n", keySize, hex.c_str());
	}

	// Декодируем SPKI DER → CERT_PUBLIC_KEY_INFO
	CERT_PUBLIC_KEY_INFO* pubKeyInfo = NULL;
	DWORD pubKeyInfoSize = 0;
	if (!CryptDecodeObjectEx(
		X509_ASN_ENCODING,
		X509_PUBLIC_KEY_INFO,
		reinterpret_cast<const BYTE*>(keyData), keySize,
		CRYPT_DECODE_ALLOC_FLAG, NULL,
		&pubKeyInfo, &pubKeyInfoSize))
	{
		LG("enc", "CryptDecodeObjectEx failed: %u\n", GetLastError());
		return FALSE;
	}

	// Загружаем CryptImportPublicKeyInfoEx2 динамически
	static PFN_CryptImportPublicKeyInfoEx2 pfnImport = NULL;
	if (!pfnImport) {
		HMODULE hCrypt32 = GetModuleHandleA("crypt32.dll");
		if (hCrypt32)
			pfnImport = (PFN_CryptImportPublicKeyInfoEx2)GetProcAddress(hCrypt32, "CryptImportPublicKeyInfoEx2");
	}
	if (!pfnImport) {
		LG("enc", "CryptImportPublicKeyInfoEx2 not available\n");
		LocalFree(pubKeyInfo);
		return FALSE;
	}

	// Импортируем в BCrypt RSA handle
	if (g_NetIF->hRsaPubKey) {
		BCryptDestroyKey(g_NetIF->hRsaPubKey);
		g_NetIF->hRsaPubKey = NULL;
	}

	BOOL result = pfnImport(
		X509_ASN_ENCODING,
		pubKeyInfo,
		0, NULL,
		&g_NetIF->hRsaPubKey);

	LocalFree(pubKeyInfo);

	if (!result) {
		LG("enc", "CryptImportPublicKeyInfoEx2 failed: %u\n", GetLastError());
		return FALSE;
	}

	g_NetIF->m_connect.CHAPSTR();
	CS_SendPrivateKey();

	return TRUE;
}


BOOL	SC_SendHandshake(LPRPACKET pk) {
	/**/
	return true;
}




BOOL	SC_Login(LPRPACKET pk)
{T_B
	uShort l_errno	=pk.ReadUInt16();
	if(l_errno)
	{
		cChar *l_errtext=pk.ReadString();
		NetLoginFailure(l_errno);
	}else			
	{
		const auto maxCharacters = static_cast<uint8_t>(pk.ReadUInt8());
		const auto characters = ReadSelectCharacters(pk);
		const auto chPassword = pk.ReadUInt8();
		NetLoginSuccess(chPassword, maxCharacters, characters);

		extern CGameWG g_oGameWG;
		g_oGameWG.SafeTerminateThread();
		g_oGameWG.BeginThread();		
	}
	updateDiscordPresence("Selecting Character", "");
	return TRUE;
T_E}

BOOL SC_Disconnect(LPRPACKET pk)
{
	T_B
		auto reason = pk.ReadUInt32();
		g_NetIF->m_connect.Disconnect(reason);
		return true;
	T_E
}


//--------------------
// Э��S->C : ѡ���ɫ
//--------------------
BOOL SC_EnterMap(LPRPACKET pk)
{T_B
	g_LogName.Init();

	g_listguild_begin	=false;

	stNetSwitchMap SMapInfo{};
	SMapInfo.sEnterRet = pk.ReadUInt16();
	if (SMapInfo.sEnterRet != ERR_SUCCESS)
	{
		NetSwitchMap(SMapInfo);
		return FALSE;
	}

	auto const bAutoLock = static_cast<bool>(pk.ReadUInt8());
	auto const bKitbagLock = static_cast<bool>(pk.ReadUInt8());
	g_stUISystem.m_sysProp.m_gameOption.bLockMode = bAutoLock;
	g_stUIEquip.SetIsLock(bKitbagLock);

	SMapInfo.chEnterType = pk.ReadUInt8();	// �����ͼ���ͣ��μ�CompCommand.h EEnterMapType
	SMapInfo.bIsNewCha = static_cast<bool>(pk.ReadUInt8());
	SMapInfo.szMapName = pk.ReadString();
	SMapInfo.bCanTeam = pk.ReadUInt8() != 0 ? true : false;
	NetSwitchMap(SMapInfo);
	LG(g_oLangRec.GetString(295), "%s\n", SMapInfo.szMapName);
	
	
	int const IMPs = pk.ReadUInt32();
	
	g_stUIEquip.UpdateIMP(IMPs);
	
	stNetActorCreate SCreateInfo;
	ReadChaBasePacket(pk, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 1;
	CCharacter* pCha = SCreateInfo.CreateCha();
	g_ulWorldID = SCreateInfo.ulWorldID;

	const char* szLogName = g_LogName.GetLogName( SCreateInfo.ulWorldID );

	stNetSkillBag SCurSkill;
	ReadChaSkillBagPacket(pk, SCurSkill, szLogName);
	NetSynSkillBag(SCreateInfo.ulWorldID, &SCurSkill);


	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

	// ����
	stNetKitbag SKitbag;
	ReadChaKitbagPacket(pk, SKitbag, szLogName);
	SKitbag.chBagType = 0;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

#if 0
	stNetKitbag SKitbagTemp;
	ReadChaKitbagPacket(pk, SKitbagTemp, szLogName);
	SKitbagTemp.chBagType = 2;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbagTemp);
#endif

	stNetShortCut SShortcut;
	ReadChaShortcutPacket(pk, SShortcut, szLogName);
	NetShortCut(SCreateInfo.ulWorldID, SShortcut);

	// ��ֻ��Ϣ
	char	chBoatNum = pk.ReadUInt8();
	for (char i = 0; i < chBoatNum; i++)
	{
		ReadChaBasePacket(pk, SCreateInfo);
		SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
		SCreateInfo.chMainCha = 2;
		SCreateInfo.CreateCha();

		szLogName = g_LogName.GetLogName( SCreateInfo.ulWorldID );

		ReadChaAttrPacket(pk, SChaAttr, szLogName);
		NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

		ReadChaKitbagPacket(pk, SKitbag, szLogName);
		NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

		ReadChaSkillStatePacket(pk, SCurSState, szLogName);
		NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);
	}

	stNetChangeCha SChangeCha;
	SChangeCha.ulMainChaID = pk.ReadUInt32();
	NetActorChangeCha(SCreateInfo.ulWorldID, SChangeCha);
	
	// ��ʱ��������ʱע�ͣ�!!!��Ҫ����entermap��
#if 0
	stNetKitbag SKitbagTemp;
	ReadChaKitbagPacket(pk, SKitbagTemp, szLogName);
	SKitbagTemp.chBagType = 2;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbagTemp);
#endif



	// �رշ�����
	//CS_AntiIndulgence_Close();

	return TRUE;
T_E}

BOOL    SC_BeginPlay(LPRPACKET pk)
{T_B
uShort	l_errno = pk.ReadUInt16();
NetBeginPlay(l_errno);
	
	return TRUE;
T_E}

BOOL	SC_EndPlay(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadUInt16();

	//char    chPassword  =pk.ReadUInt8();
	//g_Config.m_IsDoublePwd = chPassword ? true : false;
	
	const auto maxCharacters = pk.ReadUInt8();
	const auto characters = ReadSelectCharacters(pk);
	NetEndPlay(maxCharacters, characters);

	updateDiscordPresence("Selecting Character", "");
	return TRUE;
T_E}

std::optional<NetChaBehave> ReadSelectCharacter(RPacket& rpk)
{
	if (!rpk.ReadUInt8())
	{
		return {};
	}

	uShort looklen{ 0 };
	NetChaBehave cha;
	cha.sCharName = rpk.ReadString();
	cha.sJob = rpk.ReadString();
	cha.iDegree = rpk.ReadUInt16();

	cha.look_minimal.typeID = rpk.ReadUInt16();
	for (auto& id : cha.look_minimal.equip_IDs)
	{
		id = rpk.ReadUInt16();
	}

	return cha;
}

std::vector<NetChaBehave> ReadSelectCharacters(RPacket& rpk)
{
	std::vector<NetChaBehave> characters;
	characters.reserve(rpk.ReadUInt8());
	for (auto i = 0; i < characters.capacity(); ++i)
	{
		if (auto cha = ReadSelectCharacter(rpk))
		{
			characters.emplace_back(cha.value());
		}
	}
	return characters;
}


BOOL	SC_NewCha(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadUInt16();
	NetNewCha(l_errno);

	return TRUE;
T_E}

BOOL	SC_DelCha(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadUInt16();
	NetDelCha(l_errno);
	return TRUE;
T_E}

BOOL SC_CreatePassword2(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadUInt16();
	NetCreatePassword2(l_errno);
	return TRUE;
T_E}

BOOL SC_UpdatePassword2(LPRPACKET pk)
{T_B
	uShort	l_errno	=pk.ReadUInt16();
	NetUpdatePassword2(l_errno);
	return TRUE;
T_E}

//mothannakh create account 
BOOL PC_REGISTER(LPRPACKET pk){
	CGameApp::Waiting(false);
	char sucess = pk.ReadUInt8();
	if( g_NetIF->IsConnected() ){
		CS_Disconnect(DS_DISCONN);			
	}
	//register cooldown
	_dwLastTime = CGameApp::GetCurTick();
	
	if (_dwOverTime > _dwLastTime)
	{
		g_pGameApp->MsgBox("Do Not Spam.");
		return FALSE;
	}
	//test
	if(sucess == 1){
		//mothannakh account register//
		//CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());			
		registerLogin = false;
		//pkScene->LoginFlow();
		//here end
		_dwOverTime = _dwLastTime + 9000;
		g_pGameApp->MsgBox("Account Created.");
	}else{
		g_pGameApp->MsgBox( pk.ReadString() );
	}
	return TRUE;
}

BOOL	PC_Ping(LPRPACKET pk)
{T_B
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_PING);
	g_NetIF->SendPacketMessage(l_wpk);
	return TRUE;
T_E}

BOOL	SC_Ping(LPRPACKET pk)
{T_B
	{
		auto const l = std::lock_guard{g_NetIF->m_mutmov};

		WPacket wpk = g_NetIF->GetWPacket();
		wpk.WriteCmd(CMD_CM_PING);
		wpk.WriteUInt32(pk.ReadUInt32());
		wpk.WriteUInt32(pk.ReadUInt32());
		wpk.WriteUInt32(pk.ReadUInt32());
		wpk.WriteUInt32(pk.ReadUInt32());
		wpk.WriteUInt32(pk.ReadUInt32());
		g_NetIF->SendPacketMessage(wpk);
	}

	if(g_NetIF->m_curdelay > g_NetIF->m_maxdelay) g_NetIF->m_maxdelay = g_NetIF->m_curdelay;

	if(g_NetIF->m_curdelay < g_NetIF->m_mindelay) g_NetIF->m_mindelay = g_NetIF->m_curdelay;

	return TRUE;
T_E}

BOOL	SC_CheckPing(LPRPACKET pk)
{T_B
	WPacket wpk = g_NetIF->GetWPacket();
	wpk.WriteCmd(CMD_CM_CHECK_PING);
	g_NetIF->SendPacketMessage(wpk);

	return TRUE;
T_E}

BOOL	SC_Say(LPRPACKET pk)
{T_B
	uShort	 l_len;
	stNetSay l_netsay;
	l_netsay.m_srcid	=pk.ReadUInt32();
	l_netsay.m_content	=pk.ReadSequence(l_len);
	DWORD dwColour = pk.ReadUInt32();
	NetSay(l_netsay,dwColour);

	return TRUE;
T_E}

//--------------------
// Э��S->C : ϵͳ��Ϣ��ʾ
//--------------------
BOOL	SC_SysInfo(LPRPACKET pk)
{T_B
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	uShort	l_retlen;
	stNetSysInfo l_sysinfo;
	l_sysinfo.m_sysinfo	=pk.ReadSequence(l_retlen);
	NetSysInfo(l_sysinfo);
	return TRUE;
T_E}

BOOL GuildSysInfo = false;

BOOL	SC_GuildInfo(LPRPACKET pk)
{T_B
	uShort	l_retlen;
	stNetSysInfo l_sysinfo;
	l_sysinfo.m_sysinfo	=pk.ReadSequence(l_retlen);
	GuildSysInfo = true;
	NetSysInfo(l_sysinfo);
	return TRUE;
T_E}

BOOL SC_PopupNotice(LPRPACKET pk)
{T_B
	uShort	l_retlen;
	cChar *szNotice	= pk.ReadSequence(l_retlen);
	g_pGameApp->MsgBox(szNotice);
	return TRUE;
T_E}

BOOL	SC_BickerNotice( LPRPACKET pk )
{T_B
	char szData[1024];
	strncpy( szData, pk.ReadString(), 1024 - 1 );
	NetBickerInfo( szData );
	return TRUE;
T_E}

BOOL	SC_ColourNotice( LPRPACKET pk )
{T_B
	char szData[1024];
	unsigned int rgb = pk.ReadUInt32();
	strncpy( szData, pk.ReadString(), 1024 - 1 );
	NetColourInfo( rgb, szData );
	return TRUE;
T_E}

//------------------------------------
// Э��S->C : ������ɫ(���������)����
//------------------------------------
BOOL SC_ChaBeginSee(LPRPACKET pk)
{T_B
	stNetActorCreate SCreateInfo;
	char	chSeeType = pk.ReadUInt8();

	ReadChaBasePacket(pk, SCreateInfo);
	SCreateInfo.chSeeType = chSeeType;
	SCreateInfo.chMainCha = 0;
	CCharacter *pCha = SCreateInfo.CreateCha();
	if (!pCha)	return FALSE;

	const char* szLogName = g_LogName.GetLogName( SCreateInfo.ulWorldID );

	stNetNPCShow	SNpcShow;
	// NPC����״̬��Ϣ
	SNpcShow.byNpcType	   = pk.ReadUInt8();
    SNpcShow.byNpcState     = pk.ReadUInt8();
	SNpcShow.SetNpcShow( pCha );
		

	// �ж�����
	switch (pk.ReadUInt16())
	{
	case enumPoseLean:
		{
			stNetLeanInfo SLean;
			SLean.chState = pk.ReadUInt8();
			SLean.lPose = pk.ReadUInt32();
			SLean.lAngle = pk.ReadUInt32();
			SLean.lPosX = pk.ReadUInt32();
			SLean.lPosY = pk.ReadUInt32();
			SLean.lHeight = pk.ReadUInt32();
			NetActorLean(SCreateInfo.ulWorldID, SLean);
			break;
		}
	case enumPoseSeat:
		{
			stNetFace SNetFace;
			SNetFace.sAngle = pk.ReadUInt16();
			SNetFace.sPose = pk.ReadUInt16();
			NetFace( SCreateInfo.ulWorldID, SNetFace, enumACTION_SKILL_POSE );
			break;
		}
	default:
		{
			break;
		}
	}

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	return TRUE;
T_E}

//------------------------------------
// Э��S->C : ������ɫ(���������)��ʧ
//------------------------------------
BOOL	SC_ChaEndSee(LPRPACKET pk)
{T_B
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	char	chSeeType = pk.ReadUInt8(); // �μ� CompCommand.h��EEntitySeenType
	uLong l_id	= pk.ReadUInt32();
	NetActorDestroy( l_id, chSeeType );
	if (g_stUIStart.targetInfoID == l_id){
		g_stUIStart.RemoveTarget();
	}
	// log
	LG(g_LogName.GetLogName( l_id ), "+++++++++++++Destroy\n");
	//
	return TRUE;
T_E}

BOOL	SC_ItemCreate(LPRPACKET pk)
{T_B
	stNetItemCreate SCreateInfo;
	memset(&SCreateInfo, 0, sizeof(SCreateInfo));
	SCreateInfo.lWorldID = pk.ReadUInt32();	// world ID
	SCreateInfo.lHandle = pk.ReadUInt32();
	SCreateInfo.lID = pk.ReadUInt32();		// ID
	SCreateInfo.SPos.x = pk.ReadUInt32();		// ��ǰxλ��
	SCreateInfo.SPos.y = pk.ReadUInt32();		// ��ǰyλ��
	SCreateInfo.sAngle = pk.ReadUInt16();	// ����
	SCreateInfo.sNum = pk.ReadUInt16();	// ����
	SCreateInfo.chAppeType = pk.ReadUInt8();	// ����ԭ�򣨲μ�CompCommand.h EItemAppearType��
	SCreateInfo.lFromID = pk.ReadUInt32();	// �׳�Դ��ID

	ReadEntEventPacket(pk, SCreateInfo.SEvent);

	CSceneItem	*CItem = NetCreateItem(SCreateInfo);
	if (!CItem)
		return FALSE;

	// log
	LG("SC_Item", "CreateType = %d, WorldID:%u, ItemID = %d, Pos = [%d,%d], SrcID = %u, \n", SCreateInfo.chAppeType, SCreateInfo.lWorldID, SCreateInfo.lID, SCreateInfo.SPos.x, SCreateInfo.SPos.y, SCreateInfo.lFromID);
	//
	return TRUE;
T_E}

BOOL	SC_ItemDestroy(LPRPACKET pk)
{T_B
	unsigned long lID = pk.ReadUInt32();				//ID

	NetItemDisappear(lID);
	LG("SC_Item", "Item Destroy[%u]\n", lID);
	return TRUE;
T_E}

BOOL SC_AStateBeginSee(LPRPACKET pk)
{T_B
	stNetAreaState	SynAState;

	char	chValidNum = 0;
	SynAState.sAreaX = pk.ReadUInt16();
	SynAState.sAreaY = pk.ReadUInt16();
	SynAState.chStateNum = pk.ReadUInt8();
	for (char j = 0; j < SynAState.chStateNum; j++)
	{
		SynAState.State[chValidNum].chID = pk.ReadUInt8();
		if (SynAState.State[chValidNum].chID == 0)
			continue;
		SynAState.State[chValidNum].chLv = pk.ReadUInt8();
		SynAState.State[chValidNum].lWorldID = pk.ReadUInt32();
		SynAState.State[chValidNum].uchFightID = pk.ReadUInt8();
		chValidNum++;
	}
	SynAState.chStateNum = chValidNum;

	NetAreaStateBeginSee(&SynAState);

	// log
	const char* szLogName = g_LogName.GetMainLogName();

	LG(szLogName, g_oLangRec.GetString(296), SynAState.sAreaX, SynAState.sAreaY, SynAState.chStateNum);
	for (char j = 0; j < SynAState.chStateNum; j++)
		LG(szLogName, "\t%d\t%d\n", SynAState.State[j].chID, SynAState.State[j].chLv);
	LG(szLogName, "\n");
	//

	return TRUE;
T_E}

BOOL SC_AStateEndSee(LPRPACKET pk)
{T_B
	stNetAreaState	SynAState;

	SynAState.sAreaX = pk.ReadUInt16();
	SynAState.sAreaY = pk.ReadUInt16();

	NetAreaStateEndSee(&SynAState);

	// log
	LG(g_LogName.GetMainLogName(), g_oLangRec.GetString(296), SynAState.sAreaX, SynAState.sAreaY, 0);
	//

	return TRUE;
T_E}

// Э��S->C : ���ӵ��߽�ɫ
BOOL SC_AddItemCha(LPRPACKET pk)
{T_B
	long	lMainChaID = pk.ReadUInt32();
	stNetActorCreate SCreateInfo;
	ReadChaBasePacket(pk, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 2;
	SCreateInfo.CreateCha();

	const char *szLogName = g_LogName.GetLogName( SCreateInfo.ulWorldID );

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

	stNetKitbag SKitbag;
	ReadChaKitbagPacket(pk, SKitbag, szLogName);
	SKitbag.chBagType = 0;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	return TRUE;
T_E}

// Э��S->C : ɾ�����߽�ɫ
BOOL SC_DelItemCha(LPRPACKET pk)
{T_B
	long	lMainChaID = pk.ReadUInt32();

	char	chSeeType = enumENTITY_SEEN_NEW;
	uLong l_id	= pk.ReadUInt32();
	NetActorDestroy( l_id, chSeeType );

	return TRUE;
T_E}

// �������
BOOL SC_Cha_Emotion(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();
	uShort sEmotion = pk.ReadUInt16();

	NetChaEmotion( l_id, sEmotion );
	LG( g_LogName.GetLogName( l_id ), g_oLangRec.GetString(297), sEmotion );
	return TRUE;
T_E}

// Э��S->C : ��ɫ�ж�ͨ��
BOOL SC_CharacterAction(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();

	const char* szLogName = g_LogName.GetLogName( l_id );

	// try
	{
		long lPacketId = 0;
#ifdef defPROTOCOL_HAVE_PACKETID
		lPacketId = pk.ReadUInt32();
#endif
		LG(szLogName, "$$$PacketID:\t%u\n", lPacketId);

		switch(pk.ReadUInt8())
		{
		case enumACTION_MOVE:
			{
				stNetNotiMove SMoveInfo;
				SMoveInfo.sState = pk.ReadUInt16();
				if (SMoveInfo.sState != enumMSTATE_ON)
					SMoveInfo.sStopState = pk.ReadUInt16();
				Point *STurnPos;
				uShort ulTurnNum;
				STurnPos = (Point *)pk.ReadSequence(ulTurnNum);
				SMoveInfo.nPointNum = ulTurnNum / sizeof(Point);
				memcpy(SMoveInfo.SPos, STurnPos, ulTurnNum);

				// log
				long lDistX, lDistY, lDist = 0;
				LG(szLogName, "===Recieve(Move):\tTick:[%u]\n", GetTickCount());
				LG(szLogName, "Point:\t%3d\n", SMoveInfo.nPointNum);
				bool isMainCha = g_LogName.IsMainCha( l_id );
				for (int i = 0; i < SMoveInfo.nPointNum; i++)
				{
#ifdef _STATE_DEBUG
					if( isMainCha )	
					{
						



						g_pGameApp->GetDrawPoints()->Add( SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, 0xffff0000, 0.5f );
						if (SMoveInfo.sState && i==SMoveInfo.nPointNum-1)
						{
							g_pGameApp->GetDrawPoints()->Add( SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, 0xff000000, 0.3f );
						}
					}
#endif

					if (i > 0)
					{
						lDistX = SMoveInfo.SPos[i].x - SMoveInfo.SPos[i - 1].x;
						lDistY = SMoveInfo.SPos[i].y - SMoveInfo.SPos[i - 1].y;
						lDist = (long)sqrt((double)lDistX * lDistX + lDistY * lDistY);
					}
					LG(szLogName, "\t%d, %d\t%d\n", SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, lDist);
				}
				if (SMoveInfo.sState)
					LG(szLogName, "@@@End Move\tState:0x%x\n", SMoveInfo.sState);



				if (SMoveInfo.sState & enumMSTATE_CANCEL)
				{
					long	lDist;
					CCharacter	*pCMainCha = CGameScene::GetMainCha();
					if( pCMainCha )
					{
						lDist = (pCMainCha->GetCurX() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x) * (pCMainCha->GetCurX() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x)
							+ (pCMainCha->GetCurY() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y) * (pCMainCha->GetCurY() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y);
						LG(szLogName, "++++++++++++++Distance: %d\n", (long)sqrt(double(lDist)));
					}
				}
				LG(szLogName, "\n");
				//

				



				NetActorMove( l_id, SMoveInfo );

			}
			break;
		case enumACTION_SKILL_SRC:
			{
				stNetNotiSkillRepresent SSkillInfo;
				SSkillInfo.chCrt = 0;
				SSkillInfo.sStopState = enumEXISTS_WAITING;

				SSkillInfo.byFightID = pk.ReadUInt8();
				SSkillInfo.sAngle = pk.ReadUInt16();
				SSkillInfo.sState = pk.ReadUInt16();
				if (SSkillInfo.sState != enumFSTATE_ON)
					SSkillInfo.sStopState = pk.ReadUInt16();
				SSkillInfo.lSkillID = pk.ReadUInt32();
				SSkillInfo.lSkillSpeed = pk.ReadUInt32();
				char chTarType = pk.ReadUInt8();
				if (chTarType == 1)
				{
					SSkillInfo.lTargetID = pk.ReadUInt32();
					SSkillInfo.STargetPoint.x = pk.ReadUInt32();
					SSkillInfo.STargetPoint.y = pk.ReadUInt32();
				}
				else if (chTarType == 2)
				{
					SSkillInfo.lTargetID = 0;
					SSkillInfo.STargetPoint.x = pk.ReadUInt32();
					SSkillInfo.STargetPoint.y = pk.ReadUInt32();
				}
				SSkillInfo.sExecTime = pk.ReadUInt16();

				// ͬ������
				short lResNum =  pk.ReadUInt16();
				if (lResNum > 0)
				{
					SSkillInfo.SEffect.Resize( lResNum );
					for (long i = 0; i < lResNum; i++)
					{
						SSkillInfo.SEffect[i].lAttrID = pk.ReadUInt8();
						SSkillInfo.SEffect[i].lVal = pk.ReadUInt32();
						/*if(SSkillInfo.SEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SEffect[i].lAttrID==ATTR_CLEXP)
							SSkillInfo.SEffect[i].lVal= pk.ReadInt64();
						else
							SSkillInfo.SEffect[i].lVal = (long)pk.ReadUInt32();*/
					}
				}

				// ͬ������״̬
				short chSStateNum = pk.ReadUInt8();
				if ( chSStateNum > 0 )
				{
					SSkillInfo.SState.Resize( chSStateNum );
					for (short chNum = 0; chNum < chSStateNum; chNum++)
					{
						SSkillInfo.SState[chNum].chID = pk.ReadUInt8();
						SSkillInfo.SState[chNum].chLv = pk.ReadUInt8();
					}
				}

				// log
				LG(szLogName, "===Recieve(Skill Represent):\tTick:[%u]\n", GetTickCount());
				LG(szLogName, "Angle:\t%d\tFightID:%d\n", SSkillInfo.sAngle, SSkillInfo.byFightID );
				LG(szLogName, "SkillID:\t%u\tSkillSpeed:%d\n", SSkillInfo.lSkillID, SSkillInfo.lSkillSpeed);
					LG(szLogName, "TargetInfo(ID, PosX, PosY):\t%d\n", SSkillInfo.lTargetID, SSkillInfo.STargetPoint.x, SSkillInfo.STargetPoint.y);
				LG(szLogName, "Effect:[ID, Value]\n");
				for (DWORD i = 0; i < SSkillInfo.SEffect.GetCount(); i++)
					LG(szLogName, "\t%d,\t%d\n", SSkillInfo.SEffect[i].lAttrID, SSkillInfo.SEffect[i].lVal);
				if (SSkillInfo.SState.GetCount() > 0)
					LG(szLogName, "Skill State:[ID, LV]\n");
				for (DWORD chNum = 0; chNum < SSkillInfo.SState.GetCount(); chNum++)
					LG(szLogName, "\t%d, %d\n", SSkillInfo.SState[chNum].chID, SSkillInfo.SState[chNum].chLv);
				if (SSkillInfo.sState)
					LG(szLogName, "@@@End Skill\tState:0x%x\n", SSkillInfo.sState);
				LG(szLogName, "\n");
				//

				NetActorSkillRep(l_id, SSkillInfo);
			}
			break;
		case enumACTION_SKILL_TAR:
			{
				stNetNotiSkillEffect SSkillInfo{};

				SSkillInfo.byFightID = pk.ReadUInt8();
				SSkillInfo.sState = pk.ReadUInt16();
				SSkillInfo.bDoubleAttack = pk.ReadUInt8() ? true : false;
				SSkillInfo.bMiss = pk.ReadUInt8() ? true : false;
				if (SSkillInfo.bBeatBack = pk.ReadUInt8() ? true : false)
				{
					SSkillInfo.SPos.x = pk.ReadUInt32();
					SSkillInfo.SPos.y = pk.ReadUInt32();
				}
				SSkillInfo.lSrcID = pk.ReadUInt32();
				SSkillInfo.SSrcPos.x = pk.ReadUInt32();
				SSkillInfo.SSrcPos.y = pk.ReadUInt32();
				SSkillInfo.lSkillID = pk.ReadUInt32();
				SSkillInfo.SSkillTPos.x = pk.ReadUInt32();
				SSkillInfo.SSkillTPos.y = pk.ReadUInt32();
				SSkillInfo.sExecTime = pk.ReadUInt16();

				// ͬ������
				pk.ReadUInt8();
				short lResNum = pk.ReadUInt16();
				if (lResNum > 0)
				{
					SSkillInfo.SEffect.Resize(lResNum);
					for (long i = 0; i < lResNum; i++)
					{
						SSkillInfo.SEffect[i].lAttrID = pk.ReadUInt8();
						SSkillInfo.SEffect[i].lVal = pk.ReadUInt32();
						/*if(SSkillInfo.SEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SEffect[i].lAttrID==ATTR_CLEXP)
							SSkillInfo.SEffect[i].lVal= pk.ReadInt64();
						else
							SSkillInfo.SEffect[i].lVal = (long)pk.ReadUInt32();*/

						char val[32];
						char buff[234];
						sprintf(buff, "ID = %d val = %s\r\n", SSkillInfo.SEffect[i].lAttrID, _i64toa(SSkillInfo.SEffect[i].lVal, val, 10));
						::OutputDebugStr(buff);
					}
				}

				short chSStateNum = 0;
				// ͬ������״̬
				if (pk.ReadUInt8() == 1)
				{
					pk.ReadUInt32();
					chSStateNum = pk.ReadUInt8();
					if (chSStateNum > 0)
					{
						SSkillInfo.SState.Resize( chSStateNum );
						for (short chNum = 0; chNum < chSStateNum; chNum++)
						{
							SSkillInfo.SState[chNum].chID = pk.ReadUInt8();
							SSkillInfo.SState[chNum].chLv = pk.ReadUInt8();
							
							pk.ReadUInt32();
							pk.ReadUInt32();
						}
					}
				}

				short lSrcResNum = 0;
				short chSrcSStateNum = 0;
				if (pk.ReadUInt8())
				{
					// ����Դ״̬
					SSkillInfo.sSrcState = pk.ReadUInt16();
					// ͬ������Դ����
					pk.ReadUInt8();
					lSrcResNum = pk.ReadUInt16();
					if ( lSrcResNum > 0)
					{
						SSkillInfo.SSrcEffect.Resize( lSrcResNum );
						for (long i = 0; i < lSrcResNum; i++)
						{
							SSkillInfo.SSrcEffect[i].lAttrID = pk.ReadUInt8();
							SSkillInfo.SSrcEffect[i].lVal = pk.ReadUInt32();
							/*if(SSkillInfo.SSrcEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SSrcEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SSrcEffect[i].lAttrID==ATTR_CLEXP)
								SSkillInfo.SSrcEffect[i].lVal= pk.ReadInt64();
							else
								SSkillInfo.SSrcEffect[i].lVal = (long)pk.ReadUInt32();*/
						}
					}
					NetActorSkillEff(l_id, SSkillInfo);
					break; 
					
					if (pk.ReadUInt8() == 1)
					{
						// ͬ������Դ����״̬
						pk.ReadUInt32();
						chSrcSStateNum = pk.ReadUInt8();
						if (chSrcSStateNum > 0)
						{
							SSkillInfo.SSrcState.Resize( chSrcSStateNum );
							for (short chNum = 0; chNum < chSrcSStateNum; chNum++)
							{
								SSkillInfo.SSrcState[chNum].chID = pk.ReadUInt8();
								SSkillInfo.SSrcState[chNum].chLv = pk.ReadUInt8();
								pk.ReadUInt32();
								pk.ReadUInt32();
							}
						}
					}
				}

				NetActorSkillEff(l_id, SSkillInfo);
			}
			break;
		case enumACTION_LEAN: // �п�
			{
				stNetLeanInfo SLean;
				SLean.chState = pk.ReadUInt8();
				if (!SLean.chState)
				{
					SLean.lPose = pk.ReadUInt32();
					SLean.lAngle = pk.ReadUInt32();
					SLean.lPosX = pk.ReadUInt32();
					SLean.lPosY = pk.ReadUInt32();
					SLean.lHeight = pk.ReadUInt32();
				}

				// log
				LG(szLogName, "===Recieve(Lean):\tTick:[%u]\n", GetTickCount());
				if (SLean.chState)
					LG(szLogName, "@@@End Lean\tState:%d\n", SLean.chState);
				LG(szLogName, "\n");
				//

				NetActorLean(l_id, SLean);
			}
			break;
		case enumACTION_ITEM_FAILED:
			{
				stNetSysInfo l_sysinfo;
				short sFailedID = pk.ReadUInt16();
				l_sysinfo.m_sysinfo = g_GetUseItemFailedInfo(sFailedID);
				NetSysInfo(l_sysinfo);
			}
			break;
		case enumACTION_LOOK: // ���½�ɫ���
			{
				stNetLookInfo SLookInfo;
				ReadChaLookPacket(pk, SLookInfo, szLogName);
				CCharacter* pCha = CGameApp::GetCurScene()->SearchByID( l_id );

				if(! g_stUIMap.IsPKSilver() || pCha->GetMainType()==enumMainPlayer)
				{
					NetChangeChaPart(l_id, SLookInfo);
				}
			}
			break;
		case enumACTION_LOOK_ENERGY: // ���½�ɫ��۵�����
			{
				stLookEnergy SLookEnergy;
				ReadChaLookEnergyPacket(pk, SLookEnergy, szLogName);
				NetChangeChaLookEnergy(l_id, SLookEnergy);
			}
			break;
		case enumACTION_KITBAG: // ���½�ɫ������
			{
				stNetKitbag SKitbag;
				ReadChaKitbagPacket(pk, SKitbag, szLogName);
				SKitbag.chBagType = 0;
				NetChangeKitbag(l_id, SKitbag);
			}
			break;
		case enumACTION_ITEM_INFO:
			{
			}
			break;
		case enumACTION_SHORTCUT: // ���¿����
			{
				stNetShortCut SShortcut;
				ReadChaShortcutPacket(pk, SShortcut, szLogName);
				NetShortCut(l_id, SShortcut);
			}
			break;
		case enumACTION_TEMP:
			{
				stTempChangeChaPart STempChaPart;
				STempChaPart.dwItemID = pk.ReadUInt32();
				STempChaPart.dwPartID = pk.ReadUInt32();
				NetTempChangeChaPart(l_id, STempChaPart);
			}
			break;
		case enumACTION_CHANGE_CHA:
			{
				stNetChangeCha SChangeCha;
				SChangeCha.ulMainChaID = pk.ReadUInt32();

				NetActorChangeCha(l_id, SChangeCha);
			}
			break;
		case enumACTION_FACE:
			{
				stNetFace SNetFace;
				SNetFace.sAngle = pk.ReadUInt16();
				SNetFace.sPose = pk.ReadUInt16();
                NetFace( l_id, SNetFace, enumACTION_FACE );

				// log
				LG(szLogName, "===Recieve(Face):\tTick:[%u]\n", GetTickCount());
				LG(szLogName, "Angle[%d]\tPose[%d]\n", SNetFace.sAngle, SNetFace.sPose);
				LG(szLogName, "\n");
				//

			}
			break;
		case enumACTION_SKILL_POSE:
			{
				stNetFace SNetFace;
				SNetFace.sAngle = pk.ReadUInt16();
				SNetFace.sPose = pk.ReadUInt16();
                NetFace( l_id, SNetFace, enumACTION_SKILL_POSE );

				// log
				LG(szLogName, "===Recieve(Skill Pos):\tTick:[%u]\n", GetTickCount());
				LG(szLogName, "Angle[%d]\tPose[%d]\n", SNetFace.sAngle, SNetFace.sPose);
				LG(szLogName, "\n");
				//

			}
			break;
		case enumACTION_PK_CTRL:
			{
				stNetPKCtrl	SNetPKCtrl;
				ReadChaPKPacket(pk, SNetPKCtrl, szLogName);

				SNetPKCtrl.Exec( l_id );
			}
			break;
		case enumACTION_BANK:	// ������
			{
				stNetKitbag SKitbag;
				ReadChaKitbagPacket(pk, SKitbag, szLogName);
				SKitbag.chBagType = 1;
				NetChangeKitbag(l_id, SKitbag);
			}
			break;
		case enumACTION_GUILDBANK:	// ������
			{
				stNetKitbag SKitbag;
				ReadChaKitbagPacket(pk, SKitbag, szLogName);
				SKitbag.chBagType = 3;
				NetChangeKitbag(l_id, SKitbag);
			}
			break;
		case enumACTION_KITBAGTMP:	// ����ʱ����
			{
				stNetKitbag STempKitbag;
				ReadChaKitbagPacket(pk, STempKitbag, szLogName);
				STempKitbag.chBagType = 2;
				NetChangeKitbag(l_id, STempKitbag);
				break;
			}
		case enumACTION_REQUESTGUILDLOGS:{
			g_stUIGuildMgr.RequestGuildLogs(pk);
			break;
		}
		case enumACTION_UPDATEGUILDLOGS:{
			g_stUIGuildMgr.UpdateGuildLogs(pk);
			break;
		}
		default:
			break;
		}
	}
	//catch (...)
	//{
	//	MessageBox(0, "!!!!!!!!!!!!!!!!!!!!exception: Notice Action", "error", 0);
	//}

	return TRUE;
T_E}

// Э��S->C : ��ɫ������ж�ʧ��
BOOL SC_FailedAction(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();

	char	chType, chReason;
	chType = pk.ReadUInt8();
	chReason = pk.ReadUInt8();
    NetFailedAction( chReason );    
	return TRUE;
T_E}

// ͬ����ɫ����
BOOL SC_SynAttribute(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();
	const char* szLogName = g_LogName.GetLogName( l_id );

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);

	//char val[32];
	//char buff[245];
	//sprintf(buff, "NetSynAttr %s\r\n", _i64toa(SChaAttr.SEff[0].lVal , val, 10));
	//OutputDebugStr(buff);

	NetSynAttr(l_id, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff );

	return TRUE;
T_E}

// ͬ��������
BOOL SC_SynSkillBag(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();
	const char* szLogName = g_LogName.GetLogName( l_id );

	stNetSkillBag SCurSkill;
	if (ReadChaSkillBagPacket(pk, SCurSkill, szLogName))
		NetSynSkillBag(l_id, &SCurSkill);

	return TRUE;
T_E}

// ͬ��������
BOOL SC_SynDefaultSkill(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();
	stNetDefaultSkill	SDefaultSkill;
	SDefaultSkill.sSkillID = pk.ReadUInt16();
	SDefaultSkill.Exec();
	return TRUE;
T_E}

BOOL SC_SynSkillState(LPRPACKET pk)
{T_B
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	uLong l_id = pk.ReadUInt32();
	const char* szLogName = g_LogName.GetLogName( l_id );

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);

	NetSynSkillState(l_id, &SCurSState);

	return TRUE;
T_E}

BOOL SC_SynTeam(LPRPACKET pk)
{T_B
	stNetTeamState	STeamState;

	STeamState.ulID = pk.ReadUInt32();
	STeamState.lHP = pk.ReadUInt32();
    STeamState.lMaxHP = pk.ReadUInt32();
	STeamState.lSP = pk.ReadUInt32();
    STeamState.lMaxSP = pk.ReadUInt32();
	STeamState.lLV = pk.ReadUInt32();

    LG( "Team", "Refresh, ID[%u], HP[%d], MaxHP[%d], SP[%d], MaxSP[%d], LV[%d]\n", STeamState.ulID, STeamState.lHP, STeamState.lMaxHP, STeamState.lSP, STeamState.lMaxSP, STeamState.lLV );

	stNetLookInfo SLookInfo;
	ReadChaLookPacket(pk, SLookInfo, const_cast<char*>(g_oLangRec.GetString(299)));
	stNetChangeChaPart	&SFace = SLookInfo.SLook;
	STeamState.SFace.sTypeID = SFace.sTypeID;
	STeamState.SFace.sHairID = SFace.sHairID;
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		STeamState.SFace.SLink[i].sID = SFace.SLink[i].sID;
		STeamState.SFace.SLink[i].sNum = SFace.SLink[i].sNum;
		STeamState.SFace.SLink[i].chForgeLv = SFace.SLink[i].chForgeLv;
		STeamState.SFace.SLink[i].lFuseID = SFace.SLink[i].lDBParam[1]>>16;
	}

	NetSynTeam(&STeamState);

	return true;
T_E}

BOOL SC_SynTLeaderID(LPRPACKET pk)
{T_B
	long	lID = pk.ReadUInt32();
	long	lLeaderID = pk.ReadUInt32();

	NetChaTLeaderID(lID, lLeaderID);

	// log
	LG(g_LogName.GetLogName( lID ), g_oLangRec.GetString(300), lLeaderID, lID);
	//

	return TRUE;
T_E}

BOOL SC_HelpInfo( LPRPACKET packet )
{T_B
	NET_HELPINFO Info;
	memset( &Info, 0, sizeof(NET_HELPINFO) );

	Info.byType = packet.ReadUInt8();
	if( Info.byType == mission::MIS_HELP_DESP || Info.byType == mission::MIS_HELP_IMAGE || 
		Info.byType == mission::MIS_HELP_BICKER )
	{
		const char* pszDesp = packet.ReadString();
		strncpy( Info.szDesp, pszDesp, ROLE_MAXNUM_DESPSIZE - 1 );
	}
	else if( Info.byType == mission::MIS_HELP_SOUND )
	{
		Info.sID = packet.ReadUInt16();
	}
	else
	{
		return FALSE;
	}

	NetShowHelp( Info );
	return TRUE;
T_E}

// NPC �Ի���Ϣ����
BOOL SC_TalkInfo( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	BYTE byCmd = packet.ReadUInt8();
	USHORT sLen = 0;
	const char* pszDesp = packet.ReadString();
	if( pszDesp == NULL ) return FALSE;
	NetShowTalk( pszDesp, byCmd, dwNpcID  );
	return TRUE;
T_E}

BOOL SC_FuncInfo( LPRPACKET packet )
{T_B
	NET_FUNCPAGE FuncPage;
	memset( &FuncPage, 0, sizeof(NET_FUNCPAGE) );

	DWORD dwNpcID = packet.ReadUInt32();
	BYTE byPage  = packet.ReadUInt8();
	strncpy( FuncPage.szTalk, packet.ReadString(), ROLE_MAXNUM_DESPSIZE - 1 );
	BYTE byCount = packet.ReadUInt8();
	
	if( byCount > ROLE_MAXNUM_FUNCITEM ) byCount = ROLE_MAXNUM_FUNCITEM;
	for( int i = 0; i < byCount; i++ )
	{
		const char* pszFunc = packet.ReadString();
		strncpy( FuncPage.FuncItem[i].szFunc, pszFunc, ROLE_MAXNUM_FUNCITEMSIZE - 1 );
	}

	BYTE byMisCount = packet.ReadUInt8();
	if( byMisCount > ROLE_MAXNUM_CAPACITY ) {	byMisCount = ROLE_MAXNUM_CAPACITY; }

	for( int i = 0; i < byMisCount; i++ )
	{
		const char* pszMis = packet.ReadString();
		strncpy( FuncPage.MisItem[i].szMis, pszMis, ROLE_MAXNUM_FUNCITEMSIZE - 1 );
		FuncPage.MisItem[i].byState = packet.ReadUInt8();
	}

	NetShowFunction( byPage, byCount, byMisCount, FuncPage, dwNpcID );
	return TRUE;
T_E}

BOOL SC_CloseTalk( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	NetCloseTalk( dwNpcID );
	return TRUE;
T_E}

BOOL SC_TradeData( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	BYTE byPage = packet.ReadUInt8();
	BYTE byIndex = packet.ReadUInt8();
	USHORT sItemID = packet.ReadUInt16();
	USHORT sCount = packet.ReadUInt16();
	DWORD dwPrice = packet.ReadUInt32();

	NetUpdateTradeData( dwNpcID, byPage, byIndex, sItemID, sCount, dwPrice );
	return TRUE;
T_E}

BOOL SC_TradeAllData( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	BYTE  byType = packet.ReadUInt8();
	DWORD dwParam = packet.ReadUInt32();
	BYTE  byCount = packet.ReadUInt8();

	NET_TRADEINFO Info;
	for( BYTE i = 0; i < byCount; i++ )
	{
		BYTE byItemType = packet.ReadUInt8();
		switch( byItemType )
		{
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:		
		case mission::TI_SYNTHESIS:		
			{			
				Info.TradePage[byItemType].byCount = packet.ReadUInt8();
				if( Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM )
					Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

				for( BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++ )
				{
					Info.TradePage[byItemType].sItemID[n] = packet.ReadUInt16();
					if( byType == mission::TRADE_GOODS )
					{
						Info.TradePage[byItemType].sCount[n] = packet.ReadUInt16();
						Info.TradePage[byItemType].dwPrice[n] = packet.ReadUInt32();
						Info.TradePage[byItemType].byLevel[n] = packet.ReadUInt8();
					}
				}
			}
			break;
		default:
			break;
		}
	}
	NetUpdateTradeAllData( Info, byType, dwNpcID, dwParam );
	return TRUE;
T_E}

BOOL SC_TradeInfo( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	BYTE  byType = packet.ReadUInt8();
	DWORD dwParam = packet.ReadUInt32();
	BYTE  byCount = packet.ReadUInt8();

	NET_TRADEINFO Info;
	for( BYTE i = 0; i < byCount; i++ )
	{
		BYTE byItemType = packet.ReadUInt8();
		switch( byItemType )
		{
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:		
		case mission::TI_SYNTHESIS:		
			{			
				Info.TradePage[byItemType].byCount = packet.ReadUInt8();
				if( Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM )
					Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

				for( BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++ )
				{
					Info.TradePage[byItemType].sItemID[n] = packet.ReadUInt16();
					if( byType == mission::TRADE_GOODS )
					{
						Info.TradePage[byItemType].sCount[n] = packet.ReadUInt16();
						Info.TradePage[byItemType].dwPrice[n] = packet.ReadUInt32();
						Info.TradePage[byItemType].byLevel[n] = packet.ReadUInt8();
					}
				}
			}
			break;
		default:
			break;
		}
	}
	NetShowTrade( Info, byType, dwNpcID, dwParam );
	return TRUE;
T_E}

BOOL SC_TradeUpdate( LPRPACKET packet )
{T_B
	//���н��׸���,ֻ���ڴ򿪺��н��׽�������²Ÿ���

	DWORD dwNpcID = packet.ReadUInt32();
	BYTE  byType = packet.ReadUInt8();
	DWORD dwParam = packet.ReadUInt32();
	BYTE  byCount = packet.ReadUInt8();

	NET_TRADEINFO Info;
	for( BYTE i = 0; i < byCount; i++ )
	{
		BYTE byItemType = packet.ReadUInt8();
		switch( byItemType )
		{
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:		
		case mission::TI_SYNTHESIS:		
			{			
				Info.TradePage[byItemType].byCount = packet.ReadUInt8();
				if( Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM )
					Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

				for( BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++ )
				{
					Info.TradePage[byItemType].sItemID[n] = packet.ReadUInt16();
					if( byType == mission::TRADE_GOODS )
					{
						Info.TradePage[byItemType].sCount[n] = packet.ReadUInt16();
						Info.TradePage[byItemType].dwPrice[n] = packet.ReadUInt32();
						Info.TradePage[byItemType].byLevel[n] = packet.ReadUInt8();
					}
				}
			}
			break;
		default:
			break;
		}
	}

	if(g_stUINpcTrade.GetIsShow())
	{
		NetShowTrade( Info, byType, dwNpcID, dwParam );
	}

	return TRUE;
T_E}

BOOL SC_TradeResult( LPRPACKET packet )
{T_B
	BYTE byType = packet.ReadUInt8();
	BYTE byIndex = packet.ReadUInt8();
	BYTE byCount = packet.ReadUInt8();
	USHORT sItemID = packet.ReadUInt16();
	DWORD  dwMoney  = packet.ReadUInt32();
	LG("trade", g_oLangRec.GetString(301), byType, byIndex, byCount, sItemID, dwMoney);
	  NetTradeResult( byType, byIndex, byCount, sItemID, dwMoney );
	LG("trade", g_oLangRec.GetString(302));
	return TRUE;
T_E}

BOOL SC_CharTradeInfo( LPRPACKET packet )
{T_B
	USHORT usCmd = packet.ReadUInt16();
	switch( usCmd )
	{
	case CMD_MC_CHARTRADE_REQUEST:
		{
			BYTE byType = packet.ReadUInt8();
			DWORD dwRequestID = packet.ReadUInt32();
			NetShowCharTradeRequest( byType, dwRequestID );
		}
		break;
	case CMD_MC_CHARTRADE_CANCEL:
		{
			DWORD dwCharID = packet.ReadUInt32();
			NetCancelCharTrade( dwCharID );
		}
		break;
	case CMD_MC_CHARTRADE_MONEY:
		{
			DWORD dwCharID = packet.ReadUInt32();
			DWORD dwMoney  = packet.ReadUInt32();
			int currency = packet.ReadUInt8();
	
			if(currency==0){
				NetTradeShowMoney( dwCharID, dwMoney );
			}else if(currency==1){
				NetTradeShowIMP( dwCharID, dwMoney );
			}
		}
		break;
	case CMD_MC_CHARTRADE_ITEM:
		{
			DWORD dwRequestID = packet.ReadUInt32();
			BYTE byOpType = packet.ReadUInt8();
			if( byOpType == mission::TRADE_DRAGTO_ITEM )
			{
				BYTE byItemIndex = packet.ReadUInt8();
				BYTE byIndex = packet.ReadUInt8();
				BYTE byCount = packet.ReadUInt8();
				
				// ��Ʒʵ������
				NET_CHARTRADE_ITEMDATA Data;
				memset( &Data, 0, sizeof(NET_CHARTRADE_ITEMDATA) );

				NetTradeAddItem( dwRequestID, byOpType, 0, byIndex, byCount, byItemIndex, Data );
			}
			else if( byOpType == mission::TRADE_DRAGTO_TRADE )
			{
				USHORT sItemID = packet.ReadUInt16();
				BYTE byItemIndex = packet.ReadUInt8();
				BYTE byIndex = packet.ReadUInt8();				
				BYTE byCount = packet.ReadUInt8();				
				USHORT sType = packet.ReadUInt16();
				
				if( sType == enumItemTypeBoat )
				{
					NET_CHARTRADE_BOATDATA Data;
					memset( &Data, 0, sizeof(NET_CHARTRADE_BOATDATA) );

					if( packet.ReadUInt8() == 0 )
					{
						// ��Ϣ����
					}
					else
					{
						strncpy( Data.szName, packet.ReadString(), BOAT_MAXSIZE_BOATNAME - 1 );
						Data.sBoatID = packet.ReadUInt16();
						Data.sLevel = packet.ReadUInt16();
						Data.dwExp = packet.ReadUInt32();
						Data.dwHp = packet.ReadUInt32();
						Data.dwMaxHp = packet.ReadUInt32();
						Data.dwSp = packet.ReadUInt32();
						Data.dwMaxSp = packet.ReadUInt32();
						Data.dwMinAttack = packet.ReadUInt32();
						Data.dwMaxAttack = packet.ReadUInt32();
						Data.dwDef = packet.ReadUInt32();
						Data.dwSpeed = packet.ReadUInt32();
						Data.dwShootSpeed = packet.ReadUInt32();
						Data.byHasItem = packet.ReadUInt8();
						Data.byCapacity = packet.ReadUInt8();
						Data.dwPrice = packet.ReadUInt32();
						NetTradeAddBoat( dwRequestID, byOpType, sItemID, byIndex, byCount, byItemIndex, Data );
					}
				}
				else
				{
					// ��Ʒʵ������
					NET_CHARTRADE_ITEMDATA Data;
					memset( &Data, 0, sizeof(NET_CHARTRADE_ITEMDATA) );

					Data.sEndure[0] = packet.ReadUInt16();
					Data.sEndure[1] = packet.ReadUInt16();
					Data.sEnergy[0] = packet.ReadUInt16();
					Data.sEnergy[1] = packet.ReadUInt16();
					Data.byForgeLv = packet.ReadUInt8();
					Data.bValid = packet.ReadUInt8() != 0 ? true : false;
					Data.bItemTradable = packet.ReadUInt8();
					Data.expiration = packet.ReadUInt32();

					Data.lDBParam[enumITEMDBP_FORGE] = packet.ReadUInt32();
					Data.lDBParam[enumITEMDBP_INST_ID] = packet.ReadUInt32();



					Data.byHasAttr = packet.ReadUInt8();
					if( Data.byHasAttr )
					{
						for( int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++ )
						{
							Data.sInstAttr[i][0] = packet.ReadUInt16();
							Data.sInstAttr[i][1] = packet.ReadUInt16();
						}
					}

					NetTradeAddItem( dwRequestID, byOpType, sItemID, byIndex, byCount, byItemIndex, Data );
				}
			}
			else
			{
				MessageBox( NULL, g_oLangRec.GetString(303), g_oLangRec.GetString(25), MB_OK );
				return FALSE;
			}
			
		}
		break;
	case CMD_MC_CHARTRADE_PAGE:
		{
			BYTE byType = packet.ReadUInt8();
			DWORD dwAcceptID = packet.ReadUInt32();
			DWORD dwRequestID = packet.ReadUInt32();
			NetShowCharTradeInfo( byType, dwAcceptID, dwRequestID );
		}
		break;
	case CMD_MC_CHARTRADE_VALIDATEDATA:
		{
			DWORD dwCharID = packet.ReadUInt32();
			NetValidateTradeData( dwCharID );
		}
		break;
	case CMD_MC_CHARTRADE_VALIDATE:
		{
			DWORD dwCharID = packet.ReadUInt32();
			NetValidateTrade( dwCharID );
		}
		break;
	case CMD_MC_CHARTRADE_RESULT:
		{
			BYTE byResult = packet.ReadUInt8();
			if( byResult == mission::TRADE_SUCCESS )
			{
				NetTradeSuccess();
			}
			else
			{
				NetTradeFailed();
			}
			
		}
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
T_E}

BOOL SC_DailyBuffInfo(LPRPACKET packet)
{
	T_B
		const auto imgName = packet.ReadString();
	if (!imgName)
	{
		LG("DailyBuffInfo","Error invalid reading image name \n");
		return FALSE;
	}
	const auto labelInfo = packet.ReadString();
	if (!labelInfo)
	{
		LG("DailyBuffInfo", "Error invalid reading labelInfo  \n");
		return FALSE;
	}
	//show the form 
	g_stUIMap.SetupDailyBuffForm(imgName, labelInfo);
		return TRUE;
	T_E
}
BOOL SC_MissionInfo( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	NET_MISSIONLIST list;
	memset( &list, 0, sizeof(NET_MISSIONLIST) );

	list.byListType = packet.ReadUInt8();
	list.byPrev = packet.ReadUInt8();
	list.byNext = packet.ReadUInt8();
	list.byPrevCmd = packet.ReadUInt8();
	list.byNextCmd = packet.ReadUInt8();
	list.byItemCount = packet.ReadUInt8();
	
	if( list.byItemCount > ROLE_MAXNUM_FUNCITEM ) list.byItemCount = ROLE_MAXNUM_FUNCITEM;
	for( int i = 0; i < list.byItemCount; i++ )
	{
		USHORT sLen = 0;
		const char* pszFunc = packet.ReadString();
		strncpy( list.FuncPage.FuncItem[i].szFunc, pszFunc, 32 );
	}

	NetShowMissionList( dwNpcID, list );
	return TRUE;
T_E}

BOOL SC_MisPage( LPRPACKET packet )
{T_B
	BYTE byCmd = packet.ReadUInt8();
	DWORD dwNpcID = packet.ReadUInt32();
	NET_MISPAGE page;
	memset( &page, 0,sizeof(NET_MISPAGE) );
	
	// ��������
	const char* pszName = packet.ReadString();
	strncpy( page.szName, pszName, ROLE_MAXSIZE_MISNAME - 1 );

	switch( byCmd )
	{
	case ROLE_MIS_BTNACCEPT:
	case ROLE_MIS_BTNDELIVERY:
	case ROLE_MIS_BTNPENDING:
		{
			// ����������Ϣ
			page.byNeedNum = packet.ReadUInt8();
			for( int i = 0; i < page.byNeedNum; i++ )
			{
				page.MisNeed[i].byType = packet.ReadUInt8();
				if( page.MisNeed[i].byType == mission::MIS_NEED_ITEM || page.MisNeed[i].byType == mission::MIS_NEED_KILL )
				{
					page.MisNeed[i].wParam1 = packet.ReadUInt16();
					page.MisNeed[i].wParam2 = packet.ReadUInt16();
					page.MisNeed[i].wParam3 = packet.ReadUInt8();
				}
				else if( page.MisNeed[i].byType == mission::MIS_NEED_DESP )
				{
					const char* pszTemp = packet.ReadString();
					strncpy( page.MisNeed[i].szNeed, pszTemp, ROLE_MAXNUM_NEEDDESPSIZE - 1 );
				}
				else
				{
					// δ֪����������
					LG( "mission_error", g_oLangRec.GetString(304));
					return FALSE;
				}
			}
			
			// ��������Ϣ
			page.byPrizeSelType = packet.ReadUInt8();
			page.byPrizeNum = packet.ReadUInt8();
			for( int i = 0; i < page.byPrizeNum; i++ )
			{
				page.MisPrize[i].byType = packet.ReadUInt8();
				page.MisPrize[i].wParam1 = packet.ReadUInt16();
				page.MisPrize[i].wParam2 = packet.ReadUInt16();
			}

			// ����������Ϣ
			const char* pszDesp = packet.ReadString();
			strncpy( page.szDesp, pszDesp, ROLE_MAXNUM_DESPSIZE - 1 );
		}
		break;
	default:
		return FALSE;
	}

	NetShowMisPage( dwNpcID, byCmd, page );
	return TRUE;
T_E}

BOOL SC_MisLog( LPRPACKET packet )
{T_B
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	NET_MISLOG_LIST LogList;
	memset( &LogList, 0, sizeof(NET_MISLOG_LIST) );

	LogList.byNumLog = packet.ReadUInt8();
	for( int i = 0; i < LogList.byNumLog; i++ )
	{		
		LogList.MisLog[i].wMisID  = packet.ReadUInt16();
		LogList.MisLog[i].byState = packet.ReadUInt8();
	}

	NetMisLogList( LogList );
	return TRUE;
T_E}

BOOL SC_MisLogInfo( LPRPACKET packet )
{T_B
	NET_MISPAGE page;
	memset( &page, 0,sizeof(NET_MISPAGE) );

	// ���������Ϣ
	WORD wMisID = packet.ReadUInt16();	
	const char* pszName = packet.ReadString();
	strncpy( page.szName, pszName, ROLE_MAXSIZE_MISNAME - 1 );

	// ����������Ϣ
	page.byNeedNum = packet.ReadUInt8();
	for( int i = 0; i < page.byNeedNum; i++ )
	{
		page.MisNeed[i].byType = packet.ReadUInt8();
		if( page.MisNeed[i].byType == mission::MIS_NEED_ITEM || page.MisNeed[i].byType == mission::MIS_NEED_KILL )
		{
			page.MisNeed[i].wParam1 = packet.ReadUInt16();
			page.MisNeed[i].wParam2 = packet.ReadUInt16();
			page.MisNeed[i].wParam3 = packet.ReadUInt8();
		}
		else if( page.MisNeed[i].byType == mission::MIS_NEED_DESP )
		{
			const char* pszTemp = packet.ReadString();
			strncpy( page.MisNeed[i].szNeed, pszTemp, ROLE_MAXNUM_NEEDDESPSIZE - 1 );
		}
		else
		{
			// δ֪����������
			LG( "mission_error", g_oLangRec.GetString(304));
			return FALSE;
		}
	}

	// ��������Ϣ
	page.byPrizeSelType = packet.ReadUInt8();
	page.byPrizeNum = packet.ReadUInt8();
	for( int i = 0; i < page.byPrizeNum; i++ )
	{
		page.MisPrize[i].byType = packet.ReadUInt8();
		page.MisPrize[i].wParam1 = packet.ReadUInt16();
		page.MisPrize[i].wParam2 = packet.ReadUInt16();
	}

	// ����������Ϣ
	const char* pszDesp = packet.ReadString();
	strncpy( page.szDesp, pszDesp, ROLE_MAXNUM_DESPSIZE - 1 );

	NetShowMisLog( wMisID, page );
	return TRUE;
T_E}

BOOL SC_MisLogClear( LPRPACKET packet )
{T_B
	WORD wMisID  = packet.ReadUInt16();

	NetMisLogClear( wMisID );
	return TRUE;
T_E}

BOOL SC_MisLogAdd( LPRPACKET packet )
{T_B
	WORD wMisID  = packet.ReadUInt16();
	BYTE byState = packet.ReadUInt8();

	NetMisLogAdd( wMisID, byState );
	return TRUE;
T_E}

BOOL SC_MisLogState( LPRPACKET packet )
{T_B
	WORD wID = packet.ReadUInt16();
	BYTE byState = packet.ReadUInt8();

	NetMisLogState( wID, byState );
	return TRUE;
T_E}

BOOL SC_TriggerAction( LPRPACKET packet )
{T_B
    stNetNpcMission info;
    info.byType = packet.ReadUInt8();	
	info.sID = packet.ReadUInt16();		// ���ݻ��������ID
	info.sNum = packet.ReadUInt16();		// ��Ҫ�ݻ����������
	info.sCount = packet.ReadUInt16();	// ����ɼ���
    NetTriggerAction( info );
	return TRUE;
T_E}

BOOL SC_NpcStateChange( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	BYTE  byState = packet.ReadUInt8();
	NetNpcStateChange( dwNpcID, byState );
	return TRUE;
T_E}

BOOL SC_EntityStateChange( LPRPACKET packet )
{T_B
	DWORD dwEntityID = packet.ReadUInt32();
	BYTE byState = packet.ReadUInt8();
	NetEntityStateChange( dwEntityID, byState );
	return TRUE;
T_E}

BOOL SC_Forge( LPRPACKET packet )
{T_B
	NetShowForge();
	return TRUE;
T_E}


BOOL SC_Unite( LPRPACKET packet )
{T_B
	NetShowUnite();
	return TRUE;
T_E}

BOOL SC_Milling( LPRPACKET packet )
{T_B
	NetShowMilling();
	return TRUE;
T_E}

BOOL SC_Fusion( LPRPACKET packet )
{T_B
	NetShowFusion();
	return TRUE;
T_E}

BOOL SC_Upgrade( LPRPACKET packet )
{T_B
	NetShowUpgrade();
	return TRUE;
T_E}

BOOL SC_EidolonMetempsychosis( LPRPACKET packet )
{T_B
	//NetShowEidolonMetempsychosis();
	NetShowEidolonFusion();
	return TRUE;
T_E}

BOOL SC_Eidolon_Fusion(LPRPACKET packet)
{T_B
	NetShowEidolonFusion();
	return TRUE;
T_E}

BOOL SC_Purify(LPRPACKET packet)
{T_B
	NetShowPurify();
	return TRUE;
T_E}

BOOL SC_Fix(LPRPACKET packet)
{T_B
	NetShowRepairOven();
	return TRUE;
T_E}

BOOL SC_GMSend(LPRPACKET packet)
{T_B
	g_stUIMail.ShowQuestionForm();
	return TRUE;
T_E}

BOOL SC_GMRecv(LPRPACKET packet)
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	CS_GMRecv(dwNpcID);
	return TRUE;
T_E}

BOOL SC_GetStone(LPRPACKET packet)
{T_B
	NetShowGetStone();
	return TRUE;
T_E}

BOOL SC_Energy(LPRPACKET packet)
{T_B
	NetShowEnergy();
	return TRUE;
T_E}

BOOL SC_Tiger(LPRPACKET packet)
{T_B
	NetShowTiger();
	return TRUE;
T_E}

BOOL SC_CreateBoat( LPRPACKET packet )
{T_B
	DWORD dwBoatID = packet.ReadUInt32();
	xShipBuildInfo Info;
	memset( &Info, 0, sizeof(xShipBuildInfo) );

	char szBoat[BOAT_MAXSIZE_NAME] = {0};	// �����Զ���
	strncpy( szBoat, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szName, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szDesp, packet.ReadString(), BOAT_MAXSIZE_DESP - 1 );
	strncpy( Info.szBerth, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	Info.byIsUpdate = packet.ReadUInt8();
	Info.sPosID = packet.ReadUInt16();
	Info.dwBody = packet.ReadUInt32();
	strncpy( Info.szBody, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	
	if( Info.byIsUpdate )
	{
		Info.byHeader = packet.ReadUInt8();
		Info.dwHeader = packet.ReadUInt32();
		strncpy( Info.szHeader, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

		Info.byEngine = packet.ReadUInt8();
		Info.dwEngine = packet.ReadUInt32();
		strncpy( Info.szEngine, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
		for( int i = 0; i < BOAT_MAXNUM_MOTOR; i++ )
		{
			Info.dwMotor[i] = packet.ReadUInt32();
		}
	}
	Info.byCannon = packet.ReadUInt8();
	strncpy( Info.szCannon, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.byEquipment = packet.ReadUInt8();
	strncpy( Info.szEquipment, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.dwMoney = packet.ReadUInt32();
	Info.dwMinAttack = packet.ReadUInt32();
	Info.dwMaxAttack = packet.ReadUInt32();
	Info.dwCurEndure = packet.ReadUInt32();
	Info.dwMaxEndure = packet.ReadUInt32();
	Info.dwSpeed  = packet.ReadUInt32();
	Info.dwDistance = packet.ReadUInt32();
	Info.dwDefence = packet.ReadUInt32();
	Info.dwCurSupply = packet.ReadUInt32();
	Info.dwMaxSupply = packet.ReadUInt32();
	Info.dwConsume = packet.ReadUInt32();
	Info.dwAttackTime = packet.ReadUInt32();
	Info.sCapacity = packet.ReadUInt16();

	NetCreateBoat( Info );
	return TRUE;
T_E}

BOOL SC_UpdateBoat( LPRPACKET packet )
{T_B
	DWORD dwBoatID = packet.ReadUInt32();
	xShipBuildInfo Info;
	memset( &Info, 0, sizeof(xShipBuildInfo) );
	
	char szBoat[BOAT_MAXSIZE_NAME] = {0};	// �����Զ���
	strncpy( szBoat, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szName, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szDesp, packet.ReadString(), BOAT_MAXSIZE_DESP - 1 );
	strncpy( Info.szBerth, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	Info.byIsUpdate = packet.ReadUInt8();
	Info.sPosID = packet.ReadUInt16();
	Info.dwBody = packet.ReadUInt32();
	strncpy( Info.szBody, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	
	if( Info.byIsUpdate )
	{
		Info.byHeader = packet.ReadUInt8();
		Info.dwHeader = packet.ReadUInt32();
		strncpy( Info.szHeader, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

		Info.byEngine = packet.ReadUInt8();
		Info.dwEngine = packet.ReadUInt32();
		strncpy( Info.szEngine, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
		for( int i = 0; i < BOAT_MAXNUM_MOTOR; i++ )
		{
			Info.dwMotor[i] = packet.ReadUInt32();
		}
	}
	Info.byCannon = packet.ReadUInt8();
	strncpy( Info.szCannon, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.byEquipment = packet.ReadUInt8();
	strncpy( Info.szEquipment, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.dwMoney = packet.ReadUInt32();
	Info.dwMinAttack = packet.ReadUInt32();
	Info.dwMaxAttack = packet.ReadUInt32();
	Info.dwCurEndure = packet.ReadUInt32();
	Info.dwMaxEndure = packet.ReadUInt32();
	Info.dwSpeed  = packet.ReadUInt32();
	Info.dwDistance = packet.ReadUInt32();
	Info.dwDefence = packet.ReadUInt32();
	Info.dwCurSupply = packet.ReadUInt32();
	Info.dwMaxSupply = packet.ReadUInt32();
	Info.dwConsume = packet.ReadUInt32();
	Info.dwAttackTime = packet.ReadUInt32();
	Info.sCapacity = packet.ReadUInt16();

	NetUpdateBoat( Info );
	return TRUE;
T_E}

BOOL SC_BoatInfo( LPRPACKET packet )
{T_B
	DWORD dwBoatID = packet.ReadUInt32();
	xShipBuildInfo Info;
	memset( &Info, 0, sizeof(xShipBuildInfo) );

	char szBoat[BOAT_MAXSIZE_NAME] = {0};	// �����Զ���
	strncpy( szBoat, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szName, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	strncpy( Info.szDesp, packet.ReadString(), BOAT_MAXSIZE_DESP - 1 );
	strncpy( Info.szBerth, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	Info.byIsUpdate = packet.ReadUInt8();
	Info.sPosID = packet.ReadUInt16();
	Info.dwBody = packet.ReadUInt32();
	strncpy( Info.szBody, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
	
	if( Info.byIsUpdate )
	{
		Info.byHeader = packet.ReadUInt8();
		Info.dwHeader = packet.ReadUInt32();
		strncpy( Info.szHeader, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

		Info.byEngine = packet.ReadUInt8();
		Info.dwEngine = packet.ReadUInt32();
		strncpy( Info.szEngine, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );
		for( int i = 0; i < BOAT_MAXNUM_MOTOR; i++ )
		{
			Info.dwMotor[i] = packet.ReadUInt32();
		}
	}
	Info.byCannon = packet.ReadUInt8();
	strncpy( Info.szCannon, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.byEquipment = packet.ReadUInt8();
	strncpy( Info.szEquipment, packet.ReadString(), BOAT_MAXSIZE_NAME - 1 );

	Info.dwMoney = packet.ReadUInt32();
	Info.dwMinAttack = packet.ReadUInt32();
	Info.dwMaxAttack = packet.ReadUInt32();
	Info.dwCurEndure = packet.ReadUInt32();
	Info.dwMaxEndure = packet.ReadUInt32();
	Info.dwSpeed  = packet.ReadUInt32();
	Info.dwDistance = packet.ReadUInt32();
	Info.dwDefence = packet.ReadUInt32();
	Info.dwCurSupply = packet.ReadUInt32();
	Info.dwMaxSupply = packet.ReadUInt32();
	Info.dwConsume = packet.ReadUInt32();
	Info.dwAttackTime = packet.ReadUInt32();
	Info.sCapacity = packet.ReadUInt16();

	NetBoatInfo( dwBoatID, szBoat, Info );
	return TRUE;
T_E}

BOOL SC_UpdateBoatPart( LPRPACKET packet )
{T_B
	return TRUE;
T_E}

BOOL SC_BoatList( LPRPACKET packet )
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	BYTE byType = packet.ReadUInt8();
	BYTE byNumBoat = packet.ReadUInt8();
	BOAT_BERTH_DATA Data;
	memset( &Data, 0, sizeof(BOAT_BERTH_DATA) );
	for( BYTE i = 0; i < byNumBoat; i++ )
	{
		strncpy( Data.szName[i], packet.ReadString(), BOAT_MAXSIZE_BOATNAME + BOAT_MAXSIZE_DESP - 1 );
	}

	NetShowBoatList( dwNpcID, byNumBoat, Data, byType );
	return TRUE;
T_E}

//BOOL	SC_BoatBagList( LPRPACKET packet )
//{
//	BYTE byNumBoat = packet.ReadUInt8();
//	BOAT_BERTH_DATA Data;
//	memset( &Data, 0, sizeof(BOAT_BERTH_DATA) );
//	for( BYTE i = 0; i < byNumBoat; i++ )
//	{
//		strncpy( Data.szName[i], packet.ReadString(), BOAT_MAXSIZE_BOATNAME - 1 );
//	}
//
//	NetShowBoatBagList( byNumBoat, Data );
//	return TRUE;
//}

BOOL SC_StallInfo( LPRPACKET packet )
{T_B
	DWORD dwCharID = packet.ReadUInt32();
	BYTE byNum = packet.ReadUInt8();
	const char* pszName = packet.ReadString();
	if( !pszName ) return FALSE;

	NetStallInfo( dwCharID, byNum, pszName );

	for( BYTE i = 0; i < byNum; ++i )
	{
		BYTE byGrid = packet.ReadUInt8();
		USHORT sItemID = packet.ReadUInt16();
		BYTE byCount = packet.ReadUInt8();
		DWORD dwMoney = packet.ReadUInt32();
		USHORT sType = packet.ReadUInt16();

		if( sType == enumItemTypeBoat )
		{
			NET_CHARTRADE_BOATDATA Data;
			memset( &Data, 0, sizeof(NET_CHARTRADE_BOATDATA) );

			if( packet.ReadUInt8() == 0 )
			{
				// ��Ϣ����
			}
			else
			{
				strncpy( Data.szName, packet.ReadString(), BOAT_MAXSIZE_BOATNAME - 1 );
				Data.sBoatID = packet.ReadUInt16();
				Data.sLevel = packet.ReadUInt16();
				Data.dwExp = packet.ReadUInt32();
				Data.dwHp = packet.ReadUInt32();
				Data.dwMaxHp = packet.ReadUInt32();
				Data.dwSp = packet.ReadUInt32();
				Data.dwMaxSp = packet.ReadUInt32();
				Data.dwMinAttack = packet.ReadUInt32();
				Data.dwMaxAttack = packet.ReadUInt32();
				Data.dwDef = packet.ReadUInt32();
				Data.dwSpeed = packet.ReadUInt32();
				Data.dwShootSpeed = packet.ReadUInt32();
				Data.byHasItem = packet.ReadUInt8();
				Data.byCapacity = packet.ReadUInt8();
				Data.dwPrice = packet.ReadUInt32();
				NetStallAddBoat( byGrid, sItemID, byCount, dwMoney, Data );
			}
		}
		else
		{
			// ��Ʒʵ������
			NET_CHARTRADE_ITEMDATA Data;
			memset( &Data, 0, sizeof(NET_CHARTRADE_ITEMDATA) );

			Data.sEndure[0] = packet.ReadUInt16();
			Data.sEndure[1] = packet.ReadUInt16();
			Data.sEnergy[0] = packet.ReadUInt16();
			Data.sEnergy[1] = packet.ReadUInt16();
			Data.byForgeLv = packet.ReadUInt8();
			Data.bValid = packet.ReadUInt8() != 0 ? true : false;
			Data.bItemTradable = packet.ReadUInt8();
			Data.expiration = packet.ReadUInt32();

			Data.lDBParam[enumITEMDBP_FORGE] = packet.ReadUInt32();
			Data.lDBParam[enumITEMDBP_INST_ID] = packet.ReadUInt32();
			


			Data.byHasAttr = packet.ReadUInt8();
			if( Data.byHasAttr )
			{
				for( int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++ )
				{
					Data.sInstAttr[i][0] = packet.ReadUInt16();
					Data.sInstAttr[i][1] = packet.ReadUInt16();
				}
			}
			NetStallAddItem( byGrid, sItemID, byCount, dwMoney, Data );
		}
	}
	return TRUE;
T_E}

BOOL SC_StallUpdateInfo( LPRPACKET packet )
{T_B
	return TRUE;
T_E}

BOOL SC_StallDelGoods( LPRPACKET packet )
{T_B
	DWORD dwCharID = packet.ReadUInt32();
	BYTE byGrid = packet.ReadUInt8();
	BYTE byCount = packet.ReadUInt8();
	NetStallDelGoods( dwCharID, byGrid, byCount );
	return TRUE;
T_E}

BOOL SC_StallClose( LPRPACKET packet )
{T_B
	DWORD dwCharID = packet.ReadUInt32();
	NetStallClose( dwCharID );
	return TRUE;
T_E}

BOOL SC_StallSuccess( LPRPACKET packet )
{T_B
	DWORD dwCharID = packet.ReadUInt32();
	NetStallSuccess( dwCharID );
	return TRUE;
T_E}

BOOL SC_SynStallName(LPRPACKET packet)
{T_B
	DWORD dwCharID = packet.ReadUInt32();
	const char	*szStallName = packet.ReadString();
	NetStallName( dwCharID, szStallName );
	return TRUE;
T_E}

BOOL SC_StartExit( LPRPACKET packet )
{T_B
	DWORD dwExitTime = packet.ReadUInt32();
	NetStartExit( dwExitTime );
	return TRUE;
T_E}

BOOL SC_CancelExit( LPRPACKET packet )
{T_B
	NetCancelExit();
	return TRUE;
T_E}

BOOL SC_UpdateHairRes( LPRPACKET packet )
{T_B
	stNetUpdateHairRes rv;
	rv.ulWorldID = packet.ReadUInt32();
	rv.nScriptID = packet.ReadUInt16();
	rv.szReason = packet.ReadString();
	rv.Exec();
	return TRUE;
T_E}

BOOL SC_OpenHairCut( LPRPACKET packet )
{T_B
	stNetOpenHair hair;
	hair.Exec();
	return TRUE;
T_E}

BOOL SC_TeamFightAsk(LPRPACKET packet)
{T_B
	char szLogName[128] = {0};
	strcpy(szLogName, g_oLangRec.GetString(305));

	stNetTeamFightAsk SFightAsk;
	SFightAsk.chSideNum2 = packet.ReverseReadUInt8();
	SFightAsk.chSideNum1 = packet.ReverseReadUInt8();
	LG(szLogName, g_oLangRec.GetString(306), SFightAsk.chSideNum1, SFightAsk.chSideNum2);
	for (char i = 0; i < SFightAsk.chSideNum1 + SFightAsk.chSideNum2; i++)
	{
		SFightAsk.Info[i].szName = packet.ReadString();
		SFightAsk.Info[i].chLv = packet.ReadUInt8();
		SFightAsk.Info[i].szJob = packet.ReadString();
		SFightAsk.Info[i].usFightNum = packet.ReadUInt16();
		SFightAsk.Info[i].usVictoryNum = packet.ReadUInt16();
		LG(szLogName, g_oLangRec.GetString(307), SFightAsk.Info[i].szName, SFightAsk.Info[i].chLv, SFightAsk.Info[i].szJob);
	}
	LG(szLogName, "\n");
	SFightAsk.Exec();
	return TRUE;
T_E}

BOOL SC_BeginItemRepair(LPRPACKET packet)
{T_B
	NetBeginRepairItem();
	return TRUE;
T_E}

BOOL SC_ItemRepairAsk(LPRPACKET packet)
{T_B
	stNetItemRepairAsk	SRepairAsk;
	SRepairAsk.cszItemName = packet.ReadString();
	SRepairAsk.lRepairMoney = packet.ReadUInt32();
	SRepairAsk.Exec();

	return TRUE;
T_E}

BOOL SC_ItemForgeAsk(LPRPACKET packet)
{T_B
	stSCNetItemForgeAsk	SForgeAsk;
	SForgeAsk.chType = packet.ReadUInt8();
	SForgeAsk.lMoney = packet.ReadUInt32();
	SForgeAsk.Exec();

	return TRUE;
T_E}

BOOL SC_ItemForgeAnswer(LPRPACKET packet)
{T_B
	stNetItemForgeAnswer	SForgeAnswer;
	SForgeAnswer.lChaID = packet.ReadUInt32();
	SForgeAnswer.chType = packet.ReadUInt8();
	SForgeAnswer.chResult = packet.ReadUInt8();
	SForgeAnswer.Exec();

	return TRUE;
T_E}

BOOL SC_ItemUseSuc(LPRPACKET packet)
{T_B
	unsigned int nChaID = packet.ReadUInt32();
	short sItemID = packet.ReadUInt16();
	NetItemUseSuccess(nChaID, sItemID);

	return TRUE;
T_E}

BOOL SC_KitbagCapacity(LPRPACKET packet)
{T_B
	unsigned int nChaID = packet.ReadUInt32();
	short sKbCap = packet.ReadUInt16();
	NetKitbagCapacity(nChaID, sKbCap);

	return TRUE;
T_E}

BOOL SC_EspeItem(LPRPACKET packet)
{T_B
	stNetEspeItem	SEspItem;
	unsigned int nChaID = packet.ReadUInt32();
	SEspItem.chNum = packet.ReadUInt8();
	for(int i = 0; i < 1; i++)
	{
		SEspItem.SContent[i].sPos = packet.ReadUInt16();
		SEspItem.SContent[i].sEndure = packet.ReadUInt16();
		SEspItem.SContent[i].sEnergy = packet.ReadUInt16();
		SEspItem.SContent[i].bItemTradable = packet.ReadUInt8();
		SEspItem.SContent[i].expiration = packet.ReadUInt32();
	}

	NetEspeItem( nChaID, SEspItem );
	return TRUE;
T_E}

BOOL   SC_MapCrash(LPRPACKET packet)
{T_B
	const char* pszDesp = packet.ReadString();
	if( pszDesp == NULL ) 
		return FALSE;

	NetShowMapCrash(pszDesp);
	return TRUE;
T_E}

BOOL	SC_Message(LPRPACKET pk)
{T_B
	/*
	uLong l_id = pk.ReadUInt32();

    NetShowMessage( pk.ReadUInt32() );
	return TRUE;
	*/
	const char* pszDesp = pk.ReadString();
	if( pszDesp == NULL ) 
		return FALSE;

	NetShowMapCrash(pszDesp);
	return TRUE;


T_E}

BOOL SC_QueryCha(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();

	stNetSysInfo	SShowInfo;
	char	szInfo[512] = "";
	const char	*pChaName = pk.ReadString();
	const char	*pMapName = pk.ReadString();
	long	lPosX = pk.ReadUInt32();
	long	lPosY = pk.ReadUInt32();
	long	lChaID = pk.ReadUInt32();
	sprintf(szInfo, g_oLangRec.GetString(308), pChaName, lChaID, pMapName, lPosX, lPosY);
	SShowInfo.m_sysinfo = szInfo;
	NetSysInfo(SShowInfo);

	return TRUE;
T_E}

BOOL SC_QueryChaItem(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();

	return TRUE;
T_E}

BOOL SC_QueryChaPing(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();

	stNetSysInfo	SShowInfo;
	char	szInfo[512] = "";
	const char	*pChaName = pk.ReadString();
	const char	*pMapName = pk.ReadString();
	long	lPing = pk.ReadUInt32();
	sprintf(szInfo, g_oLangRec.GetString(309), pMapName, lPing);
	SShowInfo.m_sysinfo = szInfo;
	NetSysInfo(SShowInfo);

	return TRUE;
T_E}

BOOL SC_QueryRelive(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();

	stNetQueryRelive SQueryRelive;
	SQueryRelive.szSrcChaName = pk.ReadString();
	SQueryRelive.chType = pk.ReadUInt8();
	NetQueryRelive(l_id, SQueryRelive);

	return TRUE;
T_E}

BOOL SC_PreMoveTime(LPRPACKET pk)
{T_B
	uLong ulPreMoveTime = pk.ReadUInt32();
	NetPreMoveTime(ulPreMoveTime);

	return TRUE;
T_E}

BOOL SC_MapMask(LPRPACKET pk)
{T_B
	uLong	l_id = pk.ReadUInt32();
	uShort	usLen = 0;
	BYTE	*pMapMask = 0;
	if (pk.ReadUInt8())
		pMapMask = (BYTE*)pk.ReadSequence(usLen);

	//char	*szMask = new char[usLen + 1];
	//memcpy(szMask, pMapMask, usLen);
	//szMask[usLen] = '\0';
	//LG("���ͼ", "%s\n", szMask);

	NetMapMask(l_id, pMapMask, usLen);

	return TRUE;
T_E}

BOOL SC_SynEventInfo(LPRPACKET pk)
{T_B
	stNetEvent	SNetEvent;
	ReadEntEventPacket(pk, SNetEvent);
	
	SNetEvent.ChangeEvent();
	return TRUE;
T_E}

BOOL SC_SynSideInfo(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();
	const char* szLogName = g_LogName.GetLogName( l_id );
	stNetChaSideInfo	SNetSideInfo;
	ReadChaSidePacket(pk, SNetSideInfo, szLogName);
	NetChaSideInfo(l_id, SNetSideInfo);

	return TRUE;
T_E}

BOOL SC_SynAppendLook(LPRPACKET pk)
{T_B
	uLong l_id = pk.ReadUInt32();
	const char* szLogName = g_LogName.GetLogName( l_id );
	stNetAppendLook	SNetAppendLook;
	ReadChaAppendLookPacket(pk, SNetAppendLook, szLogName);
	SNetAppendLook.Exec(l_id);

	return TRUE;
T_E}

BOOL SC_KitbagCheckAnswer(LPRPACKET packet)
{T_B
	bool bLock = packet.ReadUInt8() ? true : false;
	NetKitbagCheckAnswer(bLock);

    return TRUE;
T_E}

BOOL SC_StoreOpenAnswer(LPRPACKET packet)
{T_B
	bool bValid = packet.ReadUInt8() ? true : false; // �̳��Ƿ���
	if(bValid)
	{
		g_stUIStore.ClearStoreTreeNode();
		g_stUIStore.ClearStoreItemList();
		//g_stUIStore.SetStoreBuyButtonEnable(true);

		long lVip = packet.ReadUInt32(); // VIP
		long lMoBean   = packet.ReadUInt32();	// Ħ��
		long lRplMoney = packet.ReadUInt32();	// ���ң�ˮ����
		g_stUIStore.SetStoreMoney(lMoBean, lRplMoney);
		g_stUIStore.SetStoreVip(lVip);

		long lAfficheNum = packet.ReadUInt32(); // ��������
		int i;
		for(i = 0; i < lAfficheNum; i++)
		{
			long lAfficheID = packet.ReadUInt32(); // ����ID
			uShort len;
			cChar *szTitle = packet.ReadSequence(len); // �������
			cChar *szComID = packet.ReadSequence(len); // ��Ӧ��ƷID,�ö��Ÿ���

			// ���ӹ���
		}
		long lFirstClass = 0;
		long lClsNum = packet.ReadUInt32(); // ��������
		for(i = 0; i < lClsNum; i++)
		{
			short lClsID = packet.ReadUInt16(); // ����ID
			uShort len;
			cChar *szClsName = packet.ReadSequence(len); // ������
			short lParentID = packet.ReadUInt16(); // ����ID

			// �����̳������
			g_stUIStore.AddStoreTreeNode(lParentID, lClsID, szClsName);

			// �ύ��һ������
			if(i == 0)
			{
				lFirstClass = lClsID;
			}
		}

		g_stUIStore.AddStoreUserTreeNode();// ���Ӹ��˹���
		g_stUIStore.ShowStoreForm(true);

		if(lFirstClass > 0)
		{
			::CS_StoreListAsk(lFirstClass, 1, (short)CStoreMgr::GetPageSize());
		}
	}
	else
	{
		// �̳�δ����,����ҳ
		g_stUIStore.ShowStoreWebPage();
	}

	g_stUIStore.DarkScene(false);
	g_stUIStore.ShowStoreLoad(false);

	return TRUE;
T_E}

BOOL SC_StoreListAnswer(LPRPACKET packet)
{T_B
	g_stUIStore.ClearStoreItemList();

	short sPageNum = packet.ReadUInt16(); // ҳ��
	short sPageCur = packet.ReadUInt16(); // ��ǰҳ
	short sComNum = packet.ReadUInt16(); // ��Ʒ��

	long i;
	for(i = 0; i < sComNum; i++)
	{
		long lComID = packet.ReadUInt32(); // ��ƷID
		uShort len;
		cChar *szComName = packet.ReadSequence(len); // ��Ʒ����
		long lPrice = packet.ReadUInt32(); // ��Ʒ�۸�
		cChar *szRemark = packet.ReadSequence(len); // ��Ʒ��ע
		bool isHot = packet.ReadUInt8() ? true : false;	// �Ƿ���������Ʒ
		long nTime = packet.ReadUInt32();
		long lComNumber = packet.ReadUInt32();	// ��Ʒʣ�������-1������
		long lComExpire = packet.ReadUInt32();	// ��Ʒ����ʱ��

		// ���ӵ��̳�
		g_stUIStore.AddStoreItemInfo(i, lComID, szComName, lPrice, szRemark, isHot, nTime, lComNumber, lComExpire);

		short sItemClsNum = packet.ReadUInt16(); // ������������
		int j;
		for(j = 0; j < sItemClsNum; j++)
		{
			short sItemID = packet.ReadUInt16(); // ����ID
			short sItemNum = packet.ReadUInt16(); // ��������
			short sFlute = packet.ReadUInt16(); // ��������
			short pItemAttrID[5];
			short pItemAttrVal[5];
			int k;
			for(k = 0; k < 5; k++)
			{
				pItemAttrID[k] = packet.ReadUInt16(); // ����ID
				pItemAttrVal[k] = packet.ReadUInt16(); // ����ֵ
			}

			// ��Ʒϸ������
			g_stUIStore.AddStoreItemDetail(i, j, sItemID, sItemNum, sFlute, pItemAttrID, pItemAttrVal);
		}
	}

	// ����ҳ��
	g_stUIStore.SetStoreItemPage(sPageCur, sPageNum);

	return TRUE;
T_E}

BOOL SC_StoreBuyAnswer(LPRPACKET packet)
{T_B
	bool bSucc = packet.ReadUInt8() ? true : false; // �����Ƿ�ɹ�
	long lMoBean = 0;
	long lRplMoney = 0;

	// ֪ͨ��ҹ���ɹ�
	// ...
	if(bSucc)
	{
		//lMoBean = packet.ReadUInt32();
		lRplMoney = packet.ReadUInt32();
		g_stUIEquip.UpdateIMP(lRplMoney);
		g_stUIStore.SetStoreMoney(-1, lRplMoney);
	}
	else
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(907)); // �������ʧ��!
	}

	g_stUIStore.SetStoreBuyButtonEnable(true);
	return TRUE;
T_E}

BOOL SC_StoreChangeAnswer(LPRPACKET packet)
{T_B
	bool bSucc = packet.ReadUInt8() ? true : false; // �һ������Ƿ�ɹ�
	if(bSucc)
	{
		long lMoBean = packet.ReadUInt32(); // ��ǰĦ������
		long lRplMoney = packet.ReadUInt32(); // ��ǰ��������

		g_stUIStore.SetStoreMoney(lMoBean, lRplMoney);
	}
	else
	{
		g_pGameApp->MsgBox(g_oLangRec.GetString(908));	// ���Ҷһ�ʧ��!
	}
	return TRUE;
T_E}

BOOL SC_StoreHistory(LPRPACKET packet)
{T_B
	long lNum = packet.ReadUInt32(); // ��ȡ�Ľ��׼�¼����
	int i;
	for(i = 0; i < lNum; i++)
	{
		uShort len;
		cChar *szTime = packet.ReadSequence(len); // ����ʱ��
		long lComID = packet.ReadUInt32(); // ��ƷID
		cChar *szName = packet.ReadSequence(len); // ��Ʒ����
		long lMoney = packet.ReadUInt32(); // ���׽��
	}
	return TRUE;
T_E}

BOOL SC_ActInfo(LPRPACKET packet)
{T_B
	bool bSucc = packet.ReadUInt8() ? true : false; // �ʺ���Ϣ��ѯ�Ƿ�ɹ�
	if(bSucc)
	{
		long lMoBean = packet.ReadUInt32(); // Ħ������
		long lRplMoney = packet.ReadUInt32(); // ��������
	}
	return TRUE;
T_E}

BOOL SC_StoreVIP(LPRPACKET packet)
{T_B
	bool bSucc = packet.ReadUInt8() ? true : false;
	if(bSucc)
	{
		short sVipID = packet.ReadUInt16();
		short sMonth = packet.ReadUInt16();
		long lShuijing = packet.ReadUInt32();
		long lModou = packet.ReadUInt32();

		g_stUIStore.SetStoreVip(sVipID);
		g_stUIStore.SetStoreMoney(lModou, lShuijing);
	}
	return TRUE;
T_E}

BOOL SC_BlackMarketExchangeData(LPRPACKET packet)
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	g_stUIBlackTrade.SetNpcID(dwNpcID);

	stBlackTrade SBlackTrade;
	short sCount = packet.ReadUInt16();
	for(short sIndex = 0; sIndex < sCount; ++sIndex)
	{
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex   = sIndex;
		SBlackTrade.sSrcID   = packet.ReadUInt16();		// ������ƷID
		SBlackTrade.sSrcNum  = packet.ReadUInt16();		// ������Ʒ����
		SBlackTrade.sTarID   = packet.ReadUInt16();		// Ŀ����ƷID
		SBlackTrade.sTarNum  = packet.ReadUInt16();		// Ŀ����Ʒ����
		SBlackTrade.sTimeNum = packet.ReadUInt16();		// timeֵ

		g_stUIBlackTrade.SetItem(& SBlackTrade);
	}

	g_stUIBlackTrade.ShowBlackTradeForm(true);

	return TRUE;
T_E}

BOOL SC_ExchangeData(LPRPACKET packet)
{T_B
	DWORD dwNpcID = packet.ReadUInt32();
	g_stUIBlackTrade.SetNpcID(dwNpcID);

	stBlackTrade SBlackTrade;

	short sCount = packet.ReadUInt16();
	for(short sIndex = 0; sIndex < sCount; ++sIndex)
	{
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex   = sIndex;
		SBlackTrade.sSrcID   = packet.ReadUInt16();		// ������ƷID
		SBlackTrade.sSrcNum  = packet.ReadUInt16();		// ������Ʒ����
		SBlackTrade.sTarID   = packet.ReadUInt16();		// Ŀ����ƷID
		SBlackTrade.sTarNum  = packet.ReadUInt16();		// Ŀ����Ʒ����
		SBlackTrade.sTimeNum = 0;

		g_stUIBlackTrade.SetItem(& SBlackTrade);
	}

	g_stUIBlackTrade.ShowBlackTradeForm(true);

	return TRUE;
T_E}

BOOL SC_BlackMarketExchangeUpdate(LPRPACKET packet)
{T_B
	//���жһ�����,ֻ���ڴ򿪺��жһ���������²Ÿ���!!!�м�

	DWORD dwNpcID = packet.ReadUInt32();
	stBlackTrade SBlackTrade;

	// ��ԭ�ȵĺ��жһ��������
	g_stUIBlackTrade.ClearItemData();

	short sCount = packet.ReadUInt16();
	for(short sIndex = 0; sIndex < sCount; ++sIndex)
	{
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex   = sIndex;
		SBlackTrade.sSrcID   = packet.ReadUInt16();		// ������ƷID
		SBlackTrade.sSrcNum  = packet.ReadUInt16();		// ������Ʒ����
		SBlackTrade.sTarID   = packet.ReadUInt16();		// Ŀ����ƷID
		SBlackTrade.sTarNum  = packet.ReadUInt16();		// Ŀ����Ʒ����
		SBlackTrade.sTimeNum = packet.ReadUInt16();		// timeֵ

		if(g_stUIBlackTrade.GetIsShow() && g_stUIBlackTrade.GetNpcID() == dwNpcID)
		{
			g_stUIBlackTrade.SetItem(& SBlackTrade);
		}
	}

	if(g_stUIBlackTrade.GetIsShow() && g_stUIBlackTrade.GetNpcID() == dwNpcID)
	{
		g_stUIBlackTrade.ShowBlackTradeForm(true);
	}

	return TRUE;
T_E}

BOOL SC_BlackMarketExchangeAsr(LPRPACKET packet)
{T_B
	bool bSucc = (packet.ReadUInt8() == 1) ? true : false;
	if(bSucc)
	{
		stBlackTrade SBlackTrade;
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sSrcID  = packet.ReadUInt16();
		SBlackTrade.sSrcNum = packet.ReadUInt16();
		SBlackTrade.sTarID  = packet.ReadUInt16();
		SBlackTrade.sTarNum = packet.ReadUInt16();

		g_stUIBlackTrade.ExchangeAnswerProc(bSucc, &SBlackTrade);
	}

	return TRUE;
T_E}

BOOL SC_TigerItemID(LPRPACKET packet)
{T_B
	short sNum = packet.ReadUInt16();	// ����
	short sItemID[3] = {0};

	for(int i = 0; i < 3; i++)
	{
		sItemID[i] = packet.ReadUInt16();
	}

	g_stUISpirit.UpdateErnieNumber(sNum, sItemID[0], sItemID[1], sItemID[2]);

	if(sNum == 3)
	{
		// ���һ��
		g_stUISpirit.ShowErnieHighLight();
	}

	return TRUE;
T_E}

BOOL SC_VolunteerList(LPRPACKET packet)
{T_B
	short sPageNum = packet.ReadUInt16();	//��ҳ��
	short sPage = packet.ReadUInt16();		//��ǰҳ��
	short sRetNum = packet.ReadUInt16();		//־Ը������

	g_stUIFindTeam.SetFindTeamPage(sPage, sPageNum);
	g_stUIFindTeam.RemoveTeamInfo();

	for(int i = 0; i < sRetNum; i++)
	{
		const char *szName = packet.ReadString();
		long level = packet.ReadUInt32();
		long job = packet.ReadUInt32();
		const char *szMapName = packet.ReadString();

		g_stUIFindTeam.AddFindTeamInfo(i, szName, level, job, szMapName);
	}

	return TRUE;
T_E}

BOOL SC_VolunteerState(LPRPACKET packet)
{T_B
	bool bState = (packet.ReadUInt8() == 0) ? false : true;
	g_stUIFindTeam.SetOwnFindTeamState(bState);

	return TRUE;
T_E}

BOOL SC_VolunteerOpen(LPRPACKET packet)
{T_B
	bool bState = (packet.ReadUInt8() == 0) ? false : true;
	short sPageNum = packet.ReadUInt16();	//��ҳ��
	short sRetNum = packet.ReadUInt16();		//־Ը������

	g_stUIFindTeam.SetOwnFindTeamState(bState);
	g_stUIFindTeam.SetFindTeamPage(1, sPageNum <= 0 ? 1 : sPageNum);
	g_stUIFindTeam.RemoveTeamInfo();

	for(int i = 0; i < sRetNum; i++)
	{
		const char *szName = packet.ReadString();
		long level = packet.ReadUInt32();
		long job = packet.ReadUInt32();
		const char *szMapName = packet.ReadString();

		g_stUIFindTeam.AddFindTeamInfo(i, szName, level, job, szMapName);
	}

	g_stUIFindTeam.ShowFindTeamForm();

	return TRUE;
T_E}

BOOL SC_VolunteerAsk(LPRPACKET packet)
{T_B
	const char *szName = packet.ReadString();
	g_stUIFindTeam.FindTeamAsk(szName);
	
	return TRUE;
T_E}

BOOL SC_SyncKitbagTemp(LPRPACKET packet)
{
	/*stNetActorCreate SCreateInfo;
	ReadChaBasePacket(packet, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 1;
	SCreateInfo.CreateCha();*/
	//g_ulWorldID = SCreateInfo.ulWorldID;

	stNetKitbag SKitbagTemp;
	ReadChaKitbagPacket(packet, SKitbagTemp, "KitbagTemp");
	SKitbagTemp.chBagType = 2;
	NetChangeKitbag(g_ulWorldID, SKitbagTemp);

	//CWorldScene* pWorldScene = dynamic_cast<CWorldScene*>(g_pGameApp->GetCurScene());
	//if(pWorldScene)

	return TRUE;
}

BOOL SC_SyncTigerString(LPRPACKET packet)
{
	const char *szString = packet.ReadString();
	g_stUISpirit.UpdateErnieString(szString);

	return TRUE;
}

BOOL SC_MasterAsk(LPRPACKET packet)
{
	const char *szName = packet.ReadString(); // ����
	DWORD dwCharID = packet.ReadUInt32();
	g_stUIChat.MasterAsk(szName, dwCharID);
	return TRUE;
}

BOOL SC_PrenticeAsk(LPRPACKET packet)
{
	const char *szName = packet.ReadString(); // ����
	DWORD dwCharID = packet.ReadUInt32();
	g_stUIChat.PrenticeAsk(szName, dwCharID);
	return TRUE;
}

BOOL PC_MasterRefresh(LPRPACKET packet)
{
	unsigned char l_type =packet.ReadUInt8();
	switch (l_type)
	{
	case MSG_MASTER_REFRESH_ONLINE:
		{
			uLong ulChaID = packet.ReadUInt32();
			NetMasterOnline(ulChaID);
		}
		break;
	case MSG_MASTER_REFRESH_OFFLINE:
		{
			uLong ulChaID = packet.ReadUInt32();
			NetMasterOffline(ulChaID);
		}
		break;
	case MSG_MASTER_REFRESH_DEL:
		{
			uLong ulChaID = packet.ReadUInt32();
			NetMasterDel(ulChaID);
		}
		break;
	case MSG_MASTER_REFRESH_ADD:
		{
			cChar	*l_grp		=packet.ReadString();
			uLong	l_chaid		=packet.ReadUInt32();
			cChar	*l_chaname	=packet.ReadString();
			cChar	*l_motto	=packet.ReadString();
			uShort	l_icon		=packet.ReadUInt16();
			NetMasterAdd(l_chaid,l_chaname,l_motto,l_icon,l_grp);
		}
		break;
	case MSG_MASTER_REFRESH_START:
		{
			stNetFrndStart l_self;
			l_self.lChaid	=packet.ReadUInt32();
			l_self.szChaname=packet.ReadString();
			l_self.szMotto	=packet.ReadString();
			l_self.sIconID	=packet.ReadUInt16();
			stNetFrndStart l_nfs[100];
			uShort	l_nfnum=0,l_grpnum	=packet.ReadUInt16();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++)
			{
				cChar*	l_grp		=packet.ReadString();
				uShort	l_grpmnum	=packet.ReadUInt16();
				for(uShort l_grpmi =0;l_grpmi<l_grpmnum;l_grpmi++)
				{
					l_nfs[l_nfnum].szGroup	=l_grp;
					l_nfs[l_nfnum].lChaid	=packet.ReadUInt32();
					l_nfs[l_nfnum].szChaname=packet.ReadString();
					l_nfs[l_nfnum].szMotto	=packet.ReadString();
					l_nfs[l_nfnum].sIconID	=packet.ReadUInt16();
					l_nfs[l_nfnum].cStatus	=packet.ReadUInt8();
					l_nfnum	++;
				}
			}
			NetMasterStart(l_self,l_nfs,l_nfnum);
		}
		break;

	case MSG_PRENTICE_REFRESH_ONLINE:
		{
			uLong ulChaID = packet.ReadUInt32();
			NetPrenticeOnline(ulChaID);
		}
		break;
	case MSG_PRENTICE_REFRESH_OFFLINE:
		{
			uLong ulChaID = packet.ReadUInt32();
			NetPrenticeOffline(ulChaID);
		}
		break;
	case MSG_PRENTICE_REFRESH_DEL:
		{
			uLong ulChaID = packet.ReadUInt32();
			NetPrenticeDel(ulChaID);
		}
		break;
	case MSG_PRENTICE_REFRESH_ADD:
		{
			cChar	*l_grp		=packet.ReadString();
			uLong	l_chaid		=packet.ReadUInt32();
			cChar	*l_chaname	=packet.ReadString();
			cChar	*l_motto	=packet.ReadString();
			uShort	l_icon		=packet.ReadUInt16();
			NetPrenticeAdd(l_chaid,l_chaname,l_motto,l_icon,l_grp);
		}
		break;
	case MSG_PRENTICE_REFRESH_START:
		{
			stNetFrndStart l_self;
			l_self.lChaid	=packet.ReadUInt32();
			l_self.szChaname=packet.ReadString();
			l_self.szMotto	=packet.ReadString();
			l_self.sIconID	=packet.ReadUInt16();
			stNetFrndStart l_nfs[100];
			uShort	l_nfnum=0,l_grpnum	=packet.ReadUInt16();
			for(uShort l_grpi =0;l_grpi<l_grpnum;l_grpi++)
			{
				cChar*	l_grp		=packet.ReadString();
				uShort	l_grpmnum	=packet.ReadUInt16();
				for(uShort l_grpmi =0;l_grpmi<l_grpmnum;l_grpmi++)
				{
					uShort index = l_grpmi % (sizeof(l_nfs) / sizeof(l_nfs[0]));
					l_nfs[index].szGroup	= l_grp;
					l_nfs[index].lChaid		= packet.ReadUInt32();
					l_nfs[index].szChaname	= packet.ReadString();
					l_nfs[index].szMotto	= packet.ReadString();
					l_nfs[index].sIconID	= packet.ReadUInt16();
					l_nfs[index].cStatus	= packet.ReadUInt8();
					l_nfnum	++;
				}
			}
			NetPrenticeStart(l_self,l_nfs,min(l_nfnum, (sizeof(l_nfs) / sizeof(l_nfs[0]))));
		}
		break;
	}
	return TRUE;
}

BOOL PC_MasterCancel(LPRPACKET packet)
{
	unsigned char reason =packet.ReadUInt8();
	uLong ulChaID = packet.ReadUInt32();
	NetMasterCancel(packet.ReadUInt32(),reason);
	return TRUE;
}

BOOL PC_MasterRefreshInfo(LPRPACKET packet)
{
	unsigned long l_chaid	=packet.ReadUInt32();
	const char	* l_motto	=packet.ReadString();
	unsigned short l_icon	=packet.ReadUInt16();
	unsigned short l_degr	=packet.ReadUInt16();
	const char	* l_job		=packet.ReadString();
	const char	* l_guild	=packet.ReadString();
	NetMasterRefreshInfo(l_chaid,l_motto,l_icon,l_degr,l_job,l_guild);
	return TRUE;
}

BOOL PC_PrenticeRefreshInfo(LPRPACKET packet)
{
	unsigned long l_chaid	=packet.ReadUInt32();
	const char	* l_motto	=packet.ReadString();
	unsigned short l_icon	=packet.ReadUInt16();
	unsigned short l_degr	=packet.ReadUInt16();
	const char	* l_job		=packet.ReadString();
	const char	* l_guild	=packet.ReadString();
	NetPrenticeRefreshInfo(l_chaid,l_motto,l_icon,l_degr,l_job,l_guild);
	return TRUE;
}

BOOL SC_ChaPlayEffect(LPRPACKET packet)
{
	unsigned int uiWorldID = packet.ReadUInt32();
	int nEffectID = packet.ReadUInt32();

	NetChaPlayEffect(uiWorldID, nEffectID);

	return TRUE;
}

BOOL SC_Say2Camp(LPRPACKET packet)
{
	cChar *szName = packet.ReadString();
	cChar *szContent = packet.ReadString();

	//g_pGameApp->SysInfo("[��Ӫ]%s:%s", szName, szContent);
	NetSideInfo(szName, szContent);

	return TRUE;
}

BOOL SC_GMMail(LPRPACKET packet)
{
	cChar *szTitle = packet.ReadString();
	cChar *szContent = packet.ReadString();
	long lTime = packet.ReadUInt32();

	g_stUIMail.ShowAnswerForm(szTitle, szContent);

	return TRUE;
}

BOOL SC_CheatCheck(LPRPACKET packet)
{
	short count = packet.ReadUInt16(); // ͼƬ����
	for(int i = 0; i < count; i++)
	{
		char *picture = NULL;
		short size = packet.ReadUInt16(); // ͼƬ��С
		picture = new char[size];
		for(int j = 0; j < size; j++)
		{
			picture[j] = packet.ReadUInt8();
		}

		g_stUINumAnswer.SetBmp(i, (BYTE*)picture, size);

		delete [] picture;
	}

	g_stUINumAnswer.Refresh();
	g_stUINumAnswer.ShowForm();

	return TRUE;
}

BOOL SC_ListAuction(LPRPACKET pk)
{
	short sNum = pk.ReverseReadUInt16(); // ��������
	stChurchChallenge stInfo;

	for(int i = 0; i < sNum; i++)
	{
		short  sItemID   = pk.ReadUInt16();	// id
		cChar* szName    = pk.ReadString();	// name
		cChar* szChaName = pk.ReadString();	// ��ս��
		short  sCount    = pk.ReadUInt16();	// ����
		long   baseprice = pk.ReadUInt32();	// �׼�
		long   minbid    = pk.ReadUInt32();	// ��ͳ���
		long   curprice  = pk.ReadUInt32();	// ��ǰ���ļ�

		stInfo.sChurchID  = sItemID;
		stInfo.sCount     = sCount;
		stInfo.nBasePrice = baseprice;
		stInfo.nMinbid    = minbid;
		stInfo.nCurPrice  = curprice;
		strncpy(stInfo.szChaName, szChaName, sizeof(stInfo.szChaName) - 1);
		strncpy(stInfo.szName, szName, sizeof(stInfo.szName) - 1);
	}

	NetChurchChallenge(&stInfo);

	return TRUE;
}

BOOL SC_RequestDropRate(LPRPACKET pk) {
	float rate = pk.ReadFloat32();
	g_DropBonus = rate;
	if(!g_stUIStart.frmMonsterInfo->GetIsShow()) {
		g_stUIStart.frmMonsterInfo->Show();
	}
	g_stUIStart.SetMonsterInfo();
	g_pGameApp->Waiting(false);
	return true;
}

BOOL SC_RequestExpRate(LPRPACKET pk) {
	float rate = pk.ReadFloat32();
	g_ExpBonus = rate;
	return true;
}

BOOL SC_RefreshSelectScreen(LPRPACKET pk) {
	const char chaDelSlot = pk.ReadUInt8();
	const auto characters = ReadSelectCharacters(pk);
	if (g_pGameApp->GetCurScene()->GetSceneTypeID() != enumSelectChaScene)
	{
		return true;
	}

	if (chaDelSlot != -1) {
		CSelectChaScene& rkScene = CSelectChaScene::GetCurrScene();
		rkScene.m_nCurChaIndex = chaDelSlot;
		rkScene.DelCurrentSelCha();
	}

	CSelectChaScene::GetCurrScene().SelectCharacters(characters);
	
	return true;
}





// �������ݱ�=======================================================================
void ReadChaBasePacket(LPRPACKET pk, stNetActorCreate &SCreateInfo)
{T_B
	// ��������
	SCreateInfo.ulChaID		= pk.ReadUInt32();
	SCreateInfo.ulWorldID	= pk.ReadUInt32();
	SCreateInfo.ulCommID	= pk.ReadUInt32();
	SCreateInfo.szCommName	= pk.ReadString();
	SCreateInfo.chGMLv		= pk.ReadUInt8();
	SCreateInfo.lHandle		= pk.ReadUInt32();
	SCreateInfo.chCtrlType	= pk.ReadUInt8();	// ��ɫ���ͣ��ο�CompCommand.h EChaCtrlType��
	SCreateInfo.szName = pk.ReadString();
	SCreateInfo.strMottoName = pk.ReadString(); // ����������
	SCreateInfo.sIcon       = pk.ReadUInt16();   // ��ɫ����ͷ��
	SCreateInfo.lGuildID = pk.ReadUInt32();
	SCreateInfo.strGuildName = pk.ReadString(); // ������
	SCreateInfo.strGuildMotto = pk.ReadString(); // ����������
	SCreateInfo.chGuildPermission = pk.ReadUInt32(); // ����������
	SCreateInfo.strStallName = pk.ReadString(); // ����������
	SCreateInfo.sState = pk.ReadUInt16();
	SCreateInfo.SArea.centre.x = pk.ReadUInt32();
	SCreateInfo.SArea.centre.y = pk.ReadUInt32();
	SCreateInfo.SArea.radius = pk.ReadUInt32();
	SCreateInfo.sAngle = pk.ReadUInt16();
	SCreateInfo.ulTLeaderID = pk.ReadUInt32(); // �ӳ�ID
	
	SCreateInfo.chIsPlayer = pk.ReadUInt8();

	const char* szLogName = g_LogName.SetLogName( SCreateInfo.ulWorldID, SCreateInfo.szName );
	ReadChaSidePacket(pk, SCreateInfo.SSideInfo, szLogName);
	ReadEntEventPacket(pk, SCreateInfo.SEvent, szLogName);
	ReadChaLookPacket(pk, SCreateInfo.SLookInfo, szLogName);
	ReadChaPKPacket(pk, SCreateInfo.SPKCtrl, szLogName);
	ReadChaAppendLookPacket(pk, SCreateInfo.SAppendLook, szLogName);

	LG(szLogName, "+++++++++++++Create(State: %u\tPos: [%d, %d]\n", SCreateInfo.sState, SCreateInfo.SArea.centre.x, SCreateInfo.SArea.centre.y);
	LG(szLogName, "CtrlType:%d, TeamdID:%u\n", SCreateInfo.chCtrlType, SCreateInfo.ulTLeaderID );

T_E}

BOOL ReadChaSkillBagPacket(LPRPACKET pk, stNetSkillBag &SCurSkill, const char *szLogName)
{T_B
	memset(&SCurSkill, 0, sizeof(SCurSkill));
	stNetDefaultSkill	SDefaultSkill;
	SDefaultSkill.sSkillID = pk.ReadUInt16();
	SDefaultSkill.Exec();

	SCurSkill.chType = pk.ReadUInt8();
	short sSkillNum = pk.ReadUInt16();
	if (sSkillNum <= 0)
		return TRUE;

	SCurSkill.SBag.Resize( sSkillNum );
	SSkillGridEx* pSBag = SCurSkill.SBag.GetValue();
	short i = 0;
	for (; i < sSkillNum; i++)
	{
		pSBag[i].sID = pk.ReadUInt16();
		pSBag[i].chState = pk.ReadUInt8();
		pSBag[i].chLv = pk.ReadUInt8();
		pSBag[i].sUseSP = pk.ReadUInt16();
		pSBag[i].sUseEndure = pk.ReadUInt16();
		pSBag[i].sUseEnergy = pk.ReadUInt16();
		pSBag[i].lResumeTime = pk.ReadUInt32();
		pSBag[i].sRange[0] = pk.ReadUInt16();
		if (pSBag[i].sRange[0] != enumRANGE_TYPE_NONE)
			for (short j = 1; j < defSKILL_RANGE_PARAM_NUM; j++)
				pSBag[i].sRange[j] = pk.ReadUInt16();
	}

	// log
	LG(szLogName, "Syn Skill Bag, Type:%d,\tTick:[%u]\n", SCurSkill.chType, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(310));
	char	szRange[256];
	for (i = 0; i < sSkillNum; i++)
	{
		sprintf(szRange, "%d", pSBag[i].sRange[0]);
		if (pSBag[i].sRange[0] != enumRANGE_TYPE_NONE)
			for (short j = 1; j < defSKILL_RANGE_PARAM_NUM; j++)
				sprintf(szRange + strlen(szRange), ",%d", pSBag[i].sRange[j]);
		LG(szLogName, "\t%4d\t%4d\t%4d\t%6d\t%6d\t%6d\t%18d\t%s\n", pSBag[i].sID, pSBag[i].chState, pSBag[i].chLv, pSBag[i].sUseSP, pSBag[i].sUseEndure, pSBag[i].sUseEnergy, pSBag[i].lResumeTime, szRange);
	}
	LG(szLogName, "\n");
	//

	return TRUE;
T_E}

void ReadChaSkillStatePacket(LPRPACKET pk, stNetSkillState &SCurSState, const char* szLogName)
{T_B
	unsigned long currentClient = GetTickCount();	
	unsigned long currentServer = pk.ReadUInt32()/1000;//current server time
	memset(&SCurSState, 0, sizeof(SCurSState));
	SCurSState.chType = 0;
	short sNum = pk.ReadUInt8();
	if ( sNum>0 )
	{
		SCurSState.SState.Resize( sNum );
		for (int nNum = 0; nNum < sNum; nNum++)
		{
			SCurSState.SState[nNum].chID = pk.ReadUInt8();
			SCurSState.SState[nNum].chLv = pk.ReadUInt8();
			
			
			unsigned long duration = pk.ReadUInt32();//duration
			unsigned long start = pk.ReadUInt32()/1000;//start time
			
			
			unsigned long dif = currentServer - currentClient;
			unsigned long end = start - dif + duration;
			
			SCurSState.SState[nNum].lTimeRemaining = duration==0?0:end-currentClient; //end time, corrected for client
		}
	}

	// log
	LG(szLogName, "Syn Skill State: Num[%d]\tTick[%u]\n", sNum, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(311));
	for (char i = 0; i < sNum; i++)
		LG(szLogName, "\t%8d\t%4d\n", SCurSState.SState[i].chID, SCurSState.SState[i].chLv);
	LG(szLogName, "\n");
	//
T_E}

void ReadChaAttrPacket(LPRPACKET pk, stNetChaAttr& SChaAttr, const  char* szLogName)
{T_B
	memset(&SChaAttr, 0, sizeof(SChaAttr));
	SChaAttr.chType = pk.ReadUInt8();
	SChaAttr.sNum = pk.ReadUInt16();
	for (short i = 0; i < SChaAttr.sNum; i++)
	{
		SChaAttr.SEff[i].lAttrID = pk.ReadUInt8();
		SChaAttr.SEff[i].lVal = (long)pk.ReadUInt32();
	}

	// log
	LG(szLogName, "Syn Character Attr: Count=%d\t, Type:%d\tTick:[%u]\n", SChaAttr.sNum, SChaAttr.chType, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(312));
	for (short i = 0; i < SChaAttr.sNum; i++)
	{
		LG(szLogName, "\t%d\t%d\n", SChaAttr.SEff[i].lAttrID, SChaAttr.SEff[i].lVal);
	}
	LG(szLogName, "\n");
	//
T_E}

void ReadChaLookPacket(LPRPACKET pk, stNetLookInfo &SLookInfo, const char *szLogName)
{T_B
	memset(&SLookInfo, 0, sizeof(SLookInfo));
	SLookInfo.chSynType = pk.ReadUInt8();
	stNetChangeChaPart	&SChaPart = SLookInfo.SLook;
	SChaPart.sTypeID = pk.ReadUInt16();
	if( pk.ReadUInt8() == 1 ) // �������
	{
		SChaPart.sPosID = pk.ReadUInt16();
		SChaPart.sBoatID = pk.ReadUInt16();
		SChaPart.sHeader = pk.ReadUInt16();
		SChaPart.sBody = pk.ReadUInt16();
		SChaPart.sEngine = pk.ReadUInt16();
		SChaPart.sCannon = pk.ReadUInt16();
		SChaPart.sEquipment = pk.ReadUInt16();

		// log
		LG(szLogName, "===Recieve(Look):\tTick:[%u]\n", GetTickCount());
		LG(szLogName, "TypeID:%d, PoseID:%d\n", SChaPart.sTypeID, SChaPart.sPosID);
		LG(szLogName, "\tPart: Boat:%u, Header:%u, Body:%u, Engine:%u, Cannon:%u, Equipment:%u\n", SChaPart.sBoatID, SChaPart.sHeader, SChaPart.sBody, SChaPart.sEngine, SChaPart.sCannon, SChaPart.sEquipment );
		LG(szLogName, "\n");
		//
	}
	else
	{
		SChaPart.sHairID = pk.ReadUInt16();
		SItemGrid *pItem;
		for (int i = 0; i < enumEQUIP_NUM; i++)
		{
			pItem = &SChaPart.SLink[i];
			pItem->sID = pk.ReadUInt16();
			pItem->dwDBID = pk.ReadUInt32();
			pItem->sNeedLv = pk.ReadUInt16();
			if (pItem->sID == 0)
			{
				memset(pItem, 0, sizeof(SItemGrid));
				continue;
			}
			if (SLookInfo.chSynType == enumSYN_LOOK_CHANGE)
			{
				pItem->sEndure[0] = pk.ReadUInt16();
				pItem->sEnergy[0] = pk.ReadUInt16();
				pItem->SetValid(pk.ReadUInt8() != 0 ? true : false);
				pItem->bItemTradable = pk.ReadUInt8();
				pItem->expiration = pk.ReadUInt32();
				continue;
			}
			else
			{
				pItem->sNum = pk.ReadUInt16();
				pItem->sEndure[0] = pk.ReadUInt16();
				pItem->sEndure[1] = pk.ReadUInt16();
				pItem->sEnergy[0] = pk.ReadUInt16();
				pItem->sEnergy[1] = pk.ReadUInt16();
				pItem->chForgeLv = pk.ReadUInt8();
				pItem->SetValid(pk.ReadUInt8() != 0 ? true : false);
				pItem->bItemTradable = pk.ReadUInt8();
				pItem->expiration = pk.ReadUInt32();

			}

			if (pk.ReadUInt8() == 0)
				continue;

			pItem->SetDBParam(enumITEMDBP_FORGE, pk.ReadUInt32());
			pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadUInt32());
			if (pk.ReadUInt8()) // ����ʵ������
			{
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
				{
					pItem->sInstAttr[j][0] = pk.ReadUInt16();
					pItem->sInstAttr[j][1] = pk.ReadUInt16();
				}
			}
		}

		// log
		LG(szLogName, "===Recieve(Look)\tTick:[%u]\n", GetTickCount());
		LG(szLogName, "TypeID:%d, HairID:%d\n", SChaPart.sTypeID, SChaPart.sHairID);
		for (int i = 0; i < enumEQUIP_NUM; i++)
			LG(szLogName, "\tLink: %d\n", SChaPart.SLink[i].sID);
		LG(szLogName, "\n");
		//
	}
T_E}

void ReadChaLookEnergyPacket(LPRPACKET pk, stLookEnergy &SLookEnergy, const char *szLogName)
{T_B
	memset(&SLookEnergy, 0, sizeof(SLookEnergy));
	for (int i = 0; i < enumEQUIP_NUM; i++)
	{
		SLookEnergy.sEnergy[i] = pk.ReadUInt16();
	}
T_E}

void ReadChaPKPacket(LPRPACKET pk, stNetPKCtrl &SNetPKCtrl, const char *szLogName)
{T_B
	char	chPKCtrl = pk.ReadUInt8();
	std::bitset<8> states(chPKCtrl);
	SNetPKCtrl.bInPK = states[0];
	SNetPKCtrl.bInGymkhana = states[1];
	SNetPKCtrl.pkGuild = states[2];

	// log
	LG(szLogName, "===Recieve(PKCtrl)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tInGymkhana: %d, InPK: %d\n", SNetPKCtrl.bInGymkhana, SNetPKCtrl.bInPK);
	LG(szLogName, "\n");
	//
T_E}

void ReadChaSidePacket(LPRPACKET pk, stNetChaSideInfo &SNetSideInfo, const char *szLogName)
{T_B
	SNetSideInfo.chSideID = pk.ReadUInt8();

	// log
	LG(szLogName, "===Recieve(SideInfo)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tSideID: %d\n", SNetSideInfo.chSideID);
	LG(szLogName, "\n");
	//
T_E}

void ReadChaAppendLookPacket(LPRPACKET pk, stNetAppendLook &SNetAppendLook, const char *szLogName)
{T_B
	for (char i = 0; i < defESPE_KBGRID_NUM; i++)
	{
		SNetAppendLook.sLookID[i] = pk.ReadUInt16();
		if (SNetAppendLook.sLookID[i] != 0)
			SNetAppendLook.bValid[i] = pk.ReadUInt8() != 0 ? true : false;
	}

	// log
	LG(szLogName, "===Recieve(Append Look)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tAppend Look:%d(%d), %d(%d), %d(%d), %d(%d)\n",
		SNetAppendLook.sLookID[0], SNetAppendLook.bValid[0],
		SNetAppendLook.sLookID[1], SNetAppendLook.bValid[1],
		SNetAppendLook.sLookID[2], SNetAppendLook.bValid[2],
		SNetAppendLook.sLookID[3], SNetAppendLook.bValid[3]);
	LG(szLogName, "\n");
	//
T_E}

void ReadEntEventPacket(LPRPACKET pk, stNetEvent &SNetEvent,const char *szLogName)
{T_B
	SNetEvent.lEntityID = pk.ReadUInt32();
	SNetEvent.chEntityType = pk.ReadUInt8();
	SNetEvent.usEventID = pk.ReadUInt16();
	SNetEvent.cszEventName = pk.ReadString();

	// log
	if (szLogName)
	{
		LG(szLogName, "===Recieve(Event)\tTick:[%u]\n", GetTickCount());
		LG(szLogName, "\tEntityID: %u, EventID: %u, EventName: %s\n", SNetEvent.lEntityID, SNetEvent.usEventID, SNetEvent.cszEventName);
		LG(szLogName, "\n");
	}
	//
T_E}

void ReadChaKitbagPacket(LPRPACKET pk, stNetKitbag &SKitbag, const char *szLogName)
{T_B
	memset(&SKitbag, 0, sizeof(SKitbag));
	SKitbag.chType = pk.ReadUInt8(); // ���ο�CompCommand.h��ESynKitbagType��
	int nGridNum = 0;
	if (SKitbag.chType == enumSYN_KITBAG_INIT) // ����������
	{
		SKitbag.nKeybagNum = pk.ReadUInt16();
	}
	LG(szLogName, "===Recieve(Update Kitbag):\tGridNum:%d\tType:%d\tTick:[%u]\n", SKitbag.nKeybagNum, SKitbag.chType, GetTickCount());
	stNetKitbag::stGrid* Grid = SKitbag.Grid;
	SItemGrid *pItem;
	CItemRecord* pItemRec;
	while (1)
	{
		short sGridID = pk.ReadUInt16();
		if(sGridID < 0) break;

		Grid[nGridNum].sGridID = sGridID;

		pItem = &Grid[nGridNum].SGridContent;
		pItem->sID = pk.ReadUInt16();
		LG(szLogName, g_oLangRec.GetString(313), Grid[nGridNum].sGridID, pItem->sID);
		if (pItem->sID > 0) // ���ڵ���
		{
			pItem->dwDBID = pk.ReadUInt32();
			pItem->sNeedLv		=	pk.ReadUInt16();
			pItem->sNum			=	pk.ReadUInt16();
			pItem->sEndure[0]	=	pk.ReadUInt16();
			pItem->sEndure[1]	=	pk.ReadUInt16();
			pItem->sEnergy[0]	=	pk.ReadUInt16();
			pItem->sEnergy[1]	=	pk.ReadUInt16();
			LG(szLogName, g_oLangRec.GetString(314), pItem->sNum, pItem->sEndure[0], pItem->sEndure[1], pItem->sEnergy[0], pItem->sEnergy[1]);
			pItem->chForgeLv = pk.ReadUInt8();
			pItem->SetValid(pk.ReadUInt8() != 0 ? true : false);
			pItem->bItemTradable = pk.ReadUInt8();
			pItem->expiration = pk.ReadUInt32();

			pItemRec = GetItemRecordInfo( pItem->sID );
			if(pItemRec==NULL)
			{
				char szBuf[256] = { 0 };
				sprintf( szBuf, g_oLangRec.GetString(315), pItem->sID );
				MessageBox( 0, szBuf, "Error", 0 );
#ifdef USE_DSOUND
				if( g_dwCurMusicID )
				{
					AudioSDL::get_instance()->stop( g_dwCurMusicID );
					g_dwCurMusicID = 0;
					Sleep( 60 );
				}
#endif
				exit(0);
			}

			if( pItemRec->sType == enumItemTypeBoat ) // �����ߣ�д�봬��WorldID�����ڿͻ��˽������봬��ɫ�ҹ�
			{
				pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadUInt32());
			}

			pItem->SetDBParam(enumITEMDBP_FORGE, pk.ReadUInt32());
			if( pItemRec->sType == enumItemTypeBoat ) 
			{
				pk.ReadUInt32();
			}
			else
			{
				pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadUInt32());
			}

			LG(szLogName, g_oLangRec.GetString(316), pItem->GetDBParam(enumITEMDBP_FORGE));
			if (pk.ReadUInt8()) // ����ʵ������
			{
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
				{
					pItem->sInstAttr[j][0] = pk.ReadUInt16();
					pItem->sInstAttr[j][1] = pk.ReadUInt16();
					LG(szLogName, g_oLangRec.GetString(317), pItem->sInstAttr[j][0], pItem->sInstAttr[j][1]);
				}
			}
		}
		nGridNum++;
		if (nGridNum > defMAX_KBITEM_NUM_PER_TYPE) // ���ó��ֵ����
		{
			LG(g_oLangRec.GetString(318), g_oLangRec.GetString(319), nGridNum, defMAX_KBITEM_NUM_PER_TYPE);
			break;
		}
	}
	SKitbag.nGridNum = nGridNum;
	LG(szLogName, g_oLangRec.GetString(320), SKitbag.nGridNum);
T_E}

void ReadChaShortcutPacket(LPRPACKET pk, stNetShortCut &SShortcut, const char* szLogName)
{T_B
	memset(&SShortcut, 0, sizeof(SShortcut));
	LG(szLogName, "===Recieve(Update Shortcut):\tTick:[%u]\n", GetTickCount());
	for (int i = 0; i < SHORT_CUT_NUM; i++)
	{
		SShortcut.chType[i] = pk.ReadUInt8();
		SShortcut.byGridID[i] = pk.ReadUInt16();
		LG(szLogName, g_oLangRec.GetString(321), SShortcut.chType[i], SShortcut.byGridID[i]);
	}
T_E}


BOOL PC_PKSilver(LPRPACKET packet)
{T_B
    std::string szName;
    long sLevel;
    std::string szProfession;
    long lPkval;
    for(int i = 0; i < MAX_PKSILVER_PLAYER; i++)
    {
        szName = packet.ReadString();
        sLevel = packet.ReadUInt32();
        szProfession = packet.ReadString();
        lPkval = packet.ReadUInt32();
        g_stUIPKSilver.AddFormAttribute(i, szName, sLevel, szProfession, lPkval);
    }

    g_stUIPKSilver.ShowPKSilverForm();
    return TRUE;
T_E}


BOOL SC_LifeSkillShow(LPRPACKET packet)
{T_B
    long lType = packet.ReadUInt32();
    switch(lType)
    {
    case 0:     //  �ϳ�
        {
            g_stUICompose.ShowComposeForm();
        }  break;
    case 1:     //  �ֽ�
        {
            g_stUIBreak.ShowBreakForm();
        }  break;
    case 2:     //  ����
        {
            g_stUIFound.ShowFoundForm();
        }  break;
    case 3:     //  ���
        {
            g_stUICooking.ShowCookingForm();
        }  break;
    }
    return TRUE;
T_E}


BOOL SC_LifeSkill(LPRPACKET packet)
{T_B
    long lType = packet.ReadUInt32();
    short ret = packet.ReadUInt16();
    std::string txt = packet.ReadString();

    switch(lType)
    {
    case 0:     //  �ϳ�
        {
            g_stUICompose.CheckResult(ret, txt.c_str());
        }  break;
    case 1:     //  �ֽ�
        {
            g_stUIBreak.CheckResult(ret, txt.c_str());
        }  break;
    case 2:     //  ����
        {
            std::string strVer[3];
            Util_ResolveTextLine(txt.c_str(), strVer, 3, ',');
            g_stUIFound.CheckResult(ret, strVer[0].c_str(), strVer[1].c_str(), strVer[2].c_str());
        }  break;
    case 3:     //  ���
        {
            g_stUICooking.CheckResult(ret);
        }  break;
    }

    return TRUE;
T_E}


BOOL SC_LifeSkillAsr(LPRPACKET packet)
{T_B
    long lType = packet.ReadUInt32();
    short tim = packet.ReadUInt16();
    std::string txt = packet.ReadString();

    switch(lType)
    {
    case 0:     //  �ϳ�
        {
            g_stUICompose.StartTime(tim);
        }  break;
    case 1:     //  �ֽ�
        {
            g_stUIBreak.StartTime(tim, txt.c_str());
        }  break;
    case 2:     //  ����
        {
            g_stUIFound.StartTime(tim);
        }  break;
    case 3:     //  ���
        {
            g_stUICooking.StartTime(tim);
        }  break;
    }
    return TRUE;
T_E}


BOOL	SC_DropLockAsr(LPRPACKET	pk)
{
	T_B
	const auto success = pk.ReadUInt8();
	if (success)
	{
		g_pGameApp->SysInfo("Locking successful!");
	}
	else
	{
		g_pGameApp->SysInfo("Locking failed!");
	}
	return	TRUE;
	T_E
};


BOOL SC_UnlockItemAsr(LPRPACKET pk)
{
	T_B
		g_pGameApp->SysInfo(
			[&] {
				switch (pk.ReadUInt8()) {
				case 1:
					return "Item Unlocked";
				case 2:
					return "2nd password incorrect!";
				default:
					return "Unlocking failed";
				}
			}());
	return	TRUE;
	T_E
}

//==================================================================================

