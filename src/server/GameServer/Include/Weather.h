// , ,  ,  
// 


// 2
// . 
// . , , 

#pragma once

class SubMap;

class CWeather
{
public:

	CWeather(BYTE btType) // 
	:_dwLastLocationTick(0), _dwFrequence(2)
	{
		_btType = btType;
		_dwStateLastTime = 20; // 20
	}
	
	void	SetRange(int sx, int sy, int w, int h);	// 
	void	RandLocation(SubMap *pMap);				// , 
	void	SetFrequence(DWORD dwFre);				// , 
	void	SetStateLastTime(DWORD dwTime);			// , 

protected:

	BYTE	_btType;  //  0  1  2
	int		_sx;	  // 
	int		_sy;
	int		_w;
	int		_h;
	DWORD	_dwLastLocationTick;
	DWORD	_dwFrequence;
	DWORD	_dwStateLastTime;
};

inline void CWeather::SetRange(int sx, int sy, int w, int h)
{
	_sx = sx;
	_sy = sy;
	_w  = w;
	_h  = h;
}

inline void CWeather::SetFrequence(DWORD dwFre)
{
	_dwFrequence = dwFre;
}

inline void CWeather::SetStateLastTime(DWORD dwTime)
{
	_dwStateLastTime = dwTime;
}


// , , , 
class CWeatherMgr 
{
public:
	
	~CWeatherMgr();
	void	AddWeatherRange(CWeather *pWeather);
	void	RemoveWeather(CWeather *pWeather);
	void	Run(SubMap *pMap);	// , WeatherRandLocation
	void	ClearAll();

protected:

	std::list<CWeather*>  _WeatherList;
};

inline void CWeatherMgr::AddWeatherRange(CWeather *pWeather)
{
	_WeatherList.push_back(pWeather);
}

inline void CWeatherMgr::RemoveWeather(CWeather *pWeather)
{
	_WeatherList.remove(pWeather);
	delete pWeather;
}

inline void CWeatherMgr::ClearAll()
{
	for(std::list<CWeather*>::iterator it = _WeatherList.begin(); it!=_WeatherList.end(); it++)
	{
		CWeather *pCur = (*it);
		delete pCur;
	}

	_WeatherList.clear();
}


