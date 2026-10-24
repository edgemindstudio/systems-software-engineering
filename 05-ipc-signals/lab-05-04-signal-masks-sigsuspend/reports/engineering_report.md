# Signal Masks and sigsuspend — Engineering Report

## Objective

Demonstrate blocked delivery, pending-signal state, and race-free waiting with `sigsuspend()`.

## Runtime Observations

```text
pending_before_sigsuspend=yes
handler_observed_after_unblock=yes
```

The validation confirmed:

```text
masked_pending_signal_verified=yes
sigsuspend_wakeup_verified=yes
```

## Engineering Interpretation

`SIGUSR1` was blocked before delivery, became pending, and was then handled when `sigsuspend()` temporarily replaced the mask atomically.

## Result

PASS — pending-signal behavior and race-free wakeup were verified.
