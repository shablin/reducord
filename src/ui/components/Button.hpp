/**
 * @file Button.hpp
 * @brief Defines the Button component for triggering optimization tasks in the Reducord UI.
 * 
 * This file contains the declaration of the Button class, which is responsible for rendering
 * an "Optimize" button in the Reducord UI. When clicked, the button initiates
 * the optimization process based on the selected tasks in the StateContext.
 * The button is disabled while an optimization process is ongoing.
 * 
 * @author The Reducord Authors
 * @date 2026-03-14
 */


#pragma once

#include "Component.hpp"
#include "./core/Optimizer.hpp"
#include <imgui.h>


namespace Reducord::UI
{
	/**
	 * @class Button
	 * 
	 * @brief Component responsible for rendering an "Optimize" button in the Reducord UI.
	 * 
	 * The Button component renders a button that, when clicked, initiates the optimization process
	 * based on the selected tasks in the StateContext. The button is disabled while an optimization
	 * process is ongoing to prevent multiple concurrent optimizations.
	 */
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


		/**
		 * @brief Initiates the optimization process based
		 * on the selected tasks in the StateContext.
		 */
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


		/**
		 * @brief Initiates the optimization process using the TaskRunner,
		 * which manages task execution and progress updates.
		 * 
		 * The TaskRunner allows for conditional task addition based
		 * on the StateContext's settings and handles the execution
		 * flow of the optimization tasks.
		 * 
		 * This method simplifies the initiation of the optimization process
		 * by leveraging the TaskRunner's capabilities to manage tasks
		 * and update application state accordingly.
		 * 
		 * It ensures that only the selected tasks are executed and that progress
		 * is tracked and logged effectively.
		 */
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