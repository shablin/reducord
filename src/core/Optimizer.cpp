#include <windows.h>
#include <tlhelp32.h>
#include <filesystem>
#include "Optimizer.hpp"
#include "./models/AppStats.hpp"
#include "./tasks/Tasks.hpp"


namespace fs = std::filesystem;
namespace Reducord::Core::Optimizer
{
	bool Utils::IsDiscordRunning()
	{
		bool exists = false;
		PROCESSENTRY32W entry;
		entry.dwSize = sizeof(PROCESSENTRY32W);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32FirstW(snapshot, &entry))
		{
			do {
				if (_wcsicmp(entry.szExeFile, L"Discord.exe") == 0)
				{
					exists = true;
					break;
				}
			} while (Process32NextW(snapshot, &entry));
			CloseHandle(snapshot);
		}
		return exists;
	}

	std::string Optimizer::Utils::GetDiscordPath()
	{
		char* appdata = nullptr;
		size_t env_len = 0;
		
		if (_dupenv_s(&appdata, &env_len, "APPDATA") == 0 && appdata != nullptr)
		{
			fs::path discord_path = fs::path(appdata) / "discord";
			free(appdata);
			return discord_path.string();
		}

		return "";
	}

	std::string Optimizer::Utils::FormattedSize(uintmax_t bytes)
	{
		const char* suffixes[] = { "B", "KB", "MB", "GB", "TB" };
		int suffix_idx = 0;
		double dbytes = static_cast<double>(bytes);

		while (dbytes >= 1024.0 && suffix_idx < 4)
		{
			dbytes /= 1024.0;
			suffix_idx++;
		}

		std::stringstream ss;
		ss << std::fixed << std::setprecision(2)
			<< dbytes << " "
			<< suffixes[suffix_idx];
		return ss.str();
	}

	std::unique_ptr<ITask> TaskFactory::CreateTask(TaskType type)
	{
		if (type <= TaskType::CF_CNT) {
			throw ERROR_ILLEGAL_CHARACTER;
		}
		switch (type)
		{
		case TaskType::CleanCache:
			return std::make_unique<DiscordCacheTask>();
		case TaskType::CleanLogs:
			return std::make_unique<DiscordLogsTask>();
		case TaskType::CleanVersions:
			return std::make_unique<DiscordVersionsTask>();
		case TaskType::HigherPriorityProcess:
			return std::make_unique<DiscordHighPriority>();
		default:
			return nullptr;
		}
	}

	void TaskManager::RunQueue(Models::AppStats& stats,
							   Logger::ILogger &logger,
							   const std::vector<TaskType> &tasks) 
	{
		RunQueue(stats, logger, tasks, [](){});
	}

	size_t _CountTasks(const std::vector<TaskType> &tasks) {
		size_t r = tasks.size();
		for (const auto task : tasks) {
			if (task <= TaskType::CF_CNT) r--;
		}
		return r;
	}

	void _RunTask(Models::AppStats& stats, Logger::ILogger &logger, TaskType tt) {
		auto task = TaskFactory::CreateTask(tt);
		if (task) {
			stats.current_step++;
			logger.Info(
				"Stage " + std::to_string(stats.current_step) +
				"/" + std::to_string(stats.total_steps) +
				": " + task->GetName()
			);
			task->Execute(stats, logger);
		}
	}

	void TaskManager::RunQueue(Models::AppStats& stats,
							   Logger::ILogger &logger,
							   const std::vector<TaskType> &tasks,
								std::function<void()> callback)
	{
		auto tasksCnt = _CountTasks(tasks);
		stats.is_optimizing = true;
		stats.total_steps = static_cast<int>(tasksCnt);
		stats.current_step = 0;


		logger.Info("[Running tasks]: " + std::to_string(tasksCnt));
		bool doMulti = false;
		std::vector<TaskType> multi;

		for (const auto& task : tasks)
		{
			if (task == TaskType::CF_ParallelStart) {
				doMulti = true;
				continue;
			}

			if (task == TaskType::CF_ParallelEnd) {
				doMulti = false;
				Logger::ILogger* logger_ctx = &logger;
				Models::AppStats* stats_ctx = &stats;
				
				std::vector<std::thread> workers(4);
				for (const auto& taskM : multi) {
					std::thread worker(
						[logger_ctx, stats_ctx, taskM](){
							_RunTask(*stats_ctx, *logger_ctx, taskM);
						}
					);
					workers.push_back(move(worker));
				}
				for (auto& worker : workers) if (worker.joinable()) worker.join();
				multi.clear();
				continue;
			}

			if (doMulti) {
				multi.push_back(task);
				continue;
			}

			_RunTask(stats, logger, task);
		}

		stats.is_optimizing = false;
		logger.Success("All selected tasks completed");
		callback();
	}
}