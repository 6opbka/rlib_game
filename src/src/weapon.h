#pragma once
#include "gameobject.h"
#include "bullet.h"
#include "utility.hpp"

class Weapon : public GameObject
{
private:
    /* data */
public:
    Weapon();
    Weapon(const Weapon& other);

    ~Weapon();

    Vector2 direction = {0,0};
    float barrel_offset = 90.0f;

    shared_ptr<Bullet> bullet_template;
    

    void initialize(shared_ptr<GameRoot> root, shared_ptr<Bullet> bullet);

    void read_input();

    void shoot();

    void update(const float delta_time);
    void follow_mouse();
    shared_ptr<GameObject> clone() const override;
};