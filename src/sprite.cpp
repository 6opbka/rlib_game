#include "src/sprite.h"
#include <iostream>
#include "src/gameobject.h"

using namespace std;

Sprite::Sprite(std::shared_ptr<Texture2D> texture_,
     Vector2 pos_,
     Vector2 sprite_size_,
     Vector2 origin_offset_)
{
    texture = texture_;
    
    sprite_size = sprite_size_;
    origin_offset = origin_offset_;
    pos_in_spritesheet = pos_;
    sprite_rect = {pos_in_spritesheet.x,pos_in_spritesheet.y,sprite_size.x,sprite_size.y};
}

Sprite::~Sprite(){
    // UnloadTexture(*texture);
}

void Sprite::load_texture(const char* path){

}

void Sprite::draw(){
     if (texture->id == 0) {
        cerr << "Invalid sprite in: " <<  "\n";
        return;
    }
    
    if(parent){
        rotation = parent->rotation;
    }

    
    Vector2 world_pos = get_world_position();
    
    Rectangle sprite_rect_temp = sprite_rect;
    Vector2 origin_offset_temp = origin_offset;
    sprite_rect_temp.x = pos_in_spritesheet.x;
    sprite_rect_temp.y = pos_in_spritesheet.y;
    if(flip_y){
        sprite_rect_temp.height = -sprite_rect.height;
        origin_offset_temp.y = -origin_offset.y;
    }
    if(flip_x){
        sprite_rect_temp.width = -sprite_rect.width;
        origin_offset_temp.x = -origin_offset.x;
    }

    
    Rectangle dest_rec = {world_pos.x, world_pos.y, sprite_size.x * scale.x, sprite_size.y * scale.y};
    
    Vector2 origin = {(static_cast<float>(sprite_size.x)/2+origin_offset_temp.x)*scale.x,
         (static_cast<float>(sprite_size.y)/2+origin_offset_temp.y)*scale.y};

    DrawTexturePro(*texture, sprite_rect_temp, dest_rec, origin, rotation, WHITE);
}

Vector2 Sprite::get_world_position() const {
    if (parent) return parent->get_world_position() + local_position;
    return global_position;
}

void Sprite::on_parent_added(){
    if (!parent) return;
    scale = {parent->scale.x * scale.x, parent->scale.y * scale.x};
    cout<<parent->name<<"\n";
    
}

std::shared_ptr<Sprite> Sprite::clone() const {
    auto new_sprite = make_shared<Sprite>(
        this->texture,             // shared texture
        pos_in_spritesheet,
        sprite_size,
        origin_offset
    );
    new_sprite->flip_x = flip_x;
    new_sprite->flip_y = flip_y;
    new_sprite->local_position = local_position;
    new_sprite->scale = scale;
    new_sprite->rotation = rotation;
    return new_sprite;
}
