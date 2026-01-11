#include "tower/TowerManager.h"
#include <algorithm>
#include <cmath>

TowerManager::TowerManager()
    : selectedTower(nullptr)
{
}

void TowerManager::Update(float dt) {
    // Update all towers
    for (auto& tower : towers) {
        tower.Update(dt);
    }
    
    // Update projectiles
    UpdateProjectiles(dt);
}

void TowerManager::Draw() const {
    // Draw tower ranges first (behind towers)
    DrawRanges();
    
    // Draw all towers
    for (const auto& tower : towers) {
        tower.Draw();
    }
    
    // Draw projectiles on top
    DrawProjectiles();
}

void TowerManager::DrawRanges() const {
    for (const auto& tower : towers) {
        if (tower.IsSelected()) {
            tower.DrawRange();
        }
    }
}

void TowerManager::Clear() {
    towers.clear();
    projectiles.clear();
    selectedTower = nullptr;
}

bool TowerManager::PlaceTower(TowerType type, Vector2 position) {
    // Check if there's already a tower at this position
    if (HasTowerAt(position, 40.0f)) {
        return false;
    }
    
    towers.emplace_back(type, position);
    return true;
}

bool TowerManager::RemoveTower(Vector2 position) {
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        float dx = it->GetPosition().x - position.x;
        float dy = it->GetPosition().y - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        
        if (dist < 30.0f) {
            if (selectedTower == &(*it)) {
                selectedTower = nullptr;
            }
            towers.erase(it);
            return true;
        }
    }
    return false;
}

Tower* TowerManager::GetTowerAt(Vector2 position, float tolerance) {
    for (auto& tower : towers) {
        float dx = tower.GetPosition().x - position.x;
        float dy = tower.GetPosition().y - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        
        if (dist < tolerance) {
            return &tower;
        }
    }
    return nullptr;
}

void TowerManager::SelectTower(Tower* tower) {
    DeselectAll();
    if (tower) {
        tower->SetSelected(true);
        selectedTower = tower;
    }
}

void TowerManager::DeselectAll() {
    for (auto& tower : towers) {
        tower.SetSelected(false);
    }
    selectedTower = nullptr;
}

void TowerManager::AddProjectile(const Projectile& proj) {
    projectiles.push_back(proj);
}

void TowerManager::UpdateProjectiles(float dt) {
    for (auto& proj : projectiles) {
        if (!proj.active) continue;
        
        // Calculate direction to target
        float dx = proj.targetPos.x - proj.position.x;
        float dy = proj.targetPos.y - proj.position.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        
        // Check if reached target
        if (dist < PROJECTILE_HIT_RADIUS) {
            proj.active = false;
            
            // Trigger hit callback
            if (onProjectileHit) {
                onProjectileHit(proj, proj.targetPos);
            }
            continue;
        }
        
        // Normalize and move
        float invDist = 1.0f / (dist + 0.0001f);
        proj.position.x += dx * invDist * proj.speed * dt;
        proj.position.y += dy * invDist * proj.speed * dt;
    }
    
    // Remove inactive projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return !p.active; }),
        projectiles.end()
    );
}

void TowerManager::DrawProjectiles() const {
    for (const auto& proj : projectiles) {
        if (!proj.active) continue;
        
        // Draw projectile
        DrawCircleV(proj.position, 5.0f, proj.color);
        
        // Draw trail
        DrawLineEx(
            proj.position,
            {proj.position.x - (proj.targetPos.x - proj.position.x) * 0.1f,
             proj.position.y - (proj.targetPos.y - proj.position.y) * 0.1f},
            3.0f,
            {proj.color.r, proj.color.g, proj.color.b, 128}
        );
    }
}

void TowerManager::SetOnProjectileHit(std::function<void(Projectile&, Vector2)> callback) {
    onProjectileHit = std::move(callback);
}

bool TowerManager::HasTowerAt(Vector2 position, float tolerance) const {
    for (const auto& tower : towers) {
        float dx = tower.GetPosition().x - position.x;
        float dy = tower.GetPosition().y - position.y;
        float dist = std::sqrt(dx * dx + dy * dy);
        
        if (dist < tolerance) {
            return true;
        }
    }
    return false;
}
