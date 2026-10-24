# Lab 02-04 — Shared Library

## Objective

Build and consume a versioned ELF shared library.

## Engineering Focus

The lab demonstrates:

- position-independent code with `-fPIC`;
- shared-object creation with `-shared`;
- ELF SONAME metadata;
- conventional versioned symlinks;
- consumer linking through `-lcounter`;
- local runtime discovery through RUNPATH;
- dynamic symbol inspection;
- runtime dependency inspection with `ldd`.

## Shared-Library Layout

```text
libcounter.so      -> libcounter.so.1
libcounter.so.1    -> libcounter.so.1.0.0
libcounter.so.1.0.0
```

## Validation

```bash
./scripts/validate.sh
```
