#include "src/collider.h"
#include <iostream>
#include "src/gameobject.h"
using namespace std;

Collider::Collider(CollisionLayer col_layer,CollisionLayer col_mask):
    layer(col_layer),
    mask(col_mask)
{
    
}


void Collider::draw() const {
    if (!parent) {
        cout << "no parent\n";
        return;
    }
    cout<<"No shape, base class!\n";
}

void Collider::on_parent_added(){
    if (!parent) cout<<"no parent"<<endl; return;
    scale = {parent->scale.x * scale.x, parent->scale.y * scale.x};
    cout<<"scale ";
    cout<<scale<<"\n";
}


bool Collider::can_collide_with(const Collider& other){

    return (this->mask & other.layer)!=0;

}

ColliderShape Collider::get_col_shape() const{
    return collider_shape;
}
