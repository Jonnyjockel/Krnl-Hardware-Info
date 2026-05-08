#ifndef KRNL_HARDWARE_INFO_APP_HARDWARE_INFO_SERVICE_H_
#define KRNL_HARDWARE_INFO_APP_HARDWARE_INFO_SERVICE_H_

#include "DriverClient.h"

#include <string>

namespace krnl::app {

struct HardwareInfoResult {
    bool succeeded;
    DWORD win32_error;
    KRNL_STATUS_CODE status_code;
    std::string message;
};

struct DriverStatusInfo {
    HardwareInfoResult result;
    KRNL_DRIVER_STATUS_RESPONSE response;
};

struct CpuidInfo {
    HardwareInfoResult result;
    KRNL_CPUID_RESPONSE response;
};

class HardwareInfoService {
public:
    HardwareInfoResult Connect();
    bool IsConnected() const;

    DriverStatusInfo QueryDriverStatus() const;
    CpuidInfo QueryCpuVendor() const;
    CpuidInfo QueryHypervisorPresentBit() const;

private:
    static HardwareInfoResult FromDriverClientStatus(const DriverClientStatus& status);
    CpuidInfo QueryCpuidLeaf(uint32_t leaf, uint32_t subleaf) const;

    DriverClient client_;
};

}  // namespace krnl::app

#endif
