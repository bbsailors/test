#include "enemy/EnemyManager.h"

#include <algorithm>

void EnemyManager::AddEnemy(const Enemy& e)
{
    enemies.push_back(e);
}

void EnemyManager::Update(float dt, const std::vector<Vector2>& waypoints)
{
    // Update all enemies first
    for (auto& e : enemies) {
        e.Update(dt, waypoints);
    }

    // Remove dead enemies (killed or reached the end)
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return !e.alive; }),
        enemies.end()
    );
}

void EnemyManager::Draw() const
{
    for (const auto& e : enemies) {
        e.Draw(true);
    }
}

int EnemyManager::AliveCount() const
{
    return (int)enemies.size();
}

bool EnemyManager::IsEmpty() const
{
    return enemies.empty();
}

void EnemyManager::Clear()
{
    enemies.clear();
}
