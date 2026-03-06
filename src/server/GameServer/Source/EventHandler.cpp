#include "StdAfx.h"
#include "EventHandler.h"
#include "GameAppNet.h"
#include "Character.h"
#include "CommFunc.h"



//-------------------------------------
//  : 
//-------------------------------------
void CEventHandler::Event_ChaEmotion(CCharacter *pCha, short sEmotionNo)
{
	WPACKET	wpk = GETWPACKET();
	WRITE_CMD(wpk, CMD_MC_CHA_EMOTION);
	WRITE_LONG(wpk, pCha->GetID());
	WRITE_SHORT(wpk, (short)sEmotionNo);
	pCha->NotiChgToEyeshot(wpk, false);
}


CEventHandler g_EventHandler;
