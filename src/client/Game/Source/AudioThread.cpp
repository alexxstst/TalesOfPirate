
#include "stdafx.h"
#include "audiothread.h"
#include "audiosdl.h"


CAudioThread::CAudioThread(void)
:_nCurMusicID(0), _bLoop(false), _nLastTime(0)
{
}

CAudioThread::~CAudioThread(void)
{
}


void CAudioThread::play(DWORD musID, bool loop)
{
    _nCurMusicID = musID;
    _bLoop = loop;
}


unsigned int CAudioThread::Run()
{
    for(;;)
    {
        if(_nCurMusicID)
        {
            try
            {
                AudioSDL::get_instance()->play(_nCurMusicID, _bLoop);
            }
            catch(...)
            {
            }

            _nCurMusicID = 0;
        }
        _nLastTime = GetTickCount();
        Sleep(30);
    }

    return 0;
}


void CAudioThread::FrameMove()
{
    if(!_nLastTime)
    {
        Begin();
    }

    if((GetTickCount() - _nLastTime) > 1000)
    {
        Terminate();
        Begin();
    }
}
