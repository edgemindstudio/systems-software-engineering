# Systems Software Engineering Roadmap

This roadmap defines the progression of the Systems Software Engineering laboratory program.

The sequence is intentionally practical and cumulative. Later sections assume competence developed in earlier sections.

## Domain 00 — Engineering Environment

Purpose:

Establish the engineering workstation and understand the development toolchain.

Labs:

- Lab 00-01 — Linux Engineering Environment
- Lab 00-02 — Compilation Pipeline Investigation
- Lab 00-03 — Git Engineering Workflow
- Lab 00-04 — Build System Foundations

Expected outcome:

The learner can configure and verify a Linux systems development environment, understand how source code becomes an executable, use Git safely in an engineering workflow, and work with common build systems.

## Domain 01 — C Systems Foundations

Labs:

- Lab 01-01 — Memory Representation
- Lab 01-02 — Pointer Mechanics
- Lab 01-03 — Stack Investigation
- Lab 01-04 — Dynamic Memory
- Lab 01-05 — Dynamic Array
- Lab 01-06 — Linked Structures
- Lab 01-07 — Hash Table

Expected outcome:

The learner can reason about memory representation, pointers, ownership, allocation, lifetime, and reusable data structures in C.

## Domain 02 — Library Engineering

Labs:

- Lab 02-01 — Multi-Module C Project
- Lab 02-02 — Error-Handling Architecture
- Lab 02-03 — Static Library
- Lab 02-04 — Shared Library
- Lab 02-05 — ABI Breakage Investigation

Expected outcome:

The learner can design modular C interfaces, separate implementation from public API, handle failures consistently, and understand static versus dynamic library behavior.

## Domain 03 — Linux File I/O

Labs:

- Lab 03-01 — Raw File Copy
- Lab 03-02 — File Descriptor Investigation
- Lab 03-03 — Seek and Sparse Files
- Lab 03-04 — Buffered vs Unbuffered I/O
- Lab 03-05 — File Metadata Utility
- Lab 03-06 — Directory Walker
- Lab 03-07 — Durable Write Investigation
- Lab 03-08 — mmap File Processor

Expected outcome:

The learner can work directly with Linux file descriptors, file metadata, directory traversal, durability concerns, memory-mapped files, and low-level I/O behavior.

## Domain 04 — Processes

Labs:

- Lab 04-01 — Process Explorer
- Lab 04-02 — fork Laboratory
- Lab 04-03 — exec Laboratory
- Lab 04-04 — Process Supervisor
- Lab 04-05 — Zombie and Orphan Investigation
- Lab 04-06 — Mini Shell

Expected outcome:

The learner can reason about process creation, execution, parent-child relationships, exit status, process lifecycle, and shell-like execution.

## Domain 05 — IPC and Signals

Labs:

- Lab 05-01 — Anonymous Pipes
- Lab 05-02 — Pipeline Executor
- Lab 05-03 — Named Pipes
- Lab 05-04 — Signals
- Lab 05-05 — Shared Memory IPC
- Lab 05-06 — IPC Service

Expected outcome:

The learner can build and debug communication between processes using common Unix IPC mechanisms.

## Domain 06 — Memory

Labs:

- Lab 06-01 — Process Address-Space Explorer
- Lab 06-02 — Page-Fault Investigation
- Lab 06-03 — mmap Anonymous Memory
- Lab 06-04 — Memory Allocator I
- Lab 06-05 — Memory Allocator II
- Lab 06-06 — Allocator Debugging
- Lab 06-07 — Allocator Benchmarking

Expected outcome:

The learner can reason about virtual memory, process layout, page behavior, allocation, fragmentation, allocator design, and allocator performance.

## Domain 07 — Concurrency

Labs:

- Lab 07-01 — Thread Fundamentals
- Lab 07-02 — Race Condition
- Lab 07-03 — Mutex Laboratory
- Lab 07-04 — Condition Variables
- Lab 07-05 — Thread Pool
- Lab 07-06 — Deadlock Laboratory
- Lab 07-07 — Reader/Writer Workload
- Lab 07-08 — Atomic Operations

Expected outcome:

The learner can build and debug multithreaded software and reason about race conditions, synchronization, deadlocks, coordination, and atomic operations.

## Domain 08 — Networking

Labs:

- Lab 08-01 — TCP Client
- Lab 08-02 — TCP Server
- Lab 08-03 — Protocol Design
- Lab 08-04 — Multi-Client Server
- Lab 08-05 — Nonblocking I/O
- Lab 08-06 — Event-Driven Server
- Lab 08-07 — Network Failure Simulation

Expected outcome:

The learner can build robust networked programs using sockets and handle partial I/O, malformed input, disconnects, concurrency, and scalable I/O models.

## Domain 09 — C++ Systems Engineering

Labs:

- Lab 09-01 — RAII
- Lab 09-02 — Copy and Move Semantics
- Lab 09-03 — Smart Pointer Investigation
- Lab 09-04 — Generic Containers
- Lab 09-05 — C API to C++ Wrapper
- Lab 09-06 — C++ Systems Utility

Expected outcome:

The learner can use modern C++ resource-management techniques for systems programming without losing sight of underlying system behavior.

## Domain 10 — Debugging

Labs:

- Lab 10-01 — Segmentation Fault Incident
- Lab 10-02 — Memory Leak Incident
- Lab 10-03 — Heap Corruption Incident
- Lab 10-04 — Descriptor Leak
- Lab 10-05 — Intermittent Concurrency Failure
- Lab 10-06 — Broken Build
- Lab 10-07 — Regression Hunt

Expected outcome:

The learner can investigate failures with incomplete information and select appropriate debugging techniques independently.

## Domain 11 — Performance

Labs:

- Lab 11-01 — Benchmarking Correctly
- Lab 11-02 — CPU Profiling
- Lab 11-03 — Algorithmic Optimization
- Lab 11-04 — Allocation Profiling
- Lab 11-05 — Cache Behavior
- Lab 11-06 — System-Call Analysis

Expected outcome:

The learner can measure system performance correctly, identify bottlenecks, optimize software, and provide evidence for improvements.

## Domain 12 — Binary and Runtime Fundamentals

Labs:

- Lab 12-01 — ELF Inspector
- Lab 12-02 — Basic ELF Parser
- Lab 12-03 — Symbol Investigation
- Lab 12-04 — Dynamic Loader Investigation
- Lab 12-05 — Assembly Inspection

Expected outcome:

The learner can inspect ELF binaries, symbols, sections, loading behavior, generated assembly, and executable structure.

## Incident Program

The incident program contains realistic engineering failures:

- Incident 01 — Memory Growth
- Incident 02 — File Corruption
- Incident 03 — Hanging Service
- Incident 04 — CPU Regression
- Incident 05 — Broken Deployment
- Incident 06 — Network Server Collapse
- Incident 07 — CI-Only Failure
- Incident 08 — Unfamiliar Repository Ticket
- Incident 09 — Security and Robustness Defect
- Incident 10 — Regression Investigation

## Major Projects

- Systems Utility Suite
- Unix Shell
- Memory Allocator
- Thread Runtime
- Network Service
- Binary Inspector
- Final Capstone

## Progression Rule

The roadmap is intended to be completed sequentially unless a later lab has clearly satisfied prerequisites through equivalent work.

The program should become progressively less guided as competence increases.