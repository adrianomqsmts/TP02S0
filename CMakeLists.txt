# cmake_minimum_required(VERSION <specify CMake version here>)
project(TP02SO C)

set(CMAKE_C_STANDARD 99)

add_executable(TP02SO main.c ProcessCommander.c ProcessCommander.h ProcessManager.c ProcessManager.h Process.c Process.h ProcessReporter.c ProcessReporter.h)