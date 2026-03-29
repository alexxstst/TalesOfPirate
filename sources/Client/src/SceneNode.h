#pragma once

#include <windows.h>
#include "Script.h"
#include "ChaAttrType.h"

// SetShadeShow(int iIdx, bool bShow)iIdx
#define SCENENODE_SHADOW  0  // Scene Node

#define	EFFECT_RIPPLE1	  97
#define	EFFECT_RIPPLE2	  98

#define	CHA_EFFECT1	  452
#define	CHA_EFFECT2	  453
#define	CHA_EFFECT3	  459
#define	CHA_EFFECT4	  468

#define	CHA1	  45
#define	CHA2	  48
#define	CHA3	  49
#define	CHA4	  31

enum NODE_TYPE
{
	NODE_CHA  = 0,
	NODE_ITEM = 1,
	NODE_OBJ = 2,
	NODE_EFF = 3,
};

struct SGameAttr
{
    long lAttr[MAX_ATTR_CLIENT];
    SGameAttr()
    {
        clear();
    }
    void    set(short sType, long lValue) { lAttr[sType] = lValue; }
    void    add(short sType, long lValue) { lAttr[sType] += lValue;}
    long    get(short sType)              { return lAttr[sType];   }
    void    clear()                       { memset(lAttr, 0, sizeof(lAttr)); }
};

class CGameScene;
class CEvent;
class CSceneNode : public CScript
{
public:
	
    virtual void	FrameMove(DWORD dwTimeParam) { _dwFrameCnt++;  }
	virtual void	Render()					 { _dwRenderCnt++; }
    virtual void    RefreshUI(int nParam = 0){}

    virtual void    SetScene( CGameScene* scene )    { _pScene = scene;}

	virtual void	setEvent( CEvent* pEvent )		{}

	CGameScene*	GetScene()			{	return _pScene;			}

	DWORD	getID()					{   return _dwID;			}
	void	setID(DWORD dwID)		{   _dwID = dwID;			}

	DWORD   getTypeID()				{   return _dwTypeID;		}
	void	setTypeID(DWORD dwID)	{   _dwTypeID = dwID;		}

	DWORD	getAttachID()			{	return _dwAttachID;		}
	void	setAttachID(DWORD dwID)	{	_dwAttachID = dwID;		}

	void	SetValid(BOOL bValid);
	BOOL	IsValid()				{	return _bValid;			}

    BOOL    IsHide()                {   return _bHide;          }
    void    SetHide(BOOL bHide)     {   _bHide = bHide;         }

	int		GetCurX()				{ return _nCurX;            }
	int		GetCurY()				{ return _nCurY;            }

	void	setYaw(int nYawAngle);
	int		getYaw()				{ return _nYaw;             }

	void	setPitch(int nPitchAngle);
	int		getPitch()				{ return _nPitch;           }

	void	setRoll(int nRollAngle);
	int		getRoll()				{ return _nRoll;            }

    void    setHeightOff(int nHeightOff);
    int     getHeightOff()          { return _nHeightOff;       }

	void    setPoseHeightOff(int nHeightOff);
	int     getPoseHeightOff()      { return _nPoseHeightOff;   }

    void    setPos(int nX, int nY);
    void    setSize(int nSize)      { _nSize = nSize;           }
    int     getSize()               { return _nSize;            }

	virtual	bool GetRunTimeMatrix(MPMatrix44* mat, DWORD dummy_id){ return false; }

	//lemon add@2004.9.27 for  bind effect;
	int				GetEffectNum()						{ return _iEffNum;                              }
	int				GetShadeNum()						{ return _iShadeNum;                            }
	void			SetEffectID(int iIdx, int iID)		{ _iEffID[iIdx] = iID;                          }
	void			SetShadeID(int iIdx, int iID)		{ _iShadeID[iIdx] = iID;                        }
	int				GetEffectID(int iIdx)				{ return _iEffID[iIdx];                         }
	int				GetShadeID(int iIdx)				{ return _iShadeID[iIdx];                       }
	void			AddEffect(int iID)					{ _iEffID[_iEffNum] = iID; _iEffNum++;          }
	void			AddShade(int iID)					{ _iShadeID[_iShadeNum] = iID; _iShadeNum++;    }
	void			RemoveEffect();			
	void			RemoveShade();
    SGameAttr*      getGameAttr()                       { return &_Attr;                                }   

	void			SetShadeShow(int iIdx,bool	bShow);
	void			SetEffectShow(int iIdx,bool	bShow);

	D3DXVECTOR3&	GetPos()							{ return _vPos;									}
	D3DXVECTOR3&	getPos()							{ return _vPos;									}

    long            lTag;

protected:
	CSceneNode();
	virtual ~CSceneNode(){}

	virtual BOOL	_Create( int nID, int nType ) = 0;

	virtual void	_UpdateYaw()              = 0;
	virtual void	_UpdatePitch()            = 0;
	virtual void	_UpdateRoll()             = 0;
    virtual void    _UpdateHeight()           = 0;
    virtual void    _UpdatePos()              = 0;
    virtual void    _UpdateValid(BOOL bValid) {}
    
	DWORD		_dwID;
	DWORD		_dwTypeID;
	DWORD		_dwAttachID;

	DWORD		_dwFrameCnt;
	DWORD		_dwRenderCnt;
	BOOL		_bValid;

	int			_nCurX;
	int			_nCurY;
	
	int			_nYaw;
	int			_nPitch;
	int			_nRoll;
    int         _nHeightOff;
	int			_nPoseHeightOff;
    
    BOOL        _bHide; //  (, )
    int         _nSize; // 

	//lemon add@2004.9.27 for  bind effect;
	int						_iEffNum;
	int						_iEffID[16];
	int						_iShadeNum;
	int						_iShadeID[16];

    SGameAttr               _Attr;

	D3DXVECTOR3		_vPos;				// 

protected:

    CGameScene	*_pScene;

private:
	friend class CGameScene;

	BOOL   _CreateNode( int nScriptID, int nType, CGameScene* pScene )
    {
		_pScene = pScene;
        _dwTypeID = nScriptID;

        _nHeightOff = 0;
        _nPoseHeightOff = 0;
        _bHide = FALSE;
        _dwAttachID = 0;
        _nSize = 0;
        _dwFrameCnt = 0;
        _dwRenderCnt = 0;
        _nCurX = 0;
        _nCurY = 0;
        _Attr.clear();
		return _Create( nScriptID, nType );
	}
};

inline void CSceneNode::setYaw(int nYawAngle)
{
 	_nYaw = nYawAngle;
	
	_UpdateYaw();
}

inline void CSceneNode::setPitch(int nPitchAngle)
{
	_nPitch = nPitchAngle;
	_UpdatePitch();
}

inline void CSceneNode::setRoll(int nRollAngle)
{
	_nRoll = nRollAngle;
	_UpdateRoll();
}

inline void CSceneNode::setHeightOff(int nHeightOff)
{
    _nHeightOff = nHeightOff;
    _UpdateHeight();
}

inline void CSceneNode::setPoseHeightOff(int nHeightOff)
{
	_nPoseHeightOff = nHeightOff;
	_UpdateHeight();
}

inline void CSceneNode::setPos(int nX, int nY)
{
    _nCurX = nX;
    _nCurY = nY;
    _UpdatePos();
}

inline void	CSceneNode::SetValid(BOOL bValid)
{	
    _bValid	= bValid;	
    _UpdateValid(bValid);

    if( !bValid ) _dwAttachID=0;
}

inline void CSceneNode::RemoveEffect()				
{ 
	for (int n = 0; n < 16; ++n)
	{
		_iEffID[n] = -1;
	}
	_iEffNum = 0;
}

inline void CSceneNode::RemoveShade()
{ 
	for (int n = 0; n < 16; ++n)
	{
		_iShadeID[n] = -1;
	}
	_iShadeNum = 0;
}
