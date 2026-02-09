#pragma once

#include <list>
#include <unordered_map>
#include <optional>
#include <stdexcept>
#include <cstddef>

/*
  LRUCache<Key, Value>
  - O(1) average get/put
  - Most-recent at front of list
  - Least-recent at back of list
*/
template <typename Key, typename Value>
class LRUCache {
public:
    explicit LRUCache(std::size_t capacity)
        : capacity_(capacity) {
        if (capacity_ == 0) {
            throw std::invalid_argument("LRUCache capacity must be > 0");
        }
    }

    std::size_t size() const noexcept { return items_.size(); }
    std::size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return items_.empty(); }

    bool contains(const Key& key) const {
        return map_.find(key) != map_.end();
    }

    // Returns value if found; otherwise std::nullopt.
    std::optional<Value> get(const Key& key) {
        auto it = map_.find(key);
        if (it == map_.end()) return std::nullopt;

        touch_(it->second);
        return it->second->second;
    }

    // Insert new or update existing.
    void put(const Key& key, const Value& value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second->second = value;
            touch_(it->second);
            return;
        }

        if (items_.size() >= capacity_) {
            evictOne_();
        }

        items_.emplace_front(key, value);
        map_[key] = items_.begin();
    }

    // Removes an entry if present. Returns true if removed.
    bool erase(const Key& key) {
        auto it = map_.find(key);
        if (it == map_.end()) return false;
        items_.erase(it->second);
        map_.erase(it);
        return true;
    }

    void clear() {
        items_.clear();
        map_.clear();
    }

protected:
    using ListIt = typename std::list<std::pair<Key, Value>>::iterator;

    ListIt begin_() noexcept { return items_.begin(); }
    ListIt end_() noexcept { return items_.end(); }

    ListIt findIt_(const Key& key) {
        auto it = map_.find(key);
        return (it == map_.end()) ? items_.end() : it->second;
    }

    void touch_(ListIt nodeIt) {
        items_.splice(items_.begin(), items_, nodeIt);
    }

    void insertMostRecent_(const Key& key, const Value& value) {
        items_.emplace_front(key, value);
        map_[key] = items_.begin();
    }

    void eraseIt_(ListIt nodeIt) {
        map_.erase(nodeIt->first);
        items_.erase(nodeIt);
    }

    void evictOne_() {
        const Key& lruKey = items_.back().first;
        map_.erase(lruKey);
        items_.pop_back();
    }

private:
    std::size_t capacity_;
    std::list<std::pair<Key, Value>> items_;
    std::unordered_map<Key, ListIt> map_;
};
