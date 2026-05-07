#ifndef KRNL_HARDWARE_INFO_APP_DRIVER_CLIENT_H_
#define KRNL_HARDWARE_INFO_APP_DRIVER_CLIENT_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

#include <string>

#include "../shared/krnl_contracts.h"

namespace krnl::app {

struct DriverClientStatus {
    bool succeeded;
    DWORD win32_error;
    KRNL_STATUS_CODE status_code;
    std::string message;
};

class DriverClient {
public:
    DriverClient();
    ~DriverClient();

    DriverClient(const DriverClient&) = delete;
    DriverClient& operator=(const DriverClient&) = delete;

    DriverClient(DriverClient&& other) noexcept;
    DriverClient& operator=(DriverClient&& other) noexcept;

    DriverClientStatus Open();
    void Close();
    bool IsOpen() const;

    DriverClientStatus QueryDriverStatus(KRNL_DRIVER_STATUS_RESPONSE* response) const;
    DriverClientStatus QueryCpuid(const KRNL_CPUID_REQUEST& request, KRNL_CPUID_RESPONSE* response) const;

private:
    DriverClientStatus SendIoctl(
        DWORD control_code,
        const void* input_buffer,
        DWORD input_size,
        void* output_buffer,
        DWORD output_size,
        DWORD* bytes_returned) const;

    static DriverClientStatus Ok(const std::string& message);
    static DriverClientStatus Fail(KRNL_STATUS_CODE status_code, DWORD win32_error, const std::string& message);

    HANDLE device_handle_;
};

}  // namespace krnl::app

#endif
