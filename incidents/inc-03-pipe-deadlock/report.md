# INC-03 — Pipe EOF / Hang Condition

## Symptom

A reader waits for end-of-file even though the expected payload has already been consumed.

## Reproduction

The program writes one byte to a pipe, reads it, leaves the write end open, and attempts another nonblocking read before finally closing the writer.

## Evidence

```text
writer_still_open_no_eof=yes
eof_after_writer_close=yes
```

## Root Cause

A pipe reader receives EOF only after every file descriptor referring to the write end has been closed. An accidentally retained writer can therefore keep a reader waiting indefinitely.

## Remediation

Close unused pipe endpoints immediately in every process after `fork()` or after ownership is otherwise established.

## Verification

Before closing the writer, the read produced no EOF condition. After the write end was closed, the next read returned EOF.

## Engineering Lesson

Pipe correctness depends on descriptor ownership, not only on data flow. A logically unused write descriptor can still change synchronization behavior.
