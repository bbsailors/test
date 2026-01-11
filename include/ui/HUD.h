#pragma once
#include "raylib.h"

// Basit HUD çizim fonksiyonları.
// UISystem, bu statik yardımcıyı kullanarak gerçek verileri ekrana basar.

class HUD {
public:
    static void Draw(Font font, int hp, int money, int currentWave, int totalWaves);
};