#pragma once

// Система логирования на основе LogManager.
// Использует std::format (C++20), потоковую запись в файлы и цветной вывод в консоль.

#define  WIN32_LEAN_AND_MEAN

#include <format>
#include <fstream>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <windows.h>

namespace TalesOfPirate::Utils::Logs {
	// Уровни логирования — от самого детального до критического
	enum class LogLevel {
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};

	// Запись лога с меткой времени, уровнем и именем подсистемы
	struct LogUtilEntry {
		char MessageTime[23]{};
		std::string Message{};
		LogLevel LogLevel{LogLevel::Debug};
		std::string LogSystem{};

		LogUtilEntry();
	};

	// Файловый поток для одного канала логирования.
	// Автоматически ротирует файлы при смене даты.
	class LogStream {
		std::ofstream _logStream{};
		std::string _path{};
		std::string _logSystem{};
		SYSTEMTIME _currentSystemTime{};

		[[nodiscard]] std::string GenerateFileName() const;

	public:
		LogStream(const std::string& path, const std::string& logSystem);
		~LogStream();

		void Write(const LogUtilEntry& entry, const SYSTEMTIME& sysTime);
		void Flush();
		LogLevel MinimumLogLevel{LogLevel::Trace};
	};

	// Менеджер логирования: управляет каналами, очередью сообщений и фоновым потоком записи.
	// Все сообщения дублируются в канал "common".
	// При включённой консоли — выводит цветные сообщения в stdout.
	class LogManager {
		std::map<std::string, std::shared_ptr<LogStream>> _channels{};
		std::mutex _queueMutex{};
		std::queue<LogUtilEntry> _logsQueue{};
		std::thread _logThread{};
		bool _stopped{};
		std::string _filePath{};
		bool _enabledGlobalConsole;

		// Вывод сообщения в консоль с цветовой раскраской по уровню
		static void PrintConsoleMessage(const LogUtilEntry& logEntry);

	public:
		LogManager();
		~LogManager();

		void InitLogger(const std::string& path);
		bool AddLogger(const std::string& loggerName);
		void SetLevel(const std::string& logSystem, LogLevel logLevel);
		bool IsLogLevel(const std::string& logSystem, LogLevel logLevel);
		void EnableGlobalConsole(bool status);

		// Путь к каталогу логов
		const std::string& GetLogDirectory() const { return _filePath; }

		void Shutdown();
		void
		InternalLog(LogLevel logLevel, const std::string& logSystem, const std::string& value);
		void InternalLog(const LogUtilEntry& logEntry);

		template <class... _Types>
		void Log(LogLevel logLevel, const std::string& logSystem,
				 std::format_string<std::remove_cvref_t<_Types>...> _Fmt, _Types&&... _Args) {
			InternalLog(logLevel, logSystem, vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}

		template <class... _Types>
		void LogTrace(const std::string& logSystem, std::format_string<std::remove_cvref_t<_Types>...> _Fmt,
					  _Types&&... _Args) {
			InternalLog(LogLevel::Trace, logSystem, vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}

		template <class... _Types>
		void LogDebug(const std::string& logSystem, std::format_string<std::remove_cvref_t<_Types>...> _Fmt,
					  _Types&&... _Args) {
			InternalLog(LogLevel::Debug, logSystem, vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}

		template <class... _Types>
		void LogInfo(const std::string& logSystem, std::format_string<std::remove_cvref_t<_Types>...> _Fmt,
					 _Types&&... _Args) {
			InternalLog(LogLevel::Info, logSystem, vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}

		template <class... _Types>
		void LogWarning(const std::string& logSystem, std::format_string<std::remove_cvref_t<_Types>...> _Fmt,
						_Types&&... _Args) {
			InternalLog(LogLevel::Warning, logSystem, vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}

		template <class... _Types>
		void LogError(const std::string& logSystem, std::format_string<std::remove_cvref_t<_Types>...> _Fmt,
					  _Types&&... _Args) {
			InternalLog(LogLevel::Error, logSystem, vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}

		template <class... _Types>
		void LogFatal(const std::string& logSystem, std::format_string<std::remove_cvref_t<_Types>...> _Fmt,
					  _Types&&... _Args) {
			InternalLog(LogLevel::Fatal, logSystem, vformat(_Fmt.get(), std::make_format_args(_Args...)));
		}
	};

	// Глобальный экземпляр менеджера логов
	extern LogManager g_logManager;

	// Быстрый лог в канал "common" на уровне Debug
	template <class... _Types>
	void ToLog(std::format_string<std::remove_cvref_t<_Types>...> _Fmt, _Types&&... _Args) {
		g_logManager.InternalLog(LogLevel::Debug, "common", vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}

	// Быстрый лог в произвольный канал на уровне Debug
	template <class... _Types>
	void ToLogService(const std::string& logger, std::format_string<std::remove_cvref_t<_Types>...> _Fmt,
					  _Types&&... _Args) {
		g_logManager.InternalLog(LogLevel::Debug, logger, vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}

	// Быстрый лог в произвольный канал с указанием уровня
	template <class... _Types>
	void ToLogService(const std::string& logger, LogLevel level,
					  std::format_string<std::remove_cvref_t<_Types>...> _Fmt, _Types&&... _Args) {
		g_logManager.InternalLog(level, logger, vformat(_Fmt.get(), std::make_format_args(_Args...)));
	}
}

// Оператор вывода LogLevel в ostream (для записи в файл)
std::ostream& operator<<(std::ostream& stream, const TalesOfPirate::Utils::Logs::LogLevel& io);

// Специализация std::formatter для LogLevel (для std::format)
template <>
struct std::formatter<TalesOfPirate::Utils::Logs::LogLevel> : std::formatter<std::string> {
	template <class format_context>
	auto format(const TalesOfPirate::Utils::Logs::LogLevel& io, format_context& ctx) const {
		switch (io) {
		case TalesOfPirate::Utils::Logs::LogLevel::Trace:
			return formatter<string>::format("Trace", ctx);
		case TalesOfPirate::Utils::Logs::LogLevel::Debug:
			return formatter<string>::format("Debug", ctx);
		case TalesOfPirate::Utils::Logs::LogLevel::Info:
			return formatter<string>::format("Info", ctx);
		case TalesOfPirate::Utils::Logs::LogLevel::Warning:
			return formatter<string>::format("Warning", ctx);
		case TalesOfPirate::Utils::Logs::LogLevel::Error:
			return formatter<string>::format("Error", ctx);
		case TalesOfPirate::Utils::Logs::LogLevel::Fatal:
			return formatter<string>::format("Fatal", ctx);
		}

		return formatter<string>::format("UNKNOWN", ctx);
	}
};

using namespace TalesOfPirate::Utils::Logs;
