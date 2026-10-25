# Section Completion Checklist

For each lab:

- [ ] strict-warning build succeeds;
- [ ] binary-inspection command succeeds;
- [ ] expected ELF/symbol/relocation/runtime evidence is present;
- [ ] runtime behavior is verified where applicable;
- [ ] evidence is reviewed before commit;
- [ ] generated binaries and object files remain untracked.

For the section:

- [ ] ELF header inspection is verified;
- [ ] section and symbol inspection is verified;
- [ ] relocation inspection is verified;
- [ ] dynamic dependency inspection is verified;
- [ ] SONAME and RUNPATH behavior are verified;
- [ ] loader trace evidence is captured;
- [ ] `git diff --check` passes.
