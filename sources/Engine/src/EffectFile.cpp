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
CMPEffectFile::CMPEffectFile(MPRender*	 pDev)
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
void CMPEffectFile::InitDev(MPRender* pDev)
{
    m_pDev = pDev;
}

BOOL CMPEffectFile::LoadEffectFromFile( LPCSTR pszfile)
{
	HRESULT hr;
	if(hr = D3DXCreateEffectFromFile(m_pDev->GetDevice(),pszfile,NULL,NULL,0,NULL,&m_pEffect,NULL); FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] D3DXCreateEffectFromFile failed: file={}, hr=0x{:08X}",
		             __FUNCTION__, pszfile ? pszfile : "(null)", static_cast<std::uint32_t>(hr));
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
		if(HRESULT hr = m_pEffect->OnLostDevice(); FAILED(hr))
		{
			ToLogService("errors", LogLevel::Error,
			             "[{}] OnLostDevice failed: hr=0x{:08X}",
			             __FUNCTION__, static_cast<std::uint32_t>(hr));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMPEffectFile::OnResetDevice()
{
    if(m_pEffect)
	{
		if(HRESULT hr = m_pEffect->OnResetDevice(); FAILED(hr))
		{
			ToLogService("errors", LogLevel::Error,
			             "[{}] OnResetDevice failed: hr=0x{:08X}",
			             __FUNCTION__, static_cast<std::uint32_t>(hr));
			return FALSE;
		}
	}
	return TRUE;
}
MPRender*	CMPEffectFile::GetDev()
{
	return m_pDev;
}

BOOL CMPEffectFile::SetTechnique(int iIdx)
{
	if (HRESULT hr = m_pEffect->SetTechnique(_vecTechniques[iIdx]); FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] SetTechnique failed: iIdx={}, tech_num={}, hr=0x{:08X}",
		             __FUNCTION__, iIdx, _iTechNum, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::SetTexture(LPCSTR TextureValue, IDirect3DTextureX* pTexture)
{
	if (HRESULT hr = m_pEffect->SetTexture(TextureValue, pTexture); FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] SetTexture failed: name={}, tex_ptr=0x{:X}, hr=0x{:08X}",
		             __FUNCTION__, TextureValue ? TextureValue : "(null)",
		             reinterpret_cast<std::uintptr_t>(pTexture), static_cast<std::uint32_t>(hr));
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
	if (HRESULT hr = m_pEffect->Begin(&m_passes, dwIsSave); FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] Effect::Begin failed: dwIsSave={}, hr=0x{:08X}",
		             __FUNCTION__, dwIsSave, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::Pass(UINT ipass)
{
	if (HRESULT hr = m_pEffect->BeginPass(ipass); FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] BeginPass failed: ipass={}, hr=0x{:08X}",
		             __FUNCTION__, ipass, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	if (HRESULT hr = m_pEffect->CommitChanges(); FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] CommitChanges failed: ipass={}, hr=0x{:08X}",
		             __FUNCTION__, ipass, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::End()
{
	if (HRESULT hr = m_pEffect->EndPass(); FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] EndPass failed: hr=0x{:08X}",
		             __FUNCTION__, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	if (HRESULT hr = m_pEffect->End(); FAILED(hr))
	{
		ToLogService("errors", LogLevel::Error,
		             "[{}] Effect::End failed: hr=0x{:08X}",
		             __FUNCTION__, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}

