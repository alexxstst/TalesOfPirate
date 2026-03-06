#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "DBCCommon.h"
#include "Picture.h"

_DBC_USING
extern const char* GetResPath(const char *pszRes);

class CPicSet
{
public:
	CPicSet();
	~CPicSet();

	enum
	{
		ID_2,
		ID_3,
		ID_4,
		ID_5,
		ID_6,
		ID_7,
		ID_9,
		ID_A,
		ID_C,
		ID_E,
		ID_F,
		ID_H,
		ID_K,
		ID_L,
		ID_M,
		ID_N,
		ID_P,
		ID_R,
		ID_T,
		ID_U,
		ID_V,
		ID_X,
		ID_Y,
		ID_Z,

		ID_MAX
	};

	bool init();

	CPicture *GetPicture(char id);
	char RandGetID();

private:
	std::map<char, CPicture *> m_mapList;
	char m_ids[ID_MAX];

};
