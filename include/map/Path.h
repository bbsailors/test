#pragma once

#include "raylib.h"
#include <vector>

// ============================================================
// Path: Manages enemy pathfinding waypoints
// ============================================================
// Simple waypoint-based path for enemies to follow
// ============================================================

class Path {
public:
    Path();
    
    // Set waypoints
    void SetWaypoints(const std::vector<Vector2>& points);
    void AddWaypoint(Vector2 point);
    void ClearWaypoints();
    
    // Get waypoints
    const std::vector<Vector2>& GetWaypoints() const { return waypoints; }
    Vector2 GetWaypoint(int index) const;
    int GetWaypointCount() const { return static_cast<int>(waypoints.size()); }
    
    // Get spawn and end points
    Vector2 GetSpawnPoint() const;
    Vector2 GetEndPoint() const;
    
    // Calculate total path length
    float GetTotalLength() const;
    
    // Get position along path (0.0 = start, 1.0 = end)
    Vector2 GetPositionAlongPath(float t) const;
    
    // Drawing (for debug)
    void Draw(Color color = WHITE, float thickness = 2.0f) const;
    void DrawWaypoints(Color color = YELLOW, float radius = 5.0f) const;
    
    // Validation
    bool IsValid() const { return waypoints.size() >= 2; }
    
private:
    std::vector<Vector2> waypoints;
};

// ============================================================
// PathFollower: Helper for entities following a path
// ============================================================
struct PathFollower {
    int currentWaypointIndex;
    float distanceToNext;
    bool reachedEnd;
    
    PathFollower()
        : currentWaypointIndex(0)
        , distanceToNext(0.0f)
        , reachedEnd(false)
    {}
    
    void Reset() {
        currentWaypointIndex = 0;
        distanceToNext = 0.0f;
        reachedEnd = false;
    }
};
