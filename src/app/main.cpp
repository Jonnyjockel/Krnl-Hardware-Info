#include "CpuidText.h"
#include "HardwareInfoService.h"
#include "Logger.h"

#include <sstream>

int main() {
    krnl::app::HardwareInfoService hardware_info;

    krnl::app::LogInfo("KRNL Hardware Info console smoke test");

    const krnl::app::HardwareInfoResult open_status = hardware_info.Connect();
    if (!open_status.succeeded) {
        if (open_status.win32_error == ERROR_FILE_NOT_FOUND) {
            krnl::app::LogError("The app launched correctly, but the KRNL driver device was not found.");
            krnl::app::LogError("Win32 error 2 means Windows could not find \\\\.\\KrnlHardwareInfo.");
            krnl::app::LogError("This usually means the driver is not built, installed, or started yet.");
            krnl::app::LogError("Driver testing needs the WDK, test-signing, and the VM workflow from docs/build.md, docs/test-signing.md, and docs/driver-service.md.");
            return 1;
        }

        std::ostringstream message;
        message << open_status.message << " Win32 error: " << open_status.win32_error;
        krnl::app::LogError(message.str());
        return 1;
    }

    const krnl::app::DriverStatusInfo driver_status = hardware_info.QueryDriverStatus();
    if (!driver_status.result.succeeded) {
        std::ostringstream message;
        message << driver_status.result.message << " Win32 error: " << driver_status.result.win32_error;
        krnl::app::LogError(message.str());
        return 1;
    }

    std::ostringstream status_message;
    status_message << "KRNL driver version "
                   << driver_status.response.version_major << '.'
                   << driver_status.response.version_minor << '.'
                   << driver_status.response.version_patch
                   << " loaded=" << static_cast<int>(driver_status.response.driver_loaded);
    krnl::app::LogInfo(status_message.str());

    const krnl::app::CpuidInfo cpu_vendor = hardware_info.QueryCpuVendor();
    if (cpu_vendor.result.succeeded) {
        std::ostringstream cpuid_message;
        cpuid_message << "CPU vendor: " << cpu_vendor.response.vendor_string;
        krnl::app::LogInfo(cpuid_message.str());
        krnl::app::LogInfo("CPUID leaf 0 registers: " + krnl::app::FormatCpuidRegisters(cpu_vendor.response));
    } else {
        std::ostringstream message;
        message << cpu_vendor.result.message << " Win32 error: " << cpu_vendor.result.win32_error;
        krnl::app::LogError(message.str());
    }

    const krnl::app::CpuidInfo hypervisor_bit = hardware_info.QueryHypervisorPresentBit();
    if (hypervisor_bit.result.succeeded) {
        krnl::app::LogInfo(krnl::app::FormatHypervisorPresent(hypervisor_bit.response.hypervisor_present));
        krnl::app::LogInfo("CPUID leaf 1 registers: " + krnl::app::FormatCpuidRegisters(hypervisor_bit.response));
    } else {
        std::ostringstream message;
        message << hypervisor_bit.result.message << " Win32 error: " << hypervisor_bit.result.win32_error;
        krnl::app::LogError(message.str());
    }

    krnl::app::LogInfo(krnl::app::ExplainCpuidRegisters());

    return 0;
}
