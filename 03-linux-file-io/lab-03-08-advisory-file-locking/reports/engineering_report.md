# Advisory File Locking — Engineering Report

## Objective

Demonstrate process coordination using POSIX advisory record locks through `fcntl`.

## Runtime Observations

The first process acquired the lock:

```text
lock=acquired pid=7330
```

While that process held the lock, a competing process reported:

```text
lock=busy
contender_exit_status=3
```

After the holder exited and released the lock, another attempt reported:

```text
lock=acquired pid=7336
```

## Engineering Interpretation

The advisory write lock prevented a cooperating process using the same locking protocol from acquiring the conflicting lock.

The lock was released when the holder closed the file descriptor / terminated, after which another process could acquire it.

Advisory locks coordinate participants that honor the protocol; they do not generally make ordinary reads and writes impossible for unrelated processes that ignore the lock.

## Result

PASS — lock acquisition, contention detection, and post-release acquisition were demonstrated successfully.
