# Systems Software Engineering Laboratory Standard

This document defines the engineering standard used for labs in this repository and the baseline quality expectations that also inform incidents and larger projects.

## Purpose

Each lab should develop a practical systems-engineering competency.

Labs should not become command-following tutorials where successful completion is disconnected from understanding.

The expected cycle is:

```text
Understand the system concept
        ↓
Build or run the artifact
        ↓
Verify important behavior
        ↓
Capture evidence
        ↓
Explain the result
```

Later labs should require increasing independence in investigation, tool selection, validation, and interpretation.

## Core Lab Structure

A lab README should normally contain the following sections.

### Objective

A clear statement of the systems competency being developed.

### Engineering Context

Why the behavior or interface matters in real systems work.

### Prerequisites

Knowledge, tools, or earlier work expected before beginning.

### Tools

The primary compiler, debugger, tracer, sanitizer, profiler, build system, or operating-system interfaces used.

### Tasks

The engineering work that must be performed.

### Deliverables

The source, tests, evidence, reports, measurements, or other artifacts that must be produced.

### Validation

How correctness or the investigated behavior is demonstrated.

### Completion Criteria

The conditions that must be satisfied before the lab is considered complete.

## Suggested Lab Directories

A lab may contain:

```text
src/
include/
tests/
scripts/
artifacts/
evidence/
reports/
benchmarks/
docs/
```

Directories should only be created when they serve a real purpose.

Empty structure should not be added merely for visual consistency.

## Engineering Quality Standard

Where applicable, completed work should demonstrate:

- successful reproducible builds;
- appropriate compiler warning levels;
- correct error handling;
- deliberate resource ownership and cleanup;
- clean execution;
- meaningful automated tests;
- integration tests where component interaction matters;
- sanitizer checks where memory, undefined behavior, or concurrency is relevant;
- static analysis where it materially helps;
- reproducible commands;
- preserved runtime evidence;
- documented observations and limitations.

Not every lab needs every validation technique. The selected validation should match the relevant failure modes.

## Evidence Standard

Evidence should be produced by actual execution.

Examples include:

- compiler diagnostics;
- linker diagnostics;
- debugger output;
- sanitizer reports;
- Valgrind output;
- traces;
- benchmark measurements;
- profiler output;
- system-call observations;
- binary inspection;
- failure reproduction;
- before-and-after comparisons;
- integration-test transcripts.

Evidence should support a specific engineering conclusion.

Generated files should not be committed merely because a tool produced them.

## Debugging Standard

A debugging lab should demonstrate a disciplined investigation loop:

```text
reproduce
    ↓
observe
    ↓
form hypothesis
    ↓
collect evidence
    ↓
identify root cause
    ↓
implement correction
    ↓
validate
    ↓
document
```

Random code modification until a symptom disappears does not satisfy the debugging standard.

## Performance Standard

Performance claims must be supported by measurement.

Where practical, the work should identify:

- workload;
- baseline;
- measurement method;
- repeated trials where appropriate;
- observed result;
- proposed or implemented change;
- new measurement;
- interpretation;
- tradeoffs and limitations.

Compiler optimization must be considered when benchmarking; work that can be optimized away is not a valid performance measurement.

## Concurrency Standard

Concurrency work should explicitly consider:

- ownership of shared state;
- synchronization mechanism;
- wakeup conditions;
- shutdown behavior;
- bounded versus unbounded work;
- race and deadlock risks.

Where supported by the environment, ThreadSanitizer or another race-detection method should be used for meaningful concurrent artifacts.

## Networking and IPC Standard

Stream-oriented protocols should define their own message boundaries rather than assuming one read equals one message.

Where applicable, work should test:

- partial reads and writes;
- framing;
- disconnects;
- malformed input;
- concurrent clients;
- lifecycle and shutdown behavior;
- bounded queues or other resource limits.

## Memory-Safety Standard

Code that manipulates dynamic memory, raw buffers, binary formats, or ownership-sensitive resources should validate:

- bounds;
- lifetimes;
- error paths;
- cleanup;
- allocation failures where practical.

ASan and UBSan should be used when they materially strengthen confidence in the artifact.

## Binary-Parsing Standard

Binary-format work should not blindly trust offsets, lengths, indexes, or string references.

Before dereferencing file-backed metadata, parsers should validate relevant bounds and representation assumptions.

Unsupported formats should be rejected explicitly rather than silently misinterpreted.

## Investigation Standard

Investigation labs should preserve the evidence necessary to reconstruct the reasoning.

The final report should explain what the evidence means, not simply reproduce terminal output.

## Incident Standard

Incident work should provide less guidance than normal labs.

An incident may begin with:

- a repository;
- an issue report;
- logs;
- a failing command;
- failing tests;
- performance symptoms;
- environmental information.

The engineer is expected to determine the investigation strategy.

A completed incident should normally establish:

```text
symptom
root cause
evidence
correction
validation
```

## Project Standard

Major projects should integrate multiple earlier competencies and should be more independent than labs.

A project should normally include:

```text
README.md
docs/architecture.md
docs/engineering_report.md
evidence/README.md
evidence/validation.txt
```

along with source, tests, scripts, and build files appropriate to the project.

Project completion should demonstrate not only feature behavior but also failure handling, reproducibility, cleanup, and engineering scope.

## Documentation Standard

Reports should explain actual observations and reasoning.

A useful engineering report answers:

- What was built or investigated?
- Why does the behavior matter?
- What evidence was collected?
- What failed or could fail?
- What design decisions were made?
- How was the result validated?
- What limitations remain?

Measured values must not be invented or generalized beyond the evidence produced during execution.

## Repository Hygiene

Generated build directories, binaries, object files, temporary workspaces, transient socket files, logs, and similar runtime artifacts should normally remain untracked unless a specific artifact is intentionally preserved as evidence.

Before committing completed work, check:

```text
working-tree status
whitespace errors
staged-file list
generated-artifact leakage
validation evidence
```

## Completion

A lab is complete when:

- the required behavior is correct;
- important failure paths have been considered;
- validation is appropriate to the task;
- evidence is preserved where needed;
- generated artifacts are under control;
- documentation explains what was demonstrated.

The standard is not merely “the program ran.” The artifact should support a defensible engineering conclusion.
