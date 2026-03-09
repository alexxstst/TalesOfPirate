#include "stdafx.h"
#include "BassMusicInterface.h"
#include "BassLoader.h"

#define BASS2_MUSIC_RAMP			0x200	// normal ramping

#define BASS_CONFIG_BUFFER			0

BassMusicInfo::BassMusicInfo() {
	mVolume = 0.0;
	mVolumeAdd = 0.0;
	mVolumeCap = 1.0;
	mStopOnFade = false;
	mHMusic = NULL;
	mHStream = NULL;
}

BassMusicInterface::BassMusicInterface(HWND theHWnd) {
	LoadBassDLL();

	MIXERCONTROLDETAILS mcd;
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL mlct;
	MIXERLINE mixerLine;
	HMIXEROBJ phmx;
	MIXERCAPS pmxcaps;

	mixerOpen((HMIXER*)&phmx, 0, 0, 0, MIXER_OBJECTF_MIXER);
	mixerGetDevCaps(0, &pmxcaps, sizeof(pmxcaps));

	mxlc.cbStruct = sizeof(mxlc);
	mxlc.cbmxctrl = sizeof(mlct);
	mxlc.pamxctrl = &mlct;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mixerLine.cbStruct = sizeof(mixerLine);
	mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	mixerGetLineInfo(phmx, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
	mxlc.dwLineID = mixerLine.dwLineID;
	mixerGetLineControls(phmx, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

	mcd.cbStruct = sizeof(mcd);
	mcd.dwControlID = mlct.dwControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;
	mcd.cbDetails = sizeof(mxcd_u);
	mcd.paDetails = &mxcd_u;

	mixerGetControlDetails(phmx, &mcd, 0L);

	//return mxcd_u.dwValue;

	BOOL success;

	if (gBass->mVersion2) {
		success = gBass->BASS_Init2(1, 44100, 0, theHWnd, NULL);
		gBass->BASS_SetConfig(BASS_CONFIG_BUFFER, 2000);
	}
	else
		success = gBass->BASS_Init(-1, 44100, 0, theHWnd);

	mixerSetControlDetails(phmx, &mcd, 0L);

	gBass->BASS_Start();

	mixerClose((HMIXER)phmx);

	mMaxMusicVolume = 40;
}

BassMusicInterface::~BassMusicInterface() {
	gBass->BASS_Stop();
	gBass->BASS_Free();

	FreeBassDLL();
}

bool BassMusicInterface::LoadMusic(int theSongId, const std::string& theFileName) {
	HMUSIC aHMusic = NULL;
	HSTREAM aStream = NULL;

	std::string anExt;
	int aDotPos = theFileName.find_last_of('.');
	if (aDotPos != std::string::npos)
		anExt = StringToLower(theFileName.substr(aDotPos + 1));

	if (anExt == "wav" || anExt == "ogg" || anExt == "mp3")
		aStream = gBass->BASS_StreamCreateFile(FALSE, (void*)theFileName.c_str(), 0, 0, 0);
	else {
		if (gBass->mVersion2)
			aHMusic = gBass->BASS_MusicLoad2(FALSE, (void*)theFileName.c_str(), 0, 0,
											 BASS_MUSIC_LOOP | BASS2_MUSIC_RAMP, 0);
		else
			aHMusic = gBass->BASS_MusicLoad(FALSE, (void*)theFileName.c_str(), 0, 0, BASS_MUSIC_LOOP);
	}

	if (aHMusic == NULL && aStream == NULL)
		return false;

	BassMusicInfo aMusicInfo;
	aMusicInfo.mHMusic = aHMusic;
	aMusicInfo.mHStream = aStream;
	mMusicMap.emplace(theSongId, aMusicInfo);

	return true;
}

void BassMusicInterface::PlayMusic(int theSongId, int theOffset, bool noLoop) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
		aMusicInfo->mVolumeAdd = 0.0;
		aMusicInfo->mStopOnFade = noLoop;
		gBass->BASS_ChannelSetAttributes(aMusicInfo->GetHandle(), -1, (int)(aMusicInfo->mVolume * 100), -101);

		gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
		if (aMusicInfo->mHMusic) {
			if (gBass->mVersion2)
				gBass->BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset,
										BASS2_MUSIC_RAMP | (noLoop ? 0 : BASS_MUSIC_LOOP), TRUE);
			else
				gBass->BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset, noLoop ? 0 : BASS_MUSIC_LOOP, TRUE);
		}
		else {
			BOOL flush = theOffset == -1 ? FALSE : TRUE;
			gBass->BASS_StreamPlay(aMusicInfo->mHStream, flush, noLoop ? 0 : BASS_MUSIC_LOOP);
			if (theOffset > 0)
				gBass->BASS_ChannelSetPosition(aMusicInfo->mHStream, theOffset);
		}
	}
}

void BassMusicInterface::StopMusic(int theSongId) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::StopAllMusic() {
	auto anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
		++anItr;
	}
}

void BassMusicInterface::UnloadMusic(int theSongId) {
	StopMusic(theSongId);

	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		if (aMusicInfo->mHStream)
			gBass->BASS_StreamFree(aMusicInfo->mHStream);
		else if (aMusicInfo->mHMusic)
			gBass->BASS_MusicFree(aMusicInfo->mHMusic);

		mMusicMap.erase(anItr);
	}
}

void BassMusicInterface::UnloadAllMusic() {
	StopAllMusic();
	for (auto anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		if (aMusicInfo->mHStream)
			gBass->BASS_StreamFree(aMusicInfo->mHStream);
		else if (aMusicInfo->mHMusic)
			gBass->BASS_MusicFree(aMusicInfo->mHMusic);
	}
	mMusicMap.clear();
}

void BassMusicInterface::PauseMusic(int theSongId) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		gBass->BASS_ChannelPause(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::PauseAllMusic() {
	for (auto anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		gBass->BASS_ChannelPause(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::ResumeAllMusic() {
	for (auto anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		gBass->BASS_ChannelResume(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::ResumeMusic(int theSongId) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		gBass->BASS_ChannelResume(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::FadeIn(int theSongId, int theOffset, double theSpeed, bool noLoop) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolumeAdd = theSpeed;
		aMusicInfo->mStopOnFade = noLoop;

		gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
		gBass->BASS_ChannelSetAttributes(aMusicInfo->GetHandle(), -1, (int)(aMusicInfo->mVolume * 100), -101);
		if (aMusicInfo->mHMusic) {
			if (theOffset == -1)
				gBass->BASS_MusicPlay(aMusicInfo->mHMusic);
			else {
				if (gBass->mVersion2)
					gBass->BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset,
											BASS2_MUSIC_RAMP | (noLoop ? 0 : BASS_MUSIC_LOOP), TRUE);
				else
					gBass->BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset, noLoop ? 0 : BASS_MUSIC_LOOP, TRUE);
			}
		}
		else {
			BOOL flush = theOffset == -1 ? FALSE : TRUE;
			gBass->BASS_StreamPlay(aMusicInfo->mHStream, flush, noLoop ? 0 : BASS_MUSIC_LOOP);
			if (theOffset > 0)
				gBass->BASS_ChannelSetPosition(aMusicInfo->mHStream, theOffset);
		}
	}
}

void BassMusicInterface::FadeOut(int theSongId, bool stopSong, double theSpeed) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;

		if (aMusicInfo->mVolume != 0.0) {
			aMusicInfo->mVolumeAdd = -theSpeed;
		}

		aMusicInfo->mStopOnFade = stopSong;
	}
}

void BassMusicInterface::FadeOutAll(bool stopSong, double theSpeed) {
	auto anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolumeAdd = -theSpeed;
		aMusicInfo->mStopOnFade = stopSong;

		++anItr;
	}
}

void BassMusicInterface::SetVolume(double theVolume) {
	int aVolume = (int)(theVolume * mMaxMusicVolume);

	if (gBass->mVersion2) {
		gBass->BASS_SetConfig(/*BASS_CONFIG_GVOL_MUSIC*/6, (int)(theVolume * 100));
		gBass->BASS_SetConfig(/*BASS_CONFIG_GVOL_STREAM*/5, (int)(theVolume * 100));
	}
	else
		gBass->BASS_SetGlobalVolumes(aVolume, aVolume, aVolume);
}

void BassMusicInterface::SetSongVolume(int theSongId, double theVolume) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolume = theVolume;
		gBass->BASS_ChannelSetAttributes(aMusicInfo->GetHandle(), -1, (int)(aMusicInfo->mVolume * 100), -101);
	}
}

void BassMusicInterface::SetSongMaxVolume(int theSongId, double theMaxVolume) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolumeCap = theMaxVolume;
		aMusicInfo->mVolume = min(aMusicInfo->mVolume, theMaxVolume);
		gBass->BASS_ChannelSetAttributes(aMusicInfo->GetHandle(), -1, (int)(aMusicInfo->mVolume * 100), -101);
	}
}

bool BassMusicInterface::IsPlaying(int theSongId) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		return gBass->BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PLAYING;
	}

	return false;
}

void BassMusicInterface::SetMusicAmplify(int theSongId, double theAmp) {
	auto anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;
		gBass->BASS_MusicSetAmplify(aMusicInfo->GetHandle(), (int)(theAmp * 100));
	}
}

void BassMusicInterface::Update() {
	auto anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end()) {
		BassMusicInfo* aMusicInfo = &anItr->second;

		if (aMusicInfo->mVolumeAdd != 0.0) {
			aMusicInfo->mVolume += aMusicInfo->mVolumeAdd;

			if (aMusicInfo->mVolume > aMusicInfo->mVolumeCap) {
				aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
				aMusicInfo->mVolumeAdd = 0.0;
			}
			else if (aMusicInfo->mVolume < 0.0) {
				aMusicInfo->mVolume = 0.0;
				aMusicInfo->mVolumeAdd = 0.0;

				if (aMusicInfo->mStopOnFade)
					gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
			}

			gBass->BASS_ChannelSetAttributes(aMusicInfo->GetHandle(), -1, (int)(aMusicInfo->mVolume * 100), -101);
		}

		++anItr;
	}
}
