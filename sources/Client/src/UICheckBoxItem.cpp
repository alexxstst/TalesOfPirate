#include "StdAfx.h"
#include "UIText.h"
#include "uicheckboxitem.h"

CCheckBoxItem::CCheckBoxItem(void)
{
}

CCheckBoxItem::~CCheckBoxItem(void)
{
}

void CCheckBoxItem::Render( int x, int y )	
{ 
	_pImage->Render(x, y);	    
	static int nOffY = ( 16 - ui::GetHeight( GetLanguageString(489).c_str() ) ) / 2;
	ui::Render( m_strCaption.c_str(), x + 20, y + nOffY, m_dwColor );
}
