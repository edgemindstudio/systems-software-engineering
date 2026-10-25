# SysDiag Architecture

## Overview

```text
             +----------------+
             |    sysdiag     |
             |      CLI       |
             +--------+-------+
                      |
              framed AF_UNIX
                      |
             +--------v-------+
             | sysdiag-daemon |
             +--------+-------+
                      |
                acceptor thread
                      |
             +--------v-------+
             | bounded fd     |
             | circular queue |
             +--------+-------+
                      |
          +-----------+-----------+
          |           |           |
       worker      worker      worker
          |           |           |
          +---- diagnostics -------+
                |      |      |
              /proc   stat   ELF
```

## IPC

The transport is an `AF_UNIX` stream socket.

The protocol uses a 32-bit big-endian length prefix so message boundaries do not depend on stream read boundaries.

Exact-read and write-all helpers handle partial I/O and `EINTR`.

## Concurrency

The acceptor thread performs only connection acceptance and queue insertion.

A bounded circular queue transfers descriptor ownership to workers.

When the queue is full, the acceptor waits on `not_full`.

When the queue is empty, workers wait on `not_empty`.

This creates explicit backpressure and bounds pending application work.

## Request Lifecycle

Each connection carries exactly one request and one response.

A worker:

1. removes a descriptor from the queue;
2. reads the framed request;
3. rejects embedded NUL/binary requests;
4. dispatches the command;
5. writes one framed structured response;
6. closes the descriptor;
7. updates completion/error accounting.

## Diagnostics Layer

The diagnostics module is independent of the socket lifecycle.

It implements:

- `PING`
- `PROC`
- `STAT`
- `ELF`

This separation keeps transport/concurrency concerns distinct from operating-system inspection logic.

## PROC

`PROC` reads selected fields from `/proc/<pid>/status`.

For `PROC self`, `self` refers to the daemon process because the diagnostic executes inside the daemon.

## STAT

`STAT` uses `lstat()` and reports object type, byte size, mode bits, inode, and hard-link count.

Symlinks are inspected as symlinks.

## ELF

`ELF` reads one `Elf64_Ehdr` directly and validates:

- magic
- ELF64 class
- little-endian encoding

It then reports selected header values.

This capstone intentionally performs header-level ELF inspection; the dedicated Binary Inspector project contains the deeper ELF table parser.

## Shutdown

`SIGINT` or `SIGTERM`:

1. sets a `sig_atomic_t` stop flag;
2. closes the listening socket;
3. ends the accept loop;
4. marks the queue stopping;
5. wakes blocked workers/acceptor state;
6. drains queued requests;
7. joins workers;
8. prints final statistics;
9. unlinks the Unix socket path.

## Validation Strategy

The capstone combines:

- strict-warning compilation;
- real Unix-domain socket integration;
- concurrent-client stress;
- process/filesystem/ELF diagnostics;
- representative CLI execution;
- ASan/UBSan;
- ThreadSanitizer.

The intent is to validate both subsystem behavior and subsystem integration.
