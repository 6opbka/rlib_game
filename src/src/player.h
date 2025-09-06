#pragma once
#include "gameobject.h"
#include "weapon.h"



class Player : public GameObject
{
private:

    Vector2 velocity{0, 0};
    float speed = 300.0f;         
    float acceleration = 1000.0f; 
    float friction = 1200.0f;

    

    
    enum State {
        STANDING,
        MOVING
    } state;

    /* data */
public:
    Player();
    ~Player();

    void input(const float delta_time);
    void update(const float delta_time) override;
    shared_ptr<GameObject> clone() const override;
    void init();
};
