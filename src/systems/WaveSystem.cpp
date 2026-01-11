#include "systems/WaveSystem.h"

void WaveSystem::Init() {
    waveManager.Init();
}

void WaveSystem::Update(float dt) {
    waveManager.Update(dt);
}

void WaveSystem::Reset() {
    waveManager.Reset();
}

void WaveSystem::StartNextWave() {
    waveManager.StartNextWave();
}

bool WaveSystem::IsLastWaveFinished() const {
    return waveManager.IsLastWave() && waveManager.GetActiveEnemyCount() == 0;
}

bool WaveSystem::IsVictory() const {
    return waveManager.IsVictory();
}
