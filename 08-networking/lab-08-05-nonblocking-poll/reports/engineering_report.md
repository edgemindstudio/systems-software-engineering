# Nonblocking I/O with poll — Engineering Report

## Objective

Put a socket into nonblocking mode, observe the no-data condition, and use `poll()` to wait for readability.

## Runtime Observations

```text
initial_recv_eagain=yes
poll_readable=yes
received=poll-ready
nonblocking_poll_verified=yes
nonblocking_readiness_verified=yes
```

## Engineering Interpretation

The first nonblocking `recv()` correctly returned the `EAGAIN`/`EWOULDBLOCK` condition because no data was available.

After data was written to the peer socket, `poll()` reported the descriptor readable and the subsequent receive returned the expected payload.

This demonstrates readiness-driven I/O rather than blocking indefinitely in a read call.

## Result

PASS — nonblocking behavior and readiness notification were verified.
