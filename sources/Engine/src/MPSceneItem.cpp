//
#include "stdafx.h"

#include "MPSceneItem.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwInterface.h"
#include "lwGraphicsUtil.h"


LW_BEGIN
	MPSceneItem::MPSceneItem(lwISysGraphics* sys_graphics) {
		sys_graphics->GetResourceMgr()->CreateItem(&_obj);

		BindMatrix(_obj->GetMatrix());
	}

	MPSceneItem::MPSceneItem() {
		lwSysGraphics::GetActiveIGraphicsSystem()->GetResourceMgr()->CreateItem(&_obj);

		BindMatrix(_obj->GetMatrix());
	}

	MPSceneItem::~MPSceneItem() {
		_obj->Release();
	}


	void MPSceneItem::FrameMove() {
		if (LW_RESULT r = _obj->Update(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->Update failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
		}
	}

	void MPSceneItem::Render() {
		if (LW_RESULT r = _obj->Render(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->Render failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
		}
	}

	LW_RESULT MPSceneItem::Load(std::string_view file, int arbitrary_flag) {
		LW_RESULT ret;
		ret = _obj->Load(file, arbitrary_flag);
		if (LW_FAILED(ret)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _obj->Load failed: file={}, arbitrary_flag={}, ret={}",
						 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file), arbitrary_flag, static_cast<long long>(ret));
			LG_MSGBOX("Load MPSceneItem {} error", (file.empty() ? std::string_view{"(null)"} : file));
		}

		return ret;
	}

	void MPSceneItem::SetMaterial(const D3DMATERIALX* mtl) {
		_obj->SetMaterial((lwMaterial*)mtl);
	}

	void MPSceneItem::SetOpacity(float opacity) {
		_obj->SetOpacity(opacity);
	}

	float MPSceneItem::GetOpacity() {
		return _obj->GetOpacity();
	}

	LW_RESULT MPSceneItem::Copy(const MPSceneItem* obj) {
		LW_RESULT ret = LW_RET_FAILED;
		lwIItem* item = 0;

		LW_FAILED_RET(obj->_obj->Clone(&item));
		_obj = item;
		BindMatrix(_obj->GetMatrix());

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	void MPSceneItem::Destroy() {
		_obj->Destroy();
	}

	LW_RESULT MPSceneItem::PlayObjImpPose(DWORD ctrl_type, DWORD pose_id, DWORD play_type, float start_frame,
										  float velocity) {
		LW_RESULT ret = LW_RET_FAILED;

		lwPlayPoseInfo info;
		memset(&info, 0, sizeof(info));
		info.bit_mask = PPI_MASK_DEFAULT;

		info.velocity = velocity;
		info.pose = pose_id;
		info.frame = start_frame;
		info.type = play_type;

		lwIPrimitive* p = _obj->GetPrimitive();
		if (p == NULL)
			goto __ret;

		{
			lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
			if (anim_agent == 0)
				goto __ret;

			{
				lwIAnimCtrlObj* ctrl_obj = NULL;

				lwAnimCtrlObjTypeInfo type_info;
				type_info.type = ctrl_type;
				type_info.data[0] = LW_INVALID_INDEX;
				type_info.data[1] = LW_INVALID_INDEX;

				switch (ctrl_type) {
				case ANIM_CTRL_TYPE_MAT:
					break;
				case ANIM_CTRL_TYPE_BONE:
					break;
				case ANIM_CTRL_TYPE_TEXUV:
					type_info.data[0] = 0;
					type_info.data[1] = 0;
					break;
				case ANIM_CTRL_TYPE_TEXIMG:
					break;
				}

				ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);

				if (ctrl_obj == 0)
					goto __ret;

				if (LW_RESULT r = ctrl_obj->PlayPose(&info); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] ctrl_obj->PlayPose failed: ctrl_type={}, pose_id={}, ret={}",
								 __FUNCTION__, ctrl_type, pose_id, static_cast<long long>(r));
					goto __ret;
				}
			}
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT MPSceneItem::HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) {
		return _obj->HitTestPrimitive(info, org, ray);
	}

	lwIPoseCtrl* MPSceneItem::GetObjImpPoseCtrl(DWORD ctrl_type) {
		lwIPrimitive* p = _obj->GetPrimitive();
		if (p == NULL)
			return NULL;

		lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
		lwIAnimCtrlObj* ctrl_obj = NULL;

		lwAnimCtrlObjTypeInfo type_info;
		type_info.type = ctrl_type;

		switch (ctrl_type) {
		case ANIM_CTRL_TYPE_MAT:
		case ANIM_CTRL_TYPE_BONE:
			type_info.data[0] = LW_INVALID_INDEX;
			type_info.data[1] = LW_INVALID_INDEX;
			break;
		case ANIM_CTRL_TYPE_TEXUV:
		case ANIM_CTRL_TYPE_TEXIMG:
			type_info.data[0] = 0;
			type_info.data[1] = 0;
			break;
		}

		ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);

		if (ctrl_obj == NULL)
			return NULL;

		lwIAnimCtrl* c = 0;

		switch (ctrl_type) {
		case ANIM_CTRL_TYPE_MAT:
			c = ((lwIAnimCtrlObjMat*)ctrl_obj)->GetAnimCtrl();
			break;
		case ANIM_CTRL_TYPE_BONE:
			c = ((lwIAnimCtrlObjBone*)ctrl_obj)->GetAnimCtrl();
			break;
		case ANIM_CTRL_TYPE_TEXUV:
			c = ((lwIAnimCtrlObjTexUV*)ctrl_obj)->GetAnimCtrl();
			break;
		case ANIM_CTRL_TYPE_TEXIMG:
			c = ((lwIAnimCtrlObjTexImg*)ctrl_obj)->GetAnimCtrl();
			break;
		}

		if (c == NULL)
			return NULL;

		return c->GetPoseCtrl();
	}

	LW_RESULT MPSceneItem::PlayDefaultAnimation(float velocity) {
		return _obj->PlayDefaultAnimation(velocity);
	}

	void MPSceneItem::ShowBoundingObject(int show) {
		_obj->ShowBoundingObject(show);
	}

	LW_RESULT MPSceneItem::GetObjDummyRunTimeMatrix(lwMatrix44* mat, DWORD id) {
		return _obj->GetObjDummyRunTimeMatrix(mat, id);
	}

	// added by clp
	void MPSceneItem::GetDummyLocalMatrix(lwMatrix44* mat, DWORD id) {
		_obj->GetDummyMatrix(mat, id);
	}

	lwIPrimitive* MPSceneItem::GetPrimitive() {
		return _obj->GetPrimitive();
	}

	void MPSceneItem::SetObjState(DWORD state, BYTE value) {
		_obj->SetObjState(state, value);
	}

	DWORD MPSceneItem::GetObjState(DWORD state) const {
		return _obj->GetObjState(state);
	}

	//    return mtltex_agent->LoadTextureStage(&info, NULL);

	LW_RESULT MPSceneItem::ResetItemTexture(DWORD subset, lwITex* tex, lwITex** old_tex) {
		lwIPrimitive* p = _obj->GetPrimitive();
		lwIMtlTexAgent* mtltex_agent = p->GetMtlTexAgent(subset);

		if (mtltex_agent == 0)
			return LW_RET_FAILED;

		return mtltex_agent->SetTex(tex->GetStage(), tex, old_tex);
	}

	void MPSceneItem::SetTextureLOD(DWORD level) {
		_obj->SetTextureLOD(level);
	}

LW_END
