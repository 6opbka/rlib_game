#include "src/game_root.h"
#include "src/bullet.h"
#include <set>

GameRoot::GameRoot(){
    scale = {4.0f,4.0f};
    name = "game_root";
    
}
GameRoot::~GameRoot(){}

void GameRoot::update(const float delta_time) {
    // Add pending children first
    update_mouse_pos();
    grid.clear();
    if (!pending_children.empty()) {
        for (auto& child : pending_children) {
            children.push_back(child);
            num_of_objects ++;
    
        }
        pending_children.clear();
    }

    for (auto& child : children) {
        if (!child) return;
        child->update(delta_time);
        grid_add_object(child);
    }
    camera->update(delta_time);
    check_collisions_in_grid();
    remove_marked_objects();

    // cout<<"num of objects: "<<num_of_objects<<endl;
}

void GameRoot::render(const float delta_time){\
    camera->cam_begin();
    for (auto& child : children) {
        if (child){
            
            child->render(delta_time);
            
        } 
    }
    camera->cam_end();
}

shared_ptr<GameObject> GameRoot::instantiate(shared_ptr<GameObject> object) {
    auto new_object = object->clone();
    new_object->parent = shared_from_this();  // Set parent relationship
    pending_children.push_back(new_object);
    return new_object;
}
shared_ptr<GameObject> GameRoot::clone() const{
    // return std::make_shared<GameRoot>(*this);
    return nullptr;
}

void GameRoot::grid_add_object(shared_ptr<GameObject> object) {
    if (!object) return;
int _x = static_cast<int>(object->local_position.x / spatial_collider_grid_size);
int _y = static_cast<int>(object->local_position.y / spatial_collider_grid_size);
Vector2 cell_coord = { (float)_x, (float)_y };
grid[cell_coord].push_back(object);
}

void GameRoot::check_collisions_in_grid() {
    std::set<std::pair<GameObject*, GameObject*>> checked;
    int collision_checks = 0;
    for (const auto& [cell, objects] : grid) {
        for (shared_ptr<GameObject> obj : objects) {
            if (!(obj && obj->collider)) continue;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    Vector2 neighbour = {static_cast<float>(static_cast<int>(cell.x)+dx),
                                         static_cast<float>(static_cast<int>(cell.y)+dy)};
                    if (grid.count(neighbour)) {
                        for (shared_ptr<GameObject> other : grid[neighbour]) {
                            if (obj != other && other && other->collider) {
                                auto pair = std::make_pair(std::min(obj.get(), other.get()), std::max(obj.get(), other.get()));
                                if (checked.count(pair) == 0) {
                                    obj->check_collision(other);
                                    checked.insert(pair);
                                    collision_checks++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // cout<< "number of objects: "<<num_of_objects;
    // cout << " Collision checks: " << collision_checks << std::endl;
}


void GameRoot::remove_marked_objects() {
    auto it = children.begin();
    while (it != children.end()) {
        if (!(*it) || (*it)->marked_for_deletion) {
            it = children.erase(it);
            num_of_objects--;
        } else {
            ++it;
        }
    }
}

void GameRoot::update_mouse_pos(){
    mouse_world_pos = GetScreenToWorld2D(GetMousePosition(),camera->camera);
    // cout<<mouse_world_pos<<endl;
}

void GameRoot::add_cam(shared_ptr<Cam> cam){
    camera = cam;

}

void GameRoot::init_map(){
    map.gen_map();
    Texture2D map_tex = map.create_texture();
    map.calculate_colliders();
    map_tex = map.redraw_colliders_as_tex();
    auto map_ref = make_shared<Texture2D>(map_tex);
    auto map_sprite = sprite_manager->make_sprite(map_ref,{0,0},{16.0f*64,16.0f*64});
    map.grid_add_colliders();
    auto map_ = make_shared<GameObject>();
    map_->name = "map";
    map_->sprite_manager = sprite_manager;
    map_->add_sprite(map_sprite);
    add_child(map_);
}