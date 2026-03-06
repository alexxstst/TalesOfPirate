#include "StdAfx.h"
#include "state_reading.h"
#include "actor.h"
#include "Character.h"
#include "PacketCmd.h"
#include "UIBoxForm.h"
#include "NetProtocol.h"

//---------------------------------------------------------------------------
// class CSeatState
//---------------------------------------------------------------------------
CReadingState::CReadingState(CActor* p)
: CActionState(p)
{
	_IsOver = true;
    _IsSend = true;
}

bool CReadingState::_Start()
{
	//add by ALLEN 2007-10-16
   
	CCharacter *character = GetActor()->GetCha();
	// play reading animation
	character->PlayPose( POSE_SKILL20, PLAY_LOOP );

	//CS_ReadBookStart();
	return true;

}

void CReadingState::_End()
{ 
	//add by ALLEN 2007-10-16
	CCharacter *character = GetActor()->GetCha();

	// play ending animation
	character->PlayPose( POSE_WAITING, PLAY_ONCE );
	//CS_ReadBookClose();
}

void CReadingState::FrameMove()
{

}
