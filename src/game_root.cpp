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
    grid.clear();
    if (!pending_children.empty()) {
        for (auto& child : pending_children) {
            children.push_back(child);
            num_of_objects ++;
    
        }
        pending_children.clear();
    }

    // Update existing children
    for (auto& child : children) {
        if (!child) return;
        child->update(delta_time);
        grid_add_object(child);
    }
    camera->update(delta_time);
    //Check collisions in grid now
    //better way to do this?
    check_collisions_in_grid();
    remove_marked_objects();

    //сделать рендер для коллайддера отдельно. 
    //и теперь прицеливается на точку на экране а не в мире
    
    
    
    

    cout<<"num of objects: "<<num_of_objects<<endl;
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
int _x = static_cast<int>(object->local_position.x / grid_cell_size);
int _y = static_cast<int>(object->local_position.y / grid_cell_size);
Vector2 cell_coord = { _x, _y };
grid[cell_coord].push_back(object);
}

void GameRoot::check_collisions_in_grid() {
    std::set<std::pair<GameObject*, GameObject*>> checked;
    int collision_checks = 0;
    for (const auto& [cell, objects] : grid) {
        for (shared_ptr<GameObject> obj : objects) {
            if (!(obj && obj->collider)) continue; // <-- исправлено
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

void GameRoot::add_cam(shared_ptr<Cam> cam){
    
    camera = cam;
    

}