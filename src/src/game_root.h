#pragma once
#include "gameobject.h"
#include "bullet.h"
#include "camera.h"
#include "level_map.h"




class GameRoot : public GameObject
{
public:
    GameRoot();
    ~GameRoot();

    LevelMap map;


    int num_of_objects = 0;

    shared_ptr<Cam> camera;

    Vector2 mouse_world_pos = {0.0f,0.0f};


    float pos = 0.0f;

    void update(const float delta_time);
    void render(const float delta_time);
    std::vector<std::shared_ptr<GameObject>> pending_children;
    //Partitioning grid for collision detection
    std::unordered_map<Vector2, std::vector<shared_ptr<GameObject>>, CellCoordHash> grid;
    
    void grid_add_object(shared_ptr<GameObject> object);
    void check_collisions_in_grid();

    void add_cam(shared_ptr<Cam> cam);
    
    void update_mouse_pos();

    void init_map();


    shared_ptr<GameObject> instantiate(shared_ptr<GameObject> object);
    shared_ptr<GameObject> clone() const override;
    void remove_marked_objects();
};
