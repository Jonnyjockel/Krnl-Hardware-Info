# Safety Notes

KRNL Hardware Info includes a kernel driver skeleton. Treat it as VM-only until the driver is mature, reviewed, and test coverage exists.

Guidelines:

- Test in a disposable Windows VM.
- Do not install unsigned experimental drivers on a personal machine.
- Keep test-signing mode inside disposable VMs, and turn it off when finished.
- Keep new IOCTLs small and validate every input and output buffer.
- Prefer documented Windows APIs and WDK patterns.
- Keep risky research work behind written design notes before coding it.
- Do not add evasion, hiding, or kernel patching behavior.

Related docs:

- `build.md`: build requirements and current build limits.
- `driver-service.md`: VM-only manual service commands.
- `test-signing.md`: why test-signing exists and how to turn it off again.
