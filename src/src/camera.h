#pragma once

#include "raylib.h"
#include "gameobject.h"
#include <memory>


class Cam
{
private:
    /* data */
public:

    Camera2D camera ={0};
    Vector2 offset = {0.0f,0.0f};
    float rotation = 0.0f;
    float zoom = 3.0f;

    shared_ptr<GameObject> target;

    void update(const float delta_time);
    void add_target(shared_ptr<GameObject> target_);

    void cam_begin();
    void cam_end();
    
    Cam(shared_ptr<GameObject> target_, Vector2 offset_);
    ~Cam();


};





