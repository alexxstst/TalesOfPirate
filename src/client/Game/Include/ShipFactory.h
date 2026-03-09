#pragma once
#include "scene.h"
#include "MindPower.h"
#include "CharacterModel.h"

#include "GameApp.h"
#include "UIRender.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "uiformmgr.h"
#include "uitextbutton.h"
#include "uilabel.h"
#include "uiprogressbar.h"
#include "uiscroll.h"
#include "uilist.h"
#include "uicombo.h"
#include "uiimage.h"
#include "UICheckBox.h"
#include "uiimeinput.h"
#include "uigrid.h"
#include "uilistview.h"
#include "uipage.h"
#include "uitreeview.h"
#include "uiimage.h"
#include "UILabel.h"
#include "UI3DCompent.h"
#include "UIForm.h"
#include "UIMemo.h"
#include "lua_platform.h"
#include "SceneItem.h"
#include "WorldScene.h"
#include "ItemRecord.h"
#include "CharacterAction.h"
#include "PacketCmd.h"

#include "ShipSet.h"

// follow your dreams !!!!

enum
{
    SBF_BTN_HEADLEFT =      0,
    SBF_BTN_HEADRIGHT,
    //SBF_BTN_BODYLEFT,
    //SBF_BTN_BODYRIGHT,
    SBF_BTN_POWERLEFT,
    SBF_BTN_POWERRIGHT,
    SBF_BTN_WEAPONLEFT,
    SBF_BTN_WEAPONRIGHT,
    SBF_BTN_SIGNLEFT,
    SBF_BTN_SIGNRIGHT,

    SBF_BTN_ITEM_NUM,

    SBF_BTN_VIEW3D_LEFT =   0,
    SBF_BTN_VIEW3D_RIGHT,
    SBF_BTN_VIEW3D_TOP,
    SBF_BTN_VIEW3D_BOTTOM,

    SBF_BTN_VIEW3D_NUM,

    SBF_TEXT_ATTACK =       0,
    SBF_TEXT_DEFENCE,
    SBF_TEXT_ENDURE,
    SBF_TEXT_SUPPLY,
    SBF_TEXT_SPEED,
    SBF_TEXT_COOLDOWN,
    SBF_TEXT_DISTANCE,
    SBF_TEXT_CAPACITY,
    SBF_TEXT_MONEY,
    SBF_TEXT_BODY,
    SBF_TEXT_HEAD,
    SBF_TEXT_POWER,
    SBF_TEXT_WEAPON,
    SBF_TEXT_SIGN,

    SBF_TEXT_PROP_NUM,    


};

struct xShipBuilderForm
{
    CForm* wnd;
    C3DCompent* view3d;
    CTextButton* btn_item[SBF_BTN_ITEM_NUM];
    CTextButton* btn_view3d[SBF_BTN_VIEW3D_NUM];
    CLabelEx* lbl_prop[SBF_TEXT_PROP_NUM];
    CTextButton* btn_create;
    CTextButton* btn_cancel;
    CEdit* name;

	CLabelEx* lbl_ship_level; 
	CLabelEx* lbl_ship_exp;

	//UI(Michael Chen 2005-05-26)
	CCheckBox* chkShip;
	CTextButton* btn_room_open;
	CTextButton* btn_room_close;
	CForm* wnd_boat_room;

	CImage* imgMoneyTitle;
	CImage* imgSpaceTitle;
	CImage* imgShipOrigin;
};

class xShipFactory
{
    enum
    {
        ITEM_HEAD =     0,
        ITEM_POWER,
        ITEM_WEAPON,
        ITEM_GOODS,

		MIN_LENGTH = 2,
		MAX_LENGTH = 16,
    };

public:
	enum eState
	{
		STATE_CREATE,
		STATE_INFO,
		STATE_INFO_OTHER,
		STATE_TRADE,
		STATE_TRADE_FREEDOM,
	};

    const static DWORD __PartRange[4][2];

    CGameScene* scene;
    CFormMgr* form_mgr;
    CCharacter* ship;
    xShipBuilderForm sbf;
    int yaw_degree;
    int pitch_degree;
    char item_id[4]; // head, power, weapon, goods
    char ship_name[130];
    DWORD item_part[8];
    BYTE mt_flag;
    int type;

	DWORD m_dwBoatID;		//ID--Michael Chen(2005-05-26)
	eState m_state;

static void	__ButtonYesNo(CGuiData *pSender, int x, int y, DWORD dwKey);
	static void __ProcBtnItem(CGuiData *sender, int x, int y, DWORD key);
	static void __ProcBtnView3d(CGuiData *sender, int x, int y, DWORD key);
	static void __Proc3DView(C3DCompent *pSender, int x, int y);
    static void __ProcShipName(CGuiData *pSender);

	//UI(Michael Chen 2005-05-26)
    static void __ButtonOpenRoom(CGuiData *pSender, int x, int y, DWORD dwKey);
	static void __ButtonCloseRoom(CGuiData *pSender, int x, int y, DWORD dwKey);
	//chkShip(Michael Chen 2005-05-26)
	static void __CheckShip(CGuiData *pSender);
	//(Michael Chen 2005-06-06)
	static void __HideForm(CGuiData *pSender);

private:
	xShipBuildInfo *m_pkBoatInfo;
	int				m_count;
public:
    xShipFactory();
    ~xShipFactory();

    void SetType(int t) { type = t; }
    BOOL Init(CGameScene* s);
    void ShowFlipBtn(int show, const char* name);
    void Show(int flag);
    void FrameMove();
    void Render3DView(int x, int y);
    BOOL Update(xShipBuildInfo* info, BOOL flag, const char* name); 
	BOOL UpdateBoatCreate(xShipBuildInfo* info, BOOL flag, const char* name);
	BOOL UpdateBoatInfo(xShipBuildInfo* info, BOOL flag, const char* name);
	BOOL UpdateBoatFreedomTrade(const char* name, DWORD* dwModelInfo, size_t size);
	BOOL SetShipModelInfo(DWORD* dwModelInfo, size_t size);
    BOOL CreateShip(DWORD type_id, DWORD* part_buf); // tyep_id: 301
    BOOL ChangePart(DWORD part_id, DWORD file_id);
    BOOL UpdateShipPart(CGuiData* sender);
    BOOL CheckShipName();
    BOOL Close(CGuiData* sender);
    BOOL IsVisible();
	//ID Michael Chen (2005-05-26).
	void SetBoatID(DWORD dwBoatID)	{ m_dwBoatID = dwBoatID; }
	DWORD GetBoatID() { return m_dwBoatID; }
	BOOL GetCabinByID();
	void SetBoatAttr();
	void SetManBoatAttr();
	void SetState(eState state) { m_state = state;	}
	eState GetState() { return m_state; }
	void UpdateBoatAttr();
	void ClearBoatAttr();

    void Test();
};

class xShipLaunchList
{
public:
    CGameScene* scene;
    CFormMgr* form_mgr;
    CForm* wnd;
    CMemo* memo;
    CTextButton* btn_close;
    CTextButton* btn_cancel;

	/*(Michael Chen)*/
	typedef enum enumFlag
	{
		eLaunch,
		eTrade,
		eCreate,
		eBag,
		eRepair,
		eSalvage,
		eSupply,
		eUpgrade,
	} eFlag;


static void __ProcSelectChange(CGuiData *pSender);
    static void	__ButtonClose(CGuiData *pSender, int x, int y, DWORD dwKey);

    xShipLaunchList();
    ~xShipLaunchList();
    BOOL Init(CGameScene* s);
    void SelectItem(CGuiData *pSender);
	/*flag (Michael Chen)*/
    void Update(DWORD num, const BOAT_BERTH_DATA* data, const eFlag flag=eLaunch);
	/*NPC ID (Michael Chen)*/
	void SetNpcID(DWORD dwNpcID) { m_dwNpcID = dwNpcID; }
	/*type (Michael Chen)*/
	void SetType(BYTE byType) { m_byType = byType; }

    void Test();

	//  xShipMgr    add by Philip.Wu  2006-06-02
	void CloseForm(void);
	
private:
	eFlag m_eFlag;
	DWORD m_dwNpcID;
	BYTE  m_byType;

};

class xShipMgr
{
public:
    xShipFactory* _factory;
    xShipLaunchList* _launch_list;

xShipMgr();
    ~xShipMgr();

    BOOL Init(CGameScene* s);
    void FrameMove();

	//    add by Philip.Wu  2006-06-02
	void	CloseForm();
};

/*
struct xShipInfo
{
    DWORD model_id;
    DWORD data[4]; // screw ...
    DWORD level;    
    DWORD type;
    DWORD price;
    DWORD cha_level;
    DWORD cha_profession;
    DWORD prop_hp;
    DWORD prop_res; // resistance
    DWORD prop_def; // defence
    DWORD prop_att[2]; // attack;
    DWORD prop_dis; // distance
    DWORD prop_cd; // cooldown
    DWORD prop_cap; // capacity
    DWORD prop_sp;
    DWORD prop_sp_rate;
    DWORD prop_cannon_speed;
    DWORD prop_speed; // move speed
};

class xShipSet : public CRawDataSet
{
public:
	static xShipSet* I() { return _Instance; }

	xShipSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:

	static xShipSet* _Instance; // , 

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return (CRawDataInfo*)new xShipInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (xShipInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(xShipInfo);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
	{   
		if(ParamList.size()==0) return FALSE;

		xShipInfo *pInfo = (xShipInfo*)pRawDataInfo;

        int i = 0;
        pInfo->model_id = Str2Int(ParamList[i++]);
        pInfo->data[0] = Str2Int(ParamList[i++]);
        pInfo->data[1] = Str2Int(ParamList[i++]);
        pInfo->data[2] = Str2Int(ParamList[i++]);
        pInfo->data[3] = Str2Int(ParamList[i++]);
        pInfo->level = Str2Int(ParamList[i++]);
        pInfo->type = Str2Int(ParamList[i++]);
        pInfo->price = Str2Int(ParamList[i++]);
        pInfo->cha_level = Str2Int(ParamList[i++]);
        pInfo->cha_profession = Str2Int(ParamList[i++]);
        pInfo->prop_hp = Str2Int(ParamList[i++]);
        pInfo->prop_res = Str2Int(ParamList[i++]);
        pInfo->prop_def = Str2Int(ParamList[i++]);
        pInfo->prop_att[0] = Str2Int(ParamList[i++]);
        pInfo->prop_att[1] = Str2Int(ParamList[i++]);
        pInfo->prop_dis = Str2Int(ParamList[i++]);
        pInfo->prop_cd = Str2Int(ParamList[i++]);
        pInfo->prop_cap = Str2Int(ParamList[i++]);
        pInfo->prop_sp = Str2Int(ParamList[i++]);
        pInfo->prop_sp_rate = Str2Int(ParamList[i++]);
        pInfo->prop_cannon_speed = Str2Int(ParamList[i++]);
        pInfo->prop_speed = Str2Int(ParamList[i++]);

		return TRUE;
	}
};

inline xShipInfo* GetXShipInfo(int nTypeID)
{
	return (xShipInfo*)xShipSet::I()->GetRawDataInfo(nTypeID);
}
*/
