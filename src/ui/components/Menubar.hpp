/**
 * @file MenuBar.hpp
 * @brief Defines the MenuBar component for the Reducord UI.
 * 
 * This file contains the declaration of the MenuBar class,
 * which is responsible for rendering the menu bar in the
 * Reducord application.
 * 
 * @author The Reducord Authors
 * @date 2026-03-14
 */


#pragma once

#include "Component.hpp"
#include "../StateContext.hpp"
#include <imgui.h>
#include <windows.h>
#include "version.h"


namespace Reducord::UI
{
	/**
	 * @class MenuBar
	 * 
	 * @brief Component responsible for rendering
	 * the menu bar in the Reducord UI.
	 */
	class MenuBar : public IComponent
	{
	public:


		/**
		 * @brief Constructor that initializes the MenuBar
		 * with a reference to the state context.
		 * 
		 * @param ctx Reference to the StateContext,
		 * allowing access to shared application state.
		 * 
		 * @note The constructor uses the base class constructor
		 * to initialize the context reference.
		 */
		using IComponent::IComponent;


		/**
		* @brief Renders the menu bar with a "Help" menu containing links to GitHub and an About popup.
		 * The "Help" menu provides options for users to access the GitHub repository and view information about the application.
		 * When the "GitHub" menu item is clicked, it opens the default web browser to the specified GitHub URL.
		 * When the "About" menu item is clicked, it triggers the display of an About popup with application information.
		 * The method also calls RenderAboutPopup() to handle the rendering of the About popup if it is triggered.
		 * 
		 * @note This method should be called within the main rendering loop of the application to ensure that the menu bar is displayed correctly.
		*/
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


		/**
		 * @brief Renders the About popup modal when triggered.
		 * 
		 * This method checks if the show_about_popup flag in the context is set to true.
		 * If it is, it opens a modal popup with information about
		 * the Reducord application, including its purpose, version, authors, and license.
		 * The popup also includes a "Close" button that allows users
		 * to close the popup when clicked.
		 * 
		 * @note This method should be called within the main rendering loop
		 * of the application to ensure that the About popup
		 * is displayed correctly when triggered.
		 */
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
				ImGui::TextDisabled("v" APP_VERSION_STR);
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