#include "StdAfx.h"
#include "GlobalInc.h"
#include "MPModelEff.h"

#include "MPMap.h"

#include ".\mpshademap.h"
#include "MPRender.h"

#define	 TILESIZE	1.0f


#define CLAMP(A, MIN, MAX)		( (A) > (MAX) ) ? (MAX) :  ( ( (A) < (MIN) ) ? (MIN) : (A) )

extern CMPResManger ResMgr;

CMPShadeMap::CMPShadeMap(void) {
	m_iType = SHADE_SINGLE;


	_SVerPos = D3DXVECTOR3(0.0f, 0.0f, 0);

	_dwColor = 0x80ffffff;
	_pfDailTime = NULL;
	_bShow = true;

	_iIdxTech = 2;
	_pCEffectFile = NULL;

	_eSrcBlend = D3DBLEND_SRCALPHA;
	_eDestBlend = D3DBLEND_INVSRCALPHA;

	_bUpdate = true;

	_UpSea = false;

	_pModel = NULL;
}

CMPShadeMap::~CMPShadeMap(void) {
	SAFE_DELETE(_pModel);
}

void CMPShadeMap::setTextureName(s_string& strName) {
	if ((strstr(strName.c_str(), ".dds") == NULL) && strstr(strName.c_str(), ".tga") == NULL) {
		_strTexName = strName;
	}
	else {
		_strTexName = strName.substr(0, strName.length() - 4);
	}
}


void CMPShadeMap::BoundingRes(CMPResManger* m_CResMagr) {
	int t_iID;

	t_iID = m_CResMagr->GetTextureID(_strTexName);
	if (t_iID == -1) {
		MessageBox(NULL, _strTexName.c_str(), "shade",MB_OK);
		_lpCurTex = NULL;
	}
	else {
		_lpCurTex = m_CResMagr->GetTextureByIDlw(t_iID);
	}


	if (!_pModel) {
		_pModel = new CEffectModel;
		_pModel->InitDevice(m_CResMagr->m_pDev, m_CResMagr->m_pSysGraphics->GetResourceMgr());
	}


	_pfDailTime = m_CResMagr->GetDailTime();

	_pCEffectFile = m_CResMagr->GetEffectFile();

	_pMatViewProj = m_CResMagr->GetViewProjMat();

	m_bUseSoft = m_CResMagr->m_bUseSoftOrg;


	m_dwVsConst = m_CResMagr->GetDevCap()->MaxVertexShaderConst;

	_UpSea = false;
}

bool CMPShadeMap::CreateShadeMap(float fRadius) {
	_fRadius = fRadius;
	_fGridMax = _fRadius / TILESIZE + 1;
	_iGridCrossNum = (int)_fGridMax;
	if (_fGridMax - (float)_iGridCrossNum > 0)
		_iGridCrossNum++;

	if (!SetGridNum(_iGridCrossNum))
		return false;

	//VB
	for (int n = 0; n < _iVerNum; n++) {
		_SShadePos[n] = D3DXVECTOR3(0, 0, 0);
		_SShadeUV[n] = D3DXVECTOR2(0, 0);
	}

	return true;
}

bool CMPShadeMap::SetGridNum(int iNum) {
	_iGridCrossNum = iNum; /////77777
	_iGridNum = _iGridCrossNum * _iGridCrossNum;

	_iFaceCount = _iGridNum * 2;

	_iVerNum = (_iGridCrossNum + 1) * (_iGridCrossNum + 1);
	_iIndexNum = _iGridCrossNum * _iGridCrossNum * 6;

	if (DWORD(_iVerNum * 2 + 8) > m_dwVsConst)
		m_bUseSoft = TRUE;


	//!IB


	_pModel->CreateShadeModel(_iVerNum, _iFaceCount, _iGridCrossNum, m_bUseSoft);


	return true;
}

void CMPShadeMap::setFrameTexture(s_string& strTexName, CMPResManger* pCResMagr) {
	_strTexName = strTexName;
	int t_iID;

	t_iID = pCResMagr->GetTextureID(_strTexName);
	if (t_iID == -1) {
		MessageBox(NULL, _strTexName.c_str(), "",MB_OK);
		_lpCurTex = NULL;
	}
	else {
		_lpCurTex = pCResMagr->GetTextureByIDlw(t_iID);
	}
}


void CMPShadeMap::FrameMove(DWORD dwDailTime) {
	static float dwTime = 1.5f;
	_bUpdate = false;
	dwTime += *_pfDailTime;
	if (dwTime > 1.5f) {
		dwTime = 0;
		_bUpdate = true;
	}
}

void CMPShadeMap::MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float fAngle) {
	if (!_bShow)
		return;
	if (!_bUpdate) {
		if (fEquat(_SVerPos.x, SVerPos.x) && fEquat(_SVerPos.y, SVerPos.y)) {
			return;
		}
	}
	_bUpdate = false;

	_SVerPos = SVerPos;


	//TILE
	int nX = (int)((_SVerPos.x - _fRadius / 2));
	int nY = (int)((_SVerPos.y - _fRadius / 2));

	for (int y = 0; y < _iGridCrossNum + 1; y++) {
		for (int x = 0; x < _iGridCrossNum + 1; x++) {
			int iIndex = x + y * (_iGridCrossNum + 1);

			float fGridX = (float)(nX + x * TILESIZE);
			float fGridY = (float)(nY + y * TILESIZE);
			_SShadePos[iIndex].x = fGridX;
			_SShadePos[iIndex].y = fGridY;
			if (pMap) {
				float objHeight, height;
				{
					int iGridx = (int)(fGridX * 2);
					int iGridy = (int)(fGridY * 2);
					if (y == _iGridCrossNum)
						iGridy -= 1;
					if (x == _iGridCrossNum)
						iGridx -= 1;
					height = pMap->GetGridHeight(iGridx, iGridy);
					objHeight = pMap->GetTileHeight((int)(fGridX), (int)(fGridY));

					if (_UpSea) {
						if (objHeight <= 0)
							objHeight = 0;
					}
					if (height > objHeight && !pMap->IsGridBlock(iGridx, iGridy))
						objHeight = height;
				}

				_SShadePos[iIndex].z = objHeight + 0.015f;
			}
			else
				_SShadePos[iIndex].z = SVerPos.z + 0.015f;

			float fU, fV;

			{
				fU = (fGridX - _SVerPos.x) / (_fGridMax * TILESIZE);
				fV = (fGridY - _SVerPos.y) / (_fGridMax * TILESIZE);
				_SShadeUV[iIndex].x = CLAMP(fU + 0.5f, 0.0f, 1.0f);
				_SShadeUV[iIndex].y = CLAMP(fV + 0.5f, 0.0f, 1.0f);
			}
			if (fAngle != 0) {
				D3DXMATRIX mat;

				D3DXVECTOR3 vpos(0.5f, 0.5f, 0);
				auto v = D3DXVECTOR3(0, 0, 1);
				GetMatrixRotation(&mat, &vpos, &v, fAngle);

				D3DXVec2TransformCoord(&_SShadeUV[iIndex], &_SShadeUV[iIndex], &mat);
			}
		}
	}
	if (m_bUseSoft)
		FillVertex();
}

void CMPShadeMap::RenderVS() {
	if (!_bShow)
		return;
	D3DXMATRIX t_mat;
	D3DXMatrixIdentity(&t_mat);
	if (_lpCurTex && _lpCurTex->IsLoadingOK()) {
		_pModel->m_pDev->SetTexture(0, _lpCurTex->GetTex());
	}
	else {
		_pCEffectFile->End();
		return;
	}

	_pModel->Begin();

	if (!_pCEffectFile->SetTechnique(_iIdxTech)) {
		return;
	}

	_pCEffectFile->Begin(D3DXFX_DONOTSAVESHADERSTATE); //D3DXFX_DONOTSAVESTATE
	_pCEffectFile->Pass(0);
	//to fix overlay of models
	_pCEffectFile->m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	_pModel->m_pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

	_pModel->m_pDev->SetRenderStateForced(D3DRS_SRCBLEND, _eSrcBlend);
	_pModel->m_pDev->SetRenderStateForced(D3DRS_DESTBLEND, _eDestBlend);

	_pModel->m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_pModel->m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	_pModel->m_pDev->SetVertexShader(ResMgr.GetShadeVS());
	_pModel->m_pDev->SetVertexDeclaration(ResMgr.GetShadeVDecl());

	_pModel->m_pDev->SetVertexShaderConstantF(0, t_mat, 4);
	_pModel->m_pDev->SetVertexShaderConstantF(4, *_pMatViewProj, 4);
	_pModel->m_pDev->SetVertexShaderConstantF(8, _dwColor, 1);

	D3DXVECTOR4 tv(0, 0, 0, 0);
	int nIndex = 9;
	for (int n = 0; n < _iVerNum; n++) {
		tv.x = _SShadeUV[n].x;
		tv.y = _SShadeUV[n].y;
		_pModel->m_pDev->SetVertexShaderConstantF(nIndex++, tv, 1);

		tv.x = _SShadePos[n].x;
		tv.y = _SShadePos[n].y;
		tv.z = _SShadePos[n].z;
		tv.w = 1;
		_pModel->m_pDev->SetVertexShaderConstantF(nIndex++, tv, 1);
	}

	_pModel->RenderModel();
	_pModel->End();

	_pCEffectFile->End();
}

void CMPShadeMap::FillVertex() {
	SEFFECT_SHADE_VERTEX* pVertex(0);
	if (!_pModel)
		return;

	_pModel->SetRenderNum(_iVerNum, _iFaceCount);

	_pModel->Lock((BYTE**)&pVertex);
	if (pVertex == 0) {
		MessageBox(NULL, "msgLockFailed lock error msglock error CMPShadeMap::FillVertex() line 552", "error", 0);
		return;
	}

	int nIndex = 9; //!VS9
	for (int n = 0; n < _iVerNum; n++) {
		pVertex[n].m_dwDiffuse = _dwColor;
		pVertex[n].m_SPos.x = _SShadePos[n].x;
		pVertex[n].m_SPos.y = _SShadePos[n].y;
		pVertex[n].m_SPos.z = _SShadePos[n].z;

		pVertex[n].m_SUV.x = _SShadeUV[n].x;
		pVertex[n].m_SUV.y = _SShadeUV[n].y;

		pVertex[n].m_SUV2.x = (float)nIndex;
		nIndex++;
		pVertex[n].m_SUV2.y = (float)nIndex;
		nIndex++;
	}
	_pModel->Unlock();
}

void CMPShadeMap::RenderSoft() {
	if (!_bShow)
		return;


	_pModel->Begin();
	_pCEffectFile->m_pDev->SetRenderState(D3DRS_TEXTUREFACTOR, _dwColor);
	_pCEffectFile->m_pDev->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	_pCEffectFile->m_pDev->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	_pCEffectFile->m_pDev->SetRenderStateForced(D3DRS_ALPHABLENDENABLE, TRUE);
	_pCEffectFile->m_pDev->SetRenderStateForced(D3DRS_ZWRITEENABLE, FALSE);
	_pCEffectFile->m_pDev->SetRenderStateForced(D3DRS_CULLMODE, D3DCULL_CCW);
	if (_iIdxTech != 4) {
		if (!_pCEffectFile->SetTechnique(_iIdxTech))
			return;
		_pCEffectFile->Begin(D3DXFX_DONOTSAVESTATE);
		_pCEffectFile->Pass(0);
	}

	_pModel->GetDev()->SetRenderStateForced(D3DRS_SRCBLEND, _eSrcBlend);
	_pModel->GetDev()->SetRenderStateForced(D3DRS_DESTBLEND, _eDestBlend);

	D3DXMATRIX t_mat;
	D3DXMatrixIdentity(&t_mat);

	if (_lpCurTex && _lpCurTex->IsLoadingOK())
		_pModel->m_pDev->SetTexture(0, _lpCurTex->GetTex());
	else {
		if (_iIdxTech != 4) {
			_pCEffectFile->End();
		}
		return;
	}
	//to fix models get front of each other when render wings/pets
	if (_iIdxTech != 4) {
		_pModel->GetDev()->SetTransformWorld(&t_mat);
	}

	_pModel->RenderModel();
	_pModel->End();
	_pCEffectFile->GetDev()->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	_pCEffectFile->GetDev()->SetTextureStageStateForced(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	_pCEffectFile->GetDev()->SetTextureStageStateForced(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	_pModel->m_pDev->SetVertexShader(nullptr);
	_pModel->m_pDev->SetFVF(EFFECT_SHADE_FVF);
	//this part not 100% sure of it yet need testing
	if (_iIdxTech != 4) {
		_pCEffectFile->End();
	}
}

void CMPShadeMap::Render() {
	if (!m_bUseSoft)
		RenderVS();
	else
		RenderSoft();
}

bool CMPShadeMap::SaveToFile(FILE* pFile) {
	int t_temp;
	//!
	fwrite(&_fRadius, sizeof(float), 1, pFile);

	// Имя текстуры — фиксированный 32-байтный буфер на диске.
	{
		char t_pszName[32]{};
		std::memcpy(t_pszName, _strTexName.data(),
					std::min<std::size_t>(_strTexName.size(), sizeof(t_pszName) - 1));
		fwrite(t_pszName, sizeof(char), 32, pFile);
	}

	//!
	t_temp = (int)_eSrcBlend;
	fwrite(&t_temp, sizeof(int), 1, pFile);
	t_temp = (int)_eDestBlend;
	fwrite(&t_temp, sizeof(int), 1, pFile);


	fwrite(&_dwColor, sizeof(D3DXCOLOR), 1, pFile);
	fwrite(&_iIdxTech, sizeof(int), 1, pFile);

	return true;
}

bool CMPShadeMap::LoadFromFile(FILE* pFile) {
	int t_temp;
	////!


	//!
	fread(&t_temp, sizeof(int), 1, pFile);
	_eSrcBlend = (D3DBLEND)t_temp;
	fread(&t_temp, sizeof(int), 1, pFile);
	_eDestBlend = (D3DBLEND)t_temp;

	fread(&_dwColor, sizeof(D3DXCOLOR), 1, pFile);
	fread(&_iIdxTech, sizeof(int), 1, pFile);
	return true;
}

CMPShadeEX::CMPShadeEX(int iFrameCount) {
	m_iType = SHADE_ANI;

	_iFrameCount = iFrameCount;
	_iCurFrame = 0;
	_fCurTime = 0;
	_vecFrameColor.resize(_iFrameCount);
	for (int n = 0; n < _iFrameCount; n++) {
		_vecFrameColor[n] = 0xffffffff;
	}
	_fFrameTime = 0.1f;
	_bLoop = false;
	_bShow = false;

	_iCurTex = 0;
	_vecTexOffset.clear();
	_iRow = 1;
	_iCol = 1;
	_fTexFrameTime = 0.1f;
	_fTexCurTime = 0;

	_iNumTex = 1;
	_vecTexOffset.resize(_iNumTex);
	for (int n = 0; n < _iNumTex; n++) {
		_vecTexOffset[n] = D3DXVECTOR2(0, 0);
	}
	_vecTexSave.clear();
}

CMPShadeEX::~CMPShadeEX(void) {
}

bool CMPShadeEX::CreateSpliteTexture(int iRow, int iColnum) {
	_iRow = iRow;
	_iCol = iColnum;

	_iNumTex = iRow * iColnum;

	float fsizew = 1.0f / iRow;
	float fsizeh = 1.0f / iColnum;

	_vecTexOffset.resize(_iNumTex);
	for (int h = 0; h < iColnum; h++) {
		for (int w = 0; w < iRow; w++) {
			int idx = w + h * iRow;
			_vecTexOffset[idx].x = w * fsizew;
			_vecTexOffset[idx].y = h * fsizeh;
		}
	}
	_vecTexSave.resize(_iVerNum);

	return true;
}

void CMPShadeEX::setFrameCount(int iCount) {
	_iFrameCount = iCount;
}

void CMPShadeEX::setFrameTime(float fTime) {
	_fFrameTime = fTime;
}

void CMPShadeEX::setTexFrameTime(float fTime) {
	_fTexFrameTime = fTime;
}

void CMPShadeEX::setFrameColor(int iIdx, D3DCOLOR SColor) {
	_vecFrameColor[iIdx] = SColor;
}

void CMPShadeEX::setColor(D3DXCOLOR SColor) {
	for (int n = 0; n < _iFrameCount; n++) {
		_vecFrameColor[n] = SColor;
	}
}


void CMPShadeEX::FrameMove(DWORD dwDailTime) {
	if (!_bShow)
		return;

	int iNextFrame;
	_fCurTime += *_pfDailTime;
	if (_fCurTime >= _fFrameTime) {
		_iCurFrame++;
		if (_iCurFrame >= _iFrameCount) {
			if (_bLoop)
				_iCurFrame = 0;
			else {
				_bLoop = false;
				_bShow = false;
				return;
			}
		}
		_fCurTime = 0;
	}
	if (_iCurFrame == _iFrameCount - 1)
		iNextFrame = _iCurFrame;
	else
		iNextFrame = _iCurFrame + 1;
	_fLerp = _fCurTime / _fFrameTime;
	D3DXColorLerp(&_dwColor, &_vecFrameColor[_iCurFrame], &_vecFrameColor[iNextFrame], _fLerp);

	_fTexCurTime += *_pfDailTime;
	if (_fTexCurTime >= _fTexFrameTime) {
		_iCurTex++;
		if (_iCurTex == _iNumTex) {
			_iCurTex = 0;
		}
		_fTexCurTime = 0;
	}
}

void CMPShadeEX::MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float fAngle) {
	if (!_bShow)
		return;
	if (_SVerPos == SVerPos) {
		if (_iNumTex > 1) {
			for (int n = 0; n < _iVerNum; n++) {
				_SShadeUV[n].x = _vecTexSave[n].x / _iRow;
				_SShadeUV[n].y = _vecTexSave[n].y / _iCol;
				_SShadeUV[n].x += _vecTexOffset[_iCurTex].x;
				_SShadeUV[n].y += _vecTexOffset[_iCurTex].y;
			}
		}
		return;
	}
	_SVerPos = SVerPos;
	//TILE
	int nX = (int)((_SVerPos.x - _fRadius / 2) /*/ TILESIZE*/);
	int nY = (int)((_SVerPos.y - _fRadius / 2) /*/ TILESIZE*/);

	for (int y = 0; y < _iGridCrossNum + 1; y++) {
		for (int x = 0; x < _iGridCrossNum + 1; x++) {
			int iIndex = x + y * (_iGridCrossNum + 1);

			float fGridX = (float)(nX + x * TILESIZE) /*/ TILESIZE*/;
			float fGridY = (float)(nY + y * TILESIZE) /*/ TILESIZE*/;
			_SShadePos[iIndex].x = fGridX;
			_SShadePos[iIndex].y = fGridY;
			if (pMap) {
				float height, objheight;
				int iGridx = (int)(fGridX * 2);
				int iGridy = (int)(fGridY * 2);
				if (y == _iGridCrossNum)
					iGridy -= 1;
				if (x == _iGridCrossNum)
					iGridx -= 1;
				objheight = pMap->GetGridHeight(iGridx, iGridy);
				height = pMap->GetTileHeight((int)(fGridX), (int)(fGridY));

				if (_UpSea) {
					if (height <= 0)
						height = 0;
				}
				if (objheight > height)
					height = objheight;

				_SShadePos[iIndex].z = height + 0.01f;
			}
			else
				_SShadePos[iIndex].z = SVerPos.z + 0.01f;

			float fU, fV;
			{
				fU = (fGridX - _SVerPos.x) / (_fGridMax * TILESIZE);
				fV = (fGridY - _SVerPos.y) / (_fGridMax * TILESIZE);
				_SShadeUV[iIndex].x = CLAMP(fU + 0.5f, 0.0f, 1.0f);
				_SShadeUV[iIndex].y = CLAMP(fV + 0.5f, 0.0f, 1.0f);
			}
			if (fAngle != 0) {
				D3DXMATRIX mat;

				D3DXVECTOR3 vpos(0.5f, 0.5f, 0);
				const auto v = D3DXVECTOR3(0, 0, 1);
				GetMatrixRotation(&mat, &vpos, &v, fAngle);

				D3DXVec2TransformCoord(&_SShadeUV[iIndex], &_SShadeUV[iIndex], &mat);
			}
		}
	}
	if (_iNumTex > 1) {
		for (int n = 0; n < _iVerNum; n++) {
			_vecTexSave[n] = _SShadeUV[n];
			_SShadeUV[n].x /= _iRow;
			_SShadeUV[n].y /= _iCol;
			_SShadeUV[n] += _vecTexOffset[_iCurTex];
		}
	}
}

bool CMPShadeEX::SaveToFile(FILE* pFile) {
	int t_temp;
	//!
	fwrite(&_fRadius, sizeof(float), 1, pFile);

	// Имя текстуры — фиксированный 32-байтный буфер на диске.
	{
		char t_pszName[32]{};
		std::memcpy(t_pszName, _strTexName.data(),
					std::min<std::size_t>(_strTexName.size(), sizeof(t_pszName) - 1));
		fwrite(t_pszName, sizeof(char), 32, pFile);
	}

	fwrite(&_iRow, sizeof(int), 1, pFile);
	fwrite(&_iCol, sizeof(int), 1, pFile);

	//!
	t_temp = (int)_eSrcBlend;
	fwrite(&t_temp, sizeof(int), 1, pFile);
	t_temp = (int)_eDestBlend;
	fwrite(&t_temp, sizeof(int), 1, pFile);


	//!
	//!
	fwrite(&_iFrameCount, sizeof(int), 1, pFile);
	//!
	fwrite(&_fFrameTime, sizeof(float), 1, pFile);
	//!
	for (int n = 0; n < _iFrameCount; n++) {
		fwrite(&_vecFrameColor[n], sizeof(D3DXCOLOR), 1, pFile);
	}
	///////////////!
	//!
	fwrite(&_fTexFrameTime, sizeof(float), 1, pFile);

	fwrite(&_iIdxTech, sizeof(int), 1, pFile);

	return true;
}

bool CMPShadeEX::LoadFromFile(FILE* pFile) {
	int t_temp;
	////!


	//!
	fread(&t_temp, sizeof(int), 1, pFile);
	_eSrcBlend = (D3DBLEND)t_temp;
	fread(&t_temp, sizeof(int), 1, pFile);
	_eDestBlend = (D3DBLEND)t_temp;

	//!
	fread(&_iFrameCount, sizeof(int), 1, pFile);
	//!
	fread(&_fFrameTime, sizeof(float), 1, pFile);

	//!
	_vecFrameColor.resize(_iFrameCount);
	for (int n = 0; n < _iFrameCount; n++) {
		fread(&_vecFrameColor[n], sizeof(D3DXCOLOR), 1, pFile);
	}
	///////////////!
	//!
	fread(&_fTexFrameTime, sizeof(float), 1, pFile);

	fread(&_iIdxTech, sizeof(int), 1, pFile);

	return true;
}

CMPShadeCtrl::CMPShadeCtrl(void) {
	_pShadeMap = NULL;
}

CMPShadeCtrl::~CMPShadeCtrl(void) {
	SAFE_DELETE(_pShadeMap);
}

bool CMPShadeCtrl::Create(s_string& strTexName, CMPResManger* pCResMagr, float fSize,
						  bool bAni, int iRow, int iColnum) {
	SAFE_DELETE(_pShadeMap);
	if (!bAni)
		_pShadeMap = new CMPShadeMap;
	else
		_pShadeMap = new CMPShadeEX;

	_pShadeMap->setTextureName(strTexName);
	_pShadeMap->BoundingRes(pCResMagr);

	_pShadeMap->CreateShadeMap(fSize);
	if (bAni)
		((CMPShadeEX*)_pShadeMap)->CreateSpliteTexture(iRow, iColnum);

	return true;
}

void CMPShadeCtrl::SetAlphaType(D3DBLEND eSrcBlend, D3DBLEND eDestBlend) {
	_pShadeMap->SetAlphaType(eSrcBlend, eDestBlend);
}

void CMPShadeCtrl::setFrameCount(int iCount) {
	if (_pShadeMap->m_iType == SHADE_ANI)
		((CMPShadeEX*)_pShadeMap)->setFrameCount(iCount);
}

void CMPShadeCtrl::setFrameTime(float fTime) {
	if (_pShadeMap->m_iType == SHADE_ANI)
		((CMPShadeEX*)_pShadeMap)->setFrameTime(fTime);
}

void CMPShadeCtrl::setTexFrameTime(float fTime) {
	if (_pShadeMap->m_iType == SHADE_ANI)
		((CMPShadeEX*)_pShadeMap)->setTexFrameTime(fTime);
}

void CMPShadeCtrl::setFrameColor(int iIdx, D3DCOLOR SColor) {
	if (_pShadeMap->m_iType == SHADE_ANI)
		((CMPShadeEX*)_pShadeMap)->setFrameColor(iIdx, SColor);
}

void CMPShadeCtrl::setColor(D3DXCOLOR SColor) {
	if (_pShadeMap->m_iType == SHADE_SINGLE)
		_pShadeMap->SetColor(SColor);
	else
		((CMPShadeEX*)_pShadeMap)->SetColor(SColor);
}

int CMPShadeCtrl::getFrameCount() {
	if (_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->getFrameCount();
	return 0;
}

float CMPShadeCtrl::getFrameTime() {
	if (_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->getFrameTime();
	return 0;
}

float CMPShadeCtrl::getTexFrameTime() {
	if (_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->getTexFrameTime();
	return 0;
}

void CMPShadeCtrl::getFrameColor(int iIdx, D3DCOLOR* pSColor) {
	if (_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->getFrameColor(iIdx, pSColor);
}

void CMPShadeCtrl::getColor(D3DXCOLOR* pSColor) {
	_pShadeMap->getColor(pSColor);
}

void CMPShadeCtrl::setFrameTexture(s_string& strTexName, CMPResManger* pCResMagr) {
	_pShadeMap->setFrameTexture(strTexName, pCResMagr);
}

void CMPShadeCtrl::FrameMove(DWORD dwDailTime) {
	_pShadeMap->FrameMove(dwDailTime);
}

void CMPShadeCtrl::MoveTo(D3DXVECTOR3 SVerPos, MPMap* pMap, float fAngle) {
	_pShadeMap->MoveTo(SVerPos, pMap, -fAngle);
}

void CMPShadeCtrl::Render() {
	_pShadeMap->Render();
}

bool CMPShadeCtrl::SaveToFile(char* pchName) {
	FILE* pFile;
	pFile = fopen(pchName, "wb");
	if (!pFile)
		return false;
	fwrite(&_pShadeMap->m_iType, sizeof(int), 1, pFile);


	if (_pShadeMap->m_iType == SHADE_ANI)
		return ((CMPShadeEX*)_pShadeMap)->SaveToFile(pFile);
	else
		_pShadeMap->SaveToFile(pFile);

	fclose(pFile);
	return true;
}

bool CMPShadeCtrl::LoadFromFile(char* pchName) {
	return true;
}
