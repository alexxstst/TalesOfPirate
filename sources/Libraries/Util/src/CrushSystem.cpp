//      .
//        .

#include "CrushSystem.h"

#include <tchar.h>
#include <dbghelp.h>
#include <filesystem>
#include <functional>
#include <iostream>
#include <stacktrace>
#include <windows.h>

static std::string g_dumpFilePath{};
static std::optional<std::function<void()>> g_crushFunction{};
static std::atomic<std::int32_t> incValue{};

namespace TalesOfPirate::Utils::Crush {
	long __stdcall CrushDumpFilter(EXCEPTION_POINTERS* pep) {
		CreateMiniDump(pep);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	//       ,   
	void CreateMiniDump(EXCEPTION_POINTERS* pep) {
		SYSTEMTIME st{};
		GetLocalTime(&st);
		std::string buffer = std::format("dump_{}_{}.{}.{}_{}.{}.{}_rnd{}.dmp", ::GetCurrentProcessId(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, ++incValue);

		auto dumpFilePath = (std::filesystem::path(g_dumpFilePath) / std::filesystem::path(buffer)).string();
		HANDLE hFile = CreateFile(dumpFilePath.c_str(), GENERIC_READ | GENERIC_WRITE,
								  0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if ((hFile != nullptr) && (hFile != INVALID_HANDLE_VALUE)) {
			MINIDUMP_EXCEPTION_INFORMATION mdei{};

			mdei.ThreadId = GetCurrentThreadId();
			mdei.ExceptionPointers = pep;
			mdei.ClientPointers = FALSE;

			MINIDUMP_TYPE mdt = static_cast<MINIDUMP_TYPE>(MiniDumpWithHandleData
				| MiniDumpWithFullMemory
				| MiniDumpWithFullMemoryInfo
				| MiniDumpWithThreadInfo
				| MiniDumpWithTokenInformation
				| MiniDumpWithModuleHeaders);

			const auto rv = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
											  hFile, mdt, (pep != nullptr) ? &mdei : nullptr, nullptr, nullptr);

			CloseHandle(hFile);

			std::cout << std::stacktrace::current() << std::endl;
			if (!rv) {
				std::cout << "MiniDumpWriteDump failed. Error: " << ::GetLastError() << std::endl;
				std::cout << "Press key to exit..." << std::endl;
				getchar();
				return;
			}

			std::cout << "Save minidump to " << dumpFilePath << std::endl;

			if (g_crushFunction.has_value()) {
				g_crushFunction.value()();
			}

			std::cout << "Press key to exit..." << std::endl;
			getchar();

			return;
		}

		std::cout << "Failed create file " << dumpFilePath << " error:" << ::GetLastError() << std::endl;
		std::cout << "Press key to exit..." << std::endl;
		getchar();
	}

	void SetPerThreadCRTExceptionBehavior() {
		SetUnhandledExceptionFilter(CrushDumpFilter);
	}

	void SetGlobalCRTExceptionBehavior() {
	}


	void SetupDumpSetting(const std::string& dumpPath) {
		if (!std::filesystem::exists(dumpPath)) {
			std::filesystem::create_directories(dumpPath);
		}

		g_dumpFilePath = std::filesystem::canonical(dumpPath).string();
	}

	void SetupDumpSetting(const std::string& dumpPath, const std::function<void()>& function) {
		SetupDumpSetting(dumpPath);
		g_crushFunction = function;
	}
}

typedef HRESULT (__stdcall *SET_THREAD_NAME)(HANDLE, PCWSTR);

//     SetThreadDescription (Windows 10+)
void SetThreadName(const std::string& name) {
	const std::wstring wsTmp(name.begin(), name.end());
	const auto hinstLib = LoadLibrary(TEXT("KernelBase.dll"));
	if (hinstLib != nullptr) {
		const auto threadProc = reinterpret_cast<SET_THREAD_NAME>(GetProcAddress(hinstLib, "SetThreadDescription"));
		if (nullptr != threadProc) {
			(threadProc)(GetCurrentThread(), wsTmp.c_str());
		}
	}
}
