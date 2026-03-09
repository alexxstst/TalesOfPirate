#pragma once

//=============================================================================
// FileName: ChaAttr.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CChaAttr class
//=============================================================================

#ifndef CHARACTERATTRIBUTE_H
#define CHARACTERATTRIBUTE_H

#include <memory.h>
#include "ChaAttrType.h"
#include "util.h"

extern LONG32 g_lMaxChaAttr[ATTR_MAX_NUM];

class CChaAttr
{
public:
	CChaAttr();
	~CChaAttr();

	void    Clear();
	void	Init(long lID, bool bFromFile = true);
	LONG32	GetAttr(long lNo);
	long	SetAttr(long lNo, LONG32 lVal);
	LONG32	GetAttrMaxVal(long lNo);
	long	DirectSetAttr(long lNo, LONG32 lVal); // 
	long	AddAttr(long lNo, LONG32 lVal);
	long	ReEvaluation(long lNo);

	void	SetChangeFlag();
	void	ResetChangeFlag();
	void	SetChangeFlag(const char *cszChangeFlag, unsigned short usLen = ATTR_SIGN_BYTE_NUM); // m_sChangeNumClient
	char*	GetChangeFlag(unsigned short& usLen);
	void	SetChangeBitFlag(long lBit);
	bool	GetChangeBitFlag(long lBit);
	short	GetChangeNumClient(void) {return m_sChangeNumClient;}

	long	m_lID;
	char	m_szName[32];

private:

	LONG32	m_lAttribute[ATTR_MAX_NUM];

	char	m_szChangeFlag[ATTR_SIGN_BYTE_NUM]; // 
	short	m_sChangeNumClient; // 

};

inline LONG32 CChaAttr::GetAttr(long lNo)
{
	if (lNo >= ATTR_MAX_NUM)
		return -1;

	return m_lAttribute[lNo];
}

inline LONG32 CChaAttr::GetAttrMaxVal(long lNo)
{
	if (lNo >= ATTR_MAX_NUM)
		return -1;

	return g_lMaxChaAttr[lNo];
}

// 012
inline long CChaAttr::SetAttr(long lNo, LONG32 lVal)
{
	if (lNo >= ATTR_MAX_NUM) return 0;

	long	lRet = 2;
	if (lVal > (signed __int64)(unsigned int)(g_lMaxChaAttr[lNo]))
	{
		lRet = 1;
		//if (!g_IsNosignChaAttr(lNo))
		{
			lVal = (int)g_lMaxChaAttr[lNo];
			//LG("attr_error", "Cha[%s] Attrib[%d][%d]\n", m_szName, lNo, lVal); 
			ToLogService("attr_error", "set Cha {} Attrib {} greater than max value {}", m_szName, lNo, lVal);
		}
	}
	
	if (m_lAttribute[lNo] != lVal)
	{
		SetChangeBitFlag(lNo);
		m_lAttribute[lNo] = lVal;
	}

	return lRet;
}

// 
inline long CChaAttr::DirectSetAttr(long lNo, LONG32 lVal)
{
	if (lNo >= ATTR_MAX_NUM) return 0;

	if (m_lAttribute[lNo] != lVal)
	{
		SetChangeBitFlag(lNo);
		m_lAttribute[lNo] = lVal;
	}

	return 1;
}

inline long CChaAttr::AddAttr(long lNo, LONG32 lVal)
{
	if (lNo >= ATTR_MAX_NUM)
		return 0;

	SetAttr(lNo, m_lAttribute[lNo] + lVal);
	return 1;
}

inline long CChaAttr::ReEvaluation(long lNo)
{
	if (lNo >= ATTR_MAX_NUM)
		return 0;

	//ExpressionParse(

	return 1;
}

inline void CChaAttr::SetChangeFlag()
{
	memset(m_szChangeFlag, 0xff, sizeof(char) * ATTR_SIGN_BYTE_NUM);
	m_sChangeNumClient = ATTR_CLIENT_MAX;
}

inline void CChaAttr::ResetChangeFlag()
{
	memset(m_szChangeFlag, 0, sizeof(char) * ATTR_SIGN_BYTE_NUM);
	m_sChangeNumClient = 0;
}

// m_sChangeNumClient
inline void CChaAttr::SetChangeFlag(const char *cszChangeFlag, unsigned short usLen)
{
	if (!cszChangeFlag)
		return;
	if (usLen > ATTR_SIGN_BYTE_NUM)
		usLen = ATTR_SIGN_BYTE_NUM;

	memcpy(m_szChangeFlag, cszChangeFlag, usLen);
}

inline char* CChaAttr::GetChangeFlag(unsigned short& usLen)
{
	usLen = ATTR_SIGN_BYTE_NUM;
	return m_szChangeFlag;
}

inline void CChaAttr::SetChangeBitFlag(long lBit)
{
	if (lBit >= ATTR_MAX_NUM)
		return;

	short sByteNO, sBitNO;
	sByteNO = short(lBit / 8);
	sBitNO = short(lBit % 8);

	char	chSetFlag = 0x01 << sBitNO;

	if (lBit < ATTR_CLIENT_MAX && !(m_szChangeFlag[sByteNO] & chSetFlag))
		m_sChangeNumClient++;

	size_t t = sizeof(CChaAttr);
	m_szChangeFlag[sByteNO] |= chSetFlag;
}

inline bool	CChaAttr::GetChangeBitFlag(long lBit)
{
	if (lBit >= ATTR_MAX_NUM)
		return false;

	short sByteNO, sBitNO;
	sByteNO = short(lBit / 8);
	sBitNO = short(lBit % 8);

	return m_szChangeFlag[sByteNO] & (0x01 << sBitNO) ? true : false;
}

#endif // CHARACTERATTRIBUTE_H
