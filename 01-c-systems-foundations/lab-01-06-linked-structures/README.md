# Lab 01-06 — Linked Structures

## Objective

Implement a singly linked list with explicit ownership, structural invariants, search, insertion, removal, and cleanup.

## Engineering Focus

Unlike the dynamic array, the list stores elements in separately allocated nodes and must update links correctly during structural changes.

## Operations

- initialize;
- push front;
- push back;
- find;
- remove first matching value;
- compute size;
- destroy all nodes.

## Employer Signal

The lab demonstrates pointer-heavy mutation across heap-allocated nodes while preserving a simple ownership model and testing edge cases such as empty lists, head removal, and missing values.
