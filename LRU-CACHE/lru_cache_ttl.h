#pragma once

#include "lru_cache.h"

#include <chrono>
#include <optional>
#include <cstdint>

/*
  LRUCacheTTL<Key, Value>
  - LRU eviction + TTL expiration
  - Expired entries removed lazily on access (get/put/prune)
  - put(key,value) uses default TTL (if default_ttl_ms > 0), otherwise no-expiry
  - put(key,value,ttl_ms) uses per-entry TTL; ttl_ms == 0 => no-expiry
*/

template <typename Value>
struct TTLItem {
    Value value{};
    std::chrono::steady_clock::time_point expiresAt{};
    bool hasExpiry{false};
};

template <typename Key, typename Value>
class LRUCacheTTL : private LRUCache<Key, TTLItem<Value>> {
    using Clock = std::chrono::steady_clock;
    using Item  = TTLItem<Value>;
    using Base  = LRUCache<Key, Item>;
    using It    = typename Base::ListIt;

public:
    // default_ttl_ms == 0 => put(key,value) has no expiration
    explicit LRUCacheTTL(std::size_t capacity, std::uint64_t default_ttl_ms = 0)
        : Base(capacity), defaultTtlMs_(default_ttl_ms) {}

    std::size_t size() const noexcept { return Base::size(); }
    std::size_t capacity() const noexcept { return Base::capacity(); }

    std::optional<Value> get(const Key& key) {
        It it = Base::findIt_(key);
        if (it == Base::end_()) return std::nullopt;

        if (isExpired_(it->second)) {
            Base::eraseIt_(it);
            return std::nullopt;
        }

        Base::touch_(it);
        return it->second.value;
    }

    void put(const Key& key, const Value& value) {
        if (defaultTtlMs_ == 0) {
            putNoExpiry_(key, value);
        } else {
            put(key, value, defaultTtlMs_);
        }
    }

    void put(const Key& key, const Value& value, std::uint64_t ttl_ms) {
        if (ttl_ms == 0) {
            putNoExpiry_(key, value);
            return;
        }

        Item item;
        item.value = value;
        item.hasExpiry = true;
        item.expiresAt = Clock::now() + std::chrono::milliseconds(ttl_ms);

        It it = Base::findIt_(key);
        if (it != Base::end_()) {
            it->second = item;
            Base::touch_(it);
            return;
        }

        pruneOneExpiredFromBack_();

        if (Base::size() >= Base::capacity()) {
            Base::evictOne_();
        }

        Base::insertMostRecent_(key, item);
    }

    bool erase(const Key& key) { return Base::erase(key); }
    void clear() { Base::clear(); }

    // Optional manual cleanup
    void pruneExpired() {
        for (It it = Base::begin_(); it != Base::end_(); ) {
            if (isExpired_(it->second)) {
                It toErase = it++;
                Base::eraseIt_(toErase);
            } else {
                ++it;
            }
        }
    }

private:
    static bool isExpired_(const Item& item) {
        return item.hasExpiry && (Clock::now() >= item.expiresAt);
    }

    void putNoExpiry_(const Key& key, const Value& value) {
        Item item;
        item.value = value;
        item.hasExpiry = false;

        It it = Base::findIt_(key);
        if (it != Base::end_()) {
            it->second = item;
            Base::touch_(it);
            return;
        }

        pruneOneExpiredFromBack_();

        if (Base::size() >= Base::capacity()) {
            Base::evictOne_();
        }

        Base::insertMostRecent_(key, item);
    }

    void pruneOneExpiredFromBack_() {
        // Cheap lazy cleanup: check up to 2 entries from the LRU end
        int checks = 2;
        while (checks-- > 0 && Base::size() > 0) {
            It it = Base::end_();
            --it; // back
            if (isExpired_(it->second)) {
                Base::eraseIt_(it);
            } else {
                break;
            }
        }
    }

    std::uint64_t defaultTtlMs_;
};
