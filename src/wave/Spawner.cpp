#include "wave/Spawner.h"
#include <algorithm>

// ============================================================
// Spawner Implementation
// ============================================================

Spawner::Spawner()
{
}

void Spawner::Update(float dt)
{
    if (spawnQueue.empty()) {
        return;
    }
    
    // Process queue - update delays and spawn ready enemies
    // We iterate backwards to safely remove elements
    for (int i = static_cast<int>(spawnQueue.size()) - 1; i >= 0; --i) {
        spawnQueue[i].delay -= dt;
        
        if (spawnQueue[i].delay <= 0.0f) {
            // Spawn this enemy
            if (onSpawn) {
                onSpawn(spawnQueue[i].type, spawnQueue[i].position);
            }
            
            // Remove from queue
            spawnQueue.erase(spawnQueue.begin() + i);
        }
    }
}

void Spawner::Clear()
{
    spawnQueue.clear();
}

void Spawner::QueueSpawn(EnemyType type, Vector2 position, float delay)
{
    spawnQueue.emplace_back(type, position, delay);
}

void Spawner::QueueBurst(EnemyType type, Vector2 position, int count, float intervalBetween)
{
    for (int i = 0; i < count; ++i) {
        float delay = static_cast<float>(i) * intervalBetween;
        spawnQueue.emplace_back(type, position, delay);
    }
}

void Spawner::QueueWavePattern(const std::vector<SpawnRequest>& pattern)
{
    for (const auto& request : pattern) {
        spawnQueue.push_back(request);
    }
}

void Spawner::SetOnSpawn(std::function<void(EnemyType, Vector2)> callback)
{
    onSpawn = std::move(callback);
}

// ============================================================
// Spawn Pattern Helpers
// ============================================================

namespace SpawnPatterns {

std::vector<SpawnRequest> CreateLinePattern(
    Vector2 startPos, 
    EnemyType type, 
    int count, 
    float interval)
{
    std::vector<SpawnRequest> pattern;
    pattern.reserve(count);
    
    for (int i = 0; i < count; ++i) {
        float delay = static_cast<float>(i) * interval;
        pattern.emplace_back(type, startPos, delay);
    }
    
    return pattern;
}

std::vector<SpawnRequest> CreateBurstPattern(
    Vector2 position,
    EnemyType type,
    int count)
{
    std::vector<SpawnRequest> pattern;
    pattern.reserve(count);
    
    // All spawn at the same time (delay = 0)
    for (int i = 0; i < count; ++i) {
        pattern.emplace_back(type, position, 0.0f);
    }
    
    return pattern;
}

std::vector<SpawnRequest> CreateMixedPattern(
    Vector2 position,
    EnemyType type1,
    EnemyType type2,
    int countEach,
    float interval)
{
    std::vector<SpawnRequest> pattern;
    pattern.reserve(countEach * 2);
    
    float currentDelay = 0.0f;
    
    for (int i = 0; i < countEach; ++i) {
        // Alternate between types
        pattern.emplace_back(type1, position, currentDelay);
        currentDelay += interval;
        
        pattern.emplace_back(type2, position, currentDelay);
        currentDelay += interval;
    }
    
    return pattern;
}

} // namespace SpawnPatterns
