# 09 — C++ Systems

## Purpose

This section develops practical modern C++ techniques for systems software.

The labs focus on deterministic resource management, move-only ownership, smart pointers, system-error propagation, low-level containers, and thread-safe abstractions.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- apply RAII to operating-system resources;
- implement move-only resource-owning types;
- reason about ownership transfer and moved-from states;
- use `std::unique_ptr`, `std::shared_ptr`, and `std::weak_ptr` appropriately;
- wrap POSIX file descriptors safely in C++;
- propagate POSIX failures with `std::system_error`;
- preserve cleanup guarantees along exception paths;
- build a small contiguous container with explicit size/capacity invariants;
- enforce bounds checking;
- coordinate producer/consumer behavior with standard C++ synchronization primitives;
- preserve reproducible runtime evidence.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 09-01 | RAII File Descriptor | deterministic close and move-only ownership |
| 09-02 | Move Semantics | ownership transfer and moved-from state |
| 09-03 | Smart Pointer Ownership | unique/shared lifetime and weak observation |
| 09-04 | System Error Handling | POSIX wrapper, `std::system_error`, cleanup |
| 09-05 | Small Vector Container | growth, size/capacity invariants, bounds checks |
| 09-06 | Thread-Safe C++ Queue | mutex, condition variable, shutdown |

## Completed Results

All six labs passed on the development environment.

Key observed results included:

- a file descriptor remaining valid while owned and being closed automatically when the RAII object left scope;
- a moved-from buffer becoming empty while the destination retained all four values;
- deterministic construction/destruction of unique ownership;
- shared ownership counts transitioning from `1` to `2` and back to `1`;
- a `weak_ptr` remaining valid while an owner existed and expiring after the last shared owner left scope;
- successful opening of `/etc/hosts`;
- a nonexistent path raising `std::system_error` with error value `2` (`ENOENT`);
- a custom vector growing to capacity `16` while holding 10 elements;
- correct first/last element values and successful out-of-range exception handling;
- a thread-safe queue producing the exact expected sum `5050` and joining the consumer cleanly.

## Progress

- Lab 09-01 — RAII File Descriptor: Complete
- Lab 09-02 — Move Semantics: Complete
- Lab 09-03 — Smart Pointer Ownership: Complete
- Lab 09-04 — System Error Handling: Complete
- Lab 09-05 — Small Vector Container: Complete
- Lab 09-06 — Thread-Safe C++ Queue: Complete

Domain 09 — C++ Systems: Complete
