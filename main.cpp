#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include "prime_checker.h"
#include "probabilistic_primality.h"

using Clock = std::chrono::steady_clock;

bool parseNumber(const std::string& input, uint64_t& number) {
    if (input.empty() || input.front() == '-') return false;
    try {
        std::size_t used = 0;
        const auto value = std::stoull(input, &used);
        if (used != input.size()) return false;
        number = static_cast<uint64_t>(value);
        return true;
    } catch (...) { return false; }
}

template<class Function> double measureMs(Function function) {
    const auto start = Clock::now(); function();
    return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}

int main() {
    std::cout << "==============================================\n";
    std::cout << "       TUB SONLARNI TAHLIL QILISH DASTURI\n";
    std::cout << "==============================================\n";
    std::cout << "64-bit sonlar | 5 ta algoritm | C++17\n";
    std::string input;
    while (true) {
        std::cout << "\nSon kiriting (chiqish uchun q): ";
        if (!std::getline(std::cin, input)) break;
        if (input == "q" || input == "Q") break;
        uint64_t n = 0;
        if (!parseNumber(input, n)) {
            std::cout << "Xato: 0 dan UINT64_MAX gacha butun son kiriting.\n";
            continue;
        }
        uint64_t d1=0,c1=0,d2=0,c2=0;
        bool p1=false,p2=false;
        const double t1 = measureMs([&]{ p1=isPrime(n,d1,c1); });
        const double t2 = measureMs([&]{ p2=isPrime6k1(n,d2,c2); });
        ProbablePrimeResult fermat, mr;
        const double tf = measureMs([&]{ fermat=fermatTest(n,10,2026); });
        const double tm = measureMs([&]{ mr=millerRabinTest(n,10,2026); });
        bool exact=false;
        const double td = measureMs([&]{ exact=isPrimeDeterministic64(n); });
        std::cout << "\n--- NATIJALAR ---\nSon: " << n << "\n";
        std::cout << "1. Oddiy bo'linish: " << (p1?"TUB":"TUB EMAS") << " | tekshiruv: " << c1 << " | " << t1 << " ms\n";
        if (d1) std::cout << "   Bo'luvchi: " << d1 << "\n";
        std::cout << "2. 6k +/- 1: " << (p2?"TUB":"TUB EMAS") << " | tekshiruv: " << c2 << " | " << t2 << " ms\n";
        if (d2) std::cout << "   Bo'luvchi: " << d2 << "\n";
        std::cout << "3. Fermat (10 raund): " << (fermat.probablePrime?"EHTIMOL TUB":"MURAKKAB") << " | raund: " << fermat.roundsRun << " | " << tf << " ms\n";
        std::cout << "4. Miller-Rabin (10 raund): " << (mr.probablePrime?"EHTIMOL TUB":"MURAKKAB") << " | raund: " << mr.roundsRun << " | " << tm << " ms\n";
        std::cout << "5. Deterministik Miller-Rabin: " << (exact?"TUB":"TUB EMAS") << " | " << td << " ms\n";
        std::cout << "Tekshiruv: oddiy va 6k+/-1 natijalari " << (p1==p2?"MOS":"FARQ QILDI") << ".\n";
        std::cout << "Eslatma: Fermat va 10 raundli Miller-Rabin ehtimollik testlaridir.\n";
        std::cout << "Aniq xulosa uchun deterministik 64-bit Miller-Rabin natijasidan foydalaning.\n";
    }
    std::cout << "\nDastur yakunlandi.\n";
    return 0;
}
