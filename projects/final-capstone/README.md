# Final Capstone — SysDiag

SysDiag is a local Linux systems diagnostics service composed of:

```text
sysdiag-daemon
sysdiag
```

The daemon exposes bounded, framed diagnostics requests over a Unix-domain socket. The CLI connects to that socket and requests process, filesystem, and ELF information.

This project integrates concepts developed across the repository into one cohesive service.

## Integrated Systems Concepts

- Unix-domain sockets
- explicit message framing
- robust partial I/O
- worker-thread concurrency
- bounded queues and backpressure
- mutex/condition-variable coordination
- `/proc` process inspection
- filesystem metadata
- ELF64 header inspection
- signal-aware graceful shutdown
- structured success/error responses
- concurrent-client testing
- ASan/UBSan validation
- ThreadSanitizer validation

## Commands

### PING

```bash
sysdiag --socket PATH PING
```

### PROC

Inspect the daemon process:

```bash
sysdiag --socket PATH PROC self
```

or a numeric PID:

```bash
sysdiag --socket PATH PROC 1234
```

Reports selected `/proc/<pid>/status` fields:

- Name
- State
- Threads
- VmRSS when available

### STAT

```bash
sysdiag --socket PATH STAT /path/to/file
```

Reports:

- path
- type
- size
- mode
- inode
- link count

`lstat()` semantics are used so symlinks are described rather than followed.

### ELF

```bash
sysdiag --socket PATH ELF /path/to/binary
```

Reports selected ELF64 header properties:

- class
- endianness
- ELF type
- machine
- entry point
- program-header count
- section-header count

Unsupported or truncated inputs return structured errors.

## Protocol

Every request and response uses:

```text
4-byte unsigned big-endian length
payload bytes
```

Maximum request size:

```text
2048 bytes
```

Maximum response size:

```text
8192 bytes
```

The payload is structured UTF-8-compatible text.

## Concurrency Architecture

The daemon has:

- one acceptor thread;
- a bounded circular descriptor queue;
- configurable worker threads;
- mutex-protected queue and statistics state;
- condition variables for queue-not-empty and queue-not-full.

Workers own one accepted connection, process exactly one framed request, send one framed response, close the connection, and return to the queue.

## Build

```bash
make
```

## Test

```bash
make test
```

The integration suite validates:

- PING
- STAT
- PROC
- ELF
- 60 concurrent clients
- structured unknown-command errors
- the real CLI path
- graceful shutdown

## Sanitizers

```bash
make sanitize
make tsan
```

## Full Validation

```bash
./scripts/validate.sh
```

The validation transcript is stored in:

```text
evidence/validation.txt
```

## Scope

SysDiag is intentionally local-only and uses Unix-domain sockets.

It is not a privileged monitoring agent and does not implement authentication, authorization, remote TCP exposure, TLS, persistence, kernel instrumentation, eBPF, or arbitrary command execution.

The daemon only exposes the explicitly implemented diagnostics commands.
