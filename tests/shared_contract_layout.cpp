#include "../src/shared/krnl_contracts.h"

static_assert(KRNL_PROJECT_VERSION_MAJOR == 0, "Unexpected major version.");
static_assert(sizeof(KRNL_DRIVER_STATUS_RESPONSE) == 20, "Driver status response layout changed.");
static_assert(sizeof(KRNL_CPUID_REQUEST) == 8, "CPUID request layout changed.");
static_assert(sizeof(KRNL_CPUID_RESPONSE) == 40, "CPUID response layout changed.");
static_assert(sizeof(KRNL_GENERIC_STATUS_RESPONSE) == 140, "Generic status response layout changed.");
static_assert(IOCTL_KRNL_GET_DRIVER_STATUS != IOCTL_KRNL_QUERY_CPUID, "IOCTL values must be unique.");

int main() {
    return 0;
}
