#include "StdAfx.h"
#include "MPRender.h"
#include "MPGUI.h"
#include "MPTextureSet.h"




void CGuiButton::_Render()
{
	CGuiStateParam *pParam = &(m_State[_nCurState]);

	MPTexRect *pBack = &(pParam->BackgroundImage);

	//pBack->nTexW = _nW;
	//pBack->nTexH = _nH;
	
	g_Render.RenderTextureRect(GetAbsX(), GetAbsY(), pBack); 
	
	MPTexRect *pFrame = &(pParam->FrameImage);
	if(pFrame->nTextureNo!=-1)
	{
		g_Render.RenderTextureRect(GetAbsX(), GetAbsY(), pFrame); 
	}
}

void CGuiWindow::_Render()
{
	CGuiStateParam *pParam = &(m_State[_nCurState]);
	MPTexRect *pBack = &(pParam->BackgroundImage);
	g_Render.RenderTextureRect(GetAbsX(), GetAbsY(), pBack); 
}
