# Multi-Process Pipeline — Engineering Report

## Objective

Construct and validate a two-stage process pipeline using descriptor redirection and `exec`.

## Runtime Observations

```text
      2 alpha
      1 beta
```

The validation confirmed:

```text
pipeline_counts_verified=yes
```

`strace -f` captured `pipe2`, `clone`, `dup2`, `execve`, `close`, and `wait4`.

## Engineering Interpretation

The first child redirected input/output around `sort`; the second redirected input into `uniq -c`. This is the same descriptor-plumbing pattern shells use for command pipelines.

## Result

PASS — pipeline construction, descriptor redirection, image replacement, and result verification succeeded.
