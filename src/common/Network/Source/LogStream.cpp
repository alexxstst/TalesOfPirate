
#include <direct.h>
#include "LogStream.h"

_DBC_USING
extern	bool	g_logautobak;
static RunBiDirectChain<LogStream>	*s_logmgr=0;
LogStream	dbc::g_dbclog("comm");

LogStream::LogStream(const char *LogFileName,const char *DirName)
:m_line(false),m_logline(0)
{
	if(!LogFileName ||!*LogFileName)
	{
		throw "Log file's name can't null";
	}
	m_LogFileName	=LogFileName;
	DirName			=(DirName&&*DirName)?DirName:"log";
	m_DirName		="logfile";
	if(*DirName =='/' ||*DirName =='\\')
	{
		m_DirName<<DirName;
	}else
	{
		m_DirName<<"/"<<DirName;
	}

	Open(g_logautobak);
	if(m_ofs.is_open())
	{
		LogLine l_line(*this);
		l_line<<newln<<"Log stream open success.";
	}else
	{
		std::cout<<"write file failed, log file ["<<LogFileName<<"] closed"<<std::endl;
	}
	if(!s_logmgr)
	{
		s_logmgr	=new RunBiDirectChain<LogStream>;
	}
	_BeginRun(s_logmgr);
}
LogStream::~LogStream()
{
	if(_EndRun() ==1)
	{
		delete s_logmgr;
		s_logmgr	=0;
	}
	LogLine l_line(*this);
	l_line<<newln<<"Log stream close success.";
}
void LogStream::Open(bool bakmode)
{
	char l_path[500];
	l_path[0] =0;
	if(m_DirName || strlen(m_DirName)>0)
	{
		if(*m_DirName =='/' ||*m_DirName =='\\')
		{
            strcpy(l_path,m_DirName+1);
		}else
		{
            strcpy(l_path,m_DirName);
		}
		int		l_dircount	=0;
		cChar	*l_dir =l_path;
		char	*l_dim1,*l_dim2,*l_dim;
		while( (l_dim1 =(char*)strchr(l_dir,'/')),(l_dim2 =(char*)strchr(l_dir,'\\')),(l_dim =min(l_dim1,l_dim2)),
				(l_dim =(l_dim?l_dim:max(l_dim1,l_dim2)))
				)
		{
			*l_dim	=0;
			if(strlen(l_dir) >0)
			{
				if(l_dircount ==1)
				{
					char l_olddir[500];
					char l_frmstr[300];
					strcpy(l_frmstr,l_path);
					strcat(l_frmstr,"%04d-%02d-%02d %02d.%02d.%02d.%03d");
					SYSTEMTIME l_st;
					::GetLocalTime(&l_st);
					sprintf(l_olddir,l_frmstr,l_st.wYear,l_st.wMonth,l_st.wDay,l_st.wHour,l_st.wMinute,l_st.wSecond,l_st.wMilliseconds);

					if(bakmode)
					{
						MoveFile(l_path,l_olddir);
					}
					m_DirRoot	=l_path;
				}
				_mkdir(l_path);
				l_dircount++;
			}
			*l_dim	=(l_dim ==l_dim1)?'/':'\\';
			l_dir	=l_dim +1;
		}
		if(strlen(l_dir) >0)
		{
			if(l_dircount ==1)
			{
				char l_olddir[500];
				char l_frmstr[300];
				strcpy(l_frmstr,l_path);
				strcat(l_frmstr,"%04d-%02d-%02d %02d.%02d.%02d.%03d");
				SYSTEMTIME l_st;
				::GetLocalTime(&l_st);
				sprintf(l_olddir,l_frmstr,l_st.wYear,l_st.wMonth,l_st.wDay,l_st.wHour,l_st.wMinute,l_st.wSecond,l_st.wMilliseconds);

				if(bakmode)
				{
					MoveFile(l_path,l_olddir);
				}
				m_DirRoot	=l_path;
			}
			_mkdir(l_path);
			l_dircount++;
		}
		strcat(l_path,"/");
	}
	strcat(l_path,m_LogFileName);
	strcat(l_path,".log");
	if(bakmode)
	{
		m_ofs.open(l_path,std::ios::out|std::ios::app);
	}else
	{
		m_ofs.open(l_path,std::ios::out);
	}
}
void LogStream::Backup()
{
	char l_bakdir[500];
	char l_frmstr[500];
	strcpy(l_frmstr,"%04d-%02d-%02d %02d.%02d.%02d.bak");
	SYSTEMTIME l_st;
	::GetLocalTime(&l_st);
	sprintf(l_bakdir,l_frmstr,l_st.wYear,l_st.wMonth,l_st.wDay,l_st.wHour,l_st.wMinute,l_st.wSecond);

	if(!s_logmgr)return;
	LogStream	*l_logs;
	{
		RunChainGetArmor<LogStream> l_lockMgr(*s_logmgr);
		while(l_logs	=s_logmgr->GetNextItem())
		{
			l_logs->m_mtxline.lock();
			{
				LogLine	l_line(*l_logs);
				l_line<<newln<<"Backup:log stream close success.";
			}
			l_logs->m_ofs.close();
		}
		l_lockMgr.unlock();
	}
	try{
		RunChainGetArmor<LogStream> l_lockMgr(*s_logmgr);
		while(l_logs	=s_logmgr->GetNextItem())
		{
			MoveFile(l_logs->m_DirRoot,l_logs->m_DirRoot + l_bakdir);
		}
		l_lockMgr.unlock();
	}catch(...){}
	{
		RunChainGetArmor<LogStream> l_lockMgr(*s_logmgr);
		while(l_logs	=s_logmgr->GetNextItem())
		{
			l_logs->Open();
			if(l_logs->m_ofs.is_open())
			{
				LogLine l_line(*l_logs);
				l_line<<newln<<"Backup:log stream oepn success.";
			}else
			{
				std::cout<<"Backup:write file failed, log file ["<<l_logs->m_LogFileName<<"] close"<<std::endl;
			}
			l_logs->m_mtxline.unlock();
		}
		l_lockMgr.unlock();
	}
}
void LogStream::beginln()
{
	SYSTEMTIME st; char time[100];
	::GetLocalTime(&st);
	sprintf(time, "[%02d-%02d %02d:%02d:%02d]", st.wMonth, st.wDay, st.wHour,st.wMinute, st.wSecond);
	m_mtxline.lock();
	if(m_line)
	{
		m_ofs<<std::endl;
	}
	m_line	=true;

	m_ofs<<time;
}

void LogStream::endln()
{
	m_ofs<<std::endl;
	m_line	=false;
	m_mtxline.unlock();
}
LogLine::~LogLine(){*this<<endln;}

_DBC_BEGIN

std::ofstream & newln(LogLine & line)
{
	endln(line);
	line.m_logstream.beginln();
	line.m_logstream.m_logline	=&line;
	line.m_line	=true;
	return line.m_logstream.m_ofs;
}
std::ofstream & endln(LogLine & line)
{
	if(line.m_line)
	{
		line.m_line =false;
		line.m_logstream.m_logline	=0;
		line.m_logstream.endln();
	}
	return line.m_logstream.m_ofs;
}
std::ostream  & endln(std::ostream & of)
{
	try{
		std::ofstream *l_ofs =static_cast<std::ofstream *>(&of);
		LogStream * l_ls	=reinterpret_cast<LogStream *>(CONTAINING_RECORD(l_ofs,LogStream,m_ofs));
		if(l_ls->m_logline)
		{
			endln(*(l_ls->m_logline));
		}
	}catch(...){std::cout<<"log endln exception"<<std::endl;}
	return of;
}


_DBC_END









