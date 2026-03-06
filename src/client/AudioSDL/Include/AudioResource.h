#pragma once


//---------------------------
//
//  
//  by Jampe
//  2007/03/28
//
//---------------------------

#if !defined __AUDIO_RESOURCE_H__
#define __AUDIO_RESOURCE_H__

#include "datatype.h"
#include "lock.h"
#include <time.h>
#include <map>



typedef struct tagAudioInfo
{
    int                 type;
    ulong               code;
    time_t              stamp;
    SDL_Audio_Base*     audio;
}AudioInfo;


class AudioRes : public _singleton<AudioRes>
{
    SINGLETON_OBJECT(AudioRes);
    typedef std::map<ulong, AudioInfo*> AudioQueue;
    typedef std::pair<ulong, AudioInfo*> AudioObj;
public:
    ~AudioRes()
    {
        release();
    }

    ulong get_resID(const char* resource, int type);
    AudioInfo* get_res(ulong id);
    AudioInfo* get_res(const char* resource);

    void close(const char* resource);
    void close(ulong id);
    void release();

    void checkResValid(ulong timeout = 300);     //  (),5

protected:
    AudioInfo* _addRes(const char* resource, int type);
    ulong _hashCode(const char* resource);
    AudioRes(){}

protected:
    AudioQueue      m_queue;
    Lock            m_lock;
};



#endif
