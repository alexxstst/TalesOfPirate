//    LogManager.
//           + .

#include <filesystem>
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
#include <chrono>
#include <stacktrace>
#include "logutil.h"
#include "ConsoleColor.h"
#include <cstdio>
#include "CrushSystem.h"

#pragma comment(lib, "dbghelp.lib")

namespace TalesOfPirate::Utils::Logs {
	//         
	std::mutex _consoleLock{};

	//    
	LogManager g_logManager{};

	LogStream::LogStream(const std::string& path, const std::string& logSystem) : _logSystem(logSystem) {
		if (!std::filesystem::exists(path)) {
			std::filesystem::create_directories(path);
		}

		_path = std::filesystem::canonical(path).string();
		GetSystemTime(&_currentSystemTime);
	}

	LogStream::~LogStream() {
		if (_logStream.is_open()) {
			_logStream.flush();
		}
	}

	//     ;      
	void LogStream::Write(const LogUtilEntry& entry, const SYSTEMTIME& sysTime) {
		if (sysTime.wYear > _currentSystemTime.wYear || sysTime.wMonth > _currentSystemTime.wMonth ||
			sysTime.wDay > _currentSystemTime.wDay) {
			if (_logStream.is_open()) {
				_logStream.flush();
				_logStream.close();
			}

			_currentSystemTime = sysTime;
		}

		if (!_logStream.is_open()) {
			_logStream = std::ofstream(_path + '\\' + GenerateFileName(), std::ios_base::out | std::ios_base::app);
			_logStream << "START NEW LOGGER SESSION" << '\n';
		}

		_logStream << entry.LogSystem << "|" << entry.MessageTime << "|" << entry.LogLevel << " | " << entry.Message;
		if (!entry.Message.empty() && entry.Message[entry.Message.size() - 1] != '\n') {
			_logStream << '\n';
		}
	}

	//     "system_2026_03_27.log"
	std::string LogStream::GenerateFileName() const {
		SYSTEMTIME st;
		GetLocalTime(&st);
		auto data = std::format("{}_{}_{:02}_{:02}.log", _logSystem, st.wYear, st.wMonth, st.wDay);
		std::transform(data.begin(), data.end(), data.begin(),
					   [](unsigned char c) {
						   return std::tolower(c);
					   });
		return data;
	}

	void LogStream::Flush() {
		if (_logStream.is_open()) {
			_logStream.flush();
		}
	}

	//  :    
	LogUtilEntry::LogUtilEntry() {
		SYSTEMTIME st;
		GetLocalTime(&st);
		*std::format_to(MessageTime, "{:04}{:02}.{:02} {:02}:{:02}:{:02}.{:03}", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds) = '\0';
	}

	void LogManager::InternalLog(LogLevel logLevel, const std::string& logSystem,
								 const std::string& value) {
		LogUtilEntry logEntry{};
		logEntry.LogLevel = logLevel;
		logEntry.LogSystem = logSystem;
		logEntry.Message = value;

		InternalLog(logEntry);
	}

	bool LogManager::AddLogger(const std::string& loggerName) {
		if (_channels.contains(loggerName)) {
			return false;
		}

		_channels.emplace(std::pair(loggerName, std::make_shared<LogStream>(_filePath, loggerName)));
		return true;
	}

	// Archive old log files into a timestamped subdirectory
	static void ArchiveOldLogs(const std::string& logDir) {
		namespace fs = std::filesystem;

		if (!fs::exists(logDir) || fs::is_empty(logDir)) {
			return;
		}

		// Check if there are any .log files to archive
		bool hasLogs = false;
		for (const auto& entry : fs::directory_iterator(logDir)) {
			if (entry.is_regular_file() && entry.path().extension() == ".log") {
				hasLogs = true;
				break;
			}
		}

		if (!hasLogs) {
			return;
		}

		// Generate archive directory name: logs.DD.MM.YYYY.HH.MM.SS
		SYSTEMTIME st;
		GetLocalTime(&st);
		auto archiveName = std::format("logs.{:02}.{:02}.{:04}.{:02}.{:02}.{:02}",
			st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

		fs::path archivePath = fs::path(logDir) / archiveName;
		fs::create_directories(archivePath);

		// Move all .log files to the archive directory
		for (const auto& entry : fs::directory_iterator(logDir)) {
			if (entry.is_regular_file() && entry.path().extension() == ".log") {
				auto dest = archivePath / entry.path().filename();
				std::error_code ec;
				fs::rename(entry.path(), dest, ec);
			}
		}
	}

	void LogManager::InitLogger(const std::string& filePath) {
		if (!_channels.empty()) {
			throw std::logic_error("Logger is already init!");
		}

		if (!std::filesystem::exists(filePath)) {
			std::filesystem::create_directories(filePath);
		}

		_filePath = std::filesystem::canonical(filePath).string();

		// Archive old logs before starting new session
		ArchiveOldLogs(_filePath);

		// Create default log channels
		AddLogger("common");
		AddLogger("network");
		AddLogger("connections");
		AddLogger("db");
		AddLogger("map");
		AddLogger("errors");
		AddLogger("commands");
		AddLogger("players");
		AddLogger("guilds");
		AddLogger("trade");
		AddLogger("offline");
		AddLogger("lua");
		AddLogger("store");
		AddLogger("ui");
		AddLogger("terrain");

		//  :       
		_logThread = std::thread([this]() {
			Crush::SetPerThreadCRTExceptionBehavior();
			::SetThreadName("logger");

			std::int32_t _dumpCounter{};
			while (!_stopped) {
				LogUtilEntry lg{};
				SYSTEMTIME lt{};
				GetSystemTime(&lt);

				{
					std::scoped_lock lock(_queueMutex);
					while (!_logsQueue.empty()) {
						lg = _logsQueue.front();

						//      
						std::erase_if(lg.LogSystem,
									  [](auto const& c) -> bool {
										  return !std::isalnum(c) && c != '_' && c != '-';
									  });

						auto it = _channels.find(lg.LogSystem);
						if (it == _channels.cend()) {
							AddLogger(lg.LogSystem);
							continue;
						}

						//   ,    
						if (_enabledGlobalConsole) {
							PrintConsoleMessage(lg);
						}

						//      "common"
						if (lg.LogSystem != "common") {
							_channels["common"]->Write(lg, lt);
						}

						it->second->Write(lg, lt);
						_logsQueue.pop();
					}
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				++_dumpCounter;
				if (_dumpCounter % 100) {
					for (auto& pair : _channels) {
						pair.second->Flush();
					}
				}
			}

			std::cout << "Exit logger thread..." << '\n';
		});
	}

	void LogManager::Shutdown() {
		_stopped = true;
		if (_logThread.joinable()) {
			_logThread.join();
		}

		_channels.clear();
	}

	LogManager::LogManager() {
	}

	LogManager::~LogManager() {
		Shutdown();
	}

	//     ()
	void LogManager::InternalLog(const LogUtilEntry& logEntry) {
		if (logEntry.Message.empty()) {
			return;
		}

		std::scoped_lock lock(_queueMutex);
		_logsQueue.push(logEntry);
	}

	void LogManager::EnableGlobalConsole(bool status) {
		_enabledGlobalConsole = status;
	}

	//         
	void LogManager::PrintConsoleMessage(const LogUtilEntry& logEntry) {
		std::scoped_lock lock(_consoleLock);

		const auto inputLine = std::format("{} |{}| {}", logEntry.LogLevel, logEntry.LogSystem, logEntry.Message);
		switch (logEntry.LogLevel) {
		case LogLevel::Trace:
			std::cout << inputLine << '\n';
			break;

		case LogLevel::Debug:
			std::cout << concolor::green(inputLine) << '\n';
			break;

		case LogLevel::Info:
			std::cout << concolor::aqua(inputLine) << '\n';
			break;

		case LogLevel::Warning:
			std::cout << concolor::light_yellow(inputLine) << '\n';
			break;

		case LogLevel::Error:
			std::cout << concolor::light_red(inputLine) << '\n';
			break;

		case LogLevel::Fatal:
			std::cout << concolor::red(inputLine) << '\n';
			break;
		}
	}
}

//   LogLevel  ostream
std::ostream& operator<<(std::ostream& stream, const LogLevel& io) {
	switch (io) {
	case LogLevel::Trace:
		stream << "Trace\t";
		break;

	case LogLevel::Debug:
		stream << "Debug\t";
		break;

	case LogLevel::Info:
		stream << "Info\t";
		break;
	case LogLevel::Warning:
		stream << "Warning\t";
		break;
	case LogLevel::Error:
		stream << "Error\t";
		break;
	case LogLevel::Fatal:
		stream << "Fatal\t";
		break;
	}

	return stream;
}
