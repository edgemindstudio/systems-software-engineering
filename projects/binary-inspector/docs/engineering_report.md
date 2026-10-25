# Binary Inspector — Engineering Report

## Objective

Build a direct ELF64 inspection tool in C that parses core ELF structures itself rather than delegating implementation to binutils.

The project focuses on safe structural parsing, bounded metadata access, symbol and dependency inspection, and file-offset/virtual-address mapping.

## Development Environment

Validation was performed on:

```text
Debian clang version 21.1.8 (7+b1)
GNU readelf (GNU Binutils for Debian) 2.46
Linux 6.6.87.2-microsoft-standard-WSL2 x86_64 GNU/Linux
```

## Build Quality

The normal build compiled the inspector using C17 with:

```text
-Wall
-Wextra
-Wpedantic
-Wconversion
-Wshadow
-Wstrict-prototypes
-Wmissing-prototypes
```

The validation run completed without compiler warnings.

## Integration and Oracle Validation

The integration suite reported:

```text
[test] summary
[test] sections
[test] segments
[test] symbols
[test] needed
[test] readelf header parity
[test] mapping roundtrip
[test] malformed/truncated file
[test] non-ELF rejection
binary_inspector_tests=PASS
```

## ELF Summary Parsing

The representative fixture was identified as:

```text
class=ELF64
endianness=little
type=DYN
machine=62
entry=0x1050
program_headers=14
section_headers=38
```

This demonstrates direct parsing of:

- ELF class;
- endianness;
- ELF object type;
- machine identifier;
- entry point;
- program-header count;
- section-header count.

The test suite separately compared the program-header and section-header counts against `readelf`.

## Section Inspection

Representative selected sections were reported as:

```text
4       .dynsym type=11 offset=0x3b8    size=168        addr=0x3b8
13      .text   type=1  offset=0x1050   size=326        addr=0x1050
25      .data   type=1  offset=0x3008   size=20         addr=0x4008
35      .symtab type=2  offset=0x3360   size=912        addr=0x0
```

This verifies section-name resolution through the section-header string table and correct reporting of type, file offset, size, and virtual address.

## Program Header Inspection

The test suite verified that at least one `PT_LOAD` segment was discovered.

Program-header parsing reports:

- segment type;
- file offset;
- virtual address;
- file size;
- memory size;
- flags.

## Symbol Inspection

The test fixture contains known symbols:

```text
add_values
global_counter
```

The inspector successfully found them from ELF symbol tables.

The symbol parser handles both:

```text
SHT_SYMTAB
SHT_DYNSYM
```

and validates linked string-table bounds and symbol-name termination before printing names.

## Dynamic Dependency Inspection

The representative fixture reported:

```text
libc.so.6
```

from `DT_NEEDED`.

The test suite also compared dependency names against `readelf -d` output.

The implementation itself does not invoke `readelf`; binutils is used only as an external oracle in testing.

## File Offset / Virtual Address Mapping

The integration suite selected the `.text` file offset, mapped it into a virtual address through a file-backed `PT_LOAD` segment, then mapped that virtual address back to a file offset.

Observed result:

```text
mapping roundtrip=PASS
```

The mapping relation is:

```text
virtual_address =
    p_vaddr + (file_offset - p_offset)
```

for offsets inside:

```text
[p_offset, p_offset + p_filesz)
```

The reverse mapping applies the inverse relation.

## Malformed Input Handling

The test suite created a truncated ELF input by retaining only the first 32 bytes of a valid fixture.

The inspector rejected that file.

Observed result:

```text
malformed/truncated file=PASS
```

A plain text file was also supplied as input.

Observed result:

```text
non-ELF rejection=PASS
```

These tests demonstrate that the parser does not blindly cast arbitrary input into ELF structures without initial validation.

## Structural Validation

`elf_view_validate()` verifies:

- ELF magic;
- ELF64 class;
- little-endian representation;
- current ELF version;
- ELF header size;
- program-header entry size;
- program-header table bounds;
- section-header entry size;
- section-header table bounds;
- section-name table index validity.

Command-specific parsing adds further checks.

### Section Names

Before returning a section name, the implementation verifies:

- the section-header string table is inside the file;
- the name offset is inside that table;
- the name has a terminating NUL before the table ends.

### Symbols

Before printing symbol names, the implementation verifies:

- the symbol table lies inside the file;
- its linked string table lies inside the file;
- the entry size matches `Elf64_Sym`;
- each symbol name offset is valid;
- each printed name is NUL-terminated within the table.

### Dynamic Entries

Before printing `DT_NEEDED` dependencies, the implementation verifies:

- the dynamic table lies inside the file;
- its linked string table lies inside the file;
- the entry size matches `Elf64_Dyn`;
- dependency string offsets are valid;
- dependency strings terminate inside the table.

## ASan and UBSan Validation

The Binary Inspector was rebuilt with:

```text
-fsanitize=address,undefined
-fno-omit-frame-pointer
```

and executed with:

```text
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1
UBSAN_OPTIONS=halt_on_error=1
```

The complete integration/oracle suite passed again:

```text
[test] summary
[test] sections
[test] segments
[test] symbols
[test] needed
[test] readelf header parity
[test] mapping roundtrip
[test] malformed/truncated file
[test] non-ELF rejection
binary_inspector_tests=PASS
```

No AddressSanitizer or UndefinedBehaviorSanitizer failure interrupted validation.

## Engineering Interpretation

This project demonstrates several systems-software engineering competencies:

1. **Binary-format reasoning** — ELF headers and tables are parsed directly from raw bytes.
2. **Bounds-aware parsing** — offsets and table extents are checked against file size before access.
3. **String-table safety** — name offsets and termination are validated.
4. **Runtime-layout understanding** — `PT_LOAD` mappings connect file offsets to process virtual addresses.
5. **Symbol-table interpretation** — both static and dynamic symbols are inspected.
6. **Dynamic-linking awareness** — `DT_NEEDED` entries are resolved through the linked dynamic string table.
7. **Independent verification** — selected facts are compared against `readelf` without using it as the implementation.
8. **Malformed-input handling** — truncated and non-ELF files are rejected.
9. **Sanitizer-backed validation** — the same malformed-input and structural tests pass under ASan/UBSan.
10. **Explicit scope discipline** — unsupported ELF variants are rejected rather than silently misinterpreted.

## Scope

The inspector intentionally supports:

```text
ELF64
little-endian
```

It does not claim support for:

- ELF32;
- big-endian ELF;
- every processor-specific ELF extension;
- relocation decoding;
- DWARF/debug-info parsing;
- instruction disassembly;
- executable rewriting;
- binary patching;
- arbitrary corrupted ELF recovery.

The project is intended as a focused direct parser for core ELF64 runtime structures.

## Result

**PASS**

The Binary Inspector successfully completed strict compilation, ELF header/section/segment parsing, known-symbol lookup, dynamic dependency extraction, selected parity checks against GNU `readelf`, file-offset/virtual-address round-trip validation, malformed/truncated input rejection, non-ELF rejection, and ASan/UBSan validation on the development environment.
