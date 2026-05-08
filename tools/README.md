# Tools

Developer tools belong here.

Use this folder for local helper scripts, VM setup helpers, build wrappers, signing helpers for local test certificates, and diagnostic utilities.

Current foundation files:

- `build.ps1`: MSBuild helper for the root Visual Studio solution.
- `clean.ps1`: dry-run clean helper. Actual deletion is intentionally disabled until build output paths are finalized.
- `vm-deployment-notes.md`: notes for future VM, test-signing, and deployment helpers.
