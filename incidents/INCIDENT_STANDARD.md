# Incident Investigation Standard

Every incident report must include:

- incident identifier and title;
- user-visible or system-visible symptom;
- reproduction steps;
- diagnostic evidence;
- root cause;
- remediation;
- verification;
- engineering lesson.

## Evidence Rules

- Preserve relevant command output exactly.
- Do not invent production impact.
- Do not claim causality that the evidence does not establish.
- Keep generated binaries and temporary files out of version control.
- Prefer small reproducible demonstrations over vague narrative.
