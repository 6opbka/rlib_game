#include "src/static_line_collider.h"

StaticLineCollider::StaticLineCollider(CollisionLayer layer, CollisionLayer mask, ColliderShape shape, Line line_coords_):
StaticCollider(layer,mask,shape),
line_coords(line_coords_)
{
    
}

StaticLineCollider::~StaticLineCollider ()
{
}
Line StaticLineCollider::get_col_line() const{
    return line_coords;
}