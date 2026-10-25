# Systems Incident Investigations

## Purpose

This section demonstrates disciplined systems-engineering diagnosis through ten small, reproducible incidents.

Each investigation begins with an observable failure or misleading system state, captures evidence, identifies the root cause, applies a targeted remediation, and verifies the result.

## Incident Workflow

1. **Symptom** — identify the unexpected behavior.
2. **Reproduction** — trigger the issue under controlled conditions.
3. **Evidence** — collect diagnostic output before changing the system.
4. **Root Cause** — explain the mechanism behind the symptom.
5. **Remediation** — apply the narrowest appropriate correction.
6. **Verification** — prove the correction worked.
7. **Lesson** — record the reusable engineering principle.

## Completed Incidents

| Incident | Failure Mode | Verified Evidence |
|---|---|---|
| INC-01 | File descriptor leak | FD count 3 → 11 → 3 |
| INC-02 | Zombie child | child state `Z`, then reaped |
| INC-03 | Pipe EOF / hang condition | no EOF while writer remains open |
| INC-04 | Heap use-after-free | AddressSanitizer diagnostic |
| INC-05 | Data race | ThreadSanitizer diagnostic |
| INC-06 | Stream framing failure | `HELLO` + `WORLD` received as `HELLOWORLD` |
| INC-07 | Shared-library lookup failure | `not found` → resolved with search path |
| INC-08 | Execute permission failure | exit 126 → successful execution after chmod |
| INC-09 | Sparse-file space confusion | ~64 MiB logical vs 4 KiB allocated |
| INC-10 | Tiny-write regression | observed ~4280× tiny/batched timing ratio |

## Engineering Themes

The completed incidents demonstrate practical reasoning across:

- descriptor lifecycle;
- process reaping;
- pipe EOF semantics;
- heap safety;
- concurrency races;
- stream protocol framing;
- dynamic-loader search paths;
- Unix permissions;
- sparse-file accounting;
- syscall amortization.

The reports intentionally distinguish reproduced evidence from broader production claims. These are controlled systems investigations, not claims of real customer-impacting incidents.

## Status

All 10 incident investigations passed on the development environment.

Section status: Complete
