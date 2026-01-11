#pragma once

#include "raylib.h"
#include "EnemyTypes.h"

#include <functional>
#include <vector>

// Represents a single enemy instance (data + behavior).
struct Enemy {
    // Core stats
    float hp = 0.0f;
    float maxHp = 0.0f;
    float speed = 0.0f;
    EnemyType type = EnemyType::NORMAL;
    int reward = 0;

    // Movement/path tracking
    Vector2 position{ 0, 0 };
    int currentWaypointIndex = 0;

    // State flags
    bool alive = true;
    bool rewardGiven = false;

    // Events/callbacks (wired by a higher-level system)
    std::function<void(int)> onDeath;    // Called once when enemy dies (passes reward)
    std::function<void()> onReachedEnd;  // Called once when enemy reaches final waypoint

    // Rendering (simple shapes for now)
    float radius = 12.0f;
    Color color = RED;

    Enemy() = default;
    Enemy(EnemyType type, Vector2 startPos);

    // Apply damage. If hp reaches 0, triggers death event once.
    void TakeDamage(float damage);

    // Draw enemy and optionally an HP bar.
    void Draw(bool drawHpBar = true) const;

    // Move along the waypoint path using dt.
    // Returns true if the enemy reached the end this update.
    bool Update(float dt, const std::vector<Vector2>& waypoints);
};
