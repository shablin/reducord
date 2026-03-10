#pragma once

#include "Component.hpp"
#include "../StateContext.hpp"
#include <imgui.h>
#include <windows.h>


namespace Reducord::UI
{
	class MenuBar : public IComponent
	{
	public:
		using IComponent::IComponent;

		void Render() override
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("GitHub"))
					{
						ShellExecute(nullptr, "open", "https://github.com",
									 nullptr, nullptr, SW_SHOWNORMAL);
					}

					if (ImGui::MenuItem("About"))
					{
						context.show_about_popup = true;
					}

					ImGui::EndMenu();
				}
			}

			ImGui::EndMenuBar();
			RenderAboutPopup();
		}
	private:
		void RenderAboutPopup()
		{
			if (context.show_about_popup)
			{
				ImGui::OpenPopup("About Reducord");
				context.show_about_popup = false;
			}

			if (ImGui::BeginPopupModal("About Reducord", nullptr,
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoMove))
			{
				ImGui::Text("Reducord. Lightweight Discord cleanup\n"
							"utility built with Win32 API & Dear ImGui");
				ImGui::TextDisabled("v0.2.1");
				ImGui::Spacing();

				ImGui::Text("Powered by Maxim Shablin & KCJ");
				ImGui::TextDisabled("GNU GENERAL PUBLIC LICENSE");
				ImGui::Spacing();

				if (ImGui::Button("Close", ImVec2(120, 0)))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
	};
}