# Smart Pointer Ownership — Engineering Report

## Objective

Compare exclusive ownership with `std::unique_ptr` and shared lifetime management with `std::shared_ptr` and `std::weak_ptr`.

## Runtime Observations

```text
resource_constructed=unique
unique_name=unique
resource_destroyed=unique
resource_constructed=shared
shared_use_count_initial=1
shared_use_count_nested=2
shared_use_count_after_nested=1
weak_expired_inside=no
resource_destroyed=shared
weak_expired_after_owner_scope=yes
smart_pointer_lifetime_verified=yes
```

## Engineering Interpretation

The uniquely owned resource was destroyed immediately when its owner left scope.

The shared resource's reference count increased to two when a second owner was created and returned to one after that owner left scope.

The weak observer remained valid while a shared owner existed and expired after the final shared owner was destroyed.

This demonstrates explicit ownership semantics and lifetime observation without extending object lifetime.

## Result

PASS — unique, shared, and weak smart-pointer lifetime behavior was verified.
