#include <filesystem>
#include <fstream>
#include <iostream>

int main(int in_argc, char* in_argv[])
{
	const std::string obj_path = "build\\obj\\win32\\";
	const std::string bin_path = "build\\bin\\win32\\";

	// Create folders if they don't exists.
	if (!std::filesystem::exists(obj_path))
	{
		std::filesystem::create_directories(obj_path);
	}
	if (!std::filesystem::exists(bin_path))
	{
		std::filesystem::create_directories(bin_path);
	}

	// Get files to build from source folder.
	std::cout << "Searching files..." << std::endl;
	const std::string path = "source/";
	std::vector<std::string> files;
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() == ".cpp" &&
			entry.path().filename() != "build.cpp")
		{
			std::cout << "Found: " << entry.path().string() << std::endl;
			files.push_back(entry.path().string());
		}
	}

	std::string all_files = files[0];
	for (size_t i = 1; i < files.size(); ++i)
	{
		all_files = all_files + " " + files[i];
	}

	std::string compiler_switches =
		"/std:c++20 /nologo /GS- /GR- /EHa- /Oi "
		"/Gs9999999 /Zc:threadSafeInit- /Od /Zi /Isource\\ /DNOMINMAX";

	std::string command =
		std::string("cmd.exe /c  && clang-cl ") + compiler_switches + " " +
		all_files + " /Fo" + obj_path + " /Fe" + bin_path + "nyush.exe /Fd" +
		bin_path +
		" /link /NOLOGO /NODEFAULTLIB /DEBUG /STACK:0x100000,0x100000 "
		"/INCREMENTAL:NO /SUBSYSTEM:CONSOLE /ENTRY:platform_entry "
		"user32.lib kernel32.lib userenv.lib advapi32.lib";

	std::cout << "Building..." << std::endl;
	const int result = std::system(command.c_str());
	if (result != 0)
	{
		return result;
	}

	std::string current_path = std::filesystem::current_path().string();
	std::replace(current_path.begin(), current_path.end(), '\\', '/');
	std::replace(compiler_switches.begin(), compiler_switches.end(), '\\', '/');

	std::cout << "Generating compile commands..." << std::endl;

	std::ofstream compile_commands;
	compile_commands.open("compile_commands.json");

	compile_commands << "[" << std::endl;
	for (std::size_t i = 0; i < files.size(); ++i)
	{
		std::string file = files[i];
		std::replace(file.begin(), file.end(), '\\', '/');

		compile_commands << "\t{" << std::endl;
		compile_commands << "\t\t\"command\": \"cl " << compiler_switches
						 << "\"," << std::endl;
		compile_commands << "\t\t\"directory\": \"" << current_path << "\","
						 << std::endl;
		compile_commands << "\t\t\"file\": \"" << file << "\"" << std::endl;

		if (i != files.size() - 1) [[likely]]
		{
			compile_commands << "\t}," << std::endl;
		}
		else [[unlikely]]
		{
			compile_commands << "\t}" << std::endl;
		}
	}
	compile_commands << "]" << std::endl;

	compile_commands.close();

	return 0;
}
