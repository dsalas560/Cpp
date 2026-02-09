#pragma once

#include <mutex>
#include <utility>

template <typename Cache>
class ThreadSafeCache {
public:
    template <typename... Args>
    explicit ThreadSafeCache(Args&&... args)
        : cache_(std::forward<Args>(args)...) {}

    template <typename Key>
    auto get(const Key& key) {
        std::lock_guard<std::mutex> lock(m_);
        return cache_.get(key);
    }

    template <typename Key, typename Value>
    void put(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(m_);
        cache_.put(key, value);
    }

    template <typename Key, typename Value, typename TTL>
    void put(const Key& key, const Value& value, TTL ttl) {
        std::lock_guard<std::mutex> lock(m_);
        cache_.put(key, value, ttl);
    }

    template <typename Key>
    bool erase(const Key& key) {
        std::lock_guard<std::mutex> lock(m_);
        return cache_.erase(key);
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_);
        cache_.clear();
    }

    auto size() const {
        std::lock_guard<std::mutex> lock(m_);
        return cache_.size();
    }

    auto capacity() const {
        std::lock_guard<std::mutex> lock(m_);
        return cache_.capacity();
    }

private:
    mutable std::mutex m_;
    Cache cache_;
};
