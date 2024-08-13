#pragma once

#include "nyush_types.h"

namespace nyush::builtin
{

void change_directory(const char** in_argv, const size in_argc);
void echo(const char** in_argv, const size in_argc);
void list_directory(const char** in_argv, const size in_argc);

void exec(const char* in_command, const char** in_argv, const size in_argc);

} // namespace builtin
