//=============================================================================
// FileName: SkillStateType.h
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Type
//=============================================================================

#ifndef SKILLSTATETYPE_H
#define SKILLSTATETYPE_H

const unsigned char	SSTATE_NONE			= 0;	// 

// 
const unsigned char	SSTATE_CUT			= 1;	// 
const unsigned char	SSTATE_FERVOR_ARROR	= 2;	// 
const unsigned char	SSTATE_FROST_ARROR	= 3;	// 
const unsigned char	SSTATE_SKYROCKET	= 4;	// 
const unsigned char	SSTATE_MURRAIN		= 5;	// 
const unsigned char	SSTATE_GIDDY		= 6;	// 
const unsigned char	SSTATE_FREEZE		= 7;	// 
const unsigned char	SSTATE_SLEEP		= 8;	// 
const unsigned char	SSTATE_BIND			= 9;	// 
const unsigned char	SSTATE_FROST		= 10;	// 
const unsigned char	SSTATE_BEAT_BACK	= 11;	// 
const unsigned char	SSTATE_UNBEATABLE	= 12;	// 
const unsigned char	SSTATE_TOXIN		= 13;	// 
const unsigned char	SSTATE_REBOUND		= 14;	// 
const unsigned char	SSTATE_AVATAR		= 15;	// 
const unsigned char	SSTATE_TITAN		= 16;	// 
const unsigned char	SSTATE_BLINDNESS	= 17;	// 
const unsigned char	SSTATE_HAIR			= 18;	// 
const unsigned char	SSTATE_FLOAT		= 19;	// 
const unsigned char	SSTATE_CALL			= 20;	// 
const unsigned char	SSTATE_SHIELD		= 21;	// 
const unsigned char	SSTATE_TIGER		= 22;	// 
const unsigned char	SSTATE_HIDE			= 43;	// 
const unsigned char	SSTATE_TRADE		= 85;	// 
const unsigned char	SSTATE_STALL		= 99;	// 
const unsigned char	SSTATE_REPAIR		= 100;	// 
const unsigned char	SSTATE_FORGE		= 101;	// 


const unsigned char	AREA_STATE_MAXID	= 255;

// Modify by lark.li 20080828 begin
//const unsigned char	SKILL_STATE_MAXID	= 240;
const unsigned char	SKILL_STATE_MAXID	= 254;
// End

const unsigned char	SKILL_STATE_LEVEL	= 20;

const long	SSTATE_SIGN_BYTE_NUM	= (SKILL_STATE_MAXID + sizeof(char) * 8 - 1) / (sizeof(char) * 8);

const unsigned char	AREA_STATE_NUM		= AREA_STATE_MAXID;
const unsigned char	CHA_STATE_NUM		= 16;

#endif // SKILLSTATETYPE_H
