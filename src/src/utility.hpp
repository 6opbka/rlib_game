#pragma once
#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include <iostream>

const int spatial_collider_grid_size = 64;

struct Line{
    Vector2 a = {0,0};
    Vector2 b = {0,0};
};

struct CellCoordHash {
    std::size_t operator()(const Vector2& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
    }
};

enum CollisionLayer :uint32_t {
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
    RECTANGLE = 1 << 0,
    CIRCLE = 1 << 1
};



Vector2 operator+(const Vector2 &l, const Vector2 &r) {
    return {l.x + r.x, l.y + r.y};
}

Vector2 operator-(const Vector2 &l, const Vector2 &r) {
    return {l.x - r.x, l.y - r.y};
}


// Scalar multiplication
Vector2 operator*(const Vector2 &l, float r) {
    return {l.x * r, l.y * r};
}

// Scalar division
Vector2 operator/(const Vector2& l, float r) {
    if (r == 0.0f) return {0.0f, 0.0f};
    return {l.x / r, l.y / r};
}



// Vector length
float vector2_length(const Vector2 vec) {
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

// Normalization
Vector2 vector2_normalize(const Vector2 vec) {
    float len = vector2_length(vec);
    return (len != 0.0f) ? vec / len : Vector2{0.0f, 0.0f};
}

// Compound addition
Vector2& operator+=(Vector2& left, const Vector2& right) {
    left.x += right.x;
    left.y += right.y;
    return left;
}

// Compound subtraction
Vector2& operator-=(Vector2& left, const Vector2& right) {
    left.x -= right.x;
    left.y -= right.y;
    return left;
}

float operator/ (const Vector2& left, const Vector2&right){
    return vector2_length(left)/vector2_length(right);
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

bool operator==(const Vector2& a, const Vector2& b) {
    return Vector2Equals(a,b);
}



