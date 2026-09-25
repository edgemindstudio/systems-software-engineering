# Lab 00-02 — Compilation Pipeline Investigation

## Objective

Investigate the complete C compilation pipeline from source code to a runnable ELF executable and inspect the intermediate artifacts produced at each stage.

## Engineering Context

Systems software engineers should understand what actually happens when a compiler command is executed.

A command such as:

```bash
clang program.c
```

appears simple, but several different stages are involved:

```text
C source
    ↓
Preprocessor
    ↓
Preprocessed translation unit
    ↓
Compiler
    ↓
Assembly
    ↓
Assembler
    ↓
Relocatable object file
    ↓
Linker
    ↓
Executable
```

Understanding these stages is important when diagnosing:

- compiler failures;
- linker errors;
- missing symbols;
- ABI issues;
- library problems;
- optimization behavior;
- binary-layout questions;
- build-system problems.

This lab breaks the pipeline into separate stages so that each artifact can be inspected directly.

## Prerequisites

- Lab 00-01 — Linux Engineering Environment
- Basic familiarity with C syntax
- Basic command-line navigation
- Familiarity with running compiler commands

## Tools

The lab uses:

- `clang`
- `file`
- `nm`
- `readelf`
- `objdump`
- `ldd`
- `diff`
- `wc`
- `grep`
- `less`

## Source Program

The lab uses three source files:

```text
src/
├── main.c
├── math_ops.c
└── math_ops.h
```

`main.c` calls two functions:

```text
add()
multiply()
```

Their declarations are stored in `math_ops.h`, and their implementations are stored in `math_ops.c`.

The program calculates:

```text
7 + 5
7 × 5
```

and produces:

```text
sum: 12
product: 35
```

## Repository Structure

```text
lab-00-02-compilation-pipeline-investigation/
├── README.md
├── artifacts/
│   ├── linker_error.txt
│   ├── main.i
│   ├── main.o
│   ├── main.s
│   ├── math_ops.i
│   ├── math_ops.o
│   ├── math_ops.s
│   ├── math_ops_O0.s
│   ├── math_ops_O2.s
│   ├── optimization_diff.txt
│   ├── pipeline_demo
│   └── pipeline_demo_manual
├── reports/
│   └── pipeline_report.md
├── scripts/
│   └── build_stages.sh
└── src/
    ├── main.c
    ├── math_ops.c
    └── math_ops.h
```

## Stage 1 — Preprocessing

The source files were preprocessed using:

```bash
clang -std=c17 -E src/main.c -I src -o artifacts/main.i
clang -std=c17 -E src/math_ops.c -I src -o artifacts/math_ops.i
```

This produced:

```text
main.i
math_ops.i
```

The original `main.c` contained 17 lines.

The preprocessed `main.i` contained 593 lines.

This demonstrated that preprocessing expands included headers and prepares the complete translation unit that the compiler will process.

The macros:

```c
#define FIRST_VALUE 7
#define SECOND_VALUE 5
```

were no longer present by name in the preprocessed output because they had been replaced with their values.

## Stage 2 — Compilation to Assembly

The preprocessed files were compiled to assembly:

```bash
clang -std=c17 -S artifacts/main.i -o artifacts/main.s
clang -std=c17 -S artifacts/math_ops.i -o artifacts/math_ops.s
```

The generated assembly contained labels corresponding to the original C functions:

```text
main:
add:
multiply:
```

At `-O0`, the generated code retained an explicit stack-frame structure and moved function arguments through stack locations before performing the arithmetic operations.

For example, `add()` used an `addl` instruction, while `multiply()` used `imull`.

## Stage 3 — Assembly to Object Files

The assembly files were assembled into object files:

```bash
clang -c artifacts/main.s -o artifacts/main.o
clang -c artifacts/math_ops.s -o artifacts/math_ops.o
```

The `file` command reported:

```text
ELF 64-bit LSB relocatable, x86-64
```

for both object files.

These object files contained machine code, but they were not yet executable programs.

## Stage 4 — Symbol Inspection

`nm` was used to inspect symbols in the object files.

`main.o` contained:

```text
U add
T main
U multiply
U printf
```

The `T` symbol showed that `main` was defined in the text section of `main.o`.

The `U` entries showed that:

```text
add
multiply
printf
```

were referenced but not defined inside that object file.

`math_ops.o` contained:

```text
T add
T multiply
```

This demonstrated that different object files can provide definitions for symbols required elsewhere.

## Stage 5 — ELF Inspection

The object file was inspected using:

```bash
readelf -h artifacts/main.o
readelf -S artifacts/main.o
readelf -s artifacts/main.o
readelf -r artifacts/main.o
```

The ELF header identified `main.o` as:

```text
REL (Relocatable file)
```

The object file contained multiple sections including:

```text
.text
.rodata
.symtab
.strtab
.rela.text
```

The relocation table contained entries referencing:

```text
add
multiply
printf
```

These entries identify locations that cannot be finalized until linking.

## Stage 6 — Disassembly

The machine code stored in the object files was inspected using:

```bash
objdump -d artifacts/math_ops.o
objdump -d -Mintel artifacts/math_ops.o
objdump -d -Mintel artifacts/main.o
```

This showed the machine-code bytes and their corresponding assembly instructions.

In `main.o`, the `call` instructions still contained unresolved destinations because final addresses had not yet been determined.

## Stage 7 — Linking

The object files were linked manually using:

```bash
clang artifacts/main.o artifacts/math_ops.o \
  -o artifacts/pipeline_demo_manual
```

The resulting program executed successfully:

```text
sum: 12
product: 35
```

The resulting file was reported as:

```text
ELF 64-bit LSB pie executable
```

The ELF type changed from:

```text
REL
```

to:

```text
DYN (Position-Independent Executable file)
```

The executable also had a real entry point address.

## Stage 8 — Linker Failure Investigation

A deliberate link failure was created using:

```bash
clang artifacts/main.o -o artifacts/broken_demo
```

The linker reported:

```text
undefined reference to `add'
undefined reference to `multiply'
```

This demonstrated that compilation and assembly can succeed even when symbols remain unresolved.

The linker requires the missing definitions before it can create the final executable.

The failure was preserved in:

```text
artifacts/linker_error.txt
```

## Stage 9 — Dynamic Dependencies

The executable was inspected using:

```bash
ldd artifacts/pipeline_demo_manual
```

The program depended on:

```text
libc.so.6
/lib64/ld-linux-x86-64.so.2
```

The dynamic section also showed:

```text
NEEDED Shared library: [libc.so.6]
```

This demonstrated that linking the project's object files does not make the program fully self-contained.

Runtime support and standard-library functionality are still provided through dynamically linked libraries.

## Stage 10 — Optimization Investigation

Assembly was generated at two optimization levels:

```bash
clang -std=c17 -O0 -S src/math_ops.c -I src \
  -o artifacts/math_ops_O0.s

clang -std=c17 -O2 -S src/math_ops.c -I src \
  -o artifacts/math_ops_O2.s
```

The results were:

```text
49 lines — math_ops_O0.s
32 lines — math_ops_O2.s
```

At `-O0`, `add()` used stack-frame setup and several memory operations.

At `-O2`, the same function became:

```asm
leal    (%rdi,%rsi), %eax
retq
```

The optimized version of `multiply()` became:

```asm
movl    %edi, %eax
imull   %esi, %eax
retq
```

This demonstrated that compiler optimization can significantly change the generated machine-level implementation without changing the intended behavior of the program.

The comparison is preserved in:

```text
artifacts/optimization_diff.txt
```

## Build Automation

The full pipeline is automated using:

```text
scripts/build_stages.sh
```

The script performs:

```text
[1/5] Preprocessing
[2/5] Compiling to assembly
[3/5] Assembling
[4/5] Linking
[5/5] Running
```

It can be executed with:

```bash
./scripts/build_stages.sh
```

A successful run produces:

```text
sum: 12
product: 35

Build pipeline complete.
```

## Validation

The lab is validated by confirming that:

- preprocessing produces `.i` files;
- compilation produces `.s` files;
- assembly produces relocatable `.o` files;
- symbol tables show expected defined and undefined symbols;
- relocation entries are present;
- manual linking succeeds when all required object files are supplied;
- linking fails when required definitions are missing;
- the final executable runs correctly;
- dynamic-library dependencies can be identified;
- optimized and unoptimized assembly differ while program behavior remains correct;
- the automated pipeline script completes successfully.

## Deliverables

- C source files
- header file
- preprocessed translation units
- assembly files
- relocatable object files
- final executable
- manually linked executable
- captured linker error
- optimization comparison
- build automation script
- pipeline investigation report
- this README

## Key Lessons

The most important lesson from this lab is that compilation is not one single operation.

A C program passes through several distinct stages, each producing a different artifact.

Object files can contain valid machine code while still containing unresolved symbols.

Relocation information allows the linker to repair references once final addresses are known.

Linking combines object files and resolves symbol dependencies.

The final executable can still depend on shared libraries that are resolved at runtime.

Compiler optimization can significantly alter the generated machine instructions while preserving program behavior.

## Completion Criteria

This lab is complete when the learner can explain:

1. What the preprocessor produces.
2. What the compiler produces.
3. What the assembler produces.
4. Why object files are relocatable.
5. Why object files can contain undefined symbols.
6. What relocation entries represent.
7. What the linker does.
8. Why linking can fail after compilation succeeds.
9. How an object file differs from an executable.
10. Why dynamically linked executables still contain external dependencies.
11. Why optimization changes generated assembly.

## Result

The complete C compilation pipeline was successfully inspected from source code through preprocessing, assembly generation, object creation, linking, dynamic dependency inspection, and optimization comparison.