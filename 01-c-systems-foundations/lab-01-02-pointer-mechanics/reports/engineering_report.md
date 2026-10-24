# Pointer Mechanics — Engineering Report

## Objective

Exercise pointer traversal, indirection, mutation, pointer arithmetic within arrays, and pointer-to-pointer output parameters while remaining within valid object bounds.

## Implementation Summary

The lab implements:

- read-only array traversal using pointers;
- in-place swapping;
- array reversal;
- search returning a pointer through an output parameter;
- mutation through a returned element pointer.

## Runtime Observations

The demonstration produced:

```text
sum: 30
found: 6 at index 2
reversed: 10 8 7 4 2
```

The search located the expected array element, the returned pointer referred to the correct array position, mutation through that pointer succeeded, and the final reversal operated in place.

## Failure Modes Considered

The implementation keeps pointer arithmetic within the bounds of the same array object.

The search function explicitly reports failure and clears the output pointer to `NULL` when the target is absent.

No pointer is returned to expired storage.

## Validation

The lab completed:

- a clean strict-warning build;
- automated tests;
- AddressSanitizer validation;
- UndefinedBehaviorSanitizer validation.

No warnings or sanitizer errors were reported.

## Result

PASS — pointer traversal, lookup, mutation, swapping, and reversal behaved correctly under tests and sanitizers.
