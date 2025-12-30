#include "src/line_collider.h"

LineCollider::LineCollider(CollisionLayer layer, CollisionLayer mask, ColliderShape shape, Line line_coords_):
StaticCollider(layer,mask,shape),
line_coords(line_coords_)
{
    
}

LineCollider::~LineCollider ()
{
}