# Lab 02-05 — ABI Breakage Investigation

## Objective

Demonstrate a real ABI compatibility failure that is not prevented by the dynamic linker.

## Scenario

A consumer is compiled against version 1 of a public header:

```c
struct record {
    uint32_t id;
    uint32_t value;
};
```

Version 2 changes the public structure layout while keeping:

- the same shared-library SONAME: `librecord.so.1`;
- the same exported function names.

The dynamic loader can therefore load the new library even though the consumer and library disagree about the binary layout of the public type.

## Safety of the Demonstration

The consumer calls `record_abi_size()` before using layout-sensitive operations.

The v1 library reports the same size the consumer compiled against.

The incompatible v2 library reports a different size, allowing the program to detect the ABI break without intentionally performing an out-of-bounds access.

## Engineering Lesson

API names can remain unchanged while ABI compatibility is broken.

Public structure layout is part of the ABI when callers allocate or embed that structure directly.

## Validation

```bash
./scripts/validate.sh
```
