#ifndef MPSharedPtr_H
#define MPSharedPtr_H

#include "MPEffPrerequisites.h"

template <class T> class MPSharedPtr
{
protected:
	T* m_pRep;
	unsigned int* m_pUseCount;

public:
	MP_AUTO_SHARED_MUTEX; // 

	/** SharedPtr
	@remarks
	<b>!</b> SharedPtrbind() .
	*/
	MPSharedPtr() : m_pRep(0), m_pUseCount(0) {}

	explicit MPSharedPtr(T* rep) : m_pRep(rep), m_pUseCount(new unsigned int(1)) 
	{
		MP_NEW_AUTO_SHARED_MUTEX
	}

	MPSharedPtr(const MPSharedPtr& r) 
	{
		// mutex
		MP_LOCK_MUTEX(*(r.MP_AUTO_MUTEX_NAME));
		MP_COPY_AUTO_SHARED_MUTEX(r.MP_AUTO_MUTEX_NAME);

		m_pRep = r.m_pRep;
		m_pUseCount = r.m_pUseCount; 

		if(m_pUseCount)
		{
			++(*m_pUseCount); 
		}
	}

	MPSharedPtr& operator=(const MPSharedPtr& r) {
		if (m_pRep == r.m_pRep)
			return *this;
		release();

		// mutex
		MP_LOCK_MUTEX(*(r.MP_AUTO_MUTEX_NAME));
		MP_COPY_AUTO_SHARED_MUTEX(r.MP_AUTO_MUTEX_NAME);

		m_pRep = r.m_pRep;
		m_pUseCount = r.m_pUseCount;
		if (m_pUseCount)
		{
			++(*m_pUseCount);
		}
		return *this;
	}

	virtual ~MPSharedPtr() {
		release();
	}

	inline T& operator*() const { assert(m_pRep); return *m_pRep; }
	inline T* operator->() const { assert(m_pRep); return m_pRep; }
	inline T* get() const { return m_pRep; }

	/** SharedPtr.
	@remarks
		SharedPtr!
	*/
	void bind(T* rep) {
		assert(!pRep && !pUseCount);
		MP_NEW_AUTO_SHARED_MUTEX;
		MP_LOCK_AUTO_SHARED_MUTEX;

		m_pUseCount = new unsigned int(1);
		m_pRep = rep;
	}

	inline bool unique() const { assert(m_pUseCount); MP_LOCK_AUTO_SHARED_MUTEX; return (*m_pUseCount) == 1; }
	inline unsigned int useCount() const { assert(m_pUseCount); MP_LOCK_AUTO_SHARED_MUTEX return *m_pUseCount; }
	inline unsigned int* useCountPointer() const { return m_pUseCount; }

	inline T* getPointer() const { return m_pRep; }

	inline bool isNull(void) const { return m_pRep == 0; }

	inline void setNull(void) { 
		if (m_pRep)
		{
			// 
			release();
			m_pRep = 0;
			m_pUseCount = 0;
			MP_COPY_AUTO_SHARED_MUTEX(0)
		}
	}

protected:

	inline void release(void) 
	{
		bool destroyThis = false;
		{
			// mutex()
			MP_LOCK_AUTO_SHARED_MUTEX;

			if (m_pUseCount)
			{
				if (--(*m_pUseCount) == 0) 
				{
					destroyThis = true;
				}
			}
			// boost
		}
		if (destroyThis)
			destroy();
	}

	virtual void destroy(void)
	{
		// 
		// setNull(),
		delete m_pRep;
		delete m_pUseCount;
		MP_DELETE_AUTO_SHARED_MUTEX;
	}

};

template<class T, class U> inline bool operator==(MPSharedPtr<T> const& a, MPSharedPtr<U> const& b)
{
	return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(MPSharedPtr<T> const& a, MPSharedPtr<U> const& b)
{
	return a.get() != b.get();
}

#endif
