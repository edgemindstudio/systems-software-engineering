# INC-06 — Stream Framing Failure

## Symptom

Two logical messages written separately are not preserved as distinct message boundaries when received from a stream transport.

## Reproduction

The sender writes `HELLO` and `WORLD` as separate writes to a stream socket and the receiver consumes the byte stream.

## Evidence

```text
bytes_received=10
stream_payload=HELLOWORLD
message_boundaries_preserved=no
framing_required=yes
```

## Root Cause

Stream sockets preserve byte order, not application-message boundaries. The receiver sees a continuous byte sequence and cannot infer where one logical message ends unless the protocol provides framing.

## Remediation

Define explicit framing, such as a length prefix, delimiter with escaping rules, or fixed-size records.

## Verification

The receiver obtained the correct 10 bytes but no independent boundary between `HELLO` and `WORLD`.

## Engineering Lesson

TCP-style stream transports are not message queues. Protocol design must explicitly define how records are separated and reconstructed.
