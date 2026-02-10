# LRU Cache (C++17)

A lightweight C++ project that implements a **Least Recently Used (LRU) cache** with:
- **O(1)** average-time `get` and `put`
- **Time-to-Live (TTL)** expiration
- **Thread-safe access (optional wrapper)**
- An **interactive CLI demo** to observe cache behavior in real time

This project is designed to demonstrate clean separation between **core data structures**, **cache policies**, and **user interaction**.

---

## Features

### Core LRU Cache
- Fixed-capacity cache
- Evicts the **least recently used** item when full
- Implemented using:
  - `std::list` for recency ordering
  - `std::unordered_map` for O(1) lookup

### TTL (Time-to-Live) Extension
- Entries may expire after a configurable duration
- Expiration is **lazy**:
  - Items are removed when accessed (`get` / `put`)
- Supports:
  - Default TTL for all entries
  - Per-entry TTL override
  - Entries that never expire

### Thread Safety
- Provided via a generic `ThreadSafeCache<T>` wrapper
- Uses a single `std::mutex` to guard all operations
- Does not modify cache logic or eviction behavior

---

---

## Interactive TTL Demo

The main interactive program is `ttl.cpp`.





