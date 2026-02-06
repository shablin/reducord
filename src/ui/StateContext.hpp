#pragma once

#include <memory>
#include <mutex>
#include <functional>

#include "../core/Logger.hpp"
#include "../core/models/AppStats.hpp"


namespace Reducord::UI
{
	class StateContext
	{
	private:
		Core::Models::AppStats stats;
		mutable std::mutex ctx_mutex;
	public:
		std::shared_ptr<Core::Logger::ILogger> logger;

		bool clean_cache = false;
		bool clean_logs = false;
		bool clean_versions = false;
		bool higher_priority_process = false;

		bool show_about_popup = false;

		StateContext() = default;

		const Core::Models::AppStats& GetStats() const
		{
			return stats;
		}

		Core::Models::AppStats& GetStatsMutable()
		{
			return stats;
		}

		void UpdateStats(std::function<void(Core::Models::AppStats&)> action)
		{
			std::lock_guard<std::mutex> lock(ctx_mutex);
			action(stats);
		}
	};
}