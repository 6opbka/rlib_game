#include "src/level_map.h"
#include <iostream>
#include <stack>
#include <cstdlib>
#include <map>
#include <algorithm>

using namespace std;

char WALL = '#';
char AIR = '.';

int rand_pick(){
    int fill_prob = 45;
    if(rand()%100<fill_prob){
        return WALL;
    }
    else{
        return AIR;
    }
}



vector<Vector2> calculate_neighbours(int x, int y) {
    vector<Vector2> out;

    for (int y_offset = -1; y_offset <= 1; ++y_offset) {
        for (int x_offset = -1; x_offset <= 1; ++x_offset) {
            int n_x = x + x_offset;
            int n_y = y + y_offset;

            if (n_x >= 0 && n_x < 64 && n_y >= 0 && n_y < 64) {
                out.push_back({(float)n_x, (float)n_y});
            }
        }
    }

    return out;
}

void LevelMap::gen_map() {
    const int num_elements = map_width * map_height;
    int retries = 2;

    map_vec.resize(num_elements);
    std::vector<int> new_map(num_elements);

    // Инициализация случайным образом
    for (int i = 0; i < num_elements; i++) {
        map_vec[i] = rand_pick();  // например, WALL или AIR
    }

    // Один шаг клеточного автомата
    for(int i = 0; i<retries;i++){
        for (int y = 0; y < map_height; y++) {
            for (int x = 0; x < map_width; x++) {

                std::vector<Vector2> neighbours = calculate_neighbours(x,y);
                int wall_count = 0;

                for (Vector2 n : neighbours) {
                    int n_x = (int)n.x;
                    int n_y = (int)n.y;
                    if (n_x < 0 || n_y < 0 || n_x >= map_width || n_y >= map_height)
                        continue;
                    if (map_vec[n_y * map_width + n_x] == WALL)
                        wall_count++;
                }

                int index = y * map_width + x;
                int cell = map_vec[index];

                // Классические правила:
                if (cell == WALL) {
                    new_map[index] = (wall_count >= 5) ? WALL : AIR;
                } else {
                    new_map[index] = (wall_count >= 6) ? WALL : AIR;
            }
        }
    
    }

        for(int y = 0; y<map_height;y++){
            for(int x = 0; x<map_width;x++){
                if((x==0 || x==(map_width-1))||(y==0||y==(map_height-1))){
                    new_map[y * map_width + x] = WALL;
                }
            }
        }
        map_vec.swap(new_map);
    }


    // create_texture();

    // Вывод карты
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            std::cout << (map_vec[y * map_width + x] == WALL ? '#' : '.');
        }
        std::cout << "\n";
    }
    
}

void LevelMap::normalize_edge(Line& edge){
    if(edge.a.x>edge.b.x||edge.a.y>edge.b.y){
        swap(edge.a,edge.b);    
    }
}


Texture2D LevelMap::create_texture(){
    
    render_tex = LoadRenderTexture(map_width*tile_size,map_height*tile_size);
    BeginTextureMode(render_tex);
    ClearBackground(WHITE);
    for(int y = 0; y<map_height;y++){
        for(int x = 0;x<map_width;x++){
            int cell = map_vec[y*map_width+x];
            Vector2 pos = {(float)(x*tile_size),(float)(y*tile_size)};
            if (cell == WALL) {
            DrawRectangleV(pos, { (float)tile_size, (float)tile_size }, DARKGRAY);
        } else {
            DrawRectangleV(pos, { (float)tile_size, (float)tile_size }, LIGHTGRAY);
        }
        }
    }
    EndTextureMode();
    return render_tex.texture;
}

void LevelMap::calculate_colliders(){
    vector<bool> visited(map_height*map_width);

    // Calculating raw_islands 
    vector<vector<Vector2>> raw_islands;
    for(int y = 0; y<map_height; y++){
        for(int x = 0; x<map_width; x++){
            int index = y*map_width+x;
            if(map_vec[index]==WALL && !visited[index]){
                vector<Vector2> island;
                stack<Vector2> q;
                q.push(Vector2{(float)x,(float)y});
                visited[index] = true;

                while(!q.empty()){
                    Vector2 cur = q.top();
                    q.pop();
                    island.push_back(cur);
                    static const int x_offset[4] = {1,-1,0,0};
                    static const int y_offset[4] = {0,0,1,-1};
                    for(int i =0; i<4;i++){
                        int n_x = cur.x+x_offset[i];
                        int n_y = cur.y+y_offset[i];

                        if(!inside(n_x,n_y)) continue;
                        
                            int neighbour_idx = n_y*map_width+n_x;
                            if(!visited[neighbour_idx]&&map_vec[neighbour_idx]==WALL){
                                visited[neighbour_idx] = true;
                                q.push({(float)n_x,(float)n_y});
                            }
                    }
                }
                raw_islands.push_back(island);
            }
        }
    }
    // Calculating island colliders
    int i = 0;
    
    // vector<vector<Line>> islands(raw_islands.size());
    islands.resize(raw_islands.size());
    for(auto island : raw_islands){
        vector<Line> island_edges;
        for(Vector2& pos : island){
            // cout<<"("<<pos.x<<", "<<pos.y<<"), ";
            int x = pos.x;
            int y = pos.y;

            Vector2 tl = {x*tile_size, y*tile_size};
            Vector2 tr = {(x+1)*tile_size, y*tile_size};
            Vector2 bl = {x*tile_size, (y+1)*tile_size};
            Vector2 br = {(x+1)*tile_size, (y+1)*tile_size};
            // UP
            if (!inside(x, y-1) || map_vec[(y-1)*map_width + x] != WALL) {
                island_edges.push_back({ tl, tr });
            }

            // DOWN
            if (!inside(x, y+1) || map_vec[(y+1)*map_width + x] != WALL) {
                island_edges.push_back({ bl, br });
            }

            // LEFT
            if (!inside(x-1, y) || map_vec[y*map_width + (x-1)] != WALL) {
                island_edges.push_back({ tl, bl });
            }

            // RIGHT
            if (!inside(x+1, y) || map_vec[y*map_width + (x+1)] != WALL) {
                island_edges.push_back({ tr, br });
            }
        }
        islands[i] = island_edges;
        i++;

    }

    // Normalizing egdes
    for (auto& island : islands){
        for(Line& edge : island){
            normalize_edge(edge);
        }
    }

    // Merging lines 
    // merged_islands.resize(islands.size());

    for (const auto& island : islands){
        map<int,vector<Line>> hor_edges;
        map<int,vector<Line>> ver_edges;

        for(const Line& edge : island){
            if(edge.a.y==edge.b.y){
                hor_edges[edge.a.y].push_back(edge);
            }
            else if(edge.a.x==edge.b.x){
                ver_edges[edge.a.x].push_back(edge);
            }
        }

        

        for(auto& [y, edges] : hor_edges){
            merge_collinear(edges,all_edges_hor,true);
        }
        for(auto& [x,edges] : ver_edges){
            merge_collinear(edges,all_edges_ver,false);
        }


    }

    cout<<"num raw_islands: "<<raw_islands.size()<<endl;
    cout<<"num islands: "<<islands.size()<<endl;
    cout<<"num all_edges_hor: "<<all_edges_hor.size()<<endl;
    cout<<"num all_edges_ver: "<<all_edges_ver.size()<<endl;



}

Texture2D LevelMap::redraw_colliders_as_tex(){
    
    render_tex = LoadRenderTexture(map_width*tile_size,map_height*tile_size);
    BeginTextureMode(render_tex);
    ClearBackground(WHITE);
    for(int y = 0; y<map_height;y++){
        for(int x = 0;x<map_width;x++){
            int cell = map_vec[y*map_width+x];
            Vector2 pos = {(float)(x*tile_size),(float)(y*tile_size)};
            if (cell == WALL) {
            DrawRectangleV(pos, { (float)tile_size, (float)tile_size }, DARKGRAY);
        } else {
            DrawRectangleV(pos, { (float)tile_size, (float)tile_size }, LIGHTGRAY);
        }
        }
    }
    int i = 1;
    for (const auto& edge : all_edges_ver) {
    

    unsigned char r = (unsigned char)((i * 50) % 256);
    unsigned char g = (unsigned char)((i * 80) % 256);
    unsigned char b = (unsigned char)((i * 110) % 256);

    Color c = { r, g, b, 255 };

    DrawLineV(edge.a, edge.b, c);
    i++;
    
    }
    for (const auto& edge : all_edges_hor) {
    

    unsigned char r = (unsigned char)((i * 50) % 256);
    unsigned char g = (unsigned char)((i * 80) % 256);
    unsigned char b = (unsigned char)((i * 110) % 256);

    Color c = { r, g, b, 255 };

    DrawLineV(edge.a, edge.b, c);
    i++;
    
    }
    EndTextureMode();
    return render_tex.texture;
}

bool LevelMap::inside(int x, int y){
    return (x>=0&&x<map_width&&
            y>=0&&y<map_height);
}

void LevelMap::merge_collinear(vector<Line>& edges, vector<Line>& merged, bool horizontal){
    if (edges.empty()) return;

    // 1. Сортировка
    std::sort(edges.begin(), edges.end(),
        [horizontal](const Line& e1, const Line& e2) {
            return horizontal
                ? e1.a.x < e2.a.x
                : e1.a.y < e2.a.y;
        });

    Line current = edges[0];

    // 2. Линейный проход
    for (int i = 1; i < edges.size(); i++) {
        float curr_end = horizontal ? current.b.x : current.b.y;
        float next_start = horizontal ? edges[i].a.x : edges[i].a.y;

        if (next_start <= curr_end) {
            // расширяем текущий
            if (horizontal)
                current.b.x = std::max(current.b.x, edges[i].b.x);
            else
                current.b.y = std::max(current.b.y, edges[i].b.y);
        } else {
            merged.push_back(current);
            current = edges[i];
        }
    }

    merged.push_back(current);
}

void LevelMap::grid_add_colliders(){
    for(const Line& edge: all_edges_hor){
        grid_add_hor_wall(edge);
    }
    for(const Line& edge: all_edges_ver){
        grid_add_vert_wall(edge);
    }
    

    cout<<"Cell num: "<<static_grid.size()<<endl;

}

void LevelMap::grid_add_vert_wall(const Line& edge){
    int x = edge.a.x;
    int y0 = edge.a.y;
    int y1 = edge.b.y;

    if(y1<y0) std::swap(y0,y1);

    int cell_x = floor(x/ spatial_collider_grid_size);
    int y_start = floor(y0/ spatial_collider_grid_size);
    int y_end   = floor(y1 / spatial_collider_grid_size);

    for (int y = y_start; y <= y_end; ++y) {
        static_grid[{cell_x, y}].push_back(edge);
    }

}



void LevelMap::grid_add_hor_wall(const Line& edge){
    
    int y = edge.a.y;
    int x0 = edge.a.x;
    int x1 = edge.b.x;
    
    if(x1<x0) std::swap(x0,x1);

    int cell_y = floor(y/ spatial_collider_grid_size);
    int x_start = floor(x0/ spatial_collider_grid_size);
    int x_end   = floor(x1 / spatial_collider_grid_size);

    for (int y = x_start; y <= x_end; ++y) {
        static_grid[{cell_y, y}].push_back(edge);
    }

}


LevelMap::LevelMap(){
    pixel_width = map_width*spatial_collider_grid_size;
    pixel_height = map_height*spatial_collider_grid_size;

}
LevelMap::~LevelMap(){

}