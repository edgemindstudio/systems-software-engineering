# Error-Handling Architecture — Engineering Report

## Objective

Design and validate an explicit error-handling contract suitable for reusable C library code.

## Implementation Summary

The API returns enumerated status codes and optionally fills caller-owned error information.

The library does not print errors internally and does not require global error state.

Covered statuses include:

- success;
- invalid argument;
- parse failure;
- range failure;
- divide by zero.

## Runtime Observations

The demonstration produced:

```text
84 / 2 = 42
expected error: divide_by_zero: denominator must not be zero
```

The automated tests also exercised:

- valid integer parsing;
- malformed input;
- null arguments;
- divide-by-zero handling;
- integer division overflow.

## Engineering Interpretation

The caller can distinguish failure categories programmatically while retaining human-readable diagnostic text.

Because the error structure is caller-owned, the design avoids hidden shared state and unnecessary allocation.

## Validation

The lab completed:

- a clean strict-warning build;
- successful normal-path execution;
- deliberate error-path execution;
- automated tests over all defined status categories.

No build warnings or runtime errors were reported.

## Result

PASS — explicit status propagation and structured error reporting behaved correctly across normal and failure paths.
