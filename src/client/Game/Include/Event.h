//----------------------------------------------------------------------
// :
// :lh 2004-05-26
// :
// :
//----------------------------------------------------------------------
#pragma once

class CSceneNode;
class CEventRecord;
class CCharacter;
class CGameScene;

class CEvent
{
public:
	CEvent( CGameScene* pScene );
	~CEvent();

	void	Init();

	void	SetIsValid( bool v )	{ _IsValid = v;		}
	bool	GetIsValid()			{ return _IsValid;	}

	void	SetIsEnabled( bool v )	{ _IsEnabled = v;	}
	bool	GetIsEnabled()			{ return _IsEnabled;}

	void	SetName( const char* str )	{ _strName = str;				}

	bool	IsNormal()				{ return _IsValid && _IsEnabled;	}

	void			SetInfo( CEventRecord* pInfo )	{ _pEvent=pInfo;	}
	void			SetNode( CSceneNode* pNode )	{ _pNode=pNode;		}

	CEventRecord*	GetInfo()		{ return _pEvent;	}
	CSceneNode*		GetNode()		{ return _pNode;	}

	bool			DistanceTrigger( int x, int y );
	void			ExecEvent(CCharacter* pCha);

	void			Render();

private:
	CGameScene*		_pScene;
	std::string			_strName;			// 

	bool			_IsValid;
	bool			_IsEnabled;			

	CEventRecord*   _pEvent;
    CSceneNode*     _pNode;              // Node

 // run time
    bool            _IsActive;           // 
	DWORD			_dwLastTime;		 // 

};

class CEventMgr
{
public:
	CEventMgr( CGameScene* pScene );
	~CEventMgr();

	void		Clear();

	void		DistanceTrigger( CCharacter* pCha );
	CEvent*		CreateEvent( DWORD dwEventID );
	void		Render();

	CEvent*		Search(long	lEntityID);

CGameScene* _pScene;

	typedef std::vector<CEvent*>	events;
	events		_events;

};

inline void	CEvent::Init()
{
	_IsValid = false;
	_pEvent = NULL;
	_pNode = NULL;
	_IsEnabled = true;

	_IsActive = false;
	_dwLastTime = 0;
}
