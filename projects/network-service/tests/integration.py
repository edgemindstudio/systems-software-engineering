#!/usr/bin/env python3
import concurrent.futures
import os
import signal
import socket
import struct
import subprocess
import sys
import tempfile
import time

MAX_FRAME = 4096

def read_exact(sock, n):
    chunks = []
    remaining = n
    while remaining:
        data = sock.recv(remaining)
        if not data:
            raise RuntimeError("unexpected EOF")
        chunks.append(data)
        remaining -= len(data)
    return b"".join(chunks)

def exchange(port, payload, fragmented=False):
    with socket.create_connection(("127.0.0.1", port), timeout=5.0) as sock:
        frame = struct.pack("!I", len(payload)) + payload
        if fragmented:
            for byte in frame:
                sock.sendall(bytes([byte]))
        else:
            sock.sendall(frame)
        length = struct.unpack("!I", read_exact(sock, 4))[0]
        data = read_exact(sock, length)
        return data

def wait_port(path):
    deadline = time.time() + 5.0
    while time.time() < deadline:
        if os.path.exists(path) and os.path.getsize(path) > 0:
            with open(path, "r", encoding="utf-8") as f:
                return int(f.read().strip())
        time.sleep(0.02)
    raise RuntimeError("server did not publish port")

def main():
    if len(sys.argv) != 2:
        print("usage: integration.py SERVER", file=sys.stderr)
        return 2

    server = sys.argv[1]
    os.makedirs("workspace", exist_ok=True)
    port_file = os.path.abspath("workspace/port.txt")
    log_file = os.path.abspath("workspace/server.log")

    with open(log_file, "w", encoding="utf-8") as log:
        proc = subprocess.Popen(
            [server, "--port", "0", "--workers", "4",
             "--queue", "8", "--port-file", port_file],
            stdout=log,
            stderr=subprocess.STDOUT,
            text=True,
        )

        try:
            port = wait_port(port_file)

            assert exchange(port, b"hello") == b"hello"
            print("basic_roundtrip=PASS")

            fragmented = b"fragmented-stream-message"
            assert exchange(port, fragmented, fragmented=True) == fragmented
            print("fragmented_request=PASS")

            multi = []
            for i in range(40):
                multi.append(f"client-{i:02d}".encode())

            with concurrent.futures.ThreadPoolExecutor(max_workers=16) as pool:
                futures = [pool.submit(exchange, port, payload) for payload in multi]
                results = [f.result(timeout=10.0) for f in futures]

            assert results == multi
            print("concurrent_clients=PASS")

            large = bytes((i % 251 for i in range(MAX_FRAME)))
            assert exchange(port, large) == large
            print("max_frame=PASS")

            with socket.create_connection(("127.0.0.1", port), timeout=5.0) as sock:
                sock.sendall(struct.pack("!I", MAX_FRAME + 1))
                sock.shutdown(socket.SHUT_WR)
                data = sock.recv(1)
                assert data == b""
            print("oversized_frame_rejected=PASS")

        finally:
            proc.send_signal(signal.SIGTERM)
            try:
                rc = proc.wait(timeout=10.0)
            except subprocess.TimeoutExpired:
                proc.kill()
                rc = proc.wait(timeout=5.0)

        assert rc == 0, f"server exit status {rc}"

    with open(log_file, "r", encoding="utf-8") as f:
        log_text = f.read()

    assert "shutdown=graceful" in log_text
    assert "protocol_errors=1" in log_text
    print("graceful_shutdown=PASS")
    print("network_service_tests=PASS")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
