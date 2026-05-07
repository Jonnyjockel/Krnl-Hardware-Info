# Driver TODOs

This folder currently contains a minimal WDM-style driver skeleton.

Future work:

- Add real hardware monitor queries using documented kernel APIs where possible.
- Add careful input validation for every new IOCTL.
- Add VM-only integration tests before expanding driver behavior.
- Decide how the driver will be built, packaged, installed, and test-signed.

Do not add:

- Driver hiding.
- Kernel structure patching.
- Memory scanning.
- Anti-cheat bypasses or evasion behavior.
- Undocumented kernel tricks without a written design and safety review.

