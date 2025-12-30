#include "src/rect_collider.h"
#include "src/static_line_collider.h"
#include "src/gameobject.h"


RectCollider::RectCollider(Vector2 size_, CollisionLayer col_layer,CollisionLayer col_mask):
    size(size_),
    DynamicCollider(col_layer,col_mask)
{
    collider_shape = RECTANGLE;
}

RectCollider::~RectCollider(){}



Rectangle RectCollider::get_collider_rec()const {
    Vector2 origin = {0, 0};
    if (parent->sprite) {
        Vector2 sprite_size = parent->sprite->sprite_size;
        origin = {sprite_size.x * scale.x, sprite_size.y * scale.y};
    }
    float rotation = parent->rotation; // угол в градусах
    Rectangle ret = {
        parent->local_position.x - origin.x,
        parent->local_position.y - origin.y,
        size.x * scale.x,
        size.y * scale.y
    };
    // DrawRectanglePro(ret, {0,0}, rotation, RED);
    return {
        parent->local_position.x - origin.x,
        parent->local_position.y - origin.y,
        size.x * scale.x,
        size.y * scale.y
    };
}

ColliderShape RectCollider::get_col_shape()const{
    return collider_shape;
}

std::unique_ptr<DynamicCollider> RectCollider::clone() const{
    auto new_collider = std::make_unique<RectCollider>(*this);
    new_collider->layer = this->layer;
    new_collider->mask = this->mask;
    return new_collider; // Performs a shallow copy
}

void RectCollider::calc_collider_shape() {
    if (!parent) {
        cout << "no parent\n";
        return;
    }
    // cout<<Vector2{size.x*scale.x,size.y*scale.y}<<endl;
    
    Vector2 world_pos = parent->local_position; // если есть такой метод
    rotation = parent->rotation; // угол в градусах
    rect = {world_pos.x, world_pos.y, size.x * scale.x, size.y * scale.y};

    Vector2 parent_sprite_size = {0,0};
    if(parent->sprite) parent_sprite_size = parent->sprite->sprite_size;
    if(parent->animated_sprite) parent_sprite_size = parent->animated_sprite->sprite_size;

    origin = {parent_sprite_size.x*scale.x,parent_sprite_size.y*scale.y}; // если нужно смещение центра
    
    can_draw = true;
    
}

void RectCollider::draw(){
    if(!can_draw) return;
    Color transparent_green = {0,128,0,23};
    DrawRectanglePro(rect, origin, rotation, transparent_green);
    can_draw = false;
}

void RectCollider::on_parent_added(){
    if (!parent) {cout<<"no parent"<<endl; return;}
    scale = {parent->scale.x * scale.x, parent->scale.y * scale.x};
    cout<<scale<<"\n";
}

bool RectCollider::dynamic_collide (const DynamicCollider& other) {
    calc_collider_shape();

    if(!can_collide_with(other)) return false;

    if(auto* rect = dynamic_cast<const RectCollider*>(&other)){
        return CheckCollisionRecs(get_collider_rec(), rect->get_collider_rec());
    }
    
    return false;
}

bool RectCollider::static_collide (const StaticCollider& other) {
    calc_collider_shape();

    if(!can_collide_with(other)) return false;

    if(auto* line = dynamic_cast<const StaticLineCollider*>(&other)){
        Line coords = line->get_col_line();
        return CheckCollisionLineRec(coords.a,coords.b,get_collider_rec());
    }
    
    return false;
}