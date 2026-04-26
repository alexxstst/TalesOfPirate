#include "StdAfx.h"
#include "GlobalInc.h"
#include "MPModelEff.h"

#include "mpresmanger.h"
#include "lwSysGraphics.h"
#include "mpeffectctrl.h"
#include "lwIUtil.h"
#include "ShaderLoad.h"
#include "MPRender.h"
#include "MPResourceSet.h"
#include "lwExpObj.h"
#include "lwPhysique.h"

using namespace std;

CMPResManger ResMgr;

//  Default — true: при старте прогреваем bone-кэш (animation/*.lab) и
//  geom-кэш для character meshes (model/character/*.lgo). Клиент читает
//  [Resources] preload_at_start из system.ini и зовёт SetResourcePreload(...)
//  ДО InitRes/InitRes3.
bool CMPResManger::s_resourcePreload = true;

CMPResManger::CMPResManger(void) {
	m_pDev = NULL;
	ZeroMemory(_pszTexPath, MAX_PATH);
	ZeroMemory(_pszMeshPath, MAX_PATH);
	ZeroMemory(_pszEFFectPath, MAX_PATH);

	_iTexNum = 0;
	_iMeshNum = 0;
	_iEffectNum = 0;
	_iVShaderNum = 0;

	_vecTexName.clear();
	_vecMeshName.clear();
	_mapMesh.clear();

	_vecTexList.clear();
	_vecMeshList.clear();
	_mapTexture.clear();

	_vecEffectList.clear();

	_vecVShader.clear();
	_dwShadeMapVS = 0L;
	_bMagr = false;
	_dwMinimapVS = 0L;

	_vecEffectParam.clear();

	_fSaveTime = 0;
	_fDailTime = 0;
	_fCurTime = 0;
	_pMatView = NULL;
	_pMatViewProj = NULL;


	_iPathNum = 0;
	_vecPathName.clear();
	_vecPath.clear();


	_iTobMeshNum = 0;
	_lstTobMeshs.clear();


	ZeroMemory(_psDefault, 256);

	WORD iw;
	_vecMeshList.resize(MAXMESH_COUNT);
	_vecPartCtrl.resize(MAXPART_COUNT);
	_vecPartCtrl.setsize(MAXPART_COUNT);

	for (iw = 0; iw < MAXPART_COUNT; iw++) {
		(*_vecPartCtrl[iw]) = NULL;
	}


	_iPartCtrlNum = 0;

	_vecPartArray.clear();
	_vecPartArray.resize(MAXMSG_COUNT);
	_vecValidID.resize(MAXMSG_COUNT);
	_vecValidID.setsize(MAXMSG_COUNT);
	for (iw = 0; iw < MAXMSG_COUNT; iw++) {
		_vecPartArray[iw] = NULL;
		*_vecValidID[iw] = iw;
	}


	m_bUseSoft = FALSE;

	m_pSys = 0;
	m_pSysGraphics = 0;

	m_bCanFrame = false;
	m_iCurFrame = 0;
}

CMPResManger::~CMPResManger(void) {
	for (size_t i(0); i < _vecMeshList.size(); i++) {
		SAFE_DELETE(_vecMeshList[i]);
	}
}

void CMPResManger::ReleaseTotalRes() {
	int iw;

	for (iw = 0; iw < _iPartCtrlNum; iw++) {
		CMPPartCtrl** C = _vecPartCtrl[iw];
		SAFE_DELETE(*C);
	}
	_vecPartCtrl.resize(0);
	_iPartCtrlNum = 0;

	if (_vecPartArray.size() > 0) {
		for (iw = 0; iw < MAXMSG_COUNT; iw++) {
			SAFE_DELETE(_vecPartArray[iw]);
		}
		_vecValidID.resize(0);
		_vecPartArray.clear();
	}
	_vecEffectList.clear();

	_CEffectFile.free();

	_dwShadeMapVS = 0L;
	_dwMinimapVS = 0L;

	_vecVShader.clear();
	_iVShaderNum = 0;

	_iPathNum = 0;
	_vecPathName.clear();
	_vecPath.clear();


	for (iw = 0; iw < _iTexNum; iw++) {
		SAFE_RELEASE(_vecTexList[iw]);
	}

	for (iw = 0; iw < _iMeshNum; iw++) {
		SAFE_DELETE(_vecMeshList[iw]);
	}
	std::list<CEffectModel*>::iterator iter = _lstTobMeshs.begin();
	std::list<CEffectModel*>::iterator end = _lstTobMeshs.end();
	for (; iter != end; ++iter) {
		SAFE_DELETE(*iter);
	}
	SAFE_DELETE(_CShadeModel);

	_iTexNum = 0;
	_iMeshNum = 0;
	_iEffectNum = 0;
	_iTobMeshNum = 0;

	_vecTexName.clear();
	_vecMeshName.clear();
	_vecEffectName.clear();

	_mapMesh.clear();
	_mapEffect.clear();
	_mapTexture.clear();

	_vecTexList.clear();
	_vecMeshList.clear();
	_lstTobMeshs.clear();

	_vecEffectParam.clear();
}

bool CMPResManger::InitRes2() {
	if (!LoadTotalMesh())
		return false;

	if (!LoadTotalEffect())
		return false;

	LoadTotalPath();

	return true;
}

bool CMPResManger::InitRes3() {
	LoadTotalRes();
	return true;
}

bool CMPResManger::InitRes(MPRender* pDev, D3DXMATRIX* pmat, D3DXMATRIX* pMatviewproj) {
	m_pDev = pDev;

	IDirect3DSurfaceX* pBackBuffer;
	m_pDev->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	pBackBuffer->GetDesc(&m_d3dBackBuffer);
	pBackBuffer->Release();

	RECT rc_client;
	m_pDev->GetInterfaceMgr()->dev_obj->GetWindowRect(NULL, &rc_client);
	_iFontBkWidth = (rc_client.right - rc_client.left) / 2;
	_iFontBkHeight = (rc_client.bottom - rc_client.top) / 2;


	D3DXMatrixOrthoLH(&_Mat2dViewProj, float(m_d3dBackBuffer.Width), float(m_d3dBackBuffer.Height), 0.0f, 1.0f);

	m_caps = m_pDev->GetOrgCap();
	if (m_caps.VertexShaderVersion < D3DVS_VERSION(1, 1) || m_caps.PixelShaderVersion < D3DPS_VERSION(1, 4))
		m_bUseSoftOrg = true;
	else
		m_bUseSoftOrg = false;


	m_pDev->GetDevice()->GetDeviceCaps(&m_caps);
	if (m_caps.VertexShaderVersion < D3DVS_VERSION(1, 1) || m_caps.PixelShaderVersion < D3DPS_VERSION(1, 4)) {
		m_bUseSoft = true;
	}
	else {
		m_bUseSoft = false;
	}


	_CEffectFile.InitDev(pDev);
	const char* effectName = "shader\\dx9\\eff.fx";
	if (!_CEffectFile.LoadEffectFromFile(effectName)) {
		MessageBox(NULL, "shader\\eff.fx", "ERROR", 0);
		return false;
	}

	if (!_bMagr)
		if (!LoadTotalVShader())
			return false;

	lstrcpy(_pszTexPath, "texture\\effect");

	lstrcpy(_pszMeshPath, "model");
	lstrcpy(_pszEFFectPath, "effect");

	LoadTotalTexture();

	LoadDefaultText("effect\\font.txt");

	LoadTotalData();

	_pMatView = pmat;
	D3DXMatrixInverse(&_MatBBoard, NULL, _pMatView);
	_MatBBoard._41 = 0.0f;
	_MatBBoard._42 = 0.0f;
	_MatBBoard._43 = 0.0f;

	_pMatViewProj = pMatviewproj;

	for (int n = 0; n < _iEffectNum; n++) {
		for (int m = 0; m < (int)_vecEffectList[n].size(); m++) {
			if (_vecEffectList[n][m].IsBillBoard()) {
				_vecEffectList[n][m].setBillBoardMatrix(&_MatBBoard);
			}
		}
	}

	lwRegisterOutputLoseDeviceProc(g_OnLostDevice);
	lwRegisterOutputResetDeviceProc(g_OnResetDevice);

	return true;
}

int CMPResManger::GetTextureID(const s_string& sName) {
	TEXTURE_MAP::iterator pos = _mapTexture.find(sName);
	if (pos != _mapTexture.end()) {
		return (*pos).second;
	}

	char szMsg[64];
	sprintf(szMsg, "[%s]()",
			sName.c_str());
	ToLogService("errors", LogLevel::Error, "{}", szMsg);
	return -1;
}

//-----------------------------------------------------------------------------
IDirect3DTextureX* CMPResManger::GetTextureByID(int iID) {
	if (_vecTexList[iID])
		return _vecTexList[iID]->GetTex();
	else {
		return NULL;
	}
}

//-----------------------------------------------------------------------------
lwITex* CMPResManger::GetTextureByIDlw(int iID) {
	if (_vecTexList[iID])
		return _vecTexList[iID];
	else {
		char t_pszFile[MAX_PATH];
#ifdef USE_DDS_FILE_EFFECT
		sprintf(t_pszFile, "%s\\%s.dds", _pszTexPath, _vecTexName[iID].c_str());
#else
		sprintf(t_pszFile, "%s\\%s.tga", _pszTexPath, _vecTexName[iID].c_str());
#endif
		lwITex* tex;

		if (LW_RESULT r = lwLoadTex(&tex, m_pSysGraphics->GetResourceMgr(), t_pszFile, 0, D3DFMT_A8R8G8B8);
			LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwLoadTex failed: id={}, file={}, ret={}",
						 __FUNCTION__, iID, t_pszFile, static_cast<long long>(r));
			return 0;
		}
		_vecTexList[iID] = tex;

		return _vecTexList[iID];
	}
}

//-----------------------------------------------------------------------------
lwITex* CMPResManger::GetTextureByNamelw(const s_string& sName) {
	int iTexID = GetTextureID(sName);
	if (iTexID == -1) {
		return 0;
	}

	return GetTextureByIDlw(iTexID);
}

//-----------------------------------------------------------------------------
int CMPResManger::GetMeshID(const s_string& sName) {
	MESH_MAP::iterator pos = _mapMesh.find(sName);
	if (pos != _mapMesh.end()) {
		return (*pos).second;
	}
	return -1;
}

//-----------------------------------------------------------------------------
CEffectModel* CMPResManger::GetMeshByID(int iID) {
	CEffectModel* pRetMesh(0);

	if (iID >= 7) {
		if (!_vecMeshList[iID]) {
			_vecMeshList[iID] = new CEffectModel;

			_vecMeshList[iID]->InitDevice(m_pDev);
			lwIPathInfo* path_info;
			m_pSys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);
			char szOldPath[260];
			strcpy(szOldPath, path_info->GetPath(PATH_TYPE_MODEL_ITEM));
			path_info->SetPath(PATH_TYPE_MODEL_ITEM, "model\\effect\\");
			if (!_vecMeshList[iID]->LoadModel((TCHAR*)_vecMeshName[iID].c_str())) {
				SAFE_DELETE(_vecMeshList[iID]);
				path_info->SetPath(PATH_TYPE_MODEL_ITEM, szOldPath);

				char szMsg[64];
				sprintf(szMsg, "[id=%d]", iID);
				ToLogService("errors", LogLevel::Error, "{}", szMsg);
				return 0;
			}
			if (!_vecMeshList[iID]->GetObject() || !_vecMeshList[iID]->GetObject()->GetPrimitive()) {
				ToLogService("errors", LogLevel::Error, ": effectmesh->GetObject(),effectmesh->GetPrimitive()__ID={}",
							 iID);
			}
			else
				_vecMeshList[iID]->GetObject()->GetPrimitive()->SetState(STATE_TRANSPARENT, 0);
			path_info->SetPath(PATH_TYPE_MODEL_ITEM, szOldPath);
			pRetMesh = _vecMeshList[iID];
		}
		else {
			if (_vecMeshList[iID]->IsUsing()) {
				int n = _iMeshNum;
				for (; n < MAXMESH_COUNT; ++n) {
					if (_vecMeshList[n] && _vecMeshList[n]->IsUsing()) {
						continue;
					}
					if (!_vecMeshList[n]) {
						_vecMeshList[n] = new CEffectModel;
					}

					if (_vecMeshList[n]->m_iID != iID) {
						if (!_vecMeshList[n]->Copy(*_vecMeshList[iID])) {
							SAFE_DELETE(_vecMeshList[n]);
							char szMsg[64];
							sprintf(szMsg, "[id=%d]", iID);
							ToLogService("errors", LogLevel::Error, "{}", szMsg);
							return 0;
						}
					}

					break;
				}
				if (n >= MAXMESH_COUNT) {
					ToLogService("errors", LogLevel::Error, "");
					return 0;
				}
				pRetMesh = _vecMeshList[n];
			}
			else {
				pRetMesh = _vecMeshList[iID];
			}
		}
	}
	else {
		pRetMesh = _vecMeshList[iID];
	}
	pRetMesh->m_iID = iID;
	pRetMesh->SetUsing(true);
	return pRetMesh;
}

//-----------------------------------------------------------------------------
CEffectModel* CMPResManger::GetMeshByName(const s_string& sName) {
	int iMeshID = GetMeshID(sName);
	if (iMeshID == -1) {
		return 0;
	}
	return GetMeshByID(iMeshID);
}

//-----------------------------------------------------------------------------
void CMPResManger::DeleteMesh(CEffectModel& rEffectModel) {
	if (rEffectModel.m_iID >= 7) {
		rEffectModel.SetUsing(false);
	}
}

int CMPResManger::GetEffectID(const s_string& pszName) {
	EFFECT_MAP::iterator pos = _mapEffect.find(pszName);
	if (pos != _mapEffect.end()) {
		return (*pos).second;
	}
	return -1;
}

std::vector<I_Effect>& CMPResManger::GetEffectByID(int iID) {
	I_Effect* pEffect = &(_vecEffectList[iID][0]);

	int n = (int)_vecEffectList[iID].size();
	if (n <= 0) {
		char t_pszFile[MAX_PATH];
		sprintf(t_pszFile, "%s\\%s", _pszEFFectPath, _vecEffectName[iID].c_str());

		if (!LoadEffectFromFile(iID, t_pszFile))
			return _vecEffectList[iID];
		_vecEffectList[iID][0].setEffectName(_vecEffectName[iID]);
	}
	return _vecEffectList[iID];
}

I_Effect* CMPResManger::GetSubEffectByID(int iID, int iSubIdx) {
	return &_vecEffectList[iID][iSubIdx];
}

IDirect3DVertexShaderX* CMPResManger::GetVShaderByID(int iID) {
	if (!m_bUseSoft)
		return _vecVShader[iID];
	else
		return _vecVShader[0];
}

IDirect3DVertexDeclarationX* CMPResManger::GetVDeclByID(int iID) {
	if (!m_bUseSoft)
		return _vecVDecl[iID];
	else
		return _vecVDecl[0];
}

IDirect3DVertexShaderX* CMPResManger::GetShadeVS() {
	return _dwShadeMapVS;
}

IDirect3DVertexDeclarationX* CMPResManger::GetShadeVDecl() {
	return _vecVDecl[4];
}

CEffectModel* CMPResManger::GetShadeMesh() {
	return _CShadeModel;
}

EffParameter* CMPResManger::GetEffectParamByID(int iID) {
	return &_vecEffectParam[iID];
}

IDirect3DVertexDeclarationX* CMPResManger::GetMinimapVDecl() {
	return _vecVDecl[5];
}

bool CMPResManger::LoadTotalTexture() {
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE t_hFind = NULL;

		lstrcpy(t_Path, _pszTexPath);
		lstrcat(t_Path, "\\*.tga");

		if ((t_hFind = FindFirstFile(t_Path, &t_sfd)) == INVALID_HANDLE_VALUE)
			return false;
		string sFileName;
		do {
			if (!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				sFileName = t_sfd.cFileName;
				transform(sFileName.begin(), sFileName.end(),
						  sFileName.begin(),
						  [](unsigned char c) {
							  return std::tolower(c);
						  });
				sFileName = sFileName.substr(0, sFileName.rfind('.'));

				{
					_mapTexture[sFileName] = (int)_vecTexName.size();
					_vecTexName.push_back(sFileName.c_str());
				}
			}
		}
		while (FindNextFile(t_hFind, &t_sfd));
		FindClose(t_hFind);
	}

	_iTexNum = (int)_vecTexName.size();
	_vecTexList.resize(_iTexNum);
	for (int iw = 0; iw < _iTexNum; iw++) {
		_vecTexList[iw] = NULL;
	}
	return true;
}

void CMPResManger::LoadTotalData() {
	if (!s_resourcePreload)
		return;

	char t_Path[MAX_PATH];
	WIN32_FIND_DATA t_sfd;
	HANDLE t_hFind = NULL;

	// Pre-warm bone-кэша для animation\*.lab.
	lstrcpy(t_Path, "animation\\");
	lstrcat(t_Path, "\\*.lab");

	int count = 0;
	if ((t_hFind = FindFirstFile(t_Path, &t_sfd)) == INVALID_HANDLE_VALUE)
		return;
	do {
		if (!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			int length = (int)strlen(t_sfd.cFileName);
			char* sname = &t_sfd.cFileName[length - 4];
			if (strcmp(sname, ".lab") != 0) {
				continue;
			}
			char path[LW_MAX_PATH];
			sprintf(path, "%s%s", "animation\\", t_sfd.cFileName);

			g_GeomManager.LoadBoneData(t_sfd.cFileName);
			count++;
			if (count == 50) {
				break;
			}
		}
	}
	while (FindNextFile(t_hFind, &t_sfd));
	FindClose(t_hFind);
}

void CMPResManger::LoadTotalRes() {
	LoadTotalPartCtrl();

	if (!s_resourcePreload)
		return;

	char t_Path[MAX_PATH];
	WIN32_FIND_DATA t_sfd;
	HANDLE t_hFind = NULL;

	// Pre-warm geom-кэша для character meshes (`model\character\*.lgo`).
	// LoadGeomobj хардкодит этот путь, и здесь итерация совпадает —
	// заполняем m_GeomobjMap, чтобы первое обращение из lwPhysique::LoadPrimitive
	// не ходило на диск.
	lstrcpy(t_Path, "model\\character");
	lstrcat(t_Path, "\\*.lgo");

	static int nNum = 0;
	if ((t_hFind = FindFirstFile(t_Path, &t_sfd)) == INVALID_HANDLE_VALUE)
		return;
	do {
		if (!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			int length = (int)strlen(t_sfd.cFileName);
			char* sname = &t_sfd.cFileName[length - 4];
			if (strcmp(sname, ".lgo") != 0) {
				continue;
			}

			nNum++;
			if (nNum++ >= 900)
				break;

			g_GeomManager.LoadGeomobj(t_sfd.cFileName);
		}
	}
	while (FindNextFile(t_hFind, &t_sfd));
	FindClose(t_hFind);
}

bool CMPResManger::LoadTotalMesh() {
	_iMeshNum = 7;

	_mapMesh[MESH_TRI] = (int)_vecMeshName.size();
	_vecMeshName.push_back(MESH_TRI);

	_mapMesh[MESH_RECT] = (int)_vecMeshName.size();
	_vecMeshName.push_back(MESH_RECT);

	_mapMesh[MESH_PLANERECT] = (int)_vecMeshName.size();
	_vecMeshName.push_back(MESH_PLANERECT);

	_mapMesh[MESH_PLANETRI] = (int)_vecMeshName.size();
	_vecMeshName.push_back(MESH_PLANETRI);

	_mapMesh[MESH_RECTZ] = (int)_vecMeshName.size();
	_vecMeshName.push_back(MESH_RECTZ);

	_mapMesh[MESH_CONE] = (int)_vecMeshName.size();
	_vecMeshName.push_back(MESH_CONE);

	_mapMesh[MESH_CYLINDER] = (int)_vecMeshName.size();
	_vecMeshName.push_back(MESH_CYLINDER);

	_vecMeshList.resize(MAXMESH_COUNT);
	for (int n = 0; n < MAXMESH_COUNT; n++) {
		_vecMeshList[n] = NULL;
	}
	_vecMeshList[0] = new CEffectModel;
	_vecMeshList[0]->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_vecMeshList[0]->CreateTriangle();

	_vecMeshList[1] = new CEffectModel;
	_vecMeshList[1]->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_vecMeshList[1]->CreateRect();

	_vecMeshList[2] = new CEffectModel;
	_vecMeshList[2]->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_vecMeshList[2]->CreatePlaneRect();

	_vecMeshList[3] = new CEffectModel;
	_vecMeshList[3]->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_vecMeshList[3]->CreatePlaneTriangle();

	_vecMeshList[4] = new CEffectModel;
	_vecMeshList[4]->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_vecMeshList[4]->CreateRectZ();

	_vecMeshList[5] = new CEffectModel;
	_vecMeshList[5]->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_vecMeshList[5]->CreateCone(8, 3, 2);

	_vecMeshList[6] = new CEffectModel;
	_vecMeshList[6]->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_vecMeshList[6]->CreateCylinder(8, 3, 1, 3);

	_CShadeModel = NULL;
	_CShadeModel = new CEffectModel;
	_CShadeModel->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_CShadeModel->CreateShadeModel();


	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE t_hFind = NULL;

		lstrcpy(t_Path, "model\\effect");
		lstrcat(t_Path, "\\*.lgo");

		if ((t_hFind = FindFirstFile(t_Path, &t_sfd)) == INVALID_HANDLE_VALUE)
			return true;
		string sFileName;

		lwIPathInfo* path_info;
		m_pSys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);
		char szOldPath[260];
		strcpy(szOldPath, path_info->GetPath(PATH_TYPE_MODEL_ITEM));
		path_info->SetPath(PATH_TYPE_MODEL_ITEM, "model\\effect\\");

		do {
			if (!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				int length = (int)strlen(t_sfd.cFileName);
				char* sname = &t_sfd.cFileName[length - 4];
				if (strcmp(sname, ".lgo") != 0) {
					continue;
				}
				sFileName = t_sfd.cFileName;
				transform(sFileName.begin(), sFileName.end(),
						  sFileName.begin(),
						  [](unsigned char c) {
							  return std::tolower(c);
						  });

				_vecMeshList[_iMeshNum] = new CEffectModel;
				_vecMeshList[_iMeshNum]->InitDevice(m_pDev);
				_vecMeshList[_iMeshNum]->LoadModel(sFileName.c_str());

				_mapMesh[sFileName] = (int)_vecMeshName.size();
				_vecMeshName.push_back(sFileName.c_str());
				_iMeshNum++;
			}
		}
		while (FindNextFile(t_hFind, &t_sfd));
		FindClose(t_hFind);
		path_info->SetPath(PATH_TYPE_MODEL_ITEM, szOldPath);
	}

	return true;
}

I_Effect* CMPResManger::AddEffectToMgr(const s_string& strName) {
	_iEffectNum++;

	_vecEffectList.resize(_iEffectNum);

	_vecEffectName.resize(_iEffectNum);

	_vecEffectParam.resize(_iEffectNum);

	_mapEffect[strName] = _iEffectNum - 1;
	_vecEffectName[_iEffectNum - 1] = strName;

	_vecEffectList[_iEffectNum - 1].resize(1);

	I_Effect* pEffect = &(_vecEffectList[_iEffectNum - 1][0]);
	_vecEffectList[_iEffectNum - 1][0].ReleaseAll();

	return &_vecEffectList[_iEffectNum - 1][0];
}

void CMPResManger::AddUniteEffectToMgr(std::vector<I_Effect>& vecEffArray) {
	_iEffectNum++;
	_vecEffectList.resize(_iEffectNum);
	_vecEffectName.resize(_iEffectNum);
	_mapEffect[vecEffArray[0].getEffectName()] = _iEffectNum - 1;
	_vecEffectName[_iEffectNum - 1] = vecEffArray[0].getEffectName();
	_vecEffectList[_iEffectNum - 1] = vecEffArray;
	for (INT n = 0; n < (INT)vecEffArray.size(); n++) {
		_vecEffectList[_iEffectNum - 1][n].BoundingRes(this);
	}
}


bool CMPResManger::LoadEffectFromFile(int idx, char* pszFileName) {
	FILE* t_pFile;
	t_pFile = fopen(pszFileName, "rb");
	if (!t_pFile)
		return false;
	DWORD t_dwVersion;
	int t_temp;
	fread(&t_dwVersion, sizeof(t_dwVersion), 1, t_pFile);

	fread(&t_temp, sizeof(int), 1, t_pFile);
	_vecEffectParam[idx].m_iIdxTech = t_temp;
	char t_pszName[32];

	fread(&_vecEffectParam[idx].m_bUsePath, sizeof(bool), 1, t_pFile);
	fread(t_pszName, sizeof(char), 32, t_pFile);
	_vecEffectParam[idx].m_szPathName = t_pszName;

	fread(&_vecEffectParam[idx].m_bUseSound, sizeof(bool), 1, t_pFile);
	fread(t_pszName, sizeof(char), 32, t_pFile);
	_vecEffectParam[idx].m_szSoundName = t_pszName;

	fread(&_vecEffectParam[idx].m_bRotating, sizeof(bool), 1, t_pFile);
	fread(&_vecEffectParam[idx].m_SVerRota, sizeof(D3DXVECTOR3), 1, t_pFile);
	fread(&_vecEffectParam[idx].m_fRotaVel, sizeof(float), 1, t_pFile);

	fread(&t_temp, sizeof(int), 1, t_pFile);

	_vecEffectList[idx].resize(t_temp);

	for (int n = 0; n < t_temp; n++) {
		_vecEffectList[idx][n].LoadFromFile(t_pFile, t_dwVersion);
		_vecEffectList[idx][n].Reset();
		_vecEffectList[idx][n].m_pDev = m_pDev;
	}

	fclose(t_pFile);
	return true;
}

bool CMPResManger::LoadTotalEffect() {
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE t_hFind = NULL;

		lstrcpy(t_Path, _pszEFFectPath);
		lstrcat(t_Path, "\\*.eff");

		char t_pszFile[MAX_PATH];

		if ((t_hFind = FindFirstFile(t_Path, &t_sfd)) == INVALID_HANDLE_VALUE)
			return false;
		do {
			if (!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				int length = (int)strlen(t_sfd.cFileName);
				char* sname = &t_sfd.cFileName[length - 4];
				if (stricmp(sname, ".eff") != 0) {
					continue;
				}
				string sFileName;
				sFileName = t_sfd.cFileName;
				transform(sFileName.begin(), sFileName.end(),
						  sFileName.begin(),
						  [](unsigned char c) {
							  return std::tolower(c);
						  });

				sprintf(t_pszFile, "%s\\%s", _pszEFFectPath, t_sfd.cFileName);
				_vecEffectList.resize(_iEffectNum + 1);
				_vecEffectList[_iEffectNum].clear();
				_vecEffectParam.resize(_iEffectNum + 1);

				if (!LoadEffectFromFile(_iEffectNum, t_pszFile)) {
					char szData[1024];
					sprintf(szData, "(%s)", t_pszFile);
					MessageBox(NULL, szData, "Error", MB_OK);
				}

				_vecEffectName.resize(_iEffectNum + 1);

				_mapEffect[t_sfd.cFileName] = _iEffectNum;

				_vecEffectName[_iEffectNum] = t_sfd.cFileName;

				_vecEffectList[_iEffectNum][0].setEffectName(_vecEffectName[_iEffectNum]);

				_iEffectNum++;
			}
		}
		while (FindNextFile(t_hFind, &t_sfd));
		FindClose(t_hFind);
	}

	return true;
}

bool CMPResManger::LoadTotalVShader() {
	char t_Path[MAX_PATH];

	LPD3DXBUFFER pCode;
	if (!m_bUseSoft) {
		sprintf(t_Path, "shader\\dx9\\eff1.vsh");
		while (SUCCEEDED(D3DXAssembleShaderFromFile( t_Path, NULL, NULL, 0, &pCode, NULL ))) {
			_iVShaderNum++;
			_vecVShader.resize(_iVShaderNum);
			if (HRESULT hr = m_pDev->GetDevice()->CreateVertexShader(
				(DWORD*)pCode->GetBufferPointer(),
				(IDirect3DVertexShaderX**)&_vecVShader[_iVShaderNum - 1]); FAILED(hr)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateVertexShader failed (eff loop): path={}, idx={}, hr=0x{:08X}",
							 __FUNCTION__, t_Path, _iVShaderNum - 1, static_cast<std::uint32_t>(hr));
				MessageBox(NULL, t_Path, "ERROR", 0);
				return false;
			}

			sprintf(t_Path, "shader\\dx9\\eff%d.vsh", _iVShaderNum + 1);
			pCode->Release();
			pCode = NULL;
		}

		sprintf(t_Path, "shader\\dx9\\shadeeff.vsh");
		if (SUCCEEDED(D3DXAssembleShaderFromFile( t_Path, NULL, NULL, 0, &pCode, NULL ))) {
			if (HRESULT hr = m_pDev->GetDevice()->CreateVertexShader(
				(DWORD*)pCode->GetBufferPointer(),
				(IDirect3DVertexShaderX**)&_dwShadeMapVS); FAILED(hr)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateVertexShader failed (shadeeff): hr=0x{:08X}",
							 __FUNCTION__, static_cast<std::uint32_t>(hr));
				MessageBox(NULL, "shader\\shadeeff.vsh", "ERROR", 0);
				return false;
			}
			pCode->Release();
			pCode = NULL;
		}
		else {
			MessageBox(NULL, "shader\\shadeeff.vsh", "ERROR", 0);
			return false;
		}
	}
	else {
		sprintf(t_Path, "shader\\dx9\\eff2.vsh");
		if (SUCCEEDED(D3DXAssembleShaderFromFile( t_Path, NULL, NULL, 0, &pCode, NULL ))) {
			_iVShaderNum++;
			_vecVShader.resize(_iVShaderNum);
			if (HRESULT hr = m_pDev->GetDevice()->CreateVertexShader(
				(DWORD*)pCode->GetBufferPointer(),
				(IDirect3DVertexShaderX**)&_vecVShader[_iVShaderNum - 1]); FAILED(hr)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateVertexShader failed (eff2): idx={}, hr=0x{:08X}",
							 __FUNCTION__, _iVShaderNum - 1, static_cast<std::uint32_t>(hr));
				_vecVShader[0] = 0L;
			}

			pCode->Release();
			pCode = NULL;
		}
	}


	return true;
}

bool CMPResManger::LoadTotalVShader(lwISysGraphics* sys_graphics) {
	lwISystem* sys = sys_graphics->GetSystem();

	char path[LW_MAX_PATH];
	lwIPathInfo* path_info = 0;
	sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);

	lwIResourceMgr* res_mgr;
	lwIShaderMgr* shader_mgr;

	sys_graphics->GetInterface((LW_VOID**)&res_mgr, LW_GUID_RESOURCEMGR);
	shader_mgr = res_mgr->GetShaderMgr();


	DWORD shader_type[] =
	{
		VSTU_EFFECT_E1,
		VSTU_EFFECT_E2,
		VSTU_EFFECT_E3,
		VSTU_EFFECT_E4,
		VSTU_SHADE_E6,
		VSTU_MINIMAP_E6,
	};


	// decl
	D3DVERTEXELEMENT9 ve_dec_effect1[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0}, // Include this line
		{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};

	D3DVERTEXELEMENT9 ve_dec_effect2[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};

	D3DVERTEXELEMENT9 ve_dec_shade[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};

	D3DVERTEXELEMENT9 ve_dec_map[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};


	const char* shader_file[] =
	{
		"eff1.hlsl",
		"eff2.hlsl",
		"eff3.hlsl",
		"eff4.hlsl",
		"shadeeff.hlsl",
		"minimap.hlsl",
	};
	// Все шейдеры HLSL после полного рефакторинга (vs_2_0).
	const DWORD shader_compile_flag[] =
	{
		VS_FILE_HLSL, VS_FILE_HLSL, VS_FILE_HLSL,
		VS_FILE_HLSL, VS_FILE_HLSL, VS_FILE_HLSL,
	};

	D3DVERTEXELEMENT9* decl_tab[] =
	{
		ve_dec_effect1,
		ve_dec_effect2,
		ve_dec_effect1,
		ve_dec_effect1,
		ve_dec_shade,
		ve_dec_map,
	};

	DWORD decl_type[] =
	{
		VDT_EFF_134,
		VDT_EFF_2,
		VDT_EFF_134,
		VDT_EFF_134,
		VDT_EFF_SHADE,
		VDT_EFF_MINIMAP,
	};

	int decl_num = sizeof(decl_type) / sizeof(decl_type[0]);
	_vecVDecl.resize(decl_num);

	for (int i = 0; i < decl_num; i++) {
		IDirect3DVertexDeclarationX* this_decl;
		if (LW_SUCCEEDED(shader_mgr->QueryVertexDeclaration(&this_decl, decl_type[i]))) {
			_vecVDecl[i] = this_decl;
			continue;
		}

		if (LW_RESULT r = shader_mgr->RegisterVertexDeclaration(decl_type[i], decl_tab[i]); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] shader_mgr->RegisterVertexDeclaration failed: i={}, decl_type={}, ret={}",
						 __FUNCTION__, i, decl_type[i], static_cast<long long>(r));
			return false;
		}
		shader_mgr->QueryVertexDeclaration(&this_decl, decl_type[i]);
		_vecVDecl[i] = this_decl;
	}

	for (int i = 0; i < decl_num; i++) {
		sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_SHADER), shader_file[i]);
		if (LW_RESULT r = shader_mgr->RegisterVertexShader(shader_type[i], path, shader_compile_flag[i]);
			LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] shader_mgr->RegisterVertexShader failed: i={}, shader_type={}, path={}, ret={}",
						 __FUNCTION__, i, shader_type[i], path, static_cast<long long>(r));
			return false;
		}

		if (i < 4) {
			_iVShaderNum++;
			_vecVShader.resize(_iVShaderNum);
			_vecVShader[_iVShaderNum - 1] = 0;

			shader_mgr->QueryVertexShader(&_vecVShader[_iVShaderNum - 1], shader_type[i]);
		}
		else if (i == 4) {
			shader_mgr->QueryVertexShader(&_dwShadeMapVS, shader_type[i]);
		}
		else if (i == 5) {
			shader_mgr->QueryVertexShader(&_dwMinimapVS, shader_type[i]);
		}
	}


	_bMagr = true;
	return true;
}

int CMPResManger::GetEffPathID(const s_string& pszName) {
	for (size_t i(0); i < _vecPathName.size(); ++i) {
		if (stricmp(_vecPathName[i].c_str(), pszName.c_str()) == 0)
			return (int)i;
	}
	return -1;
}

CEffPath* CMPResManger::GetEffPath(int iID) {
	return &_vecPath[iID];
}

bool CMPResManger::LoadTotalPath() {
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE t_hFind = NULL;

		lstrcpy(t_Path, _pszEFFectPath);
		lstrcat(t_Path, "\\*.csf");

		char t_pszFile[MAX_PATH];

		if ((t_hFind = FindFirstFile(t_Path, &t_sfd)) == INVALID_HANDLE_VALUE)
			return false;
		do {
			if (!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				sprintf(t_pszFile, "%s\\%s", _pszEFFectPath, t_sfd.cFileName);
				_vecPathName.push_back(t_sfd.cFileName);

				_iPathNum++;
				_vecPath.resize(_iPathNum);
				_vecPath[_iPathNum - 1].LoadPathFromFile(t_pszFile);
			}
		}
		while (FindNextFile(t_hFind, &t_sfd));
		FindClose(t_hFind);
	}

	return true;
}

bool CMPResManger::LoadDefaultText(const char* pszFileName) {
	FILE* pfile = fopen(pszFileName, "rt");

	if (!pfile) {
		MessageBox(NULL, _psDefault, "ERROR", 0);
		return false;
	}
	int idx = 0;
	int rval = fgetc(pfile);
	while (rval != EOF) {
		_psDefault[idx] = (char)rval;

		rval = fgetc(pfile);

		idx++;
		if (idx >= 418)
			break;
	}
	fclose(pfile);
	return true;
}

int CMPResManger::GetPartCtrlID(const s_string& pszName) {
	for (size_t n(0); n < _vecPartName.size(); n++) {
		if (stricmp(_vecPartName[n].c_str(), pszName.c_str()) == 0) {
			return (int)n;
		}
	}
	return -1;
}

CMPPartCtrl* CMPResManger::GetPartCtrlByID(int iID) {
	if (iID > MAXPART_COUNT) {
		ToLogService("errors", LogLevel::Error, "lemon");
		return NULL;
	}
	if (iID < 0) {
		ToLogService("errors", LogLevel::Error, "ID[{}]", iID);
		return NULL;
	}
	if ((*_vecPartCtrl[iID]) == NULL) {
		char t_Path[MAX_PATH];
		sprintf(t_Path, "%s\\%s", _pszEFFectPath, _vecPartName[iID].c_str());

		(*_vecPartCtrl[iID]) = new CMPPartCtrl;
		if (!(*_vecPartCtrl[iID])->LoadFromFile(t_Path)) {
			ToLogService("errors", LogLevel::Error, "Load {} error", _vecPartName[iID].c_str());
			return NULL;
		}
		else {
			const auto v = D3DXVECTOR3(0, 0, 0);
			(*_vecPartCtrl[iID])->MoveTo(&v);
		}
	}
	return (*_vecPartCtrl[iID]);
}

void CMPResManger::LoadTotalPartCtrl() {
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE t_hFind = NULL;

		char t_FilePath[MAX_PATH];

		lstrcpy(t_Path, _pszEFFectPath);
		lstrcat(t_Path, "\\*.par");

		_vecPartCtrl.resize(MAXPART_COUNT);

		if ((t_hFind = FindFirstFile(t_Path, &t_sfd)) == INVALID_HANDLE_VALUE)
			return;
		string sFileName;
		do {
			if (!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				_iPartCtrlNum++;

				sFileName = t_sfd.cFileName;
				transform(sFileName.begin(), sFileName.end(),
						  sFileName.begin(),
						  [](unsigned char c) {
							  return std::tolower(c);
						  });
				_vecPartName.push_back(sFileName.c_str());

				_vecPartCtrl.setsize(_iPartCtrlNum);
				sprintf(t_FilePath, "%s\\%s", _pszEFFectPath, _vecPartName[_iPartCtrlNum - 1].c_str());

				(*_vecPartCtrl[_iPartCtrlNum - 1]) = new CMPPartCtrl;
				if (!(*_vecPartCtrl[_iPartCtrlNum - 1])->LoadFromFile(t_FilePath)) {
					SAFE_DELETE((*_vecPartCtrl[_iPartCtrlNum - 1]));
					ToLogService("errors", LogLevel::Error, "Load {} error", sFileName.c_str());
				}
			}
		}
		while (FindNextFile(t_hFind, &t_sfd));
		FindClose(t_hFind);
	}
}

CMPPartCtrl* CMPResManger::NewPartCtrl(const s_string& strName) {
	_iPartCtrlNum++;
	if (_iPartCtrlNum >= MAXPART_COUNT) {
		_iPartCtrlNum--;
		return NULL;
	}
	_vecPartCtrl.setsize(_iPartCtrlNum);
	_vecPartName.push_back(strName);

	(*_vecPartCtrl[_iPartCtrlNum - 1]) = new CMPPartCtrl;

	return (*_vecPartCtrl[_iPartCtrlNum - 1]);
}

void CMPResManger::DeletePartCtrl(int iID) {
}

CEffectModel* CMPResManger::NewTobMesh() {
	CEffectModel* pModel = new CEffectModel;
	pModel->InitDevice(m_pDev, m_pSysGraphics->GetResourceMgr());
	_lstTobMeshs.push_back(pModel);
	_iTobMeshNum++;
	return pModel;
}

bool CMPResManger::DeleteTobMesh(CEffectModel& rEffectModel) {
	std::list<CEffectModel*>::iterator iter = find(_lstTobMeshs.begin(), _lstTobMeshs.end(), &rEffectModel);
	if (iter != _lstTobMeshs.end()) {
		delete &rEffectModel;
		_lstTobMeshs.erase(iter);
		return true;
	}
	return false;
}

BOOL CMPResManger::OnResetDevice() {
	if (!_CEffectFile.OnResetDevice())
		return FALSE;
	DWORD shader_type[] =
	{
		VSTU_EFFECT_E1,
		VSTU_EFFECT_E2,
		VSTU_EFFECT_E3,
		VSTU_EFFECT_E4,
		VSTU_SHADE_E6,
		VSTU_MINIMAP_E6,
	};

	DWORD decl_type[] =
	{
		VDT_EFF_134,
		VDT_EFF_2,
		VDT_EFF_134,
		VDT_EFF_134,
		VDT_EFF_SHADE,
		VDT_EFF_MINIMAP,
	};
	lwIShaderMgr* shader_mgr;
	lwIResourceMgr* res_mgr;

	m_pSysGraphics->GetInterface((LW_VOID**)&res_mgr, LW_GUID_RESOURCEMGR);
	shader_mgr = res_mgr->GetShaderMgr();

	if (m_bUseSoft) {
		shader_mgr->QueryVertexShader(&_vecVShader[0], shader_type[1]);
	}
	else {
		constexpr int total = sizeof(shader_type) / sizeof(shader_type[0]);
		for (int i = 0; i < total; i++) {
			if (i < 4) {
				shader_mgr->QueryVertexShader(&_vecVShader[i], shader_type[i]);
			}
			else if (i == 4) {
				shader_mgr->QueryVertexShader(&_dwShadeMapVS, shader_type[i]);
			}
			else if (i == 5) {
				shader_mgr->QueryVertexShader(&_dwMinimapVS, shader_type[i]);
			}
			shader_mgr->QueryVertexDeclaration(&_vecVDecl[i], decl_type[i]);
		}
	}

	IDirect3DSurfaceX* pBackBuffer;
	m_pDev->GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	pBackBuffer->GetDesc(&m_d3dBackBuffer);
	pBackBuffer->Release();

	D3DXMatrixOrthoLH(&_Mat2dViewProj, float(m_d3dBackBuffer.Width), float(m_d3dBackBuffer.Height), 0.0f, 1.0f);

	_iFontBkWidth = m_d3dBackBuffer.Width / 2;
	_iFontBkHeight = m_d3dBackBuffer.Height / 2;


	_vecMeshList[0]->CreateTriangle();
	_vecMeshList[1]->CreateRect();
	_vecMeshList[2]->CreatePlaneRect();
	_vecMeshList[3]->CreatePlaneTriangle();
	_vecMeshList[4]->CreateRectZ();
	_vecMeshList[5]->CreateCone(8, 3, 2);
	_vecMeshList[6]->CreateCylinder(8, 3, 1, 3);


	return TRUE;
}

BOOL CMPResManger::OnLostDevice() {
	if (!_CEffectFile.OnLostDevice())
		return FALSE;

	return TRUE;
}

LW_RESULT g_OnLostDevice() {
	return ResMgr.OnLostDevice();
}

LW_RESULT g_OnResetDevice() {
	return ResMgr.OnResetDevice();
}


void CMPResManger::RestoreEffect() {
	m_pDev->SetRenderStateForced(D3DRS_ZENABLE, TRUE);
	m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, TRUE);
	m_pDev->SetRenderStateForced(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetRenderStateForced(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDev->SetRenderStateForced(D3DRS_DITHERENABLE,FALSE);
	m_pDev->SetRenderStateForced(D3DRS_CULLMODE, D3DCULL_CCW); // ???????
	m_pDev->SetRenderStateForced(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pDev->SetRenderStateForced(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	m_pDev->SetRenderStateForced(D3DRS_LIGHTING, TRUE);
	m_pDev->SetRenderStateForced(D3DRS_CLIPPING, TRUE);


	m_pDev->GetInterfaceMgr()->dev_obj->SetTextureForced(0, 0);
	m_pDev->GetInterfaceMgr()->dev_obj->SetTextureForced(1, 0);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pDev->SetSamplerStateForced(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerStateForced(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pDev->SetSamplerStateForced(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerStateForced(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetTextureStageStateForced(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_pDev->SetTextureStageStateForced(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageStateForced(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	m_pDev->SetTextureStageStateForced(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pDev->SetTextureStageStateForced(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageStateForced(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

//-----------------------------------------------------------------------------
void CMPResManger::FrameMove(DWORD dwTime) {
	m_iCurFrame += 1;
	if (m_iCurFrame > 1)
		return;


	_fCurTime = (float)GetTickCount() / 1000;
	if (!_bInitTime) {
		_fSaveTime = _fCurTime;
		_bInitTime = true;
	}
	_fDailTime = _fCurTime - _fSaveTime;
	_fSaveTime = _fCurTime;

	D3DXMatrixInverse(&_MatBBoard, NULL, _pMatView);
	_MatBBoard._41 = 0.0f;
	_MatBBoard._42 = 0.0f;
	_MatBBoard._43 = 0.0f;

	D3DXMatrixTranspose(&_MatViewProjPose, _pMatViewProj);
	if (_vecValidID.size() >= MAXMSG_COUNT)
		return;

	for (WORD iw = 0; iw < MAXMSG_COUNT; ++iw) {
		if (_vecPartArray[iw]) {
			_vecPartArray[iw]->FrameMove(dwTime);
		}
	}
}

//-----------------------------------------------------------------------------
void CMPResManger::Render() {
	if (m_iCurFrame < 1)
		return;
	m_iCurFrame = 0;
	if (_vecValidID.size() >= MAXMSG_COUNT)
		return;

	for (WORD iw = 0; iw < MAXMSG_COUNT; ++iw) {
		if (_vecPartArray[iw]) {
			if (!_vecPartArray[iw]->IsPlaying()) {
				SAFE_DELETE(_vecPartArray[iw]);
				_vecValidID.push_front(iw);
				continue;
			}
			_vecPartArray[iw]->Render();
		}
	}
}

void CMPResManger::Clear() {
	for (int i = 0; i < (int)_vecPartArray.size(); ++i) {
		CMPPartCtrl* part = _vecPartArray[i];
		if (part) {
			part->Reset();
		}
	}
}

void CMPResManger::UpdateMatrix() {
	D3DXMatrixInverse(&_MatBBoard, NULL, _pMatView);
	_MatBBoard._41 = 0.0f;
	_MatBBoard._42 = 0.0f;
	_MatBBoard._43 = 0.0f;

	D3DXMatrixTranspose(&_MatViewProjPose, _pMatViewProj);
}

void CMPResManger::BeginEffect(int iIdx) {
	_CEffectFile.SetTechnique(iIdx);
	_CEffectFile.Begin(D3DXFX_DONOTSAVESTATE);
	_CEffectFile.Pass(0);
}

void CMPResManger::EndEffect() {
	_CEffectFile.End();
}

void CMPResManger::SendResMessage(const s_string& strPartName, D3DXVECTOR3 vPos, MPMap* pMap) {
	int id = GetPartCtrlID(strPartName);
	if (id < 0) {
		return;
	}
	if (_vecValidID.empty()) {
		return;
	}

	CMPPartCtrl* tctrl = GetPartCtrlByID(id);
	if (!tctrl) {
		return;
	}
	WORD idx = *_vecValidID.front();

	_vecPartArray[idx] = new CMPPartCtrl;
	_vecPartArray[idx]->CopyPartCtrl(tctrl);
	_vecPartArray[idx]->BindingRes(this);

	_vecPartArray[idx]->Reset();
	_vecPartArray[idx]->MoveTo(&vPos, pMap);
	_vecPartArray[idx]->Play(1);

	_vecValidID.pop_front();
}
