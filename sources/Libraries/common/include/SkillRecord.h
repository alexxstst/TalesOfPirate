//======================================================================================================================
// FileName: SkillRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CSkillRecordSet class
//======================================================================================================================

#ifndef	SKILLRECORD_H
#define	SKILLRECORD_H

#include <tchar.h>
#include <string>
#include <source_location>
#include "util.h"
#include "TableData.h"
#include "SkillBag.h"

const char cchSkillRecordKeyValue = (char)0xfe;				// -1,0,-2

#define	defSKILL_NAME_LEN	17
#define defSKILL_JOB_SELECT_NUM	9
#define defSKILL_ITEM_NEED_NUM	8
#define defSKILL_PRE_SKILL_NUM	3
#define defSKILL_ICON_NAME_LEN	17
#define defEFFECT_SELF_ATTR_NUM	2
#define defEFFECT_TAR_ATTR_NUM	2
#define defSELF_EFFECT_NUM		2
#define defEXPEND_ITEM_NUM	2
#define defSKILL_OPERATE_NUM	3
#define defSKILL_POSE_NUM       10
#define defSKILL_EFFECT_SCRIPT_LEN	33
#define defSKILL_RANGE_SET_SCRIPT	33
#define defSKILL_ACTION_EFFECT  3
#define defSKILL_ITEM_EFFECT    2

enum ESkillItemNeed
{
	enumSKILL_ITEM_NEED_TYPE	= 1,
	enumSKILL_ITEM_NEED_ID		= 2,
};

enum ESkillObjType
{
	// 12345

	enumSKILL_TYPE_SELF		= 1,	// 
	enumSKILL_TYPE_TEAM		= 2,	// 
	enumSKILL_TYPE_SCENE	= 3,	// 
	enumSKILL_TYPE_ENEMY	= 4,	// 
	enumSKILL_TYPE_ALL		= 5,	// 
	enumSKILL_TYPE_PLAYER_DIE	= 6,	// 
	enumSKILL_TYPE_EXCEPT_SELF  = 7,    // 

	// 
	enumSKILL_TYPE_REPAIR	= 17,	// 
	enumSKILL_TYPE_TREE		= 18,	// 
	enumSKILL_TYPE_MINE		= 19,	// 
	enumSKILL_TYPE_TRADE	= 22,	// 
	enumSKILL_TYPE_FISH		= 28,	// 
	enumSKILL_TYPE_SALVAGE	= 29,	// 

};

enum eSelectCha		// 
{
	enumSC_NONE = 0,		// 
	enumSC_ALL,				// 
	enumSC_PLAYER,			// ,,

	enumSC_ENEMY,			// :PK+,
	enumSC_PLAYER_ASHES,	// ()

	enumSC_MONS,			// 
	enumSC_MONS_REPAIRABLE, // 
	enumSC_MONS_TREE,       // 
	enumSC_MONS_MINE,		// 
	enumSC_MONS_FISH,		// 
	enumSC_MONS_DBOAT,		// 

	enumSC_SELF,			// 
	enumSC_TEAM,			// 
};

enum ESkillEffType
{
	enumSKILL_EFF_BANEFUL	= 0,	// 
	enumSKILL_EFF_HELPFUL	= 1,	// 
};

enum ESkillUpgrade
{
	enumSKILL_UPGRADE_NONE,			// 
	enumSKILL_UPGRADE_CAN,			// 
	enumSKILL_UPGRADE_MAX,			// 
};

enum ESkillFightType
{
	enumSKILL_LAND_LIVE		= 0, // 
	enumSKILL_FIGHT			= 1, // 
	enumSKILL_SAIL			= 2, // 
	enumSKILL_SEE_LIVE		= 3, // 
};

enum ESkillSrcType
{
	enumSKILL_SRC_HUMAN	= 1,	// 
	enumSKILL_SRC_BOAT	= 2,	// 
};

enum ESkillTarHabitatType
{
	enumSKILL_TAR_LAND	= 1,	// 
	enumSKILL_TAR_SEA	= 2,	// 
	enumSKILL_TAR_LORS	= 3,	// 
};

enum ESkillType
{
	enumSKILL_INBORN	= 0,	// 
	enumSKILL_ACTIVE	= 1,	// 
	enumSKILL_PASSIVE	= 2,	// 
};

enum ESkillPhaseType
{
	enumSKILL_NOT_MANUAL_ADD   = 6,	// 
};


class CSkillRecord : public CRawDataInfo
{
public:
	CSkillRecord();

	short	sID;												//
	std::string	szName;											//
	char    chFightType;										// 
	char	chJobSelect[defSKILL_JOB_SELECT_NUM][2];			// 
																// 0 1 2 3 4 5 6 7 8
	short	sItemNeed[3][defSKILL_ITEM_NEED_NUM][2];			// (0)(1)(2)
	short	sConchNeed[defSKILL_ITEM_NEED_NUM][3];				// 
	char	chPhase;											// 123456
	char	chType;												// 12
	short	sLevelDemand;										// 
	short	sPremissSkill[defSKILL_PRE_SKILL_NUM][2];			// 3
	char	chPointExpend;										// 
	char	chSrcType;											// 
	char	chTarType;											// 
	short	sApplyDistance;										// 
	char	chApplyTarget;										// 
	char	chApplyType;										// 123
	char	chHelpful;											// ,
	short	sAngle;												// 0-360
	short	sRadii;												// 
	char	chRange;											// 
	std::string	szPrepare;											//
	std::string	szUseSP;											// SP
	std::string	szUseEndure;										//
	std::string	szUseEnergy;										//
	std::string	szSetRange;											//
	std::string	szRangeState;										//
	std::string	szUse;												//
	std::string	szEffect;											//
	std::string	szActive;											//
	std::string	szInactive;											//
	int		nStateID;											// 
	short	sSelfAttr[defEFFECT_SELF_ATTR_NUM];					// 
	short	sSelfEffect[defSELF_EFFECT_NUM];					// 
	short	sItemExpend[defEXPEND_ITEM_NUM][2];					// ID
	short	sBeingTime;											// 
	short	sTargetAttr[defEFFECT_TAR_ATTR_NUM];				// 
	short	sSplashPara;										// 
	short	sTargetEffect;										// 
	short	sSplashEffect;										// 
	short	sVariation;											// 
	short	sSummon;											// 
	short	sPreTime;											// 
	std::string	szFireSpeed;										//
	char	chOperate[defSKILL_OPERATE_NUM];					// 012

public:		// 
	short	sActionHarm;										// 
	char	chActionPlayType;									// :0-,1-
	short	sActionPose[defSKILL_POSE_NUM];						// 
	short	sActionKeyFrme;										// 
	short	sWhop;												// 
	short	sActionDummyLink[defSKILL_ACTION_EFFECT];			// link
	short	sActionEffect[defSKILL_ACTION_EFFECT];				// 
	short	sActionEffectType[defSKILL_ACTION_EFFECT];			// :0-,1-
	short	sItemDummyLink;										// link
	short	sItemEffect1[defSKILL_ITEM_EFFECT];					// ,0-,1-
	short	sItemEffect2[defSKILL_ITEM_EFFECT];					// 
	short	sSkyEffectActionKeyFrame;							// 
	short   sSkyEffectActionDummyLink;							// dummy
	short   sSkyEffectItemDummyLink;							// dummy;
	short   sSkyEffect;											// 
	short	sSkySpd;											// 
	short	sWhoped;											// 
	short   sTargetDummyLink;									// link
	short	sTargetEffectID;									// 
	char	chTargetEffectTime;									//  ,0-12()
    short   sAgroundEffectID;                                   // ,
	short	sWaterEffectID;										// , 
	std::string	szICON;												//
	char	chPlayTime;											//
	std::string	szDescribeHint;									// hint
	std::string	szEffectHint;									// hint
	std::string	szExpendHint;									// hint

public:
    bool    IsPlayCyc()         { return chPlayTime==1;     }   // 
    bool    IsAttackArea()		{ return chApplyType==2;    }   // 
	bool	IsHarmRange()		{ return chApplyType!=1;	}	// 
    bool    IsActKeyHarm()      { return sActionHarm==1;    }   // 
    bool    IsEffectHarm()      { return sActionHarm==2;    }   // 
    bool    IsNoHarm()          { return sActionHarm==0;    }   // 
	bool	IsShow()			{ return szICON[1]!='\0';	}	// 
	bool	IsPlayRand()		{ return chActionPlayType==1;	}
	int		GetPoseNum()		{ return _nPoseNum;			}

public:
    void    SetSkillGrid( SSkillGridEx& v )   { _Skill = v;   }
    SSkillGridEx&     GetSkillGrid()  { return _Skill;        }

	int     GetSPExpend()       { return _Skill.sUseSP;     }   // SP
    short   GetRange()          { return _Skill.sRange[1];  }   // 
    short*  GetParam()          { return &_Skill.sRange[1]; }
    int     GetShape()          { return _Skill.sRange[0];  }   // 
    int     GetDistance()       { return sApplyDistance;    }   // 
    int     GetLevel()          { return _Skill.chLv;       }
    int     GetFireSpeed()      { return _Skill.lResumeTime;}   // 
    bool    GetIsValid()        { return _Skill.chState!=0; }   // 

	int     GetUpgrade()		{ return _nUpgrade;			}
	bool    GetIsUpgrade()		{ return _nUpgrade==enumSKILL_UPGRADE_CAN;	}

	bool	GetIsUse()			{ return chType!=2;			}
	bool	GetIsHelpful()		{ return chHelpful==1;		}	// 

	void	Refresh( int nJob );

	void	SetIsActive( bool v )	{ _IsActive = v;		}
	bool	GetIsActive()			{ return _IsActive;		}

	void	SetAttackTime( DWORD v ){ _dwAttackTime=v;			}
	bool	IsAttackTime( DWORD v )	{ return v>=_dwAttackTime;	}

	int		GetSelectCha()		{ return _eSelectCha;		}

	bool	IsAutoAttack()		{ return !IsPlayCyc() && GetDistance()>0;	}

	void	RefreshPrivateData();				// 
	
	bool	IsJobAllow( int nJob );
	int		GetJobMax( int nJob );

private:
    SSkillGridEx		_Skill;
	int					_nUpgrade;				// 0-,1-,2-

	bool				_IsActive;				// ,
	DWORD				_dwAttackTime;			// 

	eSelectCha			_eSelectCha;			// 

	int					_nPoseNum;				// pose

	// added by clp
public:
	bool IsReadingSkill()
	{
		return sID == 512;
	}
};

CSkillRecord* GetSkillRecordInfo(int nTypeID, const std::source_location& loc = std::source_location::current());
CSkillRecord* GetSkillRecordInfo(const char* szName, const std::source_location& loc = std::source_location::current());

inline bool CSkillRecord::IsJobAllow( int nJob )
{
	if( chJobSelect[0][0]==-1 ) return true;

	for (char i = 0; i<defSKILL_JOB_SELECT_NUM; i++ )
	{
		if (chJobSelect[i][0] == cchSkillRecordKeyValue)
			return false;
		else if( chJobSelect[i][0]==nJob )
			return true;
	}
	return false;
}

inline int CSkillRecord::GetJobMax( int nJob )
{
	if( chJobSelect[0][0]==-1 ) return chJobSelect[0][1];

	for (int i = 0; i<defSKILL_JOB_SELECT_NUM; i++ )
	{
		if (chJobSelect[i][0] == cchSkillRecordKeyValue)
			return -1;
		else if( chJobSelect[i][0]==nJob )
			return chJobSelect[i][1];
	}

	return -1;
}

#endif //SKILLRECORD_H
