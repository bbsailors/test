#pragma once

#include "wave/WaveManager.h"

// ============================================================
// WaveSystem (systems folder)
// ============================================================
// System-level wrapper that the Game class uses.
// Delegates to WaveManager for actual implementation.
// ============================================================

class WaveSystem
{
public:
    WaveSystem() = default;
    
    // Called every frame by the game loop
    void Update(float dt);
    
    // Initialize the wave system
    void Init();
    
    // Reset for new game
    void Reset();
    
    // Start next wave (player triggered)
    void StartNextWave();
    
    // Check if player beat all waves
    bool IsLastWaveFinished() const;
    
    // Check if victory achieved
    bool IsVictory() const;
    
    // -------------------- Wave Manager Access --------------------
    
    WaveManager& GetWaveManager() { return waveManager; }
    const WaveManager& GetWaveManager() const { return waveManager; }
    
    // -------------------- Convenience Getters --------------------
    
    int GetCurrentWave() const { return waveManager.GetCurrentWave(); }
    int GetTotalWaves() const { return waveManager.GetTotalWaves(); }
    int GetGold() const { return waveManager.GetGold(); }
    bool CanAfford(int cost) const { return waveManager.CanAfford(cost); }
    
private:
    WaveManager waveManager;
};
