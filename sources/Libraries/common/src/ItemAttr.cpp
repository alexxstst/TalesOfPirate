//=============================================================================
// FileName: ItemAttr.cpp
// Creater: ZhangXuedong
// Date: 2004.12.28
// Comment: CItemAttr class
//=============================================================================

#include "ItemAttr.h"
#include "ItemRecord.h"

bool CItemAttr::Init(short sItemID)
{
	CItemRecord *pCItemRec;
	pCItemRec = GetItemRecordInfo(sItemID);
	if (!pCItemRec)
		return false;

	memset(m_sAttr, 0, sizeof(m_sAttr));
	m_sAttr[ITEMATTR_COE_STR]	= pCItemRec->sStrCoef;	
	m_sAttr[ITEMATTR_COE_AGI]	= pCItemRec->sAgiCoef;	
	m_sAttr[ITEMATTR_COE_DEX]	= pCItemRec->sDexCoef;	
	m_sAttr[ITEMATTR_COE_CON]	= pCItemRec->sConCoef;	
	m_sAttr[ITEMATTR_COE_STA]	= pCItemRec->sStaCoef;	
	m_sAttr[ITEMATTR_COE_LUK]	= pCItemRec->sLukCoef;	
	m_sAttr[ITEMATTR_COE_ASPD]	= pCItemRec->sASpdCoef;	
	m_sAttr[ITEMATTR_COE_ADIS]	= pCItemRec->sADisCoef;	
	m_sAttr[ITEMATTR_COE_MNATK]	= pCItemRec->sMnAtkCoef;	
	m_sAttr[ITEMATTR_COE_MXATK]	= pCItemRec->sMxAtkCoef;	
	m_sAttr[ITEMATTR_COE_DEF]	= pCItemRec->sDefCoef;	
	m_sAttr[ITEMATTR_COE_MXHP]	= pCItemRec->sMxHpCoef;	
	m_sAttr[ITEMATTR_COE_MXSP]	= pCItemRec->sMxSpCoef;	
	m_sAttr[ITEMATTR_COE_FLEE]	= pCItemRec->sFleeCoef;	
	m_sAttr[ITEMATTR_COE_HIT]	= pCItemRec->sHitCoef;	
	m_sAttr[ITEMATTR_COE_CRT]	= pCItemRec->sCrtCoef;	
	m_sAttr[ITEMATTR_COE_MF]	= pCItemRec->sMfCoef;	
	m_sAttr[ITEMATTR_COE_HREC]	= pCItemRec->sHRecCoef;	
	m_sAttr[ITEMATTR_COE_SREC]	= pCItemRec->sSRecCoef;	
	m_sAttr[ITEMATTR_COE_MSPD]	= pCItemRec->sMSpdCoef;	
	m_sAttr[ITEMATTR_COE_COL]	= pCItemRec->sColCoef;	
	m_sAttr[ITEMATTR_COE_PDEF]	= 0;	

	m_sAttr[ITEMATTR_VAL_STR]	= pCItemRec->sStrValu[0];	
	m_sAttr[ITEMATTR_VAL_AGI]	= pCItemRec->sAgiValu[0];	
	m_sAttr[ITEMATTR_VAL_DEX]	= pCItemRec->sDexValu[0];	
	m_sAttr[ITEMATTR_VAL_CON]	= pCItemRec->sConValu[0];	
	m_sAttr[ITEMATTR_VAL_STA]	= pCItemRec->sStaValu[0];	
	m_sAttr[ITEMATTR_VAL_LUK]	= pCItemRec->sLukValu[0];	
	m_sAttr[ITEMATTR_VAL_ASPD]	= pCItemRec->sASpdValu[0];	
	m_sAttr[ITEMATTR_VAL_ADIS]	= pCItemRec->sADisValu[0];	
	m_sAttr[ITEMATTR_VAL_MNATK]	= pCItemRec->sMnAtkValu[0];	
	m_sAttr[ITEMATTR_VAL_MXATK]	= pCItemRec->sMxAtkValu[0];	
	m_sAttr[ITEMATTR_VAL_DEF]	= pCItemRec->sDefValu[0];	
	m_sAttr[ITEMATTR_VAL_MXHP]	= pCItemRec->sMxHpValu[0];	
	m_sAttr[ITEMATTR_VAL_MXSP]	= pCItemRec->sMxSpValu[0];	
	m_sAttr[ITEMATTR_VAL_FLEE]	= pCItemRec->sFleeValu[0];	
	m_sAttr[ITEMATTR_VAL_HIT]	= pCItemRec->sHitValu[0];	
	m_sAttr[ITEMATTR_VAL_CRT]	= pCItemRec->sCrtValu[0];	
	m_sAttr[ITEMATTR_VAL_MF]	= pCItemRec->sMfValu[0];		
	m_sAttr[ITEMATTR_VAL_HREC]	= pCItemRec->sHRecValu[0];	
	m_sAttr[ITEMATTR_VAL_SREC]	= pCItemRec->sSRecValu[0];	
	m_sAttr[ITEMATTR_VAL_MSPD]	= pCItemRec->sMSpdValu[0];	
	m_sAttr[ITEMATTR_VAL_COL]	= pCItemRec->sColValu[0];
	m_sAttr[ITEMATTR_VAL_PDEF]	= pCItemRec->sPDef[0];

	m_sAttr[ITEMATTR_LHAND_VAL]	= pCItemRec->sLHandValu;
	m_sAttr[ITEMATTR_MAXURE]	= pCItemRec->sEndure[1];
	m_sAttr[ITEMATTR_MAXENERGY]	= pCItemRec->sEnergy[1];
	m_sAttr[ITEMATTR_MAXFORGE]	= pCItemRec->chForgeLv;

	m_bInitFlag = true;

	return true;
}

//=============================================================================
bool CItemRecordAttr::Init(short sItemID)
{
	CItemRecord *pCItemRec;
	pCItemRec = GetItemRecordInfo(sItemID);
	if (!pCItemRec)
		return false;

	m_sAttr[ITEMATTR_COE_STR][0]	= pCItemRec->sStrCoef;	
	m_sAttr[ITEMATTR_COE_AGI][0]	= pCItemRec->sAgiCoef;	
	m_sAttr[ITEMATTR_COE_DEX][0]	= pCItemRec->sDexCoef;	
	m_sAttr[ITEMATTR_COE_CON][0]	= pCItemRec->sConCoef;	
	m_sAttr[ITEMATTR_COE_STA][0]	= pCItemRec->sStaCoef;	
	m_sAttr[ITEMATTR_COE_LUK][0]	= pCItemRec->sLukCoef;	
	m_sAttr[ITEMATTR_COE_ASPD][0]	= pCItemRec->sASpdCoef;	
	m_sAttr[ITEMATTR_COE_ADIS][0]	= pCItemRec->sADisCoef;	
	m_sAttr[ITEMATTR_COE_MNATK][0]	= pCItemRec->sMnAtkCoef;	
	m_sAttr[ITEMATTR_COE_MXATK][0]	= pCItemRec->sMxAtkCoef;	
	m_sAttr[ITEMATTR_COE_DEF][0]	= pCItemRec->sDefCoef;	
	m_sAttr[ITEMATTR_COE_MXHP][0]	= pCItemRec->sMxHpCoef;	
	m_sAttr[ITEMATTR_COE_MXSP][0]	= pCItemRec->sMxSpCoef;	
	m_sAttr[ITEMATTR_COE_FLEE][0]	= pCItemRec->sFleeCoef;	
	m_sAttr[ITEMATTR_COE_HIT][0]	= pCItemRec->sHitCoef;	
	m_sAttr[ITEMATTR_COE_CRT][0]	= pCItemRec->sCrtCoef;	
	m_sAttr[ITEMATTR_COE_MF][0]		= pCItemRec->sMfCoef;	
	m_sAttr[ITEMATTR_COE_HREC][0]	= pCItemRec->sHRecCoef;	
	m_sAttr[ITEMATTR_COE_SREC][0]	= pCItemRec->sSRecCoef;	
	m_sAttr[ITEMATTR_COE_MSPD][0]	= pCItemRec->sMSpdCoef;	
	m_sAttr[ITEMATTR_COE_COL][0]	= pCItemRec->sColCoef;	
	m_sAttr[ITEMATTR_COE_PDEF][0]	= 0;	

	m_sAttr[ITEMATTR_COE_STR][1]	= pCItemRec->sStrCoef;	
	m_sAttr[ITEMATTR_COE_AGI][1]	= pCItemRec->sAgiCoef;	
	m_sAttr[ITEMATTR_COE_DEX][1]	= pCItemRec->sDexCoef;	
	m_sAttr[ITEMATTR_COE_CON][1]	= pCItemRec->sConCoef;	
	m_sAttr[ITEMATTR_COE_STA][1]	= pCItemRec->sStaCoef;	
	m_sAttr[ITEMATTR_COE_LUK][1]	= pCItemRec->sLukCoef;	
	m_sAttr[ITEMATTR_COE_ASPD][1]	= pCItemRec->sASpdCoef;	
	m_sAttr[ITEMATTR_COE_ADIS][1]	= pCItemRec->sADisCoef;	
	m_sAttr[ITEMATTR_COE_MNATK][1]	= pCItemRec->sMnAtkCoef;	
	m_sAttr[ITEMATTR_COE_MXATK][1]	= pCItemRec->sMxAtkCoef;	
	m_sAttr[ITEMATTR_COE_DEF][1]	= pCItemRec->sDefCoef;	
	m_sAttr[ITEMATTR_COE_MXHP][1]	= pCItemRec->sMxHpCoef;	
	m_sAttr[ITEMATTR_COE_MXSP][1]	= pCItemRec->sMxSpCoef;	
	m_sAttr[ITEMATTR_COE_FLEE][1]	= pCItemRec->sFleeCoef;	
	m_sAttr[ITEMATTR_COE_HIT][1]	= pCItemRec->sHitCoef;	
	m_sAttr[ITEMATTR_COE_CRT][1]	= pCItemRec->sCrtCoef;	
	m_sAttr[ITEMATTR_COE_MF][1]		= pCItemRec->sMfCoef;	
	m_sAttr[ITEMATTR_COE_HREC][1]	= pCItemRec->sHRecCoef;	
	m_sAttr[ITEMATTR_COE_SREC][1]	= pCItemRec->sSRecCoef;	
	m_sAttr[ITEMATTR_COE_MSPD][1]	= pCItemRec->sMSpdCoef;	
	m_sAttr[ITEMATTR_COE_COL][1]	= pCItemRec->sColCoef;	
	m_sAttr[ITEMATTR_COE_PDEF][1]	= 0;	

	m_sAttr[ITEMATTR_VAL_STR][0]	= pCItemRec->sStrValu[0];	
	m_sAttr[ITEMATTR_VAL_AGI][0]	= pCItemRec->sAgiValu[0];	
	m_sAttr[ITEMATTR_VAL_DEX][0]	= pCItemRec->sDexValu[0];	
	m_sAttr[ITEMATTR_VAL_CON][0]	= pCItemRec->sConValu[0];	
	m_sAttr[ITEMATTR_VAL_STA][0]	= pCItemRec->sStaValu[0];	
	m_sAttr[ITEMATTR_VAL_LUK][0]	= pCItemRec->sLukValu[0];	
	m_sAttr[ITEMATTR_VAL_ASPD][0]	= pCItemRec->sASpdValu[0];	
	m_sAttr[ITEMATTR_VAL_ADIS][0]	= pCItemRec->sADisValu[0];	
	m_sAttr[ITEMATTR_VAL_MNATK][0]	= pCItemRec->sMnAtkValu[0];	
	m_sAttr[ITEMATTR_VAL_MXATK][0]	= pCItemRec->sMxAtkValu[0];	
	m_sAttr[ITEMATTR_VAL_DEF][0]	= pCItemRec->sDefValu[0];	
	m_sAttr[ITEMATTR_VAL_MXHP][0]	= pCItemRec->sMxHpValu[0];	
	m_sAttr[ITEMATTR_VAL_MXSP][0]	= pCItemRec->sMxSpValu[0];	
	m_sAttr[ITEMATTR_VAL_FLEE][0]	= pCItemRec->sFleeValu[0];	
	m_sAttr[ITEMATTR_VAL_HIT][0]	= pCItemRec->sHitValu[0];	
	m_sAttr[ITEMATTR_VAL_CRT][0]	= pCItemRec->sCrtValu[0];	
	m_sAttr[ITEMATTR_VAL_MF][0]		= pCItemRec->sMfValu[0];		
	m_sAttr[ITEMATTR_VAL_HREC][0]	= pCItemRec->sHRecValu[0];	
	m_sAttr[ITEMATTR_VAL_SREC][0]	= pCItemRec->sSRecValu[0];	
	m_sAttr[ITEMATTR_VAL_MSPD][0]	= pCItemRec->sMSpdValu[0];	
	m_sAttr[ITEMATTR_VAL_COL][0]	= pCItemRec->sColValu[0];	
	m_sAttr[ITEMATTR_VAL_PDEF][0]	= pCItemRec->sPDef[0];	

	m_sAttr[ITEMATTR_VAL_STR][1]	= pCItemRec->sStrValu[1];	
	m_sAttr[ITEMATTR_VAL_AGI][1]	= pCItemRec->sAgiValu[1];	
	m_sAttr[ITEMATTR_VAL_DEX][1]	= pCItemRec->sDexValu[1];	
	m_sAttr[ITEMATTR_VAL_CON][1]	= pCItemRec->sConValu[1];	
	m_sAttr[ITEMATTR_VAL_STA][1]	= pCItemRec->sStaValu[1];	
	m_sAttr[ITEMATTR_VAL_LUK][1]	= pCItemRec->sLukValu[1];	
	m_sAttr[ITEMATTR_VAL_ASPD][1]	= pCItemRec->sASpdValu[1];	
	m_sAttr[ITEMATTR_VAL_ADIS][1]	= pCItemRec->sADisValu[1];	
	m_sAttr[ITEMATTR_VAL_MNATK][1]	= pCItemRec->sMnAtkValu[1];	
	m_sAttr[ITEMATTR_VAL_MXATK][1]	= pCItemRec->sMxAtkValu[1];	
	m_sAttr[ITEMATTR_VAL_DEF][1]	= pCItemRec->sDefValu[1];	
	m_sAttr[ITEMATTR_VAL_MXHP][1]	= pCItemRec->sMxHpValu[1];	
	m_sAttr[ITEMATTR_VAL_MXSP][1]	= pCItemRec->sMxSpValu[1];	
	m_sAttr[ITEMATTR_VAL_FLEE][1]	= pCItemRec->sFleeValu[1];	
	m_sAttr[ITEMATTR_VAL_HIT][1]	= pCItemRec->sHitValu[1];	
	m_sAttr[ITEMATTR_VAL_CRT][1]	= pCItemRec->sCrtValu[1];	
	m_sAttr[ITEMATTR_VAL_MF][1]		= pCItemRec->sMfValu[1];		
	m_sAttr[ITEMATTR_VAL_HREC][1]	= pCItemRec->sHRecValu[1];	
	m_sAttr[ITEMATTR_VAL_SREC][1]	= pCItemRec->sSRecValu[1];	
	m_sAttr[ITEMATTR_VAL_MSPD][1]	= pCItemRec->sMSpdValu[1];	
	m_sAttr[ITEMATTR_VAL_COL][1]	= pCItemRec->sColValu[1];
	m_sAttr[ITEMATTR_VAL_PDEF][1]	= pCItemRec->sPDef[1];

	m_sAttr[ITEMATTR_MAXURE][0]		= pCItemRec->sEndure[0];
	m_sAttr[ITEMATTR_MAXURE][1]		= pCItemRec->sEndure[1];
	m_sAttr[ITEMATTR_MAXENERGY][0]	= pCItemRec->sEnergy[0];
	m_sAttr[ITEMATTR_MAXENERGY][1]	= pCItemRec->sEnergy[1];
	m_sAttr[ITEMATTR_MAXFORGE][0]	= pCItemRec->chForgeLv;
	m_sAttr[ITEMATTR_MAXFORGE][1]	= pCItemRec->chForgeLv;

	return true;
}
