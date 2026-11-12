#include "prime_checker.h"

#include <cstdint>
#include <iostream>
#include <string>

int main() {
    uint64_t divisor = 0;
    uint64_t checks = 0;
    int passed = 0;
    int failed = 0;

    auto check = [&](bool condition, const std::string& name) {
        if (condition) {
            ++passed;
            std::cout << "[OK]   " << name << '\\n';
        } else {
            ++failed;
            std::cout << "[FAIL] " << name << '\\n';
        }
    };

    check(!isPrime(0, divisor, checks), "0 tub emas");
    check(!isPrime(1, divisor, checks), "1 tub emas");
    check(isPrime(2, divisor, checks), "2 tub");
    check(isPrime(3, divisor, checks), "3 tub");
    check(!isPrime(4, divisor, checks) && divisor == 2, "4 murakkab");
    check(isPrime(17, divisor, checks), "17 tub");
    check(isPrime(97, divisor, checks), "97 tub");
    check(!isPrime(25, divisor, checks) && divisor == 5, "25 ning bo'luvchisi 5");
    check(!isPrime(49, divisor, checks) && divisor == 7, "49 ning bo'luvchisi 7");
    check(!isPrime(100, divisor, checks) && divisor == 2, "100 ning bo'luvchisi 2");
    check(isPrime(1000003, divisor, checks), "1000003 tub");
    check(!isPrime(1000005, divisor, checks), "1000005 murakkab");

    std::cout << "\\nJami: " << passed + failed
              << ", muvaffaqiyatli: " << passed
              << ", xato: " << failed << '\\n';
    return failed == 0 ? 0 : 1;
}
