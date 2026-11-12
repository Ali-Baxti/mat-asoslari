#include "prime_checker.h"
#include "probabilistic_primality.h"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using Clock=std::chrono::steady_clock;
template<class F> double timed(F f){auto s=Clock::now();f();return std::chrono::duration<double,std::micro>(Clock::now()-s).count();}
int main(){
 const std::vector<uint64_t> samples={97,997,99991,999983,1000003,1000005,9999991,99999989,1000000007,18446744073709551557ULL};
 std::ofstream csv("benchmark_results.csv");
 if(!csv){std::cerr<<"CSV faylini yaratib bo'lmadi.\n";return 1;}
 csv<<"number,algorithm,result,checks_or_rounds,time_microseconds\n";
 for(uint64_t n:samples){
  uint64_t divisor=0,checks=0; bool answer=false;
  double us=timed([&]{answer=isPrime(n,divisor,checks);});
  csv<<n<<",trial_division,"<<(answer?"prime":"composite")<<","<<checks<<","<<us<<"\n";
  us=timed([&]{answer=isPrime6k1(n,divisor,checks);});
  csv<<n<<",six_k_plus_minus_one,"<<(answer?"prime":"composite")<<","<<checks<<","<<us<<"\n";
  ProbablePrimeResult f; us=timed([&]{f=fermatTest(n,10,2026);});
  csv<<n<<",fermat_10_rounds,"<<(f.probablePrime?"probable_prime":"composite")<<","<<f.roundsRun<<","<<us<<"\n";
  ProbablePrimeResult mr; us=timed([&]{mr=millerRabinTest(n,10,2026);});
  csv<<n<<",miller_rabin_10_rounds,"<<(mr.probablePrime?"probable_prime":"composite")<<","<<mr.roundsRun<<","<<us<<"\n";
  bool exact=false; us=timed([&]{exact=isPrimeDeterministic64(n);});
  csv<<n<<",deterministic_miller_rabin_64,"<<(exact?"prime":"composite")<<",7,"<<us<<"\n";
 }
 std::cout<<"Tayyor: "<<samples.size()<<" ta son, "<<samples.size()*5<<" ta o'lchov benchmark_results.csv fayliga yozildi.\n";
 std::cout<<"Vaqt mikrosekundlarda. Katta sonlar uchun sinov bo'linish usuli uzoq ishlashi mumkin.\n";
 return 0;
}
