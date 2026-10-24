# Section Completion Checklist

For each lab:

- [ ] clean build succeeds;
- [ ] strict-warning compilation succeeds;
- [ ] diagnostic tool or expected failure executes;
- [ ] validation distinguishes expected diagnostic failure from harness failure;
- [ ] runtime evidence is generated and reviewed;
- [ ] engineering report is updated with actual observations;
- [ ] generated build/workspace/core artifacts remain untracked.

For the section:

- [ ] GDB crash location and backtrace are captured;
- [ ] LLDB crash location and backtrace are captured;
- [ ] Valgrind detects heap misuse;
- [ ] AddressSanitizer detects invalid memory access;
- [ ] UndefinedBehaviorSanitizer detects undefined behavior;
- [ ] assertion failure is isolated and interpreted;
- [ ] deterministic defect isolation and correction are verified;
- [ ] `git diff --check` passes.
