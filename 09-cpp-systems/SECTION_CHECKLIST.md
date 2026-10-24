# Section Completion Checklist

For each lab:

- [ ] clean build succeeds;
- [ ] strict-warning compilation succeeds;
- [ ] deterministic validation passes;
- [ ] runtime evidence is generated and reviewed;
- [ ] engineering report is updated with actual observations;
- [ ] generated build/workspace artifacts remain untracked.

For the section:

- [ ] RAII cleanup is verified;
- [ ] move-only ownership transfer is verified;
- [ ] smart-pointer lifetime behavior is verified;
- [ ] system-error handling preserves cleanup guarantees;
- [ ] custom container growth and bounds behavior are verified;
- [ ] thread-safe queue coordination is verified;
- [ ] `git diff --check` passes.
