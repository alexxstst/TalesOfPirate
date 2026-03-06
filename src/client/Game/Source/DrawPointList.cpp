#include "StdAfx.h"
#include "DrawPointList.h"
#include "EffectObj.h"
#include "GameApp.h"
#include "Scene.h"
#include "MPFont.h"

using namespace std;

//static CEffectBox  CPathBox;

CDrawPointList::CDrawPointList(void)
: _nMaxCount(10), _bIsEnabled(false)
{
}

CDrawPointList::~CDrawPointList(void)
{
	Clear();
}

void CDrawPointList::Reader()
{
	if( !_bIsEnabled ) return;

	if(points.empty())
		return;

	static BOOL        bCreatePathBox = FALSE;

	//if(bCreatePathBox==FALSE)
	{
		//CPathBox.Create(g_Render.GetDevice(),0.25f);
		CPathBox.Show(TRUE);
		CPathBox.setWriteFrame(TRUE);
		CPathBox.ShowLine(TRUE);
		CPathBox.ShowBox(TRUE);
		CPathBox.setScale( 0.2f, 0.2f, 0.2f );
		CPathBox.setColor(0xff00ffff);

		bCreatePathBox = TRUE;
	}
	for(unsigned int m = 0; m < points.size(); m++)
	{
		CPathBox.setColor( points[m].color );
		CPathBox.setPos(points[m].pos);
		CPathBox.Render();

		if( points[m].color == 0xffff0000 )
		{
			static char buf[80] = { 0 };
			sprintf (buf, "%d,%d", (int)points[m].pos.x, (int)points[m].pos.y );
			g_pGameApp->GetFont()->Draw3DText( buf, points[m].pos, 0xff000000, 0.3f );
		}
	}
}

void CDrawPointList::Add( int x, int y, DWORD c, float h ) 
{ 
	if( !_bIsEnabled ) return;

	if( points.size()>_nMaxCount ) points.clear();

	float fX = (float)x / 100.0f;
	float fY = (float)y / 100.0f;
    float fZ = CGameApp::GetCurScene()->GetGridHeight( fX, fY ) + h; 

	points.push_back( stPoint( D3DXVECTOR3(fX, fY, fZ), c ) );
}

void CDrawPointList::Add( D3DXVECTOR3 p, DWORD c, float h )		
{ 
	if( !_bIsEnabled ) return;

	if( points.size()>_nMaxCount ) points.clear();

	p.z += h;
	points.push_back(stPoint(p,c));	
}

void	CDrawPointList::OnLostDevice()
{
	CPathBox.ReleaseBox();
}

void	CDrawPointList::OnResetDevice()
{
	CPathBox.Create(g_Render.GetDevice(),0.25f);
	CPathBox.Show(TRUE);
	CPathBox.setWriteFrame(TRUE);
	CPathBox.ShowLine(TRUE);
	CPathBox.ShowBox(TRUE);
	CPathBox.setScale( 0.2f, 0.2f, 0.2f );
	CPathBox.setColor(0xff00ffff);
}
