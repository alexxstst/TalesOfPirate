#pragma once

#ifndef _INC_STDDEF
#include <stddef.h>	// declares size_t
#endif

#ifndef _INC_STDLIB
#include <cstdlib>	// declares _MAX_PATH
#endif

#include <list>
#include <string>

#define LOG_POSITION

#ifdef cnew
#undef cnew
#endif

#ifdef cdelete
#undef cdelete
#endif

#ifdef LOG_POSITION
#define cnew new(pi_Alloc::allocate,  __FILE__, __LINE__)

#define cdelete(pointer) (pi_Alloc::deallocate((void*)pointer))
#define cdestroy(classname, pointer) (##classname::destroy##classname(pointer))

#define IMPLEMENT_CDELETE(classname)\
	public:\
		static inline void destroy##classname(##classname* p##classname)\
		{\
			if(pi_Alloc::isAllocated(p##classname))\
			{\
				p##classname->~##classname();\
			}\
			pi_Alloc::deallocate(p##classname);\
		}
#else
#define cnew new

#define cdelete(pointer) (delete pointer)
#define cdestroy(classname, pointer) (delete pointer)

#define IMPLEMENT_CDELETE(classname)
#endif

class pi_Alloc
{
	pi_Alloc(void);
public:
#ifdef LOG_POSITION
	static void *allocate(size_t size, pi_Alloc* dummy, const char* fileName, int lineNo);
#else
	static void *allocate(size_t size, pi_Alloc* dummy);
#endif
	static void assertValid(void* addr);
	static void deallocate(void* addr);
	static int isAllocated(void* addr);
	static int numBlocks();
	static int numBytes();
};

#ifdef LOG_POSITION
inline void *operator new(size_t size, void *(*f)(size_t, pi_Alloc*, const char*, int), const char* fileName = __FILE__, int lineNo = __LINE__)
{
	return (*f)(size, (pi_Alloc *)0, fileName, lineNo);
}

inline void *operator new[](size_t size, void *(*f)(size_t, pi_Alloc*, const char*, int), const char* fileName = __FILE__, int lineNo = __LINE__)
{
	return (*f)(size, (pi_Alloc *)0, fileName, lineNo);
}
#else
inline void *operator new(size_t size, void *(*f)(int, pi_Alloc*))
{
	return (*f)(size, (pi_Alloc *)0);
}

inline void *operator new[](size_t size, void *(*f)(int, pi_Alloc*))
{
	return (*f)(size, (pi_Alloc *)0);
}
#endif

class pi_LeakReporter
{
public:
	typedef struct pi_LeakItem
	{
		int addr;
		char fileName[_MAX_PATH];
		int d_lineNo;
		size_t d_size;
	};
	
	typedef std::list< pi_LeakItem > pi_LeakList;
	pi_LeakList pi_leakList;
	std::string pi_fileName;

	void addItem(void* addr, const char* fileName, int lineNo, size_t size);
	void removeItem(void* addr);

	void dumpReport();

	pi_LeakReporter(const char* fileName = "memoryleak.log");
	~pi_LeakReporter(void);
};

extern pi_LeakReporter pi_leakReporter;
