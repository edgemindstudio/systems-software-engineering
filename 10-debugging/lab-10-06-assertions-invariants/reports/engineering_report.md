# Assertions and Invariants — Engineering Report

## Objective

Use `assert()` to enforce an internal precondition and isolate the expected assertion failure inside a child process.

## Runtime Observations

```text
Assertion `denominator > 0' failed.
child_signaled=yes
signal=6
assertion_failure_isolated=yes
assert_invariant_verified=yes
```

## Engineering Interpretation

The invalid denominator violated the function invariant and caused the child process to abort.

The parent observed signal 6 (`SIGABRT`) through `waitpid()` and remained in control, demonstrating how expected fatal invariant failures can be isolated for testing.

This makes the assertion behavior verifiable without allowing the test harness itself to terminate unexpectedly.

## Result

PASS — assertion enforcement and isolated failure observation were verified.
