#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
labs=(
 "lab-08-01-address-resolution"
 "lab-08-02-tcp-loopback-echo"
 "lab-08-03-udp-datagram"
 "lab-08-04-length-prefixed-framing"
 "lab-08-05-nonblocking-poll"
 "lab-08-06-ipv6-loopback"
 "lab-08-07-threaded-tcp-service"
)
printf '========================================\n'
printf '08 — NETWORKING\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for lab in "${labs[@]}"; do
 printf '\n>>> %s\n' "$lab"
 (cd "$ROOT_DIR/$lab" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL NETWORKING LABS PASSED\n'
printf '========================================\n'
