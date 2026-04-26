#ifndef MPDataStream_H
#define MPDataStream_H

#include "MPEffPrerequisites.h"
#include "MPSharedPtr.h"

/** .
@remarks
This class performs basically the same tasks as std::basic_istream, 
except that it does not have any formatting capabilities, and is
designed to be subclassed to receive data from multiple sources,
including libraries which have no compatiblity with the STL's
stream interfaces. As such, this is an abstraction of a set of 
wrapper classes which pretend to be standard stream classes but 
can actually be implemented quite differently. 
@par
	ArchiveFactory
ArchiveMPDataStream
@note
	

*/
class MPDataStream {
public:
	MPDataStream() : m_Size(0) {
	}

	MPDataStream(const String& name) : m_Name(name), m_Size(0) {
	}

	virtual ~MPDataStream() {
	}

	template <typename T>
	MPDataStream& operator>>(T& val);
	/** , eof.
	@param buf 
	@param count 
	@returns 
	*/
	virtual size_t read(void* buf, size_t count) = 0;
	/** .
	@remarks
		,
	..
	@param buf 
	@param maxCount 
	@param delim 
	@returns 
	*/
	virtual size_t readLine(char* buf, size_t maxCount, const String& delim = "\n") = 0;

	/** Returns a String containing the next line of data, optionally 
	trimmed for whitespace. 
	@remarks
	This is a convenience method for text streams only, allowing you to 
	retrieve a String object containing the next line of data. The data
	is read up to the next newline character and the result trimmed if
	required.
	@param 
	trimAfter If true, the line is trimmed for whitespace (as in 
	String.trim(true,true))
	*/
	virtual String getLine(bool trimAfter = true);

	/** . */
	virtual String getAsString(void);

	/** .
	@param delim 
	@returns 
	*/
	virtual size_t skipLine(const String& delim = "\n") = 0;

	/** ,. */
	virtual void skip(long count) = 0;

	/** .
	*/
	virtual void seek(size_t pos) = 0;

	/**  */
	virtual size_t tell(void) const = 0;

	/** eoftrue.*/
	virtual bool eof(void) const = 0;

	/** . */
	virtual void close(void) = 0;

	/** 0. */
	size_t size(void) const {
		return m_Size;
	}

	/// .
	const String& getName(void) const {
		return m_Name;
	}

protected:
	/// ()
	String m_Name;
	/// (0)
	size_t m_Size;
#define MP_STREAM_TEMP_SIZE 128
	char m_TmpArea[MP_STREAM_TEMP_SIZE];
};

typedef MPSharedPtr<MPDataStream> MPDataStreamPtr;
typedef std::list<MPDataStreamPtr> MPDataStreamList;
typedef MPSharedPtr<MPDataStreamList> MPDataStreamListPtr;

/** DataStream.*/
class MPMemoryDataStream : public MPDataStream {
public:
	/** .
	@param pMem 
	@param size 
	@param freeOnClose true.
	*/
	MPMemoryDataStream(void* pMem, size_t size, bool freeOnClose = false);
	/** .
	@param name 
	@param pMem 
	@param size 
	@param freeOnClose true.
	*/
	MPMemoryDataStream(const String& name, void* pMem, size_t size,
					   bool freeOnClose = false);
	/** .
	@param sourceStream 
	@param freeOnClose true.
	*/
	MPMemoryDataStream(MPDataStream& sourceStream, bool freeOnClose = true);
	/** .
	@param sourceStream ()
	@param freeOnClose true.
	*/
	MPMemoryDataStream(MPDataStreamPtr& sourceStream, bool freeOnClose = true);
	/** .
	@param name 
	@param sourceStream 
	@param freeOnClose true.
	*/
	MPMemoryDataStream(const String& name, MPDataStream& sourceStream,
					   bool freeOnClose = true);
	/** .
	@param name 
	@param sourceStream ()
	@param freeOnClose true.
	*/
	MPMemoryDataStream(const String& name, const MPDataStreamPtr& sourceStream,
					   bool freeOnClose = true);
	/** .
	@param size 
	@param freeOnClose true.
	*/
	MPMemoryDataStream(size_t size, bool freeOnClose = true);
	/** .
	@param name 
	@param size 
	@param freeOnClose true.
	*/
	MPMemoryDataStream(const String& name, size_t size, bool freeOnClose = true);

	~MPMemoryDataStream();

	/** . */
	uchar* getPtr(void) {
		return m_Data;
	}

	/** . */
	uchar* getCurrentPtr(void) {
		return m_Pos;
	}

	/** . */
	void setFreeOnClose(bool free) {
		m_FreeOnClose = free;
	}

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek(size_t pos);
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	uchar* m_Data;
	uchar* m_Pos;
	uchar* m_End;
	bool m_FreeOnClose;
};

typedef MPSharedPtr<MPMemoryDataStream> MPMemoryDataStreamPtr;

/** DataStream,std::basic_istream.*/
class MPFileStreamDataStream : public MPDataStream {
public:
	/** stl
	@param s stl
	@param freeOnClose true.
	*/
	MPFileStreamDataStream(std::ifstream* s, bool freeOnClose = true);
	/** stl
	@param name 
	@param s stl
	@param freeOnClose true.
	*/
	MPFileStreamDataStream(const String& name, std::ifstream* s, bool freeOnClose = true);
	/** stl, 
	@remarks
	This variant tells the class the size of the stream too, which 
	means this class does not need to seek to the end of the stream 
	to determine the size up-front. This can be beneficial if you have
	metadata about the contents of the stream already.
	@param name 
	@param s stl
	@param size 
	@param freeOnClose true.
	*/
	MPFileStreamDataStream(const String& name, std::ifstream* s, size_t size, bool freeOnClose = true);

	~MPFileStreamDataStream();

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek(size_t pos);
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	std::ifstream* m_pStream;
	bool m_FreeOnClose;
};

/** DataStream,C.*/
class MPFileHandleDataStream : public MPDataStream {
public:
	/// c
	MPFileHandleDataStream(FILE* handle);
	/// c
	MPFileHandleDataStream(const String& name, FILE* handle);
	~MPFileHandleDataStream();

	/** @copydoc DataStream::read		*/
	size_t read(void* buf, size_t count);
	/** @copydoc DataStream::readLine	*/
	size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
	/** @copydoc DataStream::skipLine	*/
	size_t skipLine(const String& delim = "\n");
	/** @copydoc DataStream::skip		*/
	void skip(long count);
	/** @copydoc DataStream::seek		*/
	void seek(size_t pos);
	/** @copydoc DataStream::tell		*/
	size_t tell(void) const;
	/** @copydoc DataStream::eof		*/
	bool eof(void) const;
	/** @copydoc DataStream::close		*/
	void close(void);

protected:
	FILE* m_FileHandle;
};


#endif
