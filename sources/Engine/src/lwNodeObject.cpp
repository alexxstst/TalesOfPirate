//
#include "stdafx.h"

#if 1

#include "lwNodeObject.h"
#include "lwMisc.h"
#include "lwIUtil.h"
#include "lwPrimitiveHelper.h"
#include "lwHelperGeometry.h"
#include "lwTreeNode.h"

LW_BEGIN
	// lwNodeBase
	lwNodeBase::lwNodeBase() {
		// base
		_type = MODELNODE_INVALID;
		_id = LW_INVALID_INDEX;
		lwMatrix44Identity(&_mat_local);
		lwMatrix44Identity(&_mat_world);

		// state set
		_state_set.Alloc(OBJECT_STATE_NUM);
		_state_set.SetValue(STATE_VISIBLE, 1);
		_state_set.SetValue(STATE_ENABLE, 1);

		// link state
		_parent = 0;
		_link_parent_id = LW_INVALID_INDEX;
		_link_id = LW_INVALID_INDEX;
	}

	//        // update world matrix
	//__ret:
	//    return ret;

	static DWORD __tree_proc_loadmodel(lwITreeNode* node, void* param) {
		DWORD ret = TREENODE_PROC_RET_ABORT;

		lwIResourceMgr* res_mgr = (lwIResourceMgr*)param;
		lwModelNodeInfo* node_info = (lwModelNodeInfo*)node->GetData();

		const std::string& tex_path = res_mgr->GetTexturePath();

		lwINode* model_node = 0;
		if (LW_RESULT r = res_mgr->CreateNode(&model_node, node_info->_type); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] res_mgr->CreateNode failed: type={}, id={}, ret={}",
						 __FUNCTION__, node_info->_type, node_info->_id, static_cast<long long>(r));
			goto __ret;
		}

		model_node->SetID(node_info->_id);
		// link_id and link_parent_id cannot be asigned here
		// because the initialize value of them is zero
		// which is valid for link_id and link_parent_id

		// now fixed it @_@
		model_node->SetLinkID(node_info->_link_id);
		model_node->SetParentLinkID(node_info->_link_parent_id);

		switch (node_info->_type) {
		case NODE_PRIMITIVE:
			if (LW_RESULT r = ((lwINodePrimitive*)model_node)->Load((lwIGeomObjInfo*)node_info->_data, tex_path, 0);
				LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] NodePrimitive::Load failed: id={}, tex_path={}, ret={}",
							 __FUNCTION__, node_info->_id, (tex_path.empty() ? std::string_view{"(null)"} : tex_path), static_cast<long long>(r));
				goto __ret;
			}
			break;
		case NODE_BONECTRL:
			if (LW_RESULT r = ((lwINodeBoneCtrl*)model_node)->Load((lwIAnimDataBone*)node_info->_data); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] NodeBoneCtrl::Load failed: id={}, ret={}",
							 __FUNCTION__, node_info->_id, static_cast<long long>(r));
				goto __ret;
			}
			break;
		case NODE_DUMMY:
			if (LW_RESULT r = ((lwINodeDummy*)model_node)->Load((lwIHelperDummyObjInfo*)node_info->_data);
				LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] NodeDummy::Load failed: id={}, ret={}",
							 __FUNCTION__, node_info->_id, static_cast<long long>(r));
				goto __ret;
			}
			break;
		case NODE_HELPER:
			assert(0);
			break;
		default:
			assert(0);
		}
		{
			lwITreeNode* model_tree = LW_NEW(lwTreeNode);
			model_tree->SetData(model_node);

			// set param
			node_info->_param = model_tree;

			// link tree parent
			lwITreeNode* node_parent = node->GetParent();
			if (node_parent) {
				lwModelNodeInfo* parent_info = (lwModelNodeInfo*)node_parent->GetData();
				lwITreeNode* parent_obj_node = (lwITreeNode*)parent_info->_param;

				parent_obj_node->InsertChild(0, model_tree);

				lwINode* parent_obj_data = (lwINode*)parent_obj_node->GetData();
				assert(parent_obj_data && "invalid parent_obj_data when called __tree_proc_loadmodel");

				model_node->SetParent(parent_obj_data);
			}
		}

		ret = TREENODE_PROC_RET_CONTINUE;
	__ret:
		return ret;
	}

	LW_RESULT lwLoadModelInfo(lwITreeNode** ret_obj_tree, lwModelInfo* info, lwIResourceMgr* res_mgr) {
		LW_RESULT ret = LW_RET_FAILED;

		if (info->_obj_tree->EnumTree(__tree_proc_loadmodel, (void*)res_mgr, TREENODE_PROC_PREORDER)
			== TREENODE_PROC_RET_ABORT)
			goto __ret;
		{
			lwModelNodeInfo* root_info = (lwModelNodeInfo*)info->_obj_tree->GetData();

			*ret_obj_tree = (lwITreeNode*)root_info->_param;
		}
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	// lwNodePrimitive
	LW_STD_IMPLEMENTATION(lwNodePrimitive)

	lwNodePrimitive::lwNodePrimitive(lwIResourceMgr* res_mgr)
		: _res_mgr(res_mgr) {
		_type = NODE_PRIMITIVE;

		_mesh_agent = 0;
		_mtltex_agent_seq = 0;
		_anim_agent = 0;
		_render_agent = 0;
		_helper_object = 0;
		_ref_ctrl_obj_bone = 0;

		_mtltex_agent_seqsize = 0;
	}

	lwNodePrimitive::~lwNodePrimitive() {
		Destroy();
	}

	LW_RESULT lwNodePrimitive::_DestroyMtlTexAgent() {
		if (_mtltex_agent_seqsize > 0) {
			for (DWORD i = 0; i < _mtltex_agent_seqsize; i++) {
				LW_IF_RELEASE(_mtltex_agent_seq[i]);
			}
			LW_DELETE_A(_mtltex_agent_seq);
			_mtltex_agent_seqsize = 0;
		}

		return LW_RET_OK;
	}

	LW_RESULT lwNodePrimitive::_UpdateBoundingObject(lwIAnimCtrlObjBone* ctrl_obj) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_helper_object == 0)
			goto __addr_ret_ok;

		lwIBoundingSphere* b;
		lwBoundingSphereInfo* s;

		if ((b = _helper_object->GetBoundingSphere()) == 0)
			goto __addr_ret_ok;
		{
			DWORD bs_num = b->GetObjNum();

			for (DWORD j = 0; j < bs_num; j++) {
				s = b->GetDataInfo(j);

				assert(s->id < LW_MAX_BONEDUMMY_NUM);

				lwMatrix44* mat = ctrl_obj->GetDummyRTM(s->id);
				if (mat) {
					s->mat = *mat;
				}
			}
		}

	__addr_ret_ok:
		ret = LW_RET_OK;

		return ret;
	}

	LW_RESULT lwNodePrimitive::_UpdateTransparentState() {
		// i recommend this procedure to be invoked when initialize and something changing
		// of transparent state such as opacity, additive/subtractive blend state.
		// consequently it is not advised to update per-frame
		DWORD i;
		BYTE state;
		lwIMtlTexAgent* mtltex_agent;

		if (_state_set.GetValue(STATE_UPDATETRANSPSTATE) == 0)
			goto __ret;

		state = 1;

		for (i = 0; i < _mtltex_agent_seqsize; i++) {
			if ((mtltex_agent = _mtltex_agent_seq[i]) == NULL)
				break;

			if (mtltex_agent->GetOpacity() != 1.0f)
				goto __set_state;

			if (mtltex_agent->GetTransparencyType() != MTLTEX_TRANSP_FILTER)
				goto __set_state;
		}

		state = 0;

	__set_state:
		_state_set.SetValue(STATE_TRANSPARENT, state);
	__ret:
		return LW_RET_OK;
	}

	LW_RESULT lwNodePrimitive::Destroy() {
		LW_SAFE_RELEASE(_mesh_agent);
		LW_SAFE_RELEASE(_anim_agent);
		LW_SAFE_RELEASE(_render_agent);
		LW_SAFE_RELEASE(_helper_object);

		_DestroyMtlTexAgent();

		return LW_RET_OK;
	}

	LW_RESULT lwNodePrimitive::AllocateMtlTexAgentSeq(DWORD num) {
		_DestroyMtlTexAgent();

		_mtltex_agent_seq = LW_NEW(lwIMtlTexAgent*[num]);
		_mtltex_agent_seqsize = num;
		memset(_mtltex_agent_seq, 0, sizeof(lwIMtlTexAgent*) * num);

		return LW_RET_OK;
	}

	LW_RESULT lwNodePrimitive::Load(lwIGeomObjInfo* geom_info, std::string_view tex_path, const lwResFile* res) {
		LW_RESULT ret = LW_RET_FAILED;

		DWORD i;

		lwGeomObjInfo* info = (lwGeomObjInfo*)geom_info;

		lwISystem* sys = _res_mgr->GetSysGraphics()->GetSystem();
		lwIOptionMgr* opt_mgr = sys->GetOptionMgr();
		BYTE create_helper_primitive = opt_mgr->GetByteFlag(OPTION_FLAG_CREATEHELPERPRIMITIVE);

		LoadRenderCtrl(&info->rcci);

		// base info
		// warning:_typeMODELNODE_
		// type

		// _id, _link_id, _link_parent_idlwLoadModelInfo
		_mat_local = info->mat_local;
		_state_set.SetValueSeq(0, info->state_ctrl._state_seq, OBJECT_STATE_NUM);

		// mtltex info
		if (LW_RESULT r = AllocateMtlTexAgentSeq(info->mtl_num); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] AllocateMtlTexAgentSeq failed: mtl_num={}, ret={}",
						 __FUNCTION__, info->mtl_num, static_cast<long long>(r));
			goto __ret;
		}

		for (i = 0; i < info->mtl_num; i++) {
			if (LW_RESULT r = LoadMtlTex(i, &info->mtl_seq[i], tex_path); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] LoadMtlTex failed: mtl_id={}, tex_path={}, ret={}",
							 __FUNCTION__, i, (tex_path.empty() ? std::string_view{"(null)"} : tex_path), static_cast<long long>(r));
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
							 "[{}] _helper_object->LoadHelperInfo failed: helper_type={}, ret={}",
							 __FUNCTION__, static_cast<std::uint32_t>(info->helper_data.type),
							 static_cast<long long>(r));
				LG_MSGBOX("LoadHelperInfo");
			}
		}

		// anim info
		if (info->anim_size > 0) {
			if (LW_RESULT r = LoadAnimData(&info->anim_data, tex_path, res); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] LoadAnimData failed: anim_size={}, ret={}",
							 __FUNCTION__, info->anim_size, static_cast<long long>(r));
				goto __ret;
			}
		}


		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::LoadMesh(lwMeshInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		LW_IF_RELEASE(_mesh_agent);

		if (LW_RESULT r = _res_mgr->CreateMeshAgent(&_mesh_agent); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _res_mgr->CreateMeshAgent failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _mesh_agent->LoadMesh(info); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _mesh_agent->LoadMesh failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, std::string_view tex_path) {
		LW_RESULT ret = LW_RET_FAILED;

		if (mtl_id < 0 || mtl_id >= LW_MAX_SUBSET_NUM)
			goto __ret;

		if (LW_RESULT r = _res_mgr->CreateMtlTexAgent(&_mtltex_agent_seq[mtl_id]); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _res_mgr->CreateMtlTexAgent failed: mtl_id={}, ret={}",
						 __FUNCTION__, mtl_id, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _mtltex_agent_seq[mtl_id]->LoadMtlTex(info, tex_path); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _mtltex_agent_seq[mtl_id]->LoadMtlTex failed: mtl_id={}, tex_path={}, ret={}",
						 __FUNCTION__, mtl_id, (tex_path.empty() ? std::string_view{"(null)"} : tex_path), static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::LoadAnimData(lwIAnimDataInfo* data_info, std::string_view tex_path, const lwResFile* res) {
		LW_RESULT ret = LW_RET_FAILED;

		lwAnimDataInfo* info = (lwAnimDataInfo*)data_info;

		void* data;
		LW_SAFE_RELEASE(_anim_agent);

		if (LW_RESULT r = _res_mgr->CreateAnimCtrlAgent(&_anim_agent); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _res_mgr->CreateAnimCtrlAgent failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		lwAnimCtrlObjTypeInfo type_info;

		if (info->anim_mat) {
			lwIAnimCtrlMatrix* ctrl = NULL;
			lwIAnimCtrlObjMat* ctrl_obj = NULL;

			data = info->anim_mat;

			type_info.type = ANIM_CTRL_TYPE_MAT;
			type_info.data[0] = LW_INVALID_INDEX;
			type_info.data[1] = LW_INVALID_INDEX;

			if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrlObj(MAT) failed: type={}, ret={}",
							 __FUNCTION__, type_info.type, static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrl(MAT) failed: type={}, ret={}",
							 __FUNCTION__, type_info.type, static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] ctrl->LoadData(MAT) failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			ctrl_obj->AttachAnimCtrl(ctrl);
			ctrl_obj->SetTypeInfo(&type_info);

			_anim_agent->AddAnimCtrlObj(ctrl_obj);
		}
		if (info->anim_bone) {
			lwIAnimCtrlBone* ctrl = NULL;
			lwIAnimCtrlObjBone* ctrl_obj = NULL;

			data = info->anim_bone;

			type_info.type = ANIM_CTRL_TYPE_BONE;
			type_info.data[0] = LW_INVALID_INDEX;
			type_info.data[1] = LW_INVALID_INDEX;

			if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrlObj(BONE) failed: type={}, ret={}",
							 __FUNCTION__, type_info.type, static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrl(BONE) failed: type={}, ret={}",
							 __FUNCTION__, type_info.type, static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] ctrl->LoadData(BONE) failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
				goto __ret;
			}

			ctrl_obj->AttachAnimCtrl(ctrl);
			ctrl_obj->SetTypeInfo(&type_info);

			_anim_agent->AddAnimCtrlObj(ctrl_obj);
		}


		for (DWORD i = 0; i < _mtltex_agent_seqsize; i++) {
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
								 "[{}] CreateAnimCtrlObj(MTLOPACITY) failed: mtl_id={}, ret={}",
								 __FUNCTION__, i, static_cast<long long>(r));
					goto __ret;
				}

				if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] CreateAnimCtrl(MTLOPACITY) failed: mtl_id={}, ret={}",
								 __FUNCTION__, i, static_cast<long long>(r));
					goto __ret;
				}

				if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] ctrl->LoadData(MTLOPACITY) failed: mtl_id={}, ret={}",
								 __FUNCTION__, i, static_cast<long long>(r));
					goto __ret;
				}

				ctrl_obj->AttachAnimCtrl(ctrl);
				ctrl_obj->SetTypeInfo(&type_info);
				_anim_agent->AddAnimCtrlObj(ctrl_obj);
			}

			for (DWORD j = 0; j < LW_MAX_TEXTURESTAGE_NUM; j++) {
				if (info->anim_tex[i][j]) {
					lwIAnimCtrlTexUV* ctrl = NULL;
					lwIAnimCtrlObjTexUV* ctrl_obj = NULL;

					data = info->anim_tex[i][j];

					type_info.type = ANIM_CTRL_TYPE_TEXUV;
					type_info.data[0] = i;
					type_info.data[1] = j;

					if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type);
						LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] CreateAnimCtrlObj(TEXUV) failed: mtl_id={}, stage={}, ret={}",
									 __FUNCTION__, i, j, static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] CreateAnimCtrl(TEXUV) failed: mtl_id={}, stage={}, ret={}",
									 __FUNCTION__, i, j, static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] ctrl->LoadData(TEXUV) failed: mtl_id={}, stage={}, ret={}",
									 __FUNCTION__, i, j, static_cast<long long>(r));
						goto __ret;
					}

					ctrl_obj->AttachAnimCtrl(ctrl);
					ctrl_obj->SetTypeInfo(&type_info);

					_anim_agent->AddAnimCtrlObj(ctrl_obj);
				}

				// image
				if (info->anim_img[i][j]) {
					lwIAnimCtrlTexImg* ctrl = NULL;
					lwIAnimCtrlObjTexImg* ctrl_obj = NULL;

					data = info->anim_img[i][j];

					type_info.type = ANIM_CTRL_TYPE_TEXIMG;
					type_info.data[0] = i;
					type_info.data[1] = j;

					{const auto _t = std::string{tex_path}; _tcscpy(info->anim_img[i][j]->_tex_path, _t.c_str());}

					if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj, type_info.type);
						LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] CreateAnimCtrlObj(TEXIMG) failed: mtl_id={}, stage={}, ret={}",
									 __FUNCTION__, i, j, static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, type_info.type); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] CreateAnimCtrl(TEXIMG) failed: mtl_id={}, stage={}, ret={}",
									 __FUNCTION__, i, j, static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = ctrl->LoadData(data); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] ctrl->LoadData(TEXIMG) failed: mtl_id={}, stage={}, ret={}",
									 __FUNCTION__, i, j, static_cast<long long>(r));
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

	LW_RESULT lwNodePrimitive::LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci) {
		LW_RESULT ret = LW_RET_FAILED;

		lwIRenderCtrlAgent* render_agent = 0;

		if (_render_agent)
			goto __ret;

		if (LW_RESULT r = _res_mgr->CreateRenderCtrlAgent(&render_agent); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _res_mgr->CreateRenderCtrlAgent failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = render_agent->SetRenderCtrl(rcci->ctrl_id); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] render_agent->SetRenderCtrl failed: ctrl_id={}, ret={}",
						 __FUNCTION__, rcci->ctrl_id, static_cast<long long>(r));
			goto __ret;
		}

		render_agent->SetVertexDeclaration(rcci->decl_id);
		render_agent->SetVertexShader(rcci->vs_id);

		_render_agent = render_agent;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::Copy(lwINodePrimitive* src_obj) {
		LW_RESULT ret = LW_RET_FAILED;

		_id = src_obj->GetID();
		_mat_local = *src_obj->GetLocalMatrix();
		_mat_world = *src_obj->GetWorldMatrix();

		_parent = src_obj->GetParent();
		_link_id = src_obj->GetLinkID();
		_link_parent_id = src_obj->GetParentLinkID();

		lwIMeshAgent* src_mesh_agent = src_obj->GetMeshAgent();
		if (src_mesh_agent) {
			if (LW_RESULT r = src_mesh_agent->Clone(&_mesh_agent); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] src_mesh_agent->Clone failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}
		}
		{
			lwIMtlTexAgent* src_mtltex_agent;
			DWORD src_mtltex_agent_seqsize = GetMtlTexAgentSeqSize();

			AllocateMtlTexAgentSeq(src_mtltex_agent_seqsize);

			for (DWORD i = 0; i < src_mtltex_agent_seqsize; i++) {
				if ((src_mtltex_agent = src_obj->GetMtlTexAgent(i)) == NULL)
					continue;

				if (LW_RESULT r = src_mtltex_agent->Clone(&_mtltex_agent_seq[i]); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] src_mtltex_agent->Clone failed: mtl_id={}, ret={}",
								 __FUNCTION__, i, static_cast<long long>(r));
					goto __ret;
				}
			}

			lwIAnimCtrlAgent* src_anim_agent = src_obj->GetAnimCtrlAgent();
			if (src_anim_agent) {
				if (LW_RESULT r = src_anim_agent->Clone(&_anim_agent); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] src_anim_agent->Clone failed: id={}, ret={}",
								 __FUNCTION__, _id, static_cast<long long>(r));
					goto __ret;
				}
			}

			lwIRenderCtrlAgent* src_render_agent = src_obj->GetRenderCtrlAgent();
			if (src_render_agent) {
				if (LW_RESULT r = _render_agent->Clone(&_render_agent); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _render_agent->Clone failed: id={}, ret={}",
								 __FUNCTION__, _id, static_cast<long long>(r));
					goto __ret;
				}
			}

			lwIHelperObject* src_helper_object = src_obj->GetHelperObject();
			if (src_helper_object) {
				if (LW_RESULT r = src_helper_object->Clone(&_helper_object); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] src_helper_object->Clone failed: id={}, ret={}",
								 __FUNCTION__, _id, static_cast<long long>(r));
					goto __ret;
				}
			}

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::Update() {
		LW_RESULT ret = LW_RET_FAILED;

		if (_anim_agent) {
			if (LW_RESULT r = _anim_agent->Update(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _anim_agent->Update failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
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
									 "[{}] AnimCtrlObjMat::UpdateObject failed: idx={}, ret={}",
									 __FUNCTION__, i, static_cast<long long>(r));
						goto __ret;
					}

					if (LW_RESULT r = ((lwIAnimCtrlObjMat*)ctrl_obj)->GetRTM(&_mat_local); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjMat::GetRTM failed: idx={}, ret={}",
									 __FUNCTION__, i, static_cast<long long>(r));
						goto __ret;
					}
				}
				break;
				case ANIM_CTRL_TYPE_BONE: {
					if (LW_RESULT r = ((lwIAnimCtrlObjBone*)ctrl_obj)->UpdateObject(
						(lwIAnimCtrlObjBone*)ctrl_obj, _mesh_agent->GetMesh()); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjBone::UpdateObject failed: idx={}, ret={}",
									 __FUNCTION__, i, static_cast<long long>(r));
						goto __ret;
					}
				}
				break;
				case ANIM_CTRL_TYPE_MTLOPACITY: {
					if (LW_RESULT r = ((lwIAnimCtrlObjMtlOpacity*)ctrl_obj)->UpdateObject(); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjMtlOpacity::UpdateObject failed: idx={}, ret={}",
									 __FUNCTION__, i, static_cast<long long>(r));
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
									 "[{}] AnimCtrlObjTexUV::UpdateObject failed: idx={}, ret={}",
									 __FUNCTION__, i, static_cast<long long>(r));
						goto __ret;
					}
				}
				break;
				case ANIM_CTRL_TYPE_TEXIMG: {
					if (LW_RESULT r = ((lwIAnimCtrlObjTexImg*)ctrl_obj)->UpdateObject(); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlObjTexImg::UpdateObject failed: idx={}, ret={}",
									 __FUNCTION__, i, static_cast<long long>(r));
						goto __ret;
					}

					lwITex* tex;
					lwIMtlTexAgent* mtltex_agent;

					if (LW_RESULT r = ((lwIAnimCtrlTexImg*)ctrl_obj)->GetRunTimeTex(&tex); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] AnimCtrlTexImg::GetRunTimeTex failed: idx={}, ret={}",
									 __FUNCTION__, i, static_cast<long long>(r));
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

		if (_parent) {
			if (_ref_ctrl_obj_bone) {
				// update world matrix
				lwMatrix44 mat_parent;

				if (LW_RESULT r = _parent->GetLinkMatrix(&mat_parent, LW_INVALID_INDEX); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _parent->GetLinkMatrix(BONE_INVALID) failed: id={}, ret={}",
								 __FUNCTION__, _id, static_cast<long long>(r));
					goto __ret;
				}

				lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);

				// update render ctrl blend matrix
				lwIAnimCtrlObjBone* ctrl_obj = ((lwINodeBoneCtrl*)_parent)->GetAnimCtrlObj();
				if (ctrl_obj == 0)
					goto __ret;

				if (LW_RESULT r = ctrl_obj->UpdateObject(_ref_ctrl_obj_bone, _mesh_agent->GetMesh()); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] ctrl_obj->UpdateObject(ref_bone) failed: id={}, ret={}",
								 __FUNCTION__, _id, static_cast<long long>(r));
					goto __ret;
				}

				if (LW_RESULT r = ctrl_obj->UpdateHelperObject(_helper_object); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] ctrl_obj->UpdateHelperObject failed: id={}, ret={}",
								 __FUNCTION__, _id, static_cast<long long>(r));
					goto __ret;
				}
			}
			else {
				lwMatrix44 mat_parent;

				if (LW_RESULT r = _parent->GetLinkMatrix(&mat_parent, _link_parent_id); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _parent->GetLinkMatrix(parent_link) failed: id={}, link_parent_id={}, ret={}",
								 __FUNCTION__, _id, _link_parent_id, static_cast<long long>(r));
					goto __ret;
				}

				if (_link_id != LW_INVALID_INDEX) {
					lwMatrix44 mat_link;

					if (LW_RESULT r = GetLinkMatrix(&mat_link, _link_id); LW_FAILED(r)) {
						ToLogService("errors", LogLevel::Error,
									 "[{}] GetLinkMatrix(self) failed: id={}, link_id={}, ret={}",
									 __FUNCTION__, _id, _link_id, static_cast<long long>(r));
						goto __ret;
					}

					lwMatrix44InverseNoScaleFactor(&mat_link, &mat_link);

					lwMatrix44Multiply(&mat_parent, &mat_link, &mat_parent);
				}

				lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);
			}
		}
		else {
			_mat_world = _mat_local;
		}

		if (_helper_object) {
			_helper_object->SetParentMatrix(&_mat_world);
		}

		_UpdateTransparentState();

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::Render() {
		LW_RESULT ret = LW_RET_FAILED;

		if (!_CheckVisibleState())
			goto __addr_ret_ok;

		_render_agent->SetMatrix(&_mat_world);
		_render_agent->BindAnimCtrlAgent(_anim_agent);
		_render_agent->BindMeshAgent(_mesh_agent);

		if (LW_RESULT r = _render_agent->BeginSet(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->BeginSet failed: id={}, ret={}",
						 __FUNCTION__, _id, static_cast<long long>(r));
			goto __ret;
		}

		for (DWORD i = 0; i < _mtltex_agent_seqsize; i++) {
			if (_mtltex_agent_seq[i] == NULL)
				continue;

			_render_agent->BindMtlTexAgent(_mtltex_agent_seq[i]);

			if (_mtltex_agent_seq[i]->BeginPass() == RES_PASS_DEFAULT) {
				if (LW_RESULT r = _render_agent->BeginSetSubset(i); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _render_agent->BeginSetSubset failed: subset={}, ret={}",
								 __FUNCTION__, i, static_cast<long long>(r));
					goto __ret;
				}

				if (LW_RESULT r = _render_agent->DrawSubset(i); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _render_agent->DrawSubset failed: subset={}, ret={}",
								 __FUNCTION__, i, static_cast<long long>(r));
					goto __ret;
				}

				if (LW_RESULT r = _render_agent->EndSetSubset(i); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _render_agent->EndSetSubset failed: subset={}, ret={}",
								 __FUNCTION__, i, static_cast<long long>(r));
					goto __ret;
				}
			}

			_mtltex_agent_seq[i]->EndPass();
		}

		if (LW_RESULT r = _render_agent->EndSet(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->EndSet failed: id={}, ret={}",
						 __FUNCTION__, _id, static_cast<long long>(r));
			goto __ret;
		}

		if (_helper_object) {
			if (LW_RESULT r = _helper_object->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _helper_object->Render failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
			}
		}

	__addr_ret_ok:
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::RenderSubset(DWORD subset) {
		LW_RESULT ret = LW_RET_FAILED;

		if (subset < 0 || subset >= LW_MAX_SUBSET_NUM)
			goto __ret;

		if (_mtltex_agent_seq[subset] == NULL)
			goto __ret;

		_render_agent->BindAnimCtrlAgent(_anim_agent);
		_render_agent->BindMeshAgent(_mesh_agent);
		_render_agent->BindMtlTexAgent(_mtltex_agent_seq[subset]);

		if (LW_RESULT r = _render_agent->BeginSet(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->BeginSet failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _render_agent->BeginSetSubset(subset); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->BeginSetSubset failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _render_agent->DrawSubset(subset); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->DrawSubset failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _render_agent->EndSetSubset(subset); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->EndSetSubset failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _render_agent->EndSet(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _render_agent->EndSet failed: subset={}, ret={}",
						 __FUNCTION__, subset, static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::RenderHelperObject() {
		LW_RESULT ret = LW_RET_FAILED;

		if (_helper_object == 0)
			goto __ret;

		if (LW_RESULT r = _helper_object->Render(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _helper_object->Render failed: id={}, ret={}",
						 __FUNCTION__, _id, static_cast<long long>(r));
			goto __ret;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray) {
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

	LW_RESULT lwNodePrimitive::GetLinkMatrix(lwMatrix44* mat, DWORD link_id) {
		LW_RESULT ret = LW_RET_FAILED;

		if (link_id == LW_INVALID_INDEX) {
			*mat = _mat_world;
		}
		else {
			if (_helper_object == 0)
				goto __ret;

			lwIHelperDummy* hd = _helper_object->GetHelperDummy();
			if (hd == 0)
				goto __ret;

			lwHelperDummyInfo* info = hd->GetDataInfoWithID(link_id);

			if (info == 0)
				goto __ret;

			*mat = info->mat;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	void lwNodePrimitive::SetMaterial(const lwMaterial* mtl) {
		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (_mtltex_agent_seq[i] == NULL)
				continue;

			_mtltex_agent_seq[i]->SetMaterial(mtl);
		}
	}

	void lwNodePrimitive::SetOpacity(float opacity) {
		for (DWORD i = 0; i < LW_MAX_SUBSET_NUM; i++) {
			if (_mtltex_agent_seq[i] == NULL)
				continue;

			_mtltex_agent_seq[i]->SetOpacity(opacity);
		}
	}

	// SetParentparentVertexBlendCtrl
	// 1.lwAnimCtrlAgentANIM_CTRL_TYPE_BONE
	// 2._ref_ctrl_obj_bone
	// 3.lwRenderCtrlAgentvs_typevs_ctrlvs_type
	//   fvfvs_typevs_ctrl
	//   vs_type,vs_ctrl
	LW_RESULT lwNodePrimitive::SetParent(lwINode* parent) {
		LW_RESULT ret = LW_RET_FAILED;

		lwAnimCtrlObjTypeInfo type_info;
		type_info.type = ANIM_CTRL_TYPE_MAT;
		type_info.data[0] = LW_INVALID_INDEX;
		type_info.data[1] = LW_INVALID_INDEX;

		//check parent model type
		if (parent == 0) {
			if (_parent == 0)
				goto __addr_ret_ok;

			if (_parent->GetType() == NODE_BONECTRL) {
				if (_anim_agent) {
					lwIAnimCtrlObj* ctrl_obj = _anim_agent->RemoveAnimCtrlObj(&type_info);

					LW_IF_RELEASE(ctrl_obj);
					_ref_ctrl_obj_bone = 0;
				}
			}

			_parent = 0;
			goto __addr_ret_ok;
		}

		// clear current parent
		if (LW_RESULT r = SetParent(0); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] SetParent(0) failed: id={}, ret={}",
						 __FUNCTION__, _id, static_cast<long long>(r));
			goto __ret;
		}

		if (parent->GetType() == NODE_BONECTRL) {
			lwIAnimCtrlObj* ctrl_obj = 0;

			// create anim ctrl agent
			if (_anim_agent == 0) {
				if (LW_RESULT r = _res_mgr->CreateAnimCtrlAgent(&_anim_agent); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _res_mgr->CreateAnimCtrlAgent failed: id={}, ret={}",
								 __FUNCTION__, _id, static_cast<long long>(r));
					goto __ret;
				}
			}
			else {
				ctrl_obj = _anim_agent->GetAnimCtrlObj(&type_info);
			}

			// create ctrl bone
			if (ctrl_obj == 0) {
				if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj(&ctrl_obj, ANIM_CTRL_TYPE_BONE); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] _res_mgr->CreateAnimCtrlObj(BONE) failed: id={}, ret={}",
								 __FUNCTION__, _id, static_cast<long long>(r));
					goto __ret;
				}

				_anim_agent->AddAnimCtrlObj(ctrl_obj);

				_ref_ctrl_obj_bone = (lwIAnimCtrlObjBone*)ctrl_obj;
			}
		}

		_parent = parent;


	__addr_ret_ok:
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive::GetSubsetNum(DWORD* subset_num) {
		LW_RESULT ret = LW_RET_FAILED;

		if (subset_num == 0)
			goto __ret;

		// use mesh subset?
		if (_mesh_agent == 0)
			goto __ret;

		{
			lwIMesh* mesh = _mesh_agent->GetMesh();
			if (mesh == 0)
				goto __ret;

			lwMeshInfo* info = mesh->GetMeshInfo();
			if (info == 0)
				goto __ret;

			*subset_num = info->subset_num;

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	// lwNodeBoneCtrl
	LW_STD_IMPLEMENTATION(lwNodeBoneCtrl)

	lwNodeBoneCtrl::lwNodeBoneCtrl(lwIResourceMgr* res_mgr)
		: _res_mgr(res_mgr), _ctrl_obj(0) {
		_type = NODE_BONECTRL;
	}

	lwNodeBoneCtrl::~lwNodeBoneCtrl() {
		LW_IF_RELEASE(_ctrl_obj);
	}

	LW_RESULT lwNodeBoneCtrl::GetLinkMatrix(lwMatrix44* mat, DWORD link_id) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_ctrl_obj == 0)
			goto __ret;

		if (link_id == LW_INVALID_INDEX) {
			*mat = _mat_world;
			goto __addr_ret_ok;
		}

		{
			lwMatrix44* rtm = _ctrl_obj->GetDummyRTM(link_id);

			if (rtm == NULL)
				goto __ret;

			lwMatrix44Multiply(mat, rtm, &_mat_world);
		}
	__addr_ret_ok:
		ret = LW_RET_OK;

	__ret:
		return ret;
	}

	LW_RESULT lwNodeBoneCtrl::Update() {
		LW_RESULT ret = LW_RET_FAILED;

		// update world matrix
		if (_parent) {
			lwMatrix44 mat_parent;

			if (LW_RESULT r = _parent->GetLinkMatrix(&mat_parent, _link_parent_id); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _parent->GetLinkMatrix failed: id={}, link_parent_id={}, ret={}",
							 __FUNCTION__, _id, _link_parent_id, static_cast<long long>(r));
				goto __ret;
			}

			if (_link_id != LW_INVALID_INDEX) {
				lwMatrix44 mat_link;

				if (LW_RESULT r = GetLinkMatrix(&mat_link, _link_id); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] GetLinkMatrix(self) failed: id={}, link_id={}, ret={}",
								 __FUNCTION__, _id, _link_id, static_cast<long long>(r));
					goto __ret;
				}

				lwMatrix44InverseNoScaleFactor(&mat_link, &mat_link);

				lwMatrix44Multiply(&mat_parent, &mat_link, &mat_parent);
			}

			lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);
		}
		else {
			_mat_world = _mat_local;
		}

		if (_ctrl_obj == 0)
			goto __addr_ret_ok;

		if (LW_RESULT r = _ctrl_obj->UpdateAnimCtrl(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _ctrl_obj->UpdateAnimCtrl failed: id={}, ret={}",
						 __FUNCTION__, _id, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _ctrl_obj->UpdateObject(_ctrl_obj, NULL); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _ctrl_obj->UpdateObject failed: id={}, ret={}",
						 __FUNCTION__, _id, static_cast<long long>(r));
			goto __ret;
		}

	__addr_ret_ok:
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeBoneCtrl::Load(lwIAnimDataBone* data) {
		LW_RESULT ret = LW_RET_FAILED;

		lwIAnimCtrlBone* ctrl_bone = 0;
		lwIAnimCtrlObjBone* ctrl_obj_bone = 0;

		if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl_bone, ANIM_CTRL_TYPE_BONE); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] CreateAnimCtrl(BONE) failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&ctrl_obj_bone, ANIM_CTRL_TYPE_BONE);
			LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] CreateAnimCtrlObj(BONE) failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = ctrl_bone->LoadData((void*)data); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] ctrl_bone->LoadData failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		ctrl_obj_bone->AttachAnimCtrl(ctrl_bone);

		_ctrl_obj = ctrl_obj_bone;

		ctrl_obj_bone = 0;
		ctrl_bone = 0;

		ret = LW_RET_OK;
	__ret:
		LW_IF_RELEASE(ctrl_bone);
		LW_IF_RELEASE(ctrl_obj_bone);

		return ret;
	}

	LW_RESULT lwNodeBoneCtrl::PlayPose(const lwPlayPoseInfo* info) {
		return _ctrl_obj->PlayPose(info);
	}

	lwIPoseCtrl* lwNodeBoneCtrl::GetPoseCtrl() {
		return lwIAnimCtrlObj_GetPoseCtrl(_ctrl_obj);
	}

	lwPlayPoseInfo* lwNodeBoneCtrl::GetPlayPoseInfo() {
		return lwIAnimCtrlObj_GetPlayPoseInfo(_ctrl_obj);
	}


	//lwNodeDummy
	LW_STD_IMPLEMENTATION(lwNodeDummy)

	lwNodeDummy::lwNodeDummy(lwIResourceMgr* res_mgr)
		: _res_mgr(res_mgr), _ctrl_obj(0), _ass_obj(0) {
		_type = NODE_DUMMY;
	}

	lwNodeDummy::~lwNodeDummy() {
		LW_IF_RELEASE(_ass_obj);
		LW_IF_RELEASE(_ctrl_obj);
	}

	LW_RESULT lwNodeDummy::GetLinkMatrix(lwMatrix44* mat, DWORD link_id) {
		LW_RESULT ret = LW_RET_FAILED;

		if (link_id != LW_INVALID_INDEX)
			goto __ret;

		*mat = _mat_world;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeDummy::Load(lwIHelperDummyObjInfo* data) {
		LW_RESULT ret = LW_RET_FAILED;

		lwHelperDummyObjInfo* info = (lwHelperDummyObjInfo*)data;

		_id = info->GetID();
		_mat_local = *info->GetMatrix();

		LW_SAFE_RELEASE(_ctrl_obj);
		lwIAnimDataMatrix* anim_data = info->GetAnimDataMatrix();
		if (anim_data) {
			lwIAnimCtrlMatrix* ctrl;

			if (LW_RESULT r = _res_mgr->CreateAnimCtrl((lwIAnimCtrl**)&ctrl, ANIM_CTRL_TYPE_MAT); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrl(MAT) failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = _res_mgr->CreateAnimCtrlObj((lwIAnimCtrlObj**)&_ctrl_obj, ANIM_CTRL_TYPE_MAT);
				LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAnimCtrlObj(MAT) failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = ctrl->LoadData(anim_data); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] ctrl->LoadData failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}

			_ctrl_obj->AttachAnimCtrl(ctrl);
		}

		if (_res_mgr->GetByteSet()->GetValue(OPT_CREATE_ASSISTANTOBJECT)) {
			lwAssObjInfo info;
			_res_mgr->GetAssObjInfo(&info);

			if (LW_RESULT r = CreateAssistantObject(&info.size, info.color); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] CreateAssistantObject failed: id={}, color={}, ret={}",
							 __FUNCTION__, _id, info.color, static_cast<long long>(r));
				goto __ret;
			}

			_ass_obj->GetStateSet()->SetValue(STATE_VISIBLE, 0);
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeDummy::Update() {
		LW_RESULT ret = LW_RET_FAILED;

		// first update anim data
		if (_ctrl_obj) {
			if (LW_RESULT r = _ctrl_obj->UpdateAnimCtrl(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _ctrl_obj->UpdateAnimCtrl failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = _ctrl_obj->UpdateObject(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _ctrl_obj->UpdateObject failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}

			if (LW_RESULT r = _ctrl_obj->GetRTM(&_mat_local); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _ctrl_obj->GetRTM failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}
		}

		// update world matrix
		if (_parent) {
			lwMatrix44 mat_parent;

			if (LW_RESULT r = _parent->GetLinkMatrix(&mat_parent, _link_parent_id); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _parent->GetLinkMatrix failed: id={}, link_parent_id={}, ret={}",
							 __FUNCTION__, _id, _link_parent_id, static_cast<long long>(r));
				goto __ret;
			}

			if (_link_id != LW_INVALID_INDEX) {
				lwMatrix44 mat_link;

				if (LW_RESULT r = GetLinkMatrix(&mat_link, _link_id); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] GetLinkMatrix(self) failed: id={}, link_id={}, ret={}",
								 __FUNCTION__, _id, _link_id, static_cast<long long>(r));
					goto __ret;
				}

				lwMatrix44InverseNoScaleFactor(&mat_link, &mat_link);

				lwMatrix44Multiply(&mat_parent, &mat_link, &mat_parent);
			}

			lwMatrix44Multiply(&_mat_world, &_mat_local, &mat_parent);
		}
		else {
			_mat_world = _mat_local;
		}

		if (_ass_obj) {
			if (LW_RESULT r = _ass_obj->Update(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _ass_obj->Update failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
			}
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeDummy::Render() {
		if (_ass_obj) {
			if (LW_RESULT r = _ass_obj->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _ass_obj->Render failed: ret={}",
							 __FUNCTION__, static_cast<long long>(r));
			}
		}

		return LW_RET_OK;
	}

	LW_RESULT lwNodeDummy::CreateAssistantObject(const lwVector3* size, DWORD color) {
		LW_RESULT ret = LW_RET_FAILED;

		lwVector3 s = *size / 2;

		lwVector3 axis_x(s.x, 0.0f, 0.0f);
		lwVector3 axis_y(0.0f, s.y, 0.0f);
		lwVector3 axis_z(0.0f, 0.0f, s.z);


		if (LW_RESULT r = _res_mgr->CreateNode((lwINode**)&_ass_obj, NODE_PRIMITIVE); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _res_mgr->CreateNode(NODE_PRIMITIVE) failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		if (LW_RESULT r = _ass_obj->SetParent(this); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _ass_obj->SetParent failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}
		{
			const int v_num = 24 + 6;
			lwVector3 vert_buf[v_num] =
			{
				// axis
				lwVector3(0.0f, 0.0f, 0.0f),
				lwVector3(axis_x),
				lwVector3(0.0f, 0.0f, 0.0f),
				lwVector3(axis_y),
				lwVector3(0.0f, 0.0f, 0.0f),
				lwVector3(axis_z),

				// bottom
				lwVector3(-s.x, -s.y, -s.z),
				lwVector3(-s.x, -s.y, s.z),

				lwVector3(-s.x, -s.y, s.z),
				lwVector3(s.x, -s.y, s.z),

				lwVector3(s.x, -s.y, s.z),
				lwVector3(s.x, -s.y, -s.z),

				lwVector3(s.x, -s.y, -s.z),
				lwVector3(-s.x, -s.y, -s.z),

				// top
				lwVector3(-s.x, s.y, -s.z),
				lwVector3(-s.x, s.y, s.z),

				lwVector3(-s.x, s.y, s.z),
				lwVector3(s.x, s.y, s.z),

				lwVector3(s.x, s.y, s.z),
				lwVector3(s.x, s.y, -s.z),

				lwVector3(s.x, s.y, -s.z),
				lwVector3(-s.x, s.y, -s.z),

				// side
				lwVector3(-s.x, -s.y, -s.z),
				lwVector3(-s.x, s.y, -s.z),

				lwVector3(-s.x, -s.y, s.z),
				lwVector3(-s.x, s.y, s.z),

				lwVector3(s.x, -s.y, s.z),
				lwVector3(s.x, s.y, s.z),

				lwVector3(s.x, -s.y, -s.z),
				lwVector3(s.x, s.y, -s.z),
			};


			DWORD cor_buf[v_num];
			for (DWORD i = 0; i < v_num; i++) {
				if (i < 6) {
					const DWORD c[3] =
					{
						0xffff0000,
						0xff00ff00,
						0xff0000ff,
					};

					cor_buf[i] = c[i / 2];
				}
				else {
					cor_buf[i] = color;
				}
			}

			lwSubsetInfo subset;
			lwSubsetInfo_Construct(&subset, v_num / 2, 0, v_num, 0);

			if (LW_RESULT r = lwLoadPrimitiveLineList(_ass_obj, "assistant_object", v_num, vert_buf, cor_buf, &subset,
													  1); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwLoadPrimitiveLineList(assistant_object) failed: v_num={}, ret={}",
							 __FUNCTION__, v_num, static_cast<long long>(r));
				_ass_obj->Release();
				goto __ret;
			}

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	// lwNodeHelper
	LW_STD_IMPLEMENTATION(lwNodeHelper)

	lwNodeHelper::lwNodeHelper(lwIResourceMgr* res_mgr)
		: _res_mgr(res_mgr) {
		_type = NODE_HELPER;

		_obj_dummy = 0;
		_obj_box = 0;
		_obj_mesh = 0;
	}

	lwNodeHelper::~lwNodeHelper() {
		Destroy();
	}

	LW_RESULT lwNodeHelper::Destroy() {
		LW_SAFE_RELEASE(_obj_dummy);
		LW_SAFE_RELEASE(_obj_box);
		LW_SAFE_RELEASE(_obj_mesh);

		return LW_RET_OK;
	}

	LW_RESULT lwNodeHelper::LoadHelperInfo(const lwHelperInfo* info, int create_instance_flag) {
		LW_RESULT ret = LW_RET_FAILED;

		if (info->type & HELPER_TYPE_DUMMY) {
			lwHelperDummy* d = LW_NEW(lwHelperDummy);

			d->SetResourceMgr(_res_mgr);
			d->SetDataInfo(&info->dummy_seq[0], info->dummy_num);

			if (create_instance_flag) {
				if (LW_RESULT r = d->CreateInstance(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] HelperDummy->CreateInstance failed: dummy_num={}, ret={}",
								 __FUNCTION__, info->dummy_num, static_cast<long long>(r));
					goto __ret;
				}
			}

			LW_IF_RELEASE(_obj_dummy);
			_obj_dummy = d;
		}

		if (info->type & HELPER_TYPE_BOX) {
			lwHelperBox* b = LW_NEW(lwHelperBox);

			b->SetResourceMgr(_res_mgr);
			b->SetDataInfo(&info->box_seq[0], info->box_num);

			if (create_instance_flag) {
				if (LW_RESULT r = b->CreateInstance(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] HelperBox->CreateInstance failed: box_num={}, ret={}",
								 __FUNCTION__, info->box_num, static_cast<long long>(r));
					goto __ret;
				}
			}

			LW_IF_RELEASE(_obj_box);
			_obj_box = b;
		}

		if (info->type & HELPER_TYPE_MESH) {
			lwHelperMesh* m = LW_NEW(lwHelperMesh);

			m->SetResourceMgr(_res_mgr);
			m->SetDataInfo(&info->mesh_seq[0], info->mesh_num);

			if (create_instance_flag) {
				if (LW_RESULT r = m->CreateInstance(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] HelperMesh->CreateInstance failed: mesh_num={}, ret={}",
								 __FUNCTION__, info->mesh_num, static_cast<long long>(r));
					goto __ret;
				}
			}

			LW_IF_RELEASE(_obj_mesh);
			_obj_mesh = m;
		}

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeHelper::Copy(const lwINodeHelper* src) {
		lwNodeHelper* s = (lwNodeHelper*)src;

		if (s->_obj_dummy) {
			lwHelperDummy* d = LW_NEW(lwHelperDummy);
			d->Clone((lwHelperDummy*)s->_obj_dummy);
			_obj_dummy = d;
		}
		if (s->_obj_box) {
			lwHelperBox* b = LW_NEW(lwHelperBox);
			b->Clone((lwHelperBox*)s->_obj_box);
			_obj_box = b;
		}
		if (s->_obj_mesh) {
			lwHelperMesh* m = LW_NEW(lwHelperMesh);
			m->Clone((lwHelperMesh*)s->_obj_mesh);
			_obj_mesh = m;
		}

		return LW_RET_OK;
	}

	LW_RESULT lwNodeHelper::Render() {
		LW_RESULT ret = LW_RET_FAILED;

		if (!_CheckVisibleState())
			goto __ret_ok;

		if (_obj_dummy) {
			if (LW_RESULT r = _obj_dummy->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj_dummy->Render failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}
		}
		if (_obj_box) {
			if (LW_RESULT r = _obj_box->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj_box->Render failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}
		}
		if (_obj_mesh) {
			if (LW_RESULT r = _obj_mesh->Render(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] _obj_mesh->Render failed: id={}, ret={}",
							 __FUNCTION__, _id, static_cast<long long>(r));
				goto __ret;
			}
		}

	__ret_ok:
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeHelper::GetLinkMatrix(lwMatrix44* mat, DWORD link_id) {
		__debugbreak();
		return 0;
	}

	LW_RESULT lwNodeHelper::Update() {
		return 0;
	}

	// lwNodeObject
	static DWORD __tree_enum_update(lwITreeNode* node, void* param) {
		DWORD ret = TREENODE_PROC_RET_ABORT;

		lwINode* obj = (lwINode*)node->GetData();
		if (obj == 0) {
			assert(0 && "call __tree_enum_update error");
			goto __ret;
		}

		if (LW_RESULT r = obj->Update(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] obj->Update failed: type={}, id={}, ret={}",
						 __FUNCTION__, obj->GetType(), obj->GetID(), static_cast<long long>(r));
			goto __ret;
		}

		ret = TREENODE_PROC_RET_CONTINUE;
	__ret:
		return ret;
	}

	static DWORD __tree_enum_render(lwITreeNode* node, void* param) {
		DWORD ret = TREENODE_PROC_RET_ABORT;

		lwINode* obj = (lwINode*)node->GetData();
		if (obj == 0) {
			assert(0 && "call __tree_enum_render error");
			goto __ret;
		}

		if (LW_RESULT r = ((lwINodePrimitive*)obj)->Render(); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] NodePrimitive::Render failed: id={}, ret={}",
						 __FUNCTION__, obj->GetID(), static_cast<long long>(r));
			goto __ret;
		}

		ret = TREENODE_PROC_RET_CONTINUE;
	__ret:
		return ret;
	}

	static DWORD __tree_proc_cullprimitive(lwITreeNode* node, void* param) {
		DWORD ret = TREENODE_PROC_RET_ABORT;

		lwISceneMgr* scn_mgr = (lwISceneMgr*)param;
		lwINode* obj = (lwINode*)node->GetData();

		if (obj == 0) {
			assert(0 && "call __tree_proc_cullprimitive error");
			goto __ret;
		}

		if (obj->GetType() == NODE_PRIMITIVE) {
			__debugbreak();
		}

		ret = TREENODE_PROC_RET_CONTINUE;
	__ret:
		return ret;
	}


	static DWORD __tree_enum_destroy(lwITreeNode* node, void* param) {
		DWORD ret = TREENODE_PROC_RET_ABORT;

		lwINode* obj = (lwINode*)node->GetData();
		if (obj == 0) {
			assert(0 && "call __tree_enum_destroy error");
			goto __ret;
		}

		if (obj->GetType() == NODE_PRIMITIVE) {
			if (LW_RESULT r = obj->Release(); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] obj->Release failed: id={}, ret={}",
							 __FUNCTION__, obj->GetID(), static_cast<long long>(r));
				goto __ret;
			}
		}

		ret = TREENODE_PROC_RET_CONTINUE;
	__ret:
		return ret;
	}

	static DWORD __tree_enum_query(lwITreeNode* node, void* param) {
		DWORD ret = TREENODE_PROC_RET_CONTINUE;

		lwINode* obj = (lwINode*)node->GetData();
		if (obj == 0) {
			assert(0 && "call __tree_enum_query error");
			goto __ret;
		}
		{
			lwModelNodeQueryInfo* info = (lwModelNodeQueryInfo*)param;

			if (info->mask & MNQI_ID) {
				if (obj->GetID() != info->id)
					goto __ret;
			}
			if (info->mask & MNQI_TYPE) {
				if (obj->GetType() != info->type)
					goto __ret;
			}
			if (info->mask & MNQI_DATA) {
				if (obj != info->data)
					goto __ret;
			}
			if (info->mask & MNQI_DESCRIPTOR) {
				if (obj->GetDescriptor() != info->descriptor)
					goto __ret;
			}
			if (info->mask & MNQI_USERPROC) {
				if ((*info->proc)(node, info->proc_param) == TREENODE_PROC_RET_CONTINUE)
					goto __ret;
			}

			info->node = node;

			ret = TREENODE_PROC_RET_ABORT;
		}
	__ret:
		return ret;
	}

	LW_RESULT lwDestroyNodeObject(lwITreeNode* node) {
		LW_RESULT ret = LW_RET_FAILED;


		if (node->EnumTree(__tree_enum_destroy, 0, TREENODE_PROC_INORDER) == TREENODE_PROC_RET_ABORT)
			goto __ret;

		lwReleaseTreeNodeList(node);

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_STD_IMPLEMENTATION(lwNodeObject)

	lwNodeObject::lwNodeObject(lwIResourceMgr* res_mgr)
		: _res_mgr(res_mgr), _obj_root(0) {
		_obj_root = LW_NEW(lwTreeNode);
		lwINode* node = 0;
		_res_mgr->CreateNode(&node, NODE_DUMMY);
		_obj_root->SetData(node);
	}

	lwNodeObject::~lwNodeObject() {
		Destroy();

		LW_IF_RELEASE(((lwINode*)_obj_root->GetData()));
		LW_RELEASE(_obj_root);
	}

	LW_RESULT lwNodeObject::Update() {
		LW_RESULT ret = LW_RET_FAILED;

		if (_obj_root->EnumTree(__tree_enum_update, 0, TREENODE_PROC_PREORDER) == TREENODE_PROC_RET_ABORT)
			goto __ret;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeObject::Render() {
		LW_RESULT ret = LW_RET_FAILED;

		if (_obj_root->EnumTree(__tree_enum_render, 0, TREENODE_PROC_PREORDER) == TREENODE_PROC_RET_ABORT)
			goto __ret;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeObject::IgnoreNodesRender(const IgnoreStruct* is) {
		LW_RESULT ret = LW_RET_FAILED;

		if (_obj_root->EnumTree(__tree_enum_render, (void*)is, TREENODE_PROC_PREORDER_IGNORE) ==
			TREENODE_PROC_RET_ABORT)
			goto __ret;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeObject::CullPrimitive() {
		LW_RESULT ret = LW_RET_FAILED;

		lwISysGraphics* sys_grh = _res_mgr->GetSysGraphics();
		lwISceneMgr* scn_mgr = sys_grh->GetSceneMgr();
		lwIOptionMgr* opt_mgr = sys_grh->GetSystem()->GetOptionMgr();
		BOOL cull_flag = opt_mgr->GetByteFlag(OPTION_FLAG_CULLPRIMITIVE_MODEL);

		if (cull_flag == 0)
			goto __ret_ok;

		if (_obj_root->EnumTree(__tree_proc_cullprimitive, (void*)scn_mgr, TREENODE_PROC_PREORDER) ==
			TREENODE_PROC_RET_ABORT)
			goto __ret;

	__ret_ok:
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeObject::Destroy() {
		LW_RESULT ret = LW_RET_FAILED;

		lwITreeNode* obj_node = _obj_root->GetChild();
		if (obj_node == 0)
			goto __addr_ret_ok;

		if (LW_RESULT r = lwDestroyNodeObject(obj_node); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwDestroyNodeObject failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		_obj_root->SetChild(0);

	__addr_ret_ok:
		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	void lwNodeObject::SetMatrix(const lwMatrix44* mat) {
		lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
		o->SetLocalMatrix(mat);
	}

	lwMatrix44* lwNodeObject::GetMatrix() {
		lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
		return o->GetWorldMatrix();
	}

	lwIByteSet* lwNodeObject::GetStateSet() {
		lwINodeDummy* o = (lwINodeDummy*)_obj_root->GetData();
		return o->GetStateSet();
	}

	LW_RESULT lwNodeObject::QueryTreeNode(lwModelNodeQueryInfo* info) {
		return _obj_root->EnumTree(__tree_enum_query, (void*)info, TREENODE_PROC_PREORDER) == TREENODE_PROC_RET_ABORT
				   ? LW_RET_OK
				   : LW_RET_FAILED;
	}

	LW_RESULT lwNodeObject::InsertTreeNode(lwITreeNode* parent_node, lwITreeNode* prev_node, lwITreeNode* node) {
		LW_RESULT ret = LW_RET_FAILED;

		if (parent_node == 0) {
			parent_node = _obj_root;
		}

		if (LW_RESULT r = parent_node->InsertChild(prev_node, node); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] parent_node->InsertChild failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}
		{
			lwITreeNode* parent = node->GetParent();
			assert(parent && "invalid parent when called lwNodeObject::InsertTreeNode");

			lwINode* model_parent = (lwINode*)parent->GetData();
			assert(model_parent && "invalid model_parent when called lwNodeObject::InsertTreeNode");

			lwINode* model = (lwINode*)node->GetData();
			assert(model && "invalid model when called lwNodeObject::InsertTreeNode");

			if (LW_RESULT r = model->SetParent(model_parent); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] model->SetParent failed: id={}, ret={}",
							 __FUNCTION__, model->GetID(), static_cast<long long>(r));
				goto __ret;
			}

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	LW_RESULT lwNodeObject::RemoveTreeNode(lwITreeNode* node) {
		LW_RESULT ret = LW_RET_FAILED;

		lwITreeNode* parent = node->GetParent();

		assert(parent && "invalid node when called lwNodeObject::RemoveTreeNode");

		if (parent == 0)
			goto __ret;

		if (LW_RESULT r = parent->RemoveChild(node); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] parent->RemoveChild failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}

		{
			lwINode* model = (lwINode*)node->GetData();
			if (model == 0)
				goto __ret;

			if (LW_RESULT r = model->SetParent(0); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] model->SetParent(0) failed: id={}, ret={}",
							 __FUNCTION__, model->GetID(), static_cast<long long>(r));
				goto __ret;
			}

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	LW_RESULT lwNodeObject::Load(std::string_view file, DWORD flag, lwITreeNode* parent_node) {
		LW_RESULT ret = LW_RET_FAILED;

		lwModelInfo info;
		lwITreeNode* tree_node = 0;

		if (flag != MODELOBJECT_LOAD_RESET
			&& flag != MODELOBJECT_LOAD_MERGE
			&& flag != MODELOBJECT_LOAD_MERGE2) {
			goto __ret;
		}

		if (LW_RESULT r = info.Load(file); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] info.Load failed: file={}, flag={}, ret={}",
						 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file), flag, static_cast<long long>(r));
			goto __ret;
		}


		if (LW_RESULT r = lwLoadModelInfo(&tree_node, &info, _res_mgr); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwLoadModelInfo failed: file={}, ret={}",
						 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file), static_cast<long long>(r));
			goto __ret;
		}

		if (flag == MODELOBJECT_LOAD_RESET || flag == MODELOBJECT_LOAD_MERGE) {
			if (flag == MODELOBJECT_LOAD_RESET) {
				if (LW_RESULT r = Destroy(); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] Destroy failed: file={}, ret={}",
								 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file), static_cast<long long>(r));
					goto __ret;
				}
			}

			lwITreeNode* child_node = tree_node->GetChild();
			lwITreeNode* sibling_node;

			while (child_node) {
				sibling_node = child_node->GetSibling();

				if (LW_RESULT r = InsertTreeNode(parent_node, 0, child_node); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] InsertTreeNode(child) failed: file={}, ret={}",
								 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file), static_cast<long long>(r));
					goto __ret;
				}

				child_node = sibling_node;
			}

			tree_node->SetChild(0);
			// release root dummy node
		}
		else if (flag == MODELOBJECT_LOAD_MERGE2) {
			if (LW_RESULT r = InsertTreeNode(parent_node, 0, tree_node); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] InsertTreeNode(merge2) failed: file={}, ret={}",
							 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file), static_cast<long long>(r));
				goto __ret;
			}
		}

		ret = LW_RET_OK;
	__ret:
		if (tree_node) {
			lwDestroyNodeObject(tree_node);
		}
		return ret;
	}

	// assistant proc
	DWORD __tree_proc_play_pose(lwITreeNode* node, void* param) {
		DWORD ret = TREENODE_PROC_RET_ABORT;

		lwPlayPoseInfo* info = (lwPlayPoseInfo*)param;
		lwINode* base_node = (lwINode*)node->GetData();
		if (base_node == 0)
			goto __ret;

		switch (base_node->GetType()) {
		case NODE_PRIMITIVE:
			if (LW_RESULT r = lwNodePrimitive_PlayPoseAll((lwINodePrimitive*)base_node, info); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwNodePrimitive_PlayPoseAll failed: id={}, ret={}",
							 __FUNCTION__, base_node->GetID(), static_cast<long long>(r));
				goto __ret;
			}
			break;
		case NODE_BONECTRL:
			if (LW_RESULT r = lwNodeBoneCtrl_PlayPose((lwINodeBoneCtrl*)base_node, info); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwNodeBoneCtrl_PlayPose failed: id={}, ret={}",
							 __FUNCTION__, base_node->GetID(), static_cast<long long>(r));
				goto __ret;
			}
			break;
		case NODE_DUMMY:
			if (LW_RESULT r = lwNodeDummy_PlayPose((lwINodeDummy*)base_node, info); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] lwNodeDummy_PlayPose failed: id={}, ret={}",
							 __FUNCTION__, base_node->GetID(), static_cast<long long>(r));
				goto __ret;
			}
			break;
		case NODE_HELPER:
			break;
		default:
			__debugbreak();
		}

		ret = TREENODE_PROC_RET_CONTINUE;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeObject_PlayDefaultPose(lwINodeObject* obj) {
		LW_RESULT ret = LW_RET_FAILED;

		lwPlayPoseInfo ppi;
		memset(&ppi, 0, sizeof(ppi));
		ppi.bit_mask = PPI_MASK_DEFAULT;
		ppi.pose = 0;
		ppi.frame = 0.0f;
		ppi.type = PLAY_LOOP;
		ppi.velocity = 0.5f; // doesnt seem to change anything

		if (obj->GetTreeNodeRoot()->EnumTree(__tree_proc_play_pose, (void*)&ppi, TREENODE_PROC_PREORDER)
			!= TREENODE_PROC_RET_CONTINUE)
			goto __ret;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive_PlayPose(lwINodePrimitive* obj, const lwPlayPoseInfo* info, DWORD ctrl_type, DWORD subset,
									   DWORD stage) {
		LW_RESULT ret = LW_RET_FAILED;
		lwIAnimCtrlAgent* anim_agent = obj->GetAnimCtrlAgent();
		if (anim_agent == 0) {
			ret = LW_RET_OK_1;
			goto __ret;
		}

		lwAnimCtrlObjTypeInfo type_info;
		type_info.type = ctrl_type;
		type_info.data[0] = subset;
		type_info.data[1] = stage;

		{
			lwIAnimCtrlObj* ctrl_obj = anim_agent->GetAnimCtrlObj(&type_info);
			if (ctrl_obj == 0) {
				ret = LW_RET_OK_1;
				goto __ret;
			}

			if (LW_RESULT r = ctrl_obj->PlayPose(info); LW_FAILED(r)) {
				ToLogService("errors", LogLevel::Error,
							 "[{}] ctrl_obj->PlayPose failed: ctrl_type={}, subset={}, stage={}, ret={}",
							 __FUNCTION__, ctrl_type, subset, stage, static_cast<long long>(r));
				goto __ret;
			}

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	LW_RESULT lwNodePrimitive_PlayPoseAll(lwINodePrimitive* obj, const lwPlayPoseInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		lwIAnimCtrlAgent* anim_agent = obj->GetAnimCtrlAgent();
		if (anim_agent == 0) {
			ret = LW_RET_OK_1;
			goto __ret;
		}

		{
			lwAnimCtrlObjTypeInfo type_info;
			lwIAnimCtrlObj* ctrl_obj;
			DWORD n = anim_agent->GetAnimCtrlObjNum();

			for (DWORD i = 0; i < n; i++) {
				ctrl_obj = anim_agent->GetAnimCtrlObj(i);

				// lwAnimCtrlObjBonelwPrimitive
				ctrl_obj->GetTypeInfo(&type_info);
				if (type_info.type == ANIM_CTRL_TYPE_BONE)
					continue;

				if (LW_RESULT r = ctrl_obj->PlayPose(info); LW_FAILED(r)) {
					ToLogService("errors", LogLevel::Error,
								 "[{}] ctrl_obj->PlayPose failed: idx={}, type={}, ret={}",
								 __FUNCTION__, i, type_info.type, static_cast<long long>(r));
					goto __ret;
				}
			}

			ret = LW_RET_OK;
		}
	__ret:
		return ret;
	}

	LW_RESULT lwNodeBoneCtrl_PlayPose(lwINodeBoneCtrl* obj, const lwPlayPoseInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		lwIAnimCtrlObjBone* ctrl_obj_bone = obj->GetAnimCtrlObj();
		if (ctrl_obj_bone == 0) {
			ret = LW_RET_OK_1;
			goto __ret;
		}

		if (LW_RESULT r = ctrl_obj_bone->PlayPose(info); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] ctrl_obj_bone->PlayPose failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}


		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeDummy_PlayPose(lwINodeDummy* obj, const lwPlayPoseInfo* info) {
		LW_RESULT ret = LW_RET_FAILED;

		lwIAnimCtrlObjMat* ctrl_obj = obj->GetAnimCtrlObj();
		if (ctrl_obj == 0) {
			ret = LW_RET_OK_1;
			goto __ret;
		}

		if (LW_RESULT r = ctrl_obj->PlayPose(info); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] ctrl_obj->PlayPose failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			goto __ret;
		}


		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNode_ShowBoundingObject(lwINode* obj, DWORD flag) {
		LW_RESULT ret = LW_RET_FAILED;

		lwIHelperObject* helper_obj;
		lwINodePrimitive* dummy_obj;

		DWORD type = obj->GetType();

		switch (type) {
		case NODE_PRIMITIVE:
			helper_obj = ((lwINodePrimitive*)obj)->GetHelperObject();
			if (helper_obj) {
				helper_obj->SetVisible(flag);
			}
			ret = LW_RET_OK;
			break;
		case NODE_BONECTRL:
			ret = LW_RET_OK;
			break;
		case NODE_HELPER:
			ret = LW_RET_OK;
			break;
		case NODE_LIGHT:
			ret = LW_RET_OK;
			break;
		case NODE_CAMERA:
			ret = LW_RET_OK;
			break;
		case NODE_DUMMY:
			dummy_obj = ((lwINodeDummy*)obj)->GetAssistantObject();
			if (dummy_obj) {
				dummy_obj->GetStateSet()->SetValue(STATE_VISIBLE, (BYTE)flag);
			}
			ret = LW_RET_OK;
			break;
		default:
			;
		}

		return ret;
	}

	static DWORD __tree_proc_show_bo(lwITreeNode* node, void* param) {
		if (LW_RESULT r = lwNode_ShowBoundingObject((lwINode*)node->GetData(), *(DWORD*)param); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] lwNode_ShowBoundingObject failed: ret={}",
						 __FUNCTION__, static_cast<long long>(r));
			return TREENODE_PROC_RET_ABORT;
		}

		return TREENODE_PROC_RET_CONTINUE;
	}


	static DWORD __tree_proc_dump_objtree(lwITreeNode* node, void* param) {
		lwINode* obj_node = (lwINode*)node->GetData();
		FILE* fp = (FILE*)param;

		DWORD l = node->GetDepthLevel();
		DWORD c = 0;
		lwINode* p = obj_node->GetParent();
		while (p) {
			c++;
			p = p->GetParent();
		}
		assert(l == c);

		for (DWORD i = 0; i < l; i++) {
			fprintf(fp, "\t");
		}

		fprintf(fp, "type: %d, id: %d, parent: %d\n",
				obj_node->GetType(),
				obj_node->GetID(),
				l == 0 ? -1 : obj_node->GetParent()->GetID());


		return TREENODE_PROC_RET_CONTINUE;
	}

	static DWORD __tree_proc_getprinum(lwITreeNode* node, void* param) {
		lwINode* obj = (lwINode*)node->GetData();

		if (obj->GetType() == NODE_PRIMITIVE) {
			if (obj->GetStateSet()->GetValue(STATE_FRAMECULLING)) {
				*((DWORD*)param) += 1;
			}
		}

		return TREENODE_PROC_RET_CONTINUE;
	}

	LW_RESULT lwNodeObject_ShowBoundingObject(lwINodeObject* obj, DWORD flag) {
		LW_RESULT ret = LW_RET_FAILED;

		if (obj->GetTreeNodeRoot()->EnumTree(__tree_proc_show_bo, (void*)&flag, TREENODE_PROC_PREORDER)
			!= TREENODE_PROC_RET_CONTINUE)
			goto __ret;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}

	LW_RESULT lwNodeObject_DumpObjectTree(lwINodeObject* obj, std::string_view file) {
		LW_RESULT ret = LW_RET_FAILED;

		FILE* fp = fopen(std::string{file}.c_str(), "wt");
		if (fp == 0)
			goto __ret;

		if (obj->GetTreeNodeRoot()->EnumTree(__tree_proc_dump_objtree, (void*)fp, TREENODE_PROC_PREORDER)
			!= TREENODE_PROC_RET_CONTINUE)
			goto __ret;

		ret = LW_RET_OK;


	__ret:
		if (fp) {
			fclose(fp);
		}
		return ret;
	}

	LW_RESULT lwNodeObject_GetPrimitiveCullingNum(lwINodeObject* obj, DWORD* num) {
		LW_RESULT ret = LW_RET_FAILED;

		*num = 0;

		if (obj->GetTreeNodeRoot()->EnumTree(__tree_proc_getprinum, (void*)num, TREENODE_PROC_PREORDER)
			!= TREENODE_PROC_RET_CONTINUE)
			goto __ret;

		ret = LW_RET_OK;
	__ret:
		return ret;
	}


LW_END

#endif
