#pragma once


namespace Reducord::UI
{
	class StateContext;

	class IComponent
	{
	protected:
		StateContext &context;
	
	public:
		explicit IComponent(StateContext& ctx) : context(ctx) {}
		virtual ~IComponent() = default;
		virtual void Render() = 0;
	};
}