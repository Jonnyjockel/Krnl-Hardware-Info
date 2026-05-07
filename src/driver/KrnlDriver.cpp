#include <ntddk.h>
#include <intrin.h>

#include "../shared/krnl_contracts.h"

extern "C" DRIVER_INITIALIZE DriverEntry;

extern "C" void KrnlDriverUnload(_In_ PDRIVER_OBJECT DriverObject);
extern "C" NTSTATUS KrnlDispatchCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp);
extern "C" NTSTATUS KrnlDispatchDeviceControl(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp);
extern "C" NTSTATUS KrnlDispatchUnsupported(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp);

namespace {

NTSTATUS CompleteIrp(_Inout_ PIRP irp, NTSTATUS status, ULONG_PTR information) {
    irp->IoStatus.Status = status;
    irp->IoStatus.Information = information;
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS HandleGetDriverStatus(_Out_writes_bytes_(output_length) void* output_buffer, ULONG output_length, ULONG_PTR* bytes_written) {
    if (output_buffer == nullptr || output_length < sizeof(KRNL_DRIVER_STATUS_RESPONSE)) {
        return STATUS_BUFFER_TOO_SMALL;
    }

    auto* response = static_cast<KRNL_DRIVER_STATUS_RESPONSE*>(output_buffer);
    RtlZeroMemory(response, sizeof(*response));

    response->version_major = KRNL_PROJECT_VERSION_MAJOR;
    response->version_minor = KRNL_PROJECT_VERSION_MINOR;
    response->version_patch = KRNL_PROJECT_VERSION_PATCH;
    response->status_code = KrnlStatusOk;
    response->driver_loaded = 1;
    response->test_build = 1;

    *bytes_written = sizeof(*response);
    return STATUS_SUCCESS;
}

NTSTATUS HandleQueryCpuid(
    _In_reads_bytes_(input_length) const void* input_buffer,
    ULONG input_length,
    _Out_writes_bytes_(output_length) void* output_buffer,
    ULONG output_length,
    ULONG_PTR* bytes_written) {
    if (input_buffer == nullptr || input_length < sizeof(KRNL_CPUID_REQUEST)) {
        return STATUS_INVALID_PARAMETER;
    }

    if (output_buffer == nullptr || output_length < sizeof(KRNL_CPUID_RESPONSE)) {
        return STATUS_BUFFER_TOO_SMALL;
    }

    /*
     * METHOD_BUFFERED uses one system buffer for input and output, so copy the
     * request before writing the response.
     */
    KRNL_CPUID_REQUEST request = {};
    RtlCopyMemory(&request, input_buffer, sizeof(request));

    auto* response = static_cast<KRNL_CPUID_RESPONSE*>(output_buffer);
    RtlZeroMemory(response, sizeof(*response));

    int registers[4] = {};
    __cpuidex(registers, static_cast<int>(request.leaf), static_cast<int>(request.subleaf));

    response->leaf = request.leaf;
    response->subleaf = request.subleaf;
    response->eax = static_cast<uint32_t>(registers[0]);
    response->ebx = static_cast<uint32_t>(registers[1]);
    response->ecx = static_cast<uint32_t>(registers[2]);
    response->edx = static_cast<uint32_t>(registers[3]);

    if (request.leaf == 0) {
        RtlCopyMemory(&response->vendor_string[0], &registers[1], sizeof(uint32_t));
        RtlCopyMemory(&response->vendor_string[4], &registers[3], sizeof(uint32_t));
        RtlCopyMemory(&response->vendor_string[8], &registers[2], sizeof(uint32_t));
        response->vendor_string[KRNL_CPU_VENDOR_STRING_LENGTH - 1] = '\0';
    }

    if (request.leaf == 1) {
        response->hypervisor_present = ((response->ecx & (1u << 31)) != 0u) ? 1u : 0u;
    }

    /*
     * TODO: Add future safe, documented hardware information queries here.
     * Do not add driver hiding, kernel patching, memory scanning, or evasion
     * behavior to this IOCTL handler.
     */
    *bytes_written = sizeof(*response);
    return STATUS_SUCCESS;
}

}  // namespace

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    UNICODE_STRING device_name = {};
    UNICODE_STRING symbolic_link_name = {};
    PDEVICE_OBJECT device_object = nullptr;

    RtlInitUnicodeString(&device_name, KRNL_DEVICE_NAME);
    RtlInitUnicodeString(&symbolic_link_name, KRNL_DOS_DEVICE_NAME);

    NTSTATUS status = IoCreateDevice(
        DriverObject,
        0,
        &device_name,
        KRNL_FILE_DEVICE_TYPE,
        0,
        FALSE,
        &device_object);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = IoCreateSymbolicLink(&symbolic_link_name, &device_name);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(device_object);
        return status;
    }

    for (ULONG i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; ++i) {
        DriverObject->MajorFunction[i] = KrnlDispatchUnsupported;
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE] = KrnlDispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = KrnlDispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = KrnlDispatchDeviceControl;
    DriverObject->DriverUnload = KrnlDriverUnload;

    device_object->Flags |= DO_BUFFERED_IO;
    device_object->Flags &= ~DO_DEVICE_INITIALIZING;

    return STATUS_SUCCESS;
}

extern "C" void KrnlDriverUnload(_In_ PDRIVER_OBJECT DriverObject) {
    UNICODE_STRING symbolic_link_name = {};
    RtlInitUnicodeString(&symbolic_link_name, KRNL_DOS_DEVICE_NAME);

    IoDeleteSymbolicLink(&symbolic_link_name);

    if (DriverObject->DeviceObject != nullptr) {
        IoDeleteDevice(DriverObject->DeviceObject);
    }
}

extern "C" NTSTATUS KrnlDispatchCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    return CompleteIrp(Irp, STATUS_SUCCESS, 0);
}

extern "C" NTSTATUS KrnlDispatchDeviceControl(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);

    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    const ULONG ioctl_code = stack->Parameters.DeviceIoControl.IoControlCode;
    const ULONG input_length = stack->Parameters.DeviceIoControl.InputBufferLength;
    const ULONG output_length = stack->Parameters.DeviceIoControl.OutputBufferLength;
    void* system_buffer = Irp->AssociatedIrp.SystemBuffer;

    ULONG_PTR bytes_written = 0;
    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

    switch (ioctl_code) {
        case IOCTL_KRNL_GET_DRIVER_STATUS:
            status = HandleGetDriverStatus(system_buffer, output_length, &bytes_written);
            break;

        case IOCTL_KRNL_QUERY_CPUID:
            status = HandleQueryCpuid(system_buffer, input_length, system_buffer, output_length, &bytes_written);
            break;

        default:
            /*
             * TODO: Add new IOCTLs only after extending src/shared/krnl_contracts.h
             * and adding tests for the expected request/response layout.
             */
            status = STATUS_INVALID_DEVICE_REQUEST;
            break;
    }

    return CompleteIrp(Irp, status, bytes_written);
}

extern "C" NTSTATUS KrnlDispatchUnsupported(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    return CompleteIrp(Irp, STATUS_NOT_SUPPORTED, 0);
}
