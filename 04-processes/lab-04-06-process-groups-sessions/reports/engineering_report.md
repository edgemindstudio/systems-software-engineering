# Process Groups and Sessions — Engineering Report

## Objective

Inspect process-group and session identifiers and demonstrate explicit creation of a new process group and a new session.

## Runtime Observations

The parent reported:

```text
parent pid=7965 ppid=7953 pgid=7831 sid=424
```

The process-group child reported:

```text
group_child pid=7966 ppid=7965 pgid=7966 sid=424
```

The session child reported:

```text
session_child pid=7967 ppid=7965 pgid=7967 sid=7967
```

The validation confirmed:

```text
new_process_group_verified=yes
new_session_verified=yes
```

## Engineering Interpretation

After `setpgid(0, 0)`, the group child became the leader of a new process group, so its PGID matched its PID while it remained in the existing session.

After `setsid()`, the session child became the leader of a new session and a new process group, making:

```text
PID = PGID = SID
```

These relationships underpin shell job control, terminal ownership, daemonization, and grouped signal delivery.

## Result

PASS — process-group creation and new-session creation behaved as expected.
