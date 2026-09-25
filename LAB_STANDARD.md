# Systems Software Engineering Laboratory Standard

This document defines the standard used when creating and completing labs in this repository.

## Purpose

Each lab should develop a practical systems-engineering competency.

Labs should avoid becoming simple tutorials where the learner follows commands without understanding the engineering problem.

The learner should increasingly be required to investigate, reason, test, and explain.

## Core Lab Structure

A lab README should normally contain:

### Objective

A clear statement of what the lab develops.

### Engineering Context

Why this skill matters in real systems work.

### Prerequisites

Knowledge or earlier labs expected before beginning.

### Tools

The primary engineering tools used in the lab.

### Tasks

The work that must be performed.

### Deliverables

The files, code, reports, evidence, or measurements that must be produced.

### Validation

How correctness is demonstrated.

### Completion Criteria

The conditions that must be satisfied before the lab is considered complete.

## Suggested Lab Directories

Labs may contain:

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

Empty structure should not be added simply for consistency.

## Engineering Quality Standard

Where applicable, completed labs should demonstrate:

- successful builds;
- appropriate compiler warnings enabled;
- correct error handling;
- clean execution;
- automated tests;
- sanitizer checks;
- static analysis;
- reproducible commands;
- documented observations;
- meaningful commit history.

## Investigation Standard

Investigation labs should preserve evidence.

Examples include:

- debugger output;
- traces;
- compiler diagnostics;
- linker diagnostics;
- benchmark results;
- profiler output;
- binary inspection;
- failure reproduction;
- before-and-after comparisons.

The final report should explain what the evidence means.

## Debugging Standard

A debugging lab should demonstrate:

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

The learner should avoid changing code randomly until the symptom disappears.

## Performance Standard

Performance claims should be supported by measurement.

Where practical, reports should include:

- baseline;
- methodology;
- workload;
- measurement;
- change;
- new measurement;
- interpretation;
- tradeoffs.

## Incident Standard

Incident labs should provide less guidance than normal labs.

An incident may provide:

- a repository;
- an issue report;
- logs;
- a reproduction command;
- failing tests;
- performance symptoms;
- environmental information.

The learner should determine the appropriate investigation strategy.

## Documentation Standard

Reports should explain the learner's own observations and reasoning.

They should not simply repeat commands.

A useful report answers:

- What happened?
- Why did it happen?
- What evidence supports the conclusion?
- What changed?
- How was the result validated?
- What was learned?

## Completion

A lab is complete when the required behavior is correct, the evidence is preserved, and the learner can explain the engineering concepts demonstrated by the work.