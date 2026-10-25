# Systems Utility Suite

A compact Linux systems utility implemented in C.

`sysutil` combines several low-level operating-system tasks behind one consistent command-line interface. The project integrates file descriptors, metadata, directory traversal, robust I/O, binary inspection, `/proc` parsing, error handling, testing, and sanitizer-backed validation.

## Commands

```text
sysutil stat PATH
sysutil copy SOURCE DESTINATION
sysutil walk DIRECTORY
sysutil hex FILE [MAX_BYTES]
sysutil proc [PID|self]
sysutil help
```

## Capabilities

### `stat`

Reports selected filesystem metadata using `lstat(2)`:

- path
- file type
- size
- permission mode
- inode
- link count

Symbolic links are inspected as links rather than followed.

### `copy`

Copies a regular file using explicit `open(2)`, `read(2)`, and `write(2)` loops.

The implementation:

- handles interrupted reads/writes;
- handles partial writes;
- rejects non-regular source files;
- creates/truncates the destination;
- propagates source permission bits to the destination;
- reports the copied byte count.

### `walk`

Recursively traverses a directory tree using `opendir(3)`, `readdir(3)`, and `lstat(2)`.

The traversal does not follow symbolic links, avoiding symlink-induced directory cycles.

### `hex`

Displays a bounded hexadecimal/ASCII view of a file.

The optional `MAX_BYTES` argument limits inspection without requiring the full file to be loaded into memory.

### `proc`

Reads selected process information from `/proc/<pid>/status`.

Reported fields:

- Name
- State
- Threads
- VmRSS when available

`self` is supported for deterministic local validation.

## Build

```bash
make
```

The normal build uses Clang with strict warning flags.

## Tests

```bash
make test
```

## Sanitizers

```bash
make sanitize
```

This rebuilds with AddressSanitizer and UndefinedBehaviorSanitizer and executes the integration suite.

## Full Validation

```bash
./scripts/validate.sh
```

Validation performs:

1. clean strict-warning build;
2. command-level integration tests;
3. sanitizer build and integration tests;
4. representative command runs;
5. evidence capture.

## Project Structure

```text
systems-utility-suite/
├── include/sysutil/
├── src/
├── tests/
├── scripts/
├── docs/
├── evidence/
├── Makefile
└── README.md
```

## Engineering Goals

This project is intentionally broader than a single lab. It demonstrates how several systems-programming mechanisms can be composed behind a stable CLI and shared implementation conventions.

The project favors:

- explicit ownership;
- bounded buffers;
- checked system calls;
- deterministic output where practical;
- nonzero exit status on failure;
- reproducible tests;
- evidence-backed validation.
