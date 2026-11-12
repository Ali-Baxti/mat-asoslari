# Tub sonni tekshirish va algoritmlarni taqqoslash

**Fan:** Matematik asoslar  
**Mavzu:** Sonlarni tubikka tekshiruvchi deterministik algoritmlarning kompleks dasturiy ta'minotini ishlab chiqish  
**Dasturlash tili:** C++17

## Loyiha maqsadi

Kiritilgan son tub yoki tub emasligini ikki deterministik algoritm yordamida aniqlash va tekshiruvlar soni hamda ishlash vaqtini taqqoslash.

## Algoritmlar

### 1. Oddiy toq bo'luvchilar usuli

3 dan boshlab toq bo'luvchilar sqrt(n) gacha tekshiriladi. Vaqt murakkabligi O(sqrt(n)), qo'shimcha xotira O(1).

### 2. 6k +/- 1 usuli

2 va 3 ga bo'linmaydigan tub sonlar 6k-1 yoki 6k+1 ko'rinishida bo'lishi mumkin. Shu sababli faqat shu ko'rinishdagi nomzod bo'luvchilar tekshiriladi. Vaqt murakkabligi O(sqrt(n)).

## Dastur imkoniyatlari

- Sonni tub yoki tub emasligini aniqlash.
- Ikki algoritm natijasini solishtirish.
- Har bir algoritm tekshiruvlar sonini ko'rsatish.
- Ishlash vaqtini millisekundlarda o'lchash.
- Topilgan bo'luvchini chiqarish.

## Kompilyatsiya

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o prime_checker
./prime_checker
```

Windowsda bajariladigan fayl nomi prime_checker.exe bo'ladi.

## Avtomatik testlar

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Testlar ikki algoritmni bir xil sonlarda tekshiradi va natijalar mos kelmasa xato kodi qaytaradi.

## Loyiha tuzilishi

- main.cpp — foydalanuvchi interfeysi va vaqt o'lchash.
- prime_checker.h — ikkita deterministik algoritm.
- tests.cpp — algoritmlarni taqqoslovchi testlar.
- CMakeLists.txt — yig'ish va test sozlamalari.
- .github/workflows/cpp.yml — GitHub Actions avtomatik tekshiruvi.
