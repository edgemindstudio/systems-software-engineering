# Evidence — Valgrind Heap Diagnostics

Run:

```bash
./scripts/validate.sh
```

The script writes diagnostic output to `evidence/validation.txt`.

This domain intentionally includes crashing or sanitizer-detected programs. The validation harness interprets the expected diagnostic outcome and records `PASS` only when the intended defect is actually observed.
