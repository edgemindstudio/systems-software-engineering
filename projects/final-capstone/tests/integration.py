#!/usr/bin/env python3
import concurrent.futures
import os
import signal
import socket
import struct
import subprocess
import sys
import time

def read_exact(sock, count):
    out = bytearray()
    while len(out) < count:
        chunk = sock.recv(count - len(out))
        if not chunk:
            raise RuntimeError("unexpected EOF")
        out.extend(chunk)
    return bytes(out)

def request(sock_path, text):
    payload = text.encode()
    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as sock:
        sock.settimeout(5.0)
        sock.connect(sock_path)
        sock.sendall(struct.pack("!I", len(payload)) + payload)
        length = struct.unpack("!I", read_exact(sock, 4))[0]
        return read_exact(sock, length).decode()

def wait_socket(path):
    deadline = time.time() + 5.0
    while time.time() < deadline:
        if os.path.exists(path):
            return
        time.sleep(0.02)
    raise RuntimeError("socket did not appear")

def main():
    if len(sys.argv) != 3:
        print("usage: integration.py DAEMON CLIENT", file=sys.stderr)
        return 2

    daemon = os.path.abspath(sys.argv[1])
    client = os.path.abspath(sys.argv[2])

    os.makedirs("workspace", exist_ok=True)
    sock_path = os.path.abspath("workspace/sysdiag.sock")
    log_path = os.path.abspath("workspace/daemon.log")
    fixture = os.path.abspath("workspace/fixture")

    subprocess.check_call(
        ["clang", "-std=c17", "-g", "-O0",
         "tests/fixture.c", "-o", fixture]
    )

    with open("workspace/sample.txt", "w", encoding="utf-8") as f:
        f.write("capstone\n")
    sample = os.path.abspath("workspace/sample.txt")

    with open(log_path, "w", encoding="utf-8") as log:
        proc = subprocess.Popen(
            [daemon, "--socket", sock_path,
             "--workers", "4", "--queue", "8"],
            stdout=log,
            stderr=subprocess.STDOUT,
            text=True,
        )

        try:
            wait_socket(sock_path)

            out = request(sock_path, "PING")
            assert "OK command=PING" in out and "pong=yes" in out
            print("ping=PASS")

            out = request(sock_path, f"STAT {sample}")
            assert "OK command=STAT" in out
            assert "type=regular" in out
            assert "size=9" in out
            print("stat=PASS")

            out = request(sock_path, "PROC self")
            assert "OK command=PROC" in out
            assert "Name:" in out
            assert "State:" in out
            assert "Threads:" in out
            print("proc=PASS")

            out = request(sock_path, f"ELF {fixture}")
            assert "OK command=ELF" in out
            assert "class=ELF64" in out
            assert "endianness=little" in out
            print("elf=PASS")

            commands = []
            for i in range(60):
                commands.append("PING" if i % 2 == 0 else f"STAT {sample}")

            with concurrent.futures.ThreadPoolExecutor(max_workers=20) as pool:
                futures = [pool.submit(request, sock_path, cmd)
                           for cmd in commands]
                responses = [f.result(timeout=10.0) for f in futures]

            assert all(r.startswith("OK ") for r in responses)
            print("concurrent_clients=PASS")

            out = request(sock_path, "UNKNOWN")
            assert out.startswith("ERR ")
            print("unknown_command=PASS")

            # Verify CLI path too.
            cli = subprocess.check_output(
                [client, "--socket", sock_path, "PING"],
                text=True,
            )
            assert "pong=yes" in cli
            print("cli=PASS")

        finally:
            proc.send_signal(signal.SIGTERM)
            try:
                rc = proc.wait(timeout=10.0)
            except subprocess.TimeoutExpired:
                proc.kill()
                rc = proc.wait(timeout=5.0)

        assert rc == 0, f"daemon exit status {rc}"

    with open(log_path, "r", encoding="utf-8") as f:
        log_text = f.read()

    assert "shutdown=graceful" in log_text
    assert "accepted_requests=" in log_text
    assert "completed_requests=" in log_text
    print("graceful_shutdown=PASS")
    print("final_capstone_tests=PASS")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
