# Exec and Process Image Replacement — Engineering Report

## Objective

Verify that `execve()` replaces the current process image without creating a new process identity.

## Runtime Observations

The child reported immediately before `execve`:

```text
pre_exec_pid=7868
```

The target program reported:

```text
post_exec_pid=7868
argc=3
argv1=alpha
argv2=beta
env_token=exec-preserved-env
```

The validation confirmed:

```text
pid_preserved_across_exec=yes
```

`strace -f` also captured the process creation, `execve`, and `wait4` lifecycle.

## Engineering Interpretation

`execve()` does not create another process. It replaces the calling process's program image while preserving its PID.

The target received the explicitly supplied argument vector and inherited environment state, demonstrating how process identity and selected process attributes survive image replacement.

## Result

PASS — PID continuity, argument delivery, environment propagation, and process-image replacement were verified.
