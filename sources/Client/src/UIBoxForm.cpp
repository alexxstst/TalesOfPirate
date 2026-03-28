#include "StdAfx.h"
#include "uiboxform.h"
#include "uiform.h"
#include "uiedit.h"
#include "uilabel.h"
#include "tools.h"
#include "uiformmgr.h"
#include "stringlib.h"
using namespace GUI;

static char szBuf[80] = { 0 };

CHideForm	CBoxMgr::_cTrade;	
CHideForm	CBoxMgr::_cSelect;	
CHideForm	CBoxMgr::_cNumber;
CHideForm	CBoxMgr::_cPassword;
CHideForm	CBoxMgr::_cMsg;

bool	CBoxMgr::_cStart = false;
DWORD	CBoxMgr::_cTick = 0;
int	CBoxMgr::_cSeconds = 0;
CForm* CBoxMgr::_cfrmError = NULL;

#define MAX_BOX_NUM  1000000000
//---------------------------------------------------------------------------
// class CBoxMgr
//---------------------------------------------------------------------------
bool CBoxMgr::Init()
{
	// ���׶Ի���
    CForm* frmTrade  = _FindForm("frmTrade");   
	if ( !frmTrade ) return false;

	CEdit* edtNumber = dynamic_cast<CEdit*>(frmTrade->Find("edtNumber"));
	if( !edtNumber ) return Error( g_oLangRec.GetString(45), frmTrade->GetName(), "edtNumber" );
	edtNumber->SetIsDigit( true );

	CLabel* labPrice = dynamic_cast<CLabel*>(frmTrade->Find("labPrice"));
	if( !labPrice )	return Error( g_oLangRec.GetString(45), frmTrade->GetName(), "labPrice" );

	CLabel* labName = dynamic_cast<CLabelEx*>( frmTrade->Find("labName") );
	if( !labName )	return Error( g_oLangRec.GetString(45), frmTrade->GetName(), "labName" );

	_cTrade.Init( frmTrade );

	// ѡ��Ի���
    CForm* frmSelect  = _FindForm("frmSelect");   
	if ( !frmSelect ) return false;
	//frmSelect->SetPos( g_Render.GetScrWidth(), g_Render.GetScrHeight() - 151 );

	CLabel* labInfo = dynamic_cast<CLabelEx*>( frmSelect->Find("labInfo") );
	if( !labInfo )	return Error( g_oLangRec.GetString(45), frmSelect->GetName(), "labInfo" );

	_cSelect.Init( frmSelect );

	// ���������Ի���
    CForm* frmNumber = _FindForm("frmNumber");
    if( !frmNumber ) return false;

    edtNumber = dynamic_cast<CEdit*>(frmNumber->Find( "edtNumber" ));
	if( !edtNumber )	return Error( g_oLangRec.GetString(45), frmNumber->GetName(), "edtNumber" );
	edtNumber->SetIsDigit( true );

	labInfo = dynamic_cast<CLabelEx*>( frmNumber->Find("labInfo") );
	if( !labInfo )	return Error( g_oLangRec.GetString(45), frmNumber->GetName(), "labInfo" );

	_cNumber.Init( frmNumber );


	// ��������Ի���
	CForm* frmPassword = _FindForm("frmPass");
	if( !frmPassword ) return false;

	CEdit* edtPassword = dynamic_cast<CEdit*>(frmPassword->Find( "edtPassword" ));
	if( !edtPassword )	return Error( g_oLangRec.GetString(45), frmPassword->GetName(), "edtPassword" );

	labInfo = dynamic_cast<CLabelEx*>( frmPassword->Find("labTitle") );
	if( !labInfo )	return Error( g_oLangRec.GetString(45), frmPassword->GetName(), "labTitle" );

	_cPassword.Init( frmPassword );


	// һ����Ϣ�Ի���
    CForm* frmError  = _FindForm("frmError");   
	if ( !frmError ) return false;

	CLabel* labError = dynamic_cast<CLabelEx*>( frmError->Find("labError") );
	if( !labError )	return Error( g_oLangRec.GetString(45), frmError->GetName(), "labError" );

	_cfrmError = frmError;

	_cMsg.Init( frmError );

	return true;
}

CForm* CBoxMgr::_FindForm(const char * frmName)
{
	CForm* form = CFormMgr::s_Mgr.Find( frmName, 4 );
	if( !form )	{ char _buf[256]; snprintf(_buf, sizeof(_buf), g_oLangRec.GetString(464), frmName); g_logManager.InternalLog(LogLevel::Debug, "ui", _buf); }
	return form;
}

void CBoxMgr::End()
{
	_ClearBox( _cTrade );
	_ClearBox( _cSelect );
	_ClearBox( _cNumber );
	_ClearBox( _cPassword );
	_ClearBox( _cMsg );
}

void CBoxMgr::_ClearBox( CHideForm& list )
{
	CForm* frm = NULL;
	int nCount = list.GetCount();
	for( int i=0; i<nCount; i++ )
	{	
		frm = list[i];

		if(frm->GetPointer())
		{
			delete frm->GetPointer(); // UI��������
			frm->SetPointer( NULL );
		}
	}
}

stTradeBox* CBoxMgr::ShowTradeBox( FormMouseEvent evtForm, float fUnitMoney, int nTotalNum, const char* szName )
{
	CForm* frm = _cTrade.GetHide();
	stTradeBox* t = (stTradeBox*)frm->GetPointer();
	if( !t )
	{
		t = new stTradeBox;
		t->labName =  dynamic_cast<CLabel*>(frm->Find( "labName" ));
		t->labPrice = dynamic_cast<CLabel*>(frm->Find( "labPrice" ));
		t->edtNumber = dynamic_cast<CEdit*>(frm->Find( "edtNumber" ));
		t->frmDialog = frm;

		frm->SetPointer( (void*)t );
	}

	if( szName )
	{
		t->labName->SetCaption( szName );
	}
	else
	{
		t->labName->SetCaption( "" );
	}

	frm->evtEntrustMouseEvent = evtForm;
	// frm->nTag = fUnitMoney;
	frm->ShowModal();

	if( nTotalNum<0 || nTotalNum>MAX_BOX_NUM )
		nTotalNum = MAX_BOX_NUM;

	if( fUnitMoney<0.0f || fUnitMoney>=MAX_BOX_NUM )
		fUnitMoney = MAX_BOX_NUM;

	if( nTotalNum>0 )
	{
		char buf[32] = { 0 };
		sprintf( buf, "%d", nTotalNum );
		t->edtNumber->SetCaption( buf );
	}
	else
	{
		t->edtNumber->SetCaption( "" );
	}
	t->nTotalNum = nTotalNum;
	t->fUnitMoney = (float)(int)fUnitMoney;				// ��Ϸ����ʾȡ��������� call ����
	return t;
}

stSelectBox* CBoxMgr::ShowSelectBox( FormMouseEvent evtForm, const char* szTitle, bool bModal )
{
	CForm* frm = _cSelect.GetHide();
	stSelectBox* t = (stSelectBox*)frm->GetPointer();
	if( !t )
	{
		t = new stSelectBox;
		t->frmDialog = frm;
		t->labInfo = dynamic_cast<CLabel*>(frm->Find( "labInfo" ));

		frm->SetPointer( (void*)t );
	}
	if( szTitle ) t->labInfo->SetCaption( szTitle );
	frm->evtEntrustMouseEvent = evtForm;
	frm->SetIsEscClose(false);

	//frm->SetSize( CGuiFont::s_Font.GetWidth( t->labInfo->GetCaption() ) + t->labInfo->GetLeft() * 2, frm->GetHeight() );
    frm->SetPos( (GetRender().GetScreenWidth() - frm->GetWidth() )/2
        , ( GetRender().GetScreenHeight() - frm->GetHeight())/2 );
    frm->Refresh();

	(bModal)?frm->ShowModal():frm->Show();	
	return t;
}

stNumBox* CBoxMgr::ShowNumberBox( FormMouseEvent evtForm, int nTotalNum, const char* szTitle, bool IsMax, bool bModal )
{
	CForm* frm = _cNumber.GetHide();
	stNumBox* t = (stNumBox*)frm->GetPointer();
	if( !t )
	{
		t = new stNumBox;
		t->frmDialog = frm;
		t->labInfo = dynamic_cast<CLabel*>(frm->Find( "labInfo" ));
		t->edtNumber = dynamic_cast<CEdit*>(frm->Find( "edtNumber" ));

		frm->SetPointer( (void*)t );
	}
	if( szTitle ) 
	{
		t->labInfo->SetCaption( szTitle );
	}
	else
	{
		t->labInfo->SetCaption( g_oLangRec.GetString(454) );
	}

	if( nTotalNum<0 || nTotalNum>MAX_BOX_NUM )
		nTotalNum = MAX_BOX_NUM;

	t->nTotalNum = nTotalNum;

	if( IsMax && nTotalNum>0 )
	{
		char buf[32] = { 0 };
		sprintf( buf, "%d", nTotalNum );
		t->edtNumber->SetCaption( buf );
	}
	else
	{
		t->edtNumber->SetCaption( "" );
	}
	t->pointer = NULL;

	//frm->SetSize( CGuiFont::s_Font.GetWidth( t->labInfo->GetCaption() ) + t->labInfo->GetLeft() * 2, frm->GetHeight() );
    frm->SetPos( (GetRender().GetScreenWidth() - frm->GetWidth() )/2
        , ( GetRender().GetScreenHeight() - frm->GetHeight())/2 );
    frm->Refresh();

	frm->evtEntrustMouseEvent = evtForm;
	if( bModal ) frm->ShowModal();
	else frm->Show();
	return t;
}

stMsgTimeBox* CBoxMgr::ShowMsgTime(  FormMouseEvent evtForm, const char* szTitle, int iSeconds )
{
	stMsgTimeBox* t = (stMsgTimeBox*)_cfrmError->GetPointer();

	if( !t )
	{
		t = new stMsgTimeBox;
		t->frmDialog = _cfrmError;
		t->labInfo = dynamic_cast<CLabel*>(_cfrmError->Find( "labError" ));

		_cfrmError->SetPointer( (void*)t );
	}
	if( szTitle ) t->labInfo->SetCaption( szTitle );

	_cfrmError->SetSize( CGuiFont::s_Font.GetWidth( t->labInfo->GetCaption() ) + t->labInfo->GetLeft() * 2, _cfrmError->GetHeight() );
    _cfrmError->SetPos( (GetRender().GetScreenWidth() - _cfrmError->GetWidth() )/2
        , ( GetRender().GetScreenHeight() - _cfrmError->GetHeight())/2 );
    _cfrmError->Refresh();

	_cfrmError->evtEntrustMouseEvent = evtForm;
	_cfrmError->ShowModal();

	_cStart = true;
	_cSeconds = iSeconds;
	return t;
}

stMsgBox* CBoxMgr::ShowMsgBox( FormMouseEvent evtForm, const char* szTitle, bool bModal )
{
	CForm* frm = _cMsg.GetHide();
	stMsgBox* t = (stMsgBox*)frm->GetPointer();
	if( !t )
	{
		t = new stMsgBox;
		t->frmDialog = frm;
		t->labInfo = dynamic_cast<CLabel*>(frm->Find( "labError" ));

		frm->SetPointer( (void*)t );
	}
	if( szTitle ) t->labInfo->SetCaption( szTitle );

	frm->SetSize( CGuiFont::s_Font.GetWidth( t->labInfo->GetCaption() ) + t->labInfo->GetLeft() * 2, frm->GetHeight() );
    frm->SetPos( (GetRender().GetScreenWidth() - frm->GetWidth() )/2
        , ( GetRender().GetScreenHeight() - frm->GetHeight())/2 );
    frm->Refresh();

	frm->evtEntrustMouseEvent = evtForm;
	(bModal)?frm->ShowModal():frm->Show();
	return t;
}

stPasswordBox* CBoxMgr::ShowPasswordBox( FormMouseEvent evtForm, const char* szTitle, const char* szPassword, bool bModal )
{
	CForm* frm = _cPassword.GetHide();
	stPasswordBox* t = (stPasswordBox*)frm->GetPointer();
	if( !t )
	{
		t = new stPasswordBox;
		t->frmDialog = frm;
		t->labInfo = dynamic_cast<CLabel*>(frm->Find( "labTitle" ));
		t->edtPassword = dynamic_cast<CEdit*>(frm->Find( "edtPassword" ));

		frm->SetPointer( (void*)t );
	}
	if ( szTitle ) t->labInfo->SetCaption( szTitle );

	if (szPassword) t->edtPassword->SetCaption(szPassword);
	else t->edtPassword->SetCaption("");

	t->edtPassword->SetIsPassWord(true);

	//frm->SetSize( CGuiFont::s_Font.GetWidth( t->labInfo->GetCaption() ) + t->labInfo->GetLeft() * 2, frm->GetHeight() );
	frm->SetPos( (GetRender().GetScreenWidth() - frm->GetWidth() )/2
		, ( GetRender().GetScreenHeight() - frm->GetHeight())/2 );
	frm->Refresh();

	frm->evtEntrustMouseEvent = evtForm;
	(bModal)?frm->ShowModal():frm->Show();
	return t;
}


void CBoxMgr::FrameMove(DWORD dwTime)
{
	static CTimeWork time(250);
	if( time.IsTimeOut( dwTime ) )
	{
		CForm* frm = NULL;
		for( int i=0; i<_cTrade.GetCount(); i++ )
		{
			frm = _cTrade[ i ];
			if( frm->GetIsShow() )
			{
				stTradeBox* t = (stTradeBox*)frm->GetPointer();
				t->Refresh();
			}
		}

		for( int i=0; i<_cNumber.GetCount(); i++ )
		{
			frm = _cNumber[ i ];
			if( frm->GetIsShow() )
			{
				stNumBox* t = (stNumBox*)frm->GetPointer();
				t->Refresh();
			}
		}
	}

	// Add by lark.li 20080726 begin
	if(_cStart)
	{
		if(_cTick==0)
		{
			_cTick = dwTime;
		}
		else
		{
			if(dwTime - _cTick > _cSeconds * 1000)
			{
				if(_cfrmError)
				{
					CLabel* labInfo = dynamic_cast<CLabel*>(_cfrmError->Find( "labError" ));
					//labInfo->SetCaption(
					_cfrmError->evtEntrustMouseEvent(labInfo, 0, 0, 0, 0);
					_cStart = false;
					_cTick = 0;
				}
			}
		}
	}


	// End
}

void CBoxMgr::CloseAllBox()
{
	_cTrade.CloseAll();
	_cSelect.CloseAll();
	_cNumber.CloseAll();
	_cPassword.CloseAll();
	_cMsg.CloseAll();
}

//---------------------------------------------------------------------------
// class stTradeBox
//---------------------------------------------------------------------------
stTradeBox::stTradeBox() 
: labPrice(NULL), edtNumber(NULL), frmDialog(NULL), labName(NULL)
, fUnitMoney(0.0), nTotalNum(0) 
{
}

int stTradeBox::GetTradeNum() 
{ 
	int n = atoi( edtNumber->GetCaption() ); 
	if( nTotalNum>0 && n>nTotalNum )
		n=nTotalNum;

	if( n<0 ) n=0; 
	if( n>MAX_BOX_NUM ) n=MAX_BOX_NUM;
	return n;
}

void stTradeBox::Refresh()
{
	int n = atoi( edtNumber->GetCaption() );
	if( nTotalNum>0 )
	{
		if( n>nTotalNum )
		{
			n = nTotalNum;

			sprintf( szBuf ,"%d", n );
			edtNumber->SetCaption( szBuf );
		}
		if( n<0 )
		{
			n=0;

			sprintf( szBuf ,"%d", n );
			edtNumber->SetCaption( szBuf );
		}
	}

	sprintf( szBuf ,"%s", StringSplitNum( (int)((float)n * fUnitMoney) ) );
	labPrice->SetCaption(szBuf);
}

//---------------------------------------------------------------------------
// class stSelectBox
//---------------------------------------------------------------------------
stSelectBox::stSelectBox() 
: frmDialog(NULL), labInfo(NULL), dwTag(0), dwParam(0), pointer(0)
{
}

//---------------------------------------------------------------------------
// class stNumBox
//---------------------------------------------------------------------------
stNumBox::stNumBox()
: frmDialog(NULL), edtNumber(NULL), labInfo(NULL), pointer(NULL)
{
}

int stNumBox::GetNumber()
{	
	int n = atoi( edtNumber->GetCaption() ); 
	if( nTotalNum>0 && n>nTotalNum )
		n=nTotalNum;

	if( n<0 ) n=0; 
	if( n>MAX_BOX_NUM ) n=MAX_BOX_NUM;
	return n;
}

void stNumBox::GetString(const char*& out)
{	
	out = edtNumber->GetCaption();
	//memcpy(out,edtNumber->GetCaption() ,strlen(edtNumber->GetCaption() )+1 );
}

void stNumBox::Refresh()
{
	int n = atoi( edtNumber->GetCaption() );
	if( nTotalNum>0 )
	{
		if( n>nTotalNum )
		{
			n = nTotalNum;

			sprintf( szBuf ,"%d", n );
			edtNumber->SetCaption( szBuf );
		}
		if( n<0 )
		{
			n=0;

			sprintf( szBuf, "%d", n );
			edtNumber->SetCaption( szBuf );
		}
	}
}

