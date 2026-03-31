// CharForge.cpp Created by knight-gongjian 2004.12.7.
//---------------------------------------------------------
#include "stdafx.h"
#include "CharForge.h"
#include <ForgeRecord.h>
#include <ForgeRecordStore.h>
#include "GameAppNet.h"
#include "Player.h"

//---------------------------------------------------------

mission::CForgeSystem g_ForgeSystem;

namespace mission {
	void CForgeSystem::ForgeItem(CCharacter& character, BYTE byIndex) {
		//
		if (character.m_CKitbag.IsLock()) {
			character.SystemNotice("");
			return;
		}

		//
		if (character.m_CKitbag.IsPwdLocked()) {
			character.SystemNotice("");
			return;
		}
		//add by ALLEN 2007-10-16
		if (character.IsReadBook()) {
			character.SystemNotice("");
			return;
		}
		SItemGrid* pItemData;
		if (!(pItemData = character.m_CKitbag.GetGridContByID(byIndex))) {
			character.SystemNotice("ForgeItem: ID = %d", byIndex);
			return;
		}

		CItemRecord* pItem = GetItemRecordInfo(pItemData->sID);
		if (pItem == NULL) {
			character.SystemNotice("ForgeItem: ID = %d", pItemData->sID);
			return;
		}

		if (pItem->chForgeLv == 0) {
			character.SystemNotice("%s", pItem->szName);
			return;
		}

		BYTE byLevel = pItemData->chForgeLv;
		if (byLevel >= ROLE_MAXNUM_FORGE) {
			character.SystemNotice("%s", pItem->szName);
			return;
		}

		//
		byLevel++;

		CForgeRecord* pRecord = ForgeRecordStore::Instance()->Get(byLevel);
		if (!pRecord) {
			character.SystemNotice("ForgeItem:Level = %d", byLevel);
			return;
		}

		if (!character.HasMoney(pRecord->dwMoney)) {
			character.SystemNotice("");
			return;
		}

		for (int i = 0; i < FORGE_MAXNUM_ITEM; i++) {
			if (pRecord->ForgeItem[i].sItem == 0)
				break;
			if (!character.HasItem(pRecord->ForgeItem[i].sItem, pRecord->ForgeItem[i].byNum)) {
				char szForgeItem[64] = "";
				CItemRecord* pForgeItem = (CItemRecord*)GetItemRecordInfo(pRecord->ForgeItem[i].sItem);
				if (pForgeItem) {
					strcpy(szForgeItem, pForgeItem->szName.c_str());
				}
				character.SystemNotice("%s%d", szForgeItem, pRecord->ForgeItem[i].byNum);
				return;
			}
		}

		BOOL bSuccess = TRUE;
		//
		if (byLevel > pItem->chForgeLv) {
			//
			bSuccess = FALSE;
		}
		else {
			//
			if (byLevel > pItem->chForgeSteady) {
				//
				if (rand() % 100 >= pRecord->byRate) {
					//
					bSuccess = FALSE;
				}
			}
		}

		for (int i = 0; i < FORGE_MAXNUM_ITEM; i++) {
			if (pRecord->ForgeItem[i].sItem == 0)
				break;
			if (!character.TakeItem(pRecord->ForgeItem[i].sItem, pRecord->ForgeItem[i].byNum, "")) {
				char szForgeItem[64] = "";
				CItemRecord* pForgeItem = (CItemRecord*)GetItemRecordInfo(pRecord->ForgeItem[i].sItem);
				if (pForgeItem) {
					strcpy(szForgeItem, pForgeItem->szName.c_str());
				}
				character.SystemNotice("%s%d", szForgeItem, pRecord->ForgeItem[i].byNum);
				return;
			}
		}

		if (bSuccess) {
			//
			character.m_CKitbag.SetChangeFlag(false);
			SItemGrid* pGrid = character.m_CKitbag.GetGridContByID(byIndex);
			if (pGrid == NULL || !character.ItemForge(pGrid, byLevel)) {
				character.SystemNotice("%s(%d)", pItem->szName.c_str(), byLevel);
				return;
			}

			character.SystemNotice("%s(%d)", pItem->szName.c_str(), byLevel);
			character.SynKitbagNew(enumSYN_KITBAG_FORGES);
		}
		else {
			if (pRecord->byFailure == BYTE(-1)) {
				character.m_CKitbag.SetChangeFlag(false);
				character.KbClearItem(true, true, byIndex);
				character.SystemNotice("%s", pItem->szName);
				character.SynKitbagNew(enumSYN_KITBAG_FORGEF);
			}
			else {
				//
				character.m_CKitbag.SetChangeFlag(false);
				byLevel = pRecord->byFailure;
				SItemGrid* pGrid = character.m_CKitbag.GetGridContByID(byIndex);
				if (pGrid == NULL || !character.ItemForge(pGrid, byLevel)) {
					character.SystemNotice("%s(%d)", pItem->szName.c_str(), byLevel);
					return;
				}

				character.SystemNotice("%s(%d)", pItem->szName.c_str(), byLevel);
				character.SynKitbagNew(enumSYN_KITBAG_FORGEF);
			}
		}
	}
}
