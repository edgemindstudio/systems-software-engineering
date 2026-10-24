# Lab 02-01 — Multi-Module C Project

## Objective

Organize a small C project around a stable public interface while keeping formatting helpers private to the implementation.

## Architecture

```text
include/telemetry.h          public API
src/telemetry.c              public API implementation
src/format_internal.h        private implementation header
src/format_internal.c        private helper implementation
src/main.c                   consumer
tests/test_telemetry.c       automated tests
```

Only `include/telemetry.h` is intended for consumers.

## Engineering Focus

The lab demonstrates:

- public versus private headers;
- multiple translation units;
- internal helper boundaries;
- interface-oriented tests;
- strict warning flags;
- explicit build dependencies.

## Validation

```bash
./scripts/validate.sh
```
