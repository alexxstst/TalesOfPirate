#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "GameMovie.h"

class GameLoading {
public:
	GameLoading();

	virtual ~GameLoading();
	virtual void Create(std::string param);
	virtual void Close();
	bool Active();

public:
	HWND m_hLoading;
	HWND m_hImage;
	HBITMAP m_hBitmap;
	bool m_bShow;
	bool m_bWait;
	std::string szParam;
	static GameLoading* Init();

private:
	static GameLoading* _instance;
};
