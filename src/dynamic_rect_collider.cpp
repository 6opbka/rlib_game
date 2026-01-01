#include "src/dynamic_rect_collider.h"
#include "src/static_line_collider.h"
#include "src/dynamic_circle_collider.h"
#include "src/gameobject.h"


RectCollider::RectCollider(Vector2 size_, CollisionLayer col_layer,CollisionLayer col_mask):
    size(size_),
    DynamicCollider(col_layer,col_mask)
{
    collider_shape = RECTANGLE;
}

RectCollider::~RectCollider(){}



Rectangle RectCollider::get_collider_rec() const {
    Vector2 world = parent->get_world_position();

    Vector2 scaledSize = {
        size.x * parent->scale.x,
        size.y * parent->scale.y
    };

    Vector2 scaledOffset = {
        offset.x * parent->scale.x,
        offset.y * parent->scale.y
    };

    Vector2 center = {
        world.x + scaledOffset.x,
        world.y + scaledOffset.y
    };

    return {
        center.x - scaledSize.x * 0.5f,
        center.y - scaledSize.y * 0.5f,
        scaledSize.x,
        scaledSize.y
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



void RectCollider::draw() {
    auto r = get_collider_rec();
    DrawCircleV({r.x, r.y}, 3, GREEN);               // top-left
    DrawCircleV({r.x+r.width, r.y+r.height}, 3, BLUE); // bottom-right
    DrawCircleV(parent->get_world_position(), 3, YELLOW); // центр
    DrawRectangleLinesEx(get_collider_rec(), 1.0f, RED);
    // DrawCircleV(parent->get_world_position(), 4, BLUE);

}

void RectCollider::on_parent_added(){
    if (!parent) {cout<<"no parent"<<endl; return;}
    // scale = {parent->scale.x * scale.x, parent->scale.y * scale.y};
    // cout<<scale<<"\n";
}

bool RectCollider::dynamic_collide(const DynamicCollider& other) {
    if (!can_collide_with(other)) return false;
    return other.collide_with_rect(*this);
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


bool RectCollider::collide_with_rect(const RectCollider& other) const {
    return CheckCollisionRecs(
        get_collider_rec(),
        other.get_collider_rec()
    );
}


bool RectCollider::collide_with_circle(const DynamicCircleCollider& other) const{
    return CheckCollisionCircleRec(other.get_position(), other.get_radius(), get_collider_rec());
}

bool RectCollider::collide_with_static_line (const StaticLineCollider& other) {
    return false;
}

