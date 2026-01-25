#pragma once
#include "static_collider.h"

class StaticLineCollider:public StaticCollider
{
private:
    /* data */
public:
    Line line_coords = {{0.0f,0.0f},{0.0f,0.0f}};
    
    StaticLineCollider(CollisionLayer layer, CollisionLayer mask, ColliderShape shape, Line line_coords_, AABB aabb_);
    ~StaticLineCollider();
    Line get_col_line() const;
};
