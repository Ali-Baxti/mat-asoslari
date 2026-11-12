#ifndef PRIME_CHECKER_H
#define PRIME_CHECKER_H

#include <cstdint>

// Deterministik sinov bo'linish algoritmi.
// divisor: topilgan bo'luvchi; checks: tekshirilgan toq bo'luvchilar soni.
inline bool isPrime(uint64_t n, uint64_t& divisor, uint64_t& checks) {
    checks = 0;
    divisor = 0;

    if (n < 2) return false;
    if (n == 2 || n == 3) return true;

    if (n % 2 == 0) {
        divisor = 2;
        return false;
    }

    // n / i shakli i * i ko'paytmasidagi overflow'ni oldini oladi.
    for (uint64_t i = 3; i <= n / i; i += 2) {
        ++checks;
        if (n % i == 0) {
            divisor = i;
            return false;
        }
    }

    return true;
}

// 6k +/- 1 optimallashtirilgan deterministik algoritm.
// 2 va 3 dan katta tub sonlar 6k-1 yoki 6k+1 ko'rinishida bo'ladi.
inline bool isPrime6k1(uint64_t n, uint64_t& divisor, uint64_t& checks) {
    checks = 0;
    divisor = 0;
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) { divisor = 2; return false; }
    if (n % 3 == 0) { divisor = 3; return false; }
    for (uint64_t i = 5; i <= n / i; i += 6) {
        ++checks;
        if (n % i == 0) { divisor = i; return false; }
        ++checks;
        if (n % (i + 2) == 0) { divisor = i + 2; return false; }
    }
    return true;
}

#endif
