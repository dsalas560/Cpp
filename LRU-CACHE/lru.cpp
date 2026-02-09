#include <iostream>
#include <string>
#include "lru_cache.h"

int main() {
    LRUCache<int, std::string> cache(3);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");

    if (auto v = cache.get(2)) {
        std::cout << "get(2) = " << *v << "\n";
    }

    // Evicts key=1 (least recently used)
    cache.put(4, "four");

    std::cout << "contains(1)? " << (cache.contains(1) ? "yes" : "no") << "\n";
    std::cout << "contains(2)? " << (cache.contains(2) ? "yes" : "no") << "\n";
    std::cout << "size = " << cache.size() << "\n";
    return 0;
}
