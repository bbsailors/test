#pragma once

#include <vector>
#include "Enemy.h"

// Owns and updates a collection of enemies.
class EnemyManager {
public:
    // Adds a new enemy instance into the internal list.
    void AddEnemy(const Enemy& e);

    // Updates all enemies; dt is delta time.
    void Update(float dt, const std::vector<Vector2>& waypoints);

    // Draws all enemies.
    void Draw() const;

    // Returns number of currently alive enemies (size of list).
    int AliveCount() const;

    // True if there are no enemies left.
    bool IsEmpty() const;

    // Clears all enemies (useful on reset / game over / victory).
    void Clear();
    
    // Get access to enemies for tower targeting
    std::vector<Enemy>& GetEnemies() { return enemies; }
    const std::vector<Enemy>& GetEnemies() const { return enemies; }

private:
    std::vector<Enemy> enemies;
};
