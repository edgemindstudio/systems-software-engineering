# Multi-Module C Project — Engineering Report

## Objective

Validate a multi-translation-unit C project with a stable public interface and implementation details kept private.

## Implementation Summary

The project separates:

```text
include/telemetry.h
```

as the consumer-facing API from:

```text
src/format_internal.h
```

as a private implementation header.

The implementation is split across multiple source files and linked into a single consumer program.

## Runtime Observations

The demonstration produced:

```text
avg_temp=73.33C peak_rpm=2200
```

The automated tests passed.

The validation also confirmed:

```text
public header:
telemetry.h

private header:
format_internal.h
```

This shows that consumer-facing declarations and implementation-only helpers are physically separated.

## Engineering Interpretation

The public/private header split creates a clearer module boundary and reduces accidental coupling between consumers and implementation details.

The build also demonstrates independent compilation of multiple translation units followed by final linking.

## Validation

The lab completed:

- a clean strict-warning build;
- multi-object linking;
- consumer execution;
- automated tests;
- public/private header inspection.

No build warnings or runtime errors were reported.

## Result

PASS — the project demonstrated clean module boundaries, public/private header separation, multi-file compilation, and interface-oriented testing.
