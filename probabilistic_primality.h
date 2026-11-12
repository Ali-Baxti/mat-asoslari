#ifndef PROBABILISTIC_PRIMALITY_H
#define PROBABILISTIC_PRIMALITY_H

#include <cstdint>
#include <random>

// Overflow-safe (a + b) mod m; talab: a < m va b < m.
inline uint64_t addMod(uint64_t a, uint64_t b, uint64_t m) {
    return a >= m - b ? a - (m - b) : a + b;
}

// Ko'paytirishni qo'shish va ikkiga bo'lish orqali bajaradi.
// __int128 talab qilmagani uchun C++17 kompilyatorlari bilan ko'chma.
inline uint64_t multiplyMod(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t result = 0;
    a %= m;
    while (b > 0) {
        if (b & 1U) result = addMod(result, a, m);
        b >>= 1U;
        if (b > 0) a = addMod(a, a, m);
    }
    return result;
}

inline uint64_t powerMod(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    base %= modulus;
    while (exponent > 0) {
        if (exponent & 1U) result = multiplyMod(result, base, modulus);
        exponent >>= 1U;
        if (exponent > 0) base = multiplyMod(base, base, modulus);
    }
    return result;
}

struct ProbablePrimeResult {
    bool probablePrime = false;
    uint64_t witness = 0;
    uint64_t roundsRun = 0;
};

// Fermat testi: ehtimollik asosidagi test. Natija 'ehtimol tub' ma'nosini beradi.
inline ProbablePrimeResult fermatTest(uint64_t n, uint64_t rounds, uint64_t seed = 2026) {
    if (n < 2) return {false, 0, 0};
    if (n == 2 || n == 3) return {true, 0, 0};
    if (n % 2 == 0) return {false, 2, 0};
    if (rounds == 0) rounds = 1;

    std::mt19937_64 generator(seed);
    std::uniform_int_distribution<uint64_t> chooseBase(2, n - 2);
    for (uint64_t round = 0; round < rounds; ++round) {
        const uint64_t a = chooseBase(generator);
        if (powerMod(a, n - 1, n) != 1) return {false, a, round + 1};
    }
    return {true, 0, rounds};
}

// Miller-Rabin kuchli ehtimollik testi.
inline ProbablePrimeResult millerRabinTest(uint64_t n, uint64_t rounds, uint64_t seed = 2026) {
    if (n < 2) return {false, 0, 0};
    if (n == 2 || n == 3) return {true, 0, 0};
    if (n % 2 == 0) return {false, 2, 0};
    if (rounds == 0) rounds = 1;

    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1U) == 0) { d >>= 1U; ++s; }

    std::mt19937_64 generator(seed);
    std::uniform_int_distribution<uint64_t> chooseBase(2, n - 2);
    for (uint64_t round = 0; round < rounds; ++round) {
        const uint64_t a = chooseBase(generator);
        uint64_t x = powerMod(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool passedRound = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = multiplyMod(x, x, n);
            if (x == n - 1) { passedRound = true; break; }
            if (x == 1) return {false, a, round + 1};
        }
        if (!passedRound) return {false, a, round + 1};
    }
    return {true, 0, rounds};
}

#endif
