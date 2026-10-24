# Git Engineering Workflow Report

## Overview

This lab used an isolated Git repository to practice common source-control workflows without affecting the history of the main Systems Software Engineering repository.

The exercises moved beyond basic Git commands and simulated normal engineering situations involving feature development, parallel work, merge conflicts, bad commits, release tagging, staging behavior, recovery, and regression investigation.

## Repository Isolation

The practice repository was generated under:

```text
workspace/git-workflow-demo/
```

The workspace is ignored by the parent repository so history-changing exercises can be performed safely.

A setup script creates the repository, source code, tests, Makefile, `.gitignore`, initial commit, and local Git identity.

This separation made it possible to perform merges, reverts, branch deletion, tagging, and bisect operations without risking the history of the real Systems Software Engineering repository.

## Baseline Repository

The generated repository contained:

```text
.gitignore
Makefile
README.md
src/counter.c
tests/test_counter.sh
```

The baseline program calculates the sum of all integers from `1` through a supplied limit.

For example:

```text
./counter 10
55
```

The initial test suite verified several known values.

The generated `counter` executable was excluded from Git through `.gitignore`, allowing builds to occur without leaving the repository in a modified or untracked state.

## Feature Branch Workflow

The first feature added optional labeled output to the counter utility.

The original behavior:

```text
./counter 10
55
```

was preserved.

The new behavior:

```text
./counter --label 10
sum=55
```

was implemented on:

```text
feature/output-label
```

The feature included changes to both the implementation and automated tests.

Before committing, the working-tree diff was inspected.

The feature was then staged, the staged diff was reviewed, and the work was committed as:

```text
Add optional labeled counter output
```

The feature branch was compared against `main` before integration.

It was merged using a merge commit so the branch history remained visible.

## Branch Review and Integration

Before the feature was merged, the following Git operations were used to inspect it:

```text
git diff
git diff --cached
git diff main..feature/output-label
git log
git show
```

This demonstrated the distinction between:

- uncommitted working-tree changes;
- staged changes;
- committed branch history;
- differences between branches.

The merged version was rebuilt and tested successfully.

## Parallel Development

Two additional branches were created independently from the same version of `main`.

The first branch:

```text
feature/help-text
```

improved command usage information.

The second branch:

```text
feature/short-label
```

introduced the shorter:

```text
-l
```

alias for:

```text
--label
```

Both branches modified the same argument-handling area of `src/counter.c`.

The help-text branch was merged first.

When the short-label branch was subsequently merged, Git was unable to automatically determine the correct combined version of the overlapping code.

## Merge Conflict Resolution

The second merge produced a real content conflict in:

```text
src/counter.c
```

Git inserted conflict markers similar to:

```text
<<<<<<< HEAD
current main content
=======
incoming branch content
>>>>>>> feature/short-label
```

The conflict was inspected using:

```text
git status
git diff
```

The conflict was not resolved by simply selecting one branch.

Instead, the intent of both changes was preserved.

The final interface supported:

```text
./counter 10
./counter --label 10
./counter -l 10
```

The combined usage information became:

```text
usage: ./counter [-l|--label] LIMIT
  -l, --label   print output as sum=VALUE
```

After the conflict markers were removed, the source was rebuilt and the full test suite passed.

The resolved file was staged and the merge was completed with a merge commit.

This demonstrated that conflict resolution often requires understanding the intended behavior of both changes rather than mechanically choosing one side.

## Bad Commit and Revert Investigation

A deliberate off-by-one regression was introduced into the summation loop.

The correct condition:

```c
for (long i = 1; i <= limit; ++i)
```

was changed to:

```c
for (long i = 1; i < limit; ++i)
```

This caused incorrect results.

For example:

```text
./counter 5
10

./counter 10
45
```

instead of:

```text
15
55
```

The automated test suite also failed.

The bad change was deliberately committed as:

```text
Optimize counter loop boundary
```

This simulated a defective change that had already entered shared repository history.

Rather than rewriting history, the change was reversed using:

```text
git revert
```

Git created a new commit that restored the correct implementation while preserving the original bad commit in history.

## Reverting a Revert

A second `git revert HEAD` was performed after the corrective revert.

Because `HEAD` now referred to the revert commit, reverting it reapplied the original defective change.

Git recorded this as a reapplication of the bad commit.

This demonstrated an important property of Git:

```text
revert bad commit
    → undo the bad change

revert the revert
    → reapply the bad change
```

The repository was then repaired again with another revert.

The final source returned to:

```c
for (long i = 1; i <= limit; ++i)
```

and all tests passed.

This exercise demonstrated that `git revert` creates new history rather than deleting or rewriting previous commits.

## Working Tree Restoration

A temporary uncommitted modification was added to `README.md`.

The change was inspected using:

```text
git status
git diff
```

The modification was then discarded using:

```text
git restore README.md
```

The file returned to its committed state and the working tree became clean.

This demonstrated that `git restore` can be used to discard unwanted working-tree changes.

## Staging and Unstaging

Another temporary modification was created and staged with:

```text
git add README.md
```

The staged state was inspected using:

```text
git diff --cached
```

The file was then removed from the staging area using:

```text
git restore --staged README.md
```

The modification remained in the working tree but was no longer staged.

This demonstrated the distinction between three Git states:

```text
HEAD
index / staging area
working tree
```

The remaining working-tree change was then discarded with `git restore`.

## Release Tagging

After the repository had returned to a stable state and the test suite passed, an annotated release tag was created:

```text
v1.0.0
```

The tag message was:

```text
Stable counter service baseline
```

The tag was inspected using:

```text
git tag
git show v1.0.0 --no-patch
```

The release tag identified the stable commit:

```text
8bb49a1
```

at the time the tag was created.

This demonstrated the distinction between a branch and a tag.

A branch normally moves forward as new commits are added.

An annotated tag provides a fixed name for a specific repository state.

## Regression Investigation with Git Bisect

A separate experimental branch was created after the stable `v1.0.0` release.

Several commits were added.

The sequence included normal documentation and ignore-rule changes, one deliberately defective source-code change, and additional innocent commits after the regression.

The deliberately defective commit changed:

```c
i <= limit
```

to:

```c
i < limit
```

The commit was recorded as:

```text
42221a9 Simplify counter iteration boundary
```

Additional commits were added after it so the bad commit was no longer simply the most recent change.

The current state failed the automated test suite.

## Bisect Setup

Git bisect was started with:

```text
git bisect start
```

The current broken state was marked as bad:

```text
git bisect bad HEAD
```

The previously tagged stable release was marked as good:

```text
git bisect good v1.0.0
```

This established a known-good and known-bad range of history.

## Automated Bisect

Instead of manually testing each selected commit, an automated test command was supplied through:

```text
git bisect run
```

The command rebuilt the program and executed the test suite for each commit selected by Git.

Git performed a binary search through the relevant history.

The investigation identified:

```text
42221a9af36d111b070967a0077f6bc36262b174
```

as the first bad commit.

The associated commit was:

```text
Simplify counter iteration boundary
```

The commit contained exactly the off-by-one change that caused the regression.

This demonstrated that Git history can be used directly as part of the debugging process.

With a reproducible automated test, `git bisect` can efficiently identify the first revision where a failure appears.

## Returning to Stable State

After the regression was identified, bisect mode was exited using:

```text
git bisect reset
```

The experimental branch was deleted.

The repository returned to:

```text
main
```

The project was rebuilt and tested again.

The final result was:

```text
PASS: counter tests
```

and:

```text
nothing to commit, working tree clean
```

## Evidence Produced

The lab preserved evidence including:

```text
evidence/git_history_before_bisect.txt
evidence/bisect_investigation.txt
evidence/final_git_workflow_state.txt
```

These files record repository history, branch state, tagging information, bisect activity, and the final validated repository state.

## Engineering Lessons

The lab demonstrated that Git is more than a source-code storage mechanism.

It supports engineering work through:

- isolated feature development;
- focused and reviewable commits;
- branch comparison;
- parallel development;
- controlled integration;
- merge-conflict detection;
- explicit conflict resolution;
- safe reversal of shared changes;
- working-tree recovery;
- staging and unstaging;
- release identification;
- repository-history inspection;
- regression investigation.

The merge-conflict exercise showed that successful resolution requires understanding the intended behavior of competing changes.

The revert exercise showed that shared history can be corrected without deleting or rewriting previous commits.

The staging exercise demonstrated that the working tree, index, and committed snapshot are separate states.

The release-tagging exercise demonstrated how stable revisions can be identified explicitly.

The bisect exercise demonstrated that meaningful commit history and automated tests can work together as a debugging system.

## Result

The Git Engineering Workflow lab was completed successfully.

The final practice repository returned to a clean and tested `main` branch.

The lab exercised feature development, branch integration, merge-conflict resolution, recovery from bad commits, staging behavior, release tagging, and automated regression investigation.

The deliberately introduced regression was successfully located using `git bisect`, and the stable repository state remained intact at completion.
