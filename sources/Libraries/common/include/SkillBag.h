//=============================================================================
// FileName: SkillBag.h
// Creater: ZhangXuedong
// Date: 2005.02.17
// Comment: Skill Bag
//=============================================================================

#ifndef SKILLBAG_H
#define SKILLBAG_H

#include "CompCommand.h"

#define defMAX_SKILL_NO		500
#define defMAX_SKILL_LV		20

// ,CompCommand.h - ESynSkillBagType
// ,,
// ,

#define defSKILL_RANGE_PARAM_NUM	4

struct SSkillGrid
{
	char	chState;	// CompCommand.hESkillUseState
	char	chLv;		// 
	short	sID;		// 

	uint32_t	lColdDownT;	// 

	short	sReverseID;	// 
};

struct SSkillGridEx // 
{
	char	chState;	// CompCommand.hESkillUseState
	char	chLv;		// 
	short	sID;		// 
	short	sUseSP;		// SP
	short	sUseEndure;	// 
	short	sUseEnergy;	// 
	long	lResumeTime;// 
	short	sRange[defSKILL_RANGE_PARAM_NUM]; // +
};

class CSkillBag
{
public:
	CSkillBag()
	{
		m_sSkillNum = 0;
		for (short i = 0; i <= defMAX_SKILL_NO; i++)
		{
			m_SBag[i].sID = i;
			m_SBag[i].chLv = 0;
			m_SBag[i].chState = enumSUSTATE_INACTIVE;
			m_SBag[i].lColdDownT = 0;
		}
		SetChangeFlag(false);
	}
	void		Init(void);
	bool		Add(SSkillGrid *pGridCont);
	bool		Del(short sSkillID);
	SSkillGrid*	GetSkillContByID(short sSkillID);
	SSkillGrid*	GetSkillContByNum(short sNum);
	short		GetSkillNum();
	bool		HasSkill(short sID);
	bool		SetState(short sID, char chState);
	char		GetState(short sID);

	void		SetChangeFlag(bool bChange = true);
	short		GetChangeSkillNum();
	SSkillGrid*	GetChangeSkill(short sChangeID);

	SSkillGrid	*m_pSBag[defMAX_SKILL_NO + 1];
	short		m_sSkillNum;

protected:

private:
	SSkillGrid	m_SBag[defMAX_SKILL_NO + 1];

	short		m_sChangeNum;
	short		m_sChangeSkill[defMAX_SKILL_NO + 1];
	char		m_szSkillIsChanged[defMAX_SKILL_NO + 1];
};

inline void CSkillBag::Init(void)
{
	m_sSkillNum = 0;
	for (short i = 0; i <= defMAX_SKILL_NO; i++)
	{
		m_SBag[i].sID = i;
		m_SBag[i].chLv = 0;
		m_SBag[i].lColdDownT = 0;
		m_SBag[i].chState = enumSUSTATE_INACTIVE;
	}
	SetChangeFlag(false);
}

inline void CSkillBag::SetChangeFlag(bool bChange)
{
	memset(m_szSkillIsChanged, 0, sizeof(char) * (defMAX_SKILL_NO + 1));
	if (bChange)
	{
		m_sChangeNum = m_sSkillNum;

		for (short i = 0; i < m_sChangeNum; i++)
		{
			m_sChangeSkill[i] = i;
			m_szSkillIsChanged[m_pSBag[i]->sID] = 1;
		}
	}
	else
		m_sChangeNum = 0;
}

inline bool CSkillBag::Add(SSkillGrid *pGridCont)
{
	if (pGridCont->sID < 0 || pGridCont->sID > defMAX_SKILL_NO)
		return false;

	if (pGridCont->chLv == 0)
		return false;

	if ((unsigned char)pGridCont->chLv > defMAX_SKILL_LV)
		pGridCont->chLv = defMAX_SKILL_LV;

	if (m_SBag[pGridCont->sID].chLv == 0)
	{
		m_pSBag[m_sSkillNum] = m_SBag + pGridCont->sID;
		m_SBag[pGridCont->sID].sReverseID = m_sSkillNum;
		m_sSkillNum++;
	}

	if (m_SBag[pGridCont->sID].chState != pGridCont->chState || m_SBag[pGridCont->sID].chLv != pGridCont->chLv)
	{
		if (m_szSkillIsChanged[pGridCont->sID] == 0)
		{
			m_szSkillIsChanged[pGridCont->sID] = 1;
			m_sChangeSkill[m_sChangeNum] = m_SBag[pGridCont->sID].sReverseID;
			m_sChangeNum++;
		}
	}

	m_SBag[pGridCont->sID].chState = pGridCont->chState;
	m_SBag[pGridCont->sID].chLv = pGridCont->chLv;
	m_SBag[pGridCont->sID].lColdDownT = 0;

	return true;
}

inline bool CSkillBag::Del(short sSkillID)
{
	if (sSkillID < 0 || sSkillID > defMAX_SKILL_NO)
		return false;

	if (m_SBag[sSkillID].chLv != 0)
	{
		m_sSkillNum--;
		m_SBag[sSkillID].chLv = 0;
		m_pSBag[m_SBag[sSkillID].sReverseID] = m_pSBag[m_sSkillNum];
		m_pSBag[m_sSkillNum]->sReverseID = m_SBag[sSkillID].sReverseID;
		m_pSBag[m_sSkillNum] = &m_SBag[sSkillID];
		m_SBag[sSkillID].sReverseID = m_sSkillNum;

		if (m_szSkillIsChanged[sSkillID] == 0)
		{
			m_szSkillIsChanged[sSkillID] = 1;
			m_sChangeSkill[m_sChangeNum] = m_sSkillNum;
			m_sChangeNum++;
		}
	}

	return true;
}

inline SSkillGrid* CSkillBag::GetSkillContByID(short sSkillID)
{
	if (sSkillID < 0 || sSkillID > defMAX_SKILL_NO)
		return 0;

	if (m_SBag[sSkillID].chLv == 0)
		return 0;
	else
		return m_SBag + sSkillID;
}

inline SSkillGrid* CSkillBag::GetSkillContByNum(short sNum)
{
	if (sNum < 0 || sNum >= m_sSkillNum)
		return nullptr;

	if (m_pSBag[sNum]->chLv == 0)
		return nullptr;
	else
		return m_pSBag[sNum];
}

inline short CSkillBag::GetSkillNum()
{
	return m_sSkillNum;
}

inline bool CSkillBag::HasSkill(short sSkillID)
{
	if (sSkillID < 0 || sSkillID > defMAX_SKILL_NO)
		return false;

	if (m_SBag[sSkillID].chLv > 0)
		return true;

	return false;
}

inline bool CSkillBag::SetState(short sID, char chState)
{
	if (sID < 0) // 
	{
		for (short i = 0; i < m_sSkillNum; i++)
		{
			if (m_pSBag[i]->chState == chState)
				continue;
			m_pSBag[i]->chState = chState;
			if (m_szSkillIsChanged[m_pSBag[i]->sID] == 0)
			{
				m_szSkillIsChanged[m_pSBag[i]->sID] = 1;
				m_sChangeSkill[i] = i;
				m_sChangeNum++;
			}
		}
	}
	else
	{
		if (sID > defMAX_SKILL_NO)
			return false;
		else
		{
			if (m_SBag[sID].chState != chState)
			{
				m_SBag[sID].chState = chState;
				if (m_szSkillIsChanged[sID] == 0)
				{
					m_szSkillIsChanged[sID] = 1;
					m_sChangeSkill[m_sChangeNum] = m_SBag[sID].sReverseID;
					m_sChangeNum++;
				}
			}
		}
	}

	return true;
}

inline char CSkillBag::GetState(short sID)
{
	if (sID < 0 || sID > defMAX_SKILL_NO)
		return enumSUSTATE_INACTIVE;

	if (m_SBag[sID].chLv == 0)
		return enumSUSTATE_INACTIVE;

	return m_SBag[sID].chState;
}

inline short CSkillBag::GetChangeSkillNum()
{
	return m_sChangeNum;
}

inline SSkillGrid* CSkillBag::GetChangeSkill(short sChangeID)
{
	if (sChangeID < 0 || sChangeID >= m_sChangeNum)
		return 0;

	if (m_sChangeSkill[sChangeID] < 0 || m_sChangeSkill[sChangeID] > defMAX_SKILL_NO)
		return 0;

	return m_pSBag[m_sChangeSkill[sChangeID]];
}

//=============================================================================
inline char* SkillBagData2String(CSkillBag *pSkillBag, char *szStrBuf, int nLen)
{
	if (!pSkillBag || !szStrBuf) return NULL;

	char	szData[512];
	int		nBufLen = 0, nDataLen;
	szStrBuf[0] = '\0';

	SSkillGrid *pGridCont;
	sprintf(szData, "%d;", pSkillBag->m_sSkillNum);
	nDataLen = (int)strlen(szData);
	if (nBufLen + nDataLen >= nLen) return NULL;
	strcat(szStrBuf, szData);
	nBufLen += nDataLen;
	for (int j = 0; j < pSkillBag->m_sSkillNum; j++)
	{
		pGridCont = pSkillBag->m_pSBag[j];
		sprintf(szData, "%d,%d;", pGridCont->sID, pGridCont->chLv);
		nDataLen = (int)strlen(szData);
		if (nBufLen + nDataLen >= nLen) return NULL;
		strcat(szStrBuf, szData);
		nBufLen += nDataLen;
	}

	return szStrBuf;
}

inline bool String2SkillBagData(CSkillBag *pSkillBag, std::string &strData)
{
	if (!pSkillBag)
		return false;

	pSkillBag->Init();

	int nCount = 0;
	std::string strList[defMAX_SKILL_NO + 1];
	std::string strSubList[2];
	Util_ResolveTextLine(strData.c_str(), strList, defMAX_SKILL_NO + 1, ';');
	const auto sSkillNum = Str2Int(strList[nCount++]);
	SSkillGrid SGridCont;
	for (int j = 0; j < sSkillNum; j++)
	{
		Util_ResolveTextLine(strList[nCount++].c_str(), strSubList, 3, ',');
		SGridCont.sID = Str2Int(strSubList[0]);
		SGridCont.chLv = Str2Int(strSubList[1]);

		SGridCont.chState = enumSUSTATE_INACTIVE;
		pSkillBag->Add(&SGridCont);
	}

	return true;
}

#endif // SKILLBAG_H
