# Lab 01-04 — Dynamic Memory

## Objective

Implement explicit heap ownership, allocation failure handling, resizing, and cleanup.

## Engineering Focus

The lab provides a small owned integer buffer that supports:

- initialization;
- checked allocation;
- growth with `realloc`;
- push operations;
- deterministic destruction;
- sanitizer validation.

## Ownership Rule

After successful initialization, the buffer owns `data`. Only `int_buffer_destroy` releases it. Callers do not free internal storage directly.

## Employer Signal

The key outcome is not merely calling `malloc`; it is designing and testing a clear ownership contract and proving that the normal path is free of sanitizer-detected memory errors.
