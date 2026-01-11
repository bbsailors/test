#pragma once

#include "raylib.h"
#include "enemy/EnemyTypes.h"
#include "EconomySystem.h"

#include <vector>
#include <functional>

// ============================================================
// WaveData: Defines the structure of a single wave
// ============================================================
struct WaveData {
    int enemyCount;           // Total enemies in this wave
    float spawnInterval;      // Time between spawns (seconds)
    EnemyType primaryType;    // Main enemy type for this wave
    EnemyType secondaryType;  // Secondary enemy type (optional mixing)
    float secondaryRatio;     // Ratio of secondary type (0.0 - 1.0)
    bool isBossWave;          // True if this is the final boss wave
    
    WaveData()
        : enemyCount(5)
        , spawnInterval(1.5f)
        , primaryType(EnemyType::NORMAL)
        , secondaryType(EnemyType::NORMAL)
        , secondaryRatio(0.0f)
        , isBossWave(false)
    {}
    
    WaveData(int count, float interval, EnemyType primary, 
             EnemyType secondary = EnemyType::NORMAL, 
             float secRatio = 0.0f, bool boss = false)
        : enemyCount(count)
        , spawnInterval(interval)
        , primaryType(primary)
        , secondaryType(secondary)
        , secondaryRatio(secRatio)
        , isBossWave(boss)
    {}
};

// ============================================================
// WaveState: Current state of wave progression
// ============================================================
enum class WaveState {
    WAITING,        // Waiting for player to start or between waves
    SPAWNING,       // Currently spawning enemies
    IN_PROGRESS,    // All enemies spawned, waiting for them to be killed
    COMPLETED,      // All waves completed (victory condition)
    BOSS_WARNING    // Displaying boss warning before final wave
};

// ============================================================
// WaveManager: Manages wave progression, spawning, and economy
// ============================================================
class WaveManager {
public:
    WaveManager();
    
    // -------------------- Core Methods --------------------
    
    // Called every frame by the game loop
    void Update(float dt);
    
    // Initialize/reset the wave system
    void Init();
    void Reset();
    
    // -------------------- Wave Control --------------------
    
    // Start the next wave (player triggered or auto)
    void StartNextWave();
    
    // Check if ready to start next wave
    bool CanStartNextWave() const;
    
    // Force start (for testing/debug)
    void ForceStartWave(int waveIndex);
    
    // -------------------- Spawn Configuration --------------------
    
    // Set the spawn point for enemies (start of path)
    void SetSpawnPoint(Vector2 point);
    
    // Callback when an enemy should be spawned
    // Parameters: EnemyType, Vector2 spawnPosition
    void SetOnSpawnEnemy(std::function<void(EnemyType, Vector2)> callback);
    
    // Callback when all enemies in current wave are dead
    void SetOnWaveCleared(std::function<void()> callback);
    
    // Callback when all waves are completed (victory)
    void SetOnAllWavesCompleted(std::function<void()> callback);
    
    // Callback for boss warning display
    void SetOnBossWarning(std::function<void()> callback);
    
    // -------------------- Enemy Death Notification --------------------
    
    // Called by EnemySystem when an enemy dies (awards gold)
    void OnEnemyKilled(int reward);
    
    // Called by EnemySystem when an enemy reaches the base
    void OnEnemyReachedBase();
    
    // Notify that an enemy was removed (for tracking active count)
    void OnEnemyRemoved();
    
    // -------------------- Getters --------------------
    
    int GetCurrentWave() const { return currentWave; }
    int GetTotalWaves() const { return totalWaves; }
    WaveState GetWaveState() const { return waveState; }
    bool IsWaveActive() const { return waveState == WaveState::SPAWNING || waveState == WaveState::IN_PROGRESS; }
    bool IsLastWave() const { return currentWave >= totalWaves; }
    bool IsVictory() const { return waveState == WaveState::COMPLETED; }
    
    // Time until next wave auto-starts (if applicable)
    float GetTimeUntilNextWave() const { return waveStartTimer; }
    
    // Get enemies remaining to spawn in current wave
    int GetEnemiesRemainingToSpawn() const { return enemiesRemainingToSpawn; }
    
    // Get active enemy count
    int GetActiveEnemyCount() const { return activeEnemyCount; }
    
    // -------------------- Economy Access --------------------
    
    EconomySystem& GetEconomy() { return economy; }
    const EconomySystem& GetEconomy() const { return economy; }
    
    // Convenience methods for economy
    int GetGold() const { return economy.GetGold(); }
    bool CanAfford(int cost) const { return economy.CanAfford(cost); }
    bool SpendGold(int amount) { return economy.SpendGold(amount); }
    void AddGold(int amount) { economy.AddGold(amount); }
    
private:
    // -------------------- Wave Configuration --------------------
    
    void InitializeWaveConfigs();
    WaveData GetWaveConfig(int waveIndex) const;
    EnemyType DetermineEnemyType(const WaveData& wave);
    
    // -------------------- Internal State --------------------
    
    EconomySystem economy;
    
    int currentWave;          // Current wave number (1-indexed for display)
    int totalWaves;           // Total number of waves
    WaveState waveState;
    
    // Spawning state
    Vector2 spawnPoint;
    float spawnTimer;         // Timer for next spawn
    int enemiesRemainingToSpawn;
    int activeEnemyCount;     // Enemies currently alive on the map
    int enemiesSpawnedThisWave;
    
    // Wave transition
    float waveStartTimer;     // Countdown to auto-start next wave
    float bossWarningTimer;   // Timer for boss warning display
    bool autoStartWaves;      // Whether waves auto-start
    
    // Wave configurations
    std::vector<WaveData> waveConfigs;
    
    // Callbacks
    std::function<void(EnemyType, Vector2)> onSpawnEnemy;
    std::function<void()> onWaveCleared;
    std::function<void()> onAllWavesCompleted;
    std::function<void()> onBossWarning;
    
    // Constants
    static constexpr float DEFAULT_WAVE_DELAY = 5.0f;      // Seconds between waves
    static constexpr float BOSS_WARNING_DURATION = 3.0f;   // Boss warning display time
    static constexpr int STARTING_GOLD = 100;              // Initial gold
};
