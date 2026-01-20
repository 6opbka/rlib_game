#include "src/player.h"
#include "src/utility.hpp"
#include <iostream>

Player::Player(){


}
Player::~Player(){}
using namespace std;

void Player::input(const float delta_time) {
    Vector2 direction = {0, 0};

    // Handle input
    if (IsKeyDown(KEY_D)) {
       direction.x += 1.0f; 
       animated_sprite->flip_x = false;
    }
    if (IsKeyDown(KEY_A)) {
        direction.x -= 1.0f;
        animated_sprite->flip_x = true;
    }
    if (IsKeyDown(KEY_S)) direction.y += 1.0f;
    if (IsKeyDown(KEY_W)) direction.y -= 1.0f;

    // Movement input
    if (direction.x != 0.0f || direction.y != 0.0f) {
        direction = Vector2Normalize(direction);

        velocity += direction * acceleration * delta_time;

        float vel_len = Vector2Length(velocity);
        if (vel_len > speed) {
            velocity = Vector2Normalize(velocity) * speed;
        }
    } else {
        float vel_len = Vector2Length(velocity);
        if (vel_len > 0.0f) {
            Vector2 frictionForce = Vector2Normalize(velocity) * friction * delta_time;
            

            if (Vector2Length(frictionForce) > vel_len) {
                velocity = {0.0f, 0.0f};
            } else {
                velocity -= frictionForce;
            }
        }
    }

    state = (Vector2Length(direction)!=0) ? MOVING : STANDING;

    local_position += velocity * delta_time;
}

shared_ptr<GameObject> Player::clone() const{
    return nullptr;
}

void Player::update(const float delta_time) {

    GameObject::update(delta_time);
    input(delta_time);
    // cout<<"pois:"<<local_position.x<<" "<<local_position.y<<endl;
    
    // cout<<(sprite!=nullptr)<<"\n"
    if(animated_sprite!=nullptr) {
        animated_sprite->current_state = state;

    // cout<<sprite->current_state<<"\n";
    }
    
}

void Player::init(){
    state = STANDING;
    if(animated_sprite) animated_sprite->looping = true;
}


