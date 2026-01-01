#pragma once
#include "dynamic_collider.h"

class DynamicCircleCollider:public DynamicCollider
{
private:
    /* data */
public:
    float radius = 1.0f;
    ColliderShape collider_shape = CIRCLE;
    

    void draw();
    Vector2 get_position()const;
    float get_radius()const;
    DynamicCircleCollider(Vector2, CollisionLayer, CollisionLayer);
    ~DynamicCircleCollider();

    bool dynamic_collide(const DynamicCollider& other);
    bool static_collide(const StaticCollider& other);

    bool collide_with_rect(const RectCollider& other) const;
    bool collide_with_circle(const DynamicCircleCollider& other)const;
    bool collide_with_static_line(const StaticLineCollider& other);

    std::unique_ptr<DynamicCollider> clone() const;
};
