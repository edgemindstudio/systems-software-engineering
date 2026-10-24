# File Metadata and Links — Engineering Report

## Objective

Inspect regular-file, hard-link, and symbolic-link metadata using `stat`, `fstat`, and `lstat`.

## Runtime Observations

The original file and hard link shared:

```text
inode=435039
links=2
size=9
mode=0644
```

The symbolic link showed different behavior:

```text
stat:  regular file, inode=435039
lstat: symlink, inode=435040, links=1, size=12
fstat: regular file, inode=435039
```

The validation explicitly confirmed:

```text
hardlink_inode_shared=yes
symlink_detected=yes
```

## Engineering Interpretation

A hard link is another directory entry referring to the same inode, so the original name and hard-link name report the same inode and increased link count.

`stat()` follows a symbolic link to its target, while `lstat()` reports metadata for the link object itself.

Opening the symbolic-link pathname and then calling `fstat()` reports the target because the descriptor refers to the opened target object.

## Result

PASS — inode identity, hard-link semantics, and symbolic-link inspection behavior were verified.
