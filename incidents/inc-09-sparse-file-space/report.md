# INC-09 — Sparse-File Space Misinterpretation

## Symptom

A file appears to be roughly 64 MiB in logical size while consuming only a few KiB of physical filesystem blocks.

## Reproduction

The program seeks 64 MiB beyond the beginning of a new file and writes a single byte.

## Evidence

```text
logical_bytes=67108865
allocated_bytes=4096
sparse_behavior_observed=yes
```

The shell tools reported approximately:

```text
4.0K    workspace/sparse.bin
65M     workspace/sparse.bin   # apparent size
```

## Root Cause

The seek created a hole rather than materializing every intervening byte. The filesystem tracks a large logical address range while allocating blocks only for written data and metadata.

## Remediation

When diagnosing storage usage, distinguish logical file size from allocated disk blocks. Use tools/options that report the quantity relevant to the investigation.

## Verification

The file's logical size greatly exceeded its allocated byte count.

## Engineering Lesson

File size and disk consumption are different measurements. Sparse files can make either metric misleading when interpreted as the other.
