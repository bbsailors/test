#pragma once

#include "raylib.h"
#include "TowerTypes.h"
#include <functional>

// Forward declaration
struct Enemy;

// ============================================================
// Projectile: Visual representation of tower attacks
// ============================================================
struct Projectile {
    Vector2 position;
    Vector2 targetPos;
    float speed;
    float damage;
    float splashRadius;
    float slowAmount;
    float slowDuration;
    bool active;
    Color color;
    
    Projectile()
        : position{0, 0}
        , targetPos{0, 0}
        , speed(400.0f)
        , damage(0.0f)
        , splashRadius(0.0f)
        , slowAmount(0.0f)
        , slowDuration(0.0f)
        , active(false)
        , color(WHITE)
    {}
};

// ============================================================
// Tower: A defensive structure that attacks enemies
// ============================================================
class Tower {
public:
    Tower();
    Tower(TowerType type, Vector2 position);
    
    // Core update - handles cooldown
    void Update(float dt);
    
    // Check if enemy is in range
    bool IsInRange(Vector2 enemyPos) const;
    
    // Get distance to a position
    float GetDistanceTo(Vector2 pos) const;
    
    // Check if ready to fire
    bool CanFire() const;
    
    // Fire at target position, returns projectile
    Projectile Fire(Vector2 targetPos);
    
    // Drawing
    void Draw() const;
    void DrawRange() const;
    
    // Getters
    TowerType GetType() const { return type; }
    Vector2 GetPosition() const { return position; }
    float GetRange() const { return stats.range; }
    float GetDamage() const { return stats.damage; }
    float GetSplashRadius() const { return stats.splashRadius; }
    float GetSlowAmount() const { return stats.slowAmount; }
    float GetSlowDuration() const { return stats.slowDuration; }
    int GetCost() const { return stats.cost; }
    const TowerStats& GetStats() const { return stats; }
    
    // Selection state for UI
    bool IsSelected() const { return selected; }
    void SetSelected(bool sel) { selected = sel; }
    
private:
    TowerType type;
    Vector2 position;
    TowerStats stats;
    
    float fireCooldown;     // Current cooldown timer
    float cooldownTime;     // Time between shots (1/fireRate)
    
    bool selected;          // For UI highlighting
    
    // Visual properties
    float radius;
    Color baseColor;
    Color accentColor;
};
