//
#include "stdafx.h"
#include "lwEfxTrack.h"

LW_BEGIN
	lwEfxTrack::lwEfxTrack() {
		_data = 0;
	}

	lwEfxTrack::~lwEfxTrack() {
		LW_SAFE_RELEASE(_data);
	}


	LW_RESULT lwEfxTrack::Load(std::string_view file) {
		FILE* fp = fopen(std::string{file}.c_str(), "rb");
		if (fp == NULL)
			return LW_RET_FAILED;

		_data = LW_NEW(lwAnimDataMatrix());
		_data->Load(fp, 0);

		fclose(fp);

		return LW_RET_OK;
	}

	LW_RESULT lwEfxTrack::Save(std::string_view file) {
		FILE* fp = fopen(std::string{file}.c_str(), "wb");
		if (fp == NULL)
			return LW_RET_FAILED;

		if (LW_RESULT r = _data->Save(fp); LW_FAILED(r)) {
			ToLogService("errors", LogLevel::Error,
						 "[{}] _data->Save failed: file={}, ret={}",
						 __FUNCTION__, (file.empty() ? std::string_view{"(null)"} : file), static_cast<long long>(r));
		}

		fclose(fp);

		return LW_RET_OK;
	}

LW_END
