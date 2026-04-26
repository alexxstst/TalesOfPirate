#pragma once


#include "ThreadBase.h"

#include <cstdint>


class CAudioThread : public CThreadBase {
public:
	CAudioThread(void);
	virtual ~CAudioThread(void);

	unsigned int Run();

	void play(std::uint32_t musID, bool loop = false);
	void FrameMove();

private:
	std::uint32_t _nCurMusicID;
	bool _bLoop;
	DWORD _nLastTime;
};
