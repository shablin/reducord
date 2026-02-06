#pragma once

#include "./Component.hpp"
#include "../StateContext.hpp"
#include <imgui.h>


namespace Reducord::UI
{
	class ProgressBar : public IComponent
	{
	public:
		using IComponent::IComponent;

		void Render() override
		{
			const auto& stats = context.GetStats();

			if (!stats.is_optimizing && stats.progress <= 0.0f)
				return;

			ImGui::Spacing();
			float current_progress = stats.progress.load();

			char overlay[64];
			sprintf_s(overlay, "%.0f%%", current_progress * 100.0f);

			ImGui::ProgressBar(current_progress, ImVec2(-1, 0), overlay);

			if (stats.is_optimizing)
			{
				ImGui::TextDisabled("Working...");
				ImGui::SameLine();
				ImGui::TextUnformatted(stats.current_step_name.c_str());
			}
			else if (stats.progress >= 1.0f)
			{
				ImGui::TextColored(ImVec4(0, 1, 0, 1), "Done!");
			}
		}
	};
}