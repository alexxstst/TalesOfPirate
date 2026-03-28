
#include <SDL.h>
#include <SDL_mixer.h>

#include "util.h"
#include "playsound.h"


//
// 1.0 �汾�����ֲ��ŷ��� by claude Fan at 2004-08-22
//


//
// 2.0 �汾�����ֲ��ŷ��� by claude Fan at 2004-10-20
//

const int ERROR_TIME = 10000;
const char *ERROR_FILE = "error_sound";

#include <stdexcept>
using namespace std;
#pragma warning(disable: 4311 4312)
#pragma comment(lib, "sdl.lib")
#pragma comment(lib, "sdl_mixer.lib")

// base define
#define MUS_LEN 9
#define MUS_ID_START 0
#define MUS_ID_END (MUS_ID_START + MUS_LEN - 1)

// env_snd define
#define ENV_SND_LEN 3
#define ENV_SND_ID_START MUS_ID_START
#define ENV_SND_ID_END (ENV_SND_ID_START + ENV_SND_LEN - 1)

// cmn_snd define
#define CMN_SND_LEN 5
#define CMN_SND_ID_START (ENV_SND_ID_START + ENV_SND_LEN)
#define CMN_SND_ID_END (CMN_SND_ID_START + CMN_SND_LEN - 1)

// bkg_snd define
#define BKG_SND_LEN 1
#define BKG_SND_ID_START (CMN_SND_ID_START + CMN_SND_LEN)
#define BKG_SND_ID_END (BKG_SND_ID_START + BKG_SND_LEN - 1)

#if (ENV_SND_LEN + CMN_SND_LEN + BKG_SND_LEN != MUS_LEN)
#error "MUS_LEN != ENV_SND_LEN + CMN_SND_LEN + BKG_SND_LEN"
#endif

// thread handle
HANDLE thrdpool[MUS_LEN];

// signal
HANDLE sig_exit;
HANDLE sig_task;
HANDLE sig_cmd_array[MUS_LEN];

// music init flag
bool mus_init_flag = false;

// main window handle and callback message id
HWND mus_hwnd = NULL;
UINT mus_msg_id = 0;

// share data about mix and background sound
struct mus_dat
    {
    // for main thread write, child thread read
    volatile bool used; // be in use or not

    int cmd; enum {CMD_NONE, CMD_PLAY, CMD_STOP, CMD_VOL};
    int group; // channel group
    int channel; // channel to play music
    char* fname; // music file name
    int vol; // volume
    bool loop; // loop or not
    bool mem_cache; // cache in memory or load from disk
    bool mix; // mix or background music

    // only for child thread write
    Mix_Chunk* wave; // wave chunk
    Mix_Music* music; // music
    int wave_cache_id;
    int errcode; enum {OP_SUCCESS, OP_FAILED, OP_FILE_NOT_FOUND};

    // for main thread read, child thread write
    volatile int status; enum {PMT_PLAYING, PMT_FREE}; // current status of child thread

    // process exit signal handle
    HANDLE sig_exit;

    // constructor
    mus_dat()
        {
        used = false; cmd = CMD_NONE; group = 0; channel = -1;
        fname = new char[MAX_PATH + 1]; memset(fname, 0, sizeof fname);
        vol = MIX_MAX_VOLUME; loop = true; mem_cache = false;
        mix = true; wave = NULL; music = NULL;
        wave_cache_id = -1; errcode = OP_SUCCESS; status = PMT_FREE;
        sig_exit = NULL;}
    ~mus_dat() {delete fname;}
    };
mus_dat mus_dat_array[MUS_LEN];


// wave cache and share data
#define WAV_CACHE_LEN (20)
#define WAV_CACHE_ID_START (0)
#define WAV_CACHE_ID_END (WAV_CACHE_ID_START + WAV_CACHE_LEN - 1)

#define WAV_CACHE_IS_FULL (-2)
#define WAV_CACHE_NOT_FOUND (-1)

static int cache_discard_id = -1; // the id prior to discard
inline void wav_cache_set_discard(int id) {cache_discard_id = id;}
inline int wav_cache_get_discard() {return cache_discard_id;}

static bool cmn_snd_enable_flag = true;
inline void cmn_snd_enable(bool enable) {cmn_snd_enable_flag = enable;}
inline bool is_cmn_snd_enable() {return cmn_snd_enable_flag;}
static bool env_snd_enable_flag = true;
inline void env_snd_enable(bool enable) {env_snd_enable_flag = enable;}
inline bool is_env_snd_enable() {return env_snd_enable_flag;}

struct wave_cache_dat
    {
    bool used; // be in use or not

    char* fname; // filename
    Uint8* buf; // start address of wave buffer
    Uint32 buf_len; // length of wave buffer
    DWORD time_stamp; // time stamp when loading
    volatile int ref_cnt; // reference count
    int vol; // volume

    // constructor
    wave_cache_dat()
        {
        used = false; fname = new char[MAX_PATH + 1]; memset(fname, 0, sizeof fname);
        buf = NULL, buf_len = 0; time_stamp = 0; vol = MIX_MAX_VOLUME;
        ref_cnt = 0;}
    ~wave_cache_dat()
        {
        delete fname; if (buf) {delete buf; buf = NULL;}
        buf_len = 0; ref_cnt = 0; used = false;}
    };
wave_cache_dat wave_cache_dat_array[WAV_CACHE_LEN];


// music customer interface
inline void mus_init(int id, bool used, int cmd, int group, int channel,
                     char const* fname, bool loop, bool mem_cache, bool mix)
    {
    mus_dat* pdat = &mus_dat_array[id];
    pdat->used = used; pdat->cmd = cmd; pdat->group = group;
    pdat->channel = channel; pdat->loop = loop;
    pdat->mem_cache = mem_cache; pdat->mix = mix;
    if (fname != NULL)
        {size_t len = min(MAX_PATH, strlen(fname));
        memcpy(pdat->fname, fname, len); pdat->fname[len] = 0;}
    return;}

#define env_snd_init(id, used, cmd, group, channel) \
    mus_init(id, used, cmd, group, channel, NULL, true, false, true)
#define cmn_snd_init(id, used, cmd, group, channel) \
    mus_init(id, used, cmd, group, channel, NULL, false, true, true)
#define bkg_snd_init(id, used, cmd) \
    mus_init(id, used, cmd, -1, -1, NULL, true, false, false)

inline bool is_env_snd(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    if ((pdat->loop == true) && (pdat->mem_cache == false)
        && (pdat->mix == true)) return true;
    else return false;}

inline bool is_cmn_snd(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    if ((pdat->loop == false) && (pdat->mem_cache == true)
        && (pdat->mix == true)) return true;
    else return false;}

inline bool is_bkg_snd(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    if ((pdat->mem_cache == false)
        && (pdat->mix == false)) return true;
    else return false;}

inline bool is_free(int id)
    {
    return (mus_dat_array[id].status == mus_dat::PMT_FREE) ? true : false;}

inline bool is_playing(int id) {return !is_free(id);}

inline void dump_mus_dat(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    ToLogService("common", LogLevel::Error, "thread#{} [{}], cmd={}, group={}, channel={}, fname={}, vol={}, loop={}, mem_cache={}, mix={}, wave={}, music={}, wave_cache_id={}, status={}",
            id, (pdat->used) ? "used" : "not use", pdat->cmd, pdat->group,
            pdat->channel, pdat->fname, pdat->vol, pdat->loop,
            pdat->mem_cache, pdat->mix, (void*)pdat->wave, (void*)pdat->music,
            pdat->wave_cache_id,
            (pdat->status == mus_dat::PMT_PLAYING) ? "playing" : "free");
    }

inline void dump_mus_dat()
    {
    for (int i = MUS_ID_START; i <= MUS_ID_END; ++ i) dump_mus_dat(i);
    ToLogService("common", LogLevel::Error, "");
    }

bool play_env_snd(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    pdat->used = true;
    pdat->status = mus_dat::PMT_PLAYING; // block the following request
    Mix_Chunk* wave = NULL;
    int loops = (pdat->loop) ? -1 : 0;

    wave = Mix_LoadWAV(pdat->fname);
    if (wave == NULL)
        {
        ToLogService("common", LogLevel::Error, "thread#{} failed Mix_LoadWAV: {}: fname length={}, {}",
            id, pdat->fname, strlen(pdat->fname), SDL_GetError());
        pdat->status = mus_dat::PMT_FREE;
        pdat->used = false; return false;}

    // set volume of channel and save
    Mix_Volume(pdat->channel, pdat->vol);

    int channel = Mix_PlayChannel(pdat->channel, wave, loops);
    if (channel == -1)
        { // some error occured
        ToLogService("common", LogLevel::Error, "thread#{} play channel error, dump...", id);
        dump_mus_dat(id); Mix_FreeChunk(wave);
        pdat->status = mus_dat::PMT_FREE;
        pdat->used = false;
        return false;}
    else {
        ToLogService("common", "thread#{} pdat->channel={}, play channel={}",
                id, pdat->channel, channel);
        if (pdat->loop) // continue blocking the following play request
            {
            pdat->channel = channel;
            pdat->wave = wave;}
        else {
            // wait it for playing end
            ToLogService("common", "thread#{} wait {} for playing end",
                    id, pdat->fname);

            DWORD ret; // tell whether to exit loop when playing
            while (Mix_Playing(channel) == 1)
                {
                SDL_Delay(1);
                ret = WaitForSingleObject(pdat->sig_exit, 1);
                if (ret == WAIT_OBJECT_0)
                    {
                    ToLogService("common", "thread#{} recv sig_exit when playing", id);
                    break;}
                }

            Mix_FreeChunk(wave); pdat->status = mus_dat::PMT_FREE;
            pdat->used = false;}
        return true;}
    }

bool play_cmn_snd(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    pdat->used = true;
    pdat->status = mus_dat::PMT_PLAYING; // block the following request
    wave_cache_dat* pwcd = &wave_cache_dat_array[pdat->wave_cache_id]; 
    Mix_Chunk* wave = NULL;
    int loops = (pdat->loop) ? -1 : 0;

    wave = Mix_LoadWAV_RW(SDL_RWFromConstMem((void const *)pwcd->buf,
                          pwcd->buf_len), 0);
    if (wave == NULL)
        {
        ToLogService("common", LogLevel::Error, "thread#{} Couldn't load {}: {}", id,
                pdat->fname, SDL_GetError());
        pdat->status = mus_dat::PMT_FREE;
        pdat->used = false;
        return false;}

    // set volume of channel and save
    Mix_Volume(pdat->channel, pdat->vol);

    int channel = Mix_PlayChannel(pdat->channel, wave, loops);
    if (channel == -1)
        {
        // some error occured
        ToLogService("common", LogLevel::Error, "thread#{} play channel error, dump...", id);
        dump_mus_dat(id);
        Mix_FreeChunk(wave);
        pdat->status = mus_dat::PMT_FREE;
        pdat->used = false;
        return false;}
    else {
        if (pdat->loop) // continue blocking the following play request
            {
            pdat->channel = channel;
            pdat->wave = wave;}
        else {
            // wait it for playing end
            ToLogService("common", "thread#{} wait {} for playing end",
                    id, pdat->fname);

            DWORD ret; // tell whether to exit loop when playing
            while (Mix_Playing(channel) == 1)
                {
                SDL_Delay(1);
                ret = WaitForSingleObject(pdat->sig_exit, 1);
                if (ret == WAIT_OBJECT_0)
                    {
                    ToLogService("common", "thread#{} recv sig_exit when playing", id);
                    break;}
                }

            Mix_FreeChunk(wave);
            pdat->status = mus_dat::PMT_FREE;
            pdat->used = false;}

        return true;}
    }

void bkg_snd_finished_callback()
    {
    mus_dat_array[BKG_SND_ID_START].status = mus_dat::PMT_FREE;
    mus_dat_array[BKG_SND_ID_START].used = false;

    if ((mus_hwnd != NULL) && (mus_msg_id != 0))
        PostMessage(mus_hwnd, mus_msg_id, 0, 0);

    ToLogService("common", "background music finished");
    }

bool play_bkg_snd(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    pdat->used = true;
    pdat->status = mus_dat::PMT_PLAYING; // block the following request
    Mix_Music* music = NULL;
    int loops = (pdat->loop) ? -1 : 0;

    // check...
    if (Mix_PlayingMusic() || Mix_PausedMusic()) Mix_HaltMusic();
    if (pdat->music) {Mix_FreeMusic(pdat->music); pdat->music = NULL;}

    // set the music volume
    Mix_VolumeMusic(pdat->vol);

    // load the requested music file
    music = Mix_LoadMUS(pdat->fname);
    if (music == NULL)
        {
        ToLogService("common", LogLevel::Error, "thread#{} Couldn't load {}: {}", id, pdat->fname,
           SDL_GetError());
        pdat->status = mus_dat::PMT_FREE;
        pdat->used = false;
        pdat->errcode = mus_dat::OP_FILE_NOT_FOUND;
        return false;}

    ToLogService("common", "thread#1 playing background music [{}]", pdat->fname);
    Mix_HookMusicFinished(bkg_snd_finished_callback);
    if (Mix_PlayMusic(music, loops) == -1)
        {
        // some error occured
        ToLogService("common", LogLevel::Error, "thread#{} play [{}] error, dump...", id, pdat->fname);
        dump_mus_dat(id); Mix_FreeMusic(music);
        pdat->status = mus_dat::PMT_FREE;
        pdat->used = false;
        pdat->errcode = mus_dat::OP_FAILED;
        return false;}
    else {
        pdat->music = music;
        pdat->errcode = mus_dat::OP_SUCCESS;
        return true;}

    return true;}

bool stop_env_snd(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];

    if (Mix_Playing(pdat->channel) == 1)
        Mix_HaltChannel(pdat->channel);

    if (pdat->wave)
        {
        Mix_FreeChunk(pdat->wave);
        pdat->wave = NULL;}

    pdat->status = mus_dat::PMT_FREE;
    pdat->used = false;
    ToLogService("common", "thread#{} has been stopped by caller [{}]", id, GetTickCount());
    return true;}

bool stop_cmn_snd(int id) {return true;} // you can't stop a common sound

bool stop_bkg_snd(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];

    if (Mix_PlayingMusic() || Mix_PausedMusic())
        Mix_HaltMusic();

    if (pdat->music)
        {
        Mix_FreeMusic(pdat->music);
        pdat->music = NULL;}

    pdat->status = mus_dat::PMT_FREE;
    pdat->used = false;
    pdat->errcode = mus_dat::OP_SUCCESS;
    return true;}

bool vol_env_snd(int id) // called only when playing
    {
    mus_dat* pdat = &mus_dat_array[id];

    Mix_Volume(pdat->channel, pdat->vol);
    return true;}

bool vol_cmn_snd(int id) {return vol_env_snd(id);}

bool vol_bkg_snd(int id)
    {
    if (is_free(id)) return true;

    mus_dat* pdat = &mus_dat_array[id];
    Mix_VolumeMusic(pdat->vol);
    pdat->errcode = mus_dat::OP_SUCCESS;
    return true;}

void dispatch_cmd(int id)
    {
    if (is_free(id))
        {
        // free status, only deal with command "CMD_PLAY" and "CMD_VOL"
        switch (mus_dat_array[id].cmd)
            {
            case mus_dat::CMD_PLAY:
                if (is_env_snd(id)) play_env_snd(id);
                else if (is_cmn_snd(id)) play_cmn_snd(id);
                else if (is_bkg_snd(id)) play_bkg_snd(id);
                else {
                    ToLogService("common", LogLevel::Error, "thread#{} unknown cmd_play, dump...", id);
                    dump_mus_dat(id);}
                break;
#if 0
            case mus_dat::CMD_VOL:
                if (is_env_snd(id)) ;
                else if (is_cmn_snd(id)) ;
                else if (is_bkg_snd(id)) vol_bkg_snd(id);
                else {
                    ToLogService("common", LogLevel::Error, "thread#{} unknown cmd_vol, dump...", id);
                    dump_mus_dat(id);}
                break;
#endif
            default:
                break; // nothing to do
            }
        }
    else
        {
        // playing status, only deal with command "CMD_STOP" and "CMD_VOL"
        switch (mus_dat_array[id].cmd)
            {
            case mus_dat::CMD_STOP:
                if (is_env_snd(id)) stop_env_snd(id);
                else if (is_cmn_snd(id)) stop_cmn_snd(id);
                else if (is_bkg_snd(id)) stop_bkg_snd(id);
                else {
                    ToLogService("common", LogLevel::Error, "thread#{} unknown cmd_stop, dump...", id);
                    dump_mus_dat(id);}
                break;

            case mus_dat::CMD_VOL:
                if (is_env_snd(id)) vol_env_snd(id);
                else if (is_cmn_snd(id)) vol_cmn_snd(id);
                else if (is_bkg_snd(id)) vol_bkg_snd(id);
                else {
                    ToLogService("common", LogLevel::Error, "thread#{} unknown cmd_vol, dump...", id);
                    dump_mus_dat(id);}
                break;

            default:
                break; // nothing to do
            }
        }    
    return;}

void dispatch_status(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];

    if (pdat->status == mus_dat::PMT_FREE) {Sleep(5); return;}
    if (pdat->status == mus_dat::PMT_PLAYING) {SDL_Delay(1); return;}
    return;}

void dispatch_exit(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];

    if (Mix_Playing(pdat->channel) == 1)
        Mix_HaltChannel(pdat->channel);

    if (pdat->wave)
        Mix_FreeChunk(pdat->wave);

    if (Mix_PlayingMusic() || Mix_PausedMusic())
        Mix_HaltMusic();

    if (pdat->music)
        Mix_FreeMusic(pdat->music);

    pdat->used = false;
    ToLogService("common", "thread#{} exit", id);
    }

DWORD WINAPI play_mus_thread(LPVOID lpParameter)
    {
    DWORD ret;
    int id = (int)lpParameter;
    HANDLE sig_cmd = sig_cmd_array[id];
    mus_dat_array[id].sig_exit = sig_exit;

    while (1)
        {
        ret = WaitForSingleObject(sig_exit, 1); // wait 1ms
        if (ret == WAIT_OBJECT_0) break;
        if (ret == WAIT_ABANDONED)
            {
            ToLogService("common", LogLevel::Error, "WAIT_ABANDONED!!!");
            break;}

        ret = WaitForSingleObject(sig_cmd, 2); // wait 2ms
        if (ret == WAIT_OBJECT_0) dispatch_cmd(id);
        else if (ret == WAIT_TIMEOUT) dispatch_status(id);
        else Sleep(20);}

    dispatch_exit(id);
    return 0;}


// music producer interface
inline bool is_used(int id)
    {
    return (mus_dat_array[id].used) ? true : false;}

inline void signal(int id) {SetEvent(sig_cmd_array[id]);}

inline void mus_cmd_add(int id, char const* fname, bool loop)
    {
    mus_dat* pdat = &mus_dat_array[id];
    pdat->used = true;
    pdat->loop = loop;
    size_t len = min(MAX_PATH, strlen(fname));
    memcpy(pdat->fname, fname, len);
    pdat->fname[len] = 0;}

inline void mus_cmd_stop(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    pdat->cmd = mus_dat::CMD_STOP;
    signal(id);}

inline void mus_cmd_vol(int id, int vol)
    {
    mus_dat* pdat = &mus_dat_array[id];
    pdat->cmd = mus_dat::CMD_VOL;
    pdat->vol = vol;
    signal(id);}

inline void mus_cmd_play(int id)
    {
    mus_dat* pdat = &mus_dat_array[id];
    pdat->cmd = mus_dat::CMD_PLAY;
    signal(id);}



    // the current version of wave cache is unsafe with multi-thread caller
    //  it's only designed for a single-thread caller
inline void dump_cache_dat(int id)
    {
    wave_cache_dat* p = &wave_cache_dat_array[id];
    ToLogService("common", LogLevel::Error, "cache#{} [{}], fname={}, buf={}, buf_len={}, time_stamp={}, ref_cnt={}",
            id, (p->used) ? "used" : "not use", p->fname, (void*)p->buf,
            p->buf_len, p->time_stamp, (int)p->ref_cnt);
    }

inline void dump_cache_dat()
    {
    for (int i = WAV_CACHE_ID_START; i <= WAV_CACHE_ID_END; ++ i) dump_cache_dat(i);

    ToLogService("common", LogLevel::Error, "");
    }

inline void wave_cache_set_used(wave_cache_dat* p, bool used = true)
    {p->used = used;}

inline bool wave_cache_is_used(wave_cache_dat* p)
    {return p->used;}

inline void wave_cache_record(wave_cache_dat* p)
    {p->time_stamp = GetTickCount();}

inline void wave_cache_unload(wave_cache_dat* p)
    {
    // free the wave file from heap
    if (p->buf)
        {
        delete p->buf;
        p->buf = NULL;}

    p->buf_len = 0;
    return;}

bool wave_cache_load(wave_cache_dat* p)
    {
    // read the wave file into heap
    FILE* fp = NULL;
    if (fp = fopen(p->fname, "rb"), fp == NULL)
        {
        ToLogService("common", LogLevel::Error, "failed to open file {}", p->fname);
        return false;}

    fseek(fp, 0, SEEK_END); long len = ftell(fp);
    fseek(fp, 0, SEEK_SET); Uint8* buf = NULL;

    try {
        buf = new Uint8[len];
        len = (Uint32)fread(buf, sizeof Uint8, len, fp);}
    catch (bad_alloc& ba)
        {
        ToLogService("common", LogLevel::Error, "'new' except: {} in wave_cache_load", ba.what());
        fclose(fp);
        return false;}
    catch (...)
        {
        ToLogService("common", LogLevel::Error, "other except... in wave_cache_load");
        if (buf) delete buf;
        fclose(fp);
        return false;}
    
    p->buf = buf;
    p->buf_len = len;
    fclose(fp);
    return true;}

int wave_cache_find(char const* fname)
// if find, return the index found
// if not, and if cache is full, return WAV_CACHE_ID_FULL
//         and if cache is not full, return the a free index
    {
    wave_cache_dat* p;
    int id = WAV_CACHE_ID_START;
    DWORD time_stamp = wave_cache_dat_array[id].time_stamp;
    bool found = false;
    int found_id;

    int i = WAV_CACHE_ID_START;
	for (; i <= WAV_CACHE_ID_END; ++i)
        {
        p = &wave_cache_dat_array[i];
        if (!p->used) break;

        if (p->time_stamp < time_stamp)
            {
            time_stamp = p->time_stamp;
            id = i;}

        if (!found && (strcmp(p->fname, fname) == 0))
            {
            found = true;
            found_id = i;}
        }

    wav_cache_set_discard(id); // refresh discard id

    if (found) return found_id;
    else return (i > WAV_CACHE_ID_END) ? WAV_CACHE_IS_FULL : i;}

Uint8 const* wave_cache_new(wave_cache_dat* p, char const* fname)
    {
    size_t len = min(MAX_PATH, strlen(fname));
    memcpy(p->fname, fname, len);
    p->fname[len] = 0;

    if (wave_cache_load(p))
        {
        wave_cache_record(p);
        wave_cache_set_used(p);
        return p->buf;}
    else return NULL;}

Uint8 const* wave_cache_replace(wave_cache_dat* p, char const* fname)
    {
    wave_cache_unload(p);
    return wave_cache_new(p, fname);}

int wave_cache_add(char const* fname)
    { // add the file to cache, or replace the last-called (discard)
    int id;
    wave_cache_dat* p = NULL;
    if (id = wave_cache_find(fname), id == WAV_CACHE_IS_FULL)
        {
        // replace a wave cache
        id = wav_cache_get_discard();
        ToLogService("common", "replace cache#{} with file {}", id, fname);
        p = &wave_cache_dat_array[id];
        wave_cache_replace(p, fname);
        return id;}
    else {
        // find it or need to new one
        p = &wave_cache_dat_array[id];
        if (wave_cache_is_used(p))
            {
            wave_cache_record(p);
            return id;}
        else {
            wave_cache_new(p, fname);
            return id;}
        }
    }

void wave_cache_exit()
    { // release all memory allocated
    wave_cache_dat* p;
    for (int i = WAV_CACHE_ID_START; i <= WAV_CACHE_ID_END; ++ i)
        {
        p = &wave_cache_dat_array[i];
        if (!p->used) break;

        // release buffer        
        p->buf_len = 0;
        p->time_stamp = 0;

        if (p->buf) delete p->buf;

        p->buf = NULL;
        p->fname[0] = 0;
        p->used = false;}
    }


// music manager singleton

bool mus_mgr_init(bool enable, DWORD_PTR dwValue)
    {
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
        ToLogService("common", LogLevel::Error, "Couldn't initialize SDL: {}", SDL_GetError());
        return false;}

    if (!enable) return false;

    // init music
    int i;
    ToLogService("common", "mus_mgr_init");

    // init the share data
    for (i = ENV_SND_ID_START; i <= ENV_SND_ID_END; ++ i)
        env_snd_init(i, false, mus_dat::CMD_NONE, 0, i);
    for (i = CMN_SND_ID_START; i <= CMN_SND_ID_END; ++ i)
        cmn_snd_init(i, false, mus_dat::CMD_NONE, 0, i);
    for (i = BKG_SND_ID_START; i <= BKG_SND_ID_END; ++ i)
        bkg_snd_init(i, false, mus_dat::CMD_NONE);

    // open audio device
    int audio_rate = MIX_DEFAULT_FREQUENCY;
    Uint16 audio_format = MIX_DEFAULT_FORMAT;
    int audio_channels = MIX_DEFAULT_CHANNELS;
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, 4096) < 0)
        {
        ToLogService("common", LogLevel::Error, "Couldn't open audio: {}", SDL_GetError());
        return false;}

    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);

    // create exit signal
    sig_exit = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (sig_exit == NULL)
        {
        ToLogService("common", LogLevel::Error, "create exit signal error");
        return false;}
    if ((sig_exit != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
        {
        ToLogService("common", LogLevel::Error, "exit signal already exist");
        return false;}

    sig_task = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (sig_task == NULL)
        {
        ToLogService("common", LogLevel::Error, "create task signal error");
        return false;}
    if ((sig_task != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS))
        {
        ToLogService("common", LogLevel::Error, "task signal already exist");
        return false;}

    // create child thread and command signals
    DWORD tid = 0;
	for (i = MUS_ID_START; i <= MUS_ID_END; ++ i)
	{
		sig_cmd_array[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		thrdpool[i] = CreateThread(NULL, 0, play_mus_thread, (LPVOID)i, 0, &tid);
		if( dwValue && INVALID_HANDLE_VALUE != thrdpool[i] )
		{
			SetThreadAffinityMask( thrdpool[i], dwValue );
		}
	}

    mus_init_flag = true;

    return true;}

void mus_mgr_reset(int op) // -1 clear all, 0 clear all except bkg_snd
    {
    if (!mus_init_flag) return;

    // reset music
    int i;
    ToLogService("common", "mus_mgr_reset");

    for (i = ENV_SND_ID_START; i <= ENV_SND_ID_END; ++ i)
        {
        if (is_used(i)) mus_cmd_stop(i);}
    for (i = CMN_SND_ID_START; i <= CMN_SND_ID_END; ++ i)
        {
        if (is_used(i)) mus_cmd_stop(i);}

    if (op == -1) bkg_snd_stop();}

void mus_mgr_exit()
    {
    if (mus_init_flag)
        {
        ToLogService("common", LogLevel::Error, "before post exit signal: [{}]", GetTickCount());
        dump_mus_dat();
        dump_cache_dat();
        SetEvent(sig_exit);
        
        Sleep(3000); // wait 3 second

        DWORD exitcode;
        for (int i = MUS_ID_START; i < MUS_ID_END; ++ i)
            {
            if (!GetExitCodeThread(thrdpool[i], &exitcode))
                {
                if (exitcode == STILL_ACTIVE)
                    ToLogService("common", LogLevel::Error, "thread#{} failed to exit, kill it!", i);

                if (TerminateThread(thrdpool[i], 0))
                    ToLogService("common", LogLevel::Error, "thread#{} was killed successfully!", i);
                else {
                    ToLogService("common", LogLevel::Error, "thread#{} can't be killed, err = {}!", i, GetLastError());}
                }
            else {
                ToLogService("common", LogLevel::Error, "thread#{} exit successfully!", i);
                continue;}
            }

        wave_cache_exit();
        ToLogService("common", LogLevel::Error, "after post exit signal:");
        dump_mus_dat();
        dump_cache_dat();

        for (int i = MUS_ID_START; i <= MUS_ID_END; ++ i)
            CloseHandle(sig_cmd_array[i]);

        CloseHandle(sig_task);
        CloseHandle(sig_exit);
        Mix_CloseAudio();}
    
    SDL_Quit();
    ToLogService("common", "mus_mgr_exit");
    }

#if 0
struct mus_mgr
    {
    friend class cfl_singleton_ap<mus_mgr>;
    friend class auto_ptr;

private:
    mus_mgr() {::mus_mgr_init();}
    ~mus_mgr() {::mus_mgr_exit();}
    mus_mgr(mus_mgr const&) {}
    mus_mgr& operator =(mus_mgr const&) {}

public:
    bool init() {return true;}
    }; typedef cfl_singleton_ap<mus_mgr> MUS_MGR;

// no use, just help music manager initialize and destroy :p
bool no_use = MUS_MGR::instance()->init();
#endif


// interface exported
bool env_snd_is_full()
    {
    if (!is_env_snd_enable()) return true;

    int i = ENV_SND_ID_START;
	for (; i <= ENV_SND_ID_END; ++i)
	{
		if (!is_used(i)) break;
	}

    return (i > ENV_SND_ID_END) ? false : true;}

int env_snd_add(char const* fname)
    {
    if (!is_env_snd_enable()) return -1;

	int i = ENV_SND_ID_START;
	for (; i <= ENV_SND_ID_END; ++i)
	{
		if (!is_used(i))
		{
			mus_cmd_add(i, fname, true);
			break;
		}
	}

    return (i > ENV_SND_ID_END) ? -1 : i;}

bool env_snd_vol(int id, int vol)
    {
    if (vol < 0 || !is_env_snd_enable()) return false;

    if (id < ENV_SND_ID_START || id > ENV_SND_ID_END)
        throw logic_error("out of index in env_snd_vol\n");

    if (vol == 0) mus_cmd_stop(id); // stop it
    else mus_cmd_vol(id, vol);
    return true;}

bool env_snd_play(int id)
    {
    if (!is_env_snd_enable()) return false;

    if (id < ENV_SND_ID_START || id > ENV_SND_ID_END)
        throw logic_error("out of index in env_snd_play\n");

    mus_cmd_play(id); return true;}

void cmn_snd_set_cache(int n /* = 20 */) {}

bool cmn_snd_play(char const* fname, int vol)
    {
    if (!is_cmn_snd_enable()) return false;

    int i = CMN_SND_ID_START;
    for (; i < CMN_SND_ID_END; ++ i)
        {
        if (!is_used(i)) break;}
    if (i > CMN_SND_ID_END) return false;

    // add the wave file to wave cache and music data array
    if (vol <= 0) return false;

    int id = wave_cache_add(fname);
    mus_cmd_add(i, fname, false);
    mus_dat_array[i].wave_cache_id = id;
    mus_dat_array[i].vol = vol;

    // play it
    mus_cmd_play(i);
    return true;}

void snd_enable(bool enable /* = true */)
    {
    cmn_snd_enable(enable);
    env_snd_enable(enable);}

void bkg_snd_set_notify(HWND hwnd, UINT msg_id)
    {
    mus_hwnd = hwnd; mus_msg_id = msg_id;}

bool bkg_snd_play(char const* fname, bool loop /* = false */)
    {
    if ((_access(fname, 00) == -1) || (_access(fname, 04) == -1))
        {
        ToLogService("common", LogLevel::Error, "{} not found or can't be read", fname);
        return false;}
    if (is_used(BKG_SND_ID_START)) return false;

    mus_cmd_add(BKG_SND_ID_START, fname, loop);
    mus_cmd_play(BKG_SND_ID_START);

    mus_dat* pdat = &mus_dat_array[BKG_SND_ID_START];
	int i = 0;
    while (true)
        {
        if ((pdat->status == mus_dat::PMT_PLAYING)
            && (pdat->used == true)) break;
        else if ((pdat->errcode == mus_dat::OP_FAILED)
                || (pdat->errcode == mus_dat::OP_FILE_NOT_FOUND)) break;
        else Sleep(1);

		if (++i > ERROR_TIME)
			{
			ToLogService("common", LogLevel::Error, "bkg_snd_play time out!");
			break;
			}
		}

    return true;}

void bkg_snd_vol(int vol)
    {
    if (vol < 0) return;

    mus_cmd_vol(BKG_SND_ID_START, vol);}

void bkg_snd_stop()
    {
    if (!is_used(BKG_SND_ID_START)) return;

    mus_cmd_stop(BKG_SND_ID_START);

    mus_dat* pdat = &mus_dat_array[BKG_SND_ID_START];

	int i = 0;
    while (true)
        {
        if ((pdat->status == mus_dat::PMT_FREE)
            && (pdat->used == false)) break;
        else Sleep(1);
		
		if (++i > ERROR_TIME)
			{
			ToLogService("common", LogLevel::Error, "bkg_snd_stop time out!");
			break;
			}
		}

    ToLogService("common", "stop bkg_snd success");
    }
