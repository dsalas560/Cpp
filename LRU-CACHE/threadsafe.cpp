#include <iostream>
#include <thread>
#include <vector>
#include <string>

#include "lru_cache.h"
#include "lru_cache_threadsafe.h"

int main() {
    using Cache = LRUCache<int, std::string>;
    ThreadSafeCache<Cache> cache(5);

    auto writer = [&cache](int base) {
        for (int i = 0; i < 50; ++i) {
            cache.put(base + i, "v" + std::to_string(base + i));
        }
    };

    auto reader = [&cache](int key) {
        for (int i = 0; i < 100; ++i) {
            (void)cache.get(key);
        }
    };

    std::vector<std::thread> threads;
    threads.emplace_back(writer, 0);
    threads.emplace_back(writer, 1000);
    threads.emplace_back(reader, 10);
    threads.emplace_back(reader, 1010);

    for (auto& t : threads) t.join();

    std::cout << "Final size (<= capacity): " << cache.size()
              << " (capacity=" << cache.capacity() << ")\n";
    return 0;
}
