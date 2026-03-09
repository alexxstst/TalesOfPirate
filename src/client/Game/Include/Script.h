#pragma once
#include <vector>

enum eScriptReturn
{
	R_OK = 1,
	R_FAIL = -1,
};

class CScript
{
public:
	CScript();
	virtual ~CScript();

	DWORD GetScriptID() { return _dwScriptID; }

static CScript* GetScriptObj( DWORD id ) {
		if( id<_dwCount ) return _AllObj[id];
		return NULL;
	}
	static bool		Init();
	static bool		Clear();

private:
	DWORD		_dwScriptID;			// ID-

static CScript**		_AllObj;
	static DWORD			_dwCount;
	static DWORD			_dwFreeCount;
	static DWORD			_dwLastFree;

};

class CScriptMgr
{
public:
	CScriptMgr();
	~CScriptMgr();

	bool Init();
	bool LoadScript();
	bool Clear();
	bool DoFile( const char* szLuaFile );
	bool DoString( const char* szLuaString );

	std::string	GetStoneHint( const char* szHintFun, int Lv );

	bool DoString( const char* szFunc, const char* szFormat, ... );

};
