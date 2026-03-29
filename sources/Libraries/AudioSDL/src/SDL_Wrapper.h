
//---------------------------
//
//  SDL
//  by Jampe
//  2007/03/28
//
//---------------------------



#if !defined __SDL_WRAPPER_H__
#define __SDL_WRAPPER_H__


#include "../inc/datatype.h"
#include <SDL.h>
#include <SDL_mixer.h>


#define CHUNK_SIZE              4096

//  for wave
class SDL_Sound : public SDL_Audio_Base
{
public:
    SDL_Sound()
        :   m_device(0), m_channel(-1)
    {}
    virtual ~SDL_Sound()
    {
        release();
    }

    bool init(const char* filename);
    bool release();

    bool play();
    bool fadeIn(int ms);
    bool fadeOut(int ms);
    bool stop();
    bool pause();
    bool resume();
    bool rewind();  //  always false

    bool is_playing();
    bool is_paused();

    void set_volume(int vol);

    virtual int get_channel()
    {
        return m_channel;
    }

private:
    int         m_channel;
    Mix_Chunk*  m_device;
};



//  for mp3
class SDL_Music : public SDL_Audio_Base
{
public:
    SDL_Music()
        :   m_device(0)
    {}
    virtual ~SDL_Music()
    {
        release();
    }

    bool init(const char* filename);
    bool release();

    bool play();
    bool fadeIn(int ms);
    bool fadeOut(int ms);
    bool stop();
    bool pause();
    bool resume();
    bool rewind();

    bool is_playing();
    bool is_paused();

    void set_volume(int vol);

private:
    Mix_Music*  m_device;
};


#endif
