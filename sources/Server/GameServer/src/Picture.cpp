#include "stdafx.h"
#include "Picture.h"
#include <string>

CPicture::CPicture(std::string name)
{
	_img = NULL;
	m_start = 0;
	SetSize(0);
	SetName(name);
}

CPicture::CPicture(const CPicture &rPic)
{
	*this = rPic;
}

CPicture::~CPicture()
{
	if(_img)
	{
		delete [] _img;
		_img = NULL;
		SetSize(0);
		m_start = 0;
	}
}

CPicture& CPicture::operator=(const CPicture &rPic)
{
	if(this != &rPic)
	{
		m_strName = rPic.m_strName;
		m_nID = rPic.m_nID;
		m_size = rPic.m_size;
		m_start = rPic.m_start;

		if(m_size > 0)
		{
			_img = new char[m_size];
			memcpy(_img, rPic._img, m_size);
		}
	}
	
	return *this;
}

char CPicture::GetImgByte(uInt index)
{
	BYTE BitMask[] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

	if((index < 0) || (index > GetSize() - 1))
		return 0;

	if(index > m_start && ((rand() & 3) == 0))
	{
		return (_img[index] & BitMask[rand() & 7]);
	}

	return _img[index];
}

bool CPicture::LoadImg()
{
	if(m_strName.empty())
		return false;

	FILE *fp = fopen(m_strName.c_str(), "rb");
	if(fp == NULL)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	_img = new char[lSize];
	fread(_img, lSize, 1, fp);
	fclose(fp);
	SetSize((uInt)lSize);
	memcpy(&m_start, _img + 10, sizeof(int));

	return true;
}
