# Section Completion Checklist

For each lab:

- [ ] clean build succeeds;
- [ ] strict-warning compilation succeeds;
- [ ] deterministic validation passes;
- [ ] runtime evidence is generated and reviewed;
- [ ] engineering report is updated with actual observations;
- [ ] generated build/workspace artifacts remain untracked.

For the section:

- [ ] `./scripts/validate_all.sh` passes;
- [ ] `strace` evidence is captured for syscall-oriented labs;
- [ ] sparse-file allocation behavior is observed;
- [ ] mmap persistence is verified;
- [ ] advisory lock contention and release are verified;
- [ ] `git diff --check` passes;
- [ ] section README is marked complete only after all evidence is reviewed.
