# Architecture

## Overview

`sysutil` is a single executable with command dispatch in `src/main.c`. Each command lives in a separate translation unit and exposes one public function through `include/sysutil/commands.h`.

Shared helpers live in `src/common.c`.

## Command Dispatch

The executable accepts a command name followed by command-specific arguments.

```text
main
  ├── stat  -> sysutil_cmd_stat
  ├── copy  -> sysutil_cmd_copy
  ├── walk  -> sysutil_cmd_walk
  ├── hex   -> sysutil_cmd_hex
  └── proc  -> sysutil_cmd_proc
```

This keeps CLI parsing independent from command implementations and makes each subsystem testable in isolation through the executable.

## Error Model

Commands return:

- `0` on success;
- nonzero on invalid input or operating-system failure.

System-call failures use `sysutil_error_errno()` to print a consistent diagnostic containing the failed operation and `strerror(errno)`.

## File-System Safety

`stat` and `walk` use `lstat()` so symbolic links are inspected without being followed.

`walk` recursively enters only entries whose `lstat()` type is a directory. This avoids recursion through symbolic-link cycles.

## Copy Semantics

`copy` uses:

1. `open()` source;
2. `fstat()` source;
3. reject non-regular source;
4. `open()` destination;
5. repeated `read()` calls;
6. `write_all()` for partial writes/EINTR;
7. `fchmod()` to apply source permission bits;
8. close both descriptors.

The destination is not intended to provide atomic replacement semantics. This is a focused systems utility, not a transactional file deployment mechanism.

## Process Inspection

`proc` reads `/proc/<pid>/status`, selecting a small stable subset of human-readable fields. It does not claim to be a complete process monitor.

## Test Strategy

The integration suite creates an isolated workspace and verifies:

- metadata output;
- byte-for-byte file copying;
- recursive traversal;
- symlink handling;
- bounded hex output;
- `/proc/self` inspection;
- invalid-command failure behavior.

The same integration suite is run once under the normal build and again under ASan/UBSan.
