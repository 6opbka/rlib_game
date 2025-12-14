#pragma once
#include "raylib.h"
#include <fstream>
#include <vector>

class MapGen 
{
private:
    /* data */
    bool inside(int x, int y);
public:
    MapGen();
    ~MapGen();

    void gen_map();
    Texture2D create_texture();
    void calculate_colliders();

    std::vector<int> map_vec;
    const int map_width = 64;
    const int map_height = 64;
    const int tile_size = 16;

    RenderTexture2D render_tex = {0};

};

struct Edge{
    Vector2 pos_1 = {0,0};
    Vector2 pos_2 = {0,0};
};


