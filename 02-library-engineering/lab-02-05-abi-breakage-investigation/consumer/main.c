#include "record.h"

#include <inttypes.h>
#include <stdio.h>

int main(void)
{
    const struct record item = {
        .id = 7U,
        .value = 42U
    };
    const size_t compile_size = sizeof(struct record);
    const size_t runtime_size = record_abi_size();

    printf("consumer_struct_size=%zu\n", compile_size);
    printf("library_struct_size=%zu\n", runtime_size);

    if (compile_size != runtime_size) {
        fprintf(stderr, "ABI MISMATCH: public struct layout changed\n");
        return 2;
    }

    printf("score=%" PRIu64 "\n", record_score(&item));
    return 0;
}
