/**
 * @file StateContext.hpp
 * @brief Defines the StateContext class for managing
 * shared application state in the Reducord UI.
 * 
 * This file contains the declaration of the StateContext class,
 * which serves as a central repository for shared application state
 * in the Reducord UI.
 * 
 * It includes application statistics, logger access,
 * and various flags for controlling optimization tasks
 * and UI behavior.
 
 * The StateContext provides thread-safe access to the application
 * statistics and allows components to update
 * the stats through a provided method.
 * 
 * @author The Reducord Authors
 * @date 2026-03-14
 */


#pragma once

#include <memory>
#include <mutex>
#include <functional>

#include "../core/Logger.hpp"
#include "../core/models/AppStats.hpp"


namespace Reducord::UI
{


	/**
	* @brief Class responsible for managing shared
	* application state in the Reducord UI.
	*/
	class StateContext
	{
	private:

		
		/**
		 * @brief Application statistics, including progress,
		 * files count, and optimization status.
		 * 
		 * @details Stored current stats of the optimization process
		 * Access to this object should be synchronized using @ref ctx_mutex
		 * 
		 * @see UpdateStats()
		 */
		Core::Models::AppStats stats;
		
		
		/**
		 * @brief Mutex for synchronizing access to the application statistics.
		 * 
		 * @details Guarantees thread-safe access when at the same time
		 * multiple components or threads need to read
		 * or modify the stats.
		 * 
		 * @note Marked as `mutable` to allow locking in `const`
		 * methods.
		 */
		mutable std::mutex ctx_mutex;
	public:
		std::shared_ptr<Core::Logger::ILogger> logger;

		bool clean_cache = false;
		bool clean_logs = false;
		bool clean_versions = false;
		bool higher_priority_process = false;

		bool show_about_popup = false;

		StateContext() = default;


		/**
		 * @brief Get access to stats only for reading. Thread-safe.
		 * 
		 * @return A const reference to the @ref Core::Models::AppStats.
		 *
		 * @warning Use this method when you only need
		 * to read the stats without modifying them or use
		 * the @ref UpdateStats method for thread-safe updates.
		 */
		const Core::Models::AppStats& GetStats() const
		{
			return stats;
		}


		/**
		 * @brief Get mutable access to stats.
		 * @return A reference to the @ref Core::Models::AppStats.
		 * 
		 * @note This method bypasses thread-safety @ref ctx_mutes.
		 * Use this method only for quick, non-concurrent updates
		 * to the stats. For thread-safe updates, use
		 * @ref UpdateStats instead.
		 */
		Core::Models::AppStats& GetStatsMutable()
		{
			return stats;
		}


		/**
		 * @brief Thread-safe method for updating application statistics.
		 * 
		 * @details Function blocks mutex @ref ctx_mutex
		 * to ensure that updates to the AppStats object
		 * are thread-safe.
		 * 
		 * @param action A lambda-expression or function that takes
		 * a reference to @ref Core::Models::AppStats.
		 * 
		 * @note Do not execute in @p actuon any long-running operations
		 * or blocking calls
		 */
		void UpdateStats(std::function<void(Core::Models::AppStats&)> action)
		{
			std::lock_guard<std::mutex> lock(ctx_mutex);
			action(stats);
		}
	};
}