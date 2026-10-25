# Binary Inspector Architecture

## Design

The project has two main layers:

```text
elf_view.c
    validated raw-file view
    ELF header/table access
    section-name lookup
    PT_LOAD address mapping

main.c
    command implementation
    section/segment enumeration
    symbol-table parsing
    dynamic dependency parsing
```

## File Loading

The complete file is read into a heap buffer.

This makes pointer arithmetic straightforward while retaining explicit range checks before metadata is accessed.

## Structural Validation

`elf_view_validate()` checks:

- ELF magic;
- ELF64 class;
- little-endian encoding;
- current ELF version;
- expected ELF header size;
- program-header entry size and table bounds;
- section-header entry size and table bounds;
- section-name table index validity.

## Section Names

Section names are indexes into the section-header string table.

The inspector verifies:

1. the string table lies within the file;
2. the name offset lies within that table;
3. a terminating NUL exists before the table ends.

## Symbols

For `SHT_SYMTAB` and `SHT_DYNSYM`, the tool validates the symbol table, linked string table, entry size, symbol-name offsets, and NUL termination before printing symbols.

## Dynamic Dependencies

For `SHT_DYNAMIC`, the linked string table is used to resolve `DT_NEEDED` values.

No dependency name is printed unless its offset is within the table and a terminator exists.

## Address Mapping

Only file-backed bytes in `PT_LOAD` segments are mapped.

For an offset within:

```text
[p_offset, p_offset + p_filesz)
```

the virtual address is:

```text
p_vaddr + (offset - p_offset)
```

The reverse mapping applies the inverse relation.

## Oracle Testing

The implementation does not shell out to binutils in production.

Tests use `readelf` only to compare selected independently parsed values, such as program-header and section-header counts and dynamic dependency names.
