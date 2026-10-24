#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static double seconds_between(struct timespec start, struct timespec end)
{
    const time_t sec = end.tv_sec - start.tv_sec;
    const long nsec = end.tv_nsec - start.tv_nsec;
    return (double)sec + (double)nsec / 1000000000.0;
}


int main(void)
{
    const size_t rows = 2048U;
    const size_t cols = 2048U;
    const size_t count = rows * cols;
    uint32_t *matrix = malloc(count * sizeof *matrix);
    if (matrix == NULL) return 1;

    for (size_t i = 0U; i < count; ++i) {
        matrix[i] = (uint32_t)(i & 255U);
    }

    struct timespec a0, a1, b0, b1;
    uint64_t row_sum = 0U;
    uint64_t col_sum = 0U;

    if (clock_gettime(CLOCK_MONOTONIC, &a0) != 0) return 1;
    for (size_t r = 0U; r < rows; ++r) {
        for (size_t c = 0U; c < cols; ++c) {
            row_sum += matrix[r * cols + c];
        }
    }
    if (clock_gettime(CLOCK_MONOTONIC, &a1) != 0) return 1;

    if (clock_gettime(CLOCK_MONOTONIC, &b0) != 0) return 1;
    for (size_t c = 0U; c < cols; ++c) {
        for (size_t r = 0U; r < rows; ++r) {
            col_sum += matrix[r * cols + c];
        }
    }
    if (clock_gettime(CLOCK_MONOTONIC, &b1) != 0) return 1;

    const double row_seconds = seconds_between(a0, a1);
    const double col_seconds = seconds_between(b0, b1);
    printf("row_major_seconds=%.9f\n", row_seconds);
    printf("column_major_seconds=%.9f\n", col_seconds);
    printf("row_sum=%" PRIu64 "\n", row_sum);
    printf("column_sum=%" PRIu64 "\n", col_sum);
    printf("sums_equal=%s\n", row_sum == col_sum ? "yes" : "no");
    printf("column_to_row_ratio=%.3f\n",
           row_seconds > 0.0 ? col_seconds / row_seconds : 0.0);

    free(matrix);
    return row_sum == col_sum ? 0 : 1;
}
