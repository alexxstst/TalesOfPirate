#pragma once

#ifndef __BASSLOADER_H__
#define __BASSLOADER_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include "bass.h"

struct BASS_INSTANCE
{
	BASS_INSTANCE(const char *dllName);
	virtual ~BASS_INSTANCE();

	HMODULE         mModule;
	bool			mVersion2;

	BOOL(WINAPI *BASS_Init)(int device, DWORD freq, DWORD flags, HWND win);
	BOOL(WINAPI *BASS_Init2)(int device, DWORD freq, DWORD flags, HWND win, GUID* clsid);
	void(WINAPI *BASS_Free)();
	BOOL(WINAPI *BASS_Stop)();
	BOOL(WINAPI *BASS_Start)();
	void(WINAPI *BASS_SetGlobalVolumes)(int musvol, int samvol, int strvol);
	BOOL(WINAPI *BASS_SetVolume)(DWORD volume);
	BOOL(WINAPI *BASS_MusicSetAmplify)(HMUSIC handle, DWORD amp);

	DWORD(WINAPI *BASS_SetConfig)(DWORD option, DWORD value); 

	BOOL(WINAPI *BASS_ChannelStop)(DWORD handle);
	BOOL(WINAPI *BASS_ChannelPause)(DWORD handle);
	BOOL(WINAPI *BASS_ChannelResume)(DWORD handle);
	BOOL(WINAPI *BASS_ChannelSetAttributes)(DWORD handle, int freq, int volume, int pan);
	BOOL(WINAPI *BASS_ChannelSetPosition)(DWORD handle, QWORD pos);
	DWORD(WINAPI *BASS_ChannelIsActive)(DWORD handle);

	HMUSIC(WINAPI *BASS_MusicLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
	HMUSIC(WINAPI *BASS_MusicLoad2)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags, DWORD freq);
	BOOL(WINAPI *BASS_MusicPlay)(HMUSIC handle);
	BOOL(WINAPI *BASS_MusicPlayEx)(HMUSIC handle, DWORD pos, int flags, BOOL reset);
	void(WINAPI *BASS_MusicFree)(HMUSIC handle);

	HSTREAM(WINAPI *BASS_StreamCreateFile)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags); 
	void(WINAPI *BASS_StreamFree)(HSTREAM handle);
	BOOL(WINAPI *BASS_StreamPlay)(HSTREAM handle, BOOL flush, DWORD flags);
};

extern BASS_INSTANCE *gBass;

BASS_INSTANCE* BASS_CreateInstance(char *dllName);
void BASS_FreeInstance(BASS_INSTANCE *instance);
void LoadBassDLL(); // exits on failure
void FreeBassDLL();

#endif
