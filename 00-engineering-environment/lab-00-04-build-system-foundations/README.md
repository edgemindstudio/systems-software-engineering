# Lab 00-04 — Build System Foundations

## Objective

Understand how a small multi-file C program progresses from direct compiler commands to reproducible automated builds using GNU Make, CMake, and Ninja.

## Engineering Context

Systems software projects rarely rely on manually typed compiler commands once they grow beyond trivial programs.

Build systems coordinate compilation, dependency tracking, incremental rebuilds, configuration, linking, and repeatable developer workflows.

This lab uses the same program across three build approaches:

```text
manual compiler commands
        ↓
GNU Make
        ↓
CMake → Ninja
```

## Prerequisites

- Lab 00-01 — Linux Engineering Environment
- Lab 00-02 — Compilation Pipeline Investigation
- Lab 00-03 — Git Engineering Workflow
- Basic C compilation knowledge

## Tools

- Clang
- GNU Make
- CMake
- Ninja
- Bash

## Repository Structure

```text
lab-00-04-build-system-foundations/
├── .gitignore
├── CMakeLists.txt
├── Makefile
├── README.md
├── evidence/
├── include/
│   └── math_ops.h
├── reports/
│   └── build_system_report.md
├── scripts/
│   └── build_all.sh
└── src/
    ├── main.c
    └── math_ops.c
```

## Program

The program performs two operations using a separate module:

- addition
- multiplication

Expected output:

```text
add: 14
multiply: 48
```

## Manual Build

The source files can be compiled separately into object files and then linked manually.

This demonstrates that each C source file is compiled independently before the linker combines the resulting object files.

## GNU Make

The Makefile automates:

- compilation;
- linking;
- generated dependency files;
- incremental rebuilds;
- testing;
- clean builds.

Compiler options include:

```text
-MMD -MP
```

which generate dependency files for header tracking.

If no source or dependency changed, Make should report that no work is required.

If only one implementation file changes, Make recompiles only the affected object file and relinks the executable.

If a shared header changes, all dependent translation units are rebuilt.

## CMake and Ninja

CMake is used to configure and generate a Ninja build tree.

CMake defines:

- project language;
- C standard;
- source files;
- include directories;
- compiler warning options.

Ninja then executes the generated build graph.

This demonstrates an important separation:

```text
CMake
    configures/generates

Ninja
    executes
```

## Debug and Release Builds

Separate CMake build directories can represent different configurations:

```text
build/cmake-debug/
build/cmake-release/
```

The same source tree can therefore produce independently configured build artifacts without modifying the source files.

## Build Automation

The convenience script:

```text
scripts/build_all.sh
```

performs a clean Make build, runs tests, configures CMake with Ninja, builds the Debug configuration, and executes the resulting program.

## Validation

The lab is complete when:

- the manual compiler build works;
- the Make build works;
- Make dependency tracking works;
- incremental rebuild behavior is observed;
- clean builds work;
- CMake configuration succeeds;
- Ninja builds successfully;
- the resulting executables produce the expected output;
- the automated build script completes successfully.

## Key Lessons

Build systems encode relationships between source files, generated artifacts, dependencies, compiler settings, and build targets.

Incremental builds avoid unnecessary compilation.

Dependency tracking ensures that source files are rebuilt when included headers change.

CMake does not replace the compiler or build executor. It generates a build configuration that tools such as Ninja can execute.

Separate build directories make it possible to maintain multiple configurations from one source tree.

## Result

Lab 00-04 establishes the build-system foundation used by later systems software projects in this repository.
