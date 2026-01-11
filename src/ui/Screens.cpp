#include "ui/Screens.h"
#include "raylib.h"

namespace {
    Vector2 CenteredPosition(const Font& font, const char* text, float fontSize, float spacing, float y) {
        Vector2 size = MeasureTextEx(font, text, fontSize, spacing);
        return { (float)GetScreenWidth() * 0.5f - size.x * 0.5f, y };
    }

    void DrawOverlay(Color color) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), color);
    }

    void DrawCenteredText(const Font& font, const char* text, float fontSize, float spacing, float y, Color color) {
        Vector2 pos = CenteredPosition(font, text, fontSize, spacing, y);
        DrawTextEx(font, text, pos, fontSize, spacing, color);
    }
}

namespace UIScreens {

void DrawStart(const Font& font) {
    DrawOverlay({ 5, 10, 30, 220 });

    const float titleSize = 64.0f;
    const float bodySize = 28.0f;
    const float spacing = 2.0f;

    DrawCenteredText(font, "Guardians of the Deep", titleSize, spacing, GetScreenHeight() * 0.28f, { 140, 220, 255, 255 });
    DrawCenteredText(font, "Derinliklerden gelen karanlik, Aqualis'i tehdit ediyor...", bodySize, spacing, GetScreenHeight() * 0.45f, { 220, 235, 255, 255 });
    DrawCenteredText(font, "ENTER ile savunmaya basla", bodySize, spacing, GetScreenHeight() * 0.60f, { 255, 255, 255, 255 });
    DrawCenteredText(font, "ESC ile cik", bodySize, spacing, GetScreenHeight() * 0.68f, { 200, 220, 240, 255 });
}

void DrawPaused(const Font& font) {
    DrawOverlay({ 0, 0, 0, 180 });

    const float titleSize = 52.0f;
    const float bodySize = 26.0f;
    const float spacing = 2.0f;

    DrawCenteredText(font, "PAUSED", titleSize, spacing, GetScreenHeight() * 0.35f, { 255, 255, 255, 255 });
    DrawCenteredText(font, "P ile devam et", bodySize, spacing, GetScreenHeight() * 0.48f, { 200, 220, 240, 255 });
    DrawCenteredText(font, "ESC ile ana menuyu ac", bodySize, spacing, GetScreenHeight() * 0.56f, { 200, 220, 240, 255 });
}

void DrawGameOver(const Font& font) {
    DrawOverlay({ 20, 0, 0, 200 });

    const float titleSize = 58.0f;
    const float bodySize = 26.0f;
    const float spacing = 2.0f;

    DrawCenteredText(font, "Aria dustu, krallik karanliga gomuldu", titleSize, spacing, GetScreenHeight() * 0.35f, { 255, 200, 200, 255 });
    DrawCenteredText(font, "R ile yeniden dene", bodySize, spacing, GetScreenHeight() * 0.50f, { 230, 230, 230, 255 });
    DrawCenteredText(font, "ESC ile cik", bodySize, spacing, GetScreenHeight() * 0.58f, { 210, 210, 210, 255 });
}

void DrawVictory(const Font& font) {
    DrawOverlay({ 0, 40, 20, 190 });

    const float titleSize = 58.0f;
    const float bodySize = 26.0f;
    const float spacing = 2.0f;

    DrawCenteredText(font, "Aqualis kurtarildi!", titleSize, spacing, GetScreenHeight() * 0.35f, { 190, 255, 220, 255 });
    DrawCenteredText(font, "Prenses Aria guvende, okyanus yeniden nefes aldi.", bodySize, spacing, GetScreenHeight() * 0.48f, { 230, 245, 240, 255 });
    DrawCenteredText(font, "R ile tekrar oyna", bodySize, spacing, GetScreenHeight() * 0.58f, { 220, 235, 230, 255 });
}

} // namespace UIScreens
