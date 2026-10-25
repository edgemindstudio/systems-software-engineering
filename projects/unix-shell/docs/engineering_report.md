# Unix Shell — Engineering Report

## Objective

Build a compact Unix-style shell in C that integrates command parsing, process creation, program execution, pipelines, file-descriptor redirection, built-ins, signal behavior, and exit-status handling.

The implemented shell executable is:

```text
build/sshell
```

## Development Environment

Validation was performed on:

```text
Debian clang version 21.1.8 (7+b1)
Linux 6.6.87.2-microsoft-standard-WSL2 x86_64 GNU/Linux
```

## Build Quality

The normal build compiled all translation units with C17 and strict warnings enabled:

```text
-Wall
-Wextra
-Wpedantic
-Wconversion
-Wshadow
-Wstrict-prototypes
-Wmissing-prototypes
```

The validation run completed without compiler warnings.

## Integration Validation

The integration suite exercised:

- external command execution;
- single-quoted arguments;
- double-quoted arguments;
- backslash escaping;
- multi-stage pipelines;
- output redirection with truncation;
- output redirection with append;
- input redirection;
- the `pwd` built-in;
- parent-side `cd` persistence;
- built-in output redirection;
- previous-command status reporting;
- command-not-found exit behavior;
- explicit `exit` status;
- malformed pipeline rejection.

Observed result:

```text
integration_tests=PASS
```

## Representative Runtime Evidence

### Simple Command

```text
shell-ok
```

This verified direct foreground command execution through the shell.

### Pipeline

The representative pipeline:

```text
printf 'alpha\nbeta\n' | grep beta | tr a-z A-Z
```

produced:

```text
BETA
```

This verified pipe creation, descriptor routing, process creation, and multi-stage foreground execution.

### Redirection

A command wrote:

```text
redirected
```

to a file with `>` and a later command read it back through `<`.

Observed output:

```text
redirected
```

This verified output truncation redirection and input redirection.

### Built-ins

`pwd` reported:

```text
/home/fonke/systems-software-engineering/projects/unix-shell
```

and `help` reported:

```text
builtins: cd pwd status help exit
operators: | < > >>
```

The integration suite separately verified that `cd` persisted across subsequent commands because a standalone built-in executes in the parent shell process.

## Execution Model

External commands are executed with:

- `fork()`;
- child-side descriptor setup;
- `execvp()`.

For a pipeline with `N` stages, the shell constructs `N - 1` pipes, forks each stage, connects standard input/output with `dup2()`, closes inherited pipe descriptors, and waits for all foreground children.

The reported pipeline status is the status of the final stage.

## Built-in Model

The implemented built-ins are:

```text
cd
pwd
status
help
exit
```

A standalone built-in executes in the shell process. This permits state-changing operations such as `cd` and `exit` to affect the shell.

A built-in appearing in a pipeline executes in a child and therefore cannot mutate the parent shell state.

## Parsing Model

The parser supports:

- unquoted words;
- single quotes;
- double quotes;
- backslash escaping;
- `|`;
- `<`;
- `>`;
- `>>`.

Malformed input is rejected with a nonzero parse status. The integration suite explicitly verified rejection of a pipeline ending with an empty stage.

## Exit-Status Behavior

The shell preserves the status of the previous command line.

The integration suite verified:

- status `1` after `false`;
- status `127` for command-not-found behavior;
- explicit shell exit status `37`.

A child terminated by a signal is represented as `128 + signal_number`.

## Signal Policy

The shell process ignores:

```text
SIGINT
SIGQUIT
```

Children restore the default disposition for both signals before executing command logic.

This allows foreground child commands to observe normal Unix signal behavior while keeping the shell itself from terminating from those signals.

Full terminal process-group job control is intentionally outside project scope.

## Sanitizer Validation

The project was rebuilt using:

```text
-fsanitize=address,undefined
-fno-omit-frame-pointer
```

with:

```text
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1
UBSAN_OPTIONS=halt_on_error=1
```

The complete integration suite was rerun under the sanitizer build.

Observed result:

```text
integration_tests=PASS
```

No AddressSanitizer or UndefinedBehaviorSanitizer failure interrupted the validation run.

## Engineering Interpretation

This project demonstrates several core systems-software engineering competencies:

1. **Process lifecycle management** — foreground child creation and collection are explicit.
2. **Descriptor plumbing** — pipes and redirections are implemented using Unix file descriptors and `dup2()`.
3. **Parser ownership discipline** — dynamically allocated token data is released through pipeline teardown.
4. **Parent/child semantic separation** — shell-state-changing built-ins execute in the parent when required.
5. **Exit-status semantics** — command failures are propagated rather than hidden.
6. **Signal-awareness** — parent and child signal behavior are deliberately different.
7. **Failure handling** — malformed syntax, missing executables, and system-call failures produce nonzero status.
8. **Evidence-backed validation** — the same integration suite succeeds under normal and sanitizer builds.

## Scope

This shell intentionally does not claim Bash compatibility.

The implementation excludes:

- background execution;
- terminal process-group job control;
- glob expansion;
- variable expansion;
- command substitution;
- `&&` and `||`;
- semicolon-separated command lists;
- shell scripting control structures.

These boundaries keep the project focused on process creation, execution, descriptors, parsing, and lifecycle behavior.

## Result

**PASS**

The Unix Shell successfully completed strict compilation, normal integration validation, representative command execution, pipeline and redirection checks, ASan/UBSan validation, and restoration of the normal optimized build on the development environment.
