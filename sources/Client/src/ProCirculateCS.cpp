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

using namespace std;
// Crypto++ — BLAKE2s для хеширования пароля при логине
#include "blake2.h"
#include "hex.h"
#include "filters.h"
#ifdef _TEST_CLIENT
#include "..\..\TestClient\testclient.h"
#endif

// Типы uChar, uShort, uLong, cChar определены в NetIF.h


void CProCirculateCS::BeginAction( CCharacter* pCha, DWORD type, void* param, CActionState* pState )
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_BEGINACTION);
	pk.WriteUInt32(pCha->getAttachID());

    char	szLogName[1024] = { "BeginAction" };

    if( pCha )
    {
        strcpy(szLogName, pCha->getLogName() );            
    }

	try
	{
		LG(szLogName, "$$$PacketID:\t%d\n", pCNetIf->m_ulPacketCount);

#ifdef defPROTOCOL_HAVE_PACKETID
		pk.WriteUInt32(pCNetIf->m_ulPacketCount++);
#endif
		pk.WriteUInt8((uint8_t)type);
		switch (type)
		{
		case	enumACTION_MOVE:
			{
				
				stNetMoveInfo *pMove = (stNetMoveInfo *)param;
				pk.WriteSequence((cChar *)pMove->pos_buf ,uShort(sizeof(Point) * pMove->pos_num));
				pCNetIf->SendPacketMessage(pk);


				char buffer[64] = {0};
				char buf[64] = {0};
				CCharacter	*pCha = CGameScene::GetMainCha();
				CGameScene	*pScene = g_pGameApp->GetCurScene();
				if(!pCha->IsBoat()) {
				int nArea = pScene->_pTerrain->GetTile(pCha->GetCurX()/100, pCha->GetCurY()/100 )->getIsland();
				CAreaInfo* pArea = GetAreaInfo( nArea );


				sprintf(buf, "In %s",pArea->szDataName);

				sprintf(buffer, "%s Lv%d %s", pCha->getHumanName(), pCha->getLv(), g_GetJobName((short)pCha->getGameAttr()->get(ATTR_JOB)));
				if(pCha->GetTeamLeaderID() > 0) {
				}else{

				updateDiscordPresence(buffer, buf );

				}
				}else {
					sprintf(buffer, "%s Lv%d %s", pCha->getHumanName(), pCha->getLv(), g_GetJobName((short)pCha->getGameAttr()->get(ATTR_JOB)));
					sprintf(buf, "Sailing");
					updateDiscordPresence(buffer, buf );
				}
				
				




				LG(szLogName, "###Send(Move):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "Ping:\t%3d\n", pMove->dwAveragePing);
				LG(szLogName, "Point:\t%3d\n", pMove->pos_num);
				for (DWORD i = 0; i < pMove->pos_num; i++)
				{
					LG(szLogName, "\t%d, \t%d\n", pMove->pos_buf[i].x, pMove->pos_buf[i].y);
				}
				LG(szLogName, "\n");
				break;
			}
		case	enumACTION_SKILL:
			{
				stNetSkillInfo *pSkill = (stNetSkillInfo *)param;
				pk.WriteUInt8(pSkill->chMove);
				pk.WriteUInt8(pSkill->byFightID);
				if (pSkill->chMove == 2)
				{
					pk.WriteSequence((cChar *)pSkill->SMove.pos_buf, uShort(sizeof(POINT) * pSkill->SMove.pos_num));
				}
				pk.WriteUInt32(pSkill->lSkillID);
				pk.WriteUInt32(pSkill->lTarInfo1);
				pk.WriteUInt32(pSkill->lTarInfo2);

				//for (int n = 0; n < 200; ++n)
				//{
					pCNetIf->SendPacketMessage(pk);
				//}

				LG(szLogName, "###Send(Skill):\tTick:[%d]\n", GetTickCount());
				if (pSkill->chMove == 2)
				{
					LG(szLogName, "Ping:\t%3d\n", pSkill->SMove.dwAveragePing);
					LG(szLogName, "Point:\t%3d\n", pSkill->SMove.pos_num);
					for (DWORD i = 0; i < pSkill->SMove.pos_num; i++)
					{
						LG(szLogName, "\t%d, \t%d\n", pSkill->SMove.pos_buf[i].x, pSkill->SMove.pos_buf[i].y);
					}
				}
				LG(szLogName, "Skill:\t%3d, FightID:%d\n", pSkill->lSkillID, pSkill->byFightID);
				LG(szLogName, "Target:\t%u, \t%u\n", pSkill->lTarInfo1, pSkill->lTarInfo2);
				LG(szLogName, "\n");
				break;
			}
		case	enumACTION_STOP_STATE:
			{
				pk.WriteUInt16(*((short *)param));
				pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Stop Skill State %d):\tTick:[%d]\n", *((short *)param), GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_LEAN: // �п�
			{
				stNetLeanInfo *pSLean = (stNetLeanInfo *)param;
				pk.WriteUInt32(pSLean->lPose);
				pk.WriteUInt32(pSLean->lAngle);
				pk.WriteUInt32(pSLean->lPosX);
				pk.WriteUInt32(pSLean->lPosY);
				pk.WriteUInt32(pSLean->lHeight);
				pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Lean):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_ITEM_PICK: // �����
			{
				stNetItemPick *pPick = (stNetItemPick *)param;
				pk.WriteUInt32(pPick->lWorldID);
				pk.WriteUInt32(pPick->lHandle);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Pick):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_THROW: // ������
			{
				stNetItemThrow *pThrow = (stNetItemThrow *)param;
				pk.WriteUInt16(pThrow->sGridID);
				pk.WriteUInt16((short)pThrow->lNum);
				pk.WriteUInt32(pThrow->lPosX);
				pk.WriteUInt32(pThrow->lPosY);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Throw):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_USE:
			{
				stNetUseItem *pUseItem = (stNetUseItem *)param;
				pk.WriteUInt16(pUseItem->sGridID);
				pk.WriteUInt16(pUseItem->sTarGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Use Item):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, g_oLangRec.GetString(322), pUseItem->sGridID, pUseItem->sTarGridID);
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_UNFIX: // жװ����
			{
				stNetItemUnfix *pUnfix = (stNetItemUnfix *)param;
				pk.WriteUInt8(pUnfix->chLinkID);
				pk.WriteUInt16(pUnfix->sGridID);
				if (pUnfix->sGridID < 0) // ��������
				{
					pk.WriteUInt32(pUnfix->lPosX);
					pk.WriteUInt32(pUnfix->lPosY);
				}
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Unfix):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_POS:
			{
				stNetItemPos *pChangePos = (stNetItemPos *)param;
				pk.WriteUInt16(pChangePos->sSrcGridID);
				pk.WriteUInt16(pChangePos->sSrcNum);
				pk.WriteUInt16(pChangePos->sTarGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Item pos):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_DELETE:
			{
				stNetDelItem *pDelItem = (stNetDelItem *)param;
				pk.WriteUInt16(pDelItem->sGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Del Item):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, g_oLangRec.GetString(323), pDelItem->sGridID);
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_INFO:
			{
				stNetItemInfo *pItemInfo = (stNetItemInfo *)param;
				pk.WriteUInt8(pItemInfo->chType);
				pk.WriteUInt16(pItemInfo->sGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Item Info):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, g_oLangRec.GetString(324), pItemInfo->chType, pItemInfo->sGridID);
                LG(szLogName, "\n");
				break;
				break;
			}
		case	enumACTION_SHORTCUT: // ���¿����
			{
				stNetShortCutChange *pShortcutChange = (stNetShortCutChange *)param;
				pk.WriteUInt8(pShortcutChange->chIndex);
				pk.WriteUInt8(pShortcutChange->chType);
				pk.WriteUInt16(pShortcutChange->shyGrid);
				//pk.WriteByte(pShortcutChange->shyGrid2==-1?0:1);
				//pk.WriteUInt16(pShortcutChange->shyGrid2);
				pCNetIf->SendPacketMessage(pk);
				break;
			}
		case	enumACTION_LOOK: // ������ۣ��紬�Ļ�װ��
			{
				stNetChangeChaPart *pSChaPart = (stNetChangeChaPart *)param;
				pk.WriteUInt16(pSChaPart->sTypeID);
				for (int i = 0; i < enumEQUIP_NUM; i++)
				{
					pk.WriteUInt16(pSChaPart->SLink[i].sID);
				}
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Look):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_TEMP: //��ʱ��װЭ��
			{
				stTempChangeChaPart *pSTempChaPart = (stTempChangeChaPart *)param;
				pk.WriteUInt32(pSTempChaPart->dwItemID);
				pk.WriteUInt32(pSTempChaPart->dwPartID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Temp):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_EVENT: // �����¼�
			{
				stNetActivateEvent *pEvent = (stNetActivateEvent *)param;
				pk.WriteUInt32(pEvent->lTargetID);
				pk.WriteUInt32(pEvent->lHandle);
				pk.WriteUInt16(pEvent->sEventID);
                pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Event):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_FACE:
			{
				stNetFace	*pNetFace = (stNetFace *)param;
				pk.WriteUInt16(pNetFace->sAngle);
				pk.WriteUInt16(pNetFace->sPose);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Face):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_SKILL_POSE:
			{
				stNetFace	*pNetFace = (stNetFace *)param;
				pk.WriteUInt16(pNetFace->sAngle);
				pk.WriteUInt16(pNetFace->sPose);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Skill Pos):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "Angle:%d, Pose:%d\n", pNetFace->sAngle, pNetFace->sPose );
				break;
			}
		case enumACTION_GUILDBANK:
		case	enumACTION_BANK:
			{
				stNetBank	*pNetBank = (stNetBank *)param;
				pk.WriteUInt8(pNetBank->chSrcType);
				pk.WriteUInt16(pNetBank->sSrcID);
				pk.WriteUInt16(pNetBank->sSrcNum);
				pk.WriteUInt8(pNetBank->chTarType);
				pk.WriteUInt16(pNetBank->sTarID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Bank Req):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_CLOSE_BANK:
			{
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Bank Close):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_REQUESTGUILDLOGS:
			{
				uShort* curSize = reinterpret_cast<uShort*> (param);
				pk.WriteUInt16(*curSize);

				pCNetIf->SendPacketMessage(pk);
				break;
			}	
		case	enumACTION_UPDATEGUILDLOGS:
			{
				pCNetIf->SendPacketMessage(pk);
				break;
			}

		
		case	enumACTION_REQUESTGUILDBANK:
			{
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(request guild):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_KITBAGTMP_DRAG:	// �϶���ʱ����
			{
				stNetTempKitbag* pNetTempKitbag = (stNetTempKitbag*)param;

				pk.WriteUInt16(pNetTempKitbag->sSrcGridID);
				pk.WriteUInt16(pNetTempKitbag->sSrcNum);
				pk.WriteUInt16(pNetTempKitbag->sTarGridID);

				pCNetIf->SendPacketMessage(pk);
				break;
			}
		default:
			break;
		}
	}
	catch (...)
	{
		//MessageBox(0, "!!!!!!!!!!!!!!!!!!!!exception: Begin Action", "error", 0);fix compile again
	}
}

// Э��C->S : ����ֹͣ�ж���Ϣ
void CProCirculateCS::EndAction( CActionState* pState )
{
	WPacket pk	=pCNetIf->GetWPacket();

	pk.WriteCmd(CMD_CM_ENDACTION);	//�����ж�
	pCNetIf->SendPacketMessage(pk);

	// log
    CCharacter *pMainCha = CGameApp::GetCurScene()->GetMainCha();
	if (pMainCha)
	{
		LG(pMainCha->getLogName(), "###Send(EndAction):\tTick:[%d]\n", GetTickCount());
	}
	//
}

bool CProCirculate::Connect(const char *hostname,unsigned short port,unsigned long timeout)
{
	return pCNetIf->m_connect.Connect(hostname,port,timeout);
}
void CProCirculate::Disconnect(int reason)
{
	pCNetIf->m_connect.Disconnect(reason);
}

bool CProCirculate::SendPrivateKey()
{
	// 1. Генерация 32-байт AES-256 ключа
	NTSTATUS status = BCryptGenRandom(NULL, g_NetIF->cliAesKey, 32, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (!BCRYPT_SUCCESS(status)) {
		LG("enc", "BCryptGenRandom (AES key) failed: 0x%08X\n", status);
		return false;
	}

	// 2. RSA-OAEP-SHA256 шифрование AES ключа публичным ключом сервера
	BCRYPT_OAEP_PADDING_INFO oaepInfo = {};
	oaepInfo.pszAlgId = BCRYPT_SHA256_ALGORITHM;
	oaepInfo.pbLabel = NULL;
	oaepInfo.cbLabel = 0;

	// Определяем размер зашифрованных данных
	ULONG encryptedLen = 0;
	status = BCryptEncrypt(
		g_NetIF->hRsaPubKey,
		g_NetIF->cliAesKey, 32,
		&oaepInfo,
		NULL, 0,
		NULL, 0, &encryptedLen,
		BCRYPT_PAD_OAEP);
	if (!BCRYPT_SUCCESS(status)) {
		LG("enc", "BCryptEncrypt (RSA size query) failed: 0x%08X\n", status);
		return false;
	}

	// Шифруем
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
		LG("enc", "BCryptEncrypt (RSA encrypt) failed: 0x%08X\n", status);
		return false;
	}

	// Логируем AES ключ и зашифрованные данные
	{
		std::string aesHex, encHex;
		aesHex.reserve(64);
		encHex.reserve(resultLen * 2);
		char buf[3];
		for (int i = 0; i < 32; i++) { sprintf(buf, "%02x", g_NetIF->cliAesKey[i]); aesHex += buf; }
		for (ULONG i = 0; i < resultLen; i++) { sprintf(buf, "%02x", encryptedKey[i]); encHex += buf; }
		LG("enc", "SendPrivateKey: AES-256 key (32 bytes):\n%s\n", aesHex.c_str());
		LG("enc", "SendPrivateKey: RSA-encrypted key (%u bytes):\n%s\n", resultLen, encHex.c_str());
	}

	// 3. Инициализация BCrypt AES ключа для симметричного шифрования
	if (!g_NetIF->InitAesKey()) {
		LG("enc", "InitAesKey failed\n");
		return false;
	}

	// 4. Отправка зашифрованного AES ключа серверу (сырые байты, без Base64)
	WPacket pk = pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SEND_PRIVATE_KEY);
	pk.WriteSequence(reinterpret_cast<const char*>(encryptedKey.data()), static_cast<uShort>(resultLen));

	pCNetIf->SendPacketMessage(pk);
	g_NetIF->handshakeDone = true;
	g_NetIF->_comm_enc = true;
	g_NetIF->m_connect.CHAPSTR(false);
	return true;
}


void CProCirculate::Login(const char *accounts,const char *password, const char* passport)
{
	if (!g_NetIF->handshakeDone) 
	{
		// Do not continue with login if the key exchange failed.
		return;
	}

	extern short g_sClientVer;
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_LOGIN);
	pk.WriteString(accounts);

	string digest;
	string hexencoded;
	CryptoPP::BLAKE2s d;
	CryptoPP::StringSource ss(password, true, new CryptoPP::HashFilter(d, new CryptoPP::StringSink(digest)));
	CryptoPP::StringSource ss2(digest, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hexencoded)));
	pk.WriteString(hexencoded.c_str());
	string strMac=GetMacString();
	if (strMac.empty()) strMac="Unknown";
	pk.WriteString(strMac.c_str());
	pk.WriteUInt16(911);
	pk.WriteUInt16(g_sClientVer);

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::Logout()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_LOGOUT);
	pCNetIf->SendPacketMessage(pk);
	Sleep(1000); // Даём серверу время обработать logout
}

void CProCirculate::BeginPlay(char cha_index)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_BGNPLAY);
	pk.WriteUInt8(cha_index);

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::EndPlay()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_ENDPLAY);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::NewCha(const char* chaname, const char* birth, int type, int hair, int face)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_NEWCHA);
	pk.WriteString(chaname);
	pk.WriteString(birth);
	pk.WriteUInt32(type);
	pk.WriteUInt32(hair);
	pk.WriteUInt32(face);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::DelCha(uint8_t cha_index, const char szPassword2[])
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_DELCHA);
	pk.WriteUInt8(cha_index);
	pk.WriteString(szPassword2);

	pCNetIf->SendPacketMessage(pk);
}


void CProCirculate::OpenRankings()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_RANK);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::Say(const char *content)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SAY);			
	pk.WriteSequence(content, uShort(strlen(content))+1);
	pCNetIf->SendPacketMessage(pk);
}


void CProCirculate::SynBaseAttribute(CChaAttr *pCAttr)
{
	char	chAttrNum = 0;
	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
		if (pCAttr->GetChangeBitFlag(i))
			chAttrNum++;

	if (chAttrNum == 0)
		return;

	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SYNATTR);	
	pk.WriteUInt8(chAttrNum);
	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
	{
		if (pCAttr->GetChangeBitFlag(i))
		{
			pk.WriteUInt16(i);
			pk.WriteUInt32((uLong)pCAttr->GetAttr(i));
		}
	}

	// log
	char szReqChangeAttr[256] = {0};
	strcpy(szReqChangeAttr, g_oLangRec.GetString(325));

	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
	{
		if (pCAttr->GetChangeBitFlag(i))
			LG(szReqChangeAttr, g_oLangRec.GetString(326), i, pCAttr->GetAttr(i));
	}
	LG(szReqChangeAttr, "\n");

	pCNetIf->SendPacketMessage(pk);
}


void CProCirculate::RefreshChaData(long lWorldID, long lHandle)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_REFRESH_DATA);	
	pk.WriteUInt32(lWorldID);
	pk.WriteUInt32(lHandle);

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::SkillUpgrade(short sSkillID, char chAddLv)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SKILLUPGRADE);	
	pk.WriteUInt16(sSkillID);
	pk.WriteUInt8(chAddLv);

	// log
	char szReqChangeAttr[256] = {0};
	strcpy(szReqChangeAttr, g_oLangRec.GetString(327));

	LG(szReqChangeAttr, g_oLangRec.GetString(328), sSkillID, chAddLv);

	pCNetIf->SendPacketMessage(pk);
}

