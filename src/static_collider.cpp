#include "src/static_collider.h"


StaticCollider::StaticCollider(CollisionLayer layer, CollisionLayer mask, ColliderShape shape):
    col_layer(layer),
    col_mask(mask),
    col_shape(shape)
{
}

// StaticCollider::~StaticCollider()
// {
// }

