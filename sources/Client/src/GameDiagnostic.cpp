#include "stdafx.h"
#include "GameDiagnostic.h"

namespace Corsairs::Client::Diagnostic {
	GameDiagnostic& GameDiagnostic::Instance() {
		static GameDiagnostic instance;
		return instance;
	}
} // namespace Corsairs::Client::Diagnostic
