#include "prime_checker.h"
#include "probabilistic_primality.h"
#include <cstdint>
#include <iostream>

int main() {
    int passed=0,failed=0;
    auto check=[&](bool ok,const char* label){ if(ok){++passed;std::cout<<"[OK] "<<label<<"\n";}else{++failed;std::cout<<"[FAIL] "<<label<<"\n";} };
    const uint64_t values[]={0,1,2,3,4,5,17,25,49,97,100,997,1000003,1000005,9999991};
    for(uint64_t n:values){uint64_t a=0,b=0,c=0,d=0;check(isPrime(n,a,b)==isPrime6k1(n,c,d),"deterministik algoritmlar mosligi");}
    check(!millerRabinTest(0,10).probablePrime,"Miller-Rabin: 0");
    check(!millerRabinTest(1,10).probablePrime,"Miller-Rabin: 1");
    check(millerRabinTest(2,10).probablePrime,"Miller-Rabin: 2");
    check(millerRabinTest(97,10).probablePrime,"Miller-Rabin: 97");
    check(!millerRabinTest(561,12).probablePrime,"Miller-Rabin: Carmichael 561");
    check(!millerRabinTest(1000005,10).probablePrime,"Miller-Rabin: 1000005");
    check(fermatTest(97,10).probablePrime,"Fermat: 97");
    check(!fermatTest(15,10).probablePrime,"Fermat: 15");
    check(multiplyMod(UINT64_MAX-1,UINT64_MAX-1,UINT64_MAX)==1,"modulyar ko'paytirish overflow sinovi");
    std::cout<<"\nJami: "<<passed+failed<<", muvaffaqiyatli: "<<passed<<", xato: "<<failed<<"\n";
    return failed==0?0:1;
}
