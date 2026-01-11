#include "wave/WaveManager.h"
#include <cstdlib>  // for rand()
#include <algorithm>

// ============================================================
// Constructor
// ============================================================
WaveManager::WaveManager()
    : currentWave(0)
    , totalWaves(10)
    , waveState(WaveState::WAITING)
    , spawnPoint{0.0f, 0.0f}
    , spawnTimer(0.0f)
    , enemiesRemainingToSpawn(0)
    , activeEnemyCount(0)
    , enemiesSpawnedThisWave(0)
    , waveStartTimer(0.0f)
    , bossWarningTimer(0.0f)
    , autoStartWaves(false)
{
    InitializeWaveConfigs();
}

// ============================================================
// Initialization & Reset
// ============================================================
void WaveManager::Init()
{
    Reset();
    economy.Init(STARTING_GOLD);
}

void WaveManager::Reset()
{
    currentWave = 0;
    waveState = WaveState::WAITING;
    spawnTimer = 0.0f;
    enemiesRemainingToSpawn = 0;
    activeEnemyCount = 0;
    enemiesSpawnedThisWave = 0;
    waveStartTimer = DEFAULT_WAVE_DELAY;
    bossWarningTimer = 0.0f;
    
    economy.Reset();
    economy.Init(STARTING_GOLD);
    
    InitializeWaveConfigs();
}

// ============================================================
// Wave Configuration - Level Design
// ============================================================
void WaveManager::InitializeWaveConfigs()
{
    waveConfigs.clear();
    
    // Wave 1: Introduction - Easy, slow enemies
    // 5 Void Walkers (NORMAL), slow spawn
    waveConfigs.push_back(WaveData(5, 2.0f, EnemyType::NORMAL));
    
    // Wave 2: More enemies, slightly faster spawn
    waveConfigs.push_back(WaveData(7, 1.8f, EnemyType::NORMAL));
    
    // Wave 3: Introduce fast enemies
    // Mix of NORMAL (70%) and FAST (30%)
    waveConfigs.push_back(WaveData(8, 1.5f, EnemyType::NORMAL, EnemyType::FAST, 0.3f));
    
    // Wave 4: Fast enemy focus
    waveConfigs.push_back(WaveData(10, 1.2f, EnemyType::FAST, EnemyType::NORMAL, 0.2f));
    
    // Wave 5: Mid-game challenge - introduce tanks
    waveConfigs.push_back(WaveData(8, 1.5f, EnemyType::NORMAL, EnemyType::TANK, 0.25f));
    
    // Wave 6: Mixed assault
    waveConfigs.push_back(WaveData(12, 1.3f, EnemyType::NORMAL, EnemyType::FAST, 0.4f));
    
    // Wave 7: Tank heavy
    waveConfigs.push_back(WaveData(10, 1.8f, EnemyType::TANK, EnemyType::NORMAL, 0.3f));
    
    // Wave 8: Speed rush
    waveConfigs.push_back(WaveData(15, 0.8f, EnemyType::FAST, EnemyType::NORMAL, 0.2f));
    
    // Wave 9: Pre-boss challenge - everything mixed
    waveConfigs.push_back(WaveData(18, 1.0f, EnemyType::NORMAL, EnemyType::TANK, 0.35f));
    
    // Wave 10: BOSS WAVE - Abyss Lord
    // Spawn some minions then the boss (BOSS type is Abyss Lord)
    WaveData bossWave(6, 2.0f, EnemyType::TANK, EnemyType::FAST, 0.4f, true);
    waveConfigs.push_back(bossWave);
    
    totalWaves = static_cast<int>(waveConfigs.size());
}

WaveData WaveManager::GetWaveConfig(int waveIndex) const
{
    if (waveIndex < 0 || waveIndex >= static_cast<int>(waveConfigs.size())) {
        // Return default safe wave
        return WaveData();
    }
    return waveConfigs[waveIndex];
}

EnemyType WaveManager::DetermineEnemyType(const WaveData& wave)
{
    // Use random ratio to determine enemy type
    float roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    if (roll < wave.secondaryRatio) {
        return wave.secondaryType;
    }
    return wave.primaryType;
}

// ============================================================
// Main Update Loop
// ============================================================
void WaveManager::Update(float dt)
{
    switch (waveState) {
        case WaveState::WAITING:
            // Auto-start countdown (optional)
            if (autoStartWaves && currentWave < totalWaves) {
                waveStartTimer -= dt;
                if (waveStartTimer <= 0.0f) {
                    StartNextWave();
                }
            }
            break;
            
        case WaveState::BOSS_WARNING:
            // Display boss warning before final wave
            bossWarningTimer -= dt;
            if (bossWarningTimer <= 0.0f) {
                // Transition to spawning
                waveState = WaveState::SPAWNING;
                
                WaveData config = GetWaveConfig(currentWave - 1);
                enemiesRemainingToSpawn = config.enemyCount;
                spawnTimer = 0.0f; // Start spawning immediately
                enemiesSpawnedThisWave = 0;
            }
            break;
            
        case WaveState::SPAWNING:
            // Handle enemy spawning
            spawnTimer -= dt;
            
            if (spawnTimer <= 0.0f && enemiesRemainingToSpawn > 0) {
                // Spawn an enemy
                WaveData config = GetWaveConfig(currentWave - 1);
                EnemyType typeToSpawn = DetermineEnemyType(config);
                
                // For boss wave, spawn boss (Abyss Lord) as last enemy
                if (config.isBossWave && enemiesRemainingToSpawn == 1) {
                    typeToSpawn = EnemyType::BOSS; // Abyss Lord
                }
                
                // Trigger spawn callback
                if (onSpawnEnemy) {
                    onSpawnEnemy(typeToSpawn, spawnPoint);
                }
                
                enemiesRemainingToSpawn--;
                enemiesSpawnedThisWave++;
                activeEnemyCount++;
                
                // Reset spawn timer
                spawnTimer = config.spawnInterval;
            }
            
            // Check if all enemies spawned
            if (enemiesRemainingToSpawn <= 0) {
                waveState = WaveState::IN_PROGRESS;
            }
            break;
            
        case WaveState::IN_PROGRESS:
            // Waiting for all enemies to be killed
            if (activeEnemyCount <= 0) {
                // Wave cleared!
                if (onWaveCleared) {
                    onWaveCleared();
                }
                
                // Check for victory
                if (currentWave >= totalWaves) {
                    waveState = WaveState::COMPLETED;
                    if (onAllWavesCompleted) {
                        onAllWavesCompleted();
                    }
                } else {
                    // Prepare for next wave
                    waveState = WaveState::WAITING;
                    waveStartTimer = DEFAULT_WAVE_DELAY;
                }
            }
            break;
            
        case WaveState::COMPLETED:
            // Victory state - do nothing, game should transition
            break;
    }
}

// ============================================================
// Wave Control
// ============================================================
void WaveManager::StartNextWave()
{
    if (!CanStartNextWave()) {
        return;
    }
    
    currentWave++;
    
    WaveData config = GetWaveConfig(currentWave - 1);
    
    // Check for boss wave - show warning first
    if (config.isBossWave) {
        waveState = WaveState::BOSS_WARNING;
        bossWarningTimer = BOSS_WARNING_DURATION;
        
        if (onBossWarning) {
            onBossWarning();
        }
    } else {
        // Normal wave - start spawning immediately
        waveState = WaveState::SPAWNING;
        enemiesRemainingToSpawn = config.enemyCount;
        spawnTimer = 0.0f; // Spawn first enemy immediately
        enemiesSpawnedThisWave = 0;
    }
}

bool WaveManager::CanStartNextWave() const
{
    // Can start if:
    // 1. Not currently in a wave
    // 2. Haven't reached total waves
    // 3. No active enemies (wave must be cleared)
    return waveState == WaveState::WAITING && 
           currentWave < totalWaves && 
           activeEnemyCount <= 0;
}

void WaveManager::ForceStartWave(int waveIndex)
{
    if (waveIndex < 1 || waveIndex > totalWaves) {
        return;
    }
    
    // Reset current state
    activeEnemyCount = 0;
    currentWave = waveIndex - 1; // Will be incremented by StartNextWave
    waveState = WaveState::WAITING;
    
    StartNextWave();
}

// ============================================================
// Configuration
// ============================================================
void WaveManager::SetSpawnPoint(Vector2 point)
{
    spawnPoint = point;
}

void WaveManager::SetOnSpawnEnemy(std::function<void(EnemyType, Vector2)> callback)
{
    onSpawnEnemy = std::move(callback);
}

void WaveManager::SetOnWaveCleared(std::function<void()> callback)
{
    onWaveCleared = std::move(callback);
}

void WaveManager::SetOnAllWavesCompleted(std::function<void()> callback)
{
    onAllWavesCompleted = std::move(callback);
}

void WaveManager::SetOnBossWarning(std::function<void()> callback)
{
    onBossWarning = std::move(callback);
}

// ============================================================
// Enemy Event Handlers
// ============================================================
void WaveManager::OnEnemyKilled(int reward)
{
    // Award gold for killing enemy
    economy.AddGold(reward);
    
    // Decrement active count
    if (activeEnemyCount > 0) {
        activeEnemyCount--;
    }
}

void WaveManager::OnEnemyReachedBase()
{
    // Enemy reached base - decrement counter (handled by game for HP)
    if (activeEnemyCount > 0) {
        activeEnemyCount--;
    }
}

void WaveManager::OnEnemyRemoved()
{
    // Generic removal notification
    if (activeEnemyCount > 0) {
        activeEnemyCount--;
    }
}
