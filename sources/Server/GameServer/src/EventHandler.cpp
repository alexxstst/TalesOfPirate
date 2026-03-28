#include "StdAfx.h"
#include "EventHandler.h"
#include "GameAppNet.h"
#include "Character.h"
#include "CommFunc.h"



//-------------------------------------
// �¼� : ��ɫ������
//-------------------------------------
void CEventHandler::Event_ChaEmotion(CCharacter *pCha, short sEmotionNo)
{
	// Типизированная сериализация: анимация эмоции
	auto wpk = net::msg::serialize(net::msg::McChaEmotionMessage{pCha->GetID(), (int64_t)(short)sEmotionNo});
	pCha->NotiChgToEyeshot(wpk, false);
}


CEventHandler g_EventHandler;
