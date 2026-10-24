# Lab 01-03 — Stack Investigation

## Objective

Observe automatic storage, nested calls, recursion, and stack-address movement without treating a platform-specific direction as a language guarantee.

## Engineering Focus

The program records addresses of local objects across recursive calls and compares them as opaque integer representations. It demonstrates that each active call has distinct automatic storage.

The lab intentionally avoids returning pointers to expired local variables.

## Optional Debugger Work

After building:

```bash
gdb ./build/lab
```

Useful commands include:

```text
break recursive_probe
run
backtrace
info frame
info locals
```

## Employer Signal

The lab connects C lifetime rules to debugger-visible call frames and avoids common stack-lifetime mistakes.
