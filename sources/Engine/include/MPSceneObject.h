//
#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include "lwInterface.h"
#include "lwObjectMethod.h"
#include "MPSceneItem.h"
#include "MPCharacter.h"

LW_BEGIN
	class MPSceneObject : public lwMatrixCtrl {
	private:
		lwIModel* _model;
		MPSceneItem* _link_item_seq[LW_MAX_LINK_ITEM_NUM];
		DWORD _link_item_num;

	public:
		MPSceneObject();
		MPSceneObject(lwISysGraphics* sys_graphics);
		virtual ~MPSceneObject();

		LW_RESULT Load(std::string_view file, DWORD model_id = LW_INVALID_INDEX);


		void FrameMove();
		void Render();
		void SetMaterial(const D3DMATERIALX* mtl);
		void SetOpacity(float opacity);

		void Destroy();

		lwIPoseCtrl* GetObjImpPoseCtrl(DWORD skin_id, DWORD ctrl_type);
		LW_RESULT PlayObjImpPose(DWORD obj_id, DWORD ctrl_type, DWORD pose_id, DWORD play_type,
								 float start_frame = 0.0f, float velocity = 1.0f);

		LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);
		LW_RESULT HitTestHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray, std::string_view type_name);
		LW_RESULT HitTestHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
									std::string_view type_name);
		LW_RESULT HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);
		LW_RESULT HitTestPrimitiveHelperBox(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
											std::string_view type_name);
		LW_RESULT HitTestPrimitiveHelperMesh(lwPickInfo* info, const lwVector3* org, const lwVector3* ray,
											 std::string_view type_name);

		lwIPrimitive* GetPrimitive(DWORD id);

		void ShowHelperObject(int show);
		void ShowHelperMesh(int show);
		void ShowHelperBox(int show);
		void ShowBoundingObject(int show);

		LW_RESULT PlayDefaultAnimation(float velocity = 1.0f);

		void SetObjState(DWORD state, BYTE value);
		DWORD GetObjState(DWORD state) const;

		lwIModel* GetObject() {
			return _model;
		}

		lwIHelperObject* GetHelperObject() {
			return _model->GetHelperObject();
		}

		LW_RESULT SetItemLink(const lwSceneItemLinkInfo* info);
		LW_RESULT UnwieldItem(const MPSceneItem* obj);

		DWORD GetLinkItemNum() const {
			return _link_item_num;
		}

		MPSceneItem* GetLinkItem(DWORD id);
		DWORD GetPrimitiveNum() const;
		int GetPrimitiveBox(DWORD id, D3DXVECTOR3* p1, D3DXVECTOR3* p2);
		void SetTextureLOD(DWORD level);
		float GetOpacity();
		DWORD GetTransparentFlag() const;
	};

	typedef MPSceneObject MPSceneObject;

	class lwSceneObjectChair : public lwMatrixCtrl {
	private:
		lwMatrix44 _mat;
		lwBox _box;
		lwIPrimitive* _obj;

	public:
		lwSceneObjectChair();
		~lwSceneObjectChair();


		LW_RESULT Create();
		LW_RESULT Render();
		LW_RESULT HitTest(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);

		void SetMatrix(const lwMatrix44* mat) {
			_mat = *mat;
		}

		lwMatrix44* GetMatrix() {
			return &_mat;
		}
	};

	typedef lwSceneObjectChair MPSceneObjectChair;

LW_END
