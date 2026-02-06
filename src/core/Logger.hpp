#pragma once

#include <string>
#include <vector>


namespace Reducord::Core::Logger
{
	enum class LogLevel
	{
		Info,
		Warning,
		Error,
		Success
	};

	struct LogEntry
	{
		LogLevel level;
		std::string message;
	};


	class ILogger
	{
	public:
		virtual ~ILogger() = default;
		virtual void Info(const std::string &message) = 0;
		virtual void Warning(const std::string &message) = 0;
		virtual void Error(const std::string &message) = 0;
		virtual void Success(const std::string &message) = 0;

		virtual std::vector<LogEntry> GetLogs() const = 0;
		virtual void Clear() = 0;
	};
}