#include "src/camera.h"


Cam::Cam(shared_ptr<GameObject> target_, Vector2 offset_){
    camera.target = target_->local_position;
    camera.offset = offset_;
    camera.rotation = this->rotation;
    camera.zoom = this->zoom;
}

Cam::~Cam(){}


void Cam::update(const float delta_time){
    if(!target) {cout<<"no target"<<endl; return;}
    Vector2 pos = target->local_position;
    camera.target = pos;
}

void Cam::cam_begin(){
    BeginMode2D(camera);
}

void Cam::cam_end(){
    EndMode2D();
}

void Cam::add_target(shared_ptr<GameObject> target_){
    target = target_;
}