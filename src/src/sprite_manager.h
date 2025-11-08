#pragma once
#include "raylib.h"
#include <unordered_map>
#include <string>
#include <memory>



// All of the sprites should be created through this class's instance

class Sprite;
class AnimatedSprite;

class SpriteManager
{
private:
    std::unordered_map<std::string,std::shared_ptr<Texture2D>> texture_cache;

public:
    SpriteManager();
    ~SpriteManager();

    std::shared_ptr<Sprite> make_sprite(const std::string& path, Vector2 pos, Vector2 sprite_size,Vector2 origin_offset = {0.0f,0.0f});
    std::shared_ptr<Sprite> make_sprite(std::shared_ptr<Texture2D> texture, Vector2 pos, Vector2 sprite_size, Vector2 origin_offset = {0.0f,0.0f});
    
    std::shared_ptr<AnimatedSprite> make_animated_sprite(const std::string& path, Vector2 pos, Vector2 sprite_size,Vector2 origin_offset = {0.0f,0.0f});
    void unload_all();
    void unload_sprite(const std::string& path);

    std::shared_ptr<Texture2D> cache_tex(const std::string& path);
};
