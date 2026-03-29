#include "lua_gamectrl.h"


// , 0
inline int IsChaInTeam(CCharacter* pCha)
{
	if (pCha)
    {
		CPlayer *pPlayer = pCha->GetPlayer();
		if(pPlayer)
			return pPlayer->GetTeamMemberCnt();
	}
	return 0;
}

// , 0
inline CCharacter* GetTeamCha(CCharacter* pCha, int nNo)
{
	if (!pCha)
		return nullptr;
	CPlayer *pPlayer = pCha->GetPlayer();
	if(pPlayer==NULL) return nullptr;

	if(nNo>=pPlayer->GetTeamMemberCnt())
		return nullptr;

	CPlayer *pMember = g_pGameApp->GetPlayerByDBID(pPlayer->GetTeamMemberDBID(nNo));
	if (!pMember)
		return nullptr;

	if(pMember->GetCtrlCha()->IsLiveing()==false)
		return nullptr;

	return pMember->GetCtrlCha();
}

//
inline int IsChaInRegion(CCharacter* pCha, int nRegionDef)
{
	if (pCha)
	{
		if(pCha->IsInArea(nRegionDef))
			return 1;
		return 0;
	}
	return 0;
}


//
inline std::string GetChaDefaultName(CCharacter* pCha)
{
	if (pCha)
		return std::string(pCha->GetName());
	return std::string();
}

extern int GetChaAttr_raw(lua_State *pLS);
//
// Delegates to raw GetChaAttr_raw from Expand.h
inline int GetChaAttrI_raw(lua_State *L)
{
	return GetChaAttr_raw(L);
}

//
// Inlined version of SetChaAttr logic (original lua_SetChaAttr was removed)
inline int SetChaAttrI_raw(lua_State *L)
{
	int nParaNum = lua_gettop(L);
	if (nParaNum > 3) return 0;

	CCharacter* pCCha = static_cast<CCharacter*>(lua_touserdata(L, 1));
	short sAttrIndex = static_cast<int64_t>(lua_tonumber(L, 2));
	LONG32 lValue = static_cast<int64_t>(lua_tonumber(L, 3));

	if (!pCCha) return 0;
	if (sAttrIndex < 0 || sAttrIndex >= ATTR_MAX_NUM) return 0;

	long lSetRet = pCCha->setAttr(sAttrIndex, lValue);
	if (lSetRet == 0) return 0;
	return 1;
}

//
inline int IsPlayer(CCharacter* pCha)
{
	if(!pCha) return 0;
	if(pCha->GetPlayer())
		return 1;
	return 0;
}

// ,
inline void SetChaAttrMax(int nNo, unsigned int lValue)
{
	if(nNo < ATTR_MAX_NUM)
	{
		g_lMaxChaAttr[nNo] = (LONG32)lValue;
		g_pGameApp->ChaAttrMaxValInit(true);
	}
}

// , ,
inline void AddWeatherRegion(int btType, int dwFre, int dwLastTime, int sx, int sy, int w, int h)
{
	sx = sx / 2;
	sy = sy / 2;

	w = w / 2 + w%2;
	h = h / 2 + h%2;

	CWeather *pNew = new CWeather((BYTE)btType);
	pNew->SetFrequence(dwFre + 10 + rand()%20);
	pNew->SetRange(sx, sy, w, h);
	pNew->SetStateLastTime(dwLastTime);

	g_pScriptMap->m_WeatherMgr.AddWeatherRange(pNew);

	ToLogService("common", "add weather area[{}], occur time limit = {}, duration = {}, location = {} {}, {} {}", btType, dwFre, dwLastTime, sx, sy, w, h);
}


//
inline void ClearMapWeather()
{
	if(!g_pScriptMap) return;

	g_pScriptMap->m_WeatherMgr.ClearAll();
	ToLogService("common", "weed out map[{}] upon all weather area!", g_pScriptMap->GetName());
}

//
inline void SetBoatCtrlTick(CCharacter* pCha, int tick)
{
	if(pCha)
		pCha->m_dwBoatCtrlTick = tick;
}

//
inline int GetBoatCtrlTick(CCharacter* pCha)
{
	if(pCha)
		return (int)pCha->m_dwBoatCtrlTick;
	return 0;
}

// ,
//  : , (1   2),
//
inline CCharacter* SummonCha(CCharacter* pHost, int sType, int sChaInfoID)
{
	if(!pHost) return nullptr;

	Point	Pos = pHost->GetPos();

	CCharacter *pCha = NULL;

	if(sType==1)		//
	{
		pCha = pHost->GetSubMap()->ChaSpawn((short)sChaInfoID, enumCHACTRL_PLAYER_PET, rand()%360, &Pos);
		if (pCha)
		{
			pCha->m_HostCha = pHost;
			pCha->SetPlayer(pHost->GetPlayer());
			pCha->m_AIType = 0;
		}
	}
	else if(sType==2)		//
	{
		Pos.move(rand() % 360, 3 * 100);
		pCha = pHost->GetSubMap()->ChaSpawn((short)sChaInfoID, enumCHACTRL_PLAYER_PET, rand()%360, &Pos);
		if (pCha)
		{
			pCha->m_HostCha = pHost;
			pCha->SetPlayer(pHost->GetPlayer());
			pCha->m_AIType = 5;
		}
	}

	if(pCha==NULL)
	{
		pHost->SystemNotice( "call character[%d %d]failed", sType, sChaInfoID );
		return nullptr;
	}

	return pCha;
}

//
//  :
//
inline void DelCha(CCharacter* pCTarCha)
{
	if (!pCTarCha)
		return;
	if (pCTarCha->IsPlayerCtrlCha())
		return;
	pCTarCha->Free();
}
