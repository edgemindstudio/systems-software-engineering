# Directory Traversal — Engineering Report

## Objective

Traverse a directory with POSIX directory APIs and classify encountered entries.

## Runtime Observations

The controlled directory contained two regular files, one symbolic link, and one subdirectory.

The program reported:

```text
file-a.txt  regular
file-b.txt  regular
link-a      symlink
subdir      directory
```

The validation confirmed:

```text
entry_classification_verified=yes
```

## Engineering Interpretation

`opendir()` and `readdir()` enumerate directory entries, while `lstat()` provides reliable type classification without following symbolic links.

The implementation skips `.` and `..`, constructs paths with bounds checking, and handles per-entry metadata errors without corrupting traversal state.

## Result

PASS — deterministic directory traversal and entry classification worked correctly.
