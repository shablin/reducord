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
		void StartOptimizationProcess__OLD()
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

		private: 
		void StartOptimizationProcess() {
			Core::Optimizer::TaskRunner::GetOrCreate()
			.AddTaskConditional(Core::Optimizer::TaskType::CleanCache, context.clean_cache)
			.AddTaskConditional(Core::Optimizer::TaskType::CleanLogs, context.clean_logs)
			.AddTaskConditional(Core::Optimizer::TaskType::CleanVersions, context.clean_versions)
			.AddTaskConditional(Core::Optimizer::TaskType::HigherPriorityProcess, context.higher_priority_process)
			.Flush(context);
		}
	};
}