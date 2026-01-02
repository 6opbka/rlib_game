#include "src/game_root.h"
#include "src/bullet.h"
#include <set>
#include <algorithm>

GameRoot::GameRoot(){
    
    name = "game_root";
    dynamic_grid.resize(map.map_height*map.map_width);
    cout<<"dynamic_grid size: "<<dynamic_grid.size()<<"\n";
}
GameRoot::~GameRoot(){}

inline Cell GameRoot::world_to_grid(Vector2 world) {
    const float half_w =
        map.map_width * spatial_collider_grid_size * 0.5f;
    const float half_h =
        map.map_height * spatial_collider_grid_size * 0.5f;

    Cell cell;
    cell.x = static_cast<int>((world.x + half_w)
                              / spatial_collider_grid_size);
    cell.y = static_cast<int>((world.y + half_h)
                              / spatial_collider_grid_size);

    return cell;
}

inline Vector2 GameRoot::grid_to_world(Cell grid) {
    const float half_w =
        map.map_width * spatial_collider_grid_size * 0.5f;
    const float half_h =
        map.map_height * spatial_collider_grid_size * 0.5f;

    return {
        grid.x * spatial_collider_grid_size
            - half_w
            + spatial_collider_grid_size * 0.5f,

        grid.y * spatial_collider_grid_size
            - half_h
            + spatial_collider_grid_size * 0.5f
    };
}

void GameRoot::update(const float delta_time) {
    // Add pending children first
    update_mouse_pos();
    for(auto& cell: dynamic_grid){
        cell.clear();
    }
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
        add_dynamic_object(child);
    }
    camera->update(delta_time);
    check_collisions_in_grid();
    remove_marked_objects();

    // cout<<"num of objects: "<<num_of_objects<<endl;
}

void GameRoot::render(const float delta_time) {
    camera->cam_begin();

    for (auto& child : children) {
        if (child) {
            child->render(delta_time);
        }
    }

    const float half_w =
        map.map_width * spatial_collider_grid_size * 0.5f;
    const float half_h =
        map.map_height * spatial_collider_grid_size * 0.5f;

    for (int index : debug_cells) {
        int x = index % map.map_width;
        int y = index / map.map_width;

        float wx =
            x * spatial_collider_grid_size
            - half_w
            + spatial_collider_grid_size * 0.5f;

        float wy =
            y * spatial_collider_grid_size
            - half_h
            + spatial_collider_grid_size * 0.5f;

        DrawRectangleLines(
            wx - spatial_collider_grid_size * 0.5f,
            wy - spatial_collider_grid_size * 0.5f,
            spatial_collider_grid_size,
            spatial_collider_grid_size,
            RED
        );
    }

    camera->cam_end();
}

shared_ptr<GameObject> GameRoot::instantiate(shared_ptr<GameObject> object) {
    if (!object) return nullptr;

    object->parent = shared_from_this();
    pending_children.push_back(object);
    return object;
}

shared_ptr<GameObject> GameRoot::clone() const{
    // return std::make_shared<GameRoot>(*this);
    return nullptr;
}

void GameRoot::add_dynamic_object(std::shared_ptr<GameObject> object) {
    if (!object || !object->collider) return;

    Rectangle aabb = object->collider->get_aabb_world();

    Cell minC = world_to_grid({ aabb.x, aabb.y });
    Cell maxC = world_to_grid({
        aabb.x + aabb.width  - 0.001f,
        aabb.y + aabb.height - 0.001f
    });

    for (int y = minC.y; y <= maxC.y; ++y) {
        for (int x = minC.x; x <= maxC.x; ++x) {

            if (x < 0 || y < 0 ||
                x >= map.map_width ||
                y >= map.map_height)
                continue;

            dynamic_grid[y * map.map_width + x]
                .push_back(object.get());
        }
    }
}




void GameRoot::check_collisions_in_grid() {
    int collision_checks = 0;
    debug_cells.clear();
    // Только "вперёд"
    static const int offsets[][2] = {
        {0, 0},
        {1, 0},
        {0, 1},
        {1, 1},
        {-1, 1}
    };

    for (int index = 0; index < dynamic_grid.size(); ++index) {
        auto& objects = dynamic_grid[index];
        if (objects.empty()) continue;
        debug_cells.push_back(index);

        int x = index % map.map_width;
        int y = index / map.map_width;
        

        for (int i = 0; i < objects.size(); ++i) {
            GameObject* obj = objects[i];
            if (!obj || !obj->collider) continue;

            for (auto& o : offsets) {
                int nx = x + o[0];
                int ny = y + o[1];

                if (nx < 0 || ny < 0 ||
                    nx >= map.map_width ||
                    ny >= map.map_height)
                    continue;
                

                auto& neighbour_cell =
                    dynamic_grid[ny * map.map_width + nx];

                if (neighbour_cell.empty()) continue;

                // Та же ячейка — только i+1
                if (o[0] == 0 && o[1] == 0) {
                    for (int j = i + 1; j < neighbour_cell.size(); ++j) {
                        GameObject* other = neighbour_cell[j];
                        if (!other || !other->collider) continue;

                        obj->check_collision_dynamic(*other);
                        collision_checks++;
                    }
                }
                // Соседние ячейки — все объекты
                else {
                    for (GameObject* other : neighbour_cell) {
                        if (!other || !other->collider) continue;

                        obj->check_collision_dynamic(*other);
                        collision_checks++;
                    }
                }
            }
        }
    }
    
    // std::cout << "collision checks: " << collision_checks << std::endl;
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
    map.calculate_edges();
    map_tex = map.redraw_colliders_as_tex();
    auto map_ref = make_shared<Texture2D>(map_tex);
    auto map_sprite = sprite_manager->make_sprite(map_ref,{0,0},{16.0f*64,16.0f*64});
    map.generate_collider_list();
    map.grid_add_colliders();
    auto map_ = make_shared<GameObject>();
    // map_->scale = {2.0f,2.0f};
    map_->name = "map";
    map_->sprite_manager = sprite_manager;
    map_->add_sprite(map_sprite);
    add_child(map_);
}