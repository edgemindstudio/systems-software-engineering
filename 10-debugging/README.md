# 10 — Debugging

## Purpose

This section develops practical debugging and failure-analysis skills for Linux systems software.

The labs focus on debugger-driven crash analysis, memory diagnostics, sanitizers, assertions, and reproducible defect isolation.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- compile systems programs with debug information;
- use GDB to identify a segmentation fault, source line, and call stack;
- use LLDB to independently identify the same class of crash and capture a backtrace;
- diagnose heap misuse with Valgrind Memcheck;
- detect heap-buffer overflows with AddressSanitizer;
- detect undefined behavior with UndefinedBehaviorSanitizer;
- use assertions to enforce internal invariants;
- isolate expected process failure from harness failure;
- reduce a defect to a deterministic reproducer;
- verify a corrected implementation against the same reproducer;
- preserve debugging evidence without committing generated binaries or core files.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 10-01 | GDB Segmentation-Fault Analysis | SIGSEGV, source line, backtrace |
| 10-02 | LLDB Crash Analysis | SIGSEGV, source line, backtrace |
| 10-03 | Valgrind Heap Diagnostics | invalid read after free |
| 10-04 | AddressSanitizer | heap-buffer-overflow |
| 10-05 | UndefinedBehaviorSanitizer | signed integer overflow |
| 10-06 | Assertions and Invariants | isolated SIGABRT from failed assertion |
| 10-07 | Reproducible Defect Isolation | failing case reproduced and fix verified |

## Completed Results

All seven labs passed on the development environment.

Key observed results included:

- GDB stopped on `SIGSEGV` inside `crash_here()` at `src/main.c:5` and showed `main()` as the caller;
- LLDB stopped on `SIGSEGV` inside `trigger_fault()` at `main.c:6` and produced a backtrace through `main()`;
- on this WSL environment, LLDB required ASLR disabling to be turned off and remained attached after collecting diagnostics, so the harness bounded the session with `timeout` and recorded the post-diagnostic timeout separately;
- Valgrind reported an invalid 4-byte read from a freed 4-byte allocation and confirmed that no heap blocks remained allocated at exit;
- AddressSanitizer reported a 4-byte write immediately past a 16-byte heap allocation at `src/main.c:14`;
- UndefinedBehaviorSanitizer detected signed integer overflow for `2147483647 + 1`;
- a failed assertion terminated the child with signal 6 (`SIGABRT`) while the parent process remained in control;
- the deterministic defect reproducer returned `0` from the buggy implementation where `1` was expected, and the corrected implementation returned `1`.

## Environment Note

The LLDB experiment exposed a WSL-specific debugger lifecycle behavior. LLDB successfully launched the inferior, captured the `SIGSEGV`, identified the exact source line, generated the expected backtrace, and accepted a `process kill` command. The LLDB batch process nevertheless did not terminate cleanly before the harness timeout, so the validation script treats exit code `124` as acceptable only after all required crash evidence has already been verified.

## Progress

- Lab 10-01 — GDB Segmentation-Fault Analysis: Complete
- Lab 10-02 — LLDB Crash Analysis: Complete
- Lab 10-03 — Valgrind Heap Diagnostics: Complete
- Lab 10-04 — AddressSanitizer: Complete
- Lab 10-05 — UndefinedBehaviorSanitizer: Complete
- Lab 10-06 — Assertions and Invariants: Complete
- Lab 10-07 — Reproducible Defect Isolation: Complete

Domain 10 — Debugging: Complete
