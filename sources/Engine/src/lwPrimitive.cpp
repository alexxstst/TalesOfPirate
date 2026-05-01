//

#include "stdafx.h"


#include "lwPrimitive.h"
#include "lwSystem.h"
#include "lwSysGraphics.h"
#include "lwAnimCtrl.h"
#include "lwPathInfo.h"
#include "lwD3D.h"
#include "lwShaderMgr.h"
#include "lwResourceMgr.h"
#include "lwRenderImp.h"

#define USE_VS_INDEXED

LW_BEGIN
	//LW_STD_RELEASE( lwPrimitive )
	//    //switch( guid )
	//    //{
	//    //default:
	//    //}
	//    return ret;
	LW_STD_IMPLEMENTATION(lwPrimitive)

	// lwPrimitive
	lwPrimitive::lwPrimitive(lwIResourceMgr* mgr)
		: _res_mgr(mgr), _id(LW_INVALID_INDEX), _parent_id(LW_INVALID_INDEX), _helper_object(0) {
		_anim_agent = NULL;
		_render_agent = NULL;
		_mesh_agent = NULL;

		memset(_mtltex_agent_seq, 0, sizeof(_mtltex_agent_seq));

		_state_ctrl.SetState(STATE_FRAMECULLING, 0);
	}

	lwPrimitive::~lwPrimitive() {
		Destroy();
	}

	LW_RESULT lwPrimitive::_UpdateTransparentState() {
		// i recommend this procedure to be invoked when initialize and something changing
		// of transparent state such as opacity, additive/subtractive blend state.
		// consequently it is not advised to update per-frame
		BYTE state = 1;
		lwIMtlTexAgent* mtltex_agent;

		if (_state_ctrl.GetState(STATE_UPDATETRANSPSTATE) == 0)
			goto __ret;
		{
			for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
				if ((mtltex_agent = _mtltex_agent_seq[i]) == NULL)
					break;

				if (mtltex_agent->GetOpacity() != 1.0f)
					goto __set_state;

				if (mtltex_agent->GetTransparencyType() != MTLTEX_TRANSP_FILTER)
					goto __set_state;
			}

			state = 0;
		}
	__set_state:
		_state_ctrl.SetState(STATE_TRANSPARENT, state);
	__ret:
		return LW_RET_OK;
	}

	LW_RESULT lwPrimitive::DestroyMtlTexAgent(DWORD subset) {
		LW_SAFE_RELEASE(_mtltex_agent_seq[ subset ]);
		return LW_RET_OK;
	}

	LW_RESULT lwPrimitive::DestroyMeshAgent() {
		LW_SAFE_RELEASE(_mesh_agent);
		return LW_RET_OK;
	}

	LW_RESULT lwPrimitive::DestroyRenderCtrlAgent() {
		LW_SAFE_RELEASE(_render_agent);
		return LW_RET_OK;
	}

	LW_RESULT lwPrimitive::Destroy() {
		DestroyMeshAgent();

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			DestroyMtlTexAgent(i);
		}

		LW_SAFE_RELEASE(_anim_agent);
		LW_SAFE_RELEASE(_helper_object);
		DestroyRenderCtrlAgent();

		return LW_RET_OK;
	}


	LW_RESULT lwPrimitive::LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, std::string_view tex_path) {
		LW_RESULT ret = LW_RET_FAILED;

		if (mtl_id < 0 || mtl_id >= LW_MAX_SUBSET_NUM)
			goto __ret;

		if (LW_RESULT r = _res_mgr->CreateMtlTexAgent(&_mtltex_agent_seq[mtl_id]); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] CreateMtlTexAgent failed: mtl_id={}, ret={}",
						 __FUNCTION__, static_cast<long long>(mtl_id), static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _mtltex_agent_seq[mtl_id]->LoadMtlTex(info, tex_path); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] LoadMtlTex failed: mtl_id={}, tex_path='{}', ret={}",
						 __FUNCTION__, static_cast<long long>(mtl_id),
						 (tex_path.empty() ? std::string_view{"(null)"} : tex_path), static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::Load(lwIGeomObjInfo* geom_info, std::string_view tex_path, const lwResFile* res) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD i;

		lwGeomObjInfo* info = (lwGeomObjInfo*)geom_info;

		lwISystem* sys = _res_mgr->GetSysGraphics()->GetSystem();
		lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
		BYTE create_helper_primitive = opt_mgr->GetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE);


		if (LW_RESULT r = LoadRenderCtrl(&info->rcci); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] LoadRenderCtrl failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		// begin base info

		_id = info->id;
		_parent_id = info->parent_id;
		_render_agent->SetLocalMatrix(&info->mat_local);
		_state_ctrl = info->state_ctrl;

		// mtltex info
		for (i = 0; i < info->mtl_num; i++) {
			if (LW_RESULT r = LoadMtlTex(i, &info->mtl_seq[i], tex_path); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] LoadMtlTex failed: i={}, tex_path='{}', ret={}",
							 __FUNCTION__, static_cast<long long>(i),
							 (tex_path.empty() ? std::string_view{"(null)"} : tex_path), static_cast<long long>(r));
				goto __ret;
			}
		}

		// mesh info
		if (LW_RESULT r = LoadMesh(&info->mesh); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] LoadMesh failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		if (res) {
			lwResFileMesh info;
			info.obj_id = res->obj_id;
			info.res_type = res->res_type;
			_tcscpy(info.file_name, res->file_name);

			_mesh_agent->GetMesh()->SetResFile(&info);
		}

		// helper info
		if (info->helper_data.type != HELPER_TYPE_INVALID) {
			LW_IF_RELEASE(_helper_object);
			_res_mgr->CreateHelperObject(&_helper_object);
			if (LW_RESULT r = _helper_object->LoadHelperInfo(&info->helper_data, create_helper_primitive);
				LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] LoadHelperInfo failed: create_helper_primitive={}, ret={}",
							 __FUNCTION__, static_cast<long long>(create_helper_primitive),
							 static_cast<long long>(r));
				LG_MSGBOX("LoadHelperInfo");
			}
		}

		// anim info
		if (info->anim_size > 0) {
			if (LW_RESULT r = LoadAnimData(&info->anim_data, tex_path, res); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] LoadAnimData failed: tex_path='{}', ret={}",
							 __FUNCTION__, (tex_path.empty() ? std::string_view{"(null)"} : tex_path),
							 static_cast<long long>(r));
				goto __ret;
			}
		}


		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::LoadMesh(lwMeshInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		LW_SAFE_RELEASE(_mesh_agent);

		if (LW_RESULT r = _res_mgr->CreateMeshAgent(&_mesh_agent); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] CreateMeshAgent failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _mesh_agent->LoadMesh(info); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] LoadMesh(lwMeshInfo*) failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::LoadMesh(const lwResFileMesh* info) {
		LW_RESULT ret = LW_RET_FAILED;

		LW_SAFE_RELEASE(_mesh_agent);

		if (LW_RESULT r = _res_mgr->CreateMeshAgent(&_mesh_agent); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] CreateMeshAgent failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _mesh_agent->LoadMesh(info); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] LoadMesh(lwResFileMesh*) failed: file_name='{}', ret={}",
						 __FUNCTION__, info ? info->file_name : "(null)",
						 static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci) {
		LW_RESULT ret = LW_RET_FAILED;

		lwIRenderCtrl* render_ctrl = 0;
		lwIRenderCtrlAgent* render_agent = 0;

		if (_render_agent)
			goto __ret;


		render_agent = LW_NEW(lwRenderCtrlAgent(_res_mgr));
		render_agent->SetVertexDeclaration(rcci->decl_id);
		render_agent->SetVertexShader(rcci->vs_id);

		if (LW_RESULT r = render_agent->SetRenderCtrl(rcci->ctrl_id); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] SetRenderCtrl failed: ctrl_id={}, ret={}",
						 __FUNCTION__, static_cast<long long>(rcci->ctrl_id),
						 static_cast<long long>(r));
			goto __ret;
		}

		_render_agent = render_agent;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::LoadAnimData(lwIAnimDataInfo* data_info, std::string_view tex_path, const lwResFile* res) {
		LW_RESULT ret = LW_RET_FAILED;

		lwAnimDataInfo* info = (lwAnimDataInfo*)data_info;

		void* data;
		lwAnimCtrlObjTypeInfo type_info;

		if (_anim_agent == 0) {
			if (LW_RESULT r = _res_mgr->CreateAnimCtrlAgent(&_anim_agent); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrlAgent failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}
		}


		if (info->anim_mat) {
			data = info->anim_mat;

			type_info.type = ANIM_CTRL_TYPE_MAT;
			type_info.data[0] = LW_INVALID_INDEX;
			type_info.data[1] = LW_INVALID_INDEX;

			lwIAnimCtrlMatrix* ctrl = NULL;
			lwIAnimCtrlObjMat* ctrl_obj = NULL;

			if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrlObj(MAT) failed: type={}, ret={}",
							 __FUNCTION__, static_cast<long long>(type_info.type),
							 static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrl(MAT) failed: type={}, ret={}",
							 __FUNCTION__, static_cast<long long>(type_info.type),
							 static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] AnimCtrl(MAT)::LoadData failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			ctrl_obj->AttachAnimCtrl(ctrl);
			ctrl_obj->SetTypeInfo(&type_info);
			_anim_agent->AddAnimCtrlObj(ctrl_obj);
		}
		if (info->anim_bone) {
			data = info->anim_bone;

			type_info.type = ANIM_CTRL_TYPE_BONE;
			type_info.data[0] = LW_INVALID_INDEX;
			type_info.data[1] = LW_INVALID_INDEX;

			lwIAnimCtrlMatrix* ctrl = NULL;
			lwIAnimCtrlObjMat* ctrl_obj = NULL;


			if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrlObj(BONE) failed: type={}, ret={}",
							 __FUNCTION__, static_cast<long long>(type_info.type),
							 static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrl(BONE) failed: type={}, ret={}",
							 __FUNCTION__, static_cast<long long>(type_info.type),
							 static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] AnimCtrl(BONE)::LoadData failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			ctrl_obj->AttachAnimCtrl(ctrl);
			ctrl_obj->SetTypeInfo(&type_info);
			_anim_agent->AddAnimCtrlObj(ctrl_obj);
		}


		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (_mtltex_agent_seq[i] == NULL)
				continue;

			if (info->anim_mtlopac[i]) {
				data = info->anim_mtlopac[i];

				type_info.type = ANIM_CTRL_TYPE_MTLOPACITY;
				type_info.data[0] = i;
				type_info.data[1] = LW_INVALID_INDEX;


				lwIAnimCtrlMtlOpacity* ctrl = NULL;
				lwIAnimCtrlObjMtlOpacity* ctrl_obj = NULL;

				if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type);
					LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] CreateAnimCtrlObj(MTLOPACITY) failed: i={}, type={}, ret={}",
								 __FUNCTION__, static_cast<long long>(i),
								 static_cast<long long>(type_info.type),
								 static_cast<long long>(r));
					goto __ret;
				}

				if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] CreateAnimCtrl(MTLOPACITY) failed: i={}, type={}, ret={}",
								 __FUNCTION__, static_cast<long long>(i),
								 static_cast<long long>(type_info.type),
								 static_cast<long long>(r));
					goto __ret;
				}

				if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] AnimCtrl(MTLOPACITY)::LoadData failed: i={}, ret={}",
								 __FUNCTION__, static_cast<long long>(i),
								 static_cast<long long>(r));
					goto __ret;
				}

				ctrl_obj->AttachAnimCtrl(ctrl);
				ctrl_obj->SetTypeInfo(&type_info);
				_anim_agent->AddAnimCtrlObj(ctrl_obj);
			}

			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				if (info->anim_tex[i][j]) {
					data = info->anim_tex[i][j];

					type_info.type = ANIM_CTRL_TYPE_TEXUV;
					type_info.data[0] = i;
					type_info.data[1] = j;


					lwIAnimCtrlTexUV* ctrl = NULL;
					lwIAnimCtrlObjTexUV* ctrl_obj = NULL;

					if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type);
						LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] CreateAnimCtrlObj(TEXUV) failed: i={}, j={}, type={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(j),
									 static_cast<long long>(type_info.type),
									 static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] CreateAnimCtrl(TEXUV) failed: i={}, j={}, type={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(j),
									 static_cast<long long>(type_info.type),
									 static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrl(TEXUV)::LoadData failed: i={}, j={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(j),
									 static_cast<long long>(r));
						goto __ret;
					}

					ctrl_obj->AttachAnimCtrl(ctrl);
					ctrl_obj->SetTypeInfo(&type_info);
					_anim_agent->AddAnimCtrlObj(ctrl_obj);
				}

				// image
				if (info->anim_img[i][j]) {
					data = info->anim_img[i][j];

					type_info.type = ANIM_CTRL_TYPE_TEXIMG;
					type_info.data[0] = i;
					type_info.data[1] = j;

					{const auto _t = std::string{tex_path}; _tcscpy(info->anim_img[i][j]->_tex_path, _t.c_str());}

					lwIAnimCtrlTexImg* ctrl = NULL;
					lwIAnimCtrlObjTexImg* ctrl_obj = NULL;

					if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type);
						LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] CreateAnimCtrlObj(TEXIMG) failed: i={}, j={}, type={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(j),
									 static_cast<long long>(type_info.type),
									 static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] CreateAnimCtrl(TEXIMG) failed: i={}, j={}, type={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(j),
									 static_cast<long long>(type_info.type),
									 static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrl(TEXIMG)::LoadData failed: i={}, j={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(j),
									 static_cast<long long>(r));
						goto __ret;
					}

					ctrl_obj->AttachAnimCtrl(ctrl);
					ctrl_obj->SetTypeInfo(&type_info);
					_anim_agent->AddAnimCtrlObj(ctrl_obj);
				}
			}
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::Clone(lwIPrimitive** ret_obj) {
		LW_RESULT ret = LW_RET_FAILED;

		lwIPrimitive* o;

		_res_mgr->CreatePrimitive(&o);

		o->SetID(_id);
		o->SetParentID(_parent_id);
		o->SetState(&_state_ctrl);

		if (_mesh_agent) {
			lwIMeshAgent* mesh_agent;
			if (LW_RESULT r = _mesh_agent->Clone(&mesh_agent); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] MeshAgent::Clone failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			o->SetMeshAgent(mesh_agent);
		}

		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (_mtltex_agent_seq[i] == NULL)
				continue;

			lwIMtlTexAgent* mtltex_agent;
			if (LW_RESULT r = _mtltex_agent_seq[i]->Clone(&mtltex_agent); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] MtlTexAgent::Clone failed: i={}, ret={}",
							 __FUNCTION__, static_cast<long long>(i),
							 static_cast<long long>(r));
				goto __ret;
			}

			o->SetMtlTexAgent(i, mtltex_agent);
		}

		if (_anim_agent) {
			lwIAnimCtrlAgent* anim_agent;
			if (LW_RESULT r = _anim_agent->Clone(&anim_agent); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] AnimCtrlAgent::Clone failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}
			o->SetAnimCtrlAgent(anim_agent);
		}

		if (_render_agent) {
			lwIRenderCtrlAgent* render_agent;
			if (LW_RESULT r = _render_agent->Clone(&render_agent); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] RenderCtrlAgent::Clone failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}
			o->SetRenderCtrl(render_agent);
		}

		if (_helper_object) {
			lwIHelperObject* helper_obj;
			_helper_object->Clone(&helper_obj);
			o->SetHelperObject(helper_obj);
		}

		*ret_obj = o;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	void lwPrimitive::SetMatrixLocal(const lwMatrix44* mat) {
		if (_render_agent) {
			_render_agent->SetLocalMatrix(mat);
		}
	}

	void lwPrimitive::SetMatrixParent(const lwMatrix44* mat) {
		if (_render_agent) {
			_render_agent->SetParentMatrix(mat);
		}
	}

	void lwPrimitive::SetMaterial(const lwMaterial* mtl) {
		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (_mtltex_agent_seq[i] == NULL)
				continue;

			_mtltex_agent_seq[i]->SetMaterial(mtl);
		}
	}

	void lwPrimitive::SetOpacity(float opacity) {
		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (_mtltex_agent_seq[i] == NULL)
				continue;

			_mtltex_agent_seq[i]->SetOpacity(opacity);
		}
	}

	LW_RESULT lwPrimitive::Update() {
		LW_RESULT ret = LW_RET_FAILED;

		if (_anim_agent) {
			if (LW_RESULT r = _anim_agent->Update(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] AnimAgent::Update failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			lwAnimCtrlObjTypeInfo type_info;
			lwIAnimCtrlObj* ctrl_obj;
			DWORD anim_ctrl_num = _anim_agent->GetAnimCtrlObjNum();
			for (DWORD i = 0; i < anim_ctrl_num; i++) {
				ctrl_obj = _anim_agent->GetAnimCtrlObj(i);

				if (ctrl_obj->GetAnimCtrl() && !ctrl_obj->IsPlaying())
					continue;

				ctrl_obj->GetTypeInfo(&type_info);
				switch (type_info.type) {
				case ANIM_CTRL_TYPE_MAT: {
					if (LW_RESULT r = ((lwIAnimCtrlObjMat*)ctrl_obj)->UpdateObject(); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjMat::UpdateObject failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}

					lwMatrix44 mat;
					if (LW_RESULT r = ((lwIAnimCtrlObjMat*)ctrl_obj)->GetRTM(&mat); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjMat::GetRTM failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}
					_render_agent->SetLocalMatrix(&mat);
				}
				break;
				case ANIM_CTRL_TYPE_BONE: {
					if (LW_RESULT r = ((lwIAnimCtrlObjBone*)ctrl_obj)->UpdateObject(
						(lwIAnimCtrlObjBone*)ctrl_obj, _mesh_agent->GetMesh()); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjBone::UpdateObject failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = ((lwIAnimCtrlObjBone*)ctrl_obj)->UpdateHelperObject(_helper_object);
						LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjBone::UpdateHelperObject failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}
				}
				break;
				case ANIM_CTRL_TYPE_MTLOPACITY: {
					if (LW_RESULT r = ((lwIAnimCtrlObjMtlOpacity*)ctrl_obj)->UpdateObject(); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjMtlOpacity::UpdateObject failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}

					lwIMtlTexAgent* mtltex_agent;
					mtltex_agent = GetMtlTexAgent(type_info.data[0]);
					if (mtltex_agent == 0)
						goto __ret;

					float o = 1.0f;
					((lwIAnimCtrlObjMtlOpacity*)ctrl_obj)->GetRunTimeOpacity(&o);
					mtltex_agent->SetOpacity(o);
				}
				break;
				case ANIM_CTRL_TYPE_TEXUV: {
					if (LW_RESULT r = ((lwIAnimCtrlObjTexUV*)ctrl_obj)->UpdateObject(); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjTexUV::UpdateObject failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}
				}
				break;
				case ANIM_CTRL_TYPE_TEXIMG: {
					lwITex* tex;
					lwIMtlTexAgent* mtltex_agent;

					if (LW_RESULT r = ((lwIAnimCtrlObjTexImg*)ctrl_obj)->UpdateObject(); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjTexImg::UpdateObject failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = ((lwIAnimCtrlObjTexImg*)ctrl_obj)->GetRunTimeTex(&tex); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjTexImg::GetRunTimeTex failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}

					mtltex_agent = GetMtlTexAgent(type_info.data[0]);
					if (mtltex_agent == 0)
						goto __ret;

					mtltex_agent->SetTextureTransformImage(type_info.data[1], tex);
				}
				break;
				default:
					;
				}
			}
		}

		if (_helper_object) {
			_helper_object->SetParentMatrix(_render_agent->GetGlobalMatrix());
		}

		_UpdateTransparentState();

		ret = LW_RET_OK;
	__ret:
		return ret;
	}


	lwMatrix44* lwPrimitive::GetMatrixLocal() {
		return _render_agent ? _render_agent->GetLocalMatrix() : NULL;
	}

	lwMatrix44* lwPrimitive::GetMatrixGlobal() {
		return _render_agent ? _render_agent->GetGlobalMatrix() : NULL;
	}

	LW_RESULT lwPrimitive::Render() {
		LW_RESULT ret = LW_RET_FAILED;

		if (_state_ctrl.GetState(STATE_VISIBLE) == 0)
			goto __addr_ret_ok;

		if (_mesh_agent->GetMesh()->IsLoadingOK() == 0) {
			goto __addr_ret_ok;
		}

		_render_agent->BindAnimCtrlAgent(_anim_agent);
		_render_agent->BindMeshAgent(_mesh_agent);

		if (LW_RESULT r = _render_agent->BeginSet(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] RenderAgent::BeginSet failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		{
			for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
				if (_mtltex_agent_seq[i] == NULL)
					break;

				if (!_mtltex_agent_seq[i]->GetRenderFlag())
					continue;

				if (_mtltex_agent_seq[i]->BeginPass() != RES_PASS_DEFAULT)
					continue;

				_render_agent->BindMtlTexAgent(_mtltex_agent_seq[i]);

				ret = _render_agent->BeginSetSubset(i);
				if (LW_FAILED(ret)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] RenderAgent::BeginSetSubset failed: i={}, ret={}",
								 __FUNCTION__, static_cast<long long>(i),
								 static_cast<long long>(ret));
					goto __ret;
				}

				if (ret == LW_RET_OK_1)
					goto __ret;

				if (LW_RESULT r = _render_agent->DrawSubset(i); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] RenderAgent::DrawSubset failed: i={}, ret={}",
								 __FUNCTION__, static_cast<long long>(i),
								 static_cast<long long>(r));
					goto __ret;
				}

				if (LW_RESULT r = _render_agent->EndSetSubset(i); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] RenderAgent::EndSetSubset failed: i={}, ret={}",
								 __FUNCTION__, static_cast<long long>(i),
								 static_cast<long long>(r));
					goto __ret;
				}

				_mtltex_agent_seq[i]->EndPass();
			}

			if (LW_RESULT r = _render_agent->EndSet(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] RenderAgent::EndSet failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			if (_helper_object) {
				if (LW_RESULT r = _helper_object->Render(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _helper_object->Render failed: ret={}",
								 __FUNCTION__, static_cast<long long>(r));
				}
			}
		}
	__addr_ret_ok:
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::RenderSubset(DWORD subset) {
		if (subset < 0 || subset >= LW_MAX_SUBSET_NUM)
			return ERR_INVALID_PARAM;

		if (_mtltex_agent_seq[subset] == NULL)
			return ERR_INVALID_PARAM;

		_render_agent->BindAnimCtrlAgent(_anim_agent);
		_render_agent->BindMeshAgent(_mesh_agent);
		_render_agent->BindMtlTexAgent(_mtltex_agent_seq[subset]);

		if (LW_RESULT r = _render_agent->BeginSet(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->BeginSet failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
		}
		if (LW_RESULT r = _render_agent->BeginSetSubset(subset); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->BeginSetSubset failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
		}
		if (LW_RESULT r = _render_agent->DrawSubset(subset); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->DrawSubset failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
		}
		if (LW_RESULT r = _render_agent->EndSetSubset(subset); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->EndSetSubset failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
		}
		if (LW_RESULT r = _render_agent->EndSet(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->EndSet failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
		}

		// subset render doest not render helper object


		return LW_RET_OK;
	}

	LW_RESULT lwPrimitive::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_helper_object == 0)
			goto __ret;

		{
			lwIBoundingSphere* bs = 0;
			lwIBoundingBox* bb = 0;

			lwMatrix44* mat = _render_agent->GetGlobalMatrix();

			if ((bs = _helper_object->GetBoundingSphere()) != 0) {
				if (LW_SUCCEEDED(bs->HitTest(info, org, ray, mat)))
					goto __addr_ret_ok;
			}

			if ((bb = _helper_object->GetBoundingBox()) != 0) {
				if (LW_SUCCEEDED(bb->HitTest(info, org, ray, mat)))
					goto __addr_ret_ok;
			}
		}
		goto __ret;

	__addr_ret_ok:

		ret = LW_RET_OK;

	__ret:

		return ret;
	}

	LW_RESULT lwPrimitive::SetTextureLOD(DWORD level) {
		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (_mtltex_agent_seq[i] == NULL)
				continue;

			_mtltex_agent_seq[i]->SetTextureLOD(level);
		}

		return LW_RET_OK;
	}

	LW_RESULT lwPrimitive::GetSubsetNum(DWORD* subset_num) {
		LW_RESULT ret = LW_RET_FAILED;

		if (subset_num == 0)
			goto __ret;
		{
			// use mesh subset?
			if (_mesh_agent == 0)
				goto __ret;
			{
				lwIMesh* mesh = _mesh_agent->GetMesh();
				if (mesh == 0)
					goto __ret;
				{
					lwMeshInfo* info = mesh->GetMeshInfo();
					if (info == 0)
						goto __ret;

					*subset_num = info->subset_num;
				}
			}
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::ExtractGeomObjInfo(lwIGeomObjInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		lwGeomObjInfo* a = (lwGeomObjInfo*)info;

		// render ctrl state
		a->rcci.ctrl_id = _render_agent->GetRenderCtrlVS()->GetType();
		a->rcci.vs_id = _render_agent->GetVertexShader();
		a->rcci.decl_id = _render_agent->GetVertexDeclaration();

		a->id = _id;
		a->parent_id = _parent_id;
		a->mat_local = *_render_agent->GetLocalMatrix();
		a->state_ctrl = _state_ctrl;

		// mesh
		if (_mesh_agent == 0)
			goto __ret;

		{
			lwIMesh* mesh = _mesh_agent->GetMesh();
			if (mesh == 0)
				goto __ret;

			if (LW_RESULT r = mesh->ExtractMesh(&a->mesh); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] Mesh::ExtractMesh failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			// mtltex
			{
				DWORD i = 0;
				for (; i < LW_MAX_SUBSET_NUM; i++) {
					if (_mtltex_agent_seq[i] == 0)
						break;

					if (LW_RESULT r = _mtltex_agent_seq[i]->ExtractMtlTex(&a->mtl_seq[i]); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] MtlTexAgent::ExtractMtlTex failed: i={}, ret={}",
									 __FUNCTION__, static_cast<long long>(i),
									 static_cast<long long>(r));
						goto __ret;
					}
				}
				a->mtl_num = i;

				// animation
				if (_anim_agent) {
					if (LW_RESULT r = _anim_agent->ExtractAnimData(&a->anim_data); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimAgent::ExtractAnimData failed: ret={}",
									 __FUNCTION__, static_cast<long long>(r));
						goto __ret;
					}
				}


				// helper info
				if (_helper_object) {
					if (LW_RESULT r = _helper_object->ExtractHelperInfo(&a->helper_data); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] HelperObject::ExtractHelperInfo failed: ret={}",
									 __FUNCTION__, static_cast<long long>(r));
						goto __ret;
					}
				}

				a->mtl_size = lwGetMtlTexInfoSize(&a->mtl_seq[0], a->mtl_num);
				a->mesh_size = lwMeshInfo_GetDataSize(&a->mesh);
				a->helper_size = a->helper_data.GetDataSize();
				a->anim_size = a->anim_data.GetDataSize();
			}
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwPrimitive::PlayDefaultAnimation(float velocity) {
		LW_RESULT ret = LW_RET_FAILED;

		lwPlayPoseInfo ppi;
		memset(&ppi, 0, sizeof(ppi));
		ppi.bit_mask = PPI_MASK_DEFAULT;
		ppi.pose = 0;
		ppi.frame = 0.0f;
		ppi.type = PLAY_LOOP;
		ppi.velocity = velocity;

		if (_anim_agent == NULL)
			goto __ret_ok;

		{
			lwIAnimCtrlObj* ctrl_obj;
			DWORD n = _anim_agent->GetAnimCtrlObjNum();
			for (DWORD i = 0; i < n; i++) {
				ctrl_obj = _anim_agent->GetAnimCtrlObj(i);
				if (LW_RESULT r = ctrl_obj->PlayPose(&ppi); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] ctrl_obj->PlayPose failed: i={}, n={}, ret={}",
								 __FUNCTION__, i, n, static_cast<long long>(r));
				}
			}
		}
	__ret_ok:
		ret = LW_RET_OK;
		return ret;
	}

	LW_RESULT lwPrimitivePlayDefaultAnimation(lwIPrimitive* obj, float velocity) {
		lwPlayPoseInfo ppi;
		memset(&ppi, 0, sizeof(ppi));
		ppi.bit_mask = PPI_MASK_DEFAULT;
		ppi.pose = 0;
		ppi.frame = 0.0f;
		ppi.type = PLAY_LOOP;
		//  velocity передаётся колсайтом: 1.0f для full-speed (legacy IsGlitched-режим
		//  на 30 FPS или для деревьев/моделей из hardcoded списка), либо 30.0f/fps
		//  для нормализованной анимации, синхронной с 30-FPS-видом.
		ppi.velocity = velocity;


		lwIAnimCtrlAgent* anim_agent = obj->GetAnimAgent();
		if (anim_agent == NULL)
			return LW_RET_FAILED;


		lwIAnimCtrlObj* ctrl_obj;
		DWORD n = anim_agent->GetAnimCtrlObjNum();

		for (DWORD i = 0; i < n; i++) {
			ctrl_obj = anim_agent->GetAnimCtrlObj(i);

			if (LW_RESULT r = ctrl_obj->PlayPose(&ppi); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] ctrl_obj->PlayPose failed: i={}, n={}, ret={}",
							 __FUNCTION__, i, n, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwPrimitive::ResetTexture(DWORD subset, DWORD stage, std::string_view file, std::string_view tex_path) {
		LW_RESULT ret = LW_RET_FAILED;

		lwTexInfo tex_info;
		lwITex* new_tex;

		if (subset >= LW_MAX_SUBSET_NUM)
			goto __ret;
		if (_mtltex_agent_seq[subset] == 0)
			goto __ret;

		{
			lwITex* tex = _mtltex_agent_seq[subset]->GetTex(stage);
			if (tex == 0)
				goto __ret;

			tex->GetTexInfo(&tex_info);
			{const auto _f = std::string{file}; _tcscpy(tex_info.file_name, _f.c_str());}
			tex_info.level = D3DX_DEFAULT;
			tex_info.type = TEX_TYPE_FILE;

			LW_FAILED_RET(_res_mgr->CreateTex(&new_tex));
			LW_FAILED_RET(new_tex->LoadTexInfo(&tex_info, tex_path));
			LW_FAILED_RET(new_tex->LoadVideoMemory());
			_mtltex_agent_seq[subset]->SetTex(stage, new_tex, &tex);
			LW_RELEASE(tex);
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}


LW_END
