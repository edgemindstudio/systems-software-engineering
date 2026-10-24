# Hash Table — Engineering Report

## Objective

Implement a reusable string-to-integer hash table combining hashing, separate chaining, owned keys, replacement semantics, resizing, lookup, deletion, and cleanup.

## Implementation Summary

The table uses:

- deterministic string hashing;
- bucket indexing;
- separate chaining for collisions;
- copied/owned string keys;
- replacement when an existing key is inserted again;
- automatic bucket-array growth;
- full entry and key cleanup.

## Runtime Observations

The demonstration inserted several entries, replaced an existing key, removed one key, and produced:

```text
beta=200
size=3 buckets=8
```

The original table started with four buckets and expanded to eight as the load increased.

The automated tests inserted 100 generated keys, verified every lookup, replaced an existing value without increasing table size, removed an entry, checked a missing key, and validated full destruction.

## Failure Modes Considered

The implementation checks allocation failures for both entries and copied keys.

Resize allocation failure leaves the existing table intact.

Removal correctly reconnects bucket chains and frees both the owned key and entry.

Potential bucket-count growth overflow is checked before resizing.

## Validation

The lab completed:

- a clean strict-warning build;
- automated tests over 100 entries;
- AddressSanitizer validation;
- UndefinedBehaviorSanitizer validation.

No warnings, memory errors, or sanitizer failures were reported.

## Result

PASS — hashing, replacement, lookup, removal, collision chains, resizing, ownership, and cleanup behaved correctly under tests and sanitizers.
