#pragma once

#include "Component.hpp"
#include "./core/Optimizer.hpp"
#include <imgui.h>


namespace Reducord::UI
{
	class Button : public IComponent
	{
	public:
		using IComponent::IComponent;

		void Render() override
		{
			const auto& stats = context.GetStats();
			bool is_busy = stats.is_optimizing;

			ImGui::BeginDisabled(is_busy);

			if (ImGui::Button("Optimize", ImVec2(-1, 45)))
			{
				StartOptimizationProcess();
			}

			ImGui::EndDisabled();
		}
	private:
		void StartOptimizationProcess()
		{
			std::vector<Core::Optimizer::TaskType> selected_tasks;

			if (context.clean_cache)
				selected_tasks.push_back(Core::Optimizer::TaskType::CleanCache);
			if (context.clean_logs)
				selected_tasks.push_back(Core::Optimizer::TaskType::CleanLogs);
			if (context.clean_versions)
				selected_tasks.push_back(Core::Optimizer::TaskType::CleanVersions);
			if (context.higher_priority_process)
				selected_tasks.push_back(Core::Optimizer::TaskType::HigherPriorityProcess);

			if (selected_tasks.empty())
			{
				context.logger->Warning("Nothing to do. Please select at least one");
				return;
			}

			std::thread worker([this, selected_tasks]()
			{
				Core::Optimizer::TaskManager::RunQueue(
					context.GetStatsMutable(),
					*context.logger,
					selected_tasks
				);
			});

			worker.detach();
		}
	};
}