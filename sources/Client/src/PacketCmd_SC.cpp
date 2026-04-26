#include "StdAfx.h"
#include "Character.h"
#include "Scene.h"
#include "GameApp.h"
#include "actor.h"
#include "GameDiagnostic.h"

using Corsairs::Client::Diagnostic::GameDiagnostic;
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
#include "EncodingUtil.h"
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
#include "MapRecordStore.h"


#ifdef _TEST_CLIENT
#include "..\..\TestClient\testclient.h"
#endif


#include "SceneObj.h"
#include "Scene.h"
#include "CommandMessages.h"

//  uChar, uShort, uLong, cChar   NetIF.h


//--------------------------------------------------
// 
// 
//      PacketCmd_DoSomething(LPPacket pk)
//
// 
//       CCharacter *pCha
//       CCharacter* pMainCha
//       CSceneItem* pItem
//       CGameScene* pScene
//      APP  CGameApp*   g_pGameApp
//--------------------------------------------------
extern char g_szSendKey[4];
extern char g_szRecvKey[4];

static unsigned long g_ulWorldID = 0;

// Forward declarations    sub-packet   
void ReadChaKitbagFromMsg(const net::msg::ChaKitbagInfo& info, stNetKitbag& SKitbag);
void ReadChaShortcutFromMsg(const net::msg::ChaShortcutInfo& info, stNetShortCut& SShortcut);

using namespace std;

BOOL SC_UpdateGuildGold(LPRPACKET pk) {
	net::msg::McUpdateGuildGoldMessage msg;
	net::msg::deserialize(pk, msg);
	g_stUIGuildBank.UpdateGuildGold(msg.data.c_str());
	return true;
}

BOOL SC_ShowStallSearch(LPRPACKET pk) {
	//  count-first:   McShowStallSearchMessage
	net::msg::McShowStallSearchMessage msg;
	net::msg::deserialize(pk, msg);
	NetMC_LISTGUILD_BEGIN();
	for (size_t i = 0; i < msg.entries.size(); ++i) {
		auto& e = msg.entries[i];
		NetMC_LISTGUILD(i + 1, e.name.c_str(), e.stallName.c_str(), e.location.c_str(), e.count, e.cost);
	}
	NetMC_LISTGUILD_END();
	return TRUE;
}

BOOL SC_ShowRanking(LPRPACKET pk) {
	//  count-first:   McShowRankingMessage
	net::msg::McShowRankingMessage msg;
	net::msg::deserialize(pk, msg);
	NetMC_LISTGUILD_BEGIN();
	for (size_t i = 0; i < msg.entries.size(); ++i) {
		auto& e = msg.entries[i];
		const std::string buf = std::format("{:03}>", static_cast<int>(i + 1));
		NetMC_LISTGUILD(i + 1, buf.c_str(), e.name.c_str(), e.guild.c_str(), (uShort)e.level, e.score);
	}
	NetMC_LISTGUILD_END();
	return TRUE;
}

// CryptImportPublicKeyInfoEx2   crypt32.dll (Vista+),
//    wincrypt.h   _WIN32_WINNT >= 0x0600.
//   0x0500   .
typedef BOOL (WINAPI *PFN_CryptImportPublicKeyInfoEx2)(
	DWORD dwCertEncodingType,
	PCERT_PUBLIC_KEY_INFO pInfo,
	DWORD dwFlags,
	void* pvAuxInfo,
	BCRYPT_KEY_HANDLE* phKey
);

BOOL SC_SendPublicKey(LPRPACKET pk) {
	//  SPKI DER     (WriteSequence = [uint16 len][data])
	uShort keySize = 0;
	const char* keyData = pk.ReadSequence(keySize);
	if (keySize == 0) {
		//        RSA/AES
		ToLogService("connections", "SC_SendPublicKey: server encryption disabled (empty key)");
		g_NetIF->handshakeDone = true;
		g_NetIF->_comm_enc = 0;
		g_NetIF->m_connect.CHAPSTR(); // CONNECTING  HANDSHAKE
		g_NetIF->m_connect.CHAPSTR(false); // HANDSHAKE  CONNECTED
		return TRUE;
	}
	if (!keyData) {
		ToLogService("connections", "SC_SendPublicKey: null key data");
		return FALSE;
	}

	//   SPKI DER   hex
	{
		std::string hex;
		hex.reserve(keySize * 2);
		for (uShort i = 0; i < keySize; i++) {
			std::format_to(std::back_inserter(hex), "{:02x}", static_cast<unsigned char>(keyData[i]));
		}
		ToLogService("connections", "SC_SendPublicKey: received SPKI DER key ({} bytes): {}", keySize, hex);
	}

	//  SPKI DER  CERT_PUBLIC_KEY_INFO
	CERT_PUBLIC_KEY_INFO* pubKeyInfo = NULL;
	DWORD pubKeyInfoSize = 0;
	if (!CryptDecodeObjectEx(
		X509_ASN_ENCODING,
		X509_PUBLIC_KEY_INFO,
		reinterpret_cast<const BYTE*>(keyData), keySize,
		CRYPT_DECODE_ALLOC_FLAG, NULL,
		&pubKeyInfo, &pubKeyInfoSize)) {
		ToLogService("connections", LogLevel::Error, "CryptDecodeObjectEx failed: {}", GetLastError());
		return FALSE;
	}

	//  CryptImportPublicKeyInfoEx2 
	static PFN_CryptImportPublicKeyInfoEx2 pfnImport = NULL;
	if (!pfnImport) {
		HMODULE hCrypt32 = GetModuleHandleA("crypt32.dll");
		if (hCrypt32)
			pfnImport = (PFN_CryptImportPublicKeyInfoEx2)GetProcAddress(hCrypt32, "CryptImportPublicKeyInfoEx2");
	}
	if (!pfnImport) {
		ToLogService("connections", LogLevel::Error, "CryptImportPublicKeyInfoEx2 not available");
		LocalFree(pubKeyInfo);
		return FALSE;
	}

	//   BCrypt RSA handle
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
		ToLogService("connections", LogLevel::Error, "CryptImportPublicKeyInfoEx2 failed: {}", GetLastError());
		return FALSE;
	}

	g_NetIF->m_connect.CHAPSTR();
	CS_SendPrivateKey();

	return TRUE;
}


BOOL SC_SendHandshake(LPRPACKET pk) {
	/**/
	return true;
}


BOOL SC_Login(LPRPACKET pk) {
	net::msg::McLoginResponse resp;
	net::msg::deserialize(pk, resp);

	if (!resp.data.has_value()) {
		NetLoginFailure(static_cast<uShort>(resp.errCode));
	}
	else {
		const auto& d = resp.data.value();
		//  ChaSlotData  NetChaBehave
		std::vector<NetChaBehave> characters;
		for (const auto& cha : d.characters) {
			if (cha.valid) {
				NetChaBehave nb;
				nb.sCharName = cha.chaName;
				nb.sJob = cha.job;
				nb.iDegree = static_cast<short>(cha.degree);
				nb.look_minimal.typeID = static_cast<uint16_t>(cha.typeId);
				for (size_t ei = 0; ei < cha.equipIds.size() && ei < nb.look_minimal.equip_IDs.size(); ++ei)
					nb.look_minimal.equip_IDs[ei] = static_cast<uint16_t>(cha.equipIds[ei]);
				characters.emplace_back(std::move(nb));
			}
		}
		NetLoginSuccess(d.hasPassword2 ? 1 : 0, static_cast<uint8_t>(d.maxChaNum), characters);

		extern CGameWG g_oGameWG;
		g_oGameWG.SafeTerminateThread();
		g_oGameWG.BeginThread();
	}
	updateDiscordPresence("Selecting Character", "");
	return TRUE;
}

BOOL SC_Disconnect(LPRPACKET pk) {
	net::msg::McDisconnectMessage msg;
	net::msg::deserialize(pk, msg);
	g_NetIF->m_connect.Disconnect(msg.reason);
	return true;
}


// =================================================================
//  CMD_MC_ENTERMAP   net::msg  stNet* ()
// =================================================================

// clientIsBoatItem     isBoat   

static void convertBaseInfo(const net::msg::ChaBaseInfo& src, stNetActorCreate& dst) {
	dst.ulChaID = static_cast<unsigned long>(src.chaId);
	dst.ulWorldID = static_cast<unsigned long>(src.worldId);
	dst.ulCommID = static_cast<unsigned long>(src.commId);
	dst.szCommName = src.commName;
	dst.chGMLv = static_cast<char>(src.gmLv);
	dst.lHandle = static_cast<long>(src.handle);
	dst.chCtrlType = static_cast<char>(src.ctrlType);
	dst.szName = src.name;
	dst.strMottoName = src.motto;
	dst.sIcon = static_cast<short>(src.icon);
	dst.lGuildID = static_cast<long>(src.guildId);
	dst.strGuildName = src.guildName;
	dst.strGuildMotto = src.guildMotto;
	dst.chGuildPermission = static_cast<int>(src.guildPermission);
	dst.strStallName = src.stallName;
	dst.sState = static_cast<short>(src.state);
	dst.SArea.centre.x = static_cast<long>(src.posX);
	dst.SArea.centre.y = static_cast<long>(src.posY);
	dst.SArea.radius = static_cast<long>(src.radius);
	dst.sAngle = static_cast<short>(src.angle);
	dst.ulTLeaderID = static_cast<unsigned long>(src.teamLeaderId);
	dst.chIsPlayer = static_cast<int>(src.isPlayer);
	// Side
	dst.SSideInfo.chSideID = static_cast<char>(src.side.sideId);
	// Event
	dst.SEvent.lEntityID = static_cast<long>(src.event.entityId);
	dst.SEvent.chEntityType = static_cast<char>(src.event.entityType);
	dst.SEvent.usEventID = static_cast<unsigned short>(src.event.eventId);
	dst.SEvent.cszEventName = src.event.eventName;
	// Look
	dst.SLookInfo.chSynType = static_cast<char>(src.look.synType);
	dst.SLookInfo.SLook.sTypeID = static_cast<short>(src.look.typeId);
	if (src.look.isBoat) {
		dst.SLookInfo.SLook.sPosID = static_cast<USHORT>(src.look.boatParts.posId);
		dst.SLookInfo.SLook.sBoatID = static_cast<USHORT>(src.look.boatParts.boatId);
		dst.SLookInfo.SLook.sHeader = static_cast<USHORT>(src.look.boatParts.header);
		dst.SLookInfo.SLook.sBody = static_cast<USHORT>(src.look.boatParts.body);
		dst.SLookInfo.SLook.sEngine = static_cast<USHORT>(src.look.boatParts.engine);
		dst.SLookInfo.SLook.sCannon = static_cast<USHORT>(src.look.boatParts.cannon);
		dst.SLookInfo.SLook.sEquipment = static_cast<USHORT>(src.look.boatParts.equipment);
	}
	else {
		dst.SLookInfo.SLook.sHairID = static_cast<short>(src.look.hairId);
		for (int i = 0; i < enumEQUIP_NUM; ++i) {
			const auto& eq = src.look.equips[i];
			auto& item = dst.SLookInfo.SLook.SLink[i];
			item.sID = static_cast<short>(eq.id);
			item.dwDBID = static_cast<DWORD>(eq.dbId);
			item.sNeedLv = static_cast<short>(eq.needLv);
			if (eq.id == 0) continue;
			if (src.look.synType == net::msg::SYN_LOOK_CHANGE) {
				item.sEndure[0] = static_cast<short>(eq.endure0);
				item.sEnergy[0] = static_cast<short>(eq.energy0);
				item.bValid = eq.valid != 0;
				item.bItemTradable = eq.tradable != 0;
				item.expiration = static_cast<long>(eq.expiration);
			}
			else {
				item.sNum = static_cast<short>(eq.num);
				item.sEndure[0] = static_cast<short>(eq.endure0);
				item.sEndure[1] = static_cast<short>(eq.endure1);
				item.sEnergy[0] = static_cast<short>(eq.energy0);
				item.sEnergy[1] = static_cast<short>(eq.energy1);
				item.chForgeLv = static_cast<char>(eq.forgeLv);
				item.bValid = eq.valid != 0;
				item.bItemTradable = eq.tradable != 0;
				item.expiration = static_cast<long>(eq.expiration);
				if (eq.hasExtra) {
					item.lDBParam[enumITEMDBP_FORGE] = static_cast<long>(eq.forgeParam);
					item.lDBParam[enumITEMDBP_INST_ID] = static_cast<long>(eq.instId);
					if (eq.hasInstAttr) {
						for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; ++j) {
							item.sInstAttr[j][0] = static_cast<short>(eq.instAttr[j][0]);
							item.sInstAttr[j][1] = static_cast<short>(eq.instAttr[j][1]);
						}
					}
				}
			}
		}
	}
	// PK ctrl
	std::bitset<8> pkbits(static_cast<unsigned long>(src.pkCtrl));
	dst.SPKCtrl.bInPK = pkbits[0];
	dst.SPKCtrl.bInGymkhana = pkbits[1];
	dst.SPKCtrl.pkGuild = pkbits[2];
	// Append look
	for (int i = 0; i < defESPE_KBGRID_NUM; ++i) {
		dst.SAppendLook.sLookID[i] = static_cast<short>(src.appendLook[i].lookId);
		dst.SAppendLook.bValid[i] = src.appendLook[i].valid != 0;
	}
}

static void convertSkillBag(const net::msg::ChaSkillBagInfo& src, stNetSkillBag& dst) {
	memset(&dst, 0, sizeof(dst));
	stNetDefaultSkill defSkill;
	defSkill.sSkillID = static_cast<short>(src.defSkillId);
	defSkill.Exec();
	dst.chType = static_cast<char>(src.synType);
	auto count = static_cast<short>(src.skills.size());
	if (count <= 0) return;
	dst.SBag.Resize(count);
	SSkillGridEx* pSBag = dst.SBag.GetValue();
	for (short i = 0; i < count; ++i) {
		const auto& sk = src.skills[i];
		pSBag[i].sID = static_cast<short>(sk.id);
		pSBag[i].chState = static_cast<char>(sk.state);
		pSBag[i].chLv = static_cast<char>(sk.level);
		pSBag[i].sUseSP = static_cast<short>(sk.useSp);
		pSBag[i].sUseEndure = static_cast<short>(sk.useEndure);
		pSBag[i].sUseEnergy = static_cast<short>(sk.useEnergy);
		pSBag[i].lResumeTime = static_cast<long>(sk.resumeTime);
		for (int j = 0; j < defSKILL_RANGE_PARAM_NUM; ++j)
			pSBag[i].sRange[j] = static_cast<short>(sk.range[j]);
	}
}

static void convertSkillState(const net::msg::ChaSkillStateInfo& src, stNetSkillState& dst) {
	unsigned long currentClient = GetTickCount();
	unsigned long currentServer = static_cast<unsigned long>(src.currentTime) / 1000;
	memset(&dst, 0, sizeof(dst));
	dst.chType = 0;
	auto count = static_cast<short>(src.states.size());
	if (count <= 0) return;
	dst.SState.Resize(count);
	for (int i = 0; i < count; ++i) {
		const auto& st = src.states[i];
		dst.SState[i].chID = static_cast<BYTE>(st.stateId);
		dst.SState[i].chLv = static_cast<BYTE>(st.stateLv);
		unsigned long duration = static_cast<unsigned long>(st.duration);
		unsigned long start = static_cast<unsigned long>(st.startTime) / 1000;
		unsigned long dif = currentServer - currentClient;
		unsigned long end = start - dif + duration;
		dst.SState[i].lTimeRemaining = duration == 0 ? 0 : end - currentClient;
	}
}

static void convertAttr(const net::msg::ChaAttrInfo& src, stNetChaAttr& dst) {
	memset(&dst, 0, sizeof(dst));
	dst.chType = static_cast<char>(src.synType);
	dst.sNum = static_cast<short>(src.attrs.size());
	for (short i = 0; i < dst.sNum && i < MAX_ATTR_CLIENT; ++i) {
		dst.SEff[i].lAttrID = static_cast<long>(src.attrs[i].attrId);
		dst.SEff[i].lVal = static_cast<LONG64>(src.attrs[i].attrVal);
	}
}

static void convertKitbag(const net::msg::ChaKitbagInfo& src, stNetKitbag& dst) {
	memset(&dst, 0, sizeof(dst));
	dst.chType = static_cast<char>(src.synType);
	if (src.synType == net::msg::SYN_KITBAG_INIT)
		dst.nKeybagNum = static_cast<int>(src.capacity);
	dst.nGridNum = 0;
	for (const auto& item : src.items) {
		if (dst.nGridNum >= defMAX_KBITEM_NUM_PER_TYPE) break;
		auto& grid = dst.Grid[dst.nGridNum];
		grid.sGridID = static_cast<short>(item.gridId);
		auto& d = grid.SGridContent;
		d.sID = static_cast<short>(item.itemId);
		if (item.itemId > 0) {
			d.dwDBID = static_cast<DWORD>(item.dbId);
			d.sNeedLv = static_cast<short>(item.needLv);
			d.sNum = static_cast<short>(item.num);
			d.sEndure[0] = static_cast<short>(item.endure0);
			d.sEndure[1] = static_cast<short>(item.endure1);
			d.sEnergy[0] = static_cast<short>(item.energy0);
			d.sEnergy[1] = static_cast<short>(item.energy1);
			d.chForgeLv = static_cast<char>(item.forgeLv);
			d.bValid = item.valid != 0;
			d.bItemTradable = item.tradable != 0;
			d.expiration = static_cast<long>(item.expiration);
			if (item.isBoat) {
				d.lDBParam[enumITEMDBP_INST_ID] = static_cast<long>(item.boatWorldId);
			}
			d.lDBParam[enumITEMDBP_FORGE] = static_cast<long>(item.forgeParam);
			if (!item.isBoat) {
				d.lDBParam[enumITEMDBP_INST_ID] = static_cast<long>(item.instId);
			}
			if (item.hasInstAttr) {
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; ++j) {
					d.sInstAttr[j][0] = static_cast<short>(item.instAttr[j][0]);
					d.sInstAttr[j][1] = static_cast<short>(item.instAttr[j][1]);
				}
			}
		}
		dst.nGridNum++;
	}
}

static void convertShortcut(const net::msg::ChaShortcutInfo& src, stNetShortCut& dst) {
	memset(&dst, 0, sizeof(dst));
	for (int i = 0; i < SHORT_CUT_NUM; ++i) {
		dst.chType[i] = static_cast<char>(src.entries[i].type);
		dst.byGridID[i] = static_cast<short>(src.entries[i].gridId);
	}
}

//--------------------
// CMD_MC_ENTERMAP     (S->C)
//--------------------
BOOL SC_EnterMap(LPRPACKET pk) {
	g_listguild_begin = false;

	net::msg::McEnterMapMessage msg;
	net::msg::deserialize(pk, msg);

	if (msg.errCode != 0) {
		stNetSwitchMap SMapInfo{};
		SMapInfo.sEnterRet = static_cast<short>(msg.errCode);
		NetSwitchMap(SMapInfo);
		return FALSE;
	}

	const auto& d = msg.data.value();

	g_stUISystem.m_sysProp.m_gameOption.bLockMode = d.autoLock != 0;
	g_stUIEquip.SetIsLock(d.kitbagLock != 0);

	stNetSwitchMap SMapInfo{};
	SMapInfo.sEnterRet = 0;
	SMapInfo.chEnterType = static_cast<char>(d.enterType);
	SMapInfo.bIsNewCha = d.isNewCha != 0;
	SMapInfo.szMapName = d.mapName;
	SMapInfo.bCanTeam = d.canTeam != 0;
	NetSwitchMap(SMapInfo);
	g_logManager.InternalLog(LogLevel::Debug, "common", d.mapName);

	g_stUIEquip.UpdateIMP(static_cast<int>(d.imp));

	stNetActorCreate SCreateInfo;
	convertBaseInfo(d.baseInfo, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 1;
	SCreateInfo.CreateCha();
	g_ulWorldID = SCreateInfo.ulWorldID;

	stNetSkillBag SCurSkill;
	convertSkillBag(d.skillBag, SCurSkill);
	NetSynSkillBag(SCreateInfo.ulWorldID, &SCurSkill);

	stNetSkillState SCurSState;
	convertSkillState(d.skillState, SCurSState);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	stNetChaAttr SChaAttr;
	convertAttr(d.attr, SChaAttr);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff);

	stNetKitbag SKitbag;
	convertKitbag(d.kitbag, SKitbag);
	SKitbag.chBagType = 0;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

	stNetShortCut SShortcut;
	convertShortcut(d.shortcut, SShortcut);
	NetShortCut(SCreateInfo.ulWorldID, SShortcut);

	for (const auto& boat : d.boats) {
		convertBaseInfo(boat.baseInfo, SCreateInfo);
		SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
		SCreateInfo.chMainCha = 2;
		SCreateInfo.CreateCha();

		convertAttr(boat.attr, SChaAttr);
		NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff);

		convertKitbag(boat.kitbag, SKitbag);
		NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

		convertSkillState(boat.skillState, SCurSState);
		NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);
	}

	stNetChangeCha SChangeCha;
	SChangeCha.ulMainChaID = static_cast<unsigned long>(d.ctrlChaId);
	NetActorChangeCha(SCreateInfo.ulWorldID, SChangeCha);

	return TRUE;
}

BOOL SC_BeginPlay(LPRPACKET pk) {
	net::msg::McBgnPlayResponse resp;
	net::msg::deserialize(pk, resp);
	NetBeginPlay(resp.errCode);

	return TRUE;
}

BOOL SC_EndPlay(LPRPACKET pk) {
	net::msg::McEndPlayResponse resp;
	net::msg::deserialize(pk, resp);

	if (resp.data.has_value()) {
		const auto& data = resp.data.value();
		std::vector<NetChaBehave> characters;
		characters.reserve(data.characters.size());
		for (const auto& cha : data.characters) {
			if (cha.valid) {
				NetChaBehave behave;
				behave.sCharName = cha.chaName;
				behave.sJob = cha.job;
				behave.iDegree = cha.degree;
				behave.look_minimal.typeID = cha.typeId;
				for (int i = 0; i < net::msg::EQUIP_NUM && i < static_cast<int>(cha.equipIds.size()); ++i)
					behave.look_minimal.equip_IDs[i] = cha.equipIds[i];
				characters.push_back(std::move(behave));
			}
		}
		NetEndPlay(data.maxChaNum, characters);
	}
	else {
		NetEndPlay(0, {});
	}

	updateDiscordPresence("Selecting Character", "");
	return TRUE;
}


BOOL SC_NewCha(LPRPACKET pk) {
	net::msg::McNewChaResponse resp;
	net::msg::deserialize(pk, resp);
	NetNewCha(resp.errCode);

	return TRUE;
}

BOOL SC_DelCha(LPRPACKET pk) {
	net::msg::McDelChaResponse resp;
	net::msg::deserialize(pk, resp);
	NetDelCha(resp.errCode);
	return TRUE;
}

BOOL SC_CreatePassword2(LPRPACKET pk) {
	net::msg::McCreatePassword2Response resp;
	net::msg::deserialize(pk, resp);
	NetCreatePassword2(resp.errCode);
	return TRUE;
}

BOOL SC_UpdatePassword2(LPRPACKET pk) {
	net::msg::McUpdatePassword2Response resp;
	net::msg::deserialize(pk, resp);
	NetUpdatePassword2(resp.errCode);
	return TRUE;
}

//mothannakh create account
BOOL PC_REGISTER(LPRPACKET pk) {
	//   McRegisterResponseMessage
	net::msg::McRegisterResponseMessage msg;
	net::msg::deserialize(pk, msg);

	CGameApp::Waiting(false);
	if (g_NetIF->IsConnected()) {
		CS_Disconnect(DS_DISCONN);
	}
	_dwLastTime = CGameApp::GetCurTick();

	if (_dwOverTime > _dwLastTime) {
		g_pGameApp->MsgBox("Do Not Spam.");
		return FALSE;
	}

	if (msg.success == 1) {
		registerLogin = false;
		_dwOverTime = _dwLastTime + 9000;
		g_pGameApp->MsgBox("Account Created.");
	}
	else {
		g_pGameApp->MsgBox(msg.errorMessage.c_str());
	}
	return TRUE;
}

//     GroupServer
BOOL PC_Ping(LPRPACKET pk) {
	auto l_wpk = net::msg::serializeCpPingCmd();
	g_NetIF->SendPacketMessage(l_wpk);
	return TRUE;
}

//     GameServer (  )
BOOL SC_Ping(LPRPACKET pk) {
	net::msg::McPingMessage msg;
	net::msg::deserialize(pk, msg);
	{
		auto const l = std::lock_guard{g_NetIF->m_mutmov};
		auto wpk = net::msg::serialize(net::msg::CmPingResponseMessage{msg.v1, msg.v2, msg.v3, msg.v4, msg.v5});
		g_NetIF->SendPacketMessage(wpk);
	}

	if (g_NetIF->m_curdelay > g_NetIF->m_maxdelay) g_NetIF->m_maxdelay = g_NetIF->m_curdelay;

	if (g_NetIF->m_curdelay < g_NetIF->m_mindelay) g_NetIF->m_mindelay = g_NetIF->m_curdelay;

	return TRUE;
}

//    
BOOL SC_CheckPing(LPRPACKET pk) {
	auto wpk = net::msg::serializeCmCheckPingCmd();
	g_NetIF->SendPacketMessage(wpk);

	return TRUE;
}

BOOL SC_Say(LPRPACKET pk) {
	net::msg::McSayMessage msg;
	net::msg::deserialize(pk, msg);
	stNetSay l_netsay;
	l_netsay.m_srcid = msg.sourceId;
	l_netsay.m_content = msg.content.c_str();

	// [UTF-8 migration] Hex-дамп входящего содержимого для определения
	// формата сервера (CP1251/GBK vs UTF-8). Смотреть канал "net" в log.
	ToLogService("net", "SC_Say src={} bytes[{}]={} text='{}'",
				 msg.sourceId,
				 static_cast<int>(msg.content.size()),
				 encoding::HexDump(msg.content),
				 msg.content);

	NetSay(l_netsay, msg.color);

	return TRUE;
}

//--------------------
// S->C : 
//--------------------
BOOL SC_SysInfo(LPRPACKET pk) {
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	net::msg::McSysInfoMessage msg;
	net::msg::deserialize(pk, msg);
	stNetSysInfo l_sysinfo;
	l_sysinfo.m_sysinfo = msg.info.c_str();
	NetSysInfo(l_sysinfo);
	return TRUE;
}

BOOL GuildSysInfo = false;

BOOL SC_GuildInfo(LPRPACKET pk) {
	net::msg::McSysInfoMessage msg;
	net::msg::deserialize(pk, msg);
	stNetSysInfo l_sysinfo;
	l_sysinfo.m_sysinfo = msg.info.c_str();
	GuildSysInfo = true;
	NetSysInfo(l_sysinfo);
	return TRUE;
}

BOOL SC_PopupNotice(LPRPACKET pk) {
	net::msg::McPopupNoticeMessage msg;
	net::msg::deserialize(pk, msg);
	g_pGameApp->MsgBox(msg.notice.c_str());
	return TRUE;
}

BOOL SC_BickerNotice(LPRPACKET pk) {
	net::msg::McBickerNoticeMessage msg;
	net::msg::deserialize(pk, msg);
	char szData[1024];
	strncpy(szData, msg.text.c_str(), 1024 - 1);
	NetBickerInfo(szData);
	return TRUE;
}

BOOL SC_ColourNotice(LPRPACKET pk) {
	net::msg::McColourNoticeMessage msg;
	net::msg::deserialize(pk, msg);
	char szData[1024];
	strncpy(szData, msg.text.c_str(), 1024 - 1);
	NetColourInfo(static_cast<unsigned int>(msg.color), szData);
	return TRUE;
}

//------------------------------------
// S->C : ()
//------------------------------------
BOOL SC_ChaBeginSee(LPRPACKET pk) {
	//   McChaBeginSeeMessage
	net::msg::McChaBeginSeeMessage msg;
	net::msg::deserialize(pk, msg);

	stNetActorCreate SCreateInfo;
	convertBaseInfo(msg.base, SCreateInfo);
	SCreateInfo.chSeeType = static_cast<char>(msg.seeType);
	SCreateInfo.chMainCha = 0;
	CCharacter* pCha = SCreateInfo.CreateCha();
	if (!pCha) return FALSE;

	stNetNPCShow SNpcShow;
	// NPC   
	SNpcShow.byNpcType = static_cast<decltype(SNpcShow.byNpcType)>(msg.npcType);
	SNpcShow.byNpcState = static_cast<decltype(SNpcShow.byNpcState)>(msg.npcState);
	SNpcShow.SetNpcShow(pCha);

	//    std::variant
	switch (msg.poseType) {
	case enumPoseLean: {
		auto& lean = std::get<net::msg::LeanInfo>(msg.pose);
		stNetLeanInfo SLean;
		SLean.chState = static_cast<decltype(SLean.chState)>(lean.leanState);
		SLean.lPose = static_cast<long>(lean.pose);
		SLean.lAngle = static_cast<long>(lean.angle);
		SLean.lPosX = static_cast<long>(lean.posX);
		SLean.lPosY = static_cast<long>(lean.posY);
		SLean.lHeight = static_cast<long>(lean.height);
		NetActorLean(SCreateInfo.ulWorldID, SLean);
		break;
	}
	case enumPoseSeat: {
		auto& seat = std::get<net::msg::SeatInfo>(msg.pose);
		stNetFace SNetFace;
		SNetFace.sAngle = static_cast<short>(seat.seatAngle);
		SNetFace.sPose = static_cast<short>(seat.seatPose);
		NetFace(SCreateInfo.ulWorldID, SNetFace, enumACTION_SKILL_POSE);
		break;
	}
	default: {
		break;
	}
	}

	stNetChaAttr SChaAttr;
	convertAttr(msg.attr, SChaAttr);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff);

	stNetSkillState SCurSState;
	convertSkillState(msg.skillState, SCurSState);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	return TRUE;
}

//------------------------------------
// S->C : ()
//------------------------------------
BOOL SC_ChaEndSee(LPRPACKET pk) {
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	net::msg::McChaEndSeeMessage msg;
	net::msg::deserialize(pk, msg);
	char chSeeType = static_cast<char>(msg.seeType);
	uLong l_id = static_cast<uLong>(msg.worldId);
	NetActorDestroy(l_id, chSeeType);
	if (g_stUIStart.targetInfoID == l_id) {
		g_stUIStart.RemoveTarget();
	}
	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("+++++++++++++Destroy [{}]", l_id));
	//
	return TRUE;
}

BOOL SC_ItemCreate(LPRPACKET pk) {
	//    
	net::msg::McItemCreateMessage msg;
	net::msg::deserialize(pk, msg);

	stNetItemCreate SCreateInfo;
	memset(&SCreateInfo, 0, sizeof(SCreateInfo));
	SCreateInfo.lWorldID = static_cast<decltype(SCreateInfo.lWorldID)>(msg.worldId);
	SCreateInfo.lHandle = static_cast<decltype(SCreateInfo.lHandle)>(msg.handle);
	SCreateInfo.lID = static_cast<decltype(SCreateInfo.lID)>(msg.itemId);
	SCreateInfo.SPos.x = static_cast<decltype(SCreateInfo.SPos.x)>(msg.posX);
	SCreateInfo.SPos.y = static_cast<decltype(SCreateInfo.SPos.y)>(msg.posY);
	SCreateInfo.sAngle = static_cast<decltype(SCreateInfo.sAngle)>(msg.angle);
	SCreateInfo.sNum = static_cast<decltype(SCreateInfo.sNum)>(msg.num);
	SCreateInfo.chAppeType = static_cast<decltype(SCreateInfo.chAppeType)>(msg.appeType);
	SCreateInfo.lFromID = static_cast<decltype(SCreateInfo.lFromID)>(msg.fromId);
	SCreateInfo.SEvent.lEntityID = static_cast<decltype(SCreateInfo.SEvent.lEntityID)>(msg.event.entityId);
	SCreateInfo.SEvent.chEntityType = static_cast<decltype(SCreateInfo.SEvent.chEntityType)>(msg.event.entityType);
	SCreateInfo.SEvent.usEventID = static_cast<decltype(SCreateInfo.SEvent.usEventID)>(msg.event.eventId);
	SCreateInfo.SEvent.cszEventName = msg.event.eventName;

	CSceneItem* CItem = NetCreateItem(SCreateInfo);
	if (!CItem)
		return FALSE;

	// log
	ToLogService("common", "CreateType = {}, WorldID:{}, ItemID = {}, Pos = [{},{}], SrcID = {}",
				 static_cast<int>(SCreateInfo.chAppeType),
				 SCreateInfo.lWorldID, SCreateInfo.lID, SCreateInfo.SPos.x, SCreateInfo.SPos.y, SCreateInfo.lFromID);
	//
	return TRUE;
}

BOOL SC_ItemDestroy(LPRPACKET pk) {
	net::msg::McItemDestroyMessage msg;
	net::msg::deserialize(pk, msg);
	unsigned long lID = static_cast<unsigned long>(msg.worldId);

	NetItemDisappear(lID);
	ToLogService("common", "Item Destroy[{}]", lID);
	return TRUE;
}

BOOL SC_AStateBeginSee(LPRPACKET pk) {
	net::msg::McAStateBeginSeeMessage msg;
	net::msg::deserialize(pk, msg);

	stNetAreaState SynAState;
	char chValidNum = 0;
	SynAState.sAreaX = static_cast<short>(msg.areaX);
	SynAState.sAreaY = static_cast<short>(msg.areaY);
	SynAState.chStateNum = static_cast<char>(msg.states.size());
	for (char j = 0; j < SynAState.chStateNum; j++) {
		SynAState.State[chValidNum].chID = static_cast<decltype(SynAState.State[0].chID)>(msg.states[j].stateId);
		if (SynAState.State[chValidNum].chID == 0)
			continue;
		SynAState.State[chValidNum].chLv = static_cast<decltype(SynAState.State[0].chLv)>(msg.states[j].stateLv);
		SynAState.State[chValidNum].lWorldID = static_cast<decltype(SynAState.State[0].lWorldID)>(msg.states[j].
			worldId);
		SynAState.State[chValidNum].uchFightID = static_cast<decltype(SynAState.State[0].uchFightID)>(msg.states[j].
			fightId);
		chValidNum++;
	}
	SynAState.chStateNum = chValidNum;

	NetAreaStateBeginSee(&SynAState);

	//   
	{
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 SafeVFormat(GetLanguageString(296), SynAState.sAreaX, SynAState.sAreaY,
											 SynAState.chStateNum));
		for (char j = 0; j < SynAState.chStateNum; j++) {
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("\t{}\t{}", static_cast<int>(SynAState.State[j].chID),
												 static_cast<int>(SynAState.State[j].chLv)));
		}
	}
	//

	return TRUE;
}

BOOL SC_AStateEndSee(LPRPACKET pk) {
	//    
	net::msg::McAStateEndSeeMessage msg;
	net::msg::deserialize(pk, msg);

	stNetAreaState SynAState;
	SynAState.sAreaX = static_cast<decltype(SynAState.sAreaX)>(msg.areaX);
	SynAState.sAreaY = static_cast<decltype(SynAState.sAreaY)>(msg.areaY);

	NetAreaStateEndSee(&SynAState);

	// log
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 SafeVFormat(GetLanguageString(296), SynAState.sAreaX, SynAState.sAreaY, 0));
	//

	return TRUE;
}

//    (item cha)
BOOL SC_AddItemCha(LPRPACKET pk) {
	//   McAddItemChaMessage
	net::msg::McAddItemChaMessage msg;
	net::msg::deserialize(pk, msg);

	stNetActorCreate SCreateInfo;
	convertBaseInfo(msg.base, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 2;
	SCreateInfo.CreateCha();

	stNetChaAttr SChaAttr;
	convertAttr(msg.attr, SChaAttr);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff);

	stNetKitbag SKitbag;
	convertKitbag(msg.kitbag, SKitbag);
	SKitbag.chBagType = 0;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

	stNetSkillState SCurSState;
	convertSkillState(msg.skillState, SCurSState);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	return TRUE;
}

// S->C : 
BOOL SC_DelItemCha(LPRPACKET pk) {
	net::msg::McDelItemChaMessage msg;
	net::msg::deserialize(pk, msg);

	char chSeeType = enumENTITY_SEEN_NEW;
	uLong l_id = static_cast<uLong>(msg.worldId);
	NetActorDestroy(l_id, chSeeType);

	return TRUE;
}

// 
BOOL SC_Cha_Emotion(LPRPACKET pk) {
	net::msg::McChaEmotionMessage msg;
	net::msg::deserialize(pk, msg);

	uLong l_id = static_cast<uLong>(msg.worldId);
	uShort sEmotion = static_cast<uShort>(msg.emotion);

	NetChaEmotion(l_id, sEmotion);
	g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(297), sEmotion));
	return TRUE;
}

// S->C : 
BOOL SC_CharacterAction(LPRPACKET pk) {
	//  CMD_MC_NOTIACTION  McCharacterActionMessage
	net::msg::McCharacterActionMessage msg;
	net::msg::deserialize(pk, msg);

	uLong l_id = static_cast<uLong>(msg.worldId);
	{
		using namespace net::msg;

		if (msg.actionType == ActionType::MOVE) {
			auto& move = std::get<ActionMoveData>(msg.data);
			stNetNotiMove SMoveInfo;
			SMoveInfo.sState = static_cast<short>(move.moveState);
			if (SMoveInfo.sState != enumMSTATE_ON)
				SMoveInfo.sStopState = static_cast<short>(move.stopState);
			SMoveInfo.nPointNum = static_cast<int>(move.waypoints.size()) / sizeof(Point);
			memcpy(SMoveInfo.SPos, move.waypoints.data(), move.waypoints.size());

			// log — диагностика приёма Move-пакета. Раньше всё писалось в "common"
			// безусловно, что засоряло общий лог при активном движении любого
			// игрока в радиусе видимости. Теперь — канал "movie" уровня Debug
			// под флагом GameDiagnostic::IsMoveEnabled (ini [Logging] move).
			// Туда же ушла и визуальная отрисовка точек пути на карте, которая
			// раньше жила под #ifdef _STATE_DEBUG (см. цикл for ниже).
			CCharacter* pMainDbg = CGameScene::GetMainCha();
			bool isMainCha = pMainDbg && pMainDbg->getAttachID() == l_id;
			const bool moveDiag = GameDiagnostic::Instance().IsMoveEnabled();

			long lDistX, lDistY, lDist = 0;
			if (moveDiag) {
				ToLogService("movie", LogLevel::Debug,
							 "===Receive(Move): Tick=[{}], actor={}, isMain={}, points={}, state=0x{:x}",
							 GetTickCount(), l_id, isMainCha, SMoveInfo.nPointNum, SMoveInfo.sState);
			}
			for (int i = 0; i < SMoveInfo.nPointNum; i++) {
				//  Визуальная отладка: красные точки пути на карте, чёрная —
				//  endpoint при stop-state. Под общим флагом move-диагностики.
				if (moveDiag && isMainCha) {
					g_pGameApp->GetDrawPoints()->Add(SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, 0xffff0000, 0.5f);
					if (SMoveInfo.sState && i == SMoveInfo.nPointNum - 1) {
						g_pGameApp->GetDrawPoints()->Add(SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, 0xff000000, 0.3f);
					}
				}

				if (i > 0) {
					lDistX = SMoveInfo.SPos[i].x - SMoveInfo.SPos[i - 1].x;
					lDistY = SMoveInfo.SPos[i].y - SMoveInfo.SPos[i - 1].y;
					lDist = (long)sqrt((double)lDistX * lDistX + lDistY * lDistY);
				}
				if (moveDiag) {
					ToLogService("movie", LogLevel::Debug,
								 "\t{}, {}\t{}", SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, lDist);
				}
			}
			if (moveDiag && SMoveInfo.sState) {
				ToLogService("movie", LogLevel::Debug, "@@@End Move\tState:0x{:x}", SMoveInfo.sState);
			}


			if (SMoveInfo.sState & enumMSTATE_CANCEL) {
				long lDist;
				CCharacter* pCMainCha = CGameScene::GetMainCha();
				if (pCMainCha) {
					lDist = (pCMainCha->GetCurX() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x) * (pCMainCha->GetCurX() -
							SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x)
						+ (pCMainCha->GetCurY() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y) * (pCMainCha->GetCurY() -
							SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y);
					if (moveDiag) {
						ToLogService("movie", LogLevel::Debug,
									 "++++++++++++++Distance: {}", (long)sqrt(double(lDist)));
					}
				}
			}
			//


			NetActorMove(l_id, SMoveInfo);
		}
		else if (msg.actionType == ActionType::SKILL_SRC) {
			auto& d = std::get<ActionSkillSrcData>(msg.data);
			stNetNotiSkillRepresent SSkillInfo;
			SSkillInfo.chCrt = 0;
			SSkillInfo.sStopState = enumEXISTS_WAITING;

			SSkillInfo.byFightID = static_cast<BYTE>(d.fightId);
			SSkillInfo.sAngle = static_cast<short>(d.angle);
			SSkillInfo.sState = static_cast<short>(d.state);
			if (SSkillInfo.sState != enumFSTATE_ON)
				SSkillInfo.sStopState = static_cast<short>(d.stopState);
			SSkillInfo.lSkillID = static_cast<long>(d.skillId);
			SSkillInfo.lSkillSpeed = static_cast<long>(d.skillSpeed);
			if (d.targetType == 1) {
				SSkillInfo.lTargetID = static_cast<long>(d.targetId);
				SSkillInfo.STargetPoint.x = static_cast<long>(d.targetX);
				SSkillInfo.STargetPoint.y = static_cast<long>(d.targetY);
			}
			else if (d.targetType == 2) {
				SSkillInfo.lTargetID = 0;
				SSkillInfo.STargetPoint.x = static_cast<long>(d.targetX);
				SSkillInfo.STargetPoint.y = static_cast<long>(d.targetY);
			}
			SSkillInfo.sExecTime = static_cast<short>(d.execTime);

			//  
			short lResNum = static_cast<short>(d.effects.size());
			if (lResNum > 0) {
				SSkillInfo.SEffect.Resize(lResNum);
				for (long i = 0; i < lResNum; i++) {
					SSkillInfo.SEffect[i].lAttrID = static_cast<long>(d.effects[i].attrId);
					SSkillInfo.SEffect[i].lVal = static_cast<long>(d.effects[i].attrVal);
				}
			}

			//  
			short chSStateNum = static_cast<short>(d.states.size());
			if (chSStateNum > 0) {
				SSkillInfo.SState.Resize(chSStateNum);
				for (short chNum = 0; chNum < chSStateNum; chNum++) {
					SSkillInfo.SState[chNum].chID = static_cast<char>(d.states[chNum].stateId);
					SSkillInfo.SState[chNum].chLv = static_cast<char>(d.states[chNum].stateLv);
				}
			}

			// log
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("===Recieve(Skill Represent):\tTick:[{}]", GetTickCount()));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("Angle:\t{}\tFightID:{}", SSkillInfo.sAngle,
												 static_cast<int>(SSkillInfo.byFightID)));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("SkillID:\t{}\tSkillSpeed:{}", SSkillInfo.lSkillID,
												 SSkillInfo.lSkillSpeed));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("TargetInfo(ID, PosX, PosY):\t{}, {}, {}", SSkillInfo.lTargetID,
												 SSkillInfo.STargetPoint.x, SSkillInfo.STargetPoint.y));
			g_logManager.InternalLog(LogLevel::Debug, "common", "Effect:[ID, Value]");
			for (DWORD i = 0; i < SSkillInfo.SEffect.GetCount(); i++)
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("\t{},\t{}", SSkillInfo.SEffect[i].lAttrID,
													 SSkillInfo.SEffect[i].lVal));
			if (SSkillInfo.SState.GetCount() > 0)
				g_logManager.InternalLog(LogLevel::Debug, "common", "Skill State:[ID, LV]");
			for (DWORD chNum = 0; chNum < SSkillInfo.SState.GetCount(); chNum++)
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("\t{}, {}", static_cast<int>(SSkillInfo.SState[chNum].chID),
													 static_cast<int>(SSkillInfo.SState[chNum].chLv)));
			if (SSkillInfo.sState)
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("@@@End Skill\tState:0x{:x}", SSkillInfo.sState));
			//

			NetActorSkillRep(l_id, SSkillInfo);
		}
		else if (msg.actionType == ActionType::SKILL_TAR) {
			auto& d = std::get<ActionSkillTarData>(msg.data);
			stNetNotiSkillEffect SSkillInfo{};

			SSkillInfo.byFightID = static_cast<BYTE>(d.fightId);
			SSkillInfo.sState = static_cast<short>(d.state);
			SSkillInfo.bDoubleAttack = d.doubleAttack;
			SSkillInfo.bMiss = d.miss;
			SSkillInfo.bBeatBack = d.beatBack;
			if (SSkillInfo.bBeatBack) {
				SSkillInfo.SPos.x = static_cast<long>(d.beatBackX);
				SSkillInfo.SPos.y = static_cast<long>(d.beatBackY);
			}
			SSkillInfo.lSrcID = static_cast<long>(d.srcId);
			SSkillInfo.SSrcPos.x = static_cast<long>(d.srcPosX);
			SSkillInfo.SSrcPos.y = static_cast<long>(d.srcPosY);
			SSkillInfo.lSkillID = static_cast<long>(d.skillId);
			SSkillInfo.SSkillTPos.x = static_cast<long>(d.skillTargetX);
			SSkillInfo.SSkillTPos.y = static_cast<long>(d.skillTargetY);
			SSkillInfo.sExecTime = static_cast<short>(d.execTime);

			//  
			short lResNum = static_cast<short>(d.effects.size());
			if (lResNum > 0) {
				SSkillInfo.SEffect.Resize(lResNum);
				for (long i = 0; i < lResNum; i++) {
					SSkillInfo.SEffect[i].lAttrID = static_cast<long>(d.effects[i].attrId);
					SSkillInfo.SEffect[i].lVal = static_cast<long>(d.effects[i].attrVal);

					const std::string buff = std::format("ID = {} val = {}\r\n",
														 SSkillInfo.SEffect[i].lAttrID,
														 SSkillInfo.SEffect[i].lVal);
					::OutputDebugStringA(buff.c_str());
				}
			}

			//  
			if (d.hasStates) {
				short chSStateNum = static_cast<short>(d.states.size());
				if (chSStateNum > 0) {
					SSkillInfo.SState.Resize(chSStateNum);
					for (short chNum = 0; chNum < chSStateNum; chNum++) {
						SSkillInfo.SState[chNum].chID = static_cast<char>(d.states[chNum].stateId);
						SSkillInfo.SState[chNum].chLv = static_cast<char>(d.states[chNum].stateLv);
					}
				}
			}

			//  
			if (d.hasSrcEffect) {
				SSkillInfo.sSrcState = static_cast<short>(d.srcState);
				short lSrcResNum = static_cast<short>(d.srcEffects.size());
				if (lSrcResNum > 0) {
					SSkillInfo.SSrcEffect.Resize(lSrcResNum);
					for (long i = 0; i < lSrcResNum; i++) {
						SSkillInfo.SSrcEffect[i].lAttrID = static_cast<long>(d.srcEffects[i].attrId);
						SSkillInfo.SSrcEffect[i].lVal = static_cast<long>(d.srcEffects[i].attrVal);
					}
				}
				//  
				if (d.srcHasStates) {
					short chSrcStateNum = static_cast<short>(d.srcStates.size());
					if (chSrcStateNum > 0) {
						SSkillInfo.SSrcState.Resize(chSrcStateNum);
						for (short chNum = 0; chNum < chSrcStateNum; chNum++) {
							SSkillInfo.SSrcState[chNum].chID = static_cast<char>(d.srcStates[chNum].stateId);
							SSkillInfo.SSrcState[chNum].chLv = static_cast<char>(d.srcStates[chNum].stateLv);
						}
					}
				}
			}

			NetActorSkillEff(l_id, SSkillInfo);
		}
		else if (msg.actionType == ActionType::LEAN) {
			auto& lean = std::get<ActionLeanData>(msg.data);
			stNetLeanInfo SLean;
			SLean.chState = static_cast<char>(lean.leanState);
			if (!SLean.chState) {
				SLean.lPose = static_cast<long>(lean.pose);
				SLean.lAngle = static_cast<long>(lean.angle);
				SLean.lPosX = static_cast<long>(lean.posX);
				SLean.lPosY = static_cast<long>(lean.posY);
				SLean.lHeight = static_cast<long>(lean.height);
			}

			// log
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("===Recieve(Lean):\tTick:[{}]", GetTickCount()));
			if (SLean.chState)
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("@@@End Lean\tState:{}", static_cast<int>(SLean.chState)));
			//

			NetActorLean(l_id, SLean);
		}
		else if (msg.actionType == ActionType::ITEM_FAILED) {
			stNetSysInfo l_sysinfo;
			short sFailedID = static_cast<short>(std::get<ActionItemFailedData>(msg.data).failedId);
			l_sysinfo.m_sysinfo = g_GetUseItemFailedInfo(sFailedID);
			NetSysInfo(l_sysinfo);
		}
		else if (msg.actionType == ActionType::LOOK) {
			// LOOK    std::variant -> stNetLookInfo
			stNetLookInfo SLookInfo;
			memset(&SLookInfo, 0, sizeof(SLookInfo));
			auto& lk = std::get<ChaLookInfo>(msg.data);
			SLookInfo.chSynType = static_cast<char>(lk.synType);
			stNetChangeChaPart& SChaPart = SLookInfo.SLook;
			SChaPart.sTypeID = static_cast<short>(lk.typeId);
			if (lk.isBoat) {
				SChaPart.sPosID = static_cast<short>(lk.boatParts.posId);
				SChaPart.sBoatID = static_cast<short>(lk.boatParts.boatId);
				SChaPart.sHeader = static_cast<short>(lk.boatParts.header);
				SChaPart.sBody = static_cast<short>(lk.boatParts.body);
				SChaPart.sEngine = static_cast<short>(lk.boatParts.engine);
				SChaPart.sCannon = static_cast<short>(lk.boatParts.cannon);
				SChaPart.sEquipment = static_cast<short>(lk.boatParts.equipment);
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("===Recieve(Look):\tTick:[{}]", GetTickCount()));
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("TypeID:{}, PoseID:{}", SChaPart.sTypeID, SChaPart.sPosID));
			}
			else {
				SChaPart.sHairID = static_cast<short>(lk.hairId);
				for (int i = 0; i < enumEQUIP_NUM; i++) {
					SItemGrid* pItem = &SChaPart.SLink[i];
					auto& eq = lk.equips[i];
					pItem->sID = static_cast<short>(eq.id);
					pItem->dwDBID = static_cast<DWORD>(eq.dbId);
					pItem->sNeedLv = static_cast<short>(eq.needLv);
					if (pItem->sID == 0) {
						memset(pItem, 0, sizeof(SItemGrid));
						continue;
					}
					if (SLookInfo.chSynType == enumSYN_LOOK_CHANGE) {
						pItem->sEndure[0] = static_cast<short>(eq.endure0);
						pItem->sEnergy[0] = static_cast<short>(eq.energy0);
						pItem->SetValid(eq.valid != 0);
						pItem->bItemTradable = static_cast<int>(eq.tradable);
						pItem->expiration = static_cast<long>(eq.expiration);
					}
					else {
						pItem->sNum = static_cast<short>(eq.num);
						pItem->sEndure[0] = static_cast<short>(eq.endure0);
						pItem->sEndure[1] = static_cast<short>(eq.endure1);
						pItem->sEnergy[0] = static_cast<short>(eq.energy0);
						pItem->sEnergy[1] = static_cast<short>(eq.energy1);
						pItem->chForgeLv = static_cast<char>(eq.forgeLv);
						pItem->SetValid(eq.valid != 0);
						pItem->bItemTradable = static_cast<int>(eq.tradable);
						pItem->expiration = static_cast<long>(eq.expiration);
						if (eq.hasExtra) {
							pItem->SetDBParam(enumITEMDBP_FORGE, static_cast<long>(eq.forgeParam));
							pItem->SetDBParam(enumITEMDBP_INST_ID, static_cast<long>(eq.instId));
							if (eq.hasInstAttr) {
								for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++) {
									pItem->sInstAttr[j][0] = static_cast<short>(eq.instAttr[j][0]);
									pItem->sInstAttr[j][1] = static_cast<short>(eq.instAttr[j][1]);
								}
							}
						}
					}
				}
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("===Recieve(Look)\tTick:[{}]", GetTickCount()));
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("TypeID:{}, HairID:{}", SChaPart.sTypeID, SChaPart.sHairID));
				for (int i = 0; i < enumEQUIP_NUM; i++)
					g_logManager.InternalLog(LogLevel::Debug, "common",
											 std::format("\tLink: {}", SChaPart.SLink[i].sID));
			}

			CCharacter* pCha = CGameApp::GetCurScene()->SearchByID(l_id);
			if (!g_stUIMap.IsPKSilver() || pCha->GetMainType() == enumMainPlayer) {
				NetChangeChaPart(l_id, SLookInfo);
			}
		}
		else if (msg.actionType == ActionType::LOOK_ENERGY) {
			//  :    std::variant
			auto& energyData = std::get<ActionLookEnergyData>(msg.data);
			stLookEnergy SLookEnergy;
			memset(&SLookEnergy, 0, sizeof(SLookEnergy));
			for (int i = 0; i < enumEQUIP_NUM; i++)
				SLookEnergy.sEnergy[i] = static_cast<short>(energyData.energy[i]);
			NetChangeChaLookEnergy(l_id, SLookEnergy);
		}
		else if (msg.actionType == ActionType::KITBAG) {
			stNetKitbag SKitbag;
			ReadChaKitbagFromMsg(std::get<ChaKitbagInfo>(msg.data), SKitbag);
			SKitbag.chBagType = 0;
			NetChangeKitbag(l_id, SKitbag);
		}
		else if (msg.actionType == ActionType::ITEM_INFO) {
			//  
		}
		else if (msg.actionType == ActionType::SHORTCUT) {
			stNetShortCut SShortcut;
			ReadChaShortcutFromMsg(std::get<ChaShortcutInfo>(msg.data), SShortcut);
			NetShortCut(l_id, SShortcut);
		}
		else if (msg.actionType == ActionType::TEMP) {
			auto& temp = std::get<ActionTempData>(msg.data);
			stTempChangeChaPart STempChaPart;
			STempChaPart.dwItemID = static_cast<DWORD>(temp.itemId);
			STempChaPart.dwPartID = static_cast<DWORD>(temp.partId);
			NetTempChangeChaPart(l_id, STempChaPart);
		}
		else if (msg.actionType == ActionType::CHANGE_CHA) {
			stNetChangeCha SChangeCha;
			SChangeCha.ulMainChaID = static_cast<uLong>(std::get<ActionChangeChaData>(msg.data).mainChaId);
			NetActorChangeCha(l_id, SChangeCha);
		}
		else if (msg.actionType == ActionType::FACE) {
			auto& face = std::get<ActionFaceData>(msg.data);
			stNetFace SNetFace;
			SNetFace.sAngle = static_cast<short>(face.angle);
			SNetFace.sPose = static_cast<short>(face.pose);
			NetFace(l_id, SNetFace, enumACTION_FACE);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("===Recieve(Face):\tTick:[{}]", GetTickCount()));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("Angle[{}]\tPose[{}]", SNetFace.sAngle, SNetFace.sPose));
		}
		else if (msg.actionType == ActionType::SKILL_POSE) {
			auto& face = std::get<ActionFaceData>(msg.data);
			stNetFace SNetFace;
			SNetFace.sAngle = static_cast<short>(face.angle);
			SNetFace.sPose = static_cast<short>(face.pose);
			NetFace(l_id, SNetFace, enumACTION_SKILL_POSE);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("===Recieve(Skill Pos):\tTick:[{}]", GetTickCount()));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("Angle[{}]\tPose[{}]", SNetFace.sAngle, SNetFace.sPose));
		}
		else if (msg.actionType == ActionType::PK_CTRL) {
			//  : PK-  std::variant
			stNetPKCtrl SNetPKCtrl;
			char chPKCtrl = static_cast<char>(std::get<ActionPkCtrlData>(msg.data).pkCtrl);
			std::bitset<8> states(chPKCtrl);
			SNetPKCtrl.bInPK = states[0];
			SNetPKCtrl.bInGymkhana = states[1];
			SNetPKCtrl.pkGuild = states[2];
			SNetPKCtrl.Exec(l_id);
		}
		else if (msg.actionType == ActionType::BANK) {
			stNetKitbag SKitbag;
			ReadChaKitbagFromMsg(std::get<ChaKitbagInfo>(msg.data), SKitbag);
			SKitbag.chBagType = 1;
			NetChangeKitbag(l_id, SKitbag);
		}
		else if (msg.actionType == ActionType::GUILDBANK) {
			stNetKitbag SKitbag;
			ReadChaKitbagFromMsg(std::get<ChaKitbagInfo>(msg.data), SKitbag);
			SKitbag.chBagType = 3;
			NetChangeKitbag(l_id, SKitbag);
		}
		else if (msg.actionType == ActionType::KITBAGTMP) {
			stNetKitbag STempKitbag;
			ReadChaKitbagFromMsg(std::get<ChaKitbagInfo>(msg.data), STempKitbag);
			STempKitbag.chBagType = 2;
			NetChangeKitbag(l_id, STempKitbag);
		}
		else if (msg.actionType == ActionType::REQUESTGUILDLOGS) {
			auto& d = std::get<ActionRequestGuildLogsData>(msg.data);
			g_stUIGuildMgr.RequestGuildLogs(d);
		}
		else if (msg.actionType == ActionType::UPDATEGUILDLOGS) {
			auto& d = std::get<ActionUpdateGuildLogsData>(msg.data);
			g_stUIGuildMgr.UpdateGuildLogs(d);
		}
	}

	return TRUE;
}

BOOL SC_FailedAction(LPRPACKET pk) {
	//    
	net::msg::McFailedActionMessage msg;
	net::msg::deserialize(pk, msg);
	NetFailedAction(static_cast<char>(msg.reason));
	return TRUE;
}

// 
BOOL SC_SynAttribute(LPRPACKET pk) {
	net::msg::McSynAttributeMessage msg;
	net::msg::deserialize(pk, msg);
	uLong l_id = static_cast<uLong>(msg.worldId);

	stNetChaAttr SChaAttr;
	memset(&SChaAttr, 0, sizeof(SChaAttr));
	SChaAttr.chType = static_cast<char>(msg.attr.synType);
	SChaAttr.sNum = static_cast<short>(msg.attr.attrs.size());
	for (short i = 0; i < SChaAttr.sNum; i++) {
		SChaAttr.SEff[i].lAttrID = static_cast<long>(msg.attr.attrs[i].attrId);
		SChaAttr.SEff[i].lVal = static_cast<long>(msg.attr.attrs[i].attrVal);
	}

	//   
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("Syn Character Attr: Count={}\t, Type:{}\tTick:[{}]", SChaAttr.sNum,
										 static_cast<int>(SChaAttr.chType), GetTickCount()));
	g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(312));
	for (short i = 0; i < SChaAttr.sNum; i++) {
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 std::format("\t{}\t{}", SChaAttr.SEff[i].lAttrID, SChaAttr.SEff[i].lVal));
	}

	NetSynAttr(l_id, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff);

	return TRUE;
}

//   
BOOL SC_SynSkillBag(LPRPACKET pk) {
	//    
	net::msg::McSynSkillBagMessage msg;
	net::msg::deserialize(pk, msg);

	uLong l_id = static_cast<uLong>(msg.worldId);

	stNetSkillBag SCurSkill;
	memset(&SCurSkill, 0, sizeof(SCurSkill));

	//  default skill
	stNetDefaultSkill SDefaultSkill;
	SDefaultSkill.sSkillID = static_cast<decltype(SDefaultSkill.sSkillID)>(msg.skillBag.defSkillId);
	SDefaultSkill.Exec();

	SCurSkill.chType = static_cast<decltype(SCurSkill.chType)>(msg.skillBag.synType);
	short sSkillNum = static_cast<short>(msg.skillBag.skills.size());
	if (sSkillNum > 0) {
		SCurSkill.SBag.Resize(sSkillNum);
		SSkillGridEx* pSBag = SCurSkill.SBag.GetValue();
		for (short i = 0; i < sSkillNum; i++) {
			const auto& sk = msg.skillBag.skills[i];
			pSBag[i].sID = static_cast<decltype(pSBag[i].sID)>(sk.id);
			pSBag[i].chState = static_cast<decltype(pSBag[i].chState)>(sk.state);
			pSBag[i].chLv = static_cast<decltype(pSBag[i].chLv)>(sk.level);
			pSBag[i].sUseSP = static_cast<decltype(pSBag[i].sUseSP)>(sk.useSp);
			pSBag[i].sUseEndure = static_cast<decltype(pSBag[i].sUseEndure)>(sk.useEndure);
			pSBag[i].sUseEnergy = static_cast<decltype(pSBag[i].sUseEnergy)>(sk.useEnergy);
			pSBag[i].lResumeTime = static_cast<decltype(pSBag[i].lResumeTime)>(sk.resumeTime);
			for (int j = 0; j < defSKILL_RANGE_PARAM_NUM; j++)
				pSBag[i].sRange[j] = static_cast<short>(sk.range[j]);
		}

		// log
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 std::format("Syn Skill Bag, Type:{},\tTick:[{}]", static_cast<int>(SCurSkill.chType),
											 GetTickCount()));
		g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(310));
		for (short i = 0; i < sSkillNum; i++) {
			std::string szRange = std::format("{}", pSBag[i].sRange[0]);
			if (pSBag[i].sRange[0] != enumRANGE_TYPE_NONE) {
				for (short j = 1; j < defSKILL_RANGE_PARAM_NUM; j++) {
					std::format_to(std::back_inserter(szRange), ",{}", pSBag[i].sRange[j]);
				}
			}
			g_logManager.InternalLog(LogLevel::Debug, "common", std::format(
										 "\t{:4}\t{:4}\t{:4}\t{:6}\t{:6}\t{:6}\t{:18}\t{}", pSBag[i].sID,
										 static_cast<int>(pSBag[i].chState), static_cast<int>(pSBag[i].chLv),
										 pSBag[i].sUseSP, pSBag[i].sUseEndure, pSBag[i].sUseEnergy,
										 pSBag[i].lResumeTime, szRange));
		}
	}

	NetSynSkillBag(l_id, &SCurSkill);
	return TRUE;
}

// 
BOOL SC_SynDefaultSkill(LPRPACKET pk) {
	net::msg::McSynDefaultSkillMessage msg;
	net::msg::deserialize(pk, msg);
	uLong l_id = static_cast<uLong>(msg.worldId);
	stNetDefaultSkill SDefaultSkill;
	SDefaultSkill.sSkillID = static_cast<decltype(SDefaultSkill.sSkillID)>(msg.skillId);
	SDefaultSkill.Exec();
	return TRUE;
}

BOOL SC_SynSkillState(LPRPACKET pk) {
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	net::msg::McSynSkillStateMessage msg;
	net::msg::deserialize(pk, msg);
	uLong l_id = static_cast<uLong>(msg.worldId);

	unsigned long currentClient = GetTickCount();
	unsigned long currentServer = static_cast<unsigned long>(msg.skillState.currentTime) / 1000;

	stNetSkillState SCurSState;
	memset(&SCurSState, 0, sizeof(SCurSState));
	SCurSState.chType = 0;
	short sNum = static_cast<short>(msg.skillState.states.size());
	if (sNum > 0) {
		SCurSState.SState.Resize(sNum);
		for (int nNum = 0; nNum < sNum; nNum++) {
			SCurSState.SState[nNum].chID = static_cast<decltype(SCurSState.SState[nNum].chID)>(msg.skillState.states[
				nNum].stateId);
			SCurSState.SState[nNum].chLv = static_cast<decltype(SCurSState.SState[nNum].chLv)>(msg.skillState.states[
				nNum].stateLv);

			unsigned long duration = static_cast<unsigned long>(msg.skillState.states[nNum].duration);
			unsigned long start = static_cast<unsigned long>(msg.skillState.states[nNum].startTime) / 1000;

			unsigned long dif = currentServer - currentClient;
			unsigned long end = start - dif + duration;

			SCurSState.SState[nNum].lTimeRemaining = duration == 0 ? 0 : end - currentClient;
		}
	}

	// log
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("Syn Skill State: Num[{}]\tTick[{}]", sNum, GetTickCount()));
	g_logManager.InternalLog(LogLevel::Debug, "common", GetLanguageString(311));
	for (char i = 0; i < sNum; i++)
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 std::format("\t{:8}\t{:4}", static_cast<int>(SCurSState.SState[i].chID),
											 static_cast<int>(SCurSState.SState[i].chLv)));

	NetSynSkillState(l_id, &SCurSState);

	return TRUE;
}

BOOL SC_SynTeam(LPRPACKET pk) {
	//   McSynTeamMessage
	net::msg::McSynTeamMessage msg;
	net::msg::deserialize(pk, msg);

	stNetTeamState STeamState;
	STeamState.ulID = static_cast<decltype(STeamState.ulID)>(msg.memberId);
	STeamState.lHP = static_cast<decltype(STeamState.lHP)>(msg.hp);
	STeamState.lMaxHP = static_cast<decltype(STeamState.lMaxHP)>(msg.maxHP);
	STeamState.lSP = static_cast<decltype(STeamState.lSP)>(msg.sp);
	STeamState.lMaxSP = static_cast<decltype(STeamState.lMaxSP)>(msg.maxSP);
	STeamState.lLV = static_cast<decltype(STeamState.lLV)>(msg.level);

	ToLogService("players", "Refresh, ID[{}], HP[{}], MaxHP[{}], SP[{}], MaxSP[{}], LV[{}]", STeamState.ulID,
				 STeamState.lHP,
				 STeamState.lMaxHP, STeamState.lSP, STeamState.lMaxSP, STeamState.lLV);

	//  ChaLookInfo  stNetChangeChaPart  
	STeamState.SFace.sTypeID = static_cast<short>(msg.look.typeId);
	STeamState.SFace.sHairID = static_cast<short>(msg.look.hairId);
	for (int i = 0; i < enumEQUIP_NUM; i++) {
		const auto& eq = msg.look.equips[i];
		STeamState.SFace.SLink[i].sID = static_cast<short>(eq.id);
		STeamState.SFace.SLink[i].sNum = static_cast<short>(eq.num);
		STeamState.SFace.SLink[i].chForgeLv = static_cast<char>(eq.forgeLv);
		STeamState.SFace.SLink[i].lFuseID = static_cast<long>(eq.instId) >> 16;
	}

	NetSynTeam(&STeamState);

	return true;
}

BOOL SC_SynTLeaderID(LPRPACKET pk) {
	net::msg::McSynTLeaderIdMessage msg;
	net::msg::deserialize(pk, msg);
	long lID = static_cast<long>(msg.worldId);
	long lLeaderID = static_cast<long>(msg.leaderId);

	NetChaTLeaderID(lID, lLeaderID);

	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(300), lLeaderID, lID));
	//

	return TRUE;
}

BOOL SC_HelpInfo(LPRPACKET packet) {
	//   McHelpInfoMessage
	net::msg::McHelpInfoMessage msg;
	net::msg::deserialize(packet, msg);

	NET_HELPINFO Info;
	memset(&Info, 0, sizeof(NET_HELPINFO));

	Info.byType = static_cast<decltype(Info.byType)>(msg.type);
	if (Info.byType == mission::MIS_HELP_DESP || Info.byType == mission::MIS_HELP_IMAGE ||
		Info.byType == mission::MIS_HELP_BICKER) {
		strncpy(Info.szDesp, msg.desp.c_str(), ROLE_MAXNUM_DESPSIZE - 1);
	}
	else if (Info.byType == mission::MIS_HELP_SOUND) {
		Info.sID = static_cast<decltype(Info.sID)>(msg.soundId);
	}
	else {
		return FALSE;
	}

	NetShowHelp(Info);
	return TRUE;
}

// NPC 
BOOL SC_TalkInfo(LPRPACKET packet) {
	net::msg::McTalkInfoMessage msg;
	net::msg::deserialize(packet, msg);
	if (msg.text.empty()) return FALSE;
	NetShowTalk(msg.text.c_str(), static_cast<BYTE>(msg.cmd), static_cast<DWORD>(msg.npcId));
	return TRUE;
}

BOOL SC_FuncInfo(LPRPACKET packet) {
	net::msg::McFuncInfoMessage msg;
	net::msg::deserialize(packet, msg);

	NET_FUNCPAGE FuncPage;
	memset(&FuncPage, 0, sizeof(NET_FUNCPAGE));

	DWORD dwNpcID = static_cast<DWORD>(msg.npcId);
	BYTE byPage = static_cast<BYTE>(msg.page);
	strncpy(FuncPage.szTalk, msg.talkText.c_str(), ROLE_MAXNUM_DESPSIZE - 1);

	BYTE byCount = static_cast<BYTE>(msg.funcItems.size());
	if (byCount > ROLE_MAXNUM_FUNCITEM) byCount = ROLE_MAXNUM_FUNCITEM;
	for (int i = 0; i < byCount; i++) {
		strncpy(FuncPage.FuncItem[i].szFunc, msg.funcItems[i].name.c_str(), ROLE_MAXNUM_FUNCITEMSIZE - 1);
	}

	BYTE byMisCount = static_cast<BYTE>(msg.missionItems.size());
	if (byMisCount > ROLE_MAXNUM_CAPACITY) {
		byMisCount = ROLE_MAXNUM_CAPACITY;
	}

	for (int i = 0; i < byMisCount; i++) {
		strncpy(FuncPage.MisItem[i].szMis, msg.missionItems[i].name.c_str(), ROLE_MAXNUM_FUNCITEMSIZE - 1);
		FuncPage.MisItem[i].byState = static_cast<BYTE>(msg.missionItems[i].state);
	}

	NetShowFunction(byPage, byCount, byMisCount, FuncPage, dwNpcID);
	return TRUE;
}

BOOL SC_CloseTalk(LPRPACKET packet) {
	net::msg::McCloseTalkMessage msg;
	net::msg::deserialize(packet, msg);
	NetCloseTalk(static_cast<DWORD>(msg.npcId));
	return TRUE;
}

BOOL SC_TradeData(LPRPACKET packet) {
	net::msg::McTradeDataMessage msg;
	net::msg::deserialize(packet, msg);

	NetUpdateTradeData(static_cast<DWORD>(msg.npcId), static_cast<BYTE>(msg.page),
					   static_cast<BYTE>(msg.index), static_cast<USHORT>(msg.itemId),
					   static_cast<USHORT>(msg.count), static_cast<DWORD>(msg.price));
	return TRUE;
}

//  :  McTradeAllDataMessage  NET_TRADEINFO
static void convertTradeMsg(const net::msg::McTradeAllDataMessage& msg, NET_TRADEINFO& Info) {
	memset(&Info, 0, sizeof(Info));
	for (const auto& page : msg.pages) {
		BYTE byItemType = static_cast<BYTE>(page.itemType);
		if (byItemType > 3) continue; // TI_WEAPON..TI_SYNTHESIS
		Info.TradePage[byItemType].byCount = static_cast<BYTE>(page.items.size());
		if (Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM)
			Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;
		for (BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++) {
			Info.TradePage[byItemType].sItemID[n] = static_cast<USHORT>(page.items[n].itemId);
			if (msg.tradeType == mission::TRADE_GOODS) {
				Info.TradePage[byItemType].sCount[n] = static_cast<USHORT>(page.items[n].count);
				Info.TradePage[byItemType].dwPrice[n] = static_cast<DWORD>(page.items[n].price);
				Info.TradePage[byItemType].byLevel[n] = static_cast<BYTE>(page.items[n].level);
			}
		}
	}
}

BOOL SC_TradeAllData(LPRPACKET packet) {
	//   McTradeAllDataMessage
	net::msg::McTradeAllDataMessage msg;
	net::msg::deserialize(packet, msg);

	NET_TRADEINFO Info;
	convertTradeMsg(msg, Info);
	NetUpdateTradeAllData(Info, static_cast<BYTE>(msg.tradeType), static_cast<DWORD>(msg.npcId),
						  static_cast<DWORD>(msg.param));
	return TRUE;
}

BOOL SC_TradeInfo(LPRPACKET packet) {
	//   McTradeAllDataMessage (  )
	net::msg::McTradeAllDataMessage msg;
	net::msg::deserialize(packet, msg);

	NET_TRADEINFO Info;
	convertTradeMsg(msg, Info);
	NetShowTrade(Info, static_cast<BYTE>(msg.tradeType), static_cast<DWORD>(msg.npcId), static_cast<DWORD>(msg.param));
	return TRUE;
}

BOOL SC_TradeUpdate(LPRPACKET packet) {
	//   McTradeAllDataMessage (  )
	net::msg::McTradeAllDataMessage msg;
	net::msg::deserialize(packet, msg);

	NET_TRADEINFO Info;
	convertTradeMsg(msg, Info);

	if (g_stUINpcTrade.GetIsShow()) {
		NetShowTrade(Info, static_cast<BYTE>(msg.tradeType), static_cast<DWORD>(msg.npcId),
					 static_cast<DWORD>(msg.param));
	}

	return TRUE;
}

BOOL SC_TradeResult(LPRPACKET packet) {
	net::msg::McTradeResultMessage msg;
	net::msg::deserialize(packet, msg);
	BYTE byType = static_cast<BYTE>(msg.type);
	BYTE byIndex = static_cast<BYTE>(msg.index);
	BYTE byCount = static_cast<BYTE>(msg.count);
	USHORT sItemID = static_cast<USHORT>(msg.itemId);
	DWORD dwMoney = static_cast<DWORD>(msg.money);
	g_logManager.InternalLog(LogLevel::Debug, "trade",
							 SafeVFormat(GetLanguageString(301), byType, byIndex, byCount, sItemID, dwMoney));
	NetTradeResult(byType, byIndex, byCount, sItemID, dwMoney);
	g_logManager.InternalLog(LogLevel::Debug, "trade", GetLanguageString(302));
	return TRUE;
}

BOOL SC_CharTradeInfo(LPRPACKET packet) {
	//      
	USHORT usCmd = packet.ReadInt64();
	switch (usCmd) {
	case CMD_MC_CHARTRADE_REQUEST: {
		net::msg::McCharTradeRequestMessage msg;
		net::msg::deserialize(packet, msg);
		NetShowCharTradeRequest(static_cast<BYTE>(msg.tradeType), static_cast<DWORD>(msg.chaId));
	}
	break;
	case CMD_MC_CHARTRADE_CANCEL: {
		net::msg::McCharTradeCancelMessage msg;
		net::msg::deserialize(packet, msg);
		NetCancelCharTrade(static_cast<DWORD>(msg.chaId));
	}
	break;
	case CMD_MC_CHARTRADE_MONEY: {
		net::msg::McCharTradeMoneyMessage msg;
		net::msg::deserialize(packet, msg);
		if (msg.isIMP == 0) {
			NetTradeShowMoney(static_cast<DWORD>(msg.chaId), static_cast<DWORD>(msg.money));
		}
		else if (msg.isIMP == 1) {
			NetTradeShowIMP(static_cast<DWORD>(msg.chaId), static_cast<DWORD>(msg.money));
		}
	}
	break;
	case CMD_MC_CHARTRADE_ITEM: {
		//      
		net::msg::McCharTradeItemMessage msg;
		net::msg::deserialize(packet, msg);

		if (auto* remove = std::get_if<net::msg::McCharTradeItemRemoveData>(&msg.data)) {
			//     
			NET_CHARTRADE_ITEMDATA Data;
			memset(&Data, 0, sizeof(NET_CHARTRADE_ITEMDATA));
			NetTradeAddItem(static_cast<DWORD>(msg.mainChaId), static_cast<BYTE>(msg.opType), 0,
							static_cast<BYTE>(remove->tradeIndex), static_cast<BYTE>(remove->count),
							static_cast<BYTE>(remove->bagIndex), Data);
		}
		else if (auto* add = std::get_if<net::msg::McCharTradeItemAddData>(&msg.data)) {
			bool isBoat = (add->itemType == enumItemTypeBoat);
			if (isBoat) {
				// 
				auto* boatPtr = std::get_if<net::msg::TradeBoatData>(&add->equipData);
				if (boatPtr && boatPtr->hasBoat) {
					NET_CHARTRADE_BOATDATA Data;
					memset(&Data, 0, sizeof(NET_CHARTRADE_BOATDATA));
					strncpy(Data.szName, boatPtr->name.c_str(), BOAT_MAXSIZE_BOATNAME - 1);
					Data.sBoatID = static_cast<decltype(Data.sBoatID)>(boatPtr->ship);
					Data.sLevel = static_cast<decltype(Data.sLevel)>(boatPtr->lv);
					Data.dwExp = static_cast<DWORD>(boatPtr->cexp);
					Data.dwHp = static_cast<DWORD>(boatPtr->hp);
					Data.dwMaxHp = static_cast<DWORD>(boatPtr->mxhp);
					Data.dwSp = static_cast<DWORD>(boatPtr->sp);
					Data.dwMaxSp = static_cast<DWORD>(boatPtr->mxsp);
					Data.dwMinAttack = static_cast<DWORD>(boatPtr->mnatk);
					Data.dwMaxAttack = static_cast<DWORD>(boatPtr->mxatk);
					Data.dwDef = static_cast<DWORD>(boatPtr->def);
					Data.dwSpeed = static_cast<DWORD>(boatPtr->mspd);
					Data.dwShootSpeed = static_cast<DWORD>(boatPtr->aspd);
					Data.byHasItem = static_cast<BYTE>(boatPtr->useGridNum);
					Data.byCapacity = static_cast<BYTE>(boatPtr->capacity);
					Data.dwPrice = static_cast<DWORD>(boatPtr->price);
					NetTradeAddBoat(static_cast<DWORD>(msg.mainChaId), static_cast<BYTE>(msg.opType),
									static_cast<USHORT>(add->itemId), static_cast<BYTE>(add->tradeIndex),
									static_cast<BYTE>(add->count), static_cast<BYTE>(add->bagIndex), Data);
				}
			}
			else {
				//  
				auto* itemPtr = std::get_if<net::msg::TradeItemData>(&add->equipData);
				if (itemPtr) {
					NET_CHARTRADE_ITEMDATA Data;
					memset(&Data, 0, sizeof(NET_CHARTRADE_ITEMDATA));
					Data.sEndure[0] = static_cast<short>(itemPtr->endure0);
					Data.sEndure[1] = static_cast<short>(itemPtr->endure1);
					Data.sEnergy[0] = static_cast<short>(itemPtr->energy0);
					Data.sEnergy[1] = static_cast<short>(itemPtr->energy1);
					Data.byForgeLv = static_cast<BYTE>(itemPtr->forgeLv);
					Data.bValid = itemPtr->valid != 0;
					Data.bItemTradable = static_cast<int>(itemPtr->tradable);
					Data.expiration = static_cast<long>(itemPtr->expiration);
					Data.lDBParam[enumITEMDBP_FORGE] = static_cast<long>(itemPtr->forgeParam);
					Data.lDBParam[enumITEMDBP_INST_ID] = static_cast<long>(itemPtr->instId);
					Data.byHasAttr = itemPtr->hasInstAttr ? 1 : 0;
					if (itemPtr->hasInstAttr) {
						for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; ++j) {
							Data.sInstAttr[j][0] = static_cast<short>(itemPtr->instAttr[j][0]);
							Data.sInstAttr[j][1] = static_cast<short>(itemPtr->instAttr[j][1]);
						}
					}
					NetTradeAddItem(static_cast<DWORD>(msg.mainChaId), static_cast<BYTE>(msg.opType),
									static_cast<USHORT>(add->itemId), static_cast<BYTE>(add->tradeIndex),
									static_cast<BYTE>(add->count), static_cast<BYTE>(add->bagIndex), Data);
				}
			}
		}
		else {
			MessageBox(NULL, GetLanguageString(303).c_str(), GetLanguageString(25).c_str(), MB_OK);
			return FALSE;
		}
	}
	break;
	case CMD_MC_CHARTRADE_PAGE: {
		net::msg::McCharTradePageMessage msg;
		net::msg::deserialize(packet, msg);
		NetShowCharTradeInfo(static_cast<BYTE>(msg.tradeType), static_cast<DWORD>(msg.mainChaId),
							 static_cast<DWORD>(msg.otherChaId));
	}
	break;
	case CMD_MC_CHARTRADE_VALIDATEDATA: {
		net::msg::McCharTradeValidateDataMessage msg;
		net::msg::deserialize(packet, msg);
		NetValidateTradeData(static_cast<DWORD>(msg.chaId));
	}
	break;
	case CMD_MC_CHARTRADE_VALIDATE: {
		net::msg::McCharTradeValidateMessage msg;
		net::msg::deserialize(packet, msg);
		NetValidateTrade(static_cast<DWORD>(msg.chaId));
	}
	break;
	case CMD_MC_CHARTRADE_RESULT: {
		net::msg::McCharTradeResultMessage msg;
		net::msg::deserialize(packet, msg);
		if (msg.result == mission::TRADE_SUCCESS) {
			NetTradeSuccess();
		}
		else {
			NetTradeFailed();
		}
	}
	break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}

BOOL SC_DailyBuffInfo(LPRPACKET packet) {
	net::msg::McDailyBuffInfoMessage msg;
	net::msg::deserialize(packet, msg);
	if (msg.imgName.empty()) {
		ToLogService("errors", LogLevel::Error, "DailyBuffInfo: invalid reading image name");
		return FALSE;
	}
	if (msg.labelInfo.empty()) {
		ToLogService("errors", LogLevel::Error, "DailyBuffInfo: invalid reading labelInfo");
		return FALSE;
	}
	//show the form
	g_stUIMap.SetupDailyBuffForm(msg.imgName.c_str(), msg.labelInfo.c_str());
	return TRUE;
}

BOOL SC_MissionInfo(LPRPACKET packet) {
	//    
	net::msg::McMissionInfoMessage msg;
	net::msg::deserialize(packet, msg);

	NET_MISSIONLIST list;
	memset(&list, 0, sizeof(NET_MISSIONLIST));

	list.byListType = static_cast<decltype(list.byListType)>(msg.listType);
	list.byPrev = static_cast<decltype(list.byPrev)>(msg.prev);
	list.byNext = static_cast<decltype(list.byNext)>(msg.next);
	list.byPrevCmd = static_cast<decltype(list.byPrevCmd)>(msg.prevCmd);
	list.byNextCmd = static_cast<decltype(list.byNextCmd)>(msg.nextCmd);
	list.byItemCount = static_cast<decltype(list.byItemCount)>(msg.items.size());

	if (list.byItemCount > ROLE_MAXNUM_FUNCITEM) list.byItemCount = ROLE_MAXNUM_FUNCITEM;
	for (int i = 0; i < list.byItemCount; i++) {
		strncpy(list.FuncPage.FuncItem[i].szFunc, msg.items[i].c_str(), 32);
	}

	NetShowMissionList(static_cast<DWORD>(msg.npcId), list);
	return TRUE;
}

BOOL SC_MisPage(LPRPACKET packet) {
	//    
	net::msg::McMisPageMessage msg;
	net::msg::deserialize(packet, msg);

	BYTE byCmd = static_cast<BYTE>(msg.cmd);
	DWORD dwNpcID = static_cast<DWORD>(msg.npcId);
	NET_MISPAGE page;
	memset(&page, 0, sizeof(NET_MISPAGE));

	strncpy(page.szName, msg.name.c_str(), ROLE_MAXSIZE_MISNAME - 1);

	switch (byCmd) {
	case ROLE_MIS_BTNACCEPT:
	case ROLE_MIS_BTNDELIVERY:
	case ROLE_MIS_BTNPENDING: {
		//  
		page.byNeedNum = static_cast<decltype(page.byNeedNum)>(msg.needs.size());
		for (int i = 0; i < page.byNeedNum; i++) {
			page.MisNeed[i].byType = static_cast<decltype(page.MisNeed[i].byType)>(msg.needs[i].needType);
			if (page.MisNeed[i].byType == mission::MIS_NEED_ITEM || page.MisNeed[i].byType == mission::MIS_NEED_KILL) {
				page.MisNeed[i].wParam1 = static_cast<decltype(page.MisNeed[i].wParam1)>(msg.needs[i].param1);
				page.MisNeed[i].wParam2 = static_cast<decltype(page.MisNeed[i].wParam2)>(msg.needs[i].param2);
				page.MisNeed[i].wParam3 = static_cast<decltype(page.MisNeed[i].wParam3)>(msg.needs[i].param3);
			}
			else if (page.MisNeed[i].byType == mission::MIS_NEED_DESP) {
				strncpy(page.MisNeed[i].szNeed, msg.needs[i].desp.c_str(), ROLE_MAXNUM_NEEDDESPSIZE - 1);
			}
			else {
				g_logManager.InternalLog(LogLevel::Error, "errors", GetLanguageString(304));
				return FALSE;
			}
		}

		//  
		page.byPrizeSelType = static_cast<decltype(page.byPrizeSelType)>(msg.prizeSelType);
		page.byPrizeNum = static_cast<decltype(page.byPrizeNum)>(msg.prizes.size());
		for (int i = 0; i < page.byPrizeNum; i++) {
			page.MisPrize[i].byType = static_cast<decltype(page.MisPrize[i].byType)>(msg.prizes[i].type);
			page.MisPrize[i].wParam1 = static_cast<decltype(page.MisPrize[i].wParam1)>(msg.prizes[i].param1);
			page.MisPrize[i].wParam2 = static_cast<decltype(page.MisPrize[i].wParam2)>(msg.prizes[i].param2);
		}

		//  
		strncpy(page.szDesp, msg.description.c_str(), ROLE_MAXNUM_DESPSIZE - 1);
	}
	break;
	default:
		return FALSE;
	}

	NetShowMisPage(dwNpcID, byCmd, page);
	return TRUE;
}

BOOL SC_MisLog(LPRPACKET packet) {
#ifdef _TEST_CLIENT
	return TRUE;
#endif
	//    
	net::msg::McMisLogMessage msg;
	net::msg::deserialize(packet, msg);

	NET_MISLOG_LIST LogList;
	memset(&LogList, 0, sizeof(NET_MISLOG_LIST));

	LogList.byNumLog = static_cast<decltype(LogList.byNumLog)>(msg.logs.size());
	for (int i = 0; i < LogList.byNumLog; i++) {
		LogList.MisLog[i].wMisID = static_cast<decltype(LogList.MisLog[i].wMisID)>(msg.logs[i].misId);
		LogList.MisLog[i].byState = static_cast<decltype(LogList.MisLog[i].byState)>(msg.logs[i].state);
	}

	NetMisLogList(LogList);
	return TRUE;
}

BOOL SC_MisLogInfo(LPRPACKET packet) {
	//    
	net::msg::McMisLogInfoMessage msg;
	net::msg::deserialize(packet, msg);

	WORD wMisID = static_cast<WORD>(msg.misId);
	NET_MISPAGE page;
	memset(&page, 0, sizeof(NET_MISPAGE));

	strncpy(page.szName, msg.name.c_str(), ROLE_MAXSIZE_MISNAME - 1);

	//  
	page.byNeedNum = static_cast<decltype(page.byNeedNum)>(msg.needs.size());
	for (int i = 0; i < page.byNeedNum; i++) {
		page.MisNeed[i].byType = static_cast<decltype(page.MisNeed[i].byType)>(msg.needs[i].needType);
		if (page.MisNeed[i].byType == mission::MIS_NEED_ITEM || page.MisNeed[i].byType == mission::MIS_NEED_KILL) {
			page.MisNeed[i].wParam1 = static_cast<decltype(page.MisNeed[i].wParam1)>(msg.needs[i].param1);
			page.MisNeed[i].wParam2 = static_cast<decltype(page.MisNeed[i].wParam2)>(msg.needs[i].param2);
			page.MisNeed[i].wParam3 = static_cast<decltype(page.MisNeed[i].wParam3)>(msg.needs[i].param3);
		}
		else if (page.MisNeed[i].byType == mission::MIS_NEED_DESP) {
			strncpy(page.MisNeed[i].szNeed, msg.needs[i].desp.c_str(), ROLE_MAXNUM_NEEDDESPSIZE - 1);
		}
		else {
			g_logManager.InternalLog(LogLevel::Error, "errors", GetLanguageString(304));
			return FALSE;
		}
	}

	//  
	page.byPrizeSelType = static_cast<decltype(page.byPrizeSelType)>(msg.prizeSelType);
	page.byPrizeNum = static_cast<decltype(page.byPrizeNum)>(msg.prizes.size());
	for (int i = 0; i < page.byPrizeNum; i++) {
		page.MisPrize[i].byType = static_cast<decltype(page.MisPrize[i].byType)>(msg.prizes[i].type);
		page.MisPrize[i].wParam1 = static_cast<decltype(page.MisPrize[i].wParam1)>(msg.prizes[i].param1);
		page.MisPrize[i].wParam2 = static_cast<decltype(page.MisPrize[i].wParam2)>(msg.prizes[i].param2);
	}

	//  
	strncpy(page.szDesp, msg.description.c_str(), ROLE_MAXNUM_DESPSIZE - 1);

	NetShowMisLog(wMisID, page);
	return TRUE;
}

BOOL SC_MisLogClear(LPRPACKET packet) {
	net::msg::McMisLogClearMcMessage msg;
	net::msg::deserialize(packet, msg);

	NetMisLogClear(static_cast<WORD>(msg.missionId));
	return TRUE;
}

BOOL SC_MisLogAdd(LPRPACKET packet) {
	//    
	net::msg::McMisLogAddMessage msg;
	net::msg::deserialize(packet, msg);
	NetMisLogAdd(static_cast<WORD>(msg.missionId), static_cast<BYTE>(msg.state));
	return TRUE;
}

BOOL SC_MisLogState(LPRPACKET packet) {
	//    
	net::msg::McMisLogStateMessage msg;
	net::msg::deserialize(packet, msg);
	NetMisLogState(static_cast<WORD>(msg.missionId), static_cast<BYTE>(msg.state));
	return TRUE;
}

BOOL SC_TriggerAction(LPRPACKET packet) {
	//    
	net::msg::McTriggerActionMessage msg;
	net::msg::deserialize(packet, msg);
	stNetNpcMission info;
	info.byType = static_cast<decltype(info.byType)>(msg.type);
	info.sID = static_cast<decltype(info.sID)>(msg.id);
	info.sNum = static_cast<decltype(info.sNum)>(msg.num);
	info.sCount = static_cast<decltype(info.sCount)>(msg.count);
	NetTriggerAction(info);
	return TRUE;
}

BOOL SC_NpcStateChange(LPRPACKET packet) {
	net::msg::McNpcStateChangeMessage msg;
	net::msg::deserialize(packet, msg);
	NetNpcStateChange(static_cast<DWORD>(msg.npcId), static_cast<BYTE>(msg.state));
	return TRUE;
}

BOOL SC_EntityStateChange(LPRPACKET packet) {
	net::msg::McEntityStateChangeMessage msg;
	net::msg::deserialize(packet, msg);
	NetEntityStateChange(static_cast<DWORD>(msg.entityId), static_cast<BYTE>(msg.state));
	return TRUE;
}

BOOL SC_Forge(LPRPACKET packet) {
	NetShowForge();
	return TRUE;
}


BOOL SC_Unite(LPRPACKET packet) {
	NetShowUnite();
	return TRUE;
}

BOOL SC_Milling(LPRPACKET packet) {
	NetShowMilling();
	return TRUE;
}

BOOL SC_Fusion(LPRPACKET packet) {
	NetShowFusion();
	return TRUE;
}

BOOL SC_Upgrade(LPRPACKET packet) {
	NetShowUpgrade();
	return TRUE;
}

BOOL SC_EidolonMetempsychosis(LPRPACKET packet) {
	//NetShowEidolonMetempsychosis();
	NetShowEidolonFusion();
	return TRUE;
}

BOOL SC_Eidolon_Fusion(LPRPACKET packet) {
	NetShowEidolonFusion();
	return TRUE;
}

BOOL SC_Purify(LPRPACKET packet) {
	NetShowPurify();
	return TRUE;
}

BOOL SC_Fix(LPRPACKET packet) {
	NetShowRepairOven();
	return TRUE;
}

BOOL SC_GMSend(LPRPACKET packet) {
	g_stUIMail.ShowQuestionForm();
	return TRUE;
}

BOOL SC_GMRecv(LPRPACKET packet) {
	net::msg::McGmRecvMessage msg;
	net::msg::deserialize(packet, msg);
	CS_GMRecv(static_cast<DWORD>(msg.npcId));
	return TRUE;
}

BOOL SC_GetStone(LPRPACKET packet) {
	NetShowGetStone();
	return TRUE;
}

BOOL SC_Energy(LPRPACKET packet) {
	NetShowEnergy();
	return TRUE;
}

BOOL SC_Tiger(LPRPACKET packet) {
	NetShowTiger();
	return TRUE;
}

//  :  McBoatSyncAttrMessage  xShipBuildInfo
static void convertBoatMsg(const net::msg::McBoatSyncAttrMessage& msg, xShipBuildInfo& Info) {
	memset(&Info, 0, sizeof(xShipBuildInfo));
	strncpy(Info.szName, msg.shipName.c_str(), BOAT_MAXSIZE_NAME - 1);
	strncpy(Info.szDesp, msg.shipDesc.c_str(), BOAT_MAXSIZE_DESP - 1);
	strncpy(Info.szBerth, msg.berthName.c_str(), BOAT_MAXSIZE_NAME - 1);
	Info.byIsUpdate = static_cast<BYTE>(msg.isUpdate);
	Info.sPosID = static_cast<decltype(Info.sPosID)>(msg.body.partId);
	Info.dwBody = static_cast<DWORD>(msg.body.model);
	strncpy(Info.szBody, msg.body.name.c_str(), BOAT_MAXSIZE_NAME - 1);
	if (Info.byIsUpdate) {
		Info.byHeader = static_cast<BYTE>(msg.header.partId);
		Info.dwHeader = static_cast<DWORD>(msg.header.model);
		strncpy(Info.szHeader, msg.header.name.c_str(), BOAT_MAXSIZE_NAME - 1);
		Info.byEngine = static_cast<BYTE>(msg.engine.partId);
		Info.dwEngine = static_cast<DWORD>(msg.engine.model);
		strncpy(Info.szEngine, msg.engine.name.c_str(), BOAT_MAXSIZE_NAME - 1);
		for (int i = 0; i < BOAT_MAXNUM_MOTOR && i < 4; i++)
			Info.dwMotor[i] = static_cast<DWORD>(msg.motorModels[i]);
	}
	Info.byCannon = static_cast<BYTE>(msg.cannonId);
	strncpy(Info.szCannon, msg.cannonName.c_str(), BOAT_MAXSIZE_NAME - 1);
	Info.byEquipment = static_cast<BYTE>(msg.equipmentId);
	strncpy(Info.szEquipment, msg.equipmentName.c_str(), BOAT_MAXSIZE_NAME - 1);
	Info.dwMoney = static_cast<DWORD>(msg.money);
	Info.dwMinAttack = static_cast<DWORD>(msg.minAttack);
	Info.dwMaxAttack = static_cast<DWORD>(msg.maxAttack);
	Info.dwCurEndure = static_cast<DWORD>(msg.curEndure);
	Info.dwMaxEndure = static_cast<DWORD>(msg.maxEndure);
	Info.dwSpeed = static_cast<DWORD>(msg.speed);
	Info.dwDistance = static_cast<DWORD>(msg.distance);
	Info.dwDefence = static_cast<DWORD>(msg.defence);
	Info.dwCurSupply = static_cast<DWORD>(msg.curSupply);
	Info.dwMaxSupply = static_cast<DWORD>(msg.maxSupply);
	Info.dwConsume = static_cast<DWORD>(msg.consume);
	Info.dwAttackTime = static_cast<DWORD>(msg.attackTime);
	Info.sCapacity = static_cast<decltype(Info.sCapacity)>(msg.boatCapacity);
}

BOOL SC_CreateBoat(LPRPACKET packet) {
	//   McBoatSyncAttrMessage
	net::msg::McBoatSyncAttrMessage msg;
	net::msg::deserialize(packet, msg);
	xShipBuildInfo Info;
	convertBoatMsg(msg, Info);
	NetCreateBoat(Info);
	return TRUE;
}

BOOL SC_UpdateBoat(LPRPACKET packet) {
	//   McBoatSyncAttrMessage
	net::msg::McBoatSyncAttrMessage msg;
	net::msg::deserialize(packet, msg);
	xShipBuildInfo Info;
	convertBoatMsg(msg, Info);
	NetUpdateBoat(Info);
	return TRUE;
}

BOOL SC_BoatInfo(LPRPACKET packet) {
	//   McBoatSyncAttrMessage
	net::msg::McBoatSyncAttrMessage msg;
	net::msg::deserialize(packet, msg);
	xShipBuildInfo Info;
	convertBoatMsg(msg, Info);
	char szBoat[BOAT_MAXSIZE_NAME] = {0};
	strncpy(szBoat, msg.boatName.c_str(), BOAT_MAXSIZE_NAME - 1);
	NetBoatInfo(static_cast<DWORD>(msg.boatId), szBoat, Info);
	return TRUE;
}

BOOL SC_UpdateBoatPart(LPRPACKET packet) {
	return TRUE;
}

BOOL SC_BoatList(LPRPACKET packet) {
	//   McBerthListMessage
	net::msg::McBerthListMessage msg;
	net::msg::deserialize(packet, msg);

	BOAT_BERTH_DATA Data;
	memset(&Data, 0, sizeof(BOAT_BERTH_DATA));
	BYTE byNumBoat = static_cast<BYTE>(msg.count);
	for (BYTE i = 0; i < byNumBoat && i < static_cast<BYTE>(msg.names.size()); i++) {
		strncpy(Data.szName[i], msg.names[i].c_str(), BOAT_MAXSIZE_BOATNAME + BOAT_MAXSIZE_DESP - 1);
	}

	NetShowBoatList(static_cast<DWORD>(msg.npcId), byNumBoat, Data, static_cast<BYTE>(msg.type));
	return TRUE;
}

//BOOL	SC_BoatBagList( LPRPACKET packet )
//{
//	BYTE byNumBoat = packet.ReadInt64();
//	BOAT_BERTH_DATA Data;
//	memset( &Data, 0, sizeof(BOAT_BERTH_DATA) );
//	for( BYTE i = 0; i < byNumBoat; i++ )
//	{
//		strncpy( Data.szName[i], packet.ReadString().c_str(), BOAT_MAXSIZE_BOATNAME - 1 );
//	}
//
//	NetShowBoatBagList( byNumBoat, Data );
//	return TRUE;
//}

BOOL SC_StallInfo(LPRPACKET packet) {
	//   McStallSyncDataMessage
	net::msg::McStallSyncDataMessage msg;
	net::msg::deserialize(packet, msg);

	if (msg.name.empty()) return FALSE;
	NetStallInfo(static_cast<DWORD>(msg.stallerId), static_cast<BYTE>(msg.num), msg.name.c_str());

	for (const auto& g : msg.goods) {
		BYTE byGrid = static_cast<BYTE>(g.grid);
		USHORT sItemID = static_cast<USHORT>(g.itemId);
		BYTE byCount = static_cast<BYTE>(g.count);
		DWORD dwMoney = static_cast<DWORD>(g.money);

		if (g.isBoat) {
			NET_CHARTRADE_BOATDATA Data;
			memset(&Data, 0, sizeof(NET_CHARTRADE_BOATDATA));
			if (g.hasBoat) {
				strncpy(Data.szName, g.boat.name.c_str(), BOAT_MAXSIZE_BOATNAME - 1);
				Data.sBoatID = static_cast<decltype(Data.sBoatID)>(g.boat.ship);
				Data.sLevel = static_cast<decltype(Data.sLevel)>(g.boat.lv);
				Data.dwExp = static_cast<DWORD>(g.boat.cexp);
				Data.dwHp = static_cast<DWORD>(g.boat.hp);
				Data.dwMaxHp = static_cast<DWORD>(g.boat.mxhp);
				Data.dwSp = static_cast<DWORD>(g.boat.sp);
				Data.dwMaxSp = static_cast<DWORD>(g.boat.mxsp);
				Data.dwMinAttack = static_cast<DWORD>(g.boat.mnatk);
				Data.dwMaxAttack = static_cast<DWORD>(g.boat.mxatk);
				Data.dwDef = static_cast<DWORD>(g.boat.def);
				Data.dwSpeed = static_cast<DWORD>(g.boat.mspd);
				Data.dwShootSpeed = static_cast<DWORD>(g.boat.aspd);
				Data.byHasItem = static_cast<BYTE>(g.boat.useGridNum);
				Data.byCapacity = static_cast<BYTE>(g.boat.capacity);
				Data.dwPrice = static_cast<DWORD>(g.boat.price);
				NetStallAddBoat(byGrid, sItemID, byCount, dwMoney, Data);
			}
		}
		else {
			NET_CHARTRADE_ITEMDATA Data;
			memset(&Data, 0, sizeof(NET_CHARTRADE_ITEMDATA));
			Data.sEndure[0] = static_cast<short>(g.item.endure0);
			Data.sEndure[1] = static_cast<short>(g.item.endure1);
			Data.sEnergy[0] = static_cast<short>(g.item.energy0);
			Data.sEnergy[1] = static_cast<short>(g.item.energy1);
			Data.byForgeLv = static_cast<BYTE>(g.item.forgeLv);
			Data.bValid = g.item.valid != 0;
			Data.bItemTradable = static_cast<int>(g.item.tradable);
			Data.expiration = static_cast<long>(g.item.expiration);
			Data.lDBParam[enumITEMDBP_FORGE] = static_cast<long>(g.item.forgeParam);
			Data.lDBParam[enumITEMDBP_INST_ID] = static_cast<long>(g.item.instId);
			Data.byHasAttr = g.item.hasInstAttr ? 1 : 0;
			if (g.item.hasInstAttr) {
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; ++j) {
					Data.sInstAttr[j][0] = static_cast<short>(g.item.instAttr[j][0]);
					Data.sInstAttr[j][1] = static_cast<short>(g.item.instAttr[j][1]);
				}
			}
			NetStallAddItem(byGrid, sItemID, byCount, dwMoney, Data);
		}
	}
	return TRUE;
}

BOOL SC_StallUpdateInfo(LPRPACKET packet) {
	return TRUE;
}

BOOL SC_StallDelGoods(LPRPACKET packet) {
	net::msg::McStallDelGoodsMessage msg;
	net::msg::deserialize(packet, msg);
	NetStallDelGoods(static_cast<DWORD>(msg.charId), static_cast<BYTE>(msg.grid), static_cast<BYTE>(msg.count));
	return TRUE;
}

BOOL SC_StallClose(LPRPACKET packet) {
	net::msg::McStallCloseMessage msg;
	net::msg::deserialize(packet, msg);
	NetStallClose(static_cast<DWORD>(msg.charId));
	return TRUE;
}

BOOL SC_StallSuccess(LPRPACKET packet) {
	net::msg::McStallSuccessMessage msg;
	net::msg::deserialize(packet, msg);
	NetStallSuccess(static_cast<DWORD>(msg.charId));
	return TRUE;
}

BOOL SC_SynStallName(LPRPACKET packet) {
	//    
	net::msg::McSynStallNameMessage msg;
	net::msg::deserialize(packet, msg);
	NetStallName(static_cast<DWORD>(msg.charId), msg.name.c_str());
	return TRUE;
}

BOOL SC_StartExit(LPRPACKET packet) {
	net::msg::McStartExitMessage msg;
	net::msg::deserialize(packet, msg);
	NetStartExit(static_cast<DWORD>(msg.exitTime));
	return TRUE;
}

BOOL SC_CancelExit(LPRPACKET packet) {
	NetCancelExit();
	return TRUE;
}

BOOL SC_UpdateHairRes(LPRPACKET packet) {
	net::msg::McUpdateHairResMessage msg;
	net::msg::deserialize(packet, msg);
	stNetUpdateHairRes rv;
	rv.ulWorldID = static_cast<decltype(rv.ulWorldID)>(msg.worldId);
	rv.nScriptID = static_cast<decltype(rv.nScriptID)>(msg.scriptId);
	rv.szReason = msg.reason;
	rv.Exec();
	return TRUE;
}

BOOL SC_OpenHairCut(LPRPACKET packet) {
	stNetOpenHair hair;
	hair.Exec();
	return TRUE;
}

BOOL SC_TeamFightAsk(LPRPACKET packet) {
	//   McTeamFightAskMessage (count-first)
	net::msg::McTeamFightAskMessage msg;
	net::msg::deserialize(packet, msg);

	stNetTeamFightAsk SFightAsk;
	SFightAsk.chSideNum1 = static_cast<char>(msg.srcCount);
	SFightAsk.chSideNum2 = static_cast<char>(msg.tarCount);
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 SafeVFormat(GetLanguageString(306), static_cast<int>(SFightAsk.chSideNum1),
										 static_cast<int>(SFightAsk.chSideNum2)));
	for (size_t i = 0; i < msg.players.size(); i++) {
		SFightAsk.Info[i].szName = msg.players[i].name;
		SFightAsk.Info[i].chLv = static_cast<char>(msg.players[i].lv);
		SFightAsk.Info[i].szJob = msg.players[i].job;
		SFightAsk.Info[i].usFightNum = static_cast<unsigned short>(msg.players[i].fightNum);
		SFightAsk.Info[i].usVictoryNum = static_cast<unsigned short>(msg.players[i].victoryNum);
		g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(307),
																		SFightAsk.Info[i].szName.c_str(),
																		static_cast<int>(SFightAsk.Info[i].chLv),
																		SFightAsk.Info[i].szJob.c_str()));
	}
	SFightAsk.Exec();
	return TRUE;
}

BOOL SC_BeginItemRepair(LPRPACKET packet) {
	NetBeginRepairItem();
	return TRUE;
}

BOOL SC_ItemRepairAsk(LPRPACKET packet) {
	//   McItemRepairAskMcMessage
	net::msg::McItemRepairAskMcMessage msg;
	net::msg::deserialize(packet, msg);
	stNetItemRepairAsk SRepairAsk;
	SRepairAsk.cszItemName = msg.itemName;
	SRepairAsk.lRepairMoney = static_cast<long>(msg.repairCost);
	SRepairAsk.Exec();

	return TRUE;
}

BOOL SC_ItemForgeAsk(LPRPACKET packet) {
	net::msg::McItemForgeAskMessage msg;
	net::msg::deserialize(packet, msg);
	stSCNetItemForgeAsk SForgeAsk;
	SForgeAsk.chType = static_cast<decltype(SForgeAsk.chType)>(msg.type);
	SForgeAsk.lMoney = static_cast<decltype(SForgeAsk.lMoney)>(msg.money);
	SForgeAsk.Exec();

	return TRUE;
}

BOOL SC_ItemForgeAnswer(LPRPACKET packet) {
	net::msg::McItemForgeAnswerMessage msg;
	net::msg::deserialize(packet, msg);
	stNetItemForgeAnswer SForgeAnswer;
	SForgeAnswer.lChaID = static_cast<decltype(SForgeAnswer.lChaID)>(msg.worldId);
	SForgeAnswer.chType = static_cast<decltype(SForgeAnswer.chType)>(msg.type);
	SForgeAnswer.chResult = static_cast<decltype(SForgeAnswer.chResult)>(msg.result);
	SForgeAnswer.Exec();

	return TRUE;
}

BOOL SC_ItemUseSuc(LPRPACKET packet) {
	net::msg::McItemUseSuccMessage msg;
	net::msg::deserialize(packet, msg);
	NetItemUseSuccess(static_cast<unsigned int>(msg.worldId), static_cast<short>(msg.itemId));

	return TRUE;
}

BOOL SC_KitbagCapacity(LPRPACKET packet) {
	net::msg::McKitbagCapacityMessage msg;
	net::msg::deserialize(packet, msg);
	NetKitbagCapacity(static_cast<unsigned int>(msg.worldId), static_cast<short>(msg.capacity));

	return TRUE;
}

BOOL SC_EspeItem(LPRPACKET packet) {
	net::msg::McEspeItemMessage msg;
	net::msg::deserialize(packet, msg);

	stNetEspeItem SEspItem;
	unsigned int nChaID = static_cast<unsigned int>(msg.worldId);
	SEspItem.chNum = static_cast<decltype(SEspItem.chNum)>(msg.items.size());
	for (int i = 0; i < 1 && i < static_cast<int>(msg.items.size()); i++) {
		SEspItem.SContent[i].sPos = static_cast<decltype(SEspItem.SContent[0].sPos)>(msg.items[i].position);
		SEspItem.SContent[i].sEndure = static_cast<decltype(SEspItem.SContent[0].sEndure)>(msg.items[i].endure);
		SEspItem.SContent[i].sEnergy = static_cast<decltype(SEspItem.SContent[0].sEnergy)>(msg.items[i].energy);
		SEspItem.SContent[i].bItemTradable = static_cast<decltype(SEspItem.SContent[0].bItemTradable)>(msg.items[i].
			tradable);
		SEspItem.SContent[i].expiration = static_cast<decltype(SEspItem.SContent[0].expiration)>(msg.items[i].
			expiration);
	}

	NetEspeItem(nChaID, SEspItem);
	return TRUE;
}

BOOL SC_MapCrash(LPRPACKET packet) {
	net::msg::McMapCrashMessage msg;
	net::msg::deserialize(packet, msg);
	if (msg.text.empty())
		return FALSE;

	NetShowMapCrash(msg.text.c_str());
	return TRUE;
}

BOOL SC_Message(LPRPACKET pk) {
	net::msg::McMessageMessage msg;
	net::msg::deserialize(pk, msg);
	if (msg.text.empty())
		return FALSE;

	NetShowMapCrash(msg.text.c_str());
	return TRUE;
}

BOOL SC_QueryCha(LPRPACKET pk) {
	net::msg::McQueryChaMessage msg;
	net::msg::deserialize(pk, msg);

	stNetSysInfo SShowInfo;
	SShowInfo.m_sysinfo = SafeVFormat(GetLanguageString(308), msg.name.c_str(), static_cast<long>(msg.chaId2),
									  msg.mapName.c_str(), static_cast<long>(msg.posX), static_cast<long>(msg.posY));
	NetSysInfo(SShowInfo);

	return TRUE;
}

BOOL SC_QueryChaItem(LPRPACKET pk) {
	net::msg::McQueryChaItemMessage msg;
	net::msg::deserialize(pk, msg);

	return TRUE;
}

BOOL SC_QueryChaPing(LPRPACKET pk) {
	net::msg::McQueryChaPingMessage msg;
	net::msg::deserialize(pk, msg);

	stNetSysInfo SShowInfo;
	SShowInfo.m_sysinfo = SafeVFormat(GetLanguageString(309), msg.mapName.c_str(), static_cast<long>(msg.ping));
	NetSysInfo(SShowInfo);

	return TRUE;
}

BOOL SC_QueryRelive(LPRPACKET pk) {
	net::msg::McQueryReliveMessage msg;
	net::msg::deserialize(pk, msg);

	stNetQueryRelive SQueryRelive;
	SQueryRelive.szSrcChaName = msg.sourceName;
	SQueryRelive.chType = static_cast<decltype(SQueryRelive.chType)>(msg.reliveType);
	NetQueryRelive(static_cast<uLong>(msg.chaId), SQueryRelive);

	return TRUE;
}

BOOL SC_PreMoveTime(LPRPACKET pk) {
	net::msg::McPreMoveTimeMessage msg;
	net::msg::deserialize(pk, msg);
	NetPreMoveTime(static_cast<uLong>(msg.time));

	return TRUE;
}

BOOL SC_MapMask(LPRPACKET pk) {
	//   McMapMaskMessage
	net::msg::McMapMaskMessage msg;
	net::msg::deserialize(pk, msg);

	uLong l_id = static_cast<uLong>(msg.worldId);
	BYTE* pMapMask = msg.hasData ? reinterpret_cast<BYTE*>(msg.data.data()) : nullptr;
	uShort usLen = static_cast<uShort>(msg.data.size());

	NetMapMask(l_id, pMapMask, usLen);

	return TRUE;
}

BOOL SC_SynEventInfo(LPRPACKET pk) {
	net::msg::McSynEventInfoMessage msg;
	net::msg::deserialize(pk, msg);

	stNetEvent SNetEvent;
	SNetEvent.lEntityID = static_cast<decltype(SNetEvent.lEntityID)>(msg.entityId);
	SNetEvent.chEntityType = static_cast<decltype(SNetEvent.chEntityType)>(msg.entityType);
	SNetEvent.usEventID = static_cast<decltype(SNetEvent.usEventID)>(msg.eventId);
	SNetEvent.cszEventName = msg.eventName;

	SNetEvent.ChangeEvent();
	return TRUE;
}

BOOL SC_SynSideInfo(LPRPACKET pk) {
	net::msg::McSynSideInfoMessage msg;
	net::msg::deserialize(pk, msg);
	uLong l_id = static_cast<uLong>(msg.worldId);

	stNetChaSideInfo SNetSideInfo;
	SNetSideInfo.chSideID = static_cast<decltype(SNetSideInfo.chSideID)>(msg.side.sideId);

	// log
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format("===Recieve(SideInfo)\tTick:[{}]", GetTickCount()));
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("\tSideID: {}", static_cast<int>(SNetSideInfo.chSideID)));

	NetChaSideInfo(l_id, SNetSideInfo);

	return TRUE;
}

BOOL SC_SynAppendLook(LPRPACKET pk) {
	//   McAppendLookMessage
	net::msg::McAppendLookMessage msg;
	net::msg::deserialize(pk, msg);

	uLong l_id = static_cast<uLong>(msg.worldId);
	stNetAppendLook SNetAppendLook;
	for (int i = 0; i < defESPE_KBGRID_NUM; i++) {
		SNetAppendLook.sLookID[i] = static_cast<short>(msg.slots[i].lookId);
		if (SNetAppendLook.sLookID[i] != 0)
			SNetAppendLook.bValid[i] = msg.slots[i].valid != 0;
	}
	SNetAppendLook.Exec(l_id);

	return TRUE;
}

BOOL SC_KitbagCheckAnswer(LPRPACKET packet) {
	net::msg::McKitbagCheckAnswerMessage msg;
	net::msg::deserialize(packet, msg);
	bool bLock = msg.locked ? true : false;
	NetKitbagCheckAnswer(bLock);

	return TRUE;
}

BOOL SC_StoreOpenAnswer(LPRPACKET packet) {
	//   McStoreOpenAnswerMessage
	net::msg::McStoreOpenAnswerMessage msg;
	net::msg::deserialize(packet, msg);

	if (msg.isValid) {
		g_stUIStore.ClearStoreTreeNode();
		g_stUIStore.ClearStoreItemList();

		g_stUIStore.SetStoreMoney(static_cast<long>(msg.moBean), static_cast<long>(msg.replMoney));
		g_stUIStore.SetStoreVip(static_cast<long>(msg.vip));

		long lFirstClass = 0;
		for (size_t i = 0; i < msg.classes.size(); i++) {
			short lClsID = static_cast<short>(msg.classes[i].classId);
			short lParentID = static_cast<short>(msg.classes[i].parentId);
			g_stUIStore.AddStoreTreeNode(lParentID, lClsID, msg.classes[i].className.c_str());
			if (i == 0) lFirstClass = lClsID;
		}

		g_stUIStore.AddStoreUserTreeNode();
		g_stUIStore.ShowStoreForm(true);

		if (lFirstClass > 0) {
			::CS_StoreListAsk(lFirstClass, 1, (short)CStoreMgr::GetPageSize());
		}
	}
	else {
		g_stUIStore.ShowStoreWebPage();
	}

	g_stUIStore.DarkScene(false);
	g_stUIStore.ShowStoreLoad(false);

	return TRUE;
}

BOOL SC_StoreListAnswer(LPRPACKET packet) {
	//   McStoreListAnswerMessage
	net::msg::McStoreListAnswerMessage msg;
	net::msg::deserialize(packet, msg);

	g_stUIStore.ClearStoreItemList();

	for (long i = 0; i < static_cast<long>(msg.products.size()); i++) {
		const auto& p = msg.products[i];
		g_stUIStore.AddStoreItemInfo(i, static_cast<long>(p.comId), p.comName.c_str(),
									 static_cast<long>(p.price), p.remark.c_str(), p.isHot,
									 static_cast<long>(p.time), static_cast<long>(p.quantity),
									 static_cast<long>(p.expire));

		for (int j = 0; j < static_cast<int>(p.variants.size()); j++) {
			const auto& v = p.variants[j];
			short pItemAttrID[5];
			short pItemAttrVal[5];
			for (int k = 0; k < 5; k++) {
				pItemAttrID[k] = static_cast<short>(v.attrs[k].attrId);
				pItemAttrVal[k] = static_cast<short>(v.attrs[k].attrVal);
			}
			g_stUIStore.AddStoreItemDetail(i, j, static_cast<short>(v.itemId),
										   static_cast<short>(v.itemNum), static_cast<short>(v.flute), pItemAttrID,
										   pItemAttrVal);
		}
	}

	g_stUIStore.SetStoreItemPage(static_cast<short>(msg.pageCurrent), static_cast<short>(msg.pageTotal));

	return TRUE;
}

BOOL SC_StoreBuyAnswer(LPRPACKET packet) {
	//   McStoreBuyAnswerMessage
	net::msg::McStoreBuyAnswerMessage msg;
	net::msg::deserialize(packet, msg);

	if (msg.success) {
		long lRplMoney = static_cast<long>(msg.newMoney);
		g_stUIEquip.UpdateIMP(lRplMoney);
		g_stUIStore.SetStoreMoney(-1, lRplMoney);
	}
	else {
		g_pGameApp->MsgBox(GetLanguageString(907));
	}

	g_stUIStore.SetStoreBuyButtonEnable(true);
	return TRUE;
}

BOOL SC_StoreChangeAnswer(LPRPACKET packet) {
	//   McStoreChangeAnswerMessage
	net::msg::McStoreChangeAnswerMessage msg;
	net::msg::deserialize(packet, msg);

	if (msg.success) {
		g_stUIStore.SetStoreMoney(static_cast<long>(msg.moBean), static_cast<long>(msg.replMoney));
	}
	else {
		g_pGameApp->MsgBox(GetLanguageString(908));
	}
	return TRUE;
}

BOOL SC_StoreHistory(LPRPACKET packet) {
	//   McStoreHistoryMessage
	net::msg::McStoreHistoryMessage msg;
	net::msg::deserialize(packet, msg);
	return TRUE;
}

BOOL SC_ActInfo(LPRPACKET packet) {
	//   McActInfoMessage
	net::msg::McActInfoMessage msg;
	net::msg::deserialize(packet, msg);
	return TRUE;
}

BOOL SC_StoreVIP(LPRPACKET packet) {
	//   McStoreVipMessage
	net::msg::McStoreVipMessage msg;
	net::msg::deserialize(packet, msg);

	if (msg.success) {
		g_stUIStore.SetStoreVip(static_cast<short>(msg.vipId));
		g_stUIStore.SetStoreMoney(static_cast<long>(msg.modou), static_cast<long>(msg.shuijing));
	}
	return TRUE;
}

BOOL SC_BlackMarketExchangeData(LPRPACKET packet) {
	net::msg::McBlackMarketExchangeDataMessage msg;
	net::msg::deserialize(packet, msg);

	DWORD dwNpcID = static_cast<DWORD>(msg.npcId);
	g_stUIBlackTrade.SetNpcID(dwNpcID);

	stBlackTrade SBlackTrade;
	for (short sIndex = 0; sIndex < static_cast<short>(msg.exchanges.size()); ++sIndex) {
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex = sIndex;
		SBlackTrade.sSrcID = static_cast<short>(msg.exchanges[sIndex].srcId);
		SBlackTrade.sSrcNum = static_cast<short>(msg.exchanges[sIndex].srcCount);
		SBlackTrade.sTarID = static_cast<short>(msg.exchanges[sIndex].tarId);
		SBlackTrade.sTarNum = static_cast<short>(msg.exchanges[sIndex].tarCount);
		SBlackTrade.sTimeNum = static_cast<short>(msg.exchanges[sIndex].timeValue);

		g_stUIBlackTrade.SetItem(&SBlackTrade);
	}

	g_stUIBlackTrade.ShowBlackTradeForm(true);

	return TRUE;
}

BOOL SC_ExchangeData(LPRPACKET packet) {
	net::msg::McExchangeDataMessage msg;
	net::msg::deserialize(packet, msg);

	DWORD dwNpcID = static_cast<DWORD>(msg.npcId);
	g_stUIBlackTrade.SetNpcID(dwNpcID);

	stBlackTrade SBlackTrade;
	for (short sIndex = 0; sIndex < static_cast<short>(msg.exchanges.size()); ++sIndex) {
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex = sIndex;
		SBlackTrade.sSrcID = static_cast<short>(msg.exchanges[sIndex].srcId);
		SBlackTrade.sSrcNum = static_cast<short>(msg.exchanges[sIndex].srcCount);
		SBlackTrade.sTarID = static_cast<short>(msg.exchanges[sIndex].tarId);
		SBlackTrade.sTarNum = static_cast<short>(msg.exchanges[sIndex].tarCount);
		SBlackTrade.sTimeNum = 0;

		g_stUIBlackTrade.SetItem(&SBlackTrade);
	}

	g_stUIBlackTrade.ShowBlackTradeForm(true);

	return TRUE;
}

BOOL SC_BlackMarketExchangeUpdate(LPRPACKET packet) {
	net::msg::McBlackMarketExchangeUpdateMessage msg;
	net::msg::deserialize(packet, msg);

	DWORD dwNpcID = static_cast<DWORD>(msg.npcId);
	stBlackTrade SBlackTrade;

	g_stUIBlackTrade.ClearItemData();

	for (short sIndex = 0; sIndex < static_cast<short>(msg.exchanges.size()); ++sIndex) {
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sIndex = sIndex;
		SBlackTrade.sSrcID = static_cast<short>(msg.exchanges[sIndex].srcId);
		SBlackTrade.sSrcNum = static_cast<short>(msg.exchanges[sIndex].srcCount);
		SBlackTrade.sTarID = static_cast<short>(msg.exchanges[sIndex].tarId);
		SBlackTrade.sTarNum = static_cast<short>(msg.exchanges[sIndex].tarCount);
		SBlackTrade.sTimeNum = static_cast<short>(msg.exchanges[sIndex].timeValue);

		if (g_stUIBlackTrade.GetIsShow() && g_stUIBlackTrade.GetNpcID() == dwNpcID) {
			g_stUIBlackTrade.SetItem(&SBlackTrade);
		}
	}

	if (g_stUIBlackTrade.GetIsShow() && g_stUIBlackTrade.GetNpcID() == dwNpcID) {
		g_stUIBlackTrade.ShowBlackTradeForm(true);
	}

	return TRUE;
}

BOOL SC_BlackMarketExchangeAsr(LPRPACKET packet) {
	net::msg::McBlackMarketExchangeAsrMessage msg;
	net::msg::deserialize(packet, msg);

	bool bSucc = (msg.success == 1) ? true : false;
	if (bSucc) {
		stBlackTrade SBlackTrade;
		memset(&SBlackTrade, 0, sizeof(stBlackTrade));

		SBlackTrade.sSrcID = static_cast<short>(msg.srcId);
		SBlackTrade.sSrcNum = static_cast<short>(msg.srcCount);
		SBlackTrade.sTarID = static_cast<short>(msg.tarId);
		SBlackTrade.sTarNum = static_cast<short>(msg.tarCount);

		g_stUIBlackTrade.ExchangeAnswerProc(bSucc, &SBlackTrade);
	}

	return TRUE;
}

BOOL SC_TigerItemID(LPRPACKET packet) {
	net::msg::McTigerItemIdMessage msg;
	net::msg::deserialize(packet, msg);

	short sNum = static_cast<short>(msg.num);
	g_stUISpirit.UpdateErnieNumber(sNum, static_cast<short>(msg.itemId0), static_cast<short>(msg.itemId1),
								   static_cast<short>(msg.itemId2));

	if (sNum == 3) {
		g_stUISpirit.ShowErnieHighLight();
	}

	return TRUE;
}

BOOL SC_VolunteerList(LPRPACKET packet) {
	net::msg::McVolunteerListMessage msg;
	net::msg::deserialize(packet, msg);

	short sPageNum = static_cast<short>(msg.pageTotal);
	short sPage = static_cast<short>(msg.page);

	g_stUIFindTeam.SetFindTeamPage(sPage, sPageNum);
	g_stUIFindTeam.RemoveTeamInfo();

	for (int i = 0; i < static_cast<int>(msg.volunteers.size()); i++) {
		g_stUIFindTeam.AddFindTeamInfo(i, msg.volunteers[i].name.c_str(),
									   static_cast<long>(msg.volunteers[i].level),
									   static_cast<long>(msg.volunteers[i].job),
									   msg.volunteers[i].map.c_str());
	}

	return TRUE;
}

BOOL SC_VolunteerState(LPRPACKET packet) {
	net::msg::McVolunteerStateMessage msg;
	net::msg::deserialize(packet, msg);
	bool bState = (msg.state == 0) ? false : true;
	g_stUIFindTeam.SetOwnFindTeamState(bState);

	return TRUE;
}

BOOL SC_VolunteerOpen(LPRPACKET packet) {
	net::msg::McVolunteerOpenMessage msg;
	net::msg::deserialize(packet, msg);

	bool bState = (msg.state == 0) ? false : true;
	short sPageNum = static_cast<short>(msg.pageTotal);

	g_stUIFindTeam.SetOwnFindTeamState(bState);
	g_stUIFindTeam.SetFindTeamPage(1, sPageNum <= 0 ? 1 : sPageNum);
	g_stUIFindTeam.RemoveTeamInfo();

	for (int i = 0; i < static_cast<int>(msg.volunteers.size()); i++) {
		g_stUIFindTeam.AddFindTeamInfo(i, msg.volunteers[i].name.c_str(),
									   static_cast<long>(msg.volunteers[i].level),
									   static_cast<long>(msg.volunteers[i].job),
									   msg.volunteers[i].map.c_str());
	}

	g_stUIFindTeam.ShowFindTeamForm();

	return TRUE;
}

BOOL SC_VolunteerAsk(LPRPACKET packet) {
	net::msg::McVolunteerAskMessage msg;
	net::msg::deserialize(packet, msg);
	g_stUIFindTeam.FindTeamAsk(msg.name.c_str());

	return TRUE;
}

BOOL SC_SyncKitbagTemp(LPRPACKET packet) {
	//   McKitbagTempSyncMessage
	net::msg::McKitbagTempSyncMessage msg;
	net::msg::deserialize(packet, msg);

	stNetKitbag SKitbagTemp;
	convertKitbag(msg.kitbag, SKitbagTemp);
	SKitbagTemp.chBagType = 2;
	NetChangeKitbag(g_ulWorldID, SKitbagTemp);

	return TRUE;
}

BOOL SC_SyncTigerString(LPRPACKET packet) {
	//   McTigerStopMessage
	net::msg::McTigerStopMessage msg;
	net::msg::deserialize(packet, msg);
	g_stUISpirit.UpdateErnieString(msg.text.c_str());

	return TRUE;
}

BOOL SC_MasterAsk(LPRPACKET packet) {
	net::msg::McMasterAskMessage msg;
	net::msg::deserialize(packet, msg);
	g_stUIChat.MasterAsk(msg.name.c_str(), static_cast<DWORD>(msg.chaId));
	return TRUE;
}

BOOL SC_PrenticeAsk(LPRPACKET packet) {
	net::msg::McPrenticeAskMessage msg;
	net::msg::deserialize(packet, msg);
	g_stUIChat.PrenticeAsk(msg.name.c_str(), static_cast<DWORD>(msg.chaId));
	return TRUE;
}

BOOL PC_MasterRefresh(LPRPACKET packet) {
	//   PcMasterRefreshFullMessage
	net::msg::PcMasterRefreshFullMessage msg;
	net::msg::deserialize(packet, msg);

	//  : 0-4 = , 5-9 = 
	bool isMaster = (msg.type < 5);
	int64_t localType = isMaster ? msg.type : (msg.type - 5);

	if (localType == MSG_MASTER_REFRESH_ONLINE) {
		if (isMaster) NetMasterOnline(static_cast<uLong>(msg.chaId));
		else NetPrenticeOnline(static_cast<uLong>(msg.chaId));
	}
	else if (localType == MSG_MASTER_REFRESH_OFFLINE) {
		if (isMaster) NetMasterOffline(static_cast<uLong>(msg.chaId));
		else NetPrenticeOffline(static_cast<uLong>(msg.chaId));
	}
	else if (localType == MSG_MASTER_REFRESH_DEL) {
		if (isMaster) NetMasterDel(static_cast<uLong>(msg.chaId));
		else NetPrenticeDel(static_cast<uLong>(msg.chaId));
	}
	else if (localType == MSG_MASTER_REFRESH_ADD) {
		if (isMaster) NetMasterAdd(static_cast<uLong>(msg.chaId), msg.chaName.c_str(), msg.motto.c_str(),
								   static_cast<uShort>(msg.icon), msg.group.c_str());
		else NetPrenticeAdd(static_cast<uLong>(msg.chaId), msg.chaName.c_str(), msg.motto.c_str(),
							static_cast<uShort>(msg.icon), msg.group.c_str());
	}
	else if (localType == MSG_MASTER_REFRESH_START) {
		stNetFrndStart l_self;
		l_self.lChaid = static_cast<uLong>(msg.startData.selfChaId);
		l_self.szChaname = msg.startData.selfName;
		l_self.szMotto = msg.startData.selfMotto;
		l_self.sIconID = static_cast<uShort>(msg.startData.selfIcon);
		stNetFrndStart l_nfs[100];
		uShort l_nfnum = 0;
		for (const auto& e : msg.startData.entries) {
			if (l_nfnum >= 100) break;
			l_nfs[l_nfnum].szGroup = e.group;
			l_nfs[l_nfnum].lChaid = static_cast<uLong>(e.chaId);
			l_nfs[l_nfnum].szChaname = e.chaName;
			l_nfs[l_nfnum].szMotto = e.motto;
			l_nfs[l_nfnum].sIconID = static_cast<uShort>(e.icon);
			l_nfs[l_nfnum].cStatus = static_cast<char>(e.status);
			l_nfnum++;
		}
		if (isMaster) NetMasterStart(l_self, l_nfs, l_nfnum);
		else NetPrenticeStart(l_self, l_nfs, l_nfnum);
	}
	return TRUE;
}

BOOL PC_MasterCancel(LPRPACKET packet) {
	//   PcMasterCancelMessage
	net::msg::PcMasterCancelMessage msg;
	net::msg::deserialize(packet, msg);
	NetMasterCancel(static_cast<uLong>(msg.cancelId), static_cast<unsigned char>(msg.reason));
	return TRUE;
}

BOOL PC_MasterRefreshInfo(LPRPACKET packet) {
	//   PcMasterRefreshInfoFullMessage
	net::msg::PcMasterRefreshInfoFullMessage msg;
	net::msg::deserialize(packet, msg);
	NetMasterRefreshInfo(static_cast<unsigned long>(msg.chaId), msg.motto.c_str(),
						 static_cast<unsigned short>(msg.icon), static_cast<unsigned short>(msg.degree),
						 msg.job.c_str(), msg.guild.c_str());
	return TRUE;
}

BOOL PC_PrenticeRefreshInfo(LPRPACKET packet) {
	//   PcMasterRefreshInfoFullMessage
	net::msg::PcMasterRefreshInfoFullMessage msg;
	net::msg::deserialize(packet, msg);
	NetPrenticeRefreshInfo(static_cast<unsigned long>(msg.chaId), msg.motto.c_str(),
						   static_cast<unsigned short>(msg.icon), static_cast<unsigned short>(msg.degree),
						   msg.job.c_str(), msg.guild.c_str());
	return TRUE;
}

BOOL SC_ChaPlayEffect(LPRPACKET packet) {
	net::msg::McChaPlayEffectMessage msg;
	net::msg::deserialize(packet, msg);

	NetChaPlayEffect(static_cast<unsigned int>(msg.worldId), static_cast<int>(msg.effectId));

	return TRUE;
}

BOOL SC_Say2Camp(LPRPACKET packet) {
	net::msg::McSay2CampMessage msg;
	net::msg::deserialize(packet, msg);

	NetSideInfo(msg.chaName.c_str(), msg.content.c_str());

	return TRUE;
}

BOOL SC_GMMail(LPRPACKET packet) {
	net::msg::McGmMailMessage msg;
	net::msg::deserialize(packet, msg);

	g_stUIMail.ShowAnswerForm(msg.title.c_str(), msg.content.c_str());

	return TRUE;
}

BOOL SC_CheatCheck(LPRPACKET packet) {
	//   McCheatCheckMessage
	net::msg::McCheatCheckMessage msg;
	net::msg::deserialize(packet, msg);

	for (size_t i = 0; i < msg.pictures.size(); i++) {
		auto& pic = msg.pictures[i];
		short size = static_cast<short>(pic.bytes.size());
		char* picture = new char[size];
		for (int j = 0; j < size; j++) {
			picture[j] = static_cast<char>(pic.bytes[j]);
		}

		g_stUINumAnswer.SetBmp(static_cast<int>(i), (BYTE*)picture, size);
		delete [] picture;
	}

	g_stUINumAnswer.Refresh();
	g_stUINumAnswer.ShowForm();

	return TRUE;
}

BOOL SC_ListAuction(LPRPACKET pk) {
	//   McListAuctionMessage (count-first)
	net::msg::McListAuctionMessage msg;
	net::msg::deserialize(pk, msg);
	stChurchChallenge stInfo;

	for (const auto& e : msg.items) {
		stInfo.sChurchID = static_cast<short>(e.itemId);
		stInfo.sCount = static_cast<short>(e.itemCount);
		stInfo.nBasePrice = static_cast<long>(e.basePrice);
		stInfo.nMinbid = static_cast<long>(e.minBid);
		stInfo.nCurPrice = static_cast<long>(e.curPrice);
		strncpy(stInfo.szChaName, e.curChaName.c_str(), sizeof(stInfo.szChaName) - 1);
		strncpy(stInfo.szName, e.name.c_str(), sizeof(stInfo.szName) - 1);
	}

	NetChurchChallenge(&stInfo);

	return TRUE;
}

BOOL SC_RequestDropRate(LPRPACKET pk) {
	net::msg::McRequestDropRateMessage msg;
	net::msg::deserialize(pk, msg);
	g_DropBonus = msg.rate;
	if (!g_stUIStart.frmMonsterInfo->GetIsShow()) {
		g_stUIStart.frmMonsterInfo->Show();
	}
	g_stUIStart.SetMonsterInfo();
	g_pGameApp->Waiting(false);
	return true;
}

BOOL SC_RequestExpRate(LPRPACKET pk) {
	net::msg::McRequestExpRateMessage msg;
	net::msg::deserialize(pk, msg);
	g_ExpBonus = msg.rate;
	return true;
}

BOOL SC_RefreshSelectScreen(LPRPACKET pk) {
	//   McRefreshSelectScreenMessage
	net::msg::McRefreshSelectScreenMessage msg;
	net::msg::deserialize(pk, msg);

	if (g_pGameApp->GetCurScene()->GetSceneTypeID() != enumSelectChaScene) {
		return true;
	}

	if (msg.chaDelSlot != -1) {
		CSelectChaScene& rkScene = CSelectChaScene::GetCurrScene();
		rkScene.m_nCurChaIndex = static_cast<char>(msg.chaDelSlot);
		rkScene.DelCurrentSelCha();
	}

	//  ChaSlotData  NetChaBehave
	std::vector<NetChaBehave> characters;
	for (const auto& cha : msg.characters) {
		if (cha.valid) {
			NetChaBehave nb;
			nb.sCharName = cha.chaName;
			nb.sJob = cha.job;
			nb.iDegree = static_cast<short>(cha.degree);
			nb.look_minimal.typeID = static_cast<uint16_t>(cha.typeId);
			for (size_t ei = 0; ei < cha.equipIds.size() && ei < nb.look_minimal.equip_IDs.size(); ++ei)
				nb.look_minimal.equip_IDs[ei] = static_cast<uint16_t>(cha.equipIds[ei]);
			characters.emplace_back(std::move(nb));
		}
	}

	CSelectChaScene::GetCurrScene().SelectCharacters(characters);

	return true;
}


// ReadChaBasePacket/SkillBag/SkillState/Attr/Look/LookEnergy/PK/Side/AppendLook/EntEvent
//   pk- ,   net::msg::deserialize + convert*().

//  msg.kitbag (ChaKitbagInfo) -> stNetKitbag (   pk)
void ReadChaKitbagFromMsg(const net::msg::ChaKitbagInfo& info, stNetKitbag& SKitbag) {
	memset(&SKitbag, 0, sizeof(SKitbag));
	SKitbag.chType = static_cast<char>(info.synType);
	if (SKitbag.chType == enumSYN_KITBAG_INIT) {
		SKitbag.nKeybagNum = static_cast<int>(info.capacity);
	}
	g_logManager.InternalLog(LogLevel::Debug, "common", std::format(
								 "===Recieve(Update Kitbag):\tGridNum:{}\tType:{}\tTick:[{}]", SKitbag.nKeybagNum,
								 static_cast<int>(SKitbag.chType),
								 GetTickCount()));
	stNetKitbag::stGrid* Grid = SKitbag.Grid;
	SItemGrid* pItem;
	CItemRecord* pItemRec;
	int nGridNum = 0;
	for (size_t idx = 0; idx < info.items.size(); idx++) {
		auto& it = info.items[idx];
		Grid[nGridNum].sGridID = static_cast<short>(it.gridId);
		pItem = &Grid[nGridNum].SGridContent;
		pItem->sID = static_cast<short>(it.itemId);
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 SafeVFormat(GetLanguageString(313), Grid[nGridNum].sGridID, pItem->sID));
		if (pItem->sID > 0) {
			pItem->dwDBID = static_cast<DWORD>(it.dbId);
			pItem->sNeedLv = static_cast<short>(it.needLv);
			pItem->sNum = static_cast<short>(it.num);
			pItem->sEndure[0] = static_cast<short>(it.endure0);
			pItem->sEndure[1] = static_cast<short>(it.endure1);
			pItem->sEnergy[0] = static_cast<short>(it.energy0);
			pItem->sEnergy[1] = static_cast<short>(it.energy1);
			g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(
										 GetLanguageString(314), pItem->sNum, pItem->sEndure[0], pItem->sEndure[1],
										 pItem->sEnergy[0], pItem->sEnergy[1]));
			pItem->chForgeLv = static_cast<char>(it.forgeLv);
			pItem->SetValid(it.valid != 0);
			pItem->bItemTradable = static_cast<int>(it.tradable);
			pItem->expiration = static_cast<long>(it.expiration);

			pItemRec = GetItemRecordInfo(pItem->sID);
			if (pItemRec == NULL) {
				auto _str315 = SafeVFormat(GetLanguageString(315), pItem->sID);
				MessageBox(0, _str315.c_str(), "Error", 0);
#ifdef USE_DSOUND
				if (g_dwCurMusicID) {
					Corsairs::Client::Audio::AudioSDL::Instance().Stop(g_dwCurMusicID);
					g_dwCurMusicID = 0;
					Sleep(60);
				}
#endif
				exit(0);
			}

			if (it.isBoat) {
				pItem->SetDBParam(enumITEMDBP_INST_ID, static_cast<long>(it.boatWorldId));
			}

			pItem->SetDBParam(enumITEMDBP_FORGE, static_cast<long>(it.forgeParam));
			if (!it.isBoat) {
				pItem->SetDBParam(enumITEMDBP_INST_ID, static_cast<long>(it.instId));
			}

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 SafeVFormat(GetLanguageString(316), pItem->GetDBParam(enumITEMDBP_FORGE)));
			if (it.hasInstAttr) {
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++) {
					pItem->sInstAttr[j][0] = static_cast<short>(it.instAttr[j][0]);
					pItem->sInstAttr[j][1] = static_cast<short>(it.instAttr[j][1]);
					g_logManager.InternalLog(LogLevel::Debug, "common",
											 SafeVFormat(GetLanguageString(317), pItem->sInstAttr[j][0],
														 pItem->sInstAttr[j][1]));
				}
			}
		}
		nGridNum++;
		if (nGridNum > defMAX_KBITEM_NUM_PER_TYPE) {
			g_logManager.InternalLog(LogLevel::Error, "errors",
									 SafeVFormat(GetLanguageString(319), nGridNum, defMAX_KBITEM_NUM_PER_TYPE));
			break;
		}
	}
	SKitbag.nGridNum = nGridNum;
	g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(320), SKitbag.nGridNum));
}

//  msg.shortcut (ChaShortcutInfo) -> stNetShortCut (   pk)
void ReadChaShortcutFromMsg(const net::msg::ChaShortcutInfo& info, stNetShortCut& SShortcut) {
	memset(&SShortcut, 0, sizeof(SShortcut));
	g_logManager.InternalLog(LogLevel::Debug, "common",
							 std::format("===Recieve(Update Shortcut):\tTick:[{}]", GetTickCount()));
	for (int i = 0; i < SHORT_CUT_NUM; i++) {
		SShortcut.chType[i] = static_cast<char>(info.entries[i].type);
		SShortcut.byGridID[i] = static_cast<BYTE>(info.entries[i].gridId);
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 SafeVFormat(GetLanguageString(321), static_cast<int>(SShortcut.chType[i]),
											 static_cast<int>(SShortcut.byGridID[i])));
	}
}


BOOL PC_PKSilver(LPRPACKET packet) {
	//   McPkSilverMessage
	net::msg::McPkSilverMessage msg;
	net::msg::deserialize(packet, msg);

	for (int i = 0; i < MAX_PKSILVER_PLAYER; i++) {
		g_stUIPKSilver.AddFormAttribute(i, msg.players[i].name,
										static_cast<long>(msg.players[i].level), msg.players[i].profession,
										static_cast<long>(msg.players[i].pkVal));
	}

	g_stUIPKSilver.ShowPKSilverForm();
	return TRUE;
}


BOOL SC_LifeSkillShow(LPRPACKET packet) {
	net::msg::McLifeSkillShowMessage msg;
	net::msg::deserialize(packet, msg);
	long lType = static_cast<long>(msg.type);
	switch (lType) {
	case 0: //  
	{
		g_stUICompose.ShowComposeForm();
	}
	break;
	case 1: //  
	{
		g_stUIBreak.ShowBreakForm();
	}
	break;
	case 2: //  
	{
		g_stUIFound.ShowFoundForm();
	}
	break;
	case 3: //  
	{
		g_stUICooking.ShowCookingForm();
	}
	break;
	}
	return TRUE;
}


BOOL SC_LifeSkill(LPRPACKET packet) {
	net::msg::McLifeSkillMessage msg;
	net::msg::deserialize(packet, msg);
	long lType = static_cast<long>(msg.type);
	short ret = static_cast<short>(msg.result);
	std::string txt = msg.text;

	switch (lType) {
	case 0: //  
	{
		g_stUICompose.CheckResult(ret, txt.c_str());
	}
	break;
	case 1: //  
	{
		g_stUIBreak.CheckResult(ret, txt.c_str());
	}
	break;
	case 2: //  
	{
		std::string strVer[3];
		Util_ResolveTextLine(txt.c_str(), strVer, 3, ',');
		g_stUIFound.CheckResult(ret, strVer[0].c_str(), strVer[1].c_str(), strVer[2].c_str());
	}
	break;
	case 3: //  
	{
		g_stUICooking.CheckResult(ret);
	}
	break;
	}

	return TRUE;
}


BOOL SC_LifeSkillAsr(LPRPACKET packet) {
	net::msg::McLifeSkillAsrMessage msg;
	net::msg::deserialize(packet, msg);
	long lType = static_cast<long>(msg.type);
	short tim = static_cast<short>(msg.time);
	std::string txt = msg.text;

	switch (lType) {
	case 0: //  
	{
		g_stUICompose.StartTime(tim);
	}
	break;
	case 1: //  
	{
		g_stUIBreak.StartTime(tim, txt.c_str());
	}
	break;
	case 2: //  
	{
		g_stUIFound.StartTime(tim);
	}
	break;
	case 3: //  
	{
		g_stUICooking.StartTime(tim);
	}
	break;
	}
	return TRUE;
}


BOOL SC_DropLockAsr(LPRPACKET pk) {
	net::msg::McDropLockAsrMessage msg;
	net::msg::deserialize(pk, msg);
	if (msg.success) {
		g_pGameApp->SysInfo("Locking successful!");
	}
	else {
		g_pGameApp->SysInfo("Locking failed!");
	}
	return TRUE;
}


BOOL SC_UnlockItemAsr(LPRPACKET pk) {
	net::msg::McUnlockItemAsrMessage msg;
	net::msg::deserialize(pk, msg);
	g_pGameApp->SysInfo(
		[&] {
			switch (msg.result) {
			case 1:
				return "Item Unlocked";
			case 2:
				return "2nd password incorrect!";
			default:
				return "Unlocking failed";
			}
		}());
	return TRUE;
}

//==================================================================================
