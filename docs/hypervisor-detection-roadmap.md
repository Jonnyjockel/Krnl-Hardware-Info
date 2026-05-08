# Hypervisor Detection Roadmap

KRNL talks about hypervisor traces, but the current project is still at the
foundation stage. This document explains what safe virtualization artifact
research can mean here without pretending there is a finished detection engine.

The framing for this project is:

- learning how app/driver communication works
- hardware telemetry
- virtualization artifact research
- VM safety checks
- beginner-friendly documentation around low-level Windows behavior

It is not about stealth, bypasses, anti-cheat evasion, driver hiding, or loading
random vulnerable drivers.

## What Is Real Today

Currently implemented:

- driver status IOCTL
- generic CPUID request/response IOCTL
- CPUID leaf `0` vendor-string handling in the response
- CPUID leaf `1` hypervisor-present bit handling in the response

Important beginner detail: the current console app asks for CPUID leaf `0` to
show the CPU vendor string, then asks leaf `1` to show the hypervisor-present
bit as one signal.

That means the current code is CPUID plumbing, not a full hypervisor detector.

## Detection Layer 1: CPUID Hypervisor Bit

One common signal is CPUID leaf `1`, ECX bit `31`.

If that bit is set, software often treats it as "a hypervisor is present." The
current driver already knows how to place that bit into
`KRNL_CPUID_RESPONSE.hypervisor_present` when the caller requests leaf `1`.

Limitations:

- the bit is only one signal
- some environments may expose it differently
- one bit is not enough to confidently describe the whole system

Good future work:

- add tests around CPUID response parsing
- show more context around what the bit does and does not prove

## Detection Layer 2: CPUID Vendor Leaf `0x40000000`

Many hypervisors expose a vendor or interface leaf starting at CPUID
`0x40000000`.

This can return strings associated with virtualization platforms. Future KRNL
research could query this leaf and show the raw result in a beginner-friendly
way.

Limitations:

- not every hypervisor exposes the same leaves
- vendor strings can be absent, changed, or hidden
- the meaning should be documented before the app turns it into a label

Good future work:

- add an explicit app-side query for leaf `0x40000000`
- document the raw registers and string decoding
- keep the output as "observed artifact" rather than "perfect detection"

## Detection Layer 3: SMBIOS / BIOS Strings

VMs often leave clues in SMBIOS, BIOS, baseboard, or system manufacturer
strings. These are usually safer to inspect from user mode first.

Examples of future user-mode sources may include documented Windows APIs,
PowerShell/WMI-style inspection, or other normal OS-provided inventory data.

Limitations:

- strings can be missing or generic
- real hardware can have odd firmware strings too
- firmware data is not proof by itself

Good future work:

- start in user mode
- show the raw fields
- document why a field is suspicious or normal
- avoid kernel code unless there is a clear reason

## Detection Layer 4: PCI / Device Artifacts

Virtual machines may expose recognizable PCI devices, storage controllers,
network adapters, display adapters, or guest integration devices.

This kind of artifact research should prefer documented user-mode enumeration
first. Kernel code should only be considered if a design explains why user mode
is not enough.

Limitations:

- device lists vary by VM platform and configuration
- drivers can change visible names
- passthrough hardware can make results messy
- one matching device is still only one signal

Good future work:

- enumerate devices safely in user mode where possible
- group results as artifacts, not accusations
- document which artifacts were observed in which VM setups

## Detection Layer 5: Timing Checks

Timing checks are future research only.

Some virtualization research looks at timing differences, instruction latency,
or jitter. This can get complicated quickly because modern CPUs, power
management, background load, debuggers, and the VM host can all affect timing.

Limitations:

- noisy results
- false positives
- host load affects measurements
- easy to over-interpret
- may require careful research design before any code exists

Good future work:

- write a design note first
- keep it VM-only
- collect repeated measurements
- clearly label results as experimental
- avoid using timing as a bypass or evasion feature

## Why No Single Check Is Perfect

Virtualization detection is a pile of signals, not one magic answer.

CPUID can say one thing. SMBIOS can say another. Device artifacts may be obvious
on one VM and quiet on another. Timing can be noisy even on real hardware.

KRNL should present these as observations:

- "CPUID hypervisor bit is set"
- "CPUID hypervisor vendor leaf returned this string"
- "SMBIOS manufacturer contains this value"
- "these VM-like devices were found"
- "timing result looked unusual in this VM test"

It should not pretend to give a perfect yes/no answer from one check.

## What Belongs Where

- `src/app`: display results, explain signals, format raw values, and run
  user-mode artifact checks when possible.
- `src/shared`: fixed-size structs and IOCTLs only when app/driver communication
  needs a new contract.
- `src/driver`: kernel-side queries only when user mode is not enough and the
  behavior has a written safety/design note.
- `docs`: research notes, limitations, examples, and safety warnings.
- `tests`: compile-time contract checks and future VM-only integration tests.

## Not Part Of This Project

Do not add:

- anti-cheat bypasses
- stealth or evasion behavior
- driver hiding
- vulnerable-driver loading
- memory scanning
- kernel patching
- undocumented kernel tricks without written design approval

The useful version of this project is transparent: it helps people learn what
signals exist, what they mean, and where the limits are.
