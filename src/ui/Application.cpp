#include <iostream>
#include <vector>
#include "components/Component.hpp"
#include "Application.hpp"
#include "ImGuiLogger.hpp"

#include "components/SettingsComponent.hpp"
#include "components/Menubar.hpp"
#include "components/Button.hpp"
#include "components/LogComponent.hpp"
#include "components/ProgressBar.hpp"


namespace Reducord::UI
{
	Application::Application()
	{
		context.logger = std::make_shared<ImGuiLogger>();

		components.push_back(std::make_unique<MenuBar>(context));
		components.push_back(std::make_unique<SettingsComponent>(context));
		components.push_back(std::make_unique<Button>(context));
		components.push_back(std::make_unique<LogComponent>(context));
		components.push_back(std::make_unique<ProgressBar>(context));

		context.logger->Info("Ready. Please choose the options "
							 "(multiple if you want) "
							 "and click button above");
	}

	void Application::Render()
	{
		for (auto& comp : components)
		{
			comp->Render();
		}
	}
}