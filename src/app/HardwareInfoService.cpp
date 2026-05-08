#include "HardwareInfoService.h"

namespace krnl::app {

HardwareInfoResult HardwareInfoService::Connect() {
    return FromDriverClientStatus(client_.Open());
}

bool HardwareInfoService::IsConnected() const {
    return client_.IsOpen();
}

DriverStatusInfo HardwareInfoService::QueryDriverStatus() const {
    DriverStatusInfo info = {};
    info.result = FromDriverClientStatus(client_.QueryDriverStatus(&info.response));
    return info;
}

CpuidInfo HardwareInfoService::QueryCpuVendor() const {
    return QueryCpuidLeaf(0, 0);
}

CpuidInfo HardwareInfoService::QueryHypervisorPresentBit() const {
    return QueryCpuidLeaf(1, 0);
}

HardwareInfoResult HardwareInfoService::FromDriverClientStatus(const DriverClientStatus& status) {
    return HardwareInfoResult{
        status.succeeded,
        status.win32_error,
        status.status_code,
        status.message};
}

CpuidInfo HardwareInfoService::QueryCpuidLeaf(uint32_t leaf, uint32_t subleaf) const {
    KRNL_CPUID_REQUEST request = {};
    request.leaf = leaf;
    request.subleaf = subleaf;

    CpuidInfo info = {};
    info.result = FromDriverClientStatus(client_.QueryCpuid(request, &info.response));
    return info;
}

}  // namespace krnl::app
