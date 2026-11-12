# Tub sonni tekshirish va algoritmlarni taqqoslash

**Fan:** Matematik asoslar  
**Mavzu:** Sonlarni tubikka tekshiruvchi deterministik va ehtimollik algoritmlarining kompleks dasturiy ta'minotini ishlab chiqish  
**Til:** C++17

## Maqsad
Kiritilgan 64-bit butun sonning tub yoki murakkabligini to'rtta algoritm bilan tekshirish va natijalarni taqqoslash.

## Algoritmlar

### 1. Oddiy sinov bo'linish
Toq bo'luvchilar 3 dan sqrt(n) gacha tekshiriladi. Vaqt murakkabligi O(sqrt(n)), xotira O(1).

### 2. 6k +/- 1 sinov bo'linish
2 va 3 ga bo'linmaydigan tub sonlar 6k-1 yoki 6k+1 ko'rinishida bo'ladi. Faqat shu nomzodlar tekshiriladi. Vaqt O(sqrt(n)), xotira O(1).

### 3. Fermat ehtimollik testi
Fermat kichik teoremasiga asoslanadi: tub n uchun a^(n-1) ≡ 1 (mod n), agar 1<a<n-1 bo'lsa. Bir nechta asos tekshiriladi. Carmichael sonlari kabi murakkab sonlar testdan o'tib ketishi mumkin; natija "ehtimol tub" deb talqin qilinadi.

### 4. Miller-Rabin ehtimollik testi
n-1=d*2^s ko'rinishidan foydalanadigan kuchli ehtimollik testi. Raundlar soni oshishi bilan xato ehtimoli kamayadi, ammo bu versiya ham "ehtimol tub" xulosasini beradi. Dastur 10 raund va qayta ishlab bo'ladigan seed ishlatadi.

### 5. Deterministik Miller-Rabin (64-bit)
Maxsus tanlangan 7 ta asos butun uint64_t diapazoni uchun deterministik natija beradi. Bu ehtimollikdagi Miller-Rabin versiyasidan farq qiladi.

## Modulyar arifmetika
Modulyar ko'paytirish qo'shish va ikkiga bo'lish orqali bajariladi, shu sababli C++17 da __int128 talab qilmaydi.

## Kompilyatsiya va ishga tushirish

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o prime_checker
./prime_checker
```

Windowsda prime_checker.exe faylini ishga tushiring.

## Avtomatik testlar

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Testlar chegaraviy qiymatlar, kichik va katta tub/murakkab sonlar, Carmichael 561 hamda modulyar ko'paytirishni qamrab oladi.

## Fayllar

- main.cpp — konsol interfeysi, vaqt o'lchash va taqqoslash.
- prime_checker.h — ikkita deterministik algoritm.
- probabilistic_primality.h — Fermat, Miller-Rabin va modulyar arifmetika.
- tests.cpp — avtomatik testlar.
- CMakeLists.txt — yig'ish va test sozlamalari.
- .github/workflows/cpp.yml — GitHub Actions avtomatik tekshiruvi.

## Benchmark va statistik ma'lumotlar

Barcha algoritmlarni bir xil oldindan tanlangan sonlarda o'lchash uchun benchmark dasturi mavjud.

```bash
cmake --build build
./build/prime_benchmark
```

Windowsda: `build\\Debug\\prime_benchmark.exe` (yoki Release konfiguratsiyasidagi mos papka) ni ishga tushiring.

Dastur `benchmark_results.csv` faylini yaratadi. Uni Excel orqali ochib, algoritmlar kesimida vaqt diagrammasi tuzish mumkin. O'lchangan vaqt qurilma, operatsion tizim va kompilyator optimallashtirishlariga bog'liq; juda qisqa vaqtlar shovqin ta'sirida o'zgarishi mumkin.
