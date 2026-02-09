#pragma once

#include <filesystem>
#include "../models/AppStats.hpp"
#include "../OptimizerUtils.hpp"

#include "ITask.hpp"

namespace fs = std::filesystem;

namespace Reducord::Core::Optimizer {
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
						" (" + Utils::FormattedSize(stats.total_size_bytes) + ")"
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
						   " (" + Utils::FormattedSize(stats.total_size_bytes) + ")"
						   " files");
		}
	};
}