# Lab 06-07 — Address-Space Limits

## Objective

Apply a strict `RLIMIT_AS` limit in a child process and verify that an oversized `malloc()` fails cleanly instead of destabilizing the parent process.
