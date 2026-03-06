//=============================================================================
// FileName: CharacterPacket.cpp
// Creater: ZhangXuedong
// Date: 2005.05.09
// Comment: Build Character Packet
//=============================================================================
#include "stdafx.h"
#include "Character.h"
#include "Player.h"
#include "GameApp.h"

void CCharacter::WriteBaseInfo(WPACKET &pkret, Char chLookType)
{
	CPlayer	*pCPlayer = GetPlayer();

	WRITE_LONG(pkret, GetCat());
	WRITE_LONG(pkret, GetID());
	if (pCPlayer)
	{
		if (g_Config.m_bBlindChaos && IsPlayerCha() && LOOK_OTHER == chLookType && IsPKSilver())
		{
			WRITE_LONG(pkret, pCPlayer->GetMainCha()->GetID());
			WRITE_STRING(pkret, "");
			WRITE_CHAR(pkret, pCPlayer->GetGMLev());
		}
		else
		{
			WRITE_LONG(pkret, pCPlayer->GetMainCha()->GetID());
			WRITE_STRING(pkret, pCPlayer->GetMainCha()->GetName());
			WRITE_CHAR(pkret, pCPlayer->GetGMLev());
		}
	}
	else
	{
		WRITE_LONG(pkret, GetID());
		WRITE_STRING(pkret, "");
		WRITE_CHAR(pkret, 0);
	}
	if (g_Config.m_bBlindChaos && IsPlayerCha() && LOOK_OTHER == chLookType && IsPKSilver())
	{
		WRITE_LONG(pkret, GetHandle());
		WRITE_CHAR(pkret, (Char)m_CChaAttr.GetAttr(ATTR_CHATYPE));
		WRITE_STRING(pkret, "");
		WRITE_STRING(pkret, "");
		WRITE_SHORT(pkret, GetPlyMainCha()->GetIcon());
		WRITE_LONG(pkret, 0);
		WRITE_STRING(pkret, "");
		WRITE_STRING(pkret, "");
		WRITE_STRING(pkret, "");
	}
	else
	{
		WRITE_LONG(pkret, GetHandle());
		WRITE_CHAR(pkret, (Char)m_CChaAttr.GetAttr(ATTR_CHATYPE));
		WRITE_STRING(pkret, m_name);
		WRITE_STRING(pkret, GetMotto());
		WRITE_SHORT(pkret, GetPlyMainCha()->GetIcon());
		WRITE_LONG(pkret, GetValidGuildID());
		WRITE_STRING(pkret, GetValidGuildName());
		WRITE_STRING(pkret, GetValidGuildMotto());
		WRITE_LONG(pkret, guildPermission);
		WRITE_STRING(pkret, GetStallName());
	}
	WRITE_SHORT(pkret, GetExistState());
	WRITE_LONG(pkret, GetPos().x);
	WRITE_LONG(pkret, GetPos().y);
	WRITE_LONG(pkret, GetRadius());
	WRITE_SHORT(pkret, GetAngle());
	// ID
	CPlayer	*pCPly = GetPlayer();
	if (pCPly)
		WRITE_LONG(pkret, pCPly->getTeamLeaderID());
	else
		WRITE_LONG(pkret, 0);

	if (IsPlayerCha()){
		WRITE_CHAR(pkret, 1);
	}else{
		WRITE_CHAR(pkret, 0);
	}
	
	//
	WriteSideInfo(pkret);
	WriteEventInfo(pkret);

	WriteLookData(pkret, chLookType);
	WritePKCtrl(pkret);
	WriteAppendLook(m_CKitbag, pkret, true);
}

void CCharacter::WritePKCtrl(WPACKET &pkret)
{
	WRITE_CHAR(pkret, m_chPKCtrl.to_ulong());
}

void CCharacter::WriteSideInfo(WPACKET &pkret)
{
	WRITE_CHAR(pkret, (Char)GetSideID());
}

void CCharacter::WriteSkillbag(WPACKET &pk, int nSynType)
{
	SSkillGrid	*pSkillGrid = 0;
	CSkillTempData	*pSkillTData = 0;

	WRITE_SHORT(pk, m_sDefSkillNo);

	WRITE_CHAR(pk, nSynType);

	short	sChangeSkillNum = m_CSkillBag.GetChangeSkillNum();
	CCharacter	*pCCtrlCha = GetPlyCtrlCha();
	bool	bIsBoatCtrl = pCCtrlCha->IsBoat();
	bool	bAddBoatSkill = false;
	if (bIsBoatCtrl) // 
	{
		pSkillGrid = pCCtrlCha->m_CSkillBag.GetSkillContByNum(0);
		if (pSkillGrid)
		{
			pSkillTData = g_pGameApp->GetSkillTData(pSkillGrid->sID, pSkillGrid->chLv);
			if (pSkillTData)
			{
				bAddBoatSkill = true;
				sChangeSkillNum += 1;
			}
		}
	}
	WRITE_SHORT(pk, sChangeSkillNum);
	if (bAddBoatSkill)
	{
		WRITE_SHORT(pk, pSkillGrid->sID);
		WRITE_CHAR(pk, pSkillGrid->chState);
		WRITE_CHAR(pk, pSkillGrid->chLv);
		WRITE_SHORT(pk, pSkillTData->sUseSP);
		WRITE_SHORT(pk, pSkillTData->sUseEndure);
		WRITE_SHORT(pk, pSkillTData->sUseEnergy);
		WRITE_LONG(pk, pSkillTData->lResumeTime);
		WRITE_SHORT(pk, pSkillTData->sRange[0]);
		if (pSkillTData->sRange[0] != enumRANGE_TYPE_NONE)
		{
			for (short j = 1; j < defSKILL_RANGE_EXTEP_NUM; j++)
				WRITE_SHORT(pk, pSkillTData->sRange[j]);
		}
	}
	for (short i = 0; i < sChangeSkillNum; i++)
	{
		pSkillGrid = m_CSkillBag.GetChangeSkill(i);
		if (!pSkillGrid)
			return;
		pSkillTData = g_pGameApp->GetSkillTData(pSkillGrid->sID, pSkillGrid->chLv);
		if (!pSkillTData)
			return;
		WRITE_SHORT(pk, pSkillGrid->sID);
		WRITE_CHAR(pk, pSkillGrid->chState);
		WRITE_CHAR(pk, pSkillGrid->chLv);
		WRITE_SHORT(pk, pSkillTData->sUseSP);
		WRITE_SHORT(pk, pSkillTData->sUseEndure);
		WRITE_SHORT(pk, pSkillTData->sUseEnergy);
		WRITE_LONG(pk, pSkillTData->lResumeTime);
		WRITE_SHORT(pk, pSkillTData->sRange[0]);
		if (pSkillTData->sRange[0] != enumRANGE_TYPE_NONE)
		{
			for (short j = 1; j < defSKILL_RANGE_EXTEP_NUM; j++)
				WRITE_SHORT(pk, pSkillTData->sRange[j]);
		}
	}
}

void CCharacter::WriteKitbag(CKitbag &CKb, WPACKET &WtPk, int nSynType)
{
	SItemGrid	*pGridCont;
	CItemRecord* pItemRec;

	WRITE_CHAR(WtPk, nSynType);
	Short sCapacity = CKb.GetCapacity();
	if (nSynType == enumSYN_KITBAG_INIT)
		WRITE_SHORT(WtPk, sCapacity);
	for (int i = 0; i < sCapacity; i++)
	{
		if (!CKb.IsSingleChange(i))
			continue;
		WRITE_SHORT(WtPk, i);
		pGridCont = CKb.GetGridContByID(i);
		if (!pGridCont)
		{
			WRITE_SHORT(WtPk, 0);
			continue;
		}
		pItemRec = GetItemRecordInfo( pGridCont->sID );
		if (!pItemRec)
		{
			WRITE_SHORT(WtPk, 0);
			continue;
		}
		// 
		WRITE_SHORT(WtPk, pGridCont->sID);
		WRITE_LONG(WtPk, pGridCont->dwDBID	);
		WRITE_SHORT(WtPk, pGridCont->sNeedLv);
		WRITE_SHORT(WtPk, pGridCont->sNum);
		WRITE_SHORT(WtPk, pGridCont->sEndure[0]);
		WRITE_SHORT(WtPk, pGridCont->sEndure[1]);
		WRITE_SHORT(WtPk, pGridCont->sEnergy[0]);
		WRITE_SHORT(WtPk, pGridCont->sEnergy[1]);
		WRITE_CHAR(WtPk, pGridCont->chForgeLv);
		WRITE_CHAR(WtPk, pGridCont->IsValid() ? 1 : 0);
		WRITE_CHAR(WtPk, pGridCont->bItemTradable);
		WRITE_LONG(WtPk, pGridCont->expiration);

		pItemRec = GetItemRecordInfo( pGridCont->sID );
		if( pItemRec->sType == enumItemTypeBoat ) // WorldID
		{
			CCharacter	*pCBoat = GetPlayer()->GetBoat((DWORD)pGridCont->GetDBParam(enumITEMDBP_INST_ID));
			if (pCBoat)
				WRITE_LONG(WtPk, pCBoat->GetID());
			else
				WRITE_LONG(WtPk, 0);
		}

		WRITE_LONG(WtPk, pGridCont->GetDBParam(enumITEMDBP_FORGE));
		WRITE_LONG(WtPk, pGridCont->GetDBParam(enumITEMDBP_INST_ID));
		if (pGridCont->IsInstAttrValid()) // 
		{
			WRITE_CHAR(WtPk, 1);
			for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
			{
				WRITE_SHORT(WtPk, pGridCont->sInstAttr[j][0]);
				WRITE_SHORT(WtPk, pGridCont->sInstAttr[j][1]);
			}
		}
		else
			WRITE_CHAR(WtPk, 0); // 
	}
	WRITE_SHORT(WtPk, -1); // 
}

// client: ReadChaLookPacket
// void NetSynAttr( DWORD dwWorldID, char chType, short sNum, stEffect *pEffect )
Short GetChaosEquip(Long type, Long job) {
	if (type == enumEQUIP_BODY) {
		switch (job) {
			case JOB_TYPE_JUJS: return 1933;
			case JOB_TYPE_SHUANGJS: return 1930;
			case JOB_TYPE_JUJISHOU: return 1945;
			case JOB_TYPE_FENGYINSHI: return 1957;
			case JOB_TYPE_HANGHAISHI: return 1978;
			case JOB_TYPE_SHENGZHIZHE: return 1960;
			default: break;
		}
	} else if (type == enumEQUIP_GLOVE) {
		switch (job) {
			case JOB_TYPE_JUJS: return 477;
			case JOB_TYPE_SHUANGJS: return 1937;
			case JOB_TYPE_JUJISHOU: return 1949;
			case JOB_TYPE_FENGYINSHI: return 1964;
			case JOB_TYPE_HANGHAISHI: return 1982;
			case JOB_TYPE_SHENGZHIZHE: return 1967;
			default: break;
		}
	} else if (type == enumEQUIP_SHOES) {
		switch (job) {
			case JOB_TYPE_JUJS: return 653;
			case JOB_TYPE_SHUANGJS: return 1941;
			case JOB_TYPE_JUJISHOU: return 1953;
			case JOB_TYPE_FENGYINSHI: return 1971;
			case JOB_TYPE_HANGHAISHI: return 1986;
			case JOB_TYPE_SHENGZHIZHE: return 1974;
			default: break;
		}
	} else if (type == enumEQUIP_RHAND) {
		switch (job) {
			case JOB_TYPE_JUJS: return 3803;
			case JOB_TYPE_SHUANGJS: return 3800;
			case JOB_TYPE_JUJISHOU: return 3807;
			case JOB_TYPE_FENGYINSHI: return 3811;
			case JOB_TYPE_HANGHAISHI: return 3818;
			case JOB_TYPE_SHENGZHIZHE: return 3814;
			default: break;
		}
	} else if (type == enumEQUIP_LHAND) {
		switch (job) {
			case JOB_TYPE_JUJS: return 0;
			case JOB_TYPE_SHUANGJS: return 3800;
			case JOB_TYPE_JUJISHOU: return 0;
			case JOB_TYPE_FENGYINSHI: return 0;
			case JOB_TYPE_HANGHAISHI: return 0;
			case JOB_TYPE_SHENGZHIZHE: return 0;
			default: break;
		}
	} else if (type == enumEQUIP_HEAD) {
		switch (job) {
			case JOB_TYPE_JUJS: return 0;
			case JOB_TYPE_SHUANGJS: return 0;
			case JOB_TYPE_JUJISHOU: return 0;
			case JOB_TYPE_FENGYINSHI: return 0;
			case JOB_TYPE_HANGHAISHI: return 2107;
			case JOB_TYPE_SHENGZHIZHE: return 2207;
			default: break;
		}
	}
	return 0;
}

void CCharacter::WriteLookData(WPACKET &WtPk, Char chLookType, Char chSynType)
{
	WRITE_CHAR(WtPk, chSynType);
	WRITE_SHORT(WtPk, m_SChaPart.sTypeID);
	if( m_CChaAttr.GetAttr(ATTR_CHATYPE) == enumCHACTRL_PLAYER && IsBoat() )
	{
		WRITE_CHAR( WtPk, 1); // 
		WRITE_SHORT( WtPk, m_SChaPart.sPosID );
		WRITE_SHORT( WtPk, m_SChaPart.sBoatID );
		WRITE_SHORT( WtPk, m_SChaPart.sHeader );
		WRITE_SHORT( WtPk, m_SChaPart.sBody );
		WRITE_SHORT( WtPk, m_SChaPart.sEngine );
		WRITE_SHORT( WtPk, m_SChaPart.sCannon );
		WRITE_SHORT( WtPk, m_SChaPart.sEquipment );
	}
	else
	{
		// modify by kong@pkodev.net 11.08.2017 [begin]
		long nJob = (long)getAttr(ATTR_JOB);
		if (g_Config.m_bBlindChaos && IsPlayerCha() && LOOK_OTHER == chLookType && IsPKSilver())
		{
			WRITE_CHAR(WtPk, 0);
			WRITE_SHORT(WtPk, 0); // Hair ID: 0 (according to client)
			SItemGrid *pItem;

			int nItemCnt = enumEQUIP_NUM;

			for (int i = 0; i < nItemCnt; i++)
			{
				pItem = &m_SChaPart.SLink[i];
				if (chSynType == enumSYN_LOOK_CHANGE)
				{
					if (!pItem->IsChange())
					{
						WRITE_SHORT(WtPk, 0);
						continue;
					}
				}

				Short eqID = GetChaosEquip(i, nJob);
				//WRITE_LONG(WtPk, pItem->dwDBID);
				WRITE_SHORT(WtPk, eqID); // pItem->sID
				WRITE_LONG(WtPk, pItem->dwDBID);
				WRITE_SHORT(WtPk, pItem->sNeedLv);
				if (eqID == 0)
					continue;

				if (chSynType == enumSYN_LOOK_CHANGE)
				{
					WRITE_SHORT(WtPk, pItem->sEndure[0]);
					WRITE_SHORT(WtPk, pItem->sEnergy[0]);
					WRITE_CHAR(WtPk, pItem->IsValid() ? 1 : 0);
					WRITE_CHAR(WtPk, pItem->bItemTradable);
					WRITE_LONG(WtPk, pItem->expiration);

					continue;
				}
				else
				{
					WRITE_SHORT(WtPk, pItem->sNum);
					WRITE_SHORT(WtPk, pItem->sEndure[0]);
					WRITE_SHORT(WtPk, pItem->sEndure[1]);
					WRITE_SHORT(WtPk, pItem->sEnergy[0]);
					WRITE_SHORT(WtPk, pItem->sEnergy[1]);
					WRITE_CHAR(WtPk, pItem->chForgeLv);
					WRITE_CHAR(WtPk, pItem->IsValid() ? 1 : 0);
					WRITE_CHAR(WtPk, pItem->bItemTradable);
					WRITE_LONG(WtPk, pItem->expiration);

				}
				if(chLookType!=LOOK_SELF) // , 
				{
					WRITE_CHAR(WtPk, 0);
					continue;
				}
				WRITE_CHAR(WtPk, 1);

				WRITE_LONG(WtPk, pItem->GetDBParam(enumITEMDBP_FORGE));
				WRITE_LONG(WtPk, pItem->GetDBParam(enumITEMDBP_INST_ID));
				if (pItem->IsInstAttrValid())
				{
					WRITE_CHAR(WtPk, 1);
					for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
					{
						WRITE_SHORT(WtPk, pItem->sInstAttr[j][0]);
						WRITE_SHORT(WtPk, pItem->sInstAttr[j][1]);
					}
				}
				else
					WRITE_CHAR(WtPk, 0);
			}
			return;
		} // modification [ends]

		WRITE_CHAR( WtPk, 0); // The appearance of human characters
		WRITE_SHORT(WtPk, m_SChaPart.sHairID);
		SItemGrid *pItem;

		int nItemCnt = enumEQUIP_NUM;

		//if(chLookType==LOOK_TEAM) nItemCnt = 3; // , 

		for (int i = 0; i < nItemCnt; i++)
		{
			pItem = &m_SChaPart.SLink[i];
			if (chSynType == enumSYN_LOOK_CHANGE)
			{
				if (!pItem->IsChange())
				{
					WRITE_SHORT(WtPk, 0);
					continue;
				}
			}
			WRITE_SHORT(WtPk, pItem->sID);
			WRITE_LONG(WtPk, pItem->dwDBID);
			WRITE_SHORT(WtPk, pItem->sNeedLv);
			if (pItem->sID == 0)
				continue;
			if (chSynType == enumSYN_LOOK_CHANGE)
			{
				WRITE_SHORT(WtPk, pItem->sEndure[0]);
				WRITE_SHORT(WtPk, pItem->sEnergy[0]);
				WRITE_CHAR(WtPk, pItem->IsValid() ? 1 : 0);
				WRITE_CHAR(WtPk, pItem->bItemTradable);
				WRITE_LONG(WtPk, pItem->expiration);
				continue;
			}
			else
			{
				WRITE_SHORT(WtPk, pItem->sNum);
				WRITE_SHORT(WtPk, pItem->sEndure[0]);
				WRITE_SHORT(WtPk, pItem->sEndure[1]);
				WRITE_SHORT(WtPk, pItem->sEnergy[0]);
				WRITE_SHORT(WtPk, pItem->sEnergy[1]);
				WRITE_CHAR(WtPk, pItem->chForgeLv);
				WRITE_CHAR(WtPk, pItem->IsValid() ? 1 : 0);
				WRITE_CHAR(WtPk, pItem->bItemTradable);
				WRITE_LONG(WtPk, pItem->expiration);
			}

			//if(chLookType!=LOOK_SELF) // , 
			//{
			//	WRITE_CHAR(WtPk, 0);
			//	continue;
			//}
			WRITE_CHAR(WtPk, 1);

			WRITE_LONG(WtPk, pItem->GetDBParam(enumITEMDBP_FORGE));
			WRITE_LONG(WtPk, pItem->GetDBParam(enumITEMDBP_INST_ID));
			if (pItem->IsInstAttrValid())
			{
				WRITE_CHAR(WtPk, 1);
				for (int j = 0; j < defITEM_INSTANCE_ATTR_NUM; j++)
				{
					WRITE_SHORT(WtPk, pItem->sInstAttr[j][0]);
					WRITE_SHORT(WtPk, pItem->sInstAttr[j][1]);
				}
			}
			else
				WRITE_CHAR(WtPk, 0);
		}
	}
}

// 
bool CCharacter::WriteAppendLook(CKitbag &CKb, WPACKET &pk, bool bInit)
{
	SItemGrid	*pGridCont;
	bool	bHasData = false;
	for (int i = 0; i < defESPE_KBGRID_NUM; i++)
	{
		if (!bHasData && CKb.IsSingleChange(i))
			bHasData = true;
		pGridCont = CKb.GetGridContByID(i);
		if (!pGridCont || !ItemIsAppendLook(pGridCont))
		{
			WRITE_SHORT(pk, 0);
			continue;
		}
		WRITE_SHORT(pk, pGridCont->sID);
		WRITE_CHAR(pk, pGridCont->IsValid() ? 1 : 0);
	}

	if (bInit) return true;
	else return bHasData;
}

void CCharacter::WriteShortcut(WPACKET &WtPk)
{
	for (int i = 0; i < SHORT_CUT_NUM; i++)
	{
		WRITE_CHAR(WtPk, m_CShortcut.chType[i]);
		WRITE_SHORT(WtPk, m_CShortcut.byGridID[i]);
	}
}

void CCharacter::WriteBoat(WPACKET &WtPk)
{
	CPlayer	*pCPlayer = GetPlayer();
	if (!pCPlayer)
	{
		WRITE_CHAR(WtPk, 0);
		return;
	}
	WRITE_CHAR(WtPk, pCPlayer->GetNumBoat());
	CCharacter	*pCBoat;
	for (BYTE i = 0; i < pCPlayer->GetNumBoat(); i++)
	{
		pCBoat = pCPlayer->GetBoat(i);
		if (!pCBoat)
			continue;
		WriteItemChaBoat(WtPk, pCBoat);
	}
}

void CCharacter::WriteItemChaBoat(WPACKET &WtPk, CCharacter *pCBoat)
{
	pCBoat->WriteBaseInfo(WtPk);
	pCBoat->WriteAttr(WtPk, enumATTRSYN_INIT);
	pCBoat->m_CKitbag.SetChangeFlag(true);
	pCBoat->WriteKitbag(pCBoat->m_CKitbag, WtPk, enumSYN_KITBAG_INIT); // 
	pCBoat->WriteSkillState(WtPk);
}
