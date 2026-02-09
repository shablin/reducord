#include "CliApp.hpp"
#include <Windows.h>
#include <cstdio>

#include "../ui/StateContext.hpp"
#include "../core/Optimizer.hpp"
#include "CliLogger.hpp"
#include "flags.h"

void press_enter() {
    INPUT ip = {0};
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = VK_RETURN;
    ip.ki.time = 1;
    SendInput(1, &ip, sizeof(INPUT));
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

#pragma region Callbacks

void flag_clean_cache() {
    Reducord::Core::Optimizer::TaskRunner::GetOrCreate()
    .AddTask(Reducord::Core::Optimizer::TaskType::CleanCache);
}
void flag_clean_logs() {
    Reducord::Core::Optimizer::TaskRunner::GetOrCreate()
    .AddTask(Reducord::Core::Optimizer::TaskType::CleanLogs);
}
void flag_clean_versions() {
    Reducord::Core::Optimizer::TaskRunner::GetOrCreate()
    .AddTask(Reducord::Core::Optimizer::TaskType::CleanVersions);
}
void flag_high_priority() {
    Reducord::Core::Optimizer::TaskRunner::GetOrCreate()
    .AddTask(Reducord::Core::Optimizer::TaskType::HigherPriorityProcess);
}
void flag_help() {
    printf(
        "Usage:" "\n"
        "\t" "reducord [flags...]" "\n"
        "Flags:" "\n"
        "\t" "--help" "\n"
        "\t" "--clean-cache" "\n"
        "\t" "--clean-logs" "\n"
        "\t" "--clean-versions" "\n"
        "\t" "--high-priority" "\n"
    );
}

void flag_unknown(char* flag) {
    printf("Unknown flag: %s\n", flag);
    flag_help();
}

#pragma endregion

namespace Reducord::Cli::App {

    void setup_terminal() {
        AttachConsole(ATTACH_PARENT_PROCESS);
        freopen("CONOUT$", "w", stdout);
        printf("\x1b[2K\x1b[0E");
    }

    void cleanup_terminal() {
        press_enter();
        freopen("NUL", "w", stdout);
        FreeConsole();
    }

    int run(int argc, char** argv) {
        Reducord::UI::StateContext ctx;
        ctx.logger = std::make_shared<Reducord::Cli::CliLogger>();

        flag_handler_t flags = {0};
        flags_mk_handler(&flags);

        flags_add_flag(&flags, FLAG_HELP, (char*)"--help", &flag_help);

        flags_add_flag(&flags, FLAG_CLEAN_CACHE, (char*)"--clean-cache", &flag_clean_cache);
        flags_add_flag(&flags, FLAG_CLEAN_LOGS, (char*)"--clean-logs", &flag_clean_logs);
        flags_add_flag(&flags, FLAG_CLEAN_VERSIONS, (char*)"--clean-versions", &flag_clean_versions);
        flags_add_flag(&flags, FLAG_HIGH_PRIORITY, (char*)"--high-priority", &flag_high_priority);

        flags_execute_unk(&flags, argc, argv, &flag_unknown);

        Reducord::Core::Optimizer::TaskRunner::GetOrCreate()
        .FlushEx(ctx, false);
        return 0;
    }
}