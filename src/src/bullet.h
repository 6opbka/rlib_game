#pragma once
#include "gameobject.h"

class Bullet : public GameObject
{
private:

public:
    Bullet();
    ~Bullet();




    Bullet(const Bullet& other);

    void move(float delta_time);

    shared_ptr<GameObject> clone() const override;
    void update(float delta_time)override;

    void on_collision() override;
};

