# Network Service

A concurrent framed TCP service implemented in C for Linux.

The project is intentionally more substantial than a socket echo exercise: it defines an explicit binary framing protocol, handles partial stream I/O, uses a bounded connection queue with worker threads, rejects malformed oversized frames, and shuts down gracefully on `SIGINT`/`SIGTERM`.

## Protocol

Each request and response is:

```text
4-byte unsigned big-endian payload length
payload bytes
```

Maximum payload size:

```text
4096 bytes
```

The server echoes each valid payload using the same framing.

Because TCP is a byte stream, the implementation never assumes one `read()` corresponds to one message. Header and payload reads use exact-length loops, and writes use a complete-write loop.

## Server

```bash
./build/netservice-server \
  --port 0 \
  --workers 4 \
  --queue 32 \
  --port-file workspace/port.txt
```

`--port 0` requests an ephemeral loopback port.

The server prints its effective address, port, worker count, and queue capacity.

## Client

```bash
./build/netservice-client PORT "hello"
```

Example response:

```text
response_length=5
response=hello
```

## Concurrency Model

The acceptor thread places accepted sockets into a bounded circular queue.

Worker threads remove connections from the queue and service framed requests.

When the queue is full, the acceptor waits for capacity. This bounds pending accepted connections inside the application.

## Graceful Shutdown

`SIGINT` and `SIGTERM` stop the listening socket, stop new queue insertion, wake blocked workers, drain already queued connections, join workers, and print final connection/protocol statistics.

`SIGPIPE` is ignored so peer disconnects become ordinary write errors rather than terminating the process.

## Tests

```bash
make test
```

The integration suite verifies:

- basic request/response;
- one-byte-at-a-time fragmented request delivery;
- 40 concurrent clients;
- maximum-size frame;
- oversized-frame rejection;
- graceful shutdown.

## Sanitizers

```bash
make sanitize
```

This rebuilds the service with AddressSanitizer and UndefinedBehaviorSanitizer and reruns the integration suite.

## Full Validation

```bash
./scripts/validate.sh
```

Validation evidence is written to:

```text
evidence/validation.txt
```

## Scope

The service is loopback-only and intentionally does not implement TLS, authentication, persistence, application-level authorization, multi-process preforking, `epoll`, or production observability infrastructure.

Its focus is protocol framing, robust stream I/O, bounded concurrency, connection lifecycle handling, malformed-client behavior, and shutdown correctness.
