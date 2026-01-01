#pragma once
#include "raylib.h"
#include <memory>
#include "utility.hpp"
#include "static_collider.h"



class GameObject;
class RectCollider;
class DynamicCircleCollider;

class StaticLineCollider;




class DynamicCollider {
public:
    DynamicCollider() = default;
    DynamicCollider(CollisionLayer col_layer,CollisionLayer col_mask);
    virtual ~DynamicCollider() = default;


    CollisionLayer layer = LAYER_NONE;
    CollisionLayer mask = LAYER_ALL;
 
    ColliderShape collider_shape = NONE;


    Vector2 position = {0.0f, 0.0f};
    Vector2 offset = {0.0f, 0.0f};

    std::shared_ptr<GameObject> parent;

    bool can_collide_with (const DynamicCollider& other)const ;
    bool can_collide_with (const StaticCollider& other)const;


    virtual bool dynamic_collide(const DynamicCollider& other) = 0;
    virtual bool static_collide (const StaticCollider& other) = 0;

    virtual bool collide_with_rect(const RectCollider& other)const = 0;
    virtual bool collide_with_circle(const DynamicCircleCollider& other)const= 0;
    
    virtual bool collide_with_static_line (const StaticLineCollider& other) = 0;


    virtual void calc_collider_shape() const ;
    virtual void on_parent_added();
    virtual void draw() = 0;

    virtual ColliderShape get_col_shape() const;    

    virtual std::unique_ptr<DynamicCollider> clone() const = 0;
};
