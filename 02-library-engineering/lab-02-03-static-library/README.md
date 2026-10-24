# Lab 02-03 — Static Library

## Objective

Build a reusable static archive and link a separate consumer against it.

## Engineering Focus

The lab demonstrates:

- compiling library objects separately;
- creating an archive with `ar`;
- indexing the archive;
- linking a consumer with `-L` and `-l`;
- inspecting archive members and defined symbols;
- confirming the final executable does not depend on a project-specific shared object.

## Artifact

```text
build/lib/libtextops.a
```

## Validation

```bash
./scripts/validate.sh
```
