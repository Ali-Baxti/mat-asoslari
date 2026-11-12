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

## Grafik interfeys (ixtiyoriy)
Qt 6 Widgets o'rnatilgan bo'lsa, CMake `prime_checker_gui` targetini ham yaratadi. Qt o'rnatilmagan bo'lsa, konsol dasturi, testlar va benchmark odatdagidek yig'iladi.

```bash
cmake -S . -B build
cmake --build build --config Release
```

Qt mavjud bo'lsa, `prime_checker_gui` dasturini ishga tushiring. Qt yo'q bo'lsa, CMake GUI targetini avtomatik o'tkazib yuboradi.

## Konsol kompilyatsiyasi
```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o prime_checker
./prime_checker
```

## Test va benchmark
```bash
ctest --test-dir build --output-on-failure
./build/prime_benchmark
```

Benchmark 10 ta namunaviy son bo'yicha algoritmlarni o'lchab, `benchmark_results.csv` yaratadi. Sinov bo'linish usullari 10^12 dan katta sonlarda uzoq ishlamasligi uchun benchmarkdan o'tkazib yuboriladi.

## Fayllar
- `main.cpp` — konsol interfeysi.
- `gui/main.cpp` — Qt 6 grafik interfeysi.
- `prime_checker.h` — deterministik sinov bo'linish algoritmlari.
- `probabilistic_primality.h` — Fermat va Miller-Rabin algoritmlari.
- `tests.cpp` — avtomatik testlar.
- `benchmark.cpp` — CSV benchmark.
- `docs/DEFENSE_GUIDE_UZ.md` — himoya qo'llanmasi.
- `docs/ALGORITHM_FLOWCHARTS.md` — algoritm blok-sxemalari.

## Ilmiy xulosa
Fermat va tasodifiy Miller-Rabin ehtimollik natijasini beradi. Deterministik Miller-Rabin uint64 diapazoni uchun aniq xulosa beradi. Benchmark vaqtlari qurilma, kompilyator va tizim yuklamasiga bog'liq.

## CI: avtomatik tekshiruv
GitHub Actions har bir `main` push va pull requestda Linux (GCC) hamda Windows (MSVC) muhitida loyihani yig'adi va testlarni ishga tushiradi. Linux job benchmarkni ham bajarib, `benchmark_results.csv` faylini artifact sifatida saqlaydi.

Holatni GitHub repository sahifasidagi **Actions** bo'limidan tekshiring. Workflow muvaffaqiyatli tugamaguncha testlardan o'tdi deb hisoblamang.

## Ilmiy ish uchun tavsiya etilgan natijalar
Benchmark CSV faylini Excel yoki LibreOffice Calc orqali oching. Bir xil son va algoritm uchun bir necha marta alohida o'lchov o'tkazib, medianani taqqoslang. Hisobotga CPU modeli, OS, kompilyator versiyasi va Release/Debug rejimini yozing. Turli kompyuterlardagi vaqtlarni bevosita teng sharoitdagi o'lchov deb qabul qilmang.
