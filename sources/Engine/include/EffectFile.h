// DXEffectFile.h: interface for the CMPEffectFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXEFFECTFILE_H__F7B73B81_55EE_11BD_AC67_0008C720ECD1__INCLUDED_)
#define AFX_DXEFFECTFILE_H__F7B73B81_55EE_11BD_AC67_0008C720ECD1__INCLUDED_

#if _MSC_VER > 1000
//#pragma once
#endif // _MSC_VER > 1000
class   MPRender;

#include "D3dx9effect.h"
#include "i_effect.h"

class CMPEffectFile  
{
public:
	CMPEffectFile(MPRender*	 pDev);
	CMPEffectFile();
	virtual ~CMPEffectFile();

	void InitDev(MPRender* pDev);

	MPRender*	GetDev();
	BOOL OnResetDevice();
	BOOL OnLostDevice();

	//BOOL LoadEffectFromResource(TCHAR* pszsrc);
	BOOL LoadEffectFromFile(LPCSTR pszfile);

	BOOL SetTechnique(int iIdx);

	//BOOL CreateVertexShader();
	//BOOL SetVertexShader();

	BOOL SetTexture(LPCSTR TextureValue,IDirect3DTextureX* pTexture);
	BOOL SetDword(LPCSTR DwName, DWORD dwvalue);

	BOOL Begin(DWORD  dwIsSave = 0);
	BOOL Pass(UINT ipass = 0);
	BOOL End();

	int	 GetTechCount()						{ return _iTechNum;}
	int  GetPassCount()						{ return m_passes;}
	void free();

public:
	MPRender*			 m_pDev;

	LPD3DXEFFECT         m_pEffect;
	UINT                 m_passes;

protected:
	int									_iTechNum;
	std::vector<D3DXHANDLE>		_vecTechniques;

	DWORD								_dwVShader;
};
#endif // !defined(AFX_DXEFFECTFILE_H__F7B73B81_55EE_11BD_AC67_0008C720ECD1__INCLUDED_)
