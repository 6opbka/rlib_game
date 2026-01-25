#pragma once
#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include <iostream>
#include <cstdint>

const int spatial_collider_grid_size = 16;

struct Line{
    Vector2 a = {0,0};
    Vector2 b = {0,0};
    float length = 0.0f;
};

struct Cell{
    int x = 0;
    int y = 0;
};

struct AABB{
    Vector2 min{0.0f,0.0f};
    Vector2 max{0.0f,0.0f};
};


struct CellCoordHash {
    std::size_t operator()(const Cell& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
    }
};

struct VecCoordHash {
    std::size_t operator()(const Vector2& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
    }
};

enum CollisionLayer:uint32_t {
    LAYER_NONE = 0,
    LAYER_PLAYER = 1 << 0,
    LAYER_ENEMY = 1 << 1,
    LAYER_BULLET = 1 << 2,
    LAYER_ENEMY_BULLET = 1 << 3,
    LAYER_WALL = 1 << 4,
    LAYER_ALL = 0xFFFFFFFF
};

inline CollisionLayer operator|(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline CollisionLayer operator&(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

enum ColliderShape:uint8_t{
    NONE = 0,
	LINE = 1		<< 0,
    RECTANGLE = 1	<< 1,
    CIRCLE = 1		<< 2
};




inline std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}


inline bool operator==(const Cell& a, const Cell& b) {
    return (a.x==b.x)&&(a.y==b.y);
}


inline bool CheckCollisionLineRec(Vector2 p0, Vector2 p1, Rectangle r)
{
    // 1. Если старт внутри — сразу true
    if (CheckCollisionPointRec(p0, r))
        return true;

    // 2. Broad phase: AABB отрезка
    Rectangle segmentBounds {
        std::min(p0.x, p1.x),
        std::min(p0.y, p1.y),
        std::abs(p1.x - p0.x),
        std::abs(p1.y - p0.y)
    };

    if (!CheckCollisionRecs(segmentBounds, r))
        return false;

    // 3. Узкая фаза: 4 стороны
    float x0 = r.x;
    float y0 = r.y;
    float x1 = r.x + r.width;
    float y1 = r.y + r.height;

    Vector2 tl { x0, y0 };
    Vector2 tr { x1, y0 };
    Vector2 br { x1, y1 };
    Vector2 bl { x0, y1 };

    return
        CheckCollisionLines(p0, p1, tl, tr, nullptr) ||
        CheckCollisionLines(p0, p1, tr, br, nullptr) ||
        CheckCollisionLines(p0, p1, br, bl, nullptr) ||
        CheckCollisionLines(p0, p1, bl, tl, nullptr);
}




