
#include "../inc/AudioSDL.h"
#include "AudioResource.h"
#include "SDL_Wrapper.h"


bool g_sdl_audio_valid = false;

AudioSDL::AudioSDL()
{
}

AudioSDL::~AudioSDL()
{
}

bool AudioSDL::init()
{
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		return false;
	}
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, CHUNK_SIZE) < 0)
	{
		return false;
	}
	return (g_sdl_audio_valid = true);
}

void AudioSDL::release()
{
	Mix_CloseAudio();
	SDL_Quit();
    g_sdl_audio_valid = false;
}

bool AudioSDL::is_valid()
{
    return g_sdl_audio_valid;
}

ulong AudioSDL::get_resID(const char* resource, int type)
{
    return AudioRes::get_instance()->get_resID(resource, type);
}

bool AudioSDL::play(ulong id, bool loop /* = false */)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->play(loop);
}

bool AudioSDL::fadeIn(ulong id, int ms, bool loop /* = false */)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->fadeIn(ms, loop);
}

bool AudioSDL::fadeOut(ulong id, int ms)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->fadeOut(ms);
}

bool AudioSDL::stop(ulong id)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->stop();
}

bool AudioSDL::pause(ulong id)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->pause();
}

bool AudioSDL::resume(ulong id)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->resume();
}

bool AudioSDL::rewind(ulong id)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->rewind();
}

bool AudioSDL::is_playing(ulong id)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->is_playing();
}

bool AudioSDL::is_paused(ulong id)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->is_paused();
}

bool AudioSDL::is_stopped(ulong id)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    return audio->audio->is_stopped();
}

bool AudioSDL::volume(ulong id, int vol)
{
    AudioInfo* audio = AudioRes::get_instance()->get_res(id);
    if(!audio)
    {
        return false;
    }
    audio->audio->set_volume(vol);
    return true;
}

void AudioSDL::checkRes(ulong timeout /* = 300 */)
{
    AudioRes::get_instance()->checkResValid(timeout);
}

