//
#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include "lwInterfaceExt.h"
#include "lwClassDecl.h"
#include "lwITypes.h"
#include "lwPrimitive.h"
#include "lwLinkCtrl.h"


LW_BEGIN
	class lwModel : public lwIModel, public lwLinkCtrl {
	private:
		lwIResourceMgr* _res_mgr;
		lwISceneMgr* _scene_mgr;

		lwStateCtrl _state_ctrl;
		char _file_name[LW_MAX_NAME];
		lwIPrimitive* _obj_seq[LW_MAX_MODEL_GEOM_OBJ_NUM];
		DWORD _obj_num;
		lwMatrix44 _mat_base;
		DWORD _id;
		DWORD _model_id;

		lwIHelperObject* _helper_object;
		float _opacity;

		LW_STD_DECLARATION()

	public:
		lwModel(lwIResourceMgr* res_mgr);
		~lwModel();

		DWORD GetModelID() const {
			return _model_id;
		}

		void SetFileName(const char* file) {
			_tcscpy(_file_name, file);
		}

		char* GetFileName() {
			return _file_name;
		}

		LW_RESULT Clone(lwIModel** ret_obj);
		LW_RESULT Copy(lwIModel* src_obj);

		lwMatrix44* GetMatrix() {
			return &_mat_base;
		}

		void SetMatrix(const lwMatrix44* mat) {
			_mat_base = *mat;
		}

		LW_RESULT Load(lwIModelObjInfo* info);
		LW_RESULT Load(const char* file, DWORD model_id = LW_INVALID_INDEX);
		LW_RESULT Update();
		LW_RESULT Render();
		LW_RESULT RenderPrimitive(DWORD id);
		LW_RESULT RenderHelperObject();
		LW_RESULT Destroy();

		void SetMaterial(const lwMaterial* mtl);
		void SetOpacity(float opacity);


		LW_RESULT HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);
		LW_RESULT HitTestPrimitiveHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
											 const char* type_name);
		LW_RESULT HitTestPrimitiveHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
											const char* type_name);
		LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);
		LW_RESULT HitTestHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
									const char* type_name);
		LW_RESULT HitTestHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray, const char* type_name);

		LW_RESULT PlayDefaultAnimation(bool IsGlitched = false);

		LW_RESULT SortPrimitiveObj();


		void ShowHelperObject(int show);
		void ShowHelperMesh(int show);
		void ShowHelperBox(int show);
		void ShowBoundingObject(int show);

		lwIHelperObject* GetHelperObject() {
			return _helper_object;
		}

		DWORD GetPrimitiveNum() const {
			return _obj_num;
		}

		lwIPrimitive* GetPrimitive(DWORD id) {
			return _obj_seq[id];
		}

		void SetObjState(DWORD state, BYTE value) {
			return _state_ctrl.SetState(state, value);
		}

		DWORD GetObjState(DWORD state) const {
			return _state_ctrl.GetState(state);
		}

		LW_RESULT RegisterSceneMgr(lwISceneMgr* scene_mgr) {
			_scene_mgr = scene_mgr;
			return LW_RET_OK;
		}

		LW_RESULT SetItemLink(const lwItemLinkInfo* info);
		LW_RESULT ClearItemLink(lwIItem* obj);

		// link ctrl method
		virtual LW_RESULT GetLinkCtrlMatrix(lwMatrix44* mat, DWORD link_id);

		LW_RESULT SetTextureLOD(DWORD level);

		float GetOpacity() {
			return _opacity;
		}

		LW_RESULT CullPrimitive();
		DWORD GetCullingPrimitiveNum();
		LW_RESULT ExtractModelInfo(lwIModelObjInfo* out_info);
	};


LW_END
