//----------------------------------------------------------------------
// :
// :lh 2004-05-26
// :
// :
//----------------------------------------------------------------------
#pragma once

class CSkillRecord;
class CCharacter;

// ,
class CIsSkillUse {
public:
	CIsSkillUse();

	// 
	bool IsValid(CSkillRecord* pSkill, CCharacter* pSelf);

	// 
	bool IsUse(CSkillRecord* pSkill, CCharacter* pSelf, CCharacter* pTarget);

	// 
	bool IsAttack(CSkillRecord* pSkill, CCharacter* pSelf, CCharacter* pTarget);

	// ,
	const char* GetError();

private:
	enum eError {
		enumNone,

		enumSelf,
		enumFish,
		enumDieBoat,
		enumOnlyTeam,
		enumTree,
		enumMine,
		enumDie,
		enumRepair,

		enumAttackMain,
		enumAttackTeam,
		enumHelpMons,
		enumAttackDie,
		enumTargetError,
		enumAttackPlayer,

		enumInValid,
		enumNotEnergy,
		enumNotAttack,
		enumNotUse,
		enumNotMP,
		enumNotTarget,
	};

	eError _eError;
	CSkillRecord* _pSkill;
};

extern CIsSkillUse g_SkillUse;
