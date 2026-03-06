#include "stdafx.h"
#include "PicSet.h"
#include "util2.h"

CPicSet::CPicSet()
{
}

CPicSet::~CPicSet()
{
	std::map<char, CPicture *>::iterator it = m_mapList.begin();
	for(; it != m_mapList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	m_mapList.clear();
}

bool CPicSet::init()
{
	char ids[] = {'2', '3', '4', '5', '6', '7', '9', 'A', 'C', 'E', 'F', 'H', 'K', 'L', 'M', 'N', 'P', 'R', 'T', 'U', 'V', 'X', 'Y', 'Z'};
	memcpy(m_ids, ids, ID_MAX);

	for(int i = 0; i < ID_MAX; i++)
	{
		std::ostringstream oss;
		oss << "Pic/" << ids[i] << ids[i] << ".bmp";
		std::string strFilename = GetResPath(oss.str().c_str());
		CPicture *pPic = new CPicture(strFilename);
		if(pPic->LoadImg())
		{
			pPic->SetID(ids[i]);
			std::pair<char, CPicture *> MapNode(ids[i], pPic);
			m_mapList.insert(MapNode);
		}
		else
		{
			return false;
		}
	}

	return true;
}

char CPicSet::RandGetID()
{
	int index = rand() % ID_MAX;
	return m_ids[index];
}

CPicture *CPicSet::GetPicture(char id)
{
	std::map<char, CPicture *>::iterator it = m_mapList.find(id);
	if(it != m_mapList.end())
		return m_mapList[id];
	return NULL;
}
