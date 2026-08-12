# Tools

Developer tools belong here.

Use this folder for local helper scripts, VM setup helpers, build wrappers, signing helpers for local test certificates, and diagnostic utilities.

Current foundation files:

- `build.ps1`: MSBuild helper for the root Visual Studio solution.
- `clean.ps1`: clean helper. Default is dry-run; pass `-Execute` to actually delete `build`/`out`/`artifacts`. `-Execute` requires a real interactive confirmation (typed `y`/`yes`) and refuses to run when input is not interactive (e.g. piped/redirected). No `-Force` bypass.
- `deploy-to-vm.ps1`: copies the built app executable (and driver binary, if built) into a deploy folder for manual VM testing. Default destination: `vm-deploy\` next to the repo; override with `-Destination`. See `vm-deployment-notes.md` for the manual VM workflow.
- `vm-deployment-notes.md`: safe manual VM workflow and notes for future deployment helpers.
