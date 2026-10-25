# Final Capstone — Engineering Report

## Objective

Build a cohesive local Linux systems diagnostics service that integrates Unix-domain IPC, explicit message framing, robust stream I/O, bounded worker-thread concurrency, `/proc` inspection, filesystem metadata, ELF inspection, structured errors, graceful shutdown, and sanitizer-backed verification.

The capstone consists of:

```text
sysdiag-daemon
sysdiag
```

The daemon serves diagnostics requests over a Unix-domain socket. The CLI connects to the daemon, sends one framed request, receives one framed response, and exits.

## Development Environment

Validation was performed on:

```text
Debian clang version 21.1.8 (7+b1)
Python 3.13.12
Linux 6.6.87.2-microsoft-standard-WSL2 x86_64 GNU/Linux
```

## Build Quality

The normal build compiled the protocol layer, diagnostics module, daemon, and CLI using C17 with:

```text
-Wall
-Wextra
-Wpedantic
-Wconversion
-Wshadow
-Wstrict-prototypes
-Wmissing-prototypes
-pthread
```

The validation run completed without compiler warnings.

## Integration and Concurrency Validation

The complete integration suite reported:

```text
ping=PASS
stat=PASS
proc=PASS
elf=PASS
concurrent_clients=PASS
unknown_command=PASS
cli=PASS
graceful_shutdown=PASS
final_capstone_tests=PASS
```

### PING

The daemon returned:

```text
OK command=PING
pong=yes
```

This verified basic client/daemon IPC and framed request/response behavior.

### STAT

The integration suite created a known regular file and queried it through the daemon.

The result verified:

- structured success response;
- regular-file type;
- expected byte size;
- filesystem metadata path.

`STAT` uses `lstat()` so symbolic links are described rather than followed.

### PROC

The `PROC self` diagnostic inspects the daemon process through:

```text
/proc/self/status
```

The representative run reported:

```text
OK command=PROC
Name:   sysdiag-daemon
State:  S (sleeping)
VmRSS:      1584 kB
Threads:        4
reported_fields=4
```

This verified process inspection of:

- process name;
- process state;
- resident memory when available;
- thread count.

### ELF

The representative ELF request reported:

```text
OK command=ELF
path=/home/fonke/systems-software-engineering/projects/final-capstone/workspace/fixture-demo
class=ELF64
endianness=little
type=DYN
machine=62
entry=0x1050
program_headers=14
section_headers=38
```

This verified direct ELF64 header inspection inside the diagnostics service.

The capstone intentionally performs header-level ELF inspection. The dedicated Binary Inspector project contains the deeper section, segment, symbol, dependency, and address-mapping implementation.

### Concurrent Clients

The integration suite submitted 60 independent requests using 20 concurrent client workers.

Observed result:

```text
concurrent_clients=PASS
```

This exercised:

- Unix-domain connection acceptance;
- bounded descriptor queueing;
- worker-thread scheduling;
- concurrent request processing;
- shared runtime statistics.

### Structured Error Handling

An unsupported request was sent:

```text
UNKNOWN
```

The daemon returned a structured error response beginning with:

```text
ERR
```

Observed result:

```text
unknown_command=PASS
```

### CLI Path

The real `sysdiag` executable was used to issue a `PING` request to the running daemon.

Observed result:

```text
cli=PASS
```

This confirms the user-facing client path works independently of the Python integration harness.

## Representative Daemon Configuration

The representative daemon run used:

```text
workers=3
queue_capacity=8
```

The daemon reported:

```text
socket=workspace/sysdiag-demo.sock
workers=3
queue_capacity=8
accepted_requests=3
completed_requests=3
request_errors=0
shutdown=graceful
```

The important lifecycle result is:

```text
accepted_requests=3
completed_requests=3
request_errors=0
shutdown=graceful
```

Every accepted representative request completed and the daemon shut down cleanly.

## IPC Protocol

The transport is an `AF_UNIX` stream socket.

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

The protocol layer uses exact-read and write-all loops so correctness does not depend on stream read/write boundaries.

`EINTR` is retried.

## Concurrency Architecture

The daemon separates connection acceptance from request execution.

The main thread:

1. accepts Unix-domain connections;
2. inserts descriptors into a bounded circular queue.

Worker threads:

1. wait for queued descriptors;
2. take exclusive ownership of one connection;
3. read one framed request;
4. dispatch diagnostics;
5. send one framed response;
6. close the descriptor;
7. update completion/error accounting.

The queue is protected by one mutex and coordinated with:

```text
not_empty
not_full
```

This produces explicit application-level backpressure.

## Bounded Resource Model

The connection queue has a configured fixed capacity.

When full, the acceptor waits rather than accumulating unbounded accepted descriptors inside the application.

This carries the bounded-resource design used earlier in the Thread Runtime and Network Service projects into the capstone.

## Diagnostics Separation

Transport and diagnostics logic are intentionally separated.

The diagnostics layer implements:

```text
PING
STAT
PROC
ELF
```

without owning socket lifecycle or worker synchronization.

This separation reduces coupling between operating-system inspection logic and IPC/concurrency mechanics.

## Graceful Shutdown

The daemon handles:

```text
SIGINT
SIGTERM
```

Shutdown behavior is:

1. record a stop request;
2. close the listening socket;
3. exit the accept loop;
4. mark the descriptor queue as stopping;
5. wake blocked workers and queue waiters;
6. drain already queued requests;
7. join worker threads;
8. print final statistics;
9. unlink the Unix socket path.

`SIGPIPE` is ignored so peer disconnects become ordinary I/O failures rather than process termination.

The integration suite verified:

```text
graceful_shutdown=PASS
```

## AddressSanitizer and UndefinedBehaviorSanitizer

The capstone was rebuilt with:

```text
-fsanitize=address,undefined
-fno-omit-frame-pointer
```

and executed with:

```text
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1
UBSAN_OPTIONS=halt_on_error=1
```

The full integration suite passed again:

```text
ping=PASS
stat=PASS
proc=PASS
elf=PASS
concurrent_clients=PASS
unknown_command=PASS
cli=PASS
graceful_shutdown=PASS
final_capstone_tests=PASS
```

No AddressSanitizer or UndefinedBehaviorSanitizer failure interrupted validation.

## ThreadSanitizer Validation

The project was separately rebuilt with:

```text
-fsanitize=thread
-fno-omit-frame-pointer
```

and executed with:

```text
TSAN_OPTIONS=halt_on_error=1
```

The complete integration/concurrency suite passed:

```text
ping=PASS
stat=PASS
proc=PASS
elf=PASS
concurrent_clients=PASS
unknown_command=PASS
cli=PASS
graceful_shutdown=PASS
final_capstone_tests=PASS
```

No ThreadSanitizer data-race report interrupted validation.

This is significant because the capstone deliberately combines:

- a producer/acceptor thread;
- multiple worker threads;
- mutable queue state;
- shared request counters;
- concurrent clients;
- shutdown state transitions.

## Engineering Interpretation

This capstone demonstrates integrated systems-software engineering across multiple subsystems:

1. **IPC design** — Unix-domain sockets provide local process-to-process communication.
2. **Protocol framing** — explicit length prefixes preserve application message boundaries over a byte stream.
3. **Robust I/O** — exact-read and complete-write loops handle partial operations and `EINTR`.
4. **Concurrent architecture** — the acceptor and worker pool divide connection admission from request execution.
5. **Backpressure** — a bounded descriptor queue prevents unbounded internal pending work.
6. **Synchronization discipline** — mutexes and condition variables protect queue state and coordination.
7. **Process introspection** — `/proc` is queried directly for runtime process state.
8. **Filesystem inspection** — `lstat()` provides object type and metadata without unintentionally following symlinks.
9. **Binary-format awareness** — ELF64 header fields are parsed and surfaced as diagnostics.
10. **Structured failure semantics** — unsupported or invalid requests produce explicit `ERR` responses.
11. **Lifecycle management** — signal-driven shutdown drains work, joins workers, reports statistics, and removes the socket.
12. **Observable correctness** — accepted/completed/error counters support operational verification.
13. **Memory/UB validation** — ASan and UBSan cover the integrated service.
14. **Race validation** — ThreadSanitizer validates the concurrent service under real multi-client load.
15. **System integration** — concepts demonstrated independently in earlier projects are brought together behind one coherent daemon/CLI interface.

## Relationship to Earlier Major Projects

The capstone intentionally synthesizes concepts previously demonstrated across the repository:

- Systems Utility Suite → filesystem/process diagnostics;
- Unix Shell → process/system-call discipline and error semantics;
- Memory Allocator → bounded-resource and ownership reasoning;
- Thread Runtime → worker coordination, queues, lifecycle, and concurrency;
- Network Service → framing, robust stream I/O, bounded connection handling, graceful shutdown;
- Binary Inspector → ELF understanding and binary metadata interpretation.

The implementation does not mechanically embed those earlier project codebases; it applies the same systems concepts in a unified service architecture.

## Scope

SysDiag is intentionally local-only.

It does not implement:

- remote TCP exposure;
- TLS;
- authentication;
- authorization;
- persistence;
- privileged operations;
- arbitrary command execution;
- kernel modules;
- eBPF;
- production telemetry infrastructure;
- distributed coordination.

The daemon only exposes explicitly implemented diagnostics commands.

## Result

**PASS**

The Final Capstone successfully completed strict compilation, Unix-domain IPC validation, process inspection, filesystem inspection, ELF inspection, 60-request concurrent-client testing, structured error handling, real CLI execution, graceful shutdown verification, ASan/UBSan validation, and ThreadSanitizer validation on the development environment.
