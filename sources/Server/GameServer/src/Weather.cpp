#include "StdAfx.h"
#include "Weather.h"
#include "Submap.h"


// , 
DWORD g_dwLastWeatherTick[20] = { 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 };

// , 
void CWeather::RandLocation(SubMap *pMap)
{
	DWORD dwCurTick = GetTickCount();

	if( (dwCurTick - _dwLastLocationTick) < (_dwFrequence * 1000)) return;

	_dwLastLocationTick = dwCurTick;
	
	// , 
		
	int sx = _sx + rand()%_w;
	int sy = _sy + rand()%_h;
	int nw = 1 +  rand()%3; // , 2
	int nh = 1 +  rand()%3;

	Rect	SRange = {{sx * 2 * 100, sy * 2 * 100}, {(sx + nw) * 2 * 100, (sy + nh) * 2 * 100}};
	short	sStateParam[defSKILL_STATE_PARAM_NUM];	// 

    sStateParam[0] = _btType;
    sStateParam[1] = 1;
    sStateParam[2] = (short)_dwStateLastTime;

	pMap->RangeAddState(&SRange, 0, g_pCSystemCha->GetID(), g_pCSystemCha->GetHandle(), enumSKILL_TYPE_ENEMY, enumSKILL_TAR_LORS, enumSKILL_EFF_HELPFUL, sStateParam);

	CSkillStateRecord *pEff = GetCSkillStateRecordInfo(_btType);
	if(pEff)
	{
		if( (dwCurTick - g_dwLastWeatherTick[_btType]) > 1000 * g_Config.m_lWeather) // , 2
		{
			g_dwLastWeatherTick[_btType] = dwCurTick; 
			char szText[128]; 
			
			auto& weatherFmt = LanguageRecordStore::Instance()->GetKeyString("GM_WEATHER_CPP_00001");
			snprintf(szText, sizeof(szText), "%s",
				SafeVFormat(weatherFmt, SRange.ltop.x / 100, SRange.ltop.y / 100, pEff->szDataName).c_str());

			g_pGameApp->LocalNotice( szText );
			//LG("weather", "[%s][%s], time = %d\n", pMap->GetName(), szText, dwCurTick / 1000);
			ToLogService("common", "[{}]predict[{}], time = {}", pMap->GetName(), szText, dwCurTick / 1000);
		}
	}
	
	// LG("weather", "%d, [%d %d] w = %d h = %d\n", _btType, sx * 2, sy * 2, nw, nh);
}


CWeatherMgr::~CWeatherMgr()
{
	for(std::list<CWeather*>::iterator it = _WeatherList.begin(); it!=_WeatherList.end(); it++)
	{
		CWeather *pWeather = (*it);
		delete pWeather;
	}

	_WeatherList.clear();
}

void CWeatherMgr::Run(SubMap *pMap)
{
	for(std::list<CWeather*>::iterator it = _WeatherList.begin(); it!=_WeatherList.end(); it++)
	{
		CWeather *pWeather = (*it);
		pWeather->RandLocation(pMap);
	}
}

