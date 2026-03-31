//=============================================================================
// FileName: JobInitEquip.h
// Creater: ZhangXuedong
// Date: 2005.01.08
// Comment:
//=============================================================================

#ifndef JOBINITEQUIP_H
#define JOBINITEQUIP_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

#define defJOB_INIT_EQUIP_MAX	6

class CJobEquipRecord : public CRawDataInfo
{
public:
	char	chID;			//
	char	chJob;			//
	short	sItemID[defJOB_INIT_EQUIP_MAX];		//
};

#endif // JOBINITEQUIP_H
