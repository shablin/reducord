#pragma once
#include "../core/Logger.hpp"
#include <mutex>


namespace Reducord::UI
{
	class ImGuiLogger : public Core::Logger::ILogger
	{
	private:
		std::vector<Core::Logger::LogEntry> logs;
		mutable std::mutex log_mutex;
		void Write(Core::Logger::LogLevel level, const std::string& msg);
	public:
		void Info(const std::string &message) override;
		void Warning(const std::string& message) override;
		void Error(const std::string& message) override;
		void Success(const std::string& message) override;

		std::vector<Core::Logger::LogEntry> GetLogs() const override;
		void Clear() override;
	};
}