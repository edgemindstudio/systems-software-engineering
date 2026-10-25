# Systems Utility Suite — Engineering Report

## Objective

Build a cohesive Linux systems utility in C that combines several low-level operating-system tasks behind one consistent command-line interface.

The implemented `sysutil` executable provides:

- filesystem metadata inspection;
- robust file copying;
- recursive directory traversal;
- bounded hexadecimal inspection;
- Linux `/proc` process inspection.

The project is intentionally broader than an isolated lab. It integrates interfaces and practices developed across the foundational systems domains.

## Development Environment

Validation was performed on:

```text
Debian clang version 21.1.8 (7+b1)
Linux 6.6.87.2-microsoft-standard-WSL2 x86_64 GNU/Linux
```

## Build Quality

The normal build compiled all translation units with C17 and strict warnings enabled:

```text
-Wall
-Wextra
-Wpedantic
-Wconversion
-Wshadow
-Wstrict-prototypes
-Wmissing-prototypes
```

The validation run completed without compiler warnings.

## Integration Validation

The integration suite exercised:

- help/usage handling;
- regular-file metadata;
- symbolic-link metadata without following the link;
- byte-for-byte file copying;
- recursive directory traversal;
- symbolic-link handling during traversal;
- bounded hexadecimal output;
- `/proc/self` inspection;
- invalid-command failure behavior.

Observed result:

```text
integration_tests=PASS
```

## Representative Runtime Evidence

### File Metadata

`sysutil stat README.md` reported:

```text
path=README.md
type=regular
size=2785
mode=0644
links=1
```

The inode value was also reported but is environment-specific.

### Bounded Hex Inspection

A 32-byte inspection of `README.md` produced two formatted rows and:

```text
displayed_bytes=32
```

This verified bounded reading and hexadecimal/ASCII formatting.

### Process Inspection

`sysutil proc self` reported the running utility itself:

```text
pid=self
Name:   sysutil
State:  R (running)
VmRSS:      1408 kB
Threads:        1
reported_fields=4
```

The exact state and resident-memory value are runtime-specific; the validation establishes that the expected `/proc` fields were successfully parsed.

## Sanitizer Validation

The project was rebuilt using:

```text
-fsanitize=address,undefined
-fno-omit-frame-pointer
```

with:

```text
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1
UBSAN_OPTIONS=halt_on_error=1
```

The complete integration suite was then rerun.

Observed result:

```text
integration_tests=PASS
```

No AddressSanitizer or UndefinedBehaviorSanitizer failure interrupted the validation run.

## Engineering Interpretation

The project demonstrates several important systems-engineering properties:

1. **Modular command architecture** — command dispatch is separated from command implementations.
2. **Explicit resource ownership** — file descriptors, directory streams, heap allocations, and `FILE *` streams have defined release paths.
3. **Robust low-level I/O** — copy operations account for interrupted reads and partial writes.
4. **Symlink-aware traversal** — recursive walking uses `lstat()` and does not recurse through symbolic links.
5. **Bounded data inspection** — the hex command limits reads rather than requiring full-file buffering.
6. **Linux-native introspection** — process information is derived from `/proc` rather than simulated data.
7. **Failure-oriented CLI behavior** — invalid inputs and system-call failures return nonzero status and produce diagnostics.
8. **Evidence-backed quality checks** — normal and sanitizer builds execute the same integration suite.

## Scope

This is a portfolio systems utility, not a replacement for mature Unix utilities such as `cp`, `stat`, `find`, `xxd`, or process-monitoring tools.

In particular:

- copy does not provide atomic deployment semantics;
- traversal does not attempt every policy available in production `find` implementations;
- process inspection exposes only a small subset of `/proc`;
- filesystem behavior can differ across mounted filesystems and operating environments.

These boundaries are intentional and keep the project focused on systems mechanisms and engineering discipline.

## Result

**PASS**

The Systems Utility Suite successfully completed strict compilation, normal integration validation, representative command execution, ASan/UBSan validation, and restoration of the normal optimized build on the development environment.
