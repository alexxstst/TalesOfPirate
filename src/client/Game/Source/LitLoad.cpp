//
#include "stdafx.h"
#include <stdio.h>
#include "LitLoad.h"

LitMgr g_lit_mgr;

int LitMgr::Load(const char* file)
{
    FILE* fp = fopen(file, "rt");
    if(fp == 0)
        return 0;

    char str[64];

    fscanf(fp, "%s %d\n", str, &_lit_num);

    _lit_seq = LW_NEW(LitInfo[_lit_num]);

    LitInfo* p;

    for(DWORD i = 0; i < _lit_num; i++)
    {
        p = &_lit_seq[i];

        fscanf(fp, "%s %d", str, &p->obj_type);

        switch(p->obj_type)
        {
        case 0: // character
            fscanf(fp, "%d %s %s %d", &p->anim_type, p->file, p->mask, &p->str_num);
            for(DWORD j = 0; j < p->str_num; j++)
            {
                fscanf(fp, " %s", p->str_buf[j]);
            }
            break;
        case 1: // scene
        case 2: // item
            fscanf(fp, "%d %s %d %d %d", &p->anim_type, p->file, &p->sub_id, &p->color_op, &p->str_num);
            for(DWORD j = 0; j < p->str_num; j++)
            {
                fscanf(fp, " %s", p->str_buf[j]);
            }
            break;
        default:
            assert(0 && "invalid lit type");
        }
    }

    fclose(fp);
    return 1;
}
LitInfo* LitMgr::Lit(DWORD obj_type, DWORD sub_id, const char* file)
{
    LitInfo* p;

    for(DWORD i = 0; i < _lit_num; i++)
    {
        p = &_lit_seq[i];

        if(p->obj_type == obj_type && _tcsicmp(p->file, file) == 0)
        {
            return p;
        }
    }

    return 0;
}

//int LitMgr::Lit(MPSceneObj* obj)
//{
//}
//int LitMgr::Lit(MPCharacter* obj)
//{
//}
//int LitMgr::Lit(MPSceneItem* obj)
//{
//}
