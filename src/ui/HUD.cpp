#include "ui/HUD.h"
#include "raylib.h"


void HUD::Draw(Font font, int hp, int money, int currentWave, int totalWaves) {
    const float fontSize = 32.0f;
    const float spacing = 2.0f;

    Color hpColor = { 230, 41, 55, 255 };    // kırmızı ton
    Color goldColor = { 255, 203, 0, 255 };  // altın sarısı
    Color waveColor = { 100, 200, 255, 255 }; // açık mavi

    // HP (sol üst)
    DrawTextEx(font, TextFormat("HP: %d", hp), Vector2{ 20, 20 }, fontSize, spacing, hpColor);


    // GOLD (sol üst, HP'nin altında)
    DrawTextEx(font, TextFormat("GOLD: %d", money), Vector2{ 20, 60 }, fontSize, spacing, goldColor);


    // WAVE X/Y (sağ üst)
    const char* waveText = totalWaves > 0
        ? TextFormat("WAVE %d/%d", currentWave, totalWaves)
        : TextFormat("WAVE %d", currentWave);

    Vector2 textSize = MeasureTextEx(font, waveText, fontSize, spacing);

    DrawTextEx(
        font,
        waveText,
        Vector2{ (float)GetScreenWidth() - textSize.x - 20, 20 },
        fontSize,
        spacing,
        waveColor
    );
}
