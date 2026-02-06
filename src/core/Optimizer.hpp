#pragma once

#include "Logger.hpp"
#include "./models/AppStats.hpp"


namespace Reducord::Core::Optimizer
{
	enum class TaskType
	{
		CleanCache,
		CleanLogs,
		CleanVersions,
		HigherPriorityProcess
	};

	class ITask
	{
	public:
		virtual ~ITask() = default;
		virtual void Execute(Models::AppStats &state, Logger::ILogger &logger) = 0;
		virtual std::string GetName() const = 0;
	};

	class TaskFactory
	{
	public:
		static std::unique_ptr<ITask> CreateTask(TaskType type);
	};


	class TaskManager
	{
	public:
		static void RunQueue(Models::AppStats &state,
							 Logger::ILogger& logger,
							 const std::vector<TaskType> &tasks);
	};

	namespace Utils
	{
		bool IsDiscordRunning();
		std::string GetDiscordPath();
		std::string FormattedSize(uintmax_t bytes);
	}
}