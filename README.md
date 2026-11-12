# Tub sonni tekshirish dasturi

**Fan:** Matematik asoslar  
**Mavzu:** Sonlarni tubikka tekshiruvchi deterministik algoritmlarning kompleks dasturiy ta'minotini ishlab chiqish  
**Dasturlash tili:** C++

## Loyiha haqida

Dastur kiritilgan butun sonning tub yoki tub emasligini deterministik sinov bo'linish algoritmi yordamida aniqlaydi. Natijada sonning holati, topilgan bo'luvchi (agar mavjud bo'lsa), tekshirishlar soni va bajarilish vaqti ko'rsatiladi.

## Algoritm

1. 2 dan kichik sonlar tub emas.
2. 2 va 3 tub son hisoblanadi.
3. 2 dan boshqa juft sonlar tub emas.
4. Toq sonlar uchun 3 dan boshlab faqat toq bo'luvchilar tekshiriladi.
5. Tekshirish i <= n / i sharti bilan davom etadi; bu i * i ko'paytmasidagi overflow xavfini oldini oladi.
6. Bo'luvchi topilmasa, son tub hisoblanadi.

## Algoritm murakkabligi

- Vaqt: **O(√n)**.
- Qo'shimcha xotira: **O(1)**.

## Kompilyatsiya va ishga tushirish

### Faqat asosiy dasturni g++ bilan

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o prime_checker
./prime_checker
```

Windows:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o prime_checker.exe
prime_checker.exe
```

### CMake va avtomatik testlar

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Sinov holatlari

| Son | Kutiladigan natija |
|---:|---|
| 0, 1 | Tub emas |
| 2, 3, 17, 97 | Tub |
| 4, 25, 100 | Tub emas |
| 49 | Tub emas, bo'luvchi 7 |

## Loyiha fayllari

- main.cpp — foydalanuvchi bilan ishlaydigan asosiy dastur.
- prime_checker.h — tub sonni aniqlash algoritmi.
- tests.cpp — avtomatik testlar.
- CMakeLists.txt — yig'ish va test sozlamalari.
- .gitignore — kompilyatsiya fayllarini Git'dan chiqaradi.
