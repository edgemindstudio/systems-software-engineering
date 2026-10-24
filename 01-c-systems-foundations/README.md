# 01 — C Systems Foundations

## Purpose

This section develops the C-level reasoning expected in systems software engineering: memory representation, pointer semantics, stack behavior, dynamic allocation, ownership, data-structure implementation, defensive error handling, testing, and runtime validation.

The labs are intentionally small enough to inspect completely but substantial enough to produce engineering evidence rather than isolated syntax exercises.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- reason about bytes, object representation, alignment, and endianness;
- use pointers and pointer arithmetic safely;
- distinguish stack lifetime from dynamic lifetime;
- manage heap allocation and ownership explicitly;
- design a reusable dynamic array;
- implement linked structures with clear invariants;
- implement a chained hash table with resizing;
- use compiler warnings and sanitizers as engineering tools;
- write repeatable tests and validation scripts;
- document implementation decisions and failure modes.

## Labs

| Lab | Topic | Primary Evidence |
|---|---|---|
| 01-01 | Memory Representation | byte layout, endianness, alignment, padding |
| 01-02 | Pointer Mechanics | pointer arithmetic, indirection, mutation through pointers |
| 01-03 | Stack Investigation | frames, recursion, lifetime, stack addresses |
| 01-04 | Dynamic Memory | allocation, ownership, cleanup, sanitizer validation |
| 01-05 | Dynamic Array | reusable vector-style container, growth policy |
| 01-06 | Linked Structures | singly linked list, invariants, insertion/removal |
| 01-07 | Hash Table | separate chaining, hashing, replacement, resizing |

## Quality Standard

Every lab contains:

- source code;
- a strict-warning Makefile;
- automated tests;
- a repeatable validation script;
- an evidence directory;
- an engineering report;
- a lab README explaining what the implementation proves.

Generated binaries and build artifacts are ignored. Runtime validation evidence is produced on the development machine and preserved separately from generated build artifacts.

## Tooling

The section uses the engineering environment established in Domain 00:

- Clang
- GNU Make
- Bash
- AddressSanitizer
- UndefinedBehaviorSanitizer
- GDB for optional inspection
- Valgrind when useful for additional runtime analysis

## Validation

From this directory:

```bash
./scripts/validate_all.sh
```

The script performs clean builds, executes each program, runs automated tests, and validates both the demonstration programs and test binaries with AddressSanitizer and UndefinedBehaviorSanitizer.

## Completed Results

All seven labs passed on the development environment using Clang 21.1.8.

The completed section demonstrated:

- little-endian byte representation and observable structure padding/alignment;
- pointer traversal, pointer-to-pointer lookup, mutation, and array reversal;
- distinct recursive automatic-storage addresses and instrumentation-dependent stack-address trends;
- explicit dynamic-memory ownership with successful growth and cleanup;
- dynamic-array growth, insertion, indexed access, and removal;
- linked-list insertion, search, removal, and full cleanup;
- hash-table insertion, replacement, lookup, deletion, collision handling, and resizing.

All labs completed clean builds, automated tests, and AddressSanitizer/UndefinedBehaviorSanitizer validation without reported warnings or runtime errors.

## Progress

- Lab 01-01 — Memory Representation: Complete
- Lab 01-02 — Pointer Mechanics: Complete
- Lab 01-03 — Stack Investigation: Complete
- Lab 01-04 — Dynamic Memory: Complete
- Lab 01-05 — Dynamic Array: Complete
- Lab 01-06 — Linked Structures: Complete
- Lab 01-07 — Hash Table: Complete

Domain 01 — C Systems Foundations: Complete
