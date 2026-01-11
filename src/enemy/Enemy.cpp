#include "enemy/Enemy.h"

#include <algorithm>
#include <cmath>

Enemy::Enemy(EnemyType t, Vector2 startPos)
{
    type = t;
    position = startPos;

    alive = true;
    rewardGiven = false;
    currentWaypointIndex = 0;

    // Initialize per-type stats (tweak values freely later)
    switch (type) {
    case EnemyType::NORMAL:
        maxHp = hp = 100.0f;
        speed = 80.0f;
        reward = 10;
        radius = 12.0f;
        color = RED;
        break;

    case EnemyType::FAST:
        maxHp = hp = 60.0f;
        speed = 130.0f;
        reward = 6;
        radius = 10.0f;
        color = ORANGE;
        break;

    case EnemyType::TANK:
        maxHp = hp = 220.0f;
        speed = 45.0f;
        reward = 20;
        radius = 15.0f;
        color = DARKGREEN;
        break;

    case EnemyType::BOSS:
        // Abyss Lord - Final boss with massive HP
        maxHp = hp = 1000.0f;
        speed = 35.0f;
        reward = 200;
        radius = 25.0f;
        color = PURPLE;
        break;
    }
}

void Enemy::TakeDamage(float damage)
{
    // Ignore damage if already dead.
    if (!alive) return;

    hp -= damage;

    // Handle death only once.
    if (hp <= 0.0f) {
        hp = 0.0f;
        alive = false;

        // Trigger reward only once.
        if (!rewardGiven) {
            rewardGiven = true;
            if (onDeath) {
                onDeath(reward);
            }
        }
    }
}

void Enemy::Draw(bool drawHpBar) const
{
    DrawCircleV(position, radius, color);

    if (!drawHpBar) return;

    // Simple HP bar above the enemy
    float barW = radius * 2.2f;
    float barH = 5.0f;
    float x = position.x - barW / 2.0f;
    float y = position.y - radius - 10.0f;

    float ratio = (maxHp <= 0.0f) ? 0.0f : (hp / maxHp);
    ratio = std::clamp(ratio, 0.0f, 1.0f);

    DrawRectangle((int)x, (int)y, (int)barW, (int)barH, DARKGRAY);
    DrawRectangle((int)x, (int)y, (int)(barW * ratio), (int)barH, GREEN);
    DrawRectangleLines((int)x, (int)y, (int)barW, (int)barH, BLACK);
}

bool Enemy::Update(float dt, const std::vector<Vector2>& waypoints)
{
    // Do not update dead enemies.
    if (!alive) return false;

    // No path -> no movement.
    if (waypoints.empty()) return false;

    // Clamp index to valid range (defensive)
    int idx = std::clamp(currentWaypointIndex, 0, (int)waypoints.size() - 1);
    Vector2 target = waypoints[idx];

    Vector2 toTarget{ target.x - position.x, target.y - position.y };
    float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

    // Small epsilon for waypoint arrival
    const float arriveEps = 3.0f;

    if (dist <= arriveEps) {
        currentWaypointIndex++;

        // If we passed the last waypoint, we reached the base/end.
        if (currentWaypointIndex >= (int)waypoints.size()) {
            alive = false; // Mark for removal by manager

            // Notify the system that an enemy reached the end.
            if (onReachedEnd) {
                onReachedEnd();
            }

            return true;
        }

        return false;
    }

    // Normalize direction safely
    Vector2 dir{ toTarget.x / (dist + 1e-6f), toTarget.y / (dist + 1e-6f) };

    // Move using dt
    position.x += dir.x * speed * dt;
    position.y += dir.y * speed * dt;

    return false;
}
