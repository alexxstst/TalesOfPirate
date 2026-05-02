//
#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwInterfaceExt.h"

LW_BEGIN
	class lwAnimCtrlObjMat : public lwIAnimCtrlObjMat {
		enum { THIS_TYPE = ANIM_CTRL_TYPE_MAT };

		LW_STD_DECLARATION();

	private:
		lwIResourceMgr* _res_mgr;
		lwIAnimCtrlMatrix* _anim_ctrl;
		lwPlayPoseInfo _ppi;
		lwAnimCtrlObjTypeInfo _type_info;

		lwMatrix44 _rtm;

	public:
		lwAnimCtrlObjMat(lwIResourceMgr* res_mgr);
		~lwAnimCtrlObjMat();

		LW_RESULT Clone(lwIAnimCtrlObjMat** ret_obj);
		lwIAnimCtrlMatrix* AttachAnimCtrl(lwIAnimCtrlMatrix* ctrl);
		lwIAnimCtrlMatrix* DetachAnimCtrl();

		lwIAnimCtrl* GetAnimCtrl() {
			return _anim_ctrl;
		}

		lwPlayPoseInfo* GetPlayPoseInfo() {
			return &_ppi;
		}

		LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT PlayPose(const lwPlayPoseInfo* info);

		BOOL IsPlaying() {
			return !(_anim_ctrl == NULL || _ppi.type == PLAY_INVALID);
		}

		LW_RESULT UpdateAnimCtrl();
		LW_RESULT UpdateObject();

		LW_RESULT GetRTM(lwMatrix44* mat);
	};

	class lwAnimCtrlObjBone : public lwIAnimCtrlObjBone {
		enum { THIS_TYPE = ANIM_CTRL_TYPE_BONE };

		LW_STD_DECLARATION();

	private:
		lwIResourceMgr* _res_mgr;
		lwIAnimCtrlBone* _anim_ctrl;
		lwPlayPoseInfo _ppi;
		lwAnimCtrlObjTypeInfo _type_info;

		lwIndexMatrix44* _dummy_rtm_seq;
		DWORD _dummy_rtm_num;
		lwMatrix44* _bone_rtm_seq;
		DWORD _bone_rtm_num;

	public:
		lwAnimCtrlObjBone(lwIResourceMgr* res_mgr);
		~lwAnimCtrlObjBone();

		// base method
		LW_RESULT Clone(lwIAnimCtrlObjBone** ret_obj);
		lwIAnimCtrlBone* AttachAnimCtrl(lwIAnimCtrlBone* ctrl);
		lwIAnimCtrlBone* DetachAnimCtrl();

		lwIAnimCtrl* GetAnimCtrl() {
			return _anim_ctrl;
		}

		lwPlayPoseInfo* GetPlayPoseInfo() {
			return &_ppi;
		}

		void SetPlayPoseInfo(const lwPlayPoseInfo* ppi) {
			_ppi = *ppi;
		}

		LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT PlayPose(const lwPlayPoseInfo* info);

		BOOL IsPlaying() {
			return !(_anim_ctrl == NULL || _ppi.type == PLAY_INVALID);
		}

		LW_RESULT UpdateAnimCtrl();
		LW_RESULT UpdateObject(lwIAnimCtrlObjBone* ctrl_obj, lwIMesh* mesh_obj);
		LW_RESULT UpdateHelperObject(lwIHelperObject* helper_obj);

		lwMatrix44* GetBoneRTMSeq() {
			return _bone_rtm_seq;
		}

		lwMatrix44* GetDummyRTM(DWORD id);

		lwIndexMatrix44* GetDummyRTMSeq() {
			return _dummy_rtm_seq;
		}

		DWORD GetBoneRTTMNum() {
			return _bone_rtm_num;
		}

		DWORD GetDummyRTMNum() {
			return _dummy_rtm_num;
		}
	};


	class lwAnimCtrlObjTexUV : public lwIAnimCtrlObjTexUV {
		enum { THIS_TYPE = ANIM_CTRL_TYPE_TEXUV };

		LW_STD_DECLARATION();

	private:
		lwIResourceMgr* _res_mgr;
		lwIAnimCtrlTexUV* _anim_ctrl;
		lwPlayPoseInfo _ppi;
		lwAnimCtrlObjTypeInfo _type_info;

		lwMatrix44 _rtm;

	public:
		lwAnimCtrlObjTexUV(lwIResourceMgr* res_mgr);
		~lwAnimCtrlObjTexUV();

		LW_RESULT Clone(lwIAnimCtrlObjTexUV** ret_obj);
		lwIAnimCtrlTexUV* AttachAnimCtrl(lwIAnimCtrlTexUV* ctrl);
		lwIAnimCtrlTexUV* DetachAnimCtrl();

		lwIAnimCtrl* GetAnimCtrl() {
			return _anim_ctrl;
		}

		lwPlayPoseInfo* GetPlayPoseInfo() {
			return &_ppi;
		}

		LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT PlayPose(const lwPlayPoseInfo* info);

		BOOL IsPlaying() {
			return !(_anim_ctrl == NULL || _ppi.type == PLAY_INVALID);
		}

		LW_RESULT UpdateAnimCtrl();
		LW_RESULT UpdateObject();

		LW_RESULT GetRTM(lwMatrix44* mat);
	};

	class lwAnimCtrlObjTexImg : public lwIAnimCtrlObjTexImg {
		enum { THIS_TYPE = ANIM_CTRL_TYPE_TEXIMG };

		LW_STD_DECLARATION();

	private:
		lwIResourceMgr* _res_mgr;
		lwIAnimCtrlTexImg* _anim_ctrl;
		lwPlayPoseInfo _ppi;
		lwAnimCtrlObjTypeInfo _type_info;

		lwITex* _rt_tex;

	public:
		lwAnimCtrlObjTexImg(lwIResourceMgr* res_mgr);
		~lwAnimCtrlObjTexImg();

		LW_RESULT Clone(lwIAnimCtrlObjTexImg** ret_obj);
		lwIAnimCtrlTexImg* AttachAnimCtrl(lwIAnimCtrlTexImg* ctrl);
		lwIAnimCtrlTexImg* DetachAnimCtrl();

		lwIAnimCtrl* GetAnimCtrl() {
			return _anim_ctrl;
		}

		lwPlayPoseInfo* GetPlayPoseInfo() {
			return &_ppi;
		}

		LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT PlayPose(const lwPlayPoseInfo* info);

		BOOL IsPlaying() {
			return !(_anim_ctrl == NULL || _ppi.type == PLAY_INVALID);
		}

		LW_RESULT UpdateAnimCtrl();
		LW_RESULT UpdateObject();

		LW_RESULT GetRunTimeTex(lwITex** tex);
	};

	class lwAnimCtrlObjMtlOpacity : public lwIAnimCtrlObjMtlOpacity {
		enum { THIS_TYPE = ANIM_CTRL_TYPE_MTLOPACITY };

		LW_STD_DECLARATION();

	private:
		lwIResourceMgr* _res_mgr;
		lwIAnimCtrlMtlOpacity* _anim_ctrl;
		lwPlayPoseInfo _ppi;
		lwAnimCtrlObjTypeInfo _type_info;

		float _rt_opacity;

	public:
		lwAnimCtrlObjMtlOpacity(lwIResourceMgr* res_mgr);
		~lwAnimCtrlObjMtlOpacity();

		LW_RESULT Clone(lwIAnimCtrlObjMtlOpacity** ret_obj);
		lwIAnimCtrlMtlOpacity* AttachAnimCtrl(lwIAnimCtrlMtlOpacity* ctrl);
		lwIAnimCtrlMtlOpacity* DetachAnimCtrl();

		lwIAnimCtrl* GetAnimCtrl() {
			return _anim_ctrl;
		}

		lwPlayPoseInfo* GetPlayPoseInfo() {
			return &_ppi;
		}

		LW_RESULT SetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT GetTypeInfo(lwAnimCtrlObjTypeInfo* info);
		LW_RESULT PlayPose(const lwPlayPoseInfo* info);

		BOOL IsPlaying() {
			return !(_anim_ctrl == NULL || _ppi.type == PLAY_INVALID);
		}

		LW_RESULT UpdateAnimCtrl();
		LW_RESULT UpdateObject();

		LW_RESULT GetRunTimeOpacity(float* opacity);
	};

	// lwAnimCtrlAgent
	class lwAnimCtrlAgent : public lwIAnimCtrlAgent {
		LW_STD_DECLARATION()

	private:
		lwIResourceMgr* _res_mgr;
		lwIAnimCtrlObj** _obj_seq;
		DWORD _obj_num;

	private:
	public:
		lwAnimCtrlAgent(lwIResourceMgr* res_mgr);
		~lwAnimCtrlAgent();

		LW_RESULT AddAnimCtrlObj(lwIAnimCtrlObj* obj);
		lwIAnimCtrlObj* RemoveAnimCtrlObj(lwAnimCtrlObjTypeInfo* info);
		lwIAnimCtrlObj* GetAnimCtrlObj(lwAnimCtrlObjTypeInfo* info);

		lwIAnimCtrlObj* GetAnimCtrlObj(DWORD id) {
			return _obj_seq[id];
		}

		DWORD GetAnimCtrlObjNum() {
			return _obj_num;
		}

		LW_RESULT Update();

		LW_RESULT Clone(lwIAnimCtrlAgent** ret_obj);
		LW_RESULT ExtractAnimData(lwAnimDataInfo* data_info);
	};

LW_END
