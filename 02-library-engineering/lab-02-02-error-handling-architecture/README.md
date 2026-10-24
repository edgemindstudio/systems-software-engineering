# Lab 02-02 — Error-Handling Architecture

## Objective

Design a small C API whose failures are explicit, inspectable, and testable.

## Error Contract

Functions return an enum status code and optionally populate caller-owned error information.

This avoids:

- hidden global error state;
- ambiguous sentinel values;
- printing errors inside reusable library code;
- forcing allocation merely to report an error.

## Covered Failure Modes

- invalid arguments;
- parse failures;
- integer range errors;
- division by zero.

## Validation

```bash
./scripts/validate.sh
```
