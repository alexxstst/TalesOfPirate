#pragma once
#include "unicode/resbund.h"		//资源管理信息
#include "unicode/ucnv.h"			//字符编码转换
#include "unicode/uclean.h"			//字符编码转换
#include "unicode/msgfmt.h"			//格式化字符串
	
//#include "pi_Alloc.h"

#include <map>
#include <memory>
// Use to compare the contents of two pointers
struct charValueLess
{
	bool operator()(const char* _Left, const char*  _Right) const
	{
		if(_Left == NULL || _Right == NULL)
		{
			printf("%s\t%s\r\n",_Left,_Right);
			return false;
		}

		int ret = strcmp(_Left, _Right);	

		if(ret < 0)
			return true;
		else
			return false;
	}
};

typedef std::map <const char*, std::unique_ptr<char[]>, charValueLess> StringMap;
//typedef map <const char*, const char*, less<const char*> > StringMap;

class CFormatParameter
{
private:
	Formattable* m_MsgArgs;
	int paraNum;
private:
	CFormatParameter(){}
public:
	CFormatParameter(int paraNum);
	~CFormatParameter();

	Formattable* GetMsgArgs(){ return m_MsgArgs;} 
	int GetParaNum() { return paraNum; }
	void setDouble(int index, double d);
	void  setLong(int index, int32_t l);
	void  setInt64(int index, int64_t ll) ;
	void  setDate(int index, UDate d) ;
	void  setString(int index, const UnicodeString &stringToCopy);
};

class CResourceBundleManage
{

public:
	CResourceBundleManage(const char* configFileName);
	virtual ~CResourceBundleManage(void);

private:
	std::unique_ptr<char[]> m_ResDir{};
	std::unique_ptr<char[]> m_ResLocale{};

	StringMap mapRes;
	ResourceBundle* m_pResourceBundle;
	UConverter *m_pConverter;
	int m_MaxSize;

	int m_LogFlag;

	FILE* m_LogFile;

private:
	UErrorCode ToCodePageString(UConverter *conv, UChar* source, char* target, int destCapacity, int& len);
	bool Init();

public:
	int GetSize(void);			// 取得资源个数
	
	void Release(void);

	const char* LoadResString(const char* key);
	UnicodeString LoadUResString(const char* key);

	int Format(const char* key, CFormatParameter& parameter, char buffer[]);
};
