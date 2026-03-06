#pragma once

#include <string>
#include "DBCCommon.h"

_DBC_USING

class CPicture
{
public:
	CPicture(std::string name);
	CPicture(const CPicture &rPic);
	~CPicture();
	
	CPicture& operator=(const CPicture &rPic);

	std::string GetName() { return m_strName; }
	void SetName(std::string name) { m_strName = name; }

	char GetID() { return m_nID; }
	void SetID(char id) { m_nID = id; }

	uInt GetSize() { return m_size; }
	
	char GetImgByte(uInt index);

	bool LoadImg();

private:
	void SetSize(uInt size) { m_size = size; }

	std::string m_strName; //
	char m_nID;
	char *_img;
	uInt m_size;
	uInt m_start;
};
