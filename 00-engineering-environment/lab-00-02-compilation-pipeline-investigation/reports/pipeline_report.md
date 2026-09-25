# Compilation Pipeline Investigation Report

## 1. Source Files

For this lab, I used three source files:

- `main.c`
- `math_ops.c`
- `math_ops.h`

The `main.c` file contains the `main()` function and calls two functions, `add()` and `multiply()`. These functions are declared in `math_ops.h` and implemented in `math_ops.c`.

I also used two macros in `main.c`:

```c
#define FIRST_VALUE 7
#define SECOND_VALUE 5
```

The program adds and multiplies these two values and prints the results.

When compiled and executed normally, the program produced:

```text
sum: 12
product: 35
```

---

## 2. Preprocessing

The first stage I investigated was preprocessing.

I used Clang with the `-E` option to generate:

```text
main.i
math_ops.i
```

One of the first things I noticed was how much larger the preprocessed file became. My original `main.c` file contained only 17 lines, while `main.i` contained 593 lines.

This happened mainly because the preprocessor expanded the included header files. For example:

```c
#include <stdio.h>
```

was no longer just a single line. The declarations and definitions needed from the header became part of the preprocessed translation unit.

I also noticed that these macros:

```c
FIRST_VALUE
SECOND_VALUE
```

could no longer be found in `main.i`.

The preprocessor had replaced them with their actual values:

```text
7
5
```

This helped me understand that the compiler does not work directly on the exact source file I originally typed. Preprocessing happens first, and the compiler receives the resulting translation unit.

The preprocessing stage therefore handles things such as:

- header inclusion;
- macro replacement;
- conditional preprocessing;
- preparation of the source for compilation.

---

## 3. Assembly Generation

The next stage was compiling the preprocessed files into assembly language.

I generated:

```text
main.s
math_ops.s
```

When I inspected `main.s`, I found the label:

```text
main:
```

In `math_ops.s`, I found:

```text
add:
multiply:
```

These correspond directly to the C functions I wrote.

At `-O0`, the generated assembly for `add()` included instructions such as:

```asm
pushq   %rbp
movq    %rsp, %rbp
movl    %edi, -4(%rbp)
movl    %esi, -8(%rbp)
movl    -4(%rbp), %eax
addl    -8(%rbp), %eax
popq    %rbp
retq
```

The `multiply()` function had a similar structure but used:

```asm
imull
```

for multiplication.

This showed me that the compiler translates the C functions into architecture-specific assembly instructions.

I was also able to see the relationship between C function arguments and registers. On this x86-64 system, the arguments appeared in registers such as `%edi` and `%esi`.

---

## 4. Object Files

After generating the assembly files, I assembled them into object files:

```text
main.o
math_ops.o
```

The `file` command reported both as:

```text
ELF 64-bit LSB relocatable, x86-64
```

The important word here is `relocatable`.

The object files already contain machine code, but they are not complete executable programs. Some addresses and symbol references still need to be resolved before the program can run.

The ELF header for `main.o` also reported:

```text
Type: REL (Relocatable file)
```

and the entry point address was:

```text
0x0
```

This makes sense because `main.o` is not yet a runnable program.

---

## 5. Symbols

I used `nm` to inspect the symbols inside the object files.

For `main.o`, I found:

```text
U add
T main
U multiply
U printf
```

The letter `T` beside `main` shows that `main` is defined in the text section of that object file.

The letter `U` means that the symbol is undefined in that particular object file.

This means that `main.o` knows that it needs:

```text
add
multiply
printf
```

but their definitions are not contained inside `main.o`.

When I inspected `math_ops.o`, I found:

```text
T add
T multiply
```

This showed that `math_ops.o` provides the definitions for `add()` and `multiply()`.

This was an important observation because it showed me that an object file can contain valid machine code while still depending on symbols that will be resolved later.

---

## 6. Relocations

I inspected the relocation entries using:

```bash
readelf -r artifacts/main.o
```

The relocation table contained entries for:

```text
add
multiply
printf
```

as well as references to string data.

For example, there were relocation entries using types such as:

```text
R_X86_64_PLT32
R_X86_64_PC32
```

At this point, the final addresses for these symbols are not yet known.

The relocation entries tell the linker where references exist inside the object file and what needs to be adjusted when the final program is created.

This also explained something I saw when disassembling `main.o`.

The `call` instructions looked like:

```asm
call   1e <main+0x1e>
```

with placeholder values rather than final addresses for `add`, `multiply`, or `printf`.

The object file contains the instruction, but the final destination still has to be resolved.

---

## 7. Linking

I manually linked the two object files using:

```bash
clang artifacts/main.o artifacts/math_ops.o \
  -o artifacts/pipeline_demo_manual
```

The program executed successfully and produced:

```text
sum: 12
product: 35
```

When I inspected the resulting file, it was no longer reported as a relocatable object.

Instead, it was reported as:

```text
ELF 64-bit LSB pie executable
```

The ELF header also changed from:

```text
REL
```

to:

```text
DYN (Position-Independent Executable file)
```

The executable now had a real entry point:

```text
0x1050
```

and program headers were present.

When I inspected the symbols again, I found:

```text
T add
T main
T multiply
```

The final executable therefore contained the definitions from both object files.

This showed me that one of the linker's major responsibilities is combining multiple object files and resolving references between them.

---

## 8. Linker Failure

I deliberately tried to link only `main.o`:

```bash
clang artifacts/main.o -o artifacts/broken_demo
```

The linker failed and reported:

```text
undefined reference to `add'
undefined reference to `multiply'
```

This failure helped make the distinction between compilation and linking very clear.

`main.o` was created successfully because the compiler and assembler did not require the definitions of `add()` and `multiply()` to be inside the same source file.

The object file only needed to record that those symbols were required.

However, when creating the final executable, the linker needed actual definitions for them.

Because `math_ops.o` was missing from the link command, the linker could not resolve those symbols.

So compilation can succeed even when linking later fails.

---

## 9. Dynamic Dependencies

I inspected the finished executable with:

```bash
ldd artifacts/pipeline_demo_manual
```

The program depended on:

```text
libc.so.6
/lib64/ld-linux-x86-64.so.2
```

The dynamic section also contained:

```text
NEEDED Shared library: [libc.so.6]
```

This means that even though my own object files had been successfully linked together, the executable was still not completely self-contained.

Functions such as `printf()` come from the C standard library.

I also noticed that the final executable still showed:

```text
U __libc_start_main@GLIBC_2.34
```

This helped me understand that some symbols are intentionally resolved through shared libraries when the program is loaded or executed.

---

## 10. Optimization

I generated assembly from `math_ops.c` using both:

```text
-O0
-O2
```

The `-O0` assembly contained 49 lines, while the `-O2` version contained 32 lines.

The difference in the function bodies was much more important than just the number of lines.

At `-O0`, `add()` used a stack frame and moved the arguments into local stack locations before performing the operation.

The generated code included instructions such as:

```asm
pushq   %rbp
movq    %rsp, %rbp
movl    %edi, -4(%rbp)
movl    %esi, -8(%rbp)
movl    -4(%rbp), %eax
addl    -8(%rbp), %eax
popq    %rbp
retq
```

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

This showed me that the same C source code can result in very different machine-level implementations depending on the compiler optimization level.

The compiler removed unnecessary stack operations and produced a much smaller implementation while preserving the behavior of the program.

---

## 11. Engineering Understanding

Before this lab, it was easy to think of compilation as simply:

```bash
clang program.c
```

and then receiving an executable.

After investigating each stage separately, I now understand that several different transformations take place.

The complete path for this program was:

```text
main.c / math_ops.c
        ↓
Preprocessor
        ↓
main.i / math_ops.i
        ↓
Compiler
        ↓
main.s / math_ops.s
        ↓
Assembler
        ↓
main.o / math_ops.o
        ↓
Linker
        ↓
pipeline_demo
```

The preprocessor prepares the translation unit by expanding headers and replacing macros.

The compiler translates the preprocessed C code into assembly language.

The assembler converts the assembly instructions into machine code stored inside relocatable object files.

Those object files can still contain unresolved symbols and relocation entries because their final locations are not yet known.

The linker combines the object files, resolves symbols between them, performs the required relocations, and produces the final executable.

The finished executable can still depend on shared libraries such as libc, which are handled through dynamic linking.

I also learned that compiler optimization can significantly change the generated assembly without changing the intended behavior of the C program.

The biggest thing I took away from this lab is that an executable is not produced in one simple step. It is the result of several separate stages, and each stage produces a different type of artifact that can be inspected independently.