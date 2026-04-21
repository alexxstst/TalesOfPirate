// NpcRecord.h Created by knight-gongjian 2004.11.24.
//---------------------------------------------------------
#pragma once

#ifndef _NPCRECORD_H_
#define _NPCRECORD_H_

#include <tchar.h>
#include "util.h"
#include "TableData.h"

//---------------------------------------------------------

#define NPC_MAXSIZE_NAME			128 // npc
#define NPC_MAXSIZE_MSGPROC			16	// npc

// DTO одной NPC-записи карты. Загружается через NpcRecordStore.
class CNpcRecord : public EntityData {
public:
	char szName[NPC_MAXSIZE_NAME];
	USHORT sNpcType;
	USHORT sCharID;
	BYTE byShowType;
	DWORD dwxPos0, dwyPos0;
	DWORD dwxPos1, dwyPos1;
	USHORT sDir;
	USHORT sParam1, sParam2;
	char szNpc[NPC_MAXSIZE_NAME];
	char szMsgProc[NPC_MAXSIZE_MSGPROC];
	char szMisProc[NPC_MAXSIZE_MSGPROC];
};

//---------------------------------------------------------

#endif // _NPCRECORD_H_
