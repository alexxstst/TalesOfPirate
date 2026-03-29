//
#include "stdafx.h"
#include "lwEfxTrack.h"

LW_BEGIN

lwEfxTrack::lwEfxTrack()
{
    _data = 0;
}
lwEfxTrack::~lwEfxTrack()
{
    LW_SAFE_RELEASE(_data);
}


LW_RESULT lwEfxTrack::Load(const char* file)
{
    FILE* fp = fopen(file, "rb");
    if(fp ==NULL)
        return LW_RET_FAILED;

    _data = LW_NEW(lwAnimDataMatrix());
    _data->Load(fp, 0);

    fclose(fp);

    return LW_RET_OK;
}
LW_RESULT lwEfxTrack::Save(const char* file)
{
    FILE* fp = fopen(file, "wb");
    if(fp ==NULL)
        return LW_RET_FAILED;

    _data->Save(fp);

    fclose(fp);

    return LW_RET_OK;
}

LW_END
