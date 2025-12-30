#pragma once
#include "utility.hpp"

class StaticCollider
{
private:
    /* data */
public:
    CollisionLayer col_layer = LAYER_NONE;
    CollisionLayer col_mask = LAYER_ALL;

    ColliderShape col_shape = NONE;

    Vector2 position = {0.0f,0.0f};
    Vector2 scale = {2.0f,2.0f};
    Vector2 offset = {0.0f,0.0f};
    
    StaticCollider(CollisionLayer layer, CollisionLayer mask, ColliderShape shape);
    virtual ~StaticCollider() = default;
};

