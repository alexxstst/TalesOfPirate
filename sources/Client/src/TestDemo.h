#pragma once


#include "Character.h"

#include "SceneObj.h"
#include "SceneObjFile.h"


#ifdef TESTDEMO


class CTestDemo
{
private:
    lwIResourceMgr* _res_mgr;
    lwIModel* _skybox;

public:
    CTestDemo();
    ~CTestDemo();

    BOOL Init();
    void FrameMove();
    void Render();
};

extern CTestDemo* g_pTestDemo;
void InitTestDemo();
void ReleaseTestDemo();

#endif
