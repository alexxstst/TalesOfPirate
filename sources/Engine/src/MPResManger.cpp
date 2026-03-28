#include "StdAfx.h"
//#include "../../../proj/EffectEditer.h"
//#include <mindpower.h>
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

MINDPOWER_API CMPResManger        ResMgr;

CMPResManger::CMPResManger(void)
{
	m_pDev = NULL;
	ZeroMemory(_pszTexPath,MAX_PATH);
	ZeroMemory(_pszMeshPath,MAX_PATH);
	ZeroMemory(_pszEFFectPath,MAX_PATH);

	_iTexNum				= 0;
	_iMeshNum				= 0;
	_iEffectNum				= 0;
	_iVShaderNum			= 0;

	_vecTexName.clear();
	_vecMeshName.clear();
	_mapMesh.clear();

	_vecTexList.clear();
	_vecMeshList.clear();
	_mapTexture.clear();

	//_vecEffTech.clear();
	_vecEffectList.clear();

	_vecVShader.clear();
	_dwShadeMapVS = 0L;
	_dwFontVS	 = 0L;
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


	ZeroMemory(_psDefault,256);

	WORD iw;
//#ifndef USE_GAME
	_vecMeshList.resize(MAXMESH_COUNT);
	_vecPartCtrl.resize(MAXPART_COUNT);
	_vecPartCtrl.setsize(MAXPART_COUNT);

	for( iw = 0; iw < MAXPART_COUNT; iw++)
	{
		(*_vecPartCtrl[iw]) = NULL;
	}
//#else
//	_vecPartCtrl.clear();
//#endif


	_iPartCtrlNum = 0;

	_vecPartArray.clear();
	_vecPartArray.resize(MAXMSG_COUNT);
	_vecValidID.resize(MAXMSG_COUNT);
	_vecValidID.setsize(MAXMSG_COUNT);
	for(iw = 0; iw <  MAXMSG_COUNT; iw++)
	{
		_vecPartArray[iw] = NULL;
		*_vecValidID[iw] = iw;
	}


	m_bUseSoft = FALSE;

    m_pSys = 0;
    m_pSysGraphics = 0;

	m_bCanFrame = false;
	m_iCurFrame = 0;
}

CMPResManger::~CMPResManger(void)
{
	//ReleaseTotalRes();
	for (size_t i(0); i<_vecMeshList.size(); i++)
	{
		SAFE_DELETE(_vecMeshList[i]);
	}
#if RESOURCE_SCRIPT == 1
	std::set<string>::iterator iter, end;
	ofstream outfile;

	outfile.open("ParticleSet.txt", ios_base::app);
	end = _mapParticle.end();
	iter= _mapParticle.begin();
	for(; iter!=end; ++iter)
	{
		outfile << *iter << "\t0" << endl;
	}
	outfile.close();

	outfile.open("PathSet.txt", ios_base::app);
	end = _mapPath.end();
	iter = _mapPath.begin();
	for(; iter!=end; ++iter)
	{
		outfile << *iter << "\t1" << endl;
	}
	outfile.close();

	outfile.open("EffectSet.txt", ios_base::app);
	end = _mapEffect.end();
	iter = _mapEffect.begin();
	for(; iter!=end; ++iter)
	{
		outfile << *iter << "\t2" << endl;
	}
	outfile.close();

	outfile.open("MeshSet.txt", ios_base::app);
	end = _mapMesh.end();
	iter = _mapMesh.begin();
	for(; iter!=end; ++iter)
	{
		outfile << *iter << "\t3" << endl;
	}
	outfile.close();

	outfile.open("TextureSet.txt", ios_base::app);
	end = _mapTexture.end();
	iter = _mapTexture.begin();
	for(; iter!=end; ++iter)
	{
		outfile << *iter << "\t3" << endl;
	}
	outfile.close();

	
#endif
}

void	CMPResManger::ReleaseTotalRes()
{
#ifndef SCRIPT_TABLE
	CEffectScript::m_cEffScript.ReleaseScript();
#else
	//CEff_ParamSet* pSingleSet = CEff_ParamSet::I();
	//SAFE_DELETE( pSingleSet );
	//CGroup_ParamSet* pGroupSet = CGroup_ParamSet::I();
	//SAFE_DELETE( pGroupSet );
#endif

	int iw;

//#ifndef USE_GAME
	for(iw = 0; iw < _iPartCtrlNum; iw++)
	{
        CMPPartCtrl** C = _vecPartCtrl[iw];
		SAFE_DELETE(*C);
	}
	_vecPartCtrl.resize(0);
//#else
//	_vecPartCtrl.clear();
//#endif
	_iPartCtrlNum = 0;

	if(_vecPartArray.size() > 0)
	{
		for(iw = 0; iw < MAXMSG_COUNT; iw++)
		{
			SAFE_DELETE(_vecPartArray[iw]);
		}
		_vecValidID.resize(0);
		_vecPartArray.clear();
	}
	_vecEffectList.clear();

	_CEffectFile.free();

	_dwShadeMapVS	= 0L;
	_dwFontVS		= 0L;
	_dwMinimapVS = 0L;

	_vecVShader.clear();
	_iVShaderNum = 0;

	_iPathNum = 0;
	_vecPathName.clear();
	_vecPath.clear();


	for(iw = 0; iw < _iTexNum; iw++)
	{
		SAFE_RELEASE(_vecTexList[iw]);
	}

	for( iw = 0; iw < _iMeshNum; iw++)
	{
		SAFE_DELETE(_vecMeshList[iw]);
	}
	std::list<CEffectModel*>::iterator iter = _lstTobMeshs.begin();
	std::list<CEffectModel*>::iterator end  = _lstTobMeshs.end();
	for(; iter != end; ++iter)
	{
		SAFE_DELETE(*iter);
	}
	SAFE_DELETE(_CShadeModel);

	_iTexNum				= 0;
	_iMeshNum				= 0;
	_iEffectNum				= 0;
	_iTobMeshNum			= 0;

	_vecTexName.clear();
	_vecMeshName.clear();
	_vecEffectName.clear();

	_mapMesh.clear();
	_mapEffect.clear();
	_mapTexture.clear();

	_vecTexList.clear();
	_vecMeshList.clear();
	_lstTobMeshs.clear();

	//_vecEffTech.clear();

	_vecEffectParam.clear();
}

bool CMPResManger::InitRes2()
{
	if(!LoadTotalMesh())
		return false;

	if(!LoadTotalEffect())
		return false;

	//LoadTotalData();

	LoadTotalPath();

	return true;
}

bool CMPResManger::InitRes3()
{
	LoadTotalRes();
	return true;
}

#ifdef USE_RENDER
bool	CMPResManger::InitRes(MPRender*		pDev, D3DXMATRIX* pmat, D3DXMATRIX* pMatviewproj)
#else
bool	CMPResManger::InitRes(LPDIRECT3DDEVICE8		pDev, D3DXMATRIX* pmat, D3DXMATRIX* pMatviewproj)
#endif
{
	m_pDev = pDev;

	IDirect3DSurfaceX* pBackBuffer;
#ifdef USE_RENDER
	m_pDev->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
#else
	m_pDev->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
#endif
    pBackBuffer->GetDesc( &m_d3dBackBuffer );
    pBackBuffer->Release();

	//_iFontBkWidth = m_d3dBackBuffer.Width/2;
	//_iFontBkHeight= m_d3dBackBuffer.Height/2;
	//_iFontBkWidth = m_pDev->GetScrWidth()/2; //m_d3dBackBuffer.Width/2;
	//_iFontBkHeight= m_pDev->GetScrHeight()/2; //m_d3dBackBuffer.Height/2;
	//_iFontBkWidth = m_pDev->GetScrWidth()/2;//m_d3dBackBuffer.Width/2;
	//_iFontBkHeight= m_pDev->GetScrHeight()/2;//m_d3dBackBuffer.Height/2;
	RECT rc_client;
	m_pDev->GetInterfaceMgr()->dev_obj->GetWindowRect(NULL, &rc_client);
	_iFontBkWidth = (rc_client.right - rc_client.left) / 2;
	_iFontBkHeight= (rc_client.bottom - rc_client.top) / 2;


	D3DXMatrixOrthoLH(&_Mat2dViewProj, float(m_d3dBackBuffer.Width), float(m_d3dBackBuffer.Height), 0.0f, 1.0f);
	
	m_caps = m_pDev->GetOrgCap();
	if(  m_caps.VertexShaderVersion < D3DVS_VERSION(1,1) || m_caps.PixelShaderVersion < D3DPS_VERSION(1,4) )
		m_bUseSoftOrg = true;
	else
		m_bUseSoftOrg = false;


#ifdef USE_RENDER
	m_pDev->GetDevice()->GetDeviceCaps(&m_caps);
#else
	m_pDev->GetDeviceCaps(&m_caps);       // initialize m_pd3dDevice before using
#endif
	if(  m_caps.VertexShaderVersion < D3DVS_VERSION(1,1) || m_caps.PixelShaderVersion < D3DPS_VERSION(1,4))
	{	
		m_bUseSoft = true;
		//MessageBox(NULL,"soft","ERROR",0);
	}
	else
	{
		//MessageBox(NULL,"shard","ERROR",0);

		m_bUseSoft = false;
	}


	if(!CScriptFile::m_ctScript.OpenFileRead("effect/model.txt"))
	{
		ToLogService("errors", LogLevel::Error, "ȱ�� effect/model.txt");
		return false;
	}

	_CEffectFile.InitDev(pDev);
	#if (defined LW_USE_DX9)
	const char* effectName = "shader\\dx9\\eff.fx";
#else
	const char* effectName = "shader\\dx8\\eff.fx";
#endif
	if(!_CEffectFile.LoadEffectFromFile(effectName))
	{
		MessageBox(NULL,"shader\\eff.fx","ERROR",0);
		return false;
	}

#ifndef SCRIPT_TABLE
	if(!CEffectScript::m_cEffScript.InitScript())
		return false;
#else
	bool	bBinary = false;
	CEff_ParamSet* pEffSet = new CEff_ParamSet(0, 100);
	pEffSet->LoadRawDataInfo("scripts/table/MagicSingleinfo", bBinary);

	CGroup_ParamSet* pGroupSet = new CGroup_ParamSet(0, 10);
	pGroupSet->LoadRawDataInfo("scripts/table/MagicGroupinfo", bBinary);
#endif

	if(!_bMagr)
		if(!LoadTotalVShader())
			return false;

//#ifdef USE_DDS_FILE
	lstrcpy(_pszTexPath,"texture\\effect");
//#else
//	lstrcpy(_pszTexPath,"texture_src\\effect");
//#endif

	lstrcpy(_pszMeshPath,"model");
	lstrcpy(_pszEFFectPath,"effect");

	LoadTotalTexture();

	LoadDefaultText("effect\\font.txt");

	//LoadTotalPartCtrl();

	LoadTotalData();

	_pMatView = pmat;
	D3DXMatrixInverse( &_MatBBoard, NULL, _pMatView );
	_MatBBoard._41 = 0.0f;
    _MatBBoard._42 = 0.0f;
    _MatBBoard._43 = 0.0f;

	_pMatViewProj = pMatviewproj;

	for(int n = 0; n < _iEffectNum; n++)
	{
		for(int m = 0; m < (int)_vecEffectList[n].size(); m++)
		{
			if(_vecEffectList[n][m].IsBillBoard())
			{
				_vecEffectList[n][m].setBillBoardMatrix(&_MatBBoard);
			}
		}
	}

	lwRegisterOutputLoseDeviceProc(g_OnLostDevice);
	lwRegisterOutputResetDeviceProc(g_OnResetDevice);

	return true;
}

int		CMPResManger::GetTextureID(const s_string &sName)
{
#if RESOURCE_SCRIPT == 1
	// ��Դ�ű�����ʱ���Զ������ļ���
	StrMapIter iter = _mapTexture.find(sName);
	if (iter == _mapTexture.end())
		_mapTexture.insert(sName);
#endif

	TEXTURE_MAP::iterator pos = _mapTexture.find( sName );
	if( pos != _mapTexture.end() )
	{
		return (*pos).second;
	}

	//for (size_t i=0; i<_vecTexName.size(); i++)
	//{
	//	if (stricmp(sName.c_str(), _vecTexName[i].c_str()) == 0)
	//		// Success
	//	{
	//		__asm int 3;
	//		return (int)i;
	//	}
	//}
	
#if RESOURCE_SCRIPT == 2
	// ��Դ�ű�ʹ�ò���ʱ����������δ������Ч�ű���
	ToLogService("errors", LogLevel::Error, ": CMPResManger::GetTextureID(),TextureName={}", sName.c_str());
#endif

	// Failure
	char szMsg[64];
	sprintf(szMsg,"ȱ����Ч��ͼ[%s](�ļ������ڻ���Ч��Դ�����ļ�����)",
		sName.c_str());
	ToLogService("errors", LogLevel::Error, "{}",szMsg);
	return -1;
}
//-----------------------------------------------------------------------------
IDirect3DTextureX*	CMPResManger::GetTextureByID( int iID)
{ 
	if(_vecTexList[iID])
		return _vecTexList[iID]->GetTex();
	else
	{
		return NULL;
	}
}
//-----------------------------------------------------------------------------
lwITex*		CMPResManger::GetTextureByIDlw( int iID)
{
	if(_vecTexList[iID])
		return _vecTexList[iID];
	else
	{
		char t_pszFile[MAX_PATH];
#ifdef USE_DDS_FILE_EFFECT
		sprintf(t_pszFile, "%s\\%s.dds",_pszTexPath,_vecTexName[iID].c_str());
#else
		sprintf(t_pszFile, "%s\\%s.tga",_pszTexPath,_vecTexName[iID].c_str());
#endif
		lwITex* tex;

		if(LW_FAILED(lwLoadTex(&tex, m_pSysGraphics->GetResourceMgr(), t_pszFile, 0, D3DFMT_A4R4G4B4)))
		{
			char szMsg[64];
			sprintf(szMsg, "������Ч��ͼ[id=%d]����", iID);
			ToLogService("errors", LogLevel::Error, "{}",szMsg);
			return 0;
		}
		//#endif
		_vecTexList[iID] = tex;

		return _vecTexList[iID];
	}
}
//-----------------------------------------------------------------------------
lwITex* CMPResManger::GetTextureByNamelw(const s_string &sName)
{
	int iTexID = GetTextureID(sName);
	if (iTexID == -1)
	{
		return 0;
	}

	return GetTextureByIDlw(iTexID);

}
//-----------------------------------------------------------------------------
int		CMPResManger::GetMeshID(const s_string &sName)
{
#if RESOURCE_SCRIPT == 1
	StrMapIter iter = _mapMesh.find(pszName);
	if (iter == _mapMesh.end())
		_mapMesh.insert(pszName);
#endif

	MESH_MAP::iterator pos = _mapMesh.find( sName );
	if( pos != _mapMesh.end() )
	{
		return (*pos).second;
	}

	//for (size_t i(0); i<_vecMeshName.size(); i++)
	//{
	//	if (stricmp(sName.c_str(), _vecMeshName[i].c_str()) == 0)
	//	{
	//		__asm int 3;
	//		// Success 
	//		return (int)i;
	//	}
	//}

#if RESOURCE_SCRIPT == 2
	ToLogService("errors", LogLevel::Error, ": CMPResManger::GetMeshID(),MeshName={}",sName.c_str());
#endif

	// Failure
	//char szMsg[64];
	//sprintf(szMsg,"ȱ����Чģ��[%s](�ļ������ڻ���Ч��Դ�����ļ�����)",
	//	sName.c_str());
	//LG("ERROR","msg%s",szMsg);
	return -1;
}

//-----------------------------------------------------------------------------
CEffectModel* CMPResManger::GetMeshByID( int iID)
{

	CEffectModel* pRetMesh(0);

	if(iID >=7)
	{
		if(!_vecMeshList[iID])
		{	// ��һ����Ҫģ�Ͷ��󣬴���
			_vecMeshList[iID] = new CEffectModel;

			_vecMeshList[iID]->InitDevice(m_pDev);
			lwIPathInfo* path_info;
			m_pSys->GetInterface( (LW_VOID**)&path_info, LW_GUID_PATHINFO );
			char szOldPath[260];
			strcpy(szOldPath, path_info->GetPath(PATH_TYPE_MODEL_ITEM));
			path_info->SetPath( PATH_TYPE_MODEL_ITEM, "model\\effect\\" );
			if(!_vecMeshList[iID]->LoadModel((TCHAR*)_vecMeshName[iID].c_str()))
			{
				SAFE_DELETE(_vecMeshList[iID]);
				path_info->SetPath( PATH_TYPE_MODEL_ITEM, szOldPath );

				char szMsg[64];
				sprintf(szMsg,"������Чģ��[id=%d]ʧ��", iID);
				ToLogService("errors", LogLevel::Error, "{}",szMsg);
				return 0;
			}
			if(!_vecMeshList[iID]->GetObject() || !_vecMeshList[iID]->GetObject()->GetPrimitive())
			{
				ToLogService("errors", LogLevel::Error, ": effectmesh->GetObject(),effectmesh->GetPrimitive()__ID={}",iID);
			}else
				_vecMeshList[iID]->GetObject()->GetPrimitive()->SetState(STATE_TRANSPARENT, 0);
			path_info->SetPath( PATH_TYPE_MODEL_ITEM, szOldPath );
			pRetMesh = _vecMeshList[iID];
		}
		else
		{	// ֮����Ҫģ�Ͷ��󣬴���
			if (_vecMeshList[iID]->IsUsing())
			{
				int n = _iMeshNum;
				for (; n < MAXMESH_COUNT; ++n)
				{
					if (_vecMeshList[n] && _vecMeshList[n]->IsUsing())
					{	// ����б������λ���Ѿ���ģ���ˣ�����ģ������ʹ�ã��������һ��λ��
						continue;
					}
					if(!_vecMeshList[n])	
					{	//���Ϊ���´���һ��ģ�Ͷ��󣨳�ʼģ��Ϊδʹ��״̬��
						_vecMeshList[n] = new CEffectModel;
					}

					// ������λ�õ�ģ��û�б�ʹ�ã���ʹ�����ģ�Ͷ���
					if (_vecMeshList[n]->m_iID != iID)
					{	// �����ģ������Ҫ��ģ�Ͳ�ͬ�򿽱�һ��
						if (!_vecMeshList[n]->Copy(*_vecMeshList[iID]))
						{
							SAFE_DELETE(_vecMeshList[n]);
							char szMsg[64];
							sprintf(szMsg,"ȱ����Чģ��[id=%d]��������", iID);
							ToLogService("errors", LogLevel::Error, "{}",szMsg);
							return 0;
						}
					}

					//�������ʾ�Ѿ��ҵ����õ�ģ�Ͷ����������ȽϹ��죬ע�⿼����֮ǰ�����ԣ�
					break;
				}
				if(n >= MAXMESH_COUNT)
				{
					ToLogService("errors", LogLevel::Error, "��Чģ������");
					return 0;
				}
				pRetMesh =_vecMeshList[n];
			}
			else
			{
				pRetMesh = _vecMeshList[iID];
			}
		}
	}
	else 
	{
		pRetMesh = _vecMeshList[iID];
	}
	pRetMesh->m_iID = iID;
	pRetMesh->SetUsing(true);
	return pRetMesh;
}

//-----------------------------------------------------------------------------
CEffectModel* CMPResManger::GetMeshByName(const s_string &sName)
{
	int iMeshID = GetMeshID(sName);
	if (iMeshID == -1)
	{
		return 0;
	}
	return GetMeshByID(iMeshID);

}
//-----------------------------------------------------------------------------
void CMPResManger::DeleteMesh(CEffectModel& rEffectModel)
{
	if(rEffectModel.m_iID >=7)
	{
		rEffectModel.SetUsing(false);
	}
}

//int			CMPResManger::GetItemID(s_string pszName)
//{
//	std::vector<s_string>::iterator it = find( _vecItemName.begin(),_vecItemName.end(), pszName );
//	if ( it != _vecItemName.end() )
//	{
//		return (int)(it - _vecItemName.begin());
//	}
//	return -1;
//
//}
//CEffectModel*	CMPResManger::GetItemByID(int iID)
//{
//	return &_vecItemList[iID];
//}
//////////////////////////////////////////////////////////////////////////

int		CMPResManger::GetEffectID(const s_string &pszName)
{
#if RESOURCE_SCRIPT == 1
	StrMapIter iter = _mapEffect.find(pszName);
	if (iter == _mapEffect.end())
		_mapEffect.insert(pszName);
#endif

	//char *pszDataName = _strlwr( _strdup( pszName.c_str() ) );
	//s_string strName = pszDataName;
	//transform(pszName.begin(), pszName.end(), //source
	//		  pszName.begin(), //destination
	//		  tolower);


	EFFECT_MAP::iterator pos = _mapEffect.find( pszName );
	if( pos != _mapEffect.end() )
	{
		return (*pos).second;
	}

	//std::vector<s_string>::iterator it = find( _vecEffectName.begin(), _vecEffectName.end(), pszName );
	//for (size_t i(0); i<_vecEffectName.size(); ++i)
	//{
	//	if (stricmp(_vecEffectName[i].c_str(), pszName.c_str()) == 0)
	//	{
	//		__asm int 3;
	//		return (int)i;
	//	}
	//}
	//if ( it != _vecEffectName.end() )
	//{
	//	//SAFE_DELETE_ARRAY(pszDataName);
	//	return (int)(it - _vecEffectName.begin());
	//}
	////SAFE_DELETE_ARRAY(pszDataName);
#if RESOURCE_SCRIPT == 2
	ToLogService("errors", LogLevel::Error, ": CMPResManger::GetEffectID(),EffectName={}",pszName.c_str());
#endif


	return -1;
}
std::vector<I_Effect>&	CMPResManger::GetEffectByID( int iID)
{
	I_Effect *pEffect = &(_vecEffectList[iID][0]);

	int n = (int)_vecEffectList[iID].size();
	if( n <=0)
	{
		char t_pszFile[MAX_PATH];
		sprintf(t_pszFile, "%s\\%s",_pszEFFectPath,_vecEffectName[iID].c_str());

		//__asm int 3;
		if(!LoadEffectFromFile(iID, t_pszFile))
			return _vecEffectList[iID];
		_vecEffectList[iID][0].setEffectName(_vecEffectName[iID]);

	}
	return _vecEffectList[iID];
}

I_Effect*	CMPResManger::GetSubEffectByID(int iID, int iSubIdx)
{
	return &_vecEffectList[iID][iSubIdx];
}

IDirect3DVertexShaderX*	CMPResManger::GetVShaderByID(int iID)
{
	if(!m_bUseSoft)
		return _vecVShader[iID];
	else
		return _vecVShader[0];
}

IDirect3DVertexDeclarationX* CMPResManger::GetVDeclByID(int iID)
{
	if (!m_bUseSoft)
		return _vecVDecl[iID] ;
	else
		return _vecVDecl[0];
}
	
IDirect3DVertexShaderX*	CMPResManger::GetShadeVS()
{
	return _dwShadeMapVS;
}
IDirect3DVertexDeclarationX* CMPResManger::GetShadeVDecl()
{
	return _vecVDecl[5];
}

CEffectModel*	CMPResManger::GetShadeMesh()
{
	return _CShadeModel;
}

IDirect3DVertexShaderX*	CMPResManger::GetFontVS()
{
	return _dwFontVS;
}
IDirect3DVertexDeclarationX* CMPResManger::GetFontVDecl()
{
	return _vecVDecl[4];
}

//int		CMPResManger::GetEffectTechByID(int iID)
//{
//	return _vecEffTech[iID];
//}

EffParameter*   CMPResManger::GetEffectParamByID(int iID)
{
	return &_vecEffectParam[iID];
}

IDirect3DVertexDeclarationX* CMPResManger::GetMinimapVDecl()
{
	return _vecVDecl[6];
}



//CMPShadeCtrl*	CMPResManger::GetShadeMapIns(s_string strName)
//{
//	std::vector<s_string>::iterator it = find( _vecShadeName.begin(),_vecShadeName.end(),strName );
//	if ( it == _vecShadeName.end() )
//		return NULL;
//	CMPShadeCtrl* pShade = new CMPShadeCtrl;
//
//	FILE* pFile;
//	pFile = fopen(strName.c_str(), "rb");
//	if(!pFile)
//		return false;
//	
//	int itype;
//	fread(&itype, sizeof(int),1,pFile);
//
//	float fRadius;
//	fread(&fRadius,sizeof(float),1,pFile);
//
//	char t_pszName[32];
//	fread(t_pszName, sizeof(char),32,pFile);
//
//	int irow,icol;
//	if(itype == SHADE_ANI)
//	{
//		fread(&irow,sizeof(int),1,pFile);
//		fread(&icol,sizeof(int),1,pFile);
//	}
//
//	pShade->Create(t_pszName,this,fRadius,itype == SHADE_ANI ? true : false, irow,icol);
//
//	if(itype == SHADE_ANI)
//		((CMPShadeEX*)pShade->GetShadeMap())->LoadFromFile(pFile);
//	else
//		pShade->GetShadeMap()->LoadFromFile(pFile);
//
//	fclose(pFile);
//	return pShade;
//}

bool	CMPResManger::LoadTotalTexture()
{
#if RESOURCE_SCRIPT == 0 || RESOURCE_SCRIPT == 1
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE  t_hFind = NULL;

		lstrcpy(t_Path,_pszTexPath);
#ifdef USE_DDS_FILE_EFFECT
		lstrcat(t_Path,"\\*.dds");
#else
		lstrcat(t_Path,"\\*.tga");
#endif

		//char pszname[32];
		//AfxMessageBox(t_Path);
		//char t_pszFile[MAX_PATH];

		//LPDIRECT3DTEXTURE8  t_lptex = NULL;
		if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
			return false;
		string sFileName;
		do{
			if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//sprintf(t_pszFile, "%s\\%s",_pszTexPath,t_sfd.cFileName);
				////ȫ��ת����Сд
				//memset(pszname,0,32);
				//char *pszDataName = _strlwr( _strdup( t_sfd.cFileName ) );
				//int len = lstrlen(pszDataName);
				//memcpy(pszname, pszDataName,len - 4); 
				//pszname[len - 4 + 1] = '\0';

				sFileName = t_sfd.cFileName;
				transform(sFileName.begin(), sFileName.end(),
					sFileName.begin(),
					[](unsigned char c) { return std::tolower(c); });
				sFileName = sFileName.substr(0, sFileName.rfind('.'));

				{
					//SAFE_RELEASE(t_lptex);
					_mapTexture[sFileName] = (int)_vecTexName.size();
					_vecTexName.push_back(sFileName.c_str());
					//SAFE_DELETE_ARRAY(pszDataName);
				}
			}
		}while(FindNextFile(t_hFind,&t_sfd));
		FindClose(t_hFind);
	}
#else
	{
		MPResourceInfo* pResInfo(0);
		char szName[32];
		memset(szName,0,32);

		for(int i(1); i<MPResourceSet::GetInstance().GetLastID() +1; i++)
		{
			pResInfo = MPResourceSet::GetInstance().GetResourceInfoByID(i);
			if (!pResInfo)
				continue;
			if (pResInfo->GetType() == MPResourceInfo::RT_TEXTURE)
			{
				if((strstr(pResInfo->szDataName,".dds")==NULL)&&strstr(pResInfo->szDataName,".tga")==NULL)
				{
					_mapTexture[pResInfo->szDataName] = (int)_vecTexName.size();
					_vecTexName.push_back(pResInfo->szDataName);
				}
				else
				{
					int len = lstrlen(pResInfo->szDataName);
					memcpy(szName, pResInfo->szDataName,len - 4); 
					_mapTexture[pResInfo->szDataName] = (int)_vecTexName.size();
					_vecTexName.push_back(szName);
					memset(szName,0,32);
				}
			}
		}
	}
#endif

	_iTexNum = (int)_vecTexName.size();
	_vecTexList.resize(_iTexNum);
	for(int iw = 0; iw < _iTexNum; iw++)
	{
		_vecTexList[iw] = NULL;
	}
	return true;
}

void CMPResManger::LoadTotalData()
{
#ifndef _UNLOADRES
	char t_Path[MAX_PATH];
	WIN32_FIND_DATA t_sfd;
	HANDLE  t_hFind = NULL;


	// װ�ض���
	lstrcpy(t_Path,"animation\\");
	lstrcat(t_Path,"\\*.lab");

	int count = 0;
	if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
		return;
	do{
		if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			int length = (int)strlen(t_sfd.cFileName);
			char *sname = &t_sfd.cFileName[length - 4];
			if(strcmp(sname,".lab") != 0)
			{
				continue; 
			}
			char path[ LW_MAX_PATH ];
			sprintf( path, "%s%s", "animation\\", t_sfd.cFileName );

			//ȫ��ת����Сд
			if( !g_GeomManager.LoadBoneData( t_sfd.cFileName ) )
			{
				//LG("error","msg:װ��ģ�Ͷ���ʧ��(%s)��", path );
			}
			count ++;
			if( count == 50 )
			{
				break;
				//__asm int 3;
			}
			else if( count == 200 )
			{
				//__asm int 3;
			}
			else if( count == 300 )
			{
				//__asm int 3;
				break;
			}
			else if( count == 400 )
			{
				//__asm int 3;
			}
		}

	}while(FindNextFile(t_hFind,&t_sfd));
	FindClose(t_hFind);
#endif
}

void CMPResManger::LoadTotalRes()
{
	//LoadTotalEffect();
	LoadTotalPartCtrl();

	char t_Path[MAX_PATH];
	WIN32_FIND_DATA t_sfd;
	HANDLE  t_hFind = NULL;

//	lstrcpy(t_Path,"model\\effect");
//	lstrcat(t_Path,"\\*.lgo");
//
//	if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
//		return;
//	string sFileName;
//
//	lwIPathInfo* path_info;
//	m_pSys->GetInterface( (LW_VOID**)&path_info, LW_GUID_PATHINFO );
//	char szOldPath[260];
//	strcpy(szOldPath, path_info->GetPath(PATH_TYPE_MODEL_ITEM));
//	path_info->SetPath( PATH_TYPE_MODEL_ITEM, "model\\effect\\" );
//
//	do{
//		if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//		{
//			int length = (int)strlen(t_sfd.cFileName);
//			char *sname = &t_sfd.cFileName[length - 4];
//			if(strcmp(sname,".lgo") != 0)
//			{
//				continue; 
//			}
//			//ȫ��ת����Сд
//			sFileName = t_sfd.cFileName;
//			transform(sFileName.begin(), sFileName.end(),
//				sFileName.begin(),
//				tolower);
//
//#ifndef _UNLOADRES
//			_vecMeshList[_iMeshNum] = new CEffectModel;
//			_vecMeshList[_iMeshNum]->InitDevice(m_pDev);
//			_vecMeshList[_iMeshNum]->LoadModel(sFileName.c_str());
//#endif
//
//			_mapMesh[sFileName] = (int)_vecMeshName.size();
//			_vecMeshName.push_back(sFileName.c_str());
//			_iMeshNum++;
//		}
//
//	}while(FindNextFile(t_hFind,&t_sfd));
//	FindClose(t_hFind);
//	path_info->SetPath( PATH_TYPE_MODEL_ITEM, szOldPath );

#ifndef _UNLOADRES
	//// ����
	//lstrcpy(t_Path,"model\\item");
	//lstrcat(t_Path,"\\*.lgo");

	//if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
	//	return;
	//do{
	//	if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	//	{
	//		int length = (int)strlen(t_sfd.cFileName);
	//		char *sname = &t_sfd.cFileName[length - 4];
	//		if(strcmp(sname,".lgo") != 0)
	//		{
	//			continue; 
	//		}

	//		//ȫ��ת����Сд
	//		if( !g_GeomManager.LoadGeomobj( t_sfd.cFileName ) )
	//		{
	//			//LG("error","msg:װ��itemģ��ʧ��(%s)��", t_sfd.cFileName );
	//		}
	//	}

	//}while(FindNextFile(t_hFind,&t_sfd));
	//FindClose(t_hFind);

	// ��ɫģ��
	lstrcpy(t_Path,"model\\character");
	lstrcat(t_Path,"\\*.lgo");

	static int nNum = 0;
	if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
		return;
	do{
		if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			int length = (int)strlen(t_sfd.cFileName);
			char *sname = &t_sfd.cFileName[length - 4];
			if(strcmp(sname,".lgo") != 0)
			{
				continue; 
			}
			
			nNum++;
			if( nNum++ >= 900 )
				break;

			//ȫ��ת����Сд
			if( !g_GeomManager.LoadGeomobj( t_sfd.cFileName ) )
			{
				//LG("error","msg:װ��ģ��ʧ��(%s)��", t_sfd.cFileName );
			}
		}

	}while(FindNextFile(t_hFind,&t_sfd));
	FindClose(t_hFind);
	//char szData[32];
	//sprintf( szData, "%d", nNum );
	//MessageBox(NULL, szData, "", MB_OK );

#endif
}

bool	CMPResManger::LoadTotalMesh()
{
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
	for (int n = 0; n< MAXMESH_COUNT; n++)
	{
		_vecMeshList[n] = NULL;
	}
	_vecMeshList[0] = new CEffectModel;
	_vecMeshList[0]->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_vecMeshList[0]->CreateTriangle();

	_vecMeshList[1] = new CEffectModel;
	_vecMeshList[1]->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_vecMeshList[1]->CreateRect();

	_vecMeshList[2] = new CEffectModel;
	_vecMeshList[2]->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_vecMeshList[2]->CreatePlaneRect();

	_vecMeshList[3] = new CEffectModel;
	_vecMeshList[3]->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_vecMeshList[3]->CreatePlaneTriangle();

	_vecMeshList[4] = new CEffectModel;
	_vecMeshList[4]->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_vecMeshList[4]->CreateRectZ();

	_vecMeshList[5] = new CEffectModel;
	_vecMeshList[5]->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_vecMeshList[5]->CreateCone(8,3,2);

	_vecMeshList[6] = new CEffectModel;
	_vecMeshList[6]->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_vecMeshList[6]->CreateCylinder(8,3,1,3);

	_CShadeModel= NULL;
	_CShadeModel = new CEffectModel;
	_CShadeModel->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_CShadeModel->CreateShadeModel();
	
#if USE_RESOURCE_SCRIPT == 0 || USE_RESOURCE_SCRIPT == 1
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE  t_hFind = NULL;

		lstrcpy(t_Path,"model\\effect");
		lstrcat(t_Path,"\\*.lgo");

		if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
			return true;
		string sFileName;

		lwIPathInfo* path_info;
		m_pSys->GetInterface( (LW_VOID**)&path_info, LW_GUID_PATHINFO );
		char szOldPath[260];
		strcpy(szOldPath, path_info->GetPath(PATH_TYPE_MODEL_ITEM));
		path_info->SetPath( PATH_TYPE_MODEL_ITEM, "model\\effect\\" );

		do{
			if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				int length = (int)strlen(t_sfd.cFileName);
				char *sname = &t_sfd.cFileName[length - 4];
				if(strcmp(sname,".lgo") != 0)
				{
					continue; 
				}
				//ȫ��ת����Сд
				sFileName = t_sfd.cFileName;
				transform(sFileName.begin(), sFileName.end(),
					sFileName.begin(),
					[](unsigned char c) { return std::tolower(c); });

				_vecMeshList[_iMeshNum] = new CEffectModel;
				_vecMeshList[_iMeshNum]->InitDevice(m_pDev);
				_vecMeshList[_iMeshNum]->LoadModel(sFileName.c_str());

				_mapMesh[sFileName] = (int)_vecMeshName.size();
				_vecMeshName.push_back(sFileName.c_str());
				_iMeshNum++;
			}

		}while(FindNextFile(t_hFind,&t_sfd));
		FindClose(t_hFind);
		path_info->SetPath( PATH_TYPE_MODEL_ITEM, szOldPath );

		// ����
		lstrcpy(t_Path,"model\\item");
		lstrcat(t_Path,"\\*.lgo");

		if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
			return true;
		do{
			if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				int length = (int)strlen(t_sfd.cFileName);
				char *sname = &t_sfd.cFileName[length - 4];
				if(strcmp(sname,".lgo") != 0)
				{
					continue; 
				}

				//ȫ��ת����Сд
				if( !g_GeomManager.LoadGeomobj( t_sfd.cFileName ) )
				{
					//LG("error","msg:װ��itemģ��ʧ��(%s)��", t_sfd.cFileName );
				}
			}

		}while(FindNextFile(t_hFind,&t_sfd));
		FindClose(t_hFind);

		//// ��ɫģ��
		//lstrcpy(t_Path,"model\\character");
		//lstrcat(t_Path,"\\*.lgo");

		//if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
		//	return true;
		//do{
		//	if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		//	{
		//		int length = (int)strlen(t_sfd.cFileName);
		//		char *sname = &t_sfd.cFileName[length - 4];
		//		if(strcmp(sname,".lgo") != 0)
		//		{
		//			continue; 
		//		}

		//		//ȫ��ת����Сд
		//		if( !g_GeomManager.LoadGeomobj( t_sfd.cFileName ) )
		//		{
		//			//LG("error","msg:װ��ģ��ʧ��(%s)��", t_sfd.cFileName );
		//		}
		//	}

		//}while(FindNextFile(t_hFind,&t_sfd));
		//FindClose(t_hFind);
	}
#else
	{
		MPResourceInfo* pResInfo(0);
		for(int i(1); i<MPResourceSet::GetInstance().GetLastID() +1; i++)
		{
			pResInfo = MPResourceSet::GetInstance().GetResourceInfoByID(i);
			if (!pResInfo)
				continue;
			if (pResInfo->GetType() == MPResourceInfo::RT_MESH)
			{
				_mapMesh[pResInfo->szDataName] = (int)_vecMeshName.size();
				_vecMeshName.push_back(pResInfo->szDataName);
				_iMeshNum++;
			}
		}
	}
#endif

	return true;
}

I_Effect* 	CMPResManger::AddEffectToMgr(const s_string& strName)
{
	_iEffectNum++;

	_vecEffectList.resize( _iEffectNum );

	_vecEffectName.resize(_iEffectNum );

	_vecEffectParam.resize(_iEffectNum);

	_mapEffect[strName] = _iEffectNum - 1;
	_vecEffectName[_iEffectNum - 1] = strName;

	_vecEffectList[_iEffectNum - 1].resize(1);

	I_Effect *pEffect = &(_vecEffectList[_iEffectNum - 1][0]);
	_vecEffectList[_iEffectNum - 1][0].ReleaseAll();

	//_vecEffectParam[_iEffectNum - 1]

	return &_vecEffectList[_iEffectNum - 1][0];
}

void	CMPResManger::AddUniteEffectToMgr(std::vector<I_Effect>& vecEffArray)
{
	_iEffectNum++;
	_vecEffectList.resize( _iEffectNum );
	_vecEffectName.resize( _iEffectNum );
	_mapEffect[vecEffArray[0].getEffectName()] = _iEffectNum - 1;
	_vecEffectName[_iEffectNum - 1] = vecEffArray[0].getEffectName();
	_vecEffectList[_iEffectNum - 1] = vecEffArray;
	for (INT n = 0; n < (INT)vecEffArray.size(); n++)
	{
		_vecEffectList[_iEffectNum - 1][n].BoundingRes(this);
	}
}


//!װ�����Ч�����ļ�
bool	CMPResManger::LoadEffectFromFile(int idx, char* pszFileName)
{
	FILE* t_pFile;
	t_pFile = fopen(pszFileName, "rb");
	if(!t_pFile)
		return false;
	//!�汾
	DWORD t_dwVersion;
	int   t_temp;
	fread(&t_dwVersion,sizeof(t_dwVersion),1,t_pFile);
	//if(t_dwVersion != 1)
	//	return false;

	fread(&t_temp,sizeof(int),1,t_pFile);
	_vecEffectParam[idx].m_iIdxTech = t_temp;
	char t_pszName[32];

	fread(&_vecEffectParam[idx].m_bUsePath, sizeof(bool),1,t_pFile);
	fread(t_pszName, sizeof(char),32,t_pFile);
	_vecEffectParam[idx].m_szPathName = t_pszName;

	fread(&_vecEffectParam[idx].m_bUseSound, sizeof(bool),1,t_pFile);
	fread(t_pszName, sizeof(char),32,t_pFile);
	_vecEffectParam[idx].m_szSoundName = t_pszName;

	fread(&_vecEffectParam[idx].m_bRotating, sizeof(bool),1,t_pFile);
	fread(&_vecEffectParam[idx].m_SVerRota, sizeof(D3DXVECTOR3),1,t_pFile);
	fread(&_vecEffectParam[idx].m_fRotaVel, sizeof(float),1,t_pFile);

	fread(&t_temp,sizeof(int),1,t_pFile);

	_vecEffectList[idx].resize(t_temp);

	for(int n = 0; n < t_temp; n++)
	{
		_vecEffectList[idx][n].LoadFromFile(t_pFile,t_dwVersion);
		_vecEffectList[idx][n].Reset();
		_vecEffectList[idx][n].m_pDev = m_pDev;
	}

	fclose(t_pFile);
	return true;
}
bool	CMPResManger::LoadTotalEffect()
{
#if RESOURCE_SCRIPT == 0 || RESOURCE_SCRIPT == 1
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE  t_hFind = NULL;

		lstrcpy(t_Path,_pszEFFectPath);
		lstrcat(t_Path,"\\*.eff");

		char t_pszFile[MAX_PATH];

		if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
			return false;
		do{
			if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				int length = (int)strlen(t_sfd.cFileName);
				char *sname = &t_sfd.cFileName[length - 4];
				if(stricmp(sname,".eff") != 0)
				{
					//LG(t_sfd.cFileName, "���Ǳ�׼��.eff�ļ�,��ӱ���Ŀ¼ɾ��");
					continue;
				}
				////ȫ��ת����Сд
				string sFileName;
				sFileName = t_sfd.cFileName;
				transform(sFileName.begin(), sFileName.end(),
					sFileName.begin(),
					[](unsigned char c) { return std::tolower(c); });

				sprintf(t_pszFile, "%s\\%s",_pszEFFectPath,t_sfd.cFileName);
				_vecEffectList.resize( _iEffectNum + 1);
				_vecEffectList[_iEffectNum].clear();
				_vecEffectParam.resize(_iEffectNum + 1);

				if(!LoadEffectFromFile(_iEffectNum, t_pszFile))
				{
					char szData[1024];
					sprintf( szData, "װ����Ч�ļ�(%s)ʧ�ܣ�", t_pszFile );
					MessageBox( NULL, szData, "Error", MB_OK );
					//return false;
				}

				_vecEffectName.resize(_iEffectNum + 1);

				_mapEffect[t_sfd.cFileName] = _iEffectNum;

				_vecEffectName[_iEffectNum] = t_sfd.cFileName;

				//�����Ч���ĵ�һ����Ч����������Ϊ�ļ�����
				_vecEffectList[_iEffectNum][0].setEffectName(_vecEffectName[_iEffectNum]);

				_iEffectNum++;
			}

		}while(FindNextFile(t_hFind,&t_sfd));
		FindClose(t_hFind);
	}
#else
	{
		MPResourceInfo* pResInfo(0);
		char szFile[MAX_PATH];

		for(int i(1); i<MPResourceSet::GetInstance().GetLastID() + 1; i++)
		{
			pResInfo = MPResourceSet::GetInstance().GetResourceInfoByID(i);
			if (!pResInfo)
				continue;
			if (pResInfo->GetType() == MPResourceInfo::RT_EFF)
			{
				_vecEffectList.resize( _iEffectNum + 1);
				_vecEffectList[_iEffectNum].clear();
				_vecEffectParam.resize(_iEffectNum + 1);

				//if(!LoadEffectFromFile(_iEffectNum, t_pszFile))
				//	return false;

				_vecEffectName.resize(_iEffectNum + 1);
				_mapEffect[pResInfo->szDataName] = _iEffectNum;
				_vecEffectName[_iEffectNum] = pResInfo->szDataName;
				//SAFE_DELETE_ARRAY(pszDataName);

				//�����Ч���ĵ�һ����Ч����������Ϊ�ļ�����
				//_vecEffectList[_iEffectNum][0].setEffectName(_vecEffectName[_iEffectNum]);

				_iEffectNum++;
			}
		}
	}
#endif
	return true;
}

//void	CMPResManger::UpdateTotalModel()
//{
//	_vecMeshList[0].CreateTriangle();
//	_vecMeshList[1].CreateRect();
//	_vecMeshList[2].CreatePlaneRect();
//}

bool	CMPResManger::LoadTotalVShader()
{
	char t_Path[MAX_PATH];
		
	LPD3DXBUFFER pCode;   //!?????
	if(!m_bUseSoft)
	{
		// TODO
		//DWORD dwEffVerDecl[] =
		//{
		//	D3DVSD_STREAM( 0 ),
		//		D3DVSD_REG( D3DVSDE_POSITION , D3DVSDT_FLOAT3 ), // Position of first mesh
		//		D3DVSD_REG( D3DVSDE_BLENDINDICES,D3DVSDT_FLOAT1),
		//		D3DVSD_REG( D3DVSDE_DIFFUSE, D3DVSDT_D3DCOLOR ), // diffuse
		//		D3DVSD_REG( D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2 ), // Tex coords
		//		D3DVSD_END()
		//};

#if (defined LW_USE_DX9)
		sprintf(t_Path, "shader\\dx9\\eff1.vsh");
#else
		sprintf(t_Path,"shader\\eff1.vsh");
#endif
		while(SUCCEEDED(D3DXAssembleShaderFromFile( t_Path, NULL, NULL, 0, &pCode, NULL )))
		{
			_iVShaderNum++;
			_vecVShader.resize(_iVShaderNum);
			//_vecVShader[_iVShaderNum - 1] = new DWORD;
		//_DbgOut( " technique.Name", _iTechNum, S_OK,  (TCHAR*)technique.Name );
#ifdef USE_RENDER
			if( FAILED(m_pDev->GetDevice()->CreateVertexShader( 
				(DWORD*)pCode->GetBufferPointer(),
				(IDirect3DVertexShaderX**)&_vecVShader[_iVShaderNum - 1] ) ) )
#else
			if( FAILED(m_pDev->CreateVertexShader( dwEffVerDecl, 
				(DWORD*)pCode->GetBufferPointer(),
				&_vecVShader[_iVShaderNum - 1] , FALSE ) ) )
#endif
			 {
				MessageBox(NULL,t_Path,"ERROR",0);
				return false;
			 }

#if (defined LW_USE_DX9)
			sprintf(t_Path, "shader\\dx9\\eff%d.vsh", _iVShaderNum + 1);
#else
			sprintf(t_Path,"shader\\eff%d.vsh",_iVShaderNum + 1);
#endif
			pCode->Release();
			pCode = NULL;
		}

		// TODO
		//DWORD dwFontDecl[] =
		//{
		//	D3DVSD_STREAM( 0 ),
		//		D3DVSD_REG( D3DVSDE_POSITION ,		D3DVSDT_FLOAT3 ), // Position of first mesh
		//		D3DVSD_REG( D3DVSDE_BLENDWEIGHT,	D3DVSDT_FLOAT1),
		//		D3DVSD_REG( D3DVSDE_DIFFUSE,		D3DVSDT_D3DCOLOR ), // diffuse
		//		D3DVSD_REG( D3DVSDE_TEXCOORD0,		D3DVSDT_FLOAT2 ), // Tex coords
		//		D3DVSD_END()
		//};
#if (defined LW_USE_DX9)
		sprintf(t_Path, "shader\\dx9\\font.vsh");
#else
		sprintf(t_Path,"shader\\font.vsh");
#endif
		//_dwFontVS = new DWORD;
		if(SUCCEEDED(D3DXAssembleShaderFromFile( t_Path, NULL, NULL, 0, &pCode, NULL )))
		{

#ifdef USE_RENDER
			if( FAILED(m_pDev->GetDevice()->CreateVertexShader( 
				(DWORD*)pCode->GetBufferPointer(),
				(IDirect3DVertexShaderX**)&_dwFontVS ) ) )
#else
			if( FAILED(m_pDev->CreateVertexShader( dwFontDecl, 
				(DWORD*)pCode->GetBufferPointer(),
				&_dwFontVS , FALSE ) ) )
#endif
		 {
			 MessageBox(NULL,"shader\\font.vsh","ERROR",0);
			 return false;
		 }
		 pCode->Release();
		 pCode = NULL;
		}
		else
		{
			MessageBox(NULL,"shader\\font.vsh","ERROR",0);
			return false;
		}

		// TODO
		//DWORD dwEffShadeDecl[] =
		//{
		//	D3DVSD_STREAM( 0 ),
		//		D3DVSD_REG( D3DVSDE_POSITION ,		D3DVSDT_FLOAT3 ), // Position of first mesh
		//		//D3DVSD_REG( D3DVSDE_BLENDWEIGHT,	D3DVSDT_FLOAT1),
		//		//D3DVSD_REG( D3DVSDE_NORMAL,			D3DVSDT_FLOAT3),
		//		D3DVSD_REG( D3DVSDE_DIFFUSE,		D3DVSDT_D3DCOLOR ), // diffuse
		//		D3DVSD_REG( D3DVSDE_TEXCOORD0,		D3DVSDT_FLOAT2 ), // Tex coords
		//		D3DVSD_REG( D3DVSDE_TEXCOORD1,		D3DVSDT_FLOAT2 ), // Tex coords
		//		D3DVSD_END()
		//};
#if (defined LW_USE_DX9)
		sprintf(t_Path, "shader\\dx9\\shadeeff.vsh");
#else
		sprintf(t_Path,"shader\\shadeeff.vsh");
#endif
		//_dwShadeMapVS = new DWORD;
		if(SUCCEEDED(D3DXAssembleShaderFromFile( t_Path, NULL, NULL, 0, &pCode, NULL )))
		{

#ifdef USE_RENDER
			if( FAILED(m_pDev->GetDevice()->CreateVertexShader( 
				(DWORD*)pCode->GetBufferPointer(),
				(IDirect3DVertexShaderX**)&_dwShadeMapVS ) ) )
#else
			if( FAILED(m_pDev->CreateVertexShader( dwEffShadeDecl, 
				(DWORD*)pCode->GetBufferPointer(),
				&_dwShadeMapVS , FALSE ) ) )
#endif
		 {
			 MessageBox(NULL,"shader\\shadeeff.vsh","ERROR",0);
			 return false;
		 }
		 pCode->Release();
		 pCode = NULL;
		}
		else
		{
			MessageBox(NULL,"shader\\shadeeff.vsh","ERROR",0);
			return false;
		}
	}
	else
	{
		// TODO
		//DWORD dwEffVerDecl[] =
		//{
		//	D3DVSD_STREAM( 0 ),
		//		D3DVSD_REG( D3DVSDE_POSITION , D3DVSDT_FLOAT3 ), // Position of first mesh
		//		D3DVSD_REG( D3DVSDE_BLENDINDICES,D3DVSDT_FLOAT1),
		//		D3DVSD_REG( D3DVSDE_DIFFUSE, D3DVSDT_D3DCOLOR ), // diffuse
		//		D3DVSD_REG( D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2 ), // Tex coords
		//		D3DVSD_END()
		//};

#if (defined LW_USE_DX9)
		sprintf(t_Path, "shader\\dx9\\eff2.vsh");
#else
		sprintf(t_Path,"shader\\eff2.vsh");
#endif
		if(SUCCEEDED(D3DXAssembleShaderFromFile( t_Path, NULL, NULL, 0, &pCode, NULL )))
		{
			_iVShaderNum++;
			_vecVShader.resize(_iVShaderNum);
			//_vecVShader[_iVShaderNum - 1] = new DWORD;
			//_DbgOut( " technique.Name", _iTechNum, S_OK,  (TCHAR*)technique.Name );
#ifdef USE_RENDER
			if( FAILED(m_pDev->GetDevice()->CreateVertexShader( 
				(DWORD*)pCode->GetBufferPointer(),
				(IDirect3DVertexShaderX**)&_vecVShader[_iVShaderNum - 1] ) ) )
			{
				//MessageBox(NULL,"shader\\eff2.vsh","ERROR",0);
				//return false;
				_vecVShader[0] = 0L;
			}
#else
			if( FAILED(m_pDev->CreateVertexShader( dwEffVerDecl, 
				(DWORD*)pCode->GetBufferPointer(),
				&_vecVShader[_iVShaderNum - 1] , FALSE ) ) )
			{
				//MessageBox(NULL,"shader\\eff2.vsh","ERROR",0);
				//return false;
				_vecVShader[0] = 0L;
			}
#endif

			pCode->Release();
			pCode = NULL;
		}
	}



	return true;
}

bool	CMPResManger::LoadTotalVShader(lwISysGraphics* sys_graphics)
{
	lwISystem* sys = sys_graphics->GetSystem();

	char path[ LW_MAX_PATH ];
	lwIPathInfo* path_info = 0;
	sys->GetInterface( (LW_VOID**)&path_info, LW_GUID_PATHINFO );

	lwIResourceMgr* res_mgr;
	lwIShaderMgr* shader_mgr;

	sys_graphics->GetInterface( (LW_VOID**)&res_mgr, LW_GUID_RESOURCEMGR );
	shader_mgr = res_mgr->GetShaderMgr();

	
	DWORD shader_type[] =
	{
		VSTU_EFFECT_E1,
		VSTU_EFFECT_E2,
		VSTU_EFFECT_E3,
		VSTU_EFFECT_E4,
		VSTU_FONT_E5,
		VSTU_SHADE_E6,
		VSTU_MINIMAP_E6,
	};

#if(defined LW_USE_DX8)
	DWORD dwDecEff[] =
	{
		D3DVSD_STREAM( 0 ),
			D3DVSD_REG( D3DVSDE_POSITION , D3DVSDT_FLOAT3 ), // Position of first mesh
			D3DVSD_REG( D3DVSDE_BLENDINDICES,D3DVSDT_FLOAT1),
			D3DVSD_REG( D3DVSDE_DIFFUSE, D3DVSDT_D3DCOLOR ), // diffuse
			D3DVSD_REG( D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2 ), // Tex coords
			D3DVSD_END()
	};

	if(m_bUseSoft)
	{
		sprintf( path, "%s%s", path_info->GetPath( PATH_TYPE_SHADER ), "eff2.vso" );

		if( LW_FAILED( shader_mgr->RegisterVertexShader( shader_type[1], path, 0, dwDecEff, sizeof(dwDecEff),1 )))
		{
			MessageBox(NULL,"load shader\\eff2.vso","ERROR",0);
			return false;
		}
		_iVShaderNum++;
		_vecVShader.resize(_iVShaderNum);
		_vecVShader[_iVShaderNum - 1] = 0;
		shader_mgr->QueryVertexShader(&_vecVShader[_iVShaderNum - 1],shader_type[1]);

		_bMagr = true;
		return true;
	}

	DWORD dwDecFont[] =
	{
		D3DVSD_STREAM( 0 ),
			D3DVSD_REG( D3DVSDE_POSITION ,		D3DVSDT_FLOAT3 ), // Position of first mesh
			D3DVSD_REG( D3DVSDE_BLENDWEIGHT,	D3DVSDT_FLOAT1),
			D3DVSD_REG( D3DVSDE_DIFFUSE,		D3DVSDT_D3DCOLOR ), // diffuse
			D3DVSD_REG( D3DVSDE_TEXCOORD0,		D3DVSDT_FLOAT2 ), // Tex coords
			D3DVSD_END()
	};

	DWORD dwDecShade[] =
	{
		D3DVSD_STREAM( 0 ),
			D3DVSD_REG( D3DVSDE_POSITION ,		D3DVSDT_FLOAT3 ), // Position of first mesh
			//D3DVSD_REG( D3DVSDE_BLENDWEIGHT,	D3DVSDT_FLOAT1),
			//D3DVSD_REG( D3DVSDE_NORMAL,			D3DVSDT_FLOAT3),
			D3DVSD_REG( D3DVSDE_DIFFUSE,		D3DVSDT_D3DCOLOR ), // diffuse
			D3DVSD_REG( D3DVSDE_TEXCOORD0,		D3DVSDT_FLOAT2 ), // Tex coords
			D3DVSD_REG( D3DVSDE_TEXCOORD1,		D3DVSDT_FLOAT2 ), // Tex coords
			D3DVSD_END()
	};
	DWORD dwMapDecl[] =
	{
		D3DVSD_STREAM( 0 ),
			D3DVSD_REG( D3DVSDE_POSITION ,		D3DVSDT_FLOAT3 ), // Position of first mesh
			D3DVSD_REG( D3DVSDE_BLENDWEIGHT,	D3DVSDT_FLOAT1),
			D3DVSD_REG( D3DVSDE_DIFFUSE,		D3DVSDT_D3DCOLOR ), // diffuse
			D3DVSD_REG( D3DVSDE_TEXCOORD0,		D3DVSDT_FLOAT2 ), // Tex coords
			D3DVSD_END()
	};

	DWORD* decl_tab[] =
	{
		dwDecEff,
		dwDecEff,
		dwDecEff,
		dwDecEff,
		dwDecFont,
		dwDecShade,
		dwMapDecl,
	};
	DWORD decl_size[] =
	{
		sizeof(dwDecEff),
		sizeof(dwDecEff),
		sizeof(dwDecEff),
		sizeof(dwDecEff),
		sizeof(dwDecFont),
		sizeof(dwDecShade),
		sizeof(dwMapDecl),
	};

	const char* shader_file[] = 
	{
		"eff1.vso",
		"eff2.vso",
		"eff3.vso",
		"eff4.vso",
		"font.vso",
		"shadeeff.vso",
		"minimap.vso",
	};
	for( int i = 0; i < 7; i++ )
	{
		sprintf( path, "%s%s", path_info->GetPath( PATH_TYPE_SHADER ), shader_file[i] );

		if( LW_FAILED( shader_mgr->RegisterVertexShader( shader_type[i], path, 0, decl_tab[i], decl_size[i],1 )))
		{
			MessageBox(NULL,shader_file[i],"ERROR",0);

			return false;
		}
		if(i< 4)
		{
			_iVShaderNum++;
			_vecVShader.resize(_iVShaderNum);
			_vecVShader[_iVShaderNum - 1] = 0;
			shader_mgr->QueryVertexShader(&_vecVShader[_iVShaderNum - 1],shader_type[i]);
		}else
		{
			if(i==4)
				shader_mgr->QueryVertexShader(&_dwFontVS,shader_type[i]);
			if(i==5)
				shader_mgr->QueryVertexShader(&_dwShadeMapVS,shader_type[i]);
			if(i==6)
				shader_mgr->QueryVertexShader(&_dwMinimapVS,shader_type[i]);
		}

	} 
#endif

#if(defined LW_USE_DX9)

	// decl
	D3DVERTEXELEMENT9 ve_dec_effect1[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,  0}, // Include this line
		{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};

	D3DVERTEXELEMENT9 ve_dec_effect2[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,  0},
		{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};

	D3DVERTEXELEMENT9 ve_dec_font[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0},
	};

	D3DVERTEXELEMENT9 ve_dec_shade[] =
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		//D3DVSD_REG( D3DVSDE_BLENDWEIGHT,	D3DVSDT_FLOAT1),
		//D3DVSD_REG( D3DVSDE_NORMAL,			D3DVSDT_FLOAT3),
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
		"eff1.vsh",
		"eff2.vsh",
		"eff3.vsh",
		"eff4.vsh",
		"font.vsh",
		"shadeeff.vsh",
		"minimap.vsh",
	};

	D3DVERTEXELEMENT9* decl_tab[] =
	{
		ve_dec_effect1,
		ve_dec_effect2,
		ve_dec_effect1,
		ve_dec_effect1,
		ve_dec_font,
		ve_dec_shade,
		ve_dec_map,
	};

	DWORD decl_type[] =
	{
		VDT_EFF_134,
		VDT_EFF_2,
		VDT_EFF_134,
		VDT_EFF_134,
		VDT_EFF_FONT,
		VDT_EFF_SHADE,
		VDT_EFF_MINIMAP,
	};

    int decl_num = sizeof(decl_type) / sizeof(decl_type[0]);
	_vecVDecl.resize(decl_num);

    for(int i = 0; i < decl_num; i++)
    {
        IDirect3DVertexDeclarationX* this_decl;
		if (LW_SUCCEEDED(shader_mgr->QueryVertexDeclaration(&this_decl, decl_type[i])))
		{
			_vecVDecl[i] = this_decl;
			continue;
		}

        if(LW_FAILED(shader_mgr->RegisterVertexDeclaration(decl_type[i], decl_tab[i])))
            return false;
		shader_mgr->QueryVertexDeclaration(&this_decl, decl_type[i]);
		_vecVDecl[i] = this_decl;
    }

    for(int i = 0; i < decl_num; i++)
    {
        sprintf(path, "%s%s", path_info->GetPath(PATH_TYPE_SHADER), shader_file[i]);
        if(LW_FAILED(shader_mgr->RegisterVertexShader(shader_type[i], path, VS_FILE_ASM)))
            return false;

		if (i < 4) {
			_iVShaderNum++;
			_vecVShader.resize(_iVShaderNum);
			_vecVShader[_iVShaderNum - 1] = 0;

			shader_mgr->QueryVertexShader(&_vecVShader[_iVShaderNum - 1], shader_type[i]);
		}
		else {
			if (i == 4) {
				shader_mgr->QueryVertexShader(&_dwFontVS, shader_type[i]);
			}
			if (i == 5) {
				shader_mgr->QueryVertexShader(&_dwShadeMapVS, shader_type[i]);
			}
			if (i == 6) {
				shader_mgr->QueryVertexShader(&_dwMinimapVS, shader_type[i]);
			}
		}

    }

#endif

	_bMagr = true;
	return true;
}

//bool	CMPResManger::LoadTotalShadeMap()
//{
//	char t_Path[MAX_PATH];
//	WIN32_FIND_DATA t_sfd;
//	HANDLE  t_hFind = NULL;
//
//	lstrcpy(t_Path,_pszEFFectPath);
//	lstrcat(t_Path,"\\*.shade");
//
//	char t_pszFile[MAX_PATH];
//
//	if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
//		return false;
//	do{
//		if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//		{
//			sprintf(t_pszFile, "%s\\%s",_pszEFFectPath,t_sfd.cFileName);
//			_vecShadeName.push_back(t_sfd.cFileName);
//
//			_iShadeCnt++;
//			//_vecShade.resize(_iShadeCnt);
//		}
//
//	}while(FindNextFile(t_hFind,&t_sfd));
//	FindClose(t_hFind);
//	return true;
//}

int		CMPResManger::GetEffPathID(const s_string& pszName)
{
#if RESOURCE_SCRIPT == 1
	StrMapIter iter = _mapPath.find(pszName);
	if (iter == _mapPath.end())
		_mapPath.insert(pszName);
#endif

	for (size_t i(0); i<_vecPathName.size(); ++i)
	{
		if (stricmp(_vecPathName[i].c_str(), pszName.c_str()) == 0)
			// Success
			return (int)i;
	}

	//std::vector<s_string>::iterator it = find( _vecPathName.begin(), _vecPathName.end(), pszName );
	//if ( it != _vecPathName.end() )
	//{
	//	return (int)(it - _vecPathName.begin());
	//}
#if RESOURCE_SCRIPT == 2
	ToLogService("errors", LogLevel::Error, ": CMPResManger::GetEffPathID(),EffPathName={}",pszName.c_str());
#endif

	// Failure
	return -1;
}

CEffPath*	CMPResManger::GetEffPath(int iID)
{
	return &_vecPath[iID];
}

bool	CMPResManger::LoadTotalPath()
{
#if RESOURCE_SCRIPT == 0 || RESOURCE_SCRIPT == 1
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE  t_hFind = NULL;

		lstrcpy(t_Path,_pszEFFectPath);
		lstrcat(t_Path,"\\*.csf");

		char t_pszFile[MAX_PATH];

		if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
			return false;
		do{
			if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				sprintf(t_pszFile, "%s\\%s",_pszEFFectPath,t_sfd.cFileName);
				_vecPathName.push_back(t_sfd.cFileName);

				_iPathNum++;
				_vecPath.resize(_iPathNum);
				_vecPath[_iPathNum - 1].LoadPathFromFile(t_pszFile);
			}

		}while(FindNextFile(t_hFind,&t_sfd));
		FindClose(t_hFind);
	}
#else
	{
		MPResourceInfo* pResInfo(0);
		char szFile[MAX_PATH];

		for(int i(1); i<MPResourceSet::GetInstance().GetLastID() +1; i++)
		{
			pResInfo = MPResourceSet::GetInstance().GetResourceInfoByID(i);
			if (!pResInfo)
				continue;
			if (pResInfo->GetType() == MPResourceInfo::RT_PATH)
			{
				_iPathNum++;
				_vecPathName.push_back(pResInfo->szDataName);

				sprintf(szFile, "%s\\%s",_pszEFFectPath,pResInfo->szDataName);
				_vecPath.resize(_iPathNum);
				_vecPath[_iPathNum - 1].LoadPathFromFile(szFile);
			}
		}
	}
#endif
	return true;
}

bool	CMPResManger::LoadDefaultText(const char* pszFileName)
{
	FILE* pfile = fopen( pszFileName, "rt" );

	if(!pfile)
	{
		MessageBox(NULL,_psDefault,"ERROR",0);
		return false;
	}
	int idx = 0;
	int rval = fgetc(pfile);
	while( rval != EOF )
	{
		_psDefault[idx] = (char)rval;

		rval = fgetc(pfile);

		idx++;
		if(idx >= 418)
			break;
	}
	//
	fclose(pfile);
	return true;
}

int		CMPResManger::GetPartCtrlID(const s_string& pszName)
{
	// _strdup����malloc�����ڴ棬��ɾ����delete,������
	//pszName.lo

	//char *pszDataName = _strlwr( _strdup( pszName.c_str() ) );
	//s_string strName = pszDataName;

	//transform (pszName.begin(), pszName.end(), //source
	//		   pszName.begin(), //destination
	//			tolower);

#if RESOURCE_SCRIPT == 1
	StrMapIter iter = _mapParticle.find(pszName);
	if (iter == _mapParticle.end())
		_mapParticle.insert(pszName);
	ofstream outfile("ParticleSet.txt", ios_base::app);
	outfile << pszName << "\t0" << endl;
#endif

	for(size_t n(0); n <_vecPartName.size(); n++ )
	{
		if(stricmp(_vecPartName[n].c_str(), pszName.c_str()) == 0)
		{
			//SAFE_DELETE_ARRAY(pszDataName);
			return (int)n;
		}
	}
	//SAFE_DELETE_ARRAY(pszDataName);
#if RESOURCE_SCRIPT == 2
	ToLogService("errors", LogLevel::Error, ": CMPResManger::GetPartCtrlID(),PartCtrlName={}",pszName.c_str());
#endif
	return -1;
}

//#ifdef USE_GAME
//CMemoryBuf*		CMPResManger::GetPartCtrlByID(int iID)
//{
//	if(iID > MAXPART_COUNT)
//	{
//		LG("error", "msg��Ч����̫�࣬��lemon");
//		return NULL;
//	}
//	if(iID < 0)
//	{
//		LG("error","msg��ЧID[%d]",iID);
//		return NULL;
//	}
//	_vecPartCtrl[iID].mseek(0,SEEK_SET);
//	return &_vecPartCtrl[iID];
//}
//#else
CMPPartCtrl*	CMPResManger::GetPartCtrlByID(int iID)
{
	//if((*_vecPartCtrl[iID])->m_iPartNum<=0 && (*_vecPartCtrl[iID])->m_iStripNum <=0 &&  
	//	(*_vecPartCtrl[iID])->GetModelNum()<=0)
	if(iID > MAXPART_COUNT)
	{
		ToLogService("errors", LogLevel::Error, "��Ч����̫�࣬��lemon");
		return NULL;
	}
	if(iID < 0)
	{
		ToLogService("errors", LogLevel::Error, "��ЧID[{}]",iID);
		return NULL;
	}
	if((*_vecPartCtrl[iID]) == NULL)
	{
		char t_Path[MAX_PATH];
		sprintf(t_Path, "%s\\%s",_pszEFFectPath,_vecPartName[iID].c_str());

		(*_vecPartCtrl[iID]) = new CMPPartCtrl;
		if(!(*_vecPartCtrl[iID])->LoadFromFile(t_Path))
		{
			//SAFE_DELETE( (*_vecPartCtrl[iID]) ); 
			ToLogService("errors", LogLevel::Error, "Load {} error",_vecPartName[iID].c_str());
			return NULL;
		}
		else
		{
			const auto v = D3DXVECTOR3(0, 0, 0);
			(*_vecPartCtrl[iID])->MoveTo(&v);
		}
	}
	return (*_vecPartCtrl[iID]);
}
//#endif

void	CMPResManger::LoadTotalPartCtrl()
{
#if RESOURCE_SCRIPT == 0 || RESOURCE_SCRIPT == 1
	//�����ļ�Ŀ¼����
	{
		char t_Path[MAX_PATH];
		WIN32_FIND_DATA t_sfd;
		HANDLE  t_hFind = NULL;

		char t_FilePath[MAX_PATH];

		lstrcpy(t_Path,_pszEFFectPath);
		lstrcat(t_Path,"\\*.par");

		_vecPartCtrl.resize(MAXPART_COUNT);

		if((t_hFind=FindFirstFile(t_Path,&t_sfd))==INVALID_HANDLE_VALUE)
			return;
		string sFileName;
		do{
			if(!(t_sfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				_iPartCtrlNum++;

				sFileName = t_sfd.cFileName;
				transform(sFileName.begin(), sFileName.end(),
					sFileName.begin(),
					[](unsigned char c) { return std::tolower(c); });
				_vecPartName.push_back(sFileName.c_str());

				#ifdef USE_GAME
				sprintf(t_pszFile, "%s\\%s",_pszEFFectPath,t_sfd.cFileName);
				_vecPartCtrl[_iPartCtrlNum - 1].LoadFile(t_pszFile);
				#else
							_vecPartCtrl.setsize(_iPartCtrlNum);
				#endif
				sprintf(t_FilePath, "%s\\%s",_pszEFFectPath,_vecPartName[_iPartCtrlNum - 1].c_str());

				(*_vecPartCtrl[_iPartCtrlNum - 1]) = new CMPPartCtrl;
				if(!(*_vecPartCtrl[_iPartCtrlNum - 1])->LoadFromFile(t_FilePath))
				{
					SAFE_DELETE( (*_vecPartCtrl[_iPartCtrlNum - 1]) ); 
					ToLogService("errors", LogLevel::Error, "Load {} error",sFileName.c_str());
				}
			}

		}while(FindNextFile(t_hFind,&t_sfd));
		FindClose(t_hFind);
		//#ifdef USE_GAME
		//	_vecPartCtrl.resize(_iPartCtrlNum);
		//#endif
	}
#else
	//�ӽű��ļ��м���
	{
		MPResourceInfo* pResInfo(0);
		for(int i(1); i<MPResourceSet::GetInstance().GetLastID() +1; i++)
		{
			pResInfo = MPResourceSet::GetInstance().GetResourceInfoByID(i);
			if (!pResInfo)
				continue;
			if (pResInfo->GetType() == MPResourceInfo::RT_PAR)
			{
				_iPartCtrlNum++;
				_vecPartName.push_back(pResInfo->szDataName);
			}
		}
	}
#endif
}

CMPPartCtrl*	CMPResManger::NewPartCtrl(const s_string& strName)
{
//#ifndef USE_GAME
	_iPartCtrlNum++;
	if(_iPartCtrlNum >=MAXPART_COUNT)
	{
		_iPartCtrlNum--;
		return NULL;
	}
	_vecPartCtrl.setsize(_iPartCtrlNum);
	_vecPartName.push_back(strName);

	(*_vecPartCtrl[_iPartCtrlNum - 1]) = new CMPPartCtrl;

	return (*_vecPartCtrl[_iPartCtrlNum - 1]);
//#else
//	return NULL;
//#endif
}

void	CMPResManger::DeletePartCtrl(int iID)
{
//#ifndef USE_GAME
	//_iPartCtrlNum--;
	//if(_iPartCtrlNum <0)
	//{
	//	_iPartCtrlNum = 0;
	//	return;
	//}
	//std::vector<s_string>:: iterator it = find(_vecPartName.begin(),_vecPartName.end(),_vecPartName[iID]);
	//if(it != _vecPartName.end())
	//	_vecPartName.erase(it);

	//SAFE_DELETE((*_vecPartCtrl[iID]));
	//_vecPartCtrl.remove(iID);
//#endif
}

CEffectModel*	CMPResManger::NewTobMesh()
{
	CEffectModel* pModel = new CEffectModel;
	pModel->InitDevice(m_pDev,m_pSysGraphics->GetResourceMgr());
	_lstTobMeshs.push_back(pModel);
	_iTobMeshNum++;
	return pModel;
}
bool CMPResManger::DeleteTobMesh(CEffectModel& rEffectModel)
{
	std::list<CEffectModel*>::iterator iter = find(_lstTobMeshs.begin(), _lstTobMeshs.end(), &rEffectModel);
	if (iter != _lstTobMeshs.end())
	{
		delete &rEffectModel;
		_lstTobMeshs.erase(iter);
		return true;
	}
	return false;
}

BOOL CMPResManger::OnResetDevice()
{
#ifdef USE_RENDER

	if(!_CEffectFile.OnResetDevice())
		return FALSE;
	DWORD shader_type[] =
	{
		VSTU_EFFECT_E1,
			VSTU_EFFECT_E2,
			VSTU_EFFECT_E3,
			VSTU_EFFECT_E4,
			VSTU_FONT_E5,
			VSTU_SHADE_E6,
			VSTU_MINIMAP_E6,
	};
	
	DWORD decl_type[] =
	{
		VDT_EFF_134,
		VDT_EFF_2,
		VDT_EFF_134,
		VDT_EFF_134,
		VDT_EFF_FONT,
		VDT_EFF_SHADE,
		VDT_EFF_MINIMAP,
	};
	lwIShaderMgr* shader_mgr;
	lwIResourceMgr* res_mgr;

	m_pSysGraphics->GetInterface( (LW_VOID**)&res_mgr, LW_GUID_RESOURCEMGR );
	shader_mgr = res_mgr->GetShaderMgr();

	if(m_bUseSoft)
	{
		shader_mgr->QueryVertexShader(&_vecVShader[0],shader_type[1]);
	}else
	{
		for( int i = 0; i < 7; i++ )
		{
			if(i< 4)
			{
				shader_mgr->QueryVertexShader(&_vecVShader[i],shader_type[i]);
			}else
			{
				if(i==4)
					shader_mgr->QueryVertexShader(&_dwFontVS,shader_type[i]);
				if(i==5)
					shader_mgr->QueryVertexShader(&_dwShadeMapVS,shader_type[i]);
				if(i==6)
					shader_mgr->QueryVertexShader(&_dwMinimapVS,shader_type[i]);
			}
			shader_mgr->QueryVertexDeclaration(&_vecVDecl[i], decl_type[i]);
		} 
	}

	IDirect3DSurfaceX* pBackBuffer;
#ifdef USE_RENDER
	m_pDev->GetDevice()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
#else
	m_pDev->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
#endif
	pBackBuffer->GetDesc( &m_d3dBackBuffer );
	pBackBuffer->Release();

	D3DXMatrixOrthoLH(&_Mat2dViewProj, float(m_d3dBackBuffer.Width), float(m_d3dBackBuffer.Height), 0.0f, 1.0f);

	// ��ResetDevice��call back�����У�g_Render��GetScrWidth ��û����������
    // �������lwDeviceObject�Ľӿ�
	_iFontBkWidth = /*m_pDev->GetScrWidth()/2;//*/m_d3dBackBuffer.Width/2;
	_iFontBkHeight= /*m_pDev->GetScrHeight()/2;//*/m_d3dBackBuffer.Height/2;
    //RECT rc_client;
    //m_pDev->GetInterfaceMgr()->dev_obj->GetWindowRect(NULL, &rc_client);
    //_iFontBkWidth = (rc_client.right - rc_client.left) / 2;
    //_iFontBkHeight= (rc_client.bottom - rc_client.top) / 2;
	
	_vecMeshList[0]->CreateTriangle();
	_vecMeshList[1]->CreateRect();
	_vecMeshList[2]->CreatePlaneRect();
	_vecMeshList[3]->CreatePlaneTriangle();
	_vecMeshList[4]->CreateRectZ();
	_vecMeshList[5]->CreateCone(8, 3, 2);
	_vecMeshList[6]->CreateCylinder(8, 3, 1, 3);


#endif
	return TRUE;
}
BOOL CMPResManger::OnLostDevice()
{
#ifdef USE_RENDER
	if(!_CEffectFile.OnLostDevice())
		return FALSE;
	
#endif
	return TRUE;
}

LW_RESULT	g_OnLostDevice()
{
//#ifdef USE_RENDER
	return ResMgr.OnLostDevice();
//#endif

}
LW_RESULT	g_OnResetDevice()
{
//#ifdef USE_RENDER
	return ResMgr.OnResetDevice();
//#endif
}


void CMPResManger::RestoreEffect()
{
#ifdef USE_RENDER
	m_pDev->SetRenderStateForced(D3DRS_ZENABLE, TRUE);
	m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, TRUE);
	m_pDev->SetRenderStateForced(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDev->SetRenderStateForced(D3DRS_ALPHATESTENABLE, FALSE);
	m_pDev->SetRenderStateForced(D3DRS_DITHERENABLE,FALSE);
	m_pDev->SetRenderStateForced(D3DRS_CULLMODE, D3DCULL_CCW); // ???????
	m_pDev->SetRenderStateForced(D3DRS_SRCBLEND,D3DBLEND_ONE);
	m_pDev->SetRenderStateForced(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	m_pDev->SetRenderStateForced(D3DRS_LIGHTING, TRUE);
	m_pDev->SetRenderStateForced(D3DRS_CLIPPING, TRUE);



    m_pDev->GetInterfaceMgr()->dev_obj->SetTextureForced(0, 0);
    m_pDev->GetInterfaceMgr()->dev_obj->SetTextureForced(1, 0);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pDev->SetTextureStageStateForced(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);  
	m_pDev->SetSamplerStateForced(0, D3DSAMP_ADDRESSU,   D3DTADDRESS_WRAP);  
	m_pDev->SetSamplerStateForced(0, D3DSAMP_ADDRESSV,   D3DTADDRESS_WRAP);
	m_pDev->SetSamplerStateForced(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDev->SetSamplerStateForced(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDev->SetTextureStageStateForced(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    m_pDev->SetTextureStageStateForced(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pDev->SetTextureStageStateForced(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
    m_pDev->SetTextureStageStateForced(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    m_pDev->SetTextureStageStateForced(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pDev->SetTextureStageStateForced(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

#endif
}
//-----------------------------------------------------------------------------
void CMPResManger::FrameMove(DWORD dwTime)
{
	m_iCurFrame += 1;
	if(m_iCurFrame > 1)
		return;


	_fCurTime = (float)GetTickCount()/1000;
	if(!_bInitTime)
	{
		_fSaveTime = _fCurTime;
		_bInitTime = true;
	}
	_fDailTime = _fCurTime - _fSaveTime;
	_fSaveTime = _fCurTime;

	D3DXMatrixInverse( &_MatBBoard, NULL, _pMatView );
	_MatBBoard._41 = 0.0f;
	_MatBBoard._42 = 0.0f;
	_MatBBoard._43 = 0.0f;

	D3DXMatrixTranspose(&_MatViewProjPose, _pMatViewProj);
	//Transpose(_MatViewProjPose,*_pMatViewProj);
	if(_vecValidID.size() >= MAXMSG_COUNT)
		return;

	for(WORD iw = 0; iw < MAXMSG_COUNT; ++iw)
	{
		if(_vecPartArray[iw])
		{
			_vecPartArray[iw]->FrameMove(dwTime);
		}
	}
}
//-----------------------------------------------------------------------------
void CMPResManger::Render()
{
	if(m_iCurFrame < 1)
		return;
	m_iCurFrame = 0;
	if(_vecValidID.size() >= MAXMSG_COUNT)
		return;

	for(WORD iw = 0; iw < MAXMSG_COUNT; ++iw)
	{
		if(_vecPartArray[iw])
		{
			if(!_vecPartArray[iw]->IsPlaying())
			{
				SAFE_DELETE(_vecPartArray[iw]);
				_vecValidID.push_front(iw);
				continue;
			}
			_vecPartArray[iw]->Render();
		}
	}
}

