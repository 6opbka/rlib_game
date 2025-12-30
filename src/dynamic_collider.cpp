#include "src/dynamic_collider.h"
#include "src/static_collider.h"
#include <iostream>
#include "src/gameobject.h"
using namespace std;

DynamicCollider::DynamicCollider(CollisionLayer col_layer,CollisionLayer col_mask):
    layer(col_layer),
    mask(col_mask)
{}


void DynamicCollider::calc_collider_shape(){
    if (!parent) {
        cout << "no parent\n";
        return;
    }
    cout<<"No shape, base class!\n";
}

void DynamicCollider::on_parent_added(){
    if (!parent) cout<<"no parent"<<endl; return;
    scale = {parent->scale.x * scale.x, parent->scale.y * scale.x};
    cout<<"scale ";
    cout<<scale<<"\n";
}


bool DynamicCollider::can_collide_with(const DynamicCollider& other){

    return (this->mask & other.layer)!=0;

}

bool DynamicCollider::can_collide_with(const StaticCollider& other){

    return (this->mask & other.col_layer)!=0;

}

ColliderShape DynamicCollider::get_col_shape() const{
    return collider_shape;
}
