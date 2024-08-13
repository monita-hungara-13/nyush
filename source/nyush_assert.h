#pragma once

#include "nyush_platform.h"

namespace
{

constexpr nyush::size length(const char* in_string)
{
	const char* p = in_string;
	for (; *p;) ++p;
	return p - in_string;
}

} // namespace

namespace nyush
{

inline bool trap(
	const char* in_message, const char* in_file, [[maybe_unused]] int in_line)
{

	// NOTE: Shittiest code ever?

	constexpr const char* start = "\n\nASSERTION FAILED!!! ";
	constexpr const char* space = ": ";
	constexpr const char* end = "!!!\n\n";
	size written_bytes = 0;
	platform_write_error(start, length(start), &written_bytes);
	written_bytes = 0;
	platform_write_error(in_file, length(in_file), &written_bytes);
	written_bytes = 0;
	platform_write_error(space, length(space), &written_bytes);
	written_bytes = 0;
	platform_write_error(in_message, length(in_message), &written_bytes);
	written_bytes = 0;
	platform_write_error(end, length(end), &written_bytes);
	__debugbreak();
	return true;
}

} // namespace nyush

#define nyush_assert(EXPRESSION)                                               \
	do                                                                         \
	{                                                                          \
		(void)((EXPRESSION) || nyush::trap(#EXPRESSION, __FILE__, __LINE__));  \
	} while (false);
