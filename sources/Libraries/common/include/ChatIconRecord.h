#pragma once
#include "TableData.h"

class CChatIconInfo : public CRawDataInfo {
public:
	char szSmall[16]{};
	int  nSmallX{0};
	int  nSmallY{0};
	char szSmallOff[16]{};
	int  nSmallOffX{0};
	int  nSmallOffY{0};
	char szBig[16]{};
	int  nBigX{0};
	int  nBigY{0};
	char szHint[32]{};
};
