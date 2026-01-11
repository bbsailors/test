#pragma once

#include "raylib.h"
#include "core/GameConfig.h"
#include <string>

// UI katmanının sorumlulukları:
// - HUD çizimi (HP, Para, Dalga)
// - Menü / Game Over / Victory ekranları
// - Hikaye / dalga uyarı bantları

enum class UIScreenState {
	None,
	Start,
	PlayingHUD,
	Paused,
	GameOver,
	Victory
};

struct HUDData {
	int hp = 0;
	int money = 0;
	int currentWave = 0;
	int totalWaves = 0;
};

struct StoryBanner {
	std::string line1;
	std::string line2;
	float timer = 0.0f;
	float duration = 0.0f;
	bool active = false;
};

class UISystem {
public:
	UISystem();

	void SetScreen(UIScreenState state);
	UIScreenState GetScreen() const;

	void SetHUDData(const HUDData& data);

	// Her frame çağrılmalı; zamanlayıcıları günceller.
	void Update(float dt);

	// Kısa hikaye / uyarı mesajı göster.
	void ShowStory(const std::string& line1, const std::string& line2, float durationSeconds);

	// Her frame en sonda çağrılmalı; uygun UI ekranını çizer.
	void Draw();

private:
	void DrawStoryBanner();

	UIScreenState currentScreen;
	HUDData hudData;
	StoryBanner story;
};

