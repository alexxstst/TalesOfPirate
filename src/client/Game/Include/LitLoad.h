//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwClassDecl.h"
#include "lwInterface.h"
#include "lwShaderTypes.h"
#include "lwIUtil.h"
//#include "MPCharacter.h"
//#include "MPSceneObject.h"
//#include "MPSceneItem.h"
LW_USING

struct LitInfo
{
    DWORD obj_type;
    DWORD anim_type;
    DWORD sub_id;
    DWORD color_op;
    char file[128];
    char mask[128];
    char str_buf[8][128];
    DWORD str_num;
};


class LitMgr
{
    LitInfo* _lit_seq;
    DWORD _lit_num;

public:
    LitMgr()
        : _lit_seq(0), _lit_num(0)
    {}
    ~LitMgr()
    {
        LW_SAFE_DELETE_A(_lit_seq);
    }

    int Load(const char* file);
    LitInfo* Lit(DWORD obj_type, DWORD sub_id, const char* file);
    //int Lit(MPSceneObj* obj);
    //int Lit(MPCharacter* obj);
    //int Lit(MPSceneItem* obj);
};
