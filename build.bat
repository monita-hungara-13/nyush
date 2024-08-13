@echo off

IF NOT EXIST build\obj\win32 MKDIR build\obj\win32

clang-cl /std:c++20 /EHsc source\build.cpp /Fobuild\obj\win32\ /Febuild.exe
