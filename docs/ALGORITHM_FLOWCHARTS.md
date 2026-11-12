# Algoritm blok-sxemalari

## 1. Sinov bo'linish
```mermaid
flowchart TD
 A([Boshlash]) --> B[n ni kiritish]
 B --> C{n < 2?}
 C -- Ha --> N[Tub emas]
 C -- Yo'q --> D{i = 2}
 D --> E{i <= n / i?}
 E -- Yo'q --> P[Tub]
 E -- Ha --> F{n mod i = 0?}
 F -- Ha --> N
 F -- Yo'q --> G[i = i + 1]
 G --> E
 N --> Z([Tugatish])
 P --> Z
```

## 2. 6k +/- 1
```mermaid
flowchart TD
 A([Boshlash]) --> B[n ni kiritish]
 B --> C{n < 2?}
 C -- Ha --> N[Tub emas]
 C -- Yo'q --> D{2 yoki 3 ga bo'linadimi?}
 D -- Ha --> N
 D -- Yo'q --> E[i = 5]
 E --> F{i <= n / i?}
 F -- Yo'q --> P[Tub]
 F -- Ha --> G{n mod i = 0?}
 G -- Ha --> N
 G -- Yo'q --> H{n mod (i+2) = 0?}
 H -- Ha --> N
 H -- Yo'q --> I[i = i + 6]
 I --> F
 N --> Z([Tugatish])
 P --> Z
```

## 3. Miller-Rabin asosiy g'oyasi
```mermaid
flowchart TD
 A([Boshlash]) --> B[n ni kiritish]
 B --> C[n-1 = d * 2^s ko'rinishiga keltirish]
 C --> D[Asos a ni tanlash]
 D --> E[x = a^d mod n]
 E --> F{x = 1 yoki n-1?}
 F -- Ha --> G[Keyingi raund]
 F -- Yo'q --> H[x ni kvadratlash]
 H --> I{x = n-1?}
 I -- Ha --> G
 I -- Yo'q --> J{Raundlar tugadimi?}
 J -- Yo'q --> H
 J -- Ha --> K[Murakkab son]
 G --> L{Barcha raundlar tugadimi?}
 L -- Yo'q --> D
 L -- Ha --> M[Ehtimol tub]
 K --> N([Tugatish])
 M --> N
```
