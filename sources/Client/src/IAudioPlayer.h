#ifndef IAUDIOPLAYER_H
#define IAUDIOPLAYER_H

#include <windows.h>

#define AUDIODLL_API __declspec(dllexport)

#include <string>

using namespace std;

struct NiVector
{
	FLOAT	X, Y, Z;

	NiVector() { X = Y = Z = 0.F; }
	NiVector( FLOAT InX, FLOAT InY, FLOAT InZ ) : X( InX ), Y( InY ), Z( InZ ) {}
};

class IAudioPlayer
{
public:
	IAudioPlayer() {}
	virtual ~IAudioPlayer() {}

	virtual BOOL	Initialize() = 0;
	virtual	BOOL	LoadMusic( const string& Filename ) = 0;
	virtual BOOL	CreateInstance( const string& Filename ) = 0;
	virtual VOID	Destroy() = 0;
	virtual VOID	SetListener( const NiVector& Location, const NiVector& Up, const NiVector& Right, const NiVector& Front ) = 0;
	virtual VOID	Update( BOOL Realtime ) = 0;
	virtual VOID	Flush() = 0;
};

AUDIODLL_API IAudioPlayer* CreateAudioPlayer();
AUDIODLL_API VOID	RemoveAudioPlayer();

#endif 
