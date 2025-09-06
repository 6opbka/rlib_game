#include <raylib-cpp.hpp>
#include "src/gameobject.h"
#include "src/player.h"
#include "src/weapon.h"
#include "src/bullet.h"
#include "src/game_root.h"
#include "src/sprite_manager.h"
#include "src/rect_collider.h"



int main() {
    int screen_width = 800;
    int screen_height = 800;
    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screen_width, screen_height, "Raylib C++ Starter Kit Example");




    auto game_root = make_shared<GameRoot>();
    auto sprite_manager = make_shared<SpriteManager>();

    cout<<"here1\n";

    
    auto weapon_sprite = sprite_manager->make_sprite("resources/Laser.png",{0,0},{32.0f,32.0f},{-25.0f,0.0f});
    auto player_sprite = sprite_manager->make_sprite("resources/knight_spritesheet.png",{0,0},{16.0f,16.0f});
    auto bullet_sprite = sprite_manager->make_sprite("resources/bullet_1.png",{0,0},{17.0f,9.0f});
    auto player_sprite2 = sprite_manager->make_animated_sprite("resources/knight_spritesheet.png",{0,0},{16.0f,16.0f});
    auto wall_sprite = sprite_manager->make_sprite("resources/wall.png",{0,0},{16.0f,16.0f});

    // auto player_collider = make_unique<Collider>(Vector2{32.0f,32.0f});
    // auto bullet_collider = make_unique<Collider>(Vector2{24.0f,16.0f});
    // auto wall_collider = make_unique<Collider>(Vector2{32.0f,32.0f});

    auto player_collider = make_unique<RectCollider>(Vector2{32.0f,32.0f},LAYER_PLAYER,LAYER_ENEMY_BULLET);
    auto bullet_collider = make_unique<RectCollider>(Vector2{24.0f,16.0f},LAYER_BULLET,LAYER_WALL | LAYER_ENEMY);
    auto wall_collider = make_unique<RectCollider>(Vector2{32.0f,32.0f},LAYER_WALL,LAYER_NONE);
    
    

    cout<<player_sprite2<<"\n";

    auto bullet = make_shared<Bullet>();
    bullet->sprite_manager = sprite_manager;
    bullet->add_sprite(bullet_sprite);
    bullet->add_collider(move(bullet_collider));

    auto wall = make_shared<GameObject>();
    wall->sprite_manager = sprite_manager;
    wall->add_sprite(wall_sprite);
    wall->add_collider(move(wall_collider));
    wall->local_position = {400.0f,200.0f};
    wall->name = "wall";
  

    auto player = make_shared<Player>();
    player->name = "player";
    player->sprite_manager = sprite_manager;
    player->add_animated_sprite(player_sprite2);
    player->add_collider(move(player_collider));

    auto weapon = make_shared<Weapon>();
    weapon->name = "weapon";
    weapon->sprite_manager = sprite_manager;
    weapon->add_sprite(weapon_sprite);

    // Set up relationships
    weapon->initialize(game_root, bullet);
    game_root->add_child(player);
    game_root->add_child(wall);
    game_root->root = game_root;
    player->add_child(weapon);


    SetTargetFPS(144);


    while (!w.ShouldClose()) {
        float delta_time = GetFrameTime();


        BeginDrawing();
        ClearBackground(RAYWHITE);
        int fps = GetFPS();
        
        textColor.DrawText(to_string(fps), 190, 200, 20);
        
        game_root->render(delta_time);
        game_root->update(delta_time);
        // game_root->instantiate(bullet);
        
        
        EndDrawing();
    }

    return 0;
}