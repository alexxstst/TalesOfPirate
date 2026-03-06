#pragma once

//-----------------
// 
//-----------------
inline int lua_objIsValid(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    if(!pNode) return 0;

    lua_pushnumber(L, pNode->IsValid());
    return 1;
}

//-----------
// ID
//-----------
inline int lua_objGetID(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    lua_pushnumber(L, pNode->getID());
    return 1;
}


//--------------------
// ()
//--------------------
inline int lua_objSetPos (lua_State * L)
{
    // 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber (L, 2) &&  lua_isnumber (L, 3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    if(!pNode) return 0;

    pNode->setPos((int)lua_tonumber(L, 2), (int)lua_tonumber(L,3));
    return 0;
}

//-------------
// 
//-------------
inline int lua_objGetPos(lua_State *L)
{
   // 
    BOOL bValid = (lua_gettop (L)==1); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    if(!pNode) return 0;
    lua_pushnumber(L, pNode->GetCurX());
    lua_pushnumber(L, pNode->GetCurY());
    return 2;
}

//-------------
// 
//-------------
inline int lua_objGetFaceAngle(lua_State *L)
{
   // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    if(!pNode) return 0;
    lua_pushnumber(L, pNode->getYaw());
    return 1;
}

//-------------
// 
//-------------
inline int lua_objSetFaceAngle(lua_State *L)
{
   // 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2)); 
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    if(!pNode) return 0;
    pNode->setYaw((int)lua_tonumber(L, 2));
    return 0;
}

//---------
// 
//---------
inline int lua_objGetAttr(lua_State* L)
{
    // 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    if(!pNode) return 0;

    short sType = (short)lua_tonumber(L, 2);
    
    lua_pushnumber(L, pNode->getGameAttr()->get(sType));
    return 1;
}

//---------
// 
//---------
inline int lua_objSetAttr(lua_State* L)
{
    // 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CSceneNode *pNode = (CSceneNode*)lua_touserdata(L, 1);
    if(!pNode) return 0;

    short sType  = (short)lua_tonumber(L, 2);
    long  lValue = (long)lua_tonumber(L, 3);
    
    pNode->getGameAttr()->set(sType, lValue);
    pNode->RefreshUI();
    return 0;
}

//--------------------
// ,
//--------------------
inline int lua_chaSay(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isstring(L, 2));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CGameScene *pScene = g_pGameApp->GetCurScene();
    if(pScene==NULL)
    {
        SCENE_NULL_ERROR
        return 0;
    }
    // 
    CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
    const char *pszText = lua_tostring(L, 2);
	if( pCha )
	{
		//extern CCozeMgr g_stUICoze;
		//g_stUICoze.OnRoadSay( pCha, pszText );
		CCozeForm::GetInstance()->OnSightMsg(pCha, pszText);
	}
    return 0;
}

//-------------------------
// ()
//-------------------------
inline int lua_chaMoveTo(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber (L, 2) &&  lua_isnumber (L, 3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    CGameScene *pScene = g_pGameApp->GetCurScene();
    if(pScene==NULL)
    {
        SCENE_NULL_ERROR
        return 0;
    }
    
    CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
    if(!pCha) return 0;

	CWorldScene* pWorld = dynamic_cast<CWorldScene*>(pScene);
	if( pWorld ) pWorld->GetMouseDown().ActMove( pCha, (int)lua_tonumber(L, 2), (int)lua_tonumber(L, 3) );
    return 0;
}

//-------------
// 
//-------------
inline int lua_chaStop(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==1 && lua_islightuserdata(L, 1));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
    if(!pCha) return 0;

    pCha->GetActor()->Stop();
    return 0;
}


//-----------------
// 
//-----------------
inline int lua_chaChangePart(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==2 && lua_islightuserdata(L, 1) && lua_isnumber (L, 2));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
    if(!pCha) return 0;

    int nTabID = (int)lua_tonumber(L, 2);
    BOOL bOK = pCha->ChangePart(nTabID);
    lua_pushnumber(L, bOK);
    return 1;
}

//-------------
// 
//-------------
inline int lua_chaPlayPose(lua_State *L)
{
    // 
    BOOL bValid = (lua_gettop (L)==3 && lua_islightuserdata(L, 1) && lua_isnumber (L, 2) && lua_isnumber(L,3));
    if(!bValid) 
    {
        PARAM_ERROR
        return 0;
    }
    
    CCharacter *pCha = (CCharacter*)lua_touserdata(L, 1);
    if(!pCha) return 0;

    
    int nPoseID = (int)lua_tonumber(L, 2);
    BOOL bHold = (BOOL)lua_tonumber(L, 3); 
    pCha->GetActor()->PlayPose( nPoseID, bHold!=FALSE );
    return 0;
}



