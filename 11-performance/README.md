# 11 — Performance

## Purpose

This section develops disciplined performance-measurement skills for Linux systems software.

The labs focus on reliable timing, repeated measurements, cache locality, syscall amortization, allocation cost, compiler optimization effects, and careful interpretation of environment-sensitive results.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- measure elapsed time with `clock_gettime(CLOCK_MONOTONIC)`;
- validate that timing intervals are positive and structurally sound;
- summarize repeated trials with minimum, mean, and maximum values;
- compare equivalent memory-access workloads with different locality characteristics;
- quantify syscall overhead through different I/O granularities;
- compare repeated heap allocation with reusable storage;
- compare `-O0` and `-O2` builds while verifying identical computational results;
- preserve enough work/output to prevent trivial benchmark elimination;
- distinguish reproducible methodology from machine-specific absolute timing;
- document benchmark results without overstating portability.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 11-01 | Monotonic Timing | positive elapsed interval from `CLOCK_MONOTONIC` |
| 11-02 | Repeated Benchmark Statistics | seven trials with min/mean/max ordering |
| 11-03 | Cache Locality | row-major vs column-major traversal |
| 11-04 | Syscall Amortization | 1-byte writes vs 4 KiB writes |
| 11-05 | Allocation Reuse | repeated `malloc/free` vs reusable buffer |
| 11-06 | Optimization Effects | `-O0` vs `-O2` with identical result |

## Completed Results

All six labs passed on the development environment.

Observed results from the final validation run included:

- a 20,000,000-iteration CPU workload measured at `0.030277435` seconds with a positive monotonic interval;
- seven repeated benchmark trials ranging from `0.004080279` to `0.004143483` seconds, with a mean of `0.004098133` seconds;
- row-major traversal at `0.000812163` seconds and column-major traversal at `0.011290243` seconds, with identical sums and an observed column/row ratio of `13.901`;
- writing 32 MiB one byte at a time in `2.704782264` seconds versus 4 KiB at a time in `0.000635290` seconds, producing an observed ratio of `4257.555`;
- 500,000 repeated allocation/free cycles in `0.002281429` seconds versus reusable-buffer access in `0.000106719` seconds, producing an observed ratio of `21.378`;
- identical results from `-O0` and `-O2` builds, with elapsed times of `0.039192705` and `0.025058530` seconds respectively, for an observed `-O0`/`-O2` ratio of `1.564`.

## Measurement Interpretation

The ratios above are observations from one controlled run on the development environment. They are not universal constants.

Processor model, virtualization, scheduler activity, compiler version, memory hierarchy, thermal state, and background load can all change absolute timings and ratios.

The engineering value of these labs is therefore the methodology:

- equivalent work is compared;
- output invariants are verified;
- measurements use a monotonic clock;
- repeated trials are summarized where appropriate;
- results are interpreted as environment-specific evidence rather than universal performance claims.

## Progress

- Lab 11-01 — Monotonic Timing: Complete
- Lab 11-02 — Repeated Benchmark Statistics: Complete
- Lab 11-03 — Cache Locality: Complete
- Lab 11-04 — Syscall Amortization: Complete
- Lab 11-05 — Allocation Reuse: Complete
- Lab 11-06 — Optimization Effects: Complete

Domain 11 — Performance: Complete
