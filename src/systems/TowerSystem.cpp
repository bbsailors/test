#include "systems/TowerSystem.h"
#include <cmath>
#include <algorithm>
#include <limits>

TowerSystem::TowerSystem()
    : selectedType(TowerType::CORAL_CANNON)
    , previewPos{0, 0}
    , previewActive(false)
    , currentEnemies(nullptr)
{
    // Set up projectile hit callback
    manager.SetOnProjectileHit(
        [this](Projectile& proj, Vector2 hitPos) {
            if (currentEnemies) {
                HandleProjectileHit(proj, hitPos, *currentEnemies);
            }
        }
    );
}

void TowerSystem::Update(float dt, std::vector<Enemy>& enemies) {
    currentEnemies = &enemies;
    
    // Update tower manager (handles cooldowns and projectiles)
    manager.Update(dt);
    
    // For each tower, find target and fire if ready
    for (auto& tower : manager.GetTowers()) {
        if (!tower.CanFire()) continue;
        
        Enemy* target = FindTarget(tower, enemies);
        if (target && target->alive) {
            // Fire projectile at target
            Projectile proj = tower.Fire(target->position);
            manager.AddProjectile(proj);
        }
    }
}

void TowerSystem::Draw() const {
    manager.Draw();
    
    if (previewActive) {
        DrawPreview();
    }
}

void TowerSystem::Reset() {
    manager.Clear();
    previewActive = false;
    currentEnemies = nullptr;
}

bool TowerSystem::TryPlaceTower(TowerType type, Vector2 position, int cost, std::function<bool(int)> canAfford) {
    // Check if we can afford it
    if (!canAfford || !canAfford(cost)) {
        return false;
    }
    
    // Check if placement is valid
    if (canPlaceAt && !canPlaceAt(position)) {
        return false;
    }
    
    // Check if there's already a tower there
    if (manager.HasTowerAt(position, 40.0f)) {
        return false;
    }
    
    // Spend gold
    if (onSpendGold && !onSpendGold(cost)) {
        return false;
    }
    
    // Place the tower
    return manager.PlaceTower(type, position);
}

bool TowerSystem::RemoveTower(Vector2 position) {
    return manager.RemoveTower(position);
}

void TowerSystem::HandleClick(Vector2 mousePos) {
    Tower* tower = manager.GetTowerAt(mousePos, 30.0f);
    if (tower) {
        manager.SelectTower(tower);
    } else {
        manager.DeselectAll();
    }
}

void TowerSystem::DeselectAll() {
    manager.DeselectAll();
}

Tower* TowerSystem::GetSelectedTower() const {
    return manager.GetSelectedTower();
}

void TowerSystem::SetCanPlaceAt(std::function<bool(Vector2)> callback) {
    canPlaceAt = std::move(callback);
}

void TowerSystem::SetOnSpendGold(std::function<bool(int)> callback) {
    onSpendGold = std::move(callback);
}

int TowerSystem::GetTowerCost(TowerType type) {
    return GetTowerStats(type).cost;
}

void TowerSystem::DrawPreview() const {
    if (!previewActive) return;
    
    TowerStats stats = GetTowerStats(selectedType);
    bool canPlace = (!canPlaceAt || canPlaceAt(previewPos)) && 
                    !manager.HasTowerAt(previewPos, 40.0f);
    
    Color previewColor = canPlace ? Color{0, 255, 0, 100} : Color{255, 0, 0, 100};
    
    // Draw range preview
    DrawCircle((int)previewPos.x, (int)previewPos.y, stats.range, {previewColor.r, previewColor.g, previewColor.b, 30});
    DrawCircleLines((int)previewPos.x, (int)previewPos.y, stats.range, previewColor);
    
    // Draw tower preview
    DrawCircle((int)previewPos.x, (int)previewPos.y, 20.0f, previewColor);
}

Enemy* TowerSystem::FindTarget(Tower& tower, std::vector<Enemy>& enemies) {
    Enemy* bestTarget = nullptr;
    float bestPriority = -std::numeric_limits<float>::max();
    
    for (auto& enemy : enemies) {
        if (!enemy.alive) continue;
        
        // Check if in range
        if (!tower.IsInRange(enemy.position)) continue;
        
        // Priority: closest to end (highest waypoint index) and closest to tower
        // This makes towers focus on enemies that are closest to reaching the base
        float priority = static_cast<float>(enemy.currentWaypointIndex) * 1000.0f 
                        - tower.GetDistanceTo(enemy.position);
        
        if (priority > bestPriority) {
            bestPriority = priority;
            bestTarget = &enemy;
        }
    }
    
    return bestTarget;
}

void TowerSystem::HandleProjectileHit(Projectile& proj, Vector2 hitPos, std::vector<Enemy>& enemies) {
    // Handle splash damage
    if (proj.splashRadius > 0.0f) {
        // AoE damage
        for (auto& enemy : enemies) {
            if (!enemy.alive) continue;
            
            float dx = enemy.position.x - hitPos.x;
            float dy = enemy.position.y - hitPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist <= proj.splashRadius) {
                // Damage falls off with distance
                float falloff = 1.0f - (dist / proj.splashRadius) * 0.5f;
                enemy.TakeDamage(proj.damage * falloff);
            }
        }
        
        // Draw splash effect (visual feedback)
        // Note: This is instant, for proper effects you'd use a particle system
    } else {
        // Single target - find closest enemy to hit position
        Enemy* target = nullptr;
        float closestDist = 30.0f; // Max distance to consider a hit
        
        for (auto& enemy : enemies) {
            if (!enemy.alive) continue;
            
            float dx = enemy.position.x - hitPos.x;
            float dy = enemy.position.y - hitPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist < closestDist) {
                closestDist = dist;
                target = &enemy;
            }
        }
        
        if (target) {
            target->TakeDamage(proj.damage);
            
            // Apply slow effect
            if (proj.slowAmount > 0.0f) {
                // Reduce enemy speed temporarily
                // Note: For proper slow, you'd add slow state to Enemy
                target->speed *= (1.0f - proj.slowAmount);
            }
        }
    }
}
