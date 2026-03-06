#pragma once

//=============================================================================
// FileName: SkillTemp.h
// Creater: ZhangXuedong
// Date: 2005.03.03
// Comment: Skill Temp Data
//=============================================================================

#ifndef SKILLTEMP_H
#define SKILLTEMP_H

#include "PreAlloc.h"

#define defSKILL_RANGE_BASEP_NUM	3	// 
#define defSKILL_RANGE_EXTEP_NUM	4	// 
#define defSKILL_STATE_PARAM_NUM	3	// 

class CSkillTempData : public dbc::PreAllocStru
{
public:
	CSkillTempData(unsigned long ulSize = 1) : PreAllocStru(1)
	{
		sRange[0] = enumRANGE_TYPE_NONE;
		sStateParam[0] = SSTATE_NONE;
	}

	short	sUseEndure{};	// 
	short	sUseEnergy{};	// 
	short	sUseSP{};		// SP

	long	lResumeTime{};// 
	short	sRange[defSKILL_RANGE_EXTEP_NUM];	// +
	short	sStateParam[defSKILL_STATE_PARAM_NUM];	// 

protected:

private:
	void	Initially();
	void	Finally();

};

inline void CSkillTempData::Initially()
{
	sUseEndure = 0;
	sUseEnergy = 0;
	sUseSP = 0;
	lResumeTime = 0;
	sRange[0] = enumRANGE_TYPE_NONE;
	sStateParam[0] = SSTATE_NONE;
}

inline void CSkillTempData::Finally()
{
}

#endif // SKILLTEMP_H
