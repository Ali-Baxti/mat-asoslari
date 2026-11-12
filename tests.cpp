#include "prime_checker.h"
#include <cstdint>
#include <iostream>

int main() {
    const uint64_t values[]={0,1,2,3,4,5,17,25,49,97,100,997,1000003,1000005,9999991};
    int passed=0,failed=0;
    for(uint64_t n:values) {
        uint64_t d1=0,c1=0,d2=0,c2=0;
        bool a=isPrime(n,d1,c1),b=isPrime6k1(n,d2,c2);
        if(a==b) { ++passed; std::cout << "[OK] " << n << ": natijalar mos\n"; }
        else { ++failed; std::cout << "[FAIL] " << n << ": natijalar farq qildi\n"; }
    }
    std::cout << "Jami: " << passed+failed << ", muvaffaqiyatli: " << passed << ", xato: " << failed << "\n";
    return failed==0 ? 0 : 1;
}
