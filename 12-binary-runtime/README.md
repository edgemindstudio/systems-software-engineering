# 12 — Binary & Runtime

## Purpose
This section develops practical understanding of Linux ELF binaries, symbol tables, relocations, dynamic dependencies, loader behavior, and runtime linking.

## Employer-Facing Outcomes
The completed labs demonstrate ELF header inspection, section/symbol analysis, relocation analysis, dynamic dependency inspection, SONAME/RUNPATH reasoning, and runtime loader tracing.

## Labs
| Lab | Topic | Core evidence |
|---|---|---|
| 12-01 | ELF Header Inspection | ELF64, PIE, x86-64, interpreter metadata |
| 12-02 | Sections and Symbols | `.text`, `.data`, `.bss`, symbol classes |
| 12-03 | Relocations | undefined symbol and `R_X86_64_PLT32` relocation |
| 12-04 | Dynamic Dependencies | interpreter, `DT_NEEDED`, `ldd` |
| 12-05 | Shared Library Runtime Loading | SONAME, RUNPATH, `ldd`, `LD_DEBUG` |

## Completed Results
All five labs passed in the final warning-clean validation run.

Observed evidence included:
- ELF64 little-endian x86-64 PIE metadata with interpreter `/lib64/ld-linux-x86-64.so.2`;
- `.text`, `.data`, and `.bss` sections with expected `T`, `D`, and `B` symbol classifications;
- an undefined `external_transform` symbol with an `R_X86_64_PLT32` relocation;
- `libm.so.6` and `libc.so.6` dynamic dependencies;
- shared-library SONAME `libruntimemath.so.1`;
- application RUNPATH `$ORIGIN/../lib`;
- successful project-local shared-library resolution;
- runtime output `runtime_multiply=42`;
- `LD_DEBUG` evidence of loader search and initialization.

## Engineering Interpretation
These labs connect source code to the binary and runtime structures produced by the Linux toolchain. They show how symbols and relocations bridge compilation and linking, how dynamic dependencies are encoded, and how SONAME/RUNPATH metadata controls runtime discovery.

## Progress
- Lab 12-01 — Complete
- Lab 12-02 — Complete
- Lab 12-03 — Complete
- Lab 12-04 — Complete
- Lab 12-05 — Complete

Domain 12 — Binary & Runtime: Complete
