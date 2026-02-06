#include <windows.h>
#include <tlhelp32.h>
#include <filesystem>
#include "Optimizer.hpp"
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

	class DiscordCacheTask : public ITask
	{
	public:
		std::string GetName() const override
		{
			return "Clean Discord Cache";
		}

		void Execute(Models::AppStats& stats, Logger::ILogger& logger) override
		{
			stats.total_files_count = 0;
			stats.total_size_bytes = 0;
			stats.progress = 0.0f;

			if (Utils::IsDiscordRunning())
			{
				logger.Error("Discord is currently running. Please close it first");
				return;
			}

			std::string ds_path = Utils::GetDiscordPath();
			if (ds_path.empty())
			{
				logger.Error("Could not determine APPDATA dir");
				return;
			}

			fs::path discord_path(ds_path);
			if (!fs::exists(discord_path))
			{
				logger.Error("Discord dir not found at: " + ds_path);
				return;
			}

			logger.Info("Calculating cache...");

			std::vector<std::string> target_dirs = {
				"Cache", "Code Cache",
				"GPUCache", "DawnCache",
				"DawnGraphiteCache",
				"DawnWebGPUCache"
			};

			std::vector<fs::path> files_to_delete;

			for (const auto& dir : target_dirs)
			{
				fs::path p = discord_path / dir;
				if (fs::exists(p))
				{
					try
					{
						for (const auto &file : fs::recursive_directory_iterator(p))
						{
							if (file.is_regular_file())
							{
								stats.total_size_bytes += file.file_size();
								stats.total_files_count++;
								files_to_delete.push_back(file.path());
							}
						}
					}
					catch (...) { continue; }
				}
			}

			if (files_to_delete.empty())
			{
				logger.Success("Cache is already clean");
				stats.progress = 1.0f;
				return;
			}

			logger.Info("Found " + std::to_string(stats.total_files_count) +
						"(" + Utils::FormattedSize(stats.total_size_bytes) + ")"
						" files");

			size_t deleted = 0;
			size_t total = files_to_delete.size();

			for (const auto& file : files_to_delete)
			{
				try
				{
					if (fs::remove(file))
						deleted++;

					stats.progress = static_cast<float>(deleted) /
									 static_cast<float>(total);
				} catch (const fs::filesystem_error&) { }
			}

			logger.Success("Cleanup finished. Removed " + std::to_string(deleted) +
						   "(" + Utils::FormattedSize(total) + ")" " files");
		}
	};

	class DiscordLogsTask : public ITask
	{
	public:
		std::string GetName() const override
		{
			return "Clean Logs";
		}

		void Execute(Models::AppStats& stats, Core::Logger::ILogger& logger) override
		{
			logger.Info("Not implemented yet...");
		}
	};

	class DiscordVersionsTask : public ITask
	{
	public:
		std::string GetName() const override
		{
			return "Clean Versions";
		}

		void Execute(Models::AppStats& stats, Core::Logger::ILogger& logger) override
		{
			logger.Info("Not implemented yet...");
		}
	};

	class DiscordHighPriority : public ITask
	{
	public:
		std::string GetName() const override
		{
			return "High Priority Process";
		}

		void Execute(Models::AppStats& stats, Core::Logger::ILogger& logger) override
		{
			logger.Info("Not implemented yet...");
		}
	};

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
		stats.is_optimizing = true;
		stats.total_steps = static_cast<int>(tasks.size());
		stats.current_step = 0;

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
	}
}