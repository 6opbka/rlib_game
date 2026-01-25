#include "src/static_collider.h"


StaticCollider::StaticCollider(CollisionLayer layer, CollisionLayer mask, ColliderShape shape, AABB aabb_):
    col_layer(layer),
    col_mask(mask),
    col_shape(shape)
{
}

AABB StaticCollider::get_aabb(){
    return aabb;
}


// StaticCollider::~StaticCollider()
// {
// }

