# Section Completion Checklist

For each lab:

- [ ] clean build succeeds;
- [ ] strict-warning compilation succeeds;
- [ ] automated tests pass;
- [ ] validation evidence is generated and reviewed;
- [ ] engineering report is updated with actual observations;
- [ ] generated build output remains untracked.

For the section:

- [ ] `./scripts/validate_all.sh` passes;
- [ ] static archive contents/symbols were inspected;
- [ ] shared-library SONAME and runtime dependencies were inspected;
- [ ] ABI-breakage behavior was reproduced and explained;
- [ ] `git diff --check` passes;
- [ ] section README progress is marked complete only after validation.
