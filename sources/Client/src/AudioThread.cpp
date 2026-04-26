#include "stdafx.h"
#include "audiothread.h"
#include "audiosdl.h"

using namespace Corsairs::Client::Audio;


CAudioThread::CAudioThread(void)
	: _nCurMusicID(0), _bLoop(false), _nLastTime(0) {
}

CAudioThread::~CAudioThread(void) {
}


void CAudioThread::play(std::uint32_t musID, bool loop) {
	_nCurMusicID = musID;
	_bLoop = loop;
}


unsigned int CAudioThread::Run() {
	for (;;) {
		if (_nCurMusicID) {
			AudioSDL::Instance().Play(_nCurMusicID, _bLoop);
			_nCurMusicID = 0;
		}
		_nLastTime = GetTickCount();
		Sleep(30);
	}

	return 0;
}


void CAudioThread::FrameMove() {
	if (!_nLastTime) {
		Begin();
	}

	if ((GetTickCount() - _nLastTime) > 1000) {
		Terminate();
		Begin();
	}
}
