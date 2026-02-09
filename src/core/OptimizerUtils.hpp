#pragma once

#include <string>

namespace Reducord::Core::Optimizer::Utils {
    bool IsDiscordRunning();
	std::string GetDiscordPath();
	std::string FormattedSize(uintmax_t bytes);
}