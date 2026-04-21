//----------------------------------------------------------------------
//	2005/4/13	Arcol	Extended and added interface: UI_SetFormStyleEx
//----------------------------------------------------------------------
#include "stdafx.h"
#include "script.h"
#include <LuaBridge.h>
#include "FontManager.h"
#include "MPFont.h"
#include "uiguidata.h"
#include "UIScript.h"
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
#include "sceneobjset.h"
#include "LuaInterface.h"
#include "ui3dcompent.h"
#include "UIPicList.h"
#include "uitextparse.h"
#include "uimemo.h"
#include "uigoodsgrid.h"
#include "uifastcommand.h"
#include "UIHeadSay.h"
#include "UISkillList.h"
#include "CharacterRecord.h"
#include "MapRecordStore.h"
#include "EffectRecordStore.h"
#include "uimenu.h"
#include "uiCozeform.h"
#include "UIChat.h"
#include "StringLib.h"
#include "uititle.h"
#include "uiequipform.h"
#include "uirichedit.h"

#include "crypto_facade.h"
#include <fstream>

#include <Windows.h>  //Lark.li

using namespace GUI;

static UIScript<CItemObj> g_ItemScript;

static CList* GetList( int list_id )
{
	CGuiData* p = CGuiData::GetGui( list_id );
	if( !p ) return NULL;

    CList *f = dynamic_cast<CList*>(p);
    if( f )
    {
        return f;
    }

    CListView *l = dynamic_cast<CListView*>(p);
    if( l )
    {
		return l->GetList();
    }

    return NULL;
}
//---------------------------------------------------------------------------
// UI_Script
//---------------------------------------------------------------------------
int UI_LoadScript( const std::string& file)
{
	LoadLuaScript(g_LuaState, file.c_str());
	return R_OK;
}

enum eCompentType
{
	LABEL_TYPE		= 0,
	LABELEX_TYPE	= 1,
	BUTTON_TYPE		= 2,
	COMBO_TYPE		= 3,
	EDIT_TYPE		= 4,
	IMAGE_TYPE		= 5,
	LIST_TYPE		= 6,
	PROGRESS_TYPE	= 7,
	CHECK_TYPE		= 8,
	CHECK_GROUP_TYPE= 9,
	GRID_TYPE		= 10,
	PAGE_TYPE		= 11,
	FIX_LIST_TYPE	= 12,
	CHECK_FIX_LIST_TYPE	= 13,
	DRAG_TITLE_TYPE		= 14,
	TREE_TYPE			= 15,
	IMAGE_FRAME_TYPE	= 16,
    UI3D_COMPENT_TYPE	= 17,
	MEMO_TYPE			= 18,
	MEMOEX_TYPE			= 19,	
	GOODS_GRID_TYPE		= 20,
	FAST_COMMANG_TYPE	= 21,
	COMMAND_ONE_TYPE	= 22,
	IMAGE_FLASH_TYPE	= 23,
	SCROLL_TYPE			= 24,
    SKILL_LIST_TYPE		= 25,

	LISTEX_TYPE			= 26,
	MENU_TYPE			= 27,
	RICHMEMO_TYPE		= 28,
	TITLE_TYPE			= 29,
	RICHEDIT_TYPE		= 30,

	AMPHI_LIST_TYPE		= 31,//ADD by sunny.sun20080718

	GUI_END,
};

enum eHotKey
{
	ALT_KEY = 0,
	CTRL_KEY,
	SHIFT_KEY
};

int UI_CreateCompent( int formId, int type, const std::string& pszName, int w, int h, int x, int y )
{
	CForm *f = dynamic_cast<CForm*>(CGuiData::GetGui( formId ));
	if( !f ) return R_FAIL;

	CGuiData* g = NULL;
	switch( type )
	{
	case BUTTON_TYPE:   g = new CTextButton( *f );  break;
	case COMBO_TYPE:    g = new CCombo( *f );   break;
	case EDIT_TYPE:     g = new CEdit( *f );    break;
	case IMAGE_TYPE:    g = new CImage( *f );   break;
	case LABEL_TYPE:    g = new CLabel( *f );   break;
	case LABELEX_TYPE:  g=new CLabelEx( *f );   break;
	case LIST_TYPE:     g = new CList( *f );    break;
	case PROGRESS_TYPE: g = new CProgressBar( *f ); break;
	case CHECK_TYPE:    g = new CCheckBox( *f );    break;
	case CHECK_GROUP_TYPE: g = new CCheckGroup( *f ); break;
	case GRID_TYPE:	    g = new CGrid(*f);  break;
	case PAGE_TYPE:     g = new CPage(*f);  break;
	case FIX_LIST_TYPE: g = new CFixList(*f);   break;
	case CHECK_FIX_LIST_TYPE:   g = new CCheckFixList(*f);  break;
	case DRAG_TITLE_TYPE:       g = new CDragTitle(*f);     break;
	case TREE_TYPE:     g = new CTreeView(*f);  break;
	case IMAGE_FRAME_TYPE:  g = new CFrameImage(*f);    break;		
    case UI3D_COMPENT_TYPE: g = new C3DCompent(*f);     break;
	case MEMO_TYPE :        g = new CMemo(*f);      break;
	case MEMOEX_TYPE:       g = new CMemoEx(*f); break;
	case RICHMEMO_TYPE:		g = new CRichMemo(*f); break;
	case GOODS_GRID_TYPE:   g = new CGoodsGrid(*f);   break;
	case FAST_COMMANG_TYPE: g = new CFastCommand(*f); break;
	case COMMAND_ONE_TYPE:  g = new COneCommand(*f);  break;
	case IMAGE_FLASH_TYPE:  g = new CFlashImage(*f);    break;
	case SCROLL_TYPE:       g = new CScroll(*f);        break;
	case SKILL_LIST_TYPE:   g = new CSkillList(*f);     break;
	case MENU_TYPE:			g = new CMenu( f );			break;
	case TITLE_TYPE:		g = new CTitle( *f );		break;
	case RICHEDIT_TYPE:		g = new CRichEdit( *f );	break;
	default: return R_FAIL;
	}

	g->SetName( pszName.c_str() );
	g->SetPos( x, y );
	g->SetSize( w, h );
	return g->GetID();
}

int UI_SetFormTempleteMax( int max )
{
	if( CFormMgr::s_Mgr.SetFormTempleteMax( max ) )
		return R_OK;

	return R_FAIL;
}

// Resolve image path: prefer unencrypted original, decrypt .wsd → .dec if needed
// Returns the path to use for loading (original or .dec)
static std::string ResolveImagePath(const std::string& originalPath) {
	// If original file exists (.png/.tga/.bmp/.dds) — use it directly
	struct stat st;
	if (stat(originalPath.c_str(), &st) == 0) {
		return originalPath;
	}

	// Check if encrypted .wsd version exists
	std::string wsdPath = originalPath;
	if (wsdPath.length() > 3) {
		wsdPath.replace(wsdPath.length() - 3, 3, "wsd");
	}

	if (stat(wsdPath.c_str(), &st) != 0) {
		// Neither original nor .wsd exists — return original path, let engine handle the error
		return originalPath;
	}

	// Decrypt .wsd → .dec
	std::string decPath = originalPath;
	if (decPath.length() > 3) {
		decPath.replace(decPath.length() - 3, 3, "dec");
	}

	// If .dec already exists from a previous run — use it
	if (stat(decPath.c_str(), &st) == 0) {
		return decPath;
	}

	// Decrypt
	const unsigned char imgTableKey[] = { 0x48, 0x73, 0x29, 0xCA, 0xBB, 0x54, 0xCF, 0xB0, 0xF4, 0xBF, 0x70, 0xA0, 0xAA, 0x4B, 0x12, 0xF5 };
	const unsigned char imgTableIV[] = { 0x43, 0x2a, 0x46, 0x29, 0x4a, 0x40, 0x4e, 0x63, 0x52, 0x66, 0x55, 0x6a, 0x58, 0x6e, 0x32, 0x72 };

	if (!crypto::AesGcmDecryptFile(wsdPath, decPath, imgTableKey, imgTableIV)) {
		ToLogService("lua", LogLevel::Error, "Failed to decrypt '{}'", wsdPath);
		return originalPath;
	}

	// Delete encrypted .wsd after successful decryption
	remove(wsdPath.c_str());
	return decPath;
}

int UI_AddAllFormTemplete( int form_id )
{
	CForm* p = dynamic_cast<CForm*>(CGuiData::GetGui( form_id ));
	if( !p ) return R_FAIL;

	int n=CFormMgr::s_Mgr.GetFormTempleteMax();
	for( int i=0; i<n; ++i )
		CFormMgr::s_Mgr.AddForm( p, i );

	return R_OK;
}

int UI_AddFormToTemplete( int formid, int nTempleteNo )
{
	CForm* p = dynamic_cast<CForm*>(CGuiData::GetGui( formid ));
	if( !p ) return R_FAIL;
	
	if( CFormMgr::s_Mgr.AddForm( p, nTempleteNo ) )
		return R_OK;

	return R_FAIL;
}

int UI_SwitchTemplete( int nTempleteNo )
{
	if( CFormMgr::s_Mgr.SwitchTemplete( nTempleteNo ) )
		return R_OK;

	return R_FAIL;
}

int UI_CreateForm(const std::string& pszName, int isModal, int w, int h, int x, int y, int isTitle, int isShowFrame )
{
	CForm *f = new CForm();
	f->SetName( pszName.c_str() );
	f->SetSize( w, h );
	f->SetPos( x, y );
	f->GetFrameImage()->SetIsTitle( isTitle!=0 ? true: false );
	f->GetFrameImage()->SetIsShowFrame( isShowFrame!=0 ? true: false );
//#if _DEBUG
//	char buffer[255];
//	::OutputDebugStr(buffer);
//#endif
	return f->GetID();
}

int UI_FormSetIsEscClose( int nFormID, int IsEscClose )
{
	CForm* p = dynamic_cast<CForm*>(CGuiData::GetGui( nFormID ));
	if( !p ) return R_FAIL;
	
	p->SetIsEscClose( IsEscClose ? true : false );
	return R_OK;
}

int UI_FormSetEnterButton( int nFormID, int nButtonID )
{
	CForm* p = dynamic_cast<CForm*>(CGuiData::GetGui( nFormID ));
	if( !p ) return R_FAIL;

	CTextButton* b = dynamic_cast<CTextButton*>(CGuiData::GetGui( nButtonID ));
	if( !b ) return R_FAIL;

	if( b->GetForm()!=p ) return R_FAIL;

	p->SetEnterButton( b );
	return R_OK;
}

int UI_FormSetHotKey( int id, int control_key, int key )
{
	CForm* p = dynamic_cast<CForm*>(CGuiData::GetGui( id ));
	if( !p ) return R_FAIL;

	p->SetHotKey(key);
	return R_OK;
}

int UI_SetIsDrag( int id, int isDrag )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	p->SetIsDrag( isDrag!=0 );
	return R_OK;
}

int UI_LoadFormImage( int id, const std::string& client_in, int cw, int ch, int tx, int ty, const std::string& file_in, int w, int h )
{
	std::string resolvedClient = ResolveImagePath(client_in);
	std::string resolvedFrame = ResolveImagePath(file_in);

	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CForm *f = dynamic_cast<CForm*>(p);
	if( !f ) return R_FAIL;

	f->GetFrameImage()->LoadImage( resolvedClient.c_str(), cw, ch, tx, ty, resolvedFrame.c_str(), w, h );
	return R_OK;
}

int UI_LoadFrameImage( int id, const std::string& client, int cw, int ch, int tx, int ty, const std::string& file, int w, int h )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CFrameImage *f = dynamic_cast<CFrameImage*>(p);
	if( !f ) return R_FAIL;

	f->GetFrameImage()->LoadImage( client.c_str(), cw, ch, tx, ty, file.c_str(), w, h );
	return R_OK;
}

int UI_ShowForm(int id, int show)
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CForm *f = dynamic_cast<CForm*>(p);
	if( !f ) return R_FAIL;

	f->ScriptSetShow( show ? true : false );
	return R_OK;
}

int UI_CreateListView( int formId, const std::string& pszName, int w, int h, int x, int y, int col, int style )
{
	CForm *f = dynamic_cast<CForm*>(CGuiData::GetGui( formId ));
	if( !f ) return R_FAIL;

	CListView * g = new CListView(*f, col, (CListView::eStyle)style);
	g->SetName( pszName.c_str() );
	g->SetPos( x, y );
	g->SetSize( w, h );

	return g->GetID();
}

int UI_ListViewSetTitle( int listviewid, int index, int width, const std::string& titleimage, int w, int h, int sx, int sy )
{
	CListView *f = dynamic_cast<CListView*>(CGuiData::GetGui( listviewid ));
	if( !f ) return R_FAIL;

	f->GetTitle()->SetColumnWidth( index, width );
	CImage* p = dynamic_cast<CImage*>(f->GetColumnImage(index));
	if( p ) p->GetImage()->LoadImage( titleimage.c_str(), w, h, 0, sx, sy );

	CListItems *pItem = f->GetList()->GetItems();
	pItem->SetColumnWidth( index, width );
	return R_OK;
}

int  UI_ListViewSetTitleHeight( int listviewid, int height )
{
    CListView *f = dynamic_cast<CListView*>(CGuiData::GetGui( listviewid ));
    if( !f ) return R_FAIL;

    f->SetColumnHeight( height );
    return R_OK;
}

int UI_SetListIsMouseFollow( int list, int IsFollow )
{
	CList* l = GetList( list );
	if( l )
	{
		l->GetItems()->SetIsMouseFollow( IsFollow ? true : false );
        return R_OK;
	}
    return R_FAIL;
}

int UI_SetListFontColor( int list, unsigned int nBackColor, unsigned int nSelectColor )
{	
	CList* l = GetList( list );
	if( l )
	{
		l->SetFontColor( nBackColor );
		l->SetSelectColor( nSelectColor );
        return R_OK;
	}
    return R_FAIL;
}

int UI_LoadListFixSelect( int id, const std::string& imagefile, int w, int h, int sx, int sy )
{
	CFixList *f = dynamic_cast<CFixList*>(CGuiData::GetGui( id ));
	if( !f ) return R_FAIL;

	f->GetSelectImage()->LoadImage(imagefile.c_str(), w, h, 0, sx, sy);
	return R_OK;
}

// Load CheckFixList check images
int UI_LoadCheckFixListCheck( int id, const std::string& checkimage, int cw, int ch, int csx, int csy
							 , const std::string& uncheckimage, int uw, int uh, int usx, int usy )
{
	CCheckFixList *f = dynamic_cast<CCheckFixList*>(CGuiData::GetGui( id ));
	if( !f ) return R_FAIL;

	f->GetCheckImage()->LoadImage(checkimage.c_str(), cw, ch, 0, csx, csy);
	f->GetUnCheckImage()->LoadImage(uncheckimage.c_str(), uw, uh, 0, usx, usy);
	return R_OK;
}

int UI_SetSize( int id, int w, int h )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	p->SetSize( w, h );
	return R_OK;
}

int UI_SetPos( int id, int x, int y )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	p->SetPos( x, y );
	return R_OK;
}

int UI_GetScroll( int id )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CList * list = dynamic_cast<CList*>(p);
	if( list ) return list->GetScroll()->GetID();

	CListView * view = dynamic_cast<CListView*>(p);
	if( view ) return view->GetList()->GetScroll()->GetID();

//	CListEx * lst = dynamic_cast<CListEx*>(p);
//	if( lst ) return lst->GetScroll()->GetID();


	CCombo * combo = dynamic_cast<CCombo*>(p);
	if( combo ) return combo->GetList()->GetScroll()->GetID();

	CTreeView * tree = dynamic_cast<CTreeView*>(p);
	if( tree ) return tree->GetScroll()->GetID();

	CGoodsGrid * grid = dynamic_cast<CGoodsGrid*>(p);
	if( grid ) return grid->GetScroll()->GetID();

	CMemo *memo   = dynamic_cast<CMemo *>(p);
	if( memo ) return memo->GetScroll()->GetID();

	CMemoEx *memoex   = dynamic_cast<CMemoEx *>(p);
	if( memoex ) return memoex->GetScroll()->GetID();

	CRichMemo* richmemo = dynamic_cast<CRichMemo *>(p);
	if( richmemo ) return richmemo->GetScroll()->GetID();

    CSkillList* skill = dynamic_cast<CSkillList*>(p);
    if( skill ) return skill->GetScroll()->GetID();

	return R_FAIL;
}



int UI_SetScrollStyle( int id, int style )
{
	CGuiData* t = CGuiData::GetGui( id );
	if( !t ) return R_FAIL;

	CScroll* s = dynamic_cast<CScroll*>(t);
	if( !s ) return R_FAIL;

	s->SetStyle( (CScroll::eStyle)style );
	return R_OK;
}


int UI_GetList( int id )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CCombo * combo = dynamic_cast<CCombo*>(p);
	if( combo ) return combo->GetList()->GetID();

	CListView * v = dynamic_cast<CListView*>(p);
	if( v ) return v->GetList()->GetID();

	CList * list = dynamic_cast<CList*>(p);
	if( list ) return list->GetID();

	return R_FAIL;
}

enum 
{
	SCROLL_UP = 0,
	SCROLL_DOWN,
	SCROLL_SCROLL,
	EXLIST_BUTTON,
};

int UI_GetScrollObj( int id, int scrolltype )
{
	CScroll* p = dynamic_cast<CScroll*>(CGuiData::GetGui( id ));
	if( !p ) return R_FAIL;

	switch( scrolltype )
	{
	case SCROLL_UP: return p->GetUp()->GetID();
	case SCROLL_DOWN: return p->GetDown()->GetID();
	case SCROLL_SCROLL: return p->GetScroll()->GetID();
	}
	return R_FAIL;
}

int UI_LoadComboImage( int id, const std::string& edit, int ew, int eh, int ex, int ey, const std::string& button, int bw, int bh, int bx, int by, int isHorizontal )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CCombo * combo = dynamic_cast<CCombo*>(p);
	if( !combo ) return R_FAIL;

	combo->GetEdit()->GetImage()->LoadImage( edit.c_str(), ew, eh, 0, ex, ey );
	combo->GetEdit()->SetSize(ew,eh);
	combo->GetButton()->LoadImage( button.c_str(), bw, bh, bx, by, isHorizontal!=0 );
	combo->GetButton()->SetSize(bw,bh);
	return R_OK;
}

int UI_ComboSetStyle( int id, int style )
{
	CCombo * combo = dynamic_cast<CCombo*>(CGuiData::GetGui( id ));
	if( !combo ) return R_FAIL;

	combo->SetListStyle( (CCombo::eListStyle) style );
	return R_OK;
}

int UI_ComboSetTextColor( int id, unsigned int color )
{
	CCombo * combo = dynamic_cast<CCombo*>(CGuiData::GetGui( id ));
	if( !combo ) return R_FAIL;

	combo->GetEdit()->SetTextColor( color );
	return R_OK;
}

int UI_LoadButtonImage( int id, const std::string& file, int w, int h, int sx, int sy, int isHorizontal )
{
	std::string resolved = ResolveImagePath(file);

	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CTextButton * b = dynamic_cast<CTextButton*>(p);
	if( !b ) return R_FAIL;

	b->LoadImage( resolved.c_str(), w, h, sx, sy, isHorizontal!=0 ? true: false );
	//b->GetImage()->TintColour( 131, 188, 225 );
	return R_OK;
}

int UI_LoadImageUnencrypted(int id, const std::string& file, int frame, int w, int h, int tx, int ty)
{
	CGuiData* p = CGuiData::GetGui(id);
	if (!p) return R_FAIL;

	CGuiPic* img = p->GetImage();
	if (!img) return R_FAIL;

	if (img->LoadImage(file.c_str(), w, h, frame, tx, ty)) {
		//img->TintColour( 131, 188, 225 );
		return R_OK;
	}
	return R_FAIL;
}


int UI_LoadImage( int id, const std::string& file, int frame, int w, int h, int tx, int ty )
{
	if (file.empty())
	{
		return R_FAIL;
	}

	std::string resolved = ResolveImagePath(file);

	CGuiData* p = CGuiData::GetGui(id);
	if (!p) return R_FAIL;

	CGuiPic* img = p->GetImage();
	if (!img) return R_FAIL;

	if (img->LoadImage(resolved.c_str(), w, h, frame, tx, ty)) {
		//img->TintColour( 131, 188, 225 );
		return R_OK;
	}
	return R_FAIL;
}

// Load scaled image
int UI_LoadScaleImage( int id, const std::string& file, int frame, int w, int h, int tx, int ty, float scalex, float scaley )
{
	std::string resolved = ResolveImagePath(file);

	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CGuiPic* img = p->GetImage();
	if( !img ) return R_FAIL;

	if( img->LoadImage( resolved.c_str(), w, h, frame, tx, ty, scalex, scaley ) ) return R_OK;

	return R_FAIL;
}
// Load flash scaled image
int UI_LoadFlashScaleImage( int id,int flash , const std::string& file, int frame, int w, int h, int tx, int ty, float scalex, float scaley )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;
	
	CProgressBar * pro = dynamic_cast<CProgressBar*>(p);
	if( !pro ) return R_FAIL;
	pro->SetFlashNum( flash );
	
	CGuiPic* img = p->GetImage();
	if( !img ) return R_FAIL;
	
	if( img->LoadImage( file.c_str(), w, h, frame, tx, ty, scalex, scaley ) ) return R_OK;
	return R_FAIL;
}

int UI_SetIsEnabled( int id, int isEnabled )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	p->SetIsEnabled( isEnabled!=0?true:false );
	return R_OK;
}

int UI_SetTag( int id, int tag )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	p->nTag = tag;
	return R_OK;
}

int UI_CopyImage( int targetid, int sourceid )
{
	CGuiData* t = CGuiData::GetGui( targetid );
	if( !t ) return R_FAIL;

	CGuiPic* pt = t->GetImage();
	if( !pt ) return R_FAIL;

	CGuiData* s = CGuiData::GetGui( sourceid );
	if( !s ) return R_FAIL;

	CGuiPic* ps = s->GetImage();
	if( !ps ) return R_FAIL;

	*ps = *pt;
	return R_OK;
}

int UI_CopyCompent( int targetid, int sourceid )
{
	CGuiData* t = CGuiData::GetGui( targetid );
	if( !t ) return R_FAIL;

	CGuiData* s = CGuiData::GetGui( sourceid );
	if( !s ) return R_FAIL;

	t->Clone( s );

	// const type_info& p = typeid(t);

	return R_OK;
}

int UI_SetIsShow( int id, int isshow )
{
	CGuiData* t = CGuiData::GetGui( id );
	if( !t ) return R_FAIL;

	t->ScriptSetShow( isshow!=0 ? true: false );
	return R_OK;
}

int UI_SetAlign( int id, int align )
{
	CCompent* s = dynamic_cast<CCompent*>( CGuiData::GetGui( id ) );
	if( !s ) return R_FAIL;

	s->SetAlign( (eCompentAlign)align );
	return R_OK;
}

int UI_SetIsKeyFocus( int id, int IsKeyFocus )
{
	CCompent* s = dynamic_cast<CCompent*>( CGuiData::GetGui( id ) );
	if( !s ) return R_FAIL;

	s->SetIsFocus( IsKeyFocus!=FALSE );
	return R_OK;
}

int UI_SetButtonModalResult( int id, int modal )
{
    CTextButton* g = dynamic_cast<CTextButton*>(CGuiData::GetGui( id ));
    if( !g ) return R_FAIL;

    g->SetFormModal( (CForm::eModalResult)modal );
    return R_OK;
}

int UI_SetLabelExFont( int id, int nFontIndex, int IsShadow, unsigned int dwShadowColor )
{
	CLabelEx* g = dynamic_cast<CLabelEx*>(CGuiData::GetGui( id ));
	if( !g ) return R_FAIL;

	g->SetIsShadow( IsShadow != 0 );
	g->SetFont( nFontIndex );
	g->SetShadowColor( (DWORD)dwShadowColor );
	return R_OK;
}

int UI_SetTitleFont( int id, int nFontIndex, unsigned int color, int h )
{
	CTitle* g = dynamic_cast<CTitle*>(CGuiData::GetGui( id ));
	if( !g ) return R_FAIL;

	g->SetFont( nFontIndex );
	g->SetTextColor( color );
	g->SetFontH( h );
	return R_OK;
}

int UI_ButtonSetHint( int id, const std::string& hint )
{
	CTextButton * b = dynamic_cast<CTextButton*>( CGuiData::GetGui( id ) );
	if( !b ) return R_FAIL;

	b->SetHint( hint.c_str() );
	return R_OK;
}

int UI_SetMaxImage( int id, int max )
{
    CGuiData* t = CGuiData::GetGui( id );
    if( !t ) return R_FAIL;

    CGuiPic* p = t->GetImage();
    if( p )
    {
        p->SetMax( max );
    }
    return R_OK;
}

int UI_SetHint( int id, const std::string& hint )
{
    CGuiData* t = CGuiData::GetGui( id );
    if( !t ) return R_FAIL;

    t->SetHint( hint.c_str() );
    return R_OK;
}

int UI_SetProgressStyle( int id, int style )
{
	CGuiData* t = CGuiData::GetGui( id );
	if( !t ) return R_FAIL;

	CProgressBar* s = dynamic_cast<CProgressBar*>(t);
	if( !s ) return R_FAIL;

	s->SetStyle( (CProgressBar::eStyle)style );
	return R_OK;
}

int UI_AddListText( int id, const std::string& text )
{
	CList* s = GetList( id );
	if( !s ) return R_FAIL;
	
	s->Add( text.c_str() );
	return R_OK;
}

int UI_ListSetItemMargin( int id,  int left, int top )
{
	CList* s = GetList( id );
	if( !s ) return R_FAIL;

	s->GetItems()->SetItemMargin( left, top );
	return R_OK;
}

int UI_ListSetItemImageMargin( int id,  int left, int top )
{
	CList* s = GetList( id );
	if( !s ) return R_FAIL;

	s->GetItems()->SetImageMargin( left, top );
	return R_OK;
}

int UI_AddListBarText( int id, const std::string& text, float prgress )
{
	CList* s = GetList( id );
	if( !s ) return R_FAIL;

	CItemRow* item = s->GetItems()->NewItem();
	CItemBar* bar = new CItemBar;
	bar->SetString( text.c_str() );
	item->SetBegin( bar );
	bar->SetScale( prgress );
	return R_OK;

}

int UI_AddGroupBox( int id, int checkbox )
{
	CGuiData* t = CGuiData::GetGui( id );
	if( !t ) return R_FAIL;

	CGuiData* b = CGuiData::GetGui( checkbox );
	if( !b ) return R_FAIL;

	CCheckBox* box = dynamic_cast<CCheckBox*>(b);
	if( !box ) return R_FAIL;

	CCheckGroup* s = dynamic_cast<CCheckGroup*>(t);
	if( !s ) return R_FAIL;

	s->AddCheckBox( box );
	return R_OK;
}

int UI_SetEditEnterButton( int nEditID, int nButtonID )
{
	CEdit* t = dynamic_cast<CEdit*>(CGuiData::GetGui( nEditID ));
	if( !t ) return R_FAIL;

	CTextButton* b = dynamic_cast<CTextButton*>(CGuiData::GetGui( nButtonID ));
	if( !b ) return R_FAIL;

	t->SetEnterButton( b );
	return R_OK;
}

int UI_SetEditCursorColor( int nEditID, unsigned int color )
{
	CEdit* t = dynamic_cast<CEdit*>(CGuiData::GetGui( nEditID ));
	if( !t ) return R_FAIL;

	t->SetCursorColor( color );
	return R_OK;
}

int UI_SetEditMaxNum( int id, int num )
{
	CEdit* t = dynamic_cast<CEdit*>(CGuiData::GetGui( id ));
	if( !t ) return R_FAIL;

	t->SetMaxNum( num );
	return R_OK;
}


int UI_SetEditMaxNumVisible( int id, int num )
{
	CEdit* t = dynamic_cast<CEdit*>(CGuiData::GetGui( id ));
	if( !t ) return R_FAIL;

	t->SetMaxNumVisible( num );
	return R_OK;
}


int UI_SetTextColor( int id, unsigned int color )
{
	CGuiData* t = CGuiData::GetGui( id );
	if( !t ) return R_FAIL;

	t->SetTextColor( color );
	return R_OK;
}

int UI_SetListRowHeight( int id, int height )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CList* lst = dynamic_cast<CList*>(p);
	if( lst ) 
	{
		lst->SetRowHeight( height );
		return R_OK;
	}

	CListView * lsv = dynamic_cast<CListView*>(p);
	if( lsv )
	{
		lsv->GetList()->SetRowHeight( height );
		return R_OK;
	}

	CFixList* fls = dynamic_cast<CFixList*>(p);
	if( fls ) 
	{
		fls->SetRowHeight( height );
		return R_OK;
	}

	CSkillList* skill = dynamic_cast<CSkillList*>(p);
	if( skill ) 
	{
		skill->SetRowHeight( height );
		return R_OK;
	}
	return R_FAIL;
}

int UI_ListLoadSelectImage( int id, const std::string& file, int w, int h, int sx, int sy )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CListView* pView = dynamic_cast<CListView*>(p);
	CList* lst = NULL;
	if( pView )
	{
		lst = pView->GetList();
	}

	if( !lst ) lst = dynamic_cast<CList*>(p);
	if( lst ) 
	{
		lst->GetItems()->GetSelect()->GetImage()->LoadImage( file.c_str(), w, h, 0, sx, sy, 1.0f, 1.0f );
		return R_OK;
	}

    CSkillList* skill = dynamic_cast<CSkillList*>(p);
    if( skill )
    {
        skill->GetSelect()->LoadImage ( file.c_str(), w, h, 0, sx, sy, 1.0f, 1.0f );
        return R_OK;
    }
    return R_FAIL;
}

int UI_LoadSkillListButtonImage( int id, const std::string& file, int w, int h, int sx, int sy, int item_w, int item_h )
{
	CSkillList* p = dynamic_cast<CSkillList*>( CGuiData::GetGui( id ) );
	if( !p ) return R_FAIL;

	CGuiPic* pic = p->GetButtonImage();
    pic->LoadImage( file.c_str(), w, h, 0, sx, sy, 1.0f, 1.0f );
    pic->LoadImage( file.c_str(), w, h, 1, sx + w, sy, 1.0f, 1.0f );
    pic->LoadImage( file.c_str(), w, h, 2, sx + 2 * w, sy, 1.0f, 1.0f );
    pic->LoadImage( file.c_str(), w, h, 3, sx + 3 * w, sy, 1.0f, 1.0f );

	pic->SetScale( item_w, item_h );
	return R_OK;
}

int UI_LoadListItemImage( int id, const std::string& file, int w, int h, int sx, int sy, int item_w, int item_h )
{
	CList* lst = GetList( id );
	if( !lst ) return R_FAIL;

	lst->GetItemImage()->LoadImage( file.c_str(), w, h, 0, sx, sy );
	lst->GetItemImage()->SetScale( item_w, item_h );
	return R_OK;
}

int UI_FixListSetRowSpace( int id, int height )
{
	CFixList* fls = dynamic_cast<CFixList*>(CGuiData::GetGui( id ));
	if( fls ) 
	{
		fls->SetRowSpace( height );
		return R_OK;
	}

	return R_FAIL;
}

// Set CheckFixList check display margin
int UI_CheckFixListSetCheckMargin( int id, int left, int top )
{
	CCheckFixList* fls = dynamic_cast<CCheckFixList*>(CGuiData::GetGui( id ));
	if( fls ) 
	{
		fls->SetCheckMargin( left, top );
		return R_OK;
	}

	return R_FAIL;
}

int UI_SetMargin( int id, int left, int top, int right, int bottom )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	p->SetMargin( left, top, right, bottom );
	return R_OK;
}

int UI_SetImageAlpha( int id, int alpha )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CGuiPic* pic = p->GetImage();
	if( pic ) pic->SetAlpha( alpha );
	return R_OK;
}

int UI_SetAlpha( int id, int alpha )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	p->SetAlpha( alpha );
	return R_OK;
}

int UI_GridLoadSelectImage( int id, const std::string& file, int w, int h, int tx, int ty )
{
	CGrid* g = dynamic_cast<CGrid*>( CGuiData::GetGui( id ) );
	if( !g ) return R_FAIL;

	g->GetSelectImage()->GetImage()->LoadImage( file.c_str(), w, h, 0, tx, ty );
	return R_OK;
}

int UI_SetGridIsDragSize( int id, int IsEnabled )
{
    CGrid* g = dynamic_cast<CGrid*>(CGuiData::GetGui( id ));
    if( !g ) return R_FAIL;

    g->SetIsDragSize( false );
    return R_OK;
}

int UI_SetGridSpace( int id, int x, int y )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CGoodsGrid* d = dynamic_cast<CGoodsGrid*>(p);
	if( d ) 
	{
		d->SetSpace( x, y );
		return R_OK;
	}

    CGrid* g = dynamic_cast<CGrid*>(p);
    if( g )
    {
        g->SetSpace( x, y );
        return R_OK;
    }

	return R_FAIL;
}

int UI_SetGridContent( int id, int nRow, int nCol )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CGoodsGrid* d = dynamic_cast<CGoodsGrid*>(p);
	if( d ) 
	{
		d->SetContent( nRow, nCol );
		return R_OK;
	}

	return R_FAIL;
}

int UI_SetGridUnitSize( int id, int w, int h )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CGrid* g = dynamic_cast<CGrid*>(p);
	if( g ) 
	{
		g->SetUnitSize( w, h );
		return R_OK;
	}

	CGoodsGrid* d = dynamic_cast<CGoodsGrid*>(p);
	if( d ) 
	{
		d->SetUnitSize( w, h );
		return R_OK;
	}

	return R_FAIL;
}

int UI_GoodGridLoadUnitImage( int id, const std::string& file, int w, int h, int tx, int ty )
{
	CGoodsGrid* g = dynamic_cast<CGoodsGrid*>( CGuiData::GetGui( id ) );
	if( !g ) return R_FAIL;

	g->GetUnitImage()->LoadImage( file.c_str(), w, h, 0, tx, ty );
	return R_OK;
}

int UI_AddFaceToGrid( int id, const std::string& file, int w, int h, int sx, int sy, int frame, int nTag )
{
	CGrid* g = dynamic_cast<CGrid*>(CGuiData::GetGui( id ));
	if( !g ) return R_FAIL;

	CGraph *p = new CGraph(file.c_str(), w, h, sx, sy, frame);
	p->GetImage()->SetScale( g->GetUnitWidth(), g->GetUnitHeight() );


	p->nTag = nTag;
	g->Add(p);
	return R_OK;
}

// Set maximum item count
int UI_FixListSetMaxNum( int id, int num )
{
	CFixList* g = dynamic_cast<CFixList*>(CGuiData::GetGui( id ));
	if( !g ) return R_FAIL;

	g->SetMaxNum( num );
	return R_OK;
}

// Set text corresponding to each row
int UI_FixListSetText( int id, int index, const std::string& text )
{
	CFixList* g = dynamic_cast<CFixList*>(CGuiData::GetGui( id ));
	if( !g ) return R_FAIL;

	g->SetString( index, text.c_str() );
	return R_OK;
}

enum eTreeImage
{
	enumTreeAddImage=0,
	enumTreeSubImage,
	enumTreeNodeImage,
};
int UI_TreeLoadImage( int nTreeID, int nType, const std::string& imagefile, int w, int h, int sx, int sy, int itemw, int itemh )
{
	CTreeView *f = dynamic_cast<CTreeView*>(CGuiData::GetGui( nTreeID ));
	if( !f ) return R_FAIL;

	auto pic = [&]() -> CGuiPic* {
		switch (nType)
		{
		case enumTreeAddImage: return f->GetAddImage();
		case enumTreeSubImage: return f->GetSubImage();
		case enumTreeNodeImage: return f->GetNodeImage();
		default: return nullptr;
		}
	}();

	if (pic)
	{
		std::string resolved = ResolveImagePath(imagefile);
		pic->LoadImage(resolved.c_str(), w, h, 0, sx, sy);
		pic->SetScale(itemw, itemh);
		return R_OK;
	}

	return R_FAIL;
}

int UI_TreeSetNodeTextXY(int treeID, int textX, int textY)
{
	auto* f = dynamic_cast<CTreeView*>(CGuiData::GetGui(treeID));
	if (!f) return R_FAIL;

	f->GetNodeOptions().TextX = textX;
	f->GetNodeOptions().TextY = textY;
	return R_OK;
}

int UI_CreateTextItem( const std::string& text, unsigned int color )
{
	CItem * item =  new CItem( text.c_str(), color );
	return g_ItemScript.AddObj( item );
}

int UI_CreateGraphItem( const std::string& file, int w, int h, int sx, int sy, int frame )
{
	CGraph* item =  new CGraph( file.c_str(), w, h, sx, sy, frame );
	return g_ItemScript.AddObj( item );
}

int UI_CreateGraphItemTex( int tx, int ty, int tw, int th, float scale_x, float scale_y, int nTextureID, int tag )
{
	CHintGraph * item =  new CHintGraph( 1 );
	MPTexRect *pImage = item->GetImage()->GetImage();
	pImage->nTexSX	= tx;
	pImage->nTexSY	= ty;
	pImage->nTexW	= tw;
	pImage->nTexH	= th;
	pImage->fScaleX	= scale_x;
	pImage->fScaleY	= scale_y;
	pImage->nTextureNo = nTextureID;
	item->nTag = tag;
	return g_ItemScript.AddObj( item );
}

int UI_CreateNoteGraphItem( const std::string& file, int w, int h, int sx, int sy, int frame, const std::string& text, int TextX, int TextY )
{
	CNoteGraph * item =  new CNoteGraph( frame );
	std::string resolved = ResolveImagePath(file);

	item->GetImage()->LoadAllImage( resolved.c_str(), w, h, sx, sy );
	item->GetImage()->SetScale( w, h );
	item->SetString( text.c_str() );
	item->SetTextX( TextX );
	item->SetTextY( TextY );
	return g_ItemScript.AddObj( item );
}

int UI_CreateSingleNode( int treeid, int itemid, int nodeid_parent )
{
	CTreeView* g = dynamic_cast<CTreeView*>(CGuiData::GetGui( treeid ));
	if( !g ) return R_FAIL;

	CItemObj* item = g_ItemScript.GetObj( itemid );
	if( !item ) return R_FAIL;

	CTreeNodeObj* obj = new CTreeNode( g, item );
	 
	CTreeNodeObj* parent = NULL;
	if( nodeid_parent<=0 )
		parent = g->GetRootNode();
	else
		parent = CTreeNodeObj::GetNode( nodeid_parent );

	if( parent ) parent->AddNode( obj );

	return obj->GetID();
}

int UI_CreateGridNode( int treeid, int itemid, int maxcol, int uw, int uh, int nodeid_parent )
{
	CTreeView* g = dynamic_cast<CTreeView*>(CGuiData::GetGui( treeid ));
	if( !g ) return R_FAIL;

	CItemObj* item = g_ItemScript.GetObj( itemid );
	if( !item ) return R_FAIL;

	CTreeGridNode* obj = new CTreeGridNode(g, item); 
	obj->SetIsExpand(false);
	obj->SetColMaxNum(maxcol);
	obj->SetUnitSize(uw, uh);

	CTreeNodeObj* parent = NULL;
	if( nodeid_parent<0 )
		parent = g->GetRootNode();
	else
		parent = CTreeNodeObj::GetNode( nodeid_parent );

	if( parent ) parent->AddNode( obj );

	return obj->GetID();
}

int UI_GridNodeAddItem( int nodeid, int itemid )
{
	CTreeGridNode* obj = dynamic_cast<CTreeGridNode*>(CTreeNodeObj::GetNode( nodeid ));
	if( !obj ) return R_FAIL;

	CItemObj* item = g_ItemScript.GetObj( itemid );
	if( !item ) return R_FAIL;

	obj->AddItem(item);

    // Initialize hint display
    CHintGraph* pHint = dynamic_cast<CHintGraph*>(item);
    if( pHint )
    {
        CTreeNode* pParent = dynamic_cast<CTreeNode*>( obj->GetParent() );

        if( stricmp( obj->GetItem()->GetString(), GetLanguageString(528).c_str() )==0 )
        {
            CChaRecord* pInfo = GetChaRecordInfo( pHint->nTag );
            if( pInfo )
            {
				char szBuf[128] = { 0 };
				sprintf( szBuf, "%d.%s", pInfo->lID, pInfo->szName.c_str() );
                pHint->SetHint( szBuf );
            }
        }
        else if( stricmp( obj->GetItem()->GetString(), GetLanguageString(532).c_str() )==0 )
        {
            CMapInfo* pInfo = GetMapInfo( pHint->nTag );
            if( pInfo )
            {
                pHint->SetHint( pInfo->szName );
            }
        }
        else if( stricmp( obj->GetItem()->GetString(), GetLanguageString(530).c_str() )==0 )
        {
            pHint->SetHint( g_GetAreaName(pHint->nTag) );
        }
        else if( stricmp( obj->GetItem()->GetString(), GetLanguageString(529).c_str() )==0 )
        {
            CEffectInfo *pInfo = GetEffectInfo(pHint->nTag);
            if( pInfo )
            {
				char szBuf[128] = { 0 };
				sprintf( szBuf, "%d.%s", pInfo->Id, pInfo->szName );
                pHint->SetHint( szBuf );
            }
        }
        else if( pParent )            
        {
            if( stricmp( pParent->GetItem()->GetString(), GetLanguageString(540).c_str() )==0 ) 
            {
        	    CSceneObjInfo *pInfo = GetSceneObjInfo(pHint->nTag);
                if( pInfo )
                {             
					char szBuf[128] = { 0 };
					sprintf( szBuf, "%d.%s", pInfo->_id, pInfo->_name.c_str() );
					pHint->SetHint( szBuf );
                }
            }
        }
    }
	return R_OK;
}

int UI_SetChatColor(unsigned int world, unsigned int road, unsigned int team, unsigned int guild, unsigned int gm, unsigned int system, unsigned int trade, unsigned int person)
{
	// World, Road, Team, Guild, GM, System, Trade, Private
	//g_stUICoze.chatColor.road = road;
	//g_stUICoze.chatColor.person = person;
	//g_stUICoze.chatColor.team = team;
	//g_stUICoze.chatColor.guild = guild;
	//g_stUICoze.chatColor.world = world;
	//g_stUICoze.chatColor.system = system;
	//g_stUICoze.chatColor.trade = trade;
	//g_stUICoze.chatColor.gm = gm;

	CCharMsg::SetChannelColor(CCharMsg::CHANNEL_SIGHT, road);
	CCharMsg::SetChannelColor(CCharMsg::CHANNEL_PRIVATE, person);
	CCharMsg::SetChannelColor(CCharMsg::CHANNEL_TEAM, team);
	CCharMsg::SetChannelColor(CCharMsg::CHANNEL_GUILD, guild);
	CCharMsg::SetChannelColor(CCharMsg::CHANNEL_WORLD, world);
	CCharMsg::SetChannelColor(CCharMsg::CHANNEL_SYSTEM, system);
	CCharMsg::SetChannelColor(CCharMsg::CHANNEL_TRADE, trade);
	CCharMsg::SetChannelColor(CCharMsg::CHANNEL_PUBLISH, gm);
	return R_OK;
}

int UI_CreatePageItem( int page_id )
{
	CPage* g = dynamic_cast<CPage*>(CGuiData::GetGui( page_id ));
	if( !g ) return R_FAIL;

	CPageItem* pitem = g->NewPage();
	return pitem->GetID();
}

int UI_SetPageButton( int page_id, int button_style, int bw, int bh )
{
	CPage* g = dynamic_cast<CPage*>(CGuiData::GetGui( page_id ));
	if( !g ) return R_FAIL;

	g->SetButtonSize( bw, bh );
	g->SetButtonPutStyle( (CPage::eButtonPos)button_style );	
	return R_OK;
}

enum ePageItemObj
{
	PAGE_ITEM_IMAGE=0,
	PAGE_ITEM_TITLE,
};
int UI_GetPageItemObj( int page_item_id, int type )
{
	CPageItem* g = dynamic_cast<CPageItem*>(CGuiData::GetGui( page_item_id ));
	if( !g ) return R_FAIL;

	if( type == PAGE_ITEM_IMAGE )
		return g->GetBkgImage()->GetID();
	else
		return g->GetTitle()->GetID();
}

int UI_AddCompent( int container_id, int compent_id )
{
	CContainer* g = dynamic_cast<CContainer*>(CGuiData::GetGui( container_id ));
	if( !g ) return R_FAIL;

	CCompent*	p =	dynamic_cast<CCompent*>(CGuiData::GetGui( compent_id ));
	if( !p ) return R_FAIL;

	g->AddCompent(p);
	return R_OK;
}

int UI_CreateFont( const std::string& name, const std::string& family,
				   int size800, int size1024, int nLevel, int nStyle )
{
	return FontManager::Instance().CreateFont(name, family,
											   size800, size1024, nLevel,
											   static_cast<unsigned long>(nStyle));
}

int UI_InstallFontFile( const std::string& path )
{
	return FontManager::Instance().InstallFontFile(path) ? R_OK : R_FAIL;
}

int UI_InstallFontDir( const std::string& dir )
{
	return FontManager::Instance().InstallFontsFromDir(dir);
}

int UI_DumpFonts( const std::string& dir )
{
	FontManager::Instance().DumpAllSlots(dir);
	return R_OK;
}

int UI_DumpFontAtlases( const std::string& dir )
{
	FontManager::Instance().DumpAllAtlases(dir);
	return R_OK;
}

int UI_SetFontDumpEnabled( int enabled )
{
	FontRender::SetTextDumpEnabled(enabled != 0);
	return R_OK;
}

int UI_SetFontShadowEnabled( int enabled )
{
	FontRender::SetShadowEnabled(enabled != 0);
	return R_OK;
}

int UI_SetLabelExShadowColor( int label_id, unsigned int color )
{
	CLabelEx* g = dynamic_cast<CLabelEx*>(CGuiData::GetGui( label_id ));
	if( !g ) return R_FAIL;
	
	g->SetShadowColor( color );
	return R_OK;
}

int UI_ItemBarLoadImage( const std::string& file, int w, int h, int tx, int ty )
{
	CItemBar::LoadImage( file.c_str(), w, h, tx, ty );
	return R_OK;
}

int UI_SetProgressHintStyle( int id, int style )
{
    CProgressBar* g = dynamic_cast<CProgressBar*>(CGuiData::GetGui( id ));
    if( !g ) return R_FAIL;

    g->SetHintStyle( (CProgressBar::eHintStyle)style );
    return R_OK;
}

int UI_SetProgressActiveMouse( int id, int style )
{
    CProgressBar* g = dynamic_cast<CProgressBar*>(CGuiData::GetGui( id ));
    if( !g ) return R_FAIL;

	if (style>0)
		g->SetActiveMouse( true) ;
	if (style ==0 )
		g->SetActiveMouse( false) ;
    return R_OK;
}

// Set max English characters displayed per row
int UI_SetMemoMaxNumPerRow( int id, int num  )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CMemo * memo = dynamic_cast<CMemo*>(p);
	if( memo ) 
	{
		memo->SetMaxNumPerRow( num );
		return  R_OK;
	}

	CMemoEx * memoex = dynamic_cast<CMemoEx*>(p);
	if( memoex ) 
	{
		memoex->SetMaxNumPerRow( num );
		return R_OK;
	}
	

	return R_OK;
}

// Set actual lines per page for Memo
int UI_SetMemoPageShowNum( int id, int num )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CMemo * memo = dynamic_cast<CMemo*>(p);
	if( memo )
	{
        memo->SetPageShowNum( num );
		return R_OK;
	}
	CMemoEx * memoex = dynamic_cast<CMemoEx*>(p);
	if( memoex )
	{
		memoex->SetPageShowNum( num );
		return R_OK;
	}
	return R_OK;
}

// Set row height for Memo
int UI_SetMemoRowHeight( int id, int num )
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CMemo * memo = dynamic_cast<CMemo*>(p);
	if( memo )
	{		
		memo->SetRowHeight( num );
		return R_OK;
	}

	CMemoEx * memoex = dynamic_cast<CMemoEx*>(p);
	if( memoex )
	{		
		memoex->SetRowHeight( num );
		return R_OK;
	}
	return R_OK;
}

int UI_RichSetMaxLine( int id, int line )
{
	CRichMemo* s = dynamic_cast<CRichMemo*>( CGuiData::GetGui( id ) );
	if( !s ) return R_FAIL;

	s->SetMaxLine( (unsigned short)line );
	return R_OK;
}

int UI_RichSetClipRect( int id, int x0, int y0, int x1, int y1 )
{
	CRichMemo* s = dynamic_cast<CRichMemo*>( CGuiData::GetGui( id ) );
	if( !s ) return R_FAIL;

	RECT  rt = { x0, y0, x1, y1 };
	s->SetClipRect( rt );
	return R_OK;
}

//---------------------------------------------------------------------------
// CHeadSay
//---------------------------------------------------------------------------
int UI_LoadHeadSayFaceImage( int num, int maxframe, int w, int h, const std::string& file, int cw, int ch, int tx, int ty )
{
	CGuiPic* p = CHeadSay::GetFacePic(num);
	if( !p )  return R_FAIL;

	if( maxframe > 0 ) 
	{
		p->SetMax( maxframe );
	}
	else 
	{
		return R_FAIL;
	}

	p->LoadAllImage( file.c_str(), cw, ch, tx, ty );
	p->SetScale( w, h );
	return R_OK;
}

int UI_LoadHeadSayShopImage( int num, int w, int h, const std::string& file, int cw, int ch, int tx, int ty )
{
	CGuiPic* p = CHeadSay::GetShopPic(num);
	if( !p )  return R_FAIL;

	p->LoadImage( file.c_str(), cw, ch, 0, tx, ty );
	p->SetScale( w, h );
	return R_OK;
}

int UI_LoadHeadSayShopImage2( int num, int w, int h, const std::string& file, int cw, int ch, int tx, int ty )
{
	CGuiPic* p = CHeadSay::GetShopPic2(num);
	if( !p )  return R_FAIL;

	p->LoadImage( file.c_str(), cw, ch, 0, tx, ty );
	p->SetScale( w, h );
	return R_OK;
}

int UI_SetLabelExAlign( int label_id, int Align )
{
    CLabelEx* g = dynamic_cast<CLabelEx*>(CGuiData::GetGui( label_id ));
    if( !g ) return R_FAIL;


    g->SetIsCenter( Align );

    return R_OK;
}


int UI_LoadHeadSayLifeImage( int uw, int uh, const std::string& file, int w, int h, int tx, int ty, int isHorizontal )
{
	CGuiPic* hp = CHeadSay::GetLifePic();
	CGuiPic* sp = CHeadSay::GetManaPic();
	if( !hp || !sp )  return R_FAIL;

	if( isHorizontal==TRUE )
	{
		hp->LoadImage( file.c_str(), w, h, 0, tx, ty );
		hp->LoadImage( file.c_str(), w, h, 1, tx + w, ty );
		
		sp->LoadImage( file.c_str(), w, h, 0, tx, ty );
		sp->LoadImage( file.c_str(), w, h, 1, tx + w, ty );
	}
	else
	{
		hp->LoadImage( file.c_str(), w, h, 0, tx, ty );
		hp->LoadImage( file.c_str(), w, h, 1, tx, ty + h );
		
		sp->LoadImage( file.c_str(), w, h, 0, tx, ty );
		sp->LoadImage( file.c_str(), w, h, 1, tx, ty + h );
	}
	hp->SetScale( uw, uh );
	sp->SetScale( uw, uh );
	return R_OK;
}



int UI_SetDragSnapToGrid( int nGridWidth, int nGridHeight ) 
{
	CDrag::SetSnapToGrid( nGridWidth, nGridHeight );
	return R_OK;
}


int UI_SetTextParse( int nIndex, const std::string& file, int w, int h, int sx, int sy, int frame )// Load text parse image mapping
{
	CGraph *p = new CGraph(file.c_str(), w, h, sx, sy, frame);
	g_TextParse.AddFace( nIndex, p );
	return R_OK;
}


int UI_SetFormStyle(int id ,int index)
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CForm *f = dynamic_cast<CForm*>(p);
	if( !f ) return R_FAIL;

	f->SetStyle( CForm::eFormStyle(index) );	
	return R_OK;

}

int UI_SetFormStyleEx(int id ,int index, int offWidth, int offHeight)
{
	CGuiData* p = CGuiData::GetGui( id );
	if( !p ) return R_FAIL;

	CForm *f = dynamic_cast<CForm*>(p);
	if( !f ) return R_FAIL;

	f->SetStyle( CForm::eFormStyle(index), offWidth, offHeight );	
	return R_OK;

}

// Menu
int UI_MenuLoadSelect( int id, const std::string& imagefile, int w, int h, int sx, int sy )
{
	std::string resolved = ResolveImagePath(imagefile);

	CMenu *f = dynamic_cast<CMenu*>(CGuiData::GetGui( id ));
	if( !f ) return R_FAIL;

	f->GetSelectImage()->LoadImage(resolved.c_str(), w, h, 0, sx, sy);
	return R_OK;
}

int UI_MenuLoadImage( int id, int IsShowFrame, int IsTitle, const std::string& clientfile, int cw, int ch, int tx, int ty, const std::string& framefile, int w, int h )
{
	CMenu *f = dynamic_cast<CMenu*>(CGuiData::GetGui( id ));
	if( !f ) return R_FAIL;

	std::string resolvedClient = ResolveImagePath(clientfile);
	std::string resolvedFrame = ResolveImagePath(framefile);

	CFramePic* frame = f->GetBkgImage();
	frame->SetIsTitle( IsTitle ? true : false );
	frame->SetIsShowFrame( IsShowFrame ? true : false );
	frame->LoadImage( resolvedClient.c_str(), cw, ch, tx, ty, resolvedFrame.c_str(), w, h );
	return R_OK;
}

int UI_MenuAddText( int id, const std::string& text )
{
	CMenu *f = dynamic_cast<CMenu*>(CGuiData::GetGui( id ));
	if( !f ) return R_FAIL;

	f->AddMenu( text.c_str() );
	return R_OK;
}

int UI_AddFilterTextToNameTable(const std::string& text )
{
	return CTextFilter::Add(CTextFilter::NAME_TABLE,text.c_str());
}

int UI_AddFilterTextToDialogTable(const std::string& text )
{
	return CTextFilter::Add(CTextFilter::DIALOG_TABLE,text.c_str());
}

int UI_SetHeadSayBkgColor( unsigned int color )
{
	CHeadSay::SetBkgColor( color );
	return R_OK;
}

int UI_LoadSkillActiveImage( const std::string& file, int maxframe, int w, int h, int sx, int sy )
{
	CGuiPic* pic = CSkillCommand::GetActiveImage();
	pic->SetMax( maxframe );
	pic->LoadAllImage( file.c_str(), w, h, sx, sy );
	return R_OK;
}

int UI_LoadChargeImage( int link, const std::string& file, int maxframe, int w, int h, int sx, int sy )
{
	CGuiPic* pic = CEquipMgr::GetChargePic( link );
	if( pic )
	{
		pic->SetMax( maxframe );
		pic->LoadAllImage( file.c_str(), w, h, sx, sy );
		return R_OK;
	}
	return R_FAIL;
}

int UI_SetCaption( int id, const std::string& caption )
{
	CGuiData* p = CGuiData::GetGui( id );

	if( !p ) return R_FAIL;

	p->SetCaption( caption.c_str() );
	return R_OK;
}





//---------------------------------------------------------------------------
// ScriptRegedit
//---------------------------------------------------------------------------
void MPInitLua_Gui(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
		// texture
		LUABRIDGE_REGISTER_FUNC(GetChaPhotoTexID)
		LUABRIDGE_REGISTER_FUNC(GetSceneObjPhotoTexID)
		LUABRIDGE_REGISTER_FUNC(GetEffectPhotoTexID)
		LUABRIDGE_REGISTER_FUNC(GetTextureID)
		LUABRIDGE_REGISTER_FUNC(GetTerrainTextureID)
		LUABRIDGE_REGISTER_FUNC(GetTerrainTextureType)
		LUABRIDGE_REGISTER_FUNC(GetSceneObjPhotoTexType)
		// gui
		LUABRIDGE_REGISTER_FUNC(UI_LoadScript)
		LUABRIDGE_REGISTER_FUNC(UI_SetDragSnapToGrid)
		LUABRIDGE_REGISTER_FUNC(UI_SetMaxImage)
		LUABRIDGE_REGISTER_FUNC(UI_SetFormTempleteMax)
		LUABRIDGE_REGISTER_FUNC(UI_SwitchTemplete)
		LUABRIDGE_REGISTER_FUNC(UI_AddAllFormTemplete)
		LUABRIDGE_REGISTER_FUNC(UI_AddFormToTemplete)
		// form
		LUABRIDGE_REGISTER_FUNC(UI_CreateForm)
		LUABRIDGE_REGISTER_FUNC(UI_FormSetIsEscClose)
		LUABRIDGE_REGISTER_FUNC(UI_FormSetEnterButton)
		LUABRIDGE_REGISTER_FUNC(UI_FormSetHotKey)
		LUABRIDGE_REGISTER_FUNC(UI_LoadFormImage)
		LUABRIDGE_REGISTER_FUNC(UI_ShowForm)
		LUABRIDGE_REGISTER_FUNC(UI_SetIsDrag)
		LUABRIDGE_REGISTER_FUNC(UI_SetIsKeyFocus)
		// components
		LUABRIDGE_REGISTER_FUNC(UI_CreateCompent)
		LUABRIDGE_REGISTER_FUNC(UI_SetCaption)
		LUABRIDGE_REGISTER_FUNC(UI_CopyImage)
		LUABRIDGE_REGISTER_FUNC(UI_SetSize)
		LUABRIDGE_REGISTER_FUNC(UI_SetPos)
		LUABRIDGE_REGISTER_FUNC(UI_SetTag)
		LUABRIDGE_REGISTER_FUNC(UI_SetAlign)
		// image
		LUABRIDGE_REGISTER_FUNC(UI_LoadImage)
		LUABRIDGE_REGISTER_FUNC(UI_LoadImageUnencrypted)
		LUABRIDGE_REGISTER_FUNC(UI_LoadScaleImage)
		LUABRIDGE_REGISTER_FUNC(UI_SetHint)
		LUABRIDGE_REGISTER_FUNC(UI_LoadFlashScaleImage)
		LUABRIDGE_REGISTER_FUNC(UI_SetImageAlpha)
		// combo
		LUABRIDGE_REGISTER_FUNC(UI_ComboSetTextColor)
		LUABRIDGE_REGISTER_FUNC(UI_ComboSetStyle)
		LUABRIDGE_REGISTER_FUNC(UI_LoadComboImage)
		// button
		LUABRIDGE_REGISTER_FUNC(UI_LoadButtonImage)
		LUABRIDGE_REGISTER_FUNC(UI_SetButtonModalResult)
		LUABRIDGE_REGISTER_FUNC(UI_ButtonSetHint)
		// scroll/list
		LUABRIDGE_REGISTER_FUNC(UI_GetScroll)
		LUABRIDGE_REGISTER_FUNC(UI_CopyCompent)
		LUABRIDGE_REGISTER_FUNC(UI_GetList)
		LUABRIDGE_REGISTER_FUNC(UI_AddListText)
		LUABRIDGE_REGISTER_FUNC(UI_ListLoadSelectImage)
		LUABRIDGE_REGISTER_FUNC(UI_LoadListItemImage)
		LUABRIDGE_REGISTER_FUNC(UI_ListSetItemMargin)
		LUABRIDGE_REGISTER_FUNC(UI_ListSetItemImageMargin)
		LUABRIDGE_REGISTER_FUNC(UI_SetListFontColor)
		LUABRIDGE_REGISTER_FUNC(UI_SetListRowHeight)
		LUABRIDGE_REGISTER_FUNC(UI_SetListIsMouseFollow)
		// progress/scroll style
		LUABRIDGE_REGISTER_FUNC(UI_SetProgressStyle)
		LUABRIDGE_REGISTER_FUNC(UI_SetScrollStyle)
		LUABRIDGE_REGISTER_FUNC(UI_SetProgressHintStyle)
		LUABRIDGE_REGISTER_FUNC(UI_SetProgressActiveMouse)
		// group/listview
		LUABRIDGE_REGISTER_FUNC(UI_AddGroupBox)
		LUABRIDGE_REGISTER_FUNC(UI_CreateListView)
		LUABRIDGE_REGISTER_FUNC(UI_ListViewSetTitle)
		LUABRIDGE_REGISTER_FUNC(UI_ListViewSetTitleHeight)
		LUABRIDGE_REGISTER_FUNC(UI_GoodGridLoadUnitImage)
		// chat/alpha
		LUABRIDGE_REGISTER_FUNC(UI_SetChatColor)
		LUABRIDGE_REGISTER_FUNC(UI_SetAlpha)
		// grid
		LUABRIDGE_REGISTER_FUNC(UI_GridLoadSelectImage)
		LUABRIDGE_REGISTER_FUNC(UI_SetGridIsDragSize)
		LUABRIDGE_REGISTER_FUNC(UI_GetScrollObj)
		LUABRIDGE_REGISTER_FUNC(UI_SetGridUnitSize)
		LUABRIDGE_REGISTER_FUNC(UI_AddFaceToGrid)
		LUABRIDGE_REGISTER_FUNC(UI_SetGridSpace)
		LUABRIDGE_REGISTER_FUNC(UI_SetGridContent)
		// show/enable
		LUABRIDGE_REGISTER_FUNC(UI_SetIsShow)
		LUABRIDGE_REGISTER_FUNC(UI_SetIsEnabled)
		LUABRIDGE_REGISTER_FUNC(UI_SetMargin)
		// fix list
		LUABRIDGE_REGISTER_FUNC(UI_LoadListFixSelect)
		LUABRIDGE_REGISTER_FUNC(UI_FixListSetMaxNum)
		LUABRIDGE_REGISTER_FUNC(UI_FixListSetText)
		LUABRIDGE_REGISTER_FUNC(UI_FixListSetRowSpace)
		// check fix list
		LUABRIDGE_REGISTER_FUNC(UI_CheckFixListSetCheckMargin)
		LUABRIDGE_REGISTER_FUNC(UI_LoadCheckFixListCheck)
		// edit
		LUABRIDGE_REGISTER_FUNC(UI_SetEditCursorColor)
		LUABRIDGE_REGISTER_FUNC(UI_SetEditMaxNum)
		LUABRIDGE_REGISTER_FUNC(UI_SetEditEnterButton)
		LUABRIDGE_REGISTER_FUNC(UI_SetEditMaxNumVisible)
		// text
		LUABRIDGE_REGISTER_FUNC(UI_SetTextColor)
		// frame/page
		LUABRIDGE_REGISTER_FUNC(UI_LoadFrameImage)
		LUABRIDGE_REGISTER_FUNC(UI_CreatePageItem)
		LUABRIDGE_REGISTER_FUNC(UI_GetPageItemObj)
		LUABRIDGE_REGISTER_FUNC(UI_AddCompent)
		LUABRIDGE_REGISTER_FUNC(UI_SetLabelExShadowColor)
		LUABRIDGE_REGISTER_FUNC(UI_SetLabelExAlign)
		LUABRIDGE_REGISTER_FUNC(UI_SetLabelExFont)
		LUABRIDGE_REGISTER_FUNC(UI_SetPageButton)
		// tree
		LUABRIDGE_REGISTER_FUNC(UI_TreeLoadImage)
		LUABRIDGE_REGISTER_FUNC(UI_TreeSetNodeTextXY)
		LUABRIDGE_REGISTER_FUNC(UI_CreateTextItem)
		LUABRIDGE_REGISTER_FUNC(UI_CreateNoteGraphItem)
		LUABRIDGE_REGISTER_FUNC(UI_CreateGraphItem)
		LUABRIDGE_REGISTER_FUNC(UI_CreateSingleNode)
		LUABRIDGE_REGISTER_FUNC(UI_CreateGridNode)
		LUABRIDGE_REGISTER_FUNC(UI_GridNodeAddItem)
		LUABRIDGE_REGISTER_FUNC(UI_CreateGraphItemTex)
		// item bar
		LUABRIDGE_REGISTER_FUNC(UI_ItemBarLoadImage)
		LUABRIDGE_REGISTER_FUNC(UI_AddListBarText)
		// headsay
		LUABRIDGE_REGISTER_FUNC(UI_LoadHeadSayFaceImage)
		LUABRIDGE_REGISTER_FUNC(UI_LoadHeadSayShopImage)
		LUABRIDGE_REGISTER_FUNC(UI_LoadHeadSayShopImage2)
		LUABRIDGE_REGISTER_FUNC(UI_LoadHeadSayLifeImage)
		LUABRIDGE_REGISTER_FUNC(UI_SetHeadSayBkgColor)
		// form style
		LUABRIDGE_REGISTER_FUNC(UI_SetFormStyle)
		LUABRIDGE_REGISTER_FUNC(UI_SetFormStyleEx)
		// font
		LUABRIDGE_REGISTER_FUNC(UI_CreateFont)
		LUABRIDGE_REGISTER_FUNC(UI_InstallFontFile)
		LUABRIDGE_REGISTER_FUNC(UI_InstallFontDir)
		LUABRIDGE_REGISTER_FUNC(UI_DumpFonts)
		LUABRIDGE_REGISTER_FUNC(UI_DumpFontAtlases)
		LUABRIDGE_REGISTER_FUNC(UI_SetFontDumpEnabled)
		LUABRIDGE_REGISTER_FUNC(UI_SetFontShadowEnabled)
		// text parse
		LUABRIDGE_REGISTER_FUNC(UI_SetTextParse)
		// memo
		LUABRIDGE_REGISTER_FUNC(UI_SetMemoMaxNumPerRow)
		LUABRIDGE_REGISTER_FUNC(UI_SetMemoPageShowNum)
		LUABRIDGE_REGISTER_FUNC(UI_SetMemoRowHeight)
		// rich
		LUABRIDGE_REGISTER_FUNC(UI_RichSetClipRect)
		LUABRIDGE_REGISTER_FUNC(UI_RichSetMaxLine)
		// skill
		LUABRIDGE_REGISTER_FUNC(UI_LoadSkillListButtonImage)
		LUABRIDGE_REGISTER_FUNC(UI_LoadSkillActiveImage)
		LUABRIDGE_REGISTER_FUNC(UI_LoadChargeImage)
		// menu
		LUABRIDGE_REGISTER_FUNC(UI_MenuLoadSelect)
		LUABRIDGE_REGISTER_FUNC(UI_MenuLoadImage)
		LUABRIDGE_REGISTER_FUNC(UI_MenuAddText)
		// filter
		LUABRIDGE_REGISTER_FUNC(UI_AddFilterTextToNameTable)
		LUABRIDGE_REGISTER_FUNC(UI_AddFilterTextToDialogTable)
		// title
		LUABRIDGE_REGISTER_FUNC(UI_SetTitleFont);

	// Font style constants exposed as Lua globals.
	lua_pushinteger(L, MPFONT_BOLD);   lua_setglobal(L, "MPFONT_BOLD");
	lua_pushinteger(L, MPFONT_ITALIC); lua_setglobal(L, "MPFONT_ITALIC");
	lua_pushinteger(L, MPFONT_UNLINE); lua_setglobal(L, "MPFONT_UNLINE");
}


