#pragma once
#include "UIGlobalVar.h"
#include "UILabel.h"
#include "FindPath.h"
#include "UICheckBox.h"
namespace GUI
{
// 
class CMiniMapMgr : public CUIInterface
{
public:
	CMiniMapMgr() : 
	  frmMinimap(0), MinimatRect(0), labMapPos(0), labMapName(0), frmBigmap(0){}


	// Form
	CCompent*	GetMinimapRect()			{ return MinimatRect;	}

	void		RefreshChaPos( int x, int y );
	void        RefreshMapName (const char * name );

	C3DCompent*	GetBigmapRect()				{ return d3dBigmap;		}

	bool		IsShowBigmap();

	CForm*		GetBigmapForm()				{ return frmBigmap;		}

	void		ClearRadar();

	void        CloseRadar();

	const char* GetMapName()				{ return labMapName->GetCaption(); }

	bool		IsPKSilver();
	bool		IsGuildWar();

	POINT GetMouseCoordinate() const { return mouse_map_coordinate; }

 	void	SetClockString(const char* time) { labClock->SetCaption(time); }
	void	SetClockStringClientSide();

	CLabelEx*		GetLabMapPos(){return labMapPos;}
	static void	ShowRadar(const char * szX,const char * szY);
	void SetupDailyBuffForm(const char* img, const char*labelInfo);
protected:
	virtual bool Init();
    virtual void End();
	virtual void FrameMove(DWORD dwTime);
	virtual void SwitchMap();

private:
	static void _MiniFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _RadarFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	static void _RenderBigmapEvent(C3DCompent *pSender, int x, int y);
	static void _evtShowbigmap(CGuiData *pSender);
	static void _evtHidebigmap(CGuiData *pSender);

	//enter
	static void _evtShowNPCList(CGuiData *pSender,int x,int y ,DWORD key);

	static void _evtRadarEnter(CGuiData *pSender);
	static void _RadarFormBeforeShowEvent(CForm* pForm, bool& IsShow);
	
	static void ShowRadar();

	// () 0,-1
	static int CheckCoordinateEdit(const char* input);

	// 
    void _RenderBigMapHint(void);//Add by sunny.sun 20080903

	//
	CForm*		frmMinimap;
	CCompent*	MinimatRect;

	enum 
	{
		MAP_POS_MAX = 10,
	};
    //
	CLabelEx	*labMapPosRand[MAP_POS_MAX];
	CLabelEx	*labMapPos;
	CLabelEx    *labMapName;
	CLabelEx    *labClock;
	
	CTextButton	*btnQueen;		
	CTextButton	*btnQQ;
	CTextButton	*btnPosSearch;	
	CTextButton *btnSeach;
	
	CCheckBox*	chkID;
	CForm*		frmBigmap;
	C3DCompent*	d3dBigmap;

	static CForm*		frmRadar;
	static CEdit*		edtX;
	static CEdit*		edtY;
	static CCombo*		cboAddr;
	//CMPFont				g_CFont;

	POINT mouse_map_coordinate{};
	time_t tServerTime;
	//daily buff preview form and labels , images @mothannakh 
	CForm* DailyBuffFrm{nullptr}; //main form
	CLabelEx* DailyBufInfo{nullptr}; //label typed hints 
	CImage* DBufImageType{ nullptr }; //the image info preview
};

}

