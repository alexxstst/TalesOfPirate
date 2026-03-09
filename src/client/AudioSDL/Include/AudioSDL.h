#pragma once


#if !defined __AUDIO_SDL_H__
#define __AUDIO_SDL_H__


#include "datatype.h"


class AudioSDL : public _singleton<AudioSDL> {
	SINGLETON_OBJECT(AudioSDL);

public:
	~AudioSDL();

	bool init();
	void release();
	bool is_valid();

	ulong get_resID(const char* resource, int type);

	bool play(ulong id, bool loop = false);
	bool fadeIn(ulong id, int ms, bool loop = false);
	bool fadeOut(ulong id, int ms);
	bool stop(ulong id);
	bool pause(ulong id);
	bool resume(ulong id);
	bool rewind(ulong id);

	bool is_playing(ulong id);
	bool is_paused(ulong id);
	bool is_stopped(ulong id);

	bool volume(ulong id, int vol);
	void checkRes(ulong timeout = 300);

protected:
	AudioSDL();
};


#define GetAudioDevice()            AudioSDL::get_instance()


#endif
