# Lab 00-01 — Linux Engineering Environment

## Objective

Establish, inspect, and verify a Linux development environment suitable for practical systems software engineering.

## Engineering Context

Systems software engineers work directly with compilers, debuggers, build systems, tracing tools, performance tools, binary utilities, version-control systems, and the operating system itself.

Before beginning more advanced systems work, the development environment should be understood and verified rather than assumed to be correct.

This lab establishes the workstation used throughout the rest of the repository and produces a reproducible record of the installed toolchain.

## Prerequisites

- Basic familiarity with the Linux command line
- Ability to navigate directories and execute shell commands
- Access to a Linux development environment

## Environment Used

The lab was completed using:

- Kali GNU/Linux Rolling 2026.1
- WSL2
- x86_64 architecture
- Bash shell

System details and exact tool versions are recorded in:

```text
evidence/toolchain_check.txt
```

and:

```text
reports/environment_report.md
```

## Skills Practiced

- Linux environment inspection
- compiler and toolchain verification
- debugger verification
- build-tool verification
- static-analysis verification
- memory-analysis tool verification
- tracing and profiling tool verification
- binary-inspection tool verification
- shell scripting
- package verification
- environment documentation
- reproducible evidence collection

## Tools Verified

The environment check includes tools from several engineering categories.

### Compilers

- GCC
- Clang
- G++
- Clang++

### Build Tools

- CMake
- Ninja
- GNU Make

### Debuggers

- GDB
- LLDB

### Static Analysis

- clang-tidy
- Cppcheck

### Memory and Correctness

- Valgrind

### Tracing and Performance

- strace
- ltrace
- perf

### Binary Inspection

- objdump
- readelf
- nm
- xxd

### Version Control

- Git
- GitHub CLI

## Tasks

1. Inspect the operating system and kernel.
2. Identify the CPU architecture.
3. Verify the active shell.
4. Verify the C and C++ compilers.
5. Verify the build systems.
6. Verify debugging tools.
7. Verify static-analysis tools.
8. Verify memory-analysis tools.
9. Verify tracing and performance tools.
10. Verify binary-inspection tools.
11. Verify Git and GitHub CLI.
12. Record the installed tool versions.
13. Identify missing or incorrectly configured tools.
14. Correct environment gaps where appropriate.
15. Preserve the final environment check as evidence.
16. Document the workstation configuration and readiness.

## Repository Structure

```text
lab-00-01-linux-engineering-environment/
├── README.md
├── evidence/
│   └── toolchain_check.txt
├── reports/
│   └── environment_report.md
└── scripts/
    └── check_toolchain.sh
```

## Environment Check

The main verification script is:

```text
scripts/check_toolchain.sh
```

It can be executed with:

```bash
./scripts/check_toolchain.sh
```

The output can be preserved with:

```bash
./scripts/check_toolchain.sh | tee evidence/toolchain_check.txt
```

## Validation

The lab is validated by confirming that:

- the environment-check script executes successfully;
- the required engineering tools are detected correctly;
- tool versions are recorded;
- missing tools are reported explicitly;
- the saved evidence matches the current workstation;
- the environment report documents the operating environment accurately.

## Issue Discovered During the Lab

The first version of the environment checker incorrectly reported `xxd` as missing.

The original check depended on the exit status of:

```bash
xxd -h
```

even though the executable was installed.

The check was improved to verify the executable directly using:

```bash
command -v xxd
```

The script was also improved so other tool checks did not incorrectly depend on pipeline exit behavior.

The lab also correctly identified that `ltrace` was genuinely missing. It was installed and then verified successfully.

This was a useful reminder that environment-detection scripts should test the condition they actually care about instead of assuming that a particular command invocation represents installation state.

## Key Observations

The completed workstation contains the primary tools required for the later Systems Software Engineering labs.

The environment includes:

- multiple C and C++ compilers;
- multiple debuggers;
- modern build systems;
- static-analysis tooling;
- memory-analysis tooling;
- tracing utilities;
- performance tooling;
- binary-inspection utilities;
- version-control tooling.

The primary environment is Linux running under WSL2 rather than bare-metal Linux.

This is suitable for the majority of the program, although later labs involving kernel behavior, hardware performance counters, low-level networking, or tracing may require WSL2-specific limitations to be considered.

## Deliverables

- `scripts/check_toolchain.sh`
- `evidence/toolchain_check.txt`
- `reports/environment_report.md`
- this README

## Completion Criteria

This lab is complete when:

- the environment-check script executes successfully;
- installed tools and versions are recorded;
- missing tools are identified correctly;
- required missing tools are installed where appropriate;
- environment evidence is preserved;
- the workstation configuration is documented;
- the report explains whether the environment is suitable for later systems labs.

## Result

The Linux systems-engineering environment is configured and verified for continued work in the repository.