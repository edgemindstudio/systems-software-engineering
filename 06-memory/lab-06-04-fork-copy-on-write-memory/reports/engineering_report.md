# Fork Copy-on-Write Semantics — Engineering Report

## Objective

Verify that a private mapping inherited across `fork()` becomes independently mutable in parent and child.

## Runtime Observations

```text
child_value=200
parent_value_after_child_write=100
parent_value_after_parent_write=300
child_exit_success=yes
post_fork_memory_isolation_verified=yes
```

## Engineering Interpretation

Both processes began from the same pre-fork logical memory state.

When the child wrote `200`, the parent continued to observe `100`, demonstrating that the child modification did not overwrite the parent's private page.

The parent then independently changed its own copy to `300`.

This behavior is consistent with Linux copy-on-write semantics for private memory across `fork()`.

## Result

PASS — post-fork private-memory isolation was verified.
