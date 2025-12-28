#pragma once
#include "raylib.h"
#include <cstddef>  
#include <unordered_map>

struct CellCoordHash {
    std::size_t operator()(const Vector2& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
    }
};