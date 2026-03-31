//======================================================================================================================
// FileName: HairRecord.h
// Creater: Jerry li
// Date: 2005.08.29
// Comment: CHairRecordSet class
//======================================================================================================================

#ifndef	HAIRRECORD_H
#define	HAIRRECORD_H

#include <tchar.h>
#include <string>
#include "util.h"
#include "TableData.h"

#define defHAIR_MAX_ITEM		4
#define defHAIR_MAX_FAIL_ITEM	3

class CHairRecord : public CRawDataInfo
{
public:
	CHairRecord();
	std::string	szColor;

	DWORD	dwNeedItem[defHAIR_MAX_ITEM][2];		// ID,
	DWORD	dwMoney;								// 
	DWORD	dwItemID;								// ()ID
	DWORD	dwFailItemID[defHAIR_MAX_FAIL_ITEM];	// 
	bool	IsChaUse[4];							// 4
	
	int		GetFailItemNum()		{ return _nFailNum;		}
	void	RefreshPrivateData();

private:
	int		_nFailNum;
	
};

CHairRecord* GetHairRecordInfo(int nTypeID);

#endif //HAIRRECORD_H
