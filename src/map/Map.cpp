#include "map/Map.h"
#include <cmath>

Map::Map()
    : gridWidth(0)
    , gridHeight(0)
    , tileSize(64)
    , offsetX(0)
    , offsetY(0)
    , spawnPoint{0, 0}
    , basePoint{0, 0}
{
}

void Map::Init(int screenWidth, int screenHeight) {
    // Calculate grid dimensions
    tileSize = 64;
    gridWidth = screenWidth / tileSize;
    gridHeight = (screenHeight - 100) / tileSize;  // Leave space for UI at bottom
    
    // Center the map
    offsetX = (screenWidth - gridWidth * tileSize) / 2;
    offsetY = 50;  // Leave space at top for HUD
    
    CreateDefaultMap();
    SetupPath();
}

void Map::CreateDefaultMap() {
    // Clear existing tiles
    tiles.clear();
    tiles.resize(gridHeight);
    
    // Initialize all tiles as EMPTY (can place towers)
    for (int y = 0; y < gridHeight; ++y) {
        tiles[y].resize(gridWidth);
        for (int x = 0; x < gridWidth; ++x) {
            Rectangle bounds = {
                static_cast<float>(offsetX + x * tileSize),
                static_cast<float>(offsetY + y * tileSize),
                static_cast<float>(tileSize),
                static_cast<float>(tileSize)
            };
            
            // Default to water background
            tiles[y][x] = Tile(TileType::WATER, x, y, bounds);
        }
    }
    
    // Create buildable areas (sand banks) - areas where towers can be placed
    // These are areas alongside the path
    for (int y = 1; y < gridHeight - 1; ++y) {
        for (int x = 1; x < gridWidth - 1; ++x) {
            // Create scattered buildable areas
            if ((x + y) % 3 == 0 || (x * y) % 5 == 0) {
                tiles[y][x] = Tile(TileType::EMPTY, x, y, tiles[y][x].bounds);
            }
        }
    }
    
    // Define path layout (snake pattern across the screen)
    // Path tiles: enemy walks on these
    int pathY = 4;  // Starting Y position
    
    // Horizontal path from left
    for (int x = 0; x < 4; ++x) {
        tiles[pathY][x] = Tile(TileType::PATH, x, pathY, tiles[pathY][x].bounds);
        // Make adjacent tiles buildable
        if (pathY > 0) tiles[pathY-1][x] = Tile(TileType::EMPTY, x, pathY-1, tiles[pathY-1][x].bounds);
        if (pathY < gridHeight-1) tiles[pathY+1][x] = Tile(TileType::EMPTY, x, pathY+1, tiles[pathY+1][x].bounds);
    }
    
    // Down
    for (int y = pathY; y < pathY + 3 && y < gridHeight; ++y) {
        tiles[y][3] = Tile(TileType::PATH, 3, y, tiles[y][3].bounds);
        if (3 > 0) tiles[y][2] = Tile(TileType::EMPTY, 2, y, tiles[y][2].bounds);
        if (3 < gridWidth-1) tiles[y][4] = Tile(TileType::EMPTY, 4, y, tiles[y][4].bounds);
    }
    pathY += 2;
    
    // Right
    for (int x = 3; x < 8 && x < gridWidth; ++x) {
        tiles[pathY][x] = Tile(TileType::PATH, x, pathY, tiles[pathY][x].bounds);
        if (pathY > 0) tiles[pathY-1][x] = Tile(TileType::EMPTY, x, pathY-1, tiles[pathY-1][x].bounds);
        if (pathY < gridHeight-1) tiles[pathY+1][x] = Tile(TileType::EMPTY, x, pathY+1, tiles[pathY+1][x].bounds);
    }
    
    // Up
    for (int y = pathY; y > pathY - 4 && y >= 0; --y) {
        tiles[y][7] = Tile(TileType::PATH, 7, y, tiles[y][7].bounds);
        if (7 > 0) tiles[y][6] = Tile(TileType::EMPTY, 6, y, tiles[y][6].bounds);
        if (7 < gridWidth-1) tiles[y][8] = Tile(TileType::EMPTY, 8, y, tiles[y][8].bounds);
    }
    pathY -= 3;
    
    // Right
    for (int x = 7; x < 12 && x < gridWidth; ++x) {
        tiles[pathY][x] = Tile(TileType::PATH, x, pathY, tiles[pathY][x].bounds);
        if (pathY > 0) tiles[pathY-1][x] = Tile(TileType::EMPTY, x, pathY-1, tiles[pathY-1][x].bounds);
        if (pathY < gridHeight-1) tiles[pathY+1][x] = Tile(TileType::EMPTY, x, pathY+1, tiles[pathY+1][x].bounds);
    }
    
    // Down
    for (int y = pathY; y < pathY + 5 && y < gridHeight; ++y) {
        tiles[y][11] = Tile(TileType::PATH, 11, y, tiles[y][11].bounds);
        if (11 > 0) tiles[y][10] = Tile(TileType::EMPTY, 10, y, tiles[y][10].bounds);
        if (11 < gridWidth-1) tiles[y][12] = Tile(TileType::EMPTY, 12, y, tiles[y][12].bounds);
    }
    pathY += 4;
    
    // Right to end
    for (int x = 11; x < gridWidth; ++x) {
        tiles[pathY][x] = Tile(TileType::PATH, x, pathY, tiles[pathY][x].bounds);
        if (pathY > 0) tiles[pathY-1][x] = Tile(TileType::EMPTY, x, pathY-1, tiles[pathY-1][x].bounds);
        if (pathY < gridHeight-1) tiles[pathY+1][x] = Tile(TileType::EMPTY, x, pathY+1, tiles[pathY+1][x].bounds);
    }
    
    // Mark spawn and base
    tiles[4][0] = Tile(TileType::SPAWN, 0, 4, tiles[4][0].bounds);
    if (gridWidth > 0 && pathY < gridHeight) {
        tiles[pathY][gridWidth-1] = Tile(TileType::BASE, gridWidth-1, pathY, tiles[pathY][gridWidth-1].bounds);
    }
}

void Map::SetupPath() {
    waypoints.clear();
    
    // Create waypoints based on path tiles
    // Follow the snake pattern
    waypoints.push_back(GetTileCenter(0, 4));   // Start
    waypoints.push_back(GetTileCenter(3, 4));   // First corner
    waypoints.push_back(GetTileCenter(3, 6));   // Down
    waypoints.push_back(GetTileCenter(7, 6));   // Right
    waypoints.push_back(GetTileCenter(7, 3));   // Up
    waypoints.push_back(GetTileCenter(11, 3));  // Right
    waypoints.push_back(GetTileCenter(11, 7));  // Down
    waypoints.push_back(GetTileCenter(gridWidth - 1, 7)); // End (base)
    
    // Set spawn and base points
    spawnPoint = waypoints.front();
    basePoint = waypoints.back();
}

bool Map::LoadFromFile(const char* filename) {
    // TODO: Implement file loading
    (void)filename;
    return false;
}

void Map::Draw() const {
    // Draw all tiles
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            const Tile& tile = tiles[y][x];
            
            // Draw tile background
            DrawRectangleRec(tile.bounds, tile.color);
            
            // Draw special markers
            switch (tile.type) {
                case TileType::PATH:
                    // Draw path decoration (dots)
                    DrawCircle(
                        static_cast<int>(tile.bounds.x + tile.bounds.width / 2),
                        static_cast<int>(tile.bounds.y + tile.bounds.height / 2),
                        3.0f,
                        {100, 90, 80, 255}
                    );
                    break;
                    
                case TileType::SPAWN:
                    // Draw spawn indicator
                    DrawText("S", 
                        static_cast<int>(tile.bounds.x + tile.bounds.width / 2 - 5),
                        static_cast<int>(tile.bounds.y + tile.bounds.height / 2 - 10),
                        20, PURPLE);
                    break;
                    
                case TileType::BASE:
                    // Draw base indicator (Aria's location)
                    DrawText("A",
                        static_cast<int>(tile.bounds.x + tile.bounds.width / 2 - 5),
                        static_cast<int>(tile.bounds.y + tile.bounds.height / 2 - 10),
                        20, GOLD);
                    // Draw protective circle
                    DrawCircleLines(
                        static_cast<int>(tile.bounds.x + tile.bounds.width / 2),
                        static_cast<int>(tile.bounds.y + tile.bounds.height / 2),
                        25.0f, GOLD);
                    break;
                    
                case TileType::EMPTY:
                    // Draw subtle grid pattern for buildable areas
                    DrawRectangleLinesEx(tile.bounds, 1.0f, {40, 70, 100, 100});
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    // Draw path lines connecting waypoints
    for (size_t i = 0; i < waypoints.size() - 1; ++i) {
        DrawLineEx(waypoints[i], waypoints[i + 1], 3.0f, {120, 100, 80, 150});
    }
}

void Map::DrawGrid() const {
    // Draw grid overlay (for debug/placement)
    for (int y = 0; y <= gridHeight; ++y) {
        DrawLine(
            offsetX, offsetY + y * tileSize,
            offsetX + gridWidth * tileSize, offsetY + y * tileSize,
            {255, 255, 255, 50}
        );
    }
    for (int x = 0; x <= gridWidth; ++x) {
        DrawLine(
            offsetX + x * tileSize, offsetY,
            offsetX + x * tileSize, offsetY + gridHeight * tileSize,
            {255, 255, 255, 50}
        );
    }
}

Tile* Map::GetTileAt(Vector2 worldPos) {
    int gx = static_cast<int>((worldPos.x - offsetX) / tileSize);
    int gy = static_cast<int>((worldPos.y - offsetY) / tileSize);
    return GetTileAtGrid(gx, gy);
}

const Tile* Map::GetTileAt(Vector2 worldPos) const {
    int gx = static_cast<int>((worldPos.x - offsetX) / tileSize);
    int gy = static_cast<int>((worldPos.y - offsetY) / tileSize);
    return GetTileAtGrid(gx, gy);
}

Tile* Map::GetTileAtGrid(int gridX, int gridY) {
    if (gridX < 0 || gridX >= gridWidth || gridY < 0 || gridY >= gridHeight) {
        return nullptr;
    }
    return &tiles[gridY][gridX];
}

const Tile* Map::GetTileAtGrid(int gridX, int gridY) const {
    if (gridX < 0 || gridX >= gridWidth || gridY < 0 || gridY >= gridHeight) {
        return nullptr;
    }
    return &tiles[gridY][gridX];
}

bool Map::CanPlaceTower(Vector2 worldPos) const {
    const Tile* tile = GetTileAt(worldPos);
    if (!tile) return false;
    
    // Can only place on EMPTY tiles
    return tile->type == TileType::EMPTY;
}

Vector2 Map::SnapToGrid(Vector2 worldPos) const {
    int gx = static_cast<int>((worldPos.x - offsetX) / tileSize);
    int gy = static_cast<int>((worldPos.y - offsetY) / tileSize);
    
    return GetTileCenter(gx, gy);
}

Vector2 Map::GetTileCenter(int gridX, int gridY) const {
    return {
        static_cast<float>(offsetX + gridX * tileSize + tileSize / 2),
        static_cast<float>(offsetY + gridY * tileSize + tileSize / 2)
    };
}

void Map::Clear() {
    tiles.clear();
    waypoints.clear();
    gridWidth = 0;
    gridHeight = 0;
}
