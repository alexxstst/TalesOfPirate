//=============================================================================
// FileName: SwitchMapRecord.h
// Creater: ZhangXuedong
// Date: 2004.11.23
// Comment: CSwitchMapRecord — DTO точки телепорта между картами (использует SwitchMapRecordStore).
//=============================================================================

#ifndef SWITCHMAPRECORD_H
#define SWITCHMAPRECORD_H

#include <tchar.h>
#include "TableData.h"
#include "point.h"

#define defMAP_NAME_LEN		16

// Одна запись = один невидимый предмет-триггер на карте, при касании переводит на другую карту.
// Адрес записи сохраняется в CEvent::pTableRec — поэтому после Load стор не должен перевыделять вектор.
class CSwitchMapRecord : public EntityData
{
public:
	long	lID;                           // дубликат nID
	long	lEntityID;                     // ID предмета-триггера (ItemInfo)
	long	lEventID;                      // ID события
	Point	SEntityPos;                    // позиция триггера на текущей карте
	short	sAngle;                        // направление (−1 = случайное)
	_TCHAR	szTarMapName[defMAP_NAME_LEN]; // имя целевой карты
	Point	STarPos;                       // позиция появления на целевой карте
};

#endif
