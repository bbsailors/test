#pragma once

// ============================================================
// Tower Types - Based on GDD
// ============================================================
// 1. Coral Cannon - Single target, high damage
// 2. Tidal Burst Tower - AoE splash damage
// 3. Frost Current Totem - Slow effect
// ============================================================

enum class TowerType {
    CORAL_CANNON,       // Single target, high damage
    TIDAL_BURST,        // AoE splash damage
    FROST_TOTEM         // Slow effect, low/no damage
};

// Tower stat configuration
struct TowerStats {
    int cost;               // Gold cost to build
    float damage;           // Damage per hit
    float range;            // Attack range in pixels
    float fireRate;         // Attacks per second
    float splashRadius;     // AoE radius (0 for single target)
    float slowAmount;       // Slow percentage (0.0 - 1.0)
    float slowDuration;     // How long slow lasts
    float projectileSpeed;  // Speed of projectile (0 for instant)
    
    TowerStats()
        : cost(50)
        , damage(25.0f)
        , range(150.0f)
        , fireRate(1.0f)
        , splashRadius(0.0f)
        , slowAmount(0.0f)
        , slowDuration(0.0f)
        , projectileSpeed(400.0f)
    {}
};

// Get default stats for each tower type
inline TowerStats GetTowerStats(TowerType type) {
    TowerStats stats;
    
    switch (type) {
        case TowerType::CORAL_CANNON:
            stats.cost = 50;
            stats.damage = 35.0f;
            stats.range = 150.0f;
            stats.fireRate = 1.2f;
            stats.splashRadius = 0.0f;
            stats.slowAmount = 0.0f;
            stats.projectileSpeed = 500.0f;
            break;
            
        case TowerType::TIDAL_BURST:
            stats.cost = 75;
            stats.damage = 20.0f;
            stats.range = 120.0f;
            stats.fireRate = 0.8f;
            stats.splashRadius = 60.0f;
            stats.slowAmount = 0.0f;
            stats.projectileSpeed = 350.0f;
            break;
            
        case TowerType::FROST_TOTEM:
            stats.cost = 60;
            stats.damage = 5.0f;
            stats.range = 130.0f;
            stats.fireRate = 1.5f;
            stats.splashRadius = 0.0f;
            stats.slowAmount = 0.4f;      // 40% slow
            stats.slowDuration = 2.0f;    // 2 seconds
            stats.projectileSpeed = 600.0f;
            break;
    }
    
    return stats;
}

// Get tower name for UI
inline const char* GetTowerName(TowerType type) {
    switch (type) {
        case TowerType::CORAL_CANNON: return "Coral Cannon";
        case TowerType::TIDAL_BURST: return "Tidal Burst";
        case TowerType::FROST_TOTEM: return "Frost Totem";
        default: return "Unknown";
    }
}
