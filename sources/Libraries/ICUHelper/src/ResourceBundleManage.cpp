#include "resourcebundlemanage.h"

#include "windows.h"
#include <string>
#include <memory>
#include "logutil.h"

using namespace std;

#include "pi_Alloc.h"

#define BUFFER_SIZE 255

class CLogLock
{
public:
	CLogLock() { InitializeCriticalSection(&_cs); }
	~CLogLock() { DeleteCriticalSection(&_cs); }

	void Lock() { EnterCriticalSection(&_cs); }
	void Unlock() { LeaveCriticalSection(&_cs); }

private:
    CRITICAL_SECTION _cs;
};

CFormatParameter::CFormatParameter(int paraNum)
{
	UErrorCode status = U_ZERO_ERROR;
	m_MsgArgs = new Formattable[paraNum];
	this->paraNum = paraNum;
}

CFormatParameter::~CFormatParameter()
{
	if(m_MsgArgs)
	{
		delete[] m_MsgArgs;
		m_MsgArgs = NULL;
	}
}

void CFormatParameter::setDouble(int index, double d)
{
	m_MsgArgs[index].setDouble(d);
}

void CFormatParameter::setLong(int index, int32_t l)
{
	m_MsgArgs[index].setLong(l);
}

void CFormatParameter::setInt64(int index, int64_t ll) 
{
	m_MsgArgs[index].setInt64(ll);
}

void CFormatParameter::setDate(int index, UDate d) 
{
	m_MsgArgs[index].setDate(d);
}

void CFormatParameter::setString(int index, const UnicodeString &stringToCopy)
{
	m_MsgArgs[index].setString(stringToCopy);
}

CResourceBundleManage::CResourceBundleManage(const char* configFileName)
:  m_pResourceBundle(NULL), m_pConverter(NULL), m_MaxSize(0), m_LogFile(NULL), m_LogFlag(0)
{
    char szPath[MAX_PATH]; 
    
    if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
    {
        throw "GetModuleFileName failed \n"; 
    }
	// ȡ����ڳ����·��
	const char* ret = strrchr(szPath, '\\');
	long dirLength = (long)(ret - szPath);

	size_t length = strlen(configFileName);
	//char* fileName = new char[dirLength + length + 2 ];
	auto fileName = std::make_unique<char[]>(dirLength + length + 2);
	memcpy(fileName.get(), szPath, dirLength);
	fileName[dirLength] = '\\';
	memcpy(fileName.get() + dirLength + 1, configFileName, length);
	fileName[dirLength + length + 1] = 0;

	UErrorCode status;
	Locale::setDefault(Locale::getEnglish(),status);

	const char* lpAppName = "locale";
	const char* lpLocaleKeyName = "locale";
	const char* lpPathKeyName = "path";
	const char* lpDefaultLocale = "en_US";
	//char* lpDefaultPath = new char[dirLength + 1];
	auto lpDefaultPath = std::make_unique<char[]>(dirLength + 1);
	memcpy(lpDefaultPath.get(), szPath, dirLength);
	lpDefaultPath[dirLength] = 0;

	char lpReturnedLocaleString[BUFFER_SIZE];
	char lpReturnedPathString[BUFFER_SIZE];

	length = GetPrivateProfileStringA(lpAppName,lpLocaleKeyName,lpDefaultLocale,lpReturnedLocaleString,sizeof(lpReturnedLocaleString),fileName.get());

	m_ResLocale = std::make_unique<char[]>(length + 1);
	memcpy(m_ResLocale.get(), lpReturnedLocaleString, length);
	m_ResLocale[length] = 0;

	length = GetPrivateProfileStringA(lpAppName,lpPathKeyName,lpDefaultPath.get(),lpReturnedPathString,sizeof(lpReturnedPathString),fileName.get());

	
	m_ResDir = make_unique<char[]>(length + 1);
	memcpy(m_ResDir.get(), lpReturnedPathString, length);
	m_ResDir[length] = 0;

	m_LogFlag = GetPrivateProfileInt(lpAppName, "log", 0 ,fileName.get());

	if(m_LogFlag)
	{
		string logFileName = string(m_ResDir.get()) + "\\res.log";
		
		m_LogFile = fopen(logFileName.c_str(), "w+");
	}

	status = U_ZERO_ERROR;
	m_pConverter = ucnv_open(NULL, &status); //ucnv_open("cp936", &status); // windows-936-2000
	
	if(status == U_ZERO_ERROR)
	{

	}
	else if(status == U_MEMORY_ALLOCATION_ERROR)
		::MessageBox(NULL,"Memory allocation error",NULL,0);
	else if(status == U_FILE_ACCESS_ERROR)
		::MessageBox(NULL,"The requested file cannot be found",NULL,0);
	else
		::MessageBox(NULL,"Other error!",NULL,0);

	m_MaxSize = ucnv_getMaxCharSize(m_pConverter);

	m_pResourceBundle = new ResourceBundle(m_ResDir.get(),m_ResLocale.get(), status);

	Init();
}

CResourceBundleManage::~CResourceBundleManage(void)
{
	Release();
}

// ȡ����Դ����
int CResourceBundleManage::GetSize(void)
{
	return (int)mapRes.size();
}

// ��Unicode�ַ���ת��Ϊ���ֽڱ�������
UErrorCode CResourceBundleManage::ToCodePageString(UConverter *conv, UChar* source, char* target, int destCapacity, int& len)
{
  UErrorCode status = U_ZERO_ERROR;

  len = ucnv_fromUChars(conv, target, destCapacity, source, -1, &status);

  if(U_SUCCESS(status) == FALSE)
	  return status;

  return status;
}

// ��ʼ����Դ������
bool CResourceBundleManage::Init()
{
	int len = 0;
	int maxSize = 0;

	UErrorCode status = U_ZERO_ERROR;

#if _DEBUG

	const char* name = ucnv_getName(m_pConverter, &status);
	// Заменено printf → логирование
	ToLogService("common", "Current CodePage is {}", name);

#endif

	status = U_ZERO_ERROR;

	if(U_SUCCESS(status) == TRUE)
	{
		m_pResourceBundle->resetIterator();
		
        while(m_pResourceBundle->hasNext()) {
            ResourceBundle bundle = m_pResourceBundle->getNext(status);

			const char * key = bundle.getKey();

			UnicodeString value = bundle.getString(status);

			len = 0;
			maxSize = m_MaxSize * ( 1 + value.length());

			auto buffer = std::make_unique<char[]>(maxSize);
			memset(buffer.get(), 0, maxSize);

			ToCodePageString(m_pConverter, const_cast<UChar*>(value.getTerminatedBuffer()), buffer.get(), maxSize, len);

			mapRes[key] = std::move(buffer);
		}
	
		return true;
	}

	return false;
}

// �ͷ���Դ
void CResourceBundleManage::Release(void)
{
	m_ResDir.reset(nullptr);
	m_ResLocale.reset(nullptr);
	mapRes.clear();

	delete m_pResourceBundle;

	if (m_pConverter != NULL)
		ucnv_close(m_pConverter);

	if(m_LogFlag)
	{
		if(m_LogFile)
		{
			fclose(m_LogFile);
			m_LogFile = NULL;
		}
	}
}

// ����IDȡ���ַ���
const char* CResourceBundleManage::LoadResString(const char* key)
{
	const char* ret ="";
	StringMap::iterator in = mapRes.find(key);

	if(in != mapRes.end())
	{
		ret = in->second.get();
	}
#if _DEBUG
#endif

	static CLogLock lock;
	lock.Lock();
	if(m_LogFlag)
	{
		if(m_LogFile)
		{
			fprintf(m_LogFile, "Key = [%s] Value = %s\r\n", key, ret);
		}
	}
	lock.Unlock();

	return ret;
}
 
UnicodeString CResourceBundleManage::LoadUResString(const char* key)
{
	UErrorCode status = U_ZERO_ERROR;
	return m_pResourceBundle->getStringEx(key, status);
}

// ��ʽ���ݲ�����һ���ַ���
int CResourceBundleManage::Format(const char* key, CFormatParameter& parameter, char buffer[])
{
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString str;
    FieldPosition pos;

	UnicodeString format = LoadUResString(key);

	// Create a message format
    MessageFormat msg(format, status);
    msg.format(parameter.GetMsgArgs(), parameter.GetParaNum(), str, pos, status);

	int aaa = m_MaxSize;

	int len = 0;
	int maxSize = m_MaxSize * ( 1 + str.length());

	ToCodePageString(m_pConverter, const_cast<UChar*>(str.getTerminatedBuffer()), buffer, maxSize, len);

	return len;
}