#pragma once
#include "Scene.h"
#include "SceneNode.h"
#include "MPCharacter.h"
#include "CompCommand.h"
#include "CharacterModel.h"
#include "publicfun.h"
#include "CharacterAction.h"
#include "BoolSet.h"
#include "CharacterRecord.h"
#include "Actor.h"
#include "FindPath.h"
#include "ChaState.h"

const int	ITEM_FACE_MAX =	4;			// 4?

class CShadeEff;
class CEffectObj;
class CGameScene;
class CActor;
class CSkillRecord;
class CChaRecord;
class CSkillStateRecord;
struct stSkillState;
class CEvent;
struct xShipInfo;
class CSceneHeight;



void RespawnAllPlayerMounts();
void DespawnAllPlayerMounts();

namespace GUI
{
	class CHeadSay;
}

struct LoadChaInfo
{
    LoadChaInfo()
    {
        memset( this, 0, sizeof(LoadChaInfo) );

    }

    DWORD cha_id;
    char bone[32];
    char part[5][32];
    char prop[4][32];
};

#define   ERROR_POSE_HEIGHT 9999
#define   MAX_CANCELSTATE   3

// 
enum eMainChaType
{
	enumMainNone = 0,		// 
	enumMainPlayer,			// ?
	enumMainBoat,			// 
};

enum eChaState
{
	enumChaStateMove=1,		// ?
	enumChaStateAttack,		// ?
	enumChaStateUseSkill,	// ?
	enumChaStateTrade,		// ?
	enumChaStateUseItem,	// ?

	enumChaStateNoHide,		// 
	enumChaStateNoDizzy,	// 
	enumChaStateNoAni,		// 

	enumChaStateNoShop,		// 
};

enum eChaPkState
{
	enumChaPkSelf = 1,		// PK
	enumChaPkScene = 2,			// ,PK
	enumChaPkGuild = 3, 
};

// This struct wasnt even needed, *sigh* - Mdr 
struct MountData {
		int boneID;
		float height;
		int offsetX;
		int offsetY;

		int yawAngle;
		int pose;
	};

class CCharacter : public CSceneNode, public CCharacterModel
{
	friend class CHeadSay;
	friend class CChaStateMgr;

private:
	virtual BOOL	_Create(int nCharTypeID, int nType);


public:	
	//Bone stuff
	DWORD _model_type; // lxo: 1, lmo: 2
    lwIResourceMgr* _res_mgr;
    lwINodeObject* _model_lxo;
    lwIModel* _model_lmo;
    lwIItem* _arrow;

    DWORD _cha_num;
    DWORD _act_num;
    lwINodePrimitive* _cha_obj[10];
    DWORD _act_obj[4];
	///


	virtual bool    GetRunTimeMatrix(MPMatrix44* mat, DWORD dummy_id);
	//stNetChangeChaPart look;
public:	
	void			InitState();						// 

    void            ForceMove(int nTargetX, int nTargetY);       // ?
	void			MoveTo( int x, int y );
	int			    FaceTo( int yaw );
	int 			FaceTo( int x, int y )	{ return FaceTo(_GetTargetAngle(x, y));	    }
	int				GetTargetDistance();

    void			StopMove();
	CActor*			GetActor()				{ return _pActor;	    }

	bool			GetIsArrive()						{ return _isArrive;		}
	bool			GetIsFaceTo()						{ return !_nTurnCnt;	}

	bool			UpdataItem( int nItem, DWORD nLink  );					// 
	void            UpdataFace(const stNetChangeChaPart& stPart);
	
	bool			GetIsMount()						{ return static_cast<bool>(mountOwner);}
	CCharacter*		GetMountOwner()					{return mountOwner;}
	void			SetMountOwner(CCharacter* owner)			{ mountOwner = owner;}


	bool			LoadBoat( stNetChangeChaPart& stPart );
	static xShipInfo*	ConvertPartTo8DWORD( stNetChangeChaPart& stPart, DWORD* dwBuf );

	bool			IsTeamLeader()				{ return _nLeaderID!=0 && _nLeaderID==getHumanID();		}	// 
	long			GetTeamLeaderID()			{ return _nLeaderID;	}	// ?,0

	void			SetTeamLeaderID( long v )	{ _nLeaderID=v;			}

	void			RefreshShadow();
	void			SetHide(BOOL bHide);

	//void			SetHieght(float fhei);
public: // ?
	bool			ChangeReadySkill( int nSkillID );

	static CSkillRecord*	GetReadySkillInfo()		{ return _pReadySkillInfo;		}

    static void             SetDefaultSkill( CSkillRecord* p )  { _pDefaultSkillInfo = p;       }
	static CSkillRecord*	GetDefaultSkillInfo()	            { return _pDefaultSkillInfo;	}

	static bool			    IsDefaultSkill();
	static void			    ResetReadySkill();

	static void				SetIsShowEffects( bool v )	{ _ShowEffects = v;		}
	static void				SetIsShowApparel( bool v )	{ _ShowApparel = v;		}
	static void				SetIsShowShadow( bool v )	{ _IsShowShadow = v;		}
	

	static bool				GetIsShowShadow()			{ return _IsShowShadow;		}
	static bool				GetIsShowApparel()			{ return _ShowApparel;		}
	
	




	CChaRecord*		GetDefaultChaInfo()		{ return _pDefaultChaInfo;		}
	void			SetDefaultChaInfo( CChaRecord* pInfo );

	xShipInfo*		GetShipInfo()			{ return _pShipInfo;			}

	CBoolSet&		GetPK()					{ return _PK;					}
	bool			GetIsPK()				{ return _PK.IsAny();			}
	
public:
	void			ActionKeyFrame(DWORD key_id);

	bool			ItemEffect(int nEffectID, int nItemDummy, int nAngle=999 );
	CEffectObj*		SelfEffect(int nEffectID, int nDummy=-1, bool isLoop=false, int nSize=-1, int nAngle=999 );

	void			OperatorEffect( char oper, int x, int y );

	// :Dummy,,pTarget,nTargetChaID
	CEffectObj*		SkyEffect(int nEffectID, int nBeginDummy=2, int nItemDummy=0, int nSpeed=400, D3DXVECTOR3* pTarget=NULL, int nTargetChaID=-1, CSkillRecord* pSkill=NULL );	

    // ,,?,
    bool			IsEnabled()             { return GetActor()->IsEnabled();   }

    // ?
    bool            IsInMiniMap()           { return IsValid() && GetActor()->IsEnabled() && !IsHide(); }			     

	CSceneItem*		GetAttackItem();		// ?
    
    void            PlayAni( char* pAni, int nMax );	// 
	void			StopAni();							// 

    int             GetPose( int pose );

    bool            IsMainCha();
    void            CheckIsFightArea();

	int				GetSkillSelectType();

	void			setReliveTime(short sTime)	{	_sReliveTime = sTime;			}
	short			getReliveTime()				{	return _sReliveTime;			}
	int				getPatrolX()				{   return _nPatrolX;				}
	int				getPatrolY()				{   return _nPatrolY;				}
	void			setPatrol(int x, int y)		{   _nPatrolX = x, _nPatrolY = y;	}

	bool			GetIsFight()				{	return this->_InFight;			}
	void			FightSwitch(bool isFight)	{   _FightSwitch(isFight);			}

	bool			GetIsPet();	// 
	bool			GetIsFly(); // ?


	//////////////////////// Mounts
	bool			SpawnMount(int mountID);
	bool			RespawnMount();
	bool			DespawnMount();

	int				GetIsMountEquipped()			{return IsMountEquip;}
	void			SetIsMountEquipped(int x) { IsMountEquip = x;}
	bool			GetIsOnMount()				{	return bIsOnMount; }
	void			SetIsOnMount(bool x)		{  bIsOnMount = x;}
	CCharacter*		GetMount();
	int				GetApparelID(SItemGrid app);

private:		// 
	void			_CalPos(float fProgressRate);
	int				_GetTargetAngle(int nTargetX, int nTargetY, BOOL bBack = FALSE);
    void            _DetachAllItem();
    void            _FightSwitch( bool isFight );

	float			_fStepProgressRate;
	float			_fProgressYaw;
	float			_fStep;
	int				_nAngleStep;
	int				_nTurnCnt;
	int				_nTargetX, _nTargetY;
	int				_nLastPosX, _nLastPosY;

	bool			bIsOnMount{ false };
	int				IsMountEquip; // Mount
	int				lastMount;
	char			ownerName[50];


	bool			_isArrive;
    bool            _isStopMove;
	float			_fMapHeight;	    // 
	CSceneHeight*	_pSceneHeight;
	
	static			bool _IsShowName;
    
private:
	static void	    _SetReadySkill( CSkillRecord* p );

	static CSkillRecord*    _pDefaultSkillInfo;

	static CSkillRecord*   _pReadySkillInfo;	// 

	CChaRecord*     _pDefaultChaInfo;
	CActor*         _pActor;

private:
	int				_ulChaID;

    char			_szName[33];		// 
	char			_szHumanName[33];	// 
	char			_szGuildName[33];	// 
	char			_szGuildMotto[101];	// 
	int				_nGuildID;			// ID
	int				_nGuildPermission;
	DWORD			_dwGuildColor;		// 
	DWORD			_dwNameColor;		// 
	char			_szShopName[33];	// 

	char			_szPreName[16];		// 
	DWORD			_szPreColor;

	long			_lSideID;			// ,,,
	CShadeEff*		_pSideShade;		// ,

public:	// 
	void			setSideID( long v );
	long			getSideID()							{ return _lSideID;					}

	void			setIsPlayerCha(bool v){_isPlayerCha = v;};
	bool			getIsPlayerCha(){return _isPlayerCha;};
	bool			_isPlayerCha;
	
	void			RefreshSelfEffects();
	void			setName(const char *pszName);
	const char*		getName()							{ return _szName;					}

	void			setGuildID( int nGuildID );
	
	int				getGuildID()						{ return _nGuildID;					}
	DWORD			getGuildColor()						{ return _dwGuildColor;				}
	int				getGuildPermission()				{ return _nGuildPermission;}
	void			setGuildPermission( int guildPermission )	{ _nGuildPermission = guildPermission;	}
	void			setGuildName( const char* pszName )	{ strncpy( _szGuildName, pszName, sizeof(_szGuildName) );	}
	const char*		getGuildName()						{ return _szGuildName;				}

	int				getMobID()							{ return _ulChaID;					}
	void			setMobID(int ID)					{ _ulChaID = ID;					}

	void			setGuildMotto( const char* pszName ){ strncpy( _szGuildMotto, pszName, sizeof(_szGuildMotto) );	}
	const char*		getGuildMotto()						{ return _szGuildMotto;				}

	void			setHumanName(const char *pszName)	{ strncpy( _szHumanName, pszName, sizeof( _szHumanName ) );	}
	const char*		getHumanName()						{ return _szHumanName;				}

	void			setShopName(const char *pszName)	{ strncpy( _szShopName, pszName, sizeof( _szShopName ) );	}
	const char*		getShopName()						{ return _szShopName;				}

	void			setNameColor(DWORD dwColor);
	DWORD			getNameColor()						{ return _dwNameColor;		    }

	const char*		GetPreName()						{ return _szPreName;				}
	DWORD			GetPreColor()						{ return _szPreColor;				}

	// Game Attrib Shortcut
    void			setMoveSpeed( long lSpeed )			{	_Attr.set(ATTR_MSPD, lSpeed);   }
	long			getMoveSpeed()	 					{	return _Attr.get(ATTR_MSPD);    }
    void			setHPMax( long lValue )				{	_Attr.set(ATTR_MXHP, lValue);   }
    long			getHPMax()							{	return _Attr.get(ATTR_MXHP);    }
    void			setHP( long lValue )				{   _Attr.set(ATTR_HP, lValue);	    }
    long			getHP()								{	return _Attr.get(ATTR_HP);	    }
    void			setAttackSpeed( long lValue )	    {   _Attr.set(ATTR_ASPD, lValue);	}
    long			getAttackSpeed()					{	return _Attr.get(ATTR_ASPD);	}
    long			getLv()					{	return _Attr.get(ATTR_LV);	}

    void            setChaModalType( int type )         {   _eChaModalType = (EChaModalType)type;}
    EChaModalType   getChaModalType()                   {   return _eChaModalType;				 }

    void            setChaCtrlType( int type );
    EChaCtrlType    getChaCtrlType()                    {   return _eChaCtrlType;           }

	int				GetDangeType()						{ return _nDanger;					}

	int				getNpcType()						{   return _nNpcType;				}
	void			setNpcType( int type )				{   _nNpcType = type;				}

	void			setGMLv( char v )					{   _chGMLv = v;					}
	char			getGMLv()							{   return _chGMLv;					}

public:
	CCharacter();
	virtual ~CCharacter();
	
    void            LoadingCall();

	virtual	void	FrameMove(DWORD dwTimeParam);
	virtual void	Render();
	void			RefreshUI(int nParam = 0);
	void			RefreshLevel( int nMainLevel );
    void            RefreshItem( bool isFirst=false );

	void			EnableAI(BOOL bEnable)			{ _bEnableAI = bEnable;}
	void			ResetAITick()					{ _dwLastAITick = GetTickCount(); } 

	bool			PlayPose( DWORD pose, DWORD type = PLAY_ONCE, int time=-1, int fps = 60, bool isBlend=false, bool IsGlitched = false );

public:	    
    void			setPos(int nX, int nY);
	void			setPos(int nX, int nY, int nZ);
	
    bool			IsBoat();
	bool			IsPlayer();
	bool			IsNPC();
	bool			IsMonster();
	bool			IsResource();

	void			SetMainType( eMainChaType v )	{ _eMainType = v;			}
	eMainChaType	GetMainType()					{ return _eMainType;		}
	
	CBoolSet*		GetChaState()					{ return &_ChaState;		}

	void			SetCircleColor(D3DCOLOR dwColor);

    int             DistanceFrom(CCharacter *pCha);
    BOOL            WieldItem( const lwSceneItemLinkInfo* info );

    int             ReCreate( DWORD type_id );

public:
	GUI::CHeadSay*	GetHeadSay() { return _pHeadSay.get(); }
    void            DestroyLinkItem();

    int             LoadCha( const LoadChaInfo* info );

    void            UpdateTileColor();
	char*			getLogName();
	
	void			setSecondName(const char *pszSecName) { strcpy(_szSecondName, pszSecName); }
	const char*     getSecondName()						  { return _szSecondName;              }
	void			ShowSecondName(BOOL bShow)			  { _bShowSecondName = bShow;          }
	BOOL			IsShowSecondName()					  { return _bShowSecondName;           }
	void			setPhotoID(short sID)			      { _sPhotoID = sID;				   }
	short			getPhotoID()						  { return _sPhotoID;				   }

	void			setHumanID( DWORD v )				  { _dwHumanID = v;					   }
	DWORD			getHumanID()						  { return _dwHumanID;				   }

    void            setNpcState( DWORD dwState );     // 

	void			DieTime();						  // ?

    void            SetIsForUI( bool v )                { _IsForUI = v;                 }
    bool            GetIsForUI()                        { return _IsForUI;              }

	void			setEvent( CEvent* pEvent )			{ _pEvent = pEvent;				}
	CEvent*			getEvent()							{ return _pEvent;				}

	void			SwitchFightPose();

	stNetChangeChaPart&		GetPart()					{ return _stChaPart;			}
	stNetChangeChaPart	_stChaPart;

	bool			IsUpdate()							{ return _bUpdate;				}
	CSceneItem*		GetNpcStateItem()						{ return _pNpcStateItem;		}

	bool			IsShop()							{ return _ChaState.IsFalse(enumChaStateNoShop);	}

	void			RefreshFog();

	CSceneItem*		GetHandItem( int nEquipPos );
	CEffectObj*		GetHandEff(int pos)					{return _pHandItemEff[pos];}
	void			SetHandEff(CEffectObj* eff, int pos)			{_pHandItemEff[pos] = eff;}
	
	void			SetItemFace( unsigned int nIndex, int nItem );
	int				GetItemFace( unsigned int nIndex )  { return _ItemFace[nIndex];     }


	int				GetServerX()						{ return _nServerX;				}
	int				GetServerY()						{ return _nServerY;				}
	void			SetServerPos( int x, int y )		{ _nServerX=x; _nServerY=y;		}

	//add by ALLEN 2007-10-16
	bool			IsReadingBook();

public: //     
	CChaStateMgr*	GetStateMgr()						{ return _pChaState;			}

    void            SynchroSkillState( stSkillState* pState, int nCount );
    void            HitEffect( int nAngle );

	void			RefreshShopShop();
	void			UnloadHandEff();

private:
	CChaStateMgr*	_pChaState;
	CBoolSet		_ChaState;			// ?

	struct stHit
	{
		stHit( int id, int dummy ) : nEffectID(id), nDummy(dummy) {}

		int		nEffectID;
		int		nDummy;
	};
	typedef std::vector<stHit>	hits;
	hits			_hits;

protected:
    virtual void	_UpdateYaw();
	virtual void	_UpdatePitch();
	virtual void	_UpdateRoll();
    virtual void    _UpdateHeight();
    virtual void    _UpdatePos();
	virtual void    _UpdateValid(BOOL bValid);

protected: 	
    BOOL			_bEnableAI;
	DWORD			_dwAIInterval;
	DWORD			_dwLastAITick;

	std::unique_ptr<GUI::CHeadSay> _pHeadSay;
    int             _nUIScale;		// , 
	CCharacter*		chaMount;
	CCharacter*		mountOwner;

private:			// 
	CBoolSet		_Special;		// 
	CBoolSet		_PK;			// PK

	// 
	long			_nHelixCenterX, _nHelixCenterY;				
	int				_nHelixAngle;	// 
	int				_nHelixRadii;

#ifdef _LOG_NAME_
	char			_szLogName[128];
public:
	void			setLogName( const char* str )	{ strncpy( _szLogName, str, sizeof(_szLogName) );	}

	static  bool    IsShowLogName;
#endif

private:
	float			_fMoveSpeed;
	char*			_pszFootMusic;
	char*			_pszWhoopMusic;
    float           _fMaxOpacity;       // ?,1.0f

    bool            _IsFightPose;
    bool            _InFight;           // 

    CSceneItem*     _pHandItem[ enumEQUIP_NUM ];
	CEffectObj*		_pHandItemEff[ enumEQUIP_NUM ];
	CEffectObj* CLOAKGlow[enumEQUIP_NUM]{nullptr};	//cloak glowing from iteminfo @mothannakh 
	static int  GetCloakGlowByRace(int race, int level);
	

    CSceneItem*     _pNpcStateItem;
	CSceneItem*		_pShopItem;

	int				_nNpcType;
    bool            _IsForUI;           // ?UI

	long			_nLeaderID;			// ID
	char			_szSecondName[41];	// ,
	BOOL			_bShowSecondName;	// 
	short			_sPhotoID;			// ID

    EChaModalType   _eChaModalType;		// 
	EChaCtrlType	_eChaCtrlType;		// 
	eMainChaType	_eMainType;			// 
	int				_nDanger;			// 

	DWORD			_dwHumanID;			// Group ServerID

	char			_chGMLv;
	CEvent*			_pEvent;

	xShipInfo*		_pShipInfo;
	bool			_bUpdate;
	CEffectObj*		_pBoatFog;			// ,

	CEffectObj*		_pItemFaceEff[ITEM_FACE_MAX];
	int				_ItemFace[ITEM_FACE_MAX];

private:
	bool			_IsMoveTimeType;	// false(),true
	D3DXVECTOR2		_vMoveStart, _vMoveEnd, _vMoveDir;
	DWORD 			_dwStartTime;
	float			_fMoveLen;
	
	// ?-----------------------------------
	short			_sReliveTime; // 
	int				_nPatrolX;	  // x
	int				_nPatrolY;	  // y
	// -----------------------------------------------------------

	int				_nServerX, _nServerY;

	static	bool	_IsShowShadow;
	static	bool	_ShowApparel;
	static	bool	_ShowEffects;
	

	// Added by clp
public:
	void linkTo( CCharacter *node, int boneID )
	{
		mParentNode = node;
		mParentBoneID = boneID;
	}
	void removeLink()
	{
		mParentNode = NULL;
	}
	void RemoveCloakGlow();
	void RenderCloakGlow();
protected:
	CCharacter *mParentNode;
	int mParentBoneID;
	//cloak previous level 
	int  CloakprevLevel{ 0 };
private:
	void _computeLinkedMatrix();
};

inline void CCharacter::setNameColor(DWORD dwColor)
{
	_dwNameColor = dwColor;
}

inline void CCharacter::_UpdateYaw()
{
	SetYaw( Angle2Radian((float)_nYaw) );
    UpdateYawPitchRoll();
	_nTurnCnt = 0;

    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }

}

inline void CCharacter::_UpdatePitch()
{
	SetPitch(Angle2Radian((float)_nPitch));
    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }
}

inline void CCharacter::_UpdateRoll()
{
	SetRoll(Angle2Radian((float)_nRoll));
    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }
}

inline void CCharacter::_UpdatePos()
{
    _vPos.x = (float)_nCurX / 100.0f;
    _vPos.y = (float)_nCurY / 100.0f;
    SetPos((float*)&_vPos); // , _vPos.z
    if( GetDrunkState() )
    {
        UpdateChaMatrix();
    }
}

inline bool	CCharacter::IsPlayer()
{
	return _eChaCtrlType==enumCHACTRL_PLAYER;
}

inline bool CCharacter::IsBoat()
{
    return _eChaModalType==enumMODAL_BOAT; 
}

inline bool CCharacter::IsNPC()
{
    return enumCHACTRL_NPC==_eChaCtrlType;    
}

inline bool CCharacter::IsMonster()
{
    return enumCHACTRL_MONS==_eChaCtrlType;  
}

inline CSceneItem*	CCharacter::GetAttackItem()
{
	CSceneItem* item = GetLinkItem( LINK_ID_RIGHTHAND );
	if( !item ) item = GetLinkItem( LINK_ID_LEFTHAND );
	return item;
}

inline int CCharacter::DistanceFrom(CCharacter *pCha)
{
	return (int)GetDistance(GetCurX(), GetCurY(), pCha->GetCurX(), pCha->GetCurY());
}

inline bool CCharacter::IsDefaultSkill()		
{
	return _pReadySkillInfo==NULL || _pReadySkillInfo==_pDefaultSkillInfo;	
}

inline char* CCharacter::getLogName()					
{ 
#ifdef _LOG_NAME_
	return _szLogName;
#else
	return _szName;
#endif
}

inline void CCharacter::_CalPos(float fProgressRate)
{
    float fX = (float)_nLastPosX * (1.0f - fProgressRate) + (float)_nTargetX * fProgressRate;
    float fY = (float)_nLastPosY * (1.0f - fProgressRate) + (float)_nTargetY * fProgressRate;





    setPos((int)fX, (int)fY);
}

inline bool CCharacter::IsMainCha()
{
    return this==GetScene()->GetMainCha();
}

inline void CCharacter::_DetachAllItem()
{
    DetachItem( LINK_ID_LEFTHAND );
    DetachItem( LINK_ID_RIGHTHAND );
    DetachItem( 21 );
    DetachItem( 22 );
    DetachItem( 23 );
}

inline int CCharacter::ReCreate( DWORD type_id )           
{ 
    setTypeID(type_id); 
    return CCharacterModel::ReCreate( type_id);    
}

inline void CCharacter::_SetReadySkill( CSkillRecord* p )
{
	_pReadySkillInfo = p;
}

inline void CCharacter::HitEffect( int nAngle )
{
	if( !_hits.empty() )
    {
		for( auto it=_hits.begin(); it!=_hits.end(); ++it )
        {
            SelfEffect( it->nEffectID, it->nDummy, false, -1, nAngle );
        }
    }
}


inline void CCharacter::MoveTo(int nTargetX, int nTargetY)
{
    ForceMove( nTargetX, nTargetY );
    if( !_isArrive )  FaceTo(_GetTargetAngle(nTargetX, nTargetY));
}

inline void CCharacter::CheckIsFightArea()
{
    if( _pScene->GetTerrain() )
    {
		_InFight = !(_pScene->GetTerrain()->GetTile(GetCurX()/100, GetCurY()/100)->sRegion & enumAREA_TYPE_NOT_FIGHT );
	}
}

inline int CCharacter::GetPose( int pose )
{
	if( _InFight )
    {
        switch( pose )
        {
        case POSE_WAITING:
            return POSE_WAITING2;
        case POSE_RUN:
			return POSE_RUN2;
        }
    }
    else
    {
        switch( pose )
        {
        case POSE_WAITING2:
            return POSE_WAITING;
        case POSE_RUN2:
			//if(g_cFindPath.LongPathFinding())
			//	return POSE_RUN2;
			//else
			//	return POSE_RUN;
			return POSE_RUN2;
		case POSE_RUN:
			//if(g_cFindPath.LongPathFinding())
			//	return POSE_RUN2;
			//else
			//	return POSE_RUN;
			return POSE_RUN;
        }
    }
    return pose;
}

inline void	CCharacter::StopMove()              
{ 
	if( _IsMoveTimeType ) 
		_isArrive=true; 
	else 
		_isStopMove = true;   

}

inline void	CCharacter::RefreshShadow()
{
	if( _IsShowShadow && (GetDefaultChaInfo()->chTerritory!=defCHA_TERRITORY_SEA) && GetActor()->GetState()==enumNormal && !IsHide() )
	{
		SetShadeShow( SCENENODE_SHADOW, true );
	}
	else
	{
		SetShadeShow( SCENENODE_SHADOW, false );
	}
}


inline void CCharacter::StopAni()
{
    _Special.AllFalse();
	_nHeightOff = 0;
	_fMapHeight = 0.0f;
	_nPoseHeightOff = ERROR_POSE_HEIGHT;

	_nCurX = _nHelixCenterX;
	_nCurY = _nHelixCenterY;
}

inline bool CCharacter::IsResource()
{
	return _eChaCtrlType>=enumCHACTRL_MONS_TREE && _eChaCtrlType<=enumCHACTRL_MONS_DBOAT;
}

inline CSceneItem* CCharacter::GetHandItem( int nEquipPos )
{
	if( nEquipPos<0 || nEquipPos>=enumEQUIP_NUM ) return NULL;

	return _pHandItem[nEquipPos];
}

inline int CCharacter::GetTargetDistance()
{
	if( _isArrive ) return 0;
	else return GetDistance( _nCurX, _nCurY, _nTargetX, _nTargetY );
}

inline void CCharacter::setChaCtrlType( int type )          
{
	_eChaCtrlType = (EChaCtrlType)type;
	switch( type )
	{
	case enumCHACTRL_NONE: _nDanger = 0; break;
	case enumCHACTRL_PLAYER: _nDanger = 1; break;
	case enumCHACTRL_NPC: _nDanger = 2; break;

	case enumCHACTRL_NPC_EVENT: _nDanger = 7; break;
	case enumCHACTRL_MONS_TREE: _nDanger = 7; break;
	case enumCHACTRL_MONS_MINE: _nDanger = 7; break;
	case enumCHACTRL_MONS_FISH: _nDanger = 7; break;
	case enumCHACTRL_MONS_DBOAT: _nDanger = 7; break;

	case enumCHACTRL_PLAYER_PET: _nDanger = 8; break;
	case enumCHACTRL_MONS_REPAIRABLE: _nDanger = 9; break;

	case enumCHACTRL_MONS: _nDanger = 10; break;
	default: _nDanger = 0; break;
	}
}
