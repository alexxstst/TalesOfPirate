#include "stdafx.h"

#include "createchascene.h"
#include "EncodingUtil.h"

#include "CameraCtrl.h"
#include "GameApp.h"
#include "SceneObj.h"
#include "GameConfig.h"
#include "GlobalVar.h"
#include "UIFormMgr.h"
#include "UITextButton.h"
#include "Character.h"
#include "UiTextButton.h"
#include "uiformmgr.h"
#include "UiFormMgr.h"
#include "UiForm.h"
#include "UIBoxForm.h"
#include "GameApp.h"
#include "Character.h"
#include "SceneObj.h"
#include "UiFormMgr.h"
#include "UiTextButton.h"
#include "CharacterAction.h"
#include "SceneItem.h"
#include "ItemRecord.h"
#include "PacketCmd.h"
#include "GameConfig.h"
#include "ITEMRECORD.h"
#include "Character.h"
#include "UIRender.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "uiformmgr.h"
#include "uitextbutton.h"
#include "uilabel.h"
#include "uiprogressbar.h"
#include "uiscroll.h"
#include "uilist.h"
#include "uicombo.h"
#include "uiimage.h"
#include "UICheckBox.h"
#include "uiimeinput.h"
#include "uigrid.h"
#include "uilistview.h"
#include "uipage.h"
#include "uitreeview.h"
#include "uiimage.h"
#include "UILabel.h"
#include "RenderStateMgr.h"
#include "uicompent.h"

#include "UIMemo.h"

#include "Connection.h"
#include "ServerSet.h"
#include "GameAppMsg.h"


#include "UI3DCompent.h"
#include "UIForm.h"
#include "UITemplete.h"
#include "commfunc.h"
#include "uiboxform.h"
#include "SelectChaScene.h"
#include "commfunc.h"
#include "uiTextButton.h"

using namespace std;

#define ARRAY_SIZE( array )	( sizeof( array ) / sizeof( ( array )[0] ) )

inline bool Error(const char* strInfo, const char* strFormName, const char* strCompentName) {
	{
		char _buf[512];
		snprintf(_buf, sizeof(_buf), strInfo, strFormName, strCompentName);
		g_logManager.InternalLog(LogLevel::Debug, "common", _buf);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Class: LoginScene_CreateCha (Copy from Jack)
//////////////////////////////////////////////////////////////////////////

struct PoseConfigInfo {
	DWORD pose_id;
	DWORD subset;
	DWORD stage;
	DWORD anim_type;
	DWORD play_type;
	float velocity;
};

int LoginScene_CreateCha::_CreateArrowItem(const char* file) {
	LW_IF_RELEASE(_arrow);

	_res_mgr->CreateItem(&_arrow);
	if (LW_FAILED(_arrow->Load(file)))
		return 0;

	_arrow->GetPrimitive()->SetState(STATE_VISIBLE, 0);
	_arrow->PlayDefaultAnimation(!g_stUISystem.m_sysProp.m_gameOption.bFramerate);

	return 1;
}

bool LoginScene_CreateCha::LoadArrowMark(const char* Filename, const lwVector3* Offsets) {
	memcpy(_ArrowMarksOffset, Offsets, sizeof(_ArrowMarksOffset));

	for (int Index = 0; Index < 4; Index++) {
		LW_IF_RELEASE(_ArrowMarks[Index]);

		assert(_cha_obj[Index]);

		_res_mgr->CreateItem(&(_ArrowMarks[Index]));
		if (LW_FAILED(_ArrowMarks[Index]->Load( Filename )))
			return false;

		_ArrowMarks[Index]->GetPrimitive()->SetState(STATE_VISIBLE, TRUE);
		_ArrowMarks[Index]->PlayDefaultAnimation(!g_stUISystem.m_sysProp.m_gameOption.bFramerate);

		lwMatrix44 matWorld = *(_cha_obj[Index]->GetWorldMatrix());
		lwMatrix44 matTranslate = lwMatrix44Translate(_ArrowMarksOffset[Index]);
		lwMatrix44Multiply(&matWorld, &matTranslate, &matWorld);
		_ArrowMarks[Index]->SetMatrix(&matWorld);
	}

	return true;
}

int LoginScene_CreateCha::_InitChaObjSeq() {
	const DWORD cha_num = 5;

	// init pose
	int x[5][5] =
	{
		{1, 0, 125, 500, 600},
		{2, 0, 200, 500, 600},
		{3, 0, 360, 500, 600},
		{4, 0, 500, 500, 600},
		{0, 0, 360, 500, 600},
	};

	lwModelNodeQueryInfo mnqi;
	memset(&mnqi, 0, sizeof(mnqi));
	mnqi.mask = MNQI_TYPE | MNQI_ID;
	mnqi.type = NODE_PRIMITIVE;

	for (DWORD i = 0; i < cha_num; i++) {
		mnqi.node = 0;
		mnqi.id = x[i][0];

		if (LW_FAILED(_model_lxo->QueryTreeNode(&mnqi)))
			return 0;

		_cha_obj[i] = (lwINodePrimitive*)mnqi.node->GetData();
	}

	_cha_num = cha_num;


	lwPoseInfo pi[2];
	memset(pi, 0, sizeof(pi));

	lwAnimCtrlObjTypeInfo type_info;
	type_info.type = ANIM_CTRL_TYPE_BONE;
	type_info.data[0] = LW_INVALID_INDEX;
	type_info.data[1] = LW_INVALID_INDEX;

	lwIPoseCtrl* pose_ctrl;
	lwIAnimCtrlObjBone* ctrl_obj;

	lwPlayPoseInfo play_info;
	lwPlayPoseInfo_Construct(&play_info);
	play_info.bit_mask = PPI_MASK_POSE | PPI_MASK_TYPE | PPI_MASK_VELOCITY;
	play_info.pose = 1;
	play_info.type = PLAY_LOOP;
	play_info.velocity = 1.0f;

	for (DWORD i = 0; i < _cha_num; i++) {
		lwINodeBoneCtrl* bone_ctrl = (lwINodeBoneCtrl*)_cha_obj[i]->GetParent();
		if (bone_ctrl == 0)
			return 0;

		ctrl_obj = bone_ctrl->GetAnimCtrlObj();
		if (ctrl_obj == 0)
			return 0;

		pose_ctrl = ctrl_obj->GetAnimCtrl()->GetPoseCtrl();

		pi[0].start = x[i][1];
		pi[0].end = x[i][2];
		pi[1].start = x[i][3];
		pi[1].end = x[i][4];

		pose_ctrl->InsertPose(1, &pi[0], 2);

		ctrl_obj->PlayPose(&play_info);
	}

	_act_obj[0] = 0;
	_act_obj[1] = 1;
	_act_obj[2] = 2;
	_act_obj[3] = 3;
	_act_num = 4;

	return 1;
}

void LoginScene_CreateCha::Destroy() {
	LW_SAFE_RELEASE(_model_lxo);
	LW_SAFE_RELEASE(_model_lmo);
}

void LoginScene_CreateCha::Update() {
	if (_model_lxo) {
		_model_lxo->Update();
	}
	if (_model_lmo) {
		_model_lmo->Update();
	}
	if (_arrow) {
		_arrow->Update();
	}

	for (int Index = 0; Index < 4; Index++) {
		if (_ArrowMarks[Index])
			_ArrowMarks[Index]->Update();
	}

	if (_ArrowMarks[3]) {
		lwMatrix44 matWorld = *(_cha_obj[_act_obj[3]]->GetWorldMatrix());
		lwMatrix44 matTranslate = lwMatrix44Translate(_ArrowMarksOffset[3]);
		lwMatrix44Multiply(&matWorld, &matTranslate, &matWorld);
		_ArrowMarks[3]->SetMatrix(&matWorld);
	}
}

void LoginScene_CreateCha::Render() {
	if (_model_lxo) {
		_model_lxo->IgnoreNodesRender(&ignoreStruct);
	}
	if (_model_lmo) {
		_model_lmo->Render();
	}
	if (_arrow) {
		_arrow->Render();
	}

	for (int Index = 0; Index < 4; Index++) {
		if (_ArrowMarks[Index])
			_ArrowMarks[Index]->Render();
	}
}

int LoginScene_CreateCha::LoadModelLXO(const char* file) {
	_res_mgr->CreateNodeObject(&_model_lxo);

	lwAssObjInfo assinfo;
	assinfo.size = lwVector3(1.0f, 1.0f, 1.0f);
	_res_mgr->SetAssObjInfo(ASSOBJ_MASK_SIZE, &assinfo);

	_res_mgr->SetTexturePath(".\\texture\\scene\\");

	const std::string path = std::format(".\\model\\scene\\{}", file);
	_model_lxo->Load(path.c_str(), MODELOBJECT_LOAD_RESET, 0);

	lwINodeObjectA::PlayDefaultPose(_model_lxo);
	//lwINodeObjectA::ShowBoundingObject(_model_lxo, 1);

	//lwINodeObjectA::DumpObjectTree(_model_lxo, "test.txt");

	lwITreeNode* tn_bonectrl = 0;
	lwITreeNode* tn_dummy = 0;
	lwINodeBoneCtrl* node_bonectrl = 0;
	lwINodeDummy* node_dummy = 0;

	lwModelNodeQueryInfo mnqi;
	memset(&mnqi, 0, sizeof(mnqi));
	mnqi.mask = MNQI_TYPE | MNQI_ID;
	mnqi.type = NODE_BONECTRL;
	mnqi.id = 4;
	mnqi.node = 0;

	if (LW_SUCCEEDED(_model_lxo->QueryTreeNode(&mnqi))) {
		tn_bonectrl = mnqi.node;
		node_bonectrl = (lwINodeBoneCtrl*)mnqi.node->GetData();
	}

	mnqi.type = NODE_DUMMY;
	mnqi.id = 99;
	mnqi.node = 0;

	if (LW_SUCCEEDED(_model_lxo->QueryTreeNode(&mnqi))) {
		tn_dummy = mnqi.node;
		node_dummy = (lwINodeDummy*)mnqi.node->GetData();
	}

	if (node_bonectrl && node_dummy) {
		_model_lxo->RemoveTreeNode(tn_bonectrl);
		_model_lxo->InsertTreeNode(tn_dummy, 0, tn_bonectrl);
	}

	if (_InitChaObjSeq() == 0)
		return 0;
	//
	_model_type = 1;

	// Init object pose

	{
		PoseConfigInfo pci[32];
		DWORD pci_num = 13;

		//char end_pose[64];

		pci[0].pose_id = 41, pci[0].subset = -1, pci[0].stage = -1, pci[0].anim_type = 0, pci[0].play_type = 1, pci[0].
			velocity = 1.0f;
		pci[1].pose_id = 41, pci[1].subset = 0, pci[1].stage = -1, pci[1].anim_type = 4, pci[1].play_type = 1, pci[1].
			velocity = 1.0f;
		pci[2].pose_id = 54, pci[2].subset = -1, pci[2].stage = -1, pci[2].anim_type = 0, pci[2].play_type = 1, pci[2].
			velocity = 1.0f;
		pci[3].pose_id = 54, pci[3].subset = 0, pci[3].stage = -1, pci[3].anim_type = 4, pci[3].play_type = 1, pci[3].
			velocity = 1.0f;
		pci[4].pose_id = 27, pci[4].subset = 0, pci[4].stage = 0, pci[4].anim_type = 2, pci[4].play_type = 2, pci[4].
			velocity = 0.8f;
		pci[5].pose_id = 28, pci[5].subset = 0, pci[5].stage = 0, pci[5].anim_type = 2, pci[5].play_type = 2, pci[5].
			velocity = 0.5f;
		pci[6].pose_id = 106, pci[6].subset = -1, pci[6].stage = -1, pci[6].anim_type = 1, pci[6].play_type = 2, pci[6].
			velocity = 0.3f;
		pci[7].pose_id = 60, pci[7].subset = -1, pci[7].stage = -1, pci[7].anim_type = 1, pci[7].play_type = 2, pci[7].
			velocity = 0.8f;
		pci[8].pose_id = 61, pci[8].subset = -1, pci[8].stage = -1, pci[8].anim_type = 1, pci[8].play_type = 2, pci[8].
			velocity = 0.8f;
		pci[9].pose_id = 62, pci[9].subset = -1, pci[9].stage = -1, pci[9].anim_type = 1, pci[9].play_type = 2, pci[9].
			velocity = 0.8f;
		pci[10].pose_id = 63, pci[10].subset = -1, pci[10].stage = -1, pci[10].anim_type = 1, pci[10].play_type = 2, pci
			[10].velocity = 0.8f;
		pci[11].pose_id = 64, pci[11].subset = -1, pci[11].stage = -1, pci[11].anim_type = 1, pci[11].play_type = 2, pci
			[11].velocity = 0.8f;
		pci[12].pose_id = 110, pci[12].subset = -1, pci[12].stage = -1, pci[12].anim_type = 0, pci[12].play_type = 2,
			pci[12].velocity = 1.5f;
		pci[13].pose_id = 111, pci[13].subset = -1, pci[13].stage = -1, pci[13].anim_type = 0, pci[13].play_type = 2,
			pci[13].velocity = 1.5f;

		lwModelNodeQueryInfo mnqi;
		memset(&mnqi, 0, sizeof(mnqi));
		mnqi.mask = MNQI_ID | MNQI_TYPE;
		mnqi.type = NODE_PRIMITIVE;
		mnqi.node = 0;

		lwINodePrimitive* np;
		lwAnimCtrlObjTypeInfo type_info;
		lwPlayPoseInfo ppi;
		memset(&ppi, 0, sizeof(ppi));
		ppi.bit_mask = PPI_MASK_DEFAULT;
		ppi.pose = 0;
		ppi.frame = 0.0f;
		ppi.type = PLAY_ONCE;
		ppi.velocity = 0.5f;


		for (DWORD i = 0; i < pci_num; i++) {
			mnqi.id = pci[i].pose_id;

			if (pci[i].anim_type == ANIM_CTRL_TYPE_BONE) {
				mnqi.type = NODE_BONECTRL;

				if (LW_SUCCEEDED(_model_lxo->QueryTreeNode(&mnqi))) {
					lwINodeBoneCtrl* nb = (lwINodeBoneCtrl*)mnqi.node->GetData();

					lwIAnimCtrlObj* ctrl_obj = nb->GetAnimCtrlObj();
					if (ctrl_obj == 0)
						continue;

					ppi.type = pci[i].play_type;
					ppi.velocity = pci[i].velocity;
					ctrl_obj->PlayPose(&ppi);
				}
			}
			else {
				mnqi.type = NODE_PRIMITIVE;

				if (LW_SUCCEEDED(_model_lxo->QueryTreeNode(&mnqi))) {
					np = (lwINodePrimitive*)mnqi.node->GetData();

					lwIAnimCtrlAgent* anim_agent = np->GetAnimCtrlAgent();
					if (anim_agent == 0)
						continue;

					type_info.type = pci[i].anim_type;
					type_info.data[0] = pci[i].subset;
					type_info.data[1] = pci[i].stage;
					lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
					if (ctrl_obj == 0)
						continue;

					ppi.type = pci[i].play_type;
					ppi.velocity = pci[i].velocity;
					ctrl_obj->PlayPose(&ppi);
				}
			}
		}
	}
	return 1;
}

int LoginScene_CreateCha::LoadModelLMO(const char* file) {
	_res_mgr->CreateModel(&_model_lmo);

	_model_lmo->Load(file);
	lwIPrimitive* pri_sea = 0;
	DWORD sea_id = 15;

	DWORD num = _model_lmo->GetPrimitiveNum();
	for (DWORD i = 0; i < num; i++) {
		_model_lmo->GetPrimitive(i)->SetState(STATE_TRANSPARENT, 0);
		_model_lmo->GetPrimitive(i)->SetState(STATE_UPDATETRANSPSTATE, 0);

		if (_model_lmo->GetPrimitive(i)->GetID() == sea_id)
			pri_sea = _model_lmo->GetPrimitive(i);
	}

	{
		_model_lmo->PlayDefaultAnimation(!g_stUISystem.m_sysProp.m_gameOption.bFramerate);

		int pose_enable = 1;
		if (pose_enable) {
			PoseConfigInfo pci[32];
			DWORD pci_num = 6;

			pci[0].pose_id = 0, pci[0].subset = -1, pci[0].stage = -1, pci[0].anim_type = 1, pci[0].play_type = 1, pci[
				0].velocity = 0.5;
			pci[1].pose_id = 1, pci[1].subset = -1, pci[1].stage = -1, pci[1].anim_type = 1, pci[1].play_type = 1, pci[
				1].velocity = 0.5;
			pci[2].pose_id = 2, pci[2].subset = -1, pci[2].stage = -1, pci[2].anim_type = 1, pci[2].play_type = 1, pci[
				2].velocity = 0.5;
			pci[3].pose_id = 3, pci[3].subset = -1, pci[3].stage = -1, pci[3].anim_type = 1, pci[3].play_type = 1, pci[
				3].velocity = 0.5;
			pci[4].pose_id = 4, pci[4].subset = -1, pci[4].stage = -1, pci[4].anim_type = 1, pci[4].play_type = 1, pci[
				4].velocity = 0.5;
			pci[5].pose_id = 26, pci[5].subset = -1, pci[5].stage = -1, pci[5].anim_type = 1, pci[5].play_type = 1, pci[
				5].velocity = 0.5;

			lwIPrimitive* p = 0;

			lwPlayPoseInfo ppi;
			memset(&ppi, 0, sizeof(ppi));
			ppi.bit_mask = PPI_MASK_DEFAULT;
			ppi.pose = 0;
			ppi.frame = 0.0f;
			ppi.type = PLAY_ONCE;
			ppi.velocity = 0.5f;

			lwAnimCtrlObjTypeInfo type_info;

			for (DWORD i = 0; i < num; i++) {
				p = _model_lmo->GetPrimitive(i);

				for (DWORD j = 0; j < pci_num; j++) {
					if (p->GetID() == pci[j].pose_id) {
						lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
						if (anim_agent == 0)
							continue;

						type_info.type = pci[j].anim_type;
						type_info.data[0] = pci[j].subset;
						type_info.data[1] = pci[j].stage;
						lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
						if (ctrl_obj == 0)
							continue;

						ppi.type = pci[j].play_type;
						ppi.velocity = pci[j].velocity;
						ctrl_obj->PlayPose(&ppi);
					}
				}
			}
		}
	}

	//
	_model_type = 2;

	return 1;
}

int LoginScene_CreateCha::LoadArrowItem(const char* file, const lwVector3* offset_pos4) {
	if (_CreateArrowItem(file) == 0)
		return 0;

	memcpy(_arrow_offset_pos, offset_pos4, sizeof(_arrow_offset_pos));

	return 1;
}

void LoginScene_CreateCha::OnMouseMove(int flag, int x, int y) {
	if (_model_type != 1)
		return;

	lwINodePrimitive* pri;
	lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

	lwPickInfo info;
	lwVector3 org, ray;
	dev_obj->ScreenToWorld(&org, &ray, x, y);

	lwPlayPoseInfo play_info;
	lwPlayPoseInfo_Construct(&play_info);
	play_info.bit_mask = PPI_MASK_POSE | PPI_MASK_TYPE | PPI_MASK_VELOCITY | PPI_MASK_FRAME;
	play_info.type = PLAY_LOOP_SMOOTH;
	play_info.velocity = 0.5f;
	play_info.frame = 0.0f;

	BOOL octopus_flag = 0;
	BOOL dummy_flag = 0;

	if (_arrow) {
		_arrow->GetPrimitive()->SetState(STATE_VISIBLE, 0);
	}

	for (DWORD i = 0; i < _act_num; i++) {
		if ((pri = _cha_obj[_act_obj[i]]) == 0)
			continue;

		if (LW_SUCCEEDED(pri->HitTest(&info, &org, &ray))) {
			play_info.pose = 2;

			if (i == 3) {
				dummy_flag = 1;
			}
			if (i == 2) {
				octopus_flag = 1;
			}

			/*if(_arrow)
			{
				_arrow->GetPrimitive()->SetState(STATE_VISIBLE, 1);
				lwMatrix44 mat, m;
				mat = *pri->GetWorldMatrix();
				m = lwMatrix44Translate(_arrow_offset_pos[i]);
				lwMatrix44Multiply(&mat, &m, &mat);
				_arrow->SetMatrix(&mat);
			}*/
		}
		else {
			play_info.pose = 1;
		}

		lwINodeBoneCtrl* bone_ctrl = (lwINodeBoneCtrl*)pri->GetParent();
		bone_ctrl->GetAnimCtrlObj()->PlayPose(&play_info);

		if (i == 2) // octopus
		{
			lwINodeBoneCtrl* bone_ctrl = (lwINodeBoneCtrl*)_cha_obj[4]->GetParent();
			bone_ctrl->GetAnimCtrlObj()->PlayPose(&play_info);
		}
	}

	{
		// dummy 99 obj
		lwINodeDummy* node_dummy = (lwINodeDummy*)_cha_obj[_act_obj[3]]->GetParent()->GetParent();
		lwIAnimCtrlObjMat* ctrl_mat = (lwIAnimCtrlObjMat*)node_dummy->GetAnimCtrlObj();

		if (dummy_flag == 1) {
			play_info.bit_mask = PPI_MASK_TYPE | PPI_MASK_FRAME;
			play_info.frame = ctrl_mat->GetPlayPoseInfo()->frame;
			play_info.type = PLAY_FRAME;
		}
		else {
			play_info.bit_mask = PPI_MASK_TYPE;
			play_info.type = PLAY_LOOP_SMOOTH;
		}

		ctrl_mat->PlayPose(&play_info);
	}
}

int LoginScene_CreateCha::OnLButtonDown(int flag, int x, int y) {
	if (_model_type != 1)
		return -1;

	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());
	if (pCreateChaScene) {
		if (pCreateChaScene->frmRoleInfo->GetIsShow())
			return -1;
	}

	lwINodePrimitive* pri;
	lwIDeviceObject* dev_obj = _res_mgr->GetDeviceObject();

	lwPickInfo info;
	lwVector3 org, ray;
	dev_obj->ScreenToWorld(&org, &ray, x, y);

	lwPlayPoseInfo play_info;
	lwPlayPoseInfo_Construct(&play_info);
	play_info.bit_mask = PPI_MASK_POSE | PPI_MASK_TYPE | PPI_MASK_VELOCITY | PPI_MASK_FRAME;
	play_info.type = PLAY_LOOP_SMOOTH;
	play_info.velocity = 0.5f;
	play_info.frame = 0.0f;

	BOOL octopus_flag = 0;
	BOOL dummy_flag = 0;

	if (_arrow) {
		_arrow->GetPrimitive()->SetState(STATE_VISIBLE, 0);
	}

	for (DWORD i = 0; i < _act_num; i++) {
		if ((pri = _cha_obj[_act_obj[i]]) == 0)
			continue;

		if (LW_SUCCEEDED(pri->HitTest(&info, &org, &ray))) {
			return i;
		}
	}

	return -1;
}


//////////////////////////////////////////////////////////////////////////
// Class: CCreateChaScene
//////////////////////////////////////////////////////////////////////////

//~ static ====================================================================

CForm* CCreateChaScene::frmChaFound = 0;
CEdit* CCreateChaScene::edtName = 0;
CMemo* CCreateChaScene::memChaDescribe = 0;
CLabel* CCreateChaScene::labHair = 0;
CLabel* CCreateChaScene::labFace = 0;


CForm* CCreateChaScene::frmChaCity = 0;
CMemo* CCreateChaScene::memChaDescribe2 = 0;
CLabel* CCreateChaScene::labCity = 0;

CForm* CCreateChaScene::frmQuit = 0;

CImage* CCreateChaScene::imgCities[MAX_CITY_NUM + 1][CITY_PICTURE_NUM] = {
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
};

CTextButton* CCreateChaScene::imgCitiesBlock[MAX_CITY_NUM] = {0, 0, 0};

int CCreateChaScene::iCurrCity = 0;
int CCreateChaScene::m_nCurCityIndex = 0;
bool CCreateChaScene::bShowDialog = false;


const char* GetCharacterDescription(int index) {
	switch (index) {
	case 0: return GetLanguageString(35).c_str();
	case 1: return GetLanguageString(36).c_str();
	case 2: return GetLanguageString(37).c_str();
	case 3: return GetLanguageString(38).c_str();
	default: return "";
	}
}

const char* GetCityName(int index) {
	switch (index) {
	case 0: return GetLanguageString(39).c_str();
	case 1: return GetLanguageString(40).c_str();
	case 2: return GetLanguageString(41).c_str();
	default: return "";
	}
}

const char* GetCityDescription(int index) {
	switch (index) {
	case 0: return GetLanguageString(42).c_str();
	case 1: return GetLanguageString(43).c_str();
	case 2: return GetLanguageString(44).c_str();
	default: return "";
	}
}

int CCreateChaScene::nHairTestCnt[MAX_HAIR_NUM] = {2000, 2062, 2124, 2291};
int CCreateChaScene::nFaceTestCnt[MAX_FACE_NUM] = {2554, 2554, 2554, 2554};

//int CCreateChaScene::nSelHairNum[MAX_CHA_NUM] = { 61, 61, 62, 4, };
int CCreateChaScene::nSelHairNum[MAX_CHA_NUM] = {8, 8, 8, 4,};
int CCreateChaScene::nSelFaceNum[MAX_CHA_NUM] = {8, 8, 8, 8,};


//~ Constructors ==============================================================
CCreateChaScene::CCreateChaScene(stSceneInitParam& param)
	: CGameScene(param), m_bInitOnce(false), m_bSameNameError(false) {
	ToLogService("common", "CCreateChaScene Create");
}

//~ Destructors ===============================================================
CCreateChaScene::~CCreateChaScene() {
	ToLogService("common", "CCreateChaScene Destroy");
}

//~ Scene-related functions ==========================================================

//-----------------------------------------------------------------------
bool CCreateChaScene::_Init() {
	bool bResult = CGameScene::_Init();

	if (!m_bInitOnce) {
		m_bInitOnce = true;

		m_LoginSceneCreateCha.Init(g_Render.GetInterfaceMgr()->res_mgr);
		if (m_LoginSceneCreateCha.LoadModelLXO("login03.lxo") != 1) {
			return false;
		}

		lwVector3 offsetPos4[4];
		offsetPos4[0].x = 3.276f;
		offsetPos4[0].y = 25.196f;
		offsetPos4[0].z = 4.8f;

		offsetPos4[1].x = -5.013f;
		offsetPos4[1].y = 17.272f;
		offsetPos4[1].z = 5.0f;

		offsetPos4[2].x = -21.163f;
		offsetPos4[2].y = 30.951f;
		offsetPos4[2].z = 5.254f;

		offsetPos4[3].x = 0.0f;
		offsetPos4[3].y = 0.0f;
		offsetPos4[3].z = 3.0f;

		lwVector3 OffsetPos4[4];
		OffsetPos4[0].x = 0.676f;
		OffsetPos4[0].y = 28.696f;
		OffsetPos4[0].z = 2.f;

		OffsetPos4[1].x = -7.013f;
		OffsetPos4[1].y = 21.072f;
		OffsetPos4[1].z = 2.5f;

		OffsetPos4[2].x = -20.663f;
		OffsetPos4[2].y = 30.951f;
		OffsetPos4[2].z = 2.f;

		OffsetPos4[3].x = 2.f;
		OffsetPos4[3].y = 0.5f;
		OffsetPos4[3].z = 0.1f;

		m_LoginSceneCreateCha.LoadArrowItem("target.lgo", offsetPos4);
	}

	//Set camera structure
	CCameraCtrl* pCam = g_pGameApp->GetMainCam();
	if (pCam) {
		g_pGameApp->EnableCameraFollow(TRUE);
		pCam->m_EyePos.x = -32.713f;
		pCam->m_EyePos.y = 71.002f;
		pCam->m_EyePos.z = 7.006f;

		pCam->m_RefPos.x = -0.259f;
		pCam->m_RefPos.y = 0.565f;
		pCam->m_RefPos.z = 6.366f;
	}
	g_Render.SetWorldViewFOV(Angle2Radian(42.0f));
	g_Render.SetWorldViewAspect(1.0f);
	g_Render.SetClip(1.0f, 2000.0f);

	g_Render.LookAt(pCam->m_EyePos, pCam->m_RefPos);
	g_Render.SetCurrentView(MPRender::VIEW_WORLD);

	//Create four character models
	int i = 0;
	for (; i < 4; i++) {
		m_pChaForUI[i] = AddCharacter(i + 1);
		if (m_pChaForUI[i]) {
			m_pChaForUI[i]->SetIsForUI(1);
		}
		else {
			return false;
		}
	}

	lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	m_oldTexFlag = res_bs->GetValue(OPT_RESMGR_LOADTEXTURE_MT);
	m_oldMeshFlag = res_bs->GetValue(OPT_RESMGR_LOADMESH_MT);
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, 0);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, 0);

	_InitUI();

	srand((unsigned)time(NULL));

	m_bSameNameError = false;

	return bResult;
}

//-----------------------------------------------------------------------
bool CCreateChaScene::_Clear() {
	bool bResult = CGameScene::_Clear();

	lwIByteSet* res_bs = g_Render.GetInterfaceMgr()->res_mgr->GetByteSet();
	res_bs->SetValue(OPT_RESMGR_LOADTEXTURE_MT, m_oldTexFlag);
	res_bs->SetValue(OPT_RESMGR_LOADMESH_MT, m_oldMeshFlag);

	return bResult;
}

//-----------------------------------------------------------------------
void CCreateChaScene::_FrameMove(DWORD dwTimeParam) {
	CGameScene::_FrameMove(dwTimeParam);

	m_LoginSceneCreateCha.Update();

	int iMouseX = g_pGameApp->GetMouseX();
	int iMouseY = g_pGameApp->GetMouseY();
	GetRender().ScreenConvert(iMouseX, iMouseY);

	if (frmChaCity->GetIsShow() && !bShowDialog && CFormMgr::s_Mgr.ModalFormNum() == 1) {
		if (m_bFirstShow && iMouseX != m_iFirstMourseX && iMouseY != m_iFirstMourseY) {
			m_bFirstShow = false;
		}

		if (iMouseX != m_iFirstMourseX && iMouseY != m_iFirstMourseY) {
			if (m_bFirstShow)
				m_bFirstShow = false;
			else {
				int index = GetCityZone(iMouseX, iMouseY);
				ShowCityZone(index);
			}
		}
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::_Render() {
	CGameScene::_Render();

	MPIDeviceObject* dev_obj = g_Render.GetInterfaceMgr()->dev_obj;

	RenderStateMgr* rsm = g_pGameApp->GetRenderStateMgr();

	rsm->BeginScene();
	rsm->BeginSceneObject();

	dev_obj->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	dev_obj->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	dev_obj->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	dev_obj->SetRenderState(D3DRS_COLORVERTEX, TRUE);

	dev_obj->SetRenderState(D3DRS_LIGHTING, 0);
	dev_obj->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	m_LoginSceneCreateCha.Render();

	rsm->EndSceneObject();

	rsm->BeginTranspObject();
	lwUpdateSceneTransparentObject();
	rsm->EndTranspObject();


	rsm->EndScene();
}


//-----------------------------------------------------------------------
bool CCreateChaScene::_MouseMove(int nOffsetX, int nOffsetY) {
	int iMouseX = g_pGameApp->GetMouseX();
	int iMouseY = g_pGameApp->GetMouseY();
	m_LoginSceneCreateCha.OnMouseMove(0, iMouseX, iMouseY);

	return true;
}

//-----------------------------------------------------------------------
bool CCreateChaScene::_MouseButtonDown(int nButton) {
	int iMouseX = g_pGameApp->GetMouseX();
	int iMouseY = g_pGameApp->GetMouseY();

	int nIndex = m_LoginSceneCreateCha.OnLButtonDown(0, iMouseX, iMouseY);

	if (nIndex < 0 || nIndex > 3)
		return false;

	if (nIndex == 1)
		m_nSelChaIndex = 2;
	else if (nIndex == 2)
		m_nSelChaIndex = 1;
	else
		m_nSelChaIndex = nIndex;


	if (frmLanchInfo && frmAimiInfo && frmFelierInfo && frmCaxiusInfo && frmRoleAllInfo) {
		DarkScene();

		switch (m_nSelChaIndex) {
		case 0: // Lance
			frmLanchInfo->ShowModal();
			break;

		case 1: // Carsise
			frmCaxiusInfo->ShowModal();
			break;

		case 2: // Phyllis
			frmFelierInfo->ShowModal();
			break;

		case 3: // Ami
			frmAimiInfo->ShowModal();
			break;
		}

		return true;
	}

	ShowChaFoundForm();

	return true;
}


//~ UI-related functions =============================================================
bool CCreateChaScene::_InitUI() {
	// Character creation form
	{
		frmChaFound = CFormMgr::s_Mgr.Find("frmFound", GetInitParam()->nUITemplete);
		if (!frmChaFound) {
			ToLogService("common", "msgInit frmFound UI error");
			return false;
		}
		CTextButton* btnLeftHair = (CTextButton*)frmChaFound->Find("btnLeftHair");
		if (!btnLeftHair) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "btnLeftHair");
			return false;
		}
		btnLeftHair->evtMouseClick = __gui_event_left_hair;
		CTextButton* btnRightHair = (CTextButton*)frmChaFound->Find("btnRightHair");
		if (!btnRightHair) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "btnRightHair");
			return false;
		}
		btnRightHair->evtMouseClick = __gui_event_right_hair;
		CTextButton* btnLeftFace = (CTextButton*)frmChaFound->Find("btnLeftFace");
		if (!btnLeftFace) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "btnLeftFace");
			return false;
		}
		btnLeftFace->evtMouseClick = __gui_event_left_face;
		CTextButton* btnRightFace = (CTextButton*)frmChaFound->Find("btnRightFace");
		if (!btnRightFace) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "btnRightFace");
			return false;
		}
		btnRightFace->evtMouseClick = __gui_event_right_face;

		// Initially make left/right buttons flash
		btnLeftHair->SetFlashCycle();
		btnRightHair->SetFlashCycle();
		btnLeftFace->SetFlashCycle();
		btnRightFace->SetFlashCycle();


		CTextButton* btnLeft3d = (CTextButton*)frmChaFound->Find("btnLeft3d");
		if (!btnLeft3d) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "btnLeft3d");
			return false;
		}
		btnLeft3d->evtMouseClick = __gui_event_left_rotate;
		btnLeft3d->evtMouseDownContinue = __gui_event_left_continue_rotate;

		CTextButton* btnRight3d = (CTextButton*)frmChaFound->Find("btnRight3d");
		if (!btnRight3d) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "btnRight3d");
			return false;
		}
		btnRight3d->evtMouseClick = __gui_event_right_rotate;
		btnRight3d->evtMouseDownContinue = __gui_event_right_continue_rotate;

		labHair = (CLabel*)frmChaFound->Find("labHairShow");
		if (!labHair) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "labHairShow");
			return false;
		}

		labFace = (CLabel*)frmChaFound->Find("labFaceShow");
		if (!labFace) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "labFaceShow");
			return false;
		}

		edtName = (CEdit*)frmChaFound->Find("edtName");
		if (!edtName) {
			Error(GetLanguageString(45).c_str(),
				  frmChaFound->GetName(), "edtName");
			return false;
		}
		//edtName->evtChange = __gui_event_edit_change;

		memChaDescribe = (CMemo*)frmChaFound->Find("memChaDescribe");
		if (!memChaDescribe) {
			return Error(GetLanguageString(45).c_str(),
						 frmChaFound->GetName(), "memChaDescribe");
		}
		C3DCompent* ui3dCreateCha = (C3DCompent*)frmChaFound->Find("ui3dCreateCha");
		if (ui3dCreateCha) {
			ui3dCreateCha->SetRenderEvent(__cha_render_event);
		}


		frmChaFound->evtEntrustMouseEvent = _ChaFoundFrmMouseEvent;
	}

	// Character city selection form
	{
		frmChaCity = CFormMgr::s_Mgr.Find("frmCity", GetInitParam()->nUITemplete);
		if (!frmChaFound) {
			ToLogService("common", "msgInit frmCity UI error");
			return false;
		}

		frmChaCity->evtEntrustMouseEvent = _ChaCityFrmMouseEvent;
		//Load images
		for (int i(0); i < MAX_CITY_NUM + 1; ++i) {
			for (int j(0); j < CITY_PICTURE_NUM; ++j) {
				const std::string szPicName = std::format("imgCity{}{}", i, j);
				imgCities[i][j] = (CImage*)frmChaCity->Find(szPicName.c_str());
				if (!imgCities[i][j]) {
					return Error(GetLanguageString(46).c_str(),
								 frmChaCity->GetName(), szPicName.c_str());
				}
				if (i == 0)
					imgCities[i][j]->SetIsShow(true);
				else
					imgCities[i][j]->SetIsShow(false);
			} //end of for
		}
		iCurrCity = 0;

		//Load city blocks
		for (int i(0); i < MAX_CITY_NUM; ++i) {
			const std::string szPicName = std::format("imgCity{}", i + 1);
			imgCitiesBlock[i] = (CTextButton*)frmChaCity->Find(szPicName.c_str());
			if (!imgCitiesBlock[i]) {
				return Error(GetLanguageString(46).c_str(),
							 frmChaCity->GetName(), szPicName.c_str());
			}
			imgCitiesBlock[i]->SetIsShow(true); // for debug.
		}
	}

	// Exit menu form
	{
		frmQuit = CFormMgr::s_Mgr.Find("frmQuit", GetInitParam()->nUITemplete);
		if (!frmQuit) {
			ToLogService("common", "msgInit frmQuit UI error");
			return false;
		}
		frmQuit->evtEntrustMouseEvent = _QuitFrmMouseEvent;

		frmQuit->Find("imgBack")->SetIsShow(false); //Initially hide background image
		frmQuit->SetPos(
			(g_pGameApp->GetWindowWidth() - frmQuit->GetWidth()) / 2,
			g_pGameApp->GetWindowHeight() - frmQuit->GetHeight() - 40);
		frmQuit->Refresh();
		frmQuit->Show();
	}


	// Role info forms setup
	{
		frmRoleInfo = CFormMgr::s_Mgr.Find("frmRoleInfo");
		if (!frmRoleInfo) {
			ToLogService("common", "msgInit frmRoleInfo UI error");
			return false;
		}
		frmRoleInfo->evtEntrustMouseEvent = _evtRoleInfoFormMouseEvent;
		frmRoleInfo->SetIsShow(true);
		DarkScene(true);

		frmLanchInfo = CFormMgr::s_Mgr.Find("frmLanchInfo");
		if (!frmLanchInfo) {
			ToLogService("common", "msgInit frmLanchInfo UI error");
			return false;
		}
		frmLanchInfo->evtEntrustMouseEvent = _evtLanchInfoFormMouseEvent;

		frmAimiInfo = CFormMgr::s_Mgr.Find("frmAimiInfo");
		if (!frmAimiInfo) {
			ToLogService("common", "msgInit frmAimiInfo UI error");
			return false;
		}
		frmAimiInfo->evtEntrustMouseEvent = _evtAimiInfoFormMouseEvent;

		frmFelierInfo = CFormMgr::s_Mgr.Find("frmFelierInfo");
		if (!frmFelierInfo) {
			ToLogService("common", "msgInit frmFelierInfo UI error");
			return false;
		}
		frmFelierInfo->evtEntrustMouseEvent = _evtFelierInfoFormMouseEvent;

		frmCaxiusInfo = CFormMgr::s_Mgr.Find("frmCaxiusInfo");
		if (!frmCaxiusInfo) {
			ToLogService("common", "msgInit frmCaxiusInfo UI error");
			return false;
		}
		frmCaxiusInfo->evtEntrustMouseEvent = _evtCaxiusInfoFormMouseEvent;

		frmRoleAllInfo = CFormMgr::s_Mgr.Find("frmRoleAllInfo");
		if (!frmRoleAllInfo) {
			ToLogService("common", "msgInit frmRoleAllInfo UI error");
			return false;
		}
		frmRoleAllInfo->evtEntrustMouseEvent = _evtRoleAllInfoFormMouseEvent;

		// Class description MEMO
		memChaDescribeUp = dynamic_cast<CMemo*>(frmRoleAllInfo->Find("memChaDescribeUp"));
		if (!memChaDescribeUp) {
			ToLogService("common", "msgInit memChaDescribeUp UI error");
			return false;
		}
		memChaDescribeDown = dynamic_cast<CMemo*>(frmRoleAllInfo->Find("memChaDescribeDown"));
		if (!memChaDescribeDown) {
			ToLogService("common", "msgInit memChaDescribeDown UI error");
			return false;
		}

		memset(imgChaView, 0, sizeof(CImage*) * ROLE_ALL_INFO_COUNT);

		// Character class portrait images
		for (int i = 0; i < ROLE_ALL_INFO_COUNT; ++i) {
			const std::string szChaView = std::format("imgChaView{}", i + 1);
			imgChaView[i] = dynamic_cast<CImage*>(frmRoleAllInfo->Find(szChaView.c_str()));

			if (!imgChaView[i]) {
				ToLogService("common", "msgInit {} UI error", szChaView);
				return false;
			}
		}
	}

	return true;
}


//~ Callback functions =================================================================

void CCreateChaScene::_SelectCity(CCompent* pSender, int nMsgType,
								  int x, int y, DWORD dwKey) {
	bShowDialog = false;
	if (nMsgType == CForm::mrYes) {
		//Notify server to create character
		GetCurrScene().SendChaToServ();

		//		CGameApp::Waiting();
	}
	else if (nMsgType == CForm::mrNo) {
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::_ChaFoundFrmMouseEvent(CCompent* pSender, int nMsgType,
											 int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();

	if (!encoding::EqualsIgnoreCaseAscii(pSender->GetForm()->GetName(), "frmFound")) {
		return;
	}

	if (strName == "btnYes") {
		CCreateChaScene& rkScene = GetCurrScene();

		//Confirm button event

		//Validate name
		if (!rkScene.IsValidCheckChaName(edtName->GetCaption()))
			return;

		if (rkScene.m_bSameNameError) {
			//Directly send create character request

			GetCurrScene().SendChaToServ(); // Notify server to create character
			CGameApp::Waiting();
		}
		else {
			//Open city selection form

			frmChaFound->Close();
			rkScene.InitChaCityFrm(); // Sync city selection list
			frmChaCity->ShowModal(); // Show next step form
		}
	}
	else if (strName == "btnNo") {
		//Cancel button event

		//Close this form, return to scene
		frmChaFound->Close();

		//Undim the scene
		GetCurrScene().DarkScene(false);
		frmQuit->SetIsShow(true);
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::_ChaCityFrmMouseEvent(CCompent* pSender, int nMsgType,
											int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();


	if (!encoding::EqualsIgnoreCaseAscii(pSender->GetForm()->GetName(), "frmCity")) {
		return;
	}

	int iMouseX = g_pGameApp->GetMouseX();
	int iMouseY = g_pGameApp->GetMouseY();
	GetRender().ScreenConvert(iMouseX, iMouseY);

	if (frmChaCity->GetIsShow()) {
		int index = GetCurrScene().GetCityZone(iMouseX, iMouseY);

		if (CITY_BY_INDEX <= index && index <= CITY_BL_INDEX) {
			bShowDialog = true;
			GetCurrScene().m_nCurCityIndex = index - 1;
			g_stUIBox.ShowSelectBox(_SelectCity, GetCityDescription(m_nCurCityIndex), true);
		}
	}

	//if(strName=="btnYes")
	//{
	//    //Notify server to create character
	//    GetCurrScene().SendChaToServ();

	//    CGameApp::Waiting();

	//}
	if (strName == "btnNo") {
		//Close this form
		frmChaCity->Close();

		//Open previous step form
		frmChaFound->ShowModal();
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::_QuitFrmMouseEvent(CCompent* pSender, int nMsgType,
										 int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();

	if (!encoding::EqualsIgnoreCaseAscii(pSender->GetForm()->GetName(), "frmQuit")) {
		return;
	}

	if (strName == "btnNo") {
		//Exit button event
		//g_pGameApp->MsgBox("Exit button event");
		GetCurrScene().GotoSelChaScene();
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_face(CGuiData* sender,
											int x, int y, DWORD key) {
	GetCurrScene().ChangeFace(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_face(CGuiData* sender,
											 int x, int y, DWORD key) {
	GetCurrScene().ChangeFace(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_hair(CGuiData* sender,
											int x, int y, DWORD key) {
	GetCurrScene().ChangeHair(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_hair(CGuiData* sender,
											 int x, int y, DWORD key) {
	GetCurrScene().ChangeHair(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_city(CGuiData* sender,
											int x, int y, DWORD key) {
	GetCurrScene().ChangeCity(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_city(CGuiData* sender,
											 int x, int y, DWORD key) {
	GetCurrScene().ChangeCity(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_rotate(CGuiData* sender,
											  int x, int y, DWORD key) {
	GetCurrScene().RotateChar(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_rotate(CGuiData* sender,
											   int x, int y, DWORD key) {
	GetCurrScene().RotateChar(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_left_continue_rotate(CGuiData* sender) {
	GetCurrScene().RotateChar(LEFT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__gui_event_right_continue_rotate(CGuiData* sender) {
	GetCurrScene().RotateChar(RIGHT);
}

//-----------------------------------------------------------------------
void CCreateChaScene::__cha_render_event(C3DCompent* pSender, int x, int y) {
	GetCurrScene().RenderCha(x, y);
}


//~ Logic functions =================================================================

//-----------------------------------------------------------------------
void CCreateChaScene::ChangeFace(eDirectType enumDirect) {
	if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
		return;

	if (NULL == m_pChaForUI[m_nSelChaIndex]) return;

	//Get the start index of the current face type
	const long nBeginIndex = nFaceTestCnt[m_nSelChaIndex];

	// Cycle through faces
	m_nCurFaceIndex -= nBeginIndex;
	m_nCurFaceIndex += ((int)(enumDirect));
	m_nCurFaceIndex = (m_nCurFaceIndex + nSelFaceNum[m_nSelChaIndex]) % nSelFaceNum[m_nSelChaIndex];
	m_nCurFaceIndex += nBeginIndex;

	// Change face
	BOOL bOK = m_pChaForUI[m_nSelChaIndex]->ChangePart(enumEQUIP_FACE, m_nCurFaceIndex);
	if (bOK) {
		//Change display from "name+number" to just "name"
		CItemRecord* pItem = GetItemRecordInfo(m_nCurFaceIndex);
		if (pItem) {
			labFace->SetCaption(pItem->szName);
		}
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::ChangeHair(eDirectType enumDirect) {
	if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
		return;

	if (NULL == m_pChaForUI[m_nSelChaIndex]) return;

	//Get the start index of the current hair type
	const long nBeginIndex = nHairTestCnt[m_nSelChaIndex];


	// Cycle through hair styles
	m_nCurHairIndex -= nBeginIndex;
	m_nCurHairIndex += ((int)(enumDirect));
	m_nCurHairIndex = (m_nCurHairIndex + nSelHairNum[m_nSelChaIndex]) % nSelHairNum[m_nSelChaIndex];
	m_nCurHairIndex += nBeginIndex;

	// Change hair
	BOOL bOK = m_pChaForUI[m_nSelChaIndex]->ChangePart(enumEQUIP_HEAD, m_nCurHairIndex);
	if (bOK) {
		CItemRecord* pItem = GetItemRecordInfo(m_nCurHairIndex);
		if (pItem) {
			labHair->SetCaption(pItem->szName);
		}
	}
	else {
		g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(47), m_nCurHairIndex));
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::ChangeCity(eDirectType enumDirect) {
	if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
		return;

	CLabelEx* labCityShow = (CLabelEx*)frmChaCity->Find("labCityShow");
	if (!labCityShow) return;

	//Determine current city


	m_nCurCityIndex += ((int)(enumDirect));

	// Cycle through cities
	m_nCurCityIndex = (m_nCurCityIndex + MAX_CITY_NUM) % MAX_CITY_NUM;
	//labCityShow->SetCaption( szCities[m_nCurCityIndex]);
	memChaDescribe2->SetCaption(GetCityDescription(m_nCurCityIndex));
	memChaDescribe2->ProcessCaption();
}


//-----------------------------------------------------------------------
void CCreateChaScene::RenderCha(int x, int y) {
	g_Render.GetDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	g_Render.GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3) return;


	if (!m_pChaForUI[m_nSelChaIndex]) return;

	g_Render.LookAt(D3DXVECTOR3(11.0f, 36.0f, 10.0f), D3DXVECTOR3(8.70f, 12.0f, 8.0f), MPRender::VIEW_3DUI);
	y += 100;

	MPMatrix44 old_mat = *m_pChaForUI[m_nSelChaIndex]->GetMatrix();
	m_pChaForUI[m_nSelChaIndex]->SetUIYaw(180 + m_nChaRotate);
	m_pChaForUI[m_nSelChaIndex]->SetUIScaleDis(9.0f * g_Render.GetScrWidth() / TINY_RES_X);
	m_pChaForUI[m_nSelChaIndex]->RenderForUI(x, y);
	m_pChaForUI[m_nSelChaIndex]->SetMatrix(&old_mat);

	g_Render.SetTransformView(&g_Render.GetWorldViewMatrix());
}

//-----------------------------------------------------------------------
void CCreateChaScene::RotateChar(eDirectType enumDirect) {
	m_nChaRotate += 180;
	m_nChaRotate += -((int)(enumDirect)) * 15;
	m_nChaRotate = (m_nChaRotate + 360) % 360;
	m_nChaRotate -= 180;
}


//-----------------------------------------------------------------------
void CCreateChaScene::InitChaFoundFrm() {
	if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
		return;

	if (NULL == m_pChaForUI[m_nSelChaIndex]) return;


	CTextButton* btnLeftHair = (CTextButton*)frmChaFound->Find("btnLeftHair");
	if (!btnLeftHair) return;
	CTextButton* btnRightHair = (CTextButton*)frmChaFound->Find("btnRightHair");
	if (!btnRightHair) return;

	memChaDescribe->SetCaption(GetCharacterDescription(m_nSelChaIndex));
	memChaDescribe->ProcessCaption();
	edtName->SetCaption(m_sName.c_str());


	BOOL bOK = m_pChaForUI[m_nSelChaIndex]->ChangePart(enumEQUIP_HEAD, m_nCurHairIndex);
	if (bOK) {
		CItemRecord* pItem = GetItemRecordInfo(m_nCurHairIndex);
		if (pItem) {
			labHair->SetCaption(pItem->szName);
		}
	}
	else {
		g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(47), m_nCurHairIndex));
	}
	bOK = m_pChaForUI[m_nSelChaIndex]->ChangePart(enumEQUIP_FACE, m_nCurFaceIndex);
	if (bOK) {
		//Change display from 'name+number' to just 'name'
		CItemRecord* pItem = GetItemRecordInfo(m_nCurFaceIndex);
		if (pItem) {
			labFace->SetCaption(pItem->szName);
		}
	}
	else {
		g_logManager.InternalLog(LogLevel::Error, "errors", SafeVFormat(GetLanguageString(48), m_nCurHairIndex));
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::InitChaCityFrm() {
	if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
		return;

	m_iFirstMourseX = g_pGameApp->GetMouseX();
	m_iFirstMourseY = g_pGameApp->GetMouseY();
	GetRender().ScreenConvert(m_iFirstMourseX, m_iFirstMourseY);
	m_bFirstShow = true;

	iCurrCity = 0;

	//memChaDescribe2->SetCaption(szCitiesDescripts[m_nCurCityIndex]);
	//memChaDescribe2->ProcessCaption();
	//labCity->SetCaption(szCities[m_nCurCityIndex]);
}

//-----------------------------------------------------------------------
void CCreateChaScene::InitChaData() {
	if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3)
		return;

	m_sName = ""; //Character name
	m_nCurHairIndex = nHairTestCnt[m_nSelChaIndex]; //Current hair style index
	m_nCurFaceIndex = nFaceTestCnt[m_nSelChaIndex]; //Current face type index

	//Randomly select initial city index
	int i = rand();
	if (i < RAND_MAX / 3)
		m_nCurCityIndex = 0;
	else if (i < RAND_MAX * 2 / 3)
		m_nCurCityIndex = 1;
	else
		m_nCurCityIndex = 2;

	m_nChaRotate = 0; //Last mouse drag rotation offset (-180~+180)
}


//-----------------------------------------------------------------------
bool CCreateChaScene::IsValidCheckChaName(const std::string& name) {
	/* Copy from LoginScene.cpp */
	// Обе стороны в UTF-8 после этапа A — побайтовое сравнение корректно для
	// проверки "юзер не ввёл ничего и оставил placeholder".
	if (name == GetLanguageString(49)) {
		g_pGameApp->MsgBox(GetLanguageString(49));
		return false;
	}

	if (name.empty()) {
		g_pGameApp->MsgBox(GetLanguageString(50));
		return false;
	}

	if (!::IsValidName(name.c_str())) {
		g_pGameApp->MsgBox(GetLanguageString(51));
		return false;
	}
	//return true;

	const char* s = name.c_str();
	int len = static_cast<int>(name.size());
	bool bOk = true;

	for (int i = 0; i < len; i++) {
		if (s[i] & 0x80) {
			if (!(s[i] == -93)) // Check whether it is a double-byte letter
			{
				i++;
			}
			else {
				bOk = false;
				i++;
				break;
			}
		}
		else {
			if (!(isdigit(s[i]) || isalpha(s[i]))) {
				bOk = false;
				break;
			}
		}
	}

	if (!bOk)
		g_pGameApp->MsgBox(GetLanguageString(52));

	//Check for forbidden character name words
	string sName(name);
	if (!CTextFilter::IsLegalText(CTextFilter::NAME_TABLE, sName)) {
		g_pGameApp->MsgBox(GetLanguageString(53));
		return false;
	}

	return bOk;
}

//-----------------------------------------------------------------------
void CCreateChaScene::DarkScene(bool isDark) {
	frmQuit->Find("imgBack")->SetIsShow(isDark);
}

//-----------------------------------------------------------------------
void CCreateChaScene::SendChaToServ() {
	if (m_nSelChaIndex < 0 || m_nSelChaIndex > 3) return;

	if (NULL == m_pChaForUI[m_nSelChaIndex]) return;

	//stNetChangeChaPart part;
	//memset( &part, 0, sizeof(part) );
	//part.sTypeID = (short)m_pChaForUI[m_nSelChaIndex]->getTypeID();
	//part.sHairID = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(0);
	//part.SLink[enumEQUIP_FACE] = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(1);
	//char szLookbuf[defLOOK_DATA_STRING_LEN];
	//if (LookData2String(&part, szLookbuf, defLOOK_DATA_STRING_LEN))
	//{
	//	CS_NewCha( edtName->GetCaption(), szCityNames[m_nCurCityIndex], part );
	//	CGameApp::Waiting();
	//}
	//
	//else
	//	g_pGameApp->MsgBox(GetLanguageString(54));


	int sTypeID = (short)m_pChaForUI[m_nSelChaIndex]->getTypeID();
	int sHairID = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(0);
	int faceID = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(1);
	CS_NewCha(edtName->GetCaption(), GetCityName(m_nCurCityIndex), sTypeID, sHairID, faceID);
}

//-----------------------------------------------------------------------
void CCreateChaScene::CreateNewCha() {
	CGameApp::Waiting(false);

	m_bSameNameError = false;

	//Close this form
	frmChaFound->Close();
	frmChaCity->Close();

	if (!m_pChaForUI[m_nSelChaIndex])
		return;

	string sName = edtName->GetCaption();

	stNetChangeChaPart part;
	memset(&part, 0, sizeof(part));
	part.sTypeID = (short)m_pChaForUI[m_nSelChaIndex]->getTypeID();
	part.sHairID = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(0);
	part.SLink[enumEQUIP_FACE] = (short)m_pChaForUI[m_nSelChaIndex]->GetPartID(1);
	int nChaIndex = m_nSelChaIndex;

	//Switch to character selection scene
	GotoSelChaScene();

	CSelectChaScene& rkScene = CSelectChaScene::GetCurrScene();
	rkScene.CreateCha(sName, nChaIndex, &part);
}

//-----------------------------------------------------------------------
CCreateChaScene& CCreateChaScene::GetCurrScene() {
	CCreateChaScene* pScene =
		dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	assert(NULL != pScene);

	return *pScene;
}

//-----------------------------------------------------------------------
void CCreateChaScene::NewChaError(int error_no, const char* error_info) {
	if (ERR_PT_SAMECHANAME == error_no) {
		//Close city selection list
		frmChaCity->Close();

		m_bSameNameError = true;
		frmChaFound->ShowModal();

		g_pGameApp->MsgBox(g_GetServerError(error_no));
		ToLogService("errors", LogLevel::Error, "{} Error, Code:{}, Info: {}", error_info, error_no,
					 g_GetServerError(error_no));
		CGameApp::Waiting(false);
	}
	else {
		g_pGameApp->MsgBox(g_GetServerError(error_no));
		ToLogService("errors", LogLevel::Error, "{} Error, Code:{}, Info: {}", error_info, error_no,
					 g_GetServerError(error_no));
		CGameApp::Waiting(false);

		if (!m_bSameNameError) {
			m_bSameNameError = false;
		}
	}
}

//-----------------------------------------------------------------------
void CCreateChaScene::GotoSelChaScene() {
	g_pGameApp->GotoScene(m_pkLastScene, true);
}

//-----------------------------------------------------------------------
int CCreateChaScene::GetCityZone(int x, int y) {
	if (!frmChaCity->GetIsShow())
		return 0;

	CGuiData* pSize = NULL;
	for (int i(0); i < MAX_CITY_NUM; ++i) {
		pSize = imgCitiesBlock[i];
		if (x >= pSize->GetX() && x <= pSize->GetX2() && y >= pSize->GetY() && y <= pSize->GetY2()) {
			return i + 1;
		}
	}
	return 0;
}

//-----------------------------------------------------------------------
void CCreateChaScene::ShowCityZone(int index) {
	if (!frmChaCity->GetIsShow())
		return;

	else if (iCurrCity != index) {
		if (iCurrCity == 0) {
			for (int j(0); j < CITY_PICTURE_NUM; ++j) {
				imgCities[index][j]->SetIsShow(true);
			}
			iCurrCity = index;
		}
		else {
			for (int j(0); j < CITY_PICTURE_NUM; ++j) {
				imgCities[iCurrCity][j]->SetIsShow(false);
				imgCities[index][j]->SetIsShow(true);
			}
			iCurrCity = index;
		}
	}
}


// Called during loading, refresh
void CCreateChaScene::LoadingCall() {
	CGameScene::LoadingCall();

	// Hide the role info page
	/*if(frmRoleInfo)
	{
		DarkScene();
		frmRoleInfo->ShowModal();
	}*/
}


void CCreateChaScene::ShowChaFoundForm() {
	//Initialize character data
	InitChaData();

	//Sync character creation form
	InitChaFoundFrm();

	//Dim the scene
	this->DarkScene(true);
	frmQuit->SetIsShow(false);

	//Show form
	frmChaFound->ShowModal();
}


void CCreateChaScene::ShowAllRoleInfo(int nRoleInfo) {
	if (0 < nRoleInfo && nRoleInfo <= ROLE_ALL_INFO_COUNT) {
		for (int i = 0; i < ROLE_ALL_INFO_COUNT; ++i) {
			if (imgChaView[i] && (i + 1) == nRoleInfo && memChaDescribeUp && memChaDescribeDown) {
				imgChaView[i]->SetIsShow(true);

				switch (nRoleInfo) {
				case 2: // Swordsman
				case 9:
					memChaDescribeUp->SetCaption(GetLanguageString(803).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(804).c_str());
					break;

				case 5: // Dual-Sword Fighter
					memChaDescribeUp->SetCaption(GetLanguageString(806).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(807).c_str());
					break;

				case 10: // Crusader
					memChaDescribeUp->SetCaption(GetLanguageString(808).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(809).c_str());
					break;

				case 13: // Herbalist
				case 20:
					memChaDescribeUp->SetCaption(GetLanguageString(810).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(811).c_str());
					break;

				case 16: // Cleric
				case 22:
					memChaDescribeUp->SetCaption(GetLanguageString(812).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(813).c_str());
					break;

				case 17: // Seal Master
				case 23:
					memChaDescribeUp->SetCaption(GetLanguageString(814).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(815).c_str());
					break;

				case 4: // Explorer
				case 14:
				case 21:
					memChaDescribeUp->SetCaption(GetLanguageString(816).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(817).c_str());
					break;

				case 7: // Sharpshooter
				case 18:
				case 24:
					memChaDescribeUp->SetCaption(GetLanguageString(818).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(819).c_str());
					break;

				case 3: // Hunter
				case 12:
					memChaDescribeUp->SetCaption(GetLanguageString(820).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(821).c_str());
					break;

				case 6: // Voyager
				case 15:
					memChaDescribeUp->SetCaption(GetLanguageString(822).c_str());
					memChaDescribeDown->SetCaption(GetLanguageString(823).c_str());
					break;

				case 1: // Lance (base)
					memChaDescribeUp->SetCaption(GetLanguageString(35).c_str());
					memChaDescribeDown->SetCaption("");
					break;

				case 8: // Carsise (base)
					memChaDescribeUp->SetCaption(GetLanguageString(36).c_str());
					memChaDescribeDown->SetCaption("");
					break;

				case 11: // Phyllis (base)
					memChaDescribeUp->SetCaption(GetLanguageString(37).c_str());
					memChaDescribeDown->SetCaption("");
					break;

				case 19: // Ami (base)
					memChaDescribeUp->SetCaption(GetLanguageString(38).c_str());
					memChaDescribeDown->SetCaption("");
					break;

				default:
					memChaDescribeUp->SetCaption("");
					memChaDescribeDown->SetCaption("");
					break;
				}

				memChaDescribeUp->ProcessCaption();
				memChaDescribeDown->ProcessCaption();
			}
			else {
				imgChaView[i]->SetIsShow(false);
			}
		}

		frmRoleAllInfo->ShowModal();
	}
}


void CCreateChaScene::_evtRoleInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();

	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());
	if (pCreateChaScene) {
		lwVector3 OffsetPos4[4];
		OffsetPos4[0].x = 0.676f;
		OffsetPos4[0].y = 28.696f;
		OffsetPos4[0].z = 2.f;

		OffsetPos4[1].x = -7.013f;
		OffsetPos4[1].y = 21.072f;
		OffsetPos4[1].z = 2.5f;

		OffsetPos4[2].x = -20.663f;
		OffsetPos4[2].y = 30.951f;
		OffsetPos4[2].z = 2.f;

		OffsetPos4[3].x = 2.f;
		OffsetPos4[3].y = 0.5f;
		OffsetPos4[3].z = 0.1f;

		//pCreateChaScene->m_LoginSceneCreateCha.LoadArrowMark( "hand.lgo", OffsetPos4 );

		pCreateChaScene->DarkScene(false);

		pCreateChaScene->frmRoleInfo->Close();
	}
}

// Lance race classes (1 ~ 7)
void CCreateChaScene::_evtLanchInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if (pCreateChaScene) {
		pCreateChaScene->frmLanchInfo->Close();

		if (strName == "btnNextStep") {
			pCreateChaScene->ShowChaFoundForm();
		}
		else if (strName == "btnClose") {
			pCreateChaScene->DarkScene(false);
		}
		else if (strName == "btnViewPlayer_1") {
			pCreateChaScene->ShowAllRoleInfo(1); // Lance base
		}
		else if (strName == "btnViewPlayer_2") {
			pCreateChaScene->ShowAllRoleInfo(2); // Swordsman
		}
		else if (strName == "btnViewPlayer_3") {
			pCreateChaScene->ShowAllRoleInfo(3); // Hunter
		}
		else if (strName == "btnViewPlayer_4") {
			pCreateChaScene->ShowAllRoleInfo(4); // Explorer
		}
		else if (strName == "btnViewPlayer_5") {
			pCreateChaScene->ShowAllRoleInfo(5); // Dual-Sword Fighter
		}
		else if (strName == "btnViewPlayer_6") {
			pCreateChaScene->ShowAllRoleInfo(6); // Voyager
		}
		else if (strName == "btnViewPlayer_7") {
			pCreateChaScene->ShowAllRoleInfo(7); // Sharpshooter
		}
	}
}

// Ami race classes (19 ~ 24)
void CCreateChaScene::_evtAimiInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if (pCreateChaScene) {
		pCreateChaScene->frmAimiInfo->Close();

		if (strName == "btnNextStep") {
			pCreateChaScene->ShowChaFoundForm();
		}
		else if (strName == "btnClose") {
			pCreateChaScene->DarkScene(false);
		}
		else if (strName == "btnViewPlayer_19") {
			pCreateChaScene->ShowAllRoleInfo(19); // Ami base
		}
		else if (strName == "btnViewPlayer_20") {
			pCreateChaScene->ShowAllRoleInfo(20); // Herbalist
		}
		else if (strName == "btnViewPlayer_21") {
			pCreateChaScene->ShowAllRoleInfo(21); // Explorer
		}
		else if (strName == "btnViewPlayer_22") {
			pCreateChaScene->ShowAllRoleInfo(22); // Cleric
		}
		else if (strName == "btnViewPlayer_23") {
			pCreateChaScene->ShowAllRoleInfo(23); // Seal Master
		}
		else if (strName == "btnViewPlayer_24") {
			pCreateChaScene->ShowAllRoleInfo(24); // Sharpshooter
		}
	}
}

// Phyllis race classes (11 ~ 18)
void CCreateChaScene::_evtFelierInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if (pCreateChaScene) {
		pCreateChaScene->frmFelierInfo->Close();

		if (strName == "btnNextStep") {
			pCreateChaScene->ShowChaFoundForm();
		}
		else if (strName == "btnClose") {
			pCreateChaScene->DarkScene(false);
		}
		else if (strName == "btnViewPlayer_11") {
			pCreateChaScene->ShowAllRoleInfo(11); // Phyllis base
		}
		else if (strName == "btnViewPlayer_12") {
			pCreateChaScene->ShowAllRoleInfo(12); // Hunter
		}
		else if (strName == "btnViewPlayer_13") {
			pCreateChaScene->ShowAllRoleInfo(13); // Herbalist
		}
		else if (strName == "btnViewPlayer_14") {
			pCreateChaScene->ShowAllRoleInfo(14); // Explorer
		}
		else if (strName == "btnViewPlayer_15") {
			pCreateChaScene->ShowAllRoleInfo(15); // Voyager
		}
		else if (strName == "btnViewPlayer_16") {
			pCreateChaScene->ShowAllRoleInfo(16); // Cleric
		}
		else if (strName == "btnViewPlayer_17") {
			pCreateChaScene->ShowAllRoleInfo(17); // Seal Master
		}
		else if (strName == "btnViewPlayer_18") {
			pCreateChaScene->ShowAllRoleInfo(18); // Sharpshooter
		}
	}
}

// Carsise race classes (8 ~ 10)
void CCreateChaScene::_evtCaxiusInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if (pCreateChaScene) {
		pCreateChaScene->frmCaxiusInfo->Close();

		if (strName == "btnNextStep") {
			pCreateChaScene->ShowChaFoundForm();
		}
		else if (strName == "btnClose") {
			pCreateChaScene->DarkScene(false);
		}
		else if (strName == "btnViewPlayer_8") {
			pCreateChaScene->ShowAllRoleInfo(8); // Carsise base
		}
		else if (strName == "btnViewPlayer_9") {
			pCreateChaScene->ShowAllRoleInfo(9); // Swordsman
		}
		else if (strName == "btnViewPlayer_10") {
			pCreateChaScene->ShowAllRoleInfo(10); // Crusader
		}
	}
}

void CCreateChaScene::_evtRoleAllInfoFormMouseEvent(CCompent* pSender, int nMsgType, int x, int y, DWORD dwKey) {
	string strName = pSender->GetName();
	CCreateChaScene* pCreateChaScene = dynamic_cast<CCreateChaScene*>(g_pGameApp->GetCurScene());

	if (strName == "btnPrevStep") {
		if (pCreateChaScene) {
			pCreateChaScene->frmRoleAllInfo->Close();

			switch (pCreateChaScene->m_nSelChaIndex) {
			case 0: // Lance
				pCreateChaScene->frmLanchInfo->ShowModal();
				break;

			case 1: // Carsise
				pCreateChaScene->frmCaxiusInfo->ShowModal();
				break;

			case 2: // Phyllis
				pCreateChaScene->frmFelierInfo->ShowModal();
				break;

			case 3: // Ami
				pCreateChaScene->frmAimiInfo->ShowModal();
				break;
			}
		}
	}
}
