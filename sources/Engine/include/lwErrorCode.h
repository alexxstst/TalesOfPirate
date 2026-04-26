//
#pragma once

#include "lwHeader.h"

LW_BEGIN
	enum lwErrCodeEnum {
		LW_RET_OK_1 = 1,
		LW_RET_OK_2 = 2,
		LW_RET_OK_3 = 3,

		LW_RET_NULL = 1,

		LW_RET_FAILED_1 = -1,
		LW_RET_FAILED_2 = -2,
		LW_RET_FAILED_3 = -3,

		LW_RET_UNKNOWN = -1,

		ERR_INVALID_CALL = -1,
		ERR_INVALID_PARAM = -99,
		ERR_INVALID_DATA = -2,

		// stream obj
		ERR_FULL_STREAMSIZE = -2,
		ERR_STREAM_ENTRYSEQ_FULL = -3,
		ERR_STREAM_FILLDATA_FAILED = -4,
		ERR_STREAM_REGISTER_FAILED = -5,
	};

#define LW_FAILED( r )      ( (LW_RESULT)(r) < 0 )
#define LW_SUCCEEDED( r )   ( (LW_RESULT)(r) >= 0 )

LW_END
