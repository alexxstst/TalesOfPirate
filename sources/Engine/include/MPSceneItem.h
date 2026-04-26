//
#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include "lwInterface.h"
#include "lwObjectMethod.h"


LW_BEGIN
	class MPSceneItem : public lwMatrixCtrl {
	private:
		lwIItem* _obj;

	public:
		MPSceneItem();
		MPSceneItem(lwISysGraphics* sys_graphics);
		virtual ~MPSceneItem();

		lwIItem* GetObject() {
			return _obj;
		}

		LW_RESULT Load(std::string_view file, int arbitrary_flag = 0);


		void FrameMove();
		void Render();
		void SetMaterial(const D3DMATERIALX* mtl);
		void SetOpacity(float opacity);
		float GetOpacity();


		LW_RESULT Copy(const MPSceneItem* obj);
		void Destroy();

		lwIPoseCtrl* GetObjImpPoseCtrl(DWORD ctrl_type);
		LW_RESULT PlayObjImpPose(DWORD ctrl_type, DWORD pose_id, DWORD play_type, float start_frame = 0.0f,
								 float velocity = 1.0f);

		LW_RESULT HitTestPrimitive(lwPickInfo* info, const lwVector3* org, const lwVector3* ray);

		void ShowBoundingObject(int show);

		LW_RESULT GetObjDummyRunTimeMatrix(lwMatrix44* mat, DWORD id);
		LW_RESULT PlayDefaultAnimation(bool IsGlitched = false);
		LW_RESULT ResetItemTexture(DWORD subset, lwITex* tex, lwITex** old_tex);

		lwIPrimitive* GetPrimitive();

		void SetObjState(DWORD state, BYTE value);
		DWORD GetObjState(DWORD state) const;
		void SetTextureLOD(DWORD level);

		// added by clp
	public :
		void GetDummyLocalMatrix(lwMatrix44* mat, DWORD id);
	};

	typedef MPSceneItem MSPceneItem;


LW_END
