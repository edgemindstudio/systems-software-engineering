# ABI Breakage Investigation — Engineering Report

## Objective

Reproduce and safely detect an ABI incompatibility that is not prevented by matching function names or a matching shared-library SONAME.

## Scenario

The consumer was compiled against version 1 of:

```c
struct record {
    uint32_t id;
    uint32_t value;
};
```

Version 2 changed the public structure to include an additional `uint64_t` field while retaining:

- the same SONAME: `librecord.so.1`;
- the same exported function names.

## Runtime Observations

With the v1 library, the consumer reported:

```text
consumer_struct_size=8
library_struct_size=8
score=7042
```

The consumer and library therefore agreed on the binary layout.

With the v2 library, the same already-compiled consumer reported:

```text
ABI MISMATCH: public struct layout changed
consumer_struct_size=8
library_struct_size=24
v2_exit_status=2
```

Despite the layout mismatch, both shared objects reported:

```text
SONAME: librecord.so.1
```

and both exported:

```text
record_abi_size
record_score
```

## Engineering Interpretation

The experiment demonstrates that preserving source-level function names and the ELF SONAME does not automatically preserve ABI compatibility.

Because callers allocate or embed `struct record` directly, its size, alignment, field offsets, and layout are part of the binary contract.

A replacement library can therefore satisfy the dynamic loader yet still be binary-incompatible with an existing consumer.

The compatibility check prevents the demonstration from intentionally invoking a layout-sensitive operation after the mismatch is detected.

## Validation

The lab completed:

- separate v1 and v2 shared-library builds;
- consumer compilation against the v1 header;
- successful v1 execution;
- deliberate v2 replacement;
- runtime size mismatch detection;
- nonzero controlled rejection status;
- SONAME comparison;
- exported-symbol comparison;
- automated compatibility test.

## Result

PASS — the lab reproduced a concrete ABI break and demonstrated why API stability alone does not guarantee binary compatibility.
