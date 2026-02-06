#include "ImGuiLogger.hpp"


namespace Reducord::UI
{
	void ImGuiLogger::Write(Core::Logger::LogLevel level,
							const std::string& msg)
	{
		std::lock_guard<std::mutex> lock(log_mutex);
		logs.push_back({ level, msg });
	}

	void ImGuiLogger::Info(const std::string& message)
	{
		Write(Core::Logger::LogLevel::Info, message);
	}

	void ImGuiLogger::Warning(const std::string& message)
	{
		Write(Core::Logger::LogLevel::Warning, message);
	}

	void ImGuiLogger::Error(const std::string& message)
	{
		Write(Core::Logger::LogLevel::Error, message);
	}

	void ImGuiLogger::Success(const std::string& message)
	{
		Write(Core::Logger::LogLevel::Success, message);
	}

	std::vector<Core::Logger::LogEntry> ImGuiLogger::GetLogs() const
	{
		std::lock_guard<std::mutex> lock(log_mutex);
		return logs;
	}

	void ImGuiLogger::Clear()
	{
		std::lock_guard<std::mutex> lock(log_mutex);
		logs.clear();
	}
}