# Himoya uchun qo'llanma

## 1. Loyiha mavzusi
Tub sonlarni aniqlash algoritmlarini C++17 tilida ishlab chiqish va ularning samaradorligini taqqoslash.

## 2. Maqsad va vazifalar
**Maqsad:** tub sonlarni aniqlashning turli matematik usullarini dasturiy amalga oshirish, natijalarini tekshirish va ishlash samaradorligini tajriba orqali baholash.

**Vazifalar:**
1. Tub son va murakkab son tushunchalarini o'rganish.
2. Sinov bo'linish, 6k +/- 1, Fermat va Miller-Rabin algoritmlarini amalga oshirish.
3. 64-bit diapazon uchun deterministik Miller-Rabin qo'shish.
4. Avtomatik testlar bilan to'g'rilikni tekshirish.
5. Benchmark natijalarini CSV formatida yig'ish.
6. Nazariy murakkablik va tajriba natijalarini taqqoslash.

## 3. Asosiy matematik tushunchalar
- **Tub son:** 1 dan katta va faqat 1 hamda o'ziga bo'linadigan natural son.
- **Sinov bo'linish:** agar n murakkab bo'lsa, uning kamida bitta tub bo'luvchisi sqrt(n) dan katta emas.
- **Fermat kichik teoremasi:** tub p va p ga bo'linmaydigan a uchun a^(p-1) ≡ 1 (mod p). Teskari xulosa har doim to'g'ri emas.
- **Miller-Rabin:** n-1 ni d*2^s ko'rinishida yozib, modulyar darajalar orqali murakkablik guvohini izlaydi.
- **Modulyar darajaga oshirish:** tez darajaga oshirish usuli yordamida katta darajalar qoldiq bilan hisoblanadi.

## 4. Murakkabliklar
| Algoritm | Vaqt murakkabligi | Xulosa turi |
|---|---:|---|
| Oddiy sinov bo'linish | O(sqrt(n)) | Deterministik |
| 6k +/- 1 | O(sqrt(n)) | Deterministik |
| Fermat | O(k log n * M(log n)) | Ehtimollik |
| Miller-Rabin | O(k log n * M(log n)) | Ehtimollik |
| Deterministik Miller-Rabin 64-bit | O(log n * M(log n)) | Deterministik |

Bu yerda k — raundlar soni, M(log n) — modulyar ko'paytirish xarajati. Amaldagi kodda overflow xavfsizligi uchun modulyar ko'paytirish qo'shishlar orqali bajariladi.

## 5. Tajriba metodikasi
1. Bir xil kompilyator va optimallashtirish sozlamalaridan foydalanish.
2. Har bir sonni har bir algoritmda tekshirish.
3. Juda tez ishlaydigan algoritmlar uchun o'lchovni bir necha marta takrorlash va medianani olish.
4. CSV natijalarini Excelga import qilish.
5. Alohida grafiklarda ishlash vaqtini va bo'luvchi tekshiruvlari sonini ko'rsatish.
6. Qurilma, operatsion tizim, kompilyator versiyasi va build rejimini hisobotda qayd etish.

**Muhim:** `benchmark.cpp` juda katta qiymatlarda sinov bo'linish usullarini uzoq kutib qolmaslik uchun 10^12 dan yuqori sonlarda o'tkazib yuboradi. Bu holat CSVda `skipped_large_input` deb belgilanadi. Deterministik Miller-Rabin esa 64-bit sonlarni tekshirishda davom etadi.

## 6. Himoyada berilishi mumkin bo'lgan savollar
**Savol: 1 tub sonmi?**  
Javob: Yo'q. Tub son ta'rifiga ko'ra u 1 dan katta bo'lishi kerak.

**Savol: Nega sinov bo'linishda sqrt(n) gacha tekshiriladi?**  
Javob: Agar n=a*b bo'lsa va ikkala ko'paytuvchi ham sqrt(n) dan katta bo'lsa, ularning ko'paytmasi n dan katta bo'lib ketadi. Demak, kamida bittasi sqrt(n) dan kichik yoki teng.

**Savol: 6k +/- 1 nimaga asoslangan?**  
Javob: 2 va 3 ga bo'linmaydigan har qanday tub son 6 ga bo'lganda 1 yoki 5 qoldiq beradi. Bu barcha 6k +/- 1 sonlar tub degani emas; ular faqat tekshiriladigan nomzodlardir.

**Savol: Fermat testi doim to'g'ri javob beradimi?**  
Javob: Yo'q. Carmichael sonlari kabi murakkab sonlar ayrim asoslar uchun Fermat shartini qanoatlantirishi mumkin.

**Savol: Miller-Rabin bilan deterministik Miller-Rabin farqi nima?**  
Javob: Tasodifiy asosli versiya ehtimollik xulosasini beradi. Maxsus tanlangan asoslar to'plami esa butun unsigned 64-bit diapazonida deterministik natija beradi.

**Savol: Benchmarkdagi vaqt nima uchun o'zgaradi?**  
Javob: Protsessor, operatsion tizim, kompilyator optimallashtirishi, fon jarayonlari va vaqt o'lchash aniqligi ta'sir qiladi.

## 7. Taqdimot rejasi
1. Muammo va loyiha maqsadi.
2. Tub sonlarning matematik ta'rifi.
3. Beshta algoritmning ishlash prinsipi.
4. Dastur arxitekturasi va fayllar.
5. Dasturni jonli namoyish qilish.
6. Avtomatik testlar natijasi.
7. Benchmark jadvali va diagrammalar.
8. Xulosa, cheklovlar va kelgusi rivojlantirish.

## 8. Xulosa namunasi
Ushbu loyiha tub sonlarni tekshirishning bir nechta algoritmini C++17 tilida amalga oshiradi. Deterministik sinov bo'linish usullari tushunarli va kichik qiymatlar uchun qulay, Miller-Rabin oilasi esa katta sonlarni tezroq tekshirish imkonini beradi. Tajribalar algoritmlarning nazariy murakkabligini amaliy natijalar bilan taqqoslashga yordam beradi. Yakuniy xulosa faqat haqiqiy test va benchmark natijalari asosida yozilishi lozim.
