# Lab 00-03 — Git Engineering Workflow

## Objective

Develop practical Git skills used when working on an existing engineering codebase, including branching, merging, conflict resolution, recovery, release tagging, and regression investigation.

## Engineering Context

Systems software engineers rarely work alone on a single uninterrupted version of a codebase.

Normal engineering work involves:

- creating branches from stable code;
- implementing bounded changes;
- reviewing diffs;
- integrating concurrent work;
- resolving conflicts;
- correcting bad commits;
- investigating history;
- recovering from mistakes;
- identifying regressions;
- marking releases.

Git is therefore more than a place to store source code. It is part of the engineering workflow and an important debugging tool.

This lab uses an isolated practice repository so that destructive or history-changing Git operations can be explored without affecting the main Systems Software Engineering repository.

## Prerequisites

- Lab 00-01 — Linux Engineering Environment
- Lab 00-02 — Compilation Pipeline Investigation
- Basic command-line navigation
- Basic familiarity with `git status`, `git add`, and `git commit`

## Tools

- Git
- Bash
- GCC or Clang
- standard Unix command-line utilities

## Skills Practiced

- repository inspection
- commit creation
- branch creation
- branch comparison
- diff inspection
- merging
- merge-conflict resolution
- history inspection
- reverting changes
- restoring files
- understanding reset behavior
- release tagging
- regression investigation
- `git bisect`
- engineering commit discipline

## Lab Environment

The exercises operate inside a generated repository located at:

```text
workspace/git-workflow-demo/
```

The `workspace/` directory is intentionally excluded from the parent repository.

Only reports, scripts, and selected evidence are preserved as part of the main lab.

## Workflow

The lab progresses through:

```text
stable repository
      ↓
feature development
      ↓
parallel changes
      ↓
merge conflict
      ↓
conflict resolution
      ↓
bad change
      ↓
revert and recovery
      ↓
release tag
      ↓
regression introduced
      ↓
history investigation
      ↓
git bisect
      ↓
root-cause commit identified
```

## Deliverables

- repository setup script
- workflow evidence
- conflict-resolution evidence
- regression investigation evidence
- Git workflow report
- this README

## Completion Criteria

This lab is complete when the learner can:

1. Create and inspect branches.
2. Make focused commits.
3. Compare branches and commits.
4. Merge independent work.
5. Resolve a real merge conflict.
6. Explain the difference between restoring, reverting, and resetting.
7. Undo a committed change safely with `git revert`.
8. Create and inspect an annotated release tag.
9. Navigate repository history.
10. Use `git bisect` to identify the commit that introduced a regression.
11. Explain why Git is useful for investigation as well as source control.
## Completed Engineering Scenarios

The completed lab exercised:

- isolated repository setup;
- feature branching;
- focused commits;
- branch comparison;
- merge commits;
- parallel development;
- merge-conflict resolution;
- regression introduction and recovery;
- `git revert`;
- working-tree restoration;
- staging and unstaging;
- annotated release tagging;
- automated regression investigation with `git bisect`.

## Result

Lab 00-03 is complete.

The exercises demonstrated Git as both a collaborative source-control system and an engineering investigation tool.
