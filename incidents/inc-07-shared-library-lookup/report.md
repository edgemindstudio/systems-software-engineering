# INC-07 — Shared-Library Lookup Failure

## Symptom

A successfully linked executable fails at process startup because its custom shared library cannot be found.

## Reproduction

The executable is linked against `libincidentdemo.so` in a project-local directory without embedding a runtime search path.

## Evidence

Initial execution failed:

```text
error while loading shared libraries: libincidentdemo.so: cannot open shared object file
failure_exit_code=127
```

Before remediation, `ldd` reported:

```text
libincidentdemo.so => not found
```

After supplying the project library directory through `LD_LIBRARY_PATH`:

```text
demo_value=77
```

and `ldd` resolved the dependency from the local `build/lib` directory.

## Root Cause

Link-time discovery and runtime discovery are separate concerns. `-Lbuild/lib` allowed the linker to find the library during the build but did not tell the runtime loader where to search later.

## Remediation

Provide an intentional runtime discovery strategy. For this incident, `LD_LIBRARY_PATH` demonstrates the remediation. For deployable software, an appropriate RUNPATH or installation policy may be preferable.

## Verification

The initially unresolved dependency became resolvable, and the executable ran successfully.

## Engineering Lesson

A successful link does not guarantee a successful launch. Runtime loader search policy must be treated as part of deployment design.
