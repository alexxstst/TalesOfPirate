// Scene , , , , , , 
// , Scene, Scene, Scene
// GameAppSwitchScene(),  SceneSwitchMap
#pragma once
#include "Script.h"
#include "MusicRecordStore.h"
#include "BoolSet.h"
#include "SceneSign.h"

#define SCENEMSG_CHA_CREATE             1	// 1:ID,2:
#define SCENEMSG_CHA_DESTROY            2	//  1:ID,2:
#define SCENEMSG_CHA_BEGINMOVE          3	// 1:,2:ID
#define SCENEMSG_CHA_ENDMOVE            4	// 

#define SCENEMSG_SCENEOBJ_CREATE        6	// 1:ID,2:
#define SCENEMSG_SCENEOBJ_DESTROY       7	// 1:ID,2:
#define SCENEMSG_SCENEOBJ_MOVE	        8	// 1:ID,2:

#define SCENEMSG_EFFECT_HIT		        9	// 
#define SCENEMSG_EFFECT_VALID	        10	// ,1:ID,
#define SCENEMSG_EFFECT_CREATE	        11	// 

#define SCENEMSG_SCENEITEM_CREATE       12	// 1:ID,2:item
#define SCENEMSG_SCENEITEM_DESTROY      13	// 1:ID,2:item

#define SCENEMSG_SCENEOBJ_UPDATEVALID   15  // 

struct SceneTranspObjStateDesc {
	DWORD rs_amb;
	DWORD rs_lgt;
	BOOL light_enable[3];
	D3DLIGHTX light[3];
};

// Added by clp
typedef struct ReallyBigObjectInfo {
	int typeID;
	D3DXVECTOR3 position;
	D3DXQUATERNION orientation;
	float terrainHeight;
} ReallyBigObjectInfo;

std::ostream& operator <<(std::ostream& os, const ReallyBigObjectInfo& info);
std::istream& operator >>(std::istream& is, ReallyBigObjectInfo& info);
bool operator <(const ReallyBigObjectInfo& info1, const ReallyBigObjectInfo& info2);
void operator <<(FILE* file, const ReallyBigObjectInfo& info);

struct stSceneInitParam {
	stSceneInitParam() : nUITemplete(-1), nMaxCha(0), nMaxObj(0), nMaxItem(0), nMaxEff(0), nTypeID(0) {
	}

	std::string strName; //
	int nTypeID; //
	std::string strMapFile; //
	int nUITemplete; // UI

	int nMaxCha; // ,,
	int nMaxObj; //
	int nMaxItem; //
	int nMaxEff; //
};


enum {
	SCENEOBJ_TYPE_NORMAL = 0,
	SCENEOBJ_TYPE_POSE,
	SCENEOBJ_TYPE_TERRAIN,
	SCENEOBJ_TYPE_POINTLIGHT,
	SCENEOBJ_TYPE_ENVLIGHT,
	SCENEOBJ_TYPE_FOG,
	SCENEOBJ_TYPE_ENVSOUND,
	MAX_SCENEOBJ_TYPE, //
};

enum e3DMouseState {
	enum3DNone, //
	enumFollow, //
	enumClick, //
};

enum eUserLevel //
{
	LEVEL_CHA_RUN = 0, //
	LEVEL_MOUSE_RUN, //
};

//
template <class T>
class FreeArray {
public:
	void ReSize(int n) {
		_list.clear();
	}

	void Push(T* p) {
		if (p) {
			_list.push_back(p);
		}
	}

	T* Pop() {
		T* p = nullptr;
		while (!_list.empty()) {
			p = static_cast<T*>(_list.back());
			if (p && !p->IsValid()) {
				_list.pop_back();
				return p;
			}
			_list.pop_back();
		}

		g_logManager.InternalLog(LogLevel::Debug, "common",
								 std::format("#####Pop Error, Capacity:{} Count:{}", _list.capacity(), _list.size()));
		return nullptr;
	}

private:
	typedef std::vector<void*> TList;
	TList _list;
};

class CCharacter;
class CSceneObj;
class CSceneItem;
struct SceneLight;
class CShadeEff;
class CEffectObj;
class CPugMgr;
class CPointTrack;
class CSMallWnd;
class CGameApp;
class CEventRecord;
class CLightEff;
class CSceneNode;
struct LoadChaInfo;
class CAniWnd;
class CStateSynchro;
class CCharacter2D;
class CBigMap;
class CEventMgr;
class CMinimap;
class CLargerMap;
struct stNetChangeChaPart;
class CSkillRecord;
#include "MapRecord.h"

class CGameScene : public CScript {
protected: // CGameApp
	friend class CGameApp;

	CGameScene(stSceneInitParam& param);
	virtual ~CGameScene();

	static bool _InitScene();
	static bool _ClearScene();

	virtual void _FrameMove(DWORD dwTimeParam);
	virtual void _Render();
	virtual void _RenderUI();

	virtual bool _MouseButtonDown(int nButton) {
		return false;
	}

	virtual bool _MouseButtonUp(int nButton) {
		return false;
	}

	virtual bool _MouseMove(int nOffsetX, int nOffsetY) {
		return false;
	}

	virtual bool _MouseButtonDB(int nButton) {
		return false;
	}

	virtual bool _MouseScroll(int nScroll) {
		return false;
	}

	virtual bool _HandleSuperKey();

	virtual void _KeyDownEvent(int key) {
	}

	virtual void _KeyUpEvent(int key) {
	}

private: //
	bool _CreateMemory();
	bool _ClearMemory();

protected: //
	virtual bool _Init();
	virtual bool _Clear();

public:
	void RegisterFunc();

	const int GetSceneTypeID() const {
		return _stInit.nTypeID;
	}

public:
	virtual void LoadingCall(); // loading,
	virtual void SetMainCha(int nChaID);

	virtual void SetScreen(int w, int h, bool IsFull) {
	}

	BOOL GetPickPos(int nPosX, int nPosY, D3DXVECTOR3& vPickPos) {
		if (_pTerrain == nullptr)
			return 0;

		return _pTerrain->GetPickPos(nPosX, nPosY, vPickPos);
	}

public:
	CCharacter* SearchByID(unsigned long id);
	CCharacter* SearchByHumanID(unsigned long id);
	CCharacter* SearchByName(const char* name);
	CCharacter* SearchByHumanName(const char* name);
	CSceneItem* SearchItemByID(unsigned long id);
	CSceneItem* FilterItemsByItemID(unsigned long id, bool shouldHide);

	static void ShowMinimap(BOOL bShow);

	static BOOL GetIsShowMinimap() {
		return _bShowMinimap;
	}

	static void Set3DMouseState(e3DMouseState v) {
		_e3DMouseState = v;
	}

	MPTerrain* GetTerrain() {
		return _pTerrain.get();
	}

	stSceneInitParam* GetInitParam() {
		return &_stInit;
	}

	const char* GetTerrainName() {
		return _stInit.strMapFile.c_str();
	}

	bool GetIsBlockWalk(CCharacter* pCha, int nX, int nY); // X,Y

	CEventMgr* GetEventMgr() {
		return _pEventMgr.get();
	}

	void SetupVertexFog(MPIDeviceObject* dev_obj, float Start, float End, DWORD Color, DWORD Mode, BOOL UseRange,
						FLOAT Density);

public:
	bool SwitchMap(int nMapID);

	CMinimap* GetSmallMap() {
		return _pSmallMap;
	}

	void Reload();

	CCharacter* GetSelectCha() {
		return _pSelCha;
	}

	CCharacter* SelectCharacter();
	CCharacter* AddCharacter(const LoadChaInfo* info);
	CCharacter* AddBoat(stNetChangeChaPart& part);
	CCharacter* AddCharacter(int nScriptID); //
	CCharacter* GetCha(int nArrayID); // ID()
	int GetChaCnt() {
		return _nChaCnt;
	}

	static void ResetMainCha() {
		_pMainCha = nullptr;
	}

	static CCharacter* GetMainCha() {
		return _pMainCha;
	}

	CSceneObj* AddSceneObj(int nScriptID);

	// nType:
	//  0:; 1:1223142
	CSceneItem* AddSceneItem(int nScriptID, int nType);
	CSceneItem* AddSceneItem(const char* file);

	CSceneObj* GetSceneObj(int nArrayID);
	CSceneItem* GetSceneItem(int nArrayID);

	int GetSceneObjCnt() {
		return _nSceneObjCnt;
	}

	int GetSceneItemCnt() {
		return _nSceneItemCnt;
	}

	CSceneItem* HitSceneItemText(int nScrX, int nScry); //
	CSceneItem* HitTestSceneItem(int nScrX, int nScry);
	CSceneObj* HitTestSceneObj(int nScrX, int nScrY);
	CCharacter* HitTestCharacter(int nScrX, int nScrY);
	CCharacter* HitCharacter(int nScrX, int nScrY);
	int HitTestSceneObjTerrainForInfluence(D3DXVECTOR3* t_pos, const D3DXVECTOR3* nPos);
	int HitTestSceneObjTerrain(D3DXVECTOR3* t_pos, const D3DXVECTOR3* nPos);
	int HitTestSceneObjTerrain(D3DXVECTOR3* t_pos, const D3DXVECTOR3* nOrg, const D3DXVECTOR3* nRay);
	int HitTestSceneObjChair(D3DXMATRIX* t_mat, int* h, const D3DXVECTOR3* nOrg, const D3DXVECTOR3* nRay);
	int HitTestSceneObjWall(D3DXMATRIX* t_mat, const D3DXVECTOR3* nOrg, const D3DXVECTOR3* nRay);
	int HitTestSceneObjChair(D3DXVECTOR3* t_pos, int* t_angle, int* h, const D3DXVECTOR3* nOrg,
							 const D3DXVECTOR3* nRay);
	int HitTestSceneObjWall(D3DXVECTOR3* t_pos, int* t_angle, const D3DXVECTOR3* nOrg, const D3DXVECTOR3* nRay);
	int GetMainChaPickRay(MPVector3* org, MPVector3* ray);

	int UpdateSceneAnimLight();
	int BeginUpdateSceneObjLight(const CSceneObj* obj, BOOL is_enable);
	int EndUpdateSceneObjLight(const CSceneObj* obj);
	int SetTextureLOD(DWORD level);

	void SetEnvColor(D3DCOLORVALUE* color) {
		m_dwEnvColor = *color;
	}

	void GetEnvColor(D3DCOLORVALUE* color) {
		*color = m_dwEnvColor;
	}

	CEffectObj* HitTestEffectObj(int nScrX, int nScrY);
	//BOOL			AddItem(CItem *pItem);
	//BOOL			AddEffect(CEffect *pEffect);
	void ShowSceneObjTerrain(BOOL bShow);

	BOOL IsSceneObjTerrainVisible() {
		return _bShowTerrain;
	}

	BOOL EnableCamDrag(BOOL bEnable) {
		_bEnableCamDrag = bEnable;
	}

	BOOL IsEnableCamDrag() {
		return _bEnableCamDrag;
	}

	CEffectObj* AddSceneEffect(int nEffectTypeID);
	CEffectObj* GetFirstInvalidEffObj();
	CEffectObj* GetEffect(int nArrayID);
	void RenderEffect();

	int GetSceneEffCnt() {
		return _nEffCnt;
	}

	void UnhideAllSceneObj();

	CShadeEff* GetShadeObj(int nArrayID);
	CShadeEff* GetFirstInvalidShadeObj();
	void RenderEffectMap();

	void HandleSceneMsg(int nMsgType, int nParam1 = 0, int nParam2 = 0, int nParam3 = 0);
	void ClearSceneEffect();
	void EnableChaTexLinearFilter(BOOL flag);

	void ShowSceneObj(BOOL bShow) {
		_bShowSceneObj = bShow;
	}

	BOOL IsSceneObjVisible() {
		return _bShowSceneObj;
	}

	void ResetAllSceneObjPos();
	int GetValidSceneObjCnt();

	void RenderSMallMap();

	void ShowChairObj(int show) {
		_nShowChair = show;
	}

	int IsShowChairObj() const {
		return _nShowChair;
	}

	bool LoadMap(const char* file);

	//@lemon add for search path;
	//BYTE*			GetTempTerrain(int iCurX, int iCurY);
	// xuedong 2004.08.23 for add character block
	long AddCharacterBlock(int nCurX, int nCurY, int nDist, BYTE* byBlockBuff, int nBuffWidth, int sRadii = 40);

	D3DXVECTOR3* GetMouseMap() {
		return &_vMousePos;
	}

	float GetMouseMapX() {
		return _vMousePos.x;
	}

	float GetMouseMapY() {
		return _vMousePos.y;
	}

	int GetMouseX() {
		return _nMouseX;
	}

	int GetMouseY() {
		return _nMouseY;
	}

	float GetTerrainHeight(float fX, float fY); // ,,
	float GetGridHeight(float fX, float fY); // ,,

	int GetGridRegion(int x, int y); // ,:

	BOOL IsPointVisible(float fX, float fY);

	void EnableSceneObjCulling(BOOL bEnable) {
		_bEnableSceneObjCulling = bEnable;
	}

	BOOL IsSceneObjCulling() {
		return _bEnableSceneObjCulling;
	}

	void SetFrameMove(DWORD dwTime);

	void PlayEnvSound(int nX, int nY); // x,y,
	static void PlayEnvSound(char* szFile, int nX, int nY); //
	static void PlayEnvSound(int nSoundNo, int nX, int nY); //

	static void SetSoundSize(float fVol); // 0~1,0,1
	static float GetSoundSize() {
		return (float)_fSoundSize / 128.0f;
	}

	void OnLostDevice();
	void OnResetDevice();

	CBoolSet& GetUseLevel() {
		return _UserLeve;
	}

	void RefreshLevel();

	//
	CEffectObj* CreateEffect(int nEffectID, int nX, int nY, bool isLoop = false);

	CPugMgr* GetPugMgr() {
		return _pcPugMgr.get();
	}

	BYTE GetTileTexAttr(int ix, int iy);

	CSceneSign* GetSign() {
		return &_cSceenSign;
	}

	CLargerMap* GetLargerMap() {
		return _pLargerMap;
	}

	void ShowTerrain(bool bShow) {
		m_bShowTerrain = bShow;
	}

	CMapInfo* GetCurMapInfo() {
		return _pMapInfo;
	}

public:
	//CCharacter2D*	GetTeamList(int idx){ return _pCha2D[idx]; }

	void AddAreaEff(CEffectObj* pEffectObj);
	bool DelAreaEff(long nAreaID, int nEffectID = 0); // nEffectID=0,

public: //
	DWORD m_dwValidChaCnt;
	DWORD m_dwChaPolyCnt;
	DWORD m_dwChaRenderTime;

	DWORD m_dwValidSceneObjCnt;
	DWORD m_dwRenderSceneObjCnt;
	DWORD m_dwSceneObjPolyCnt;
	DWORD m_dwSceneObjRenderTime;
	DWORD m_dwCullingTime;
	DWORD m_dwValidEffCnt;

	D3DCOLORVALUE m_dwEnvColor;
	SceneTranspObjStateDesc* _TranspObjStateSeq;
	DWORD _dwTranspObjStateNum;
	DWORD _dwTranspObjStateID;

	bool m_bShowTerrain;

protected:
	void _CreateChaArray(int nChaCnt); //
	void _CreateSceneObjArray(int nObjCnt); //
	void _CreateSceneItemArray(int nObjCnt); //
	void _CreateEffectArray(int nEffCnt);
	void _CreateShadeArray(int nShadeCnt); //
	void _ClearAllCha();
	void _ClearAllSceneObj();
	void _ClearAllSceneItem();
	void _ClearAllEff();
	void _ClearAllShade();

	CCharacter* _GetFirstInvalidCha();
	CSceneObj* _GetFirstInvalidSceneObj(int nTypeID, BOOL& bCreate);
	CSceneItem* _GetFirstInvalidSceneItem();

	const stSceneInitParam& _GetInitParam() const {
		return _stInit;
	}

	void _RenderUpSeaShade();

public:
	BOOL _bEnableCamDrag;
	std::unique_ptr<MPTerrain> _pTerrain;
	DWORD _dwMapID;

	CCharacter* _pChaArray{};
	CSceneObj* _pSceneObjArray{};
	CSceneItem* _pSceneItemArray{};
	CEffectObj* _pEffectArray{};
	CShadeEff* _pShadeArray{};

	FreeArray<CCharacter> _free_chas;
	FreeArray<CSceneItem> _free_items;
	FreeArray<CEffectObj> _free_effs;

	std::unique_ptr<CPugMgr> _pcPugMgr;

	SceneLight* _pSceneLightArray{};
	BOOL _bLightEnable[3]; // 0: parallel light, 1&2: point light
	std::list<int> _SceneObjIdx[MAX_SCENEOBJ_TYPE];

	//lemon add@2004.10.29
	S_BVECTOR<int> _vecTempShade;

	std::size_t _nShadeCnt{};
	std::size_t _nEffCnt{};
	std::size_t _nChaCnt{};
	std::size_t _nSceneObjCnt{};
	std::size_t _nSceneItemCnt{};
	std::size_t _nSceneLightCnt{};

	D3DXVECTOR3 _vMousePos;
	int _nMouseX, _nMouseY; // ,

	static e3DMouseState _e3DMouseState;
	static BOOL _bShowMinimap;
	//#if 0
	//	static CSMallWnd    *_pSmallMap;
	//#else
	static CMinimap* _pSmallMap;
	static CLargerMap* _pLargerMap;
	//#endif
	static bool _IsShowPath;
	static float _fSoundSize; //
	static bool _IsUseSound;

	BOOL _bShowTerrain;
	DWORD _dwChaTEXFilter;
	BOOL _bShowSceneObj;
	BOOL _bShowSceneItem;
	BOOL _bEnableSceneObjCulling;

	int _nShowChair;
	POINT _HitBuf;

protected: //
	stSceneInitParam _stInit;
	int _nSceneTypeID;

protected:
	static CCharacter* _pMainCha;
	static CBigMap* _pBigMap;
	//static CCharacter2D*	_pCha2D[4];

	CCharacter* _pSelCha;
	CBoolSet _UserLeve; //

	BOOL _IsShowItemName; //
	CSceneItem* _pMouseInItem; // ,

	//
	typedef std::list<CEffectObj*> areaeffs;
	areaeffs _areaeffs;

	CSceneSign _cSceenSign;
	std::unique_ptr<CEventMgr> _pEventMgr; //std::make_unique<CEventMgr>(this)
	CMapInfo* _pMapInfo;

protected: //
	D3DXVECTOR3 org, ray;

	IDirect3DSurfaceX* windowRenderTarget;
	IDirect3DSurfaceX* windowDepthSurface;
	IDirect3DDeviceX* m_pDev;
	//Pixel shader ID
	DWORD psID;

public:
	// Added by clp, RBO = really big object
	typedef std::set<CSceneObj*>::iterator RBOIterator;

	void AddRBO(CSceneObj* object) {
		_reallyBigObjectList.insert(object);
	}

	BOOL IsInRBOList(CSceneObj* object) {
		return (_reallyBigObjectList.find(object) !=
			_reallyBigObjectList.end());
	}

	RBOIterator Begin_RBO() {
		return _reallyBigObjectList.begin();
	}

	RBOIterator End_RBO() {
		return _reallyBigObjectList.end();
	}

private:
	std::set<CSceneObj*> _reallyBigObjectList;
	void _RecordRBO();
	void _ReadRBO();
};

void SetupVertexFog(MPIDeviceObject* dev_obj, float Start, float End, DWORD Color, DWORD Mode, BOOL UseRange,
					FLOAT Density);
