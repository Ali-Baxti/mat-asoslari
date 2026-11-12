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

#endif
