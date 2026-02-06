#pragma once

#include "Component.hpp"
#include "../StateContext.hpp"
#include <imgui.h>


namespace Reducord::UI
{
	class SettingsComponent : public IComponent
	{
	public:
		using IComponent::IComponent;

		void Render() override
		{
			const auto& stats = context.GetStats();
			bool is_busy = stats.is_optimizing;

			ImGui::SeparatorText("Strategy");
			ImGui::BeginDisabled(is_busy);

			ImGui::Columns(2, "StrategyGrid", false);
			ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.5f);

			// Left Col
			ImGui::Checkbox("Clean Cache", &context.clean_cache);
			ImGui::SetItemTooltip("Deletes IndexDB, GPU Cache and shaders");

			ImGui::Checkbox("Clean Logs", &context.clean_logs);

			ImGui::NextColumn();

			// Right Col
			ImGui::Checkbox("Clean Versions", &context.clean_versions);
			ImGui::Checkbox("High Priority", &context.higher_priority_process);
			ImGui::SetItemTooltip("Sets Discord priority to high");

			ImGui::Columns(1);

			ImGui::EndDisabled();
			ImGui::Spacing();
		}
	};
}