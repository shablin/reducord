/**
 * @file CliApp.hpp
 * @brief Declares the main functions for the CLI application of Reducord.
 * 
 * @author The Reducord Authors
 * @date 2026-03-15
 */


#pragma once

namespace Reducord::Cli::App {


    /**
	 * @brief Sets up the terminal for the CLI application.
     */
    void setup_terminal();
    
    
    /**
	 * @brief Cleans up the terminal after the CLI application has finished executing.
     */
    void cleanup_terminal();


    /**
	 * @brief Main entry point for the CLI application.
     */
    int run(int argc, char** argv);
}