#ifndef KRNL_HARDWARE_INFO_APP_LOGGER_H_
#define KRNL_HARDWARE_INFO_APP_LOGGER_H_

#include <string>

namespace krnl::app {

void LogInfo(const std::string& message);
void LogError(const std::string& message);

}  // namespace krnl::app

#endif
