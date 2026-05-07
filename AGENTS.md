# AGENTS.md

Guidance for future Codex sessions working in this repository.

KRNL Hardware Info is intentionally beginner-friendly, VM-first, and conservative
about kernel behavior. Treat this repo as a Windows user-mode app plus
kernel-driver foundation, not as a finished hardware monitor.

## Repo Layout

```text
.
├── README.md
├── AGENTS.md
├── docs/
├── src/
│   ├── shared/
│   ├── app/
│   └── driver/
├── tools/
├── tests/
└── third_party/      # only if vendored dependencies are needed
```

## Folder Purposes

- `docs/`: design notes, safety notes, setup notes, research notes, and
  beginner-friendly explanations.
- `src/shared/`: the explicit ABI/contract shared by user mode and kernel mode.
  Keep IOCTL definitions, device paths, version constants, enums, and fixed-size
  request/response structs here.
- `src/app/`: user-mode app code. This includes CLI/GUI logic, display
  formatting, logging, and driver communication wrappers.
- `src/driver/`: Windows kernel driver code. This includes WDK project files,
  driver entry/unload logic, device creation, dispatch routines, IOCTL handlers,
  and documented hardware telemetry logic.
- `tools/`: local developer utilities such as build wrappers, clean scripts, VM
  helpers, signing helpers, and deployment notes.
- `tests/`: shared contract tests, user-mode tests, driver-interface tests, and
  VM-only integration checklists.
- `third_party/`: vendored external code only when a dependency really must live
  in the repo.

The intended dependency direction is:

```text
src/app -> src/shared <- src/driver
```

`src/shared` must not depend on app code or driver code.

## Current State

The current foundation includes:

- `src/shared/krnl_contracts.h`: shared ABI definitions.
- `src/app/DriverClient.*`: wraps `CreateFileW` and `DeviceIoControl`.
- `src/app/Logger.*`: simple console/debug logging.
- `src/app/main.cpp`: minimal console smoke-test entry point.
- `src/driver/KrnlDriver.cpp`: minimal WDM-style driver with device creation,
  symbolic link creation, dispatch routines, driver status IOCTL, and basic
  CPUID IOCTL.

Do not pretend future TODOs are already implemented. Advanced hardware
telemetry, WinUI 3, project files, deployment scripts, and deeper hypervisor
trace detection are still future work unless the code says otherwise.

## Coding Style

- Prefer small, direct changes that match the existing code.
- Use C/C++ style compatible with the current files.
- Keep shared structs plain C-compatible.
- Use fixed-width integer types in shared contracts, such as `uint32_t` and
  `uint8_t`.
- Keep names explicit and project-prefixed for shared ABI items, for example
  `KRNL_*`.
- Keep comments useful and practical. Avoid noisy comments that restate obvious
  code.
- Keep beginner-facing docs casual, clear, and honest.
- Do not introduce unrelated refactors while doing a focused task.

## Safety Rules

- Treat all driver work as VM-only unless the user explicitly says otherwise.
- Do not recommend installing unsigned experimental drivers on a personal
  machine.
- Prefer documented Windows APIs and normal WDK patterns.
- Keep new IOCTLs small, explicit, and easy to validate.
- Validate every input and output buffer in kernel code.
- Assume malformed user-mode input is possible, even if the current app is the
  only caller.
- Do not add risky kernel research behavior without a written design and safety
  review.

## What NOT To Implement

Do not implement:

- Driver hiding.
- Kernel structure patching.
- Memory scanning.
- Anti-cheat bypasses.
- Evasion behavior.
- Persistence mechanisms.
- Credential access.
- Process injection.
- Undocumented kernel tricks without explicit design approval.

The project can be used for hardware information and security research, but the
repo should stay on the safe, transparent, documented side of that line.

## Kernel Code Rules

Kernel code belongs under `src/driver`.

When editing driver code:

- Keep cleanup paths correct.
- Complete each IRP exactly once.
- Return meaningful `NTSTATUS` values.
- Use `UNREFERENCED_PARAMETER` for intentionally unused driver parameters.
- Use WDK routines such as `RtlZeroMemory` and `RtlCopyMemory` where appropriate.
- Be careful with `METHOD_BUFFERED`: input and output share the same system
  buffer, so copy request data before writing response data.
- Do not use user-mode-only APIs in kernel code.
- Do not assume normal C++ runtime features are safe or available in the kernel.
- Avoid dynamic allocation unless there is a clear design and cleanup path.

Before adding a driver feature, check whether it can be done safely from
user-mode instead. CPUID can be called from user-mode; the current driver CPUID
path exists mainly to prove the IOCTL foundation.

## Shared Contract Rules

`src/shared/krnl_contracts.h` is the app-driver contract. Treat it like a public
ABI.

When changing shared contracts:

- Update both app and driver users of the contract.
- Add or update tests in `tests/shared_contract_layout.cpp`.
- Keep IOCTL values unique.
- Keep request/response structs fixed-size and pointer-free.
- Do not use `std::string`, `std::vector`, references, virtual types, or raw
  pointers in structs that cross the app/driver boundary.
- Preserve struct layout unless a deliberate compatibility break is part of the
  task.
- Add reserved bytes/fields when useful for alignment or future expansion.
- Document new IOCTL request/response behavior in `docs/ioctl-flow.md` or a
  related doc.

For new IOCTLs, normally update:

- `src/shared/krnl_contracts.h`
- `src/driver/KrnlDriver.cpp` or a new driver handler file
- `src/app/DriverClient.h`
- `src/app/DriverClient.cpp`
- `tests/shared_contract_layout.cpp`
- Relevant docs

## App Code Rules

User-mode app code belongs under `src/app`.

- Keep direct `DeviceIoControl` usage inside `DriverClient` or a similarly named
  driver-client layer.
- UI code should call `DriverClient`; it should not duplicate low-level IOCTL
  calls.
- Keep error messages useful for beginners.
- Log through the existing logging helper unless there is a reason to extend it.
- Do not make the app silently depend on the driver being installed; report
  driver-open failures clearly.

## Documentation Rules

When behavior changes, update docs in the same task.

Prefer practical docs that explain:

- What currently works.
- What is skeleton/TODO.
- What is intentionally not implemented.
- How app, driver, and shared contracts connect.
- Any VM-only safety requirements.

Do not rewrite the root `README.md` unless the user specifically asks for it.
The README has the author's voice; preserve that unless editing it is the task.

## When To Ask Before Editing

Ask before editing when:

- The request would add driver hiding, evasion, memory scanning, kernel patching,
  or other risky behavior.
- The change would alter shared ABI compatibility in a non-obvious way.
- The change would require choosing a build system, driver service name,
  certificate/signing flow, or deployment strategy that is not already defined.
- The change would delete files, rewrite history, or remove user-authored docs.
- The task conflicts with the safety notes or driver TODO warnings.

For normal docs, small app changes, tests, or clearly scoped contract additions,
make the change directly and explain what was done.

## Testing And Verification

Use the safest verification available for the change.

- For docs-only changes, review the generated Markdown and check the diff.
- For shared contract changes, compile or otherwise validate
  `tests/shared_contract_layout.cpp` when the toolchain is available.
- For app changes, build/run user-mode tests when project files exist.
- For driver changes, prefer compile-time checks first and VM-only runtime tests.
- Do not run driver integration tests on a personal machine.

If project files or test tooling do not exist yet, say that clearly in the final
summary instead of pretending verification happened.

## Definition Of Done

A change is done when:

- It respects the app/shared/driver layer split.
- It does not add prohibited behavior.
- Kernel-facing inputs and outputs are validated.
- Shared ABI changes are reflected in both code and tests.
- Docs are updated when behavior or workflow changes.
- The work is honest about what is implemented versus TODO.
- Reasonable verification was run, or the reason it could not be run is stated.
- Only files relevant to the request were changed.

When in doubt, keep the repo safer, clearer, and more beginner-friendly than you
found it.
