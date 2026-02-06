#pragma once

#include <vector>
#include <memory>
#include "StateContext.hpp"
#include "components/Component.hpp"


namespace Reducord::UI
{
	class Application
	{
	private:
		StateContext context;
		std::vector<std::unique_ptr<IComponent>> components;

	public:
		Application();
		void Render();
	};
}