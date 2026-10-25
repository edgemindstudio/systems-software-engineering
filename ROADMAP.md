# Systems Software Engineering Roadmap

This roadmap records the completed progression of the Systems Software Engineering laboratory program.

The sequence is practical and cumulative. Later sections build on engineering competence established in earlier domains.

## Completion Summary

```text
Domain 00 — Engineering Environment        COMPLETE
Domain 01 — C Systems Foundations          COMPLETE
Domain 02 — Library Engineering            COMPLETE
Domain 03 — Linux File I/O                 COMPLETE
Domain 04 — Processes                      COMPLETE
Domain 05 — IPC and Signals                COMPLETE
Domain 06 — Memory                         COMPLETE
Domain 07 — Concurrency                    COMPLETE
Domain 08 — Networking                     COMPLETE
Domain 09 — C++ Systems Engineering        COMPLETE
Domain 10 — Debugging                      COMPLETE
Domain 11 — Performance                    COMPLETE
Domain 12 — Binary and Runtime             COMPLETE
Incident Program                           COMPLETE
Major Projects                             COMPLETE
Final Capstone                             COMPLETE
```

## Domain 00 — Engineering Environment — COMPLETE

Purpose:

Establish the engineering workstation and understand the development toolchain.

Completed labs:

- Lab 00-01 — Linux Engineering Environment
- Lab 00-02 — Compilation Pipeline Investigation
- Lab 00-03 — Git Engineering Workflow
- Lab 00-04 — Build System Foundations

Outcome:

A verified Linux systems-development environment, practical understanding of the compilation pipeline, disciplined Git workflow, and working knowledge of common build-system foundations.

## Domain 01 — C Systems Foundations — COMPLETE

Completed labs:

- Lab 01-01 — Memory Representation
- Lab 01-02 — Pointer Mechanics
- Lab 01-03 — Stack Investigation
- Lab 01-04 — Dynamic Memory
- Lab 01-05 — Dynamic Array
- Lab 01-06 — Linked Structures
- Lab 01-07 — Hash Table

Outcome:

Practical reasoning about memory representation, pointers, ownership, allocation, lifetime, and reusable C data structures.

## Domain 02 — Library Engineering — COMPLETE

Completed labs:

- Lab 02-01 — Multi-Module C Project
- Lab 02-02 — Error-Handling Architecture
- Lab 02-03 — Static Library
- Lab 02-04 — Shared Library
- Lab 02-05 — ABI Breakage Investigation

Outcome:

Modular C interfaces, public/private implementation boundaries, consistent failure handling, static/shared linking, and ABI reasoning.

## Domain 03 — Linux File I/O — COMPLETE

Completed labs:

- Lab 03-01 — File Descriptors and Robust I/O
- Lab 03-02 — File Offsets and Descriptor Duplication
- Lab 03-03 — File Metadata and Links
- Lab 03-04 — Directory Traversal
- Lab 03-05 — Sparse Files
- Lab 03-06 — Vectored I/O
- Lab 03-07 — Memory-Mapped File I/O
- Lab 03-08 — Advisory File Locking

Outcome:

Direct work with Linux file descriptors, robust reads/writes, offsets, duplicated descriptors, metadata, links, traversal, sparse storage, vectored I/O, `mmap`, and advisory locking.

## Domain 04 — Processes — COMPLETE

Completed labs:

- Lab 04-01 — `fork()` and Copy-on-Write
- Lab 04-02 — `exec()` Image Replacement
- Lab 04-03 — `waitpid()` and Child Reaping
- Lab 04-04 — Process Trees and Ancestry
- Lab 04-05 — `fork()` / `exec()` Inheritance
- Lab 04-06 — Process Groups and Sessions

Outcome:

Practical understanding of process creation, image replacement, lifecycle management, reaping, ancestry, descriptor/state inheritance, process groups, and sessions.

## Domain 05 — IPC and Signals — COMPLETE

Completed labs:

- Lab 05-01 — Anonymous Pipes
- Lab 05-02 — Multi-Process Pipeline
- Lab 05-03 — Safe Signal Handling
- Lab 05-04 — Signal Masks and `sigsuspend()`
- Lab 05-05 — Named FIFO
- Lab 05-06 — POSIX Shared Memory

Outcome:

Practical interprocess communication and signal coordination using pipes, FIFOs, shared memory, signal handlers, signal masks, and race-resistant waiting patterns.

## Domain 06 — Memory — COMPLETE

Completed labs:

- Lab 06-01 — Virtual Address Space
- Lab 06-02 — Anonymous `mmap()`
- Lab 06-03 — Memory Protection and Guard Pages
- Lab 06-04 — `fork()` Copy-on-Write Memory
- Lab 06-05 — Page Residency and Faults
- Lab 06-06 — `madvise()` and Reclamation
- Lab 06-07 — Address-Space Limits

Outcome:

Practical understanding of virtual memory, mappings, memory protection, copy-on-write, residency, page-fault behavior, reclamation hints, and address-space constraints.

## Domain 07 — Concurrency — COMPLETE

Completed labs:

- Lab 07-01 — Thread Lifecycle
- Lab 07-02 — Mutex-Protected Counter
- Lab 07-03 — Condition Variables
- Lab 07-04 — Bounded Producer/Consumer
- Lab 07-05 — Atomic Counter
- Lab 07-06 — Race Detection
- Lab 07-07 — Reader/Writer Lock
- Lab 07-08 — Bounded Worker Pool

Outcome:

Practical multithreaded engineering with lifecycle management, mutexes, condition variables, bounded queues, atomics, race detection, reader/writer synchronization, and worker pools.

## Domain 08 — Networking — COMPLETE

Completed labs:

- Lab 08-01 — Address Resolution
- Lab 08-02 — TCP Loopback Echo
- Lab 08-03 — UDP Datagrams
- Lab 08-04 — Length-Prefixed Framing
- Lab 08-05 — Nonblocking `poll()`
- Lab 08-06 — IPv6 Loopback
- Lab 08-07 — Threaded TCP Service

Outcome:

Socket programming across TCP, UDP, IPv4/IPv6, explicit application framing, nonblocking I/O, readiness notification, concurrent clients, and service lifecycle handling.

## Domain 09 — C++ Systems Engineering — COMPLETE

Completed labs:

- Lab 09-01 — RAII File Descriptor
- Lab 09-02 — Move Semantics
- Lab 09-03 — Smart-Pointer Ownership
- Lab 09-04 — `std::system_error` Handling
- Lab 09-05 — Small Vector
- Lab 09-06 — Thread-Safe Queue

Outcome:

Modern C++ systems programming with RAII, move semantics, ownership models, system errors, custom containers, and synchronized data structures.

## Domain 10 — Debugging — COMPLETE

Completed labs:

- Lab 10-01 — GDB Segmentation-Fault Analysis
- Lab 10-02 — LLDB Crash Analysis
- Lab 10-03 — Valgrind Heap Diagnostics
- Lab 10-04 — AddressSanitizer
- Lab 10-05 — UndefinedBehaviorSanitizer
- Lab 10-06 — Assertions and Invariants
- Lab 10-07 — Defect Isolation

Outcome:

Practical failure investigation using debuggers, dynamic analysis, sanitizers, assertions, invariants, and systematic defect isolation.

## Domain 11 — Performance — COMPLETE

Completed labs:

- Lab 11-01 — Monotonic Timing
- Lab 11-02 — Repeated Benchmark Statistics
- Lab 11-03 — Cache Locality
- Lab 11-04 — System-Call Amortization
- Lab 11-05 — Allocation Reuse
- Lab 11-06 — Optimization Effects

Outcome:

Measurement-driven performance engineering using monotonic timing, repeated trials, locality experiments, syscall-cost analysis, allocation reuse, and compiler-optimization comparisons.

## Domain 12 — Binary and Runtime Fundamentals — COMPLETE

Completed labs:

- Lab 12-01 — ELF Header Inspection
- Lab 12-02 — Sections and Symbols
- Lab 12-03 — Relocations
- Lab 12-04 — Dynamic Dependencies
- Lab 12-05 — Shared-Library Runtime

Outcome:

Practical understanding of ELF headers, sections, symbols, relocations, dynamic dependencies, and runtime shared-library behavior.

## Incident Program — COMPLETE

Completed investigations:

- Incident 01 — File Descriptor Leak
- Incident 02 — Zombie Child
- Incident 03 — Pipe Deadlock
- Incident 04 — Use-After-Free
- Incident 05 — Data Race
- Incident 06 — TCP Framing Failure
- Incident 07 — Shared-Library Lookup
- Incident 08 — Execute Permission
- Incident 09 — Sparse-File Space
- Incident 10 — Tiny-Write Regression

Outcome:

Less-guided debugging and investigation across resource leaks, process lifecycle, IPC deadlock, memory safety, concurrency, protocol correctness, dynamic linking, permissions, filesystem allocation, and performance regression.

## Major Projects — COMPLETE

### 1. Systems Utility Suite

Combines filesystem metadata, robust file copy, recursive traversal, hexadecimal inspection, and `/proc` process information.

### 2. Unix Shell

Implements external command execution, quoting, pipelines, redirection, parent/child built-in behavior, signal behavior, and exit-status propagation.

### 3. Memory Allocator

Implements a fixed-arena allocator with alignment, splitting, coalescing, `calloc`, `realloc`, invariant validation, statistics, stress testing, and sanitizer validation.

### 4. Thread Runtime

Implements a bounded worker runtime with multiple producers, queue backpressure, condition-variable coordination, graceful draining, statistics, stress testing, ASan/UBSan, and ThreadSanitizer.

### 5. Network Service

Implements a concurrent length-prefixed TCP service with robust partial I/O, bounded connection queueing, worker threads, malformed-frame rejection, concurrent-client testing, and graceful shutdown.

### 6. Binary Inspector

Implements direct ELF64 parsing for headers, sections, segments, symbols, dynamic dependencies, and file-offset/virtual-address mapping with selected `readelf` parity checks.

### 7. Final Capstone — SysDiag

Integrates Unix-domain sockets, framed IPC, worker-thread concurrency, bounded queues, `/proc` inspection, filesystem metadata, ELF inspection, structured errors, concurrent clients, graceful shutdown, ASan/UBSan, and ThreadSanitizer.

## Progression Rule

The roadmap was completed sequentially, with later work progressively requiring more independent engineering judgment.

The intended progression remains:

```text
Foundational Lab
      ↓
Systems Lab
      ↓
Debugging / Measurement
      ↓
Incident Investigation
      ↓
Major Project
      ↓
Final Capstone
```

## Current State

The foundational program represented by this roadmap is complete.

Future work should extend depth rather than repeat foundational breadth. Suitable directions include:

- deeper Linux internals;
- kernel-facing interfaces;
- advanced event-driven networking;
- storage engines;
- allocators and runtimes;
- compilers/toolchains;
- distributed systems infrastructure;
- observability and performance tooling;
- production-oriented C/C++ systems components.
