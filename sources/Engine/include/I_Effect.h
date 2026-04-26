#pragma once

#include <Util.h>
//stl
#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>

#include "lwDirectX.h"

class MPRender;
class CMPResManger;

using namespace MindPower;

//--------------------------------------------------------------------------------------
//struct or type.
//--------------------------------------------------------------------------------------
#define    s_string								std::string
#define    LIST_string							std::list<s_string>
#define    VEC_string							std::vector<s_string>
#define    LIST_int								std::list<int>
#define    LIST_dword							std::list<DWORD>
#define    VEC_dword							std::vector<DWORD>
#define    VEC_word								std::vector<WORD>
#define    VEC_int								std::vector<int>
#define    VEC_bool								std::vector<bool>
#define    VEC_ptr								std::vector<DWORD>
#define    VEC_BYTE								std::vector<BYTE>
#define    VEC_float							std::vector<float>

/////////////////////////////////////////////////////////////////////////
template <class _Ty>
class S_BVECTOR {
	std::vector<_Ty> m_VECPath;
	int m_nCount;
	int m_nPos;

public:
	S_BVECTOR() {
		m_VECPath.clear();
		m_nCount = 0;
		m_nPos = 0;
	}

	void resize(WORD _nSize = 100) {
		m_VECPath.clear();
		m_VECPath.resize(_nSize);
		clear();
	}

	void addsize(WORD _nSize = 100) {
		WORD size = (WORD)m_VECPath.size();
		m_VECPath.resize(size + _nSize);
	}

	void setsize(WORD _nSize = 1) {
		if (m_VECPath.size() < _nSize) {
			m_VECPath.resize(_nSize);
			//setsizeNew( _nSize );
		}
		clear();
		m_nCount = _nSize;
	}

	void setsizeNew(WORD _nSize = 1) {
		std::vector<_Ty> temp;
		temp.resize(m_nCount);
		for (WORD i = 0; i < m_nCount; i++) {
			temp[i] = m_VECPath[i];
		}

		m_VECPath.resize(_nSize);
		for (WORD i = 0; i < m_nCount; i++) {
			m_VECPath[i] = temp[i];
		}
	}

	void clear() {
		m_nCount = 0;
		m_nPos = 0;
	}

	void push_back(_Ty& _Base) {
		m_VECPath[m_nCount] = _Base;
		++m_nCount;
	}

	void pop_front() {
		if (m_nCount > m_nPos) {
			++m_nPos;
			if (m_nPos == m_nCount) {
				clear();
			}
		}
	}

	void pop_back() {
		if (m_nCount > m_nPos) {
			m_nCount--;
			if (m_nPos == m_nCount) {
				clear();
			}
		}
	}

	int size() const {
		return m_nCount - m_nPos;
	}

	bool empty() const {
		return m_nPos == m_nCount ? true : false;
	}

	_Ty* front() {
		if (!empty())
			return &m_VECPath[m_nPos];
		return NULL;
	}

	_Ty* next() {
		if (!empty()) {
			if ((m_nPos + 1) != m_nCount)
				return &m_VECPath[m_nPos + 1];
		}
		return NULL;
	}

	_Ty* end() {
		if (!empty()) {
			return &m_VECPath[m_nCount - 1];
		}
		return NULL;
	}

	_Ty* operator[](int i) {
		if (!empty() && i >= 0 && i < size()) {
			return &m_VECPath[m_nPos + i];
		}
		return NULL;
	}

	void remove(int _iIndex) {
		if (empty())
			return;
		int ipos = m_nPos + _iIndex;

		for (int n = ipos; n < m_nCount - 1; ++n) {
			m_VECPath[n] = m_VECPath[n + 1];
		}
		m_nCount--;
		if (m_nPos == m_nCount) {
			clear();
		}
		return;
	}
};

//#include <mindpower.h>

class MPRender;
////////////////////////////////////////////////////////////////////
class CMPResManger;
class CEffectModel;
class CTexCoordList;
class CTexList;
class I_Effect;
class CEffPath;
class EffParameter;

//class lwSceneItem ;
//!40VS,95 - 15
//
#define  MAX_SHADER_VERNUM		300
#define  MAX_SHADER_IDXNUM		MAX_SHADER_VERNUM  * 3 * 3

/************************************************************************/
/*  */
/************************************************************************/
struct SEFFECT_VERTEX {
	D3DXVECTOR3 m_SPos;
	FLOAT m_fIdx; //MESHUV
	DWORD m_dwDiffuse;
	D3DXVECTOR2 m_SUV;
};

#define		EFFECT_VER_FVF	(D3DFVF_XYZB1 | D3DFVF_DIFFUSE | D3DFVF_TEX1)


struct SEFFECT_SHADE_VERTEX {
	D3DXVECTOR3 m_SPos;
	//float			m_fIdx;//MESH
	//m_fIdx[0]m_fIdx[1]uv
	DWORD m_dwDiffuse;
	D3DXVECTOR2 m_SUV;
	D3DXVECTOR2 m_SUV2;
};

#define		EFFECT_SHADE_FVF	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX2)


#define		MESH_TRI		"Triangle"
#define		MESH_PLANETRI	"TrianglePlane"
#define		MESH_RECT		"Rect"
#define		MESH_RECTZ		"RectZ"
#define		MESH_PLANERECT	"RectPlane"

#define		MESH_CYLINDER	"Cylinder"
#define		MESH_CONE		"Cone"
#define		MESH_SPHERE		"Sphere"


inline bool IsTobMesh(const s_string& strName) {
	return ((strName == MESH_CYLINDER) || (strName == MESH_CONE) || (strName == MESH_SPHERE));
}

inline bool IsCylinderMesh(const s_string& strName) {
	return (strName == MESH_CYLINDER);
}

inline bool IsDefaultMesh(const s_string& strName) {
	static s_string str[] =
	{
		MESH_TRI,
		MESH_PLANETRI,
		MESH_RECT,
		MESH_RECTZ,
		MESH_PLANERECT,
		MESH_CYLINDER,
		MESH_CONE,
		MESH_SPHERE,
	};
	for (int n = 0; n < 8; ++n) {
		if (strName == str[n])
			return true;
	}
	return false;
}

#include "MPSceneItem.h"
#include "MPCharacter.h"

#define _MAX_STRING					256

// Одно действие персонажа: номер анимации, диапазон кадров, ключевые кадры.
// _actionNo = 1-based идентификатор (0 = пустая ячейка в _actions[]).
struct ActionInfo {
	short _actionNo{0};
	short _startFrame{0};
	short _endFrame{0};
	std::vector<short> _keyFrames;

	bool IsValid() const {
		return _actionNo >= 1;
	}

	// Конвертация в engine-POD lwPoseInfo (DWORD-поля, фиксированный key_frame_seq[MAX_KEY_FRAME_NUM]).
	// Используется потребителями движка: MPParticleCtrl, CCharacterModel::LoadPose.
	lwPoseInfo ToPoseInfo() const {
		lwPoseInfo pi{};
		pi.start = static_cast<DWORD>(_startFrame);
		pi.end = static_cast<DWORD>(_endFrame);
		const size_t kfCount = (std::min)(_keyFrames.size(), static_cast<size_t>(MAX_KEY_FRAME_NUM));
		pi.key_frame_num = static_cast<DWORD>(kfCount);
		for (size_t k = 0; k < kfCount; k++) {
			pi.key_frame_seq[k] = static_cast<DWORD>(_keyFrames.at(k));
		}
		return pi;
	}
};

// Блок действий одного типа персонажа.
// _actions[i] соответствует actionNo = i + 1 (1-based). Ячейки с _actionNo == 0 — пустые.
// _validCount = число ячеек с _actionNo >= 1.
struct SChaAction {
	short _characterType{0};
	short _validCount{0};
	std::vector<ActionInfo> _actions{};

	bool IsValid() const {
		return _characterType >= 1;
	}
};

// Кэш действий персонажей в Engine-формате.
// Заполняется из CharacterActionStore (SQLite) через LoadActionDataFromStore().
// Потребитель: MPParticleCtrl::LoadPose (анимация партиклов на костях).
class CCharacterActionCache {
public:
	CCharacterActionCache() = default;
	~CCharacterActionCache() = default;

	// Загрузить данные анимаций из CharacterActionStore в кэш.
	// Предварительно данные должны быть загружены в store: CharacterActionStore::Instance()->Load(db).
	bool LoadActionDataFromStore();
	void Free();

	// Безопасный доступ к блоку действий по 1-based characterType.
	// nullptr если вне диапазона или блок пустой.
	// Указатель валиден до следующего LoadActionDataFromStore() / Free().
	const SChaAction* GetCharAction(int characterType) const;

	short GetMaxCharacterType() const {
		return _maxCharacterType;
	}

	short GetActualCharacterType() const {
		return _actualCharacterType;
	}

	static CCharacterActionCache _cache;

protected:
	short _maxCharacterType{0};
	short _actualCharacterType{0};
	std::vector<SChaAction> _characterActions;
};

long StringGetT(char* out, long out_max, const char* in, long* in_from, const char* end_list, long end_len);
void StringSkipCompartmentT(const char* in, long* in_from, const char* skip_list, long skip_len);

//class  lwSceneItem;
LW_USING


struct ModelParam {
	int iSegments;
	float fHei;
	float fTopRadius;
	float fBottomRadius;
	std::vector<D3DXVECTOR3> vecVer;
	void Create();
};

class CEffectModel : public MPSceneItem {
public:
	CEffectModel();

	bool Copy(const CEffectModel& rhs);

	CEffectModel(MPRender* pDev, lwIResourceMgr* pRes = NULL);
	virtual ~CEffectModel();

public:
	void ReleaseModel();

	void InitDevice(MPRender* pDev, lwIResourceMgr* pRes = NULL);

	bool CreateTriangle();
	bool CreatePlaneTriangle();
	bool CreateRect();
	bool CreatePlaneRect();
	bool CreateRectZ();

	//
	bool CreateShadeModel(WORD wVerNum = 6, WORD wFaceNum = 2, int iGridCrossNum = 1, bool usesoft = false);

	//
	bool CreateTob(const s_string& str, int nSeg, float fHei, float fTopRadius, float fBottomRadius) {
		if (str == MESH_CYLINDER)
			return CreateCylinder(nSeg, fHei, fTopRadius, fBottomRadius);
		if (str == MESH_CONE)
			return CreateCone(nSeg, fHei, fBottomRadius);
		return false;
	}

	bool CreateCylinder(int nSeg, float fHei, float fTopRadius, float fBottomRadius);
	bool CreateCone(int nSeg, float fHei, float fRadius);
	//bool	CreateSphere(int nSeg,float fHei,float fRadius);


	bool IsBoard() {
		return (!IsChangeably() && !IsItem());
	}

	bool IsChangeably() {
		return m_bChangeably;
	}

	bool IsItem() {
		return _lwMesh ? false : true;
	}

	bool LoadModel(const char* pszName);

	void Begin();

	void FrameMove(DWORD dwDailTime);
	void RenderModel();

	void RenderTob(ModelParam* last, ModelParam* next, float lerp);

	void End();

	DWORD GetVerCount() {
		return _dwVerCount;
	}

	DWORD GetFaceCount() {
		return _dwFaceCount;
	}


	lwILockableStreamIB* GetIndexBuffer() {
		return _lpSIB;
	}

	lwILockableStreamVB* GetVertexBuffer() {
		return _lpSVB;
	}

	void Lock(BYTE** pvEffVer);
	void Unlock();
	void LockIB(BYTE** pIdx);
	void UnlockIB();

	void SetRenderNum(WORD wVer, WORD wFace);

	MPRender* GetDev();

public:
	//!3D
	MPRender* m_pDev;

	lwIResourceMgr* m_pRes;

	s_string m_strName;

	bool m_bChangeably;

	int m_nSegments;
	float m_rHeight;
	float m_rRadius;
	float m_rBotRadius;

	SEFFECT_VERTEX* m_vEffVer;

	//MPSceneItem*				m_pModel;
	lwITex* m_oldtex;
	lwITex* m_oldtex2;
	bool m_bItem;

	int m_iID;

	bool m_bUsing;

protected:
	lwIMesh* _lwMesh;
	lwILockableStreamVB* _lpSVB;
	lwILockableStreamIB* _lpSIB;


	DWORD _dwVerCount;
	DWORD _dwFaceCount;

	//CChaModel*					_pChaModel;

public:
	// Getters and Setters
	bool IsUsing() {
		return m_bUsing;
	}

	void SetUsing(bool bUsing) {
		m_bUsing = bUsing;
	}
};

/************************************************************************/
/* */
/************************************************************************/
typedef std::vector<D3DXVECTOR2> TEXCOORD;

class CTexCoordList {
public:
	CTexCoordList();
	~CTexCoordList();

public:
	void GetCoordFromModel(CEffectModel* pCModel);
	void CreateTranslateCoord();

	void GetCurCoord(S_BVECTOR<D3DXVECTOR2>& vecOutCoord, WORD& wCurIndex, float& fCurTime, float fDailTime);

	void Reset();

	void Clear();

	void Copy(CTexCoordList* pList);

public:
	//!
	WORD m_wVerCount;
	//.
	WORD m_wCoordCount;
	//
	float m_fFrameTime;
	//
	std::vector<TEXCOORD> m_vecCoordList;

	////!
	//WORD					m_wCurIndex;
	////!
	//float					m_fCurTime;
	//!
	//TEXCOORD				m_vecCurCoord;
};

/************************************************************************/
/* */
/************************************************************************/
class CTexList {
public:
	CTexList();
	~CTexList();

public:
	void SetTextureName(const s_string& pszName);

	void GetTextureFromModel(CEffectModel* pCModel);

	void CreateSpliteTexture(int iRow, int iColnum);

	void GetCurTexture(S_BVECTOR<D3DXVECTOR2>& coord, WORD& wCurIndex, float& fCurTime, float fDailTime);
	void Reset();

	void Clear();

	void Remove();

	void Copy(CTexList* pList);

public:
	//
	WORD m_wTexCount;
	//
	float m_fFrameTime;
	//
	std::vector<TEXCOORD> m_vecTexList;

	//!
	s_string m_vecTexName;
	//!
	IDirect3DTextureX* m_lpCurTex;

	lwITex* m_pTex;
};

/************************************************************************/
/* */
/************************************************************************/
class CTexFrame {
public:
	CTexFrame();
	~CTexFrame();

public:
	void GetCoordFromModel(CEffectModel* pCModel);

	void AddTexture(const s_string& pszName);

	lwITex* GetCurTexture(WORD& wCurIndex, float& fCurTime, float fDailTime);

	void Remove();

	void Copy(CTexFrame* pList);

public:
	//
	WORD m_wTexCount;
	//
	float m_fFrameTime;
	//!
	std::vector<s_string> m_vecTexName;
	//!
	lwITex* m_lpCurTex;

	std::vector<lwITex*> m_vecTexs;

	TEXCOORD m_vecCoord;
};

class CEffectFont : public CTexList, CEffectModel {
public:
	CEffectFont();
	~CEffectFont();

public:
	bool CreateEffectFont(MPRender* pDev,
						  CMPResManger* pCResMagr, int iTexID, D3DXCOLOR dwColor, bool bUseBack = false,
						  bool bmain = false);

	void SetRenderText(char* pszText);

	void RenderEffectFont(D3DXMATRIX* pmat);
	void RenderEffectFontBack(D3DXMATRIX* pmat);

protected:
	s_string _strText;
	int _iTextNum;
	std::vector<int> _vecCurText;
	int _iTextureID;

	//
	bool _bUseBack;
	s_string _strBackBmp;
	lwITex* _lpBackTex;

	SEFFECT_VERTEX t_SEffVer[4];


	D3DXCOLOR _dwColor;
};

enum EFFECT_TYPE {
	EFFECT_NONE = 0,
	EFFECT_FRAMETEX = 1,
	EFFECT_MODELUV = 2,
	EFFECT_MODELTEXTURE = 3,
	EFFECT_MODEL = 4,
};

#define ENUMTOSTR(s)  #s

/************************************************************************/
/* Effect base class */
/************************************************************************/
class I_Effect {
public:
	I_Effect(void);
	~I_Effect(void);

public:
	void DestroyTobMesh(CMPResManger* resMgr);

	//!
	virtual void ReleaseAll();
	//!
	virtual void Reset();
	//
	virtual void Init(MPRender* pDev, EFFECT_TYPE eType,
					  WPARAM wParam, LPARAM lParam);

	void SetTexture();
	//{
	//	if(m_pCModel->IsItem())
	//	{
	//		lwITex* tex;
	//		m_pCModel->ResetItemTexture(m_CTextruelist.m_pTex,&tex);
	//	}else
	//		m_pDev->SetTexture(0, m_CTextruelist.m_lpCurTex);
	//}
	void SetVertexShader(); //		{ m_pDev->SetVertexShader(*_pdwVShader);}
	//!
	virtual void Begin() {
		if (m_pCModel)
			m_pCModel->Begin();
	}

	virtual void Render();
	//{
	//	m_pDev->SetRenderState( D3DRS_SRCBLEND,_eSrcBlend );
	//	m_pDev->SetRenderState( D3DRS_DESTBLEND,_eDestBlend);
	//	m_pCModel->RenderModel();
	//}
	virtual void End() {
		if (m_pCModel)
			m_pCModel->End();
	}

	//!
public:
	//!
	WORD getFrameCount() {
		return _wFrameCount;
	}

	WORD setFrameCount(WORD wFrameCount) {
		return _wFrameCount = wFrameCount;
	}

	//!
	EFFECT_TYPE getType() {
		return _eEffectType;
	}

	void setType(EFFECT_TYPE eType) {
		_eEffectType = eType;
	}

	//!
	float getLength() {
		return _fLength;
	}

	void setLength(float fLength) {
		_fLength = fLength;
	}

	//!
	float getFrameTime(WORD wIndex) {
		return _vecFrameTime[wIndex];
	}

	void setFrameTime(WORD wIndex, float fTime) {
		_vecFrameTime[wIndex] = fTime;
	}

	//!
	D3DXVECTOR3 getFrameSize(WORD wIndex) {
		return _vecFrameSize[wIndex];
	}

	void setFrameSize(WORD wIndex, D3DXVECTOR3& SVerSize) {
		_vecFrameSize[wIndex] = SVerSize;
	}

	//!
	D3DXVECTOR3& getFrameAngle(WORD wIndex) {
		return _vecFrameAngle[wIndex];
	}

	void setFrameAngle(WORD wIndex, D3DXVECTOR3& SVerAngle) {
		_vecFrameAngle[wIndex] = SVerAngle;
	}

	//!
	D3DXVECTOR3& getFramePos(WORD wIndex) {
		return _vecFramePos[wIndex];
	}

	void setFramePos(WORD wIndex, D3DXVECTOR3& SVerPos) {
		_vecFramePos[wIndex] = SVerPos;
	}

	//!
	D3DXCOLOR& getFrameColor(WORD wIndex) {
		return _vecFrameColor[wIndex];
	}

	void setFrameColor(WORD wIndex, D3DXCOLOR& SVerColor) {
		_vecFrameColor[wIndex] = SVerColor;
	}

	//!
	float getFrameCoordTime() {
		return m_CTexCoordlist.m_fFrameTime;
	}

	void setFrameCoordTime(float fTime) {
		m_CTexCoordlist.m_fFrameTime = fTime;
	}

	int getFrameCoordCount() {
		return (int)m_CTexCoordlist.m_vecCoordList.size();
	}

	void setFrameCoordCount(int iNum) {
		m_CTexCoordlist.m_wCoordCount = iNum;
		m_CTexCoordlist.m_vecCoordList.resize(m_CTexCoordlist.m_wCoordCount);
	}

	//!
	void getFrameCoord(TEXCOORD& vecOutCoord, WORD wIndex) {
		vecOutCoord.clear();
		vecOutCoord.resize(m_CTexCoordlist.m_wVerCount);
		for (WORD n = 0; n < m_CTexCoordlist.m_wVerCount; ++n) {
			vecOutCoord[n] = m_CTexCoordlist.m_vecCoordList[wIndex][n];
		}
	}

	void setFrameCoord(TEXCOORD& vecInCoord, WORD wIndex) {
		//for(WORD n = 0; n < m_CTexCoordlist.m_wVerCount; ++n)
		//{
		//	m_CTexCoordlist.m_vecCoordList[wIndex][n] = vecInCoord[n];
		//}
		m_CTexCoordlist.m_vecCoordList[wIndex] = vecInCoord;
	}

	//!
	TEXCOORD& getFrameTexture(WORD wIndex) {
		return m_CTextruelist.m_vecTexList[wIndex];
	}

	void setFrameTexture(WORD wIndex, TEXCOORD& lptex) {
		m_CTextruelist.m_vecTexList[wIndex] = lptex;
	}

	void SpliteTexture(int iRow, int iCol);
	void SetTextureTime(float ftime);

	///////////////////////////////////////////////////////////
	bool IsModelRect() {
		return m_strModelName == MESH_RECT;
	}

	bool IsModelPlaneRect() {
		return m_strModelName == MESH_PLANERECT;
	}

	bool IsModelTri() {
		return m_strModelName == MESH_TRI;
	}

	bool IsItem();

	bool IsChangeably();

	void GetLerpSize(D3DXVECTOR3* pSOut, WORD wIdx1, WORD wIdx2, float fLerp);
	void GetLerpAngle(D3DXVECTOR3* pSOut, WORD wIdx1, WORD wIdx2, float fLerp);
	void GetLerpPos(D3DXVECTOR3* pSOut, WORD wIdx1, WORD wIdx2, float fLerp);
	void GetLerpColor(D3DXCOLOR* pSOut, WORD wIdx1, WORD wIdx2, float fLerp);

	//!
	void GetLerpCoord(S_BVECTOR<D3DXVECTOR2>& vecOutCoord, WORD& wCurIndex, float& fCurTime, float fDailTime) {
		m_CTexCoordlist.GetCurCoord(vecOutCoord, wCurIndex, fCurTime, fDailTime);
	}

	void GetLerpTexture(S_BVECTOR<D3DXVECTOR2>& vecOutCoord, WORD& wCurIndex, float& fCurTime, float fDailTime) {
		m_CTextruelist.GetCurTexture(vecOutCoord, wCurIndex, fCurTime, fDailTime);
	}

	void GetLerpFrame(WORD& wCurIndex, float& fCurTime, float fDailTime) {
		m_CTexFrame.GetCurTexture(wCurIndex, fCurTime, fDailTime);
	}

	void GetLerpVertex(WORD wIdx1, WORD wIdx2, float fLerp) {
		m_ilast = wIdx1;
		m_inext = wIdx2;
		m_flerp = fLerp;
	}

	void GetRotaLoopMatrix(D3DXMATRIX* pmat, float& pCurRota, float fTime);

	//////////////////////////////////////////////////////////////////////////
	//!
	void BindingResInit(CMPResManger* m_CResMagr);
	//!0123shade
	int BoundingRes(CMPResManger* m_CResMagr, const char* pszParentName = "temp");

	s_string GetTextureName() {
		return m_CTextruelist.m_vecTexName;
	}

	void SetTextureName(const s_string& pszName) {
		m_CTextruelist.SetTextureName(pszName);
	}

	void SetModel(CEffectModel* pCModel);
	//
	void ChangeTexture(const s_string& pszName) {
		if (_eEffectType == EFFECT_FRAMETEX) {
		}
		else {
			SetTextureName(pszName);
			m_CTextruelist.GetTextureFromModel(m_pCModel);
		}
	}

	//!
	void ChangeModel(CEffectModel* pCModel, CMPResManger* pCResMagr);

	///////////////////////////////////////////////////////////////////////////
	//!
	s_string& getEffectName() {
		return m_strEffectName;
	}

	void setEffectName(const s_string& strName) {
		m_strEffectName = strName;
	}

	//!
	s_string& getEffectModelName();

	void setEffectModelName(const s_string& strModelName) {
		m_strModelName = strModelName;
	}

	//!BILLBOARD
	void setBillBoardMatrix(D3DXMATRIX* pMatBBoard) {
		_SpmatBBoard = pMatBBoard;
	}

	D3DXMATRIX* getBillBoardMatrix() {
		return _SpmatBBoard;
	}

	bool IsBillBoard() {
		return _bBillBoard;
	}

	void SetLoop(bool bloop) {
		_bRotaLoop = bloop;
	}

	bool IsRotaLoop() {
		return _bRotaLoop;
	}

	D3DXVECTOR4* GetRotaLoop() {
		return &_vRotaLoop;
	}

	bool IsAlpah() {
		return _bAlpha;
	}

	void EnableAlpha(bool balpha) {
		_bAlpha = balpha;
	}


	D3DBLEND GetAlphaTypeSrc() {
		return _eSrcBlend;
	}

	D3DBLEND GetAlphaTypeDest() {
		return _eDestBlend;
	}

	void SetAlphaType(D3DBLEND eSrcBlend, D3DBLEND eDestBlend) {
		_eSrcBlend = eSrcBlend;
		_eDestBlend = eDestBlend;
	}

	//!
	bool SaveToFile(FILE* pFile);
	//!
	bool LoadFromFile(FILE* pFile, DWORD dwVersion);

	void IsSame();

	void RemoveTexBack() {
		m_CTextruelist.Remove();
	}

	void AddFrameTex(const s_string& str) {
		if (_eEffectType == EFFECT_FRAMETEX) {
			m_CTexFrame.AddTexture(str);
		}
	}

	VEC_string& GetFrameTex() {
		//if(_eEffectType == EFFECT_FRAMETEX)
		{
			return m_CTexFrame.m_vecTexName;
		}
	}

	void RemoveFrameTex() {
		m_CTexFrame.Remove();
	}

	void InitTopParam() {
		if (_iUseParam > 0)
			return;
		_iUseParam = 1;
	}

	void ClearTopParam() {
		_iUseParam = 0;
	}

	void SetTobParam(int nFrame, int nSegments, float rHeight, float rRadius, float rBotRadius);
	void GetTobParam(int nFrame, int& nSegments, float& rHeight, float& rRadius, float& rBotRadius);

	int IsUseParam() {
		return _iUseParam;
	}

	bool IsRotaBoard() {
		return _bRotaBoard;
	}

	void SetRoatBoard(bool bRota) {
		_bRotaBoard = bRota;
	}


	void GetRes(CMPResManger* pCResMagr, std::vector<INT>& vecTex, std::vector<INT>& vecModel);

	void PlayModel() {
		if (!m_pCModel) return;
		if (m_pCModel->IsItem())
			m_pCModel->PlayDefaultAnimation();
	}

	void ResetModel();
	void CopyEffect(I_Effect* pEff);

	void DeleteItem(CMPResManger* pResMgr);

	//
public:
	//!3D
	MPRender* m_pDev;
	//!
	CTexCoordList m_CTexCoordlist;
	//!
	CTexList m_CTextruelist;
	//!
	CTexFrame m_CTexFrame;

	CEffectModel* m_pCModel;
	s_string m_strModelName;

	int m_nSegments;
	float m_rHeight;
	float m_rRadius;
	float m_rBotRadius;

	s_string m_strEffectName;

	int m_ilast, m_inext;
	float m_flerp;

protected:
	//!
	EFFECT_TYPE _eEffectType;
	//()
	float _fLength;
	//
	WORD _wFrameCount;
	//!
	VEC_float _vecFrameTime;
	//!	(1.0f)
	std::vector<D3DXVECTOR3> _vecFrameSize;
	//!
	std::vector<D3DXVECTOR3> _vecFrameAngle;
	//!
	std::vector<D3DXVECTOR3> _vecFramePos;

	//!	(0xffffffff)
	std::vector<D3DXCOLOR> _vecFrameColor;

	//!
	INT _iUseParam;
	std::vector<ModelParam> _CylinderParam;

	bool _bBillBoard;
	D3DXMATRIX* _SpmatBBoard;

	bool _bRotaLoop;
	D3DXVECTOR4 _vRotaLoop;

	bool _bRotaBoard;


	bool _bAlpha;

	int _iVSIndex;


	bool _bSizeSame;
	bool _bAngleSame;
	bool _bPosSame;
	bool _bColorSame;

	D3DBLEND _eSrcBlend;
	D3DBLEND _eDestBlend;
};

//class CEffectBase
//{
//public:
//	CEffectBase(){}
//	~CEffectBase(){}
//public:
//	virtual void FrameMove(DWORD	dwDailTime)	{}
//	//!
//	virtual void Render()						{}
//
//	virtual void RenderVS()						{}
//
//	virtual void RenderSoft()					{}
//
//
//protected:
//private:
//};

void Transpose(D3DMATRIX& result, D3DMATRIX& m);
