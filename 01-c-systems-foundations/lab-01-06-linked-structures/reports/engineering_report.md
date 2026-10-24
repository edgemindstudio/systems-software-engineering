# Linked Structures — Engineering Report

## Objective

Implement a singly linked list with explicit heap ownership and correct structural updates during insertion, lookup, removal, and destruction.

## Implementation Summary

The list tracks:

- head pointer;
- tail pointer;
- element count.

Operations include:

- push front;
- push back;
- search;
- removal of the first matching value;
- destruction of all nodes.

## Runtime Observations

The demonstration produced:

```text
size=3 values: 5 10 30
```

after a sequence of front insertion, back insertion, and removal.

The automated tests exercised:

- an empty list;
- head insertion;
- tail insertion;
- head removal;
- tail removal;
- removal of the final node;
- missing-value lookup and removal;
- full cleanup.

## Failure Modes Considered

Allocation failure is propagated to the caller.

Head and tail pointers are updated carefully when removing boundary nodes, and both return to `NULL` after the final node is removed.

`int_list_destroy` walks the list without dereferencing freed nodes by saving each `next` pointer before release.

## Validation

The lab completed:

- a clean strict-warning build;
- automated tests;
- AddressSanitizer validation;
- UndefinedBehaviorSanitizer validation.

No warnings, memory errors, or sanitizer failures were reported.

## Result

PASS — linked-list ownership and structural invariants remained correct across insertion, lookup, removal, and cleanup.
