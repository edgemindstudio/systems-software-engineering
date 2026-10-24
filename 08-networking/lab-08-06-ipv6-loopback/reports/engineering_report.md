# IPv6 Loopback TCP — Engineering Report

## Objective

Create an IPv6 TCP server on the `::1` loopback address and verify client/server data exchange.

## Runtime Observations

```text
ipv6_received=ipv6-ok
ipv6_loopback_verified=yes
ipv6_tcp_verified=yes
```

## Engineering Interpretation

The server successfully created an `AF_INET6` socket, bound to IPv6 loopback, listened, and accepted an IPv6 client connection.

The received payload matched the sender's message.

This confirms that IPv6 loopback networking is functional on the development environment, even though the separate `localhost` resolution experiment returned only IPv4.

## Result

PASS — IPv6 TCP loopback communication was verified.
