#include "map/Path.h"
#include <cmath>

Path::Path() {
}

void Path::SetWaypoints(const std::vector<Vector2>& points) {
    waypoints = points;
}

void Path::AddWaypoint(Vector2 point) {
    waypoints.push_back(point);
}

void Path::ClearWaypoints() {
    waypoints.clear();
}

Vector2 Path::GetWaypoint(int index) const {
    if (index < 0 || index >= static_cast<int>(waypoints.size())) {
        return {0, 0};
    }
    return waypoints[index];
}

Vector2 Path::GetSpawnPoint() const {
    if (waypoints.empty()) return {0, 0};
    return waypoints.front();
}

Vector2 Path::GetEndPoint() const {
    if (waypoints.empty()) return {0, 0};
    return waypoints.back();
}

float Path::GetTotalLength() const {
    if (waypoints.size() < 2) return 0.0f;
    
    float total = 0.0f;
    for (size_t i = 0; i < waypoints.size() - 1; ++i) {
        float dx = waypoints[i + 1].x - waypoints[i].x;
        float dy = waypoints[i + 1].y - waypoints[i].y;
        total += std::sqrt(dx * dx + dy * dy);
    }
    return total;
}

Vector2 Path::GetPositionAlongPath(float t) const {
    if (waypoints.empty()) return {0, 0};
    if (waypoints.size() == 1) return waypoints[0];
    if (t <= 0.0f) return waypoints.front();
    if (t >= 1.0f) return waypoints.back();
    
    float totalLength = GetTotalLength();
    float targetDist = t * totalLength;
    float currentDist = 0.0f;
    
    for (size_t i = 0; i < waypoints.size() - 1; ++i) {
        float dx = waypoints[i + 1].x - waypoints[i].x;
        float dy = waypoints[i + 1].y - waypoints[i].y;
        float segmentLength = std::sqrt(dx * dx + dy * dy);
        
        if (currentDist + segmentLength >= targetDist) {
            // Target is on this segment
            float segT = (targetDist - currentDist) / segmentLength;
            return {
                waypoints[i].x + dx * segT,
                waypoints[i].y + dy * segT
            };
        }
        
        currentDist += segmentLength;
    }
    
    return waypoints.back();
}

void Path::Draw(Color color, float thickness) const {
    if (waypoints.size() < 2) return;
    
    for (size_t i = 0; i < waypoints.size() - 1; ++i) {
        DrawLineEx(waypoints[i], waypoints[i + 1], thickness, color);
    }
}

void Path::DrawWaypoints(Color color, float radius) const {
    for (size_t i = 0; i < waypoints.size(); ++i) {
        DrawCircleV(waypoints[i], radius, color);
        
        // Draw index number
        DrawText(TextFormat("%d", (int)i), 
            static_cast<int>(waypoints[i].x - 5),
            static_cast<int>(waypoints[i].y - 20),
            16, color);
    }
}
