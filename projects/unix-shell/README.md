# Unix Shell

A compact Unix-style shell implemented in C.

## Features

- `fork()` + `execvp()` external command execution
- foreground pipelines with `|`
- `<`, `>`, and `>>` redirection
- single quotes, double quotes, and backslash escaping
- built-ins: `cd`, `pwd`, `status`, `help`, `exit`
- parent-side built-in execution so `cd` persists
- exit-status propagation
- shell ignores `SIGINT`/`SIGQUIT`; children restore defaults
- deterministic `-c` mode for testing
- strict-warning build and integration suite
- ASan/UBSan validation

## Build and Run

```bash
make
./build/sshell
./build/sshell -c 'printf "hello\n" | tr a-z A-Z'
```

## Supported Syntax

```text
command [args...]
command | command [| command ...]
command < input
command > output
command >> output
```

## Built-ins

```text
cd [DIR]
pwd
status
help
exit [0..255]
```

## Scope

This is not a Bash-compatible language. It intentionally excludes background jobs, terminal job control, globbing, variable expansion, command substitution, `&&`, `||`, and shell scripting constructs.

The focus is process creation, file-descriptor plumbing, parsing, foreground execution, and lifecycle handling.

## Validation

```bash
make test
make sanitize
./scripts/validate.sh
```

The full validation transcript is written to `evidence/validation.txt`.
