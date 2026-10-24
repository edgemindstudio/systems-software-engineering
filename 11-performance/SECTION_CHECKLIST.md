# Section Completion Checklist

For each lab:

- [ ] strict-warning build succeeds;
- [ ] benchmark executes deterministically;
- [ ] output preserves enough work to prevent trivial optimization removal;
- [ ] timing evidence is captured;
- [ ] interpretation avoids overclaiming environment-specific absolute numbers;
- [ ] engineering report is updated from actual observations;
- [ ] generated build/workspace/perf artifacts remain untracked.

For the section:

- [ ] monotonic timing is verified;
- [ ] repeated-trial summary statistics are verified;
- [ ] cache-locality comparison is observed;
- [ ] syscall amortization is measured;
- [ ] allocation/reuse comparison is measured;
- [ ] optimization-level comparison is measured;
- [ ] `git diff --check` passes.
