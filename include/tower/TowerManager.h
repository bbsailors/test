#pragma once

#include "Tower.h"
#include <vector>
#include <functional>

// ============================================================
// TowerManager: Manages all towers and projectiles
// ============================================================
class TowerManager {
public:
    TowerManager();
    
    // Core methods
    void Update(float dt);
    void Draw() const;
    void DrawRanges() const;
    void Clear();
    
    // Tower management
    bool PlaceTower(TowerType type, Vector2 position);
    bool RemoveTower(Vector2 position);
    Tower* GetTowerAt(Vector2 position, float tolerance = 30.0f);
    
    // Selection
    void SelectTower(Tower* tower);
    void DeselectAll();
    Tower* GetSelectedTower() const { return selectedTower; }
    
    // Projectile management
    void AddProjectile(const Projectile& proj);
    void UpdateProjectiles(float dt);
    void DrawProjectiles() const;
    
    // Get all towers (for targeting)
    std::vector<Tower>& GetTowers() { return towers; }
    const std::vector<Tower>& GetTowers() const { return towers; }
    
    // Get active projectiles (for hit detection)
    std::vector<Projectile>& GetProjectiles() { return projectiles; }
    const std::vector<Projectile>& GetProjectiles() const { return projectiles; }
    
    // Callbacks
    void SetOnProjectileHit(std::function<void(Projectile&, Vector2)> callback);
    
    // Queries
    int GetTowerCount() const { return static_cast<int>(towers.size()); }
    bool HasTowerAt(Vector2 position, float tolerance = 30.0f) const;
    
private:
    std::vector<Tower> towers;
    std::vector<Projectile> projectiles;
    Tower* selectedTower;
    
    std::function<void(Projectile&, Vector2)> onProjectileHit;
    
    static constexpr float PROJECTILE_HIT_RADIUS = 10.0f;
};
