#include "StdAfx.h"
#include "EventHandler.h"
#include "Character.h"
#include "Player.h"
#include "lua_gamectrl.h"



//-------------------------------------
//  : 
// , , 
//-------------------------------------
void CEventHandler::Event_ChaDie(CCharacter *pDead, CCharacter *pAtk)
{
	BOOL bTeam  = FALSE;
	
	// , 
	CCharacter *pValidCha[25] = { NULL,NULL,NULL,NULL,NULL,
								  NULL,NULL,NULL,NULL,NULL,
								  NULL,NULL,NULL,NULL,NULL,
								  NULL,NULL,NULL,NULL,NULL,
								  NULL,NULL,NULL,NULL,NULL};
	
	int nValidCha = 0;
	
	CPlayer *pPlayer = pAtk->GetPlayer();
	if(pPlayer==NULL) // 
	{
		MPTimer t;
		t.Begin();
		// 
		extern lua_State *g_pLuaState;
		lua_getglobal(g_pLuaState, "GetExp_New");
		if( !lua_isfunction( g_pLuaState, -1 ) )
		{
			lua_pop( g_pLuaState, 1 );
			ToLogService("common", "GetExp_New" );
			return;
		}

		luabridge::push(g_pLuaState, static_cast<CCharacter*>(pDead));
		luabridge::push(g_pLuaState, static_cast<CCharacter*>(pAtk));
		int r = lua_pcall(g_pLuaState, 2, 0, 0);
		if(r!=0) //
		{
			//LG("lua_err", "GetExp_New, ()[%s], [%s]!\n", pAtk->GetName(), pDead->GetName());
			ToLogService("lua", LogLevel::Error, "GetExp_New transact error, attacker(bugbear)[{}], people was bring down[{}]!", pAtk->GetName(), pDead->GetName());
			lua_callalert(g_pLuaState, r); 	
		}
		lua_settop(g_pLuaState, 0);
		pDead->ItemCount(pAtk); // 

		DWORD dwEndTime = t.End();
		if(dwEndTime > 10)
		{
			//LG("script_time", ", ! time = %d\n", dwEndTime);
			ToLogService("lua", LogLevel::Trace, "when player dead transfer experience assign script,account time too long! time = {}", dwEndTime);
		}
		return; // , 
	}
	
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = pDead->m_pHate->GetHarmRec(i);
		if(pHarm->btValid > 0 && pHarm->IsChaValid())
		{
			BOOL bAdd = TRUE;
			for(int j = 0; j < nValidCha; j++)
			{
				if(pHarm->pAtk==pValidCha[j])
				{
					bAdd = FALSE;
					break;
				}
			}
			if(bAdd)
			{
				pValidCha[nValidCha] = pHarm->pAtk;
				nValidCha++;
			}
			
			// 
			pPlayer = pHarm->pAtk->GetPlayer();
			if(pPlayer==NULL)
			{
				//LG("team_error", ", player!, [%s]\n", pHarm->pAtk->GetName());
				ToLogService("common", LogLevel::Error, "it appear especially error when check teammate experience assign, player finger is null!, character name[{}]", pHarm->pAtk->GetName());
				break;
			}
			
			for(int i = 0; i < pPlayer->GetTeamMemberCnt(); i++) // 
			{
				CCharacter *pOther = pPlayer->GetTeamMemberCha(i);
				if (!pOther)			 continue;
				if(!pOther->IsLiveing()) continue;

				BOOL bAdd = TRUE;
				for(int j = 0; j < nValidCha; j++)
				{
					if(pOther==pValidCha[j])
					{
						bAdd = FALSE;
						break;
					}
				}
				if(bAdd)
				{
					pValidCha[nValidCha] = pOther;
					nValidCha++;
				}
			}
		}
	}
	
	// 
	for(int i =0; i < nValidCha; i++)
	{
		CCharacter *pCur = pValidCha[i];
		if(pCur!=pAtk)	pCur->GetPlyMainCha()->m_CChaAttr.ResetChangeFlag();
	}
	
	MPTimer t;
	t.Begin();
	
	// 
	extern lua_State *g_pLuaState;
	lua_getglobal(g_pLuaState, "GetExp_New");
	if( !lua_isfunction( g_pLuaState, -1 ) )
	{
		lua_pop( g_pLuaState, 1 );
		ToLogService("common", "GetExp_New" );
		return;
	}

	MPTimer tLua; tLua.Begin();
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pDead));
	luabridge::push(g_pLuaState, static_cast<CCharacter*>(pAtk));
	int r = lua_pcall(g_pLuaState, 2, 0, 0);
	if(r!=0) // 
	{
		//LG("lua_err", "GetExp_New, [%s], [%s]!\n", pAtk->GetName(), pDead->GetName());
		ToLogService("lua", LogLevel::Error, "GetExp_New transact error, attacker[{}], bugbear wsa bring down [{}]!", pAtk->GetName(), pDead->GetName());
		lua_callalert(g_pLuaState, r); 	
	}
	lua_settop(g_pLuaState, 0);
	tLua.End();
	
	// 
	for(int i =0; i < nValidCha; i++)
	{
		CCharacter *pCur = pValidCha[i];
		if(pCur!=pAtk) pCur->GetPlyMainCha()->SynAttr(enumATTRSYN_ATTACK);
	}

	MPTimer tMission; tMission.Begin();
	// 
	pPlayer = pAtk->GetPlayer();
	pAtk->AfterObjDie(pAtk, pDead);
	for(int i = 0; i < pPlayer->GetTeamMemberCnt(); i++) // 
	{
		CCharacter *pOther = pPlayer->GetTeamMemberCha(i);
		if (!pOther)			 continue;
		if(!pOther->IsLiveing()) continue;
		pOther->AfterObjDie(pAtk, pDead);
	}
	tMission.End();
		
	MPTimer tItem; tItem.Begin();
	// 
	pDead->ItemCount(pAtk);

	tItem.End();
	
	DWORD dwEndTime = t.End();
	if(dwEndTime > 10)
	{
		//LG("script_time", ", , time = %d, exp = %d, upgrade = %d, item = %d!\n", dwEndTime, tLua.GetTimeCount(), tMission.GetTimeCount(), tItem.GetTimeCount());
		ToLogService("lua", LogLevel::Trace, "the flow of assign experience when bugbear dead, calculate time too long, time = {}, exp = {}, upgrade = {}, item = {}!", dwEndTime, tLua.GetTimeCount(), tMission.GetTimeCount(), tItem.GetTimeCount());
	}
}
