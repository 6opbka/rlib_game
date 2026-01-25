#pragma once
#include "raylib.h"
#include <fstream>
#include <vector>
#include <unordered_map>
#include "utility.hpp"
#include <memory>


class StaticCollider;

class LevelMap 
{
private:
    /* data */
    bool inside(int x, int y);
    void normalize_edge(Line& edge);
    void merge_collinear(std::vector<Line>& edges, std::vector<Line>& merged, bool horizontal);
    void grid_add_wall(StaticCollider* col);

public:
    LevelMap();
    ~LevelMap();

    void gen_map();
    Texture2D create_texture();
    Texture2D redraw_colliders_as_tex();
    
    void calculate_edges();
    void generate_collider_list();

    AABB aabb_from_line(const Line& line);
    int colliders_in_cells = 0;

    

    
    std::vector<std::unique_ptr<StaticCollider>> static_colliders;
    std::vector<std::vector<StaticCollider*>> static_grid;
    void grid_add_colliders();
    
    std::vector<int> map_vec;
    const int map_width = 64;
    const int map_height = 64;
    const int tile_size = 16;
    int pixel_width = 0;
    int pixel_height = 0;

    RenderTexture2D render_tex = {0};
    std::vector<std::vector<Line>> islands;
    std::vector<Line> all_edges_hor;
    std::vector<Line> all_edges_ver;

};