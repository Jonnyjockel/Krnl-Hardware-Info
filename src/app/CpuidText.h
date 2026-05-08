#ifndef KRNL_HARDWARE_INFO_APP_CPUID_TEXT_H_
#define KRNL_HARDWARE_INFO_APP_CPUID_TEXT_H_

#include <string>

#include "../shared/krnl_contracts.h"

namespace krnl::app {

std::string FormatCpuidRegisters(const KRNL_CPUID_RESPONSE& response);
std::string ExplainCpuidRegisters();
std::string FormatHypervisorPresent(uint8_t hypervisor_present);

}  // namespace krnl::app

#endif
