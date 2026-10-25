# Network Service — Engineering Report

## Objective

Build a concurrent framed TCP service in C with explicit application-level message boundaries, robust partial stream I/O, bounded connection concurrency, malformed-client handling, and graceful shutdown.

The project intentionally goes beyond a basic echo-server exercise by treating TCP as a byte stream and enforcing a defined protocol.

## Development Environment

Validation was performed on:

```text
Debian clang version 21.1.8 (7+b1)
Python 3.13.12
Linux 6.6.87.2-microsoft-standard-WSL2 x86_64 GNU/Linux
```

## Build Quality

The normal build compiled the shared protocol implementation, server, and client using C17 with:

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

## Protocol

Each request and response uses the same frame format:

```text
4-byte unsigned big-endian payload length
payload bytes
```

Maximum payload size:

```text
4096 bytes
```

The server echoes each valid payload using the same framing.

The protocol implementation does not assume that one `read()` corresponds to one complete message.

## Integration and Concurrency Validation

The real TCP integration suite reported:

```text
basic_roundtrip=PASS
fragmented_request=PASS
concurrent_clients=PASS
max_frame=PASS
oversized_frame_rejected=PASS
graceful_shutdown=PASS
network_service_tests=PASS
```

### Basic Round Trip

A client connected over loopback TCP, sent one framed payload, and received the same payload back.

### Fragmented Request Delivery

A complete frame was intentionally transmitted one byte at a time.

Observed result:

```text
fragmented_request=PASS
```

This demonstrates that the service correctly reconstructs a frame across arbitrary TCP stream fragmentation rather than relying on packet or `read()` boundaries.

### Concurrent Clients

The integration suite executed 40 independent client connections using concurrent client workers.

Observed result:

```text
concurrent_clients=PASS
```

This exercised the acceptor, bounded connection queue, worker-thread pool, and connection lifecycle under simultaneous activity.

### Maximum Frame

A payload of exactly:

```text
4096 bytes
```

was accepted and echoed correctly.

Observed result:

```text
max_frame=PASS
```

### Oversized Frame Rejection

A malformed client declared a frame length of:

```text
4097 bytes
```

which is one byte beyond the protocol maximum.

The server rejected the frame and closed the connection without returning a payload.

Observed result:

```text
oversized_frame_rejected=PASS
```

The integration server's final statistics separately verified one protocol error for that malformed connection.

### Graceful Shutdown

The integration harness terminated the server with `SIGTERM`.

Observed result:

```text
graceful_shutdown=PASS
```

The server drained queued work, joined workers, and exited successfully.

## Representative Server/Client Execution

A representative server was started with:

```text
workers=2
queue_capacity=4
```

on an ephemeral loopback port.

The client sent:

```text
network-service-ok
```

and observed:

```text
response_length=18
response=network-service-ok
```

The server reported:

```text
listening_address=127.0.0.1
listening_port=40857
workers=2
queue_capacity=4
accepted_connections=1
completed_connections=1
protocol_errors=0
shutdown=graceful
```

The ephemeral port value is environment-specific. The important operational result is that the real client connected, exchanged a framed payload, and the server exited with internally consistent connection statistics.

## Robust Stream I/O

`netservice_read_exact()` repeatedly calls `read()` until one of the following occurs:

- the requested byte count is received;
- EOF occurs;
- a non-recoverable error occurs.

`EINTR` is retried.

`netservice_write_all()` similarly continues until the complete buffer is written or a non-recoverable error occurs.

This handles ordinary TCP partial-read and partial-write behavior correctly.

## Bounded Concurrency Model

The server separates connection acceptance from connection processing.

The acceptor places accepted descriptors into a fixed-capacity circular queue protected by a mutex.

Two condition variables coordinate queue state:

```text
not_empty
not_full
```

Workers wait on `not_empty`.

The acceptor waits on `not_full` if the queue reaches capacity.

This creates explicit application-level backpressure and prevents unbounded accepted-connection storage.

## Worker Model

Each worker repeatedly:

1. waits for a queued descriptor;
2. removes one connection from the queue;
3. services framed requests;
4. returns framed responses;
5. exits the connection loop on EOF or protocol/I/O failure;
6. closes the client descriptor;
7. returns to the queue.

A worker owns one connection for its service lifetime.

## Shutdown Semantics

The server installs handlers for:

```text
SIGINT
SIGTERM
```

The handler records a stop request and closes the listening descriptor.

This causes the accept loop to terminate.

The connection queue then enters stopping mode, blocked workers are awakened, queued connections drain, and all worker threads are joined before process exit.

The server also ignores:

```text
SIGPIPE
```

so a peer disconnect during a write becomes an ordinary I/O failure rather than terminating the entire service.

## Protocol Error Accounting

Malformed oversized frames are counted separately from ordinary connection completion.

The integration suite deliberately generated one oversized frame and verified:

```text
protocol_errors=1
```

for that test server.

The representative normal client/server run correctly reported:

```text
protocol_errors=0
```

## AddressSanitizer and UndefinedBehaviorSanitizer

The complete service was rebuilt with:

```text
-fsanitize=address,undefined
-fno-omit-frame-pointer
```

and executed with:

```text
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1
UBSAN_OPTIONS=halt_on_error=1
```

The real TCP integration suite passed again:

```text
basic_roundtrip=PASS
fragmented_request=PASS
concurrent_clients=PASS
max_frame=PASS
oversized_frame_rejected=PASS
graceful_shutdown=PASS
network_service_tests=PASS
```

No AddressSanitizer or UndefinedBehaviorSanitizer failure interrupted the validation run.

## Engineering Interpretation

This project demonstrates several systems-software engineering competencies:

1. **TCP stream semantics** — message boundaries are defined explicitly rather than inferred from `read()` calls.
2. **Network byte order** — frame lengths use a portable 32-bit big-endian representation.
3. **Robust I/O loops** — partial reads, partial writes, EOF, and `EINTR` are handled deliberately.
4. **Protocol bounds** — oversized frames are rejected before payload processing.
5. **Concurrent connection processing** — accepted connections are serviced by worker threads.
6. **Bounded resource design** — the connection queue has an explicit fixed capacity.
7. **Backpressure** — the acceptor waits when the application queue is full.
8. **Connection lifecycle ownership** — descriptors move from acceptor to queue to exactly one worker and are then closed.
9. **Signal-aware shutdown** — the listening socket, queue, and worker lifecycle cooperate during termination.
10. **Failure containment** — `SIGPIPE` is neutralized at the process level and malformed clients do not terminate the server.
11. **Evidence-backed validation** — real loopback TCP tests, fragmented delivery, concurrent clients, malformed frames, and sanitizers all support the result.

## Scope

This service intentionally does not implement:

- TLS;
- authentication;
- authorization;
- persistence;
- application-specific commands;
- `epoll`;
- multi-process preforking;
- Internet-facing binding;
- production telemetry;
- rate limiting;
- distributed coordination.

The server binds only to loopback and is intended as a focused systems-software portfolio project demonstrating framing, robust socket I/O, bounded concurrency, malformed-client handling, and graceful shutdown.

## Result

**PASS**

The Network Service successfully completed strict compilation, real loopback TCP round-trip validation, intentionally fragmented frame delivery, 40-client concurrent execution, maximum-size frame handling, oversized-frame rejection, graceful shutdown verification, representative client/server execution, and ASan/UBSan validation on the development environment.
