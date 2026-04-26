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
	class lwItem : public lwIItem {
	private:
		lwIResourceMgr* _res_mgr;
		lwISceneMgr* _scene_mgr;

		lwLinkCtrl* _link_ctrl;
		DWORD _link_parent_id;
		DWORD _link_item_id;

		lwStateCtrl _state_ctrl;
		std::string _file_name;
		lwIPrimitive* _obj;
		lwMatrix44 _mat_base;
		DWORD _id;
		DWORD _item_type;
		float _opacity;

		LW_STD_DECLARATION()

	public:
		lwItem(lwIResourceMgr* res_mgr);
		~lwItem();

		void SetFileName(std::string_view file) {
			_file_name = file;
		}

		const std::string& GetFileName() {
			return _file_name;
		}

		virtual lwIPrimitive* GetPrimitive() {
			return _obj;
		}

		LW_RESULT Copy(lwIItem* src_obj);
		LW_RESULT Clone(lwIItem** ret_obj);

		lwMatrix44* GetMatrix() {
			return &_mat_base;
		}

		void SetMatrix(const lwMatrix44* mat) {
			_mat_base = *mat;
		}

		void SetOpacity(float opacity);

		LW_RESULT Load(const char* file, int arbitrary_flag = 0);
		LW_RESULT Load(lwIGeomObjInfo* info);
		LW_RESULT Update();
		LW_RESULT Render();
		LW_RESULT Destroy();

		LW_RESULT HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);

		void SetMaterial(const lwMaterial* mtl);

		void ShowBoundingObject(int show);

		const lwMatrix44* GetObjDummyMatrix(DWORD id);
		const lwMatrix44* GetObjBoneDummyMatrix(DWORD id);

		LW_RESULT PlayDefaultAnimation(bool IsGlitched = false);


		// dummy
		LW_RESULT GetDummyMatrix(lwMatrix44* mat, DWORD id);
		LW_RESULT GetObjDummyRunTimeMatrix(lwMatrix44* mat, DWORD id);

		LW_RESULT SetLinkCtrl(lwLinkCtrl* ctrl, DWORD link_parent_id, DWORD link_item_id);
		LW_RESULT ClearLinkCtrl();

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

		LW_RESULT SetTextureLOD(DWORD level);

		float GetOpacity() {
			return _opacity;
		}
	};


LW_END
