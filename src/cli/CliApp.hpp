#pragma once

namespace Reducord::Cli::App {
    void setup_terminal();
    void cleanup_terminal();
    int run(int argc, char** argv);
}