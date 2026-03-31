// ForgeRecord.h Created by knight-gongjian 2005.1.24.
//---------------------------------------------------------
#pragma once

#ifndef _FORGERECORD_H_
#define _FORGERECORD_H_

#include <tchar.h>
#include "util.h"
#include "TableData.h"

//---------------------------------------------------------
#define FORGE_MAXNUM_ITEM				6 //

class CForgeRecord : public CRawDataInfo
{
public:
	BYTE byLevel;	//
	BYTE byFailure; //
	BYTE byRate;	//
	BYTE byParam;	//
	DWORD dwMoney;  //

	//
	struct FORGE_ITEM
	{
		USHORT sItem;	// ID
		BYTE   byNum;	//
		BYTE   byParam; //
	};
	FORGE_ITEM ForgeItem[FORGE_MAXNUM_ITEM];
};

//---------------------------------------------------------
#endif // _FORGERECORD_H_
