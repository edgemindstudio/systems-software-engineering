# Shared Library — Engineering Report

## Objective

Build and consume a versioned ELF shared library and inspect the metadata that governs dynamic linking and runtime loading.

## Implementation Summary

The library was compiled as position-independent code using:

```text
-fPIC
```

and linked into:

```text
libcounter.so.1.0.0
```

with SONAME:

```text
libcounter.so.1
```

The conventional symlink chain was created:

```text
libcounter.so -> libcounter.so.1
libcounter.so.1 -> libcounter.so.1.0.0
```

The consumer was linked with a relative RUNPATH based on `$ORIGIN`.

## Runtime Observations

The consumer produced:

```text
library_version=1.0.0
lines=3
letter_a=5
```

`readelf` confirmed:

```text
SONAME: libcounter.so.1
RUNPATH: $ORIGIN/../lib
```

`nm -D` showed the expected exported symbols:

```text
counter_count_char
counter_count_lines
counter_version
```

`ldd` resolved:

```text
libcounter.so.1
```

from the lab's local `build/lib` directory through the configured RUNPATH.

## Engineering Interpretation

The SONAME establishes the runtime compatibility identity used by the dynamic loader, while the file-version and symlink scheme provide versioned deployment and development-link names.

The `$ORIGIN` RUNPATH allows the executable to locate its sibling library directory without requiring a system-wide installation or a manually exported `LD_LIBRARY_PATH`.

## Validation

The lab completed:

- PIC compilation;
- shared-object creation;
- SONAME inspection;
- versioned symlink creation;
- shared consumer execution;
- automated tests;
- RUNPATH inspection;
- dynamic symbol inspection;
- runtime dependency inspection.

## Result

PASS — shared-library creation, metadata, linking, and runtime resolution behaved as designed.
