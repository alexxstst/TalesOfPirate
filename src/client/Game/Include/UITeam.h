#pragma once

class CCharacter;
struct stNetTeamChaPart;

namespace GUI
{

// 
enum  eTeamStyle
{
	enumTeamFrnd,       // 
    enumTeamGroup,      // 
    enumTeamGuild,      // 
    enumTeamRoad,       // 
	enumTeamMaster,		// 
	enumTeamPrentice,	// 
	enumTeamGM,	// 
	enumTeamBlocked,
};

enum  eShowStyle
{
	enumShowQQName,       // 
	enumShowQQMotto,      // 
};


class CMemberData
{
public:
    CMemberData();
    ~CMemberData();

	DWORD	GetIndex()		{ return _dwIndex;	}
	void	SetIndex( DWORD v )	{ _dwIndex = v;	}

    long GetHP()    { return _nHp;      }
    long GetSP()    { return _nSp;      }
    long GetLV()    { return _nLv;      }
    long GetMaxHP() { return _nMaxHp;   }
    long GetMaxSP() { return _nMaxSp;   }
	BYTE GetWork()	{ return _byWork;	}

	void SetWork( BYTE v )	{ _byWork = v;	}
    void SetHP( long v )    { _nHp = v;     }
    void SetSP( long v )    { _nSp = v;     }
    void SetLV( long v )    { _nLv = v;     }
    void SetMaxHP( long v ) { _nMaxHp = v;  }
    void SetMaxSP( long v ) { _nMaxSp = v;  }

	void SetOnline(bool isOnline) {_bOnline=isOnline;}
	bool IsOnline() {return _bOnline;}

	stNetTeamChaPart*   GetFace()   { return _pFace;        }
    void SetFace( const stNetTeamChaPart& pFace );

private:
	DWORD	 _dwIndex;
	bool	 _bOnline;
	BYTE	 _byWork;
    long     _nMaxHp;   
    long     _nHp;      // 
    long     _nSp;      // 
    long     _nLv;
    long     _nMaxSp;

    stNetTeamChaPart    *_pFace;

};

class CTeam;
class CMember
{
public:
	CMember( CTeam* pTeam, unsigned long id, const char* strName , const char* strMotto, DWORD icon_id);

    virtual ~CMember();

    void Refresh();

    void SetName( const char* str );
	void SetMotto( const char* str );
	void SetJobName(const char* str);

    void SetID( DWORD id )         { _nID=id;        }
	void SetIconID( DWORD iconId );
	void SetPointer( void* p )   { _pPointer = p;        }
	void SetOnline(bool isOnline);
	void ModifyAttr(std::string motto, std::string job, DWORD dwLv, DWORD nIcon_ID, std::string guildName);


    DWORD		GetID()     { return _nID;              }
    const char* GetName()   { return _strName.c_str();  }
	const char* GetMotto()   { return _strMotto.c_str();  }
	const char* GetShowName();
	const char* GetGuildName()	{ return _strGuildName.c_str(); }

	DWORD		GetIconID() { return _nIcon_id;			}
	DWORD		GetLevel()	{ return _dwLv;}
	const char* GetJobName()	{ return _strJob.c_str();}
	void*       GetPointer()            { return _pPointer;     }
    CTeam*      GetTeam()   { return _pTeam;            }
    CMemberData*    GetData()           { return _pData;        }

	bool		IsOnline() {return _bIsOnline;}
	void		SetIsFlash(bool isFlash) { _bFlash=isFlash;}
	bool		GetIsFlash() { return _bFlash;}


	static void SetShowStyle(eShowStyle style);
	static eShowStyle _nShowStyle;

private:
    std::string          _strName;       // 
	std::string			_strMotto;
	std::string			_strJob;
	std::string			_strShowName;
	std::string			_strGuildName;

	DWORD			_dwLv;
    DWORD			_nID;			// ID
	DWORD			_nIcon_id;
	bool			_bIsOnline;
    void*           _pPointer;
	bool			_bFlash;

    CMemberData*    _pData;
    CTeam*          _pTeam;

};

class CTeam
{
public:
    CTeam(eTeamStyle nStyle, const char* szName );
    ~CTeam();

    void		Clear();

	CMember*    Add( unsigned long nID, const char* szName, const char* szMotto, DWORD icon_id);

    bool		Del( unsigned long nID );
    bool        DelByName( const char* szName );

    CMember*    Find( unsigned long nID );
    CMember*    FindByName( const char* szName );
	CMember*	GetMember( unsigned long nIndex );

    const char* GetName()   { return _strName.c_str();      }
    int         GetStyle()  { return _eStyle;               }
    DWORD       GetCount()  { return _nCount;               }
    
    void        SetPointer( void* p )   { _pPointer = p;        }
    void*       GetPointer()            { return _pPointer;     }

private:
    eTeamStyle  _eStyle;
    std::string      _strName;

    typedef std::list<CMember*>    members;
    members     _member;

    DWORD       _nCount;

    void*           _pPointer;

};

class CTeamMgr
{
public:
    CTeamMgr();
    ~CTeamMgr();

    CTeam*  Add( eTeamStyle eTeam, const char* szName="" );
    bool    Del( eTeamStyle eTeam, const char* szName="" );
    CTeam*  Find( eTeamStyle eTeam, const char* szName="" );
	void	ChangeStyle(eShowStyle style);

	void	SceneSwitch();
	void	ResetAll();

	static  DWORD	GetTeamLeaderID()	{ return _dwTeamLeaderID;		}
	static  void	SetTeamLeaderID( DWORD v )	{ _dwTeamLeaderID=v;	}

private:
	CTeam*  _pFrndTeam;				// 
    CTeam*  _pGroupTeam;			// 
    CTeam*  _pRoadTeam;				// 
	CTeam*	_pMasterTeam;			// 
	CTeam*	_pPrenticeTeam;			// 
	CTeam*	_pGMTeam;			// 
	CTeam*  _pBlockedTeam;

	static DWORD	_dwTeamLeaderID;

};

// 

}
