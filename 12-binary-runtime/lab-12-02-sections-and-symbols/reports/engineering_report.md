# Sections and Symbols — Engineering Report

## Objective
Correlate source-level functions/globals with ELF sections and symbol-table entries.

## Observations
Program output was `exported_add=12`. Selected sections were `.text`, `.data`, and `.bss`.

Selected symbols:
```text
0000000000001140 T exported_add
0000000000004018 D initialized_global
0000000000004020 B zero_initialized_global
```
The final build was warning-clean.

## Engineering Interpretation
`T` maps the function to executable code, `D` maps initialized writable data, and `B` maps zero-initialized storage.

## Result
PASS — sections and symbol classifications were verified.
