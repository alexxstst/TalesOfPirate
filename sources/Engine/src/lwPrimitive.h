//
#pragma once

#include "lwHeader.h"
#include "lwDirectX.h"
#include "lwStreamObj.h"
#include "lwExpObj.h"
#include "lwITypes.h"
#include "lwInterfaceExt.h"
#include "lwDeviceObject.h"
#include "lwResourceMgr.h"
#include "lwPrimitiveHelper.h"

LW_BEGIN
	class lwPrimitive : public lwIPrimitive {
		LW_STD_DECLARATION()

	protected:
		DWORD _id;
		DWORD _parent_id;
		lwStateCtrl _state_ctrl;

		lwIResourceMgr* _res_mgr;
		lwIMeshAgent* _mesh_agent;
		lwIMtlTexAgent* _mtltex_agent_seq[LW_MAX_SUBSET_NUM];
		lwIAnimCtrlAgent* _anim_agent;
		lwIRenderCtrlAgent* _render_agent;
		lwIHelperObject* _helper_object;

	private:
		LW_RESULT _UpdateTransparentState();

	public:
		lwPrimitive(lwIResourceMgr* mgr);
		virtual ~lwPrimitive();


		virtual void SetMatrixLocal(const lwMatrix44* mat);
		virtual void SetMatrixParent(const lwMatrix44* mat);

		lwMatrix44* GetMatrixLocal();
		lwMatrix44* GetMatrixGlobal();

		void SetID(DWORD id) {
			_id = id;
		}

		void SetParentID(DWORD id) {
			_parent_id = id;
		}

		DWORD GetID() const {
			return _id;
		}

		DWORD GetParentID() const {
			return _parent_id;
		}


		LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);

		LW_RESULT DestroyRenderCtrlAgent();
		LW_RESULT DestroyMeshAgent();
		LW_RESULT DestroyMtlTexAgent(DWORD subset);
		LW_RESULT Destroy();


		LW_RESULT Load(lwGeomObjInfo* info, std::string_view tex_path, const lwResFile* res);
		LW_RESULT LoadMesh(lwMeshInfo* info);
		LW_RESULT LoadMesh(const lwResFileMesh* info);
		LW_RESULT LoadMtlTex(DWORD mtl_id, lwMtlTexInfo* info, std::string_view tex_path);
		LW_RESULT LoadAnimData(lwAnimDataInfo* info, std::string_view tex_path, const lwResFile* res);
		LW_RESULT LoadRenderCtrl(const lwRenderCtrlCreateInfo* rcci);

		LW_RESULT ExtractGeomObjInfo(lwGeomObjInfo* info);

		virtual LW_RESULT Update();
		virtual LW_RESULT Render();
		virtual LW_RESULT RenderSubset(DWORD subset);

		LW_RESULT Clone(lwIPrimitive** ret_obj);

		lwIResourceMgr* GetResourceMgr() {
			return _res_mgr;
		}

		lwIMtlTexAgent* GetMtlTexAgent(DWORD id) {
			return _mtltex_agent_seq[id];
		}

		lwIMeshAgent* GetMeshAgent() {
			return _mesh_agent;
		}

		lwIAnimCtrlAgent* GetAnimAgent() {
			return _anim_agent;
		}

		lwIRenderCtrlAgent* GetRenderCtrlAgent() {
			return _render_agent;
		}

		lwIHelperObject* GetHelperObject() {
			return _helper_object;
		}

		LW_RESULT GetSubsetNum(DWORD* subset_num);


		void SetMeshAgent(lwIMeshAgent* agent) {
			_mesh_agent = agent;
		}

		void SetMtlTexAgent(DWORD subset, lwIMtlTexAgent* agent) {
			_mtltex_agent_seq[subset] = agent;
		}

		void SetAnimCtrlAgent(lwIAnimCtrlAgent* agent) {
			_anim_agent = agent;
		}

		void SetRenderCtrl(lwIRenderCtrlAgent* obj) {
			_render_agent = obj;
		}

		void SetHelperObject(lwIHelperObject* obj) {
			_helper_object = obj;
		}

		void SetMaterial(const lwMaterial* mtl);
		void SetOpacity(float opacity);

		// object state method
		void SetState(const lwStateCtrl* ctrl) {
			_state_ctrl = *ctrl;
		}

		void SetState(DWORD state, BYTE value) {
			_state_ctrl.SetState(state, value);
		}

		BYTE GetState(DWORD state) const {
			return _state_ctrl.GetState(state);
		}

		LW_RESULT SetTextureLOD(DWORD level);
		LW_RESULT PlayDefaultAnimation(float velocity = 1.0f);
		LW_RESULT ResetTexture(DWORD subset, DWORD stage, std::string_view file, std::string_view tex_path);

		void setPixelShader(const std::string& filename) {
			_render_agent->setPixelShader(filename);
		}
	};

	LW_RESULT lwPrimitivePlayDefaultAnimation(lwIPrimitive* obj, float velocity = 1.0f);

LW_END
