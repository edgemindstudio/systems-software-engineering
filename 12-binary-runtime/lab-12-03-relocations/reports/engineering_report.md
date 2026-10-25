# Relocations — Engineering Report

## Objective
Inspect a relocatable object with an unresolved external function reference.

## Observations
```text
0000000000000000 T call_external
                 U external_transform
```
The `.rela.text` section contained an `R_X86_64_PLT32` relocation for `external_transform - 4`. The final build was warning-clean.

## Engineering Interpretation
The object defines `call_external` but leaves `external_transform` unresolved for the linker. The relocation records how that reference must be fixed during linking.

## Result
PASS — undefined-symbol and relocation behavior were verified.
