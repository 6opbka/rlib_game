#pragma once

#include "dynamic_collider.h"



class RectCollider : public Collider
{
public:
    /* data */
    ColliderShape collider_shape = NONE;
    Vector2 size = {0.0f,0.0f};
    Vector2 scale = {2.0f, 2.0f};
    bool can_draw = false;

    RectCollider(Vector2 size_, CollisionLayer col_layer,CollisionLayer col_mask);
    ~RectCollider();

    ColliderShape get_col_shape() const override;
    void calc_collider_shape() override;
    void draw() override;
    
    Rectangle get_collider_rec() override;
    float get_collider_radius() override;

    std::unique_ptr<Collider> clone() const override;
    void on_parent_added() override;

private:
    Rectangle rect = {0,0,0,0};
    Vector2 origin = {0,0};
    float rotation = 0.0f;
};
