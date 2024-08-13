#pragma once

#include "nyush_types.h"

namespace nyush
{

void platform_current_directory(
	char* out_path_buffer, const size in_path_buffer_size);

bool platform_change_directory(const char* in_path);

const char* platform_home_directory();

bool platform_read_console(
	char* out_buffer, const size in_chars_to_read, size* out_chars_read);

void platform_write_console(const char* in_buffer, const size in_chars_to_write,
	size* out_chars_written);

void platform_write_error(const char* in_buffer, const size in_chars_to_write,
	size* out_chars_written);

[[nodiscard]] ivec2 platform_get_window_size();

bool platform_create_process(
	const char* in_executable, const char** in_argv, const size in_argc);

} // namespace nyush
