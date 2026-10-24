# Process Trees and Ancestry — Engineering Report

## Objective

Create and verify a deterministic three-generation process tree.

## Runtime Observations

The program reported:

```text
root pid=7917 ppid=7905
child pid=7918 ppid=7917
grandchild pid=7919 ppid=7918
```

The validation confirmed:

```text
three_generation_ancestry_verified=yes
```

## Engineering Interpretation

The child process's PPID matched the root process's PID, and the grandchild's PPID matched the child process's PID.

This directly demonstrates process ancestry and the tree structure created by successive calls to `fork()`.

The parent and child also waited for their descendants, keeping the intended hierarchy intact long enough to inspect and verify it.

## Result

PASS — root, child, and grandchild ancestry were verified through PID/PPID relationships.
