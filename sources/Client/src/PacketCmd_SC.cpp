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
#include "CommandMessages.h"

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

BOOL SC_UpdateGuildGold(LPRPACKET pk) {
	g_stUIGuildBank.UpdateGuildGold(pk.ReadString().c_str());
	return true;
}

BOOL SC_ShowStallSearch(LPRPACKET pk) {
	// Формат count-first: десериализуем через McShowStallSearchMessage
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
	// Формат count-first: десериализуем через McShowRankingMessage
	net::msg::McShowRankingMessage msg;
	net::msg::deserialize(pk, msg);
	NetMC_LISTGUILD_BEGIN();
	for (size_t i = 0; i < msg.entries.size(); ++i) {
		auto& e = msg.entries[i];
		char buf[8];
		sprintf(buf, "%03d>", (int)(i + 1));
		NetMC_LISTGUILD(i + 1, buf, e.name.c_str(), e.guild.c_str(), (uShort)e.level, e.score);
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

BOOL SC_SendPublicKey(LPRPACKET pk) {
	// Читаем SPKI DER публичный ключ из пакета (WriteSequence = [uint16 len][data])
	uShort keySize = 0;
	const char* keyData = pk.ReadSequence(keySize);
	if (keySize == 0) {
		// Сервер сообщил что шифрование выключено — пропускаем RSA/AES
		LG("enc", "SC_SendPublicKey: server encryption disabled (empty key)\n");
		g_NetIF->handshakeDone = true;
		g_NetIF->_comm_enc = 0;
		g_NetIF->m_connect.CHAPSTR(); // CONNECTING → HANDSHAKE
		g_NetIF->m_connect.CHAPSTR(false); // HANDSHAKE → CONNECTED
		return TRUE;
	}
	if (!keyData) {
		LG("enc", "SC_SendPublicKey: null key data\n");
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
		&pubKeyInfo, &pubKeyInfoSize)) {
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
		// Конвертируем ChaSlotData → NetChaBehave
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
	auto reason = pk.ReadInt64();
	g_NetIF->m_connect.Disconnect(reason);
	return true;
}


// =================================================================
//  CMD_MC_ENTERMAP — конвертеры net::msg → stNet* (клиентские)
// =================================================================

// clientIsBoatItem больше не нужен — isBoat читается из потока

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
// CMD_MC_ENTERMAP — вход на карту (S->C)
//--------------------
BOOL SC_EnterMap(LPRPACKET pk) {
	g_LogName.Init();
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
	LG(g_oLangRec.GetString(295), "%s\n", d.mapName.c_str());

	g_stUIEquip.UpdateIMP(static_cast<int>(d.imp));

	stNetActorCreate SCreateInfo;
	convertBaseInfo(d.baseInfo, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 1;
	SCreateInfo.CreateCha();
	g_ulWorldID = SCreateInfo.ulWorldID;

	const char* szLogName = g_LogName.GetLogName(SCreateInfo.ulWorldID);

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

		szLogName = g_LogName.GetLogName(SCreateInfo.ulWorldID);

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

std::optional<NetChaBehave> ReadSelectCharacter(RPacket& rpk) {
	if (!rpk.ReadInt64()) {
		return {};
	}

	uShort looklen{0};
	NetChaBehave cha;
	cha.sCharName = rpk.ReadString();
	cha.sJob = rpk.ReadString();
	cha.iDegree = rpk.ReadInt64();

	cha.look_minimal.typeID = rpk.ReadInt64();
	for (auto& id : cha.look_minimal.equip_IDs) {
		id = rpk.ReadInt64();
	}

	return cha;
}

std::vector<NetChaBehave> ReadSelectCharacters(RPacket& rpk) {
	std::vector<NetChaBehave> characters;
	characters.reserve(rpk.ReadInt64());
	for (auto i = 0; i < characters.capacity(); ++i) {
		if (auto cha = ReadSelectCharacter(rpk)) {
			characters.emplace_back(cha.value());
		}
	}
	return characters;
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
	CGameApp::Waiting(false);
	char sucess = pk.ReadInt64();
	if (g_NetIF->IsConnected()) {
		CS_Disconnect(DS_DISCONN);
	}
	//register cooldown
	_dwLastTime = CGameApp::GetCurTick();

	if (_dwOverTime > _dwLastTime) {
		g_pGameApp->MsgBox("Do Not Spam.");
		return FALSE;
	}
	//test
	if (sucess == 1) {
		//mothannakh account register//
		//CLoginScene* pkScene = dynamic_cast<CLoginScene*>(CGameApp::GetCurScene());
		registerLogin = false;
		//pkScene->LoginFlow();
		//here end
		_dwOverTime = _dwLastTime + 9000;
		g_pGameApp->MsgBox("Account Created.");
	}
	else {
		g_pGameApp->MsgBox(pk.ReadString().c_str());
	}
	return TRUE;
}

BOOL PC_Ping(LPRPACKET pk) {
	WPacket l_wpk = g_NetIF->GetWPacket();
	l_wpk.WriteCmd(CMD_CP_PING);
	g_NetIF->SendPacketMessage(l_wpk);
	return TRUE;
}

BOOL SC_Ping(LPRPACKET pk) {
	net::msg::McPingMessage msg;
	net::msg::deserialize(pk, msg);
	{
		auto const l = std::lock_guard{g_NetIF->m_mutmov};

		WPacket wpk = g_NetIF->GetWPacket();
		wpk.WriteCmd(CMD_CM_PING);
		wpk.WriteInt64(msg.v1);
		wpk.WriteInt64(msg.v2);
		wpk.WriteInt64(msg.v3);
		wpk.WriteInt64(msg.v4);
		wpk.WriteInt64(msg.v5);
		g_NetIF->SendPacketMessage(wpk);
	}

	if (g_NetIF->m_curdelay > g_NetIF->m_maxdelay) g_NetIF->m_maxdelay = g_NetIF->m_curdelay;

	if (g_NetIF->m_curdelay < g_NetIF->m_mindelay) g_NetIF->m_mindelay = g_NetIF->m_curdelay;

	return TRUE;
}

BOOL SC_CheckPing(LPRPACKET pk) {
	WPacket wpk = g_NetIF->GetWPacket();
	wpk.WriteCmd(CMD_CM_CHECK_PING);
	g_NetIF->SendPacketMessage(wpk);

	return TRUE;
}

BOOL SC_Say(LPRPACKET pk) {
	net::msg::McSayMessage msg;
	net::msg::deserialize(pk, msg);
	stNetSay l_netsay;
	l_netsay.m_srcid = msg.sourceId;
	l_netsay.m_content = msg.content.c_str();
	NetSay(l_netsay, msg.color);

	return TRUE;
}

//--------------------
// Э��S->C : ϵͳ��Ϣ��ʾ
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
// Э��S->C : ������ɫ(���������)����
//------------------------------------
BOOL SC_ChaBeginSee(LPRPACKET pk) {
	stNetActorCreate SCreateInfo;
	char chSeeType = pk.ReadInt64();

	ReadChaBasePacket(pk, SCreateInfo);
	SCreateInfo.chSeeType = chSeeType;
	SCreateInfo.chMainCha = 0;
	CCharacter* pCha = SCreateInfo.CreateCha();
	if (!pCha) return FALSE;

	const char* szLogName = g_LogName.GetLogName(SCreateInfo.ulWorldID);

	stNetNPCShow SNpcShow;
	// NPC����״̬��Ϣ
	SNpcShow.byNpcType = pk.ReadInt64();
	SNpcShow.byNpcState = pk.ReadInt64();
	SNpcShow.SetNpcShow(pCha);


	// �ж�����
	switch (pk.ReadInt64()) {
	case enumPoseLean: {
		stNetLeanInfo SLean;
		SLean.chState = pk.ReadInt64();
		SLean.lPose = pk.ReadInt64();
		SLean.lAngle = pk.ReadInt64();
		SLean.lPosX = pk.ReadInt64();
		SLean.lPosY = pk.ReadInt64();
		SLean.lHeight = pk.ReadInt64();
		NetActorLean(SCreateInfo.ulWorldID, SLean);
		break;
	}
	case enumPoseSeat: {
		stNetFace SNetFace;
		SNetFace.sAngle = pk.ReadInt64();
		SNetFace.sPose = pk.ReadInt64();
		NetFace(SCreateInfo.ulWorldID, SNetFace, enumACTION_SKILL_POSE);
		break;
	}
	default: {
		break;
	}
	}

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff);

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	return TRUE;
}

//------------------------------------
// Э��S->C : ������ɫ(���������)��ʧ
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
	LG(g_LogName.GetLogName(l_id), "+++++++++++++Destroy\n");
	//
	return TRUE;
}

BOOL SC_ItemCreate(LPRPACKET pk) {
	// Десериализация через типизированное сообщение
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
	LG("SC_Item", "CreateType = %d, WorldID:%u, ItemID = %d, Pos = [%d,%d], SrcID = %u, \n", SCreateInfo.chAppeType,
	   SCreateInfo.lWorldID, SCreateInfo.lID, SCreateInfo.SPos.x, SCreateInfo.SPos.y, SCreateInfo.lFromID);
	//
	return TRUE;
}

BOOL SC_ItemDestroy(LPRPACKET pk) {
	net::msg::McItemDestroyMessage msg;
	net::msg::deserialize(pk, msg);
	unsigned long lID = static_cast<unsigned long>(msg.worldId);

	NetItemDisappear(lID);
	LG("SC_Item", "Item Destroy[%u]\n", lID);
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
		SynAState.State[chValidNum].lWorldID = static_cast<decltype(SynAState.State[0].lWorldID)>(msg.states[j].worldId);
		SynAState.State[chValidNum].uchFightID = static_cast<decltype(SynAState.State[0].uchFightID)>(msg.states[j].fightId);
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
}

BOOL SC_AStateEndSee(LPRPACKET pk) {
	// Десериализация через типизированное сообщение
	net::msg::McAStateEndSeeMessage msg;
	net::msg::deserialize(pk, msg);

	stNetAreaState SynAState;
	SynAState.sAreaX = static_cast<decltype(SynAState.sAreaX)>(msg.areaX);
	SynAState.sAreaY = static_cast<decltype(SynAState.sAreaY)>(msg.areaY);

	NetAreaStateEndSee(&SynAState);

	// log
	LG(g_LogName.GetMainLogName(), g_oLangRec.GetString(296), SynAState.sAreaX, SynAState.sAreaY, 0);
	//

	return TRUE;
}

// Э��S->C : ���ӵ��߽�ɫ
BOOL SC_AddItemCha(LPRPACKET pk) {
	long lMainChaID = pk.ReadInt64();
	stNetActorCreate SCreateInfo;
	ReadChaBasePacket(pk, SCreateInfo);
	SCreateInfo.chSeeType = enumENTITY_SEEN_NEW;
	SCreateInfo.chMainCha = 2;
	SCreateInfo.CreateCha();

	const char* szLogName = g_LogName.GetLogName(SCreateInfo.ulWorldID);

	stNetChaAttr SChaAttr;
	ReadChaAttrPacket(pk, SChaAttr, szLogName);
	NetSynAttr(SCreateInfo.ulWorldID, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff);

	stNetKitbag SKitbag;
	ReadChaKitbagPacket(pk, SKitbag, szLogName);
	SKitbag.chBagType = 0;
	NetChangeKitbag(SCreateInfo.ulWorldID, SKitbag);

	stNetSkillState SCurSState;
	ReadChaSkillStatePacket(pk, SCurSState, szLogName);
	NetSynSkillState(SCreateInfo.ulWorldID, &SCurSState);

	return TRUE;
}

// Э��S->C : ɾ�����߽�ɫ
BOOL SC_DelItemCha(LPRPACKET pk) {
	net::msg::McDelItemChaMessage msg;
	net::msg::deserialize(pk, msg);

	char chSeeType = enumENTITY_SEEN_NEW;
	uLong l_id = static_cast<uLong>(msg.worldId);
	NetActorDestroy(l_id, chSeeType);

	return TRUE;
}

// �������
BOOL SC_Cha_Emotion(LPRPACKET pk) {
	uLong l_id = pk.ReadInt64();
	uShort sEmotion = pk.ReadInt64();

	NetChaEmotion(l_id, sEmotion);
	LG(g_LogName.GetLogName(l_id), g_oLangRec.GetString(297), sEmotion);
	return TRUE;
}

// Э��S->C : ��ɫ�ж�ͨ��
BOOL SC_CharacterAction(LPRPACKET pk) {
	uLong l_id = pk.ReadInt64();

	const char* szLogName = g_LogName.GetLogName(l_id);

	// try
	{
		long lPacketId = 0;
#ifdef defPROTOCOL_HAVE_PACKETID
		lPacketId = pk.ReadInt64();
#endif
		LG(szLogName, "$$$PacketID:\t%u\n", lPacketId);

		switch (pk.ReadInt64()) {
		case enumACTION_MOVE: {
			stNetNotiMove SMoveInfo;
			SMoveInfo.sState = pk.ReadInt64();
			if (SMoveInfo.sState != enumMSTATE_ON)
				SMoveInfo.sStopState = pk.ReadInt64();
			Point* STurnPos;
			uShort ulTurnNum;
			STurnPos = (Point*)pk.ReadSequence(ulTurnNum);
			SMoveInfo.nPointNum = ulTurnNum / sizeof(Point);
			memcpy(SMoveInfo.SPos, STurnPos, ulTurnNum);

			// log
			long lDistX, lDistY, lDist = 0;
			LG(szLogName, "===Recieve(Move):\tTick:[%u]\n", GetTickCount());
			LG(szLogName, "Point:\t%3d\n", SMoveInfo.nPointNum);
			bool isMainCha = g_LogName.IsMainCha(l_id);
			for (int i = 0; i < SMoveInfo.nPointNum; i++) {
#ifdef _STATE_DEBUG
				if (isMainCha) {
					g_pGameApp->GetDrawPoints()->Add(SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, 0xffff0000, 0.5f);
					if (SMoveInfo.sState && i == SMoveInfo.nPointNum - 1) {
						g_pGameApp->GetDrawPoints()->Add(SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, 0xff000000, 0.3f);
					}
				}
#endif

				if (i > 0) {
					lDistX = SMoveInfo.SPos[i].x - SMoveInfo.SPos[i - 1].x;
					lDistY = SMoveInfo.SPos[i].y - SMoveInfo.SPos[i - 1].y;
					lDist = (long)sqrt((double)lDistX * lDistX + lDistY * lDistY);
				}
				LG(szLogName, "\t%d, %d\t%d\n", SMoveInfo.SPos[i].x, SMoveInfo.SPos[i].y, lDist);
			}
			if (SMoveInfo.sState)
				LG(szLogName, "@@@End Move\tState:0x%x\n", SMoveInfo.sState);


			if (SMoveInfo.sState & enumMSTATE_CANCEL) {
				long lDist;
				CCharacter* pCMainCha = CGameScene::GetMainCha();
				if (pCMainCha) {
					lDist = (pCMainCha->GetCurX() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x) * (pCMainCha->GetCurX() -
							SMoveInfo.SPos[SMoveInfo.nPointNum - 1].x)
						+ (pCMainCha->GetCurY() - SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y) * (pCMainCha->GetCurY() -
							SMoveInfo.SPos[SMoveInfo.nPointNum - 1].y);
					LG(szLogName, "++++++++++++++Distance: %d\n", (long)sqrt(double(lDist)));
				}
			}
			LG(szLogName, "\n");
			//


			NetActorMove(l_id, SMoveInfo);
		}
		break;
		case enumACTION_SKILL_SRC: {
			stNetNotiSkillRepresent SSkillInfo;
			SSkillInfo.chCrt = 0;
			SSkillInfo.sStopState = enumEXISTS_WAITING;

			SSkillInfo.byFightID = pk.ReadInt64();
			SSkillInfo.sAngle = pk.ReadInt64();
			SSkillInfo.sState = pk.ReadInt64();
			if (SSkillInfo.sState != enumFSTATE_ON)
				SSkillInfo.sStopState = pk.ReadInt64();
			SSkillInfo.lSkillID = pk.ReadInt64();
			SSkillInfo.lSkillSpeed = pk.ReadInt64();
			char chTarType = pk.ReadInt64();
			if (chTarType == 1) {
				SSkillInfo.lTargetID = pk.ReadInt64();
				SSkillInfo.STargetPoint.x = pk.ReadInt64();
				SSkillInfo.STargetPoint.y = pk.ReadInt64();
			}
			else if (chTarType == 2) {
				SSkillInfo.lTargetID = 0;
				SSkillInfo.STargetPoint.x = pk.ReadInt64();
				SSkillInfo.STargetPoint.y = pk.ReadInt64();
			}
			SSkillInfo.sExecTime = pk.ReadInt64();

			// ͬ������
			short lResNum = pk.ReadInt64();
			if (lResNum > 0) {
				SSkillInfo.SEffect.Resize(lResNum);
				for (long i = 0; i < lResNum; i++) {
					SSkillInfo.SEffect[i].lAttrID = pk.ReadInt64();
					SSkillInfo.SEffect[i].lVal = pk.ReadInt64();
					/*if(SSkillInfo.SEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SEffect[i].lAttrID==ATTR_CLEXP)
						SSkillInfo.SEffect[i].lVal= pk.ReadInt64();
					else
						SSkillInfo.SEffect[i].lVal = (long)pk.ReadInt64();*/
				}
			}

			// ͬ������״̬
			short chSStateNum = pk.ReadInt64();
			if (chSStateNum > 0) {
				SSkillInfo.SState.Resize(chSStateNum);
				for (short chNum = 0; chNum < chSStateNum; chNum++) {
					SSkillInfo.SState[chNum].chID = pk.ReadInt64();
					SSkillInfo.SState[chNum].chLv = pk.ReadInt64();
				}
			}

			// log
			LG(szLogName, "===Recieve(Skill Represent):\tTick:[%u]\n", GetTickCount());
			LG(szLogName, "Angle:\t%d\tFightID:%d\n", SSkillInfo.sAngle, SSkillInfo.byFightID);
			LG(szLogName, "SkillID:\t%u\tSkillSpeed:%d\n", SSkillInfo.lSkillID, SSkillInfo.lSkillSpeed);
			LG(szLogName, "TargetInfo(ID, PosX, PosY):\t%d\n", SSkillInfo.lTargetID, SSkillInfo.STargetPoint.x,
			   SSkillInfo.STargetPoint.y);
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
		case enumACTION_SKILL_TAR: {
			stNetNotiSkillEffect SSkillInfo{};

			SSkillInfo.byFightID = pk.ReadInt64();
			SSkillInfo.sState = pk.ReadInt64();
			SSkillInfo.bDoubleAttack = pk.ReadInt64() ? true : false;
			SSkillInfo.bMiss = pk.ReadInt64() ? true : false;
			if (SSkillInfo.bBeatBack = pk.ReadInt64() ? true : false) {
				SSkillInfo.SPos.x = pk.ReadInt64();
				SSkillInfo.SPos.y = pk.ReadInt64();
			}
			SSkillInfo.lSrcID = pk.ReadInt64();
			SSkillInfo.SSrcPos.x = pk.ReadInt64();
			SSkillInfo.SSrcPos.y = pk.ReadInt64();
			SSkillInfo.lSkillID = pk.ReadInt64();
			SSkillInfo.SSkillTPos.x = pk.ReadInt64();
			SSkillInfo.SSkillTPos.y = pk.ReadInt64();
			SSkillInfo.sExecTime = pk.ReadInt64();

			// ͬ������
			pk.ReadInt64();
			short lResNum = pk.ReadInt64();
			if (lResNum > 0) {
				SSkillInfo.SEffect.Resize(lResNum);
				for (long i = 0; i < lResNum; i++) {
					SSkillInfo.SEffect[i].lAttrID = pk.ReadInt64();
					SSkillInfo.SEffect[i].lVal = pk.ReadInt64();
					/*if(SSkillInfo.SEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SEffect[i].lAttrID==ATTR_CLEXP)
						SSkillInfo.SEffect[i].lVal= pk.ReadInt64();
					else
						SSkillInfo.SEffect[i].lVal = (long)pk.ReadInt64();*/

					char val[32];
					char buff[234];
					sprintf(buff, "ID = %d val = %s\r\n", SSkillInfo.SEffect[i].lAttrID,
							_i64toa(SSkillInfo.SEffect[i].lVal, val, 10));
					::OutputDebugStr(buff);
				}
			}

			short chSStateNum = 0;
			// ͬ������״̬
			if (pk.ReadInt64() == 1) {
				pk.ReadInt64();
				chSStateNum = pk.ReadInt64();
				if (chSStateNum > 0) {
					SSkillInfo.SState.Resize(chSStateNum);
					for (short chNum = 0; chNum < chSStateNum; chNum++) {
						SSkillInfo.SState[chNum].chID = pk.ReadInt64();
						SSkillInfo.SState[chNum].chLv = pk.ReadInt64();

						pk.ReadInt64();
						pk.ReadInt64();
					}
				}
			}

			short lSrcResNum = 0;
			short chSrcSStateNum = 0;
			if (pk.ReadInt64()) {
				// ����Դ״̬
				SSkillInfo.sSrcState = pk.ReadInt64();
				// ͬ������Դ����
				pk.ReadInt64();
				lSrcResNum = pk.ReadInt64();
				if (lSrcResNum > 0) {
					SSkillInfo.SSrcEffect.Resize(lSrcResNum);
					for (long i = 0; i < lSrcResNum; i++) {
						SSkillInfo.SSrcEffect[i].lAttrID = pk.ReadInt64();
						SSkillInfo.SSrcEffect[i].lVal = pk.ReadInt64();
						/*if(SSkillInfo.SSrcEffect[i].lAttrID==ATTR_CEXP||SSkillInfo.SSrcEffect[i].lAttrID==ATTR_NLEXP ||SSkillInfo.SSrcEffect[i].lAttrID==ATTR_CLEXP)
							SSkillInfo.SSrcEffect[i].lVal= pk.ReadInt64();
						else
							SSkillInfo.SSrcEffect[i].lVal = (long)pk.ReadInt64();*/
					}
				}
				NetActorSkillEff(l_id, SSkillInfo);
				break;

				if (pk.ReadInt64() == 1) {
					// ͬ������Դ����״̬
					pk.ReadInt64();
					chSrcSStateNum = pk.ReadInt64();
					if (chSrcSStateNum > 0) {
						SSkillInfo.SSrcState.Resize(chSrcSStateNum);
						for (short chNum = 0; chNum < chSrcSStateNum; chNum++) {
							SSkillInfo.SSrcState[chNum].chID = pk.ReadInt64();
							SSkillInfo.SSrcState[chNum].chLv = pk.ReadInt64();
							pk.ReadInt64();
							pk.ReadInt64();
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
			SLean.chState = pk.ReadInt64();
			if (!SLean.chState) {
				SLean.lPose = pk.ReadInt64();
				SLean.lAngle = pk.ReadInt64();
				SLean.lPosX = pk.ReadInt64();
				SLean.lPosY = pk.ReadInt64();
				SLean.lHeight = pk.ReadInt64();
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
		case enumACTION_ITEM_FAILED: {
			stNetSysInfo l_sysinfo;
			short sFailedID = pk.ReadInt64();
			l_sysinfo.m_sysinfo = g_GetUseItemFailedInfo(sFailedID);
			NetSysInfo(l_sysinfo);
		}
		break;
		case enumACTION_LOOK: // ���½�ɫ���
		{
			stNetLookInfo SLookInfo;
			ReadChaLookPacket(pk, SLookInfo, szLogName);
			CCharacter* pCha = CGameApp::GetCurScene()->SearchByID(l_id);

			if (!g_stUIMap.IsPKSilver() || pCha->GetMainType() == enumMainPlayer) {
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
		case enumACTION_ITEM_INFO: {
		}
		break;
		case enumACTION_SHORTCUT: // ���¿����
		{
			stNetShortCut SShortcut;
			ReadChaShortcutPacket(pk, SShortcut, szLogName);
			NetShortCut(l_id, SShortcut);
		}
		break;
		case enumACTION_TEMP: {
			stTempChangeChaPart STempChaPart;
			STempChaPart.dwItemID = pk.ReadInt64();
			STempChaPart.dwPartID = pk.ReadInt64();
			NetTempChangeChaPart(l_id, STempChaPart);
		}
		break;
		case enumACTION_CHANGE_CHA: {
			stNetChangeCha SChangeCha;
			SChangeCha.ulMainChaID = pk.ReadInt64();

			NetActorChangeCha(l_id, SChangeCha);
		}
		break;
		case enumACTION_FACE: {
			stNetFace SNetFace;
			SNetFace.sAngle = pk.ReadInt64();
			SNetFace.sPose = pk.ReadInt64();
			NetFace(l_id, SNetFace, enumACTION_FACE);

			// log
			LG(szLogName, "===Recieve(Face):\tTick:[%u]\n", GetTickCount());
			LG(szLogName, "Angle[%d]\tPose[%d]\n", SNetFace.sAngle, SNetFace.sPose);
			LG(szLogName, "\n");
			//
		}
		break;
		case enumACTION_SKILL_POSE: {
			stNetFace SNetFace;
			SNetFace.sAngle = pk.ReadInt64();
			SNetFace.sPose = pk.ReadInt64();
			NetFace(l_id, SNetFace, enumACTION_SKILL_POSE);

			// log
			LG(szLogName, "===Recieve(Skill Pos):\tTick:[%u]\n", GetTickCount());
			LG(szLogName, "Angle[%d]\tPose[%d]\n", SNetFace.sAngle, SNetFace.sPose);
			LG(szLogName, "\n");
			//
		}
		break;
		case enumACTION_PK_CTRL: {
			stNetPKCtrl SNetPKCtrl;
			ReadChaPKPacket(pk, SNetPKCtrl, szLogName);

			SNetPKCtrl.Exec(l_id);
		}
		break;
		case enumACTION_BANK: // ������
		{
			stNetKitbag SKitbag;
			ReadChaKitbagPacket(pk, SKitbag, szLogName);
			SKitbag.chBagType = 1;
			NetChangeKitbag(l_id, SKitbag);
		}
		break;
		case enumACTION_GUILDBANK: // ������
		{
			stNetKitbag SKitbag;
			ReadChaKitbagPacket(pk, SKitbag, szLogName);
			SKitbag.chBagType = 3;
			NetChangeKitbag(l_id, SKitbag);
		}
		break;
		case enumACTION_KITBAGTMP: // ����ʱ����
		{
			stNetKitbag STempKitbag;
			ReadChaKitbagPacket(pk, STempKitbag, szLogName);
			STempKitbag.chBagType = 2;
			NetChangeKitbag(l_id, STempKitbag);
			break;
		}
		case enumACTION_REQUESTGUILDLOGS: {
			g_stUIGuildMgr.RequestGuildLogs(pk);
			break;
		}
		case enumACTION_UPDATEGUILDLOGS: {
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
}

// Э��S->C : ��ɫ������ж�ʧ��
BOOL SC_FailedAction(LPRPACKET pk) {
	// Десериализация через типизированное сообщение
	net::msg::McFailedActionMessage msg;
	net::msg::deserialize(pk, msg);
	NetFailedAction(static_cast<char>(msg.reason));
	return TRUE;
}

// ͬ����ɫ����
BOOL SC_SynAttribute(LPRPACKET pk) {
	net::msg::McSynAttributeMessage msg;
	net::msg::deserialize(pk, msg);
	uLong l_id = static_cast<uLong>(msg.worldId);
	const char* szLogName = g_LogName.GetLogName(l_id);

	stNetChaAttr SChaAttr;
	memset(&SChaAttr, 0, sizeof(SChaAttr));
	SChaAttr.chType = static_cast<char>(msg.attr.synType);
	SChaAttr.sNum = static_cast<short>(msg.attr.attrs.size());
	for (short i = 0; i < SChaAttr.sNum; i++) {
		SChaAttr.SEff[i].lAttrID = static_cast<long>(msg.attr.attrs[i].attrId);
		SChaAttr.SEff[i].lVal = static_cast<long>(msg.attr.attrs[i].attrVal);
	}

	// log
	LG(szLogName, "Syn Character Attr: Count=%d\t, Type:%d\tTick:[%u]\n", SChaAttr.sNum, SChaAttr.chType,
	   GetTickCount());
	LG(szLogName, g_oLangRec.GetString(312));
	for (short i = 0; i < SChaAttr.sNum; i++) {
		LG(szLogName, "\t%d\t%d\n", SChaAttr.SEff[i].lAttrID, SChaAttr.SEff[i].lVal);
	}

	NetSynAttr(l_id, SChaAttr.chType, SChaAttr.sNum, SChaAttr.SEff);

	return TRUE;
}

// Синхронизация сумки скиллов
BOOL SC_SynSkillBag(LPRPACKET pk) {
	// Десериализация через типизированное сообщение
	net::msg::McSynSkillBagMessage msg;
	net::msg::deserialize(pk, msg);

	uLong l_id = static_cast<uLong>(msg.worldId);
	const char* szLogName = g_LogName.GetLogName(l_id);

	stNetSkillBag SCurSkill;
	memset(&SCurSkill, 0, sizeof(SCurSkill));

	// Установка default skill
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
		LG(szLogName, "Syn Skill Bag, Type:%d,\tTick:[%u]\n", SCurSkill.chType, GetTickCount());
		LG(szLogName, g_oLangRec.GetString(310));
		char szRange[256];
		for (short i = 0; i < sSkillNum; i++) {
			sprintf(szRange, "%d", pSBag[i].sRange[0]);
			if (pSBag[i].sRange[0] != enumRANGE_TYPE_NONE)
				for (short j = 1; j < defSKILL_RANGE_PARAM_NUM; j++)
					sprintf(szRange + strlen(szRange), ",%d", pSBag[i].sRange[j]);
			LG(szLogName, "\t%4d\t%4d\t%4d\t%6d\t%6d\t%6d\t%18d\t%s\n", pSBag[i].sID, pSBag[i].chState, pSBag[i].chLv,
			   pSBag[i].sUseSP, pSBag[i].sUseEndure, pSBag[i].sUseEnergy, pSBag[i].lResumeTime, szRange);
		}
		LG(szLogName, "\n");
	}

	NetSynSkillBag(l_id, &SCurSkill);
	return TRUE;
}

// ͬ��������
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
	const char* szLogName = g_LogName.GetLogName(l_id);

	unsigned long currentClient = GetTickCount();
	unsigned long currentServer = static_cast<unsigned long>(msg.skillState.currentTime) / 1000;

	stNetSkillState SCurSState;
	memset(&SCurSState, 0, sizeof(SCurSState));
	SCurSState.chType = 0;
	short sNum = static_cast<short>(msg.skillState.states.size());
	if (sNum > 0) {
		SCurSState.SState.Resize(sNum);
		for (int nNum = 0; nNum < sNum; nNum++) {
			SCurSState.SState[nNum].chID = static_cast<decltype(SCurSState.SState[nNum].chID)>(msg.skillState.states[nNum].stateId);
			SCurSState.SState[nNum].chLv = static_cast<decltype(SCurSState.SState[nNum].chLv)>(msg.skillState.states[nNum].stateLv);

			unsigned long duration = static_cast<unsigned long>(msg.skillState.states[nNum].duration);
			unsigned long start = static_cast<unsigned long>(msg.skillState.states[nNum].startTime) / 1000;

			unsigned long dif = currentServer - currentClient;
			unsigned long end = start - dif + duration;

			SCurSState.SState[nNum].lTimeRemaining = duration == 0 ? 0 : end - currentClient;
		}
	}

	// log
	LG(szLogName, "Syn Skill State: Num[%d]\tTick[%u]\n", sNum, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(311));
	for (char i = 0; i < sNum; i++)
		LG(szLogName, "\t%8d\t%4d\n", SCurSState.SState[i].chID, SCurSState.SState[i].chLv);
	LG(szLogName, "\n");

	NetSynSkillState(l_id, &SCurSState);

	return TRUE;
}

BOOL SC_SynTeam(LPRPACKET pk) {
	stNetTeamState STeamState;

	STeamState.ulID = pk.ReadInt64();
	STeamState.lHP = pk.ReadInt64();
	STeamState.lMaxHP = pk.ReadInt64();
	STeamState.lSP = pk.ReadInt64();
	STeamState.lMaxSP = pk.ReadInt64();
	STeamState.lLV = pk.ReadInt64();

	LG("Team", "Refresh, ID[%u], HP[%d], MaxHP[%d], SP[%d], MaxSP[%d], LV[%d]\n", STeamState.ulID, STeamState.lHP,
	   STeamState.lMaxHP, STeamState.lSP, STeamState.lMaxSP, STeamState.lLV);

	stNetLookInfo SLookInfo;
	ReadChaLookPacket(pk, SLookInfo, const_cast<char*>(g_oLangRec.GetString(299)));
	stNetChangeChaPart& SFace = SLookInfo.SLook;
	STeamState.SFace.sTypeID = SFace.sTypeID;
	STeamState.SFace.sHairID = SFace.sHairID;
	for (int i = 0; i < enumEQUIP_NUM; i++) {
		STeamState.SFace.SLink[i].sID = SFace.SLink[i].sID;
		STeamState.SFace.SLink[i].sNum = SFace.SLink[i].sNum;
		STeamState.SFace.SLink[i].chForgeLv = SFace.SLink[i].chForgeLv;
		STeamState.SFace.SLink[i].lFuseID = SFace.SLink[i].lDBParam[1] >> 16;
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
	LG(g_LogName.GetLogName(lID), g_oLangRec.GetString(300), lLeaderID, lID);
	//

	return TRUE;
}

BOOL SC_HelpInfo(LPRPACKET packet) {
	NET_HELPINFO Info;
	memset(&Info, 0, sizeof(NET_HELPINFO));

	Info.byType = packet.ReadInt64();
	if (Info.byType == mission::MIS_HELP_DESP || Info.byType == mission::MIS_HELP_IMAGE ||
		Info.byType == mission::MIS_HELP_BICKER) {
		std::string pszDesp = packet.ReadString();
		strncpy(Info.szDesp, pszDesp.c_str(), ROLE_MAXNUM_DESPSIZE - 1);
	}
	else if (Info.byType == mission::MIS_HELP_SOUND) {
		Info.sID = packet.ReadInt64();
	}
	else {
		return FALSE;
	}

	NetShowHelp(Info);
	return TRUE;
}

// NPC �Ի���Ϣ����
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

BOOL SC_TradeAllData(LPRPACKET packet) {
	DWORD dwNpcID = packet.ReadInt64();
	BYTE byType = packet.ReadInt64();
	DWORD dwParam = packet.ReadInt64();
	BYTE byCount = packet.ReadInt64();

	NET_TRADEINFO Info;
	for (BYTE i = 0; i < byCount; i++) {
		BYTE byItemType = packet.ReadInt64();
		switch (byItemType) {
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:
		case mission::TI_SYNTHESIS: {
			Info.TradePage[byItemType].byCount = packet.ReadInt64();
			if (Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM)
				Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

			for (BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++) {
				Info.TradePage[byItemType].sItemID[n] = packet.ReadInt64();
				if (byType == mission::TRADE_GOODS) {
					Info.TradePage[byItemType].sCount[n] = packet.ReadInt64();
					Info.TradePage[byItemType].dwPrice[n] = packet.ReadInt64();
					Info.TradePage[byItemType].byLevel[n] = packet.ReadInt64();
				}
			}
		}
		break;
		default:
			break;
		}
	}
	NetUpdateTradeAllData(Info, byType, dwNpcID, dwParam);
	return TRUE;
}

BOOL SC_TradeInfo(LPRPACKET packet) {
	DWORD dwNpcID = packet.ReadInt64();
	BYTE byType = packet.ReadInt64();
	DWORD dwParam = packet.ReadInt64();
	BYTE byCount = packet.ReadInt64();

	NET_TRADEINFO Info;
	for (BYTE i = 0; i < byCount; i++) {
		BYTE byItemType = packet.ReadInt64();
		switch (byItemType) {
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:
		case mission::TI_SYNTHESIS: {
			Info.TradePage[byItemType].byCount = packet.ReadInt64();
			if (Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM)
				Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

			for (BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++) {
				Info.TradePage[byItemType].sItemID[n] = packet.ReadInt64();
				if (byType == mission::TRADE_GOODS) {
					Info.TradePage[byItemType].sCount[n] = packet.ReadInt64();
					Info.TradePage[byItemType].dwPrice[n] = packet.ReadInt64();
					Info.TradePage[byItemType].byLevel[n] = packet.ReadInt64();
				}
			}
		}
		break;
		default:
			break;
		}
	}
	NetShowTrade(Info, byType, dwNpcID, dwParam);
	return TRUE;
}

BOOL SC_TradeUpdate(LPRPACKET packet) {
	//���н��׸���,ֻ���ڴ򿪺��н��׽�������²Ÿ���

	DWORD dwNpcID = packet.ReadInt64();
	BYTE byType = packet.ReadInt64();
	DWORD dwParam = packet.ReadInt64();
	BYTE byCount = packet.ReadInt64();

	NET_TRADEINFO Info;
	for (BYTE i = 0; i < byCount; i++) {
		BYTE byItemType = packet.ReadInt64();
		switch (byItemType) {
		case mission::TI_WEAPON:
		case mission::TI_DEFENCE:
		case mission::TI_OTHER:
		case mission::TI_SYNTHESIS: {
			Info.TradePage[byItemType].byCount = packet.ReadInt64();
			if (Info.TradePage[byItemType].byCount > ROLE_MAXNUM_TRADEITEM)
				Info.TradePage[byItemType].byCount = ROLE_MAXNUM_TRADEITEM;

			for (BYTE n = 0; n < Info.TradePage[byItemType].byCount; n++) {
				Info.TradePage[byItemType].sItemID[n] = packet.ReadInt64();
				if (byType == mission::TRADE_GOODS) {
					Info.TradePage[byItemType].sCount[n] = packet.ReadInt64();
					Info.TradePage[byItemType].dwPrice[n] = packet.ReadInt64();
					Info.TradePage[byItemType].byLevel[n] = packet.ReadInt64();
				}
			}
		}
		break;
		default:
			break;
		}
	}

	if (g_stUINpcTrade.GetIsShow()) {
		NetShowTrade(Info, byType, dwNpcID, dwParam);
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
	LG("trade", g_oLangRec.GetString(301), byType, byIndex, byCount, sItemID, dwMoney);
	NetTradeResult(byType, byIndex, byCount, sItemID, dwMoney);
	LG("trade", g_oLangRec.GetString(302));
	return TRUE;
}

BOOL SC_CharTradeInfo(LPRPACKET packet) {
	USHORT usCmd = packet.ReadInt64();
	switch (usCmd) {
	case CMD_MC_CHARTRADE_REQUEST: {
		BYTE byType = packet.ReadInt64();
		DWORD dwRequestID = packet.ReadInt64();
		NetShowCharTradeRequest(byType, dwRequestID);
	}
	break;
	case CMD_MC_CHARTRADE_CANCEL: {
		DWORD dwCharID = packet.ReadInt64();
		NetCancelCharTrade(dwCharID);
	}
	break;
	case CMD_MC_CHARTRADE_MONEY: {
		DWORD dwCharID = packet.ReadInt64();
		DWORD dwMoney = packet.ReadInt64();
		int currency = packet.ReadInt64();

		if (currency == 0) {
			NetTradeShowMoney(dwCharID, dwMoney);
		}
		else if (currency == 1) {
			NetTradeShowIMP(dwCharID, dwMoney);
		}
	}
	break;
	case CMD_MC_CHARTRADE_ITEM: {
		DWORD dwRequestID = packet.ReadInt64();
		BYTE byOpType = packet.ReadInt64();
		if (byOpType == mission::TRADE_DRAGTO_ITEM) {
			BYTE byItemIndex = packet.ReadInt64();
			BYTE byIndex = packet.ReadInt64();
			BYTE byCount = packet.ReadInt64();

			// ��Ʒʵ������
			NET_CHARTRADE_ITEMDATA Data;
			memset(&Data, 0, sizeof(NET_CHARTRADE_ITEMDATA));

			NetTradeAddItem(dwRequestID, byOpType, 0, byIndex, byCount, byItemIndex, Data);
		}
		else if (byOpType == mission::TRADE_DRAGTO_TRADE) {
			USHORT sItemID = packet.ReadInt64();
			BYTE byItemIndex = packet.ReadInt64();
			BYTE byIndex = packet.ReadInt64();
			BYTE byCount = packet.ReadInt64();
			USHORT sType = packet.ReadInt64();

			if (sType == enumItemTypeBoat) {
				NET_CHARTRADE_BOATDATA Data;
				memset(&Data, 0, sizeof(NET_CHARTRADE_BOATDATA));

				if (packet.ReadInt64() == 0) {
					// ��Ϣ����
				}
				else {
					strncpy(Data.szName, packet.ReadString().c_str(), BOAT_MAXSIZE_BOATNAME - 1);
					Data.sBoatID = packet.ReadInt64();
					Data.sLevel = packet.ReadInt64();
					Data.dwExp = packet.ReadInt64();
					Data.dwHp = packet.ReadInt64();
					Data.dwMaxHp = packet.ReadInt64();
					Data.dwSp = packet.ReadInt64();
					Data.dwMaxSp = packet.ReadInt64();
					Data.dwMinAttack = packet.ReadInt64();
					Data.dwMaxAttack = packet.ReadInt64();
					Data.dwDef = packet.ReadInt64();
					Data.dwSpeed = packet.ReadInt64();
					Data.dwShootSpeed = packet.ReadInt64();
					Data.byHasItem = packet.ReadInt64();
					Data.byCapacity = packet.ReadInt64();
					Data.dwPrice = packet.ReadInt64();
					NetTradeAddBoat(dwRequestID, byOpType, sItemID, byIndex, byCount, byItemIndex, Data);
				}
			}
			else {
				// ��Ʒʵ������
				NET_CHARTRADE_ITEMDATA Data;
				memset(&Data, 0, sizeof(NET_CHARTRADE_ITEMDATA));

				Data.sEndure[0] = packet.ReadInt64();
				Data.sEndure[1] = packet.ReadInt64();
				Data.sEnergy[0] = packet.ReadInt64();
				Data.sEnergy[1] = packet.ReadInt64();
				Data.byForgeLv = packet.ReadInt64();
				Data.bValid = packet.ReadInt64() != 0 ? true : false;
				Data.bItemTradable = packet.ReadInt64();
				Data.expiration = packet.ReadInt64();

				Data.lDBParam[enumITEMDBP_FORGE] = packet.ReadInt64();
				Data.lDBParam[enumITEMDBP_INST_ID] = packet.ReadInt64();


				Data.byHasAttr = packet.ReadInt64();
				if (Data.byHasAttr) {
					for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++) {
						Data.sInstAttr[i][0] = packet.ReadInt64();
						Data.sInstAttr[i][1] = packet.ReadInt64();
					}
				}

				NetTradeAddItem(dwRequestID, byOpType, sItemID, byIndex, byCount, byItemIndex, Data);
			}
		}
		else {
			MessageBox(NULL, g_oLangRec.GetString(303), g_oLangRec.GetString(25), MB_OK);
			return FALSE;
		}
	}
	break;
	case CMD_MC_CHARTRADE_PAGE: {
		BYTE byType = packet.ReadInt64();
		DWORD dwAcceptID = packet.ReadInt64();
		DWORD dwRequestID = packet.ReadInt64();
		NetShowCharTradeInfo(byType, dwAcceptID, dwRequestID);
	}
	break;
	case CMD_MC_CHARTRADE_VALIDATEDATA: {
		DWORD dwCharID = packet.ReadInt64();
		NetValidateTradeData(dwCharID);
	}
	break;
	case CMD_MC_CHARTRADE_VALIDATE: {
		DWORD dwCharID = packet.ReadInt64();
		NetValidateTrade(dwCharID);
	}
	break;
	case CMD_MC_CHARTRADE_RESULT: {
		BYTE byResult = packet.ReadInt64();
		if (byResult == mission::TRADE_SUCCESS) {
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
	const auto imgName = packet.ReadString();
	if (imgName.empty()) {
		LG("DailyBuffInfo", "Error invalid reading image name \n");
		return FALSE;
	}
	const auto labelInfo = packet.ReadString();
	if (labelInfo.empty()) {
		LG("DailyBuffInfo", "Error invalid reading labelInfo  \n");
		return FALSE;
	}
	//show the form
	g_stUIMap.SetupDailyBuffForm(imgName.c_str(), labelInfo.c_str());
	return TRUE;
}

BOOL SC_MissionInfo(LPRPACKET packet) {
	// Десериализация через типизированное сообщение
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
	// Десериализация через типизированное сообщение
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
		// Потребности квеста
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
				LG("mission_error", g_oLangRec.GetString(304));
				return FALSE;
			}
		}

		// Награды квеста
		page.byPrizeSelType = static_cast<decltype(page.byPrizeSelType)>(msg.prizeSelType);
		page.byPrizeNum = static_cast<decltype(page.byPrizeNum)>(msg.prizes.size());
		for (int i = 0; i < page.byPrizeNum; i++) {
			page.MisPrize[i].byType = static_cast<decltype(page.MisPrize[i].byType)>(msg.prizes[i].type);
			page.MisPrize[i].wParam1 = static_cast<decltype(page.MisPrize[i].wParam1)>(msg.prizes[i].param1);
			page.MisPrize[i].wParam2 = static_cast<decltype(page.MisPrize[i].wParam2)>(msg.prizes[i].param2);
		}

		// Описание квеста
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
	// Десериализация через типизированное сообщение
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
	// Десериализация через типизированное сообщение
	net::msg::McMisLogInfoMessage msg;
	net::msg::deserialize(packet, msg);

	WORD wMisID = static_cast<WORD>(msg.misId);
	NET_MISPAGE page;
	memset(&page, 0, sizeof(NET_MISPAGE));

	strncpy(page.szName, msg.name.c_str(), ROLE_MAXSIZE_MISNAME - 1);

	// Потребности квеста
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
			LG("mission_error", g_oLangRec.GetString(304));
			return FALSE;
		}
	}

	// Награды квеста
	page.byPrizeSelType = static_cast<decltype(page.byPrizeSelType)>(msg.prizeSelType);
	page.byPrizeNum = static_cast<decltype(page.byPrizeNum)>(msg.prizes.size());
	for (int i = 0; i < page.byPrizeNum; i++) {
		page.MisPrize[i].byType = static_cast<decltype(page.MisPrize[i].byType)>(msg.prizes[i].type);
		page.MisPrize[i].wParam1 = static_cast<decltype(page.MisPrize[i].wParam1)>(msg.prizes[i].param1);
		page.MisPrize[i].wParam2 = static_cast<decltype(page.MisPrize[i].wParam2)>(msg.prizes[i].param2);
	}

	// Описание квеста
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
	// Десериализация через типизированное сообщение
	net::msg::McMisLogAddMessage msg;
	net::msg::deserialize(packet, msg);
	NetMisLogAdd(static_cast<WORD>(msg.missionId), static_cast<BYTE>(msg.state));
	return TRUE;
}

BOOL SC_MisLogState(LPRPACKET packet) {
	// Десериализация через типизированное сообщение
	net::msg::McMisLogStateMessage msg;
	net::msg::deserialize(packet, msg);
	NetMisLogState(static_cast<WORD>(msg.missionId), static_cast<BYTE>(msg.state));
	return TRUE;
}

BOOL SC_TriggerAction(LPRPACKET packet) {
	// Десериализация через типизированное сообщение
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
	DWORD dwNpcID = packet.ReadInt64();
	CS_GMRecv(dwNpcID);
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

BOOL SC_CreateBoat(LPRPACKET packet) {
	DWORD dwBoatID = packet.ReadInt64();
	xShipBuildInfo Info;
	memset(&Info, 0, sizeof(xShipBuildInfo));

	char szBoat[BOAT_MAXSIZE_NAME] = {0}; // �����Զ���
	strncpy(szBoat, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	strncpy(Info.szName, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	strncpy(Info.szDesp, packet.ReadString().c_str(), BOAT_MAXSIZE_DESP - 1);
	strncpy(Info.szBerth, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	Info.byIsUpdate = packet.ReadInt64();
	Info.sPosID = packet.ReadInt64();
	Info.dwBody = packet.ReadInt64();
	strncpy(Info.szBody, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	if (Info.byIsUpdate) {
		Info.byHeader = packet.ReadInt64();
		Info.dwHeader = packet.ReadInt64();
		strncpy(Info.szHeader, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

		Info.byEngine = packet.ReadInt64();
		Info.dwEngine = packet.ReadInt64();
		strncpy(Info.szEngine, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
		for (int i = 0; i < BOAT_MAXNUM_MOTOR; i++) {
			Info.dwMotor[i] = packet.ReadInt64();
		}
	}
	Info.byCannon = packet.ReadInt64();
	strncpy(Info.szCannon, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	Info.byEquipment = packet.ReadInt64();
	strncpy(Info.szEquipment, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	Info.dwMoney = packet.ReadInt64();
	Info.dwMinAttack = packet.ReadInt64();
	Info.dwMaxAttack = packet.ReadInt64();
	Info.dwCurEndure = packet.ReadInt64();
	Info.dwMaxEndure = packet.ReadInt64();
	Info.dwSpeed = packet.ReadInt64();
	Info.dwDistance = packet.ReadInt64();
	Info.dwDefence = packet.ReadInt64();
	Info.dwCurSupply = packet.ReadInt64();
	Info.dwMaxSupply = packet.ReadInt64();
	Info.dwConsume = packet.ReadInt64();
	Info.dwAttackTime = packet.ReadInt64();
	Info.sCapacity = packet.ReadInt64();

	NetCreateBoat(Info);
	return TRUE;
}

BOOL SC_UpdateBoat(LPRPACKET packet) {
	DWORD dwBoatID = packet.ReadInt64();
	xShipBuildInfo Info;
	memset(&Info, 0, sizeof(xShipBuildInfo));

	char szBoat[BOAT_MAXSIZE_NAME] = {0}; // �����Զ���
	strncpy(szBoat, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	strncpy(Info.szName, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	strncpy(Info.szDesp, packet.ReadString().c_str(), BOAT_MAXSIZE_DESP - 1);
	strncpy(Info.szBerth, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	Info.byIsUpdate = packet.ReadInt64();
	Info.sPosID = packet.ReadInt64();
	Info.dwBody = packet.ReadInt64();
	strncpy(Info.szBody, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	if (Info.byIsUpdate) {
		Info.byHeader = packet.ReadInt64();
		Info.dwHeader = packet.ReadInt64();
		strncpy(Info.szHeader, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

		Info.byEngine = packet.ReadInt64();
		Info.dwEngine = packet.ReadInt64();
		strncpy(Info.szEngine, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
		for (int i = 0; i < BOAT_MAXNUM_MOTOR; i++) {
			Info.dwMotor[i] = packet.ReadInt64();
		}
	}
	Info.byCannon = packet.ReadInt64();
	strncpy(Info.szCannon, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	Info.byEquipment = packet.ReadInt64();
	strncpy(Info.szEquipment, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	Info.dwMoney = packet.ReadInt64();
	Info.dwMinAttack = packet.ReadInt64();
	Info.dwMaxAttack = packet.ReadInt64();
	Info.dwCurEndure = packet.ReadInt64();
	Info.dwMaxEndure = packet.ReadInt64();
	Info.dwSpeed = packet.ReadInt64();
	Info.dwDistance = packet.ReadInt64();
	Info.dwDefence = packet.ReadInt64();
	Info.dwCurSupply = packet.ReadInt64();
	Info.dwMaxSupply = packet.ReadInt64();
	Info.dwConsume = packet.ReadInt64();
	Info.dwAttackTime = packet.ReadInt64();
	Info.sCapacity = packet.ReadInt64();

	NetUpdateBoat(Info);
	return TRUE;
}

BOOL SC_BoatInfo(LPRPACKET packet) {
	DWORD dwBoatID = packet.ReadInt64();
	xShipBuildInfo Info;
	memset(&Info, 0, sizeof(xShipBuildInfo));

	char szBoat[BOAT_MAXSIZE_NAME] = {0}; // �����Զ���
	strncpy(szBoat, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	strncpy(Info.szName, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	strncpy(Info.szDesp, packet.ReadString().c_str(), BOAT_MAXSIZE_DESP - 1);
	strncpy(Info.szBerth, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
	Info.byIsUpdate = packet.ReadInt64();
	Info.sPosID = packet.ReadInt64();
	Info.dwBody = packet.ReadInt64();
	strncpy(Info.szBody, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	if (Info.byIsUpdate) {
		Info.byHeader = packet.ReadInt64();
		Info.dwHeader = packet.ReadInt64();
		strncpy(Info.szHeader, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

		Info.byEngine = packet.ReadInt64();
		Info.dwEngine = packet.ReadInt64();
		strncpy(Info.szEngine, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);
		for (int i = 0; i < BOAT_MAXNUM_MOTOR; i++) {
			Info.dwMotor[i] = packet.ReadInt64();
		}
	}
	Info.byCannon = packet.ReadInt64();
	strncpy(Info.szCannon, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	Info.byEquipment = packet.ReadInt64();
	strncpy(Info.szEquipment, packet.ReadString().c_str(), BOAT_MAXSIZE_NAME - 1);

	Info.dwMoney = packet.ReadInt64();
	Info.dwMinAttack = packet.ReadInt64();
	Info.dwMaxAttack = packet.ReadInt64();
	Info.dwCurEndure = packet.ReadInt64();
	Info.dwMaxEndure = packet.ReadInt64();
	Info.dwSpeed = packet.ReadInt64();
	Info.dwDistance = packet.ReadInt64();
	Info.dwDefence = packet.ReadInt64();
	Info.dwCurSupply = packet.ReadInt64();
	Info.dwMaxSupply = packet.ReadInt64();
	Info.dwConsume = packet.ReadInt64();
	Info.dwAttackTime = packet.ReadInt64();
	Info.sCapacity = packet.ReadInt64();

	NetBoatInfo(dwBoatID, szBoat, Info);
	return TRUE;
}

BOOL SC_UpdateBoatPart(LPRPACKET packet) {
	return TRUE;
}

BOOL SC_BoatList(LPRPACKET packet) {
	DWORD dwNpcID = packet.ReadInt64();
	BYTE byType = packet.ReadInt64();
	BYTE byNumBoat = packet.ReadInt64();
	BOAT_BERTH_DATA Data;
	memset(&Data, 0, sizeof(BOAT_BERTH_DATA));
	for (BYTE i = 0; i < byNumBoat; i++) {
		strncpy(Data.szName[i], packet.ReadString().c_str(), BOAT_MAXSIZE_BOATNAME + BOAT_MAXSIZE_DESP - 1);
	}

	NetShowBoatList(dwNpcID, byNumBoat, Data, byType);
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
	DWORD dwCharID = packet.ReadInt64();
	BYTE byNum = packet.ReadInt64();
	std::string pszName = packet.ReadString();
	if (pszName.empty()) return FALSE;

	NetStallInfo(dwCharID, byNum, pszName.c_str());

	for (BYTE i = 0; i < byNum; ++i) {
		BYTE byGrid = packet.ReadInt64();
		USHORT sItemID = packet.ReadInt64();
		BYTE byCount = packet.ReadInt64();
		DWORD dwMoney = packet.ReadInt64();
		USHORT sType = packet.ReadInt64();

		if (sType == enumItemTypeBoat) {
			NET_CHARTRADE_BOATDATA Data;
			memset(&Data, 0, sizeof(NET_CHARTRADE_BOATDATA));

			if (packet.ReadInt64() == 0) {
				// ��Ϣ����
			}
			else {
				strncpy(Data.szName, packet.ReadString().c_str(), BOAT_MAXSIZE_BOATNAME - 1);
				Data.sBoatID = packet.ReadInt64();
				Data.sLevel = packet.ReadInt64();
				Data.dwExp = packet.ReadInt64();
				Data.dwHp = packet.ReadInt64();
				Data.dwMaxHp = packet.ReadInt64();
				Data.dwSp = packet.ReadInt64();
				Data.dwMaxSp = packet.ReadInt64();
				Data.dwMinAttack = packet.ReadInt64();
				Data.dwMaxAttack = packet.ReadInt64();
				Data.dwDef = packet.ReadInt64();
				Data.dwSpeed = packet.ReadInt64();
				Data.dwShootSpeed = packet.ReadInt64();
				Data.byHasItem = packet.ReadInt64();
				Data.byCapacity = packet.ReadInt64();
				Data.dwPrice = packet.ReadInt64();
				NetStallAddBoat(byGrid, sItemID, byCount, dwMoney, Data);
			}
		}
		else {
			// ��Ʒʵ������
			NET_CHARTRADE_ITEMDATA Data;
			memset(&Data, 0, sizeof(NET_CHARTRADE_ITEMDATA));

			Data.sEndure[0] = packet.ReadInt64();
			Data.sEndure[1] = packet.ReadInt64();
			Data.sEnergy[0] = packet.ReadInt64();
			Data.sEnergy[1] = packet.ReadInt64();
			Data.byForgeLv = packet.ReadInt64();
			Data.bValid = packet.ReadInt64() != 0 ? true : false;
			Data.bItemTradable = packet.ReadInt64();
			Data.expiration = packet.ReadInt64();

			Data.lDBParam[enumITEMDBP_FORGE] = packet.ReadInt64();
			Data.lDBParam[enumITEMDBP_INST_ID] = packet.ReadInt64();


			Data.byHasAttr = packet.ReadInt64();
			if (Data.byHasAttr) {
				for (int i = 0; i < defITEM_INSTANCE_ATTR_NUM; i++) {
					Data.sInstAttr[i][0] = packet.ReadInt64();
					Data.sInstAttr[i][1] = packet.ReadInt64();
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
	DWORD dwCharID = packet.ReadInt64();
	BYTE byGrid = packet.ReadInt64();
	BYTE byCount = packet.ReadInt64();
	NetStallDelGoods(dwCharID, byGrid, byCount);
	return TRUE;
}

BOOL SC_StallClose(LPRPACKET packet) {
	DWORD dwCharID = packet.ReadInt64();
	NetStallClose(dwCharID);
	return TRUE;
}

BOOL SC_StallSuccess(LPRPACKET packet) {
	DWORD dwCharID = packet.ReadInt64();
	NetStallSuccess(dwCharID);
	return TRUE;
}

BOOL SC_SynStallName(LPRPACKET packet) {
	// Десериализация через типизированное сообщение
	net::msg::McSynStallNameMessage msg;
	net::msg::deserialize(packet, msg);
	NetStallName(static_cast<DWORD>(msg.charId), msg.name.c_str());
	return TRUE;
}

BOOL SC_StartExit(LPRPACKET packet) {
	DWORD dwExitTime = packet.ReadInt64();
	NetStartExit(dwExitTime);
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
	char szLogName[128] = {0};
	strcpy(szLogName, g_oLangRec.GetString(305));

	stNetTeamFightAsk SFightAsk;
	SFightAsk.chSideNum2 = packet.ReverseReadInt64();
	SFightAsk.chSideNum1 = packet.ReverseReadInt64();
	LG(szLogName, g_oLangRec.GetString(306), SFightAsk.chSideNum1, SFightAsk.chSideNum2);
	for (char i = 0; i < SFightAsk.chSideNum1 + SFightAsk.chSideNum2; i++) {
		SFightAsk.Info[i].szName = packet.ReadString();
		SFightAsk.Info[i].chLv = packet.ReadInt64();
		SFightAsk.Info[i].szJob = packet.ReadString();
		SFightAsk.Info[i].usFightNum = packet.ReadInt64();
		SFightAsk.Info[i].usVictoryNum = packet.ReadInt64();
		LG(szLogName, g_oLangRec.GetString(307), SFightAsk.Info[i].szName.c_str(), SFightAsk.Info[i].chLv,
		   SFightAsk.Info[i].szJob.c_str());
	}
	LG(szLogName, "\n");
	SFightAsk.Exec();
	return TRUE;
}

BOOL SC_BeginItemRepair(LPRPACKET packet) {
	NetBeginRepairItem();
	return TRUE;
}

BOOL SC_ItemRepairAsk(LPRPACKET packet) {
	stNetItemRepairAsk SRepairAsk;
	SRepairAsk.cszItemName = packet.ReadString();
	SRepairAsk.lRepairMoney = packet.ReadInt64();
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
		SEspItem.SContent[i].bItemTradable = static_cast<decltype(SEspItem.SContent[0].bItemTradable)>(msg.items[i].tradable);
		SEspItem.SContent[i].expiration = static_cast<decltype(SEspItem.SContent[0].expiration)>(msg.items[i].expiration);
	}

	NetEspeItem(nChaID, SEspItem);
	return TRUE;
}

BOOL SC_MapCrash(LPRPACKET packet) {
	std::string pszDesp = packet.ReadString();
	if (pszDesp.empty())
		return FALSE;

	NetShowMapCrash(pszDesp.c_str());
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
	char szInfo[512] = "";
	sprintf(szInfo, g_oLangRec.GetString(308), msg.name.c_str(), static_cast<long>(msg.chaId2),
		msg.mapName.c_str(), static_cast<long>(msg.posX), static_cast<long>(msg.posY));
	SShowInfo.m_sysinfo = szInfo;
	NetSysInfo(SShowInfo);

	return TRUE;
}

BOOL SC_QueryChaItem(LPRPACKET pk) {
	uLong l_id = pk.ReadInt64();

	return TRUE;
}

BOOL SC_QueryChaPing(LPRPACKET pk) {
	net::msg::McQueryChaPingMessage msg;
	net::msg::deserialize(pk, msg);

	stNetSysInfo SShowInfo;
	char szInfo[512] = "";
	sprintf(szInfo, g_oLangRec.GetString(309), msg.mapName.c_str(), static_cast<long>(msg.ping));
	SShowInfo.m_sysinfo = szInfo;
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
	uLong l_id = pk.ReadInt64();
	uShort usLen = 0;
	BYTE* pMapMask = 0;
	if (pk.ReadInt64())
		pMapMask = (BYTE*)pk.ReadSequence(usLen);

	//char	*szMask = new char[usLen + 1];
	//memcpy(szMask, pMapMask, usLen);
	//szMask[usLen] = '\0';
	//LG("���ͼ", "%s\n", szMask);

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
	const char* szLogName = g_LogName.GetLogName(l_id);

	stNetChaSideInfo SNetSideInfo;
	SNetSideInfo.chSideID = static_cast<decltype(SNetSideInfo.chSideID)>(msg.side.sideId);

	// log
	LG(szLogName, "===Recieve(SideInfo)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tSideID: %d\n", SNetSideInfo.chSideID);
	LG(szLogName, "\n");

	NetChaSideInfo(l_id, SNetSideInfo);

	return TRUE;
}

BOOL SC_SynAppendLook(LPRPACKET pk) {
	uLong l_id = pk.ReadInt64();
	const char* szLogName = g_LogName.GetLogName(l_id);
	stNetAppendLook SNetAppendLook;
	ReadChaAppendLookPacket(pk, SNetAppendLook, szLogName);
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
	bool bValid = packet.ReadInt64() ? true : false; // �̳��Ƿ���
	if (bValid) {
		g_stUIStore.ClearStoreTreeNode();
		g_stUIStore.ClearStoreItemList();
		//g_stUIStore.SetStoreBuyButtonEnable(true);

		long lVip = packet.ReadInt64(); // VIP
		long lMoBean = packet.ReadInt64(); // Ħ��
		long lRplMoney = packet.ReadInt64(); // ���ң�ˮ����
		g_stUIStore.SetStoreMoney(lMoBean, lRplMoney);
		g_stUIStore.SetStoreVip(lVip);

		long lAfficheNum = packet.ReadInt64(); // ��������
		int i;
		for (i = 0; i < lAfficheNum; i++) {
			long lAfficheID = packet.ReadInt64(); // ����ID
			uShort len;
			cChar* szTitle = packet.ReadSequence(len); // �������
			cChar* szComID = packet.ReadSequence(len); // ��Ӧ��ƷID,�ö��Ÿ���

			// ���ӹ���
		}
		long lFirstClass = 0;
		long lClsNum = packet.ReadInt64(); // ��������
		for (i = 0; i < lClsNum; i++) {
			short lClsID = packet.ReadInt64(); // ����ID
			uShort len;
			cChar* szClsName = packet.ReadSequence(len); // ������
			short lParentID = packet.ReadInt64(); // ����ID

			// �����̳������
			g_stUIStore.AddStoreTreeNode(lParentID, lClsID, szClsName);

			// �ύ��һ������
			if (i == 0) {
				lFirstClass = lClsID;
			}
		}

		g_stUIStore.AddStoreUserTreeNode(); // ���Ӹ��˹���
		g_stUIStore.ShowStoreForm(true);

		if (lFirstClass > 0) {
			::CS_StoreListAsk(lFirstClass, 1, (short)CStoreMgr::GetPageSize());
		}
	}
	else {
		// �̳�δ����,����ҳ
		g_stUIStore.ShowStoreWebPage();
	}

	g_stUIStore.DarkScene(false);
	g_stUIStore.ShowStoreLoad(false);

	return TRUE;
}

BOOL SC_StoreListAnswer(LPRPACKET packet) {
	g_stUIStore.ClearStoreItemList();

	short sPageNum = packet.ReadInt64(); // ҳ��
	short sPageCur = packet.ReadInt64(); // ��ǰҳ
	short sComNum = packet.ReadInt64(); // ��Ʒ��

	long i;
	for (i = 0; i < sComNum; i++) {
		long lComID = packet.ReadInt64(); // ��ƷID
		uShort len;
		cChar* szComName = packet.ReadSequence(len); // ��Ʒ����
		long lPrice = packet.ReadInt64(); // ��Ʒ�۸�
		cChar* szRemark = packet.ReadSequence(len); // ��Ʒ��ע
		bool isHot = packet.ReadInt64() ? true : false; // �Ƿ���������Ʒ
		long nTime = packet.ReadInt64();
		long lComNumber = packet.ReadInt64(); // ��Ʒʣ�������-1������
		long lComExpire = packet.ReadInt64(); // ��Ʒ����ʱ��

		// ���ӵ��̳�
		g_stUIStore.AddStoreItemInfo(i, lComID, szComName, lPrice, szRemark, isHot, nTime, lComNumber, lComExpire);

		short sItemClsNum = packet.ReadInt64(); // ������������
		int j;
		for (j = 0; j < sItemClsNum; j++) {
			short sItemID = packet.ReadInt64(); // ����ID
			short sItemNum = packet.ReadInt64(); // ��������
			short sFlute = packet.ReadInt64(); // ��������
			short pItemAttrID[5];
			short pItemAttrVal[5];
			int k;
			for (k = 0; k < 5; k++) {
				pItemAttrID[k] = packet.ReadInt64(); // ����ID
				pItemAttrVal[k] = packet.ReadInt64(); // ����ֵ
			}

			// ��Ʒϸ������
			g_stUIStore.AddStoreItemDetail(i, j, sItemID, sItemNum, sFlute, pItemAttrID, pItemAttrVal);
		}
	}

	// ����ҳ��
	g_stUIStore.SetStoreItemPage(sPageCur, sPageNum);

	return TRUE;
}

BOOL SC_StoreBuyAnswer(LPRPACKET packet) {
	bool bSucc = packet.ReadInt64() ? true : false; // �����Ƿ�ɹ�
	long lMoBean = 0;
	long lRplMoney = 0;

	// ֪ͨ��ҹ���ɹ�
	// ...
	if (bSucc) {
		//lMoBean = packet.ReadInt64();
		lRplMoney = packet.ReadInt64();
		g_stUIEquip.UpdateIMP(lRplMoney);
		g_stUIStore.SetStoreMoney(-1, lRplMoney);
	}
	else {
		g_pGameApp->MsgBox(g_oLangRec.GetString(907)); // �������ʧ��!
	}

	g_stUIStore.SetStoreBuyButtonEnable(true);
	return TRUE;
}

BOOL SC_StoreChangeAnswer(LPRPACKET packet) {
	bool bSucc = packet.ReadInt64() ? true : false; // �һ������Ƿ�ɹ�
	if (bSucc) {
		long lMoBean = packet.ReadInt64(); // ��ǰĦ������
		long lRplMoney = packet.ReadInt64(); // ��ǰ��������

		g_stUIStore.SetStoreMoney(lMoBean, lRplMoney);
	}
	else {
		g_pGameApp->MsgBox(g_oLangRec.GetString(908)); // ���Ҷһ�ʧ��!
	}
	return TRUE;
}

BOOL SC_StoreHistory(LPRPACKET packet) {
	long lNum = packet.ReadInt64(); // ��ȡ�Ľ��׼�¼����
	int i;
	for (i = 0; i < lNum; i++) {
		uShort len;
		cChar* szTime = packet.ReadSequence(len); // ����ʱ��
		long lComID = packet.ReadInt64(); // ��ƷID
		cChar* szName = packet.ReadSequence(len); // ��Ʒ����
		long lMoney = packet.ReadInt64(); // ���׽��
	}
	return TRUE;
}

BOOL SC_ActInfo(LPRPACKET packet) {
	bool bSucc = packet.ReadInt64() ? true : false; // �ʺ���Ϣ��ѯ�Ƿ�ɹ�
	if (bSucc) {
		long lMoBean = packet.ReadInt64(); // Ħ������
		long lRplMoney = packet.ReadInt64(); // ��������
	}
	return TRUE;
}

BOOL SC_StoreVIP(LPRPACKET packet) {
	bool bSucc = packet.ReadInt64() ? true : false;
	if (bSucc) {
		short sVipID = packet.ReadInt64();
		short sMonth = packet.ReadInt64();
		long lShuijing = packet.ReadInt64();
		long lModou = packet.ReadInt64();

		g_stUIStore.SetStoreVip(sVipID);
		g_stUIStore.SetStoreMoney(lModou, lShuijing);
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
	short sNum = packet.ReadInt64(); // ����
	short sItemID[3] = {0};

	for (int i = 0; i < 3; i++) {
		sItemID[i] = packet.ReadInt64();
	}

	g_stUISpirit.UpdateErnieNumber(sNum, sItemID[0], sItemID[1], sItemID[2]);

	if (sNum == 3) {
		// ���һ��
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

BOOL SC_SyncTigerString(LPRPACKET packet) {
	std::string szString = packet.ReadString();
	g_stUISpirit.UpdateErnieString(szString.c_str());

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
	unsigned char l_type = packet.ReadInt64();
	switch (l_type) {
	case MSG_MASTER_REFRESH_ONLINE: {
		uLong ulChaID = packet.ReadInt64();
		NetMasterOnline(ulChaID);
	}
	break;
	case MSG_MASTER_REFRESH_OFFLINE: {
		uLong ulChaID = packet.ReadInt64();
		NetMasterOffline(ulChaID);
	}
	break;
	case MSG_MASTER_REFRESH_DEL: {
		uLong ulChaID = packet.ReadInt64();
		NetMasterDel(ulChaID);
	}
	break;
	case MSG_MASTER_REFRESH_ADD: {
		std::string l_grp = packet.ReadString();
		uLong l_chaid = packet.ReadInt64();
		std::string l_chaname = packet.ReadString();
		std::string l_motto = packet.ReadString();
		uShort l_icon = packet.ReadInt64();
		NetMasterAdd(l_chaid, l_chaname.c_str(), l_motto.c_str(), l_icon, l_grp.c_str());
	}
	break;
	case MSG_MASTER_REFRESH_START: {
		stNetFrndStart l_self;
		l_self.lChaid = packet.ReadInt64();
		l_self.szChaname = packet.ReadString();
		l_self.szMotto = packet.ReadString();
		l_self.sIconID = packet.ReadInt64();
		stNetFrndStart l_nfs[100];
		uShort l_nfnum = 0, l_grpnum = packet.ReadInt64();
		for (uShort l_grpi = 0; l_grpi < l_grpnum; l_grpi++) {
			std::string l_grp = packet.ReadString();
			uShort l_grpmnum = packet.ReadInt64();
			for (uShort l_grpmi = 0; l_grpmi < l_grpmnum; l_grpmi++) {
				l_nfs[l_nfnum].szGroup = l_grp;
				l_nfs[l_nfnum].lChaid = packet.ReadInt64();
				l_nfs[l_nfnum].szChaname = packet.ReadString();
				l_nfs[l_nfnum].szMotto = packet.ReadString();
				l_nfs[l_nfnum].sIconID = packet.ReadInt64();
				l_nfs[l_nfnum].cStatus = packet.ReadInt64();
				l_nfnum++;
			}
		}
		NetMasterStart(l_self, l_nfs, l_nfnum);
	}
	break;

	case MSG_PRENTICE_REFRESH_ONLINE: {
		uLong ulChaID = packet.ReadInt64();
		NetPrenticeOnline(ulChaID);
	}
	break;
	case MSG_PRENTICE_REFRESH_OFFLINE: {
		uLong ulChaID = packet.ReadInt64();
		NetPrenticeOffline(ulChaID);
	}
	break;
	case MSG_PRENTICE_REFRESH_DEL: {
		uLong ulChaID = packet.ReadInt64();
		NetPrenticeDel(ulChaID);
	}
	break;
	case MSG_PRENTICE_REFRESH_ADD: {
		std::string l_grp = packet.ReadString();
		uLong l_chaid = packet.ReadInt64();
		std::string l_chaname = packet.ReadString();
		std::string l_motto = packet.ReadString();
		uShort l_icon = packet.ReadInt64();
		NetPrenticeAdd(l_chaid, l_chaname.c_str(), l_motto.c_str(), l_icon, l_grp.c_str());
	}
	break;
	case MSG_PRENTICE_REFRESH_START: {
		stNetFrndStart l_self;
		l_self.lChaid = packet.ReadInt64();
		l_self.szChaname = packet.ReadString();
		l_self.szMotto = packet.ReadString();
		l_self.sIconID = packet.ReadInt64();
		stNetFrndStart l_nfs[100];
		uShort l_nfnum = 0, l_grpnum = packet.ReadInt64();
		for (uShort l_grpi = 0; l_grpi < l_grpnum; l_grpi++) {
			std::string l_grp = packet.ReadString();
			uShort l_grpmnum = packet.ReadInt64();
			for (uShort l_grpmi = 0; l_grpmi < l_grpmnum; l_grpmi++) {
				uShort index = l_grpmi % (sizeof(l_nfs) / sizeof(l_nfs[0]));
				l_nfs[index].szGroup = l_grp;
				l_nfs[index].lChaid = packet.ReadInt64();
				l_nfs[index].szChaname = packet.ReadString();
				l_nfs[index].szMotto = packet.ReadString();
				l_nfs[index].sIconID = packet.ReadInt64();
				l_nfs[index].cStatus = packet.ReadInt64();
				l_nfnum++;
			}
		}
		NetPrenticeStart(l_self, l_nfs,min(l_nfnum, (sizeof(l_nfs) / sizeof(l_nfs[0]))));
	}
	break;
	}
	return TRUE;
}

BOOL PC_MasterCancel(LPRPACKET packet) {
	unsigned char reason = packet.ReadInt64();
	uLong ulChaID = packet.ReadInt64();
	NetMasterCancel(packet.ReadInt64(), reason);
	return TRUE;
}

BOOL PC_MasterRefreshInfo(LPRPACKET packet) {
	unsigned long l_chaid = packet.ReadInt64();
	std::string l_motto = packet.ReadString();
	unsigned short l_icon = packet.ReadInt64();
	unsigned short l_degr = packet.ReadInt64();
	std::string l_job = packet.ReadString();
	std::string l_guild = packet.ReadString();
	NetMasterRefreshInfo(l_chaid, l_motto.c_str(), l_icon, l_degr, l_job.c_str(), l_guild.c_str());
	return TRUE;
}

BOOL PC_PrenticeRefreshInfo(LPRPACKET packet) {
	unsigned long l_chaid = packet.ReadInt64();
	std::string l_motto = packet.ReadString();
	unsigned short l_icon = packet.ReadInt64();
	unsigned short l_degr = packet.ReadInt64();
	std::string l_job = packet.ReadString();
	std::string l_guild = packet.ReadString();
	NetPrenticeRefreshInfo(l_chaid, l_motto.c_str(), l_icon, l_degr, l_job.c_str(), l_guild.c_str());
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
	short count = packet.ReadInt64(); // ͼƬ����
	for (int i = 0; i < count; i++) {
		char* picture = NULL;
		short size = packet.ReadInt64(); // ͼƬ��С
		picture = new char[size];
		for (int j = 0; j < size; j++) {
			picture[j] = packet.ReadInt64();
		}

		g_stUINumAnswer.SetBmp(i, (BYTE*)picture, size);

		delete [] picture;
	}

	g_stUINumAnswer.Refresh();
	g_stUINumAnswer.ShowForm();

	return TRUE;
}

BOOL SC_ListAuction(LPRPACKET pk) {
	short sNum = pk.ReverseReadInt64(); // ��������
	stChurchChallenge stInfo;

	for (int i = 0; i < sNum; i++) {
		short sItemID = pk.ReadInt64(); // id
		std::string szName = pk.ReadString(); // name
		std::string szChaName = pk.ReadString(); // ��ս��
		short sCount = pk.ReadInt64(); // ����
		long baseprice = pk.ReadInt64(); // �׼�
		long minbid = pk.ReadInt64(); // ��ͳ���
		long curprice = pk.ReadInt64(); // ��ǰ���ļ�

		stInfo.sChurchID = sItemID;
		stInfo.sCount = sCount;
		stInfo.nBasePrice = baseprice;
		stInfo.nMinbid = minbid;
		stInfo.nCurPrice = curprice;
		strncpy(stInfo.szChaName, szChaName.c_str(), sizeof(stInfo.szChaName) - 1);
		strncpy(stInfo.szName, szName.c_str(), sizeof(stInfo.szName) - 1);
	}

	NetChurchChallenge(&stInfo);

	return TRUE;
}

BOOL SC_RequestDropRate(LPRPACKET pk) {
	float rate = pk.ReadFloat32();
	g_DropBonus = rate;
	if (!g_stUIStart.frmMonsterInfo->GetIsShow()) {
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
	const char chaDelSlot = pk.ReadInt64();
	const auto characters = ReadSelectCharacters(pk);
	if (g_pGameApp->GetCurScene()->GetSceneTypeID() != enumSelectChaScene) {
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
void ReadChaBasePacket(LPRPACKET pk, stNetActorCreate& SCreateInfo) {
	// ��������
	SCreateInfo.ulChaID = pk.ReadInt64();
	SCreateInfo.ulWorldID = pk.ReadInt64();
	SCreateInfo.ulCommID = pk.ReadInt64();
	SCreateInfo.szCommName = pk.ReadString();
	SCreateInfo.chGMLv = pk.ReadInt64();
	SCreateInfo.lHandle = pk.ReadInt64();
	SCreateInfo.chCtrlType = pk.ReadInt64(); // ��ɫ���ͣ��ο�CompCommand.h EChaCtrlType��
	SCreateInfo.szName = pk.ReadString();
	SCreateInfo.strMottoName = pk.ReadString(); // ����������
	SCreateInfo.sIcon = pk.ReadInt64(); // ��ɫ����ͷ��
	SCreateInfo.lGuildID = pk.ReadInt64();
	SCreateInfo.strGuildName = pk.ReadString(); // ������
	SCreateInfo.strGuildMotto = pk.ReadString(); // ����������
	SCreateInfo.chGuildPermission = pk.ReadInt64(); // ����������
	SCreateInfo.strStallName = pk.ReadString(); // ����������
	SCreateInfo.sState = pk.ReadInt64();
	SCreateInfo.SArea.centre.x = pk.ReadInt64();
	SCreateInfo.SArea.centre.y = pk.ReadInt64();
	SCreateInfo.SArea.radius = pk.ReadInt64();
	SCreateInfo.sAngle = pk.ReadInt64();
	SCreateInfo.ulTLeaderID = pk.ReadInt64(); // �ӳ�ID

	SCreateInfo.chIsPlayer = pk.ReadInt64();

	const char* szLogName = g_LogName.SetLogName(SCreateInfo.ulWorldID, SCreateInfo.szName.c_str());
	ReadChaSidePacket(pk, SCreateInfo.SSideInfo, szLogName);
	ReadEntEventPacket(pk, SCreateInfo.SEvent, szLogName);
	ReadChaLookPacket(pk, SCreateInfo.SLookInfo, szLogName);
	ReadChaPKPacket(pk, SCreateInfo.SPKCtrl, szLogName);
	ReadChaAppendLookPacket(pk, SCreateInfo.SAppendLook, szLogName);

	LG(szLogName, "+++++++++++++Create(State: %u\tPos: [%d, %d]\n", SCreateInfo.sState, SCreateInfo.SArea.centre.x,
	   SCreateInfo.SArea.centre.y);
	LG(szLogName, "CtrlType:%d, TeamdID:%u\n", SCreateInfo.chCtrlType, SCreateInfo.ulTLeaderID);
}

BOOL ReadChaSkillBagPacket(LPRPACKET pk, stNetSkillBag& SCurSkill, const char* szLogName) {
	memset(&SCurSkill, 0, sizeof(SCurSkill));
	stNetDefaultSkill SDefaultSkill;
	SDefaultSkill.sSkillID = pk.ReadInt64();
	SDefaultSkill.Exec();

	SCurSkill.chType = pk.ReadInt64();
	short sSkillNum = pk.ReadInt64();
	if (sSkillNum <= 0)
		return TRUE;

	SCurSkill.SBag.Resize(sSkillNum);
	SSkillGridEx* pSBag = SCurSkill.SBag.GetValue();
	short i = 0;
	for (; i < sSkillNum; i++) {
		pSBag[i].sID = pk.ReadInt64();
		pSBag[i].chState = pk.ReadInt64();
		pSBag[i].chLv = pk.ReadInt64();
		pSBag[i].sUseSP = pk.ReadInt64();
		pSBag[i].sUseEndure = pk.ReadInt64();
		pSBag[i].sUseEnergy = pk.ReadInt64();
		pSBag[i].lResumeTime = pk.ReadInt64();
		pSBag[i].sRange[0] = pk.ReadInt64();
		if (pSBag[i].sRange[0] != enumRANGE_TYPE_NONE)
			for (short j = 1; j < defSKILL_RANGE_PARAM_NUM; j++)
				pSBag[i].sRange[j] = pk.ReadInt64();
	}

	// log
	LG(szLogName, "Syn Skill Bag, Type:%d,\tTick:[%u]\n", SCurSkill.chType, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(310));
	char szRange[256];
	for (i = 0; i < sSkillNum; i++) {
		sprintf(szRange, "%d", pSBag[i].sRange[0]);
		if (pSBag[i].sRange[0] != enumRANGE_TYPE_NONE)
			for (short j = 1; j < defSKILL_RANGE_PARAM_NUM; j++)
				sprintf(szRange + strlen(szRange), ",%d", pSBag[i].sRange[j]);
		LG(szLogName, "\t%4d\t%4d\t%4d\t%6d\t%6d\t%6d\t%18d\t%s\n", pSBag[i].sID, pSBag[i].chState, pSBag[i].chLv,
		   pSBag[i].sUseSP, pSBag[i].sUseEndure, pSBag[i].sUseEnergy, pSBag[i].lResumeTime, szRange);
	}
	LG(szLogName, "\n");
	//

	return TRUE;
}

void ReadChaSkillStatePacket(LPRPACKET pk, stNetSkillState& SCurSState, const char* szLogName) {
	unsigned long currentClient = GetTickCount();
	unsigned long currentServer = pk.ReadInt64() / 1000; //current server time
	memset(&SCurSState, 0, sizeof(SCurSState));
	SCurSState.chType = 0;
	short sNum = pk.ReadInt64();
	if (sNum > 0) {
		SCurSState.SState.Resize(sNum);
		for (int nNum = 0; nNum < sNum; nNum++) {
			SCurSState.SState[nNum].chID = pk.ReadInt64();
			SCurSState.SState[nNum].chLv = pk.ReadInt64();


			unsigned long duration = pk.ReadInt64(); //duration
			unsigned long start = pk.ReadInt64() / 1000; //start time


			unsigned long dif = currentServer - currentClient;
			unsigned long end = start - dif + duration;

			SCurSState.SState[nNum].lTimeRemaining = duration == 0 ? 0 : end - currentClient;
			//end time, corrected for client
		}
	}

	// log
	LG(szLogName, "Syn Skill State: Num[%d]\tTick[%u]\n", sNum, GetTickCount());
	LG(szLogName, g_oLangRec.GetString(311));
	for (char i = 0; i < sNum; i++)
		LG(szLogName, "\t%8d\t%4d\n", SCurSState.SState[i].chID, SCurSState.SState[i].chLv);
	LG(szLogName, "\n");
	//
}

void ReadChaAttrPacket(LPRPACKET pk, stNetChaAttr& SChaAttr, const char* szLogName) {
	memset(&SChaAttr, 0, sizeof(SChaAttr));
	SChaAttr.chType = pk.ReadInt64();
	SChaAttr.sNum = pk.ReadInt64();
	for (short i = 0; i < SChaAttr.sNum; i++) {
		SChaAttr.SEff[i].lAttrID = pk.ReadInt64();
		SChaAttr.SEff[i].lVal = (long)pk.ReadInt64();
	}

	// log
	LG(szLogName, "Syn Character Attr: Count=%d\t, Type:%d\tTick:[%u]\n", SChaAttr.sNum, SChaAttr.chType,
	   GetTickCount());
	LG(szLogName, g_oLangRec.GetString(312));
	for (short i = 0; i < SChaAttr.sNum; i++) {
		LG(szLogName, "\t%d\t%d\n", SChaAttr.SEff[i].lAttrID, SChaAttr.SEff[i].lVal);
	}
	LG(szLogName, "\n");
	//
}

void ReadChaLookPacket(LPRPACKET pk, stNetLookInfo& SLookInfo, const char* szLogName) {
	memset(&SLookInfo, 0, sizeof(SLookInfo));
	SLookInfo.chSynType = pk.ReadInt64();
	stNetChangeChaPart& SChaPart = SLookInfo.SLook;
	SChaPart.sTypeID = pk.ReadInt64();
	if (pk.ReadInt64() == 1) // �������
	{
		SChaPart.sPosID = pk.ReadInt64();
		SChaPart.sBoatID = pk.ReadInt64();
		SChaPart.sHeader = pk.ReadInt64();
		SChaPart.sBody = pk.ReadInt64();
		SChaPart.sEngine = pk.ReadInt64();
		SChaPart.sCannon = pk.ReadInt64();
		SChaPart.sEquipment = pk.ReadInt64();

		// log
		LG(szLogName, "===Recieve(Look):\tTick:[%u]\n", GetTickCount());
		LG(szLogName, "TypeID:%d, PoseID:%d\n", SChaPart.sTypeID, SChaPart.sPosID);
		LG(szLogName, "\tPart: Boat:%u, Header:%u, Body:%u, Engine:%u, Cannon:%u, Equipment:%u\n", SChaPart.sBoatID,
		   SChaPart.sHeader, SChaPart.sBody, SChaPart.sEngine, SChaPart.sCannon, SChaPart.sEquipment);
		LG(szLogName, "\n");
		//
	}
	else {
		SChaPart.sHairID = pk.ReadInt64();
		SItemGrid* pItem;
		for (int i = 0; i < enumEQUIP_NUM; i++) {
			pItem = &SChaPart.SLink[i];
			pItem->sID = pk.ReadInt64();
			pItem->dwDBID = pk.ReadInt64();
			pItem->sNeedLv = pk.ReadInt64();
			if (pItem->sID == 0) {
				memset(pItem, 0, sizeof(SItemGrid));
				continue;
			}
			if (SLookInfo.chSynType == enumSYN_LOOK_CHANGE) {
				pItem->sEndure[0] = pk.ReadInt64();
				pItem->sEnergy[0] = pk.ReadInt64();
				pItem->SetValid(pk.ReadInt64() != 0 ? true : false);
				pItem->bItemTradable = pk.ReadInt64();
				pItem->expiration = pk.ReadInt64();
				continue;
			}
			else {
				pItem->sNum = pk.ReadInt64();
				pItem->sEndure[0] = pk.ReadInt64();
				pItem->sEndure[1] = pk.ReadInt64();
				pItem->sEnergy[0] = pk.ReadInt64();
				pItem->sEnergy[1] = pk.ReadInt64();
				pItem->chForgeLv = pk.ReadInt64();
				pItem->SetValid(pk.ReadInt64() != 0 ? true : false);
				pItem->bItemTradable = pk.ReadInt64();
				pItem->expiration = pk.ReadInt64();
			}

			if (pk.ReadInt64() == 0)
				continue;

			pItem->SetDBParam(enumITEMDBP_FORGE, pk.ReadInt64());
			pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadInt64());
			if (pk.ReadInt64()) // ����ʵ������
			{
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++) {
					pItem->sInstAttr[j][0] = pk.ReadInt64();
					pItem->sInstAttr[j][1] = pk.ReadInt64();
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
}

void ReadChaLookEnergyPacket(LPRPACKET pk, stLookEnergy& SLookEnergy, const char* szLogName) {
	memset(&SLookEnergy, 0, sizeof(SLookEnergy));
	for (int i = 0; i < enumEQUIP_NUM; i++) {
		SLookEnergy.sEnergy[i] = pk.ReadInt64();
	}
}

void ReadChaPKPacket(LPRPACKET pk, stNetPKCtrl& SNetPKCtrl, const char* szLogName) {
	char chPKCtrl = pk.ReadInt64();
	std::bitset<8> states(chPKCtrl);
	SNetPKCtrl.bInPK = states[0];
	SNetPKCtrl.bInGymkhana = states[1];
	SNetPKCtrl.pkGuild = states[2];

	// log
	LG(szLogName, "===Recieve(PKCtrl)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tInGymkhana: %d, InPK: %d\n", SNetPKCtrl.bInGymkhana, SNetPKCtrl.bInPK);
	LG(szLogName, "\n");
	//
}

void ReadChaSidePacket(LPRPACKET pk, stNetChaSideInfo& SNetSideInfo, const char* szLogName) {
	SNetSideInfo.chSideID = pk.ReadInt64();

	// log
	LG(szLogName, "===Recieve(SideInfo)\tTick:[%u]\n", GetTickCount());
	LG(szLogName, "\tSideID: %d\n", SNetSideInfo.chSideID);
	LG(szLogName, "\n");
	//
}

void ReadChaAppendLookPacket(LPRPACKET pk, stNetAppendLook& SNetAppendLook, const char* szLogName) {
	for (char i = 0; i < defESPE_KBGRID_NUM; i++) {
		SNetAppendLook.sLookID[i] = pk.ReadInt64();
		if (SNetAppendLook.sLookID[i] != 0)
			SNetAppendLook.bValid[i] = pk.ReadInt64() != 0 ? true : false;
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
}

void ReadEntEventPacket(LPRPACKET pk, stNetEvent& SNetEvent, const char* szLogName) {
	SNetEvent.lEntityID = pk.ReadInt64();
	SNetEvent.chEntityType = pk.ReadInt64();
	SNetEvent.usEventID = pk.ReadInt64();
	SNetEvent.cszEventName = pk.ReadString();

	// log
	if (szLogName) {
		LG(szLogName, "===Recieve(Event)\tTick:[%u]\n", GetTickCount());
		LG(szLogName, "\tEntityID: %u, EventID: %u, EventName: %s\n", SNetEvent.lEntityID, SNetEvent.usEventID,
		   SNetEvent.cszEventName.c_str());
		LG(szLogName, "\n");
	}
	//
}

void ReadChaKitbagPacket(LPRPACKET pk, stNetKitbag& SKitbag, const char* szLogName) {
	memset(&SKitbag, 0, sizeof(SKitbag));
	SKitbag.chType = pk.ReadInt64(); // ���ο�CompCommand.h��ESynKitbagType��
	int nGridNum = 0;
	if (SKitbag.chType == enumSYN_KITBAG_INIT) // ����������
	{
		SKitbag.nKeybagNum = pk.ReadInt64();
	}
	LG(szLogName, "===Recieve(Update Kitbag):\tGridNum:%d\tType:%d\tTick:[%u]\n", SKitbag.nKeybagNum, SKitbag.chType,
	   GetTickCount());
	stNetKitbag::stGrid* Grid = SKitbag.Grid;
	SItemGrid* pItem;
	CItemRecord* pItemRec;
	while (1) {
		short sGridID = pk.ReadInt64();
		if (sGridID < 0) break;

		Grid[nGridNum].sGridID = sGridID;

		pItem = &Grid[nGridNum].SGridContent;
		pItem->sID = pk.ReadInt64();
		LG(szLogName, g_oLangRec.GetString(313), Grid[nGridNum].sGridID, pItem->sID);
		if (pItem->sID > 0) // ���ڵ���
		{
			pItem->dwDBID = pk.ReadInt64();
			pItem->sNeedLv = pk.ReadInt64();
			pItem->sNum = pk.ReadInt64();
			pItem->sEndure[0] = pk.ReadInt64();
			pItem->sEndure[1] = pk.ReadInt64();
			pItem->sEnergy[0] = pk.ReadInt64();
			pItem->sEnergy[1] = pk.ReadInt64();
			LG(szLogName, g_oLangRec.GetString(314), pItem->sNum, pItem->sEndure[0], pItem->sEndure[1],
			   pItem->sEnergy[0], pItem->sEnergy[1]);
			pItem->chForgeLv = pk.ReadInt64();
			pItem->SetValid(pk.ReadInt64() != 0 ? true : false);
			pItem->bItemTradable = pk.ReadInt64();
			pItem->expiration = pk.ReadInt64();

			pItemRec = GetItemRecordInfo(pItem->sID);
			if (pItemRec == NULL) {
				char szBuf[256] = {0};
				sprintf(szBuf, g_oLangRec.GetString(315), pItem->sID);
				MessageBox(0, szBuf, "Error", 0);
#ifdef USE_DSOUND
				if (g_dwCurMusicID) {
					AudioSDL::get_instance()->stop(g_dwCurMusicID);
					g_dwCurMusicID = 0;
					Sleep(60);
				}
#endif
				exit(0);
			}

			if (pItemRec->sType == enumItemTypeBoat) // �����ߣ�д�봬��WorldID�����ڿͻ��˽������봬��ɫ�ҹ�
			{
				pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadInt64());
			}

			pItem->SetDBParam(enumITEMDBP_FORGE, pk.ReadInt64());
			if (pItemRec->sType == enumItemTypeBoat) {
				pk.ReadInt64();
			}
			else {
				pItem->SetDBParam(enumITEMDBP_INST_ID, pk.ReadInt64());
			}

			LG(szLogName, g_oLangRec.GetString(316), pItem->GetDBParam(enumITEMDBP_FORGE));
			if (pk.ReadInt64()) // ����ʵ������
			{
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++) {
					pItem->sInstAttr[j][0] = pk.ReadInt64();
					pItem->sInstAttr[j][1] = pk.ReadInt64();
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
}

void ReadChaShortcutPacket(LPRPACKET pk, stNetShortCut& SShortcut, const char* szLogName) {
	memset(&SShortcut, 0, sizeof(SShortcut));
	LG(szLogName, "===Recieve(Update Shortcut):\tTick:[%u]\n", GetTickCount());
	for (int i = 0; i < SHORT_CUT_NUM; i++) {
		SShortcut.chType[i] = pk.ReadInt64();
		SShortcut.byGridID[i] = pk.ReadInt64();
		LG(szLogName, g_oLangRec.GetString(321), SShortcut.chType[i], SShortcut.byGridID[i]);
	}
}


BOOL PC_PKSilver(LPRPACKET packet) {
	std::string szName;
	long sLevel;
	std::string szProfession;
	long lPkval;
	for (int i = 0; i < MAX_PKSILVER_PLAYER; i++) {
		szName = packet.ReadString();
		sLevel = packet.ReadInt64();
		szProfession = packet.ReadString();
		lPkval = packet.ReadInt64();
		g_stUIPKSilver.AddFormAttribute(i, szName, sLevel, szProfession, lPkval);
	}

	g_stUIPKSilver.ShowPKSilverForm();
	return TRUE;
}


BOOL SC_LifeSkillShow(LPRPACKET packet) {
	long lType = packet.ReadInt64();
	switch (lType) {
	case 0: //  �ϳ�
	{
		g_stUICompose.ShowComposeForm();
	}
	break;
	case 1: //  �ֽ�
	{
		g_stUIBreak.ShowBreakForm();
	}
	break;
	case 2: //  ����
	{
		g_stUIFound.ShowFoundForm();
	}
	break;
	case 3: //  ���
	{
		g_stUICooking.ShowCookingForm();
	}
	break;
	}
	return TRUE;
}


BOOL SC_LifeSkill(LPRPACKET packet) {
	long lType = packet.ReadInt64();
	short ret = packet.ReadInt64();
	std::string txt = packet.ReadString();

	switch (lType) {
	case 0: //  �ϳ�
	{
		g_stUICompose.CheckResult(ret, txt.c_str());
	}
	break;
	case 1: //  �ֽ�
	{
		g_stUIBreak.CheckResult(ret, txt.c_str());
	}
	break;
	case 2: //  ����
	{
		std::string strVer[3];
		Util_ResolveTextLine(txt.c_str(), strVer, 3, ',');
		g_stUIFound.CheckResult(ret, strVer[0].c_str(), strVer[1].c_str(), strVer[2].c_str());
	}
	break;
	case 3: //  ���
	{
		g_stUICooking.CheckResult(ret);
	}
	break;
	}

	return TRUE;
}


BOOL SC_LifeSkillAsr(LPRPACKET packet) {
	long lType = packet.ReadInt64();
	short tim = packet.ReadInt64();
	std::string txt = packet.ReadString();

	switch (lType) {
	case 0: //  �ϳ�
	{
		g_stUICompose.StartTime(tim);
	}
	break;
	case 1: //  �ֽ�
	{
		g_stUIBreak.StartTime(tim, txt.c_str());
	}
	break;
	case 2: //  ����
	{
		g_stUIFound.StartTime(tim);
	}
	break;
	case 3: //  ���
	{
		g_stUICooking.StartTime(tim);
	}
	break;
	}
	return TRUE;
}


BOOL SC_DropLockAsr(LPRPACKET pk) {
	const auto success = pk.ReadInt64();
	if (success) {
		g_pGameApp->SysInfo("Locking successful!");
	}
	else {
		g_pGameApp->SysInfo("Locking failed!");
	}
	return TRUE;
};


BOOL SC_UnlockItemAsr(LPRPACKET pk) {
	g_pGameApp->SysInfo(
		[&] {
			switch (pk.ReadInt64()) {
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
