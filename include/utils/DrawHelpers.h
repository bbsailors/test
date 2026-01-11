#pragma once

#include "raylib.h"
#include <string>

// ============================================================
// Drawing Helper Utilities
// ============================================================

namespace DrawHelpers {

// Draw text centered at a position
inline void DrawTextCentered(const char* text, int x, int y, int fontSize, Color color) {
    int width = MeasureText(text, fontSize);
    DrawText(text, x - width / 2, y - fontSize / 2, fontSize, color);
}

// Draw text centered horizontally
inline void DrawTextCenteredX(const char* text, int y, int fontSize, Color color) {
    int width = MeasureText(text, fontSize);
    int screenWidth = GetScreenWidth();
    DrawText(text, (screenWidth - width) / 2, y, fontSize, color);
}

// Draw progress bar
inline void DrawProgressBar(Rectangle rect, float progress, Color bgColor, Color fgColor, Color borderColor) {
    // Background
    DrawRectangleRec(rect, bgColor);
    
    // Progress fill
    Rectangle fillRect = rect;
    fillRect.width *= progress;
    DrawRectangleRec(fillRect, fgColor);
    
    // Border
    DrawRectangleLinesEx(rect, 2.0f, borderColor);
}

// Draw health bar above an entity
inline void DrawHealthBar(Vector2 position, float width, float height, float healthPercent, float yOffset = -20.0f) {
    Rectangle rect = {
        position.x - width / 2,
        position.y + yOffset,
        width,
        height
    };
    
    Color bgColor = {60, 60, 60, 200};
    Color fgColor = healthPercent > 0.5f ? GREEN : (healthPercent > 0.25f ? YELLOW : RED);
    Color borderColor = {30, 30, 30, 255};
    
    DrawProgressBar(rect, healthPercent, bgColor, fgColor, borderColor);
}

// Draw a button
inline bool DrawButton(Rectangle rect, const char* text, int fontSize, Color bgColor, Color textColor, Color hoverColor) {
    bool isHovered = CheckCollisionPointRec(GetMousePosition(), rect);
    
    DrawRectangleRec(rect, isHovered ? hoverColor : bgColor);
    DrawRectangleLinesEx(rect, 2.0f, textColor);
    
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, 
        static_cast<int>(rect.x + rect.width / 2 - textWidth / 2),
        static_cast<int>(rect.y + rect.height / 2 - fontSize / 2),
        fontSize, textColor);
    
    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// Draw dashed line
inline void DrawDashedLine(Vector2 start, Vector2 end, float dashLength, float gapLength, float thickness, Color color) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float length = sqrtf(dx * dx + dy * dy);
    
    if (length < 0.001f) return;
    
    float dirX = dx / length;
    float dirY = dy / length;
    
    float currentPos = 0.0f;
    bool drawing = true;
    
    while (currentPos < length) {
        float segmentLength = drawing ? dashLength : gapLength;
        if (currentPos + segmentLength > length) {
            segmentLength = length - currentPos;
        }
        
        if (drawing) {
            Vector2 segStart = {start.x + dirX * currentPos, start.y + dirY * currentPos};
            Vector2 segEnd = {start.x + dirX * (currentPos + segmentLength), start.y + dirY * (currentPos + segmentLength)};
            DrawLineEx(segStart, segEnd, thickness, color);
        }
        
        currentPos += segmentLength;
        drawing = !drawing;
    }
}

// Draw circle with glow effect
inline void DrawCircleGlow(Vector2 center, float radius, Color innerColor, Color outerColor) {
    for (int i = 3; i >= 0; --i) {
        float r = radius + i * 3.0f;
        unsigned char alpha = static_cast<unsigned char>(outerColor.a / (i + 1));
        Color c = {outerColor.r, outerColor.g, outerColor.b, alpha};
        DrawCircleV(center, r, c);
    }
    DrawCircleV(center, radius, innerColor);
}

// Draw arrow
inline void DrawArrow(Vector2 from, Vector2 to, float thickness, float headSize, Color color) {
    DrawLineEx(from, to, thickness, color);
    
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float length = sqrtf(dx * dx + dy * dy);
    
    if (length < 0.001f) return;
    
    float dirX = dx / length;
    float dirY = dy / length;
    
    // Arrowhead
    Vector2 left = {
        to.x - dirX * headSize - (-dirY) * headSize * 0.5f,
        to.y - dirY * headSize - dirX * headSize * 0.5f
    };
    Vector2 right = {
        to.x - dirX * headSize + (-dirY) * headSize * 0.5f,
        to.y - dirY * headSize + dirX * headSize * 0.5f
    };
    
    DrawTriangle(to, left, right, color);
}

} // namespace DrawHelpers
