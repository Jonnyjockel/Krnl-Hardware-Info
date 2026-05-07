#include "Logger.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <iostream>

namespace krnl::app {

namespace {

void DebugLine(const std::string& level, const std::string& message) {
    const std::string line = "[KRNL][" + level + "] " + message + "\n";
    OutputDebugStringA(line.c_str());
}

}  // namespace

void LogInfo(const std::string& message) {
    DebugLine("INFO", message);
    std::cout << message << '\n';
}

void LogError(const std::string& message) {
    DebugLine("ERROR", message);
    std::cerr << message << '\n';
}

}  // namespace krnl::app
