#include "lua_gamectrl.h"


// ��ý�ɫ����ӳ�Ա����, �������0��ʾδ���
inline int lua_IsChaInTeam(lua_State *L)
{
	// �����Ϸ����б�
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

// ��ý�ɫ����ӳ�Ա����, �������0��ʾδ���
inline int lua_GetTeamCha(lua_State *L)
{
	// �����Ϸ����б�
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
		// LG("harm", "��λ��[%d], ��Ա������!\n", nNo); 
		return 0;
	}
	
	if(pMember->GetCtrlCha()->IsLiveing()==false) // ����Ѿ�����, ����nil
	{
		return 0;
	}

	lua_pushlightuserdata(L, pMember->GetCtrlCha());
	// LG("harm", "���ظ�λ��[%d]��Ա = [%s]\n", nNo, pMember->GetCtrlCha()->GetName());
	return 1;
}

// ȡ�ý�ɫ���µ��������Ա��
inline int lua_IsChaInRegion(lua_State *L)
{
	// �����Ϸ����б�
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


// ȡ�ý�ɫ����������
inline int lua_GetChaDefaultName(lua_State *L)
{
	// �����Ϸ����б�
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
// ȡ�ý�ɫ��ָ������
inline int lua_GetChaAttrI(lua_State *L)
{
	return lua_GetChaAttr(L);
}

// ���ý�ɫ��ָ������
inline int lua_SetChaAttrI(lua_State *L)
{
	return lua_SetChaAttr(L);
}

// ȡ�ý�ɫ��ָ������
inline int lua_IsPlayer(lua_State *L)
{
	// �����Ϸ����б�
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

// ���ý�ɫ��ֵ���Ե����ֵ, ����Ϸ����ʱӦ��ȫ���������
inline int lua_SetChaAttrMax(lua_State *L)
{

	// �����Ϸ����б�
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

// Ϊ��ǰ��ͼ������������, ��ͼ��ʼ��ʱ�ᱻ����, ��Ϸ��Ҳ���Ե���
inline int lua_AddWeatherRegion(lua_State *L)
{
	// �����Ϸ����б�
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

	//LG("weather", "������������[%d], ����ʱ���� = %d, ����ʱ�� = %d, location = %d %d, %d %d\n", btType, dwFre, dwLastTime, sx, sy, w, h);
	ToLogService("common", "add weather area[{}], occur time limit = {}, duration = {}, location = {} {}, {} {}", btType, dwFre, dwLastTime, sx, sy, w, h);
	return 0;
}


// �����ǰ��ͼ�ϵ�������������
inline int lua_ClearMapWeather(lua_State *L)
{
	if(!g_pScriptMap) return 0;

	g_pScriptMap->m_WeatherMgr.ClearAll();
	//LG("weather", "�����ͼ[%s]�ϵ�������������!\n", g_pScriptMap->GetName());
	ToLogService("common", "weed out map[{}] upon all weather area!", g_pScriptMap->GetName());
	return 0;
}

// ���ô�ֻ�������ȼ�ʱ
inline int lua_SetBoatCtrlTick(lua_State *L)
{
	// �����Ϸ����б�
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

// ȡ����ֻ�������ȼ�ʱ
inline int lua_GetBoatCtrlTick(lua_State *L)
{
	// �����Ϸ����б�
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

// �ٻ�ĳ���ɫ, ���� ˮ��  ����
// ���� : �ٻ���, �ٻ�����(1 ˮ��  2����), ��ɫ�����
// ���� �ٻ������Ľ�ɫָ��
inline int lua_SummonCha(lua_State *L)
{
	// �����Ϸ����б�
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
	
	if(sType==1)		// ����ˮ��
	{
		pCha = pHost->GetSubMap()->ChaSpawn(sChaInfoID, enumCHACTRL_PLAYER_PET, rand()%360, &Pos);
		if (pCha)
		{
			pCha->m_HostCha = pHost;
			pCha->SetPlayer(pHost->GetPlayer());
			pCha->m_AIType = 0;
		}
	}
	else if(sType==2)		// ���ó���
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
		//pHost->SystemNotice( "�ٻ���ɫ[%d %d]ʧ��", sType, sChaInfoID );
		pHost->SystemNotice( "call character[%d %d]failed", sType, sChaInfoID );
		return 0;
	}

	lua_pushlightuserdata(L, pCha);
	return 1;
}

// ɾ����ɫ
// ���� : ��ɫ����
// ���� ��
inline int lua_DelCha(lua_State *L)
{
	// �����Ϸ����б�
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
	if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }

	CCharacter *pCTarCha = (CCharacter*)lua_touserdata(L, 1);
	if (!pCTarCha)
		return 0;
	if (pCTarCha->IsPlayerCtrlCha()) // ��ҵ�ǰ���ƽ�ɫ����ɾ��
		return 0;
	pCTarCha->Free();

	return 0;
}

