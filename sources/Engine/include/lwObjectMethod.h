#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include <set>


LW_BEGIN
	class lwMatrixCtrl {
	protected:
		float _angle_yaw;
		float _angle_pitch;
		float _angle_roll;
		// modified by clp
		lwVector3 _scale;

		lwMatrix44* _mat_ptr;

	public:
		lwMatrixCtrl();

		void BindMatrix(lwMatrix44* mat) {
			_mat_ptr = mat;
		}

		lwMatrix44* GetMatrix() {
			return _mat_ptr;
		}

		void SetMatrix(const lwMatrix44* mat) {
			*_mat_ptr = *mat;
		}

		void SetPos(float* pos);
		void SetYaw(float yaw);
		void SetPitch(float pitch);
		void SetRoll(float roll);
		void SetScale(const lwVector3& scale);
		void UpdateYawPitchRoll();

		float GetYaw() const {
			return _angle_yaw;
		}

		float GetPitch() const {
			return _angle_pitch;
		}

		float GetRoll() const {
			return _angle_roll;
		}

		const lwVector3& GetScale() const {
			return _scale;
		}

		void GetFaceDir(lwVector3* dir) {
			dir->x = _mat_ptr->_21;
			dir->y = _mat_ptr->_22;
			dir->z = _mat_ptr->_23;
		}

		void GetRightDir(lwVector3* dir) {
			dir->x = -_mat_ptr->_11;
			dir->y = -_mat_ptr->_12;
			dir->z = -_mat_ptr->_13;
		}

		void GetUpDir(lwVector3* dir) {
			dir->x = _mat_ptr->_31;
			dir->y = _mat_ptr->_32;
			dir->z = _mat_ptr->_33;
		}
	};

LW_END
