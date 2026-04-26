//
#include "stdafx.h"
#include "lwD3DSettings.h"

LW_BEGIN
	LW_RESULT lwInitDefaultD3DCreateParam(lwD3DCreateParam* param, HWND hwnd) {
		IDirect3DX* d3d = Direct3DCreateX(D3D_SDK_VERSION);

		if (d3d == NULL)
			return LW_RET_FAILED;

		param->adapter = D3DADAPTER_DEFAULT;
		param->dev_type = D3DDEVTYPE_HAL;
		param->hwnd = hwnd;
		param->behavior_flag = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		D3DDISPLAYMODE mode;
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

		memset(&param->present_param, 0, sizeof(param->present_param));
		param->present_param.AutoDepthStencilFormat = D3DFMT_D24S8;
		param->present_param.BackBufferCount = 1;
		param->present_param.BackBufferFormat = mode.Format;
		param->present_param.BackBufferHeight = mode.Height;
		param->present_param.BackBufferWidth = mode.Width;
		param->present_param.EnableAutoDepthStencil = 1;
		param->present_param.hDeviceWindow = hwnd;
		param->present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;
		param->present_param.Windowed = 1;

		param->present_param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;


		LW_SAFE_RELEASE(d3d);

		return LW_RET_OK;
	}

	namespace {
		// Парсит десятичное число из ini-строки. Возвращает 0 на пустой/невалидный ввод —
		// прежнее поведение atoi() эквивалентно (для некорректного хвоста atoi брал префикс).
		int parseIntFromBuf(std::string_view buf) {
			int result = 0;
			const char* first = buf.data();
			const char* last = buf.data() + buf.size();
			std::from_chars(first, last, result);
			return result;
		}
	}

	LW_RESULT lwLoadD3DSettings(lwD3DCreateParam* param, const char* file) {
		memset(param, 0, sizeof(lwD3DCreateParam));

		char buf[LW_MAX_NAME];

		auto readInt = [&](const char* section, const char* key) {
			GetPrivateProfileString(section, key, "", buf, LW_MAX_NAME, file);
			return parseIntFromBuf(buf);
		};

		param->adapter = readInt("D3DADAPTER", "adapter");
		param->dev_type = (D3DDEVTYPE)readInt("D3DDEVTYPE", "devtype");
		param->behavior_flag = readInt("BEHAVIOR", "behavior");

		// present_param
		param->present_param.Windowed = readInt("PRESENT_PARAM", "windowed");
		param->present_param.BackBufferCount = readInt("PRESENT_PARAM", "backbuffer_count");
		param->present_param.BackBufferFormat = (D3DFORMAT)readInt("PRESENT_PARAM", "backbuffer_format");
		param->present_param.BackBufferHeight = readInt("PRESENT_PARAM", "backbuffer_height");
		param->present_param.BackBufferWidth = readInt("PRESENT_PARAM", "backbuffer_width");
		param->present_param.EnableAutoDepthStencil = readInt("PRESENT_PARAM", "enable_depthstencil");
		param->present_param.AutoDepthStencilFormat = (D3DFORMAT)readInt("PRESENT_PARAM", "depthstencil_format");
		param->present_param.MultiSampleType = (D3DMULTISAMPLE_TYPE)readInt("PRESENT_PARAM", "multisample_type");
		param->present_param.FullScreen_RefreshRateInHz = readInt("PRESENT_PARAM", "refresh_rate");
		param->present_param.PresentationInterval = readInt("PRESENT_PARAM", "present_interval");


		return LW_RET_OK;
	}

	LW_RESULT lwSaveD3DSettings(const char* file, const lwD3DCreateParam* param) {
		auto writeInt = [&](const char* section, const char* key, int value) {
			const std::string s = std::format("{}", value);
			WritePrivateProfileString(section, key, s.c_str(), file);
		};

		writeInt("D3DADAPTER", "adapter", param->adapter);
		writeInt("D3DDEVTYPE", "devtype", param->dev_type);
		writeInt("BEHAVIOR", "behavior", param->behavior_flag);
		// present_param
		writeInt("PRESENT_PARAM", "windowed", param->present_param.Windowed);
		writeInt("PRESENT_PARAM", "backbuffer_count", param->present_param.BackBufferCount);
		writeInt("PRESENT_PARAM", "backbuffer_format", param->present_param.BackBufferFormat);
		writeInt("PRESENT_PARAM", "backbuffer_height", param->present_param.BackBufferHeight);
		writeInt("PRESENT_PARAM", "backbuffer_width", param->present_param.BackBufferWidth);
		writeInt("PRESENT_PARAM", "enable_depthstencil", param->present_param.EnableAutoDepthStencil);
		writeInt("PRESENT_PARAM", "depthstencil_format", param->present_param.AutoDepthStencilFormat);
		writeInt("PRESENT_PARAM", "multisample_type", param->present_param.MultiSampleType);
		writeInt("PRESENT_PARAM", "refresh_rate", param->present_param.FullScreen_RefreshRateInHz);
		writeInt("PRESENT_PARAM", "present_interval", param->present_param.PresentationInterval);

		return LW_RET_OK;
	}


LW_END
