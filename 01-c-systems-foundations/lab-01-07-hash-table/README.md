# Lab 01-07 — Hash Table

## Objective

Implement a small string-to-integer hash table using separate chaining and automatic resizing.

## Engineering Focus

The implementation demonstrates:

- deterministic string hashing;
- bucket indexing;
- collision handling through linked chains;
- key replacement;
- dynamic table resizing;
- ownership of copied keys;
- deletion and full cleanup.

## Complexity Expectations

Typical average-case behavior with a reasonable load factor:

```text
insert: O(1) average
lookup: O(1) average
remove: O(1) average
resize: O(n)
```

Worst-case chained lookup remains O(n).

## Employer Signal

This lab combines hashing, heap allocation, pointer manipulation, ownership, resizing, error propagation, and tests in one reusable data structure.
