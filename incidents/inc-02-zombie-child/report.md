# INC-02 — Zombie Child Process

## Symptom

A terminated child remains visible in process state as a zombie.

## Reproduction

The child exits immediately. The parent deliberately delays its `waitpid()` call and inspects the child's `/proc/<pid>/stat` state.

## Evidence

```text
child_state_before_wait=Z
zombie_observed=yes
child_reaped=yes
```

## Root Cause

Child termination releases most resources, but the kernel retains exit status and accounting information until the parent reaps the child.

## Remediation

Use `wait()`, `waitpid()`, or an appropriate SIGCHLD handling strategy to reap completed children.

## Verification

The child was observed in state `Z` before the wait and was successfully reaped afterward.

## Engineering Lesson

Process creation implies lifecycle responsibility. A parent that forks children must also plan how their termination status will be collected.
