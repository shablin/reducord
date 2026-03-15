/**
 * @file ImGuiLogger.hpp
 * @brief Defines the ImGuiLogger class for logging messages in the Reducord UI.
 * 
 * This file contains the declaration of the ImGuiLogger class, which implements
 * the ILogger interface from the core Logger module. The ImGuiLogger is responsible for
 * storing log entries and providing thread-safe access to them for display in the UI.
 * 
 * The class includes methods for logging messages at different levels
 * (Info, Warning, Error, Success) and retrieving the stored log entries.
 * It uses a mutex to ensure thread-safe access to the log entries
 * when multiple threads may be logging messages concurrently.
 * 
 * @author The Reducord Authors
 * @date 2026-03-14
 */


#pragma once

#include "../core/Logger.hpp"
#include <mutex>


namespace Reducord::UI
{


	/**
	 * @class ImGuiLogger
	 * 
	 * @brief ImGuiLogger uses for logging messages
	 * in the Reducord UI with help of the ImGui library.
	 */
	class ImGuiLogger : public Core::Logger::ILogger
	{
	private:


		/**
		 * @brief Internal storage for log entries.
		 * @details This vector holds the log entries
		 * that have been logged through this logger.
		 * Access to this vector should be synchronized
		 * using the @ref log_mutex to ensure thread safety.
		 */
		std::vector<Core::Logger::LogEntry> logs;


		/**
		 * @brief Mutex for synchronizing access to the log entries.
		 * @details Allows multiple threads to log messages concurrently.
		 * Marked as `mutable` to allow locking in `const` methods like GetLogs(),
		 */
		mutable std::mutex log_mutex;


		/**
		 * @brief Internal method for adding a log entry to the logger.
		 * @param level The log level of the message being logged
		 * (Info, Warning, Error, Success).
		 * @param msg The message to log.
		 * 
		 * @details This method is responsible for creating a LogEntry.
		 * This responsible for creating a LogEntry object, capturing
		 * @ref log_mutex and adding the log entry to the vector @ref logs.
		 * 
		 * All public logging methods (Info, Warning, Error, Success)
		 * are wrapped around this internal method.
		 */
		void Write(Core::Logger::LogLevel level, const std::string& msg);
	public:


		/**
		 * @brief Logs an informational message.
		 *
		 * @param message The message to log.
		 * 
		 * @details Uses for logging standard informational
		 * messages that do not indicate any issues or errors.
		 */
		void Info(const std::string &message) override;
		
		
		/**
		 * @brief Logs a warning message.
		 *
		 * @param message The message to log.
		 * 
		 * @details Uses for non-critical issues or potential
		 * problems that do not prevent the application
		 * from functioning but may require attention.
		 */
		void Warning(const std::string& message) override;
		
		
		/**
		 * @brief Logs an error message.
		 * 
		 * @param message The message to log.
		 * 
		 * @details Uses for critical issues or errors that
		 * corrupt the normal flow of the application and may
		 * require immediate attention or intervention.
		 */
		void Error(const std::string& message) override;
		
		
		/**
		* @brief Logs a success message.
		* 
		* @param message The message to log.
		* 
		* @details Uses for successful completion of
		* tasks or operations, indicating that a
		* particular action was completed without issues.
		*/
		void Success(const std::string& message) override;


		/**
		 * @brief Retrieves the stored log entries.
		 * 
		 * @return A vector of LogEntry objects
		 * representing the stored log messages.
		 * 
		 * @note Method is const and thread-safe, and not modifies
		 * the internal state of the logger.
		 */
		std::vector<Core::Logger::LogEntry> GetLogs() const override;


		/**
		* @brief Clears all stored log entries.
		 * 
		 * @details This method removes all log entries
		 * from the logger, effectively resetting the log history.
		*/
		void Clear() override;
	};
}