# Tub sonni tekshirish dasturi

**Fan:** Matematik asoslar  
**Mavzu:** Sonlarni tubikka tekshiruvchi deterministik algoritmlarning kompleks dasturiy ta'minotini ishlab chiqish  
**Dasturlash tili:** C++

## Loyiha haqida

Ushbu dastur kiritilgan butun sonning tub yoki tub emasligini deterministik sinov bo'linish algoritmi yordamida aniqlaydi. Natijada sonning holati, topilgan bo'luvchi (agar mavjud bo'lsa), tekshirishlar soni va bajarilish vaqti ko'rsatiladi.

## Algoritm qanday ishlaydi?

1. 2 dan kichik sonlar tub emas.
2. 2 va 3 tub son hisoblanadi.
3. Juft sonlar (2 dan tashqari) tub emas.
4. Qolgan sonlar uchun 3 dan boshlab faqat toq bo'luvchilar tekshiriladi.
5. Tekshirish sonning kvadrat ildizigacha davom etadi. Kodda ko'paytma overflow bo'lmasligi uchun `i <= n / i` sharti ishlatilgan.
6. Bo'luvchi topilmasa, son tub hisoblanadi.

## Algoritm murakkabligi

- **Vaqt murakkabligi:** O(√n)
- **Xotira murakkabligi:** O(1)

## Ishga tushirish

### g++ orqali

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o prime_checker
./prime_checker
```

Windows'da ishga tushirish:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o prime_checker.exe
prime_checker.exe
```

## Namuna

```text
Son kiriting (chiqish uchun q): 29

Kiritilgan son: 29
Xulosa: 29 — tub son.
```

## Sinab ko'rish uchun sonlar

| Kiritish | Kutiladigan natija |
|---:|---|
| 0 | Tub emas |
| 1 | Tub emas |
| 2 | Tub |
| 17 | Tub |
| 25 | Tub emas, bo'luvchi 5 |
| 97 | Tub |
| 100 | Tub emas, bo'luvchi 2 |

## Loyiha tarkibi

- `main.cpp` — dasturning C++ kodi.
- `CMakeLists.txt` — CMake orqali yig'ish sozlamasi.
- `.gitignore` — kompilyatsiya natijalarini Git'dan chiqarib turadi.
