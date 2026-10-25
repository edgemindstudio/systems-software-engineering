# INC-04 — Heap Use-After-Free

## Symptom

The program reads from heap memory after that allocation has already been released.

## Reproduction

The test allocates an integer, stores `42`, frees the allocation, and then dereferences the stale pointer.

## Evidence

AddressSanitizer reported:

```text
ERROR: AddressSanitizer: heap-use-after-free
READ of size 4
```

The diagnostic identified the invalid read in `src/main.c:15`, the preceding `free()` in `src/main.c:10`, and the original `malloc()` in `src/main.c:6`.

The sanitizer exited nonzero:

```text
asan_exit_code=1
```

## Root Cause

The pointer remained syntactically available after `free()`, but the allocation's lifetime had ended. Dereferencing it therefore accessed invalid storage.

## Remediation

Do not access an allocation after releasing it. Establish explicit ownership and lifetime rules, and clear or transfer stale references when appropriate.

## Verification

AddressSanitizer deterministically detected the invalid read and provided allocation/free/read provenance.

## Engineering Lesson

Memory bugs are fundamentally lifetime bugs. Sanitizer evidence is especially valuable because it connects the symptom to both the allocation and release sites.
