#pragma once

#include "raylib.h"

// Oyun genelinde kullanılacak temel konfigürasyon ve paylaşılan kaynaklar.

struct GameConfig {
	int screenWidth = 1280;
	int screenHeight = 720;
	const char* windowTitle = "Guardians of the Deep";
};

// UI ve metinler için kullanılacak ana font.
struct FontResources {
	Font uiFont{};
	bool loaded = false;
};

// Uygulama seviyesinde tek bir config ve font kaynağı tutulacak.
GameConfig& GetGameConfig();
FontResources& GetFontResources();

// Fontları yükleyip boşaltmak için yardımcı fonksiyonlar.
// Not: Gerçek font dosyası assets/fonts altında olmalı.
bool LoadUIFont(const char* path, int size);
void UnloadUIFont();

