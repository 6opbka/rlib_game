#include "src/gameobject.h"
#include <iostream>

GameObject::GameObject()
{
    scale = {1.0f, 1.0f};
    rotation = 0.0f;
}

GameObject::GameObject(const GameObject& other) {
    // Copy simple fields
    name = other.name;
    local_position = other.local_position;
    scale = other.scale;
    rotation = other.rotation;
    // ...

    // Deep copy collider (clone it)
    if (other.collider) {

        collider = other.collider->clone();
        collider->parent = shared_from_this();  // clone() returns unique_ptr<Collider>
        
        //CLONE NADO POMENYAT, ne inititsya pointer posle constructora. Nujno postavit' parenta dlya collidera.
    }
    // Note: Don't copy parent or children, typically
    parent.reset();
    children.clear();
}

GameObject::~GameObject()
{
    // UnloadTexture(sprite); // Clean up GPU memory
}


void GameObject::add_child(shared_ptr<GameObject> child) {

    try {
        child->parent = shared_from_this(); // This can throw bad_weak_ptr
        child->added_parent_callback();
        children.push_back(std::move(child));
    } 
    catch(const std::bad_weak_ptr& e) {
        cerr << "Error in add_child: " << e.what() << "\n";
        throw; // Or handle differently
    }
}

void GameObject::remove_child(GameObject *child){

}

void GameObject::added_parent_callback() {
    auto p = parent.lock();
    if (!p) return;

    scale = {
        p->scale.x * scale.x,
        p->scale.y * scale.y
    };
}

void GameObject::render(const float delta_time)
{   

    draw_sprites();

    for (auto& child : children) {
            child->render(delta_time);
        }

}

void GameObject::draw_sprites(){
    if(sprite) sprite->draw();
    if(animated_sprite) animated_sprite->draw();
    if(collider) collider->draw();
}

void GameObject::add_sprite(shared_ptr<Sprite> sprite_){
    if(animated_sprite) {cout<<"animated_sprite already exists\n"; return;}
    sprite = sprite_;
    
    sprite->parent = shared_from_this();
    sprite->on_parent_added();
    
}

void GameObject::add_animated_sprite(shared_ptr<AnimatedSprite> sprite_){
    if(sprite) {cout<<"sprite already exists\n"; return;}
    animated_sprite = sprite_;
    
    animated_sprite->parent = shared_from_this();
    animated_sprite->on_parent_added();
    
}

void GameObject::add_collider(unique_ptr<Collider> collider_){
        cout<<"adding collider: "<<name<<"\n";
        //VSYE PO PIZDE POSHLO )) ) )) ) )) 000 )))
        collider = move(collider_);
        collider->parent = shared_from_this();
        cout<<"calling on parent collider\n";
        collider->on_parent_added();
}

void GameObject::check_collision_dynamic(GameObject& target) {
    if(!collider){
        cout<<"no collider\n";
        return;
    }

    if(!target.collider){
        cout<<"target has no collider\n";
        return;
    }
    collider->calc_collider_shape();
    bool collision_detected = false;
    if(collider->can_collide_with(*target.collider)){
        ColliderShape this_shape = collider->get_col_shape();
        ColliderShape target_shape = target.collider->get_col_shape();

        if(this_shape == RECTANGLE){
            if(target_shape == RECTANGLE){
                if(CheckCollisionRecs(collider->get_collider_rec(),target.collider->get_collider_rec())){
                    collision_detected = true;
                }
            }
            else if(target_shape == CIRCLE){
                if(CheckCollisionCircleRec(target.local_position, target.collider->get_collider_radius(), collider->get_collider_rec())){
                    collision_detected = true;
                }
            }
        }
        else if(this_shape == CIRCLE){
            if(target_shape == RECTANGLE){
                if(CheckCollisionCircleRec(local_position, collider->get_collider_radius(), target.collider->get_collider_rec())){
                    collision_detected = true;
                }
            }
            else if(target_shape == CIRCLE){
                if(CheckCollisionCircles(local_position, collider->get_collider_radius(), target.local_position, target.collider->get_collider_radius())){
                    collision_detected = true;
                }
            }
        }
    }
    if(collision_detected){
        this->on_collision();
        target.on_collision();

    }
}

void GameObject::check_collision_static(const Line& edge){
    if(!collider){
        cout<<"no collider\n";
        return;
    }
}


Vector2 GameObject::get_world_position() const {
    auto p = parent.lock();
    if (p){
        return p->get_world_position() + local_position;
    } 
    return local_position;
}

void GameObject::update(const float delta_time){
    if(animated_sprite) animated_sprite->update(delta_time);
    for (auto& child : children) {
            child->update(delta_time);
        }
}

std::shared_ptr<GameObject> GameObject::clone() const {
    return std::make_shared<GameObject>(*this);
}

void GameObject::on_collision(){
    // cout<<"colliding"<<endl;
}

shared_ptr<GameRoot> GameObject::get_root(){
    auto r = root.lock();
    if(!r) return NULL;
    return r;
}
