//
#include "stdafx.h"

#include "MPSceneObject.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwInterface.h"
#include "lwGraphicsUtil.h"

LW_BEGIN
	MPSceneObject::MPSceneObject(lwISysGraphics* sys_graphics)
		: _model(0) {
		sys_graphics->GetResourceMgr()->CreateModel(&_model);

		BindMatrix(_model->GetMatrix());
	}

	MPSceneObject::MPSceneObject()
		: _model(0) {
		lwSysGraphics::GetActiveIGraphicsSystem()->GetResourceMgr()->CreateModel(&_model);

		BindMatrix(_model->GetMatrix());
	}

	MPSceneObject::~MPSceneObject() {
		if (_model) {
			_model->Release();
			_model = NULL;
		}
	}


	void MPSceneObject::FrameMove() {
		if (_model) {
			if (LW_RESULT r = _model->Update(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _model->Update failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}
	}

	void MPSceneObject::Render() {
		if (_model) {
			if (LW_RESULT r = _model->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _model->Render failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}
	}

	LW_RESULT MPSceneObject::Load(const char* file, DWORD model_id) {
		LW_RESULT ret;
		ret = _model->Load(file, model_id);
		if (LW_FAILED(ret)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _model->Load failed: file={}, model_id={}, ret={}",
						 __FUNCTION__, file ? file : "(null)", model_id, static_cast<long long>(ret));
			LG_MSGBOX("Load MPSceneObject %s error", file);
		}
		return ret;
	}

	void MPSceneObject::SetMaterial(const D3DMATERIALX* mtl) {
		if (_model)
			_model->SetMaterial((lwMaterial*)mtl);
	}

	void MPSceneObject::Destroy() {
		if (_model)
			_model->Destroy();
	}

	LW_RESULT MPSceneObject::PlayObjImpPose(DWORD obj_id, DWORD ctrl_type, DWORD pose_id, DWORD play_type,
											float start_frame, float velocity) {
		lwPlayPoseInfo info;
		memset(&info, 0, sizeof(info));
		info.bit_mask = PPI_MASK_DEFAULT;

		info.velocity = velocity; //What does this change?
		info.pose = pose_id;
		info.frame = start_frame;
		info.type = play_type;

		lwIPrimitive* p = _model->GetPrimitive(obj_id);
		if (p == NULL)
			return LW_RET_FAILED;

		lwIAnimCtrlAgent* anim_agent = p->GetAnimAgent();
		lwIAnimCtrlObj* ctrl_obj = NULL;

		LW_RESULT ret = LW_RET_FAILED;

		lwAnimCtrlObjTypeInfo type_info;
		type_info.type = ctrl_type;
		type_info.data[0] = LW_INVALID_INDEX;
		type_info.data[1] = LW_INVALID_INDEX;

		switch (ctrl_type) {
		case ANIM_CTRL_TYPE_MAT:
			ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
			break;
		case ANIM_CTRL_TYPE_BONE:
			ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
			break;
		case ANIM_CTRL_TYPE_TEXUV:
			type_info.data[0] = 0;
			type_info.data[1] = 0;
			ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
			break;
		case ANIM_CTRL_TYPE_TEXIMG:
			break;
		}

		if (ctrl_obj) {
			return ctrl_obj->PlayPose(&info);
		}

		return LW_INVALID_INDEX;
	}

	LW_RESULT MPSceneObject::PlayDefaultAnimation(bool IsGlitched) {
		if (_model) {
			return _model->PlayDefaultAnimation(IsGlitched);
		}
		return 0;
	}


	LW_RESULT MPSceneObject::HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) {
		if (_model)
			return _model->HitTestPrimitive(info, org, ray);
		return 0;
	}

	LW_RESULT MPSceneObject::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) {
		if (_model)
			return _model->HitTest(info, org, ray);
		return 0;
	}

	LW_RESULT MPSceneObject::HitTestHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
											   const char* type_name) {
		if (_model)
			return _model->HitTestHelperMesh(info, org, ray, type_name);
		return 0;
	}

	LW_RESULT MPSceneObject::HitTestHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
											  const char* type_name) {
		if (_model)
			return _model->HitTestHelperBox(info, org, ray, type_name);
		return 0;
	}

	LW_RESULT MPSceneObject::HitTestPrimitiveHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
														const char* type_name) {
		if (_model)
			return _model->HitTestPrimitiveHelperMesh(info, org, ray, type_name);
		return 0;
	}

	LW_RESULT MPSceneObject::HitTestPrimitiveHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
													   const char* type_name) {
		if (_model)
			return _model->HitTestPrimitiveHelperBox(info, org, ray, type_name);
		return 0;
	}


	lwIPrimitive* MPSceneObject::GetPrimitive(DWORD id) {
		if (_model)
			return _model->GetPrimitive(id);
		return 0;
	}

	void MPSceneObject::ShowHelperObject(int show) {
		if (_model)
			_model->ShowHelperObject(show);
	}

	void MPSceneObject::ShowHelperMesh(int show) {
		if (_model)
			_model->ShowHelperMesh(show);
	}

	void MPSceneObject::ShowHelperBox(int show) {
		if (_model)
			_model->ShowHelperBox(show);
	}

	void MPSceneObject::ShowBoundingObject(int show) {
		if (_model)
			_model->ShowBoundingObject(show);
	}

	void MPSceneObject::SetObjState(DWORD state, BYTE value) {
		if (_model)
			_model->SetObjState(state, value);
	}

	DWORD MPSceneObject::GetObjState(DWORD state) const {
		if (_model)
			return _model->GetObjState(state);
		return 0;
	}

	LW_RESULT MPSceneObject::SetItemLink(const lwSceneItemLinkInfo* info) {
		if (_link_item_num >= LW_MAX_LINK_ITEM_NUM)
			return LW_RET_FAILED;

		lwItemLinkInfo ili;
		ili.id = info->id;
		ili.obj = info->obj->GetObject();
		ili.link_item_id = info->link_item_id;
		ili.link_parent_id = info->link_parent_id;

		if (LW_RESULT r = _model->SetItemLink(&ili); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _model->SetItemLink failed: id={}, link_item_id={}, link_parent_id={}, ret={}",
						 __FUNCTION__, info->id, info->link_item_id, info->link_parent_id, static_cast<long long>(r));
			return LW_RET_FAILED;
		}

		_link_item_seq[_link_item_num++] = info->obj;

		return LW_RET_OK;
	}

	LW_RESULT MPSceneObject::UnwieldItem(const MPSceneItem* obj) {
		for (DWORD i = 0; i < _link_item_num; i++) {
			if (_link_item_seq[i] == obj) {
				_model->ClearItemLink(_link_item_seq[i]->GetObject());
				_link_item_num -= 1;

				for (DWORD j = i; j < _link_item_num; j++) {
					_link_item_seq[j] = _link_item_seq[j + 1];
				}

				return LW_RET_OK;
			}
		}

		return LW_RET_FAILED;
	}

	MPSceneItem* MPSceneObject::GetLinkItem(DWORD id) {
		if (id < 0 || id >= _link_item_num)
			return NULL;

		return _link_item_seq[id];
	}

	DWORD MPSceneObject::GetPrimitiveNum() const {
		return _model->GetPrimitiveNum();
	}

	int MPSceneObject::GetPrimitiveBox(DWORD id, D3DXVECTOR3* p1, D3DXVECTOR3* p2) {
		lwIPrimitive* p = _model->GetPrimitive(id);
		if (p == NULL)
			return 0;

		lwIHelperObject* h = p->GetHelperObject();
		lwIBoundingBox* b = h->GetBoundingBox();
		if (b == NULL)
			return 0;

		lwBoundingBoxInfo* bbi = b->GetDataInfo(0);
		if (bbi == NULL)
			return 0;

		lwMatrix44 mat;
		lwMatrix44Multiply(&mat, &bbi->mat, p->GetMatrixGlobal());
		lwVector3 x1 = bbi->box.c - bbi->box.r;
		lwVector3 x2 = bbi->box.c + bbi->box.r;
		lwVec3Mat44Mul(&x1, &mat);
		lwVec3Mat44Mul(&x2, &mat);

		*p1 = *(D3DXVECTOR3*)&x1;
		*p2 = *(D3DXVECTOR3*)&x2;

		return 1;
	}

	void MPSceneObject::SetOpacity(float opacity) {
		if (_model)
			_model->SetOpacity(opacity);
	}

	void MPSceneObject::SetTextureLOD(DWORD level) {
		if (_model)
			_model->SetTextureLOD(level);
	}

	float MPSceneObject::GetOpacity() {
		return _model ? _model->GetOpacity() : 0.0f;
	}

	DWORD MPSceneObject::GetTransparentFlag() const {
		DWORD pri_num = _model->GetPrimitiveNum();
		for (DWORD i = 0; i < pri_num; i++) {
			if (_model)
				if (_model->GetPrimitive(i)->GetState(STATE_TRANSPARENT))
					return 1;
		}

		return 0;
	}

	// lwSceneObjectChair
	LW_RESULT lwLoadPrimitiveLineList(lwIPrimitive* obj, const char* name, DWORD vert_num, const lwVector3* vert_buf,
									  const DWORD* color_buf);

	lwSceneObjectChair::lwSceneObjectChair()
		: _obj(0) {
		BindMatrix(&_mat);
	}

	lwSceneObjectChair::~lwSceneObjectChair() {
		LW_SAFE_DELETE(_obj);
	}

	LW_RESULT lwSceneObjectChair::Create() {
		lwMatrix44Identity(&_mat);

		_box.c = lwVector3(0.0f, 0.0f, 0.0f);
		_box.r = lwVector3(0.5f, 0.5f, 0.5f);

		lwSysGraphics::GetActiveIGraphicsSystem()->GetResourceMgr()->CreatePrimitive(&_obj);

		const DWORD vert_num = 24 + 6;
		lwVector3 vert_buf[vert_num];
		lwGetBoxVertLineList(vert_buf, &_box);

		DWORD c = 0xff00ffff;
		DWORD color_buf[vert_num];
		for (DWORD i = 0; i < 24; i++) {
			color_buf[i] = c;
		}

		// axis
		const float len = 1.0f;
		lwVector3 ver_buf[6] =
		{
			lwVector3(0.0f, 0.0f, 0.0f),
			lwVector3(len, 0.0f, 0.0f),
			lwVector3(0.0f, 0.0f, 0.0f),
			lwVector3(0.0f, len, 0.0f),
			lwVector3(0.0f, 0.0f, 0.0f),
			lwVector3(0.0f, 0.0f, len)
		};

		memcpy(&vert_buf[24], &ver_buf[0], sizeof(lwVector3) * 6);

		DWORD* color_axis = &color_buf[24];
		color_axis[0] = color_axis[1] = 0xffff0000;
		color_axis[2] = color_axis[3] = 0xff00ff00;
		color_axis[4] = color_axis[5] = 0xff0000ff;


		lwLoadPrimitiveLineList(_obj, "ChairBox", vert_num, vert_buf, color_buf);

		return LW_RET_OK;
	}

	LW_RESULT lwSceneObjectChair::Render() {
		if (_obj) {
			_obj->SetMatrixParent(&_mat);
			if (LW_RESULT r = _obj->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->Render failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwSceneObjectChair::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) {
		return lwHitTestBox(info, org, ray, &_box, &_mat);
	}


LW_END
