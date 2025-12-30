#pragma once
#include "raylib.h"
#include <memory>
#include <vector>
#include <string>
#include "utility.hpp"
#include "sprite_manager.h"
#include "sprite.h"
#include "animated_sprite.h"
#include "rect_collider.h"

class GameRoot;
class Sprite;
class DynamicCollider;
class SpriteManager;


using namespace std;

    class GameObject : public enable_shared_from_this<GameObject> {
    public:

        bool marked_for_deletion = false;
        
        shared_ptr<Sprite> sprite;
        shared_ptr<AnimatedSprite> animated_sprite;
   
        Vector2 scale{1.0f, 1.0f};

        Vector2 local_position{0.0f,0.0f};

        weak_ptr<GameRoot> root;
        std::shared_ptr<SpriteManager> sprite_manager;
        weak_ptr<GameObject> parent;
        vector<shared_ptr<GameObject>> children;

        unique_ptr<DynamicCollider> collider;

        Vector2 collider_size = {16.0f, 16.0f};

        string name = "null";
        float rotation = 0.0f;

        void render(const float delta_time);
        virtual void update(const float delta_time);

        void add_child(shared_ptr<GameObject> child);
        void remove_child(GameObject* child);
        virtual void added_parent_callback();

        void add_sprite (shared_ptr<Sprite> sprite);
        void add_animated_sprite(shared_ptr<AnimatedSprite> sprite_);
        void add_collider(unique_ptr<DynamicCollider> collider_);

        void draw_sprites();

        Vector2 get_world_position() const;

        virtual shared_ptr<GameObject> clone() const;

        void check_collision_dynamic(GameObject& target);
        void check_collision_static(const StaticCollider* target);



        GameObject();
        GameObject(const GameObject& other);
        virtual ~GameObject();
        virtual void on_collision();

    protected:
        shared_ptr<GameRoot> get_root();
        
    };
