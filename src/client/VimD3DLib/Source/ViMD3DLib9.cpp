//ViMD3DLib9.cpp
//-------------------------------------------------------------------

#include <d3d9.h>
#include <tchar.h>

//-------------------------------------------------------------------

//VisuMotion D3D object
typedef void* VIMD3D;

typedef BOOL (*FN_ViMD3D_CheckLibVersion)(DWORD);
typedef BOOL (*FN_ViMD3D_IsDisplay)();
typedef BOOL (*FN_ViMD3D_IsLicense)();
typedef BOOL (*FN_ViMD3D_GetInfo)(DWORD&, DWORD&, DWORD&, DWORD&, DWORD&, DWORD&, DWORD&);
typedef BOOL (*FN_ViMD3D_Create9)(IDirect3DDevice9*, VIMD3D*);
typedef BOOL (*FN_ViMD3D_Release9)(VIMD3D);
typedef BOOL (*FN_ViMD3D_RasterTextures9)(VIMD3D, DWORD, DWORD, DWORD, DWORD, DWORD, IDirect3DTexture9**);
typedef BOOL (*FN_ViMD3D_RasterTexturesUV9)(VIMD3D, DWORD, DWORD, DWORD, DWORD, DWORD, IDirect3DTexture9**, float* uv);
typedef BOOL (*FN_ViMD3Dutil_GetCamFocalTranslation)(float&, float&, float, float, float, float, float, DWORD, DWORD);
typedef BOOL (*FN_ViMD3Dutil_MatrixPerspectiveFovLH)(D3DMATRIX*, float, float, float, float, float, float, DWORD, DWORD);
typedef BOOL (*FN_ViMD3Dutil_MatrixPerspectiveFovRH)(D3DMATRIX*, float, float, float, float, float, float, DWORD, DWORD);
typedef BOOL (*FN_ViMD3Dutil_MatrixModify)(D3DMATRIX*, float, float, float, float, DWORD, DWORD);

FN_ViMD3D_CheckLibVersion ViMD3D_CheckLibVersion;
FN_ViMD3D_IsDisplay ViMD3D_IsDisplay;
FN_ViMD3D_IsLicense ViMD3D_IsLicense;
FN_ViMD3D_GetInfo ViMD3D_GetInfo;
FN_ViMD3D_Create9 ViMD3D_Create9;
FN_ViMD3D_Release9 ViMD3D_Release9;
FN_ViMD3D_RasterTextures9 ViMD3D_RasterTextures9;
FN_ViMD3D_RasterTexturesUV9 ViMD3D_RasterTexturesUV9;
FN_ViMD3Dutil_GetCamFocalTranslation ViMD3Dutil_GetCamFocalTranslation;
FN_ViMD3Dutil_MatrixPerspectiveFovLH ViMD3Dutil_MatrixPerspectiveFovLH;
FN_ViMD3Dutil_MatrixPerspectiveFovRH ViMD3Dutil_MatrixPerspectiveFovRH;
FN_ViMD3Dutil_MatrixModify ViMD3Dutil_MatrixModify;

//-------------------------------------------------------------------

static BOOL _PrintError(TCHAR* szError, UINT err) {
	if (szError) {
		switch (err) {
		case 0:
			*szError = 0;
			break;
		case 1:
			lstrcpy(szError, _T("Could not load ViMD3DLib.dll or library broken."));
			break;
		case 2:
			lstrcpy(szError, _T("Wrong version of ViMD3DLib.dll."));
			break;
		case 3:
			lstrcpy(szError, _T("No valid license found."));
			break;
		case 4:
			lstrcpy(szError, _T("No WiseVision 3d display configured."));
			break;
		default:
			lstrcpy(szError, _T("Unknown error."));
			break;
		}
	}

	return (err == 0) ? TRUE : FALSE;
}

//-------------------------------------------------------------------

BOOL ViMD3D_InitLib9(TCHAR* szError, UINT version) {
	HMODULE hLib;
	if ((hLib = LoadLibrary(_T("ViMD3DLib.dll")))) {
		if (!(ViMD3D_Create9 = (FN_ViMD3D_Create9)GetProcAddress(hLib, "ViMD3D_Create9")))
			return _PrintError(szError, 1);
		if (!(ViMD3D_Release9 = (FN_ViMD3D_Release9)GetProcAddress(hLib, "ViMD3D_Release9")))
			return _PrintError(szError, 1);
		if (!(ViMD3D_RasterTextures9 = (FN_ViMD3D_RasterTextures9)GetProcAddress(hLib, "ViMD3D_RasterTextures9")))
			return _PrintError(szError, 1);
		if (!(ViMD3D_RasterTexturesUV9 = (FN_ViMD3D_RasterTexturesUV9)GetProcAddress(hLib, "ViMD3D_RasterTexturesUV9")))
			return _PrintError(szError, 1);
		if (!(ViMD3D_CheckLibVersion = (FN_ViMD3D_CheckLibVersion)GetProcAddress(hLib, "ViMD3D_CheckLibVersion")))
			return _PrintError(szError, 1);
		if (!(ViMD3D_IsDisplay = (FN_ViMD3D_IsDisplay)GetProcAddress(hLib, "ViMD3D_IsDisplay")))
			return _PrintError(szError, 1);
		if (!(ViMD3D_IsLicense = (FN_ViMD3D_IsLicense)GetProcAddress(hLib, "ViMD3D_IsLicense")))
			return _PrintError(szError, 1);
		if (!(ViMD3D_GetInfo = (FN_ViMD3D_GetInfo)GetProcAddress(hLib, "ViMD3D_GetInfo")))
			return _PrintError(szError, 1);
		if (!(ViMD3Dutil_GetCamFocalTranslation = (FN_ViMD3Dutil_GetCamFocalTranslation)GetProcAddress(hLib, "ViMD3Dutil_GetCamFocalTranslation")))
			return _PrintError(szError, 1);
		if (!(ViMD3Dutil_MatrixPerspectiveFovLH = (FN_ViMD3Dutil_MatrixPerspectiveFovLH)GetProcAddress(hLib, "ViMD3Dutil_MatrixPerspectiveFovLH")))
			return _PrintError(szError, 1);
		if (!(ViMD3Dutil_MatrixPerspectiveFovRH = (FN_ViMD3Dutil_MatrixPerspectiveFovRH)GetProcAddress(hLib, "ViMD3Dutil_MatrixPerspectiveFovRH")))
			return _PrintError(szError, 1);
		if (!(ViMD3Dutil_MatrixModify = (FN_ViMD3Dutil_MatrixModify)GetProcAddress(hLib, "ViMD3Dutil_MatrixModify")))
			return _PrintError(szError, 1);

		if (!ViMD3D_CheckLibVersion(version))
			return _PrintError(szError, 2);

		if (!ViMD3D_IsLicense())
			return _PrintError(szError, 3);

		if (!ViMD3D_IsDisplay())
			return _PrintError(szError, 4);

		return _PrintError(szError, 0);
	}

	return _PrintError(szError, 1);
}

//-------------------------------------------------------------------
//EOF
