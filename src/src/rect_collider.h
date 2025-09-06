#pragma once

#include "collider.h"



class RectCollider : public Collider
{
public:
    /* data */
    ColliderShape collider_shape = NONE;
public:
    Vector2 size = {0.0f,0.0f};
    Vector2 scale = {2.0f, 2.0f};

    RectCollider(Vector2 size_, CollisionLayer col_layer,CollisionLayer col_mask);
    ~RectCollider();

    ColliderShape get_col_shape() const override;
    void draw() const override;
    
    Rectangle get_collider_rec() override;
    float get_collider_radius() override;

    std::unique_ptr<Collider> clone() const override;
    void on_parent_added() override;
};
