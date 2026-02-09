#pragma once
#include "../core/Logger.hpp"
#include <mutex>

namespace Reducord::Cli {
    class CliLogger : public Reducord::Core::Logger::ILogger {
		private: std::vector<Core::Logger::LogEntry> logs;
		private: mutable std::mutex log_mutex;

		public: void Write(Core::Logger::LogLevel level, const std::string& msg);
		public: void Info(const std::string &message) override;
		public: void Warning(const std::string& message) override;
		public: void Error(const std::string& message) override;
		public: void Success(const std::string& message) override;

		public: std::vector<Core::Logger::LogEntry> GetLogs() const override;
		public: void Clear() override;
    };
}