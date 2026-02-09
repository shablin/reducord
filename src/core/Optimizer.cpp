#include <windows.h>
#include <tlhelp32.h>
#include <filesystem>
#include "Optimizer.hpp"
#include "tasks/__IncludeDiscord.hpp"
#include "./models/AppStats.hpp"


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

	void TaskManager::RunQueue(Models::AppStats& stats,
							   Logger::ILogger &logger,
							   const std::vector<TaskType> &tasks,
								std::function<void()> callback)
	{
		stats.is_optimizing = true;
		stats.total_steps = static_cast<int>(tasks.size());
		stats.current_step = 0;


		logger.Info("[Running tasks]: " + std::to_string(tasks.size()));

		for (const auto& task : tasks)
		{
			auto task_ = TaskFactory::CreateTask(task);
			if (task_)
			{
				stats.current_step++;
				logger.Info("Stage " + std::to_string(stats.current_step) +
							"/" + std::to_string(stats.total_steps) +
							": " + task_->GetName());
				task_->Execute(stats, logger);
			}
		}

		stats.is_optimizing = false;
		logger.Success("All selected tasks completed");
		callback();
	}
}