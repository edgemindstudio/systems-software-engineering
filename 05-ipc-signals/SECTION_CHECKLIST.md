# Section Completion Checklist

For each lab:

- [ ] clean build succeeds;
- [ ] strict-warning compilation succeeds;
- [ ] deterministic validation passes;
- [ ] runtime evidence is generated and reviewed;
- [ ] engineering report is updated with actual observations;
- [ ] generated build/workspace artifacts remain untracked.

For the section:

- [ ] anonymous pipe EOF behavior is verified;
- [ ] multi-process pipeline output is verified;
- [ ] `sigaction` handling is verified;
- [ ] handler code uses only safe minimal operations;
- [ ] masked/pending signal behavior is verified;
- [ ] `sigsuspend` wakeup is verified;
- [ ] FIFO communication is verified;
- [ ] POSIX shared-memory visibility is verified;
- [ ] shared-memory object cleanup is verified;
- [ ] `git diff --check` passes.
