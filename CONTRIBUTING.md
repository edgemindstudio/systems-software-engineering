# Contributing

This repository is currently developed as a practical systems software engineering laboratory program.

Contributions should preserve the engineering-first purpose of the project.

## General Principles

New material should:

- emphasize practical engineering work;
- use realistic tools;
- require reasoning rather than only command repetition;
- include clear completion criteria;
- preserve reproducibility;
- avoid unnecessary abstraction;
- document important engineering observations.

## Adding a Lab

A proposed lab should identify:

- target domain;
- competency being developed;
- prerequisites;
- scenario or engineering context;
- required tools;
- tasks;
- deliverables;
- validation strategy;
- completion criteria.

Labs should follow `LAB_STANDARD.md`.

## Repository Naming

Lab directories use:

```text
lab-DD-NN-short-description
```

For example:

```text
lab-03-01-raw-file-copy
```

Incident directories use:

```text
incident-NN-short-description
```

## Code Quality

Code should compile cleanly with appropriate warning levels.

Tests should be added where they provide meaningful validation.

Generated files should only be committed when they serve as useful engineering evidence.

## Commit Quality

Commits should describe meaningful units of work.

Examples:

```text
Complete compilation pipeline investigation lab

Add allocator coalescing tests

Fix partial-write handling in file copy utility

Document deadlock root-cause investigation
```

Avoid vague messages such as:

```text
update
changes
fix stuff
```

## Reports

Reports should preserve actual observations from the lab or investigation.

Measured results should not be invented or generalized beyond the evidence produced during the work.

## Scope

This repository is focused on systems software engineering.

Material should have a clear connection to low-level software construction, debugging, operating-system interaction, performance, runtime behavior, or engineering workflow.