#include "stdafx.h"
#include ".\chatrecord.h"

using namespace std;

string CChatRecord::m_strPath="";


CChatRecord::CChatRecord(void)
{
}

CChatRecord::~CChatRecord(void)
{
}

bool CChatRecord::Save(const string name, DWORD number, const string chatData)
{
	if (name.length()==0 || chatData.length()==0) return false;
	__time64_t t;
	_time64(&t);
	tm *ltime=_gmtime64(&t);
	char buf[_MAX_PATH+1];
	char folder[_MAX_PATH+1];
	sprintf(folder,"chats\\%d-%d-%d",ltime->tm_year+1900,ltime->tm_mon+1,ltime->tm_mday);
	CreateDirectory("chats",NULL);
	CreateDirectory(folder,NULL);
	ofstream chatLog;
	// �ı䱣�������¼��ʽ  modify by ning.yan  20080725 Begin
	//int i=1;
	//for (;i<1000;i++)
	//{
	//	m_strPath=folder+string(buf);
	//	if (access(m_strPath.c_str(),0)!=-1) continue;
	//	chatLog.open(m_strPath.c_str(),ios::out);
	//	if (chatLog.is_open()) break;
	//}
	//if (i>=1000)
	//{
	//	return false;
	//}
	sprintf(buf,"\\%s.txt",name.c_str());
	m_strPath=folder+string(buf);
	chatLog.open(m_strPath.c_str(),ios_base::app); // �Ը���ģʽ���ļ����½��ļ�������д���ļ�β��
	// End
	chatLog<<chatData.c_str();
	chatLog.close();
	return true;
}

string CChatRecord::GetLastSavePath()
{
	return m_strPath;
}
