# Waitpid and Child Reaping — Engineering Report

## Objective

Create multiple child processes with distinct exit statuses and explicitly reap each child with `waitpid()`.

## Runtime Observations

The children reported:

```text
child index=0 ... exit=11
child index=1 ... exit=22
child index=2 ... exit=33
```

The parent then reported:

```text
reaped ... exit=11
reaped ... exit=22
reaped ... exit=33
```

The validation confirmed:

```text
three_children_reaped=yes
```

## Engineering Interpretation

A terminating child retains exit-status information until its parent collects that status.

Calling `waitpid()` establishes explicit lifecycle ownership and prevents the terminated children from remaining unreaped.

The distinct exit codes also show how process termination state can be communicated back to the parent.

## Result

PASS — three children were created, terminated with distinct statuses, and successfully reaped.
