# Lab 04-02 — Exec and Process Image Replacement

## Objective

Show that `execve()` replaces the current process image while preserving the process identity (PID).

The launcher forks a child. The child records its PID and then execs a separate target program. The target reports the same PID plus argv/environment data.
