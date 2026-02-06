#pragma once


namespace Reducord::Core::Models
{
	struct AppStats
	{
		std::atomic<float>		progress{ 0.0f };
		std::atomic<bool>		is_optimizing{ false };

		std::atomic<size_t>		total_files_count{ 0 };
		std::atomic<uintmax_t>	total_size_bytes{ 0 };
		std::atomic<int>		current_step{ 0 };
		std::atomic<int>		total_steps{ 0 };
		std::string				current_step_name;
	};
}