#pragma once
#include "raylib.h"
#include "raymath.h"
#include "math.h"
#include <iostream>

const int spatial_collider_grid_size = 128;


Vector2 operator+(const Vector2 &l, const Vector2 &r);
Vector2 operator-(const Vector2 &l, const Vector2 &r);
Vector2 operator*(const Vector2 &l, float r);
Vector2 operator/(const Vector2& l, float r);
float vector2_length(const Vector2 vec);
Vector2 vector2_normalize(const Vector2 vec);
Vector2& operator+=(Vector2& left, const Vector2& right);
Vector2& operator-=(Vector2& left, const Vector2& right);
float operator/ (const Vector2& left, const Vector2&right);
std::ostream& operator<<(std::ostream& os, const Vector2& vec);
bool operator==(const Vector2& a, const Vector2& b);





