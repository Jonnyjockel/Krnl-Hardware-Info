#include "DriverClient.h"

#include <utility>

namespace krnl::app {

DriverClient::DriverClient()
    : device_handle_(INVALID_HANDLE_VALUE) {
}

DriverClient::~DriverClient() {
    Close();
}

DriverClient::DriverClient(DriverClient&& other) noexcept
    : device_handle_(std::exchange(other.device_handle_, INVALID_HANDLE_VALUE)) {
}

DriverClient& DriverClient::operator=(DriverClient&& other) noexcept {
    if (this != &other) {
        Close();
        device_handle_ = std::exchange(other.device_handle_, INVALID_HANDLE_VALUE);
    }

    return *this;
}

DriverClientStatus DriverClient::Open() {
    if (IsOpen()) {
        return Ok("Driver device is already open.");
    }

    device_handle_ = CreateFileW(
        KRNL_USER_DEVICE_PATH,
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (!IsOpen()) {
        const DWORD error = GetLastError();
        return Fail(KrnlStatusDeviceUnavailable, error, "Failed to open KRNL driver device.");
    }

    return Ok("Driver device opened.");
}

void DriverClient::Close() {
    if (IsOpen()) {
        CloseHandle(device_handle_);
        device_handle_ = INVALID_HANDLE_VALUE;
    }
}

bool DriverClient::IsOpen() const {
    return device_handle_ != nullptr && device_handle_ != INVALID_HANDLE_VALUE;
}

DriverClientStatus DriverClient::QueryDriverStatus(KRNL_DRIVER_STATUS_RESPONSE* response) const {
    if (response == nullptr) {
        return Fail(KrnlStatusInvalidBuffer, ERROR_INVALID_PARAMETER, "QueryDriverStatus response cannot be null.");
    }

    DWORD bytes_returned = 0;
    return SendIoctl(
        IOCTL_KRNL_GET_DRIVER_STATUS,
        nullptr,
        0,
        response,
        static_cast<DWORD>(sizeof(*response)),
        &bytes_returned);
}

DriverClientStatus DriverClient::QueryCpuid(const KRNL_CPUID_REQUEST& request, KRNL_CPUID_RESPONSE* response) const {
    if (response == nullptr) {
        return Fail(KrnlStatusInvalidBuffer, ERROR_INVALID_PARAMETER, "QueryCpuid response cannot be null.");
    }

    /*
     * This method only forwards a CPUID request to the driver. Any future
     * hypervisor trace analysis should live behind separate, explicit APIs.
     */
    DWORD bytes_returned = 0;
    return SendIoctl(
        IOCTL_KRNL_QUERY_CPUID,
        &request,
        static_cast<DWORD>(sizeof(request)),
        response,
        static_cast<DWORD>(sizeof(*response)),
        &bytes_returned);
}

DriverClientStatus DriverClient::SendIoctl(
    DWORD control_code,
    const void* input_buffer,
    DWORD input_size,
    void* output_buffer,
    DWORD output_size,
    DWORD* bytes_returned) const {
    if (!IsOpen()) {
        return Fail(KrnlStatusDeviceUnavailable, ERROR_INVALID_HANDLE, "Driver device is not open.");
    }

    const BOOL ok = DeviceIoControl(
        device_handle_,
        control_code,
        const_cast<void*>(input_buffer),
        input_size,
        output_buffer,
        output_size,
        bytes_returned,
        nullptr);

    if (!ok) {
        const DWORD error = GetLastError();
        return Fail(KrnlStatusNativeCallFailed, error, "DeviceIoControl failed.");
    }

    return Ok("DeviceIoControl succeeded.");
}

DriverClientStatus DriverClient::Ok(const std::string& message) {
    return DriverClientStatus{true, ERROR_SUCCESS, KrnlStatusOk, message};
}

DriverClientStatus DriverClient::Fail(KRNL_STATUS_CODE status_code, DWORD win32_error, const std::string& message) {
    return DriverClientStatus{false, win32_error, status_code, message};
}

}  // namespace krnl::app
