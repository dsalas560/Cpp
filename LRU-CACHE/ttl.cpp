#include <iostream>
#include <string>
#include <optional>
#include <thread>
#include <chrono>
#include <cstdint>

#include "lru_cache_ttl.h"

// -------- input helpers (safe, no crashes) --------

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

static int readIntInRange(const std::string& prompt, int minV, int maxV) {
    while (true) {
        std::string s = readLine(prompt);

        try {
            std::size_t idx = 0;
            int v = std::stoi(s, &idx);

            // ensure the whole string was a number (ignoring trailing spaces)
            while (idx < s.size() && std::isspace(static_cast<unsigned char>(s[idx]))) idx++;
            if (idx != s.size()) throw std::invalid_argument("trailing chars");

            if (v < minV || v > maxV) {
                std::cout << "Please enter a number from " << minV << " to " << maxV << ".\n";
                continue;
            }
            return v;
        } catch (...) {
            std::cout << "Invalid input. Please enter a number from " << minV << " to " << maxV << ".\n";
        }
    }
}

static std::uint64_t readUInt64InRange(const std::string& prompt,
                                      std::uint64_t minV,
                                      std::uint64_t maxV) {
    while (true) {
        std::string s = readLine(prompt);

        try {
            std::size_t idx = 0;
            unsigned long long v = std::stoull(s, &idx);

            while (idx < s.size() && std::isspace(static_cast<unsigned char>(s[idx]))) idx++;
            if (idx != s.size()) throw std::invalid_argument("trailing chars");

            if (v < minV || v > maxV) {
                std::cout << "Please enter a number from " << minV << " to " << maxV << ".\n";
                continue;
            }
            return static_cast<std::uint64_t>(v);
        } catch (...) {
            std::cout << "Invalid input. Please enter a number from " << minV << " to " << maxV << ".\n";
        }
    }
}

// -------- demo app --------

static void printMenu() {
    std::cout << "\n=== LRU Cache + TTL (Interactive) ===\n"
              << "1) Get value\n"
              << "2) Put value\n"
              << "3) Wait (sleep N seconds)\n"
              << "4) Exit\n";
}

int main() {
    // Demo settings (easy to reason about)
    constexpr std::size_t CAPACITY = 5;
    constexpr std::uint64_t DEFAULT_TTL_MS = 120000; // 2 minutes

    LRUCacheTTL<int, std::string> cache(CAPACITY, DEFAULT_TTL_MS);

    std::cout << "Cache created: capacity=" << CAPACITY
              << ", default TTL=" << (DEFAULT_TTL_MS / 1000) << " seconds\n"
              << "Notes:\n"
              << " - Items may disappear either because the cache is full (LRU eviction)\n"
              << "   or because they expire (TTL).\n"
              << " - Lazy expiration: expired items are removed when accessed (get/put).\n";

    while (true) {
        printMenu();
        int choice = readIntInRange("Select (1-4): ", 1, 4);

        if (choice == 1) {
            int key = readIntInRange("Enter key (integer): ", -1000000000, 1000000000);

            auto v = cache.get(key);
            if (v.has_value()) {
                std::cout << "HIT: key=" << key << " value=\"" << *v << "\"\n";
            } else {
                std::cout << "MISS: key=" << key << " not found (or expired)\n";
            }

        } else if (choice == 2) {
            int key = readIntInRange("Enter key (integer): ", -1000000000, 1000000000);
            std::string value = readLine("Enter value (string): ");

            // Optional per-entry TTL override (seconds)
            // 0 = use default TTL (2 minutes)
            // If you want "never expires", we support it by entering -1? (but this is unsigned)
            // So we give a separate prompt: 0 default, 1..3600 seconds override, 999999 for no-expiry
            std::cout << "TTL options:\n"
                      << "  0  = use default (120 seconds)\n"
                      << "  1..3600 = override TTL (seconds)\n"
                      << "  999999 = no expiration\n";

            std::uint64_t ttlChoice = readUInt64InRange("Enter TTL option: ", 0, 999999);

            if (ttlChoice == 0) {
                cache.put(key, value); // uses default TTL from constructor
                std::cout << "Stored key=" << key << " (default TTL)\n";
            } else if (ttlChoice == 999999) {
                cache.put(key, value, 0); // ttl_ms==0 => no expiration
                std::cout << "Stored key=" << key << " (no expiration)\n";
            } else {
                cache.put(key, value, ttlChoice * 1000ULL);
                std::cout << "Stored key=" << key << " (TTL=" << ttlChoice << " seconds)\n";
            }

            std::cout << "Current cache size: " << cache.size() << "/" << cache.capacity() << "\n";

        } else if (choice == 3) {
            std::uint64_t seconds = readUInt64InRange("Sleep how many seconds? (0-600): ", 0, 600);
            std::cout << "Sleeping for " << seconds << " seconds...\n";
            std::this_thread::sleep_for(std::chrono::seconds(seconds));
            std::cout << "Awake.\n";

        } else { // 4
            std::cout << "Exiting.\n";
            break;
        }
    }

    return 0;
}
