#pragma once

#include <functional>
#include <vector>

#include "raylib.h"
#include "enemy/EnemyManager.h"
#include "enemy/EnemyTypes.h"

// High-level wrapper that connects Enemy/EnemyManager to the core game loop.
// Core should talk to EnemySystem, not directly to Enemy/EnemyManager.
class EnemySystem
{
public:
    // Called once per frame by the core.
    void Update(float dt);

    // Called once per frame by the core.
    void Draw();

    // Provide the current waypoint path (from Map/Path system).
    void SetWaypoints(const std::vector<Vector2>& newWaypoints);

    // Spawn an enemy (Wave/Spawner will call this later; can be used for demo too).
    void SpawnEnemy(EnemyType type, Vector2 startPos);

    // Core-facing queries:
    // True if at least one enemy reached the end in the current frame.
    bool HasEnemyReachedEnd() const;

    // True if there are no enemies alive (useful for wave completion).
    bool AreAllEnemiesDead() const;

    // Optional: connect reward event to external money system.
    void SetOnReward(std::function<void(int)> cb);

    // Clears enemies and resets internal flags.
    void Reset();
    
    // Get access to enemies for tower targeting
    EnemyManager& GetManager() { return manager; }
    const EnemyManager& GetManager() const { return manager; }

private:
    EnemyManager manager;
    std::vector<Vector2> waypoints;

    // Frame flag: set to true when an enemy reaches the end this frame.
    bool reachedEndThisFrame = false;

    // Reward callback (wired by external money system or demo).
    std::function<void(int)> onReward;
};
