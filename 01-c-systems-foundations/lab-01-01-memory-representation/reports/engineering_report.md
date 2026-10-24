# Memory Representation — Engineering Report

## Objective

Inspect typed C objects at the byte level and connect source declarations to actual runtime layout, endianness, alignment, offsets, and structure padding.

## Implementation Summary

The lab uses a fixed `uint32_t` value, copies its object representation into a byte array, detects byte order without type-punning, and inspects a structure containing `char`, `uint32_t`, and `uint16_t` members.

The implementation also uses `sizeof`, `_Alignof`, and `offsetof` to expose layout properties without assuming a particular ABI in the source code.

## Runtime Observations

On the validated development environment, the value:

```text
0x01020304
```

was represented in memory as:

```text
04 03 02 01
```

The machine therefore reported little-endian byte order.

The structure probe produced:

```text
sizeof(layout_probe): 12
alignof(layout_probe): 4
offset(tag): 0
offset(value): 4
offset(code): 8
```

This shows that the structure contains padding inserted to satisfy alignment requirements.

## Failure Modes and Portability Considerations

The implementation does not assume a fixed byte order and does not rely on aliasing through incompatible pointer types.

Observed structure size, alignment, padding, and byte order are implementation properties and should not be hard-coded as universal C guarantees.

## Validation

The lab completed:

- a clean strict-warning build;
- automated tests;
- AddressSanitizer validation;
- UndefinedBehaviorSanitizer validation.

No warnings or sanitizer errors were reported.

## Result

PASS — byte representation, endianness, alignment, structure offsets, and padding were successfully inspected and validated.
