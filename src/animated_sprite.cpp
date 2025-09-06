#include "src/animated_sprite.h"
#include "raylib.h"
#include "src/sprite.h"
#include <iostream>

using namespace std;


AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture2D> texture,
        Vector2 pos, 
        Vector2 sprite_size,
        Vector2 origin_offset):
        Sprite(texture, pos, sprite_size,origin_offset)
{
    init();
}
AnimatedSprite::~AnimatedSprite(){}

void AnimatedSprite::update(float delta_time){
    play_animation(delta_time);
    
    if(looping && current_frame >= num_frames){
    // cout<<"calling again\n";

        play();
    }
    // cout<<current_frame<<"\n";
    if(current_state < num_states){
        pos_in_spritesheet.y = current_state*sprite_size.y;
    }

}

void AnimatedSprite::play_animation(float delta_time){
    if(!playing) return;
    current_time += delta_time;
    if(current_time>=frame_time){
        if (current_frame < num_frames){
            current_time = 0.0f;
            current_frame ++;
            pos_in_spritesheet.x=current_frame*sprite_size.x;
            // cout<<pos_in_spritesheet.x<<" "<<pos_in_spritesheet.y<<(texture!=nullptr)<<"\n";
        }
        else{
            playing = false;
        }
    }
    
}

void AnimatedSprite::play(){
    current_frame = 0;
    playing = true;
}

void AnimatedSprite::init(){
    num_frames = texture->width/sprite_size.x;
    num_states = texture->height/sprite_size.y;

    // TODO: There is a bug that sometimes textures do not load. 
    // Add states both to the sprite and the player. 
    // Add enemy.

    cout<<"sprite loaded: num_frames = "<< num_frames << " num states = "<< num_states << "\n";
}



