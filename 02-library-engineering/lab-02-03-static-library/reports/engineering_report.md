# Static Library — Engineering Report

## Objective

Build a reusable static archive, inspect its contents and symbols, and link a separate consumer against it.

## Implementation Summary

The library implementation was compiled into:

```text
textops.o
```

and archived as:

```text
build/lib/libtextops.a
```

using `ar`, followed by `ranlib`.

A separate consumer and test binary linked against the archive with `-L` and `-ltextops`.

## Runtime Observations

The consumer produced:

```text
words=4
upper=SYSTEMS LIBRARIES ARE REUSABLE
```

Archive inspection showed one member:

```text
textops.o
```

and the expected defined symbols:

```text
textops_ascii_upper
textops_count_words
```

The final executable was an ELF 64-bit PIE executable with debug information.

`ldd` showed runtime dependencies on the system C runtime and loader only; there was no project-specific shared-library dependency.

## Engineering Interpretation

The text-operation implementation was incorporated into the final executable through static archive linking rather than resolved from a project shared object at runtime.

The archive member and symbol-table inspection confirms what implementation object and public functions were available to the linker.

## Validation

The lab completed:

- archive creation with `ar`;
- archive indexing with `ranlib`;
- consumer linking;
- automated tests;
- archive-member inspection;
- symbol inspection with `nm`;
- executable inspection with `file`;
- runtime dependency inspection with `ldd`.

## Result

PASS — the static archive was created, inspected, consumed, and validated successfully.
