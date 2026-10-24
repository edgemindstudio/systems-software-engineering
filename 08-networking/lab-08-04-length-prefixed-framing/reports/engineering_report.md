# Length-Prefixed TCP Framing — Engineering Report

## Objective

Implement application-level message framing over TCP using a 32-bit network-order length prefix and exact-length I/O helpers.

## Runtime Observations

```text
decoded_length=21
decoded_message=framed-message-works!
framing_verified=yes
length_prefix_protocol_verified=yes
```

## Engineering Interpretation

TCP provides a byte stream rather than preserving application message boundaries.

The receiver first read an exact four-byte length prefix, converted it from network byte order, then read exactly the declared payload length.

The decoded length and message matched the sender's intended frame, demonstrating a simple but robust application framing strategy.

## Result

PASS — length-prefixed TCP framing and exact stream I/O were verified.
