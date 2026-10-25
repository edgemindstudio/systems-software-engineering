# Unix Shell Architecture

The shell is split into parser, built-in, executor, and utility modules.

`parser.c` transforms one input line into a bounded pipeline representation. It recognizes words, quoting/escaping, pipes, and redirections, and rejects malformed syntax.

`builtins.c` implements `cd`, `pwd`, `status`, `help`, and `exit`. A single-command built-in runs in the parent so state changes such as `cd` persist. A built-in in a pipeline runs in a child.

`execute.c` creates `N-1` pipes for `N` stages, forks each stage, connects descriptors with `dup2()`, applies redirection, closes inherited pipe descriptors, executes built-ins or `execvp()`, and waits for every child. The pipeline status is the status of the final stage.

The parent shell ignores `SIGINT` and `SIGQUIT`; children restore default handling before execution. Full terminal process-group job control is intentionally outside scope.

Parser allocations are owned by the pipeline and released with `sshell_pipeline_destroy()` after execution.
