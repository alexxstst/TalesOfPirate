#pragma once

#include "SceneNode.h"
#include "MindPower.h"
#include "ItemRecord.h"

class CGameScene;
class CArcTrack;
class CEvent;
class CSceneHeight;
class CEffectObj;
class CCharacterModel;

class CSceneItem : public MPSceneItem, public CSceneNode
{
private:
	virtual BOOL	_Create(int nObjTypeID,int nType);

public:
	
	CSceneItem();
	virtual ~CSceneItem();
	
	virtual	void	FrameMove(DWORD dwTimeParam);
	virtual void	Render();

	void			RenderUI();

    void            setAttachedCharacterID( DWORD id ) { _nCharacterID = id;	}
    DWORD           getAttachedCharacterID() const	{ return _nCharacterID;		}

    int             Load(const char* file);
    HRESULT         LitResetTexture(DWORD item_id, DWORD lit_id);
    HRESULT         LitUnresetTexture();

	void			SetForgeEffect( DWORD value, int nCharID=1 );		// ,

public:
	void			setIsSystem( bool v )			{ _IsSystem = v; 			}
    bool            IsHitText( int x, int y );
    void			SetHide(BOOL bHide);

	void    SetItemInfo(int id)					{ _pItemInfo = GetItemRecordInfo( id );}
	CItemRecord*    GetItemInfo()					{ return _pItemInfo;		}
    int			    GetItemDrapID();

	void			setIsShowName( bool v );
    bool            getIsShowName()                 { return _IsShowName;       }

    void            setAlpla(float alpha=0.0f);

	void			setEvent( CEvent* pEvent )		{ _pEvent = pEvent;			}
	CEvent*			getEvent()						{ return _pEvent;			}

    bool            IsPick();

    bool            GetItemHeight(float* out_height);
	//lemon add@2005.1.5 for 
	void			PlayArcAni(D3DXVECTOR3	vStart, D3DXVECTOR3 vEnd, float fVel = 0.01f, float fHei = 3.0f);
protected:

	virtual void	_UpdateYaw();
	virtual void	_UpdatePitch();
	virtual void	_UpdateRoll();
    virtual void	_UpdatePos();
	virtual void    _UpdateHeight();
	virtual void    _UpdateValid(BOOL bValid);

	bool GetRunTimeMatrix(MPMatrix44* mat, DWORD dummy_id)
	{
		return GetObjDummyRunTimeMatrix(mat,dummy_id) == 0;
	}

    float           _fTerrainHeight;

    DWORD           _nCharacterID;

protected:
	bool			_IsSystem;					// 
	bool			_IsShowName;
	CItemRecord*	_pItemInfo;
    int             _nDrapID;
	CEvent*			_pEvent;

    bool            _IsAlpha;

    int             _nNameW, _nNameH;

    CArcTrack*      _pArcTrack;
	CSceneHeight*	_pSceneHeight;

	DWORD			_dwForgeValue;

	// added by clp
public:
	CEffectObj* bindEffect( int dummyID, int effectID, bool isLoop, int angle = -1 );
	void unbindEffect();
	void setParentCharacter( CCharacterModel* character )
	{
		mParentCharacter = character;
	}
	CCharacterModel* getParentCharacter()
	{
		return mParentCharacter;
	}
	void setParentDummy( int dummyID )
	{
		mParentDummyID = dummyID;
	}
	int getParentDummy()
	{
		return mParentDummyID;
	}
private:
	CCharacterModel* mParentCharacter;
	int mParentDummyID;
	CEffectObj* mEffect;
};

inline void CSceneItem::_UpdateYaw()
{
 	SetYaw(Angle2Radian((float)_nYaw));
    UpdateYawPitchRoll();
}

inline void CSceneItem::_UpdatePitch()
{
	SetPitch(Angle2Radian((float)_nPitch));
}

inline void CSceneItem::_UpdateRoll()
{
	SetRoll(Angle2Radian((float)_nRoll));
}

inline int CSceneItem::GetItemDrapID()                 
{ 
    return _pItemInfo->sDrap; 
}

inline bool CSceneItem::IsPick()
{
    return !_IsSystem && _pItemInfo->chIsPick && getAttachedCharacterID()==-1;
}
