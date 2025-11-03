#include "src/player.h"
#include "src/utility.hpp"
#include <iostream>

Player::Player(){
    scale = {1.0f,1.0f};

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
        direction = vector2_normalize(direction);

        velocity += direction * acceleration * delta_time;

        float vel_len = vector2_length(velocity);
        if (vel_len > speed) {
            velocity = vector2_normalize(velocity) * speed;
        }
    } else {
        float vel_len = vector2_length(velocity);
        if (vel_len > 0.0f) {
            Vector2 frictionForce = vector2_normalize(velocity) * friction * delta_time;
            

            if (vector2_length(frictionForce) > vel_len) {
                velocity = {0.0f, 0.0f};
            } else {
                velocity -= frictionForce;
            }
        }
    }

    state = (vector2_length(direction)!=0) ? MOVING : STANDING;

    local_position += velocity * delta_time;
}

shared_ptr<GameObject> Player::clone() const{

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


