#pragma once

#include "../Optimizer.hpp"

namespace utils = Reducord::Core::Optimizer::Utils;

namespace Reducord::Core::Optimizer {
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
}