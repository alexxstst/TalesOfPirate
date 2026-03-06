#include "StdAfx.h"
#include "GameApp.h"
#include "uigoodsgrid.h"
#include "UIItem.h"
#include "UICommand.h"
#include "UIItemCommand.h"
#include "UIFormMgr.h"
#include "packetcmd.h"

using namespace std;
using namespace GUI;

//---------------------------------------------------------------------------
// class CGoodsGrid
//---------------------------------------------------------------------------

int CGoodsGrid::_nTmpX		= 0;
int CGoodsGrid::_nTmpY		= 0;  
int CGoodsGrid::_nTmpRow	= 0;	
int CGoodsGrid::_nTmpCol	= 0;

CGoodsGrid::CGoodsGrid(CForm& frmOwn)
: CCommandCompent(frmOwn)
, _nLeftMargin(0), _nTopMargin(0), _nRightMargin(0), _nBottomMargin(0)
, _nUnitHeight(32), _nUnitWidth(32)
, _nSpaceX(2), _nSpaceY(2)
, _nRow(0), _nCol(0), _nMaxNum(0), _pItems(NULL)
, _nFirst(0), _nLast(0), _nTmpIndex(0), _eShowStyle(enumSmall)
, evtThrowItem(NULL), evtSwapItem(NULL), evtUseCommand(NULL), evtBeforeAccept(NULL), evtRMouseEvent(NULL)
, _nCurNum(0), _IsShowHint(true)
{
	_pImage = new CGuiPic( this );
	_pUnit = new CGuiPic( this );
	_pScroll = new CScroll( frmOwn );
	_pDrag = new CDrag;


	selectItemImage = std::make_unique<CGuiPic>();
	selectItemImage->LoadImage("texture/ui/StartF.dds", 32, 32, 0, 124, 155);

	_SetSelf();
}

CGoodsGrid::CGoodsGrid(const CGoodsGrid& rhs)
: CCommandCompent(rhs), _pItems(NULL), _nRow(0), _nCol(0)
{
	_pScroll = new CScroll(*rhs._pScroll);
	_pImage = new CGuiPic(*rhs._pImage);
	_pUnit = new CGuiPic(*rhs._pUnit);

	//if( _pDrag ) delete _pDrag;
	SAFE_DELETE(_pDrag); // UI
    _pDrag = new CDrag;

	_Copy(rhs);
}

CGoodsGrid& CGoodsGrid::operator=(const CGoodsGrid& rhs)
{
	CCommandCompent::operator =(rhs);

	_ClearItem();

	*_pScroll = *rhs._pScroll;
	*_pImage = *rhs._pImage;
	*_pUnit = *rhs._pUnit;

	_Copy(rhs);
    return *this;
}

void CGoodsGrid::_Copy( const CGoodsGrid& rhs )
{
	_nLeftMargin = rhs._nLeftMargin;
	_nTopMargin = rhs._nTopMargin;
	_nRightMargin= rhs._nRightMargin;
	_nBottomMargin = rhs._nBottomMargin;

	_nUnitHeight = rhs._nUnitHeight;
	_nUnitWidth = rhs._nUnitWidth;

	_nSpaceX = rhs._nSpaceX;
	_nSpaceY = rhs._nSpaceY;

	_nMaxNum = rhs._nMaxNum;

	_nTotalW = rhs._nTotalW;
	_nTotalH = rhs._nTotalW;

	evtThrowItem = rhs.evtThrowItem;
    evtSwapItem = rhs.evtSwapItem;
    evtBeforeAccept = rhs.evtBeforeAccept;
    evtUseCommand = rhs.evtUseCommand;
	evtRMouseEvent = rhs.evtRMouseEvent;

	_eShowStyle = rhs._eShowStyle;
	_IsShowHint = rhs._IsShowHint;

	SetContent( rhs._nRow, rhs._nCol );

	//for( int i=0; i<_nMaxNum; i++ )
	//{
	//	if( rhs._pItems[i] )
	//		_pItems[i] = rhs._pItems[i]->Clone();
	//}

	_nFirst = rhs._nFirst;
	_nLast = rhs._nLast;

	_SetSelf();
}

void CGoodsGrid::_SetSelf()
{
	_pScroll->SetParent(this);
	_pScroll->SetIsShow(false);
	_pScroll->evtChange = _OnScrollChange;

	_pImage->SetParent(this);

	if( _pDrag )
	{
		_pDrag->SetIsMove( false );
		_pDrag->evtMouseDragBegin = NULL;
		_pDrag->evtMouseDragMove = NULL;
		_pDrag->evtMouseDragEnd = _DragEnd;
	}

	_pDragItem = nullptr;
    _nDragIndex = -1;
}

CGoodsGrid::~CGoodsGrid()
{
	_ClearItem();

	//delete _pImage;
	//delete _pUnit;

	SAFE_DELETE(_pImage);	// UI
	SAFE_DELETE(_pImage);	// UI
}

void CGoodsGrid::SetAlpha( BYTE alpha )
{
	_pScroll->SetAlpha( alpha );
	_pImage->SetAlpha( alpha );
	_pUnit->SetAlpha( alpha );
}

void CGoodsGrid::Render()
{
	_pImage->Render(GetX(), GetY());

	static int i;
	static int x, y, col;
	x = _nStartX;
	y = _nStartY;
	col = 0;

	if( !_pUnit->IsNull() )
	{
		for( i=_nFirst; i<_nLast; i++ )
		{
			_pUnit->Render( x, y );
			if( ++col >= _nCol )
			{
				col = 0;
				y += _nTotalH;
				x = _nStartX;
			}
			else
			{
				x += _nTotalW;
			}
		}
		x = _nStartX;
		y = _nStartY;
		col = 0;
	}
	if( _eShowStyle==enumSmall )
	{
		for( i=_nFirst; i<_nLast; i++ )
		{
			if( _pItems[i] ) _pItems[i]->Render( x, y );
			if (selectorEnabled && selectedItemIndicies[i])  selectItemImage->Render(x, y);

			if( ++col >= _nCol )
			{
				col = 0;
				y += _nTotalH;
				x = _nStartX;
			}
			else
			{
				x += _nTotalW;
			}
		}
	}
	else if( _eShowStyle == enumSale )
	{
		for( i=_nFirst; i<_nLast; i++ )
		{
			if( _pItems[i] ) _pItems[i]->SaleRender( x, y, _nUnitWidth, _nUnitHeight );

			if( ++col >= _nCol )
			{
				col = 0;
				y += _nTotalH;
				x = _nStartX;
			}
			else
			{
				x += _nTotalW;
			}
		}
	}
	else if( _eShowStyle == enumOwnDef )
	{
		for( i=_nFirst; i<_nLast; i++ )
		{
			if( _pItems[i] ) _pItems[i]->OwnDefRender( x, y, _nUnitWidth, _nUnitHeight );

			if( ++col >= _nCol )
			{
				col = 0;
				y += _nTotalH;
				x = _nStartX;
			}
			else
			{
				x += _nTotalW;
			}
		}
	}

    if( _pScroll->GetIsShow() ) _pScroll->Render();
}

void CGoodsGrid::Refresh()
{
	CCompent::Refresh();

	_pImage->Refresh();
	_pScroll->Refresh();

	_nStartX = GetX() + _nLeftMargin;
	_nStartY = GetY() + _nTopMargin;

	_nPageShowNum = _nCol * (int)( ( GetHeight() - _nBottomMargin - _nTopMargin ) / _nTotalH );
	_OnScrollChange();
}

bool CGoodsGrid::MouseRun( int x, int y, DWORD key )
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);
	if( !IsNormal() ){
		return false;
	} 

	if( InRect( x, y ) )
	{
		if( (key & Mouse_LDown) && !_isChild && GetActive()!=this ) _SetActive();

		if( _pScroll->MouseRun( x, y, key ) ) return true;
		
		_GetHitItem( x, y );
		if( _nTmpIndex==-1 ) return true;

		// The behavior of "Locked Slot" item 
		if (_pItems[_nTmpIndex])
		{
			CItemCommand* item = dynamic_cast<CItemCommand*>(_pItems[_nTmpIndex]);
			if(!item->GetCanDrag()){
				return false;
			}
		}

		if (!g_pGameApp->IsCtrlPress()
			&& ((key & Mouse_LDown) ||
				(key & Mouse_RDown) ||
				(key & Mouse_LDB))
			)
		{
			ResetItemSelections();
		}

		if (key & Mouse_LDown)
		{
			// Evaluate occupied slots only
			if (_pItems[_nTmpIndex])
			{
				if (g_pGameApp->IsCtrlPress())
				{
					selectedItemIndicies[_nTmpIndex] = !selectedItemIndicies[_nTmpIndex];
					return true;
				}

				if (_pItems[_nTmpIndex]->MouseDown())
				{
					return true;
				}
			}
		}

		
		if( key & Mouse_RDown ){
			if (_pItems[_nTmpIndex]) {
				// Select a single item whenever not pressing CTRL
				if (!g_pGameApp->IsCtrlPress())
				{
					selectedItemIndicies[_nTmpIndex] = true;
				}


				if( evtRMouseEvent ){		
					// Right click only works on a selected item
					if(selectedItemIndicies[_nTmpIndex])
					{
						evtRMouseEvent(this, _pItems[_nTmpIndex], _nTmpIndex);
					}
				}
				/*else{
					_pItems[_nTmpIndex]->ExecRightClick();
				}*/
				return true;
			}
		}
		
        if( key & Mouse_LDB )
        {
			if( _pItems[_nTmpIndex] )
			{				
				_pItems[_nTmpIndex]->Exec();
			}
			return true;
		}

		if( _pDrag && _pDrag->BeginMouseRun(this, _IsMouseIn, x, y, key )==CDrag::stDrag )
		{				
			_GetHitItem( _pDrag->GetStartX(), _pDrag->GetStartY() );
			if( _nTmpIndex==-1 ) 
			{
				_pDrag->Reset();
				return true;
			}

			if( _pItems[_nTmpIndex] )
			{
				_pDragItem = _pItems[_nTmpIndex];

				_nDragIndex = _nTmpIndex;
				_nDragOffX = _nTmpX;
				_nDragOffY = _nTmpY;
				_nDragRow = _nTmpRow;
				_nDragCol = _nTmpCol;
			}
			else
			{
				_pDrag->Reset();
			}
		}
	}

	return _IsMouseIn;
}

int CGoodsGrid::_GetHitItem( int x, int y )
{
	_nTmpX = x - _nStartX;
	_nTmpCol = _nTmpX / _nTotalW;
	if( _nTmpCol>=_nCol ) 
	{
		_nTmpIndex = -1;
		return _nTmpIndex;
	}

	_nTmpY = y - _nStartY;
	_nTmpRow = _nTmpY / _nTotalH;
	_nTmpIndex = _nTmpRow * _nCol + _nTmpCol + _nFirst;
	if( _nTmpIndex >= _nLast ) 
	{
		_nTmpIndex = -1;
		return _nTmpIndex;
	}

	_nTmpX %= _nTotalW;
	_nTmpY %= _nTotalH;
	return _nTmpIndex;
}

void CGoodsGrid::_OnScrollChange()
{
	_nFirst = (int)_pScroll->GetStep().GetPosition() * _nCol;
	_nLast = _nFirst + _nPageShowNum;
	if( _nLast>_nMaxNum ) _nLast=_nMaxNum;
}

//#include "ItemRecord.h"
//#include "uiitemcommand.h"
void CGoodsGrid::Init()
{
	_nTotalW = _nSpaceX + _nUnitWidth;
	_nTotalH = _nSpaceY + _nUnitHeight;

	_pUnit->SetScale( _nUnitWidth, _nUnitHeight );

	_pScroll->SetPos( GetWidth() - _nRightMargin - _pScroll->GetWidth(), _nTopMargin );

    //// 
    //for( int i=0; i<100; i++ )
    //{
    //    if( rand() % 3 )
    //    {
    //        int nItemScript = 25;
    //        CItemRecord* pInfo = GetItemRecordInfo(nItemScript);
    //        CItemCommand* pItem = new CItemCommand( pInfo );
    //        pItem->SetIsValid(rand()%2);
    //        pItem->SetIsSolid(rand()%2);
    //        pItem->nTag = nItemScript;
    //        pItem->SetTotalNum( rand() % 10 + 1 );
    //        SetItem( i, pItem );
    //    }
    //}

	int nHeight = GetHeight() - _nBottomMargin - _nTopMargin;
	int nRowHeight = nHeight / _nTotalH;
	float fMax = (float)( _nRow - nRowHeight );
	_pScroll->SetSize( _pScroll->GetWidth(), nHeight );
	_pScroll->SetRange( 0.0f, fMax );
	// xuqin modified
	//_pScroll->SetRange( 0.0f, _nRow );
	_pScroll->Init();
}

void CGoodsGrid::SetMargin( int left, int top, int right, int bottom ) 
{ 
	_nLeftMargin = left;
	_nTopMargin = top;
	_nRightMargin= right;
	_nBottomMargin = bottom; 
}

bool CGoodsGrid::SetContent( int nRow, int nCol )
{
	if( nCol<=0 || nRow<=0 ) return false;

	_nCurNum = 0;
	if( nCol==_nCol && nRow==_nRow ) return true;

    // :
    // 1.,_pItems
    // 2.,_pItems,,

	// _ClearItem();

    int oldMax = _nMaxNum;
    CCommandObj**  pOldItems = _pItems;

	_nRow = nRow;
	_nCol = nCol;

	_nMaxNum = nRow*nCol;

	_pItems = new CCommandObj*[_nMaxNum];
	selectedItemIndicies = std::make_unique<bool[]>(_nMaxNum);
	memset( _pItems, 0, sizeof(CCommandObj*)*_nMaxNum);

    if( pOldItems )
    {
        int nMin = oldMax<_nMaxNum ? oldMax : _nMaxNum;
        memcpy( _pItems, pOldItems, sizeof(CCommandObj*)*nMin );
        for( int i=nMin; i<oldMax; i++ )
        {
            //if( pOldItems[i] )
            //{
            //    delete pOldItems[i];
            //}

			SAFE_DELETE(pOldItems[i]);	// UI
        }
    }
	Init();
	return true;
}

bool CGoodsGrid::SetItem( unsigned int nIndex, CCommandObj* pItem )
{
	if( nIndex >= (unsigned int)_nMaxNum ) return false;

	if( _pItems[nIndex] ) 
	{
		delete _pItems[nIndex];
		_pItems[nIndex] = 0;	// UI
	}
	else
	{
		_nCurNum++;
	}

	_pItems[nIndex] = pItem;
    pItem->SetParent( this );
    pItem->SetIndex( nIndex );
	return true;
}

bool CGoodsGrid::DelItem( int nIndex )
{
	if( nIndex >= _nMaxNum ) return false;

	if( _pItems[nIndex] )
	{
		_nCurNum--;
		if( _pDragItem==_pItems[nIndex] )
		{
			if( CDrag::GetParent()==this && _pDrag )
			{
				_pDrag->Reset();
			}
			_pDragItem = nullptr;
		}

		delete _pItems[nIndex];
		_pItems[nIndex] = NULL;
		return true;
	}

	return false;
}

int CGoodsGrid::GetFreeIndex()
{
	for( int i=0; i<_nMaxNum; i++ )	
	{
		if( !_pItems[i] )
		{
			return i;
		}
	}
	return -1;
}

void CGoodsGrid::Clear()
{
	_pDragItem = nullptr;
    for( int i=0; i<_nMaxNum; i++ )
    {
        if( _pItems[i] ) 
        {
            delete _pItems[i];
            _pItems[i] = NULL;
        }
    }
}

void CGoodsGrid::_ClearItem()
{
	_pDragItem = nullptr;
	if( _pItems ) 
	{
		for( int i=0; i<_nMaxNum; i++ )
		{
			//if( _pItems[i] ) 
			//	delete _pItems[i];
			SAFE_DELETE(_pItems[i]); // UI
		}

		//delete [] _pItems;
		//_pItems = NULL;
		SAFE_DELETE_ARRAY(_pItems); // UI

		_nMaxNum = 0;
		_nRow = 0;
		_nCol = 0;
	}

	selectedItemIndicies.reset();
}

bool CGoodsGrid::MouseScroll( int nScroll )
{
	if( !IsNormal() ) return false;

	if( _IsMouseIn ) _pScroll->MouseScroll( nScroll );
	return _IsMouseIn;
}

void CGoodsGrid::DragRender()
{	
	if( _pDragItem ) 
	{
		if( _eShowStyle==enumSmall )
		{
			_pDragItem->Render( _pDrag->GetX() - _nDragOffX, _pDrag->GetY() - _nDragOffY );
		}
		else if(  _eShowStyle==enumSale )
		{
			_pDragItem->SaleRender( _pDrag->GetX() - _nDragOffX, _pDrag->GetY() - _nDragOffY, _nUnitWidth, _nUnitHeight );
		}
		else if(  _eShowStyle==enumOwnDef )
		{
			_pDragItem->OwnDefRender( _pDrag->GetX() - _nDragOffX, _pDrag->GetY() - _nDragOffY, _nUnitWidth, _nUnitHeight );
		}
	}
}

void CGoodsGrid::_DragEnd( int x, int y, DWORD key )
{
	_pDragItem = nullptr;
	if( InRect( x, y ) )
	{
		_GetHitItem( x, y );
		if( _nTmpIndex!=-1 )
		{
			if( _nTmpIndex!=_nDragIndex )
			{
				bool isSwap = false;
				if( evtSwapItem ) evtSwapItem( this, _nTmpIndex, _nDragIndex, isSwap );

				if( isSwap ) swap( _pItems[_nTmpIndex], _pItems[_nDragIndex] );
			}
		}
		return;
	}

	CForm * form = CFormMgr::s_Mgr.GetHitForm( x, y );
	if( form )
	{
		CCompent* p = form->GetHitCommand( x, y );     
		if( !p ) return;

		switch( p->SetCommand( _pItems[_nDragIndex], x, y ) )
		{
		case enumFast:
			break;
		case enumAccept:
			_pItems[_nDragIndex] = NULL;
			break;
		case enumRefuse:
			break;
		}
	}
	else
	{
		if( _pItems[_nDragIndex] )
		{
            bool isThrow = false;

            if( evtThrowItem ) evtThrowItem(this, _nDragIndex, isThrow);
            if( isThrow )
            {
                delete _pItems[_nDragIndex];
                _pItems[_nDragIndex] = NULL;
            }
		}
	}
}

eAccept	CGoodsGrid::SetCommand( CCommandObj* p, int x, int y )
{
	if( !p ) return enumRefuse;

    _GetHitItem( x, y );
    if( _nTmpIndex!=-1 )
    {
        bool isAccept = false;
        if( evtBeforeAccept ) evtBeforeAccept( this, p, _nTmpIndex, isAccept );

        if( isAccept )
        {
            if( _pItems[_nTmpIndex] )
            {
                delete _pItems[_nTmpIndex];
				_pItems[_nTmpIndex] = NULL;
				_nCurNum--;
            }

            SetItem( _nTmpIndex, p );
            return enumAccept;
        }
    }
	return enumRefuse;
}

void CGoodsGrid::RenderHint( int x, int y )
{
    if( _nTmpIndex!=-1 && _pItems[_nTmpIndex] ) 
    {
        _pItems[_nTmpIndex]->RenderHint( x, y );
    }
    else
    {
        _RenderHint( _strHint.c_str(), x, y );
    }
}

CCompent* CGoodsGrid::GetHintCompent( int x, int y )
{
    if( GetIsShow() && InRect( x, y ) )
    {
        if( !_strHint.empty() )
            return this;

		if( _IsShowHint )
		{
			_GetHitItem( x, y );
			if( _nTmpIndex!=-1 && _pItems[_nTmpIndex] && SetHintItem( _pItems[_nTmpIndex] ) ) 
			{
				
				return this;
			}
		}
    }
    return NULL;
}

void CGoodsGrid::GetFreeIndex( int* nFree, int& nCount, int nSize )
{
    nCount = 0;
    if( nSize<=0 ) return;

    for( int i=0; i<_nMaxNum; i++ )
    {
        if( !_pItems[i] )
        {
            nFree[nCount++] = i;
            if( nCount>=nSize )
            {
                return;
            }
        }
    }
}

int CGoodsGrid::FindCommand( CCommandObj* p )
{
    for( int i=0; i<_nMaxNum; i++ )
    {
        if( _pItems[i]==p )
        {
            return i;
        }
    }
    return -1;
}

bool CGoodsGrid::SwapItem( unsigned int nFirst, unsigned int nSecond )
{
	if( nFirst>(unsigned int)_nMaxNum || nSecond>(unsigned int)_nMaxNum ) return false;

	_pDragItem = nullptr;

	CCommandObj* tmp = _pItems[nFirst];
	_pItems[nFirst] = _pItems[nSecond];
	_pItems[nSecond] = tmp;
	if( _pItems[nFirst] )
	{
		_pItems[nFirst]->SetIndex( nFirst );
	}
	if( _pItems[nSecond] )
	{
		_pItems[nSecond]->SetIndex( nSecond );
	}
	return true;
}

void CGoodsGrid::Reset()
{
	_pScroll->Reset();
}

void CGoodsGrid::SetItemValid( bool v )
{
    for( int i=0; i<_nMaxNum; i++ )
    {
        if( _pItems[i] )
        {
			_pItems[i]->SetIsValid( v );
        }
    }
}


int CGoodsGrid::GetEmptyGridCount()
{
	int nCount = 0;
    for( int i=0; i<_nMaxNum; i++ )
    {
        if( ! _pItems[i] )
        {
			++nCount;
        }
    }

	return nCount;
}

int CGoodsGrid::GetUsedGridCount()
{
	int nCount = 0;
    for( int i=0; i<_nMaxNum; i++ )
    {
        if( _pItems[i] )
        {
			++nCount;
        }
    }

	return nCount;
}

void CGoodsGrid::ResetItemSelections()
{
	for (auto& b : std::span(selectedItemIndicies.get(), _nMaxNum))
	{
		b = false;
	}
}

bool GUI::CGoodsGrid::IsItemSelected(int index) const
{
	if (index > _nMaxNum || index < 0)
		return false;

	return selectedItemIndicies[index];
}

int GUI::CGoodsGrid::GetSelectedItemCount() const
{
	return std::count(selectedItemIndicies.get(), selectedItemIndicies.get() + _nMaxNum, true);
}
