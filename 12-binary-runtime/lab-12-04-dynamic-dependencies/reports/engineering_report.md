# Dynamic Dependencies — Engineering Report

## Objective
Inspect an executable's dynamic interpreter and shared-library dependencies.

## Observations
Program output was `sqrt=9.0`. The interpreter was `/lib64/ld-linux-x86-64.so.2`. `DT_NEEDED` entries included `libm.so.6` and `libc.so.6`, and `ldd` resolved both.

## Engineering Interpretation
The executable records logical shared-library dependencies that the runtime loader resolves before normal execution.

## Result
PASS — dynamic interpreter and dependency metadata were verified.
