#!/usr/bin/env bash

set -euo pipefail

LAB_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORKSPACE_DIR="$LAB_DIR/workspace"
REPO_DIR="$WORKSPACE_DIR/git-workflow-demo"

echo "========================================"
echo "GIT ENGINEERING WORKFLOW LAB SETUP"
echo "========================================"
echo

if [ -d "$REPO_DIR" ]; then
    echo "Removing previous generated practice repository..."
    rm -rf "$REPO_DIR"
fi

mkdir -p "$REPO_DIR"
cd "$REPO_DIR"

echo "[1/6] Initializing practice repository"

git init -b main

git config user.name "Systems Lab Engineer"
git config user.email "systems-lab@example.invalid"

mkdir -p src tests

cat > .gitignore <<'EOF'
# Generated build artifacts
counter
EOF

echo "[2/6] Creating source code"

cat > src/counter.c <<'EOF'
#include <stdio.h>
#include <stdlib.h>

static long sum_to(long limit)
{
    long total = 0;

    for (long i = 1; i <= limit; ++i) {
        total += i;
    }

    return total;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s LIMIT\n", argv[0]);
        return 2;
    }

    char *end = NULL;
    long limit = strtol(argv[1], &end, 10);

    if (*argv[1] == '\0' || *end != '\0' || limit < 0) {
        fprintf(stderr, "invalid limit\n");
        return 2;
    }

    printf("%ld\n", sum_to(limit));

    return 0;
}
EOF

echo "[3/6] Creating tests"

cat > tests/test_counter.sh <<'EOF'
#!/usr/bin/env bash

set -euo pipefail

PROGRAM="${1:-./counter}"

expect_output() {
    local input="$1"
    local expected="$2"
    local actual

    actual="$("$PROGRAM" "$input")"

    if [ "$actual" != "$expected" ]; then
        echo "FAIL: input=$input expected=$expected actual=$actual"
        exit 1
    fi
}

expect_output 0 0
expect_output 1 1
expect_output 5 15
expect_output 10 55
expect_output 100 5050

echo "PASS: counter tests"
EOF

chmod +x tests/test_counter.sh

echo "[4/6] Creating Makefile"

{
    printf '%s\n' 'CC ?= cc'
    printf '\n'
    printf '%s\n' 'CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O2'
    printf '\n'
    printf '%s\n' '.PHONY: all test clean'
    printf '\n'
    printf '%s\n' 'all: counter'
    printf '\n'
    printf '%s\n' 'counter: src/counter.c'
    printf '\t%s\n' '$(CC) $(CFLAGS) src/counter.c -o counter'
    printf '\n'
    printf '%s\n' 'test: counter'
    printf '\t%s\n' './tests/test_counter.sh ./counter'
    printf '\n'
    printf '%s\n' 'clean:'
    printf '\t%s\n' 'rm -f counter'
} > Makefile

echo "[5/6] Creating repository documentation"

cat > README.md <<'EOF'
# Counter Service

Small command-line program used by the Git Engineering Workflow lab.

## Build

Run:

    make

## Test

Run:

    make test

## Usage

Run:

    ./counter LIMIT

Example:

    ./counter 10

Expected output:

    55
EOF

echo "[6/6] Creating baseline commit"

git add .
git commit -m "Establish counter service baseline"

echo
echo "Running baseline tests..."
make test

echo
echo "Repository created:"
echo "$REPO_DIR"

echo
echo "Baseline commit:"
git log --oneline -1

echo
echo "Repository status:"
git status --short

echo
echo "========================================"
echo "SETUP COMPLETE"
echo "========================================"
