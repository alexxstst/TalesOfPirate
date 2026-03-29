
#include "AudioResource.h"
#include "SDL_Wrapper.h"
#include <string.h>


extern bool g_sdl_audio_valid;

ulong AudioRes::get_resID(const char* resource, int type)
{
    AudioInfo* audio = get_res(resource);
    if(!audio)
    {
        audio = _addRes(resource, type);
        if(!audio)
        {
            return 0;
        }
    }
    return audio->code;
}

AudioInfo* AudioRes::get_res(ulong id)
{
    AutoLock(&m_lock);

    AudioQueue::iterator it = m_queue.find(id);
    if(it != m_queue.end())
    {
        AudioInfo* audio = it->second;
        audio->stamp = time(0);
        return audio;
    }
    return 0;
}

AudioInfo* AudioRes::get_res(const char* resource)
{
    ulong hash = _hashCode(resource);
    return get_res(hash);
}

void AudioRes::close(const char* resource)
{
    ulong hash = _hashCode(resource);
    close(hash);
}

void AudioRes::close(ulong id)
{
    AutoLock(&m_lock);

    AudioQueue::iterator it = m_queue.find(id);
    if(it != m_queue.end())
    {
        m_queue.erase(it);
    }
}

void AudioRes::release()
{
    AutoLock(&m_lock);

    AudioQueue::iterator it;
    for(it = m_queue.begin(); it != m_queue.end(); it++)
    {
        it->second->audio->release();
        SAFE_DELETE(it->second->audio);
        SAFE_DELETE(it->second);
    }
    m_queue.clear();
}

void AudioRes::checkResValid(ulong timeout /* = 300 */)
{
    AutoLock(&m_lock);

    time_t now = time(0);
    AudioQueue::iterator it;
    for(it = m_queue.begin(); it != m_queue.end(); it++)
    {
        if((ulong)(now - it->second->stamp) > timeout)
        {
            if(it->second->audio->is_stopped())
            {
                m_queue.erase(it);
            }
        }
    }
}

AudioInfo* AudioRes::_addRes(const char* resource, int type)
{
    if(!g_sdl_audio_valid)
    {
        return 0;
    }

    AutoLock(&m_lock);

    AudioInfo* audio = new AudioInfo;
    ulong code = _hashCode(resource);

    memset(audio, 0, sizeof(AudioInfo));

    switch(type)
    {
    case TYPE_WAV:
        {
            audio->audio = new SDL_Sound;
        }  break;
    case TYPE_MP3:
    case TYPE_OGG:
        {
            audio->audio = new SDL_Music;
        }  break;
    default:
        {
            SAFE_DELETE(audio);
            return 0;
        }  break;
    }
    audio->type = type;
    audio->code = code;
    audio->stamp = time(0);
    if(audio->audio->init(resource))
    {
        m_queue.insert(AudioObj(audio->code, audio));
        return audio;
    }

    SAFE_DELETE(audio);
    return 0;
}

ulong AudioRes::_hashCode(const char* resource)
{
    size_t len = strlen(resource);
    ulong hash = (ulong)len;
	size_t step = (len >> 5 ) + 1;

	for(size_t i = len; i >= step; i -= step)
    {
	    hash = hash ^ ((hash << 5) + (hash >> 2) + (ulong)resource[i - 1]);
    }
	return hash;
}

