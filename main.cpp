#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include "prime_checker.h"

using namespace std;

bool parseNumber(const string& input, uint64_t& number) {
    if (input.empty() || input.front() == '-') return false;
    try { size_t processed=0; unsigned long long value=stoull(input,&processed); if(processed!=input.size()) return false; number=static_cast<uint64_t>(value); return true; }
    catch (...) { return false; }
}

template <typename Algorithm>
double measureMilliseconds(Algorithm algorithm,uint64_t number,bool& result,uint64_t& divisor,uint64_t& checks) {
    const auto start=chrono::steady_clock::now(); result=algorithm(number,divisor,checks);
    const auto finish=chrono::steady_clock::now();
    return chrono::duration<double,milli>(finish-start).count();
}

int main() {
    cout << "TUB SONNI TEKSHIRISH DASTURI\nIkki deterministik algoritm taqqoslanadi\n";
    while(true) {
        cout << "\nSon kiriting (chiqish uchun q): "; string input; getline(cin,input);
        if(input=="q" || input=="Q") break;
        uint64_t number=0;
        if(!parseNumber(input,number)) { cout << "Xato: manfiy bo'lmagan butun son kiriting.\n"; continue; }
        uint64_t d1=0,c1=0,d2=0,c2=0; bool p1=false,p2=false;
        double t1=measureMilliseconds(isPrime,number,p1,d1,c1);
        double t2=measureMilliseconds(isPrime6k1,number,p2,d2,c2);
        cout << "\nSon: " << number << "\nXulosa: " << (p2 ? "tub" : "tub emas") << "\n";
        cout << "1) Oddiy usul: " << (p1 ? "tub" : "tub emas") << ", tekshiruvlar=" << c1 << ", vaqt=" << t1 << " ms\n";
        if(d1) cout << "   Bo'luvchi: " << d1 << "\n";
        cout << "2) 6k +/- 1 usuli: " << (p2 ? "tub" : "tub emas") << ", tekshiruvlar=" << c2 << ", vaqt=" << t2 << " ms\n";
        if(d2) cout << "   Bo'luvchi: " << d2 << "\n";
        cout << "Algoritmlar natijasi: " << (p1==p2 ? "bir xil" : "farq bor!") << "\n";
    }
    cout << "Dastur yakunlandi.\n"; return 0;
}
