#pragma once
#include "UIGlobalVar.h"
#include "NetProtocol.h"
#include "SkillRecord.h"
#include "UIGuiData.h"

class CCharacter;
class CSkillStateRecord;
struct stNetSkillBag;

namespace GUI
{
class CItemCommand;

// 
class CEquipMgr : public CUIInterface
{
public:

	int				GetIMP(){return lIMP;}

	void			SynSkillBag(DWORD dwCharID, stNetSkillBag *pSSkillBag);

    void            UpdataEquip( const stNetChangeChaPart& SPart, CCharacter* pCha );
    void            UpdataEquipSpy( const stNetChangeChaPart& SPart, CCharacter* pCha );
    void            UpdataEquipData( const stNetChangeChaPart& SPart, CCharacter* pCha );

    void            UpdateShortCut( stNetShortCut& stShortCut );		// 
	void			DelFastCommand( CCommandObj* pObj );				// 
    bool            ExecFastKey( int key );
	void			FastChange(int nIndex, short sGridID, char chType,bool update = false);

	void            CloseAllForm();		// 

	// ,,,
	// 
	int				RefreshServerShortCut();							

	CGoodsGrid* GetGoodsGrid() { return grdItem; }	// 
	CForm* GetItemForm() { return frmInv; }   // 

	CForm* stateDrags;

	// 
	void			UnfixToGrid( CCommandObj* pItem, int nGridID, int nLink );

	CSkillRecord*	FindSkill( int nID );

	CItemCommand*	GetEquipItem( unsigned int nLink );

	bool			IsEquipCom( COneCommand* pCom );

	void			SetIsLock(bool bLock);
	bool			GetIsLock();
	
	void 			UpdateIMP(int imp);

	// 
	int				GetItemCount(int nID);

	static CGuiPic*		GetChargePic( std::uint32_t n );

	void ShowSellPrompt();
    static void		evtThrowItemEvent(CGuiData *pSender,int id,bool& isThrow);					// 
    static void		evtSwapItemEvent(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap); // 

protected:
	virtual bool Init();
    virtual void End();
	virtual void LoadingCall();
	virtual void FrameMove(DWORD dwTime);
	virtual void SwitchMap();
private:
	enum eDirectType
	{
		LEFT = -1, RIGHT = 1,
	};
	
	static void _UnequipPart(CGuiData *pSender, int x, int y, DWORD key);
	void RotateSpy(eDirectType enumDirect = LEFT);
	void RotateCha(eDirectType enumDirect = LEFT);

    CSkillList*		GetSkillList( char nType );
	void			RefreshUpgrade();
	void			RefreshSkillJob( int nJob );
	void			RenderSpy(int x,int y);
	void			RenderCha(int x,int y);
	void 			RenderModel(int x, int y, CCharacter* original, CCharacter* model, int rotation, bool refresh = false);
	
	bool			_GetThrowPos( int& x, int& y );
	static void ThrowSelectedItems(CGoodsGrid* grid);
	static void LockSelectedItems(CGoodsGrid* grid);
	static void DeleteSelectedItems(CGoodsGrid* grid);
public:
	bool			_GetCommandShortCutType( CCommandObj* pItem, char& chType, short& sGridID );
private:
	static void		_EqSpyRenderEvent(C3DCompent *pSender, int x, int y);
	static void		_ChaRenderEvent(C3DCompent *pSender, int x, int y);
	
	static void		_evtSkillFormShow(CGuiData *pSender);
	static void		_evtEquipFormShow(CGuiData *pSender);
	static void		_evtEquipFormClose( CForm *pForm, bool& IsClose );
	
	static void		_evtSpyFormClose( CForm *pForm, bool& IsClose );
	static void		_evtSpyFormShow(CGuiData *pSender);

	static void		_evtSkillUpgrade(CSkillList *pSender, CSkillCommand* pSkill);
    static void		_evtFastChange(CGuiData *pSender, CCommandObj* pItem, bool& isAccept);		// 
	static void		_evtEquipEvent(CGuiData *pSender, CCommandObj* pItem, bool& isAccept);		// 
	static void		_evtThrowEquipEvent(CGuiData *pSender, CCommandObj* pItem, bool& isThrow);  // 

	static void		_evtButtonClickEvent( CGuiData *pSender, int x, int y, DWORD key);
	static void		_evtRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID);

	bool			_UpdataEquip( SItemGrid& Item, int nLink );
	bool			_UpdataEquipSpy( SItemGrid& Item, int nLink );
    void			_ActiveFast( int num );         // 

	static void		_evtItemFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	//  MSGBOX 
	static void		_CheckLockMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	static void _RotateChaLeft(CGuiData *sender, int x, int y, DWORD key );
	static void _RotateChaRight(CGuiData *sender, int x, int y, DWORD key );
	static void _RotateChaLeftContinue(CGuiData *sender );
	static void _RotateChaRightContinue(CGuiData *sender );

	static void _RotateSpyLeft(CGuiData *sender, int x, int y, DWORD key );
	static void _RotateSpyRight(CGuiData *sender, int x, int y, DWORD key );
	static void _RotateSpyLeftContinue(CGuiData *sender );
	static void _RotateSpyRightContinue(CGuiData *sender );
	static void _ClickOpenApparel(CGuiData *pSender, int x, int y, DWORD key );
	static void _ClickTempBag(CGuiData* pSender, int x, int y, DWORD key);
	static void _OnDragStates(CGuiData* pSender, int x, int y, DWORD key);
	static int				lIMP;
	CForm*			frmSkill;

    CLabel*         labPoint;                   // 
    CLabel*         labPointLife;               // 
    CSkillList*     lstFightSkill;              // 
    CSkillList*     lstLifeSkill;               // 
    CSkillList*     lstSailSkill;               // 

	CTextButton* btnOpenTempBag;
	
	
	CCheckBox* chkLinkEqForm;
	
	CForm*			frmItemSpy;					// 
	CGoodsGrid*     grdItem;					// 
	COneCommand*    cnmEquip[enumEQUIP_NUM];    // 
	COneCommand*    cnmEquipSpy[enumEQUIP_NUM];    // 
    stNetChangeChaPart    stEquip;
    stNetChangeChaPart    stEquipSpy;
    stNetShortCut         _stShortCut;

    int				_nFastCur;					// 
    CFastCommand**  _pFastCommands;
	CLabel*			_pActiveFastLabel;			// 

	CLabel*			lblGold;					// 
	CImage*			imgItem4;					// 4

	CImage*			imgLock;
	CImage*			imgUnLock;
	CCharacter* 	eqSpyTarget;
	int 					eqSpyTargetRotate;
	int 					chaModelRotate;
	
	CCharacter* 	chaModel;
	CCharacter* 	spyModel;
	
public:
	bool	refreshChaModel;
	bool	refreshSpyModel;


private:
	struct stThrow
	{
		stThrow() : nX(0), nY(0), nGridID(0), pSelf(0) {}

		int				nX, nY;
		int				nGridID;
		CCharacter* pSelf;
	};
	stThrow		_sThrow;
	static void _evtThrowDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtThrowBoatDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	void		_SendThrowData( const stThrow& sthrow, int nThrowNum=1 );

	struct stUnfix
	{
		stUnfix() : nLink(0), nGridID(0), nX(0), nY(0), pItem(NULL) {}
		void	Reset()	{ nGridID=-1; nLink=-1;	pItem=NULL;}

		int		nLink;
		int		nGridID;
		int		nX, nY;
		CCommandObj* pItem;
	};
	stUnfix		_sUnfix;

	static void _evtUnfixDialogEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	void		_SendUnfixData( const stUnfix& unfix, int nUnfixNum=1 );
	void		_StartUnfix( stUnfix& unfix );


	static void _evtDeleteItemYesNoEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtLockItemYesNoEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
	int			_nLostGridID;
	int rightClickItemIndex{};
	CMenu* rightClickItemMenu{};

	typedef	std::vector<CSkillRecord*>	vskill;
	vskill		_skills;

	typedef std::vector<int>	ints;
	vskill		_cancels;			// 

	typedef std::vector<CSkillStateRecord*>  states;
	states		_charges;			// 

	static CGuiPic	_imgCharges[enumEQUIP_NUM];

	struct SSplitItem
	{
		static void		_evtSplitItemEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);	// 
		int			nFirst;
		int			nSecond;
		CCharacter*	pSelf;
	};
	static SSplitItem	SSplit;

public:	// 
	void	ShowRepairMsg( const char* pItemName, long lMoney );
	void    SetIsShow(bool bShow);

private:
	static void		_evtRepairEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
public:
	CForm*			frmInv;	
};

inline CSkillList* CEquipMgr::GetSkillList( char nType )	
{ 
	switch( nType )
	{
	case enumSKILL_SAIL: return lstSailSkill;
	case enumSKILL_FIGHT: return lstFightSkill;
	default: return lstLifeSkill;
	}
}	

}

