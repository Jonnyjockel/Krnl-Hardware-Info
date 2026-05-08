#include "../src/shared/krnl_contracts.h"

#include <cstddef>
#include <type_traits>

#define KRNL_ASSERT_STANDARD_ABI(TypeName) \
    static_assert(std::is_standard_layout<TypeName>::value, #TypeName " must stay standard-layout."); \
    static_assert(std::is_trivially_copyable<TypeName>::value, #TypeName " must stay trivially copyable.")

static_assert(KRNL_PROJECT_VERSION_MAJOR == 0, "Unexpected major version.");
static_assert(KRNL_PROJECT_VERSION_MINOR == 1, "Unexpected minor version.");
static_assert(KRNL_PROJECT_VERSION_PATCH == 0, "Unexpected patch version.");
static_assert(KRNL_CPU_VENDOR_STRING_LENGTH == 13, "CPU vendor string length changed.");
static_assert(KRNL_STATUS_MESSAGE_LENGTH == 128, "Status message length changed.");

static_assert(sizeof(KRNL_DRIVER_STATUS_RESPONSE) == 20, "Driver status response layout changed.");
static_assert(alignof(KRNL_DRIVER_STATUS_RESPONSE) == 4, "Driver status response alignment changed.");
KRNL_ASSERT_STANDARD_ABI(KRNL_DRIVER_STATUS_RESPONSE);
static_assert(offsetof(KRNL_DRIVER_STATUS_RESPONSE, version_major) == 0, "version_major offset changed.");
static_assert(offsetof(KRNL_DRIVER_STATUS_RESPONSE, version_minor) == 4, "version_minor offset changed.");
static_assert(offsetof(KRNL_DRIVER_STATUS_RESPONSE, version_patch) == 8, "version_patch offset changed.");
static_assert(offsetof(KRNL_DRIVER_STATUS_RESPONSE, status_code) == 12, "status_code offset changed.");
static_assert(offsetof(KRNL_DRIVER_STATUS_RESPONSE, driver_loaded) == 16, "driver_loaded offset changed.");
static_assert(offsetof(KRNL_DRIVER_STATUS_RESPONSE, test_build) == 17, "test_build offset changed.");
static_assert(offsetof(KRNL_DRIVER_STATUS_RESPONSE, reserved) == 18, "reserved offset changed.");

static_assert(sizeof(KRNL_CPUID_REQUEST) == 8, "CPUID request layout changed.");
static_assert(alignof(KRNL_CPUID_REQUEST) == 4, "CPUID request alignment changed.");
KRNL_ASSERT_STANDARD_ABI(KRNL_CPUID_REQUEST);
static_assert(offsetof(KRNL_CPUID_REQUEST, leaf) == 0, "leaf offset changed.");
static_assert(offsetof(KRNL_CPUID_REQUEST, subleaf) == 4, "subleaf offset changed.");

static_assert(sizeof(KRNL_CPUID_RESPONSE) == 40, "CPUID response layout changed.");
static_assert(alignof(KRNL_CPUID_RESPONSE) == 4, "CPUID response alignment changed.");
KRNL_ASSERT_STANDARD_ABI(KRNL_CPUID_RESPONSE);
static_assert(offsetof(KRNL_CPUID_RESPONSE, leaf) == 0, "response leaf offset changed.");
static_assert(offsetof(KRNL_CPUID_RESPONSE, subleaf) == 4, "response subleaf offset changed.");
static_assert(offsetof(KRNL_CPUID_RESPONSE, eax) == 8, "eax offset changed.");
static_assert(offsetof(KRNL_CPUID_RESPONSE, ebx) == 12, "ebx offset changed.");
static_assert(offsetof(KRNL_CPUID_RESPONSE, ecx) == 16, "ecx offset changed.");
static_assert(offsetof(KRNL_CPUID_RESPONSE, edx) == 20, "edx offset changed.");
static_assert(offsetof(KRNL_CPUID_RESPONSE, hypervisor_present) == 24, "hypervisor_present offset changed.");
static_assert(offsetof(KRNL_CPUID_RESPONSE, vendor_string) == 25, "vendor_string offset changed.");
static_assert(offsetof(KRNL_CPUID_RESPONSE, reserved) == 38, "response reserved offset changed.");

static_assert(sizeof(KRNL_GENERIC_STATUS_RESPONSE) == 140, "Generic status response layout changed.");
static_assert(alignof(KRNL_GENERIC_STATUS_RESPONSE) == 4, "Generic status response alignment changed.");
KRNL_ASSERT_STANDARD_ABI(KRNL_GENERIC_STATUS_RESPONSE);
static_assert(offsetof(KRNL_GENERIC_STATUS_RESPONSE, request_type) == 0, "request_type offset changed.");
static_assert(offsetof(KRNL_GENERIC_STATUS_RESPONSE, status_code) == 4, "generic status_code offset changed.");
static_assert(offsetof(KRNL_GENERIC_STATUS_RESPONSE, native_status) == 8, "native_status offset changed.");
static_assert(offsetof(KRNL_GENERIC_STATUS_RESPONSE, message) == 12, "message offset changed.");

static_assert(KrnlRequestTypeUnknown == 0, "Unknown request value changed.");
static_assert(KrnlRequestTypeGetDriverStatus == 1, "Driver status request value changed.");
static_assert(KrnlRequestTypeQueryCpuid == 2, "CPUID request value changed.");
static_assert(KrnlStatusOk == 0, "OK status value changed.");
static_assert(KrnlStatusNotImplemented == 1, "Not implemented status value changed.");
static_assert(KrnlStatusInvalidRequest == 2, "Invalid request status value changed.");
static_assert(KrnlStatusInvalidBuffer == 3, "Invalid buffer status value changed.");
static_assert(KrnlStatusDeviceUnavailable == 4, "Device unavailable status value changed.");
static_assert(KrnlStatusNativeCallFailed == 5, "Native call failed status value changed.");

static_assert(IOCTL_KRNL_GET_DRIVER_STATUS != IOCTL_KRNL_QUERY_CPUID, "IOCTL values must be unique.");

int main() {
    return 0;
}
