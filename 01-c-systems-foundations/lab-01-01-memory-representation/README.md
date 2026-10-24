# Lab 01-01 — Memory Representation

## Objective

Inspect how typed C objects are represented in memory and connect source-level declarations to byte-level layout.

## Engineering Questions

- How are integer bytes ordered in memory?
- How can endianness be detected without assuming a platform?
- How does `sizeof` relate to object representation?
- Why can a structure contain padding?
- How do `_Alignof` and `offsetof` expose layout constraints?

## Program

The program prints:

- the byte representation of a fixed `uint32_t`;
- detected byte order;
- sizes of selected integer types;
- offsets and total size of a padded structure;
- alignment requirements.

## Validation

```bash
make
./build/lab
make test
make sanitize
```

Then:

```bash
./scripts/validate.sh
```

## Employer Signal

This lab demonstrates byte-level reasoning, portability awareness, and understanding of alignment and structure layout—concepts that matter in binary formats, networking, ABI work, device interfaces, and low-level debugging.
