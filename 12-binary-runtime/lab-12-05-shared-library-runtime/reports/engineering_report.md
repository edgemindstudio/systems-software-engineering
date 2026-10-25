# Shared Library Runtime Loading — Engineering Report

## Objective
Build a versioned shared library and verify runtime discovery through `$ORIGIN`-relative RUNPATH.

## Observations
Library layout:
```text
libruntimemath.so -> libruntimemath.so.1
libruntimemath.so.1 -> libruntimemath.so.1.0.0
```
SONAME was `libruntimemath.so.1`. The executable recorded `NEEDED: libruntimemath.so.1` and `RUNPATH: $ORIGIN/../lib`. `ldd` resolved the project-local library. Program output was `runtime_multiply=42`. `LD_DEBUG` showed the loader searching the encoded RUNPATH and loading the versioned library.

## Engineering Interpretation
SONAME provides runtime library identity, while `$ORIGIN` allows relocatable project-local library discovery without hard-coded absolute paths.

## Result
PASS — shared-library metadata and runtime resolution were verified.
