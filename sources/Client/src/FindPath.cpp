#include "StdAfx.h"
#include "FindPath.h"
#include "EffectObj.h"
#include "Character.h"
#include "CharacterRecord.h"
#include "TerrainAttrib.h"
#include "CommFunc.h"
#include "PacketCmd.h"
#include "Scene.h"
#include "WorldScene.h"
#include "gameapp.h"
#include "UIMinimapForm.h"
#include "UIBoxForm.h"
#include "STMove.h"

struct NODE_SHARED //
{
	BYTE* buf_ptr;
	BYTE dire;
	NODE_SHARED* last_node_ptr;
};

struct PATH_LINK {
	BYTE dire;
	PATH_LINK* next_path_ptr;
};

static void DeletePathLink(PATH_LINK** ppPathLink) {
	PATH_LINK* pPathLink = *ppPathLink;
	while (pPathLink != NULL) {
		PATH_LINK* pNextPath = pPathLink->next_path_ptr;
		delete pPathLink;
		pPathLink = pNextPath;
	}
	*ppPathLink = NULL;
}

PATH_LINK* SearchPath(BYTE* block_buf, short width, short height, short sx, short sy, short tx, short ty,
					  long STEP_LIMIT) {
	NODE_SHARED* node_list = new NODE_SHARED[STEP_LIMIT]; //jze
	PATH_LINK* path_link = new PATH_LINK[STEP_LIMIT];

	memset(node_list, 0, sizeof(NODE_SHARED) * STEP_LIMIT);
	memset(path_link, 0, sizeof(PATH_LINK) * STEP_LIMIT);

	if (block_buf == NULL) {
		delete []node_list;
		delete []path_link;
		return NULL;
	}
	short off[8];
	{
		off[0] = width * (-1);
		off[1] = width;
		off[2] = -1;
		off[3] = 1;
		off[4] = (width - 1) * (-1);
		off[5] = width + 1;
		off[6] = width - 1;
		off[7] = (width + 1) * -1;
	}

	BYTE prior[8];
	prior[0] = 0;
	prior[1] = 4;
	prior[2] = 6;
	prior[3] = 2;
	prior[4] = 1;
	prior[5] = 3;
	prior[6] = 5;
	prior[7] = 7;

	UINT32 node_count = 0;
	UINT32 current_node = 0;
	UINT32 current_path = 0;

	BYTE const* target_ptr = &block_buf[ty * width + tx];

	node_list[0].buf_ptr = &block_buf[sy * width + sx];

	node_list[0].last_node_ptr = NULL;

	BYTE *last_buf_ptr, *new_buf_ptr;

	PATH_LINK* path_link_ptr = NULL;
	BOOL found_flag = FALSE;

	UINT32 lMaxStep = (height - 1) * (height - 1);
	if ((long)lMaxStep > STEP_LIMIT) lMaxStep = STEP_LIMIT; //

	BYTE const* end_block = &block_buf[STEP_LIMIT - 1];

	while (!found_flag) {
		if (node_count >= lMaxStep)
			break;

		if (current_node > node_count)
			break;

		last_buf_ptr = node_list[current_node].buf_ptr;
		for (register BYTE d = 0; d < 8; d++) {
			new_buf_ptr = last_buf_ptr + off[d];
			if (new_buf_ptr < block_buf || new_buf_ptr > end_block) {
				goto skip;
			}

			if (*new_buf_ptr == 0) {
				*new_buf_ptr = 1;
				node_count++;
				node_list[node_count].buf_ptr = new_buf_ptr;
				node_list[node_count].dire = d;
				node_list[node_count].last_node_ptr = &node_list[current_node];

				if (new_buf_ptr == target_ptr) {
					NODE_SHARED* node_ptr = &node_list[node_count];
					while (node_ptr->last_node_ptr) {
						path_link[current_path].dire = prior[node_ptr->dire];
						path_link[current_path].next_path_ptr = path_link_ptr;
						path_link_ptr = &path_link[current_path];
						node_ptr = node_ptr->last_node_ptr;

						current_path++;
					}
					found_flag = TRUE;
					break;
				}

				if (node_count >= lMaxStep) {
					if (found_flag == TRUE) {
						found_flag = FALSE;
					}
					goto skip;
				}
			}
		}
		current_node++;
	}
skip:

	if (found_flag) {
		PATH_LINK* path_return = new PATH_LINK[current_path];
		PATH_LINK* p = path_return;

		while (path_link_ptr->next_path_ptr != NULL) {
			p->dire = path_link_ptr->dire;
			p->next_path_ptr = p + 1;
			p++;
			path_link_ptr = path_link_ptr->next_path_ptr;
		}
		p->dire = path_link_ptr->dire;
		p->next_path_ptr = NULL;

		delete []node_list;
		delete []path_link;
		return path_return;
	}

	delete []node_list;
	delete []path_link;
	return NULL;
}

CFindPath::CFindPath(long step, int range) {
	MAX_PATH_STEP = step;
	STEP_LIMIT = step * step;
	_nWidth = step;
	m_iRange = range;
	_vecDir.resize((WORD)MAX_PATH_STEP);
	_vecDirSave.resize((WORD)MAX_PATH_STEP);
	_vecPathPoint.resize((WORD)MAX_PATH_STEP);

	_byTempBlock = std::make_unique<uint8_t[]>(MAX_PATH_STEP * MAX_PATH_STEP);
	_byTempBlock[0] = 1;

	_bblock = true;
	m_bFindPath = false;
	m_bLongPathFinding = false;
}

CFindPath::~CFindPath() {
}

void CFindPath::SetLongPathFinding(long step, int range) {
	MAX_PATH_STEP = step;
	STEP_LIMIT = step * step;
	_nWidth = step;
	m_iRange = range;
	_vecDir.clear();
	_vecDir.resize((WORD)MAX_PATH_STEP);
	_vecDirSave.clear();
	_vecDirSave.resize((WORD)MAX_PATH_STEP);
	_vecPathPoint.clear();
	_vecPathPoint.resize((WORD)MAX_PATH_STEP);
	_byTempBlock = std::make_unique<uint8_t[]>(MAX_PATH_STEP * MAX_PATH_STEP);
	_byTempBlock[0] = 1;
	_bblock = true;
	m_bFindPath = false;
	m_bLongPathFinding = true;
}

void CFindPath::SetShortPathFinding(long step, int range) {
	MAX_PATH_STEP = step;
	STEP_LIMIT = step * step;
	_nWidth = step;
	m_iRange = range;
	_vecDir.resize((WORD)MAX_PATH_STEP);
	_vecDirSave.resize((WORD)MAX_PATH_STEP);
	_vecPathPoint.resize((WORD)MAX_PATH_STEP);
	_byTempBlock = std::make_unique<uint8_t[]>(MAX_PATH_STEP * MAX_PATH_STEP);
	_byTempBlock[0] = 1;
	_bblock = true;
	m_bFindPath = false;
	m_bLongPathFinding = false;
}

void CFindPath::SetTargetPos(int nCurX, int nCurY, int& nTargetX, int& nTargetY, bool bback) {
	D3DXVECTOR2 vDir;
	vDir.x = float(nCurX - nTargetX);
	vDir.y = float(nCurY - nTargetY);

	float fd = D3DXVec2Length(&vDir);
	float fstep = 0.05f;
	D3DXVec2Normalize(&vDir, &vDir);
	if (bback) {
		D3DXVECTOR2 vPos((float)nTargetX + 0.25f, (float)nTargetY + 0.25f);

		int nx = nTargetX, ny = nTargetY;
		int inum = int(fd / fstep);
		if (inum < 1)
			return;
		for (int n = 0; n < inum; ++n) {
			const auto v = vDir * fstep;
			D3DXVec2Add(&vPos, &vPos, &(v));
			nx = int(vPos.x);
			ny = int(vPos.y);
			if (_byTempBlock[ny * _nWidth + nx] == 0) {
				nTargetX = nx;
				nTargetY = ny;
				return;
			}
		}
	}
	else {
		vDir = -vDir;
		D3DXVECTOR2 vPos((float)nCurX + 0.25f, (float)nCurY + 0.25f);

		int nx = nCurX, ny = nCurY;
		int inum = int(fd / fstep);
		if (inum < 1)
			return;
		for (int n = 0; n < inum; ++n) {
			const auto v = vDir * fstep;
			D3DXVec2Add(&vPos, &vPos, &(v));
			nx = int(vPos.x);
			ny = int(vPos.y);
			if (_byTempBlock[ny * _nWidth + nx] == 0) {
				nTargetX = nx;
				nTargetY = ny;
			}
			else {
				return;
			}
		}
	}
	return;
}

BOOL CFindPath::IsCross(int nCurX, int nCurY, int nTargetX, int nTargetY) {
	D3DXVECTOR2 vDir;
	vDir.x = float(nTargetX - nCurX);
	vDir.y = float(nTargetY - nCurY);

	BYTE bydir;
	if (nTargetY < nCurY) {
		if (nTargetX == nCurX)
			bydir = 0;
		else if (nTargetX > nCurX)
			bydir = 1;
		else if (nTargetX < nCurX)
			bydir = 7;
	}
	else if (nTargetY == nCurY) {
		if (nTargetX > nCurX)
			bydir = 2;
		else if (nTargetX < nCurX)
			bydir = 6;
	}
	else if (nTargetY > nCurY) {
		if (nTargetX == nCurX)
			bydir = 4;
		else if (nTargetX > nCurX)
			bydir = 3;
		else if (nTargetX < nCurX)
			bydir = 5;
	}

	float fd = D3DXVec2Length(&vDir);
	float fstep = 0.05f;
	D3DXVec2Normalize(&vDir, &vDir);

	D3DXVECTOR2 vPos((float)nCurX + 0.25f, (float)nCurY + 0.25f);

	int nx = -1, ny = -1;
	int inum = int(fd / fstep);
	if (inum < 1)
		return TRUE;
	for (int n = 1; n < inum; ++n) {
		const auto v = vDir * fstep;
		D3DXVec2Add(&vPos, &vPos, &(v));
		if (nx == int(vPos.x) && ny == int(vPos.y))
			continue;

		if (int(vPos.x) != nx || int(vPos.y) != ny) {
			nx = int(vPos.x);
			ny = int(vPos.y);


			switch (bydir) {
			case 0:
			case 2:
			case 4:
			case 6:
				if (_byTempBlock[ny * _nWidth + nx] > 0)
					return FALSE;
				break;
			case 1:
				if (_byTempBlock[ny * _nWidth + nx] > 0)
					return FALSE;
				if (_byTempBlock[ny * _nWidth + (nx - 1)] > 0)
					return FALSE;
				if (_byTempBlock[(ny + 1) * _nWidth + nx] > 0)
					return FALSE;
				break;
			case 3:
				if (_byTempBlock[ny * _nWidth + nx] > 0)
					return FALSE;
				if (_byTempBlock[(ny - 1) * _nWidth + nx] > 0)
					return FALSE;
				if (_byTempBlock[ny * _nWidth + (nx - 1)] > 0)
					return FALSE;
				break;
			case 5:
				if (_byTempBlock[ny * _nWidth + nx] > 0)
					return FALSE;
				if (_byTempBlock[(ny - 1) * _nWidth + nx] > 0)
					return FALSE;
				if (_byTempBlock[ny * _nWidth + (nx + 1)] > 0)
					return FALSE;
				break;
			case 7:
				if (_byTempBlock[ny * _nWidth + nx] > 0)
					return FALSE;
				if (_byTempBlock[ny * _nWidth + (nx + 1)] > 0)
					return FALSE;
				if (_byTempBlock[(ny - 1) * _nWidth + nx] > 0)
					return FALSE;
				break;
			}
		}
	}
	return TRUE;
}

BYTE* CFindPath::GetTempTerrain(CGameScene* pScene, CCharacter* pCha, int iCurX, int iCurY) {
	_byTempBlock[0] = 1;

	const int imax = _nWidth;
	int iRadius = imax / 2;

	int tm = iCurX - iRadius;
	int tn = iCurY - iRadius;

	char Territory = pCha->GetDefaultChaInfo()->chTerritory;

	MPTerrain* pTer = pScene->GetTerrain();
	if (!pTer) return _byTempBlock.get();

	bool isConnected = g_NetIF->IsConnected();
	const int nStartX = (iCurX - iRadius);
	const int nStartY = (iCurY - iRadius);

	register unsigned char* pValue = _byTempBlock.get();
	register int iy;
	register int ix;
	if (!isConnected) {
		for (iy = 0; iy < imax; iy++) {
			tn = nStartY + iy;
			if (tn < 0) {
				pValue = &_byTempBlock[iy * imax];
				continue;
			}

			for (ix = 0; ix < imax; ix++) {
				tm = nStartX + ix;
				if (tm < 0) {
					pValue++;
					continue;
				}

				*pValue = pTer->GetBlock()->IsGridBlock(tm, tn); //jze
				pValue++;
			}
		}
	}
	else {
		for (iy = 0; iy < imax; iy++) {
			tn = nStartY + iy;
			if (tn < 0) {
				pValue = &_byTempBlock[iy * imax];
				continue;
			}
			for (ix = 0; ix < imax; ix++) {
				tm = nStartX + ix;
				if (tm < 0) {
					pValue++;
					continue;
				}

				*pValue = pTer->GetBlock()->IsGridBlock(tm, tn); //jze

				//
				if (*pValue == 0) {
					if (!g_IsMoveAble(pCha->getChaCtrlType(), Territory,
									  (EAreaMask)pTer->GetBlock()->GetTileRegionAttr(tm * 50 / 100, tn * 50 / 100)))
						*pValue = 1;

					if (m_bLongPathFinding) {
						short sRegion = pTer->GetBlock()->GetTileRegionAttr(tm * 50 / 100, tn * 50 / 100);
						if ((sRegion & enumAREA_TYPE_MINE))
							*pValue = 1;
					}
				}

				pValue++;
			}
		}
	}

	//FILE* pFile = fopen("test.txt","wb");
	//char buf[1] = { 0 };
	//char ss = {'\n'};
	//for( int i=0; i<_nWidth; i++)
	//{
	//	for (int j = 0; j < _nWidth; j++)
	//	{
	//		fwrite(&buf,sizeof(char),1, pFile);
	//	}
	//	fwrite(&ss,sizeof(char),1, pFile);
	//}
	//fclose(pFile);

	return _byTempBlock.get();
}

bool PointPointRange(int px1, int py1, int px2, int py2, int range) {
	return ((abs(px1 - px2) < range) &&
		(abs(py1 - py2) < range));
}

// nCurX,nCurY,nTargetX,nTargetY
BOOL CFindPath::FindPath(CGameScene* pScene, CCharacter* pCha, int nSelfX, int nSelfY, int nTargetX, int nTargetY,
						 bool& IsWalkLine) {
	if (nSelfX < 0 || nSelfY < 0 || nTargetX < 0 || nTargetY < 0) {
		ToLogService("map", "msginput coordinate less than 0, Self[{}, {}], Target[{}, {}], ChaPos[{}, {}]", nSelfX,
					 nSelfY, nTargetX, nTargetY, pCha->GetCurX(), pCha->GetCurY());
		return FALSE;
	}
	MPTerrain* pTerrain = pScene->GetTerrain();
	if (!pTerrain) return FALSE;

	_vecPathPoint.clear();
	if (_bblock)
		return FALSE;

	int nCurX = nSelfX;
	int nCurY = nSelfY;

	int iRange = pCha->GetDefaultChaInfo()->sRadii * 2;

	int nx = nCurX / 100;
	int ny = nCurY / 100;
	int ntx = nTargetX / 100;
	int nty = nTargetY / 100;

	int showx = (int)pScene->GetTerrain()->GetShowCenterX();
	int showy = (int)pScene->GetTerrain()->GetShowCenterY();

	if (!PointPointRange(nx, ny, showx, showy, m_iRange)) {
		ToLogService("map", "input coordinate start is not in range, Self[{}, {}], Target[{}, {}], ChaPos[{}, {}]",
					 nSelfX, nSelfY, nTargetX, nTargetY, pCha->GetCurX(), pCha->GetCurY());
		return FALSE;
	}
	if (!PointPointRange(ntx, nty, showx, showy, m_iRange)) {
		ToLogService("map", "input coordinate target is not in range, Self[{}, {}], Target[{}, {}], ChaPos[{}, {}]",
					 nSelfX, nSelfY, nTargetX, nTargetY, pCha->GetCurX(), pCha->GetCurY());
		return FALSE;
	}
	nCurX = nCurX / 50;
	nCurY = nCurY / 50;
	nTargetX = nTargetX / 50;
	nTargetY = nTargetY / 50;

	if (nTargetX == nCurX && nTargetY == nCurY) {
		return FALSE;
	}
	D3DXVECTOR3 vOrg((float)nSelfX / 100.0f, (float)nSelfY / 100.0f, 0.5f);

	_nCurX = nx;
	_nCurY = ny;
	_vStart = D3DXVECTOR3(nCurX * 0.5f + 0.25f, nCurY * 0.5f + 0.25f, 0.5f);

	GetTempTerrain(pScene, pCha, nCurX, nCurY);

	BYTE* byBuf = _byTempBlock.get();

	_nTargetX = _nWidth / 2 + ((nTargetX - nCurX));
	_nTargetY = _nWidth / 2 + ((nTargetY - nCurY));


	if (_byTempBlock[_nTargetY * _nWidth + _nTargetX] > 0) {
		SetTargetPos((_nWidth / 2), (_nWidth / 2), _nTargetX, _nTargetY);

		if ((_nWidth / 2) == _nTargetX && (_nWidth / 2) == _nTargetY)
			return FALSE;
		if (_byTempBlock[_nTargetY * _nWidth + _nTargetX] > 0) {
			return FALSE;
		}
	}
	D3DXVECTOR3 vEnd((nCurX + (_nTargetX - _nWidth / 2)) * 0.5f + 0.25f,
					 (nCurY + (_nTargetY - _nWidth / 2)) * 0.5f + 0.25f, 0.5f);
	if (IsCross((_nWidth / 2), (_nWidth / 2), _nTargetX, _nTargetY)) {
		_vecPathPoint.clear();

		if ((_nWidth / 2) == _nTargetX && (_nWidth / 2) == _nTargetY)
			return FALSE;

		_vecPathPoint.push_back(vOrg);
		_vecPathPoint.push_back(_vStart);
		_vecPathPoint.push_back(vEnd);
		return TRUE;
	}
	if (IsWalkLine) {
		SetTargetPos((_nWidth / 2), (_nWidth / 2), _nTargetX, _nTargetY,false);
		D3DXVECTOR3 vEnd2((nCurX + (_nTargetX - _nWidth / 2)) * 0.5f + 0.25f,
						  (nCurY + (_nTargetY - _nWidth / 2)) * 0.5f + 0.25f, 0.5f);
		_vecPathPoint.clear();
		if ((_nWidth / 2) == _nTargetX && (_nWidth / 2) == _nTargetY)
			return FALSE;
		_vecPathPoint.push_back(vOrg);
		_vecPathPoint.push_back(_vStart);
		_vecPathPoint.push_back(vEnd2);
		return TRUE;
	}
	BYTE* byBuf2 = byBuf;

	int s1 = m_iRange; //jze

	PATH_LINK* pLink = ::SearchPath(byBuf2, _nWidth, s1 * 2, (_nWidth / 2), (_nWidth / 2), _nTargetX, _nTargetY,
									STEP_LIMIT);

	if (pLink == NULL) {
		ToLogService("common", "Path Not Found!");
		GetTempTerrain(pScene, pCha, nCurX, nCurY);

		SetTargetPos((_nWidth / 2), (_nWidth / 2), _nTargetX, _nTargetY,false);
		if ((_nWidth / 2) == _nTargetX && (_nWidth / 2) == _nTargetY)
			return FALSE;

		D3DXVECTOR3 vEnd2((nCurX + (_nTargetX - _nWidth / 2)) * 0.5f + 0.25f,
						  (nCurY + (_nTargetY - _nWidth / 2)) * 0.5f + 0.25f, 0.5f);
		_vecPathPoint.clear();

		_vecPathPoint.push_back(vOrg);
		_vecPathPoint.push_back(_vStart);
		_vecPathPoint.push_back(vEnd2);

		IsWalkLine = true;
		return TRUE;
	}

	_vecDir.clear();
	_vecDirSave.clear();

	int n = 0;
	PATH_LINK* pTmpLink = pLink;
	while (pTmpLink) {
		_vecDir.push_back(pTmpLink->dire);
		_vecDirSave.push_back(pTmpLink->dire);
		pTmpLink = pTmpLink->next_path_ptr;
		n++;
		if (n >= MAX_PATH_STEP) {
			ToLogService("common", "Path Searth Step = {} Overmax ", n);
			n = 0; // 0
			break;
		}
	}

	if (n == 0) {
		return FALSE;
	}

	int m;
	_vecPathPoint.clear();
	_vecPathPoint.push_back(vOrg);
	_vecPathPoint.push_back(_vStart);
	D3DXVECTOR3 tvm = _vStart;
	for (m = 1; m < _vecDirSave.size(); ++m) {
		switch (*_vecDirSave[m - 1]) {
		case 0:
			tvm.x = tvm.x;
			tvm.y = tvm.y - 0.5f;
			break;
		case 1:
			tvm.x = tvm.x + 0.5f;
			tvm.y = tvm.y - 0.5f;
			break;
		case 2:
			tvm.x = tvm.x + 0.5f;
			tvm.y = tvm.y;
			break;
		case 3:
			tvm.x = tvm.x + 0.5f;
			tvm.y = tvm.y + 0.5f;
			break;
		case 4:
			tvm.x = tvm.x;
			tvm.y = tvm.y + 0.5f;
			break;
		case 5:
			tvm.x = tvm.x - 0.5f;
			tvm.y = tvm.y + 0.5f;
			break;
		case 6:
			tvm.x = tvm.x - 0.5f;
			tvm.y = tvm.y;
			break;
		case 7:
			tvm.x = tvm.x - 0.5f;
			tvm.y = tvm.y - 0.5f;
			break;
		default:
			ToLogService("common", "msg:error");
			break;
		}
		if (*_vecDirSave[m - 1] != *_vecDirSave[m]) {
			_vecPathPoint.push_back(tvm);
		}
	}
	_vecPathPoint.push_back(vEnd);

	delete []pLink;

	return TRUE;
}

void CFindPath::RenderPath(MPMap* pMap) {
	if (_vecPathPoint.empty())
		return;

	//static CEffectBox  CPathBox;
	//static BOOL        bCreatePathBox = FALSE;

	//if(bCreatePathBox==FALSE)
	//{
	//CPathBox.Create(g_Render.GetDevice(),0.25f);
	CPathBox.Show(TRUE);
	CPathBox.setWriteFrame(TRUE);
	CPathBox.ShowLine(TRUE);
	CPathBox.ShowBox(TRUE);
	CPathBox.setColor(0xff00ffff);
	CPathBox.setScale(1, 1, 1);

	//	bCreatePathBox = TRUE;
	//}

	for (int m = 0; m < _vecPathPoint.size(); m++) {
		_vecPathPoint[m]->z = pMap->GetHeight(_vecPathPoint[m]->x, _vecPathPoint[m]->y);
		if (_vecPathPoint[m]->z < SEA_LEVEL) _vecPathPoint[m]->z = SEA_LEVEL;

		CPathBox.setPos(*_vecPathPoint[m]);
		CPathBox.Render();
	}
}

bool CFindPath::Find(CGameScene* pScene, CCharacter* pCha, int nSelfX, int nSelfY, int nTargetX, int nTargetY,
					 bool& IsWalkLine) {
	UnLock();
	if (FindPath(pScene, pCha, nSelfX, nSelfY, nTargetX, nTargetY, IsWalkLine)) {
		m_bFindPath = true;
		int count = _vecPathPoint.size();
		_nLength = 0;
		for (int i = 1; i < count; i++)
			_nLength += GetDistance((__int64)(_vecPathPoint[i - 1]->x * 100.0f),
									(__int64)(_vecPathPoint[i - 1]->y * 100.0f),
									(__int64)(_vecPathPoint[i]->x * 100.0f), (__int64)(_vecPathPoint[i]->y * 100.0f));

		if (m_bLongPathFinding && _nLength < 1000) {
			m_bFindPath = false;
			Lock();
			return false;
		}

		D3DXVECTOR3* pointer;
		if (count > 0) {
			pointer = _vecPathPoint[0];
			float fX = pointer->x * 100.0f;
			float fY = pointer->y * 100.0f;
			if (fX >= (float)nSelfX - 0.05f && fX <= (float)nSelfX + 0.05f && fY >= (float)nSelfY - 0.05f && fY <= (
				float)nSelfY + 0.05f) {
				_vecPathPoint.pop_front();
			}
			else {
				ToLogService("common", "msgok");
			}
		}

		Lock();

		count = _vecPathPoint.size();
		if (count >= 2) {
			count--;
			for (int i = 0; i < count; i++) {
				if (_vecPathPoint[i]->x == _vecPathPoint[i + 1]->x && _vecPathPoint[i]->y == _vecPathPoint[i + 1]->y) {
					ToLogService("common", "msgfind path repeat!");
					return true;
				}
			}
		}

		_vecPathPoint[0]->x = (float)nSelfX / 100.0f;
		_vecPathPoint[0]->y = (float)nSelfY / 100.0f;
		return _nLength > 0;
	}
	else {
		Lock();
		return false;
	}
}

CFindPathEx::CFindPathEx() {
	m_iTargetX = -1;
	m_iTargetY = -1;
	m_iGuideTargetX = -1;
	m_iGuideTargetY = -1;
	m_iRecordX = -1;
	m_iRecordY = -1;
	m_iDeltaFlagX = 0;
	m_iDeltaFlagY = 0;
	m_dwTime = 0;
	m_iResetTargetTimes = 0;
	m_bReach = false;
	m_bReachGuide = false;
}

void CFindPathEx::Reset() {
	m_iTargetX = -1;
	m_iTargetY = -1;
	m_iGuideTargetX = -1;
	m_iGuideTargetY = -1;
	m_iRecordX = -1;
	m_iRecordY = -1;
	m_dwTime = 0;
	m_bReach = false;
	m_bReachGuide = false;
	m_bBigObstacle = false;
	while (!m_GuideTargetList.empty())
		m_GuideTargetList.pop();
}

extern bool g_bIsMouseWalk; //
void CFindPathEx::SetTarget(int CurX, int CurY, int TargetX, int TargetY) {
	m_iGuideTargetX = CurX;
	m_iGuideTargetY = CurY;
	m_iRecordX = CurX;
	m_iRecordY = CurY;
	m_iTargetX = TargetX;
	m_iTargetY = TargetY;

	m_GuideTargetList.push(D3DXVECTOR2((FLOAT)TargetX, (FLOAT)TargetY));
	m_GuideTargetList.push(D3DXVECTOR2((FLOAT)CurX, (FLOAT)CurY));
	g_bIsMouseWalk = false; //  by Waiting 2009-08-04
}

void CFindPathEx::SetDestDirection(int CurX, int CurY, int TargetX, int TargetY) {
	m_iDeltaFlagX = (TargetX - CurX) / (abs(TargetX - CurX) ? abs(TargetX - CurX) : 1);
	m_iDeltaFlagY = (TargetY - CurY) / (abs(TargetY - CurY) ? abs(TargetY - CurY) : 1);
}

void CFindPathEx::ClearDestDirection() {
	m_iDeltaFlagX = 0;
	m_iDeltaFlagY = 0;
}

//automatic calculate guide
void CFindPathEx::CalculateGuideStraight() {
	//get scene
	CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	if (!pScene) return;
	float k;
	float deltaX;
	float deltaY;

	int TempX;
	int TempY;

	deltaX = float(m_iTargetX - m_iGuideTargetX);
	deltaY = float(m_iTargetY - m_iGuideTargetY);

	//if delta x,y less than GUIDE_RANDGE, we push target to our stack
	if (abs(deltaX) <= GUIDE_RANDGE && abs(deltaY) <= GUIDE_RANDGE) {
		m_GuideTargetList.push(D3DXVECTOR2((FLOAT)m_iTargetX, (FLOAT)m_iTargetY));
		return;
	}

	//if deltaX or deltaY equal to zero
	if (deltaX == 0) {
		if (deltaY > 0) {
			TempX = m_iGuideTargetX;
			TempY = m_iGuideTargetY + GUIDE_RANDGE;

			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}
				TempY -= 1;
			}
		}
		else if (deltaY < 0) {
			TempX = m_iGuideTargetX;
			TempY = m_iGuideTargetY - GUIDE_RANDGE;

			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}
				TempY += 1;
			}
		}
	}
	else if (deltaY == 0) {
		if (deltaX > 0) {
			TempX = m_iGuideTargetX + GUIDE_RANDGE;
			TempY = m_iGuideTargetY;

			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}
				TempX -= 1;
			}
		}
		else if (deltaX < 0) {
			TempX = m_iGuideTargetX - GUIDE_RANDGE;
			TempY = m_iGuideTargetY;

			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}
				TempX += 1;
			}
		}
	}

	//if delta x and y isn't equal to zero
	if (deltaX != 0 && deltaY != 0)
		k = abs(deltaY / deltaX);

	//four direction ++ -+ -- +-
	if (deltaX > 0 && deltaY > 0) {
		if (k > 1) {
			TempX = m_iGuideTargetX + int(float(GUIDE_RANDGE) / float(k));
			TempY = m_iGuideTargetY + GUIDE_RANDGE;

			int idx = 0;
			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				idx++;
				TempX -= (idx / k) > 1 ? idx = 0, 1 : 0;
				TempY -= 1;
			}
		}
		else if (k == 1) {
			TempX = m_iGuideTargetX + GUIDE_RANDGE;
			TempY = m_iGuideTargetY + GUIDE_RANDGE;

			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				TempX -= 1;
				TempY -= 1;
			}
		}
		else {
			TempX = m_iGuideTargetX + GUIDE_RANDGE;
			TempY = m_iGuideTargetY + int(float(GUIDE_RANDGE) * float(k));

			int idx = 0;
			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				idx++;
				TempX -= 1;
				TempY -= (idx * k) > 1 ? idx = 0, 1 : 0;
			}
		}
	}
	else if (deltaX < 0 && deltaY > 0) {
		if (k > 1) {
			TempX = m_iGuideTargetX - int(float(GUIDE_RANDGE) / float(k));
			TempY = m_iGuideTargetY + GUIDE_RANDGE;

			int idx = 0;
			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				idx++;
				TempX += (idx / k) > 1 ? idx = 0, 1 : 0;
				TempY -= 1;
			}
		}
		else if (k == 1) {
			TempX = m_iGuideTargetX - GUIDE_RANDGE;
			TempY = m_iGuideTargetY + GUIDE_RANDGE;

			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				TempX += 1;
				TempY -= 1;
			}
		}
		else {
			TempX = m_iGuideTargetX - GUIDE_RANDGE;
			TempY = m_iGuideTargetY + int(float(GUIDE_RANDGE) * float(k));

			int idx = 0;
			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				idx++;
				TempX += 1;
				TempY -= (idx * k) > 1 ? idx = 0, 1 : 0;
			}
		}
	}
	else if (deltaX < 0 && deltaY < 0) {
		if (k > 1) {
			TempX = m_iGuideTargetX - int(float(GUIDE_RANDGE) / float(k));
			TempY = m_iGuideTargetY - GUIDE_RANDGE;

			int idx = 0;
			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				idx++;
				TempX += (idx / k) > 1 ? idx = 0, 1 : 0;
				TempY += 1;
			}
		}
		else if (k == 1) {
			TempX = m_iGuideTargetX - GUIDE_RANDGE;
			TempY = m_iGuideTargetY - GUIDE_RANDGE;

			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				TempX += 1;
				TempY += 1;
			}
		}
		else {
			TempX = m_iGuideTargetX - GUIDE_RANDGE;
			TempY = m_iGuideTargetY - int(float(GUIDE_RANDGE) * float(k));

			int idx = 0;
			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				idx++;
				TempX += 1;
				TempY += (idx * k) > 1 ? idx = 0, 1 : 0;
			}
		}
	}
	else if (deltaX > 0 && deltaY < 0) {
		if (k > 1) {
			TempX = m_iGuideTargetX + int(float(GUIDE_RANDGE) / float(k));
			TempY = m_iGuideTargetY - GUIDE_RANDGE;

			int idx = 0;
			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				idx++;
				TempX -= (idx / k) > 1 ? idx = 0, 1 : 0;
				TempY += 1;
			}
		}
		else if (k == 1) {
			TempX = m_iGuideTargetX + GUIDE_RANDGE;
			TempY = m_iGuideTargetY - GUIDE_RANDGE;

			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				TempX -= 1;
				TempY += 1;
			}
		}
		else {
			TempX = m_iGuideTargetX + GUIDE_RANDGE;
			TempY = m_iGuideTargetY - int(float(GUIDE_RANDGE) * float(k));

			float idx = 0;
			while (1) {
				if (!pScene->GetTerrain()->GetBlock()->IsGridBlock(TempX * 2, TempY * 2)) {
					if (!g_IsMoveAble(pScene->GetMainCha()->getChaCtrlType(),
									  pScene->GetMainCha()->GetDefaultChaInfo()->chTerritory,
									  (EAreaMask)pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
										  TempX * 50 / 100, TempY * 50 / 100))) {
						short sRegion = pScene->GetTerrain()->GetBlock()->GetTileRegionAttr(
							TempX * 50 / 100, TempY * 50 / 100);
						if (!(sRegion & enumAREA_TYPE_MINE)) {
							m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(TempY)));
							break;
						}
					}
				}

				idx++;
				TempX -= 1;
				TempY += (idx * k) > 1 ? idx = 0, 1 : 0;
			}
		}
	}
}

void CFindPathEx::CalculateGuideBypass() {
	//get scene
	CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	if (!pScene) return;

	int TempX = m_iGuideTargetX + m_iDeltaFlagX * GUIDE_RANDGE_SMALL;
	int TempY = m_iGuideTargetY + m_iDeltaFlagY * GUIDE_RANDGE_SMALL;

	m_GuideTargetList.push(D3DXVECTOR2(float(TempX), float(m_iGuideTargetY)));
	m_GuideTargetList.push(D3DXVECTOR2(float(m_iGuideTargetX), float(TempY)));
}

bool CFindPathEx::HaveTarget() {
	if (m_iTargetX != -1 && m_iTargetY != -1) {
		return true;
	}
	else {
		return false;
	}
}

void CFindPathEx::Move() {
	//get scene
	CWorldScene* pScene = dynamic_cast<CWorldScene*>(CGameApp::GetCurScene());
	if (!pScene) return;

	COneMoveState* pMoveState = dynamic_cast<COneMoveState*>(pScene->GetMainCha()->GetActor()->GetCurState());

	m_iCurX = (int)pScene->GetMainCha()->GetPos().x;
	m_iCurY = (int)pScene->GetMainCha()->GetPos().y;

	//dead gay can't use path finding
	if (!pScene->GetMainCha()->IsEnabled()) {
		Reset();
		return;
	}

	////we reset target of path find if we don't move for more than half second.
	if ((GetTickCount() - m_dwTime) > 500) {
		if ((m_iRecordX == m_iCurX) && (m_iRecordY == m_iCurY)) {
			int tx, ty, rx, ry;
			tx = m_iTargetX;
			ty = m_iTargetY;
			rx = m_iRecordX;
			ry = m_iRecordY;

			if (m_iResetTargetTimes < MAX_RESET_TARGET_TIMES) {
				if (m_GuideTargetList.size() != 3) {
					Reset();
					SetTarget(rx, ry, tx, ty);
					m_iResetTargetTimes++;
				}
			}
			else if (m_iResetTargetTimes >= MAX_RESET_TARGET_TIMES) {
				if (m_GuideTargetList.size() != 3) {
					Reset();
					SetTarget(rx, ry, tx, ty);
					m_bBigObstacle = true;
				}
			}
		}
		else {
			m_iRecordX = m_iCurX;
			m_iRecordY = m_iCurY;
			m_iResetTargetTimes = 0;
		}
		m_dwTime = GetTickCount();
	}

	if (!m_bReach) {
		//get mouse
		CMouseDown* pMouse = &pScene->GetMouseDown();

		int CurX = (int)pScene->GetMainCha()->GetPos().x;
		int CurY = (int)pScene->GetMainCha()->GetPos().y;

		if (m_GuideTargetList.size() == 3) {
			m_iGuideTargetX = int(m_GuideTargetList.top().x);
			m_iGuideTargetY = int(m_GuideTargetList.top().y);
			m_GuideTargetList.pop();
			m_bReachGuide = true;
		}

		if (m_bReachGuide) {
			int TargetX = m_iGuideTargetX;
			int TargetY = m_iGuideTargetY;

			//set long path finding
			if (!g_cFindPath.LongPathFinding())
				g_cFindPath.SetLongPathFinding(1024, 1024);

			if (abs(TargetX - CurX) <= 20 && abs(TargetY - CurY) <= 20) {
				g_cFindPath.SetShortPathFinding(128, 38);
			}

			//move to our guide list
			pMouse->ActMove(pScene->GetMainCha(), TargetX * 100, TargetY * 100,
							g_pGameApp->IsAltPress() ? true : false);

			//set short path finding
			//g_cFindPath.SetShortPathFinding(128,38);

			m_bReachGuide = false;
		}
		else {
			m_iGuideTargetX = int(m_GuideTargetList.top().x);
			m_iGuideTargetY = int(m_GuideTargetList.top().y);
			if (m_iGuideTargetX == CurX && m_iGuideTargetY == CurY) {
				if (m_GuideTargetList.size() != 0) {
					m_GuideTargetList.pop();
					if (m_GuideTargetList.size() < 2) {
						if (!m_bBigObstacle) {
							CalculateGuideStraight();
						}
						else {
							CalculateGuideBypass();
							m_bBigObstacle = false;
							m_iResetTargetTimes = 0;
						}
					}
					m_iGuideTargetX = int(m_GuideTargetList.top().x);
					m_iGuideTargetY = int(m_GuideTargetList.top().y);
					m_bReachGuide = true;
				}
			}
		}

		if (m_iTargetX == CurX && m_iTargetY == CurY) {
			m_bReach = true;
			ClearDestDirection();
		}
	}
	else {
		Reset();
		CNavigationBar::g_cNaviBar.Show(false);
		g_cFindPath.SetShortPathFinding(128, 38);
	}

	int ServerX = pScene->GetMainCha()->GetServerX() / 100;
	int ServerY = pScene->GetMainCha()->GetServerY() / 100;

	int dc = (m_iTargetX - m_iCurX) * (m_iTargetX - m_iCurX) + (m_iTargetY - m_iCurY) * (m_iTargetY - m_iCurY);
	int ds = (m_iTargetX - ServerX) * (m_iTargetX - ServerX) + (m_iTargetY - ServerY) * (m_iTargetY - ServerY);
	int diff = dc - ds;

	if (pMoveState != NULL) {
		if (diff > 5) {
			pMoveState->SetRate(pMoveState->GetRate() * 1.5f);
		}
		else if (diff < -5) {
			pMoveState->SetRate(0.8f);
		}
	}
}
