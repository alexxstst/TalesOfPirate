//
#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include "lwInterfaceExt.h"
#include "lwExpObj.h"

LW_BEGIN
	class lwEfxTrack {
	private:
		lwAnimDataMatrix* _data;

	public:
		lwEfxTrack();
		~lwEfxTrack();

		void SetData(lwAnimDataMatrix* data) {
			_data = data;
		}

		lwAnimDataMatrix* GetData() {
			return _data;
		}

		LW_RESULT Load(const char* file);
		LW_RESULT Save(const char* file);
	};

LW_END
