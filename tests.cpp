#include "prime_checker.h"
#include "probabilistic_primality.h"
#include <cstdint>
#include <iostream>

int main(){int passed=0,failed=0;
 auto check=[&](bool ok,const char* name){if(ok){++passed;std::cout<<"[OK] "<<name<<"\n";}else{++failed;std::cout<<"[FAIL] "<<name<<"\n";}};
 const uint64_t values[]={0,1,2,3,4,5,17,25,49,97,100,997,1000003,1000005,9999991};
 for(uint64_t n:values){uint64_t a=0,b=0,c=0,d=0;bool x=isPrime(n,a,b),y=isPrime6k1(n,c,d);check(x==y,"Oddiy va 6k+/-1 algoritmlari mosligi");check(x==isPrimeDeterministic64(n),"Sinov bo'linish va deterministik MR mosligi");}
 check(!isPrimeDeterministic64(0),"0 tub emas");
 check(!isPrimeDeterministic64(1),"1 tub emas");
 check(isPrimeDeterministic64(2),"2 tub");
 check(isPrimeDeterministic64(18446744073709551557ULL),"Katta 64-bit tub son");
 check(!isPrimeDeterministic64(UINT64_MAX),"UINT64_MAX murakkab");
 check(!isPrimeDeterministic64(341550071728321ULL),"Kuchli psevdotub son 1");
 check(!isPrimeDeterministic64(3825123056546413051ULL),"Kuchli psevdotub son 2");
 check(!millerRabinTest(561,12).probablePrime,"Miller-Rabin Carmichael 561 ni aniqlaydi");
 check(fermatTest(97,10).probablePrime,"Fermat tub sonni qabul qiladi");
 check(!fermatTest(15,10).probablePrime,"Fermat murakkab sonni rad etadi");
 check(multiplyMod(UINT64_MAX-1,UINT64_MAX-1,UINT64_MAX)==1,"Modulyar ko'paytirish overflow sinovi");
 std::cout<<"\nJami: "<<passed+failed<<", muvaffaqiyatli: "<<passed<<", xato: "<<failed<<"\n";return failed==0?0:1;}
