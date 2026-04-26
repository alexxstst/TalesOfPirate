//
#include "stdafx.h"


#include "lwCoordinateSys.h"

LW_BEGIN
	LW_STD_IMPLEMENTATION(lwCoordinateSys)

	lwCoordinateSys::lwCoordinateSys() {
		_active_id = 0;

		for (DWORD i = 0; i < COORD_SEQ_SIZE; i++) {
			lwMatrix44Identity(&_coord_mat_seq[i]);
		}
	}


	LW_RESULT lwCoordinateSys::SetActiveCoordSys(DWORD id) {
		if (id >= COORD_SEQ_SIZE)
			return LW_RET_FAILED;

		_active_id = id;

		return LW_RET_OK;
	}

	LW_RESULT lwCoordinateSys::SetCoordSysMatrix(DWORD id, const lwMatrix44* mat) {
		if (id >= COORD_SEQ_SIZE || mat == 0)
			return LW_RET_FAILED;

		_coord_mat_seq[id] = *mat;

		return LW_RET_OK;
	}

	LW_RESULT lwCoordinateSys::GetCoordSysMatrix(DWORD id, lwMatrix44* mat) {
		if (id >= COORD_SEQ_SIZE || mat == 0)
			return LW_RET_FAILED;

		*mat = _coord_mat_seq[id];

		return LW_RET_OK;
	}

	LW_RESULT lwCoordinateSys::GetCurCoordSysMatrix(lwMatrix44* mat) {
		*mat = _coord_mat_seq[_active_id];
		return LW_RET_OK;
	}

	LW_RESULT lwCoordinateSys::GetActiveCoordSys(DWORD* id) {
		*id = _active_id;
		return LW_RET_OK;
	}

LW_END
