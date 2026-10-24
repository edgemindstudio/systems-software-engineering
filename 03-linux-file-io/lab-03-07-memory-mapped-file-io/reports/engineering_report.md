# Memory-Mapped File I/O — Engineering Report

## Objective

Map a regular file into memory, modify it through a shared mapping, synchronize the mapping, and verify persistent filesystem changes.

## Runtime Observations

Before the mapping mutation, the file contained:

```text
hello from mmap validation
```

The program reported:

```text
mapped_size=27
new_prefix=MMAP!
```

After execution, normal file I/O observed:

```text
MMAP! from mmap validation
```

The validation confirmed:

```text
mapped_mutation_persisted=yes
```

`strace` captured the relevant mapping operations:

```text
mmap(NULL, 27, PROT_READ|PROT_WRITE, MAP_SHARED, 3, 0) = ...
msync(..., 27, MS_SYNC) = 0
munmap(..., 27) = 0
```

## Engineering Interpretation

`MAP_SHARED` allows modifications made through the memory mapping to update the underlying file.

`msync(..., MS_SYNC)` explicitly requests synchronization before the mapping is removed.

The experiment also distinguishes application-created mappings from other mappings established by the runtime loader and C library.

## Result

PASS — shared mapping, synchronized mutation, and persistence to the underlying file were verified.
