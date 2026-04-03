#include "ItemAttr.h"
#include "ItemRecord.h"

bool CItemAttr::Init(const CItemRecord& rec) {
	memset(m_sAttr, 0, sizeof(m_sAttr));
	m_sAttr[ITEMATTR_COE_STR] = rec.sStrCoef;
	m_sAttr[ITEMATTR_COE_AGI] = rec.sAgiCoef;
	m_sAttr[ITEMATTR_COE_DEX] = rec.sDexCoef;
	m_sAttr[ITEMATTR_COE_CON] = rec.sConCoef;
	m_sAttr[ITEMATTR_COE_STA] = rec.sStaCoef;
	m_sAttr[ITEMATTR_COE_LUK] = rec.sLukCoef;
	m_sAttr[ITEMATTR_COE_ASPD] = rec.sASpdCoef;
	m_sAttr[ITEMATTR_COE_ADIS] = rec.sADisCoef;
	m_sAttr[ITEMATTR_COE_MNATK] = rec.sMnAtkCoef;
	m_sAttr[ITEMATTR_COE_MXATK] = rec.sMxAtkCoef;
	m_sAttr[ITEMATTR_COE_DEF] = rec.sDefCoef;
	m_sAttr[ITEMATTR_COE_MXHP] = rec.sMxHpCoef;
	m_sAttr[ITEMATTR_COE_MXSP] = rec.sMxSpCoef;
	m_sAttr[ITEMATTR_COE_FLEE] = rec.sFleeCoef;
	m_sAttr[ITEMATTR_COE_HIT] = rec.sHitCoef;
	m_sAttr[ITEMATTR_COE_CRT] = rec.sCrtCoef;
	m_sAttr[ITEMATTR_COE_MF] = rec.sMfCoef;
	m_sAttr[ITEMATTR_COE_HREC] = rec.sHRecCoef;
	m_sAttr[ITEMATTR_COE_SREC] = rec.sSRecCoef;
	m_sAttr[ITEMATTR_COE_MSPD] = rec.sMSpdCoef;
	m_sAttr[ITEMATTR_COE_COL] = rec.sColCoef;
	m_sAttr[ITEMATTR_COE_PDEF] = 0;

	m_sAttr[ITEMATTR_VAL_STR] = rec.sStrValu[0];
	m_sAttr[ITEMATTR_VAL_AGI] = rec.sAgiValu[0];
	m_sAttr[ITEMATTR_VAL_DEX] = rec.sDexValu[0];
	m_sAttr[ITEMATTR_VAL_CON] = rec.sConValu[0];
	m_sAttr[ITEMATTR_VAL_STA] = rec.sStaValu[0];
	m_sAttr[ITEMATTR_VAL_LUK] = rec.sLukValu[0];
	m_sAttr[ITEMATTR_VAL_ASPD] = rec.sASpdValu[0];
	m_sAttr[ITEMATTR_VAL_ADIS] = rec.sADisValu[0];
	m_sAttr[ITEMATTR_VAL_MNATK] = rec.sMnAtkValu[0];
	m_sAttr[ITEMATTR_VAL_MXATK] = rec.sMxAtkValu[0];
	m_sAttr[ITEMATTR_VAL_DEF] = rec.sDefValu[0];
	m_sAttr[ITEMATTR_VAL_MXHP] = rec.sMxHpValu[0];
	m_sAttr[ITEMATTR_VAL_MXSP] = rec.sMxSpValu[0];
	m_sAttr[ITEMATTR_VAL_FLEE] = rec.sFleeValu[0];
	m_sAttr[ITEMATTR_VAL_HIT] = rec.sHitValu[0];
	m_sAttr[ITEMATTR_VAL_CRT] = rec.sCrtValu[0];
	m_sAttr[ITEMATTR_VAL_MF] = rec.sMfValu[0];
	m_sAttr[ITEMATTR_VAL_HREC] = rec.sHRecValu[0];
	m_sAttr[ITEMATTR_VAL_SREC] = rec.sSRecValu[0];
	m_sAttr[ITEMATTR_VAL_MSPD] = rec.sMSpdValu[0];
	m_sAttr[ITEMATTR_VAL_COL] = rec.sColValu[0];
	m_sAttr[ITEMATTR_VAL_PDEF] = rec.sPDef[0];

	m_sAttr[ITEMATTR_LHAND_VAL] = rec.sLHandValu;
	m_sAttr[ITEMATTR_MAXURE] = rec.sEndure[1];
	m_sAttr[ITEMATTR_MAXENERGY] = rec.sEnergy[1];
	m_sAttr[ITEMATTR_MAXFORGE] = rec.chForgeLv;

	m_bInitFlag = true;

	return true;
}

//=============================================================================
bool CItemRecordAttr::Init(const CItemRecord& rec) {
	m_sAttr[ITEMATTR_COE_STR][0] = rec.sStrCoef;
	m_sAttr[ITEMATTR_COE_AGI][0] = rec.sAgiCoef;
	m_sAttr[ITEMATTR_COE_DEX][0] = rec.sDexCoef;
	m_sAttr[ITEMATTR_COE_CON][0] = rec.sConCoef;
	m_sAttr[ITEMATTR_COE_STA][0] = rec.sStaCoef;
	m_sAttr[ITEMATTR_COE_LUK][0] = rec.sLukCoef;
	m_sAttr[ITEMATTR_COE_ASPD][0] = rec.sASpdCoef;
	m_sAttr[ITEMATTR_COE_ADIS][0] = rec.sADisCoef;
	m_sAttr[ITEMATTR_COE_MNATK][0] = rec.sMnAtkCoef;
	m_sAttr[ITEMATTR_COE_MXATK][0] = rec.sMxAtkCoef;
	m_sAttr[ITEMATTR_COE_DEF][0] = rec.sDefCoef;
	m_sAttr[ITEMATTR_COE_MXHP][0] = rec.sMxHpCoef;
	m_sAttr[ITEMATTR_COE_MXSP][0] = rec.sMxSpCoef;
	m_sAttr[ITEMATTR_COE_FLEE][0] = rec.sFleeCoef;
	m_sAttr[ITEMATTR_COE_HIT][0] = rec.sHitCoef;
	m_sAttr[ITEMATTR_COE_CRT][0] = rec.sCrtCoef;
	m_sAttr[ITEMATTR_COE_MF][0] = rec.sMfCoef;
	m_sAttr[ITEMATTR_COE_HREC][0] = rec.sHRecCoef;
	m_sAttr[ITEMATTR_COE_SREC][0] = rec.sSRecCoef;
	m_sAttr[ITEMATTR_COE_MSPD][0] = rec.sMSpdCoef;
	m_sAttr[ITEMATTR_COE_COL][0] = rec.sColCoef;
	m_sAttr[ITEMATTR_COE_PDEF][0] = 0;

	m_sAttr[ITEMATTR_COE_STR][1] = rec.sStrCoef;
	m_sAttr[ITEMATTR_COE_AGI][1] = rec.sAgiCoef;
	m_sAttr[ITEMATTR_COE_DEX][1] = rec.sDexCoef;
	m_sAttr[ITEMATTR_COE_CON][1] = rec.sConCoef;
	m_sAttr[ITEMATTR_COE_STA][1] = rec.sStaCoef;
	m_sAttr[ITEMATTR_COE_LUK][1] = rec.sLukCoef;
	m_sAttr[ITEMATTR_COE_ASPD][1] = rec.sASpdCoef;
	m_sAttr[ITEMATTR_COE_ADIS][1] = rec.sADisCoef;
	m_sAttr[ITEMATTR_COE_MNATK][1] = rec.sMnAtkCoef;
	m_sAttr[ITEMATTR_COE_MXATK][1] = rec.sMxAtkCoef;
	m_sAttr[ITEMATTR_COE_DEF][1] = rec.sDefCoef;
	m_sAttr[ITEMATTR_COE_MXHP][1] = rec.sMxHpCoef;
	m_sAttr[ITEMATTR_COE_MXSP][1] = rec.sMxSpCoef;
	m_sAttr[ITEMATTR_COE_FLEE][1] = rec.sFleeCoef;
	m_sAttr[ITEMATTR_COE_HIT][1] = rec.sHitCoef;
	m_sAttr[ITEMATTR_COE_CRT][1] = rec.sCrtCoef;
	m_sAttr[ITEMATTR_COE_MF][1] = rec.sMfCoef;
	m_sAttr[ITEMATTR_COE_HREC][1] = rec.sHRecCoef;
	m_sAttr[ITEMATTR_COE_SREC][1] = rec.sSRecCoef;
	m_sAttr[ITEMATTR_COE_MSPD][1] = rec.sMSpdCoef;
	m_sAttr[ITEMATTR_COE_COL][1] = rec.sColCoef;
	m_sAttr[ITEMATTR_COE_PDEF][1] = 0;

	m_sAttr[ITEMATTR_VAL_STR][0] = rec.sStrValu[0];
	m_sAttr[ITEMATTR_VAL_AGI][0] = rec.sAgiValu[0];
	m_sAttr[ITEMATTR_VAL_DEX][0] = rec.sDexValu[0];
	m_sAttr[ITEMATTR_VAL_CON][0] = rec.sConValu[0];
	m_sAttr[ITEMATTR_VAL_STA][0] = rec.sStaValu[0];
	m_sAttr[ITEMATTR_VAL_LUK][0] = rec.sLukValu[0];
	m_sAttr[ITEMATTR_VAL_ASPD][0] = rec.sASpdValu[0];
	m_sAttr[ITEMATTR_VAL_ADIS][0] = rec.sADisValu[0];
	m_sAttr[ITEMATTR_VAL_MNATK][0] = rec.sMnAtkValu[0];
	m_sAttr[ITEMATTR_VAL_MXATK][0] = rec.sMxAtkValu[0];
	m_sAttr[ITEMATTR_VAL_DEF][0] = rec.sDefValu[0];
	m_sAttr[ITEMATTR_VAL_MXHP][0] = rec.sMxHpValu[0];
	m_sAttr[ITEMATTR_VAL_MXSP][0] = rec.sMxSpValu[0];
	m_sAttr[ITEMATTR_VAL_FLEE][0] = rec.sFleeValu[0];
	m_sAttr[ITEMATTR_VAL_HIT][0] = rec.sHitValu[0];
	m_sAttr[ITEMATTR_VAL_CRT][0] = rec.sCrtValu[0];
	m_sAttr[ITEMATTR_VAL_MF][0] = rec.sMfValu[0];
	m_sAttr[ITEMATTR_VAL_HREC][0] = rec.sHRecValu[0];
	m_sAttr[ITEMATTR_VAL_SREC][0] = rec.sSRecValu[0];
	m_sAttr[ITEMATTR_VAL_MSPD][0] = rec.sMSpdValu[0];
	m_sAttr[ITEMATTR_VAL_COL][0] = rec.sColValu[0];
	m_sAttr[ITEMATTR_VAL_PDEF][0] = rec.sPDef[0];

	m_sAttr[ITEMATTR_VAL_STR][1] = rec.sStrValu[1];
	m_sAttr[ITEMATTR_VAL_AGI][1] = rec.sAgiValu[1];
	m_sAttr[ITEMATTR_VAL_DEX][1] = rec.sDexValu[1];
	m_sAttr[ITEMATTR_VAL_CON][1] = rec.sConValu[1];
	m_sAttr[ITEMATTR_VAL_STA][1] = rec.sStaValu[1];
	m_sAttr[ITEMATTR_VAL_LUK][1] = rec.sLukValu[1];
	m_sAttr[ITEMATTR_VAL_ASPD][1] = rec.sASpdValu[1];
	m_sAttr[ITEMATTR_VAL_ADIS][1] = rec.sADisValu[1];
	m_sAttr[ITEMATTR_VAL_MNATK][1] = rec.sMnAtkValu[1];
	m_sAttr[ITEMATTR_VAL_MXATK][1] = rec.sMxAtkValu[1];
	m_sAttr[ITEMATTR_VAL_DEF][1] = rec.sDefValu[1];
	m_sAttr[ITEMATTR_VAL_MXHP][1] = rec.sMxHpValu[1];
	m_sAttr[ITEMATTR_VAL_MXSP][1] = rec.sMxSpValu[1];
	m_sAttr[ITEMATTR_VAL_FLEE][1] = rec.sFleeValu[1];
	m_sAttr[ITEMATTR_VAL_HIT][1] = rec.sHitValu[1];
	m_sAttr[ITEMATTR_VAL_CRT][1] = rec.sCrtValu[1];
	m_sAttr[ITEMATTR_VAL_MF][1] = rec.sMfValu[1];
	m_sAttr[ITEMATTR_VAL_HREC][1] = rec.sHRecValu[1];
	m_sAttr[ITEMATTR_VAL_SREC][1] = rec.sSRecValu[1];
	m_sAttr[ITEMATTR_VAL_MSPD][1] = rec.sMSpdValu[1];
	m_sAttr[ITEMATTR_VAL_COL][1] = rec.sColValu[1];
	m_sAttr[ITEMATTR_VAL_PDEF][1] = rec.sPDef[1];

	m_sAttr[ITEMATTR_MAXURE][0] = rec.sEndure[0];
	m_sAttr[ITEMATTR_MAXURE][1] = rec.sEndure[1];
	m_sAttr[ITEMATTR_MAXENERGY][0] = rec.sEnergy[0];
	m_sAttr[ITEMATTR_MAXENERGY][1] = rec.sEnergy[1];
	m_sAttr[ITEMATTR_MAXFORGE][0] = rec.chForgeLv;
	m_sAttr[ITEMATTR_MAXFORGE][1] = rec.chForgeLv;

	return true;
}
