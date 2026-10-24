# Safe Signal Handling — Engineering Report

## Objective

Install a `SIGUSR1` handler with `sigaction()` and keep handler work limited to async-signal-safe operations.

## Runtime Observations

```text
handler_received_SIGUSR1
main_observed_signal=yes
```

The validation confirmed:

```text
sigaction_delivery_verified=yes
```

## Engineering Interpretation

The handler updated `volatile sig_atomic_t` state and used `write()` for the fixed diagnostic message rather than unsafe stdio calls.

## Result

PASS — signal installation, delivery, safe handler behavior, and main-flow observation were verified.
