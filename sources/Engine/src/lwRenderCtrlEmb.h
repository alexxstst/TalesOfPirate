#pragma once

#include "lwHeader.h"
#include "lwStdInc.h"
#include "lwErrorCode.h"
#include "lwDirectX.h"
#include "lwITypes.h"
#include "lwInterface.h"

LW_BEGIN
	LW_RESULT lwInitInternalRenderCtrlVSProc(lwIResourceMgr* mgr);

	class lwRenderCtrlVSFixedFunction : public lwIRenderCtrlVS {
		typedef lwRenderCtrlVSFixedFunction this_type;

		LW_STD_DECLARATION();

	public:
		DWORD GetType() {
			return RENDERCTRL_VS_FIXEDFUNCTION;
		}

		LW_RESULT Clone(lwIRenderCtrlVS** obj);
		LW_RESULT Initialize(lwIRenderCtrlAgent* agent);
		LW_RESULT BeginSet(lwIRenderCtrlAgent* agent);
		LW_RESULT EndSet(lwIRenderCtrlAgent* agent);
		LW_RESULT BeginSetSubset(DWORD subset, lwIRenderCtrlAgent* agent);
		LW_RESULT EndSetSubset(DWORD subset, lwIRenderCtrlAgent* agent);
	};


LW_END
