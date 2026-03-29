#include "pi_Alloc.h"

#include <iostream>
#include <fstream>

using namespace std;

static int s_numBlocks = 0;
static size_t s_numBytes = 0;

static void oneMoreBlock(size_t bytes)
{
	++s_numBlocks;
	s_numBytes += bytes;
}

static void oneLessBlock(size_t bytes)
{
	--s_numBlocks;
	s_numBytes -= bytes;
}

int pi_Alloc::numBlocks()
{
	return s_numBlocks;
}

int pi_Alloc::numBytes()
{
	return s_numBytes;
}

union Align
{
	struct
	{
		int d_magic;
		int d_bytes;
	} d_data;
	long int d_longInt;
	long double d_longDouble;
	char* d_pointer;
};

enum
{
	ALLOCAED_MEMORY = 0xA110CAED,
	DEALLOCATED_MEMORY = ~ALLOCAED_MEMORY
};

#ifdef LOG_POSITION
void* pi_Alloc::allocate(size_t size, pi_Alloc *, const char* fileName, int lineNo)
#else
void* pi_Alloc::allocate(size_t size, pi_Alloc *)
#endif
{
	Align* align = (Align*) new char[sizeof(Align) + size];
	align->d_data.d_magic = ALLOCAED_MEMORY;
	align->d_data.d_bytes = (int)size;

	void *addr = ++align;
	oneMoreBlock(size);

#ifdef LOG_POSITION
	pi_leakReporter.addItem(addr, fileName, lineNo, size);
#endif
	return addr;
}

static void report(const void* addr, const char* gerund)
{
	cerr << "po: PROGRAMMING ERROR -- " << gerund << " ";
	if(!addr)
		cerr << "null memory address";
	else
	{
		Align *align = (Align*) addr;
		if(ALLOCAED_MEMORY == align->d_data.d_magic)
			cerr << "previously deallocated object";
		else
			cerr << "unallocated memory address";
	}

	cerr << endl;
}

void pi_Alloc::assertValid(void *addr)
{
	if(!isAllocated(addr))
		report(addr, "using");
}

void pi_Alloc::deallocate(void* addr)
{
	if(!isAllocated(addr))
	{
		report(addr, "deallocating");
		return;
	}

	Align* align = (Align*)addr;
	int size = align[-1].d_data.d_bytes;
	oneLessBlock(size);
	align[-1].d_data.d_magic = DEALLOCATED_MEMORY;

#ifdef LOG_POSITION
	pi_leakReporter.removeItem(addr);
#endif

	delete [] (char*) --align;
}

int pi_Alloc::isAllocated(void *addr)
{
	if(!addr)
		return 0;

	Align* align = (Align*)addr;
	return (ALLOCAED_MEMORY == align[-1].d_data.d_magic);
}

pi_LeakReporter::pi_LeakReporter(const char* fileName)
{
	pi_fileName = fileName;
}

pi_LeakReporter::~pi_LeakReporter(void)
{
	int b = pi_Alloc::numBlocks();
	int y = pi_Alloc::numBytes();

	if(b>0 || y>0)
	{
		cerr << "pi :MEMORY LEAK --"
			<< b << " block" << (b!=1?"s":"") << ","
			<< y << " byte" << (y!=1?"s":"") << endl;
	}

	dumpReport();
}

void pi_LeakReporter::addItem(void* addr, const char* fileName, int lineNo, size_t size)
{
	pi_LeakItem item;
	item.addr = (int)addr;

	// CRT 7.1 
#if (_MSC_VER > 1400) 
	strcpy_s(item.fileName, sizeof(item.fileName), fileName);
#else
	strncpy(item.fileName, fileName, sizeof(item.fileName));
#endif
	item.d_lineNo = lineNo;
	item.d_size = size;

	pi_leakList.push_back(item);	
}

void pi_LeakReporter::removeItem(void* addr)
{
	for(pi_LeakList::iterator it = pi_leakList.begin(); it!=pi_leakList.end();it++)
	{
		if(it->addr == (int)addr)
		{
			pi_leakList.erase(it);
			break;
		}
	}
}

void pi_LeakReporter::dumpReport()
{
	if(pi_leakReporter.pi_leakList.size() > 0)
	{
#ifdef LOG_POSITION
		cout << "memory leak begin" << endl;

		int i=0;

		for(pi_LeakList::iterator it = pi_leakReporter.pi_leakList.begin(); it!=pi_leakReporter.pi_leakList.end();it++, i++)
		{
			cout << "\t" << (i+1) << ":" << it->fileName<< " size=" << ((it->d_size > 1024) ? (it->d_size/1024.0) : it->d_size) 
				<< ((it->d_size > 1024) ? " kbytes (" : " bytes(") << it->d_lineNo << ")" << endl;
		}

		cout << "memory leak end" << endl;
#else

#endif

		//cout << "save to file (c:\\memoryleak.log)?(Y/N)" << endl;
		//char key;
		//cin >> key;

		//if(key == 'y' || key == 'Y')
		{
			try
			{
				fstream outputFile;

				outputFile.open(pi_fileName.c_str(), ios::out );
				outputFile << "memory leak begin" << endl;

				int i=0;

				for(pi_LeakList::iterator it = pi_leakReporter.pi_leakList.begin(); it!=pi_leakReporter.pi_leakList.end();it++, i++)
				{
					outputFile << "\t" << (i+1) << ":" << it->fileName<< " size=" << ((it->d_size > 1024) ? (it->d_size/1024.0) : it->d_size) 
						<< ((it->d_size > 1024) ? " kbytes (" : " bytes(") << it->d_lineNo << ")" << endl;
				}

				outputFile << "memory leak end" << endl;

				outputFile.close();
			}
			catch(...)
			{

			}
		}
	}
}
