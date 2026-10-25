# Systems Software Engineering

A practical systems software engineering portfolio built around Linux, C, C++, operating-system interfaces, debugging, concurrency, networking, performance, binary/runtime analysis, incident investigation, and progressively larger systems projects.

The repository is organized around one engineering principle:

```text
Understand
    ↓
Build
    ↓
Run
    ↓
Observe
    ↓
Break
    ↓
Debug
    ↓
Measure
    ↓
Validate
    ↓
Document
```

The emphasis is not on isolated syntax exercises. The work is intended to demonstrate how low-level software is built, inspected, tested, debugged, measured, and explained using realistic engineering tools and evidence.

## Portfolio Status

The planned laboratory program, incident program, and seven major projects are complete.

```text
13 technical domains
82 labs
10 incident investigations
7 major projects
```

The final capstone integrates Unix-domain IPC, framed request/response communication, bounded worker-thread concurrency, `/proc` inspection, filesystem metadata, ELF inspection, graceful shutdown, concurrent-client testing, ASan/UBSan, and ThreadSanitizer validation.

## Technical Areas

The repository demonstrates practical work in:

- C systems programming
- modern C++ for systems work
- Linux system calls
- file descriptors and robust I/O
- process creation and execution
- signals and interprocess communication
- virtual memory and memory protection
- multithreading and synchronization
- atomic operations
- TCP, UDP, IPv6, framing, and nonblocking I/O
- debugging with GDB and LLDB
- Valgrind and sanitizers
- benchmarking and performance analysis
- ELF, symbols, relocations, linking, and loading
- static and shared libraries
- build systems and Git engineering workflow
- incident investigation
- engineering documentation
- reproducible validation

## Repository Structure

### Technical Domains

```text
00-engineering-environment
01-c-systems-foundations
02-library-engineering
03-linux-file-io
04-processes
05-ipc-signals
06-memory
07-concurrency
08-networking
09-cpp-systems
10-debugging
11-performance
12-binary-runtime
```

Each domain contains practical labs with source, validation, evidence, and documentation appropriate to the topic.

### Incident Program

```text
incidents/
```

The incident program contains ten failure investigations covering:

- file-descriptor leaks
- zombie processes
- pipe deadlocks
- use-after-free defects
- data races
- TCP framing failures
- shared-library lookup failures
- execute-permission failures
- sparse-file storage behavior
- tiny-write performance regressions

The incident work is intentionally less guided than normal labs and emphasizes reproduction, evidence collection, root-cause reasoning, correction, and validation.

### Major Projects

```text
projects/
├── systems-utility-suite/
├── unix-shell/
├── memory-allocator/
├── thread-runtime/
├── network-service/
├── binary-inspector/
└── final-capstone/
```

The major projects increase in scope and independence.

#### Systems Utility Suite

A C systems utility combining filesystem metadata inspection, robust file copying, recursive directory traversal, hexadecimal inspection, and `/proc` process information.

#### Unix Shell

A foreground Unix shell supporting external commands, quoted tokenization, N-stage pipelines, redirection, built-ins, exit-status propagation, and signal behavior.

#### Memory Allocator

A fixed-arena allocator implementing alignment, first-fit allocation, block splitting, adjacent coalescing, `calloc`, `realloc`, statistics, invariant validation, stress tests, and sanitizer-backed verification.

#### Thread Runtime

A bounded POSIX-thread worker runtime with multiple producers, backpressure, condition-variable coordination, graceful draining, statistics, high-volume testing, ASan/UBSan, and ThreadSanitizer validation.

#### Network Service

A concurrent framed TCP service with explicit length-prefix framing, partial-I/O handling, a bounded connection queue, worker threads, malformed-frame rejection, concurrent clients, and graceful shutdown.

#### Binary Inspector

A direct ELF64 parser that reads headers, sections, program headers, symbols, dynamic dependencies, and file-offset/virtual-address mappings without using binutils as its implementation.

#### Final Capstone — SysDiag

A local diagnostics daemon and CLI using Unix-domain sockets, framed IPC, bounded worker concurrency, `/proc` inspection, filesystem metadata, ELF inspection, structured errors, graceful shutdown, ASan/UBSan, and ThreadSanitizer.

## Engineering Evidence

The repository preserves evidence where it materially supports engineering conclusions.

Depending on the lab, incident, or project, this includes:

- compiler and linker diagnostics
- test output
- sanitizer output
- debugger observations
- traces
- binary inspection
- benchmark measurements
- profiler or system-call observations
- failure reproduction
- before-and-after comparisons
- validation transcripts
- engineering reports

Evidence is intended to show how a conclusion was reached rather than only presenting final source code.

## Validation Philosophy

Completion is based on demonstrated behavior, not merely successful compilation.

Where applicable, work is validated with combinations of:

```text
strict compiler warnings
automated tests
integration tests
stress tests
ASan
UBSan
ThreadSanitizer
Valgrind
GDB / LLDB
strace / ltrace
binutils
benchmarking
runtime evidence
```

Not every tool is appropriate for every artifact. Validation is selected according to the behavior and failure modes being investigated.

## Learning and Engineering Progression

The repository progresses from guided environment and language fundamentals toward increasingly independent engineering work:

```text
Foundational Lab
      ↓
Systems Lab
      ↓
Debugging / Measurement
      ↓
Incident Investigation
      ↓
Integration Project
      ↓
Final Capstone
```

Later work requires more judgment about:

- what behavior must be verified;
- which system interfaces are relevant;
- which debugging or measurement tools to use;
- what failure cases matter;
- what evidence is sufficient;
- what design tradeoffs should be documented.

## Repository Standards

Repository-wide standards are documented in:

- [`LAB_STANDARD.md`](LAB_STANDARD.md)
- [`CONTRIBUTING.md`](CONTRIBUTING.md)
- [`ROADMAP.md`](ROADMAP.md)

Individual domains, incidents, and major projects also contain their own READMEs and evidence.

## Development Environment

The repository has been developed and validated primarily on Linux under WSL2 using a modern GNU/LLVM systems toolchain.

Representative tools used across the work include:

```text
GCC
Clang
GNU Make
CMake
Ninja
GDB
LLDB
Valgrind
strace
ltrace
perf
binutils
clang-tidy
Cppcheck
Git
GitHub CLI
```

Platform-specific limitations are documented where they materially affect a lab or validation result.

## Engineering Goal

The purpose of this repository is to demonstrate practical readiness for systems software engineering work by showing the ability to:

- enter and understand a Linux/C/C++ development environment;
- work with low-level operating-system interfaces;
- reason about memory, ownership, processes, concurrency, and I/O;
- design bounded and testable systems components;
- reproduce and diagnose failures;
- use debugging and analysis tools deliberately;
- measure performance rather than guess;
- inspect executable/runtime behavior;
- validate corrections with evidence;
- communicate engineering decisions clearly.

The repository is complete as a foundational systems-software engineering program and remains suitable for further extension with deeper flagship projects, platform-specific work, kernel-facing systems, or production-scale infrastructure.
