#include "DriverClient.h"
#include "Logger.h"

#include <sstream>

int main() {
    krnl::app::DriverClient client;

    const krnl::app::DriverClientStatus open_status = client.Open();
    if (!open_status.succeeded) {
        std::ostringstream message;
        message << open_status.message << " Win32 error: " << open_status.win32_error;
        krnl::app::LogError(message.str());
        return 1;
    }

    KRNL_DRIVER_STATUS_RESPONSE driver_status = {};
    const krnl::app::DriverClientStatus status_result = client.QueryDriverStatus(&driver_status);
    if (!status_result.succeeded) {
        std::ostringstream message;
        message << status_result.message << " Win32 error: " << status_result.win32_error;
        krnl::app::LogError(message.str());
        return 1;
    }

    std::ostringstream status_message;
    status_message << "KRNL driver version "
                   << driver_status.version_major << '.'
                   << driver_status.version_minor << '.'
                   << driver_status.version_patch
                   << " loaded=" << static_cast<int>(driver_status.driver_loaded);
    krnl::app::LogInfo(status_message.str());

    KRNL_CPUID_REQUEST cpuid_request = {};
    cpuid_request.leaf = 0;
    cpuid_request.subleaf = 0;

    KRNL_CPUID_RESPONSE cpuid_response = {};
    const krnl::app::DriverClientStatus cpuid_result = client.QueryCpuid(cpuid_request, &cpuid_response);
    if (cpuid_result.succeeded) {
        std::ostringstream cpuid_message;
        cpuid_message << "CPU vendor: " << cpuid_response.vendor_string;
        krnl::app::LogInfo(cpuid_message.str());
    } else {
        std::ostringstream message;
        message << cpuid_result.message << " Win32 error: " << cpuid_result.win32_error;
        krnl::app::LogError(message.str());
    }

    return 0;
}
