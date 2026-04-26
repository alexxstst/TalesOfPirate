#pragma once
#include "MindPower.h"

class AnimCtrlLight;

struct SceneLight {
	enum {
		SL_AMBIENT = 0,
		SL_LIGHT = 1,
		SL_INVALID = LW_INVALID_INDEX,
	};

	DWORD type;
	D3DCOLORVALUE dif;
	D3DCOLORVALUE amb;
	D3DVECTOR pos;
	float range;
	float attenuation0;
	float attenuation1;
	float attenuation2;

	lwPlayPoseInfo ppi;
	AnimCtrlLight* anim_light;

	SceneLight() {
		Reset();
	}

	void Reset() {
		type = SL_INVALID;
		anim_light = 0;
		ppi.bit_mask = PPI_MASK_DEFAULT;
		ppi.pose = 0;
		ppi.type = PLAY_LOOP;
		ppi.velocity = 1.0f; //Testing  Mdr.st
		ppi.data = 0;

		// added by roger {
		ppi.proc = NULL;
		ppi.proc_param = NULL;

		ZeroMemory(&ppi.blend_info, sizeof(ppi.blend_info));
		// }
	}

	int GetSquareMagnitude(int xo, int yo, int zo) {
		int i = (int)(xo - pos.x);
		int j = (int)(yo - pos.y);
		int k = (int)(zo - pos.z);
		return (i * i + j * j + k * k);
	}

	void GetLightData(D3DLIGHTX* lgt) {
		memset(lgt, 0, sizeof(D3DLIGHTX));
		lgt->Type = D3DLIGHT_POINT;
		lgt->Range = range;
		lgt->Position = pos;
		lgt->Ambient = amb;
		lgt->Diffuse = dif;
		lgt->Attenuation0 = attenuation0;
		lgt->Attenuation1 = attenuation1;
		lgt->Attenuation2 = attenuation2;
	}

	int UpdateAnimLight();
};

struct IndexDataSceneLight {
	DWORD id;
	SceneLight light;
};

class AnimCtrlLight {
private:
	IndexDataSceneLight* _data_seq;
	DWORD _data_num;
	lwIPoseCtrl* _pose_ctrl;

public:
	AnimCtrlLight()
		: _data_seq(0), _data_num(0), _pose_ctrl(0) {
	}

	~AnimCtrlLight() {
		SAFE_RELEASE(_pose_ctrl);
		SAFE_DELETE_ARRAY(_data_seq);
	}


	int Load(const char* file);
	int UpdateObject(SceneLight* ret_obj);
	void SetData(IndexDataSceneLight* buf, DWORD num);
};
