#include "tower/Tower.h"
#include <cmath>

Tower::Tower()
    : type(TowerType::CORAL_CANNON)
    , position{0, 0}
    , fireCooldown(0.0f)
    , cooldownTime(1.0f)
    , selected(false)
    , radius(20.0f)
    , baseColor(BLUE)
    , accentColor(SKYBLUE)
{
    stats = GetTowerStats(type);
    cooldownTime = 1.0f / stats.fireRate;
}

Tower::Tower(TowerType t, Vector2 pos)
    : type(t)
    , position(pos)
    , fireCooldown(0.0f)
    , selected(false)
{
    stats = GetTowerStats(type);
    cooldownTime = 1.0f / stats.fireRate;
    
    // Set visual properties based on type
    switch (type) {
        case TowerType::CORAL_CANNON:
            radius = 22.0f;
            baseColor = {200, 80, 80, 255};      // Coral red
            accentColor = {255, 150, 150, 255};
            break;
            
        case TowerType::TIDAL_BURST:
            radius = 25.0f;
            baseColor = {60, 120, 200, 255};     // Ocean blue
            accentColor = {100, 180, 255, 255};
            break;
            
        case TowerType::FROST_TOTEM:
            radius = 20.0f;
            baseColor = {150, 220, 255, 255};    // Ice blue
            accentColor = {220, 245, 255, 255};
            break;
    }
}

void Tower::Update(float dt) {
    // Reduce cooldown timer
    if (fireCooldown > 0.0f) {
        fireCooldown -= dt;
        if (fireCooldown < 0.0f) {
            fireCooldown = 0.0f;
        }
    }
}

bool Tower::IsInRange(Vector2 enemyPos) const {
    return GetDistanceTo(enemyPos) <= stats.range;
}

float Tower::GetDistanceTo(Vector2 pos) const {
    float dx = pos.x - position.x;
    float dy = pos.y - position.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool Tower::CanFire() const {
    return fireCooldown <= 0.0f;
}

Projectile Tower::Fire(Vector2 targetPos) {
    Projectile proj;
    proj.position = position;
    proj.targetPos = targetPos;
    proj.speed = stats.projectileSpeed;
    proj.damage = stats.damage;
    proj.splashRadius = stats.splashRadius;
    proj.slowAmount = stats.slowAmount;
    proj.slowDuration = stats.slowDuration;
    proj.active = true;
    
    // Set projectile color based on tower type
    switch (type) {
        case TowerType::CORAL_CANNON:
            proj.color = {255, 100, 100, 255};
            break;
        case TowerType::TIDAL_BURST:
            proj.color = {100, 150, 255, 255};
            break;
        case TowerType::FROST_TOTEM:
            proj.color = {200, 240, 255, 255};
            break;
    }
    
    // Reset cooldown
    fireCooldown = cooldownTime;
    
    return proj;
}

void Tower::Draw() const {
    // Draw base
    DrawCircleV(position, radius, baseColor);
    
    // Draw inner accent
    DrawCircleV(position, radius * 0.6f, accentColor);
    
    // Draw outline
    DrawCircleLines((int)position.x, (int)position.y, radius, BLACK);
    
    // Draw type indicator
    switch (type) {
        case TowerType::CORAL_CANNON:
            // Draw cannon barrel
            DrawRectangle((int)(position.x - 4), (int)(position.y - radius - 8), 8, 12, DARKGRAY);
            break;
            
        case TowerType::TIDAL_BURST:
            // Draw wave symbol
            DrawCircleLines((int)position.x, (int)position.y, radius * 0.4f, DARKBLUE);
            break;
            
        case TowerType::FROST_TOTEM:
            // Draw frost crystal
            DrawLineEx(
                {position.x - 6, position.y - 6},
                {position.x + 6, position.y + 6},
                2.0f, WHITE
            );
            DrawLineEx(
                {position.x + 6, position.y - 6},
                {position.x - 6, position.y + 6},
                2.0f, WHITE
            );
            break;
    }
    
    // Draw selection highlight
    if (selected) {
        DrawCircleLines((int)position.x, (int)position.y, radius + 4, YELLOW);
    }
    
    // Draw cooldown indicator
    if (fireCooldown > 0.0f) {
        float ratio = fireCooldown / cooldownTime;
        DrawCircleSector(
            position, 
            radius * 0.4f, 
            0.0f, 
            360.0f * ratio, 
            16, 
            {0, 0, 0, 100}
        );
    }
}

void Tower::DrawRange() const {
    // Draw range circle (semi-transparent)
    DrawCircleLines((int)position.x, (int)position.y, stats.range, {baseColor.r, baseColor.g, baseColor.b, 100});
    
    // Draw filled range when selected
    if (selected) {
        DrawCircle((int)position.x, (int)position.y, stats.range, {baseColor.r, baseColor.g, baseColor.b, 30});
    }
}
