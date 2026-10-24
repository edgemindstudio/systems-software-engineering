# 02 — Library Engineering

## Purpose

This section moves from standalone C programs to reusable library engineering.

The labs focus on module boundaries, public APIs, explicit error contracts, static archives, shared objects, runtime loading, symbol inspection, versioning, and ABI compatibility.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- separate public interfaces from private implementation details;
- organize a multi-module C codebase;
- design explicit, testable error-handling contracts;
- build and inspect static archives;
- build position-independent shared libraries;
- define and inspect ELF SONAME metadata;
- link consumers against static and shared libraries;
- inspect exported symbols with `nm` and `readelf`;
- inspect runtime dependencies with `ldd`;
- use RUNPATH for controlled local shared-library discovery;
- explain the difference between API compatibility and ABI compatibility;
- reproduce an ABI break caused by public structure-layout changes;
- detect that ABI break with a runtime compatibility check.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 02-01 | Multi-Module C Project | public/private headers, module boundaries, tests |
| 02-02 | Error-Handling Architecture | status codes, caller-owned error information |
| 02-03 | Static Library | `ar`, archive symbol table, static consumer |
| 02-04 | Shared Library | `-fPIC`, SONAME, symlinks, RUNPATH, `ldd`, dynamic symbols |
| 02-05 | ABI Breakage Investigation | same SONAME/symbols, incompatible public layout, runtime mismatch |

## Quality Standard

Every lab contains source, tests, a repeatable validation script, retained runtime evidence, README documentation, and an engineering report.

Generated build artifacts are excluded from version control.

## Section Validation

From this directory:

```bash
./scripts/validate_all.sh
```

The section-wide validator performs clean builds, executes consumers/tests, and captures binary-inspection evidence where appropriate.

## Completed Results

All five labs passed on the development environment.

The completed section demonstrated:

- a public API separated from a private implementation header;
- explicit status-code and caller-owned error reporting;
- construction and inspection of a static archive;
- a versioned shared library built with position-independent code;
- SONAME and RUNPATH inspection;
- runtime shared-library discovery through `ldd`;
- exported dynamic symbol inspection;
- a controlled ABI incompatibility where the consumer and replacement library retained the same SONAME and function symbols but disagreed on a public structure's binary layout.

The ABI investigation showed a consumer compiled against an 8-byte `struct record`, while the incompatible v2 library reported a 24-byte layout. The consumer detected the mismatch and rejected the incompatible library before performing a layout-sensitive operation.

## Progress

- Lab 02-01 — Multi-Module C Project: Complete
- Lab 02-02 — Error-Handling Architecture: Complete
- Lab 02-03 — Static Library: Complete
- Lab 02-04 — Shared Library: Complete
- Lab 02-05 — ABI Breakage Investigation: Complete

Domain 02 — Library Engineering: Complete
