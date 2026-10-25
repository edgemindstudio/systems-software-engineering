# Network Service Architecture

## Components

```text
client.c
   |
   +-- protocol.c  <---- shared framing and robust I/O
   |
server.c
   |
   +-- acceptor
   +-- bounded connection queue
   +-- worker threads
```

## Framing

TCP preserves byte order but not application message boundaries.

The protocol therefore prefixes every payload with a 32-bit unsigned length in network byte order.

`netservice_read_exact()` loops until the requested number of bytes have arrived, EOF is observed, or a non-recoverable error occurs.

`netservice_write_all()` similarly handles partial writes and `EINTR`.

## Limits

Frames larger than 4096 bytes are rejected before payload allocation or reading.

The service uses a fixed stack buffer of exactly the protocol maximum for each worker's active connection.

## Connection Queue

Accepted descriptors enter a fixed-capacity circular queue protected by a mutex.

Two conditions coordinate the queue:

- `not_empty` wakes workers;
- `not_full` wakes the acceptor.

The queue therefore provides application-level backpressure.

## Worker Model

Each worker repeatedly:

1. waits for a connection;
2. removes one descriptor;
3. reads framed requests;
4. writes framed responses;
5. terminates the connection on EOF or protocol/I/O failure;
6. closes the descriptor.

One worker owns a connection for its lifetime.

## Shutdown

Signal handlers set a `sig_atomic_t` stop flag and close the listening descriptor.

The accept loop exits, the queue enters stopping mode, workers drain queued descriptors, and all workers are joined.

Final statistics report accepted connections, completed connections, protocol errors, and graceful shutdown status.

## Testing Strategy

The integration test starts the real server on an ephemeral loopback port and uses real TCP clients.

It explicitly tests fragmented delivery by sending an entire frame one byte at a time. This validates that the implementation does not rely on packet/message alignment.

The concurrent-client test runs 40 independent client connections through a thread pool.

The malformed-client test sends a length of 4097 bytes, one byte beyond the protocol maximum, and verifies that the server closes the connection without sending a response.
