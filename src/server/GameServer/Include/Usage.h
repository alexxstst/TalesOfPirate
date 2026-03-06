#pragma once


#ifndef USAGE_H
#define USAGE_H

class Usage{
public:
	Usage():m_cpusage(0){}
	void Add(const long usage){
		try{
			m_cpusage +=usage;
		}catch(...){}
	}
	long Get(){
		long l_retval;
		try{
			l_retval	=m_cpusage;
			m_cpusage	=0;
		}catch(...){}
		return l_retval;
	}
private:
	long 		m_cpusage;
};

#endif
