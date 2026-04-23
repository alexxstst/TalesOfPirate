// DXEffectFile.cpp: implementation of the CMPEffectFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
//#include "../../../proj/EffectEditer.h"
//#include <mindpower.h>
#include "GlobalInc.h"

#include "EffectFile.h"
//#include "DXUtil.h"
#include "MPRender.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMPEffectFile::CMPEffectFile()
{
	m_pEffect = NULL;
	m_pDev = NULL;
	_vecTechniques.clear();
	_iTechNum = 0;
	_dwVShader = 0;
}
#ifdef USE_RENDER
CMPEffectFile::CMPEffectFile(MPRender*	 pDev)
#else
CMPEffectFile::CMPEffectFile(LPDIRECT3DDEVICE8 pDev)
#endif
{
    m_pDev = pDev;
    m_pEffect   = NULL;
	_vecTechniques.clear();
	_iTechNum = 0;
	_dwVShader = 0;

}
CMPEffectFile::~CMPEffectFile()
{
	free();
}
#ifdef USE_RENDER
void CMPEffectFile::InitDev(MPRender* pDev)
#else
void CMPEffectFile::InitDev(LPDIRECT3DDEVICE8 pDev)
#endif
{
    m_pDev = pDev;
}

BOOL CMPEffectFile::LoadEffectFromFile( LPCSTR pszfile)
{
	HRESULT hr;
#ifdef USE_RENDER
	if(FAILED( hr = D3DXCreateEffectFromFile(m_pDev->GetDevice(),pszfile,NULL,NULL,0,NULL,&m_pEffect,NULL)))
#else
	if(FAILED( hr = D3DXCreateEffectFromFile(m_pDev,pszfile,&m_pEffect,NULL)))
#endif
	{
		return FALSE;
	}

	D3DXHANDLE   technique;

	//if (FAILED(m_pEffect->FindNextValidTechnique(NULL, &technique)))
	//{
	//	return FALSE;
	//}
	//_vecTechniques.push_back(technique);
	//_iTechNum++;

	//_DbgOut( " technique.Name", _iTechNum, S_OK,  (TCHAR*)technique.Name );

	char t_psz[4];
	strcpy(t_psz, "t0");
	while(SUCCEEDED(m_pEffect->FindNextValidTechnique(t_psz, &technique)))
	{
		_vecTechniques.push_back(technique);
		_iTechNum++;
		//_DbgOut( " technique.Name", _iTechNum, S_OK,  (TCHAR*)technique.Name );

		sprintf(t_psz,"t%d",_iTechNum);
	}
	//_DbgOut( " technique.Name", _iTechNum, S_OK,  (TCHAR*)technique.Name );


	return TRUE;
}

//BOOL CMPEffectFile::LoadEffectFromResource(TCHAR*  pszsrc)
//{
//    HRESULT hr;
//    HMODULE hModule = NULL;
//    HRSRC rsrc;
//    HGLOBAL hgData;
//    LPVOID pvData;
//    DWORD cbData;
//	D3DXTECHNIQUE_DESC   technique;
//
//    rsrc = FindResource( hModule, pszsrc, "EFFECT" );
//    if( rsrc != NULL )
//    {
//        cbData = SizeofResource( hModule, rsrc );
//        if( cbData > 0 )
//        {
//            hgData = LoadResource( hModule, rsrc );
//            if( hgData != NULL )
//            {
//                pvData = LockResource( hgData );
//                if( pvData != NULL )
//                {
//                    if( FAILED( hr = D3DXCreateEffect( 
//                        m_pDev, pvData, cbData, 
//						&m_pEffect,NULL) ) )
//                    {
//                        return FALSE;
//                    }
//					else
//						return TRUE;
//                }
//            }
//        }
//    }
//	if (FAILED(m_pEffect->FindNextValidTechnique(NULL, &technique)))
//	{
//		return FALSE;
//	}
//	_vecTechniques.push_back(technique);
//	_iTechNum++;
//
//	while(SUCCEEDED(m_pEffect->FindNextValidTechnique(technique.Name, &technique)))
//	{
//		_vecTechniques.push_back(technique);
//		_iTechNum++;
//	}
//
//	return FALSE;
//}


void CMPEffectFile::free()
{
	SAFE_RELEASE(m_pEffect);
	_vecTechniques.clear();
	_iTechNum = 0;
	_dwVShader = 0;
//	if(_dwVShader)
//	{
//		m_pDev->DeleteVertexShader(_dwVShader);
//		_dwVShader = 0;
//	}
}

BOOL CMPEffectFile::OnLostDevice()
{
    if(m_pEffect)
	{
		if(FAILED(m_pEffect->OnLostDevice()))
			return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::OnResetDevice()
{
    if(m_pEffect)
	{
		if(FAILED(m_pEffect->OnResetDevice()))
			return FALSE;
	}
	return TRUE;
}
#ifdef USE_RENDER
MPRender*	CMPEffectFile::GetDev()
#else
LPDIRECT3DDEVICE8	CMPEffectFile::GetDev()
#endif
{
	return m_pDev;
}

BOOL CMPEffectFile::SetTechnique(int iIdx)
{
	if (FAILED(m_pEffect->SetTechnique(_vecTechniques[iIdx])))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::SetTexture(LPCSTR TextureValue, IDirect3DTextureX* pTexture)
{
	if (FAILED(m_pEffect->SetTexture(TextureValue, pTexture)))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::SetDword(LPCSTR DwName, DWORD dwvalue)
{
	return TRUE;
}

BOOL CMPEffectFile::Begin(DWORD dwIsSave)
{
	if (FAILED(m_pEffect->Begin(&m_passes, dwIsSave)))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::Pass(UINT ipass)
{
	if (FAILED(m_pEffect->BeginPass(ipass)) || FAILED(m_pEffect->CommitChanges()))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::End()
{
	if (FAILED(m_pEffect->EndPass()) || FAILED(m_pEffect->End()))
	{
		return FALSE;
	}
	return TRUE;
}

