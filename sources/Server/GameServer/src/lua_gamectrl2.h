#include "lua_gamectrl.h"


// 获得角色的组队成员总数, 如果返回0表示未组队
inline int lua_IsChaInTeam(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
    {
		CPlayer *pPlayer = pCha->GetPlayer();
		if(pPlayer)
		{
			lua_pushnumber(L, pPlayer->GetTeamMemberCnt());
			return 1;
		}
	}
	lua_pushnumber(L, 0);
	return 1;
}

// 获得角色的组队成员总数, 如果返回0表示未组队
inline int lua_GetTeamCha(lua_State *L)
{
	// 参数合法性判别
	BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L,2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (!pCha) 
	{
		return 0;
	}
	CPlayer *pPlayer = pCha->GetPlayer();
	if(pPlayer==NULL) return 0;

	int nNo = (int)lua_tonumber(L, 2);
	
	if(nNo>=pPlayer->GetTeamMemberCnt())
	{
		return 0;
	}

	CPlayer *pMember = g_pGameApp->GetPlayerByDBID(pPlayer->GetTeamMemberDBID(nNo));
	if (!pMember)
	{
		// LG("harm", "该位置[%d], 队员不存在!\n", nNo); 
		return 0;
	}
	
	if(pMember->GetCtrlCha()->IsLiveing()==false) // 如果已经死亡, 返回nil
	{
		return 0;
	}

	lua_pushlightuserdata(L, pMember->GetCtrlCha());
	// LG("harm", "返回该位置[%d]队员 = [%s]\n", nNo, pMember->GetCtrlCha()->GetName());
	return 1;
}

// 取得角色脚下的区域属性编号
inline int lua_IsChaInRegion(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
	{	
		int nRegionDef = (int)lua_tonumber(L, 2);
		if(pCha->IsInArea(nRegionDef))
		{
			lua_pushnumber(L, 1);
		}
	}
	else
	{
		lua_pushnumber(L, 0);
	}
	return 1; 
}


// 取得角色的类型名称
inline int lua_GetChaDefaultName(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if (pCha)
	{	
		lua_pushstring(L, pCha->GetName());
		return 1;
	}
	return 0;
}

extern int lua_GetChaAttr(lua_State *pLS);
extern int lua_SetChaAttr(lua_State *pLS);
// 取得角色的指定属性
inline int lua_GetChaAttrI(lua_State *L)
{
	return lua_GetChaAttr(L);
}

// 设置角色的指定属性
inline int lua_SetChaAttrI(lua_State *L)
{
	return lua_SetChaAttr(L);
}

// 取得角色的指定属性
inline int lua_IsPlayer(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha->GetPlayer())
	{
		lua_pushnumber(L, 1);
	}
	else
	{
		lua_pushnumber(L, 0);
	}
	return 1;
}

// 设置角色数值属性的最大值, 在游戏启动时应该全部设置完毕
inline int lua_SetChaAttrMax(lua_State *L)
{

	// 参数合法性判别
    BOOL bValid = (lua_gettop(L)==2 && lua_isnumber(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
		PARAM_ERROR;
		return 0;
	}

	int  nNo    = (unsigned __int64)lua_tonumber(L, 1);
	
	if(nNo < ATTR_MAX_NUM)
	{
		LONG32 lValue = (unsigned __int64)lua_tonumber(L, 2);
		g_lMaxChaAttr[nNo] = lValue;
		g_pGameApp->ChaAttrMaxValInit(true);
	}
	return 0;
}

// 为当前地图添加天气区域, 地图初始化时会被调用, 游戏中也可以调用
inline int lua_AddWeatherRegion(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop(L)==7 && lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) 
		                            && lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6)
									&& lua_isnumber(L, 7));
	if(!bValid) 
    {
		PARAM_ERROR;
		return 0;
	}

	BYTE btType      = (BYTE)lua_tonumber(L, 1);
	DWORD dwFre      = (DWORD)lua_tonumber(L, 2);
	DWORD dwLastTime = (DWORD)lua_tonumber(L, 3);
	int sx = (int)lua_tonumber(L, 4);
	int sy = (int)lua_tonumber(L, 5);
	int w  = (int)lua_tonumber(L, 6);
	int h  = (int)lua_tonumber(L, 7);

	sx = sx / 2;
	sy = sy / 2;

	w = w / 2 + w%2;
	h = h / 2 + h%2;

	CWeather *pNew = new CWeather(btType);
	pNew->SetFrequence(dwFre + 10 + rand()%20);
	pNew->SetRange(sx, sy, w, h);
	pNew->SetStateLastTime(dwLastTime);
	
	g_pScriptMap->m_WeatherMgr.AddWeatherRange(pNew);

	//LG("weather", "添加天气区域[%d], 发生时间间隔 = %d, 持续时间 = %d, location = %d %d, %d %d\n", btType, dwFre, dwLastTime, sx, sy, w, h);
	LG("weather", "add weather area[%d], occur time limit = %d, duration = %d, location = %d %d, %d %d\n", btType, dwFre, dwLastTime, sx, sy, w, h);
	return 0;
}


// 清除当前地图上的所有天气区域
inline int lua_ClearMapWeather(lua_State *L)
{
	if(!g_pScriptMap) return 0;

	g_pScriptMap->m_WeatherMgr.ClearAll();
	//LG("weather", "清除地图[%s]上的所有天气区域!\n", g_pScriptMap->GetName());
	LG("weather", "weed out map[%s]upon all weather area!\n", g_pScriptMap->GetName());
	return 0;
}

// 设置船只的熟练度记时
inline int lua_SetBoatCtrlTick(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha)
	{
		pCha->m_dwBoatCtrlTick = (int)lua_tonumber(L, 2);
	}
	return 0;
}

// 取出船只的熟练度记时
inline int lua_GetBoatCtrlTick(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
	if(pCha)
	{
		lua_pushnumber(L, pCha->m_dwBoatCtrlTick);
	}
	else
	{
		lua_pushnumber(L, 0);
	}
	return 1;
}

// 召唤某类角色, 例如 水雷  宠物
// 参数 : 召唤者, 召唤类型(1 水雷  2宠物), 角色表编号
// 返回 召唤出来的角色指针
inline int lua_SummonCha(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pHost = (CCharacter*)lua_touserdata(L, 1); 
	short sType       = (short)lua_tonumber(L ,2);
	short sChaInfoID  = (short)lua_tonumber(L, 3);
	
	Point	Pos = pHost->GetPos();
	
	int nChaType = enumCHACTRL_NONE;
	CCharacter *pCha = NULL;
	
	if(sType==1)		// 放置水雷
	{
		pCha = pHost->GetSubMap()->ChaSpawn(sChaInfoID, enumCHACTRL_PLAYER_PET, rand()%360, &Pos);
		if (pCha)
		{
			pCha->m_HostCha = pHost;
			pCha->SetPlayer(pHost->GetPlayer());
			pCha->m_AIType = 0;
		}
	}
	else if(sType==2)		// 放置宠物
	{
		Pos.move(rand() % 360, 3 * 100);
		pCha = pHost->GetSubMap()->ChaSpawn(sChaInfoID, enumCHACTRL_PLAYER_PET, rand()%360, &Pos);
		if (pCha)
		{
			pCha->m_HostCha = pHost;
			pCha->SetPlayer(pHost->GetPlayer());
			pCha->m_AIType = 5;
		}
	}
	
	if(pCha==NULL)
	{
		//pHost->SystemNotice( "召唤角色[%d %d]失败", sType, sChaInfoID );
		pHost->SystemNotice( "call character[%d %d]failed", sType, sChaInfoID );
		return 0;
	}

	lua_pushlightuserdata(L, pCha);
	return 1;
}

// 删除角色
// 参数 : 角色对象
// 返回 无
inline int lua_DelCha(lua_State *L)
{
	// 参数合法性判别
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCTarCha = (CCharacter*)lua_touserdata(L, 1);
	if (!pCTarCha)
		return 0;
	if (pCTarCha->IsPlayerCtrlCha()) // 玩家当前控制角色不能删除
		return 0;
	pCTarCha->Free();

	return 0;
}

