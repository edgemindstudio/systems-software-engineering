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
- [ ] parent/child PID relationships are observed;
- [ ] exec replacement is verified with stable PID;
- [ ] child statuses are reaped with `waitpid`;
- [ ] a multi-generation process tree is inspected;
- [ ] cwd/environment/fd inheritance is verified;
- [ ] `FD_CLOEXEC` behavior is verified;
- [ ] process-group/session behavior is verified;
- [ ] `git diff --check` passes.
