#include "nyush.h"

#include "nyush_builtin.h"
#include "nyush_memory.h"
#include "nyush_platform.h"
#include "nyush_sequences.h"
#include "nyush_string.h"

int nyush::loop()
{
	do
	{
		// Write prompt
		{
			constexpr const size current_dir_size = 1024;
			char current_dir[current_dir_size];
			platform_current_directory(current_dir, current_dir_size);

			const char* home_dir = platform_home_directory();
			const size home_dir_length = length(home_dir);

			nyush::buffer<1024> directory;
			directory.append(current_dir);
			const char* final_buffer = directory;
			const int32 index = directory.find(home_dir);
			if (index >= 0)
			{
				const size replace_index = index + home_dir_length - 1;
				directory[replace_index] = '~';
				final_buffer = directory + replace_index;
			}

			const size buffer_length = length(final_buffer);

			size written_bytes = 0;
			platform_write_console(final_buffer, buffer_length, &written_bytes);

			nyush::buffer<1024> prompt;
			prompt.append(NYUSH_VTS_FOREGROUND_GREEN
				" -> " NYUSH_VTS_FOREGROUND_DEFAULT NYUSH_VTS_SAVE_CURSOR
					NYUSH_VTS_CURSOR_HORIZONTAL_ABSOLUTE(32767));
			prompt.append(NYUSH_VTS_CURSOR_BACKWARD(18));
			prompt.append(
				NYUSH_VTS_FOREGROUND_BLACK NYUSH_VTS_BACKGROUND_BRIGHT_WHITE
				" 08/12/24 01:24:57 " NYUSH_VTS_FOREGROUND_DEFAULT
					NYUSH_VTS_BACKGROUND_DEFAULT NYUSH_VTS_RESTORE_CURSOR);

			written_bytes = 0;
			platform_write_console(prompt, prompt.length(), &written_bytes);
		}

		constexpr const size buffer_size = 1024;
		char buffer[buffer_size];
		size read_bytes = 0;
		zero(buffer, buffer_size);
		platform_read_console(buffer, buffer_size, &read_bytes);

		const char* tokens[512];
		size tokens_count = 0;

		// Get first token.
		char* next = nullptr;
		tokens[tokens_count++] = tokenize(buffer, " \r\n", next);
		if (tokens[0] == nullptr)
		{
			// Probably empty?, do nothing.
			continue;
		}

		// Get the following tokens, if any.
		while (const char* token = tokenize(nullptr, " \r\n", next))
		{
			tokens[tokens_count++] = token;
		}

		const char* command = tokens[0];
		const char** argv = tokens + 1;
		const size argc = tokens_count - 1;

		if (equals(command, ""))
		{
			continue;
		}

		if (equals(command, "cd"))
		{
			builtin::change_directory(argv, argc);
		}
		else if (equals(command, "echo"))
		{
			builtin::echo(argv, argc);
		}
		else if (equals(command, "ls"))
		{
			builtin::list_directory(argv, argc);
		}
		else if (equals(command, "exit"))
		{
			break;
		}
		else
		{
			builtin::exec(command, argv, argc);
		}

	} while (true);

	return 0;
}
