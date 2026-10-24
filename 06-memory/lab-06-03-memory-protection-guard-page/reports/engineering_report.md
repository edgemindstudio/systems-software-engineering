# Memory Protection and Guard Pages — Engineering Report

## Objective

Protect one page with `PROT_NONE` and verify that an access violation produces the expected fault in a child process.

## Runtime Observations

```text
first_page_value=90
guard_child_signaled=yes
guard_signal=11
expected_sigsegv=yes
guard_page_fault_verified=yes
```

## Engineering Interpretation

The first page remained readable and retained its value.

The protected guard page rejected access and terminated the child with signal 11, corresponding to `SIGSEGV`.

Running the expected fault in a child process kept the validation harness itself alive while still proving that page protections were enforced by the kernel.

## Result

PASS — guard-page protection and expected segmentation fault behavior were verified.
