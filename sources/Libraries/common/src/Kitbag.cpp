//=============================================================================
// FileName: Kitbag.cpp
// Creater: ZhangXuedong
// Date: 2004.12.17
// Comment: Kitbag
//=============================================================================

#include "Kitbag.h"
char g_key[9] = "19800216";


int Decrypt(char* buf, int len, const char* enc, int elen)
{
	int i;
	for(i = 0;i<elen;i++)
	{
		buf[i] = enc[i] - g_key[i%8];
	}
	return i;
}

int Encrypt(char* buf, int len, const char* pwd, int plen)
{
	int i;
	for(i = 0;i<plen;i++)
	{
		buf[i] = pwd[i] + g_key[i%8];
	}
	return i;
}

char* KitbagData2String(CKitbag* pKitbag, char* szStrBuf, int nLen)
{
	static char buff[7000];
	if (!pKitbag || !szStrBuf) return NULL;

	__int64	lnCheckSum = 0;
	char	szData[512];
	int		nBufLen = 0, nDataLen;
	int lentemp;
	char* pbufftemp;
	buff[0] = '\0';
	szStrBuf[0] = '\0';

	sprintf(szData, "%d@%d#", pKitbag->GetCapacity(), 114);
	nDataLen = (int)strlen(szData);
	if (nBufLen + nDataLen >= nLen) return NULL;
	strcat(szStrBuf, szData);
	nBufLen += nDataLen;

	pbufftemp = szStrBuf;
	szStrBuf = buff;
	lentemp = nBufLen;

	sprintf(szData, "%d;", enumKBITEM_TYPE_NUM);
	nDataLen = (int)strlen(szData);
	if (nBufLen + nDataLen >= nLen) return NULL;
	strcat(szStrBuf, szData);
	nBufLen += nDataLen;
	lnCheckSum += enumKBITEM_TYPE_NUM;

	short	sUseNum;
	SItemGrid* pGridCont;
	for (int i = 0; i < enumKBITEM_TYPE_NUM; i++)
	{
		sUseNum = pKitbag->GetUseGridNum(i);
		sprintf(szData, "%d;", sUseNum);
		nDataLen = (int)strlen(szData);
		if (nBufLen + nDataLen >= nLen) return NULL;
		strcat(szStrBuf, szData);
		nBufLen += nDataLen;

		for (int j = 0; j < pKitbag->GetCapacity(); j++)
		{
			pGridCont = pKitbag->GetGridContByID(j, i);
			if (!pGridCont)
				continue;
			sprintf(szData, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
				j, pGridCont->sID, pGridCont->sNum,
				pGridCont->sEndure[0], pGridCont->sEndure[1], pGridCont->sEnergy[0], pGridCont->sEnergy[1], pGridCont->chForgeLv, pGridCont->dwDBID, pGridCont->sNeedLv,
				pGridCont->bIsLock, pGridCont->bItemTradable, pGridCont->expiration
			);
			nDataLen = (int)strlen(szData);
			if (nBufLen + nDataLen >= nLen) return NULL;
			strcat(szStrBuf, szData);
			nBufLen += nDataLen;

			lnCheckSum += (pGridCont->sID + pGridCont->sNum + pGridCont->sEndure[0] + pGridCont->sEndure[1] + pGridCont->sEnergy[0] + pGridCont->sEnergy[1] + pGridCont->chForgeLv + pGridCont->dwDBID + pGridCont->sNeedLv + pGridCont->bIsLock + pGridCont->bItemTradable);

			for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
			{
				sprintf(szData, ",%d", pGridCont->GetDBParam(m));
				nDataLen = (int)strlen(szData);
				if (nBufLen + nDataLen >= nLen) return NULL;
				strcat(szStrBuf, szData);
				nBufLen += nDataLen;
				lnCheckSum += pGridCont->GetDBParam(m);
			}
			if (pGridCont->IsInstAttrValid())
			{
				nDataLen = 2;
				if (nBufLen + nDataLen >= nLen) return NULL;
				strcat(szStrBuf, ",1");
				nBufLen += nDataLen;

				for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
				{
					sprintf(szData, ",%d,%d", pGridCont->sInstAttr[k][0], pGridCont->sInstAttr[k][1]);
					nDataLen = (int)strlen(szData);
					if (nBufLen + nDataLen >= nLen) return NULL;
					strcat(szStrBuf, szData);
					nBufLen += nDataLen;
					lnCheckSum += pGridCont->sInstAttr[k][0] + pGridCont->sInstAttr[k][1];
				}
			}
			else
			{
				nDataLen = 2;
				if (nBufLen + nDataLen >= nLen) return NULL;
				strcat(szStrBuf, ",0");
				nBufLen += nDataLen;
			}
			sprintf(szData, ";");
			nDataLen = (int)strlen(szData);
			if (nBufLen + nDataLen >= nLen) return NULL;
			strcat(szStrBuf, szData);
			nBufLen += nDataLen;
		}
	}
	sprintf(szData, "%d", lnCheckSum);
	nDataLen = (int)strlen(szData);
	if (nBufLen + nDataLen >= nLen) return NULL;
	strcat(szStrBuf, szData);
	nBufLen += nDataLen;

	int len = Encrypt(pbufftemp + lentemp, 7000, szStrBuf, nBufLen - lentemp);

	pbufftemp[len + lentemp] = '\0';
	return pbufftemp;
}

bool String2KitbagData(CKitbag* pKitbag, std::string& strData)
{
	static char buff[7000];
	if (!pKitbag)
		return false;
	if (!strcmp(strData.c_str(), ""))
		return true;

	__int64	lnCheckSum = 0;
	const short csStrNum = 1 + enumKBITEM_TYPE_NUM * defMAX_KBITEM_NUM_PER_TYPE * 2 + 1 + 1;
	std::string strList[csStrNum];
	const short csSubNum = 8 + enumITEMDBP_MAXNUM + defITEM_INSTANCE_ATTR_NUM_VER110 * 2 + 1 + 1;
	std::string strSubList[csSubNum];
	std::string	strVer[2];
	std::string strCap[2];
	bool bIsOldVer;
	int	nSegNum;
	if (Util_ResolveTextLine(strData.c_str(), strCap, 2, '@') > 1) // ???????????
	{
		pKitbag->SetCapacity(Str2Int(strCap[0]));
		bIsOldVer = Util_ResolveTextLine(strCap[1].c_str(), strVer, 2, '#') == 1 ? true : false;
	}
	else
	{
		bIsOldVer = Util_ResolveTextLine(strCap[0].c_str(), strVer, 2, '#') == 1 ? true : false;
	}

	//	2008-7-28	yangyinyu	add	begin!
	int	iVer = atoi(strVer[0].c_str());
	//	2008-7-28	yangyinyu	add	end!

	if (bIsOldVer)
	{
		if (!strcmp(strVer[0].c_str(), ""))
			return true;
		/*	2008-7-28	yangyinyu	change	begin!	//	?????????????ID??????????114????????113??114???????
		if(strcmp(strVer[0].c_str(),"113") == 0)
		*/
		if (iVer == 113 || iVer == 114)
			//	2008-7-28	yangyinyu	change	end!
		{
			int len = Decrypt(buff, 7000, strVer[1].c_str(), (int)strVer[1].length());
			buff[len] = '\0';
			nSegNum = Util_ResolveTextLine(buff, strList, csStrNum, ';');

		}
		else
			nSegNum = Util_ResolveTextLine(strVer[0].c_str(), strList, csStrNum, ';');
	}
	else
	{
		if (!strcmp(strVer[1].c_str(), ""))
			return true;

		if (iVer == 113 || iVer == 114)
		{
			int len = Decrypt(buff, 7000, strVer[1].c_str(), (int)strVer[1].length());
			buff[len] = '\0';
			nSegNum = Util_ResolveTextLine(buff, strList, csStrNum, ';');

		}
		else
		{
			nSegNum = Util_ResolveTextLine(strVer[1].c_str(), strList, csStrNum, ';');
		}
		if (nSegNum < 2)
			return false;
	}

	short	sSegID = 0, sTCount;
	short	sPageNum = Str2Int(strList[sSegID++]);
	if (sPageNum > enumKBITEM_TYPE_NUM)
		sPageNum = enumKBITEM_TYPE_NUM;
	if (sPageNum == 0)
		return true;
	lnCheckSum += sPageNum;

	short	sUseGridNum, sGridID;
	int	nItemNumPerPage;
	if (bIsOldVer) // ?????????????
		nItemNumPerPage = (nSegNum - 1) / sPageNum - 1;
	else
		nItemNumPerPage = (nSegNum - 2) / sPageNum - 1;

	SItemGrid SGridCont;
	for (int i = 0; i < sPageNum; i++)
	{
		sUseGridNum = Str2Int(strList[sSegID++]);
		for (int j = 0; j < nItemNumPerPage; j++)
		{
			sTCount = 0;
			Util_ResolveTextLine(strList[sSegID++].c_str(), strSubList, csSubNum, ',');
			sGridID = Str2Int(strSubList[sTCount++]);
			SGridCont.sID = Str2Int(strSubList[sTCount++]);
			SGridCont.sNum = Str2Int(strSubList[sTCount++]);
			SGridCont.sEndure[0] = Str2Int(strSubList[sTCount++]);
			SGridCont.sEndure[1] = Str2Int(strSubList[sTCount++]);
			SGridCont.sEnergy[0] = Str2Int(strSubList[sTCount++]);
			SGridCont.sEnergy[1] = Str2Int(strSubList[sTCount++]);
			SGridCont.chForgeLv = Str2Int(strSubList[sTCount++]);

			if (iVer == 114)
			{
				SGridCont.dwDBID = Str2Int(strSubList[sTCount++]);
				SGridCont.sNeedLv = Str2Int(strSubList[sTCount++]);
				SGridCont.bIsLock = Str2Int(strSubList[sTCount++]);
			}
			else
			{
				SGridCont.dwDBID = 0;
				SGridCont.sNeedLv = 0;
				SGridCont.bIsLock = 0;
			}
			SGridCont.bItemTradable = Str2Int(strSubList[sTCount++]);
			SGridCont.expiration = Str2Int(strSubList[sTCount++]);

			lnCheckSum += SGridCont.sID + SGridCont.sNum + SGridCont.sEndure[0] + SGridCont.sEndure[1] + SGridCont.sEnergy[0] + SGridCont.sEnergy[1] + SGridCont.chForgeLv + SGridCont.dwDBID + SGridCont.sNeedLv + SGridCont.bIsLock + SGridCont.bItemTradable;

			for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
			{
				SGridCont.SetDBParam(m, Str2Int(strSubList[sTCount++]));
				lnCheckSum += SGridCont.GetDBParam(m);
			}

			if (!bIsOldVer && (Str2Int(strVer[0]) >= 113 || Str2Int(strVer[0]) == 112)) // ???????????????????
			{
				if (Str2Int(strSubList[sTCount++]) > 0) // ???????????
				{
					for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
					{
						SGridCont.sInstAttr[k][0] = Str2Int(strSubList[sTCount + k * 2]);
						SGridCont.sInstAttr[k][1] = Str2Int(strSubList[sTCount + k * 2 + 1]);
						lnCheckSum += (SGridCont.sInstAttr[k][0] + SGridCont.sInstAttr[k][1]);
					}
				}
				else
					SGridCont.SetInstAttrInvalid();
			}
			else
			{
				for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
				{
					SGridCont.sInstAttr[k][0] = Str2Int(strSubList[sTCount + k * 2]);
					SGridCont.sInstAttr[k][1] = Str2Int(strSubList[sTCount + k * 2 + 1]);
					lnCheckSum += (SGridCont.sInstAttr[k][0] + SGridCont.sInstAttr[k][1]);
				}
			}
			pKitbag->Push(&SGridCont, sGridID, i, true, true);
		}
	}
	if (!bIsOldVer)
	{
		char	szCheckSum[64];
		sprintf(szCheckSum, "%d", lnCheckSum);
		if (strncmp(szCheckSum, strList[sSegID++].c_str(), 64))
			return false;
	}
	else
		pKitbag->SetVer(114);

	return true;
}

bool SItemGrid2String(std::string& r, /* ???????? */ long& lnCheckSum, /* ?????????? */ SItemGrid* pGridCont, /* ??? */ int iOrder /* ?????? */)
{
	//	???????????????????
	static	char	szData[256];

	//	???????????????????
	_snprintf(szData, 256, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		iOrder,
		pGridCont->sID,
		pGridCont->sNum,
		pGridCont->sEndure[0],
		pGridCont->sEndure[1],
		pGridCont->sEnergy[0],
		pGridCont->sEnergy[1],
		pGridCont->chForgeLv,
		pGridCont->dwDBID,
		pGridCont->sNeedLv,
		pGridCont->bIsLock,
		pGridCont->bItemTradable,
		pGridCont->expiration
	);

	r += szData;

	//	?????????
	lnCheckSum +=
		pGridCont->sID +
		pGridCont->sNum +
		pGridCont->sEndure[0] +
		pGridCont->sEndure[1] +
		pGridCont->sEnergy[0] +
		pGridCont->sEnergy[1] +
		pGridCont->chForgeLv +
		pGridCont->dwDBID +
		pGridCont->sNeedLv +
		pGridCont->bIsLock +
		pGridCont->bItemTradable;
	//pGridCont->expiration;

//	???DB??????
	for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
	{
		//	???DB??????
		_snprintf(szData, 1, ",%d", pGridCont->GetDBParam(m));
		r += szData;

		//	?????????
		lnCheckSum += pGridCont->GetDBParam(m);

		if (pGridCont->IsInstAttrValid())
		{
			r += ",1";

			for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
			{
				_snprintf(szData, 256, ",%d,%d", pGridCont->sInstAttr[k][0], pGridCont->sInstAttr[k][1]);
				r += szData;

				lnCheckSum += pGridCont->sInstAttr[k][0] + pGridCont->sInstAttr[k][1];
			}
		}
		else
		{
			r += ",0";
		}

	};

	//
	return	true;
}

bool String2SItemGrid(SItemGrid* pGridCont, long& lnCheckSum, const std::string& sData, int iVer, bool bIsOldVer)
{
	//	??????????
	const short csSubNum = 8 + enumITEMDBP_MAXNUM + defITEM_INSTANCE_ATTR_NUM_VER110 * 2 + 1 + 1;

	std::string strSubList[csSubNum];

	Util_ResolveTextLine(sData.c_str(), strSubList, csSubNum, ',');

	//	?????????????
	short	sTCount = 0;
	short	sGridID = Str2Int(strSubList[sTCount++]);
	pGridCont->sID = Str2Int(strSubList[sTCount++]);
	pGridCont->sNum = Str2Int(strSubList[sTCount++]);
	pGridCont->sEndure[0] = Str2Int(strSubList[sTCount++]);
	pGridCont->sEndure[1] = Str2Int(strSubList[sTCount++]);
	pGridCont->sEnergy[0] = Str2Int(strSubList[sTCount++]);
	pGridCont->sEnergy[1] = Str2Int(strSubList[sTCount++]);
	pGridCont->chForgeLv = Str2Int(strSubList[sTCount++]);

	if (iVer == 114)
	{
		pGridCont->dwDBID = Str2Int(strSubList[sTCount++]);
		pGridCont->sNeedLv = Str2Int(strSubList[sTCount++]);
		pGridCont->bIsLock = Str2Int(strSubList[sTCount++]);
	}
	else
	{
		pGridCont->dwDBID = 0;
		pGridCont->sNeedLv = 0;
		pGridCont->bIsLock = 0;

	}
	pGridCont->bItemTradable = Str2Int(strSubList[sTCount++]);
	pGridCont->expiration = Str2Int(strSubList[sTCount++]);

	//	?????????
	lnCheckSum +=
		pGridCont->sID +
		pGridCont->sNum +
		pGridCont->sEndure[0] +
		pGridCont->sEndure[1] +
		pGridCont->sEnergy[0] +
		pGridCont->sEnergy[1] +
		pGridCont->chForgeLv +
		pGridCont->dwDBID +
		pGridCont->sNeedLv +
		pGridCont->bIsLock +
		pGridCont->bItemTradable;
	//pGridCont->expiration;


	//	??DB?????
	for (int m = 0; m < enumITEMDBP_MAXNUM; m++)
	{
		pGridCont->SetDBParam(m, Str2Int(strSubList[sTCount++]));
		lnCheckSum += pGridCont->GetDBParam(m);
	}

	//	??????????
	if (!bIsOldVer && (iVer >= 113 || iVer == 112)) // ???????????????????
	{
		if (Str2Int(strSubList[sTCount++]) > 0) // ???????????
		{
			for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
			{
				pGridCont->sInstAttr[k][0] = Str2Int(strSubList[sTCount + k * 2]);
				pGridCont->sInstAttr[k][1] = Str2Int(strSubList[sTCount + k * 2 + 1]);
				lnCheckSum += (pGridCont->sInstAttr[k][0] + pGridCont->sInstAttr[k][1]);
			}
		}
		else
			pGridCont->SetInstAttrInvalid();
	}
	else
	{
		for (int k = 0; k < defITEM_INSTANCE_ATTR_NUM; k++)
		{
			pGridCont->sInstAttr[k][0] = Str2Int(strSubList[sTCount + k * 2]);
			pGridCont->sInstAttr[k][1] = Str2Int(strSubList[sTCount + k * 2 + 1]);
			lnCheckSum += (pGridCont->sInstAttr[k][0] + pGridCont->sInstAttr[k][1]);
		}
	}

	//	*
	return	true;
}

short CKitbag::Regroup(short sSrcPosID, short sSrcNum, short sTarPosID, short sType)
{
	short	sRet;
	SItemGrid	SGridCont;

	//??????????
	//if(IsPwdLocked())
	//{
		//sRet = enumKBACT_ERROR_LOCK;
		//goto Error;
	//}

	if (m_bLock)
	{
		sRet = enumKBACT_ERROR_LOCK;
		goto Error;
	}
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//{
	//	sRet = enumKBACT_ERROR_RANGE;
	//	goto Error;
	//}
	if (sSrcPosID < 0 || sSrcPosID >= m_sCapacity
		|| sTarPosID < 0 || sTarPosID >= m_sCapacity)
	{
		sRet = enumKBACT_ERROR_RANGE;
		goto Error;
	}

	{
		SItemGrid& pSSrcCont = m_SItem[sType][sSrcPosID].SContent;
		SItemGrid& pSTarCont = m_SItem[sType][sTarPosID].SContent;
		if (pSSrcCont.sID == 0)
		{
			sRet = enumKBACT_ERROR_NULLGRID;
			goto Error;
		}

		if (pSSrcCont.sID == pSTarCont.sID) // ???????
		{
			CItemRecord* pCItem;
			pCItem = GetItemRecordInfo(pSSrcCont.sID);
			if (!pCItem)
			{
				sRet = enumKBACT_ERROR_PUSHITEMID;
				goto Error;
			}
			short	sPileNum = (short)pCItem->nPileMax;
			if (sPileNum < 1) // ????????????1
			{
				sRet = enumKBACT_ERROR_PUSHITEMID;
				goto Error;
			}

			short	sFreeNum = sPileNum - pSTarCont.sNum;
			long SrcExpiration = pSSrcCont.expiration;
			long TarExpiration = pSTarCont.expiration;
			bool SrcTradable = pSSrcCont.bItemTradable;
			bool TarTradable = pSTarCont.bItemTradable;

			if (sFreeNum > 0 && (!SrcExpiration && !TarExpiration) && (SrcTradable == TarTradable) ) // ????????????
			{
				SetSingleChangeFlag(sSrcPosID, sType);
				SetSingleChangeFlag(sTarPosID, sType);

				if (sSrcNum < 0) sSrcNum = 0;
				else if (sSrcNum == 0 || sSrcNum > pSSrcCont.sNum) sSrcNum = pSSrcCont.sNum;
				if (sSrcNum > sFreeNum) sSrcNum = sFreeNum;

				pSSrcCont.sNum -= sSrcNum;
				pSTarCont.sNum += sSrcNum;
				if (pSSrcCont.sNum == 0)
					sRet = Clear(sSrcPosID, sType);
				else
					sRet = enumKBACT_SUCCESS;
			}
			else // ???????????
				sRet = Switch(sSrcPosID, sTarPosID, sType);
		}
		else if (pSTarCont.sID == 0)
		{
			if (sSrcNum < 0) sSrcNum = 0;
			else if (sSrcNum == 0 || sSrcNum > pSSrcCont.sNum) sSrcNum = pSSrcCont.sNum;

			if (sSrcNum > 0)
			{
				SetSingleChangeFlag(sSrcPosID, sType);
				SetSingleChangeFlag(sTarPosID, sType);

				SItemGrid	SSrcCont = pSSrcCont;
				SSrcCont.sNum = sSrcNum;
				sRet = Push(&SSrcCont, sTarPosID, sType);
				pSSrcCont.sNum -= (sSrcNum - SSrcCont.sNum);
				if (sRet != enumKBACT_SUCCESS)
					goto Error;
				if (pSSrcCont.sNum == 0)
					sRet = Clear(sSrcPosID, sType);
				else
					sRet = enumKBACT_SUCCESS;
			}
		}
		else // ???????????
		{
			sRet = Switch(sSrcPosID, sTarPosID, sType);
		}
	}

Error:
	CheckValid();
	return sRet;
}

short CKitbag::Refresh(short sPosID, short sType)
{
	short	sRet;

	//??????????
	//if(IsPwdLocked())
	//{
		//sRet = enumKBACT_ERROR_LOCK;
		//goto Error;
	//}

	if (m_bLock)
	{
		sRet = enumKBACT_ERROR_LOCK;
		goto Error;
	}
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//{
	//	sRet = enumKBACT_ERROR_RANGE;
	//	goto Error;
	//}
	if (sPosID < 0 || sPosID >= m_sCapacity)
		sRet = enumKBACT_ERROR_RANGE;

	if (m_SItem[sType][sPosID].SContent.sID != 0 && m_SItem[sType][sPosID].SContent.sNum == 0)
	{
		SetSingleChangeFlag(sPosID, sType);
		m_SItem[sType][sPosID].SContent.sID = 0;

		m_sUseNum[sType]--;
		SItemUnit* pTempUnit = &m_SItem[sType][sPosID];
		m_pSItem[sType][pTempUnit->sReverseID] = m_pSItem[sType][m_sUseNum[sType]];
		m_pSItem[sType][pTempUnit->sReverseID]->sReverseID = pTempUnit->sReverseID;
		m_pSItem[sType][m_sUseNum[sType]] = pTempUnit;
		m_pSItem[sType][m_sUseNum[sType]]->sReverseID = m_sUseNum[sType];
	}

	sRet = enumKBACT_SUCCESS;

Error:
	CheckValid();
	return sRet;
}

//TODO(Ogge): Check if we can make pGrid a reference instead
short CKitbag::Push(SItemGrid* pGrid, short& sPosID, short sType, bool bCommit, bool bSureOpr)
{
	short	sRet;

	short	sPushPos = sPosID;

	//??????????
	//if(IsPwdLocked())
	//{
		//sRet = enumKBACT_ERROR_LOCK;
		//goto Error;
	//}

	if (m_bLock)
	{
		sRet = enumKBACT_ERROR_LOCK;
		goto Error;
	}
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//{
	//	sRet = enumKBACT_ERROR_RANGE;
	//	goto Error;
	//}

	if (pGrid->sNum <= 0) // ??????
	{
		sRet = enumKBACT_ERROR_POPNUM;
		goto Error;
	}

	{
		short	sPileNum = pGrid->sNum;
		if (!bSureOpr)
		{
			CItemRecord* pCItem;
			pCItem = GetItemRecordInfo(pGrid->sID);
			if (!pCItem)
			{
				sRet = enumKBACT_ERROR_PUSHITEMID;
				goto Error;
			}
			sPileNum = (short)pCItem->nPileMax;
		}

		{
			short	sLeftNum = pGrid->sNum;
			short	sFreeNum = 0;

			if (sPileNum < 1) // ????????????1
			{
				sRet = enumKBACT_ERROR_PUSHITEMID;
				goto Error;
			}

			if (sPosID >= 0 && sPosID < m_sCapacity) // ??????
			{
				if (bCommit)
				{

					bool target_slot_empty{ false };

					if (m_SItem[sType][sPosID].SContent.sID == 0) // Push slot is empty..
					{
						target_slot_empty = true;
						// copy pGrid item to sPosID slot
						m_SItem[sType][sPosID].SContent = *pGrid;

						m_SItem[sType][sPosID].SContent.sNum = 0;

						SItemUnit* pTempUnit = &m_SItem[sType][sPosID];
						m_pSItem[sType][pTempUnit->sReverseID] = m_pSItem[sType][m_sUseNum[sType]];
						m_pSItem[sType][pTempUnit->sReverseID]->sReverseID = pTempUnit->sReverseID;
						m_pSItem[sType][m_sUseNum[sType]] = pTempUnit;
						m_pSItem[sType][m_sUseNum[sType]]->sReverseID = m_sUseNum[sType];
						m_sUseNum[sType]++;
					}
					sFreeNum = sPileNum - m_SItem[sType][sPosID].SContent.sNum;


					auto is_acceptable = [&]() -> bool
					{
						if (target_slot_empty)
							return true;

						const long SrcExpiration = pGrid->expiration;
						const long TarExpiration = m_SItem[sType][sPosID].SContent.expiration;
						const bool SrcTradable = pGrid->bItemTradable;
						const bool TarTradable = m_SItem[sType][sPosID].SContent.bItemTradable;

						return !SrcExpiration && !TarExpiration &&
							SrcTradable == TarTradable;
					};

					if (m_SItem[sType][sPosID].SContent.sID == pGrid->sID && is_acceptable())
					{
						sFreeNum = sPileNum - m_SItem[sType][sPosID].SContent.sNum;
						if (sFreeNum == 0)
							sPushPos = -1;

						else if (sFreeNum > 0)
						{ 
							sLeftNum -= sFreeNum;
							if (sLeftNum <= 0)
							{
								m_SItem[sType][sPosID].SContent.sNum = sPileNum + sLeftNum;
								SetSingleChangeFlag(sPosID, sType);

								pGrid->sNum = 0;
								sRet = enumKBACT_SUCCESS;
								goto Error;
							}
							else
							{
								m_SItem[sType][sPosID].SContent.sNum = sPileNum;
								SetSingleChangeFlag(sPosID, sType);
							}
						}
					}
				}
				else
				{
					if (m_SItem[sType][sPosID].SContent.sID == 0)
						sLeftNum -= sPileNum;
					sFreeNum = sPileNum - m_SItem[sType][sPosID].SContent.sNum;
					long SrcExpiration = pGrid->expiration;
					long TarExpiration = m_SItem[sType][sPosID].SContent.expiration;
					bool SrcTradable = pGrid->bItemTradable;
					bool TarTradable = m_SItem[sType][sPosID].SContent.bItemTradable;
					if (m_SItem[sType][sPosID].SContent.sID == pGrid->sID && (!SrcExpiration && !TarExpiration) && (SrcTradable == TarTradable))
					{
					
						if (sFreeNum == 0)
							sPushPos = -1;
						else if (sFreeNum > 0)
						{
							sLeftNum -= sFreeNum;
						}
					}
					if (sLeftNum <= 0)
					{
						pGrid->sNum = 0;
						sRet = enumKBACT_SUCCESS;
						goto Error;
					}
				}
			}

			if (sPileNum > 1) // ?????????????????????????
			{
				for (short i = 0; i < m_sCapacity; i++)
				{
					sFreeNum = sPileNum - m_SItem[sType][i].SContent.sNum;
					long TarExpiration = m_SItem[sType][i].SContent.expiration;
					long SrcExpiration = pGrid->expiration;
					bool TarTradable = m_SItem[sType][i].SContent.bItemTradable;
					bool SrcTradable = pGrid->bItemTradable;

					if (m_SItem[sType][i].SContent.sID == pGrid->sID && (!SrcExpiration && !TarExpiration) && (SrcTradable == TarTradable)) // ???????????
					{
						

						if (sFreeNum > 0)
						{
							if (sPushPos < 0 || sPushPos >= m_sCapacity)
								sPushPos = i;
							sLeftNum -= sFreeNum;
							if (bCommit)
							{
								if (sLeftNum <= 0)
								{
									m_SItem[sType][i].SContent.sNum = sPileNum + sLeftNum;
									SetSingleChangeFlag(i, sType);

									pGrid->sNum = 0;
									sRet = enumKBACT_SUCCESS;
									goto Error;
								}
								else
								{
									m_SItem[sType][i].SContent.sNum = sPileNum;
									SetSingleChangeFlag(i, sType);
								}
							}
							else
							{
								if (sLeftNum <= 0)
								{
									pGrid->sNum = 0;
									sRet = enumKBACT_SUCCESS;
									goto Error;
								}
							}
						}
					}
				}
			}

			{
				short	sSearchPos = 0;
				while (sLeftNum > 0) // ???????û????£????????????
				{
					if (sSearchPos >= m_sCapacity)
					{
						pGrid->sNum = sLeftNum;

						sRet = enumKBACT_ERROR_FULL;
						goto Error;
					}
					if (m_SItem[sType][sSearchPos].SContent.sID == 0)
					{
						//if (sPushPos < 0 || sPushPos >= m_sCapacity)
						sPushPos = sSearchPos;

						sLeftNum -= sPileNum;
						if (bCommit)
						{
							SetSingleChangeFlag(sSearchPos, sType);
							memcpy(&m_SItem[sType][sSearchPos].SContent, pGrid, sizeof(SItemGrid));
							SItemUnit* pTempUnit = &m_SItem[sType][sSearchPos];
							m_pSItem[sType][pTempUnit->sReverseID] = m_pSItem[sType][m_sUseNum[sType]];
							m_pSItem[sType][pTempUnit->sReverseID]->sReverseID = pTempUnit->sReverseID;
							m_pSItem[sType][m_sUseNum[sType]] = pTempUnit;
							m_pSItem[sType][m_sUseNum[sType]]->sReverseID = m_sUseNum[sType];
							m_sUseNum[sType]++;

							if (sLeftNum <= 0)
							{
								m_SItem[sType][sSearchPos].SContent.sNum = sPileNum + sLeftNum;
								pGrid->sNum = 0;
								sRet = enumKBACT_SUCCESS;
								goto Error;
							}
							else
							{
								m_SItem[sType][sSearchPos].SContent.sNum = sPileNum;
							}
						}
						else
						{
							if (sLeftNum <= 0)
							{
								pGrid->sNum = 0;
								sRet = enumKBACT_SUCCESS;
								goto Error;
							}
						}
					}
					sSearchPos++;
				}
			}
		}
	}

	sRet = enumKBACT_SUCCESS;

Error:
	CheckValid();
	sPosID = sPushPos;
	return sRet;
}

bool CKitbag::AddCapacity(short sAddVal)
{
	if (sAddVal < 0)
		return false;

	short	sStartP = m_sCapacity;
	if (sAddVal > defMAX_KBITEM_NUM_PER_TYPE - m_sCapacity)
		m_sCapacity = defMAX_KBITEM_NUM_PER_TYPE;
	else
		m_sCapacity += sAddVal;
	for (short i = 0; i < enumKBITEM_TYPE_NUM; i++)
	{
		for (short j = sStartP; j < m_sCapacity; j++)
			m_bChangeFlag[i][j] = false;
	}

	return true;
}

short CKitbag::GetUseGridNum(short sType /*= 0*/)
{
	return m_sUseNum[sType];
}

short CKitbag::CanPush(SItemGrid* pGrid, short& sPosID, short sType)
{
	return Push(pGrid, sPosID, sType, false);
}

short CKitbag::CanPop(SItemGrid* pGrid, short sPosID, short sType)
{
	return Pop(pGrid, sPosID, sType, false);
}

short CKitbag::Pop(SItemGrid* pGrid, short sPosID, short sType, bool bCommit)
{
	short	sRet;

	//??????????
	//if(IsPwdLocked())
	//{
		//sRet = enumKBACT_ERROR_LOCK;
		//goto Error;
	//}

	if (m_bLock)
	{
		sRet = enumKBACT_ERROR_LOCK;
		goto Error;
	}
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//{
	//	sRet = enumKBACT_ERROR_RANGE;
	//	goto Error;
	//}
	if (sPosID < 0 || sPosID >= m_sCapacity)
	{
		sRet = enumKBACT_ERROR_RANGE;
		goto Error;
	}

	if (m_SItem[sType][sPosID].SContent.sID <= 0) // ?????
	{
		sRet = enumKBACT_ERROR_NULLGRID;
		goto Error;
	}

	if (pGrid->sNum < 0 || pGrid->sNum > m_SItem[sType][sPosID].SContent.sNum) // ??????
	{
		sRet = enumKBACT_ERROR_POPNUM;
		goto Error;
	}

	{
		short	sPopNum = pGrid->sNum;
		memcpy(pGrid, &m_SItem[sType][sPosID].SContent, sizeof(SItemGrid));

		if (sPopNum == 0 || sPopNum == m_SItem[sType][sPosID].SContent.sNum)
		{
			pGrid->sNum = m_SItem[sType][sPosID].SContent.sNum;
			if (bCommit)
				m_SItem[sType][sPosID].SContent.sID = 0;
		}
		else
		{
			pGrid->sNum = sPopNum;
			if (bCommit)
				m_SItem[sType][sPosID].SContent.sNum -= sPopNum;
		}

		if (bCommit)
		{
			SetSingleChangeFlag(sPosID, sType);
			if (m_SItem[sType][sPosID].SContent.sID == 0)
			{
				m_sUseNum[sType]--;
				SItemUnit* pTempUnit = &m_SItem[sType][sPosID];
				m_pSItem[sType][pTempUnit->sReverseID] = m_pSItem[sType][m_sUseNum[sType]];
				m_pSItem[sType][pTempUnit->sReverseID]->sReverseID = pTempUnit->sReverseID;
				m_pSItem[sType][m_sUseNum[sType]] = pTempUnit;
				m_pSItem[sType][m_sUseNum[sType]]->sReverseID = m_sUseNum[sType];
			}
		}
	}

	sRet = enumKBACT_SUCCESS;

Error:
	CheckValid();
	return sRet;
}

short CKitbag::Clear(short sPosID, short sType)
{
	short	sRet;

	//??????????
	//if(IsPwdLocked())
	//{
		//sRet = enumKBACT_ERROR_LOCK;
		//goto Error;
	//}

	if (m_bLock)
	{
		sRet = enumKBACT_ERROR_LOCK;
		goto Error;
	}
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//{
	//	sRet = enumKBACT_ERROR_RANGE;
	//	goto Error;
	//}
	if (sPosID < 0 || sPosID >= m_sCapacity)
		sRet = enumKBACT_ERROR_RANGE;

	// ????????????
	if (m_SItem[sType][sPosID].SContent.sID != 0)
	{
		SetSingleChangeFlag(sPosID, sType);
		m_SItem[sType][sPosID].SContent.sID = 0;

		m_sUseNum[sType]--;
		SItemUnit* pTempUnit = &m_SItem[sType][sPosID];
		m_pSItem[sType][pTempUnit->sReverseID] = m_pSItem[sType][m_sUseNum[sType]];
		m_pSItem[sType][pTempUnit->sReverseID]->sReverseID = pTempUnit->sReverseID;
		m_pSItem[sType][m_sUseNum[sType]] = pTempUnit;
		m_pSItem[sType][m_sUseNum[sType]]->sReverseID = m_sUseNum[sType];
	}

	sRet = enumKBACT_SUCCESS;

Error:
	CheckValid();
	return sRet;
}

short CKitbag::Clear(SItemGrid* pGrid, short sNum, short* psPosID)
{
	short	sRet;

	//??????????
	//if(IsPwdLocked())
	//{
		//sRet = enumKBACT_ERROR_LOCK;
		//goto Error;
	//}

	if (m_bLock)
	{
		sRet = enumKBACT_ERROR_LOCK;
		goto Error;
	}
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//{
	//	sRet = enumKBACT_ERROR_RANGE;
	//	goto Error;
	//}
	if (!pGrid)
	{
		sRet = enumKBACT_ERROR_NULLGRID;
		goto Error;
	}
	if (sNum == 0)
		sNum = pGrid->sNum;
	if (sNum > pGrid->sNum)
	{
		sRet = enumKBACT_ERROR_POPNUM;
		goto Error;
	}
	for (short i = 0; i < enumKBITEM_TYPE_NUM; i++)
	{
		for (short k = 0; k < m_sUseNum[i]; k++)
		{
			if (pGrid == GetGridContByNum(k, i))
			{
				if (psPosID)
					*psPosID = GetPosIDByNum(k, i);
				if (sNum == pGrid->sNum)
				{
					sRet = Clear(GetPosIDByNum(k, i), i);
					goto Error;
				}
				else
				{
					sRet = enumKBACT_SUCCESS;
					pGrid->sNum -= sNum;
					SetSingleChangeFlag(GetPosIDByNum(k, i), i);
					goto Error;
				}
			}
		}
	}

	sRet = enumKBACT_ERROR_NULLGRID;

Error:
	return sRet;
}

short CKitbag::Switch(short sSrcPosID, short sTarPosID, short sType)
{
	short	sRet;
	SItemGrid	SGridCont;

	//??????????
	//if(IsPwdLocked())
	//{
		//sRet = enumKBACT_ERROR_LOCK;
		//goto Error;
	//}

	if (m_bLock)
	{
		sRet = enumKBACT_ERROR_LOCK;
		goto Error;
	}
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//{
	//	sRet = enumKBACT_ERROR_RANGE;
	//	goto Error;
	//}
	if (sSrcPosID < 0 || sSrcPosID >= m_sCapacity
		|| sTarPosID < 0 || sTarPosID >= m_sCapacity)
	{
		sRet = enumKBACT_ERROR_RANGE;
		goto Error;
	}

	if (m_SItem[sType][sSrcPosID].SContent.sID == 0 && m_SItem[sType][sTarPosID].SContent.sID == 0)
	{
		sRet = enumKBACT_SUCCESS;
		goto Error;
	}

	SGridCont = m_SItem[sType][sTarPosID].SContent;
	m_SItem[sType][sTarPosID].SContent = m_SItem[sType][sSrcPosID].SContent;
	m_SItem[sType][sSrcPosID].SContent = SGridCont;

	short	sSrcRevs, sTarRevs;
	sSrcRevs = m_SItem[sType][sSrcPosID].sReverseID;
	sTarRevs = m_SItem[sType][sTarPosID].sReverseID;
	m_pSItem[sType][sSrcRevs] = &m_SItem[sType][sTarPosID];
	m_SItem[sType][sTarPosID].sReverseID = sSrcRevs;
	m_pSItem[sType][sTarRevs] = &m_SItem[sType][sSrcPosID];
	m_SItem[sType][sSrcPosID].sReverseID = sTarRevs;

	SetSingleChangeFlag(sSrcPosID, sType);
	SetSingleChangeFlag(sTarPosID, sType);

	sRet = enumKBACT_SUCCESS;

Error:
	CheckValid();
	return sRet;
}

bool CKitbag::IsFull(short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return enumKBACT_ERROR_RANGE;

	return m_sUseNum[sType] >= m_sCapacity;
}

SItemGrid* CKitbag::GetGridContByID(short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return 0;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return 0;

	if (m_SItem[sType][sPosID].SContent.sID <= 0)
		return 0;

	return &m_SItem[sType][sPosID].SContent;
}

SItemGrid* CKitbag::GetGridContByNum(short sPosNum, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return 0;
	if (sPosNum < 0 || sPosNum >= m_sUseNum[sType])
		return 0;

	if (m_pSItem[sType][sPosNum]->SContent.sID <= 0)
		return 0;

	return &m_pSItem[sType][sPosNum]->SContent;
}

short CKitbag::GetPosIDByNum(short sPosNum, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return -1;
	if (sPosNum < 0 || sPosNum >= m_sUseNum[sType])
		return -1;

	return m_pSItem[sType][sPosNum]->sPosID;
}

bool CKitbag::GetPosIDByGrid(SItemGrid* pGrid, short* psPosID, short* psType)
{
	if (!pGrid)
		return false;
	for (short i = 0; i < enumKBITEM_TYPE_NUM; i++)
	{
		for (short k = 0; k < m_sUseNum[i]; k++)
		{
			if (pGrid == GetGridContByNum(k, i))
			{
				if (psPosID)
					*psPosID = GetPosIDByNum(k, i);
				if (psType)
					*psType = i;
				return true;
			}
		}
	}

	return false;
}

short CKitbag::GetID(short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return enumKBACT_ERROR_RANGE;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return enumKBACT_ERROR_RANGE;

	return m_SItem[sType][sPosID].SContent.sID;
}

short CKitbag::GetNum(short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return enumKBACT_ERROR_RANGE;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return enumKBACT_ERROR_RANGE;

	if (m_SItem[sType][sPosID].SContent.sID == 0)
		return enumKBACT_ERROR_NULLGRID;
	return m_SItem[sType][sPosID].SContent.sNum;
}

long CKitbag::GetDBParam(short sParamID, short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return enumKBACT_ERROR_RANGE;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return enumKBACT_ERROR_RANGE;
	if (sParamID < 0 || sParamID >= enumITEMDBP_MAXNUM)
		return enumKBACT_ERROR_RANGE;

	if (m_SItem[sType][sPosID].SContent.sID == 0)
		return enumKBACT_ERROR_NULLGRID;

	return m_SItem[sType][sPosID].SContent.GetDBParam(sParamID);
}

short CKitbag::SetDBParam(short sParamID, long lParamVal, short sPosID, short sType)
{
	//if(IsPwdLocked())
	//return enumKBACT_ERROR_LOCK;
	if (m_bLock)
		return enumKBACT_ERROR_LOCK;

	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return enumKBACT_ERROR_RANGE;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return enumKBACT_ERROR_RANGE;

	if (sParamID < 0 || sParamID >= enumITEMDBP_MAXNUM)
		return enumKBACT_ERROR_RANGE;

	if (m_SItem[sType][sPosID].SContent.sID == 0)
		return enumKBACT_ERROR_NULLGRID;

	m_SItem[sType][sPosID].SContent.SetDBParam(sParamID, lParamVal);

	return enumKBACT_SUCCESS;
}

short CKitbag::GetEnergy(bool bMax, short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return enumKBACT_ERROR_RANGE;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return enumKBACT_ERROR_RANGE;

	if (m_SItem[sType][sPosID].SContent.sID == 0)
		return enumKBACT_ERROR_NULLGRID;

	if (bMax)
		return m_SItem[sType][sPosID].SContent.sEnergy[1];
	else
		return m_SItem[sType][sPosID].SContent.sEnergy[0];
}

short CKitbag::SetEnergy(bool bMax, short sEnergy, short sPosID, short sType)
{
	//if(IsPwdLocked())
	//return enumKBACT_ERROR_LOCK;
	if (m_bLock)
		return enumKBACT_ERROR_LOCK;

	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return enumKBACT_ERROR_RANGE;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return enumKBACT_ERROR_RANGE;

	if (m_SItem[sType][sPosID].SContent.sID == 0)
		return enumKBACT_ERROR_NULLGRID;

	if (bMax)
		m_SItem[sType][sPosID].SContent.sEnergy[1] = sEnergy;
	else
		m_SItem[sType][sPosID].SContent.sEnergy[0] = sEnergy;

	return enumKBACT_SUCCESS;
}

BOOL CKitbag::HasItem(short sPosID, short sType)
{
	return GetID(sPosID, sType) > 0;
}

BOOL CKitbag::IsEnable(short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return FALSE;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return FALSE;

	return !(m_SItem[sType][sPosID].byState & ITEM_DISENABLE);
}

void CKitbag::Enable(short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return ;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return;

	m_SItem[sType][sPosID].byState &= ~ITEM_DISENABLE;
}

void CKitbag::Disable(short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return ;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return;

	m_SItem[sType][sPosID].byState |= ITEM_DISENABLE;
}

BOOL CKitbag::IsLock(void)
{
	return m_bLock;
}

void CKitbag::SetSingleChangeFlag(short sPosID, short sType)
{
	m_bChangeFlag[sType][sPosID] = true;
	m_sChangeNum[sType]++;
}

bool CKitbag::IsSingleChange(short sPosID, short sType)
{
	//if (sType < 0 || sType >= enumKBITEM_TYPE_NUM)
	//	return false;
	if (sPosID < 0 || sPosID >= m_sCapacity)
		return false;

	if (m_bChangeFlag[sType][sPosID])
		return true;
	bool	bGridAttrChg = false;
	SItemGrid* pSGrid = GetGridContByID(sPosID, sType);
	if (pSGrid)
		bGridAttrChg = pSGrid->IsChange();
	if (bGridAttrChg)
		SetSingleChangeFlag(sPosID, sType);
	return bGridAttrChg;
}

bool CKitbag::IsChange(short sType)
{
	if (m_sChangeNum[sType] > 0)
		return true;

	SItemGrid* pSGrid;
	for (short i = 0; i < m_sUseNum[sType]; i++)
	{
		pSGrid = GetGridContByNum(i, sType);
		if (pSGrid && pSGrid->IsChange())
			return true;
	}

	return false;
}

short CKitbag::GetChangeNum(short sType)
{
	m_sChangeNum[sType] = 0;
	for (short i = 0; i < m_sUseNum[sType]; i++)
	{
		if (IsSingleChange(GetPosIDByNum(i, sType), sType))
			m_sChangeNum[sType]++;
	}

	return m_sChangeNum[sType];
}

void CKitbag::Lock()
{
	m_bLock = TRUE;
}

void CKitbag::UnLock()
{
	m_bLock = FALSE;
}

void CKitbag::PwdLock()
{
	m_bPwdLocked |= 0x01;
}

void CKitbag::PwdUnlock()
{
	m_bPwdLocked &= 0xfe;
}

BOOL CKitbag::IsPwdLocked()
{
	if (m_bPwdLocked & 0x01)
		return true;
	else
		return false;
}

BOOL CKitbag::IsPwdAutoLocked()
{
	if (m_bPwdLocked & 0x02)
		return true;
	else
		return false;
}

void CKitbag::PwdAutoLock(char cAuto)
{
	if (cAuto == 0)
		m_bPwdLocked &= 0xfd;
	else
		m_bPwdLocked |= 0x02;
}

int CKitbag::GetPwdLockState()
{
	return m_bPwdLocked;
}

void CKitbag::SetPwdLockState(int nLock)
{
	m_bPwdLocked = nLock;
}

void CKitbag::SetVer(short sVers)
{
	sVer = sVers;
}

short CKitbag::GetVer(void)
{
	return sVer;
}

void CKitbag::SetChangeFlag(bool bChange, short sType)
{
	SItemGrid* pSGrid;
	if (sType < 0)
	{
		for (short i = 0; i < enumKBITEM_TYPE_NUM; i++)
		{
			for (short j = 0; j < m_sCapacity; j++)
				m_bChangeFlag[i][j] = bChange;
			if (bChange)
				m_sChangeNum[i] = m_sCapacity;
			else
				m_sChangeNum[i] = 0;

			for (short k = 0; k < m_sUseNum[i]; k++)
			{
				pSGrid = GetGridContByNum(k, i);
				if (pSGrid)
					pSGrid->SetChange(bChange);
			}
		}
	}
	else if (sType < enumKBITEM_TYPE_NUM)
	{
		for (short j = 0; j < m_sCapacity; j++)
			m_bChangeFlag[sType][j] = bChange;
		if (bChange)
			m_sChangeNum[sType] = m_sCapacity;
		else
			m_sChangeNum[sType] = 0;

		for (short k = 0; k < m_sUseNum[sType]; k++)
		{
			pSGrid = GetGridContByNum(k, sType);
			if (pSGrid)
				pSGrid->SetChange(bChange);
		}
	}
}

CKitbag& CKitbag::operator=(CKitbag& bag)
{
	sVer = bag.sVer;
	m_bLock = bag.m_bLock;
	m_bPwdLocked = bag.m_bPwdLocked;
	m_sCapacity = bag.m_sCapacity;
	memcpy(m_pSItem, bag.m_pSItem, sizeof(SItemUnit*) * enumKBITEM_TYPE_NUM * defMAX_KBITEM_NUM_PER_TYPE);
	memcpy(m_sUseNum, bag.m_sUseNum, sizeof(short) * enumKBITEM_TYPE_NUM);
	memcpy(m_SItem, bag.m_SItem, sizeof(SItemUnit) * enumKBITEM_TYPE_NUM * defMAX_KBITEM_NUM_PER_TYPE);
	memcpy(m_sChangeNum, bag.m_sChangeNum, sizeof(short) * enumKBITEM_TYPE_NUM);
	memcpy(m_bChangeFlag, bag.m_bChangeFlag, sizeof(bool) * enumKBITEM_TYPE_NUM * defMAX_KBITEM_NUM_PER_TYPE);

	return *this;
}

void CKitbag::Reset(void)
{
	m_bLock = FALSE;

	for (short i = 0; i < enumKBITEM_TYPE_NUM; i++)
	{
		m_sUseNum[i] = 0;
		for (short j = 0; j < defMAX_KBITEM_NUM_PER_TYPE; j++)
		{
			m_pSItem[i][j] = m_SItem[i] + j;
			m_SItem[i][j].sPosID = j;
			m_SItem[i][j].sReverseID = j;
			m_SItem[i][j].SContent.sID = 0;
			Enable(j, i);
		}
	}
	SetChangeFlag(false, -1);
}

void CKitbag::SetCapacity(short sCapacity)
{
	if (sCapacity < 0)
		sCapacity = 0;
	if (sCapacity > defMAX_KBITEM_NUM_PER_TYPE)
		sCapacity = defMAX_KBITEM_NUM_PER_TYPE;

	m_sCapacity = sCapacity;
}

short CKitbag::GetCapacity()
{
	return m_sCapacity;
}

bool CKitbag::CheckValid(void)
{
	short	sUseNum = GetUseGridNum();
	short	sFactNum = 0;

	for (short i = 0; i < m_sCapacity; i++)
	{
		if (m_SItem[0][i].SContent.sID > 0)
			sFactNum++;
	}

	if (sFactNum != sUseNum)
	{
		//LG("??????????", "??????????????????????");
		ToLogService("common", "Grid num can't match!");
		return false;
	}

	return true;
}

CKitbag::CKitbag()
{
	Init(24); m_bPwdLocked = 0;
}

void CKitbag::Init(short sCapacity)
{
	sVer = 0;
	m_bLock = FALSE;
	SetCapacity(sCapacity);

	for (short i = 0; i < enumKBITEM_TYPE_NUM; i++)
	{
		m_sUseNum[i] = 0;
		for (short j = 0; j < defMAX_KBITEM_NUM_PER_TYPE; j++)
		{
			m_pSItem[i][j] = m_SItem[i] + j;
			m_SItem[i][j].sPosID = j;
			m_SItem[i][j].sReverseID = j;
			m_SItem[i][j].SContent.sID = 0;
			Enable(j, i);
		}
	}
	SetChangeFlag(false, -1);
}
