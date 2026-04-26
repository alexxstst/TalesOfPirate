#pragma once

#include "Scene.h"

#include "lwInterface.h"
#include "lwMath.h"
#include "lwITypes.h"
#include "lwShaderTypes.h"
#include "lwIUtil.h"
#include "ShaderLoad.h"
#include "uiimage.h"
#include "uimemo.h"

const int MAX_CHA_NUM = 4;
const int MAX_HAIR_NUM = 8;
const int MAX_FACE_NUM = 8;
const int MAX_CITY_NUM = 3;
const int CITY_PICTURE_NUM = 4;

LW_USING;
class CSceneObj;

namespace GUI {
	class CCompent;
	class CForm;
	class C3DCompent;
	class CGuiData;
	class CLabelEx;
	class CEdit;
	class CTextButton;
	class CList;
	class CMemo;
	class CLabel;
	class CTextButton;
	class CImage;
}

class CSelectChaScene;

/**
 * copy from Jack
 *
 * @author: Michael Chen
 * @date: 2005-04-26
 */
class LoginScene_CreateCha {
public:
	DWORD _model_type; // lxo: 1, lmo: 2
	lwIResourceMgr* _res_mgr;
	lwINodeObject* _model_lxo;
	lwIModel* _model_lmo;
	lwIItem* _arrow;

	DWORD _cha_num;
	DWORD _act_num;
	lwINodePrimitive* _cha_obj[10];
	DWORD _act_obj[4];
	lwVector3 _arrow_offset_pos[4];
	IgnoreStruct ignoreStruct;

	lwIItem* _ArrowMarks[4];
	lwVector3 _ArrowMarksOffset[4];

private:
	int _InitChaObjSeq();
	int _CreateArrowItem(const char* file);

public:
	LoginScene_CreateCha()
		: _res_mgr(0), _model_lxo(0), _model_lmo(0), _model_type(0), _arrow(0) {
		_cha_num = 0;
		memset(_cha_obj, 0, sizeof(_cha_obj));
		for (int Index = 0; Index < 4; Index++)
			_ArrowMarks[Index] = NULL;
	}

	~LoginScene_CreateCha() {
		Destroy();
	}

	void Init(lwIResourceMgr* res_mgr) {
		_res_mgr = res_mgr;
	}

	void Destroy();
	void Update();
	void Render();
	int LoadModelLXO(const char* file);
	int LoadModelLMO(const char* file);
	int LoadArrowItem(const char* file, const lwVector3* offset_pos4);
	void OnMouseMove(int flag, int x, int y);
	int OnLButtonDown(int flag, int x, int y);

	bool LoadArrowMark(const char* Filename, const lwVector3* Offsets);
};

/**
 * ,GUI.
 * CGameScene.
 *
 * @author: Michael Chen
 * @date: 2005-04-26
 */
class CCreateChaScene : public CGameScene {
public:
	CCreateChaScene(stSceneInitParam& param);
	~CCreateChaScene();

	static CCreateChaScene& GetCurrScene();
	void CreateNewCha();
	void NewChaError(int error_no, const char* error_info);

	void setLastScene(CSelectChaScene* pkLastScene) {
		m_pkLastScene = pkLastScene;
	}

	virtual void LoadingCall(); // loading,

	CForm* frmRoleInfo; //   


protected:
	//
	virtual void _FrameMove(DWORD dwTimeParam);
	virtual void _Render();

	virtual bool _MouseMove(int nOffsetX, int nOffsetY);
	virtual bool _MouseButtonDown(int nButton);

	virtual bool _Init();
	virtual bool _Clear();

	//UI
	bool _InitUI();

	//UI 3D
	void RenderCha(int x, int y);

	//
	static void _ChaFoundFrmMouseEvent(CCompent* pSender, int nMsgType,
									   int x, int y, DWORD dwKey);
	static void _ChaCityFrmMouseEvent(CCompent* pSender, int nMsgType,
									  int x, int y, DWORD dwKey);
	static void _QuitFrmMouseEvent(CCompent* pSender, int nMsgType,
								   int x, int y, DWORD dwKey);
	static void __gui_event_left_face(CGuiData* sender,
									  int x, int y, DWORD key);
	static void __gui_event_left_hair(CGuiData* sender,
									  int x, int y, DWORD key);
	static void __gui_event_left_city(CGuiData* sender,
									  int x, int y, DWORD key);
	static void __gui_event_right_face(CGuiData* sender,
									   int x, int y, DWORD key);
	static void __gui_event_right_hair(CGuiData* sender,
									   int x, int y, DWORD key);
	static void __gui_event_right_city(CGuiData* sender,
									   int x, int y, DWORD key);
	static void __gui_event_left_rotate(CGuiData* sender,
										int x, int y, DWORD key);
	static void __gui_event_right_rotate(CGuiData* sender,
										 int x, int y, DWORD key);
	static void __gui_event_left_continue_rotate(CGuiData* sender);
	static void __gui_event_right_continue_rotate(CGuiData* sender);

	static void __cha_render_event(C3DCompent* pSender, int x, int y);

	static void _SelectCity(CCompent* pSender, int nMsgType,
							int x, int y, DWORD dwKey);

	enum eDirectType {
		LEFT = -1, RIGHT = 1,
	};

	// 
	void ChangeFace(eDirectType enumDirect = LEFT);
	void ChangeHair(eDirectType enumDirect = LEFT);
	void ChangeCity(eDirectType enumDirect = LEFT);
	void RotateChar(eDirectType enumDirect = LEFT);
	bool IsValidCheckChaName(const std::string& name);
	void DarkScene(bool isDark = true);

	void InitChaFoundFrm();
	void InitChaCityFrm();
	void InitChaData();

	void SendChaToServ();
	void GotoSelChaScene();

	void ShowChaFoundForm();

private:
	enum {
		CITY_BY_X_MIN = 8,
		CITY_BY_X_MAX = 230,
		CITY_BY_Y_MIN = 94,
		CITY_BY_Y_MAX = 333,

		CITY_SL_X_MIN = 245,
		CITY_SL_X_MAX = 450,
		CITY_SL_Y_MIN = 254,
		CITY_SL_Y_MAX = 373,

		CITY_BL_X_MIN = 49,
		CITY_BL_X_MAX = 452,
		CITY_BL_Y_MIN = 29,
		CITY_BL_Y_MAX = 117,

		CITY_BY_INDEX = 1,
		CITY_SL_INDEX = 2,
		CITY_BL_INDEX = 3,
	};

	int GetCityZone(int x, int y);
	void ShowCityZone(int index);

private:
	// 3D 
	LoginScene_CreateCha m_LoginSceneCreateCha;

	// UI 

	// 
	static CForm* frmChaFound;
	static CEdit* edtName;
	static CMemo* memChaDescribe;
	static CLabel* labHair;
	static CLabel* labFace;

	// 
	static CForm* frmChaCity;
	static CMemo* memChaDescribe2;
	static CLabel* labCity;

	static CImage* imgCities[MAX_CITY_NUM + 1][CITY_PICTURE_NUM];
	static CTextButton* imgCitiesBlock[MAX_CITY_NUM];
	static int iCurrCity;
	static int m_nCurCityIndex;
	static bool bShowDialog;

	// 
	static CForm* frmQuit;

	static int nHairTestCnt[MAX_HAIR_NUM]; //
	static int nFaceTestCnt[MAX_FACE_NUM]; //
	static int nSelHairNum[MAX_CHA_NUM];
	static int nSelFaceNum[MAX_CHA_NUM];

private:
	int m_nSelChaIndex; //()
	std::string m_sName; //
	int m_nCurHairIndex; //
	int m_nCurFaceIndex; //
	int m_nChaRotate; //(-180~+180)						

	CSelectChaScene* m_pkLastScene; //

	static const int CHA_NUM = 4;
	CCharacter* m_pChaForUI[CHA_NUM];

	BYTE m_oldTexFlag;
	BYTE m_oldMeshFlag;

	bool m_bInitOnce;

	int m_iFirstMourseX;
	int m_iFirstMourseY;
	bool m_bFirstShow;
	bool m_bSameNameError;

	void ShowAllRoleInfo(int nRoleInfo);

	static const int ROLE_ALL_INFO_COUNT = 24;

	CImage* imgChaView[ROLE_ALL_INFO_COUNT];

	CForm* frmLanchInfo; //     
	CForm* frmAimiInfo; //     
	CForm* frmFelierInfo; //     
	CForm* frmCaxiusInfo; //     
	CForm* frmRoleAllInfo; //     

	CMemo* memChaDescribeUp;
	CMemo* memChaDescribeDown;

	static void _evtRoleInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtLanchInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtAimiInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtFelierInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtCaxiusInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtRoleAllInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey);
};
