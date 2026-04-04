//=============================================================================
// FileName: SkillTemp.h
// Creater: ZhangXuedong
// Date: 2005.03.03
// Comment: Skill Temp Data
//=============================================================================

#ifndef SKILLTEMP_H
#define SKILLTEMP_H

#define defSKILL_RANGE_BASEP_NUM	3	//
#define defSKILL_RANGE_EXTEP_NUM	4	//
#define defSKILL_STATE_PARAM_NUM	3	//

class CSkillTempData
{
public:
	CSkillTempData()
	{
		sRange[0] = enumRANGE_TYPE_NONE;
		sStateParam[0] = SSTATE_NONE;
	}

	short	sUseEndure{};
	short	sUseEnergy{};
	short	sUseSP{};

	long	lResumeTime{};
	short	sRange[defSKILL_RANGE_EXTEP_NUM];
	short	sStateParam[defSKILL_STATE_PARAM_NUM];
};

#endif // SKILLTEMP_H
