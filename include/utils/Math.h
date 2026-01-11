#pragma once

#include "raylib.h"
#include <cmath>

// ============================================================
// Math Utilities
// ============================================================

namespace MathUtils {

// Vector operations
inline float Length(Vector2 v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

inline float LengthSquared(Vector2 v) {
    return v.x * v.x + v.y * v.y;
}

inline Vector2 Normalize(Vector2 v) {
    float len = Length(v);
    if (len < 0.0001f) return {0, 0};
    return {v.x / len, v.y / len};
}

inline float Distance(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

inline float DistanceSquared(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return dx * dx + dy * dy;
}

inline Vector2 Direction(Vector2 from, Vector2 to) {
    return Normalize({to.x - from.x, to.y - from.y});
}

inline float Dot(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

inline Vector2 Lerp(Vector2 a, Vector2 b, float t) {
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
}

inline float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Angle operations
inline float ToRadians(float degrees) {
    return degrees * (PI / 180.0f);
}

inline float ToDegrees(float radians) {
    return radians * (180.0f / PI);
}

inline float AngleBetween(Vector2 a, Vector2 b) {
    return std::atan2(b.y - a.y, b.x - a.x);
}

// Clamping
inline float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

inline int Clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Random
inline float RandomFloat(float min, float max) {
    return min + static_cast<float>(GetRandomValue(0, 10000)) / 10000.0f * (max - min);
}

inline Vector2 RandomPointInCircle(Vector2 center, float radius) {
    float angle = RandomFloat(0.0f, 2.0f * PI);
    float r = RandomFloat(0.0f, radius);
    return {
        center.x + std::cos(angle) * r,
        center.y + std::sin(angle) * r
    };
}

// Collision
inline bool PointInRect(Vector2 point, Rectangle rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.width &&
           point.y >= rect.y && point.y <= rect.y + rect.height;
}

inline bool PointInCircle(Vector2 point, Vector2 center, float radius) {
    return DistanceSquared(point, center) <= radius * radius;
}

inline bool CirclesOverlap(Vector2 c1, float r1, Vector2 c2, float r2) {
    float radiiSum = r1 + r2;
    return DistanceSquared(c1, c2) <= radiiSum * radiiSum;
}

} // namespace MathUtils
