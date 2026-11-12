# Tub sonlarni tekshirish va algoritmlarni taqqoslash

**Fan:** Matematik asoslar  
**Til:** C++17  
**Sonlar diapazoni:** unsigned 64-bit (0 dan 2^64-1 gacha)

## Loyiha maqsadi
Tub sonlarni aniqlashning deterministik va ehtimollik algoritmlarini amaliy taqqoslash.

## Algoritmlar
1. **Oddiy sinov bo'linish** — O(sqrt(n)) vaqt, O(1) xotira.
2. **6k +/- 1** — 2 va 3 dan keyin 6k-1 hamda 6k+1 nomzodlarini tekshiradi.
3. **Fermat** — ehtimollik testi; Carmichael sonlarida xato musbat natija berishi mumkin.
4. **Miller-Rabin** — kuchli ehtimollik testi.
5. **Deterministik Miller-Rabin** — maxsus asoslar bilan butun uint64 diapazonida aniq natija beradi.

## Qurish va ishga tushirish
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

Qt 6 Widgets mavjud bo'lsa, ixtiyoriy `prime_checker_gui` targeti ham yaratiladi. Qt bo'lmasa, konsol dasturi, testlar va benchmark odatdagidek yig'iladi.

Konsol uchun:
```bash
./build/prime_checker
```

Benchmark uchun:
```bash
./build/prime_benchmark
```

Windows'da Visual Studio generatori bilan qurilganda executable fayllar `build/Release/` ichida bo'lishi mumkin.

## Benchmark metodikasi
Benchmark 10 ta namunaviy sonni oladi. Har bir algoritm uchun bir marta warm-up va 11 ta vaqt o'lchovi bajariladi. CSV faylida minimum, median va maksimum mikrosekundlarda yoziladi. Taqqoslashda median qiymatidan foydalanish tavsiya etiladi.

Sinov bo'linish usullari 10^12 dan katta sonlarda uzoq ishlamasligi uchun o'tkazib yuboriladi va CSV'da `skipped_large_input` deb belgilanadi. Bu algoritm natijasi emas, balki benchmark chegarasidir. Deterministik Miller-Rabin barcha namunalarni tekshiradi.

Benchmark vaqti protsessor, operatsion tizim, kompilyator, build turi va fon yuklamasiga bog'liq. Hisobotda ushbu muhit ma'lumotlarini qayd eting.

## Fayllar
- `main.cpp` — konsol interfeysi.
- `gui/main.cpp` — Qt 6 grafik interfeysi.
- `prime_checker.h` — deterministik sinov bo'linish algoritmlari.
- `probabilistic_primality.h` — Fermat va Miller-Rabin algoritmlari.
- `tests.cpp` — avtomatik testlar.
- `benchmark.cpp` — takroriy o'lchovlar bilan CSV benchmark.
- `docs/DEFENSE_GUIDE_UZ.md` — himoya qo'llanmasi.
- `docs/ALGORITHM_FLOWCHARTS.md` — algoritm blok-sxemalari.

## Ilmiy xulosa
Fermat va tasodifiy Miller-Rabin ehtimollik natijasini beradi. Deterministik Miller-Rabin uint64 diapazoni uchun aniq xulosa beradi. Tajriba xulosalari faqat amalda olingan CSV natijalari asosida yozilishi kerak.

## CI: avtomatik tekshiruv
GitHub Actions har bir `main` push va pull requestda Linux (GCC) hamda Windows (MSVC) muhitida loyihani yig'adi va testlarni ishga tushiradi. Linux job benchmarkni ham bajarib, `benchmark_results.csv` faylini artifact sifatida saqlaydi. Holatni repository'dagi **Actions** bo'limidan tekshiring.
