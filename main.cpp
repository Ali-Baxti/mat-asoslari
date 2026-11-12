#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include "prime_checker.h"
#include "probabilistic_primality.h"

using namespace std;

bool parseNumber(const string& input, uint64_t& number) {
    if(input.empty() || input.front()=='-') return false;
    try { size_t used=0; auto value=stoull(input,&used); if(used!=input.size()) return false; number=static_cast<uint64_t>(value); return true; }
    catch(...) { return false; }
}

template<class F> double measure(F f,uint64_t n,bool& answer,uint64_t& divisor,uint64_t& checks) {
    auto start=chrono::steady_clock::now(); answer=f(n,divisor,checks);
    return chrono::duration<double,milli>(chrono::steady_clock::now()-start).count();
}

int main() {
    cout << "TUB SONNI TEKSHIRISH: 4 ALGORITM TAQQOSLANISHI\n";
    while(true) {
        cout << "\nSon kiriting (chiqish: q): "; string input; getline(cin,input);
        if(input=="q" || input=="Q") break;
        uint64_t n=0; if(!parseNumber(input,n)) { cout << "Xato: manfiy bo'lmagan butun son kiriting.\n"; continue; }
        uint64_t d1=0,c1=0,d2=0,c2=0; bool p1=false,p2=false;
        double t1=measure(isPrime,n,p1,d1,c1), t2=measure(isPrime6k1,n,p2,d2,c2);
        auto a=chrono::steady_clock::now(); auto f=fermatTest(n,10,2026);
        double tf=chrono::duration<double,milli>(chrono::steady_clock::now()-a).count();
        a=chrono::steady_clock::now(); auto mr=millerRabinTest(n,10,2026);
        double tm=chrono::duration<double,milli>(chrono::steady_clock::now()-a).count();
        cout << "\nSon: " << n << "\n";
        cout << "1) Oddiy usul: " << (p1?"TUB":"TUB EMAS") << ", tekshiruv=" << c1 << ", vaqt=" << t1 << " ms\n";
        if(d1) cout << "   Bo'luvchi: " << d1 << "\n";
        cout << "2) 6k +/- 1: " << (p2?"TUB":"TUB EMAS") << ", tekshiruv=" << c2 << ", vaqt=" << t2 << " ms\n";
        if(d2) cout << "   Bo'luvchi: " << d2 << "\n";
        cout << "3) Fermat (10 raund): " << (f.probablePrime?"EHTIMOL TUB":"MURAKKAB") << ", raund=" << f.roundsRun << ", vaqt=" << tf << " ms\n";
        cout << "4) Miller-Rabin (10 raund): " << (mr.probablePrime?"EHTIMOL TUB":"MURAKKAB") << ", raund=" << mr.roundsRun << ", vaqt=" << tm << " ms\n";
        cout << "Deterministik natijalar: " << (p1==p2?"mos":"farq bor") << "\n";
        cout << "Eslatma: ehtimollik testidagi EHTIMOL TUB mutlaq isbot emas.\n";
    }
    cout << "Dastur yakunlandi.\n"; return 0;
}
