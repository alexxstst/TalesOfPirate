//======================================================================================================================
// FileName: MonRefRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.05
// Comment: CMonRefRecord — DTO региона спауна монстров (использует MonRefRecordStore).
//======================================================================================================================

#ifndef	MONSTERREFRESH_H
#define	MONSTERREFRESH_H

#include "TableData.h"
#include "point.h"
using	namespace	GAME;

#define defMAX_REGION_MONSTER_TYPE	15	// максимум типов монстров в регионе

// Регион спауна монстров на карте. Одна запись = один прямоугольник с набором типов.
class CMonRefRecord : public EntityData
{
public:
	long	lID;                                        // дубликат nID
	Point	SRegion[2];                                 // [0]=начало, [1]=конец прямоугольника
	short	sAngle;                                     // направление (−1 = случайное)
	long	lMonster[defMAX_REGION_MONSTER_TYPE][4];    // [monsterID, count, probability, refreshTime]
};

#endif //MONSTERREFRESH_H
