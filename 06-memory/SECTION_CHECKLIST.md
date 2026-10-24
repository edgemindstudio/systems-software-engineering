# Section Completion Checklist

For each lab:

- [ ] clean build succeeds;
- [ ] strict-warning compilation succeeds;
- [ ] deterministic validation passes;
- [ ] runtime evidence is generated and reviewed;
- [ ] engineering report is updated with actual observations;
- [ ] generated build/workspace artifacts remain untracked.

For the section:

- [ ] virtual address-space regions are inspected;
- [ ] anonymous mapping lifecycle is verified;
- [ ] guard-page protection produces the expected child fault;
- [ ] post-fork memory isolation is verified;
- [ ] page residency changes are observed;
- [ ] minor page faults are recorded;
- [ ] `MADV_DONTNEED` behavior is verified;
- [ ] `RLIMIT_AS` produces controlled allocation failure;
- [ ] `git diff --check` passes.
