// DXEffectFile.cpp: implementation of the CMPEffectFile class.

#include "StdAfx.h"
#include "GlobalInc.h"

#include "EffectFile.h"
#include "MPRender.h"

// Construction/Destruction
CMPEffectFile::CMPEffectFile() {
	m_pEffect = NULL;
	m_pDev = NULL;
	_vecTechniques.clear();
	_iTechNum = 0;
	_dwVShader = 0;
}

CMPEffectFile::CMPEffectFile(MPRender* pDev) {
	m_pDev = pDev;
	m_pEffect = NULL;
	_vecTechniques.clear();
	_iTechNum = 0;
	_dwVShader = 0;
}

CMPEffectFile::~CMPEffectFile() {
	free();
}

void CMPEffectFile::InitDev(MPRender* pDev) {
	m_pDev = pDev;
}

BOOL CMPEffectFile::LoadEffectFromFile(LPCSTR pszfile) {
	HRESULT hr;
	if (hr = D3DXCreateEffectFromFile(m_pDev->GetDevice(), pszfile,NULL,NULL, 0,NULL, &m_pEffect,NULL); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] D3DXCreateEffectFromFile failed: file={}, hr=0x{:08X}",
					 __FUNCTION__, pszfile ? pszfile : "(null)", static_cast<std::uint32_t>(hr));
		return FALSE;
	}

	D3DXHANDLE technique;

	//	return FALSE;


	std::string t_psz = "t0";
	while (SUCCEEDED(m_pEffect->FindNextValidTechnique(t_psz.c_str(), &technique))) {
		_vecTechniques.push_back(technique);
		_iTechNum++;

		t_psz = std::format("t{}", _iTechNum);
	}


	return TRUE;
}

//                        m_pDev, pvData, cbData, 
//						&m_pEffect,NULL) ) )
//                        return FALSE;
//						return TRUE;
//		return FALSE;
//	return FALSE;


void CMPEffectFile::free() {
	SAFE_RELEASE(m_pEffect);
	_vecTechniques.clear();
	_iTechNum = 0;
	_dwVShader = 0;
}

BOOL CMPEffectFile::OnLostDevice() {
	if (m_pEffect) {
		if (HRESULT hr = m_pEffect->OnLostDevice(); FAILED(hr)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] OnLostDevice failed: hr=0x{:08X}",
						 __FUNCTION__, static_cast<std::uint32_t>(hr));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CMPEffectFile::OnResetDevice() {
	if (m_pEffect) {
		if (HRESULT hr = m_pEffect->OnResetDevice(); FAILED(hr)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] OnResetDevice failed: hr=0x{:08X}",
						 __FUNCTION__, static_cast<std::uint32_t>(hr));
			return FALSE;
		}
	}
	return TRUE;
}

MPRender* CMPEffectFile::GetDev() {
	return m_pDev;
}

BOOL CMPEffectFile::SetTechnique(int iIdx) {
	if (HRESULT hr = m_pEffect->SetTechnique(_vecTechniques[iIdx]); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] SetTechnique failed: iIdx={}, tech_num={}, hr=0x{:08X}",
					 __FUNCTION__, iIdx, _iTechNum, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::SetTexture(LPCSTR TextureValue, IDirect3DTextureX* pTexture) {
	if (HRESULT hr = m_pEffect->SetTexture(TextureValue, pTexture); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] SetTexture failed: name={}, tex_ptr=0x{:X}, hr=0x{:08X}",
					 __FUNCTION__, TextureValue ? TextureValue : "(null)",
					 reinterpret_cast<std::uintptr_t>(pTexture), static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::SetDword(LPCSTR DwName, DWORD dwvalue) {
	return TRUE;
}

BOOL CMPEffectFile::Begin(DWORD dwIsSave) {
	if (HRESULT hr = m_pEffect->Begin(&m_passes, dwIsSave); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] Effect::Begin failed: dwIsSave={}, hr=0x{:08X}",
					 __FUNCTION__, dwIsSave, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::Pass(UINT ipass) {
	if (HRESULT hr = m_pEffect->BeginPass(ipass); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] BeginPass failed: ipass={}, hr=0x{:08X}",
					 __FUNCTION__, ipass, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	if (HRESULT hr = m_pEffect->CommitChanges(); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] CommitChanges failed: ipass={}, hr=0x{:08X}",
					 __FUNCTION__, ipass, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}

BOOL CMPEffectFile::End() {
	if (HRESULT hr = m_pEffect->EndPass(); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] EndPass failed: hr=0x{:08X}",
					 __FUNCTION__, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	if (HRESULT hr = m_pEffect->End(); FAILED(hr)) {
		ToLogService("errors", LogLevel::Error,
					 "[{}] Effect::End failed: hr=0x{:08X}",
					 __FUNCTION__, static_cast<std::uint32_t>(hr));
		return FALSE;
	}
	return TRUE;
}
