# Inheritance Across Fork/Exec — Engineering Report

## Objective

Inspect process state inherited across `fork()` and retained or filtered across `exec()`.

## Runtime Observations

The exec target reported:

```text
cwd=/home/fonke/systems-software-engineering/04-processes/lab-04-05-fork-exec-inheritance
env_token=visible-after-exec
inherited_fd_open=yes
cloexec_fd_open=no
```

The validation confirmed:

```text
cwd_environment_fd_inheritance_verified=yes
cloexec_verified=yes
```

## Engineering Interpretation

The current working directory and environment remained available after `fork` and `exec`.

A normal open descriptor also remained open across the `exec` boundary.

The second descriptor had `FD_CLOEXEC` set, so the kernel closed it during successful image replacement.

This demonstrates why descriptor-inheritance policy matters in process launchers, supervisors, shells, and security-sensitive code.

## Result

PASS — cwd, environment, ordinary descriptor inheritance, and close-on-exec behavior were verified.
