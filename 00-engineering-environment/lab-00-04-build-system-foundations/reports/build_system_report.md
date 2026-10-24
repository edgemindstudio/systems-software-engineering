# Build System Foundations Report

## Overview

This lab compared direct compiler commands, GNU Make, and CMake with Ninja using the same small multi-file C program.

The goal was to understand why build systems exist and how they improve repeatability, dependency handling, incremental compilation, and build configuration.

## Source Layout

The program is split into:

```text
src/main.c
src/math_ops.c
include/math_ops.h
```

`main.c` uses functions declared in `math_ops.h` and implemented in `math_ops.c`.

## Manual Build

The source files can be compiled independently into:

```text
main.o
math_ops.o
```

and then linked into one executable.

This demonstrates that a multi-file C build is fundamentally a dependency graph of compilation and linking operations.

## GNU Make

The Makefile describes the relationships between source files, object files, dependency files, and the final executable.

Generated `.d` files track header dependencies.

An unchanged project requires no rebuild work.

Changing one implementation file causes only the affected translation unit to be recompiled before relinking.

Changing the shared header invalidates all object files that depend on it.

## Clean vs Incremental Builds

An incremental build preserves valid artifacts and rebuilds only stale targets.

A clean build removes generated output and reconstructs the build from source.

Both workflows are important in systems engineering:

- incremental builds improve developer iteration speed;
- clean builds help verify reproducibility and eliminate stale-artifact effects.

## CMake and Ninja

CMake provides a higher-level project configuration.

The project specifies:

- C17;
- source files;
- include directories;
- compiler warning options.

CMake generates a Ninja build graph.

Ninja performs the actual compilation and linking.

This separates project description from build execution.

## Build Configurations

Separate build directories support independent configurations such as:

```text
Debug
Release
```

The same source tree can therefore be built with different optimization and debugging settings without mixing artifacts.

## Automation

`scripts/build_all.sh` provides a repeatable validation path that performs:

1. a clean GNU Make build;
2. the Make test target;
3. CMake Debug configuration using Ninja;
4. the Ninja build;
5. execution of the resulting program.

## Engineering Lessons

Build systems reduce repeated manual commands and encode dependencies explicitly.

Correct dependency tracking is essential for incremental compilation.

Generated build artifacts should be separated from source files and excluded from version control.

CMake is a build-system generator rather than the compiler itself.

Ninja is a build executor optimized for fast incremental builds.

A reproducible build process is part of software quality, not merely a convenience.

## Result

The program was successfully expressed through manual compiler commands, GNU Make, and CMake with Ninja.

The lab established practical understanding of dependency tracking, incremental builds, clean builds, configuration, and reproducible build automation.
