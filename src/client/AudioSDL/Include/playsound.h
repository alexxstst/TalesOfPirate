#pragma once


#ifndef _PLAYSOUND_H_
#define _PLAYSOUND_H_

bool mus_mgr_init(bool enable, DWORD_PTR dwValue = 0);
void mus_mgr_reset(int op = 0);
void mus_mgr_exit();


bool env_snd_is_full();
int env_snd_add(char const* fname);
bool env_snd_vol(int id, int vol);
bool env_snd_play(int id);

inline bool env_snd_del(int id) {
	return env_snd_vol(id, 0);
}

void cmn_snd_set_cache(int n = 20);
bool cmn_snd_play(char const* fname, int vol = 128);

void snd_enable(bool enable = true);

void bkg_snd_set_notify(HWND hwnd, UINT msg_id);
bool bkg_snd_play(char const* fname, bool loop = false);
void bkg_snd_vol(int vol);
void bkg_snd_stop();


inline void SetBkMusicNotify(HWND hWnd, UINT uMsgId) {
	bkg_snd_set_notify(hWnd, uMsgId);
}

inline int PlayBkMusic(char const* filename, bool loop = false) {
	return int(bkg_snd_play(filename, loop));
}

inline int StopBkMusic() {
	bkg_snd_stop();
	return 0;
}

inline void SetWaveQueueLen(int n = 20) {
	cmn_snd_set_cache(n);
}

inline int PlayMusic(char const* filename, int reserved = 0) {
	return int(cmn_snd_play(filename));
}

#endif // _PLAYSOUND_H_
