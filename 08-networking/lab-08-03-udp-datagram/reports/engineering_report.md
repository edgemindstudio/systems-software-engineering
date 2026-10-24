# UDP Datagram Exchange — Engineering Report

## Objective

Exchange a UDP datagram over IPv4 loopback and inspect the sender address returned by `recvfrom()`.

## Runtime Observations

```text
receiver_port=35710
received=udp-message
peer_family_ipv4=yes
udp_exchange_verified=yes
udp_datagram_verified=yes
```

## Engineering Interpretation

The receiver bound to an ephemeral IPv4 loopback port, and the sender transmitted a single datagram with `sendto()`.

`recvfrom()` returned the complete message together with an IPv4 peer address.

The result demonstrates connectionless message delivery and peer-address reporting for UDP sockets.

## Result

PASS — UDP loopback delivery and peer-address handling were verified.
