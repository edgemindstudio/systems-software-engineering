# Domain 00 — Engineering Environment

This domain establishes the working environment and engineering workflow used throughout the rest of the Systems Software Engineering laboratory program.

The goal is not simply to install software.

The goal is to understand the tools involved in building systems software and to establish a reproducible environment where later work can be inspected, debugged, tested, and measured.

## Labs

### Lab 00-01 — Linux Engineering Environment

Establish and verify the Linux systems-development workstation.

Topics include:

- operating system inspection;
- CPU architecture;
- C and C++ compilers;
- build tools;
- debuggers;
- static-analysis tools;
- memory-analysis tools;
- tracing tools;
- performance tools;
- binary-inspection utilities;
- Git and GitHub tooling.

Key outcome:

The learner can verify that the workstation contains the engineering tools required for later systems work and can document the environment reproducibly.

### Lab 00-02 — Compilation Pipeline Investigation

Investigate how C source code becomes a runnable executable.

Pipeline:

```text
C source
    ↓
preprocessor
    ↓
preprocessed translation unit
    ↓
compiler
    ↓
assembly
    ↓
assembler
    ↓
relocatable object
    ↓
linker
    ↓
executable
```

Topics include:

- preprocessing;
- macro expansion;
- assembly generation;
- ELF object files;
- symbols;
- relocation;
- linking;
- linker failures;
- shared-library dependencies;
- optimization.

Key outcome:

The learner can explain and inspect the major stages between source code and an executable rather than treating compilation as a black box.

### Lab 00-03 — Git Engineering Workflow

Develop practical source-control skills used in engineering teams.

Planned topics include:

- branches;
- commits;
- merges;
- conflicts;
- rebasing;
- revert;
- regression investigation;
- `git bisect`;
- tags;
- repository history.

Key outcome:

The learner can modify a shared codebase while preserving understandable history and can use Git to investigate regressions.

### Lab 00-04 — Build System Foundations

Investigate how software builds are described, automated, and reproduced.

Planned topics include:

- direct compiler invocation;
- Make;
- CMake;
- Ninja;
- dependency tracking;
- incremental builds;
- build configuration;
- clean builds.

Key outcome:

The learner can understand and create reproducible builds rather than relying on manually typed compiler commands.

## Domain Completion Criteria

This domain is complete when the learner can:

- verify and document a Linux systems-development environment;
- explain how C source becomes an executable;
- inspect intermediate compilation artifacts;
- distinguish compilation from linking;
- use Git safely for normal engineering workflows and regression investigation;
- build a small project using standard build tooling.

## Current Progress

- Lab 00-01 — Complete
- Lab 00-02 — Complete
- Lab 00-03 — Not started
- Lab 00-04 — Not started