#include "stdafx.h"
#include "BassLoader.h"
#include <stdlib.h>

BASS_INSTANCE* gBass = NULL;
static long gBassLoadCount = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CheckBassFunction(unsigned int theFunc, const char *theName)
{
	if (theFunc==0)
	{
		char aBuf[1024];
		sprintf(aBuf,"%s function not found in bass.dll",theName);
		MessageBox(NULL,aBuf,"Error",MB_OK | MB_ICONERROR);
		exit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BASS_INSTANCE::BASS_INSTANCE(const char *dllName)
{
	mModule = LoadLibrary(dllName);
	if (!mModule)
		return;

#define GETPROC(_x) CheckBassFunction(*((unsigned int *)&_x) = (unsigned int)GetProcAddress(mModule, #_x),#_x)    

	GETPROC(BASS_Init);
	GETPROC(BASS_Free);
	GETPROC(BASS_Stop);
	GETPROC(BASS_Start);

	*((unsigned int*) &BASS_SetGlobalVolumes) = (unsigned int) GetProcAddress(mModule, "BASS_SetGlobalVolumes");
	*((unsigned int*) &BASS_SetVolume) = (unsigned int) GetProcAddress(mModule, "BASS_SetVolume");

	if ((BASS_SetVolume == NULL) && (BASS_SetGlobalVolumes == NULL))
	{
		MessageBox(NULL,"Neither BASS_SetGlobalVolumes or BASS_SetVolume found in bass.dll","Error",MB_OK | MB_ICONERROR);
		exit(0);
	}	

	*((unsigned int*) &BASS_SetConfig) = (unsigned int) GetProcAddress(mModule, "BASS_SetConfig");

	GETPROC(BASS_MusicSetAmplify);
	GETPROC(BASS_ChannelStop);
	GETPROC(BASS_ChannelPause);
	GETPROC(BASS_ChannelResume);
	GETPROC(BASS_ChannelSetAttributes);
	GETPROC(BASS_ChannelSetPosition);
	GETPROC(BASS_ChannelIsActive);

	GETPROC(BASS_MusicLoad);
	GETPROC(BASS_MusicPlay);
	GETPROC(BASS_MusicPlayEx);
	GETPROC(BASS_MusicFree);

	GETPROC(BASS_StreamCreateFile);
	GETPROC(BASS_StreamFree);
	GETPROC(BASS_StreamPlay);

	mVersion2 = BASS_SetConfig != NULL;
	if (mVersion2)
	{
		// Version 2 has different BASS_Init params
		*((unsigned int*) &BASS_Init2) = (unsigned int) BASS_Init;
		BASS_Init = NULL;

		*((unsigned int*) &BASS_MusicLoad2) = (unsigned int) BASS_MusicLoad;
		BASS_MusicLoad = NULL;
	}

#undef GETPROC
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BASS_INSTANCE::~BASS_INSTANCE()
{
	if (mModule)
		FreeLibrary(mModule);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoadBassDLL()
{
	InterlockedIncrement(&gBassLoadCount);
	if (gBass!=NULL)
		return;

	gBass = new BASS_INSTANCE("bass.dll");
	if (gBass->mModule==NULL)
	{
		MessageBox(NULL,"Can't find bass.dll." ,"Error",MB_OK | MB_ICONERROR);
		exit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void FreeBassDLL()
{
	if (gBass!=NULL)
	{
		if (InterlockedDecrement(&gBassLoadCount) <= 0)
		{
			delete gBass;
			gBass = NULL;
		}
	}
}
