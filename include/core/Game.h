#pragma once
#include "GameState.h"
#include "systems/EnemySystem.h"
#include "systems/WaveSystem.h"
#include "systems/UISystem.h"
#include "systems/TowerSystem.h"
#include "map/Map.h"
#include "tower/TowerTypes.h"

class Game
{
public:
    Game();

    void Init();
    void Run();
    void HandleInput();
    void Update();
    void Draw();
    void ResetGame();

private:
    void SetupWaypoints();
    void ConnectSystems();
    void UpdateHUD();
    void HandleTowerPlacement();
    void DrawTowerUI();

    GameState currentState;
    
    // Core game systems
    EnemySystem enemySystem;
    WaveSystem waveSystem;
    UISystem uiSystem;
    TowerSystem towerSystem;
    Map gameMap;
    
    // Game state
    int playerHP;
    static constexpr int MAX_HP = 20;
    
    // Tower placement state
    TowerType selectedTowerType;
    bool placingTower;
};
