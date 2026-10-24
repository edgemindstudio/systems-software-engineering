# Virtual Address Space — Engineering Report

## Objective

Inspect representative process-memory regions and correlate runtime addresses with Linux process mappings.

## Runtime Observations

The program reported representative addresses:

```text
code=0x558c5ba20270
global=0x558c5ba23038
heap=0x558c90e91310
stack=0x7ffdd7b74528
values=7,13,11
```

The `/proc` snapshot also showed executable mappings, a `[heap]` region, and a `[stack]` region.

## Engineering Interpretation

The executable image, global/static storage, heap allocation, and stack object occupy distinct regions of the process virtual address space.

The observed addresses and `/proc` mappings demonstrate that Linux presents each process with a structured virtual address space rather than one undifferentiated memory block.

## Result

PASS — representative virtual-memory regions were observed and correlated with process mappings.
