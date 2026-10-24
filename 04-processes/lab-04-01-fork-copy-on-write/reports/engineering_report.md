# Fork and Copy-on-Write — Engineering Report

## Objective

Create a child process with `fork()` and verify that parent and child continue from the same control point while maintaining independent process-local state after the fork.

## Runtime Observations

Before the fork:

```text
before_fork pid=7846 value=41
```

The child reported:

```text
child pid=7847 ppid=7846 value=42
```

The parent reported:

```text
parent pid=7846 child=7847 value=141
```

The validation confirmed:

```text
independent_post_fork_values=yes
```

## Engineering Interpretation

The parent and child begin with logically identical user-space state after `fork`, but subsequent writes are process-local.

The observed values show that the child incremented its copy from 41 to 42 while the parent independently changed its copy from 41 to 141.

This is consistent with the copy-on-write implementation strategy commonly used by Linux, while the program relies only on the semantic guarantee of separate process address spaces.

## Result

PASS — parent/child identity and independent post-fork state were verified.
