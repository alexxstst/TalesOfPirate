#pragma once

#include <functional>
#include <string>

#define  WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>

// Установка имени потока через Win32 API (для отладчика)
void SetThreadName(const std::string& name);

namespace TalesOfPirate::Utils::Crush {
	// Настройка пути для дампов при аварийном завершении
	void SetupDumpSetting(const std::string& dumpPath);
	void SetupDumpSetting(const std::string& dumpPath, const std::function<void()>& function);

	// Установка обработчика исключений для текущего потока
	void SetPerThreadCRTExceptionBehavior();
	// Установка глобального обработчика исключений
	void SetGlobalCRTExceptionBehavior();
	// Создание минидампа при исключении
	void CreateMiniDump(EXCEPTION_POINTERS* pep);
	// Фильтр для UnhandledExceptionFilter
	long __stdcall CrushDumpFilter(EXCEPTION_POINTERS* pep);
}
