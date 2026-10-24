# Bounded Producer/Consumer Queue — Engineering Report

## Objective

Implement a fixed-capacity producer/consumer queue using a mutex and separate `not_empty` and `not_full` condition variables.

## Runtime Observations

```text
expected_sum=5050
actual_sum=5050
queue_empty=yes
producer_consumer_correct=yes
bounded_queue_coordination_verified=yes
```

## Engineering Interpretation

The producer blocked when the queue reached capacity, and the consumer blocked when the queue was empty.

The exact sum of values 1 through 100 was preserved, and the queue ended empty.

This validates both queue correctness and synchronization around capacity constraints.

## Result

PASS — bounded producer/consumer coordination and queue integrity were verified.
