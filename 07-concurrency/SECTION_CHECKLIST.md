# Section Completion Checklist

For each lab:

- [ ] clean build succeeds;
- [ ] strict-warning compilation succeeds;
- [ ] deterministic validation passes;
- [ ] runtime evidence is generated and reviewed;
- [ ] engineering report is updated with actual observations;
- [ ] generated build/workspace artifacts remain untracked.

For the section:

- [ ] thread lifecycle and join behavior are verified;
- [ ] mutex-protected shared state is verified;
- [ ] condition-variable predicate waiting is verified;
- [ ] bounded producer/consumer behavior is verified;
- [ ] atomic counter correctness is verified;
- [ ] race-detection behavior is captured;
- [ ] reader/writer lock behavior is verified;
- [ ] worker-pool completion and shutdown are verified;
- [ ] `git diff --check` passes.
