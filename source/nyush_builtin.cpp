#include "nyush_builtin.h"

#include "nyush_memory.h"
#include "nyush_platform.h"
#include "nyush_string.h"

void nyush::builtin::change_directory(const char** in_argv, const size in_argc)
{
	if (in_argc == 0)
	{
		platform_change_directory(platform_home_directory());
	}
	else
	{
		platform_change_directory(in_argv[0]);
	}
}

void nyush::builtin::echo(const char** in_argv, const size in_argc)
{
	constexpr size output_size = 1024;
	char output[output_size];
	zero(output, output_size);

	join(in_argv, in_argc, output, output_size, ' ');

	size output_length = length(output);
	size written_bytes = 0;
	platform_write_console(output, output_length, &written_bytes);
	platform_write_console("\n", 1, &written_bytes);
}

void nyush::builtin::list_directory(const char** in_argv, const size in_argc)
{
	constexpr const char* message = "*pending implementation (:* \n";
	size written_bytes = 0;
	platform_write_console(message, length(message), &written_bytes);
}

void nyush::builtin::exec(
	const char* in_command, const char** in_argv, const size in_argc)
{
	if (!platform_create_process(in_command, in_argv, in_argc))
	{
		constexpr const char* message =
			"^^^ is not recognized as a command or an executable program.\n";
		size written_bytes = 0;
		platform_write_console(message, length(message), &written_bytes);
	}
}
