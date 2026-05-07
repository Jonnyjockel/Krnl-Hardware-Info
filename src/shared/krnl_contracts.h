#ifndef KRNL_HARDWARE_INFO_CONTRACTS_H_
#define KRNL_HARDWARE_INFO_CONTRACTS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KRNL_PROJECT_VERSION_MAJOR 0u
#define KRNL_PROJECT_VERSION_MINOR 1u
#define KRNL_PROJECT_VERSION_PATCH 0u

#define KRNL_DEVICE_NAME L"\\Device\\KrnlHardwareInfo"
#define KRNL_DOS_DEVICE_NAME L"\\DosDevices\\KrnlHardwareInfo"
#define KRNL_USER_DEVICE_PATH L"\\\\.\\KrnlHardwareInfo"

#define KRNL_CPU_VENDOR_STRING_LENGTH 13u
#define KRNL_STATUS_MESSAGE_LENGTH 128u

/*
 * IOCTL values are defined here so user mode and kernel mode cannot drift.
 * The project uses METHOD_BUFFERED for the initial foundation because the I/O
 * manager copies input/output buffers and the driver can validate one buffer.
 */
#define KRNL_FILE_DEVICE_TYPE 0x8000u
#define KRNL_METHOD_BUFFERED 0u
#define KRNL_FILE_ANY_ACCESS 0u

#define KRNL_IOCTL_FUNCTION_GET_DRIVER_STATUS 0x801u
#define KRNL_IOCTL_FUNCTION_QUERY_CPUID 0x802u

#define KRNL_MAKE_IOCTL(DeviceType, Function, Method, Access) \
    (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

#define IOCTL_KRNL_GET_DRIVER_STATUS \
    KRNL_MAKE_IOCTL(KRNL_FILE_DEVICE_TYPE, KRNL_IOCTL_FUNCTION_GET_DRIVER_STATUS, KRNL_METHOD_BUFFERED, KRNL_FILE_ANY_ACCESS)

#define IOCTL_KRNL_QUERY_CPUID \
    KRNL_MAKE_IOCTL(KRNL_FILE_DEVICE_TYPE, KRNL_IOCTL_FUNCTION_QUERY_CPUID, KRNL_METHOD_BUFFERED, KRNL_FILE_ANY_ACCESS)

typedef enum KRNL_REQUEST_TYPE {
    KrnlRequestTypeUnknown = 0,
    KrnlRequestTypeGetDriverStatus = 1,
    KrnlRequestTypeQueryCpuid = 2
} KRNL_REQUEST_TYPE;

typedef enum KRNL_STATUS_CODE {
    KrnlStatusOk = 0,
    KrnlStatusNotImplemented = 1,
    KrnlStatusInvalidRequest = 2,
    KrnlStatusInvalidBuffer = 3,
    KrnlStatusDeviceUnavailable = 4,
    KrnlStatusNativeCallFailed = 5
} KRNL_STATUS_CODE;

typedef struct KRNL_DRIVER_STATUS_RESPONSE {
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t version_patch;
    uint32_t status_code;
    uint8_t driver_loaded;
    uint8_t test_build;
    uint8_t reserved[2];
} KRNL_DRIVER_STATUS_RESPONSE;

typedef struct KRNL_CPUID_REQUEST {
    uint32_t leaf;
    uint32_t subleaf;
} KRNL_CPUID_REQUEST;

typedef struct KRNL_CPUID_RESPONSE {
    uint32_t leaf;
    uint32_t subleaf;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint8_t hypervisor_present;
    char vendor_string[KRNL_CPU_VENDOR_STRING_LENGTH];
    uint8_t reserved[2];
} KRNL_CPUID_RESPONSE;

typedef struct KRNL_GENERIC_STATUS_RESPONSE {
    uint32_t request_type;
    uint32_t status_code;
    int32_t native_status;
    char message[KRNL_STATUS_MESSAGE_LENGTH];
} KRNL_GENERIC_STATUS_RESPONSE;

#ifdef __cplusplus
}

static_assert(sizeof(KRNL_CPUID_REQUEST) == 8, "KRNL_CPUID_REQUEST layout changed unexpectedly.");
static_assert(sizeof(KRNL_CPUID_RESPONSE) == 40, "KRNL_CPUID_RESPONSE layout changed unexpectedly.");
#endif

#endif
