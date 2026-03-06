#pragma once

enum eCursorState
{
	csNormalCursor=0,
	csCircleCursor,
	csEndCursor,
};

class CGameScene;
class CursorObj
{
public:
	virtual ~CursorObj(){}
	virtual void Init(CGameScene * p) {}
	virtual void Clear() {}

	virtual void MouseDown( int nButton )	{}
	virtual void MouseUp( int nButton )		{}
	virtual void MoveTo(D3DXVECTOR3 &stPos)	{}

	virtual void FrameMove( DWORD dwTime )	{}
	virtual void Render()					{}

	virtual void SetRadius( unsigned int r ){}
	virtual void SetIsShow(bool bShow){}

private:

};

class CursorMgr
{
public:
	CursorMgr();
	void InitMemory();
	void ClearMemory();
	void SceneInit(CGameScene * p);
	void SceneClear();
	void SetCursor( unsigned int cursor );

	inline void MouseDown( int nButton );
	inline void MouseUp( int nButton );

	inline void FrameMove( DWORD dwTime );
	inline void Render();
	inline void MoveTo(D3DXVECTOR3 &stPos);

	inline void SetIsVisible( bool v ) { _bIsVisible=v;	}
	
	inline CursorObj* GetCursor() { return _pCursor;	}

private:
	bool		_bIsVisible;
	CursorObj	*_pCursor;
	CursorObj	*_pCursorArray[csEndCursor];

};

inline void CursorMgr::MouseDown( int nButton )
{
	if( _bIsVisible ) _pCursor->MouseDown( nButton );
}

inline void CursorMgr::MouseUp( int nButton )
{
	if( _bIsVisible ) _pCursor->MouseUp( nButton );
}

inline void CursorMgr::FrameMove( DWORD dwTime )
{
	if( _bIsVisible ) _pCursor->FrameMove( dwTime );
}

inline void CursorMgr::Render()
{
	if( _bIsVisible ) _pCursor->Render();
}

inline void CursorMgr::MoveTo(D3DXVECTOR3 &stPos)
{
	if( _bIsVisible && stPos.x!=-1 ) _pCursor->MoveTo(stPos);
}

