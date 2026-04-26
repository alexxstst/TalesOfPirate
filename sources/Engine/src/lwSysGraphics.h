//
#pragma once

#include "lwHeader.h"
#include "lwInterfaceExt.h"
#include "lwStdInc.h"
#include "lwDirectX.h"
#include "lwClassDecl.h"
#include "lwCoordinateSys.h"

LW_BEGIN
	class lwSysGraphics : public lwISysGraphics {
	public:
		static lwISysGraphics* __sys_graphics;

		static void SetActiveIGraphicsSystem(lwISysGraphics* sys) {
			__sys_graphics = sys;
		}

		static lwISysGraphics* GetActiveIGraphicsSystem() {
			return __sys_graphics;
		}

	private:
		lwSystem* _sys;

		lwIDeviceObject* _dev_obj;
		lwIResourceMgr* _res_mgr;
		lwISceneMgr* _scene_mgr;

		lwCoordinateSys _coord_sys;

		lwOutputLoseDeviceProc _lose_dev_proc;
		lwOutputResetDeviceProc _reset_dev_proc;

		LW_STD_DECLARATION();

	public:
		lwSysGraphics(lwSystem* sys);
		~lwSysGraphics();

		lwISystem* GetSystem() {
			return (lwISystem*)_sys;
		}

		lwIDeviceObject* GetDeviceObject() {
			return _dev_obj;
		}

		lwIResourceMgr* GetResourceMgr() {
			return _res_mgr;
		}

		lwISceneMgr* GetSceneMgr() {
			return _scene_mgr;
		}

		lwICoordinateSys* GetCoordinateSys() {
			return &_coord_sys;
		}

		LW_RESULT CreateDeviceObject(lwIDeviceObject** ret_obj);
		LW_RESULT CreateResourceManager(lwIResourceMgr** ret_obj);
		LW_RESULT CreateSceneManager(lwISceneMgr** ret_obj);

		LW_RESULT ToggleFullScreen(D3DPRESENT_PARAMETERS* d3dpp, lwWndInfo* wnd_info);
		LW_RESULT TestCooperativeLevel();

		void SetOutputLoseDeviceProc(lwOutputLoseDeviceProc proc) {
			_lose_dev_proc = proc;
		}

		void SetOutputResetDeviceProc(lwOutputResetDeviceProc proc) {
			_reset_dev_proc = proc;
		}
	};

LW_END
