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
	class lwGeomManager {
	public:
		lwGeomManager();
		~lwGeomManager();

		lwGeomObjInfo* GetGeomObjInfo(std::string_view file);
		bool LoadGeomobj(std::string_view file);

		lwIAnimDataBone* GetBoneData(std::string_view file);
		bool LoadBoneData(std::string_view file);

	private:
		typedef std::map<std::string, lwGeomObjInfo*> GEOMOBJ_MAP;
		GEOMOBJ_MAP m_GeomobjMap;

		typedef std::map<std::string, lwIAnimDataBone*> ANIMDATA_MAP;
		ANIMDATA_MAP m_AnimDataMap;
	};

	extern lwGeomManager g_GeomManager;

	class lwPhysique : public lwIPhysique, public lwLinkCtrl {
	private:
		DWORD _id;
		lwIResourceMgr* _res_mgr;
		lwISceneMgr* _scene_mgr;
		lwIPrimitive* _obj_seq[LW_MAX_SUBSKIN_NUM];
		lwIAnimCtrlAgent* _anim_agent;

		lwStateCtrl _state_ctrl;
		char _file_name[LW_MAX_NAME];
		lwMatrix44 _mat_base;
		float _opacity;
		bool _start;
		bool _end;

		LW_STD_DECLARATION();

	public:
		lwPhysique(lwIResourceMgr* res_mgr);
		lwPhysique();
		~lwPhysique();

		// link ctrl method
		virtual LW_RESULT GetLinkCtrlMatrix(lwMatrix44* mat, DWORD link_id);

		lwIResourceMgr* GetResourceMgr() {
			return _res_mgr;
		}

		LW_RESULT Destroy();
		LW_RESULT LoadBone(std::string_view file);
		LW_RESULT LoadPrimitive(DWORD part_id, lwGeomObjInfo* geom_info);
		LW_RESULT LoadPrimitive(DWORD part_id, std::string_view file);
		LW_RESULT DestroyPrimitive(DWORD part_id);

		LW_RESULT CheckPrimitive(DWORD part_id) {
			return _obj_seq[part_id] ? LW_RET_OK : LW_RET_FAILED;
		}

		LW_RESULT Update();
		LW_RESULT Render();

		lwMatrix44* GetMatrix() {
			return &_mat_base;
		}

		void SetMatrix(const lwMatrix44* mat) {
			_mat_base = *mat;
		}

		void SetOpacity(float opacity);

		LW_RESULT PlayPose(const lwPlayPoseInfo* info);
		LW_RESULT PlayObjImpPose(const lwPlayPoseInfo* info, DWORD obj_id, DWORD ctrl_type);

		lwIPoseCtrl* GetObjImpPoseCtrl(DWORD skin_id, DWORD ctrl_type);

		LW_RESULT HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);;
		LW_RESULT HitTestPhysique(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);

		void ShowHelperObject(int show);
		void ShowBoundingObjectPhysique(int show);

		LW_RESULT SetItemLink(const lwItemLinkInfo* info);
		LW_RESULT ClearItemLink(lwIItem* obj);

		void SetObjState(DWORD state, BYTE value) {
			return _state_ctrl.SetState(state, value);
		}

		DWORD GetObjState(DWORD state) const {
			return _state_ctrl.GetState(state);
		}

		lwIPrimitive* GetPrimitive(DWORD id) {
			return _obj_seq[id];
		}

		lwIAnimCtrlAgent* GetAnimCtrlAgent() {
			return _anim_agent;
		}

		LW_RESULT RegisterSceneMgr(lwISceneMgr* scene_mgr) {
			_scene_mgr = scene_mgr;
			return LW_RET_OK;
		}

		LW_RESULT SetTextureLOD(DWORD level);

		float GetOpacity() {
			return _opacity;
		}

		virtual void Start() {
			_start = true;
		}

		virtual void End() {
			_end = true;
		}

		virtual bool isLoaded() {
			return _end;
		}

		void setComponentColour(size_t index, D3DCOLOR colour, const std::string& filterTextureName) {
			mIndexColourFilterList[index] = ColourFilterPair(colour, filterTextureName);
		}

		void setTextureOperation(size_t index, D3DTEXTUREOP operation) {
			mIndexTextureOPList[index] = operation;
		}

		std::string_view getTextureOperationDescription(size_t operation) {
			return _res_mgr->getTextureOperationDescription(operation);
		}

		void setPixelShader(size_t index, const std::string& filename) {
			if (_obj_seq) {
				if (_obj_seq[index]) {
					_obj_seq[index]->setPixelShader(filename);
				}
			}
		}

	private:
		typedef std::pair<D3DCOLOR, std::string> ColourFilterPair;
		typedef std::map<size_t, ColourFilterPair> IndexColourFilterPairList;
		IndexColourFilterPairList mIndexColourFilterList;

		typedef std::map<size_t, D3DTEXTUREOP> IndexTextureOPPairList;
		IndexTextureOPPairList mIndexTextureOPList;
	};

LW_END
