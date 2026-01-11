#pragma once

#include "raylib.h"
#include "enemy/EnemyTypes.h"

#include <vector>
#include <functional>

// ============================================================
// SpawnRequest: A queued spawn request
// ============================================================
struct SpawnRequest {
    EnemyType type;
    Vector2 position;
    float delay;      // Delay before spawning (seconds)
    
    SpawnRequest(EnemyType t, Vector2 pos, float d = 0.0f)
        : type(t), position(pos), delay(d) {}
};

// ============================================================
// Spawner: Helper class for managing spawn queues
// ============================================================
// This class can be used independently or by WaveManager for
// more complex spawn patterns (burst spawns, delayed spawns, etc.)
// ============================================================

class Spawner {
public:
    Spawner();
    
    // -------------------- Core Methods --------------------
    
    // Update spawner (processes queue)
    void Update(float dt);
    
    // Clear all pending spawns
    void Clear();
    
    // -------------------- Spawn Control --------------------
    
    // Queue a single spawn
    void QueueSpawn(EnemyType type, Vector2 position, float delay = 0.0f);
    
    // Queue multiple spawns of the same type
    void QueueBurst(EnemyType type, Vector2 position, int count, float intervalBetween);
    
    // Queue a wave pattern (mixed types with intervals)
    void QueueWavePattern(const std::vector<SpawnRequest>& pattern);
    
    // -------------------- Configuration --------------------
    
    // Set callback for when an enemy should be spawned
    void SetOnSpawn(std::function<void(EnemyType, Vector2)> callback);
    
    // -------------------- Getters --------------------
    
    int GetQueuedCount() const { return static_cast<int>(spawnQueue.size()); }
    bool IsEmpty() const { return spawnQueue.empty(); }
    
private:
    std::vector<SpawnRequest> spawnQueue;
    std::function<void(EnemyType, Vector2)> onSpawn;
};

// ============================================================
// Spawn Pattern Helpers (Free Functions)
// ============================================================
namespace SpawnPatterns {
    // Create a line pattern (enemies spawn in sequence from a point)
    std::vector<SpawnRequest> CreateLinePattern(
        Vector2 startPos, 
        EnemyType type, 
        int count, 
        float interval
    );
    
    // Create a burst pattern (multiple enemies at once)
    std::vector<SpawnRequest> CreateBurstPattern(
        Vector2 position,
        EnemyType type,
        int count
    );
    
    // Create a mixed pattern (alternating enemy types)
    std::vector<SpawnRequest> CreateMixedPattern(
        Vector2 position,
        EnemyType type1,
        EnemyType type2,
        int countEach,
        float interval
    );
}
