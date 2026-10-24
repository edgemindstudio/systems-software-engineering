# 08 — Networking

## Purpose

This section develops practical Linux socket-programming skills for systems software.

The labs focus on address resolution, TCP, UDP, application framing, nonblocking I/O, IPv6, and concurrent service design.

## Employer-Facing Outcomes

After completing this section, the repository demonstrates practical ability to:

- resolve host/service names with `getaddrinfo`;
- convert socket addresses to numeric text;
- create, bind, listen, accept, connect, send, and receive with TCP sockets;
- exchange UDP datagrams and inspect peer addressing;
- bind services to ephemeral loopback ports;
- frame application messages over TCP byte streams;
- handle exact-length stream reads and complete writes;
- use nonblocking sockets and `poll`;
- create IPv6 loopback services;
- build a concurrent threaded TCP service;
- coordinate service lifecycle and shutdown deterministically;
- preserve reproducible networking evidence.

## Labs

| Lab | Topic | Core Engineering Evidence |
|---|---|---|
| 08-01 | Address Resolution | `getaddrinfo`, numeric address conversion |
| 08-02 | TCP Loopback Echo | bind/listen/accept/connect/send/recv |
| 08-03 | UDP Datagram Exchange | `sendto`, `recvfrom`, peer addressing |
| 08-04 | Length-Prefixed TCP Framing | byte-stream framing and exact I/O |
| 08-05 | Nonblocking I/O with `poll` | `O_NONBLOCK`, `EAGAIN`, readiness waiting |
| 08-06 | IPv6 Loopback TCP | `AF_INET6`, `::1`, TCP exchange |
| 08-07 | Concurrent Threaded TCP Service | multiple clients and pthread workers |

## Completed Results

All seven labs passed on the development environment.

Key observed results included:

- `localhost` resolving successfully to IPv4 address `127.0.0.1` in the address-resolution lab;
- a TCP server binding an ephemeral loopback port and completing a client/server echo round trip;
- UDP delivery of `udp-message` with an IPv4 peer address;
- correct decoding of a 21-byte length-prefixed TCP message;
- a nonblocking receive returning `EAGAIN` before data arrival;
- `poll()` subsequently reporting the socket readable;
- explicit IPv6 loopback communication over `::1` succeeding;
- a threaded TCP service handling four client connections and acknowledging each successfully.

The address-resolution lab returned only IPv4 for `localhost` on this machine. The explicit IPv6 lab nevertheless passed, showing that IPv6 loopback networking itself is available even though the local name-resolution configuration did not return an IPv6 `localhost` entry in that experiment.

## Progress

- Lab 08-01 — Address Resolution: Complete
- Lab 08-02 — TCP Loopback Echo: Complete
- Lab 08-03 — UDP Datagram Exchange: Complete
- Lab 08-04 — Length-Prefixed TCP Framing: Complete
- Lab 08-05 — Nonblocking I/O with poll: Complete
- Lab 08-06 — IPv6 Loopback TCP: Complete
- Lab 08-07 — Concurrent Threaded TCP Service: Complete

Domain 08 — Networking: Complete
