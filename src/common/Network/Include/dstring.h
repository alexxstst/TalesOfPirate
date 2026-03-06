//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================
#pragma once

#include <string>
#include <cstdlib>
#include "BaseBuf.h"
#include "DBCCommon.h"

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)
//=================================================================
class dstrbuf:public BaseBuf
{
public:
	dstrbuf(uLong size):BaseBuf(size){}
	void Free(){PreAllocStru::Free();}
private:
	void Initially(){*(short*)getbuf() =0;}
};
//=================================================================
class dstring
{
public:
	dstring():m_buf(0),m_overwrite(true){};						//
	dstring(dstring&ds):m_buf(0),m_overwrite(true)
	{
		SetSize(ds.length()+1);
		strcpy(m_buf->getbuf(),ds);
	}
	dstring(cChar *str):m_buf(0),m_overwrite(true)				//
	{
		if(!str)return;
		SetSize(uLong(strlen(str)+1));
		strcpy(m_buf->getbuf(),str);
	}
	dstring(cwChar *str):m_buf(0),m_overwrite(true)				//
	{
		if(!str)return;
		SetSize(wctmbcpy((char*)0,str)+1);
		wctmbcpy(m_buf->getbuf(),str);
	}
	explicit dstring(int i):m_buf(0),m_overwrite(true)			//
	{
		char l_str[21];
		_itoa(i,l_str,10);

		SetSize(uLong(strlen(l_str)+1));
		strcpy(m_buf->getbuf(),l_str);
	}
	~dstring(){Free();}											//

	dstring& operator=(dstring &ds)
	{
		m_overwrite	=true;
		SetSize(ds.length()+1);
		strcpy(m_buf->getbuf(),ds);
		return *this;
	}
	dstring& operator=(cChar *str)
	{
		if(!str)return *this;
		m_overwrite =true;
		SetSize(uLong(strlen(str)+1));
		strcpy(m_buf->getbuf(),str);
		return *this;
	}
	dstring& operator=(int i)						//
	{
		char l_str[21];
		_itoa(i,l_str,10);

		*this =l_str;
		return *this;
	}
	dstring& operator=(LLong i)
	{
		char l_str[42];
		_i64toa(i,l_str,10);
		
		*this =l_str;
		return *this;
	}

	uLong Size()const				{return !m_buf?0:m_buf->Size();}
	inline uLong SetSize(uLong)const;
	uLong length()const				{return !m_buf?0:uLong(strlen(m_buf->getbuf()));}
	void SetOverwrite(bool flag)	{m_overwrite =flag;}

	cuChar*	c_ustr()const{return (cuChar*)operator cChar*();}
	cChar*	c_str()	const{return operator cChar*();}
	operator cChar*()const{return !m_buf?"":m_buf->getbuf();}
	char operator[](uLong i)const{return *(c_str()+i);}

	char *	GetBuffer()const{return !m_buf?0:m_buf->getbuf();}

	friend dstring& operator+=(dstring& str1,cChar *str2);
	friend dstring& operator+=(dstring& str1,const dstring& str2);

	friend const dstring operator+(const dstring&,cChar *);
	friend const dstring operator+(cChar *,const dstring&);
	friend const dstring operator+(const dstring&,const dstring&);
private:
	dstring(dstrbuf *buf):m_buf(buf){};
	void Free()const{if(m_buf){m_buf->Free();m_buf=0;}}

	mutable dstrbuf	*volatile m_buf;
	mutable bool		m_overwrite{ true };//SetSize
	inline static	PreAllocHeapPtr<dstrbuf> m_heap{ 128, 10 };
};
//==============================================================
inline uLong dstring::SetSize(uLong size)const
{
	if(Size()<size)
	{
		dstrbuf *l_buf	=m_heap.Get(size);
		if(!m_overwrite)strcpy(l_buf->getbuf(),*this);
		Free();
		m_buf =l_buf;
	}else if(size<1)Free();	//
	m_overwrite	=false;
	return Size();
}
//==============================================================
inline bool operator>(const dstring&str1,cChar*str2)
{
	return strcmp(str1,str2)>0;
}
inline bool operator>(cChar*str1,const dstring&str2)
{
	return strcmp(str1,str2)>0;
}
inline bool operator>(const dstring&str1,const dstring&str2)
{
	return strcmp(str1,str2)>0;
}
inline bool operator==(const dstring&str1,cChar*str2)
{
	return !strcmp(str1,str2);
}
inline bool operator==(cChar*str1,const dstring&str2)
{
	return !strcmp(str1,str2);
}
inline bool operator==(const dstring&str1,const dstring&str2)
{
	return !strcmp(str1,str2);
}
//==+=============================================================
inline dstring& operator+=(dstring& str1,cChar *str2)
{
	str1.SetOverwrite(false);
	str1.SetSize(str1.length()+strlen(str2)+1);
	strcat(str1.m_buf->getbuf(),str2);
	return str1;
}
/*inline dstring& operator+=(dstring& str1,int i)
{
	char l_str[21];
	_itoa(i,l_str,10);

	return str1+=l_str;
}*/
inline dstring&	operator+=(dstring& str1,LLong i)
{
	char l_str[42];
	_i64toa(i,l_str,10);

	return str1+=l_str;
}

inline dstring& operator+=(dstring& str1,const dstring& str2)
{
	str1.SetOverwrite(false);
	str1.SetSize(str1.length()+str2.length()+1);
	strcat(str1.m_buf->getbuf(),str2);
	return str1;
}
//==+============================================================
inline const dstring operator+(const dstring&str1,cChar *str2)
{
	dstring	l_str =str1;
	l_str+=str2;
	dstrbuf *l_buf =l_str.m_buf;
	l_str.m_buf	=0;
	return l_buf;
}

inline const dstring operator+(cChar *str1,const dstring&str2)
{
	dstring	l_str =str1;
	l_str+=str2;
	dstrbuf *l_buf =l_str.m_buf;
	l_str.m_buf	=0;
	return l_buf;
}

inline const dstring operator+(const dstring&str1,const dstring&str2)
{
	dstring	l_str =str1;
	l_str+=str2;
	dstrbuf *l_buf =l_str.m_buf;
	l_str.m_buf	=0;
	return l_buf;
}
//==<<============================================================
//inline dstring& operator<<(dstring&str1,int i)				{return str1+=i;}
inline dstring&	operator<<(dstring&str1,LLong i)			{return str1+=i;}
inline dstring& operator<<(dstring&str1,cChar* str2)		{return str1+=str2;}
inline dstring& operator<<(dstring&str1,const dstring& str2){return str1+=str2;}

#pragma pack(pop)
_DBC_END

