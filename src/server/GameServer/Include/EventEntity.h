// EventEntity.h Created by knight-gongjian 2004.11.23.
//---------------------------------------------------------
#pragma once

#ifndef _EVENTENTITY_H_
#define _EVENTENTITY_H_

#include "Character.h"
//---------------------------------------------------------

namespace mission
{
	class CEventEntity : public CCharacter
	{
	public:
		CEventEntity();
		virtual ~CEventEntity();

		virtual CEventEntity* IsEvent() { return this; }

		virtual void SetType() { m_byType = BASE_ENTITY; }
		BYTE	GetType() { return m_byType; }
		USHORT	GetInfoID() { return m_sInfoID; }

		virtual void Clear();

		// 
		virtual BOOL Create( SubMap& Submap, const char szName[], USHORT sID, USHORT sInfoID, DWORD dwxPos, DWORD dwyPos, USHORT sDir );

		// 
		virtual HRESULT MsgProc( CCharacter& character, dbc::RPacket& packet );

		// 
		virtual void GetState( CCharacter& character, BYTE& byState ) { byState = ENTITY_DISABLE; }

	protected:	
		BYTE	m_byType;	// 
		USHORT  m_sInfoID;  // ID
	};

	class CResourceEntity : public CEventEntity
	{
	public:
		CResourceEntity();
		virtual ~CResourceEntity();

		virtual void Clear();
		virtual void SetType() { m_byType = RESOURCE_ENTITY; }

		// 
		BOOL SetData( USHORT sItemID, USHORT sNum, USHORT sTime );

		// 
		virtual HRESULT MsgProc( CCharacter& character, dbc::RPacket& packet );

		// 
		virtual void GetState( CCharacter& character, BYTE& byState );

	private:
		USHORT	m_sID;		// ID
		USHORT	m_sNum;		// 
		USHORT	m_sTime;	// 
	};

	class CTransitEntity : public CEventEntity
	{
	public:
		CTransitEntity();
		virtual ~CTransitEntity();

		virtual void Clear();
		virtual void SetType() { m_byType = TRANSIT_ENTITY; }

		// 
		BOOL SetData( const char szMap[], USHORT sxPos, USHORT syPos );

		// 
		virtual HRESULT MsgProc( CCharacter& character, dbc::RPacket& packet );

		// 
		virtual void GetState( CCharacter& character, BYTE& byState );

	private:
		// 
		char	m_szMapName[MAX_MAPNAME_LENGTH];
		USHORT  m_sxPos;
		USHORT  m_syPos;
	};

	class CBerthEntity : public CEventEntity
	{
	public:
		CBerthEntity();
		virtual ~CBerthEntity();

		virtual void Clear();
		virtual void SetType() { m_byType = BERTH_ENTITY; }

		// 
		BOOL SetData( USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir );

		// 
		virtual HRESULT MsgProc( CCharacter& character, dbc::RPacket& packet );

		// 
		virtual void GetState( CCharacter& character, BYTE& byState );

	private:
		USHORT m_sxPos;
		USHORT m_syPos;
		USHORT m_sDir;
		USHORT m_sBerthID;
	};
}
//---------------------------------------------------------

#endif // _EVENTENTITY_H_
