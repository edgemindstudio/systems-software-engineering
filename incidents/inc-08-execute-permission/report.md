# INC-08 — Permission-Denied Execution

## Symptom

A valid shell script cannot be executed directly.

## Reproduction

The script is copied to a workspace and assigned mode `600`, removing the execute permission.

## Evidence

```text
failure_exit_code=126
remediated_output=permission-demo-ran
permission_failure_reproduced=yes
execute_permission_remediation_verified=yes
```

## Root Cause

Direct execution requires an execute bit permitted by the file's ownership/mode rules. Readability alone does not make a file directly executable.

## Remediation

Grant the required execute permission with `chmod` while keeping permissions no broader than necessary.

## Verification

After changing the workspace copy to mode `700`, direct execution succeeded and printed the expected output.

## Engineering Lesson

Unix permissions express distinct capabilities. Read, write, and execute access must be diagnosed separately rather than treating “file access” as one property.
