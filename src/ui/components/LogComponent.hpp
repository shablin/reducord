/**
 * @file LogComponent.hpp
 * @brief Defines the LogComponent class for displaying application logs in the Reducord UI.
 * 
 * This file contains the declaration of the LogComponent class, which is responsible for
 * rendering the application logs in a scrollable child window. It retrieves log entries from
 * the StateContext's logger and displays them with color coding based on log levels.
 * 
 * @author The Reducord Authors
 * @date 2026-03-14
 */


#pragma once

#include "Component.hpp"
#include "../StateContext.hpp"
#include "../../core/Logger.hpp"
#include <imgui.h>


/**
 * @namespace Reducord::UI
 * @brief Contains all UI-related classes and components for the Reducord application.
 */
namespace Reducord::UI
{
	

	/**
	 * @brief Component responsible for rendering application logs in the UI.
	 * 
	 * The LogComponent retrieves log entries from the StateContext's logger and displays them
	 * in a scrollable child window. Each log entry is color-coded based on its log level
	 * (Info, Warning, Error, Success).
	 */
	class LogComponent : public IComponent
	{
	public:


		/**
		 * @brief Constructor that initializes the LogComponent with a reference to the state context.
		 * 
		 * @param ctx Reference to the StateContext, allowing access to the logger and other shared state.
		 * @note The constructor uses the base class constructor to initialize the context reference.
		 */
		using IComponent::IComponent;


		/**
		 * @brief Renders the log entries in a scrollable child window.
		 */
		void Render() override
		{
			ImGui::SeparatorText("Details");

			if (ImGui::BeginChild("LogComponent", ImVec2(0, -45),
				ImGuiChildFlags_Border), true)
			{
				auto logs = context.logger->GetLogs();

				for (const auto& log : logs)
				{
					ImVec4 color = GetColorForLevel(log.level);
					std::string preffix = LogPreffixByLevel(log.level);
					ImGui::PushTextWrapPos();
					ImGui::TextColored(color, "[%s] %s",
									   preffix.c_str(),
									   log.message.c_str());
					ImGui::PopTextWrapPos();
				}

				if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				{
					ImGui::SetScrollHereY(1.0f);
				}
			}
			ImGui::EndChild();
		}
	private:


		/**
		 * @brief Returns the color associated with a given log level.
		 * @param level The log level for which to retrieve the color.
		 *
		 * @return An ImVec4 representing the color for the specified log level
		 */
		ImVec4 GetColorForLevel(Core::Logger::LogLevel level)
		{
			switch (level)
			{
			case Core::Logger::LogLevel::Info:		return ImVec4(0, 1, 1, 1);
			case Core::Logger::LogLevel::Warning:	return ImVec4(1, 1, 0, 1);
			case Core::Logger::LogLevel::Error:		return ImVec4(1, 0, 0, 1);
			case Core::Logger::LogLevel::Success:	return ImVec4(0, 1, 0, 1);
			default:								return ImVec4(1, 1, 1, 1);
			}
		}


		/**
		 * @brief Returns a string prefix for a given log level.
		 * @param level The log level for which to retrieve the prefix.
		 *
		 * @return A string representing the log level
		 * (e.g., "INFO", "WARN", "ERROR", "OK").
		 */
		std::string LogPreffixByLevel(Core::Logger::LogLevel level)
		{
			switch (level)
			{
				case Core::Logger::LogLevel::Info:		return "INFO";
				case Core::Logger::LogLevel::Warning:	return "WARN";
				case Core::Logger::LogLevel::Error:		return "ERROR";
				case Core::Logger::LogLevel::Success:	return "OK";
				default:								return "UNK";
			}
		}
	};
}