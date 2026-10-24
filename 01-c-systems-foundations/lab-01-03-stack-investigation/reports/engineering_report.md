# Stack Investigation — Engineering Report

## Objective

Observe automatic-storage behavior across recursive calls and distinguish C lifetime rules from platform-specific stack-layout observations.

## Implementation Summary

The program records the addresses of local automatic variables across recursive calls using `uintptr_t`.

The implementation checks that each simultaneously active recursive frame has a distinct local-object address.

It intentionally does not return pointers to local variables after their lifetime ends.

## Runtime Observations

In the normal build, the recorded local addresses decreased as recursion depth increased.

The program reported:

```text
distinct active-frame locals: yes
observed address trend: decreasing
```

Under AddressSanitizer instrumentation, the observed addresses instead increased with recursion depth:

```text
distinct active-frame locals: yes
observed address trend: increasing
```

This is an important observation: stack-address direction is not a portable C-language guarantee, and instrumentation can change the observed memory layout.

The invariant that mattered for the lab remained true in both configurations: simultaneously active recursive calls had distinct automatic-storage locations.

## Failure Modes Considered

The lab avoids returning addresses of expired local variables.

Address values are treated as observations rather than as evidence of a language-mandated stack-growth direction.

## Validation

The lab completed:

- a clean strict-warning build;
- automated tests;
- AddressSanitizer validation;
- UndefinedBehaviorSanitizer validation.

No warnings or sanitizer errors were reported.

## Result

PASS — recursive automatic-storage behavior was inspected safely and the distinction between language guarantees and implementation observations was demonstrated.
