#include "StdAfx.h"
#include "procirculate.h"
#include "GameApp.h"
#include "PacketCmd.h"
#include "Character.h"
#include "ChaAttr.h"
#include "GameConfig.h"
#include "util2.h"
#include "LoginScene.h"
#include "netprotocol.h"
#include "AreaRecord.h"
#include "CommandMessages.h"

using namespace std;
#include "CryptoUtils.h"
#ifdef _TEST_CLIENT
#include "..\..\TestClient\testclient.h"
#endif

//  uChar, uShort, uLong, cChar   NetIF.h

//  :  CMD_CM_BEGINACTION + switch   
void CProCirculateCS::BeginAction(CCharacter* pCha, DWORD type, void* param, CActionState* pState) {
	auto pk = net::msg::serializeCmBeginActionHeader(
		static_cast<int64_t>(pCha->getAttachID()),
		static_cast<int64_t>(pCNetIf->m_ulPacketCount),
		static_cast<int64_t>(type));

	switch (type) {
		case enumACTION_MOVE: {
			stNetMoveInfo* pMove = (stNetMoveInfo*)param;
			pk.WriteSequence((cChar*)pMove->pos_buf, uShort(sizeof(Point) * pMove->pos_num));
			pCNetIf->SendPacketMessage(pk);

			CCharacter* pCha = CGameScene::GetMainCha();
			CGameScene* pScene = g_pGameApp->GetCurScene();
			if (!pCha->IsBoat()) {
				int nArea = pScene->_pTerrain->GetTile(pCha->GetCurX() / 100, pCha->GetCurY() / 100)->getIsland();
				CAreaInfo* pArea = GetAreaInfo(nArea);

				const std::string buf = std::format("In {}", pArea->DataName);
				const std::string buffer = std::format("{} Lv{} {}", pCha->getHumanName(), pCha->getLv(),
						g_GetJobName((short)pCha->getGameAttr()->get(ATTR_JOB)));
				if (pCha->GetTeamLeaderID() > 0) {
				}
				else {
					updateDiscordPresence(buffer.c_str(), buf.c_str());
				}
			}
			else {
				const std::string buffer = std::format("{} Lv{} {}", pCha->getHumanName(), pCha->getLv(),
						g_GetJobName((short)pCha->getGameAttr()->get(ATTR_JOB)));
				updateDiscordPresence(buffer.c_str(), "Sailing");
			}

			break;
		}
		case enumACTION_SKILL: {
			stNetSkillInfo* pSkill = (stNetSkillInfo*)param;
			pk.WriteInt64(pSkill->chMove);
			pk.WriteInt64(pSkill->byFightID);
			if (pSkill->chMove == 2) {
				pk.WriteSequence((cChar*)pSkill->SMove.pos_buf, uShort(sizeof(POINT) * pSkill->SMove.pos_num));
			}
			pk.WriteInt64(pSkill->lSkillID);
			pk.WriteInt64(pSkill->lTarInfo1);
			pk.WriteInt64(pSkill->lTarInfo2);

			//for (int n = 0; n < 200; ++n)
			//{
			pCNetIf->SendPacketMessage(pk);
			//}

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Skill):\tTick:[{}]", GetTickCount()));
			if (pSkill->chMove == 2) {
				g_logManager.InternalLog(LogLevel::Debug, "common",
										 std::format("Ping:\t{:3}", pSkill->SMove.dwAveragePing));
				g_logManager.InternalLog(LogLevel::Debug, "common", std::format("Point:\t{:3}", pSkill->SMove.pos_num));
				for (DWORD i = 0; i < pSkill->SMove.pos_num; i++) {
					g_logManager.InternalLog(LogLevel::Debug, "common",
											 std::format("\t{}, \t{}", pSkill->SMove.pos_buf[i].x,
														 pSkill->SMove.pos_buf[i].y));
				}
			}
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("Skill:\t{:3}, FightID:{}", pSkill->lSkillID, pSkill->byFightID));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("Target:\t{}, \t{}", pSkill->lTarInfo1, pSkill->lTarInfo2));

			break;
		}
		case enumACTION_STOP_STATE: {
			pk.WriteInt64(*((short*)param));
			pCNetIf->SendPacketMessage(pk);

			// log
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Stop Skill State {}):\tTick:[{}]", *((short*)param),
												 GetTickCount()));

			//
			break;
		}
		case enumACTION_LEAN: // 
		{
			stNetLeanInfo* pSLean = (stNetLeanInfo*)param;
			pk.WriteInt64(pSLean->lPose);
			pk.WriteInt64(pSLean->lAngle);
			pk.WriteInt64(pSLean->lPosX);
			pk.WriteInt64(pSLean->lPosY);
			pk.WriteInt64(pSLean->lHeight);
			pCNetIf->SendPacketMessage(pk);

			// log
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Lean):\tTick:[{}]", GetTickCount()));

			//
			break;
		}
		case enumACTION_ITEM_PICK: // 
		{
			stNetItemPick* pPick = (stNetItemPick*)param;
			pk.WriteInt64(pPick->lWorldID);
			pk.WriteInt64(pPick->lHandle);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Pick):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_ITEM_THROW: // 
		{
			stNetItemThrow* pThrow = (stNetItemThrow*)param;
			pk.WriteInt64(pThrow->sGridID);
			pk.WriteInt64((short)pThrow->lNum);
			pk.WriteInt64(pThrow->lPosX);
			pk.WriteInt64(pThrow->lPosY);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Throw):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_ITEM_USE: {
			stNetUseItem* pUseItem = (stNetUseItem*)param;
			pk.WriteInt64(pUseItem->sGridID);
			pk.WriteInt64(pUseItem->sTarGridID);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Use Item):\tTick:[{}]", GetTickCount()));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 SafeVFormat(GetLanguageString(322), pUseItem->sGridID, pUseItem->sTarGridID));

			break;
		}
		case enumACTION_ITEM_UNFIX: // 
		{
			stNetItemUnfix* pUnfix = (stNetItemUnfix*)param;
			pk.WriteInt64(pUnfix->chLinkID);
			pk.WriteInt64(pUnfix->sGridID);
			if (pUnfix->sGridID < 0) // 
			{
				pk.WriteInt64(pUnfix->lPosX);
				pk.WriteInt64(pUnfix->lPosY);
			}
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Unfix):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_ITEM_POS: {
			stNetItemPos* pChangePos = (stNetItemPos*)param;
			pk.WriteInt64(pChangePos->sSrcGridID);
			pk.WriteInt64(pChangePos->sSrcNum);
			pk.WriteInt64(pChangePos->sTarGridID);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Item pos):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_ITEM_DELETE: {
			stNetDelItem* pDelItem = (stNetDelItem*)param;
			pk.WriteInt64(pDelItem->sGridID);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Del Item):\tTick:[{}]", GetTickCount()));
			g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(323), pDelItem->sGridID));

			break;
		}
		case enumACTION_ITEM_INFO: {
			stNetItemInfo* pItemInfo = (stNetItemInfo*)param;
			pk.WriteInt64(pItemInfo->chType);
			pk.WriteInt64(pItemInfo->sGridID);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Item Info):\tTick:[{}]", GetTickCount()));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 SafeVFormat(GetLanguageString(324), pItemInfo->chType, pItemInfo->sGridID));

			break;
			break;
		}
		case enumACTION_SHORTCUT: // 
		{
			stNetShortCutChange* pShortcutChange = (stNetShortCutChange*)param;
			pk.WriteInt64(pShortcutChange->chIndex);
			pk.WriteInt64(pShortcutChange->chType);
			pk.WriteInt64(pShortcutChange->shyGrid);
			//pk.WriteByte(pShortcutChange->shyGrid2==-1?0:1);
			//pk.WriteInt64(pShortcutChange->shyGrid2);
			pCNetIf->SendPacketMessage(pk);
			break;
		}
		case enumACTION_LOOK: // 
		{
			stNetChangeChaPart* pSChaPart = (stNetChangeChaPart*)param;
			pk.WriteInt64(pSChaPart->sTypeID);
			for (int i = 0; i < enumEQUIP_NUM; i++) {
				pk.WriteInt64(pSChaPart->SLink[i].sID);
			}
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Look):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_TEMP: //
		{
			stTempChangeChaPart* pSTempChaPart = (stTempChangeChaPart*)param;
			pk.WriteInt64(pSTempChaPart->dwItemID);
			pk.WriteInt64(pSTempChaPart->dwPartID);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Temp):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_EVENT: // 
		{
			stNetActivateEvent* pEvent = (stNetActivateEvent*)param;
			pk.WriteInt64(pEvent->lTargetID);
			pk.WriteInt64(pEvent->lHandle);
			pk.WriteInt64(pEvent->sEventID);
			pCNetIf->SendPacketMessage(pk);

			// log
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Event):\tTick:[{}]", GetTickCount()));

			//
			break;
		}
		case enumACTION_FACE: {
			stNetFace* pNetFace = (stNetFace*)param;
			pk.WriteInt64(pNetFace->sAngle);
			pk.WriteInt64(pNetFace->sPose);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Face):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_SKILL_POSE: {
			stNetFace* pNetFace = (stNetFace*)param;
			pk.WriteInt64(pNetFace->sAngle);
			pk.WriteInt64(pNetFace->sPose);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Skill Pos):\tTick:[{}]", GetTickCount()));
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("Angle:{}, Pose:{}", pNetFace->sAngle, pNetFace->sPose));
			break;
		}
		case enumACTION_GUILDBANK:
		case enumACTION_BANK: {
			stNetBank* pNetBank = (stNetBank*)param;
			pk.WriteInt64(pNetBank->chSrcType);
			pk.WriteInt64(pNetBank->sSrcID);
			pk.WriteInt64(pNetBank->sSrcNum);
			pk.WriteInt64(pNetBank->chTarType);
			pk.WriteInt64(pNetBank->sTarID);
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Bank Req):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_CLOSE_BANK: {
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(Bank Close):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_REQUESTGUILDLOGS: {
			uShort* curSize = reinterpret_cast<uShort*>(param);
			pk.WriteInt64(*curSize);

			pCNetIf->SendPacketMessage(pk);
			break;
		}
		case enumACTION_UPDATEGUILDLOGS: {
			pCNetIf->SendPacketMessage(pk);
			break;
		}

		case enumACTION_REQUESTGUILDBANK: {
			pCNetIf->SendPacketMessage(pk);

			g_logManager.InternalLog(LogLevel::Debug, "common",
									 std::format("###Send(request guild):\tTick:[{}]", GetTickCount()));

			break;
		}
		case enumACTION_KITBAGTMP_DRAG: // 
		{
			stNetTempKitbag* pNetTempKitbag = (stNetTempKitbag*)param;

			pk.WriteInt64(pNetTempKitbag->sSrcGridID);
			pk.WriteInt64(pNetTempKitbag->sSrcNum);
			pk.WriteInt64(pNetTempKitbag->sTarGridID);

			pCNetIf->SendPacketMessage(pk);
			break;
		}
		default:
			break;
	}
}

// C->S : 
void CProCirculateCS::EndAction(CActionState* pState) {
	//   
	auto pk = net::msg::serializeCmEndActionCmd();
	pCNetIf->SendPacketMessage(pk);

	CCharacter* pMainCha = CGameApp::GetCurScene()->GetMainCha();
	if (pMainCha) {
		g_logManager.InternalLog(LogLevel::Debug, "common",
								 std::format("###Send(EndAction):\tTick:[{}]", GetTickCount()));
	}
}

bool CProCirculate::Connect(const char* hostname, unsigned short port, unsigned long timeout) {
	return pCNetIf->m_connect.Connect(hostname, port, timeout);
}

void CProCirculate::Disconnect(int reason) {
	pCNetIf->m_connect.Disconnect(reason);
}

bool CProCirculate::SendPrivateKey() {
	// 1.  32- AES-256 
	NTSTATUS status = BCryptGenRandom(NULL, g_NetIF->cliAesKey, 32, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (!BCRYPT_SUCCESS(status)) {
		ToLogService("connections", "BCryptGenRandom (AES key) failed: 0x{:08X}", status);
		return false;
	}

	// 2. RSA-OAEP-SHA256  AES    
	BCRYPT_OAEP_PADDING_INFO oaepInfo = {};
	oaepInfo.pszAlgId = BCRYPT_SHA256_ALGORITHM;
	oaepInfo.pbLabel = NULL;
	oaepInfo.cbLabel = 0;

	//    
	ULONG encryptedLen = 0;
	status = BCryptEncrypt(
		g_NetIF->hRsaPubKey,
		g_NetIF->cliAesKey, 32,
		&oaepInfo,
		NULL, 0,
		NULL, 0, &encryptedLen,
		BCRYPT_PAD_OAEP);
	if (!BCRYPT_SUCCESS(status)) {
		ToLogService("connections", "BCryptEncrypt (RSA size query) failed: 0x{:08X}", status);
		return false;
	}

	// 
	std::vector<BYTE> encryptedKey(encryptedLen);
	ULONG resultLen = 0;
	status = BCryptEncrypt(
		g_NetIF->hRsaPubKey,
		g_NetIF->cliAesKey, 32,
		&oaepInfo,
		NULL, 0,
		encryptedKey.data(), encryptedLen, &resultLen,
		BCRYPT_PAD_OAEP);
	if (!BCRYPT_SUCCESS(status)) {
		ToLogService("connections", "BCryptEncrypt (RSA encrypt) failed: 0x{:08X}", status);
		return false;
	}

	//  AES    
	{
		std::string aesHex, encHex;
		aesHex.reserve(64);
		encHex.reserve(resultLen * 2);
		for (int i = 0; i < 32; i++) {
			aesHex += std::format("{:02x}", g_NetIF->cliAesKey[i]);
		}
		for (ULONG i = 0; i < resultLen; i++) {
			encHex += std::format("{:02x}", encryptedKey[i]);
		}
		ToLogService("connections", "SendPrivateKey: AES-256 key (32 bytes):\n{}", aesHex.c_str());
		ToLogService("connections", "SendPrivateKey: RSA-encrypted key ({} bytes):\n{}", resultLen, encHex.c_str());
	}

	// 3.  BCrypt AES    
	if (!g_NetIF->InitAesKey()) {
		ToLogService("connections", "InitAesKey failed");
		return false;
	}

	// 4.   AES   ( ,  Base64)
	// NOTE: WriteSequence        net::msg::serialize
	WPacket pk = pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SEND_PRIVATE_KEY);
	pk.WriteSequence(reinterpret_cast<const char*>(encryptedKey.data()), static_cast<uShort>(resultLen));

	pCNetIf->SendPacketMessage(pk);
	g_NetIF->handshakeDone = true;
	g_NetIF->_comm_enc = true;
	g_NetIF->m_connect.CHAPSTR(false);
	return true;
}

void CProCirculate::Login(const char* accounts, const char* password, const char* passport) {
	if (!g_NetIF->handshakeDone) {
		return;
	}

	extern short g_sClientVer;

	string strMac = GetMacString();
	if (strMac.empty()) strMac = "Unknown";

	net::msg::CmLoginRequest msg;
	msg.acctName = accounts;
	msg.passwordHash = HashPassword(password);
	msg.mac = strMac;
	msg.cheatMarker = 911;
	msg.clientVersion = g_sClientVer;

	WPacket pk = net::msg::serialize(msg);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::Logout() {
	//      
	auto pk = net::msg::serializeCmLogoutCmd();
	pCNetIf->SendPacketMessage(pk);
	Sleep(1000); //     logout
}

void CProCirculate::BeginPlay(char cha_index) {
	//    
	auto pk = net::msg::serialize(net::msg::CmBgnPlayMessage{(int64_t)cha_index});
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::EndPlay() {
	//     
	auto pk = net::msg::serializeCmEndPlayCmd();
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::NewCha(const char* chaname, const char* birth, int type, int hair, int face) {
	//   
	auto pk = net::msg::serialize(
		net::msg::CmNewChaMessage{chaname, birth, (int64_t)type, (int64_t)hair, (int64_t)face});
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::DelCha(uint8_t cha_index, const char szPassword2[]) {
	//  
	auto pk = net::msg::serialize(net::msg::CmDelChaMessage{(int64_t)cha_index, szPassword2});
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::OpenRankings() {
	//  
	auto pk = net::msg::serializeCmRankCmd();
	pCNetIf->SendPacketMessage(pk);
}

//  :   (CMD_CM_SAY)
void CProCirculate::Say(const char* content) {
	auto pk = net::msg::serialize(net::msg::CmSayMessage{content});
	pCNetIf->SendPacketMessage(pk);
}

//  :    (CMD_CM_SYNATTR)
void CProCirculate::SynBaseAttribute(CChaAttr* pCAttr) {
	net::msg::CmSynAttrMessage msg;
	for (int i = ATTR_STR; i <= ATTR_LUK; i++) {
		if (pCAttr->GetChangeBitFlag(i))
			msg.attrs.push_back({(int64_t)i, (int64_t)pCAttr->GetAttr(i)});
	}

	if (msg.attrs.empty())
		return;

	auto pk = net::msg::serialize(msg);

	// log
	char szReqChangeAttr[256] = {0};
	strncpy_s(szReqChangeAttr, sizeof(szReqChangeAttr), GetLanguageString(325).c_str(), _TRUNCATE);

	for (int i = ATTR_STR; i <= ATTR_LUK; i++) {
		if (pCAttr->GetChangeBitFlag(i))
			g_logManager.InternalLog(LogLevel::Debug, "common",
									 SafeVFormat(GetLanguageString(326), i, pCAttr->GetAttr(i)));
	}

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::RefreshChaData(long lWorldID, long lHandle) {
	//    
	auto pk = net::msg::serialize(net::msg::CmRefreshDataMessage{(int64_t)lWorldID, (int64_t)lHandle});
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::SkillUpgrade(short sSkillID, char chAddLv) {
	//  
	auto pk = net::msg::serialize(net::msg::CmSkillUpgradeMessage{(int64_t)sSkillID, (int64_t)chAddLv});

	g_logManager.InternalLog(LogLevel::Debug, "common", SafeVFormat(GetLanguageString(328), sSkillID, chAddLv));

	pCNetIf->SendPacketMessage(pk);
}
