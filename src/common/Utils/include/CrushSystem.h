#pragma once

#include <functional>
#include <string>

#define  WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>

void SetThreadName(const std::string& name);

namespace TalesOfPirate::Utils::Crush {
	void SetupDumpSetting(const std::string& dumpPath);
	void SetupDumpSetting(const std::string& dumpPath, const std::function<void()>& function);

	void SetPerThreadCRTExceptionBehavior();
	void SetGlobalCRTExceptionBehavior();
	void CreateMiniDump(EXCEPTION_POINTERS* pep);
	long __stdcall CrushDumpFilter(EXCEPTION_POINTERS* pep);
}