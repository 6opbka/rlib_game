#pragma once

#include "raylib.h"
#include <memory>

class GameObject;

class Sprite
{
private:
public:
    std::shared_ptr<Texture2D> texture;
    std::shared_ptr<GameObject> parent;

    Sprite(std::shared_ptr<Texture2D> texture, 
        Vector2 pos, 
        Vector2 sprite_size,
        Vector2 origin_offset = {0,0}
    );
    virtual ~Sprite();
    
    bool flip_x = false, flip_y = false;
    Rectangle sprite_rect{0.0f,0.0f,0.0f,0.0f};
    Vector2 sprite_size{16.0f, 16.0f};
    Vector2 origin_offset{0.0f, 0.0f};

    Vector2 pos_in_spritesheet {0.0f,0.0f};

    Vector2 global_position{0.0f,0.0f};
    Vector2 local_position{0.0f,0.0f};

    Vector2 scale{1.0f, 1.0f};

    float rotation = 0.0f;


    bool is_animated = false; //I know its a bad design. Dont wanna try casting the object each frame.

    
    void draw();
    Vector2 get_world_position() const;
    void on_parent_added();

    //TODO: add on parent callback, fix the weapon, player, etc stuff with new sprite object. Add animated sprite stuff.

    
    std::shared_ptr<Sprite> clone() const;

    void load_texture(const char* path);
};