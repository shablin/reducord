/**
 * @file Component.hpp
 * @brief Defines the IComponent interface for UI components in the Reducord application.
 *
 * This file contains the declaration of the IComponent interface,
 * which serves as a base class for all UI components in the
 * Reducord application. It provides a common structure for
 * rendering and accessing shared application state through
 * the StateContext.
 *
 * @author The Reducord Authors
 * @date 2026-03-14
 */


#pragma once


 /**
 * @namespace Reducord::UI
 * @brief Contains all UI-related classes and components
 * for the Reducord application.
 */
namespace Reducord::UI
{


	/**
	 * @brief Forward declaration of StateContext to avoid circular dependencies.
	 */
	class StateContext;


	/**
	 * @class IComponent
	 * @brief Interface for UI components in the Reducord application.
	 */
	class IComponent
	{
	protected:


		/**
		* @brief Reference to the state context, allowing components to access and modify shared application state.
		*/
		StateContext &context;
	
	public:


		/**
		* @brief Constructor that initializes the component with a reference to the state context.
		*/
		explicit IComponent(StateContext& ctx) : context(ctx) {}
		

		/**
		* @brief Virtual destructor for proper cleanup of derived classes
		*/
		virtual ~IComponent() = default;
		

		/**
		* @brief Renders the component.
		* This method should be overridden by derived
		* classes to implement specific rendering logic.
		*/
		virtual void Render() = 0;
	};
}