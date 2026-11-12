#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include "prime_checker.h"

using namespace std;

bool parseNumber(const string& input, uint64_t& number) {
    if (input.empty() || input.front() == '-') {
        return false;
    }

    try {
        size_t processed = 0;
        unsigned long long value = stoull(input, &processed);

        if (processed != input.size()) {
            return false;
        }

        number = static_cast<uint64_t>(value);
        return true;
    } catch (...) {
        return false;
    }
}

int main() {
    cout << "==============================================\n";
    cout << "       TUB SONNI TEKSHIRISH DASTURI\n";
    cout << "  Deterministik sinov bulinish algoritmi\n";
    cout << "==============================================\n";

    while (true) {
        cout << "\nSon kiriting (chiqish uchun q): ";

        string input;
        getline(cin, input);

        if (input == "q" || input == "Q") {
            cout << "Dastur yakunlandi. Rahmat!\n";
            break;
        }

        uint64_t number = 0;
        if (!parseNumber(input, number)) {
            cout << "Xato: 0 dan katta yoki teng butun son kiriting.\n";
            continue;
        }

        uint64_t divisor = 0;
        uint64_t checks = 0;

        const auto start = chrono::high_resolution_clock::now();
        const bool prime = isPrime(number, divisor, checks);
        const auto finish = chrono::high_resolution_clock::now();

        const auto elapsed =
            chrono::duration<double, milli>(finish - start).count();

        cout << "\n------------- NATIJA -------------\n";
        cout << "Kiritilgan son: " << number << '\n';

        if (prime) {
            cout << "Xulosa: " << number << " — tub son.\n";
        } else {
            cout << "Xulosa: " << number << " — murakkab son yoki tub emas.\n";
            if (divisor != 0) {
                cout << "Topilgan bo'luvchi: " << divisor << '\n';
            } else if (number < 2) {
                cout << "Izoh: tub sonlar 2 dan boshlanadi.\n";
            }
        }

        cout << "Tekshirilgan bo'luvchilar soni: " << checks << '\n';
        cout << "Hisoblash vaqti: " << elapsed << " ms\n";
        cout << "----------------------------------\n";
    }

    return 0;
}
