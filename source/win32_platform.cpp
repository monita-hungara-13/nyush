#include "nyush_platform.h"

#include "nyush_string.h"
#include "nyush_runtime.h"
#include <userenv.h>
#include <windows.h>

void nyush::platform_current_directory(
	char* out_path_buffer, const size in_path_buffer_size)
{
	// TODO: Return checks.
	GetCurrentDirectoryA(
		static_cast<DWORD>(in_path_buffer_size), out_path_buffer);
}

bool nyush::platform_change_directory(const char* in_path)
{
	return SetCurrentDirectoryA(in_path);
}

const char* nyush::platform_home_directory()
{
	static const char* result = nullptr;
	static char buffer[MAX_PATH];

	if (!result)
	{
		HANDLE current_process = GetCurrentProcess();
		HANDLE process_token;
		if (OpenProcessToken(current_process, TOKEN_QUERY, &process_token))
		{
			DWORD size = MAX_PATH;
			if (GetUserProfileDirectoryA(process_token, buffer, &size))
			{
				result = buffer;
			}

			CloseHandle(process_token);
		}
	}

	return result;
}

bool nyush::platform_read_console(
	char* out_buffer, const size in_chars_to_read, size* out_chars_read)
{
	// NOTE: Not sure if static handles are a good idea, will do for
	// now.
	static HANDLE std_in = GetStdHandle(STD_INPUT_HANDLE);

	return ReadConsoleA(std_in, out_buffer,
		static_cast<DWORD>(in_chars_to_read),
		reinterpret_cast<DWORD*>(out_chars_read), NULL);
}

void nyush::platform_write_console(const char* in_buffer,
	const size in_chars_to_write, size* out_chars_written)
{
	// NOTE: Not sure if static handles are a good idea, will do for
	// now.
	static HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);

	WriteConsoleA(std_out, in_buffer, static_cast<DWORD>(in_chars_to_write),
		reinterpret_cast<DWORD*>(out_chars_written), NULL);
}

void nyush::platform_write_error(const char* in_buffer,
	const size in_chars_to_write, size* out_chars_written)
{
	static HANDLE std_error = GetStdHandle(STD_ERROR_HANDLE);

	WriteConsoleA(std_error, in_buffer, static_cast<DWORD>(in_chars_to_write),
		reinterpret_cast<DWORD*>(out_chars_written), NULL);
}

nyush::ivec2 nyush::platform_get_window_size()
{
	HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
	GetConsoleScreenBufferInfo(std_out, &ScreenBufferInfo);
	ivec2 size;
	size.x =
		ScreenBufferInfo.srWindow.Right - ScreenBufferInfo.srWindow.Left + 1;
	size.y =
		ScreenBufferInfo.srWindow.Bottom - ScreenBufferInfo.srWindow.Top + 1;

	return size;
}

bool nyush::platform_create_process(
	const char* in_executable, const char** in_argv, const size in_argc)
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);

	constexpr size cmd_size = 1024;
	char cmd[cmd_size];
	copy(cmd, in_executable, cmd_size);
	if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL,
			&siStartInfo, &piProcInfo))
	{
		return false;
	}

	WaitForSingleObject(piProcInfo.hProcess, INFINITE);

	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);

	return true;
}
