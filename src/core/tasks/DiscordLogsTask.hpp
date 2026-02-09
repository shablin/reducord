#pragma once

#include "ITask.hpp"

namespace Reducord::Core::Optimizer {
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
}