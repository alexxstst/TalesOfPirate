//
#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include "lwDirectX.h"

LW_BEGIN
	class lwCamera {
	public:
		static void GetMatrixCameraToView(lwMatrix44* mat, const lwMatrix44* cam_mat);
		static void GetMatrixViewToCamera(lwMatrix44* mat, const lwMatrix44* mat_view);

	private:
		lwMatrix44 _m;

	public:
		float _fov;
		float _aspect;
		float _znear;
		float _zfar;

	public:
		lwCamera();

		int SetPerspectiveFov(float fov, float aspect, float zn, float zf);

		void SetPosition(lwVector3* pos);
		void SetPosition(lwVector3* face, lwVector3* up);
		void SetPosition(lwVector3* pos, lwVector3* face, lwVector3* up);

		void SetPosition(const lwMatrix44* m) {
			_m = *m;
		}

		void Rotate(DWORD axis, float angle);

		void SetTransform();

		lwMatrix44* GetMatrix() {
			return &_m;
		}

		void GetMatrixView(lwMatrix44* mat);
		void GetMatrixProj(lwMatrix44* mat);
	};

LW_END
