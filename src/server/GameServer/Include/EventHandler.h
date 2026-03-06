#pragma once

class CCharacter;

class CEventHandler
{


public:

	void	Event_ChaDie(CCharacter *pDead, CCharacter *pAtk);
	void	Event_ChaEmotion(CCharacter *pCha, short sMotionNo);

};

extern CEventHandler g_EventHandler;
