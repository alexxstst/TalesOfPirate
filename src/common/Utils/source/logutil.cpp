#include <filesystem>
#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
#include <chrono>
#include <stacktrace>
#include "logutil.h"
#include "ConsoleColor.h"
#include "ObjectPools.h"
#include <cstdio>
#include "CrushSystem.h"

#pragma comment(lib, "dbghelp.lib")

namespace TalesOfPirate::Utils::Logs {
	std::mutex _consoleLock{};
	LogManager g_logManager{};

	Logger g_commonLogger{"common"};

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

		_logStream << entry.MessageTime << "|" << entry.LogLevel << " | " << entry.Message;
		if (!entry.Message.empty() && entry.Message[entry.Message.size() - 1] != '\n') {
			_logStream << '\n';
		}
	}

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

	void LogManager::InitLogger(const std::string& filePath) {
		if (!_channels.empty()) {
			throw std::logic_error("Logger is already init!");
		}

		if (!std::filesystem::exists(filePath)) {
			std::filesystem::create_directories(filePath);
		}

		_filePath = std::filesystem::canonical(filePath).string();

		AddLogger("common");
		AddLogger("db");
		AddLogger("network");
		AddLogger("datafile");
		AddLogger("map");
		AddLogger("terrain");
		AddLogger("errors");
		AddLogger("commands");
		AddLogger("offline_system");
		AddLogger("players");
		AddLogger("connections");
		AddLogger("guilds");

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

						std::erase_if(lg.LogSystem,
									  [](auto const& c) -> bool {
										  return !std::isalnum(c) && c != '_' && c != '-';
									  });

						auto it = _channels.find(lg.LogSystem);
						if (it == _channels.cend()) {
							AddLogger(lg.LogSystem);
							continue;
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

	void LogManager::SetLevel(const std::string& logSystem, LogLevel logLevel) {
		_channels.find(logSystem)->second->MinimumLogLevel = logLevel;
	}

	void LogManager::InternalLog(const LogUtilEntry& logEntry) {
		if (logEntry.Message.empty()) {
			return;
		}

		std::scoped_lock lock(_queueMutex);
		_logsQueue.push(logEntry);
	}

	bool LogManager::IsLogLevel(const std::string& logSystem, LogLevel logLevel) {
		const auto it = _channels.find(logSystem);
		if (it == _channels.end()) {
			return true;
		}

		return it->second->MinimumLogLevel >= logLevel;
	}

	void Logger::PrintConsoleMessage(const LogUtilEntry& logEntry) {
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

	void Logger::ToInternalLog(const LogUtilEntry& logEntry) {
		if (_enableConsole) {
			PrintConsoleMessage(logEntry);
		}

		g_logManager.InternalLog(logEntry);
	}
}

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
