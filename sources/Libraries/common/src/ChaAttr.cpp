//=============================================================================
// FileName: ChaAttr.cpp
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CChaAttr class
//=============================================================================

#include "ChaAttr.h"
#include "CharacterRecord.h"
#include "CompCommand.h"

LONG32 g_lMaxChaAttr[ATTR_MAX_NUM];


CChaAttr::CChaAttr()
{
	Clear();
}

CChaAttr::~CChaAttr()
{
}

void CChaAttr::Clear()
{	
	memset(m_lAttribute, 0, sizeof(LONG32) * ATTR_MAX_NUM);
}

void CChaAttr::Init(long lID, bool bFromFile)
{
	ResetChangeFlag();

	int i = 0;
	for (i = ATTR_ITEMC_STR; i <= ATTR_ITEMC_PDEF; i++)
		m_lAttribute[i] = 1000;
	for (i = ATTR_STATEC_STR; i <= ATTR_STATEC_PDEF; i++)
		m_lAttribute[i] = 1000;
	for (i = ATTR_ITEMV_STR; i <= ATTR_ITEMV_PDEF; i++)
		m_lAttribute[i] = 0;
	for (i = ATTR_STATEV_STR; i <= ATTR_STATEV_PDEF; i++)
		m_lAttribute[i] = 0;
	for (i = ATTR_BOAT_SKILLC_MNATK; i <= ATTR_BOAT_SKILLC_MXSPLY; i++)
		m_lAttribute[i] = 1000;
	for (i = ATTR_BOAT_SKILLV_MNATK; i <= ATTR_BOAT_SKILLV_MXSPLY; i++)
		m_lAttribute[i] = 0;

	// Add by lark.li 20080723 begin
	// 
	//for (i = ATTR_EXTEND0; i <= ATTR_EXTEND9; i++)
	//	m_lAttribute[i] = 0;
	// End

	CChaRecord *pCChaRecord = GetChaRecordInfo(lID);
	if (!pCChaRecord)
		return;

	m_lID = lID;
	if (bFromFile)
	{
		m_lAttribute[ATTR_LV] = (LONG32)pCChaRecord->lLv;
		m_lAttribute[ATTR_HP] = (LONG32)pCChaRecord->lMxHp;
		m_lAttribute[ATTR_SP] = (LONG32)pCChaRecord->lMxSp;
		m_lAttribute[ATTR_CEXP] = pCChaRecord->lCExp;
		m_lAttribute[ATTR_NLEXP] = pCChaRecord->lNExp;
		m_lAttribute[ATTR_FAME] = (LONG32)pCChaRecord->lFame;
		m_lAttribute[ATTR_AP] = (LONG32)pCChaRecord->lAp;
		m_lAttribute[ATTR_TP] = (LONG32)pCChaRecord->lTp;
		m_lAttribute[ATTR_GD] = (LONG32)pCChaRecord->lGd;
	}
	m_lAttribute[ATTR_SPRI] = (LONG32)pCChaRecord->lSpri;
	m_lAttribute[ATTR_CHATYPE] = (LONG32)pCChaRecord->chCtrlType;
	m_lAttribute[ATTR_STR] = (LONG32)pCChaRecord->lStr;
	m_lAttribute[ATTR_DEX] = (LONG32)pCChaRecord->lDex;
	m_lAttribute[ATTR_AGI] = (LONG32)pCChaRecord->lAgi;
	m_lAttribute[ATTR_CON] = (LONG32)pCChaRecord->lCon;
	m_lAttribute[ATTR_STA] = (LONG32)pCChaRecord->lSta;
	m_lAttribute[ATTR_LUK] = (LONG32)pCChaRecord->lLuk;
	m_lAttribute[ATTR_LHAND_ITEMV] = (LONG32)pCChaRecord->lLHandVal;
	m_lAttribute[ATTR_MXHP] = (LONG32)pCChaRecord->lMxHp;
	m_lAttribute[ATTR_MXSP] = (LONG32)pCChaRecord->lMxSp;
	m_lAttribute[ATTR_MNATK] = (LONG32)pCChaRecord->lMnAtk;
	m_lAttribute[ATTR_MXATK] = (LONG32)pCChaRecord->lMxAtk;
	m_lAttribute[ATTR_DEF] = (LONG32)pCChaRecord->lDef;
	m_lAttribute[ATTR_HIT] = (LONG32)pCChaRecord->lHit;
	m_lAttribute[ATTR_FLEE] = (LONG32)pCChaRecord->lFlee;
	m_lAttribute[ATTR_MF] = (LONG32)pCChaRecord->lMf;
	m_lAttribute[ATTR_CRT] = (LONG32)pCChaRecord->lCrt;
	m_lAttribute[ATTR_HREC] = (LONG32)pCChaRecord->lHRec;
	m_lAttribute[ATTR_SREC] = (LONG32)pCChaRecord->lSRec;
	m_lAttribute[ATTR_ASPD] = (LONG32)pCChaRecord->lASpd;
	m_lAttribute[ATTR_ADIS] = (LONG32)pCChaRecord->lADis;
	m_lAttribute[ATTR_MSPD] = (LONG32)pCChaRecord->lMSpd;
	m_lAttribute[ATTR_COL] = (LONG32)pCChaRecord->lCol;
	m_lAttribute[ATTR_PDEF] = (LONG32)pCChaRecord->lPDef;

	if (bFromFile)
	{
		m_lAttribute[ATTR_BSTR] = (LONG32)pCChaRecord->lStr;
		m_lAttribute[ATTR_BDEX] = (LONG32)pCChaRecord->lDex;
		m_lAttribute[ATTR_BAGI] = (LONG32)pCChaRecord->lAgi;
		m_lAttribute[ATTR_BCON] = (LONG32)pCChaRecord->lCon;
		m_lAttribute[ATTR_BSTA] = (LONG32)pCChaRecord->lSta;
		m_lAttribute[ATTR_BLUK] = (LONG32)pCChaRecord->lLuk;

		m_lAttribute[ATTR_BMXHP] = (LONG32)pCChaRecord->lMxHp;
		m_lAttribute[ATTR_BMXSP] = (LONG32)pCChaRecord->lMxSp;
	}
	m_lAttribute[ATTR_BMNATK] = (LONG32)pCChaRecord->lMnAtk;
	m_lAttribute[ATTR_BMXATK] = (LONG32)pCChaRecord->lMxAtk;
	m_lAttribute[ATTR_BDEF] = (LONG32)pCChaRecord->lDef;
	m_lAttribute[ATTR_BHIT] = (LONG32)pCChaRecord->lHit;
	m_lAttribute[ATTR_BFLEE] = (LONG32)pCChaRecord->lFlee;
	m_lAttribute[ATTR_BMF] = (LONG32)pCChaRecord->lMf;
	m_lAttribute[ATTR_BCRT] = (LONG32)pCChaRecord->lCrt;
	m_lAttribute[ATTR_BHREC] = (LONG32)pCChaRecord->lHRec;
	m_lAttribute[ATTR_BSREC] = (LONG32)pCChaRecord->lSRec;
	m_lAttribute[ATTR_BASPD] = (LONG32)pCChaRecord->lASpd;
	m_lAttribute[ATTR_BADIS] = (LONG32)pCChaRecord->lADis;
	m_lAttribute[ATTR_BMSPD] = (LONG32)pCChaRecord->lMSpd;
	m_lAttribute[ATTR_BCOL] = (LONG32)pCChaRecord->lCol;
	m_lAttribute[ATTR_BPDEF] = (LONG32)pCChaRecord->lPDef;
}
