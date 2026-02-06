#pragma once

#include "Component.hpp"
#include "../StateContext.hpp"
#include "../../core/Logger.hpp"
#include <imgui.h>


namespace Reducord::UI
{
	class LogComponent : public IComponent
	{
	public:
		using IComponent::IComponent;

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