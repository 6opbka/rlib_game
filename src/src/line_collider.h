#pragma once
#include "static_collider.h"

class LineCollider:public StaticCollider
{
private:
    /* data */
public:
    Line line_coords = {{0.0f,0.0f},{0.0f,0.0f}};
    LineCollider(CollisionLayer layer, CollisionLayer mask, ColliderShape shape, Line line_coords_);
    ~LineCollider();
};
