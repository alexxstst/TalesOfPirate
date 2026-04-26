//
#pragma once

#include "lwHeader.h"
#include "lwInterfaceExt.h"
#include "lwExpObj.h"
#include "lwClassDecl.h"
#include "lwPoseCtrl.h"
#include "lwITypes.h"

LW_BEGIN
	class lwAnimCtrl {
	protected:
		lwIResourceMgr* _res_mgr;
		LW_DWORD _reg_id;
		LW_DWORD _ctrl_type;
		lwResFileAnimData _res_file;
		lwPoseCtrl _pose_ctrl;

	public:
	protected:
		lwAnimCtrl(lwIResourceMgr* res_mgr)
			: _res_mgr(res_mgr), _ctrl_type(ANIM_CTRL_TYPE_INVALID), _reg_id(LW_INVALID_INDEX) {
		}

		~lwAnimCtrl() {
		}

		LW_DWORD _GetCtrlType() const {
			return _ctrl_type;
		}

		void _SetResFile(const lwResFileAnimData* info) {
			_res_file = *info;
		}

		lwResFileAnimData* _GetResFileInfo() {
			return &_res_file;
		}

		LW_VOID _SetRegisterID(LW_DWORD id) {
			_reg_id = id;
		}

		LW_DWORD _GetRegisterID() const {
			return _reg_id;
		}

		lwIPoseCtrl* _GetPoseCtrl() {
			return (lwIPoseCtrl*)&_pose_ctrl;
		}
	};


	class lwAnimCtrlBone : public lwIAnimCtrlBone, public lwAnimCtrl {
		struct lwRTBD {
			DWORD flag;
			lwMatrix44* data;
		};

	private:
		lwAnimDataBone _data;
		lwMatrix44* _bone_rtmat_seq;
		lwMatrix44* _bone_rtmat_blend_seq;
		//lwIndexMatrix44lwMatrix44
		//SetDummyRTM()dummy
		//dummyindex
		lwIndexMatrix44* _dummy_rtmat_seq;
		lwRTBD* _rtbuf_seq;
		lwMatrix44* _rtmat_ptr;


		LW_STD_DECLARATION()

	private:
		LW_RESULT _BuildRunTimeBoneMatrix(lwMatrix44* out_buf, float frame, DWORD start_frame, DWORD end_frame);
		LW_RESULT _UpdateFrameDataBone(lwMatrix44** o_mat_ptr, lwMatrix44* mat_buf, float frame, DWORD start_frame,
									   DWORD end_frame, BOOL loop_flag);
		LW_RESULT _UpdateFrameDataBoneDummy();
		LW_RESULT _BlendBoneData(lwMatrix44* dst_mat_ptr, lwMatrix44* src_mat_ptr0, lwMatrix44* src_mat_ptr1, float t);

	public:
		lwAnimCtrlBone(lwIResourceMgr* res_mgr);
		~lwAnimCtrlBone();

		// == base method
		virtual LW_DWORD GetCtrlType() const {
			return lwAnimCtrl::_GetCtrlType();
		}

		virtual void SetResFile(const lwResFileAnimData* info) {
			lwAnimCtrl::_SetResFile(info);
		}

		virtual lwResFileAnimData* GetResFileInfo() {
			return lwAnimCtrl::_GetResFileInfo();
		}

		virtual LW_VOID SetRegisterID(LW_DWORD id) {
			lwAnimCtrl::_SetRegisterID(id);
		}

		virtual LW_DWORD GetRegisterID() const {
			return lwAnimCtrl::_GetRegisterID();
		}

		virtual lwIPoseCtrl* GetPoseCtrl() {
			return lwAnimCtrl::_GetPoseCtrl();
		}

		// end

		LW_RESULT LoadData(const void* data);
		LW_RESULT Destroy();

		LW_RESULT SetFrame(float frame, DWORD start_frame, DWORD end_frame);

		lwMatrix44* GetBoneRTMSeq() {
			return _rtmat_ptr;
		}

		lwIndexMatrix44* GetDummyRTMSeq() {
			return _dummy_rtmat_seq;
		}

		lwMatrix44* GetDummyRTM(DWORD id);

		DWORD GetDummyNum() {
			return _data._dummy_num;
		}

		DWORD GetBoneNum() {
			return _data._bone_num;
		}

		virtual LW_RESULT UpdatePose(lwPlayPoseInfo* info);
		virtual LW_RESULT UpdateAnimData(const lwPlayPoseInfo* info);
		virtual LW_RESULT UpdatePoseKeyFrameProc(lwPlayPoseInfo* info);

		LW_RESULT CreateRunTimeDataBuffer();
		LW_RESULT EnableRunTimeFrameBuffer(DWORD frame, DWORD flag);

		LW_RESULT ExtractAnimData(lwIAnimDataBone* out_data);
		LW_RESULT DumpRunTimeBoneData(const char* file);
		LW_RESULT DumpInitInvMat(const char* file);
	};

	class lwAnimCtrlMatrix : public lwIAnimCtrlMatrix, public lwAnimDataMatrix, public lwAnimCtrl {
		LW_STD_DECLARATION()

	private:
#ifdef USE_ANIMKEY_PRS
		lwAnimKeySetPRS _data;
		typedef lwAnimKeySetPRS lwAnimKeyType;
#else
#endif

		lwMatrix44 _rtmat_seq[1];

	public:
		lwAnimCtrlMatrix(lwIResourceMgr* res_mgr);
		~lwAnimCtrlMatrix();

		// == base method
		virtual LW_DWORD GetCtrlType() const {
			return lwAnimCtrl::_GetCtrlType();
		}

		virtual void SetResFile(const lwResFileAnimData* info) {
			lwAnimCtrl::_SetResFile(info);
		}

		virtual lwResFileAnimData* GetResFileInfo() {
			return lwAnimCtrl::_GetResFileInfo();
		}

		virtual LW_VOID SetRegisterID(LW_DWORD id) {
			lwAnimCtrl::_SetRegisterID(id);
		}

		virtual LW_DWORD GetRegisterID() const {
			return lwAnimCtrl::_GetRegisterID();
		}

		virtual lwIPoseCtrl* GetPoseCtrl() {
			return lwAnimCtrl::_GetPoseCtrl();
		}

		// end


		virtual LW_RESULT LoadData(const void* data);


		virtual LW_RESULT SetFrame(float frame);
		virtual LW_RESULT SetFrame(DWORD frame);

		virtual LW_RESULT UpdatePose(lwPlayPoseInfo* info);
		virtual LW_RESULT UpdateAnimData(const lwPlayPoseInfo* info);

		LW_RESULT GetRTM(lwMatrix44* mat);
		LW_RESULT ExtractAnimData(lwIAnimDataMatrix* out_data);
	};


	class lwAnimCtrlTexUV : public lwAnimCtrl, public lwIAnimCtrlTexUV, public lwAnimDataTexUV {
		LW_STD_DECLARATION()

	private:
		lwIAnimKeySetPRS* _keyset_prs;

		lwMatrix44 _rtmat_seq[1];

	public:
		lwAnimCtrlTexUV(lwIResourceMgr* res_mgr);
		~lwAnimCtrlTexUV();

		// == base method
		virtual LW_DWORD GetCtrlType() const {
			return lwAnimCtrl::_GetCtrlType();
		}

		virtual void SetResFile(const lwResFileAnimData* info) {
			lwAnimCtrl::_SetResFile(info);
		}

		virtual lwResFileAnimData* GetResFileInfo() {
			return lwAnimCtrl::_GetResFileInfo();
		}

		virtual LW_VOID SetRegisterID(LW_DWORD id) {
			lwAnimCtrl::_SetRegisterID(id);
		}

		virtual LW_DWORD GetRegisterID() const {
			return lwAnimCtrl::_GetRegisterID();
		}

		virtual lwIPoseCtrl* GetPoseCtrl() {
			return lwAnimCtrl::_GetPoseCtrl();
		}

		// end

		LW_RESULT LoadData(const void* data);
		LW_RESULT LoadData(const lwMatrix44* mat_seq, DWORD mat_num);

		virtual LW_RESULT SetFrame(float frame);
		virtual LW_RESULT SetFrame(DWORD frame);

		virtual LW_RESULT UpdatePose(lwPlayPoseInfo* info);
		virtual LW_RESULT UpdateAnimData(const lwPlayPoseInfo* info);
		virtual void SetAnimKeySetPRS(lwIAnimKeySetPRS* keyset);

		LW_RESULT GetRunTimeMatrix(lwMatrix44* mat);
		LW_RESULT ExtractAnimData(lwIAnimDataTexUV* out_data);
	};

	class lwAnimCtrlTexImg : public lwAnimCtrl, public lwIAnimCtrlTexImg {
		LW_STD_DECLARATION()

	private:
		lwAnimDataTexImg _data;
		lwITex** _tex_seq;
		DWORD _tex_num;
		DWORD _rttex;

	public:
		lwAnimCtrlTexImg(lwIResourceMgr* res_mgr);
		~lwAnimCtrlTexImg();

		// == base method
		virtual LW_DWORD GetCtrlType() const {
			return lwAnimCtrl::_GetCtrlType();
		}

		virtual void SetResFile(const lwResFileAnimData* info) {
			lwAnimCtrl::_SetResFile(info);
		}

		virtual lwResFileAnimData* GetResFileInfo() {
			return lwAnimCtrl::_GetResFileInfo();
		}

		virtual LW_VOID SetRegisterID(LW_DWORD id) {
			lwAnimCtrl::_SetRegisterID(id);
		}

		virtual LW_DWORD GetRegisterID() const {
			return lwAnimCtrl::_GetRegisterID();
		}

		virtual lwIPoseCtrl* GetPoseCtrl() {
			return lwAnimCtrl::_GetPoseCtrl();
		}

		// end

		LW_RESULT Destroy();

		virtual LW_RESULT LoadData(const void* data);
		virtual LW_RESULT UpdatePose(lwPlayPoseInfo* info);
		virtual LW_RESULT UpdateAnimData(const lwPlayPoseInfo* info);

		LW_RESULT ExtractAnimData(lwIAnimDataTexImg* out_data);
		LW_RESULT GetRunTimeTex(lwITex** tex);
	};

	class lwAnimCtrlMtlOpacity : public lwAnimCtrl, public lwIAnimCtrlMtlOpacity {
		LW_STD_DECLARATION();

	private:
		lwIAnimDataMtlOpacity* _data;
		float _rt_opacity;

	public:
		lwAnimCtrlMtlOpacity(lwIResourceMgr* res_mgr);
		~lwAnimCtrlMtlOpacity();

		// == base method
		virtual LW_DWORD GetCtrlType() const {
			return lwAnimCtrl::_GetCtrlType();
		}

		virtual void SetResFile(const lwResFileAnimData* info) {
			lwAnimCtrl::_SetResFile(info);
		}

		virtual lwResFileAnimData* GetResFileInfo() {
			return lwAnimCtrl::_GetResFileInfo();
		}

		virtual LW_VOID SetRegisterID(LW_DWORD id) {
			lwAnimCtrl::_SetRegisterID(id);
		}

		virtual LW_DWORD GetRegisterID() const {
			return lwAnimCtrl::_GetRegisterID();
		}

		virtual lwIPoseCtrl* GetPoseCtrl() {
			return lwAnimCtrl::_GetPoseCtrl();
		}

		// end

		LW_RESULT Destroy();

		LW_RESULT LoadData(const void* data);
		LW_RESULT UpdatePose(lwPlayPoseInfo* info);
		LW_RESULT UpdateAnimData(const lwPlayPoseInfo* info);

		LW_RESULT ExtractAnimData(lwIAnimDataMtlOpacity* out_data);
		LW_RESULT GetRunTimeOpacity(float* opacity);
	};

	LW_RESULT lwLoadAnimCtrlFromGenericFile(lwAnimCtrl* ctrl, DWORD ctrl_type, const char* file);
	LW_RESULT lwLoadAnimCtrlFromGeometryFile(lwAnimCtrl* ctrl, DWORD ctrl_type, const char* file);
	LW_RESULT lwLoadAnimCtrlFromModelFile(lwAnimCtrl* ctrl, DWORD ctrl_type, const char* file, DWORD obj_id);

	struct lwLight {
		DWORD type;
		lwVector3 pos;
		lwVector3 dir;
		lwColorValue4f amb;
		lwColorValue4f dif;
		lwColorValue4f spe;
		float range;
		float attenuation0;
		float attenuation1;
		float attenuation2;
	};

	struct lwIndexDataLight {
		DWORD id;
		lwLight data;
	};

	class lwAnimDataLight {
	public:
		lwIndexDataLight* _data_seq;
		DWORD _data_num;

	public:
		lwAnimDataLight()
			: _data_seq(0), _data_num(0) {
		}

		virtual ~lwAnimDataLight() {
			LW_SAFE_DELETE_A(_data_seq);
		}

		LW_RESULT Load(const char* file);
	};

	class lwAnimCtrlLight : /*public lwAnimCtrl, public lwIAnimCtrlLight,*/ public lwAnimDataLight {
		LW_STD_DECLARATION()

	private:
		lwLight _light;
		lwPoseCtrl _pose_ctrl;

	public:
		lwAnimCtrlLight();
		~lwAnimCtrlLight();

		virtual lwIPoseCtrl* GetPoseCtrl() {
			return (lwIPoseCtrl*)&_pose_ctrl;
		}

		LW_RESULT LoadData(const void* data);

		virtual LW_RESULT SetFrame(DWORD frame);

		virtual LW_RESULT UpdatePose(lwPlayPoseInfo* info);
		virtual LW_RESULT UpdateAnimData(float frame);
		virtual LW_RESULT UpdateObject(lwIAnimCtrlObj* ctrl_obj, lwIPrimitive* obj);
	};

LW_END
