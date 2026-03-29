//-----------------
//
//-----------------
inline int objIsValid(CSceneNode* pNode)
{
    if (!pNode) return 0;
    return pNode->IsValid();
}

//-----------
// ID
//-----------
inline int objGetID(CSceneNode* pNode)
{
    if (!pNode) return 0;
    return pNode->getID();
}


//--------------------
// ()
//--------------------
inline void objSetPos(CSceneNode* pNode, int x, int y)
{
    if (!pNode) return;
    pNode->setPos(x, y);
}

//-------------
inline std::tuple<float, float> objGetPos(CSceneNode* pNode)
{
    if (!pNode) return {0.0f, 0.0f};
    return {pNode->GetCurX(), pNode->GetCurY()};
}

//-------------
//
//-------------
inline float objGetFaceAngle(CSceneNode* pNode)
{
    if (!pNode) return 0.0f;
    return pNode->getYaw();
}

//-------------
//
//-------------
inline void objSetFaceAngle(CSceneNode* pNode, int angle)
{
    if (!pNode) return;
    pNode->setYaw(angle);
}

//---------
//
//---------
inline int objGetAttr(CSceneNode* pNode, int sType)
{
    if (!pNode) return 0;
    return pNode->getGameAttr()->get((short)sType);
}

//---------
//
//---------
inline void objSetAttr(CSceneNode* pNode, int sType, int lValue)
{
    if (!pNode) return;
    pNode->getGameAttr()->set((short)sType, (long)lValue);
    pNode->RefreshUI();
}

//--------------------
// ,
//--------------------
inline void chaSay(CCharacter* pCha, const std::string& text)
{
    if (!pCha) return;
    CGameScene *pScene = g_pGameApp->GetCurScene();
    if (!pScene) return;
    CCozeForm::GetInstance()->OnSightMsg(pCha, text.c_str());
}

//-------------------------
// ()
//-------------------------
inline void chaMoveTo(CCharacter* pCha, int x, int y)
{
    if (!pCha) return;
    CGameScene *pScene = g_pGameApp->GetCurScene();
    if (!pScene) return;
    CWorldScene* pWorld = dynamic_cast<CWorldScene*>(pScene);
    if (pWorld) pWorld->GetMouseDown().ActMove(pCha, x, y);
}

//-------------
//
//-------------
inline void chaStop(CCharacter* pCha)
{
    if (!pCha) return;
    pCha->GetActor()->Stop();
}


//-----------------
//
//-----------------
inline int chaChangePart(CCharacter* pCha, int nTabID)
{
    if (!pCha) return 0;
    return pCha->ChangePart(nTabID);
}

//-------------
//
//-------------
inline void chaPlayPose(CCharacter* pCha, int nPoseID, int bHold)
{
    if (!pCha) return;
    pCha->GetActor()->PlayPose(nPoseID, bHold != 0);
}
