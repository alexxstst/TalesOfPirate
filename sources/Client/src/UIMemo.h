#pragma once
#include "uicompent.h"
#include "uitextbutton.h"
#include "uigraph.h"

#include "uiscroll.h"
#include "netprotocol.h"

#include <array>
namespace GUI
{
	#define MEMO_MAX_LINE			128
	#define MEMO_MAX_ITEM           64 
	#define MEMO_MAX_MISSION        8 
	#define MEMO_MAX_MISNEED        6 
	#define MEMOEX_MAX_PHRASE       16

	// 
	enum MEMO_LINE_TYPE
	{
		MEMO_LINE_TITLE,		// 
		MEMO_LINE_DESP,			// 
		MEMO_LINE_INTERVAL,		// 
		MEMO_LINE_ICON,			// 	
	};

	class CMemo : public CCompent
	{
	public:
		CMemo(CForm& frmOwn);
		CMemo( const CMemo& rhs );
		CMemo& operator=(const CMemo& rhs);
		virtual ~CMemo(void);
		GUI_CLONE(CMemo)

		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun( int x, int y, DWORD key );
		virtual bool IsHandleMouse()					{ return true;								}

		const char*	GetCaption() { return _strCaption.c_str(); }
		void		SetCaption( const char * str ) { _strCaption = str; }
		void		ProcessCaption();

		virtual void	SetAlpha( BYTE alpha ){ _color = (_color & 0x00ffffff) & (alpha << 24); }
		virtual void	SetTextColor( DWORD color ) { _color = color; }
		DWORD			GetTextColor() { return _color; }

		CScroll*	GetScroll()							{ return _pScroll;							}
		void	Init();
		void	AddIcon(int index ,CGraph *pGraph);
		bool	MouseScroll( int nScroll ) ;

	public:
		GuiEvent	evtSelectChange;				// 
		void	SetMaxNumPerRow(int n) { _nMaxNum = n; }
		int		GetMaxNumPerRow() {  return _nMaxNum; } 
		void	SetPageShowNum(int n) {  _nPageShowNum = n; }
		void	SetRowHeight( int v ) { if( v>0) _nRowHeight = v;					}
		int		GetRowHeight() { return _nRowHeight;						}
		void	SetMargin( int left, int top, int right, int bottom );
		void	SetIsHaveItem(bool v ) { _bIsHaveItem = v ;}
		bool	GetIsHaveItem()        {return _bIsHaveItem ;}
		void	SetItemRowNum(int num ) { _nItemRowNum = num  ;}	 
		void	AddItemRowContent(int  row, const char* szFunc, const char* szItemEx="") ;
		void	SetIsHaveMis(bool v ) { _bIsHaveMis = v ;}
		bool	GetIsHaveMis()        {return _bIsHaveMis ;}
		void	SetMisRowNum(int num );
		void	AddMisRowContent(int row, const char* szFunc ) ;
		int		GetSelectItem() {  return  _bIsHaveItem ? _nSelectItem : -1 ; } 
		bool	GetSelectItemText( std::string& item, std::string& itemex );
		int		GetSelectMis()  {  return  _bIsHaveMis?_nSelectMission:-1;      }
		void	reset();

	protected:
		static void	_OnScrollChange(CGuiData *pSender) 
		{
			((CMemo*)(pSender->GetParent()))->_OnScrollChange();
		}
		void	_OnScrollChange();

	protected:
		void	_SetScrollRange();
		void	_Copy( const CMemo& rhs );
		void	_SetSelf();
		void	_CheckTextAlign();			// 
		void	_CheckScroll();			// 

		std::string  _strCaption;
		std::string  _str[MEMO_MAX_LINE];
		bool	_IsTextCenter;

		int		_nLeftMargin;	// 
		int		_nTopMargin;
		int		_nRightMargin;
		int		_nBottomMargin;
		int		_nRowHeight;
		int		_nMaxNum;
		int		_nRowNum;

		bool	_bIsHaveItem;
		bool	_bIsHaveMis;

		int		_nItemRowNum;
		int		_nMisRowNum;
		int		_nMemoNum;
		std::string	_strItem[MEMO_MAX_ITEM];
		std::string	_strItemEx[MEMO_MAX_ITEM];		// ,
		std::string	_strMis[MEMO_MAX_ITEM];
		int		_nSelectItem{ -1 };
		int		_nSelectMission{ -1 };
		void RenderRowAsMessage(int row);
		void RenderRowAsQuest(int row);
		void RenderRowAsSelectedQuest(int row);
		void RenderRowAsFunction(int row);

		bool RowIsMessage(int row) const { return (row >= _nRowInfo[0][0] && row < _nRowInfo[0][1]); }
		bool RowIsQuest(int row) const { return (row >= _nRowInfo[1][0] && row < _nRowInfo[1][1]); }
		bool RowIsFunction(int row) const { return (row >= _nRowInfo[2][0] && row < _nRowInfo[2][1]); }
		std::array<std::array<int, 2>, 3> _nRowInfo = {
			//{FIRST_MESSAGE_ROW, LAST_MESSAGE_ROW}
			//{FIRST_MISSION_ROW, LAST_MISSION_ROW}
			//{FIRST_FUNCTION_ROW, LAST_FUNCTION_ROW}
		};

	private:
		DWORD	_color;
		CScroll*	_pScroll;
		int		_nFirstRow{}, _nLastRow{};
		int		_nPageShowNum;

		struct stIconIndex
		{
			int		nIndex;
			CGraph  *pGraph;
		};

		typedef std::vector<stIconIndex> files;
			files _files;
	};

	class CGraph;
	class CMemoEx : public CCompent
	{
	public:
		CMemoEx(CForm& frmOwn);
		CMemoEx( const CMemoEx& rhs );
		CMemoEx& operator=(const CMemoEx& rhs);
		virtual ~CMemoEx(void);
		GUI_CLONE(CMemoEx)

		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun( int x, int y, DWORD key );
		virtual bool IsHandleMouse() { return true; }

		virtual void SetAlpha( BYTE alpha ){ _color = (_color & 0x00ffffff) & (alpha << 24); }
		virtual void SetTextColor( DWORD color ) { _color = color; }
		DWORD		 GetTextColor() { return _color; }

		CScroll*	GetScroll()	{ return _pScroll; }
		void        Init();

		bool		MouseScroll( int nScroll ) ;
		GuiEvent	evtSelectChange; // 
		GuiItemClickEvent evtClickItem;	// 
		void		SetMaxNumPerRow(int n) { _nMaxNum  = n; }
		int			GetMaxNumPerRow() { return _nMaxNum; } 
		void		SetRowNum(int n) { _nRowNum = n; }
		int			GetRowNum() const { return _nRowNum; }
		void		SetPageShowNum(int n) { _nPageShowNum = n; }
		void		SetRowHeight( int v ) { if( v > 0 ) _nRowHeight = v; }
		int			GetRowHeight() { return _nRowHeight; }
		void		SetMargin( int left, int top, int right, int bottom );
		void		Clear(); 
		void		SetMisPage( const NET_MISPAGE& page );
		BYTE		GetSelPrize() { return m_bySelPrize; }
		void		SetIsSelect( BOOL bSel ) { m_bIsSelect = bSel; }
		BOOL		IsSelPrize();
	protected:
		struct MEMO_DESP_SECTION
		{
			char* pszStart;
			char* pszEnd;
			DWORD dwColor;
			BYTE  byType;
			USHORT sData;
		};

		struct MEMO_INFO
		{
			std::string	strDesp;
			DWORD	dwColor;
			CGraph* pIcon;
			USHORT	sData;
			BYTE	byData;
			BYTE	byType;		  // 
			USHORT	sxPos, syPos; // 
		};

		// 
		USHORT		m_sNumInfo;
		MEMO_INFO	m_MemoInfo[MEMO_MAX_LINE];	// 
		NET_MISPAGE m_PageInfo;					// 
		BOOL		m_bUpdate;					// 

		// Add by lark.li 20080721 begin
		int	m_SelMem;
		// End
		static void		_OnScrollChange(CGuiData *pSender)
		{
			((CMemoEx*)(pSender->GetParent()))->_OnScrollChange();
		}

		void	_OnScrollChange();
		void	_SetScrollRange();
		void	_Copy( const CMemoEx& rhs );
		void	_SetSelf();
		void	_CheckScroll();			// 
		void	ParseMisPage();
		BOOL	ParseScript( char* pszTemp, USHORT& sNumLine, USHORT& sRow, USHORT& sCom, USHORT sStartCom, USHORT sMaxCom, DWORD dwDefColor, MEMO_INFO* pInfo, USHORT sMaxInfo );
		BOOL	SelPrizeItem( int nxPos, int nyPos, DWORD dwKey );
		
		// Add by lark.li 20080721 begin
		int SelMemInfo(int x, int y, DWORD key);
		// End
	private:
		DWORD	_color;				// 
		CScroll*	_pScroll;
		int		_nFirst, _nLast;	// 
		int		_nPageShowNum ;		// 
		int		_nLeftMargin;	// 
		int		_nTopMargin;	// 
		int		_nRightMargin;	// 
		int		_nBottomMargin;	// 
		int		_nRowHeight;
		int     _nTitleHeight;
		int     _nIconHeight;
		int     _nMaxNum;
		int     _nRowNum; 
		int     _nPhraseNum;

		// 
		BYTE	m_bySelPrize;
		CGuiPic* m_pPrizePic;
		CGuiPic* m_pPrizeSelPic;
		BOOL	m_bIsSelect;
	};

	class CImageList
	{
	public:
		CImageList(){};
		virtual ~CImageList(){};
		
		virtual CGraph* GetImage( const char* szImage )						= 0;
		virtual bool	AddImage( const char* szImage, CGraph* pImage )		= 0;
	};

	enum MEMO_COL_TYPE
	{
		COL_ICON			= 0, // 
		COL_TEXT			= 1, // 
	};

	enum MEMO_COLOR_TYPE
	{
		TEXT_COLOR_WIGHT	= 0, // 
		TEXT_COLOR_BLACK	= 1, // 
		TEXT_COLOR_RED		= 2, // 
		TEXT_COLOR_GREEN	= 3, // 
		TEXT_COLOR_BLUE		= 4, // 
		TEXT_COLOR_GRAY		= 5, // 
		TEXT_COLOR_PURPLE	= 6, // 
	};
	
	enum MEMO_FONT_TYPE
	{
		TEXT_FONT_9			= 0, // 9
		TEXT_FONT_10		= 1, // 10
	};

	class CRichMemo : public CCompent
	{
	public:
		CRichMemo(CForm& frmOwn);
		CRichMemo( const CRichMemo& rhs );
		CRichMemo& operator=(const CRichMemo& rhs);
		GUI_CLONE(CRichMemo)
		virtual ~CRichMemo();

		virtual void Init();
		virtual void Render();
		virtual void Refresh();
		virtual bool MouseRun( int x, int y, DWORD key );
		virtual bool IsHandleMouse() { return true; }
	
		// 
		CScroll* GetScroll() { return m_pScroll; }
		bool	MouseScroll( int nScroll );

		// 
		void	OnScrollChange();

		// 		
		void	Clear();
		void	AddText( const char szTitle[], const char szText[], BYTE byColType, BYTE byFontType );
		void	SetClipRect( const RECT& rect ) { m_ShowRect = rect; }
		void	SetMaxLine( USHORT sNum ) { m_sMaxLine = sNum; }
		void	SetImageList( CImageList* pList ) { m_pImageList = pList; }
		void	SetTitleInfo( DWORD dwColor, BYTE byFont ) { m_dwTitleColor = dwColor; m_byTitleFont = byFont; }
		void	SetIntervalDist( BYTE byDist ) { m_byDist = byDist; }
		void	SetAutoScroll( BOOL bAuto ) { m_bAutoScroll = bAuto; }
		void	AutoScroll();		

	protected:
		// 
		struct MEMO_COLINFO
		{
			std::string  strDesp;
			CGraph* pIcon;
			BYTE	byType;
			DWORD	dwColor;	// 
			USHORT  sxPos;
		};

		// 
		typedef std::vector<MEMO_COLINFO> MEMO_COLINFO_ARRAY;

		// 
		struct MEMO_LINE_INFO
		{
			MEMO_COLINFO_ARRAY ColInfoArray;
			BYTE	byFontType; // 
			USHORT  sFontWidth;	// 
			USHORT	sFontHeight;
			USHORT  sHeight;	// 
		};

		// 
		typedef std::list<MEMO_LINE_INFO*> MEMO_LINEINFO_LIST;
			MEMO_LINEINFO_LIST	m_LineList;

		// 
		struct MEMO_SECTION_INFO
		{
			BYTE	byFontType; // 
			USHORT  sFontWidth;	// 
			USHORT	sFontHeight;
			DWORD	dwColor;
			std::string  strTitle;
			std::string	strDesp;
		};

		
		static void	OnScrollChange( CGuiData *pSender )
		{
			((CRichMemo*)(pSender->GetParent()))->OnScrollChange();
		}
		
		void	ParseScript( const char szInfo[], DWORD dwDefColor, BYTE byFont, USHORT sStartCom, USHORT sMaxCom );
		DWORD	GetColor( BYTE byType );
		CGraph* GetImage( const char szImage[] );

	private:
		CImageList*	m_pImageList;   // 
		CScroll*	m_pScroll;		// 
		RECT		m_ShowRect;		// 
		USHORT		m_sMaxLine;		// 
		DWORD		m_dwTitleColor;	// 
		BYTE		m_byTitleFont;	// 
		BYTE		m_byDist;		// 
		BOOL		m_bAutoScroll;  // 
	};


	class CFaceImage : public CImageList
	{
		struct sImage
		{
			std::string name;
			CGraph* pImage;
		};
	public:
		CFaceImage();
		virtual ~CFaceImage();

		virtual CGraph* GetImage( const char* szImage );
		virtual bool	AddImage( const char* szImage, CGraph* pImage );
	private:
		static std::vector<sImage*> m_pData;
	};

}
