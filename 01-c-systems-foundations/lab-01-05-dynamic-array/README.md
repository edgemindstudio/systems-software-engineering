# Lab 01-05 — Dynamic Array

## Objective

Build a reusable vector-style integer container with explicit invariants and amortized growth.

## Interface

The container supports:

- initialization and destruction;
- push-back;
- indexed access with bounds checking;
- insertion;
- removal;
- capacity growth.

## Invariants

At all valid times:

```text
size <= capacity
data != NULL when capacity > 0
elements [0, size) are initialized
```

## Employer Signal

This lab moves from raw allocation to a reusable abstraction with an API, invariants, tests, growth policy, and failure handling.
