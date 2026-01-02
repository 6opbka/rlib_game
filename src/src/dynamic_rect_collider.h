#pragma once

#include "dynamic_collider.h"
#include "static_collider.h"

class DynamicCircleCollider;



class RectCollider : public DynamicCollider
{
public:
    /* data */
    ColliderShape collider_shape = RECTANGLE;
    Vector2 size = {16.0f,16.0f};
    bool can_draw = false;


    RectCollider(Vector2 size_, CollisionLayer col_layer,CollisionLayer col_mask);
    ~RectCollider();

    bool dynamic_collide (const DynamicCollider& other) override;
    bool static_collide (const StaticCollider& other) override;

    bool collide_with_rect(const RectCollider& other) const override;
    bool collide_with_circle(const DynamicCircleCollider& other)const override;


    bool collide_with_static_line (const StaticLineCollider& other) override;

    Rectangle get_aabb_world() const override;


    ColliderShape get_col_shape() const override;
    // void calc_collider_shape() const override;
    void draw() override;
    
    Rectangle get_collider_rec() const ;
    std::unique_ptr<DynamicCollider> clone() const override;
    void on_parent_added() override;

private:
    // Rectangle mutable rect = {0,0,0,0};
    Vector2 origin = {0,0};
    float rotation = 0.0f;
};
