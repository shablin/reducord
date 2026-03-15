/**
 * @file Application.hpp
 * @brief Defines the Application class for managing the main UI of the Reducord application.
 * 
 * This file contains the declaration of the Application class, which serves as the main entry point
 * for rendering the UI components of the Reducord application. The Application class holds a StateContext
 * for managing shared application state and a collection of UI components that are rendered in the main loop.
 * 
 * The Application class is responsible for initializing the components and orchestrating their rendering
 * based on the current state of the application.
 * 
 * @author The Reducord Authors
 * @date 2026-03-14
 */


#pragma once

#include <vector>
#include <memory>
#include "StateContext.hpp"
#include "components/Component.hpp"


namespace Reducord::UI
{


	/**
	 * @class Application
	 * 
	 * @brief Main application class responsible
	 * 
	 * @details This class serves as the main entry point
	 * for rendering the UI components of the
	 * Reducord application. It holds a StateContext
	 * for managing shared application state
	 * and a collection of UI components that
	 * are rendered in the main loop.
	 * 
	 * The Application class is responsible for initializing
	 * the components and orchestrating their rendering based
	 * on the current state of the application.
	 */
	class Application
	{
	private:

		/**
		 * @brief Context of the application state.
		 * 
		 * @details Contains shared state and statistics
		 * of the application, as well as a logger
		 * for logging messages across components.
		 */
		StateContext context;
		
		
		/**
		* @brief Collection of UI components to be rendered in the application.
		* 
		* @details Stores polymorphic UI components
		* as unique pointers to the @ref IComponent.
		*/
		std::vector<std::unique_ptr<IComponent>> components;

	public:


		/**
		 * @brief Constructor for the Application class.
		 * 
		 * @details Initializes the application context
		 * and sets up the UI components.
		 */
		Application();


		/**
		 * @brief General render loop for the application.
		 *
		 * @details Calls `Render()` method on
		 * each registered component to render the UI.
		 * This method should be called within the main
		 * rendering loop of the application to ensure
		 * that all components are rendered correctly.
		 */
		void Render();
	};
}