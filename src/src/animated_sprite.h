#pragma once

#include "sprite.h"     // Must include BEFORE using `Sprite`
#include "raylib.h"
#include <memory>

class AnimatedSprite : public Sprite {
public:
    float fps = 12;
    float frame_time = 1.0f/fps;
    float current_time = 0.0f;

    int current_state = 0;
    int current_frame = 0;

    int num_frames = 0;
    int num_states = 0;
    
    bool looping = true;
    bool playing = true;
    bool is_animated =true;

    AnimatedSprite(std::shared_ptr<Texture2D> texture,
                   Vector2 pos,
                   Vector2 sprite_size,
                   Vector2 origin_offset = {0, 0});
    ~AnimatedSprite();

    void update(float delta_time);
    void init();
    void play();
    void play_animation(float delta_time);
};