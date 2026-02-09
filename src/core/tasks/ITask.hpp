#pragma once

#include "../models/AppStats.hpp"
#include "../Logger.hpp"

namespace Reducord::Core::Optimizer {
    enum class TaskType
	{
		CleanCache,
		CleanLogs,
		CleanVersions,
		HigherPriorityProcess
	};

	class ITask
	{
		public: virtual ~ITask() = default;
		public: virtual void Execute(Models::AppStats &state, Logger::ILogger &logger) = 0;
		public: virtual std::string GetName() const = 0;
	};
}