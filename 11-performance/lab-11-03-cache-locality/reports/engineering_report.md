# Cache Locality — Engineering Report

## Objective

Compare row-major and column-major traversal of the same contiguous matrix while verifying identical computational results.

## Runtime Observations

```text
row_major_seconds=0.000812163
column_major_seconds=0.011290243
row_sum=534773760
column_sum=534773760
sums_equal=yes
column_to_row_ratio=13.901
cache_locality_measurement_verified=yes
```

## Engineering Interpretation

Both traversal orders produced the same sum, so the comparison performed equivalent logical work.

On this development environment, column-major traversal took approximately 13.9 times as long as row-major traversal.

The observed difference is consistent with the row-major memory layout of the contiguous allocation: sequential access exhibits better spatial locality than repeatedly jumping by a full row stride.

The exact ratio is environment-specific and should not be generalized to other processors or memory hierarchies.

## Result

PASS — equivalent-work cache-locality behavior was measured and verified.
