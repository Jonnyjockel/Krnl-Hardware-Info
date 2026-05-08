#include "CpuidText.h"

#include <iomanip>
#include <sstream>

namespace krnl::app {

namespace {

void AppendRegister(std::ostringstream& stream, const char* name, uint32_t value) {
    stream << name << "=0x"
           << std::uppercase << std::hex << std::setw(8) << std::setfill('0')
           << value
           << std::nouppercase << std::dec << std::setfill(' ');
}

}  // namespace

std::string FormatCpuidRegisters(const KRNL_CPUID_RESPONSE& response) {
    std::ostringstream stream;
    AppendRegister(stream, "EAX", response.eax);
    stream << ' ';
    AppendRegister(stream, "EBX", response.ebx);
    stream << ' ';
    AppendRegister(stream, "ECX", response.ecx);
    stream << ' ';
    AppendRegister(stream, "EDX", response.edx);
    return stream.str();
}

std::string ExplainCpuidRegisters() {
    return "CPUID returns data in four CPU registers: EAX, EBX, ECX, and EDX. "
           "Think of them as four small CPU answer boxes for the leaf you asked about.";
}

std::string FormatHypervisorPresent(uint8_t hypervisor_present) {
    if (hypervisor_present != 0) {
        return "Hypervisor present bit: set. This is one virtualization signal, not a perfect detector.";
    }

    return "Hypervisor present bit: not set. This does not prove there is no hypervisor.";
}

}  // namespace krnl::app
