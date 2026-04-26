//
#include "stdafx.h"


#include "lwItem.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwResourceMgr.h"
#include "lwAnimCtrl.h"
#include "lwRenderImp.h"
#include "lwPathInfo.h"
#include "lwD3D.h"
#include "lwShaderMgr.h"

LW_BEGIN
	//lwItem
	LW_STD_IMPLEMENTATION(lwItem)

	lwItem::lwItem(lwIResourceMgr* res_mgr)
		: _id(LW_INVALID_INDEX), _res_mgr(res_mgr), _obj(0), _link_ctrl(0) {
		_opacity = 1.0f;
		lwMatrix44Identity(&_mat_base);
	}

	lwItem::~lwItem() {
		Destroy();
	}

	LW_RESULT lwItem::Load(lwIGeomObjInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_obj)
			goto __ret;

		{
			lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
			lwISystem* sys = sys_graphics->GetSystem();
			lwIPathInfo* path_info = 0;
			sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);

			// create new item object
			lwPrimitive* imp = LW_NEW(lwPrimitive(_res_mgr));


			if (LW_RESULT r = imp->Load(info, path_info->GetPath(PATH_TYPE_TEXTURE_ITEM).c_str(), NULL); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] imp->Load failed: tex_path={}, ret={}",
							 __FUNCTION__, path_info->GetPath(PATH_TYPE_TEXTURE_ITEM), static_cast<long long>(r));
				goto __ret;
			}

			_obj = imp;

			_res_mgr->RegisterObject(&_id, this, OBJ_TYPE_ITEM);

			RegisterSceneMgr(_res_mgr->GetSysGraphics()->GetSceneMgr());

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	LW_RESULT lwItem::Load(const char* file, int arbitrary_flag) {
		if (_obj) {
			return LW_RET_FAILED;
		}

		LW_RESULT ret = LW_RET_OK;


		lwISysGraphics* sys_graphics = _res_mgr->GetSysGraphics();
		lwISystem* sys = sys_graphics->GetSystem();
		lwIPathInfo* path_info = 0;
		sys->GetInterface((LW_VOID**)&path_info, LW_GUID_PATHINFO);

		lwItem* obj_model = nullptr;

		if (arbitrary_flag == 1)
			goto __a;

		// query model pool

		_res_mgr->QueryObject((void**)&obj_model, OBJ_TYPE_ITEM, file);

		if (obj_model) {
			// copy
			obj_model->Copy(this);
		}
		else {
		__a:
			const std::string path = std::format("{}{}", path_info->GetPath(PATH_TYPE_MODEL_ITEM), file);

			lwResFile res;
			res.obj_id = 0;
			res.res_type = RES_FILE_TYPE_GEOMETRY;
			std::memset(res.file_name, 0, sizeof(res.file_name));
			std::memcpy(res.file_name, path.data(),
						std::min<std::size_t>(path.size(), sizeof(res.file_name) - 1));

			// create new item object
			lwPrimitive* imp = LW_NEW(lwPrimitive( _res_mgr ));


			// begin load mesh
			lwGeomObjInfo info;

			if (LW_RESULT r = info.Load(path.c_str()); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] info.Load failed: path={}, ret={}",
							 __FUNCTION__, path, static_cast<long long>(r));
				return LW_RET_FAILED;
			}

			if (LW_RESULT r = imp->Load(&info, path_info->GetPath(PATH_TYPE_TEXTURE_ITEM).c_str(), &res); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] imp->Load failed: path={}, file={}, ret={}",
							 __FUNCTION__, path, file ? file : "(null)", static_cast<long long>(r));
				return LW_RET_FAILED;
			}

			_obj = imp;

			SetFileName(file);
		}

		_res_mgr->RegisterObject(&_id, this, OBJ_TYPE_ITEM);

		RegisterSceneMgr(_res_mgr->GetSysGraphics()->GetSceneMgr());

		return ret;
	}

	LW_RESULT lwItem::Destroy() {
		if (_obj == 0)
			return LW_RET_FAILED;

		_obj->Destroy();
		LW_DELETE(_obj);
		_obj = 0;

		_res_mgr->UnregisterObject(NULL, _id, OBJ_TYPE_ITEM);
		_id = LW_INVALID_INDEX;

		return LW_RET_OK;
	}

	LW_RESULT lwItem::Copy(lwIItem* src_obj) {
		lwItem* o = (lwItem*)src_obj;

		_file_name = o->_file_name;

		o->_obj->Clone(&_obj);

		_id = o->_id;
		_item_type = o->_item_type;
		_mat_base = o->_mat_base;
		_link_ctrl = o->_link_ctrl;
		_link_item_id = o->_link_item_id;
		_link_parent_id = o->_link_parent_id;

		return LW_RET_OK;
	}

	LW_RESULT lwItem::Clone(lwIItem** ret_obj) {
		lwItem* o;
		_res_mgr->CreateItem((lwIItem**)&o);

		o->Copy(this);

		*ret_obj = o;


		return LW_RET_OK;
	}

	LW_RESULT lwItem::Update() {
		LW_RESULT ret = LW_RET_FAILED;

		if (_obj) {
			if (_link_ctrl) {
				lwMatrix44 mat_parent;

				if (LW_RESULT r = _link_ctrl->GetLinkCtrlMatrix(&mat_parent, _link_parent_id); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _link_ctrl->GetLinkCtrlMatrix failed: link_parent_id={}, ret={}",
								 __FUNCTION__, _link_parent_id, static_cast<long long>(r));
					goto __ret;
				}

				lwMatrix44 mat_dummy;

				if (LW_RESULT r = GetDummyMatrix(&mat_dummy, _link_item_id); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] GetDummyMatrix failed: link_item_id={}, ret={}",
								 __FUNCTION__, _link_item_id, static_cast<long long>(r));
					goto __ret;
				}


				lwMatrix44InverseNoScaleFactor(&mat_dummy, &mat_dummy);

				lwMatrix44Multiply(&_mat_base, &mat_dummy, &mat_parent);
			}

			_obj->SetMatrixParent(&_mat_base);

			if (LW_RESULT r = _obj->Update(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj->Update failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}
		}

		ret = LW_RET_OK;

	__ret:

		return ret;
	}

	LW_RESULT lwItem::Render() {
		LW_RESULT ret = LW_RET_FAILED;

		if (_state_ctrl.GetState(STATE_VISIBLE) == 0)
			goto __addr_ok;


		if (_obj) {
			if (_scene_mgr && _obj->GetState(STATE_TRANSPARENT)) {
				if (LW_RESULT r = _scene_mgr->AddTransparentPrimitive(_obj); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] AddTransparentPrimitive failed: ret={}",
								 __FUNCTION__, static_cast<long long>(r));
					goto __ret;
				}
			}
			else {
				if (LW_RESULT r = _obj->Render(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _obj->Render failed: ret={}",
								 __FUNCTION__, static_cast<long long>(r));
					goto __ret;
				}
			}
		}

	__addr_ok:
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	void lwItem::SetMaterial(const lwMaterial* mtl) {
		if (_obj) {
			_obj->SetMaterial(mtl);
		}
	}

	LW_RESULT lwItem::HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) {
		lwPickInfo p, x;
		x.dis = -1.0f;

		if (LW_SUCCEEDED(_obj->HitTest( &p, org, ray ))) {
			if ((x.dis == -1.0f) || (x.dis > p.dis)) {
				x = p;
			}
		}

		if (x.dis == -1.0f) {
			return LW_RET_FAILED;
		}
		else {
			*info = x;
			return LW_RET_OK;
		}
	}

	void lwItem::ShowBoundingObject(int show) {
		if (_obj) {
			if (_obj->GetHelperObject()) {
				_obj->GetHelperObject()->SetVisible(show);
			}
		}
	}

	const lwMatrix44* lwItem::GetObjDummyMatrix(DWORD id) {
		lwMatrix44* ret = 0;

		if (_obj == 0)
			goto __ret;
		{
			lwIHelperObject* h = _obj->GetHelperObject();
			if (h == 0)
				goto __ret;
			{
				lwIHelperDummy* hd = h->GetHelperDummy();
				if (hd == 0)
					goto __ret;

				{
					lwHelperDummyInfo* info = hd->GetDataInfoWithID(id);
					if (info == 0)
						goto __ret;

					ret = &info->mat;
				}
			}
		}
	__ret:
		return ret;
	}

	const lwMatrix44* lwItem::GetObjBoneDummyMatrix(DWORD id) {
		lwIAnimCtrlAgent* anim_agent = _obj->GetAnimAgent();

		lwAnimCtrlObjTypeInfo type_info;
		type_info.type = ANIM_CTRL_TYPE_BONE;
		type_info.data[0] = LW_INVALID_INDEX;
		type_info.data[1] = LW_INVALID_INDEX;

		lwIAnimCtrlObj* aca_bone = (lwIAnimCtrlObj*)anim_agent->GetAnimCtrlObj(&type_info);
		lwIAnimCtrlBone* c = (lwIAnimCtrlBone*)aca_bone->GetAnimCtrl();
		if (c == 0)
			return NULL;

		DWORD rtmat_num = c->GetDummyNum();
		lwIndexMatrix44* rtmat_seq = c->GetDummyRTMSeq();

		for (DWORD i = 0; i < rtmat_num; i++) {
			if (rtmat_seq[i].id == id) {
				return &rtmat_seq[id].mat;
			}
		}

		return NULL;
	}

	LW_RESULT lwItem::GetObjDummyRunTimeMatrix(lwMatrix44* mat, DWORD id) {
		LW_RESULT ret = LW_RET_FAILED;

		lwMatrix44 mat_dummy;

		if (_obj == 0)
			goto __ret;


		if (LW_RESULT r = GetDummyMatrix(&mat_dummy, id); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] GetDummyMatrix failed: id={}, ret={}",
						 __FUNCTION__, id, static_cast<long long>(r));
			goto __ret;
		}

		lwMatrix44Multiply(mat, &mat_dummy, _obj->GetMatrixGlobal());

		ret = LW_RET_OK;

	__ret:
		return ret;
	}


	LW_RESULT lwItem::GetDummyMatrix(lwMatrix44* mat, DWORD id) {
		LW_RESULT ret = LW_RET_FAILED;

		const lwMatrix44* mat_dummy;

		lwIAnimCtrlAgent* anim_agent = _obj->GetAnimAgent();
		if (anim_agent == 0)
			goto __addr_obj_dummy;
		{
			lwAnimCtrlObjTypeInfo type_info;
			type_info.type = ANIM_CTRL_TYPE_BONE;
			type_info.data[0] = LW_INVALID_INDEX;
			type_info.data[1] = LW_INVALID_INDEX;

			lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
			if (ctrl_obj == 0)
				goto __addr_obj_dummy;

			if (ctrl_obj->GetAnimCtrl() == 0)
				goto __addr_obj_dummy;


			mat_dummy = GetObjBoneDummyMatrix(id);
			goto __addr_1;
		}
	__addr_obj_dummy:
		mat_dummy = GetObjDummyMatrix(id);

	__addr_1:
		if (mat_dummy == 0)
			goto __ret;

		if (mat) {
			*mat = *mat_dummy;
		}

		ret = LW_RET_OK;

	__ret:
		return ret;
	}

	LW_RESULT lwItem::SetLinkCtrl(lwLinkCtrl* ctrl, DWORD link_parent_id, DWORD link_item_id) {
		if (LW_RESULT r = GetDummyMatrix(NULL, link_item_id); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] GetDummyMatrix failed: link_item_id={}, ret={}",
						 __FUNCTION__, link_item_id, static_cast<long long>(r));
			return LW_RET_FAILED;
		}

		_link_ctrl = ctrl;
		_link_parent_id = link_parent_id;
		_link_item_id = link_item_id;

		return LW_RET_OK;
	}

	LW_RESULT lwItem::ClearLinkCtrl() {
		_link_ctrl = 0;
		_link_parent_id = LW_INVALID_INDEX;
		_link_item_id = LW_INVALID_INDEX;

		return LW_RET_OK;
	}

	LW_RESULT lwItem::PlayDefaultAnimation(bool IsGlitched) {
		return lwPrimitivePlayDefaultAnimation(_obj, IsGlitched);
	}

	void lwItem::SetOpacity(float opacity) {
		if (_obj) {
			_opacity = opacity;
			_obj->SetOpacity(opacity);
		}
	}

	LW_RESULT lwItem::SetTextureLOD(DWORD level) {
		if (_obj) {
			_obj->SetTextureLOD(level);
		}

		return LW_RET_OK;
	}

LW_END
