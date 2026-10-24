# UndefinedBehaviorSanitizer — Engineering Report

## Objective

Trigger signed integer overflow and verify that UndefinedBehaviorSanitizer reports the undefined operation.

## Runtime Observations

```text
src/main.c:7:33: runtime error: signed integer overflow:
2147483647 + 1 cannot be represented in type 'int'
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior src/main.c:7:33
ubsan_return_code=1
ubsan_signed_overflow_verified=yes
```

## Engineering Interpretation

The expression attempted to increment `INT_MAX`, which cannot be represented as a signed `int`.

UBSan identified the exact source location and operation before normal execution could silently rely on undefined language behavior.

The nonzero process result was expected because recovery was disabled for the experiment.

## Result

PASS — signed integer overflow detection with UndefinedBehaviorSanitizer was verified.
