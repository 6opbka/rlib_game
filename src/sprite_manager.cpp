#include "src/sprite_manager.h"
#include <iostream>
#include "src/sprite.h"
#include "src/animated_sprite.h"


using namespace std;

SpriteManager::SpriteManager(){}
SpriteManager::~SpriteManager(){}

std::shared_ptr<Sprite> SpriteManager::make_sprite(const string& path, Vector2 pos, Vector2 sprite_size, Vector2 origin_offset){
    shared_ptr<Texture2D> tex = cache_tex(path);
    auto ret = std::make_shared<Sprite>(tex, pos, sprite_size, origin_offset);
    return ret;
}


std::shared_ptr<AnimatedSprite> SpriteManager::make_animated_sprite(
    const std::string& path,
    Vector2 pos,
    Vector2 sprite_size,
    Vector2 origin_offset
) 
{
    shared_ptr<Texture2D> tex = cache_tex(path);
    auto sprite = std::make_shared<AnimatedSprite>(tex, pos, sprite_size, origin_offset);
    sprite->is_animated = true;
    return sprite;
}

void SpriteManager::unload_all(){
    texture_cache.clear(); // shared_ptr сами освободят память, когда никто не использует текстуру
}

void SpriteManager::unload_sprite(const string& path){
    texture_cache.erase(path); // shared_ptr сами освободят память, когда никто не использует текстуру
}

shared_ptr<Texture2D> SpriteManager::cache_tex(const std::string& path){
    cout<<"loading sprite:"<<path<<"\n";
    shared_ptr<Texture2D> tex;
    auto it = texture_cache.find(path);
    if(it != texture_cache.end()){
        tex = it->second;
    }
    else{
        tex = std::make_shared<Texture2D>(LoadTexture(path.c_str()));
        texture_cache[path] = tex;
    }
    return tex;
}


