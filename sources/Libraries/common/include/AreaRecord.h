#pragma once

#include "TableData.h"


//  ID
class CAreaInfo : public EntityData
{
public:

	CAreaInfo()
	{
        dwColor = 0;
    }

    DWORD dwColor;
    int   nMusic;
    DWORD dwEnvColor;
    DWORD dwLightColor;
    float fLightDir[3];
	char  chType;			// 0-,1-,
};

// GetAreaInfo() определена в AreaRecordStore.h
#include "AreaRecordStore.h"
