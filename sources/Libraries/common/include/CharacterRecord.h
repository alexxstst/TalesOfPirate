//======================================================================================================================
// FileName: CharacterRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CChaRecordSet class
//======================================================================================================================

#ifndef CHARACTERRECORD_H
#define CHARACTERRECORD_H

#include <tchar.h>
#include <string>
#include <source_location>
#include "util.h"
#include "TableData.h"

// -
enum EChaModalType
{
	enumMODAL_MAIN_CHA		= 1,
	enumMODAL_BOAT,				
	enumMODAL_EMPL,
	enumMODAL_OTHER,
};

// 
enum EChaCtrlType
{
	enumCHACTRL_NONE		= 0, // 

	enumCHACTRL_PLAYER		= 1, // 

	enumCHACTRL_NPC			= 2, // NPC
	enumCHACTRL_NPC_EVENT	= 3, // NPC

	enumCHACTRL_MONS		    = 5, // 
	enumCHACTRL_MONS_TREE	    = 6, // 
	enumCHACTRL_MONS_MINE	    = 7, // 
	enumCHACTRL_MONS_FISH	    = 8, // 
	enumCHACTRL_MONS_DBOAT	    = 9, // 

	enumCHACTRL_PLAYER_PET      = 10, // 

	enumCHACTRL_MONS_REPAIRABLE = 17, // 
};

#define defCHA_AI_NONE				0 // 
#define defCHA_AI_ATTACK_PASSIVE	1 // 
#define defCHA_AI_ATTACK_ACTIVE		2 // 

const char cchChaRecordKeyValue = (char)0xff;

#define defCHA_NAME_LEN			32
#define defCHA_ICON_NAME_LEN	17

#define defCHA_SKIN_NUM			8
#define defCHA_INIT_SKILL_NUM	11		// 0,1-10
#define defCHA_INIT_ITEM_NUM	15		// Edit by Mdrst
#define defCHA_GUILD_NAME_LEN	33
#define defCHA_TITLE_NAME_LEN	33
#define defCHA_JOB_NAME_LEN		17
#define defCHA_ITEM_KIND_NUM	20
#define defCHA_DIE_EFFECT_NUM	3		// 
#define defCHA_BIRTH_EFFECT_NUM	3		// 
#define defCHA_HP_EFFECT_NUM    3

class CChaRecord : public EntityData
{
public:
	//CChaRecord();

	long	lID;								//
	std::string	szName;							//
	std::string	szIconName;						//
	char	chModalType;						// 
	char	chCtrlType;							// 
	short	sModel;								// 
	short	sSuitID;							// 
	short	sSuitNum;							// 
	short	sSkinInfo[defCHA_SKIN_NUM];			// 
	short	sFeffID[4];							// FeffID
	short	sEeffID;							// EeffID
	short   sEffectActionID[3];					// ,0-,1-,2-dummy
	short	sShadow;							// 
    short   sActionID;                          // 
    char    chDiaphaneity;                      // 
	short	sFootfall;							// 
	short	sWhoop;								// 
	short	sDirge;								// 
	char	chControlAble;						// 
	//char	chMoveAble;							// 
	char	chTerritory;						// 
	short   sSeaHeight;							// 
	short	sItemType[defCHA_ITEM_KIND_NUM];	// 
	float	fLengh;								// 
	float	fWidth;								// 
	float	fHeight;							// 
	short	sRadii;								// 
	char    nBirthBehave[defCHA_BIRTH_EFFECT_NUM];// 
	char    nDiedBehave[defCHA_DIE_EFFECT_NUM];	// 
    short   sBornEff;                           // 
    short   sDieEff;                            // 
	short	sDormancy;							// 
    char    chDieAction;                        // 
	int		_nHPEffect[defCHA_HP_EFFECT_NUM];	// hp
	bool	_IsFace;							// 
	bool	_IsCyclone;							// 
	long	lScript;							// ID
	long	lWeapon;							// ID
	long	lSkill[defCHA_INIT_SKILL_NUM][2];	// 
	long	lItem[defCHA_INIT_ITEM_NUM][2];		// 
	long	lTaskItem[defCHA_INIT_ITEM_NUM][2];	// 
	long	lMaxShowItem;						// 
	float	fAllShow;							// 
	long	lPrefix;							// 
	long	lAiNo;								// AI
	char	chCanTurn;							// 
	long	lVision;							// 
	long	lNoise;								// 
	long	lGetEXP;							// EXP
	bool	bLight;								// 

	long	lMobexp;// exp
	long	lLv;	// 
	long	lMxHp;	// HP
	long	lHp;	// HP
	long	lMxSp;	// SP
	long	lSp;	// SP
	long	lMnAtk;	// 
	long	lMxAtk;	// 
	long	lPDef;	// 
	long	lDef;	// 
	long	lHit;	// 
	long	lFlee;	// 
	long	lCrt;	// 
	long	lMf;	// 
	long	lHRec;	// hp
	long	lSRec;	// sp
	long	lASpd;	// 
	long	lADis;	// 
	long	lCDis;	// 
	long	lMSpd;	// 
	long	lCol;	// 
	long	lStr;	// strength		
	long	lAgi;	// agility		
	long	lDex;	// dexterity	
	long	lCon;	// constitution	hp
	long	lSta;	// stamina		sp
	long	lLuk;	// luck			
	long	lLHandVal;	// 
	std::string	szGuild;						//
	std::string	szTitle;						//
	std::string	szJob;							//
	LONG32	lCExp;	// 
	LONG32	lNExp;	// 
	long	lFame;	// 
	long	lAp;	// 
	long	lTp;	// technique point
	long	lGd;	// 
	long	lSpri;	// 
	long	lStor;	// ()
	long	lMxSail;	// 
	long	lSail;	// 
	long	lStasa;	// 
	long	lScsm;	// 

	long	lTStr;	// 				
	long	lTAgi;	// 				
	long	lTDex;	// 				
	long	lTCon;	// 				
	long	lTSta;	// 				
	long	lTLuk;	// 				
	long	lTMxHp;	// 			
	long	lTMxSp;	// 		
	long	lTAtk;	// 			
	long	lTDef;	// 			
	long	lTHit;	// 			
	long	lTFlee;	// 			
	long	lTMf;	// 			
	long	lTCrt;	// 			
	long	lTHRec;	// hp		hp
	long	lTSRec;	// sp		sp
	long	lTASpd;	// 			
	long	lTADis;	// 			
	long	lTSpd;	// 			
	long	lTSpri;	// 			
	long	lTScsm;	// 			

	// added by clp 
	float	scaling[3];

public:
	// ,
	bool	HaveEffectFog()				{ return _HaveEffectFog;	}
	int		GetEffectFog( int i )		{ return _nHPEffect[i];		}

	bool	IsFace()					{ return _IsFace;			}			// 
	bool	IsCyclone()					{ return _IsCyclone;		}			// 

	void	RefreshPrivateData();				// 

private:
	bool	_HaveEffectFog;

};

CChaRecord* GetChaRecordInfo(int nTypeID, const std::source_location& loc = std::source_location::current());

#endif // CHARACTERRECORD_H
