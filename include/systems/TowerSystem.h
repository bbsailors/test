#pragma once

#include "raylib.h"
#include "tower/TowerManager.h"
#include "tower/TowerTypes.h"
#include "enemy/Enemy.h"

#include <vector>
#include <functional>

// ============================================================
// TowerSystem: High-level tower management for Game class
// ============================================================
// Handles tower placement, targeting, and damage dealing
// ============================================================

class TowerSystem {
public:
    TowerSystem();
    
    // Core methods called by Game
    void Update(float dt, std::vector<Enemy>& enemies);
    void Draw() const;
    void Reset();
    
    // Tower placement (called by Game on player input)
    bool TryPlaceTower(TowerType type, Vector2 position, int cost, std::function<bool(int)> canAfford);
    bool RemoveTower(Vector2 position);
    
    // Selection
    void HandleClick(Vector2 mousePos);
    void DeselectAll();
    Tower* GetSelectedTower() const;
    
    // Set valid placement callback
    void SetCanPlaceAt(std::function<bool(Vector2)> callback);
    
    // Set callback for when gold should be spent
    void SetOnSpendGold(std::function<bool(int)> callback);
    
    // Get tower cost for UI
    static int GetTowerCost(TowerType type);
    
    // Accessors
    TowerManager& GetManager() { return manager; }
    const TowerManager& GetManager() const { return manager; }
    
    // Current selected tower type for placement
    void SetSelectedTowerType(TowerType type) { selectedType = type; }
    TowerType GetSelectedTowerType() const { return selectedType; }
    
    // Preview mode
    void SetPreviewPosition(Vector2 pos) { previewPos = pos; previewActive = true; }
    void ClearPreview() { previewActive = false; }
    void DrawPreview() const;
    
private:
    // Find best target for a tower
    Enemy* FindTarget(Tower& tower, std::vector<Enemy>& enemies);
    
    // Apply damage to enemies
    void HandleProjectileHit(Projectile& proj, Vector2 hitPos, std::vector<Enemy>& enemies);
    
    TowerManager manager;
    
    // Placement
    TowerType selectedType;
    Vector2 previewPos;
    bool previewActive;
    
    std::function<bool(Vector2)> canPlaceAt;
    std::function<bool(int)> onSpendGold;
    
    // Cache for enemy targeting
    std::vector<Enemy>* currentEnemies;
};
