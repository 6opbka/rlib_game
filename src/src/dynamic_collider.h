#pragma once
#include "raylib.h"
#include <memory>
#include "utility.hpp"


class GameObject;



class Collider {
public:
    Collider() = default;
    Collider(CollisionLayer col_layer,CollisionLayer col_mask);
    virtual ~Collider() = default;


    CollisionLayer layer = LAYER_NONE;
    CollisionLayer mask = LAYER_ALL;
 
    ColliderShape collider_shape = NONE;


    Vector2 position = {0.0f, 0.0f};
    Vector2 scale = {2.0f, 2.0f};
    Vector2 offset = {0.0f, 0.0f};

    std::shared_ptr<GameObject> parent;

    bool can_collide_with (const Collider& other);
    

    virtual void calc_collider_shape();
    virtual void on_parent_added();
    virtual void draw() = 0;

    virtual ColliderShape get_col_shape() const;

    virtual Rectangle get_collider_rec() = 0;  

    virtual float get_collider_radius() = 0;         

    virtual std::unique_ptr<Collider> clone() const = 0;
};
