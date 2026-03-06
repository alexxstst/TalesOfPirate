#pragma once

//=============================================================================
// FileName: ItemAttrType.h
// Creater: ZhangXuedong
// Date: 2004.12.28
// Comment: Item Attribute type
//=============================================================================

#ifndef ITEMATTRTYPE_H
#define ITEMATTRTYPE_H

// 0
// 

const long	ITEMATTR_COUNT_BASE0    = 0;
const long	ITEMATTR_COE_STR        = ITEMATTR_COUNT_BASE0 + 1; // strength coefficient
const long	ITEMATTR_COE_AGI        = ITEMATTR_COUNT_BASE0 + 2; // 
const long	ITEMATTR_COE_DEX        = ITEMATTR_COUNT_BASE0 + 3; // 
const long	ITEMATTR_COE_CON        = ITEMATTR_COUNT_BASE0 + 4; // 
const long	ITEMATTR_COE_STA        = ITEMATTR_COUNT_BASE0 + 5; // 
const long	ITEMATTR_COE_LUK        = ITEMATTR_COUNT_BASE0 + 6; // 
const long	ITEMATTR_COE_ASPD       = ITEMATTR_COUNT_BASE0 + 7; // 
const long	ITEMATTR_COE_ADIS       = ITEMATTR_COUNT_BASE0 + 8; // 
const long	ITEMATTR_COE_MNATK      = ITEMATTR_COUNT_BASE0 + 9; // 
const long	ITEMATTR_COE_MXATK      = ITEMATTR_COUNT_BASE0 + 10; // 
const long	ITEMATTR_COE_DEF        = ITEMATTR_COUNT_BASE0 + 11; // 
const long	ITEMATTR_COE_MXHP       = ITEMATTR_COUNT_BASE0 + 12; // Hp
const long	ITEMATTR_COE_MXSP       = ITEMATTR_COUNT_BASE0 + 13; // Sp
const long	ITEMATTR_COE_FLEE       = ITEMATTR_COUNT_BASE0 + 14; // 
const long	ITEMATTR_COE_HIT        = ITEMATTR_COUNT_BASE0 + 15; // 
const long	ITEMATTR_COE_CRT        = ITEMATTR_COUNT_BASE0 + 16; // 
const long	ITEMATTR_COE_MF         = ITEMATTR_COUNT_BASE0 + 17; // 
const long	ITEMATTR_COE_HREC       = ITEMATTR_COUNT_BASE0 + 18; // hp
const long	ITEMATTR_COE_SREC       = ITEMATTR_COUNT_BASE0 + 19; // sp
const long	ITEMATTR_COE_MSPD       = ITEMATTR_COUNT_BASE0 + 20; // 
const long	ITEMATTR_COE_COL        = ITEMATTR_COUNT_BASE0 + 21; // 
const long	ITEMATTR_COE_PDEF       = ITEMATTR_COUNT_BASE0 + 22; // 

const long	ITEMATTR_COUNT_BASE1    = 25;
const long	ITEMATTR_VAL_STR        = ITEMATTR_COUNT_BASE1 + 1; // strength value
const long	ITEMATTR_VAL_AGI        = ITEMATTR_COUNT_BASE1 + 2; // 
const long	ITEMATTR_VAL_DEX        = ITEMATTR_COUNT_BASE1 + 3; // 
const long	ITEMATTR_VAL_CON        = ITEMATTR_COUNT_BASE1 + 4; // 
const long	ITEMATTR_VAL_STA        = ITEMATTR_COUNT_BASE1 + 5; // 
const long	ITEMATTR_VAL_LUK        = ITEMATTR_COUNT_BASE1 + 6; // 
const long	ITEMATTR_VAL_ASPD       = ITEMATTR_COUNT_BASE1 + 7; // 
const long	ITEMATTR_VAL_ADIS       = ITEMATTR_COUNT_BASE1 + 8; // 
const long	ITEMATTR_VAL_MNATK      = ITEMATTR_COUNT_BASE1 + 9; // 
const long	ITEMATTR_VAL_MXATK      = ITEMATTR_COUNT_BASE1 + 10; // 
const long	ITEMATTR_VAL_DEF        = ITEMATTR_COUNT_BASE1 + 11; // 
const long	ITEMATTR_VAL_MXHP       = ITEMATTR_COUNT_BASE1 + 12; // Hp
const long	ITEMATTR_VAL_MXSP       = ITEMATTR_COUNT_BASE1 + 13; // Sp
const long	ITEMATTR_VAL_FLEE       = ITEMATTR_COUNT_BASE1 + 14; // 
const long	ITEMATTR_VAL_HIT        = ITEMATTR_COUNT_BASE1 + 15; // 
const long	ITEMATTR_VAL_CRT        = ITEMATTR_COUNT_BASE1 + 16; // 
const long	ITEMATTR_VAL_MF         = ITEMATTR_COUNT_BASE1 + 17; // 
const long	ITEMATTR_VAL_HREC       = ITEMATTR_COUNT_BASE1 + 18; // hp
const long	ITEMATTR_VAL_SREC       = ITEMATTR_COUNT_BASE1 + 19; // sp
const long	ITEMATTR_VAL_MSPD       = ITEMATTR_COUNT_BASE1 + 20; // 
const long	ITEMATTR_VAL_COL        = ITEMATTR_COUNT_BASE1 + 21; // 
const long	ITEMATTR_VAL_PDEF       = ITEMATTR_COUNT_BASE1 + 22; // 

const long	ITEMATTR_COUNT_BASE2    = 49;
const long	ITEMATTR_LHAND_VAL      = ITEMATTR_COUNT_BASE2 + 1; // 
const long	ITEMATTR_MAXURE	        = ITEMATTR_COUNT_BASE2 + 2; // 
const long	ITEMATTR_MAXFORGE       = ITEMATTR_COUNT_BASE2 + 3; // 
const long	ITEMATTR_MAXENERGY      = ITEMATTR_COUNT_BASE2 + 4; // 
const long	ITEMATTR_URE            = ITEMATTR_COUNT_BASE2 + 5; // 
const long	ITEMATTR_FORGE          = ITEMATTR_COUNT_BASE2 + 6; // 
const long	ITEMATTR_ENERGY         = ITEMATTR_COUNT_BASE2 + 7; // 


const long	ITEMATTR_MAX_NUM        = 58;
const long  ITEMATTR_CLIENT_MAX     = ITEMATTR_VAL_PDEF + 1; // 

const long  ITEMATTR_COUNT_BASE3    = 180;
const long  ITEMATTR_VAL_PARAM1		= ITEMATTR_COUNT_BASE3 + 1;	// 
const long  ITEMATTR_VAL_PARAM2		= ITEMATTR_COUNT_BASE3 + 2;	// 
const long  ITEMATTR_VAL_LEVEL		= ITEMATTR_COUNT_BASE3 + 3; // 
const long  ITEMATTR_VAL_FUSIONID   = ITEMATTR_COUNT_BASE3 + 4; // ID

// Extra attibutes by Mdr

const long  ITEMATTR_TRADABLE       = ITEMATTR_COUNT_BASE2 + 8;
const long  ITEMATTR_EXPIRATION     = ITEMATTR_COUNT_BASE3 + 5;

#endif // ITEMATTRTYPE_H
