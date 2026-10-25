# Binary Inspector

A direct ELF64 binary-inspection tool implemented in C.

`binspect` parses ELF structures itself rather than invoking `readelf`, `nm`, or `objdump`. Binutils tools are used only in the test suite as an external oracle for selected facts.

## Supported Format

The inspector intentionally supports:

```text
ELF64
little-endian
Linux-style ELF files
```

Unsupported or malformed files are rejected.

## Commands

```bash
binspect FILE summary
binspect FILE sections
binspect FILE segments
binspect FILE symbols
binspect FILE needed
binspect FILE offset-to-vaddr VALUE
binspect FILE vaddr-to-offset VALUE
```

## Capabilities

### ELF summary

Reports:

- ELF class
- byte order
- ELF type
- machine value
- entry point
- program-header count
- section-header count

### Sections

Enumerates section names, types, file offsets, sizes, and virtual addresses.

### Program headers

Enumerates segment type, file offset, virtual address, file size, memory size, and flags.

### Symbols

Reads both `.symtab` and `.dynsym` when present and reports named symbols with value, size, binding, and type.

### Dynamic dependencies

Reads `SHT_DYNAMIC` and its linked string table to report `DT_NEEDED` shared-library names.

### Address mapping

Maps file offsets to virtual addresses and vice versa through file-backed `PT_LOAD` segments.

## Safety Model

The parser validates table ranges and string-table bounds before dereferencing ELF metadata.

The project rejects:

- files smaller than an ELF header;
- invalid magic;
- non-ELF64 inputs;
- non-little-endian inputs;
- malformed header-table ranges;
- invalid section-name table references.

Further command-specific checks validate symbol and dynamic table bounds.

## Testing

```bash
make test
```

The suite builds a known ELF fixture and checks:

- header summary;
- `.text` and `.symtab` discovery;
- `PT_LOAD` segments;
- known symbols;
- dynamic dependencies;
- header-count parity with `readelf`;
- file-offset/virtual-address round-trip mapping;
- truncated-file rejection;
- non-ELF rejection.

## Sanitizers

```bash
make sanitize
```

## Full Validation

```bash
./scripts/validate.sh
```

Evidence is written to:

```text
evidence/validation.txt
```

## Scope

This is not a replacement for binutils and does not claim support for ELF32, big-endian ELF, every architecture-specific extension, relocations, DWARF parsing, disassembly, or object rewriting.

Its focus is safe direct parsing of core ELF64 runtime structures.
