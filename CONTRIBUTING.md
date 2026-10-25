# Contributing

This repository is a practical systems software engineering portfolio and laboratory program.

Contributions should preserve its engineering-first purpose, evidence discipline, reproducibility, and low-level systems focus.

## General Principles

New material should:

- develop or demonstrate a clear systems-engineering competency;
- use realistic tools and operating-system interfaces;
- require reasoning rather than only command repetition;
- define meaningful completion criteria;
- preserve reproducibility;
- include appropriate failure handling;
- validate behavior with evidence;
- avoid unnecessary abstraction;
- document important engineering observations and limitations.

## Scope

Material should have a clear connection to one or more of:

- low-level software construction;
- Linux operating-system interfaces;
- C or C++ systems programming;
- memory and resource ownership;
- processes and IPC;
- concurrency;
- networking;
- debugging;
- performance;
- executable/runtime behavior;
- build/toolchain engineering;
- incident investigation;
- systems-oriented testing and validation.

## Adding a Lab

A proposed lab should identify:

- target domain;
- competency being developed;
- prerequisites;
- engineering context;
- required tools;
- tasks;
- deliverables;
- validation strategy;
- completion criteria.

Labs should follow [`LAB_STANDARD.md`](LAB_STANDARD.md).

## Adding an Incident

An incident should begin from a realistic failure condition rather than from a step-by-step solution.

It should define enough information to reproduce or observe the symptom while leaving investigation strategy to the engineer.

A completed incident should preserve:

```text
symptom
evidence
root cause
correction
validation
```

## Adding or Extending a Major Project

Major projects should integrate multiple systems competencies and should be more independent than labs.

Where appropriate, project structure should include:

```text
README.md
docs/architecture.md
docs/engineering_report.md
evidence/README.md
evidence/validation.txt
```

Projects should include automated validation appropriate to their behavior and failure modes.

## Repository Naming

Lab directories use:

```text
lab-DD-NN-short-description
```

For example:

```text
lab-03-01-file-descriptors-robust-io
```

Incident directories use:

```text
inc-NN-short-description
```

For example:

```text
inc-06-tcp-framing-failure
```

Major project directories use concise lowercase hyphenated names, for example:

```text
thread-runtime
network-service
binary-inspector
```

## Code Quality

Code should compile cleanly with warning levels appropriate to the language and task.

Systems code should explicitly consider:

- error paths;
- resource ownership;
- cleanup;
- bounds;
- partial I/O;
- process/thread lifecycle;
- concurrency hazards;
- malformed input;
- portability assumptions;
- shutdown behavior.

Tests should be added where they provide meaningful verification.

## Validation Quality

Validation should match the artifact.

Depending on the work, appropriate tools may include:

```text
unit tests
integration tests
stress tests
ASan
UBSan
ThreadSanitizer
Valgrind
GDB
LLDB
strace
ltrace
perf
readelf
nm
objdump
static analysis
benchmarks
```

A tool should be used because it supports a relevant engineering claim, not simply to increase the number of tools listed.

## Evidence

Reports and evidence files must preserve actual observations.

Measured results must not be invented, estimated as if measured, or generalized beyond the executed workload.

Transient generated files should only be committed when they serve a clear evidentiary or reproducibility purpose.

## Generated Artifacts

Build directories, binaries, object files, temporary workspaces, runtime socket files, and similar generated outputs should normally remain untracked.

Before committing, verify that generated artifacts have not entered the staged set unintentionally.

## Commit Quality

Commits should describe meaningful units of engineering work.

Examples:

```text
Complete compilation pipeline investigation lab

Add allocator coalescing tests

Fix partial-write handling in file copy utility

Document deadlock root-cause investigation

Complete network service project
```

Avoid vague messages such as:

```text
update
changes
fix stuff
```

## Documentation Quality

Documentation should be specific to the implemented behavior.

Avoid unsupported claims such as:

- “production ready” without production evidence;
- “race free” without an appropriate basis;
- “fast” without measurement;
- “secure” without a defined threat model and evidence.

Prefer precise statements such as:

```text
The integration suite passed under ThreadSanitizer with no reported data race.

The benchmark measured X under the documented workload.

The parser rejects frames larger than the configured maximum.
```

## Reports

Engineering reports should preserve actual observations and explain why the evidence matters.

A report should distinguish:

- observed behavior;
- interpretation;
- design decision;
- limitation.

## Repository Integrity

Before a contribution is committed:

1. run the relevant validation;
2. preserve required evidence;
3. clean generated build/workspace artifacts;
4. run whitespace checks;
5. inspect the staged file list;
6. verify that only intended files are staged;
7. use a meaningful commit message.

## Current Repository State

The foundational program is complete across:

```text
13 technical domains
10 incident investigations
7 major projects
```

Future contributions should generally deepen the repository rather than duplicate already-completed foundational coverage.

Good extension areas include advanced Linux internals, kernel-facing interfaces, storage systems, compilers/toolchains, event-driven services, runtimes, observability, performance tooling, and production-oriented systems components.
