#include "core/Game.h"
#include "core/GameConfig.h"
#include "raylib.h"
#include <string>

Game::Game()
    : currentState(GameState::MENU)
    , playerHP(MAX_HP)
    , selectedTowerType(TowerType::CORAL_CANNON)
    , placingTower(false)
{
}

void Game::Init()
{
    GameConfig& config = GetGameConfig();
    InitWindow(config.screenWidth, config.screenHeight, config.windowTitle);
    SetTargetFPS(60);
    
    // Load UI font
    LoadUIFont("assets/fonts/default.ttf", 32);
    
    // Initialize map
    gameMap.Init(config.screenWidth, config.screenHeight);
    
    // Initialize systems
    waveSystem.Init();
    
    // Setup waypoints (path for enemies from map)
    SetupWaypoints();
    
    // Connect system callbacks
    ConnectSystems();
    
    // Set initial UI state
    uiSystem.SetScreen(UIScreenState::Start);
}

void Game::SetupWaypoints()
{
    // Get waypoints from map
    const std::vector<Vector2>& waypoints = gameMap.GetWaypoints();
    
    if (!waypoints.empty()) {
        enemySystem.SetWaypoints(waypoints);
        waveSystem.GetWaveManager().SetSpawnPoint(waypoints[0]);
    } else {
        // Fallback to hardcoded waypoints if map doesn't provide them
        std::vector<Vector2> fallbackWaypoints = {
            {50, 360},      // Start (left side)
            {200, 360},
            {200, 200},
            {400, 200},
            {400, 500},
            {600, 500},
            {600, 300},
            {800, 300},
            {800, 450},
            {1000, 450},
            {1200, 450}     // End (right side - base)
        };
        
        enemySystem.SetWaypoints(fallbackWaypoints);
        waveSystem.GetWaveManager().SetSpawnPoint(fallbackWaypoints[0]);
    }
}

void Game::ConnectSystems()
{
    // Connect WaveManager spawn callback to EnemySystem
    waveSystem.GetWaveManager().SetOnSpawnEnemy(
        [this](EnemyType type, Vector2 pos) {
            enemySystem.SpawnEnemy(type, pos);
        }
    );
    
    // Connect enemy death reward to WaveManager economy
    enemySystem.SetOnReward(
        [this](int reward) {
            waveSystem.GetWaveManager().OnEnemyKilled(reward);
        }
    );
    
    // Connect wave cleared event
    waveSystem.GetWaveManager().SetOnWaveCleared(
        [this]() {
            // Award wave completion bonus
            waveSystem.GetWaveManager().GetEconomy().AwardWaveCompletionBonus(
                waveSystem.GetCurrentWave()
            );
        }
    );
    
    // Boss warning story cue
    waveSystem.GetWaveManager().SetOnBossWarning([
        this
    ]() {
        uiSystem.ShowStory(
            "Voidborn leader approaches!",
            "Abyss Lord is emerging. Brace your defenses!",
            3.5f
        );
    });
    
    // Connect victory event
    waveSystem.GetWaveManager().SetOnAllWavesCompleted(
        [this]() {
            currentState = GameState::VICTORY;
            uiSystem.SetScreen(UIScreenState::Victory);
        }
    );
    
    // Connect tower system to map for placement validation
    towerSystem.SetCanPlaceAt([this](Vector2 pos) {
        return gameMap.CanPlaceTower(pos);
    });
    
    // Connect tower system to economy for spending gold
    towerSystem.SetOnSpendGold([this](int cost) {
        return waveSystem.GetWaveManager().SpendGold(cost);
    });
}

void Game::UpdateHUD()
{
    HUDData hud;
    hud.hp = playerHP;
    hud.money = waveSystem.GetGold();
    hud.currentWave = waveSystem.GetCurrentWave();
    hud.totalWaves = waveSystem.GetTotalWaves();
    uiSystem.SetHUDData(hud);
}

void Game::Run()
{
    Init();

    while (!WindowShouldClose())
    {
        HandleInput();
        Update();
        Draw();
    }

    UnloadUIFont();
    CloseWindow();
}

void Game::HandleInput()
{
    // Global exit
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (currentState == GameState::GAME) {
            currentState = GameState::PAUSE;
            uiSystem.SetScreen(UIScreenState::Paused);
        } else if (currentState == GameState::PAUSE) {
            currentState = GameState::MENU;
            uiSystem.SetScreen(UIScreenState::Start);
            ResetGame();
        } else {
            CloseWindow();
        }
        return;
    }

    switch (currentState)
    {
    case GameState::MENU:
        if (IsKeyPressed(KEY_ENTER)) {
            currentState = GameState::GAME;
            uiSystem.SetScreen(UIScreenState::PlayingHUD);
            ResetGame();
            uiSystem.ShowStory(
                "Depths stir with darkness...",
                "Protect Aria and the heart of Aqualis!",
                3.8f
            );
            waveSystem.StartNextWave();
        }
        break;

    case GameState::GAME:
        if (IsKeyPressed(KEY_P)) {
            currentState = GameState::PAUSE;
            uiSystem.SetScreen(UIScreenState::Paused);
        }
        
        // Start next wave with SPACE
        if (IsKeyPressed(KEY_SPACE)) {
            if (waveSystem.GetWaveManager().CanStartNextWave()) {
                int nextWave = waveSystem.GetCurrentWave() + 1;
                waveSystem.StartNextWave();
                uiSystem.ShowStory(
                    "Wave " + std::to_string(nextWave) + " incoming!",
                    "Voidborn surge through the currents...",
                    2.6f
                );
            }
        }
        
        // Debug keys
        if (IsKeyPressed(KEY_V)) {
            currentState = GameState::VICTORY;
            uiSystem.SetScreen(UIScreenState::Victory);
        }
        if (IsKeyPressed(KEY_G)) {
            currentState = GameState::GAMEOVER;
            uiSystem.SetScreen(UIScreenState::GameOver);
        }
        
        // Tower selection keys (1, 2, 3)
        if (IsKeyPressed(KEY_ONE)) {
            selectedTowerType = TowerType::CORAL_CANNON;
            placingTower = true;
        }
        if (IsKeyPressed(KEY_TWO)) {
            selectedTowerType = TowerType::TIDAL_BURST;
            placingTower = true;
        }
        if (IsKeyPressed(KEY_THREE)) {
            selectedTowerType = TowerType::FROST_TOTEM;
            placingTower = true;
        }
        
        // Cancel tower placement with right click
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            placingTower = false;
            towerSystem.ClearPreview();
            towerSystem.DeselectAll();
        }
        
        // Handle tower placement
        HandleTowerPlacement();
        break;

    case GameState::PAUSE:
        if (IsKeyPressed(KEY_P)) {
            currentState = GameState::GAME;
            uiSystem.SetScreen(UIScreenState::PlayingHUD);
        }
        break;

    case GameState::GAMEOVER:
    case GameState::VICTORY:
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_R)) {
            ResetGame();
            currentState = GameState::MENU;
            uiSystem.SetScreen(UIScreenState::Start);
        }
        break;
    }
}

void Game::Update()
{
    float dt = GetFrameTime();
    uiSystem.Update(dt);

    if (currentState != GameState::GAME)
        return;

    // === Wave System (handles spawning) ===
    waveSystem.Update(dt);

    // === Enemy System ===
    enemySystem.Update(dt);

    // === Tower System (with enemy targeting) ===
    towerSystem.Update(dt, enemySystem.GetManager().GetEnemies());
    
    // Update HUD preview position for tower placement
    if (placingTower) {
        Vector2 mousePos = GetMousePosition();
        Vector2 snappedPos = gameMap.SnapToGrid(mousePos);
        towerSystem.SetSelectedTowerType(selectedTowerType);
        towerSystem.SetPreviewPosition(snappedPos);
    }

    // === GAME OVER CHECK ===
    if (enemySystem.HasEnemyReachedEnd()) {
        playerHP--;
        waveSystem.GetWaveManager().OnEnemyReachedBase();
        
        if (playerHP <= 0) {
            currentState = GameState::GAMEOVER;
            uiSystem.SetScreen(UIScreenState::GameOver);
        }
    }

    // === VICTORY CHECK ===
    if (waveSystem.IsVictory()) {
        currentState = GameState::VICTORY;
        uiSystem.SetScreen(UIScreenState::Victory);
    }
    
    // Update HUD data
    UpdateHUD();
}

void Game::Draw()
{
    BeginDrawing();
    ClearBackground({12, 28, 52, 255}); // Polished ocean tone

    switch (currentState)
    {
    case GameState::MENU:
        uiSystem.Draw();
        break;

    case GameState::GAME:
        // Draw map first (background)
        gameMap.Draw();
        
        // Draw enemies
        enemySystem.Draw();
        
        // Draw towers
        towerSystem.Draw();
        
        // Draw tower selection UI
        DrawTowerUI();
        
        // Draw HUD + story banner
        uiSystem.Draw();
        
        // Draw wave status
        {
            const char* waveStatus = "";
            WaveState state = waveSystem.GetWaveManager().GetWaveState();
            switch (state) {
                case WaveState::WAITING:
                    waveStatus = "Press SPACE to start next wave";
                    break;
                case WaveState::SPAWNING:
                    waveStatus = "Wave in progress - Enemies spawning!";
                    break;
                case WaveState::IN_PROGRESS:
                    waveStatus = "Clear all enemies!";
                    break;
                case WaveState::BOSS_WARNING:
                    waveStatus = "!!! BOSS INCOMING !!!";
                    break;
                default:
                    break;
            }
            DrawText(waveStatus, 400, 680, 20, YELLOW);
        }
        break;

    case GameState::PAUSE:
        gameMap.Draw();
        enemySystem.Draw();
        towerSystem.Draw();
        uiSystem.Draw();
        break;

    case GameState::GAMEOVER:
        uiSystem.Draw();
        break;

    case GameState::VICTORY:
        uiSystem.Draw();
        break;
    }

    EndDrawing();
}

void Game::ResetGame()
{
    playerHP = MAX_HP;
    placingTower = false;
    selectedTowerType = TowerType::CORAL_CANNON;
    
    enemySystem.Reset();
    towerSystem.Reset();
    waveSystem.Reset();
    waveSystem.Init();
    
    // Reinitialize map
    GameConfig& config = GetGameConfig();
    gameMap.Init(config.screenWidth, config.screenHeight);
    
    ConnectSystems();
    SetupWaypoints();
}

void Game::HandleTowerPlacement()
{
    if (!placingTower) return;
    
    Vector2 mousePos = GetMousePosition();
    Vector2 snappedPos = gameMap.SnapToGrid(mousePos);
    
    // Left click to place
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int cost = TowerSystem::GetTowerCost(selectedTowerType);
        
        if (waveSystem.CanAfford(cost) && gameMap.CanPlaceTower(mousePos)) {
            if (towerSystem.TryPlaceTower(selectedTowerType, snappedPos, cost,
                [this](int c) { return waveSystem.CanAfford(c); })) {
                // Tower placed successfully
                // Keep placing mode active for more towers
            }
        }
    }
}

void Game::DrawTowerUI()
{
    // Draw tower selection panel at bottom
    int panelY = GetScreenHeight() - 90;
    int panelX = 20;
    
    DrawRectangle(panelX - 5, panelY - 5, 360, 80, {0, 0, 0, 150});
    DrawText("Towers: [1] [2] [3]", panelX, panelY, 16, WHITE);
    
    // Tower 1: Coral Cannon
    Color t1Color = (selectedTowerType == TowerType::CORAL_CANNON && placingTower) ? YELLOW : WHITE;
    int cost1 = TowerSystem::GetTowerCost(TowerType::CORAL_CANNON);
    DrawText(TextFormat("[1] Coral Cannon - %dg", cost1), panelX, panelY + 20, 16, t1Color);
    
    // Tower 2: Tidal Burst
    Color t2Color = (selectedTowerType == TowerType::TIDAL_BURST && placingTower) ? YELLOW : WHITE;
    int cost2 = TowerSystem::GetTowerCost(TowerType::TIDAL_BURST);
    DrawText(TextFormat("[2] Tidal Burst - %dg", cost2), panelX + 180, panelY + 20, 16, t2Color);
    
    // Tower 3: Frost Totem
    Color t3Color = (selectedTowerType == TowerType::FROST_TOTEM && placingTower) ? YELLOW : WHITE;
    int cost3 = TowerSystem::GetTowerCost(TowerType::FROST_TOTEM);
    DrawText(TextFormat("[3] Frost Totem - %dg", cost3), panelX, panelY + 40, 16, t3Color);
    
    // Instructions
    if (placingTower) {
        DrawText("Left-click to place, Right-click to cancel", panelX, panelY + 60, 14, LIGHTGRAY);
    } else {
        DrawText("Press 1/2/3 to select tower", panelX, panelY + 60, 14, GRAY);
    }
}
