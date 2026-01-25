#include "src/dynamic_circle_collider.h"
#include "src/static_line_collider.h"
#include "src/gameobject.h"



DynamicCircleCollider::DynamicCircleCollider(Vector2 size_, CollisionLayer col_layer, CollisionLayer col_mask):
    radius(size_.x),
    DynamicCollider(col_layer,col_mask)
{
}

DynamicCircleCollider::~DynamicCircleCollider()
{
}

void DynamicCircleCollider::draw() {
    if(auto p = get_parent()){
        DrawCircleLinesV(p->get_world_position(), radius, RED);
        DrawCircleV(p->get_world_position(), 4, BLUE);
    }
}

Vector2 DynamicCircleCollider::get_position()const{
    auto p = get_parent();
    if(!p) return {0,0};
    
    Vector2 pos = p->get_world_position();
    float size_ = radius*p->scale.x;

    return {
        pos.x + offset.x * p->scale.x - size_ * 0.5f,
        pos.y + offset.y * p->scale.y - size_ * 0.5f,
    };
}


float DynamicCircleCollider::get_radius()const{
    return radius;
}


bool DynamicCircleCollider::dynamic_collide(const DynamicCollider& other){
    if (!can_collide_with(other)) return false;
    return other.collide_with_circle(*this); 
}
bool DynamicCircleCollider::static_collide(const StaticCollider&other){
    if(!can_collide_with(other)) return false;

    switch (other.col_shape)
    {
    case LINE:
         return collide_with_static_line(static_cast<const StaticLineCollider&>(other));
        break;
    default:
        break;
    }
    return false;
}

bool DynamicCircleCollider::collide_with_rect(const RectCollider&other) const{
    return CheckCollisionCircleRec(get_position(), radius, other.get_collider_rec());

}
bool DynamicCircleCollider::collide_with_circle(const DynamicCircleCollider&other)const{
    return CheckCollisionCircles(other.get_position(),other.get_radius(),get_position(),radius);
}
bool DynamicCircleCollider::collide_with_static_line(const StaticLineCollider&other){
    Line line = other.get_col_line();
    return CheckCollisionCircleLine(get_position(),radius,line.a,line.b);
}

std::unique_ptr<DynamicCollider> DynamicCircleCollider::clone() const{
    auto new_collider = std::make_unique<DynamicCircleCollider>(*this);
    new_collider->layer = this->layer;
    new_collider->mask = this->mask;
    return new_collider; // Performs a shallow copy
}

Rectangle DynamicCircleCollider::get_aabb_world() const {
    if(auto p = get_parent()){
        Vector2 world_pos = p->get_world_position();
        return {
            world_pos.x - radius,
            world_pos.y - radius,
            radius * 2.0f,
            radius * 2.0f
        };
    }
    return {0,0,0,0};
}
