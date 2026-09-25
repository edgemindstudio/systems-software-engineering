# Systems Software Engineering

This repository is a practical laboratory program for developing the skills required for systems software engineering.

The focus is not on completing isolated programming exercises or memorizing theory. The goal is to build, inspect, debug, measure, and reason about software in environments that resemble real engineering work.

The labs progress from engineering environment setup and C systems fundamentals into Linux system programming, memory management, processes, concurrency, networking, debugging, performance analysis, binary inspection, and larger systems projects.

## Program Philosophy

The repository follows a practical engineering cycle:

```text
Learn
  ↓
Build
  ↓
Break
  ↓
Debug
  ↓
Measure
  ↓
Improve
  ↓
Explain
```

The emphasis is on developing engineering judgment.

A learner should not only know how something works when everything is correct. They should also learn how to investigate failures, understand unfamiliar code, work with tools, interpret evidence, test changes, and explain technical decisions.

## What This Repository Is Designed To Develop

The program is designed to build practical ability in areas including:

- C systems programming
- modern C++ for systems work
- Linux system calls
- file descriptors and I/O
- processes and program execution
- signals and interprocess communication
- memory management
- virtual memory
- concurrency
- networking
- debugging
- build systems
- static analysis
- profiling
- performance engineering
- ELF and binary inspection
- linking and loading
- Git engineering workflows
- automated testing
- incident investigation
- engineering documentation

## Repository Structure

The main curriculum is divided into technical domains:

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

Each domain contains a sequence of practical labs.

The repository also contains:

```text
incidents/
```

for realistic engineering failure investigations, and:

```text
projects/
```

for larger systems projects that combine multiple skills.

## Learning Progression

The program begins with the engineering environment and progresses toward increasingly independent systems work.

Early labs are more guided.

Later labs require more investigation, tool selection, debugging, and architectural judgment.

The intended progression is:

```text
Guided Lab
    ↓
Engineering Lab
    ↓
Investigation
    ↓
Incident
    ↓
Integration Project
    ↓
Capstone
```

As the program progresses, instructions become less prescriptive.

The learner is increasingly expected to determine:

- what part of the system is failing;
- which tools are appropriate;
- what evidence is needed;
- what change should be made;
- how the change should be validated.

## Lab Structure

Individual labs may contain directories such as:

```text
src/
scripts/
tests/
artifacts/
evidence/
reports/
benchmarks/
docs/
```

The exact structure depends on the purpose of the lab.

Every lab should clearly define:

- objective;
- engineering context;
- required tools;
- tasks;
- expected deliverables;
- validation method;
- completion criteria.

See `LAB_STANDARD.md` for the repository-wide lab standard.

## Engineering Evidence

The purpose of the repository is not simply to show finished code.

Labs may preserve engineering evidence such as:

- compiler output;
- object files;
- assembly;
- traces;
- profiling data;
- benchmark results;
- failure logs;
- debugger observations;
- reports;
- test results;
- root-cause analysis.

This evidence documents how a conclusion was reached, not only the final answer.

## Incidents

The `incidents/` section contains realistic failure scenarios including:

- memory growth;
- file corruption;
- hanging services;
- CPU regressions;
- deployment failures;
- network-service collapse;
- CI-only failures;
- unfamiliar repository work;
- security and robustness defects;
- regression investigation.

These incidents intentionally provide less guidance than normal labs.

The goal is to develop investigation and debugging judgment.

## Projects

The larger engineering projects include:

```text
systems-utility-suite
shell
allocator
thread-runtime
network-service
binary-inspector
final-capstone
```

Projects are intended to integrate knowledge from several technical domains.

## Completion Standard

A lab is not considered complete simply because the program runs.

Depending on the lab, completion may require:

- successful compilation;
- passing tests;
- clean warnings;
- sanitizer checks;
- static analysis;
- correct error handling;
- benchmark evidence;
- debugging evidence;
- engineering documentation;
- root-cause explanation;
- reproducible execution.

The quality of reasoning matters as much as the final implementation.

## Current Progress

Completed:

- Lab 00-01 — Linux Engineering Environment
- Lab 00-02 — Compilation Pipeline Investigation

In progress:

- Domain 00 — Engineering Environment

## Long-Term Goal

The goal of this repository is to develop a practical body of systems-engineering work that demonstrates the ability to:

- enter an unfamiliar Linux/C/C++ environment;
- build and inspect software;
- understand system-level behavior;
- investigate failures;
- reason about memory and concurrency;
- diagnose performance problems;
- work with binaries and toolchains;
- implement bounded changes;
- validate those changes;
- document engineering decisions clearly.

This repository is intended to grow progressively as each lab, incident, and project is completed.