#pragma once

//=============================================================================
// FileName: SkillStateRecord.h
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Record
//=============================================================================

#ifndef SKILLSTATERECORD_H
#define SKILLSTATERECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

const char cchSkillStateRecordKeyValue = (char)0xff;

#define defSKILLSTATE_NAME_LEN		17
#define defSKILLSTATE_SCRIPT_NAME	32
#define defSKILLSTATE_ACT_NUM		3
#define defSKILLSTATE_DESC_NAME_LEN	255

class CSkillStateRecord : public CRawDataInfo
{
public:
	// CSkillStateRecord();

	char	chID;									// 
	char	szName[defSKILLSTATE_NAME_LEN];			// 
	short	sFrequency;								// 
	char	szOnTransfer[defSKILLSTATE_SCRIPT_NAME];// 
	char	szAddState[defSKILLSTATE_SCRIPT_NAME];	// 
	char	szSubState[defSKILLSTATE_SCRIPT_NAME];	// 
	char	chAddType;								// 
	bool	bCanCancel;								// 
	bool	bCanMove;								// 
	bool	bCanMSkill;								// 
	bool	bCanGSkill;								// 
	bool	bCanTrade;								// 
	bool	bCanItem;								// 
	bool	bCanUnbeatable;							// 
	bool	bCanItemmed;							// 
	bool	bCanSkilled;							// 
	bool	bNoHide;								// 
	bool	bNoShow;								// 
	bool	bOptItem;								// 
	bool	bTalkToNPC;								// NPC
	char	bFreeStateID;							// 

	// 
	char	chScreen;								// 
	char    nActBehave[defSKILLSTATE_ACT_NUM];		// 
	short	sChargeLink;							// ,
    short   sAreaEffect;                            // 
	bool	IsShowCenter;							// ,
	bool	IsDizzy;								// 
	short	sEffect;								// 
	short	sDummy1;								// dummy
	short	sBitEffect;								// 
	short	sDummy2;								// dummy
	short	sIcon;									// ICON
	char	szIcon[defSKILLSTATE_NAME_LEN][10];		// icons for pots per level 
	char	szDesc[defSKILLSTATE_DESC_NAME_LEN];
	int		lColour;
public:
	void	RefreshPrivateData();

	int		GetActNum()			{ return _nActNum;		}

public:
	int		_nActNum;								// 

};

class CSkillStateRecordSet : public CRawDataSet
{
public:

	static CSkillStateRecordSet* I() { return _Instance; }

	CSkillStateRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSkillStateRecordSet* _Instance; // , 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSkillStateRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSkillStateRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSkillStateRecord);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList);
	virtual void _ProcessRawDataInfo(CRawDataInfo *pInfo);

};

inline CSkillStateRecord* GetCSkillStateRecordInfo( int nTypeID )
{
	return (CSkillStateRecord*)CSkillStateRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // SKILLSTATERECORD_H
