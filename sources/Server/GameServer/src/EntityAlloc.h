//=============================================================================
// FileName: EntityAlloc.h
// Creater: ZhangXuedong
// Date: 2005.01.18
// Comment: Entity Alloc

// modifed by knight.gong 2005.5.16. (To alloc all entities by the template of allocer)
//=============================================================================

#ifndef ENTITYALLOC_H
#define ENTITYALLOC_H

#include "Npc.h"
#include "Item.h"
#include "EventEntity.h"
#include "Player.h"
#include <memory>

#define defENTI_ALLOC_TYPE_PLY		0x01000000
#define defENTI_ALLOC_TYPE_CHA		0x02000000
#define defENTI_ALLOC_TYPE_ITEM		0x03000000
#define defENTI_ALLOC_TYPE_TNPC		0x04000000

#define defENTI_ENTEVENT 0x06000000
#define defENTI_ENTBASE(n) (defENTI_ENTEVENT + (n<<24))
#define defENTI_ALLOC_TYPE_ENTBASE			defENTI_ENTBASE(1)	// 
#define defENTI_ALLOC_TYPE_ENTRESOURCE		defENTI_ENTBASE(2)	// 
#define defENTI_ALLOC_TYPE_ENTTRANSIT		defENTI_ENTBASE(3)	// 
#define defENTI_ALLOC_TYPE_ENTBERTH			defENTI_ENTBASE(4)	// 

template <class T>
class CAlloc
{
public:
	CAlloc() = default;
	~CAlloc();

	void  clear();

	// 
	LONG getHoldSize()  const { return m_lHoldSize; }
	LONG getMaxHoldSize()  const { return m_lMaxHoldSize; }
	LONG getAllocSize() const { return m_lAllocSize; }
	
	// 
	bool  create( LONG lSize, LONG lFlag = 0 );
	T*	  alloc();
	void  destroy( T* pData );
	void  destroy( LONG lID );

	// 
	void  begin();
	T*	  end();
	T*    next();

	void  revbegin();
	T*    revnext();

	T*	  getinfo( LONG lID );

private:
	LONG m_lCur{};
	LONG m_lHoldSize{};
	LONG m_lAllocSize{};
	LONG m_lMaxHoldSize{};
	LONG m_lFlag{}; ///NOTE(Ogge): What about this one?


	std::unique_ptr<T* []> m_pHold{};
	std::unique_ptr<T[]> m_pAlloc{};
};

template<class T>
CAlloc< T >::~CAlloc()
{
	clear();
}

template<class T>
void CAlloc< T >::clear()
{
	m_lCur = 0;
	m_lHoldSize = 0;
	m_lMaxHoldSize = 0;
	m_lAllocSize = 0;
	m_pHold.reset();
	m_pAlloc.reset();
}

template<class T>
bool CAlloc< T >::create(LONG lSize, LONG lFlag)
{
	if( lSize <= 0 ) {
		return false;
	}

	clear();

	m_lFlag = lFlag;
	m_lAllocSize = lSize;

	try {
		m_pHold = std::make_unique<T* []>(m_lAllocSize);
		m_pAlloc = std::make_unique<T[]>(m_lAllocSize);
	}
	catch (const std::bad_alloc&)
	{
		clear();
		return false;
	}

	for( LONG i = 0; i < m_lAllocSize; i++ )
	{
		m_pAlloc[i].SetHandle( m_lFlag | i );
		m_pAlloc[i].SetHoldID( -1 );
		m_pHold[i] = &m_pAlloc[i];
	}

	return true;
}

template<class T>
T* CAlloc< T >::alloc()
{
	if( m_lHoldSize < m_lAllocSize )
	{
		m_pHold[m_lHoldSize]->SetHoldID( m_lHoldSize );
		m_pHold[m_lHoldSize]->Initially();
		m_lHoldSize++;
		if (m_lHoldSize > m_lMaxHoldSize) m_lMaxHoldSize = m_lHoldSize;
		return m_pHold[m_lHoldSize - 1];
	}
	return nullptr;
}

template<class T>
void CAlloc< T >::destroy( T* pData )
{
	// 
}

template<class T>
void CAlloc< T >::destroy( LONG lID )
{
	if( lID < 0 || lID >= m_lAllocSize )
	{
		return;
	}

	LONG lHoldID = m_pAlloc[lID].GetHoldID();
	if( lHoldID < 0 || lHoldID >= m_lHoldSize )
	{
		return;
	}

	m_lHoldSize--;

	std::swap(m_pHold[m_lHoldSize], m_pHold[lHoldID]);
	m_pHold[lHoldID]->SetHoldID(lHoldID);

	if (m_lCur == m_lHoldSize)
	{
		m_lCur--;
	}

	long	lTempHoldS = m_lHoldSize;
	try
	{
		m_pHold[m_lHoldSize]->Finally();
	}
	catch (...)
	{
	}
	m_pHold[lTempHoldS]->SetHoldID(-1);
}

template<class T>
T* CAlloc< T >::getinfo( LONG lID )
{
	if( lID >= m_lAllocSize ) {
		return NULL;
	}

	LONG lHoldID = m_pAlloc[lID].GetHoldID();
	if( lHoldID < 0 || lHoldID >= m_lHoldSize ) {
		return NULL;
	}

	return &m_pAlloc[lID];
}

template<class T>
void CAlloc< T >::begin()
{
	m_lCur = 0;
}

template< class T>
T* CAlloc< T >::end()
{
	return NULL;
}

template< class T>
T* CAlloc< T >::next()
{
	if( m_lCur >= m_lHoldSize ) 
	{
		return NULL;	
	}	
	return m_pHold[m_lCur++]; 
}

template<class T>
void CAlloc< T >::revbegin()
{
	m_lCur = m_lHoldSize - 1;
}

template< class T>
T* CAlloc< T >::revnext()
{
	if( m_lCur < 0 ) 
	{
		return NULL;	
	}	
	return m_pHold[m_lCur--]; 
}

class	CEntityAlloc
{
public:
	CEntityAlloc(long lChaNum = 5000, long lItemNum = 3000, long lTNpcNum = 200);
	~CEntityAlloc();

	CCharacter*	GetNewCha();
	CItem*		GetNewItem();
	mission::CTalkNpc*	GetNewTNpc();
	mission::CEventEntity* GetEventEntity( BYTE byType );
	BOOL		ClearEventEntity();

	Entity		*GetEntity(long lID);
	void		ReturnEntity(long lID);

	void		BeginGetTNpc(void) { m_TalkNpcAlloc.revbegin(); }
	mission::CTalkNpc*	GetNextTNpc(void) {return m_TalkNpcAlloc.revnext();}

	void		BeginGetItem(void) {m_ItemAlloc.revbegin();}
	CItem*		GetNextItem(void) {return m_ItemAlloc.revnext();}

	void		BeginGetCha(void) {m_ChaAlloc.revbegin();}
	CCharacter*	GetNextCha(void) {return m_ChaAlloc.revnext();}

	long		GetHoldChaNum(void) { return m_ChaAlloc.getHoldSize(); }
	long		GetHoldItemNum(void) { return m_ItemAlloc.getHoldSize(); }
	long		GetHoldTNpcNum(void) { return m_TalkNpcAlloc.getHoldSize(); }
	long		GetMaxHoldChaNum(void) { return m_ChaAlloc.getMaxHoldSize(); }
	long		GetMaxHoldItemNum(void) { return m_ItemAlloc.getMaxHoldSize(); }
	long		GetMaxHoldTNpcNum(void) { return m_TalkNpcAlloc.getMaxHoldSize(); }

	long		GetAllocChaNum(void) { return m_ChaAlloc.getAllocSize(); }
	long		GetAllocItemNum(void) { return m_ItemAlloc.getAllocSize(); }
	long		GetAllocTNpcNum(void) { return m_TalkNpcAlloc.getAllocSize(); }

private:
	typedef CAlloc<CCharacter> CHA_ALLOC;
		CHA_ALLOC m_ChaAlloc;
	typedef CAlloc<CItem> ITEM_ALLOC;
		ITEM_ALLOC m_ItemAlloc;
	typedef CAlloc<mission::CTalkNpc> TALKNPC_ALLOC;
		TALKNPC_ALLOC m_TalkNpcAlloc;
	typedef CAlloc<mission::CBerthEntity> BERTH_ALLOC;
		BERTH_ALLOC	m_BerthAlloc;
	typedef CAlloc<mission::CResourceEntity> RESOURCE_ALLOC;
		RESOURCE_ALLOC m_ResourceAlloc;

};

class	CPlayerAlloc
{
public:
	CPlayerAlloc(long lPlyNum = 3000) {m_PlyAlloc.create( lPlyNum);}
	~CPlayerAlloc() {	m_PlyAlloc.clear();}

	CPlayer*	GetNewPly();
	CPlayer*	GetPly(long lID) {return m_PlyAlloc.getinfo( lID );}
	void		ReturnPly(long lID) {m_PlyAlloc.destroy( lID );}

	long		GetHoldPlyNum(void) { return m_PlyAlloc.getHoldSize(); }
	long		GetMaxHoldPlyNum(void) { return m_PlyAlloc.getMaxHoldSize(); }
	long		GetAllocPlyNum(void) { return m_PlyAlloc.getAllocSize(); }

protected:

private:
	typedef CAlloc<CPlayer> PLAYER_ALLOC;
		PLAYER_ALLOC m_PlyAlloc;

};

#endif // ENTITYALLOC_H
