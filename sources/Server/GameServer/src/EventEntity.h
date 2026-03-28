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

		// ����ʵ��ģ����ʾ��Ϣ
		virtual BOOL Create( SubMap& Submap, const char szName[], USHORT sID, USHORT sInfoID, DWORD dwxPos, DWORD dwyPos, USHORT sDir );

		// �¼�ʵ����Ϣ����
		virtual HRESULT MsgProc( CCharacter& character, net::RPacket& packet );

		// ��ȡʵ��״̬��Ϣ
		virtual void GetState( CCharacter& character, BYTE& byState ) { byState = ENTITY_DISABLE; }

	protected:	
		BYTE	m_byType;	// ʵ������
		USHORT  m_sInfoID;  // ʵ���¼��ͻ��˱�����ϢID
	};

	class CResourceEntity : public CEventEntity
	{
	public:
		CResourceEntity();
		virtual ~CResourceEntity();

		virtual void Clear();
		virtual void SetType() { m_byType = RESOURCE_ENTITY; }

		// ������Դʵ��������Ϣ
		BOOL SetData( USHORT sItemID, USHORT sNum, USHORT sTime );

		// ����ʵ����Ϣ����
		virtual HRESULT MsgProc( CCharacter& character, net::RPacket& packet );

		// ��ȡʵ��״̬��Ϣ
		virtual void GetState( CCharacter& character, BYTE& byState );

	private:
		USHORT	m_sID;		// ��Դ��ϢID
		USHORT	m_sNum;		// ��Դ������Ϣ
		USHORT	m_sTime;	// ��Դ�ɼ�ʱ��
	};

	class CTransitEntity : public CEventEntity
	{
	public:
		CTransitEntity();
		virtual ~CTransitEntity();

		virtual void Clear();
		virtual void SetType() { m_byType = TRANSIT_ENTITY; }

		// ���ô���ʵ��������Ϣ
		BOOL SetData( const char szMap[], USHORT sxPos, USHORT syPos );

		// ����ʵ����Ϣ����
		virtual HRESULT MsgProc( CCharacter& character, net::RPacket& packet );

		// ��ȡʵ��״̬��Ϣ
		virtual void GetState( CCharacter& character, BYTE& byState );

	private:
		// �����͵��ĵ�ͼ��λ����Ϣ
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

		// ����ͣ����ֻʵ��������Ϣ
		BOOL SetData( USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir );

		// ����ʵ����Ϣ����
		virtual HRESULT MsgProc( CCharacter& character, net::RPacket& packet );

		// ��ȡʵ��״̬��Ϣ
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
