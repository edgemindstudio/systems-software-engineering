# 04 — Processes

## Purpose

This section develops practical Linux process-management skills at the system-call level.

The labs focus on process creation, process-image replacement, child lifecycle management, multi-generation ancestry, inherited process state, and process-group/session semantics.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- create child processes with `fork`;
- reason about parent/child execution and independent post-fork state;
- replace a process image with `execve`;
- pass arguments and environment state across `exec`;
- collect child status with `waitpid`;
- avoid leaving terminated children unreaped;
- build and inspect multi-generation process trees;
- reason about PID and PPID relationships;
- verify current-working-directory and environment inheritance;
- inspect file-descriptor inheritance across `fork` and `exec`;
- control descriptor inheritance with `FD_CLOEXEC`;
- inspect and manipulate process groups;
- create a new session with `setsid`;
- retain reproducible runtime evidence.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 04-01 | Fork and Copy-on-Write | parent/child PIDs, independent post-fork state |
| 04-02 | Exec and Process Image Replacement | stable PID across `execve`, argv/environment propagation |
| 04-03 | Waitpid and Child Reaping | multiple children, exit status collection |
| 04-04 | Process Trees and Ancestry | root/child/grandchild PID/PPID relationships |
| 04-05 | Inheritance Across Fork/Exec | cwd, environment, descriptor inheritance, `FD_CLOEXEC` |
| 04-06 | Process Groups and Sessions | PID/PGID/SID inspection, `setpgid`, `setsid` |

## Quality Standard

Every lab contains source code, a strict-warning Makefile, a repeatable validation script, retained runtime evidence, a README, and an engineering report.

Generated build and runtime artifacts are excluded from version control.

## Section Validation

Run:

```bash
./scripts/validate_all.sh
```

The section-wide validator performs clean builds, executes deterministic process experiments, and captures machine-generated evidence under each lab's `evidence/validation.txt`.

## Completed Results

All six labs passed on the development environment.

Key observed results included:

- a child process changing its post-fork value from 41 to 42 while the parent independently changed its copy to 141;
- an `execve` transition where the PID remained unchanged before and after image replacement;
- preservation of expected `argv` values and environment state across `exec`;
- successful collection of three child exit codes: 11, 22, and 33;
- verification of a three-generation process tree through PID/PPID relationships;
- current-working-directory and environment inheritance across fork/exec;
- a normal descriptor remaining open across `exec` while an `FD_CLOEXEC` descriptor was closed;
- creation of a new process group where the child's PID matched its PGID;
- creation of a new session where the session child's PID matched both its PGID and SID.

## Progress

- Lab 04-01 — Fork and Copy-on-Write: Complete
- Lab 04-02 — Exec and Process Image Replacement: Complete
- Lab 04-03 — Waitpid and Child Reaping: Complete
- Lab 04-04 — Process Trees and Ancestry: Complete
- Lab 04-05 — Inheritance Across Fork/Exec: Complete
- Lab 04-06 — Process Groups and Sessions: Complete

Domain 04 — Processes: Complete
