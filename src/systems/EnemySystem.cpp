#include "systems/EnemySystem.h"

// Sets the waypoint path used by all enemies.
void EnemySystem::SetWaypoints(const std::vector<Vector2>& newWaypoints)
{
    waypoints = newWaypoints;
}

// Connects the reward callback to an external system (money, demo, etc.)
void EnemySystem::SetOnReward(std::function<void(int)> cb)
{
    onReward = std::move(cb);
}

// Spawns a new enemy and wires its events.
void EnemySystem::SpawnEnemy(EnemyType type, Vector2 startPos)
{
    Enemy e(type, startPos);

    // Triggered when the enemy reaches the final waypoint (base/vault).
    e.onReachedEnd = [this]() {
        reachedEndThisFrame = true;
        };

    // Triggered when the enemy dies and gives a reward.
    e.onDeath = [this](int reward) {
        if (onReward) {
            onReward(reward);
        }
        };

    manager.AddEnemy(e);
}

// Updates all enemies and resets per-frame flags.
void EnemySystem::Update(float dt)
{
    // Reset frame-based flag.
    reachedEndThisFrame = false;

    // Update enemy manager (Enemy::Update handles empty waypoint list safely).
    manager.Update(dt, waypoints);
}

// Draws all enemies.
void EnemySystem::Draw()
{
    manager.Draw();
}

// Returns true if at least one enemy reached the end in this frame.
bool EnemySystem::HasEnemyReachedEnd() const
{
    return reachedEndThisFrame;
}

// Returns true if there are no enemies alive.
bool EnemySystem::AreAllEnemiesDead() const
{
    return manager.IsEmpty();
}

// Clears all enemies and resets internal state.
void EnemySystem::Reset()
{
    manager.Clear();
    reachedEndThisFrame = false;
}
