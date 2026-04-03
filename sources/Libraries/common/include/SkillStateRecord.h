//=============================================================================
// FileName: SkillStateRecord.h
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Record
//=============================================================================

#ifndef SKILLSTATERECORD_H
#define SKILLSTATERECORD_H

#include <tchar.h>
#include <string>
#include <source_location>
#include "util.h"
#include "TableData.h"

const char cchSkillStateRecordKeyValue = (char)0xff;

#define defSKILLSTATE_NAME_LEN		17
#define defSKILLSTATE_SCRIPT_NAME	32
#define defSKILLSTATE_ACT_NUM		3
#define defSKILLSTATE_DESC_NAME_LEN	255

class CSkillStateRecord : public CRawDataInfo {
public:
	// CSkillStateRecord();

	char chID; //
	std::string szName; //
	short sFrequency; //
	std::string szOnTransfer; //
	std::string szAddState; //
	std::string szSubState; //
	char chAddType; //
	bool bCanCancel; //
	bool bCanMove; //
	bool bCanMSkill; //
	bool bCanGSkill; //
	bool bCanTrade; //
	bool bCanItem; //
	bool bCanUnbeatable; //
	bool bCanItemmed; //
	bool bCanSkilled; //
	bool bNoHide; //
	bool bNoShow; //
	bool bOptItem; //
	bool bTalkToNPC; // NPC
	char bFreeStateID; //

	//
	char chScreen; //
	char nActBehave[defSKILLSTATE_ACT_NUM]; //
	short sChargeLink; // ,
	short sAreaEffect; //
	bool IsShowCenter; // ,
	bool IsDizzy; //
	short sEffect; //
	short sDummy1; // dummy
	short sBitEffect; //
	short sDummy2; // dummy
	short sIcon; // ICON
	char szIcon[defSKILLSTATE_NAME_LEN][10]; // icons for pots per level
	std::string szDesc;
	int lColour;

public:
	void RefreshPrivateData();

	int GetActNum() {
		return _nActNum;
	}

public:
	int _nActNum; //
};

CSkillStateRecord* GetCSkillStateRecordInfo(int nTypeID, const std::source_location& loc = std::source_location::current());

#endif // SKILLSTATERECORD_H
