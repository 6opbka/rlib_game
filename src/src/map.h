#pragma once
#include "raylib.h"
#include <fstream>
#include <vector>

class Map
{
private:
    /* data */
public:
    Map();
    ~Map();

    void gen_map();
    Texture2D create_texture();
    void calculate_colliders();

    std::vector<int> map_vec;
    const int map_width = 64;
    const int map_height = 64;

    RenderTexture2D render_tex = {0};

};


