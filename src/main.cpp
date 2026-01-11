#include "raylib.h"
#include "core/GameConfig.h"
#include "systems/UISystem.h"
#include "core/Game.h"


int main() {

    Game game;
    game.Run();
    return 0;

    GameConfig& config = GetGameConfig();

    InitWindow(config.screenWidth, config.screenHeight, config.windowTitle);
    SetTargetFPS(60);

    // UI fontunu yükle (gerçek dosya yolu assets/fonts altına eklenmeli)
    // Örnek: assets/fonts/atari-classic.ttf
    LoadUIFont("assets/fonts/default.ttf", 32);

    UISystem uiSystem;

    // Şimdilik dummy değerler; diğer kişiler kendi sistemlerini bağladığında güncellenecek.
    HUDData hud{};
    hud.hp = 20;          // Aria/base HP
    hud.money = 100;      // Başlangıç altın
    hud.currentWave = 1;  // İlk dalga
    hud.totalWaves = 10;  // Planlanan toplam dalga

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // TODO: Game, Enemy, Wave vb. sistem güncellemeleri (Kişi 1,3,5)

        // Örnek olarak zamanla dalga göstergesini güncelleyen küçük bir animasyon
        // (sadece HUD'ın çalıştığını görmek için)
        if (IsKeyPressed(KEY_UP)) {
            if (hud.currentWave < hud.totalWaves) {
                hud.currentWave++;
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            if (hud.currentWave > 1) {
                hud.currentWave--;
            }
        }

        uiSystem.SetHUDData(hud);

        BeginDrawing();
        ClearBackground(DARKBLUE);

        // TODO: Harita, düşmanlar, kuleler vs. çizimleri

        // En sonda UI katmanı
        uiSystem.Draw();

        EndDrawing();
    }

    UnloadUIFont();
    CloseWindow();
    return 0;
}

