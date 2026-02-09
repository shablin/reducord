#include "CliLogger.hpp"

namespace Reducord::Cli {

	std::string __LogLevelName(Reducord::Core::Logger::LogLevel lvl);
	std::string __LogLevelColor(Reducord::Core::Logger::LogLevel lvl);

    void CliLogger::Write(Core::Logger::LogLevel level,
							const std::string& msg)
	{
		std::lock_guard<std::mutex> lock(log_mutex);
		logs.push_back({ level, msg });
        printf(
            "%s[%s] : %s\x1b[0m\n", 
            __LogLevelColor(level).c_str(), 
            __LogLevelName(level).c_str(), 
            msg.c_str()
        );
	}

	void CliLogger::Info(const std::string& message)
	{
		Write(Core::Logger::LogLevel::Info, message);
	}

	void CliLogger::Warning(const std::string& message)
	{
		Write(Core::Logger::LogLevel::Warning, message);
	}

	void CliLogger::Error(const std::string& message)
	{
		Write(Core::Logger::LogLevel::Error, message);
	}

	void CliLogger::Success(const std::string& message)
	{
		Write(Core::Logger::LogLevel::Success, message);
	}

	std::vector<Core::Logger::LogEntry> CliLogger::GetLogs() const
	{
		std::lock_guard<std::mutex> lock(log_mutex);
		return logs;
	}

	void CliLogger::Clear()
	{
		std::lock_guard<std::mutex> lock(log_mutex);
		logs.clear();
	}

    std::string __LogLevelName(Reducord::Core::Logger::LogLevel lvl) {
        switch(lvl) {
            case Reducord::Core::Logger::LogLevel::Success: return "SUC";
            case Reducord::Core::Logger::LogLevel::Error: return "ERR";
            case Reducord::Core::Logger::LogLevel::Info: return "INF";
            case Reducord::Core::Logger::LogLevel::Warning: return "WRN";
            default: return "UNK";
        }
    }


    std::string __LogLevelColor(Reducord::Core::Logger::LogLevel lvl) {
        switch(lvl) {
            case Reducord::Core::Logger::LogLevel::Success: return "\x1b[1;32m";
            case Reducord::Core::Logger::LogLevel::Error: return "\x1b[1;31m";
            case Reducord::Core::Logger::LogLevel::Info: return "\x1b[1;36m";
            case Reducord::Core::Logger::LogLevel::Warning: return "\x1b[1;33m";
            default: return "\x1b[1;39m";
        }
    }
}