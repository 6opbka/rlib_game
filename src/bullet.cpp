#include "src/bullet.h"


Bullet::Bullet(){
    name = "bullet";
    scale = {.5f,.5f};
}
Bullet::~Bullet(){}

std::shared_ptr<GameObject> Bullet::clone() const {
    auto new_bullet = std::make_shared<Bullet>(*this);  // this is now safe

    if (new_bullet->sprite) {
        new_bullet->sprite->parent = new_bullet;
    }

    if (new_bullet->collider) {
        new_bullet->collider->parent = new_bullet;  // now shared_from_this is valid
    }

    return new_bullet;
}



void Bullet::update(float delta_time){
    move(delta_time);
    GameObject::update(delta_time);
    // check_collision();
}

void Bullet::move (float delta_time){
    Vector2 direction;
    
    direction.x = cosf(rotation*PI/180.0f);
    direction.y = sinf(rotation*PI/180.0f);

    local_position += direction*1000*delta_time;
}

Bullet::Bullet(const Bullet& other) {
    this->name = other.name;
    this->scale = other.scale;
    this->rotation = other.rotation;
    this->local_position = other.local_position;
    this->collider_size = other.collider_size;

    if (other.sprite) {
        this->sprite = other.sprite->clone();  // no parent setting here
    }

    if (other.collider) {
        this->collider = other.collider->clone();  // no parent setting here
    }
}

void Bullet::on_collision() {
    marked_for_deletion = true;

    // cout<<"ya blya sha zahuyaru sebya\n"
}


