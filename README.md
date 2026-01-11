# Guardians of the Deep

Basit bir 2D tower defense oyunu. Oyuncu, deniz krallığı **Aqualis**'i Voidborn dalgalarına karşı savunma kuleleri kurarak Prenses Aria'yı korumaya çalışır.

## Kurulum ve Çalıştırma

Bu proje CMake ve raylib kullanır.

### Gereksinimler
- CMake 3.11 veya üstü
- C++17 destekleyen bir derleyici (MSVC, clang, gcc vb.)

### Derleme Adımları (Örnek - Windows)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Derleme tamamlandığında çalıştırılabilir dosya `build/GuardiansOfTheDeep` olarak oluşur.

## Klasör Yapısı

- `assets/` : Görseller, sesler, fontlar.
- `docs/`   : GDD, genel dokümantasyon ve krediler.
- `include/`: Header dosyaları (core, systems, ui, map, enemy, tower, wave, utils).
- `src/`    : Oyun kaynak kodu.

Detaylı tasarım için `docs/GDD.md` dosyasına bakılabilir.

