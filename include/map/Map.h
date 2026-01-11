#pragma once

#include "raylib.h"
#include <vector>

// ============================================================
// TileType: Types of tiles on the map
// ============================================================
enum class TileType {
    EMPTY,          // Can place towers here
    PATH,           // Enemy path - cannot place towers
    BLOCKED,        // Decorative/blocked area
    WATER,          // Deep water (background)
    SPAWN,          // Enemy spawn point
    BASE            // Player base (Aria's location)
};

// ============================================================
// Tile: A single map tile
// ============================================================
struct Tile {
    TileType type;
    int gridX;
    int gridY;
    Rectangle bounds;
    Color color;
    
    Tile()
        : type(TileType::EMPTY)
        , gridX(0)
        , gridY(0)
        , bounds{0, 0, 0, 0}
        , color(DARKBLUE)
    {}
    
    Tile(TileType t, int gx, int gy, Rectangle b)
        : type(t)
        , gridX(gx)
        , gridY(gy)
        , bounds(b)
    {
        // Set color based on type
        switch (type) {
            case TileType::EMPTY:
                color = {30, 60, 90, 255};      // Dark blue-gray
                break;
            case TileType::PATH:
                color = {80, 70, 60, 255};      // Sandy path
                break;
            case TileType::BLOCKED:
                color = {20, 40, 60, 255};      // Darker blue
                break;
            case TileType::WATER:
                color = {15, 30, 50, 255};      // Deep water
                break;
            case TileType::SPAWN:
                color = {100, 50, 80, 255};     // Purple-ish
                break;
            case TileType::BASE:
                color = {60, 120, 100, 255};    // Teal
                break;
        }
    }
};

// ============================================================
// Map: Grid-based map for the game
// ============================================================
class Map {
public:
    Map();
    
    // Initialize map with default layout
    void Init(int screenWidth, int screenHeight);
    
    // Load map from file (future feature)
    bool LoadFromFile(const char* filename);
    
    // Drawing
    void Draw() const;
    void DrawGrid() const;
    
    // Tile queries
    Tile* GetTileAt(Vector2 worldPos);
    const Tile* GetTileAt(Vector2 worldPos) const;
    Tile* GetTileAtGrid(int gridX, int gridY);
    const Tile* GetTileAtGrid(int gridX, int gridY) const;
    
    // Placement validation
    bool CanPlaceTower(Vector2 worldPos) const;
    Vector2 SnapToGrid(Vector2 worldPos) const;
    Vector2 GetTileCenter(int gridX, int gridY) const;
    
    // Getters
    int GetGridWidth() const { return gridWidth; }
    int GetGridHeight() const { return gridHeight; }
    int GetTileSize() const { return tileSize; }
    
    // Get path waypoints
    const std::vector<Vector2>& GetWaypoints() const { return waypoints; }
    Vector2 GetSpawnPoint() const { return spawnPoint; }
    Vector2 GetBasePoint() const { return basePoint; }
    
    // Reset
    void Clear();
    
private:
    void CreateDefaultMap();
    void SetupPath();
    
    std::vector<std::vector<Tile>> tiles;
    std::vector<Vector2> waypoints;
    
    int gridWidth;
    int gridHeight;
    int tileSize;
    int offsetX;
    int offsetY;
    
    Vector2 spawnPoint;
    Vector2 basePoint;
};
