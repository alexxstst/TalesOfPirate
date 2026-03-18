#pragma once

#include "DBCCommon.h"
#include "RunBiDirectChain.h"

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

_DBC_BEGIN
#pragma pack(push)
#pragma pack(4)
class LogLine;
//======================================================================
class LogStream: public RunBiDirectItem<LogStream>
{
	friend std::ofstream & newln(LogLine & line);
	friend std::ofstream & endln(LogLine & line);
	friend std::ostream  & endln(std::ostream & of);
public:
	LogStream(const char *LogFileName,const char *DirName ="log");
	~LogStream();
	static void	Backup();
	bool is_open() const{return m_ofs.is_open();}
private:
	void beginln();
	void endln();

	void Free(){}
	void Open(bool bakmode =true);

	std::string				m_LogFileName;
	std::string				m_DirName;
	std::string				m_DirRoot;
	std::recursive_mutex				m_mtxline;
	bool	volatile	m_line;
	LogLine			*	m_logline;
	std::ofstream		m_ofs;
};
//======================================================================
class LogLine
{
	friend std::ofstream & newln(LogLine & line);
	friend std::ofstream & endln(LogLine & line);
public:
	LogLine(LogStream & logstream):m_line(false),m_logstream(logstream){}
	~LogLine();
private:
	bool			m_line;
	LogStream	&	m_logstream;
};
//======================================================================
typedef std::ofstream & (* FOper)(LogLine & line);
inline std::ofstream & operator<<(LogLine & line,FOper f)
{
	return (*f)(line);
}
std::ofstream & newln(LogLine & line);
std::ofstream & endln(LogLine & line);
std::ostream  & endln(std::ostream & of);





extern LogStream g_dbclog;

#pragma pack(pop)
_DBC_END

