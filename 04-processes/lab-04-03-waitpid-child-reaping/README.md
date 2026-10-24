# Lab 04-03 — Waitpid and Child Reaping

## Objective

Create multiple children with distinct exit codes and collect each termination status using `waitpid()`.

The lab demonstrates explicit child lifecycle ownership and avoids leaving terminated children unreaped.
