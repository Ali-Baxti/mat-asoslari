# Tub sonlarni tekshirish va algoritmlarni taqqoslash

**Fan:** Matematik asoslar  
**Til:** C++17  
**Sonlar diapazoni:** unsigned 64-bit (0 dan 2^64-1 gacha)

## Loyiha maqsadi
Tub sonlarni aniqlashning deterministik va ehtimollik algoritmlarini amaliy jihatdan taqqoslash. Dastur natija, tekshiruvlar soni va bajarilish vaqtini ko'rsatadi.

## Algoritmlar
1. **Oddiy sinov bo'linish:** toq bo'luvchilarni tekshiradi; O(sqrt(n)) vaqt, O(1) xotira.
2. **6k +/- 1:** 2 va 3 dan keyin faqat 6k-1 va 6k+1 nomzodlarini tekshiradi; O(sqrt(n)) vaqt.
3. **Fermat:** Fermat kichik teoremasiga asoslangan ehtimollik testi. Carmichael sonlari sababli xato musbat natija berishi mumkin.
4. **Miller-Rabin:** kuchli ehtimollik testi; raundlar ko'payishi xato ehtimolini kamaytiradi.
5. **Deterministik Miller-Rabin:** maxsus 7 ta asos bilan butun uint64 diapazonida deterministik natija beradi.

## Modulyar arifmetika
Overflow xavfini kamaytiruvchi qo'shish va ikkilik ko'paytirish ishlatiladi. Bu usul C++17 da __int128 talab qilmaydi.

## Talablar
- C++17 kompilyator (GCC, Clang yoki MSVC)
- CMake 3.16 yoki yangiroq (test va benchmark uchun)

## Kompilyatsiya

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o prime_checker
./prime_checker
```

## Test va benchmark

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
./build/prime_benchmark
```

Windowsda Visual Studio generatoridan foydalanilganda bajariladigan fayllar `build/Release/` ichida bo'lishi mumkin.

Benchmark 10 ta namunaviy sonni 5 algoritm bilan tekshiradi va `benchmark_results.csv` fayliga 50 ta o'lchov yozadi. Sinov bo'linish algoritmlari katta 64-bit tub sonlarda juda sekin ishlashi mumkin; bu algoritmning nazariy murakkabligini ko'rsatish uchun foydali, lekin benchmark uzoq davom etishi ehtimoli bor.

## Testlar
Chegaraviy qiymatlar, katta tub son, UINT64_MAX, kuchli psevdotub sonlar, Carmichael soni va algoritmlararo moslik tekshiriladi.

## Fayllar
- `main.cpp` — interaktiv tekshiruv.
- `prime_checker.h` — oddiy va 6k+/-1 algoritmlari.
- `probabilistic_primality.h` — Fermat, Miller-Rabin va deterministik 64-bit Miller-Rabin.
- `tests.cpp` — avtomatik testlar.
- `benchmark.cpp` — CSV benchmark.
- `CMakeLists.txt` — build va CTest sozlamalari.
- `.github/workflows/cpp.yml` — GitHub Actions CI.

## Ilmiy xulosa uchun eslatma
Fermat va oddiy Miller-Rabin natijalari ehtimollik asosida. Deterministik 64-bit Miller-Rabin esa ushbu diapazon uchun aniq natija beradi. Benchmark vaqtlari qurilma, kompilyator, optimallashtirish va tizim yuklamasiga bog'liq; natijalarni bir xil sharoitda bir necha marta o'lchash lozim.
