#include "systems/UISystem.h"
#include "ui/HUD.h"
#include "ui/Screens.h"

UISystem::UISystem()
    : currentScreen(UIScreenState::PlayingHUD) {
}

void UISystem::SetScreen(UIScreenState state) {
    currentScreen = state;
}

UIScreenState UISystem::GetScreen() const {
    return currentScreen;
}

void UISystem::SetHUDData(const HUDData& data) {
    hudData = data;
}

void UISystem::Update(float dt) {
    if (story.active) {
        story.timer -= dt;
        if (story.timer <= 0.0f) {
            story.active = false;
            story.timer = 0.0f;
        }
    }
}

void UISystem::ShowStory(const std::string& line1, const std::string& line2, float durationSeconds) {
    story.line1 = line1;
    story.line2 = line2;
    story.duration = durationSeconds;
    story.timer = durationSeconds;
    story.active = true;
}

void UISystem::DrawStoryBanner() {
    if (!story.active) return;

    const float normalized = story.duration > 0.0f ? story.timer / story.duration : 0.0f;
    const float fadeOutWindow = 0.6f;
    float alphaFactor = 1.0f;
    if (story.timer < fadeOutWindow) {
        alphaFactor = story.timer / fadeOutWindow;
    }

    unsigned char bgAlpha = static_cast<unsigned char>(180 * alphaFactor);
    unsigned char textAlpha = static_cast<unsigned char>(255 * alphaFactor);

    Color bg = { 8, 16, 32, bgAlpha };
    Color accent = { 140, 220, 255, textAlpha };
    Color textCol = { 220, 240, 255, textAlpha };

    const int bannerHeight = 140;
    DrawRectangle(0, 0, GetScreenWidth(), bannerHeight, bg);

    FontResources& fonts = GetFontResources();
    const float titleSize = 32.0f;
    const float bodySize = 24.0f;
    const float spacing = 2.0f;

    Vector2 p1 = { 40, 25 };
    DrawTextEx(fonts.uiFont, story.line1.c_str(), p1, titleSize, spacing, accent);

    if (!story.line2.empty()) {
        Vector2 p2 = { 40, 70 };
        DrawTextEx(fonts.uiFont, story.line2.c_str(), p2, bodySize, spacing, textCol);
    }
}

void UISystem::Draw() {
    FontResources& fonts = GetFontResources();
    
    // Use default font if custom font not loaded
    Font fontToUse = fonts.loaded ? fonts.uiFont : GetFontDefault();

    switch (currentScreen) {
    case UIScreenState::Start:
        UIScreens::DrawStart(fontToUse);
        break;
    case UIScreenState::Paused:
        UIScreens::DrawPaused(fontToUse);
        break;
    case UIScreenState::GameOver:
        UIScreens::DrawGameOver(fontToUse);
        break;
    case UIScreenState::Victory:
        UIScreens::DrawVictory(fontToUse);
        break;
    case UIScreenState::PlayingHUD:
        HUD::Draw(fontToUse, hudData.hp, hudData.money, hudData.currentWave, hudData.totalWaves);
        DrawStoryBanner();
        break;
    case UIScreenState::None:
    default:
        break;
    }
}
