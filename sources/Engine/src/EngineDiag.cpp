#include "stdafx.h"
#include "EngineDiag.h"

namespace Corsairs::Engine::Diagnostic {
	EngineDiag& EngineDiag::Instance() {
		static EngineDiag instance;
		return instance;
	}
} // namespace Corsairs::Engine::Diagnostic
