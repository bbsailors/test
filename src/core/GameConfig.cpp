#include "core/GameConfig.h"

static GameConfig g_config;
static FontResources g_fontResources;

GameConfig& GetGameConfig() {
	return g_config;
}

FontResources& GetFontResources() {
	return g_fontResources;
}

bool LoadUIFont(const char* path, int size) {
	if (g_fontResources.loaded) {
		return true;
	}

	g_fontResources.uiFont = LoadFontEx(path, size, nullptr, 0);
	g_fontResources.loaded = (g_fontResources.uiFont.baseSize > 0);
	return g_fontResources.loaded;
}

void UnloadUIFont() {
	if (!g_fontResources.loaded) return;

	UnloadFont(g_fontResources.uiFont);
	g_fontResources.loaded = false;
}

