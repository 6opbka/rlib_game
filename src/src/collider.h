#pragma once
#include "raylib.h"
#include <memory>


class GameObject;

enum CollisionLayer :uint32_t {
    LAYER_NONE = 0,
    LAYER_PLAYER = 1 << 0,
    LAYER_ENEMY = 1 << 1,
    LAYER_BULLET = 1 << 2,
    LAYER_ENEMY_BULLET = 1 << 3,
    LAYER_WALL = 1 << 4,
    LAYER_ALL = 0xFFFFFFFF
};

inline CollisionLayer operator|(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline CollisionLayer operator&(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

enum ColliderShape:uint8_t{
    NONE = 0,
    RECTANGLE = 1 << 0,
    CIRCLE = 1 << 1
};


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
