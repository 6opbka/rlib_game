#pragma once
#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include <iostream>

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

inline bool operator==(const Vector2& a, const Vector2& b) {
    return Vector2Equals(a,b);
}




