# Section Completion Checklist

Do not mark Domain 01 complete merely because the files exist.

For each lab:

- [ ] clean build succeeds;
- [ ] strict-warning build succeeds without warnings;
- [ ] program behavior is inspected;
- [ ] automated tests pass;
- [ ] AddressSanitizer/UndefinedBehaviorSanitizer validation passes;
- [ ] `evidence/validation.txt` is reviewed;
- [ ] engineering report observations/result are updated;
- [ ] lab README accurately reflects completed work.

For the section:

- [ ] `./scripts/validate_all.sh` passes;
- [ ] `git diff --check` passes;
- [ ] generated build directories remain untracked;
- [ ] section README progress is updated;
- [ ] one coherent completion commit is reviewed before push.
