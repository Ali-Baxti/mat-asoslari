#include "prime_checker.h"
#include "probabilistic_primality.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Clock = std::chrono::steady_clock;

struct Timing {
    double minUs = 0.0;
    double medianUs = 0.0;
    double maxUs = 0.0;
};

template <class Function>
Timing measureMedian(Function function, unsigned repetitions = 11) {
    // Warm-up: first call may include cold-cache/runtime effects.
    function();

    std::vector<double> values;
    values.reserve(repetitions);

    for (unsigned i = 0; i < repetitions; ++i) {
        const auto start = Clock::now();
        function();
        const auto stop = Clock::now();
        values.push_back(
            std::chrono::duration<double, std::micro>(stop - start).count()
        );
    }

    std::sort(values.begin(), values.end());
    return {values.front(), values[values.size() / 2], values.back()};
}

void writeRow(std::ofstream& csv,
              uint64_t number,
              const std::string& algorithm,
              const std::string& result,
              const std::string& checks,
              const Timing& timing,
              unsigned repetitions) {
    csv << number << ',' << algorithm << ',' << result << ',' << checks << ','
        << timing.minUs << ',' << timing.medianUs << ',' << timing.maxUs << ','
        << repetitions << '\n';
}

int main() {
    const std::vector<uint64_t> samples = {
        97, 997, 99991, 999983, 1000003,
        1000005, 9999991, 99999989, 1000000007,
        18446744073709551557ULL
    };

    constexpr unsigned repetitions = 11;
    constexpr uint64_t trialDivisionLimit = 1000000000000ULL;

    std::ofstream csv("benchmark_results.csv");
    if (!csv) {
        std::cerr << "CSV faylini yaratib bo'lmadi.\n";
        return 1;
    }

    csv << "number,algorithm,result,checks_or_rounds,min_us,median_us,max_us,repetitions\n";

    for (const uint64_t n : samples) {
        if (n <= trialDivisionLimit) {
            uint64_t divisor = 0;
            uint64_t checks = 0;
            bool answer = false;

            const Timing timing = measureMedian([&] {
                answer = isPrime(n, divisor, checks);
            }, repetitions);

            writeRow(csv, n, "trial_division",
                     answer ? "prime" : "composite",
                     std::to_string(checks), timing, repetitions);

            divisor = 0;
            checks = 0;
            const Timing optimizedTiming = measureMedian([&] {
                answer = isPrime6k1(n, divisor, checks);
            }, repetitions);

            writeRow(csv, n, "six_k_plus_minus_one",
                     answer ? "prime" : "composite",
                     std::to_string(checks), optimizedTiming, repetitions);
        } else {
            const Timing skipped{};
            writeRow(csv, n, "trial_division", "skipped_large_input",
                     "NA", skipped, 0);
            writeRow(csv, n, "six_k_plus_minus_one", "skipped_large_input",
                     "NA", skipped, 0);
        }

        ProbablePrimeResult fermat;
        const Timing fermatTiming = measureMedian([&] {
            fermat = fermatTest(n, 10, 2026);
        }, repetitions);
        writeRow(csv, n, "fermat_10_rounds",
                 fermat.probablePrime ? "probable_prime" : "composite",
                 std::to_string(fermat.roundsRun), fermatTiming, repetitions);

        ProbablePrimeResult millerRabin;
        const Timing mrTiming = measureMedian([&] {
            millerRabin = millerRabinTest(n, 10, 2026);
        }, repetitions);
        writeRow(csv, n, "miller_rabin_10_rounds",
                 millerRabin.probablePrime ? "probable_prime" : "composite",
                 std::to_string(millerRabin.roundsRun), mrTiming, repetitions);

        bool exact = false;
        const Timing exactTiming = measureMedian([&] {
            exact = isPrimeDeterministic64(n);
        }, repetitions);
        writeRow(csv, n, "deterministic_miller_rabin_64",
                 exact ? "prime" : "composite",
                 "7 fixed bases", exactTiming, repetitions);
    }

    csv.close();
    if (!csv) {
        std::cerr << "CSV faylini to'liq yozishda xatolik yuz berdi.\n";
        return 1;
    }

    std::cout << "Tayyor: " << samples.size() << " ta son benchmark qilindi.\n";
    std::cout << "Har bir algoritm uchun " << repetitions
              << " ta o'lchov olindi; median asosiy taqqoslash qiymati.\n";
    std::cout << "Katta sonlarda sekin sinov-bo'linish usullari o'tkazib yuboriladi.\n";
    std::cout << "Natijalar benchmark_results.csv faylida (mikrosekund).\n";
    return 0;
}
