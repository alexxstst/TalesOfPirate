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

		// 设置实体模型显示信息
		virtual BOOL Create( SubMap& Submap, const char szName[], USHORT sID, USHORT sInfoID, DWORD dwxPos, DWORD dwyPos, USHORT sDir );

		// 事件实体消息处理
		virtual HRESULT MsgProc( CCharacter& character, RPACKET packet );

		// 获取实体状态信息
		virtual void GetState( CCharacter& character, BYTE& byState ) { byState = ENTITY_DISABLE; }

	protected:	
		BYTE	m_byType;	// 实体类型
		USHORT  m_sInfoID;  // 实体事件客户端表现信息ID
	};

	class CResourceEntity : public CEventEntity
	{
	public:
		CResourceEntity();
		virtual ~CResourceEntity();

		virtual void Clear();
		virtual void SetType() { m_byType = RESOURCE_ENTITY; }

		// 设置资源实体数据信息
		BOOL SetData( USHORT sItemID, USHORT sNum, USHORT sTime );

		// 传送实体消息处理
		virtual HRESULT MsgProc( CCharacter& character, RPACKET packet );

		// 获取实体状态信息
		virtual void GetState( CCharacter& character, BYTE& byState );

	private:
		USHORT	m_sID;		// 资源信息ID
		USHORT	m_sNum;		// 资源数量信息
		USHORT	m_sTime;	// 资源采集时间
	};

	class CTransitEntity : public CEventEntity
	{
	public:
		CTransitEntity();
		virtual ~CTransitEntity();

		virtual void Clear();
		virtual void SetType() { m_byType = TRANSIT_ENTITY; }

		// 设置传送实体数据信息
		BOOL SetData( const char szMap[], USHORT sxPos, USHORT syPos );

		// 传送实体消息处理
		virtual HRESULT MsgProc( CCharacter& character, RPACKET packet );

		// 获取实体状态信息
		virtual void GetState( CCharacter& character, BYTE& byState );

	private:
		// 被传送到的地图和位置信息
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

		// 设置停泊船只实体数据信息
		BOOL SetData( USHORT sBerthID, USHORT sxPos, USHORT syPos, USHORT sDir );

		// 传送实体消息处理
		virtual HRESULT MsgProc( CCharacter& character, RPACKET packet );

		// 获取实体状态信息
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
