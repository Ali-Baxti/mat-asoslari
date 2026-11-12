#include "prime_checker.h"

#include <cassert>
#include <cstdint>
#include <iostream>

int main() {
    uint64_t divisor = 0;
    uint64_t checks = 0;

    assert(!isPrime(0, divisor, checks));
    assert(!isPrime(1, divisor, checks));
    assert(isPrime(2, divisor, checks));
    assert(isPrime(3, divisor, checks));
    assert(!isPrime(4, divisor, checks));
    assert(divisor == 2);
    assert(isPrime(17, divisor, checks));
    assert(isPrime(97, divisor, checks));
    assert(!isPrime(25, divisor, checks));
    assert(divisor == 5);
    assert(!isPrime(49, divisor, checks));
    assert(divisor == 7);
    assert(!isPrime(100, divisor, checks));
    assert(divisor == 2);
    assert(isPrime(1000003, divisor, checks));
    assert(!isPrime(1000005, divisor, checks));

    std::cout << "All tests passed!\n";
    return 0;
}
