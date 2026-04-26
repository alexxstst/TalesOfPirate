#include "stdafx.h"
#include "DSoundManager.h"
#include <io.h>
#include <fcntl.h>
#include "DSoundInstance.h"

using namespace std;

// #define USE_OGG_LIB


#ifdef USE_OGG_LIB
#include "ogg/ivorbiscodec.h"
#include "ogg/ivorbisfile.h"
#endif

#define SOUND_FLAGS (DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME |  DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLFREQUENCY)

DSoundManager::DSoundManager(HWND theHWnd) {
	mDSoundDLL = LoadLibrary("dsound.dll");
	mLastReleaseTick = 0;
	mPrimaryBuffer = NULL;

	int i;

	for (i = 0; i < MAX_SOURCE_SOUNDS; i++) {
		mSourceSounds[i] = NULL;
		mBaseVolumes[i] = 1;
		mBasePans[i] = 0;
	}

	for (i = 0; i < MAX_CHANNELS; i++)
		mPlayingSounds[i] = NULL;

	mDirectSound = NULL;

	mMasterVolume = 1.0;

	if (theHWnd != NULL) {
		typedef HRESULT (WINAPI *DirectSoundCreateFunc)(LPCGUID lpcGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter);
		DirectSoundCreateFunc aDirectSoundCreateFunc = (DirectSoundCreateFunc)GetProcAddress(
			mDSoundDLL, "DirectSoundCreate");

		if (aDirectSoundCreateFunc != NULL && aDirectSoundCreateFunc(NULL, &mDirectSound, NULL) == DS_OK) {
			HRESULT aResult = mDirectSound->SetCooperativeLevel(theHWnd,DSSCL_PRIORITY);
			if (SUCCEEDED(aResult)) {
				// Set primary buffer to 16-bit 44.1Khz
				WAVEFORMATEX aWaveFormat;
				DSBUFFERDESC aBufferDesc;

				// Set up wave format structure.
				int aBitCount = 16;
				int aChannelCount = 2;
				int aSampleRate = 44100;

				// Set up wave format structure.
				memset(&aWaveFormat, 0, sizeof(WAVEFORMATEX));
				aWaveFormat.cbSize = sizeof(WAVEFORMATEX);
				aWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
				aWaveFormat.nChannels = aChannelCount;
				aWaveFormat.nSamplesPerSec = aSampleRate;
				aWaveFormat.nBlockAlign = aChannelCount * aBitCount / 8;
				aWaveFormat.nAvgBytesPerSec =
					aWaveFormat.nSamplesPerSec * aWaveFormat.nBlockAlign;
				aWaveFormat.wBitsPerSample = aBitCount;

				// Set up DSBUFFERDESC structure.
				memset(&aBufferDesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
				aBufferDesc.dwSize = sizeof(DSBUFFERDESC1);
				aBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
				//| DSBCAPS_CTRL3D; // Need default controls (pan, volume, frequency).
				aBufferDesc.dwBufferBytes = 0;
				aBufferDesc.lpwfxFormat = NULL; //(LPWAVEFORMATEX)&aWaveFormat;

				HRESULT aResult = mDirectSound->CreateSoundBuffer(&aBufferDesc, &mPrimaryBuffer, NULL);
				if (aResult == DS_OK) {
					aResult = mPrimaryBuffer->SetFormat(&aWaveFormat);
				}
			}
			else {
				aResult = mDirectSound->SetCooperativeLevel(theHWnd,DSSCL_NORMAL);
			}
		}
	}
}

DSoundManager::~DSoundManager() {
	ReleaseChannels();
	ReleaseSounds();

	if (mPrimaryBuffer)
		mPrimaryBuffer->Release();

	if (mDirectSound != NULL)
		mDirectSound->Release();

	FreeLibrary(mDSoundDLL);
}

int DSoundManager::FindFreeChannel() {
	DWORD aTick = GetTickCount();
	if (aTick - mLastReleaseTick > 1000) {
		ReleaseFreeChannels();
		mLastReleaseTick = aTick;
	}

	for (int i = 0; i < MAX_CHANNELS; i++) {
		if (mPlayingSounds[i] == NULL)
			return i;

		if (mPlayingSounds[i]->IsReleased()) {
			delete mPlayingSounds[i];
			mPlayingSounds[i] = NULL;
			return i;
		}
	}

	return -1;
}

bool DSoundManager::Initialized() {
	/*
	if (mDirectSound!=NULL)
	{
	mDirectSound->SetCooperativeLevel(theHWnd,DSSCL_NORMAL);
	}
	*/

	return (mDirectSound != NULL);
}

int DSoundManager::VolumeToDB(double theVolume) {
	int aVol = (int)((log10(1 + theVolume * 9) - 1.0) * 2333);
	if (aVol < -2000)
		aVol = -10000;

	return aVol;
}

void DSoundManager::SetVolume(double theVolume) {
	mMasterVolume = theVolume;

	for (int i = 0; i < MAX_CHANNELS; i++)
		if (mPlayingSounds[i] != NULL)
			mPlayingSounds[i]->RehupVolume();
}

bool DSoundManager::LoadWAVSound(unsigned int theSfxID, const string& theFilename) {
	int aDataSize;

	FILE* fp;

	fp = fopen(theFilename.c_str(), "rb");

	if (fp == nullptr)
		return false;

	char aChunkType[5];
	aChunkType[4] = '\0';
	ulong aChunkSize;

	fread(aChunkType, 1, 4, fp);
	if (!strcmp(aChunkType, "RIFF") == 0)
		return false;
	fread(&aChunkSize, 4, 1, fp);

	fread(aChunkType, 1, 4, fp);
	if (!strcmp(aChunkType, "WAVE") == 0)
		return false;

	ushort aBitCount = 16;
	ushort aChannelCount = 1;
	ulong aSampleRate = 22050;

	while (!feof(fp)) {
		fread(aChunkType, 1, 4, fp);
		if (fread(&aChunkSize, 4, 1, fp) == 0)
			return false;

		int aCurPos = ftell(fp);

		if (strcmp(aChunkType, "fmt ") == 0) {
			ushort aFormatTag;
			ulong aBytesPerSec;
			ushort aBlockAlign;

			fread(&aFormatTag, 2, 1, fp);
			fread(&aChannelCount, 2, 1, fp);
			fread(&aSampleRate, 4, 1, fp);
			fread(&aBytesPerSec, 4, 1, fp);
			fread(&aBlockAlign, 2, 1, fp);
			fread(&aBitCount, 2, 1, fp);

			if (aFormatTag != 1)
				return false;
		}
		else if (strcmp(aChunkType, "dep ") == 0) {
			char aStr[256];
			ushort aStrLen;

			fread(&aStrLen, 2, 1, fp);
			if (aStrLen > 255)
				aStrLen = 255;
			fread(aStr, 1, aStrLen, fp);
			aStr[aStrLen] = '\0';

			FILETIME aSavedFileTime;
			fread(&aSavedFileTime, sizeof(FILETIME), 1, fp);

			FILETIME anActualFileTime;
			memset(&anActualFileTime, 0, sizeof(FILETIME));
			GetTheFileTime(aStr, &anActualFileTime);

			if ((aSavedFileTime.dwHighDateTime != anActualFileTime.dwHighDateTime) ||
				(aSavedFileTime.dwLowDateTime != anActualFileTime.dwLowDateTime))
				return false;
		}
		else if (strcmp(aChunkType, "data") == 0) {
			aDataSize = aChunkSize;

			mSourceDataSizes[theSfxID] = aChunkSize;

			PCMWAVEFORMAT aWaveFormat;
			DSBUFFERDESC aBufferDesc;

			// Set up wave format structure.
			memset(&aWaveFormat, 0, sizeof(PCMWAVEFORMAT));
			aWaveFormat.wf.wFormatTag = WAVE_FORMAT_PCM;
			aWaveFormat.wf.nChannels = aChannelCount;
			aWaveFormat.wf.nSamplesPerSec = aSampleRate;
			aWaveFormat.wf.nBlockAlign = aChannelCount * aBitCount / 8;
			aWaveFormat.wf.nAvgBytesPerSec =
				aWaveFormat.wf.nSamplesPerSec * aWaveFormat.wf.nBlockAlign;
			aWaveFormat.wBitsPerSample = aBitCount;
			// Set up DSBUFFERDESC structure.
			memset(&aBufferDesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
			aBufferDesc.dwSize = sizeof(DSBUFFERDESC);
			//aBufferDesc.dwFlags = DSBCAPS_CTRL3D; 
			aBufferDesc.dwFlags = SOUND_FLAGS; //DSBCAPS_CTRLDEFAULT;

			//aBufferDesc.dwFlags = 0;

			aBufferDesc.dwBufferBytes = aDataSize;
			aBufferDesc.lpwfxFormat = (LPWAVEFORMATEX)&aWaveFormat;

			if (mDirectSound->CreateSoundBuffer(&aBufferDesc, &mSourceSounds[theSfxID], NULL) != DS_OK) {
				fclose(fp);
				return false;
			}


			void* lpvPtr;
			DWORD dwBytes;
			if (mSourceSounds[theSfxID]->Lock(0, aDataSize, &lpvPtr, &dwBytes, NULL, NULL, 0) != DS_OK) {
				fclose(fp);
				return false;
			}

			int aReadSize = fread(lpvPtr, 1, aDataSize, fp);
			fclose(fp);

			if (mSourceSounds[theSfxID]->Unlock(lpvPtr, dwBytes, NULL, NULL) != DS_OK)
				return false;

			if (aReadSize != aDataSize)
				return false;

			return true;
		}

		fseek(fp, aCurPos + aChunkSize, SEEK_SET);
	}

	return false;
}

#ifdef USE_OGG_LIB
bool DSoundManager::LoadOGGSound(unsigned int theSfxID, const string& theFilename) {
	OggVorbis_File vf;
	int current_section;

	FILE* aFile = fopen(theFilename.c_str(), "rb");
	if (aFile == NULL)
		return false;


	if (ov_open(aFile, &vf, NULL, 0) < 0) {
		fclose(aFile);
		return false;
	}

	vorbis_info* anInfo = ov_info(&vf, -1);

	PCMWAVEFORMAT aWaveFormat;
	DSBUFFERDESC aBufferDesc;

	// Set up wave format structure.
	memset(&aWaveFormat, 0, sizeof(PCMWAVEFORMAT));
	aWaveFormat.wf.wFormatTag = WAVE_FORMAT_PCM;
	aWaveFormat.wf.nChannels = anInfo->channels;
	aWaveFormat.wf.nSamplesPerSec = anInfo->rate;
	aWaveFormat.wBitsPerSample = 16;
	aWaveFormat.wf.nBlockAlign = aWaveFormat.wf.nChannels * aWaveFormat.wBitsPerSample / 8;
	aWaveFormat.wf.nAvgBytesPerSec = aWaveFormat.wf.nSamplesPerSec * aWaveFormat.wf.nBlockAlign;

	int aLenBytes = (int)(ov_pcm_total(&vf, -1) * aWaveFormat.wf.nBlockAlign);
	memset(&aBufferDesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.

	mSourceDataSizes[theSfxID] = aLenBytes;

	//FUNK
	aBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	aBufferDesc.dwFlags = SOUND_FLAGS;
	aBufferDesc.dwBufferBytes = aLenBytes;
	aBufferDesc.lpwfxFormat = (LPWAVEFORMATEX)&aWaveFormat;

	if (mDirectSound->CreateSoundBuffer(&aBufferDesc, &mSourceSounds[theSfxID], NULL) != DS_OK) {
		ov_clear(&vf);
		return false;
	}

	char* aBuf;
	DWORD dwBytes;
	if (mSourceSounds[theSfxID]->Lock(0, aLenBytes, (LPVOID*)&aBuf, &dwBytes, NULL, NULL, 0) != DS_OK) {
		ov_clear(&vf);
		return false;
	}

	char* aPtr = aBuf;
	int aNumBytes = dwBytes;
	while (aNumBytes > 0) {
		long ret = ov_read(&vf, aPtr, aNumBytes, &current_section);
		if (ret == 0)
			break;
		else if (ret < 0)
			break;
		else {
			aPtr += ret;
			aNumBytes -= ret;
		}
	}

	mSourceSounds[theSfxID]->Unlock(aBuf, dwBytes, NULL, 0);
	ov_clear(&vf);
	return aNumBytes == 0;
}
#else
bool DSoundManager::LoadOGGSound(unsigned int theSfxID, const string& theFilename) {
	return false;
}
#endif


bool DSoundManager::LoadAUSound(unsigned int theSfxID, const string& theFilename) {
	FILE* fp;

	fp = fopen(theFilename.c_str(), "rb");

	if (fp == nullptr)
		return false;

	char aHeaderId[5];
	aHeaderId[4] = '\0';
	fread(aHeaderId, 1, 4, fp);
	if (!strcmp(aHeaderId, ".snd") == 0)
		return false;

	ulong aHeaderSize;
	fread(&aHeaderSize, 4, 1, fp);
	aHeaderSize = LONG_BIGE_TO_NATIVE(aHeaderSize);

	ulong aDataSize;
	fread(&aDataSize, 4, 1, fp);
	aDataSize = LONG_BIGE_TO_NATIVE(aDataSize);

	ulong anEncoding;
	fread(&anEncoding, 4, 1, fp);
	anEncoding = LONG_BIGE_TO_NATIVE(anEncoding);

	ulong aSampleRate;
	fread(&aSampleRate, 4, 1, fp);
	aSampleRate = LONG_BIGE_TO_NATIVE(aSampleRate);

	ulong aChannelCount;
	fread(&aChannelCount, 4, 1, fp);
	aChannelCount = LONG_BIGE_TO_NATIVE(aChannelCount);

	fseek(fp, aHeaderSize, SEEK_SET);

	bool ulaw = false;

	ulong aSrcBitCount = 8;
	ulong aBitCount = 16;
	switch (anEncoding) {
	case 1:
		aSrcBitCount = 8;
		aBitCount = 16;
		ulaw = true;
		break;
	case 2:
		aSrcBitCount = 8;
		aBitCount = 8;
		break;

	/*
	Support these formats?

	case 3:
	aBitCount = 16;
	break;
	case 4:
	aBitCount = 24;
	break;
	case 5:
	aBitCount = 32;
	break;*/

	default:
		return false;
	}


	ulong aDestSize = aDataSize * aBitCount / aSrcBitCount;
	mSourceDataSizes[theSfxID] = aDestSize;

	PCMWAVEFORMAT aWaveFormat;
	DSBUFFERDESC aBufferDesc;

	// Set up wave format structure.
	memset(&aWaveFormat, 0, sizeof(PCMWAVEFORMAT));
	aWaveFormat.wf.wFormatTag = WAVE_FORMAT_PCM;
	aWaveFormat.wf.nChannels = (WORD)aChannelCount;
	aWaveFormat.wf.nSamplesPerSec = aSampleRate;
	aWaveFormat.wf.nBlockAlign = (WORD)(aChannelCount * aBitCount / 8);
	aWaveFormat.wf.nAvgBytesPerSec =
		aWaveFormat.wf.nSamplesPerSec * aWaveFormat.wf.nBlockAlign;
	aWaveFormat.wBitsPerSample = (WORD)aBitCount;
	// Set up DSBUFFERDESC structure.
	memset(&aBufferDesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
	aBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	//aBufferDesc.dwFlags = DSBCAPS_CTRL3D; 
	aBufferDesc.dwFlags = SOUND_FLAGS;
	aBufferDesc.dwBufferBytes = aDestSize;
	aBufferDesc.lpwfxFormat = (LPWAVEFORMATEX)&aWaveFormat;

	if (mDirectSound->CreateSoundBuffer(&aBufferDesc, &mSourceSounds[theSfxID], NULL) != DS_OK) {
		fclose(fp);
		return false;
	}

	void* lpvPtr;
	DWORD dwBytes;
	if (mSourceSounds[theSfxID]->Lock(0, aDestSize, &lpvPtr, &dwBytes, NULL, NULL, 0) != DS_OK) {
		fclose(fp);
		return false;
	}

	uchar* aSrcBuffer = new uchar[aDataSize];

	int aReadSize = fread(aSrcBuffer, 1, aDataSize, fp);
	fclose(fp);

	if (ulaw) {
		short* aDestBuffer = (short*)lpvPtr;

		for (ulong i = 0; i < aDataSize; i++) {
			int ch = aSrcBuffer[i];

			int sign = (ch < 128) ? -1 : 1;
			ch = ch | 0x80;
			if (ch > 239)
				ch = ((0xF0 | 15) - ch) * 2;
			else if (ch > 223)
				ch = (((0xE0 | 15) - ch) * 4) + 32;
			else if (ch > 207)
				ch = (((0xD0 | 15) - ch) * 8) + 96;
			else if (ch > 191)
				ch = (((0xC0 | 15) - ch) * 16) + 224;
			else if (ch > 175)
				ch = (((0xB0 | 15) - ch) * 32) + 480;
			else if (ch > 159)
				ch = (((0xA0 | 15) - ch) * 64) + 992;
			else if (ch > 143)
				ch = (((0x90 | 15) - ch) * 128) + 2016;
			else if (ch > 128)
				ch = (((0x80 | 15) - ch) * 256) + 4064;
			else
				ch = 0xff;

			aDestBuffer[i] = sign * ch * 4;
		}
	}
	else
		memcpy(lpvPtr, aSrcBuffer, aDataSize);

	delete [] aSrcBuffer;

	if (mSourceSounds[theSfxID]->Unlock(lpvPtr, dwBytes, NULL, NULL) != DS_OK)
		return false;

	if (aReadSize != aDataSize)
		return false;

	return true;
}

bool DSoundManager::LoadSound(int theSfxID, const string& theFilename) {
	if (theSfxID >= MAX_SOURCE_SOUNDS)
		return false;

	ReleaseSound(theSfxID);

	if (!mDirectSound)
		return true; // sounds just	won't play, but this is not treated as a failure condition

	mSourceFileNames[theSfxID] = theFilename;

	string aFilename = theFilename;
	string aCachedName;

	if ((aFilename.length() > 2) && (aFilename[0] != '\\') && (aFilename[0] != '/') &&
		(aFilename[1] != ':')) {
		// Not an absolute path
		aCachedName = "cached\\" + aFilename + ".wav";
		if (LoadWAVSound(theSfxID, aFilename))
			return true;
		//MkDir(GetFileDir(aCachedName));
	}

	if (LoadWAVSound(theSfxID, aFilename + ".wav"))
		return true;

#ifdef USE_OGG_LIB
	if (LoadOGGSound(theSfxID, aFilename + ".ogg")) {
		WriteWAV(theSfxID, aCachedName, aFilename + ".ogg");
		return true;
	}
#endif

	if (LoadAUSound(theSfxID, aFilename + ".au")) {
		WriteWAV(theSfxID, aCachedName, aFilename + ".au");
		return true;
	}

	return false;
}

int DSoundManager::LoadSound(const string& theFilename) {
	int i;
	for (i = 0; i < MAX_SOURCE_SOUNDS; i++)
		if (mSourceFileNames[i] == theFilename)
			return i;

	for (i = MAX_SOURCE_SOUNDS - 1; i >= 0; i--) {
		if (mSourceSounds[i] == NULL) {
			if (!LoadSound(i, theFilename))
				return -1;
			else
				return i;
		}
	}

	return -1;
}

void DSoundManager::ReleaseSound(unsigned int theSfxID) {
	if (mSourceSounds[theSfxID] != NULL) {
		mSourceSounds[theSfxID]->Release();
		mSourceSounds[theSfxID] = NULL;
		mSourceFileNames[theSfxID] = "";
	}
}

bool DSoundManager::SetBaseVolume(unsigned int theSfxID, double theBaseVolume) {
	if ((theSfxID < 0) || (theSfxID >= MAX_SOURCE_SOUNDS))
		return false;

	mBaseVolumes[theSfxID] = theBaseVolume;
	return true;
}

bool DSoundManager::SetBasePan(unsigned int theSfxID, int theBasePan) {
	if ((theSfxID < 0) || (theSfxID >= MAX_SOURCE_SOUNDS))
		return false;

	mBasePans[theSfxID] = theBasePan;
	return true;
}

bool DSoundManager::GetTheFileTime(const string& theDepFile, FILETIME* theFileTime) {
	memset(theFileTime, 0, sizeof(FILETIME));
	HANDLE aDepFileHandle = CreateFile(theDepFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (aDepFileHandle == INVALID_HANDLE_VALUE)
		return false;

	GetFileTime(aDepFileHandle, NULL, NULL, theFileTime);
	CloseHandle(aDepFileHandle);
	return true;
}

bool DSoundManager::WriteWAV(unsigned int theSfxID, const string& theFilename, const string& theDepFile) {
	if ((theFilename.length() == 0) || (theSfxID < 0) || (theSfxID >= MAX_SOURCE_SOUNDS))
		return false;

	ulong aDataSize = mSourceDataSizes[theSfxID];

	void* lpvPtr;
	DWORD dwBytes;
	if (mSourceSounds[theSfxID]->Lock(0, aDataSize, &lpvPtr, &dwBytes, NULL, NULL, 0) != DS_OK)
		return false;

	FILE* fp;
	fp = fopen(theFilename.c_str(), "wb");

	if (fp == nullptr) {
		mSourceSounds[theSfxID]->Unlock(lpvPtr, dwBytes, NULL, NULL);
		return false;
	}

	char aChunkType[5];
	aChunkType[4] = '\0';
	ulong aChunkSize = 4 + 8 + 16 + 8 + aDataSize;

	fwrite("RIFF", 1, 4, fp);
	fwrite(&aChunkSize, 4, 1, fp);
	fwrite("WAVE", 1, 4, fp);

	ulong aBufferSize;
	mSourceSounds[theSfxID]->GetFormat(NULL, 0, &aBufferSize);

	WAVEFORMATEX* aWaveFormat = (WAVEFORMATEX*)new char[aBufferSize];
	memset(aWaveFormat, 0, sizeof(WAVEFORMATEX));
	mSourceSounds[theSfxID]->GetFormat(aWaveFormat, aBufferSize, NULL);

	ushort aFormatTag = 1;
	ushort aChannelCount = aWaveFormat->nChannels;
	ulong aSampleRate = aWaveFormat->nSamplesPerSec;
	ushort aBitCount = aWaveFormat->wBitsPerSample;
	ushort aBlockAlign = (aBitCount * aChannelCount) / 8;
	ulong aBytesPerSec = aSampleRate * aBlockAlign;

	delete aWaveFormat;

	aChunkSize = 16;
	fwrite("fmt ", 1, 4, fp);
	fwrite(&aChunkSize, 1, 4, fp);
	fwrite(&aFormatTag, 2, 1, fp);
	fwrite(&aChannelCount, 2, 1, fp);
	fwrite(&aSampleRate, 4, 1, fp);
	fwrite(&aBytesPerSec, 4, 1, fp);
	fwrite(&aBlockAlign, 2, 1, fp);
	fwrite(&aBitCount, 2, 1, fp);

	FILETIME aFileTime;
	memset(&aFileTime, 0, sizeof(FILETIME));
	GetTheFileTime(theDepFile, &aFileTime);

	ushort aStrLen = theDepFile.length();
	aChunkSize = 2 + aStrLen + sizeof(FILETIME);
	fwrite("dep ", 1, 4, fp);
	fwrite(&aChunkSize, 4, 1, fp);
	fwrite(&aStrLen, 2, 1, fp);
	fwrite(theDepFile.c_str(), 1, aStrLen, fp);
	fwrite(&aFileTime, sizeof(FILETIME), 1, fp);

	fwrite("data", 1, 4, fp);
	fwrite(&aDataSize, 4, 1, fp);
	fwrite(lpvPtr, 1, aDataSize, fp);
	fclose(fp);

	if (mSourceSounds[theSfxID]->Unlock(lpvPtr, dwBytes, NULL, NULL) != DS_OK)
		return false;

	return true;
}

SoundInstance* DSoundManager::GetSoundInstance(unsigned int theSfxID) {
	if (theSfxID > MAX_SOURCE_SOUNDS)
		return NULL;

	int aFreeChannel = FindFreeChannel();
	if (aFreeChannel < 0)
		return NULL;

	if (mDirectSound == NULL) {
		mPlayingSounds[aFreeChannel] = new DSoundInstance(this, NULL);
	}
	else {
		if (mSourceSounds[theSfxID] == NULL)
			return NULL;

		mPlayingSounds[aFreeChannel] = new DSoundInstance(this, mSourceSounds[theSfxID]);
	}

	mPlayingSounds[aFreeChannel]->SetBasePan(mBasePans[theSfxID]);
	mPlayingSounds[aFreeChannel]->SetBaseVolume(mBaseVolumes[theSfxID]);

	return mPlayingSounds[aFreeChannel];
}

void DSoundManager::ReleaseSounds() {
	for (int i = 0; i < MAX_SOURCE_SOUNDS; i++)
		if (mSourceSounds[i] != NULL) {
			mSourceSounds[i]->Release();
			mSourceSounds[i] = NULL;
		}
}

void DSoundManager::ReleaseChannels() {
	for (int i = 0; i < MAX_CHANNELS; i++)
		if (mPlayingSounds[i] != NULL) {
			delete mPlayingSounds[i];
			mPlayingSounds[i] = NULL;
		}
}

void DSoundManager::ReleaseFreeChannels() {
	for (int i = 0; i < MAX_CHANNELS; i++)
		if (mPlayingSounds[i] != NULL && mPlayingSounds[i]->IsReleased()) {
			delete mPlayingSounds[i];
			mPlayingSounds[i] = NULL;
		}
}

void DSoundManager::StopAllSounds() {
	for (int i = 0; i < MAX_CHANNELS; i++)
		if (mPlayingSounds[i] != NULL) {
			bool isAutoRelease = mPlayingSounds[i]->mAutoRelease;
			mPlayingSounds[i]->Stop();
			mPlayingSounds[i]->mAutoRelease = isAutoRelease;
		}
}


double DSoundManager::GetMasterVolume() {
	MIXERCONTROLDETAILS mcd;
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL mlct;
	MIXERLINE mixerLine;
	HMIXER hmx;
	MIXERCAPS pmxcaps;

	mixerOpen((HMIXER*)&hmx, 0, 0, 0, MIXER_OBJECTF_MIXER);
	mixerGetDevCaps(0, &pmxcaps, sizeof(pmxcaps));

	mxlc.cbStruct = sizeof(mxlc);
	mxlc.cbmxctrl = sizeof(mlct);
	mxlc.pamxctrl = &mlct;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mixerLine.cbStruct = sizeof(mixerLine);
	mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	mixerGetLineInfo((HMIXEROBJ)hmx, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
	mxlc.dwLineID = mixerLine.dwLineID;
	mixerGetLineControls((HMIXEROBJ)hmx, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

	mcd.cbStruct = sizeof(mcd);
	mcd.dwControlID = mlct.dwControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;
	mcd.cbDetails = sizeof(mxcd_u);
	mcd.paDetails = &mxcd_u;

	mixerGetControlDetails((HMIXEROBJ)hmx, &mcd, 0L);

	mixerClose(hmx);

	return mxcd_u.dwValue / (double)0xFFFF;
}

void DSoundManager::SetMasterVolume(double theVolume) {
	MIXERCONTROLDETAILS mcd;
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL mlct;
	MIXERLINE mixerLine;
	HMIXER hmx;
	MIXERCAPS pmxcaps;

	mixerOpen((HMIXER*)&hmx, 0, 0, 0, MIXER_OBJECTF_MIXER);
	mixerGetDevCaps(0, &pmxcaps, sizeof(pmxcaps));

	mxlc.cbStruct = sizeof(mxlc);
	mxlc.cbmxctrl = sizeof(mlct);
	mxlc.pamxctrl = &mlct;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mixerLine.cbStruct = sizeof(mixerLine);
	mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	mixerGetLineInfo((HMIXEROBJ)hmx, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
	mxlc.dwLineID = mixerLine.dwLineID;
	mixerGetLineControls((HMIXEROBJ)hmx, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

	mcd.cbStruct = sizeof(mcd);
	mcd.dwControlID = mlct.dwControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;
	mcd.cbDetails = sizeof(mxcd_u);
	mcd.paDetails = &mxcd_u;

	mxcd_u.dwValue = (int)(0xFFFF * theVolume);
	mixerSetControlDetails((HMIXEROBJ)hmx, &mcd, 0L);

	mixerClose(hmx);
}

void DSoundManager::Flush() {
}

void DSoundManager::SetCooperativeWindow(HWND theHWnd, bool isWindowed) {
	if (mDirectSound != NULL)
		mDirectSound->SetCooperativeLevel(theHWnd,DSSCL_NORMAL);
	/*
	if (isWindowed==true) mDirectSound->SetCooperativeLevel(theHWnd,DSSCL_NORMAL);
	else mDirectSound->SetCooperativeLevel(theHWnd,DSSCL_EXCLUSIVE);
	*/
}
#undef SOUND_FLAGS
